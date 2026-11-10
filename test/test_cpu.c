// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
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
            selected == VM_CPU_NEON ||
            selected == VM_CPU_SVE ||
            selected == VM_CPU_AVX512F ||
            selected == VM_CPU_SVE2 ||
            selected == VM_CPU_BACKEND);
    REQUIRE((selected & compiled) == selected || selected == VM_CPU_BACKEND);
    REQUIRE((selected & runtime) == selected || selected == VM_CPU_BACKEND);
    REQUIRE(vm_cpu_name(selected) != NULL);
    REQUIRE(vm_cpu_name(VM_CPU_SCALAR)[0] == 's');
    REQUIRE(vm_cpu_name(VM_CPU_AVX512F)[0] == 'a');

    if (selected != VM_CPU_BACKEND) {
        const char *forced = getenv("VECMAT_FORCE_ISA");
        if (forced && forced[0] != '\0' && strcmp(forced, "avx512") != 0)
            REQUIRE(strcmp(vm_cpu_name(selected), forced) == 0);
        else {
            if ((compiled & VM_CPU_AVX512F) && (runtime & VM_CPU_AVX512F))
                REQUIRE(selected == VM_CPU_AVX512F);
            if ((compiled & VM_CPU_SVE2) && (runtime & VM_CPU_SVE2))
                REQUIRE(selected == VM_CPU_SVE2);
            if ((compiled & VM_CPU_NEON) && (runtime & VM_CPU_NEON) &&
                (compiled & VM_CPU_SVE) == 0 && (runtime & VM_CPU_SVE) == 0)
                REQUIRE(selected == VM_CPU_NEON);
        }
    }
    REQUIRE(vm_cpu_name(VM_CPU_SVE2)[0] == 's');
    REQUIRE(vm_cpu_name(VM_CPU_NEON)[0] == 'n');
    REQUIRE(vm_cpu_note() == NULL || vm_cpu_note()[0] != '\0');
}

TEST_CASE(cpu_fma_bit_is_consistent, "[cpu]") {
    vm_cpu_init();

    const vm_cpu_features_t compiled = vm_cpu_compiled_features();
    const vm_cpu_features_t runtime = vm_cpu_runtime_features();
    const vm_cpu_features_t selected = vm_cpu_selected_features();

    /* FMA needs the same YMM state as AVX. */
    if (runtime & VM_CPU_FMA)
        REQUIRE((runtime & VM_CPU_AVX) != 0);
    /* The AVX2 and AVX-512F kernels are built with -mfma. */
    if (compiled & (VM_CPU_AVX2 | VM_CPU_AVX512F))
        REQUIRE((compiled & VM_CPU_FMA) != 0);
    /* FMA is a feature bit, never a selected tier. */
    REQUIRE(selected != VM_CPU_FMA);
    REQUIRE(strcmp(vm_cpu_name(VM_CPU_FMA), "fma") == 0);
    REQUIRE(strcmp(vm_cpu_name(VM_CPU_AVX2 | VM_CPU_FMA), "avx2") == 0);

    /* Without a force or a backend, a tier implies its whole level; only
     * AVX2 adds FMA (AVX-512F implies it). */
    const char *forced = getenv("VECMAT_FORCE_ISA");
    if (selected != VM_CPU_BACKEND && !(forced && forced[0] != '\0')) {
        if (selected == VM_CPU_AVX2)
            REQUIRE(vm_cpu_has(VM_LEVEL_AVX2));
        if (selected == VM_CPU_AVX512F)
            REQUIRE(vm_cpu_has(VM_LEVEL_AVX512));
        if ((compiled & VM_CPU_AVX2) && vm_cpu_has(VM_CPU_AVX2) && !vm_cpu_has(VM_CPU_FMA))
            REQUIRE(selected != VM_CPU_AVX2);
    }
}

