// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_CPU_H
#define VECMAT_CPU_H

/**
 * @file
 * @brief Compiled / runtime / selected ISA bits and `vm_cpu_*` probes.
 * @ingroup vecmat_fastpath
 */

#include "vecmat/config.h"

/**
 * @brief Bit mask of compiled or detected CPU ISA features.
 */
typedef uint32_t vm_cpu_features_t;

/*
 * Bit layout, by family. Enum constants must fit in `int`, so bits 0..30
 * only; bit 31 stays unused. Bits are probed facts; only the selection
 * ladder sets priority. Commented entries are reserved placeholders.
 *
 *   0..3   generic
 *   4..15  x86
 *   16..23 ARM
 *   24..30 RISC-V
 */
enum {
    /* Generic, bits 0..3. */
    VM_CPU_SCALAR  = 1u << 0,
    /* Registered external backend with no ISA bit of its own. */
    VM_CPU_BACKEND = 1u << 1,
    /* Bits 2,3 reserved. */

    /* x86, bits 4..15. */
    VM_CPU_AVX     = 1u << 4,
    VM_CPU_AVX2    = 1u << 5,
    VM_CPU_AVX512F = 1u << 6,
    /**
     * x86 FMA3 (`vfmadd*`) with usable YMM state. Not a tier of its own:
     * the AVX2 and AVX-512F tiers are built with `-mfma`. AVX2 is selected
     * only when this bit is also present; AVX-512F implies it. Never
     * returned by `vm_cpu_selected_features()`.
     */
    VM_CPU_FMA     = 1u << 7,
    // VM_CPU_F16C     = 1u << 8,
    // VM_CPU_AVX512BW = 1u << 9,
    // VM_CPU_AVX512VL = 1u << 10,
    // VM_CPU_AVX10_2  = 1u << 11, /* AVX10.2 additions. AVX10.1 is the AVX-512
    //                                set, so the probe folds AVX10 >= 1 into
    //                                VM_CPU_AVX512F; only 10.2 needs a bit. */
    // VM_CPU_AMX      = 1u << 12, /* tile + BF16 + INT8 for now. A feature,
    //                                not a tier; Linux also needs the
    //                                arch_prctl(ARCH_REQ_XCOMP_PERM) opt-in. */
    /* Bits 13..15 reserved. */

    /* ARM, bits 16..23. */
    VM_CPU_NEON    = 1u << 16,
    VM_CPU_SVE     = 1u << 17,
    VM_CPU_SVE2    = 1u << 18,
    // VM_CPU_SME      = 1u << 19,
    // VM_CPU_SME2     = 1u << 20, /* streaming mode has its own vector length,
    //                                so SME gets its own tier, apart from SVE. */
    /**
     * AArch64 IEEE binary16 arithmetic (HWCAP_FPHP). A feature, not a tier.
     * The scalar FP16 path does not consult it; a vendor module may.
     */
    VM_CPU_ARM_FP16 = 1u << 21,
    /**
     * AArch64 bfloat16 (HWCAP2_BF16). A feature, not a tier. The scalar
     * BF16 path does not consult it; a vendor module may.
     */
    VM_CPU_ARM_BF16 = 1u << 22,
    /**
     * AArch64 integer 8-bit matrix multiply (HWCAP2_I8MM). A feature, not
     * a tier. No consumer yet.
     */
    VM_CPU_I8MM     = 1u << 23,

    /* RISC-V, bits 24..30. */
    // VM_CPU_RVV      = 1u << 24, /* RISC-V V 1.0; tier via VM_LEVEL_RVV */
    // VM_CPU_ZVFH     = 1u << 25,
    // VM_CPU_ZVBB     = 1u << 26,
    /* Bits 27..30 reserved. */
    /* Bit 31 unused */
    /* int is a 32-bit signed type, so the largest value is INT_MAX = 0x7FFFFFFF. Bit 31 is the sign bit,
       and 1u << 31 equals 0x80000000, or 2147483648. That's one more than INT_MAX, so VM_CPU_X = 1u << 31 inside an
       enum is a constraint violation. GCC and Clang accept it as an extension, but -pedantic warns about it, and strict
       C11 compilers can reject it. */
};

