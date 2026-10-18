// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "vecmat/strain.h"

matrix3 vm_strain_cauchy_green(const matrix3 *F)
{
    if (!F) {
        return mat3_identity();
    }

    const matrix3 Ft = mat3_transpose(*F);
    return mat3_mul(Ft, *F);
}

matrix3 vm_strain_green_lagrange(const matrix3 *F)
{
    matrix3 E = {0};

    if (!F) {
        return E;
    }

    const matrix3 C = vm_strain_cauchy_green(F);
    const matrix3 I = mat3_identity();
    for (int i = 0; i < 9; ++i) {
        E.v[i] = VM_F(0.5) * (C.v[i] - I.v[i]);
    }

    return E;
}

bool vm_strain_almansi(matrix3 *e, const matrix3 *F)
{
    if (!e || !F) {
        return false;
    }

    const matrix3 Ft = mat3_transpose(*F);
    const matrix3 b = mat3_mul(*F, Ft);
    matrix3 b_inv;
    if (!mat3_inverse_ok(&b_inv, &b)) {
        return false;
    }

    const matrix3 I = mat3_identity();
    for (int i = 0; i < 9; ++i) {
        e->v[i] = VM_F(0.5) * (I.v[i] - b_inv.v[i]);
    }

    return true;
}

bool vm_strain_logarithmic(matrix3 *log_e, const matrix3 *U)
{
    if (!log_e || !U) {
        return false;
    }

    matrix3 axes;
    const vector3 lam = mat3_sym_eigen(*U, &axes);
    if (lam.x <= VM_F(0.0) || lam.y <= VM_F(0.0) || lam.z <= VM_F(0.0)) {
        return false;
    }

    const matrix3 D = {
        .m11 = VECMAT_LOG(lam.x),
        .m22 = VECMAT_LOG(lam.y),
        .m33 = VECMAT_LOG(lam.z)
        /* remaining members zero-initialized */
    };

    const matrix3 axes_t = mat3_transpose(axes);
    const matrix3 DA = mat3_mul(D, axes_t);
    *log_e = mat3_mul(axes, DA);

    return true;
}

bool vm_strain_logarithmic_from_grad(matrix3 *log_e, const matrix3 *F)
{
    if (!log_e || !F) {
        return false;
    }

    matrix3 R, U;
    if (!mat3_polar(&R, &U, F)) {
        return false;
    }

    return vm_strain_logarithmic(log_e, &U);
}

bool vm_strain_rotation_to_quat(quaternion *q, const matrix3 *R)
{
    if (!q || !R) {
        return false;
    }

    /* Improper (reflection) is a failure, not a silent flip. */
    if (mat3_determinant(*R) <= VM_F(0.0)) {
        return false;
    }
    *q = quat_from_mat3(*R);

    return true;
}
