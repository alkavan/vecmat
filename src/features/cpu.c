// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

#if !defined(__STDC_NO_ATOMICS__)
#include <stdatomic.h>
#endif

#if defined(__x86_64__) || defined(_M_X64) || defined(__amd64__) || \
    defined(__i386__) || defined(_M_IX86)
#define VECMAT_ARCH_X86 1
#endif

#if defined(__aarch64__) || defined(_M_ARM64)
#define VECMAT_ARCH_AARCH64 1
#endif

#if defined(VECMAT_ARCH_X86)
#if defined(_MSC_VER)
#include <intrin.h>
#else
#include <cpuid.h>
#endif
#endif

#if defined(VECMAT_ARCH_AARCH64) && defined(__linux__)
#include <sys/auxv.h>
#ifndef HWCAP_SVE
#define HWCAP_SVE (1u << 22)
#endif
#ifndef HWCAP2_SVE2
#define HWCAP2_SVE2 (1u << 1)
#endif
#endif

static vm_cpu_features_t runtime_cache;
#if !defined(__STDC_NO_ATOMICS__)
static atomic_int runtime_cached;
static atomic_flag runtime_lock = ATOMIC_FLAG_INIT;
#else
static volatile int runtime_cached;
#endif

#if defined(VECMAT_ARCH_X86)
#if defined(_MSC_VER)
/**
 * @brief CPUID wrapper (MSVC).
 *
 * @param leaf CPUID leaf.
 * @param sub CPUID subleaf.
 * @param eax EAX result.
 * @param ebx EBX result.
 * @param ecx ECX result.
 * @param edx EDX result.
 * @return 1 on success.
 */
static int vm_cpuid(unsigned leaf, unsigned sub,
                    unsigned *eax, unsigned *ebx, unsigned *ecx, unsigned *edx)
{
    int regs[4];
    __cpuidex(regs, (int)leaf, (int)sub);
    *eax = (unsigned)regs[0];
    *ebx = (unsigned)regs[1];
    *ecx = (unsigned)regs[2];
    *edx = (unsigned)regs[3];
    return 1;
}

/**
 * @brief Reads an XCR register (MSVC).
 *
 * @param idx XCR index (`0` for XCR0).
 * @return Low 32 bits of the XCR value.
 */
static unsigned vm_xgetbv(unsigned idx)
{
    return (unsigned)_xgetbv(idx);
}
#else
/**
 * @brief CPUID wrapper (GCC/Clang).
 *
 * @param leaf CPUID leaf.
 * @param sub CPUID subleaf.
 * @param eax EAX result.
 * @param ebx EBX result.
 * @param ecx ECX result.
 * @param edx EDX result.
 * @return Non-zero on success.
 */
static int vm_cpuid(const unsigned leaf, const unsigned sub,
                    unsigned *eax, unsigned *ebx, unsigned *ecx, unsigned *edx)
{
    const unsigned max = __get_cpuid_max(leaf & 0x80000000u, 0);
    if (max < leaf)
        return 0;
    return (int)__get_cpuid_count(leaf, sub, eax, ebx, ecx, edx);
}

/**
 * @brief Reads an XCR register (GCC/Clang).
 *
 * @param idx XCR index (`0` for XCR0).
 * @return Low 32 bits of the XCR value.
 */
static unsigned vm_xgetbv(unsigned idx)
{
    unsigned eax, edx;
    __asm__ volatile("xgetbv" : "=a"(eax), "=d"(edx) : "c"(idx));
    return eax;
}
#endif

/**
 * @brief Probes AVX plus OSXSAVE / YMM state.
 *
 * @return Non-zero if AVX is usable.
 */
static int vm_cpu_probe_avx(void)
{
    unsigned eax = 0, ebx = 0, ecx = 0, edx = 0;
    if (!vm_cpuid(1, 0, &eax, &ebx, &ecx, &edx))
        return 0;
    /* OSXSAVE (27) and AVX (28) */
    if ((ecx & (1u << 27)) == 0 || (ecx & (1u << 28)) == 0)
        return 0;
    return (vm_xgetbv(0) & 0x6u) == 0x6u; /* XMM+YMM state */
}

/**
 * @brief Probes AVX2 (requires usable AVX).
 *
 * @return Non-zero if AVX2 is usable.
 */
static int vm_cpu_probe_avx2(void)
{
    unsigned eax = 0, ebx = 0, ecx = 0, edx = 0;
    if (!vm_cpu_probe_avx())
        return 0;
    if (!vm_cpuid(7, 0, &eax, &ebx, &ecx, &edx))
        return 0;
    return (ebx & (1u << 5)) != 0; /* AVX2 */
}

/**
 * @brief Probes AVX-512F plus required XCR0 state.
 *
 * @return Non-zero if AVX-512F is usable.
 */
static int vm_cpu_probe_avx512f(void)
{
    unsigned eax = 0, ebx = 0, ecx = 0, edx = 0;
    if (!vm_cpuid(1, 0, &eax, &ebx, &ecx, &edx))
        return 0;
    if ((ecx & (1u << 27)) == 0)
        return 0;
    /* XMM+YMM (bits 1-2) and opmask+ZMM_Hi256+Hi16_ZMM (bits 5-7) */
    if ((vm_xgetbv(0) & 0xe6u) != 0xe6u)
        return 0;
    if (!vm_cpuid(7, 0, &eax, &ebx, &ecx, &edx))
        return 0;
    return (ebx & (1u << 16)) != 0; /* AVX512F */
}
#endif /* VECMAT_ARCH_X86 */

/**
 * @brief Probes SVE via Linux AT_HWCAP.
 *
 * @return Non-zero if SVE is usable.
 */
