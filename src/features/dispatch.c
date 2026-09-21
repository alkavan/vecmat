// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include "cpu.h"

#include <string.h>

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

static const vm_backend *vm_backends[VM_BACKEND_MAX];
static int vm_backend_count;

static int vm_backend_ops_complete(const vm_backend_ops *ops)
{
#define VECMAT_REQUIRE_OP(name, params, args) \
    if (ops->name == NULL)                    \
        return 0;
    VECMAT_DISPATCH_LIST(VECMAT_REQUIRE_OP)
#undef VECMAT_REQUIRE_OP
    return 1;
}

const vm_backend *vm_backend_best(void)
{
    const vm_backend *best = NULL;

    for (int i = 0; i < vm_backend_count; ++i) {
        const vm_backend *b = vm_backends[i];
        if (!best || b->priority > best->priority)
            best = b;
    }
    return best;
}

static void vm_cpu_bind_backend(const vm_backend *backend)
{
#define VECMAT_BIND_BACKEND(name, params, args) name##_ = backend->ops->name;
    VECMAT_DISPATCH_LIST(VECMAT_BIND_BACKEND)
#undef VECMAT_BIND_BACKEND
}

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

#if defined(VECMAT_ENABLE_NEON)
#define VECMAT_PICK_NEON(name)                  \
    if ((features & VM_CPU_NEON) != 0) {        \
        fn = name##_neon;                       \
    } else
#else
#define VECMAT_PICK_NEON(name)
#endif

#if defined(VECMAT_ENABLE_AVX512F)
#define VECMAT_PICK_AVX512F(name)               \
    if ((features & VM_CPU_AVX512F) != 0) {     \
        fn = name##_avx512;                     \
    } else
#else
#define VECMAT_PICK_AVX512F(name)
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
        VECMAT_PICK_NEON(name)                 \
        VECMAT_PICK_AVX512F(name)              \
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
    const vm_backend *backend = vm_backend_best();

    if (backend) {
        vm_cpu_bind_backend(backend);
        return;
    }
#define VECMAT_BIND(name, params, args) VECMAT_PICK(name);
    VECMAT_DISPATCH_LIST(VECMAT_BIND)
#undef VECMAT_BIND
}

/**
 * @brief Register an external backend (see `vm_backend_register`).
 */
int vm_backend_register(const vm_backend *backend)
{
    int ready;

    if (!backend || !backend->name || backend->name[0] == '\0' || !backend->ops)
        return VM_BACKEND_ERR_INVAL;
    if (!vm_backend_ops_complete(backend->ops))
        return VM_BACKEND_ERR_INVAL;

#if !defined(__STDC_NO_ATOMICS__)
    while (atomic_flag_test_and_set_explicit(&dispatch_lock, memory_order_acquire)) {
        /* spin */
    }
#endif

    for (int i = 0; i < vm_backend_count; ++i) {
        if (vm_backends[i] == backend) {
#if !defined(__STDC_NO_ATOMICS__)
            atomic_flag_clear_explicit(&dispatch_lock, memory_order_release);
#endif
            return VM_BACKEND_OK;
        }
        if (strcmp(vm_backends[i]->name, backend->name) == 0) {
#if !defined(__STDC_NO_ATOMICS__)
            atomic_flag_clear_explicit(&dispatch_lock, memory_order_release);
#endif
            return VM_BACKEND_ERR_CONFLICT;
        }
    }

    if (vm_backend_count >= VM_BACKEND_MAX) {
#if !defined(__STDC_NO_ATOMICS__)
        atomic_flag_clear_explicit(&dispatch_lock, memory_order_release);
#endif
        return VM_BACKEND_ERR_FULL;
    }

    vm_backends[vm_backend_count++] = backend;

#if !defined(__STDC_NO_ATOMICS__)
    ready = atomic_load_explicit(&dispatch_ready, memory_order_relaxed);
#else
    ready = dispatch_ready;
#endif
    if (ready)
        vm_cpu_bind(vm_cpu_selected_features());

#if !defined(__STDC_NO_ATOMICS__)
    atomic_flag_clear_explicit(&dispatch_lock, memory_order_release);
#endif
    return VM_BACKEND_OK;
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
