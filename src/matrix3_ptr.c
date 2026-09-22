// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

void mat3_identity_ptr(matrix3 *res)
{
    res->v[0] = 1.0f;
    res->v[1] = 0.0f;
    res->v[2] = 0.0f;
    res->v[3] = 0.0f;
    res->v[4] = 1.0f;
    res->v[5] = 0.0f;
    res->v[6] = 0.0f;
    res->v[7] = 0.0f;
    res->v[8] = 1.0f;
}

void mat3_mul_ptr(matrix3 *res, const matrix3 *a, const matrix3 *b)
{
    matrix3 temp;
    temp.m11 = a->m11 * b->m11 + a->m12 * b->m21 + a->m13 * b->m31;
    temp.m12 = a->m11 * b->m12 + a->m12 * b->m22 + a->m13 * b->m32;
    temp.m13 = a->m11 * b->m13 + a->m12 * b->m23 + a->m13 * b->m33;

    temp.m21 = a->m21 * b->m11 + a->m22 * b->m21 + a->m23 * b->m31;
    temp.m22 = a->m21 * b->m12 + a->m22 * b->m22 + a->m23 * b->m32;
    temp.m23 = a->m21 * b->m13 + a->m22 * b->m23 + a->m23 * b->m33;

    temp.m31 = a->m31 * b->m11 + a->m32 * b->m21 + a->m33 * b->m31;
    temp.m32 = a->m31 * b->m12 + a->m32 * b->m22 + a->m33 * b->m32;
    temp.m33 = a->m31 * b->m13 + a->m32 * b->m23 + a->m33 * b->m33;

    *res = temp;
}

void mat3_transpose_ptr(matrix3 *res, const matrix3 *m)
{
    matrix3 temp;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            temp.v[i * 3 + j] = m->v[j * 3 + i];
        }
    }
    *res = temp;
}

void mat3_inverse_ptr(matrix3 *res, const matrix3 *m)
{
    // Compute determinant
    const vm_float_t det = m->v[0] * (m->v[4] * m->v[8] - m->v[5] * m->v[7]) -
                           m->v[1] * (m->v[3] * m->v[8] - m->v[5] * m->v[6]) +
                           m->v[2] * (m->v[3] * m->v[7] - m->v[4] * m->v[6]);

    if (det == 0.0f) {
        mat3_identity_ptr(res);
        return;
    }

    const vm_float_t inv_det = 1.0f / det;

    matrix3 inv;
    inv.v[0] = (m->v[4] * m->v[8] - m->v[5] * m->v[7]) * inv_det;
    inv.v[1] = (m->v[2] * m->v[7] - m->v[1] * m->v[8]) * inv_det;
    inv.v[2] = (m->v[1] * m->v[5] - m->v[2] * m->v[4]) * inv_det;
    inv.v[3] = (m->v[5] * m->v[6] - m->v[3] * m->v[8]) * inv_det;
    inv.v[4] = (m->v[0] * m->v[8] - m->v[2] * m->v[6]) * inv_det;
    inv.v[5] = (m->v[2] * m->v[3] - m->v[0] * m->v[5]) * inv_det;
    inv.v[6] = (m->v[3] * m->v[7] - m->v[4] * m->v[6]) * inv_det;
    inv.v[7] = (m->v[1] * m->v[6] - m->v[0] * m->v[7]) * inv_det;
    inv.v[8] = (m->v[0] * m->v[4] - m->v[1] * m->v[3]) * inv_det;
    *res = inv;
}

void mat3_rotation_z_ptr(matrix3 *res, const vm_float_t radians)
{
    const vm_float_t c = VECMAT_COS(radians);
    const vm_float_t s = VECMAT_SIN(radians);

    res->m11 = c;
    res->m12 = -s;
    res->m13 = 0.0f;

    res->m21 = s;
    res->m22 = c;
    res->m23 = 0.0f;

    res->m31 = 0.0f;
    res->m32 = 0.0f;
    res->m33 = 1.0f;
}

