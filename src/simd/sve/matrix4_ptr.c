// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <arm_sve.h>
#include <string.h>
#include <vecmat.h>

#if defined(VECMAT_USE_F64)

void mat4_mul_ptr_sve(matrix4 *res, const matrix4 *a, const matrix4 *b)
{
    const svbool_t pg = svwhilelt_b64((uint64_t)0, (uint64_t)4);
    matrix4 tmp;
    const svfloat64_t a0 = svld1_f64(pg, &a->v[0]);
    const svfloat64_t a1 = svld1_f64(pg, &a->v[4]);
    const svfloat64_t a2 = svld1_f64(pg, &a->v[8]);
    const svfloat64_t a3 = svld1_f64(pg, &a->v[12]);
    for (int c = 0; c < 4; c++) {
        svfloat64_t col = svmul_n_f64_z(pg, a0, b->v[c * 4 + 0]);
        col = svmla_n_f64_z(pg, col, a1, b->v[c * 4 + 1]);
        col = svmla_n_f64_z(pg, col, a2, b->v[c * 4 + 2]);
        col = svmla_n_f64_z(pg, col, a3, b->v[c * 4 + 3]);
        svst1_f64(pg, &tmp.v[c * 4], col);
    }
    memcpy(res->v, tmp.v, sizeof(tmp.v));
}

void mat4_transpose_ptr_sve(matrix4 *res, const matrix4 *m)
{
    matrix4 tmp;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++)
            tmp.v[i * 4 + j] = m->v[j * 4 + i];
    }
    memcpy(res->v, tmp.v, sizeof(tmp.v));
}

void mat4_mul_vec4_ptr_sve(vector4 *res, const matrix4 *m, const vector4 *v)
{
    const svbool_t pg = svwhilelt_b64((uint64_t)0, (uint64_t)4);
    svfloat64_t r = svmul_n_f64_z(pg, svld1_f64(pg, &m->v[0]), v->x);
    r = svmla_n_f64_z(pg, r, svld1_f64(pg, &m->v[4]), v->y);
    r = svmla_n_f64_z(pg, r, svld1_f64(pg, &m->v[8]), v->z);
    r = svmla_n_f64_z(pg, r, svld1_f64(pg, &m->v[12]), v->w);
    svst1_f64(pg, res->v, r);
}

void mat4_mul_vec3_ptr_sve(vector3 *res, const matrix4 *m, const vector3 *v, vm_float_t w)
{
    const svbool_t pg = svwhilelt_b64((uint64_t)0, (uint64_t)4);
    svfloat64_t r = svmul_n_f64_z(pg, svld1_f64(pg, &m->v[0]), v->x);
    r = svmla_n_f64_z(pg, r, svld1_f64(pg, &m->v[4]), v->y);
    r = svmla_n_f64_z(pg, r, svld1_f64(pg, &m->v[8]), v->z);
    r = svmla_n_f64_z(pg, r, svld1_f64(pg, &m->v[12]), w);
    double t[4];
    svst1_f64(pg, t, r);
    res->x = t[0];
    res->y = t[1];
    res->z = t[2];
}

#else /* float */

void mat4_mul_ptr_sve(matrix4 *res, const matrix4 *a, const matrix4 *b)
{
    const svbool_t pg = svwhilelt_b32((uint32_t)0, (uint32_t)4);
    matrix4 tmp;
    const svfloat32_t a0 = svld1_f32(pg, &a->v[0]);
    const svfloat32_t a1 = svld1_f32(pg, &a->v[4]);
    const svfloat32_t a2 = svld1_f32(pg, &a->v[8]);
    const svfloat32_t a3 = svld1_f32(pg, &a->v[12]);
    for (int c = 0; c < 4; c++) {
        svfloat32_t col = svmul_n_f32_z(pg, a0, b->v[c * 4 + 0]);
        col = svmla_n_f32_z(pg, col, a1, b->v[c * 4 + 1]);
        col = svmla_n_f32_z(pg, col, a2, b->v[c * 4 + 2]);
        col = svmla_n_f32_z(pg, col, a3, b->v[c * 4 + 3]);
        svst1_f32(pg, &tmp.v[c * 4], col);
    }
    memcpy(res->v, tmp.v, sizeof(tmp.v));
}

void mat4_transpose_ptr_sve(matrix4 *res, const matrix4 *m)
{
    matrix4 tmp;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++)
            tmp.v[i * 4 + j] = m->v[j * 4 + i];
    }
    memcpy(res->v, tmp.v, sizeof(tmp.v));
}

void mat4_mul_vec4_ptr_sve(vector4 *res, const matrix4 *m, const vector4 *v)
{
    const svbool_t pg = svwhilelt_b32((uint32_t)0, (uint32_t)4);
    svfloat32_t r = svmul_n_f32_z(pg, svld1_f32(pg, &m->v[0]), v->x);
    r = svmla_n_f32_z(pg, r, svld1_f32(pg, &m->v[4]), v->y);
    r = svmla_n_f32_z(pg, r, svld1_f32(pg, &m->v[8]), v->z);
    r = svmla_n_f32_z(pg, r, svld1_f32(pg, &m->v[12]), v->w);
    svst1_f32(pg, res->v, r);
}

void mat4_mul_vec3_ptr_sve(vector3 *res, const matrix4 *m, const vector3 *v, vm_float_t w)
{
    const svbool_t pg = svwhilelt_b32((uint32_t)0, (uint32_t)4);
    svfloat32_t r = svmul_n_f32_z(pg, svld1_f32(pg, &m->v[0]), v->x);
    r = svmla_n_f32_z(pg, r, svld1_f32(pg, &m->v[4]), v->y);
    r = svmla_n_f32_z(pg, r, svld1_f32(pg, &m->v[8]), v->z);
    r = svmla_n_f32_z(pg, r, svld1_f32(pg, &m->v[12]), w);
    float t[4];
    svst1_f32(pg, t, r);
    res->x = t[0];
    res->y = t[1];
    res->z = t[2];
}

#endif
