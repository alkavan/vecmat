// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include "cpu.h"

#if !defined(__STDC_NO_ATOMICS__)
#include <stdatomic.h>
#endif

#define VECMAT_FN_PTR(name, params, args) name##_fn name##_ = name##_scalar;
VECMAT_DISPATCH_LIST(VECMAT_FN_PTR)
#undef VECMAT_FN_PTR

#if !defined(__STDC_NO_ATOMICS__)
static atomic_int dispatch_ready;
static atomic_flag dispatch_lock = ATOMIC_FLAG_INIT;
#else
static volatile int dispatch_ready;
#endif

#if defined(VECMAT_ENABLE_SVE2)
#define VECMAT_PICK_SVE2(name)                 \
    if ((features & VM_CPU_SVE2) != 0) {       \
        fn = name##_sve2;                      \
    } else
#else
#define VECMAT_PICK_SVE2(name)
#endif

#if defined(VECMAT_ENABLE_SVE)
#define VECMAT_PICK_SVE(name)                  \
    if ((features & VM_CPU_SVE) != 0) {        \
        fn = name##_sve;                       \
    } else
#else
#define VECMAT_PICK_SVE(name)
#endif

#if defined(VECMAT_ENABLE_AVX512)
#define VECMAT_PICK_AVX512(name)               \
    if ((features & VM_CPU_AVX512) != 0) {     \
        fn = name##_avx512;                    \
    } else
#else
#define VECMAT_PICK_AVX512(name)
#endif

#if defined(VECMAT_ENABLE_AVX2)
#define VECMAT_PICK_AVX2(name)                 \
    if ((features & VM_CPU_AVX2) != 0) {       \
        fn = name##_avx2;                      \
    } else
#else
#define VECMAT_PICK_AVX2(name)
#endif

#if defined(VECMAT_ENABLE_AVX)
#define VECMAT_PICK_AVX(name)                  \
    if ((features & VM_CPU_AVX) != 0) {        \
        fn = name##_avx;                       \
    } else
#else
#define VECMAT_PICK_AVX(name)
#endif

#define VECMAT_PICK(name)                      \
    do {                                       \
        name##_fn fn = name##_scalar;          \
        VECMAT_PICK_SVE2(name)                 \
        VECMAT_PICK_SVE(name)                  \
        VECMAT_PICK_AVX512(name)               \
        VECMAT_PICK_AVX2(name)                 \
        VECMAT_PICK_AVX(name)                  \
        { /* scalar already set */ }           \
        name##_ = fn;                          \
    } while (0)

/**
 * @brief Binds dispatched function pointers for `features`.
 *
 * @param features Selected ISA mask.
 */
static void vm_cpu_bind(const vm_cpu_features_t features)
{
#define VECMAT_BIND(name, params, args) VECMAT_PICK(name);
    VECMAT_DISPATCH_LIST(VECMAT_BIND)
#undef VECMAT_BIND
}

/**
 * @brief One-time runtime dispatch bind (thread-safe).
 */
void vm_cpu_init(void)
{
#if !defined(__STDC_NO_ATOMICS__)
    if (atomic_load_explicit(&dispatch_ready, memory_order_acquire))
        return;
    while (atomic_flag_test_and_set_explicit(&dispatch_lock, memory_order_acquire)) {
        /* spin until the initializing thread finishes */
    }
    if (!atomic_load_explicit(&dispatch_ready, memory_order_relaxed)) {
        vm_cpu_bind(vm_cpu_selected_features());
        atomic_store_explicit(&dispatch_ready, 1, memory_order_release);
    }
    atomic_flag_clear_explicit(&dispatch_lock, memory_order_release);
#else
    if (dispatch_ready)
        return;
    vm_cpu_bind(vm_cpu_selected_features());
    dispatch_ready = 1;
#endif
}
