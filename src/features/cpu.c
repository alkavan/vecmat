// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

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
static int runtime_cached;

#if defined(VECMAT_ARCH_X86)
#if defined(_MSC_VER)
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

static unsigned vm_xgetbv(unsigned idx)
{
    return (unsigned)_xgetbv(idx);
}
#else
static int vm_cpuid(const unsigned leaf, const unsigned sub,
                    unsigned *eax, unsigned *ebx, unsigned *ecx, unsigned *edx)
{
    const unsigned max = __get_cpuid_max(leaf & 0x80000000u, 0);
    if (max < leaf)
        return 0;
    return (int)__get_cpuid_count(leaf, sub, eax, ebx, ecx, edx);
}

static unsigned vm_xgetbv(unsigned idx)
{
    unsigned eax, edx;
    __asm__ volatile("xgetbv" : "=a"(eax), "=d"(edx) : "c"(idx));
    return eax;
}
#endif

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

static int vm_cpu_probe_avx2(void)
{
    unsigned eax = 0, ebx = 0, ecx = 0, edx = 0;
    if (!vm_cpu_probe_avx())
        return 0;
    if (!vm_cpuid(7, 0, &eax, &ebx, &ecx, &edx))
        return 0;
    return (ebx & (1u << 5)) != 0; /* AVX2 */
}

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

static int vm_cpu_probe_sve(void)
{
#if defined(VECMAT_ARCH_AARCH64) && defined(__linux__)
    return (getauxval(AT_HWCAP) & HWCAP_SVE) != 0;
#else
    return 0;
#endif
}

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

vm_cpu_features_t vm_cpu_runtime_features(void)
{
    if (runtime_cached)
        return runtime_cache;

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

    runtime_cache = f;
    runtime_cached = 1;
    return f;
}

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
void vm_cpu_init(void)
{
    (void)vm_cpu_runtime_features();
}
#endif