/**
 * @name Family masks
 * Every defined bit lies in exactly one family.
 * @{ */
#define VM_CPU_GENERIC_MASK 0x0000000Fu
#define VM_CPU_X86_MASK     0x0000FFF0u
#define VM_CPU_ARM_MASK     0x00FF0000u
#define VM_CPU_RISCV_MASK   0x7F000000u
/** @} */

/**
 * @name Dispatch levels
 * Feature bits a kernel file was compiled to need. Bits are probed facts;
 * a level is the set that must all be present before its code may run.
 * Files built with `-mfma` use a level that includes `VM_CPU_FMA`, except
 * AVX-512, where FMA is implied.
 * @{
 */
#define VM_LEVEL_AVX    (VM_CPU_AVX)
#define VM_LEVEL_AVX2   (VM_CPU_AVX2 | VM_CPU_FMA)
/* FMA is implied: every AVX-512F CPU also reports FMA3. A hypervisor
   that masks FMA while exposing AVX-512F is unsupported. */
#define VM_LEVEL_AVX512 (VM_CPU_AVX512F)
#define VM_LEVEL_NEON   (VM_CPU_NEON)
#define VM_LEVEL_SVE    (VM_CPU_SVE)
#define VM_LEVEL_SVE2   (VM_CPU_SVE2)
/** @} */

/**
 * @brief ISA bits compiled into this library (`VECMAT_ENABLE_*`).
 */
VEC_API vm_cpu_features_t vm_cpu_compiled_features(void);

/**
 * @brief ISA bits detected on this CPU (runtime probe).
 */
VEC_API vm_cpu_features_t vm_cpu_runtime_features(void);

/**
 * @brief ISA the picker bound: registered backend, else best compiled-and-runtime.
 *
 * `VECMAT_FORCE_ISA=scalar|avx|avx2|avx512f|neon|sve|sve2` overrides the
 * compiled/runtime pick when that ISA is compiled in. A registered backend
 * still wins. The override is read once, on the first probe.
 */
VEC_API vm_cpu_features_t vm_cpu_selected_features(void);

/**
 * @brief Non-zero when this CPU has every bit in `need` (a bit or a level).
 *
 * Reads the raw `vm_cpu_runtime_features()` mask, so `VECMAT_FORCE_ISA`
 * and registered backends do not change it.
 */
static inline int vm_cpu_has(const vm_cpu_features_t need)
{
    return (vm_cpu_runtime_features() & need) == need;
}

/**
 * @brief Short name for a feature mask (`avx2`, `neon`, `scalar`, etc.).
 */
VEC_API const char *vm_cpu_name(vm_cpu_features_t features);

/**
 * @brief Probe once and bind dispatched `_ptr` names.
 *
 * Thread-safe and idempotent (C11 atomics, double-checked locking).
 * Call before reading `vm_cpu_selected_features()` if you care about
 * the first-use race; kernels themselves call this on first use.
 */
VEC_API void vm_cpu_init(void);

/**
 * @brief Optional one-line note set by a registered module.
 *
 * NULL until a module calls `vm_cpu_set_note()`. The pointer is not copied.
 */
VEC_API const char *vm_cpu_note(void);

/**
 * @brief Attach a module note for banners. Pass NULL to clear.
 */
VEC_API void vm_cpu_set_note(const char *note);

/**
 * @brief Float widths compiled into this library (`32`, `64`, or `32|64`).
 */
VEC_API unsigned vm_compiled_float_bits(void);

/**
 * @brief Non-zero when this TU's `VECMAT_FLOAT_BITS` is missing from the linked lib.
 */
static inline int vm_abi_mismatch(void)
{
    return (vm_compiled_float_bits() & (unsigned)VECMAT_FLOAT_BITS) == 0u;
}

#endif //VECMAT_CPU_H
