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

TEST_CASE(fp16_vec4_mat4_named_fields, "[fp16]")
{
    vm_vec4_fp16 v;
    v.x = vm_fp16_from_f32(1.0f);
    v.y = vm_fp16_from_f32(2.0f);
    v.z = vm_fp16_from_f32(3.0f);
    v.w = vm_fp16_from_f32(4.0f);
    REQUIRE(v.v[0] == v.x);
    REQUIRE(v.v[1] == v.y);
    REQUIRE(v.v[2] == v.z);
    REQUIRE(v.v[3] == v.w);
    REQUIRE(vm_f32_from_fp16(v.x) == 1.0f);
    REQUIRE(vm_f32_from_fp16(v.y) == 2.0f);
    REQUIRE(vm_f32_from_fp16(v.z) == 3.0f);
    REQUIRE(vm_f32_from_fp16(v.w) == 4.0f);

    vm_mat4_fp16 a = {0};
    a.m11 = vm_fp16_from_f32(1.0f);
    a.m21 = vm_fp16_from_f32(2.0f);
    a.m31 = vm_fp16_from_f32(3.0f);
    a.m41 = vm_fp16_from_f32(4.0f);
    a.m12 = vm_fp16_from_f32(5.0f);
    a.m22 = vm_fp16_from_f32(6.0f);
    a.m32 = vm_fp16_from_f32(7.0f);
    a.m42 = vm_fp16_from_f32(8.0f);
    a.m13 = vm_fp16_from_f32(9.0f);
    a.m23 = vm_fp16_from_f32(10.0f);
    a.m33 = vm_fp16_from_f32(11.0f);
    a.m43 = vm_fp16_from_f32(12.0f);
    a.m14 = vm_fp16_from_f32(13.0f);
    a.m24 = vm_fp16_from_f32(14.0f);
    a.m34 = vm_fp16_from_f32(15.0f);
    a.m44 = vm_fp16_from_f32(16.0f);

    /* Column-major: m[col * 4 + row] */
    REQUIRE(a.m[0] == a.m11);
    REQUIRE(a.m[1] == a.m21);
    REQUIRE(a.m[2] == a.m31);
    REQUIRE(a.m[3] == a.m41);
    REQUIRE(a.m[4] == a.m12);
    REQUIRE(a.m[5] == a.m22);
    REQUIRE(a.m[6] == a.m32);
    REQUIRE(a.m[7] == a.m42);
    REQUIRE(a.m[8] == a.m13);
    REQUIRE(a.m[9] == a.m23);
    REQUIRE(a.m[10] == a.m33);
    REQUIRE(a.m[11] == a.m43);
    REQUIRE(a.m[12] == a.m14);
    REQUIRE(a.m[13] == a.m24);
    REQUIRE(a.m[14] == a.m34);
    REQUIRE(a.m[15] == a.m44);

    REQUIRE(vm_f32_from_fp16(a.m11) == 1.0f);
    REQUIRE(vm_f32_from_fp16(a.m22) == 6.0f);
    REQUIRE(vm_f32_from_fp16(a.m33) == 11.0f);
    REQUIRE(vm_f32_from_fp16(a.m44) == 16.0f);
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
    for (int i = 0; i < 4; ++i) {
        REQUIRE(near(c[i], ref[i], 1e-5f));
    }
    REQUIRE(strcmp(vm_fp16_note(), "fp16=scalar") == 0
            || strcmp(vm_fp16_note(), "fp16=neon") == 0
            || strcmp(vm_fp16_note(), "fp16=neon-cvt") == 0
            || strcmp(vm_fp16_note(), "fp16=f16c") == 0
            || strcmp(vm_fp16_note(), "fp16=avx512fp16") == 0);
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