void mat3_rotation_z_deg_ptr(matrix3 *res, const vm_float_t degrees)
{
    mat3_rotation_z_ptr(res, deg_to_rad(degrees));
}

void mat3_rotation_x_ptr(matrix3 *res, const vm_float_t radians)
{
    const vm_float_t c = VECMAT_COS(radians);
    const vm_float_t s = VECMAT_SIN(radians);
    *res = (matrix3){
        .m11 = 1.0f, .m21 = 0.0f, .m31 = 0.0f,
        .m12 = 0.0f, .m22 = c,    .m32 = s,
        .m13 = 0.0f, .m23 = -s,   .m33 = c
    };
}

void mat3_rotation_x_deg_ptr(matrix3 *res, const vm_float_t degrees)
{
    mat3_rotation_x_ptr(res, deg_to_rad(degrees));
}

void mat3_rotation_y_ptr(matrix3 *res, const vm_float_t radians)
{
    const vm_float_t c = VECMAT_COS(radians);
    const vm_float_t s = VECMAT_SIN(radians);
    *res = (matrix3){
        .m11 = c,    .m21 = 0.0f, .m31 = -s,
        .m12 = 0.0f, .m22 = 1.0f, .m32 = 0.0f,
        .m13 = s,    .m23 = 0.0f, .m33 = c
    };
}

void mat3_rotation_y_deg_ptr(matrix3 *res, const vm_float_t degrees)
{
    mat3_rotation_y_ptr(res, deg_to_rad(degrees));
}

void mat3_translate_ptr(matrix3 *res, const vector2 *t)
{
    mat3_identity_ptr(res);
    res->m13 = t->x;
    res->m23 = t->y;
}

void mat3_scale_ptr(matrix3 *res, const vector2 *s)
{
    mat3_identity_ptr(res);
    res->m11 = s->x;
    res->m22 = s->y;
}

void mat3_from_mat4_ptr(matrix3 *res, const matrix4 *m)
{
    res->m11 = m->m11; res->m21 = m->m21; res->m31 = m->m31;
    res->m12 = m->m12; res->m22 = m->m22; res->m32 = m->m32;
    res->m13 = m->m13; res->m23 = m->m23; res->m33 = m->m33;
}

void mat3_normal_ptr(matrix3 *res, const matrix3 *m)
{
    matrix3 inv;
    mat3_inverse_ptr(&inv, m);
    mat3_transpose_ptr(res, &inv);
}

void mat3_mul_vec3_ptr(vector3 *res, const matrix3 *m, const vector3 *v)
{
    const vm_float_t x = v->x;
    const vm_float_t y = v->y;
    const vm_float_t z = v->z;
    res->x = m->m11 * x + m->m12 * y + m->m13 * z;
    res->y = m->m21 * x + m->m22 * y + m->m23 * z;
    res->z = m->m31 * x + m->m32 * y + m->m33 * z;
}

void mat3_mul_vec2_ptr(vector2 *res, const matrix3 *m, const vector2 *v)
{
    const vm_float_t x = v->x;
    const vm_float_t y = v->y;
    res->x = m->m11 * x + m->m12 * y + m->m13;
    res->y = m->m21 * x + m->m22 * y + m->m23;
}

/**
 * @brief Performs one Jacobi rotation step for symmetric matrix eigenvalue decomposition.
 *
 * Rotates the matrix `a` to eliminate the off-diagonal element at (p,q) using a Givens rotation
 * if its magnitude exceeds the given tolerance. The accumulated eigenvectors in `v` are updated
 * with the same rotation.
 *
 * @param a Pointer to the symmetric matrix being diagonalized (modified in-place).
 * @param v Pointer to the matrix of accumulated eigenvectors (modified in-place).
 * @param p First rotation index (0-2).
 * @param q Second rotation index (0-2).
 * @param tol Threshold below which the off-diagonal element is considered zero.
 */
