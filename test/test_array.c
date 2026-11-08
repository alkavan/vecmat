// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <float.h>
#include <math.h>
#include <vecmat.h>
#include "unitest.h"

#ifdef VECMAT_USE_F64
#define MACH_EPS DBL_EPSILON
#else
#define MACH_EPS FLT_EPSILON
#endif

static vm_float_t neg_zero(void)
{
    return VECMAT_COPYSIGN(VM_F(0.0), VM_F(-1.0));
}

TEST_CASE(array_add_sub_mul_matches_scalar, "[array]") {
    const vm_float_t a[4] = { VM_F(1.0), VM_F(-2.0), VM_F(3.5), VM_F(0.0) };
    const vm_float_t b[4] = { VM_F(4.0), VM_F(2.0), VM_F(-1.0), VM_F(8.0) };
    vm_float_t r[4];

    vm_array_add(r, a, b, 4);
    for (int i = 0; i < 4; ++i) {
        REQUIRE(VECMAT_EQ(r[i], a[i] + b[i], EPSILON));
    }
    vm_array_sub(r, a, b, 4);
    for (int i = 0; i < 4; ++i) {
        REQUIRE(VECMAT_EQ(r[i], a[i] - b[i], EPSILON));
    }
    vm_array_mul(r, a, b, 4);
    for (int i = 0; i < 4; ++i) {
        REQUIRE(VECMAT_EQ(r[i], a[i] * b[i], EPSILON));
    }
}

TEST_CASE(array_div_matches_scalar, "[array]") {
    const vm_float_t a[4] = { VM_F(8.0), VM_F(-6.0), VM_F(3.5), VM_F(1.0) };
    const vm_float_t b[4] = { VM_F(2.0), VM_F(3.0), VM_F(-1.0), VM_F(4.0) };
    vm_float_t r[4];
    vm_array_div(r, a, b, 4);
    for (int i = 0; i < 4; ++i) {
        REQUIRE(VECMAT_EQ(r[i], a[i] / b[i], EPSILON));
    }
    /* exact alias */
    vm_float_t c[3] = { VM_F(9.0), VM_F(4.0), VM_F(-2.0) };
    const vm_float_t d[3] = { VM_F(3.0), VM_F(2.0), VM_F(4.0) };
    vm_array_div(c, c, d, 3);
    REQUIRE(VECMAT_EQ(c[0], VM_F(3.0), EPSILON));
    REQUIRE(VECMAT_EQ(c[1], VM_F(2.0), EPSILON));
    REQUIRE(VECMAT_EQ(c[2], VM_F(-0.5), EPSILON));
    /* division by zero and empty */
    vm_array_div(NULL, NULL, NULL, 0);
    const vm_float_t z[2] = { VM_F(1.0), VM_F(0.0) };
    const vm_float_t w[2] = { VM_F(0.0), VM_F(0.0) };
    vm_float_t s[2];
    vm_array_div(s, z, w, 2);
    REQUIRE(isinf(s[0]) && !signbit(s[0]));
    REQUIRE(isnan(s[1]));
}

TEST_CASE(array_fma_matches_libm, "[array]") {
    const vm_float_t a[3] = { VM_F(1.25), VM_F(-2.0), VM_F(0.5) };
    const vm_float_t b[3] = { VM_F(4.0), VM_F(3.5), VM_F(-8.0) };
    const vm_float_t c[3] = { VM_F(0.5), VM_F(1.0), VM_F(2.0) };
    vm_float_t r[3];
    vm_array_fma(r, a, b, c, 3);
    for (int i = 0; i < 3; ++i) {
        REQUIRE(r[i] == VECMAT_FMA(a[i], b[i], c[i]));
    }
}

TEST_CASE(array_alias_and_odd_length, "[array]") {
    vm_float_t a[7];
    vm_float_t b[7];
    for (int i = 0; i < 7; ++i) {
        a[i] = VM_F(0.5) * (vm_float_t)(i + 1);
        b[i] = VM_F(0.25) * (vm_float_t)(i - 2);
    }
    vm_array_add(a, a, b, 7);
    for (int i = 0; i < 7; ++i) {
        const vm_float_t left = VM_F(0.5) * (vm_float_t)(i + 1);
        const vm_float_t right = VM_F(0.25) * (vm_float_t)(i - 2);
        REQUIRE(VECMAT_EQ(a[i], left + right, EPSILON));
    }

    vm_float_t y[3] = { VM_F(1.0), VM_F(2.0), VM_F(3.0) };
    vm_axpy(y, VM_F(2.0), y, 3);
    REQUIRE(VECMAT_EQ(y[0], VM_F(3.0), EPSILON));
    REQUIRE(VECMAT_EQ(y[1], VM_F(6.0), EPSILON));
    REQUIRE(VECMAT_EQ(y[2], VM_F(9.0), EPSILON));
}

