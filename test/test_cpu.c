// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <stdint.h>
#include <vecmat.h>
#include "unitest.h"

#if defined(VECMAT_TEST_THREADS)
#if defined(_WIN32) && defined(_MSC_VER)
#include <windows.h>
#else
#include <pthread.h>
#endif
#endif

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
#if defined(_WIN32) && defined(_MSC_VER)
static DWORD WINAPI vm_cpu_init_thread(LPVOID arg)
#else
static void *vm_cpu_init_thread(void *arg)
#endif
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
#if defined(_WIN32) && defined(_MSC_VER)
    return (DWORD)ok;
#else
    return (void *)(intptr_t)ok;
#endif
}

TEST_CASE(cpu_init_concurrent, "[cpu]") {
    enum { N = 8 };
#if defined(_WIN32) && defined(_MSC_VER)
    HANDLE threads[N];
    for (int i = 0; i < N; ++i) {
        threads[i] = CreateThread(NULL, 0, vm_cpu_init_thread, NULL, 0, NULL);
        REQUIRE(threads[i] != NULL);
    }
    for (int i = 0; i < N; ++i) {
        REQUIRE(WaitForSingleObject(threads[i], INFINITE) == WAIT_OBJECT_0);
        DWORD ok = 0;
        REQUIRE(GetExitCodeThread(threads[i], &ok));
        REQUIRE(ok == 1);
        CloseHandle(threads[i]);
    }
#else
    pthread_t threads[N];
    for (int i = 0; i < N; ++i) {
        REQUIRE(pthread_create(&threads[i], NULL, vm_cpu_init_thread, NULL) == 0);
    }
    for (int i = 0; i < N; ++i) {
        void *ok = NULL;
        REQUIRE(pthread_join(threads[i], &ok) == 0);
        REQUIRE((intptr_t)ok == 1);
    }
#endif
}
#endif
