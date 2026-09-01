// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <stdint.h>
#include <vecmat.h>
#include "unitest.h"

#if defined(VECMAT_TEST_THREADS)
#include <pthread.h>
#endif

TEST_CASE(abi_float_bits_match_header, "[abi]") {
    REQUIRE(vm_compiled_float_bits() == VECMAT_FLOAT_BITS);
    REQUIRE(vm_abi_mismatch() == 0);
#if defined(VECMAT_USE_F64)
    REQUIRE(VECMAT_FLOAT_BITS == 64);
    REQUIRE(vm_compiled_float_bits() == 64);
#else
    REQUIRE(VECMAT_FLOAT_BITS == 32);
    REQUIRE(vm_compiled_float_bits() == 32);
#endif
}

TEST_CASE(cpu_features_test, "[cpu]") {
    vm_cpu_init();

    const vm_cpu_features_t compiled = vm_cpu_compiled_features();
    const vm_cpu_features_t runtime = vm_cpu_runtime_features();
    const vm_cpu_features_t selected = vm_cpu_selected_features();

    REQUIRE((compiled & VM_CPU_SCALAR) == VM_CPU_SCALAR);
    REQUIRE((runtime & VM_CPU_SCALAR) == VM_CPU_SCALAR);
    REQUIRE(selected == VM_CPU_SCALAR ||
            selected == VM_CPU_AVX ||
            selected == VM_CPU_AVX2 ||
            selected == VM_CPU_SVE ||
            selected == VM_CPU_AVX512 ||
            selected == VM_CPU_SVE2);
    REQUIRE((selected & compiled) == selected);
    REQUIRE((selected & runtime) == selected);
    REQUIRE(vm_cpu_name(selected) != NULL);
    REQUIRE(vm_cpu_name(VM_CPU_SCALAR)[0] == 's');
    REQUIRE(vm_cpu_name(VM_CPU_AVX512)[0] == 'a');

    if ((compiled & VM_CPU_AVX512) && (runtime & VM_CPU_AVX512))
        REQUIRE(selected == VM_CPU_AVX512);
    if ((compiled & VM_CPU_SVE2) && (runtime & VM_CPU_SVE2))
        REQUIRE(selected == VM_CPU_SVE2);
    REQUIRE(vm_cpu_name(VM_CPU_SVE2)[0] == 's');
}

TEST_CASE(cpu_dispatched_vec4_add_matches_scalar_shape, "[cpu][vector4]") {
    const vector4 a = {.x = 1.0f, .y = -2.0f, .z = 3.5f, .w = 0.25f};
    const vector4 b = {.x = 4.0f, .y = 5.0f, .z = -1.5f, .w = 2.0f};
    vector4 res;
    vec4_add_ptr(&res, &a, &b);
    REQUIRE(VECMAT_EQ(res.x, 5.0f, EPSILON));
    REQUIRE(VECMAT_EQ(res.y, 3.0f, EPSILON));
    REQUIRE(VECMAT_EQ(res.z, 2.0f, EPSILON));
    REQUIRE(VECMAT_EQ(res.w, 2.25f, EPSILON));
}

TEST_CASE(cpu_init_is_idempotent, "[cpu]") {
    vm_cpu_init();
    const vm_cpu_features_t first = vm_cpu_selected_features();
    vm_cpu_init();
    vm_cpu_init();
    REQUIRE(vm_cpu_selected_features() == first);
    REQUIRE(vm_cpu_runtime_features() == vm_cpu_runtime_features());
}

#if defined(VECMAT_TEST_THREADS)
static void *vm_cpu_init_thread(void *arg)
{
    (void)arg;
    vm_cpu_init();
    const vector4 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f};
    const vector4 b = {.x = 4.0f, .y = 3.0f, .z = 2.0f, .w = 1.0f};
    vector4 res;
    vec4_add_ptr(&res, &a, &b);
    const int ok = VECMAT_EQ(res.x, 5.0f, EPSILON) &&
                   VECMAT_EQ(res.y, 5.0f, EPSILON) &&
                   VECMAT_EQ(res.z, 5.0f, EPSILON) &&
                   VECMAT_EQ(res.w, 5.0f, EPSILON);
    return (void *)(intptr_t)ok;
}

TEST_CASE(cpu_init_concurrent, "[cpu]") {
    enum { N = 8 };
    pthread_t threads[N];
    for (int i = 0; i < N; ++i) {
        REQUIRE(pthread_create(&threads[i], NULL, vm_cpu_init_thread, NULL) == 0);
    }
    for (int i = 0; i < N; ++i) {
        void *ok = NULL;
        REQUIRE(pthread_join(threads[i], &ok) == 0);
        REQUIRE((intptr_t)ok == 1);
    }
}
#endif
