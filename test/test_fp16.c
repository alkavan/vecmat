// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <math.h>
#include <string.h>

#include "unitest.h"
#include "vecmat/fp16.h"

static int near(const float got, const float ref, const float tol)
{
    return fabsf(got - ref) <= tol;
}

TEST_CASE(fp16_roundtrip_exact, "[fp16]")
{
    const float exact[] = {
        0.0f, 1.0f, -1.0f, 2.0f, 0.5f, 65504.0f, 0x1p-14f, 0x1p-24f
    };
    for (unsigned i = 0; i < sizeof exact / sizeof exact[0]; ++i) {
        const vm_fp16_t h = vm_fp16_from_f32(exact[i]);
        REQUIRE(vm_f32_from_fp16(h) == exact[i]);
    }
    REQUIRE(vm_fp16_from_f32(-0.0f) == 0x8000u);
    REQUIRE(vm_f32_from_fp16(0x8000u) == 0.0f);
    REQUIRE(signbit(vm_f32_from_fp16(0x8000u)));
}

TEST_CASE(fp16_rounds_ties_to_even, "[fp16]")
{
    REQUIRE(vm_fp16_from_f32(1.0f + 0x1p-11f) == 0x3c00u);
    REQUIRE(vm_fp16_from_f32(1.0f + 3.0f * 0x1p-11f) == 0x3c02u);
    REQUIRE(vm_fp16_from_f32(0x1p-25f) == 0x0000u);
    REQUIRE(vm_fp16_from_f32(0x1p-24f + 0x1p-25f) == 0x0002u);
}

TEST_CASE(fp16_inf_and_nan, "[fp16]")
{
    REQUIRE(vm_fp16_from_f32(INFINITY) == 0x7c00u);
    REQUIRE(vm_fp16_from_f32(-INFINITY) == 0xfc00u);
    REQUIRE(vm_fp16_from_f32(70000.0f) == 0x7c00u);
    REQUIRE(vm_fp16_from_f32(NAN) == 0x7e00u);
    REQUIRE(vm_fp16_from_f32(-NAN) == 0x7e00u);
    REQUIRE(isinf(vm_f32_from_fp16(0x7c00u)));
    REQUIRE(isnan(vm_f32_from_fp16(0x7e00u)));
}

TEST_CASE(fp16_gemm_matches_converted_f32, "[fp16]")
{
    vm_fp16_t a[6], b[6];
    float c[4] = {2.0f, 2.0f, 2.0f, 2.0f};
    float ref[4] = {2.0f, 2.0f, 2.0f, 2.0f};
    for (int i = 0; i < 6; ++i) {
        const float af[] = {1.5f, -2.25f, 0.5f, 4.0f, 0.125f, -8.0f};
        const float bf[] = {3.0f, -1.0f, 0.5f, 2.0f, 6.0f, -0.25f};
        a[i] = vm_fp16_from_f32(af[i]);
        b[i] = vm_fp16_from_f32(bf[i]);
    }
    const int m = 2, n = 2, k = 3;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            float acc = 0.0f;
            for (int p = 0; p < k; ++p)
                acc += vm_f32_from_fp16(a[i * k + p]) * vm_f32_from_fp16(b[p * n + j]);
            ref[i * n + j] = 0.5f * ref[i * n + j] + 1.5f * acc;
        }
    }
    vm_gemm_fp16f32(c, n, a, k, b, n, m, n, k, 1.5f, 0.5f);
    for (int i = 0; i < 4; ++i)
        REQUIRE(near(c[i], ref[i], 1e-5f));
    REQUIRE(strcmp(vm_fp16_note(), "fp16=scalar") == 0);
}

TEST_CASE(fp16_gemm_long_k_stays_f32, "[fp16]")
{
    enum { K = 4096 };
    static vm_fp16_t a[K], b[K];
    const vm_fp16_t one = vm_fp16_from_f32(1.0f);
    for (int p = 0; p < K; ++p) {
        a[p] = one;
        b[p] = one;
    }
    float c = 0.0f;
    vm_gemm_fp16f32(&c, 1, a, K, b, 1, 1, 1, K, 1.0f, 0.0f);
    REQUIRE(c == 4096.0f);
}

TEST_CASE(fp16_gemm_leading_and_empty, "[fp16]")
{
    const vm_fp16_t a[] = {vm_fp16_from_f32(1.0f), 0, vm_fp16_from_f32(2.0f), 0};
    const vm_fp16_t b[] = {vm_fp16_from_f32(3.0f), vm_fp16_from_f32(4.0f)};
    float c[4] = {9.0f, 9.0f, 9.0f, 9.0f};
    vm_gemm_fp16f32(c, 2, a, 2, b, 2, 2, 1, 1, 1.0f, 0.0f);
    REQUIRE(c[0] == 3.0f);
    REQUIRE(c[2] == 6.0f);
    REQUIRE(c[1] == 9.0f);
    c[0] = 5.0f;
    vm_gemm_fp16f32(c, 2, a, 2, b, 2, 0, 1, 1, 1.0f, 0.0f);
    REQUIRE(c[0] == 5.0f);
}
