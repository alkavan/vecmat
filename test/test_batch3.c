// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "unitest.h"

static void fill_diag(vm_float_t *p, const vm_float_t s)
{
    for (int i = 0; i < 9; ++i) {
        p[i] = VM_F(0.0);
    }
    p[0] = s;
    p[4] = s;
    p[8] = s;
}

TEST_CASE(batch3_mul_matches_scalar, "[batch3]") {
    vm_float_t A[9], B[9], C[9];
    fill_diag(A, VM_F(2.0));
    fill_diag(B, VM_F(3.0));
    REQUIRE(vm_batch3_mul(C, 9, A, 9, B, 9, 1));

    matrix3 a, b;
    for (int i = 0; i < 9; ++i) {
        a.v[i] = A[i];
        b.v[i] = B[i];
    }
    const matrix3 c = mat3_mul(a, b);
    for (int i = 0; i < 9; ++i) {
        REQUIRE(VECMAT_EQ(C[i], c.v[i], EPSILON));
    }
}

TEST_CASE(batch3_inverse_matches_scalar, "[batch3]") {
    vm_float_t A[9], C[9];
    fill_diag(A, VM_F(2.0));
    REQUIRE(vm_batch3_inverse(C, 9, A, 9, 1));

    matrix3 a, c;
    for (int i = 0; i < 9; ++i) {
        a.v[i] = A[i];
    }
    REQUIRE(mat3_inverse_ok(&c, &a));
    for (int i = 0; i < 9; ++i) {
        REQUIRE(VECMAT_EQ(C[i], c.v[i], EPSILON));
    }
}

TEST_CASE(batch3_polar_matches_scalar, "[batch3]") {
    vm_float_t F[9], R[9], U[9];
    fill_diag(F, VM_F(2.0));
    REQUIRE(vm_batch3_polar(R, 9, U, 9, F, 9, 1));

    matrix3 f, r, u;
    for (int i = 0; i < 9; ++i) {
        f.v[i] = F[i];
    }
    REQUIRE(mat3_polar(&r, &u, &f));
    for (int i = 0; i < 9; ++i) {
        REQUIRE(VECMAT_EQ(R[i], r.v[i], EPSILON));
        REQUIRE(VECMAT_EQ(U[i], u.v[i], EPSILON));
    }
}

TEST_CASE(batch3_ddot_matches_scalar, "[batch3]") {
    vm_float_t a[6] = { VM_F(1.0), VM_F(2.0), VM_F(3.0), VM_F(0.5), VM_F(0.0), VM_F(0.0) };
    vm_float_t b[6] = { VM_F(1.0), VM_F(0.0), VM_F(1.0), VM_F(1.0), VM_F(0.0), VM_F(0.0) };
    vm_float_t d;
    REQUIRE(vm_batch3_ddot(&d, a, 6, b, 6, 1));

    vm_sym3 sa, sb;
    for (int i = 0; i < 6; ++i) {
        sa.c[i] = a[i];
        sb.c[i] = b[i];
    }
    REQUIRE(VECMAT_EQ(d, vm_sym3_ddot(&sa, &sb), EPSILON));
}

TEST_CASE(batch3_strided_three, "[batch3]") {
    enum { stride = 12 };
    vm_float_t A[3 * stride], B[3 * stride], C[3 * stride];
    for (int i = 0; i < 3; ++i) {
        fill_diag(A + i * stride, VM_F(1.0) + (vm_float_t)i);
        fill_diag(B + i * stride, VM_F(2.0));
    }
    REQUIRE(vm_batch3_mul(C, stride, A, stride, B, stride, 3));
    for (int i = 0; i < 3; ++i) {
        REQUIRE(VECMAT_EQ(C[i * stride], (VM_F(1.0) + (vm_float_t)i) * VM_F(2.0), EPSILON));
    }
}
