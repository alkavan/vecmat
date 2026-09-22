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

enum {
    VM_CPU_SCALAR  = 1u << 0,
    VM_CPU_AVX2    = 1u << 1,
    VM_CPU_SVE     = 1u << 2,
    VM_CPU_AVX512F = 1u << 3,
    VM_CPU_SVE2    = 1u << 4,
    VM_CPU_AVX     = 1u << 5,
    VM_CPU_NEON    = 1u << 6,
    /** Registered external backend with no ISA bit of its own. */
    VM_CPU_BACKEND = 1u << 7
};

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
 */
VEC_API vm_cpu_features_t vm_cpu_selected_features(void);

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
