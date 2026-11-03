// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <math.h>
#include <string.h>

#include "unitest.h"
#include "vecmat/bf16.h"

static int near(const float got, const float ref, const float tol)
{
    return fabsf(got - ref) <= tol;
}

TEST_CASE(bf16_roundtrip_exact, "[bf16]")
{
    const float exact[] = { 0.0f, 1.0f, -1.0f, 2.0f, 0.5f };
    for (unsigned i = 0; i < sizeof exact / sizeof exact[0]; ++i) {
        const vm_bf16_t h = vm_bf16_from_f32(exact[i]);
        REQUIRE(vm_f32_from_bf16(h) == exact[i]);
    }
    REQUIRE(vm_bf16_from_f32(-0.0f) == 0x8000u);
    REQUIRE(vm_f32_from_bf16(0x8000u) == 0.0f);
    REQUIRE(signbit(vm_f32_from_bf16(0x8000u)));
}

TEST_CASE(bf16_rounds_ties_to_even, "[bf16]")
{
    REQUIRE(vm_bf16_from_f32(1.0f + 0x1p-8f) == 0x3f80u);
    REQUIRE(vm_bf16_from_f32(1.0f + 3.0f * 0x1p-8f) == 0x3f82u);
}

TEST_CASE(bf16_inf_and_nan, "[bf16]")
{
    REQUIRE(vm_bf16_from_f32(INFINITY) == 0x7f80u);
    REQUIRE(vm_bf16_from_f32(-INFINITY) == 0xff80u);
    REQUIRE(vm_bf16_from_f32(NAN) == 0x7fc0u);
    REQUIRE(vm_bf16_from_f32(-NAN) == 0x7fc0u);
    REQUIRE(isinf(vm_f32_from_bf16(0x7f80u)));
    REQUIRE(isnan(vm_f32_from_bf16(0x7fc0u)));
}

TEST_CASE(bf16_gemm_matches_reference, "[bf16]")
{
    enum { m = 3, n = 2, k = 2 };
    vm_bf16_t a[m * k], b[k * n];
    float c[m * n], ref[m * n];
    const float av[] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f };
    const float bv[] = { 1.0f, 0.5f, 0.25f, 2.0f };
    for (int i = 0; i < m * k; ++i)
        a[i] = vm_bf16_from_f32(av[i]);
    for (int i = 0; i < k * n; ++i)
        b[i] = vm_bf16_from_f32(bv[i]);
    for (int i = 0; i < m * n; ++i) {
        c[i] = 1.0f;
        ref[i] = 0.5f;
    }
    for (int i = 0; i < m; ++i)
        for (int p = 0; p < k; ++p)
            for (int j = 0; j < n; ++j)
                ref[i * n + j] += 1.5f * vm_f32_from_bf16(a[i * k + p]) *
                                  vm_f32_from_bf16(b[p * n + j]);
    vm_gemm_bf16f32(c, n, a, k, b, n, m, n, k, 1.5f, 0.5f);
    for (int i = 0; i < m * n; ++i)
        REQUIRE(near(c[i], ref[i], 1e-5f));
    REQUIRE(strcmp(vm_bf16_note(), "bf16=scalar") == 0
            || strcmp(vm_bf16_note(), "bf16=avx512bf16") == 0);
}

TEST_CASE(bf16_gemm_long_k_stays_f32, "[bf16]")
{
    enum { K = 4096 };
    static vm_bf16_t a[K], b[K];
    const vm_bf16_t one = vm_bf16_from_f32(1.0f);
    for (int p = 0; p < K; ++p) {
        a[p] = one;
        b[p] = one;
    }
    float c = 0.0f;
    vm_gemm_bf16f32(&c, 1, a, K, b, 1, 1, 1, K, 1.0f, 0.0f);
    REQUIRE(c == 4096.0f);
}
