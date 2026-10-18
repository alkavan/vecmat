// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "unitest.h"

static matrix3 diag3(const vm_float_t a, const vm_float_t b, const vm_float_t c)
{
    matrix3 m;
    for (int i = 0; i < 9; ++i) {
        m.v[i] = VM_F(0.0);
    }
    m.m11 = a;
    m.m22 = b;
    m.m33 = c;
    return m;
}

static int near_mat(const matrix3 *a, const matrix3 *b, const vm_float_t tol)
{
    for (int i = 0; i < 9; ++i) {
        if (!VECMAT_EQ(a->v[i], b->v[i], tol)) {
            return 0;
        }
    }
    return 1;
}

TEST_CASE(strain_log_of_isotropic_stretch, "[strain]") {
    /* U = λ I → ln(U) = ln(λ) I */
    const vm_float_t lambda = VM_F(2.0);
    const matrix3 U = diag3(lambda, lambda, lambda);
    matrix3 logU;
    REQUIRE(vm_strain_logarithmic(&logU, &U));
    const vm_float_t ln_l = VECMAT_LOG(lambda);
    const matrix3 expect = diag3(ln_l, ln_l, ln_l);
    REQUIRE(near_mat(&logU, &expect, EPSILON * VM_F(16.0)));
}

TEST_CASE(strain_green_lagrange_pure_rotation, "[strain]") {
    /* Pure rotation → E = 0 */
    const matrix3 R = mat3_rotation_z(VM_F(0.7));
    const matrix3 E = vm_strain_green_lagrange(&R);
    const matrix3 zero = diag3(VM_F(0.0), VM_F(0.0), VM_F(0.0));
    REQUIRE(near_mat(&E, &zero, EPSILON * VM_F(16.0)));
}

TEST_CASE(strain_cauchy_green_pure_rotation, "[strain]") {
    const matrix3 R = mat3_rotation_x(VM_F(0.4));
    const matrix3 C = vm_strain_cauchy_green(&R);
    const matrix3 I = mat3_identity();
    REQUIRE(near_mat(&C, &I, EPSILON * VM_F(16.0)));
}

TEST_CASE(strain_green_lagrange_uniaxial, "[strain]") {
    /* F = diag(2, 1, 1) → C = diag(4, 1, 1) → E = diag(1.5, 0, 0) */
    const matrix3 F = diag3(VM_F(2.0), VM_F(1.0), VM_F(1.0));
    const matrix3 E = vm_strain_green_lagrange(&F);
    const matrix3 expect = diag3(VM_F(1.5), VM_F(0.0), VM_F(0.0));
    REQUIRE(near_mat(&E, &expect, EPSILON * VM_F(8.0)));
}

TEST_CASE(strain_almansi_uniaxial, "[strain]") {
    /* F = diag(2, 1, 1) → b = diag(4, 1, 1) → b^{-1} = diag(0.25, 1, 1)
       e = 1/2 (I - b^{-1}) = diag(0.375, 0, 0) */
    const matrix3 F = diag3(VM_F(2.0), VM_F(1.0), VM_F(1.0));
    matrix3 e;
    REQUIRE(vm_strain_almansi(&e, &F));
    const matrix3 expect = diag3(VM_F(0.375), VM_F(0.0), VM_F(0.0));
    REQUIRE(near_mat(&e, &expect, EPSILON * VM_F(8.0)));
}

TEST_CASE(strain_log_from_grad_matches_polar, "[strain]") {
    const matrix3 F = diag3(VM_F(1.5), VM_F(0.8), VM_F(1.2));
    matrix3 log_e;
    REQUIRE(vm_strain_logarithmic_from_grad(&log_e, &F));
    const matrix3 expect = diag3(VECMAT_LOG(VM_F(1.5)),
                                 VECMAT_LOG(VM_F(0.8)),
                                 VECMAT_LOG(VM_F(1.2)));
    REQUIRE(near_mat(&log_e, &expect, EPSILON * VM_F(32.0)));
}

TEST_CASE(strain_log_rejects_nonpositive, "[strain]") {
    const matrix3 U = diag3(VM_F(1.0), VM_F(0.0), VM_F(1.0));
    matrix3 log_e;
    REQUIRE(!vm_strain_logarithmic(&log_e, &U));
}

TEST_CASE(strain_rotation_to_quat_proper, "[strain]") {
    const matrix3 R = mat3_rotation_y(VM_F(0.5));
    quaternion q;
    REQUIRE(vm_strain_rotation_to_quat(&q, &R));
    const quaternion expect = quat_from_mat3(R);
    REQUIRE(VECMAT_EQ(q.w, expect.w, EPSILON));
    REQUIRE(VECMAT_EQ(q.x, expect.x, EPSILON));
    REQUIRE(VECMAT_EQ(q.y, expect.y, EPSILON));
    REQUIRE(VECMAT_EQ(q.z, expect.z, EPSILON));
}

TEST_CASE(strain_rotation_to_quat_rejects_improper, "[strain]") {
    /* Reflection: diag(1, 1, -1) */
    const matrix3 R = diag3(VM_F(1.0), VM_F(1.0), VM_F(-1.0));
    quaternion q;
    REQUIRE(!vm_strain_rotation_to_quat(&q, &R));
}