TEST_CASE(cpu_has_needs_every_bit, "[cpu]") {
    const vm_cpu_features_t runtime = vm_cpu_runtime_features();

    REQUIRE(vm_cpu_has(VM_CPU_SCALAR));
    REQUIRE(vm_cpu_has(0));
    REQUIRE(vm_cpu_has(runtime));
    REQUIRE(vm_cpu_has(VM_LEVEL_AVX2) ==
            ((runtime & VM_CPU_AVX2) != 0 && (runtime & VM_CPU_FMA) != 0));
    REQUIRE(vm_cpu_has(VM_LEVEL_AVX512) == ((runtime & VM_CPU_AVX512F) != 0));
    /* One missing bit fails the whole set. */
    REQUIRE(!vm_cpu_has(VM_CPU_SCALAR | VM_CPU_BACKEND));
}

TEST_CASE(cpu_bits_sit_in_family_ranges, "[cpu]") {
    const vm_cpu_features_t families[4] = {
        VM_CPU_GENERIC_MASK, VM_CPU_X86_MASK, VM_CPU_ARM_MASK, VM_CPU_RISCV_MASK
    };
    vm_cpu_features_t all = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = i + 1; j < 4; ++j)
            REQUIRE((families[i] & families[j]) == 0);
        all |= families[i];
    }
    /* The families cover bits 0..30; bit 31 is unused (int enum limit). */
    REQUIRE(all == 0x7FFFFFFFu);

    const vm_cpu_features_t generic = VM_CPU_SCALAR | VM_CPU_BACKEND;
    const vm_cpu_features_t x86 = VM_CPU_AVX | VM_CPU_AVX2 | VM_CPU_AVX512F | VM_CPU_FMA;
    const vm_cpu_features_t arm = VM_CPU_NEON | VM_CPU_SVE | VM_CPU_SVE2;
    REQUIRE((generic & ~VM_CPU_GENERIC_MASK) == 0);
    REQUIRE((x86 & ~VM_CPU_X86_MASK) == 0);
    REQUIRE((arm & ~VM_CPU_ARM_MASK) == 0);
    /* Every level stays within one family. */
    REQUIRE((VM_LEVEL_AVX2 & ~VM_CPU_X86_MASK) == 0);
    REQUIRE((VM_LEVEL_AVX512 & ~VM_CPU_X86_MASK) == 0);
    REQUIRE((VM_LEVEL_SVE2 & ~VM_CPU_ARM_MASK) == 0);
    /* Probed masks never use bit 31 or a bit outside the defined set. */
    const vm_cpu_features_t defined = generic | x86 | arm;
    REQUIRE((vm_cpu_runtime_features() & ~defined) == 0);
    REQUIRE((vm_cpu_compiled_features() & ~defined) == 0);
    REQUIRE((defined & 0x80000000u) == 0);
}

TEST_CASE(cpu_ladder_matches_if_chain, "[cpu]") {
    vm_cpu_init();
    const char *forced = getenv("VECMAT_FORCE_ISA");
    const vm_cpu_features_t selected = vm_cpu_selected_features();
    if (selected == VM_CPU_BACKEND || (forced && forced[0] != '\0'))
        return;

    /* The pre-table picker, spelled out. */
    const vm_cpu_features_t have = vm_cpu_compiled_features() & vm_cpu_runtime_features();
    vm_cpu_features_t expect = VM_CPU_SCALAR;
    if (have & VM_CPU_SVE2)
        expect = VM_CPU_SVE2;
    else if (have & VM_CPU_SVE)
        expect = VM_CPU_SVE;
    else if (have & VM_CPU_NEON)
        expect = VM_CPU_NEON;
    else if (have & VM_CPU_AVX512F)
        expect = VM_CPU_AVX512F;
    else if ((have & VM_CPU_AVX2) && (have & VM_CPU_FMA))
        expect = VM_CPU_AVX2;
    else if (have & VM_CPU_AVX)
        expect = VM_CPU_AVX;
    REQUIRE(selected == expect);
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