static void vm_mat3_jacobi_rotate(matrix3 *a, matrix3 *v, const int p, const int q, const vm_float_t tol)
{
    const vm_float_t apq = a->v[p + q * 3];
    const vm_float_t app = a->v[p + p * 3];
    const vm_float_t aqq = a->v[q + q * 3];
    const vm_float_t pair = VECMAT_FABS(app) + VECMAT_FABS(aqq);
    if (VECMAT_FABS(apq) <= tol * (VM_F(1.0) + pair)) {
        return;
    }

    const vm_float_t tau = (aqq - app) / (VM_F(2.0) * apq);
    const vm_float_t t = (tau >= 0)
        ? VM_F(1.0) / (tau + VECMAT_SQRT(VM_F(1.0) + tau * tau))
        : VM_F(-1.0) / (-tau + VECMAT_SQRT(VM_F(1.0) + tau * tau));
    const vm_float_t c = VM_F(1.0) / VECMAT_SQRT(VM_F(1.0) + t * t);
    const vm_float_t s = t * c;

    for (int i = 0; i < 3; ++i) {
        if (i == p || i == q) {
            continue;
        }
        const vm_float_t aip = a->v[i + p * 3];
        const vm_float_t aiq = a->v[i + q * 3];
        const vm_float_t np = c * aip - s * aiq;
        const vm_float_t nq = s * aip + c * aiq;
        a->v[i + p * 3] = a->v[p + i * 3] = np;
        a->v[i + q * 3] = a->v[q + i * 3] = nq;
    }

    a->v[p + p * 3] = c * c * app - VM_F(2.0) * s * c * apq + s * s * aqq;
    a->v[q + q * 3] = s * s * app + VM_F(2.0) * s * c * apq + c * c * aqq;
    a->v[p + q * 3] = a->v[q + p * 3] = VM_F(0.0);

    for (int i = 0; i < 3; ++i) {
        const vm_float_t vip = v->v[i + p * 3];
        const vm_float_t viq = v->v[i + q * 3];
        v->v[i + p * 3] = c * vip - s * viq;
        v->v[i + q * 3] = s * vip + c * viq;
    }
}

void mat3_sym_eigen_ptr(vector3 *eigenvalues, matrix3 *axes, const matrix3 *m)
{
    matrix3 a;
    a.m11 = m->m11;
    a.m22 = m->m22;
    a.m33 = m->m33;
    a.m21 = a.m12 = VM_F(0.5) * (m->m21 + m->m12);
    a.m31 = a.m13 = VM_F(0.5) * (m->m31 + m->m13);
    a.m32 = a.m23 = VM_F(0.5) * (m->m32 + m->m23);

    mat3_identity_ptr(axes);

    vm_float_t scale = VECMAT_FABS(a.m11);
    scale = VECMAT_FMAX(scale, VECMAT_FABS(a.m22));
    scale = VECMAT_FMAX(scale, VECMAT_FABS(a.m33));
    scale = VECMAT_FMAX(scale, VECMAT_FABS(a.m12));
    scale = VECMAT_FMAX(scale, VECMAT_FABS(a.m13));
    scale = VECMAT_FMAX(scale, VECMAT_FABS(a.m23));
    const vm_float_t tol = (scale > VM_F(0.0))
        ? VECMAT_EPSILON * VM_F(3.0) * scale
        : VECMAT_EPSILON;

    for (int iter = 0; iter < 32; ++iter) {
        const vm_float_t off = VECMAT_FABS(a.m12) + VECMAT_FABS(a.m13) + VECMAT_FABS(a.m23);
        if (off <= tol) {
            break;
        }
        vm_mat3_jacobi_rotate(&a, axes, 0, 1, tol);
        vm_mat3_jacobi_rotate(&a, axes, 0, 2, tol);
        vm_mat3_jacobi_rotate(&a, axes, 1, 2, tol);
    }

    eigenvalues->x = a.m11;
    eigenvalues->y = a.m22;
    eigenvalues->z = a.m33;
}
