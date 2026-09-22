// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

void mat2_identity_ptr(matrix2 *res)
{
    *res = (matrix2){0};
    res->v[0] = 1.0f;
    res->v[3] = 1.0f;
}

void mat2_mul_ptr(matrix2 *res, const matrix2 *a, const matrix2 *b)
{
    const matrix2 tmp = {
        .m11 = a->m11 * b->m11 + a->m12 * b->m21,
        .m21 = a->m21 * b->m11 + a->m22 * b->m21,
        .m12 = a->m11 * b->m12 + a->m12 * b->m22,
        .m22 = a->m21 * b->m12 + a->m22 * b->m22
    };
    *res = tmp;
}

void mat2_transpose_ptr(matrix2 *res, const matrix2 *m)
{
    *res = (matrix2){
        .m11 = m->v[0], .m21 = m->v[2],
        .m12 = m->v[1], .m22 = m->v[3]
    };
}

void mat2_inverse_ptr(matrix2 *res, const matrix2 *m)
{
    const vm_float_t det = mat2_determinant(*m);
    if (det == 0.0f) {
        mat2_identity_ptr(res);
        return;
    }
    const vm_float_t inv_det = 1.0f / det;
    *res = (matrix2){
        .m11 =  m->v[3] * inv_det, .m21 = -m->v[1] * inv_det,
        .m12 = -m->v[2] * inv_det, .m22 =  m->v[0] * inv_det
    };
}

void mat2_mul_vec2_ptr(vector2 *res, const matrix2 *m, const vector2 *v)
{
    const vm_float_t x = v->x;
    const vm_float_t y = v->y;
    res->x = m->m11 * x + m->m12 * y;
    res->y = m->m21 * x + m->m22 * y;
}

void mat2_rotation_z_ptr(matrix2 *res, const vm_float_t radians)
{
    const vm_float_t cos_theta = VECMAT_COS(radians);
    const vm_float_t sin_theta = VECMAT_SIN(radians);
    *res = (matrix2){
        .m11 = cos_theta,  .m21 = sin_theta,
        .m12 = -sin_theta, .m22 = cos_theta
    };
}

void mat2_rotation_z_deg_ptr(matrix2 *res, const vm_float_t degrees)
{
    mat2_rotation_z_ptr(res, deg_to_rad(degrees));
}

void mat2_scale_ptr(matrix2 *res, const vector2 *s)
{
    *res = (matrix2){
        .m11 = s->x, .m21 = 0.0f,
        .m12 = 0.0f, .m22 = s->y
    };
}

void mat2_from_mat3_ptr(matrix2 *res, const matrix3 *m)
{
    *res = (matrix2){
        .m11 = m->m11, .m21 = m->m21,
        .m12 = m->m12, .m22 = m->m22
    };
}
