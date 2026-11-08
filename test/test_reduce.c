// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <math.h>
#include <vecmat.h>
#include "unitest.h"

static vm_float_t neg_zero(void)
{
    return VECMAT_COPYSIGN(VM_F(0.0), VM_F(-1.0));
}

TEST_CASE(sum_compensated_recovers_one, "[reduce]") {
#ifdef VECMAT_USE_F64
    const vm_float_t big = 0x1p53;
#else
    const vm_float_t big = 0x1p24f;
#endif
    const vm_float_t x[3] = { big, VM_F(1.0), -big };
    const vm_float_t ones[3] = { VM_F(1.0), VM_F(1.0), VM_F(1.0) };
    REQUIRE(vm_sum_compensated(x, 3) == VM_F(1.0));
    REQUIRE(vm_dot_compensated(x, ones, 3) == VM_F(1.0));
    const vm_float_t v[4] = { VM_F(1.0), VM_F(2.0), VM_F(3.0), VM_F(4.0) };
    REQUIRE(vm_dot_compensated(v, v, 4) == VM_F(30.0));
    REQUIRE(vm_sum_compensated(NULL, 0) == VM_F(0.0));
    REQUIRE(!signbit(vm_sum_compensated(NULL, -2)));
    REQUIRE(vm_dot_compensated(NULL, NULL, 0) == VM_F(0.0));
}

TEST_CASE(sum_compensated_many_small_terms, "[reduce]") {
    enum { n = 1000 };
    vm_float_t x[n];
    for (int i = 0; i < n; ++i) {
        x[i] = VM_F(0.1);
    }
    const vm_float_t got = vm_sum_compensated(x, n);
    REQUIRE(VECMAT_EQ(got, VM_F(100.0), EPSILON * VM_F(100.0)));
}

TEST_CASE(sum_f32f64_widens_before_multiply, "[reduce]") {
    const float big24 = 0x1p24f;
    const float x24[3] = { big24, 1.0f, -big24 };
    const float ones[3] = { 1.0f, 1.0f, 1.0f };
    REQUIRE(vm_sum_f32f64(x24, 3) == 1.0);
    REQUIRE(vm_dot_f32f64(x24, ones, 3) == 1.0);

    const float big53 = 0x1p53f;
    const float x53[3] = { big53, 1.0f, -big53 };
    REQUIRE(vm_sum_f32f64(x53, 3) == 1.0);
    REQUIRE(vm_dot_f32f64(x53, ones, 3) == 1.0);

    const float huge = 1.0e30f;
    const double prod = vm_dot_f32f64(&huge, &huge, 1);
    REQUIRE(isfinite(prod));
    REQUIRE(isinf(huge * huge));
    REQUIRE(prod == (double)huge * (double)huge);
    REQUIRE(vm_sum_f32f64(NULL, 0) == 0.0);
    REQUIRE(!signbit(vm_dot_f32f64(NULL, NULL, -1)));
}

TEST_CASE(array_minmax_policy, "[reduce]") {
    vm_float_t lo = VM_F(5.0);
    vm_float_t hi = VM_F(6.0);
    REQUIRE(!vm_array_minmax(&lo, &hi, NULL, 0));
    REQUIRE(!vm_array_minmax(&lo, &hi, NULL, -3));
    REQUIRE(VECMAT_EQ(lo, VM_F(5.0), EPSILON));
    REQUIRE(VECMAT_EQ(hi, VM_F(6.0), EPSILON));

    const vm_float_t nans[3] = { NAN, NAN, NAN };
    REQUIRE(!vm_array_minmax(&lo, &hi, nans, 3));
    REQUIRE(VECMAT_EQ(lo, VM_F(5.0), EPSILON));
    REQUIRE(VECMAT_EQ(hi, VM_F(6.0), EPSILON));

    const vm_float_t zeros[2] = { VM_F(0.0), neg_zero() };
    REQUIRE(vm_array_minmax(&lo, &hi, zeros, 2));
    REQUIRE(lo == VM_F(0.0) && signbit(lo));
    REQUIRE(hi == VM_F(0.0) && !signbit(hi));

    const vm_float_t zeros_rev[2] = { neg_zero(), VM_F(0.0) };
    REQUIRE(vm_array_minmax(&lo, &hi, zeros_rev, 2));
    REQUIRE(signbit(lo));
    REQUIRE(!signbit(hi));

    const vm_float_t only_neg[1] = { neg_zero() };
    REQUIRE(vm_array_minmax(&lo, &hi, only_neg, 1));
    REQUIRE(signbit(lo) && signbit(hi));

    const vm_float_t mixed[5] = { NAN, VM_F(2.0), -INFINITY, VM_F(-1.0), INFINITY };
    REQUIRE(vm_array_minmax(&lo, &hi, mixed, 5));
    REQUIRE(isinf(lo) && signbit(lo));
    REQUIRE(isinf(hi) && !signbit(hi));
}