TEST_CASE(array_axpy_forward_error, "[array]") {
    vm_float_t y[4] = { VM_F(1.0), VM_F(-2.0), VM_F(3.5), VM_F(8.0) };
    const vm_float_t x[4] = { VM_F(0.5), VM_F(4.0), VM_F(-1.0), VM_F(0.25) };
    const vm_float_t old[4] = { y[0], y[1], y[2], y[3] };
    const vm_float_t alpha = VM_F(-1.5);
    vm_axpy(y, alpha, x, 4);
    for (int i = 0; i < 4; ++i) {
        const double ref = (double)old[i] + (double)alpha * (double)x[i];
        const double scale = fabs((double)old[i]) + fabs((double)alpha * (double)x[i]);
        const double bound = 8.0 * (double)MACH_EPS * scale;
        REQUIRE(fabs((double)y[i] - ref) <= bound + (double)MACH_EPS);
        REQUIRE(VECMAT_EQ(y[i], (vm_float_t)ref, EPSILON));
    }
}

TEST_CASE(array_empty_and_specials, "[array]") {
    vm_array_add(NULL, NULL, NULL, 0);
    vm_array_sub(NULL, NULL, NULL, -1);
    vm_array_mul(NULL, NULL, NULL, 0);
    vm_array_div(NULL, NULL, NULL, 0);
    vm_array_fma(NULL, NULL, NULL, NULL, -4);
    vm_axpy(NULL, VM_F(1.0), NULL, 0);

    const vm_float_t a[2] = { INFINITY, INFINITY };
    const vm_float_t b[2] = { VM_F(-1.0), -INFINITY };
    vm_float_t r[2];
    vm_array_add(r, a, b, 2);
    REQUIRE(isinf(r[0]) && !signbit(r[0]));
    REQUIRE(isnan(r[1]));
}

TEST_CASE(array_suffix_matches_unsuffixed, "[array]") {
    const vm_float_t a[2] = { VM_F(1.0), VM_F(2.0) };
    const vm_float_t b[2] = { VM_F(3.0), VM_F(4.0) };
    vm_float_t r[2];
    vm_float_t s[2];
    vm_array_add(r, a, b, 2);
#if VECMAT_FLOAT_BITS == 64
    vm_array_add64(s, a, b, 2);
#else
    vm_array_add32(s, a, b, 2);
#endif
    REQUIRE(VECMAT_EQ(r[0], s[0], EPSILON));
    REQUIRE(VECMAT_EQ(r[1], s[1], EPSILON));
    REQUIRE(VECMAT_EQ(r[0], VM_F(4.0), EPSILON));
}

TEST_CASE(rsqrt_specials_and_normals, "[array]") {
    const vm_float_t x[6] = {
        VM_F(0.0), neg_zero(), INFINITY, VM_F(-1.0), -INFINITY, NAN
    };
    vm_float_t y[6];
    vm_rsqrt_array(y, x, 6);
    REQUIRE(isinf(y[0]) && !signbit(y[0]));
    REQUIRE(isinf(y[1]) && signbit(y[1]));
    REQUIRE(y[2] == VM_F(0.0) && !signbit(y[2]));
    REQUIRE(isnan(y[3]));
    REQUIRE(isnan(y[4]));
    REQUIRE(isnan(y[5]));

    vm_float_t squares[3] = { VM_F(1.0), VM_F(4.0), VM_F(0.25) };
    vm_rsqrt_array(squares, squares, 3);
    REQUIRE(VECMAT_EQ(squares[0], VM_F(1.0), EPSILON));
    REQUIRE(VECMAT_EQ(squares[1], VM_F(0.5), EPSILON));
    REQUIRE(VECMAT_EQ(squares[2], VM_F(2.0), EPSILON));

#ifdef VECMAT_USE_F64
    const vm_float_t sub = 0x1p-140;
    const vm_float_t expect = 0x1p70;
#else
    const vm_float_t sub = 0x1p-140f;
    const vm_float_t expect = 0x1p70f;
#endif
    vm_float_t got;
    vm_rsqrt_array(&got, &sub, 1);
    REQUIRE(VECMAT_EQ(got, expect, EPSILON));

    const vm_float_t sample = VM_F(2.5);
    vm_rsqrt_array(&got, &sample, 1);
    const double ref = 1.0 / sqrt((double)sample);
    const double ulp = 2.0 * (double)MACH_EPS * fabs(ref);
    REQUIRE(fabs((double)got - ref) <= ulp + (double)MACH_EPS);

    /* wider positive range, including a tail past the SIMD width */
    vm_float_t xs[17];
    vm_float_t ys[17];
    for (int i = 0; i < 17; ++i) {
        xs[i] = VM_F(0.1) * (vm_float_t)(i + 1);
    }
    vm_rsqrt_array(ys, xs, 17);
    for (int i = 0; i < 17; ++i) {
        const double r = 1.0 / sqrt((double)xs[i]);
        const double bound = 2.0 * (double)MACH_EPS * fabs(r);
        REQUIRE(fabs((double)ys[i] - r) <= bound + (double)MACH_EPS);
    }
}