static int vm_cpu_probe_sve(void)
{
#if defined(VECMAT_ARCH_AARCH64) && defined(__linux__)
    if ((getauxval(AT_HWCAP) & HWCAP_SVE) == 0)
        return 0;
    /*
     * Kernels load a whole vec4/mat4-column in one predicate
     * (`svwhilelt(..., 4)`). That needs VL >= 16 B for F32 and
     * VL >= 32 B for F64. 128-bit SVE (common on Neoverse N2)
     * must not be selected for the F64 build.
     */
    {
        unsigned long vl_bytes;
        __asm__ volatile(".arch_extension sve\n\trdvl %0, #8"
                         : "=r"(vl_bytes));
#if defined(VECMAT_USE_F64)
        if (vl_bytes < 32ul)
            return 0;
#else
        if (vl_bytes < 16ul)
            return 0;
#endif
    }
    return 1;
#else
    return 0;
#endif
}

/**
 * @brief Probes SVE2 via Linux AT_HWCAP2.
 *
 * @return Non-zero if SVE2 is usable.
 */
static int vm_cpu_probe_sve2(void)
{
#if defined(VECMAT_ARCH_AARCH64) && defined(__linux__)
    if (!vm_cpu_probe_sve())
        return 0;
    return (getauxval(AT_HWCAP2) & HWCAP2_SVE2) != 0;
#else
    return 0;
#endif
}

/**
 * @brief ISA bits compiled into this binary.
 *
 * @return Feature mask of enabled backends.
 */
vm_cpu_features_t vm_cpu_compiled_features(void)
{
    vm_cpu_features_t f = VM_CPU_SCALAR;
#if defined(VECMAT_ENABLE_AVX) || defined(__AVX__)
    f |= VM_CPU_AVX;
#endif
#if defined(VECMAT_ENABLE_AVX2) || defined(__AVX2__)
    f |= VM_CPU_AVX2;
#endif
#if defined(VECMAT_ENABLE_AVX512) || defined(__AVX512F__)
    f |= VM_CPU_AVX512;
#endif
#if defined(VECMAT_ENABLE_SVE) || defined(__ARM_FEATURE_SVE)
    f |= VM_CPU_SVE;
#endif
#if defined(VECMAT_ENABLE_SVE2) || defined(__ARM_FEATURE_SVE2)
    f |= VM_CPU_SVE2;
#endif
    return f;
}

/**
 * @brief ISA bits detected on this CPU (cached).
 *
 * @return Feature mask of usable backends.
 */
vm_cpu_features_t vm_cpu_runtime_features(void)
{
#if !defined(__STDC_NO_ATOMICS__)
    if (atomic_load_explicit(&runtime_cached, memory_order_acquire))
        return runtime_cache;
#else
    if (runtime_cached)
        return runtime_cache;
#endif

    vm_cpu_features_t f = VM_CPU_SCALAR;
#if defined(VECMAT_ARCH_X86)
    if (vm_cpu_probe_avx())
        f |= VM_CPU_AVX;
    if (vm_cpu_probe_avx2())
        f |= VM_CPU_AVX2;
    if (vm_cpu_probe_avx512f())
        f |= VM_CPU_AVX512;
#endif
    if (vm_cpu_probe_sve())
        f |= VM_CPU_SVE;
    if (vm_cpu_probe_sve2())
        f |= VM_CPU_SVE2;

#if !defined(__STDC_NO_ATOMICS__)
    while (atomic_flag_test_and_set_explicit(&runtime_lock, memory_order_acquire)) {
        /* spin */
    }
    if (!atomic_load_explicit(&runtime_cached, memory_order_relaxed)) {
        runtime_cache = f;
        atomic_store_explicit(&runtime_cached, 1, memory_order_release);
    }
    atomic_flag_clear_explicit(&runtime_lock, memory_order_release);
    return runtime_cache;
#else
    runtime_cache = f;
    runtime_cached = 1;
    return f;
#endif
}

/**
 * @brief Highest-priority ISA that is both compiled and present.
 *
 * @return Single selected feature bit (or scalar).
 */
vm_cpu_features_t vm_cpu_selected_features(void)
{
    const vm_cpu_features_t have =
        vm_cpu_compiled_features() & vm_cpu_runtime_features();

    if (have & VM_CPU_SVE2)
        return VM_CPU_SVE2;
    if (have & VM_CPU_SVE)
        return VM_CPU_SVE;
    if (have & VM_CPU_AVX512)
        return VM_CPU_AVX512;
    if (have & VM_CPU_AVX2)
        return VM_CPU_AVX2;
    if (have & VM_CPU_AVX)
        return VM_CPU_AVX;
    return VM_CPU_SCALAR;
}

/**
 * @brief Short name of the highest bit set in `features`.
 *
 * @param features Feature mask.
 * @return Stable string such as `avx2` or `scalar`.
 */
const char *vm_cpu_name(const vm_cpu_features_t features)
{
    if (features & VM_CPU_SVE2)
        return "sve2";
    if (features & VM_CPU_SVE)
        return "sve";
    if (features & VM_CPU_AVX512)
        return "avx512";
    if (features & VM_CPU_AVX2)
        return "avx2";
    if (features & VM_CPU_AVX)
        return "avx";
    if (features & VM_CPU_SCALAR)
        return "scalar";
    return "none";
}

#if !defined(VECMAT_RUNTIME_DISPATCH)
/**
 * @brief No-op dispatch init; still warms the runtime feature cache.
 */
void vm_cpu_init(void)
{
    (void)vm_cpu_runtime_features();
}
#endif
