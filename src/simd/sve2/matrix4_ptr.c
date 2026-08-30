// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <arm_sve.h>
#include <string.h>
#include <vecmat.h>

#if defined(VECMAT_USE_F64)

void mat4_mul_ptr_sve2(matrix4 *res, const matrix4 *a, const matrix4 *b)
{
    const svbool_t pg = svwhilelt_b64((uint64_t)0, (uint64_t)4);
    matrix4 tmp;
    for (int i = 0; i < 4; i++) {
        svfloat64_t row = svdup_n_f64(0.0);
        for (int k = 0; k < 4; k++) {
            const svfloat64_t bk = svld1_f64(pg, &b->v[k * 4]);
            row = svmla_n_f64_z(pg, row, bk, a->v[i * 4 + k]);
        }
        svst1_f64(pg, &tmp.v[i * 4], row);
    }
    memcpy(res->v, tmp.v, sizeof(tmp.v));
}

void mat4_transpose_ptr_sve2(matrix4 *res, const matrix4 *m)
{
    const svbool_t pg = svwhilelt_b64((uint64_t)0, (uint64_t)4);
    const svuint64_t idx = svindex_u64(0, 4);
    matrix4 tmp;
    svst1_f64(pg, &tmp.v[0],  svld1_gather_u64index_f64(pg, &m->v[0], idx));
    svst1_f64(pg, &tmp.v[4],  svld1_gather_u64index_f64(pg, &m->v[1], idx));
    svst1_f64(pg, &tmp.v[8],  svld1_gather_u64index_f64(pg, &m->v[2], idx));
    svst1_f64(pg, &tmp.v[12], svld1_gather_u64index_f64(pg, &m->v[3], idx));
    memcpy(res->v, tmp.v, sizeof(tmp.v));
}

void mat4_mul_vec4_ptr_sve2(vector4 *res, const matrix4 *m, const vector4 *v)
{
    const svbool_t pg = svwhilelt_b64((uint64_t)0, (uint64_t)4);
    svfloat64_t r = svmul_n_f64_z(pg, svld1_f64(pg, &m->v[0]), v->x);
    r = svmla_n_f64_z(pg, r, svld1_f64(pg, &m->v[4]), v->y);
    r = svmla_n_f64_z(pg, r, svld1_f64(pg, &m->v[8]), v->z);
    r = svmla_n_f64_z(pg, r, svld1_f64(pg, &m->v[12]), v->w);
    svst1_f64(pg, res->v, r);
}

void mat4_mul_vec3_ptr_sve2(vector3 *res, const matrix4 *m, const vector3 *v, vm_float_t w)
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

void mat4_mul_ptr_sve2(matrix4 *res, const matrix4 *a, const matrix4 *b)
{
    const svbool_t pg = svwhilelt_b32((uint32_t)0, (uint32_t)4);
    matrix4 tmp;
    for (int i = 0; i < 4; i++) {
        svfloat32_t row = svdup_n_f32(0.0f);
        for (int k = 0; k < 4; k++) {
            const svfloat32_t bk = svld1_f32(pg, &b->v[k * 4]);
            row = svmla_n_f32_z(pg, row, bk, a->v[i * 4 + k]);
        }
        svst1_f32(pg, &tmp.v[i * 4], row);
    }
    memcpy(res->v, tmp.v, sizeof(tmp.v));
}

void mat4_transpose_ptr_sve2(matrix4 *res, const matrix4 *m)
{
    const svbool_t pg = svwhilelt_b32((uint32_t)0, (uint32_t)4);
    const svuint32_t idx = svindex_u32(0, 4);
    matrix4 tmp;
    svst1_f32(pg, &tmp.v[0],  svld1_gather_u32index_f32(pg, &m->v[0], idx));
    svst1_f32(pg, &tmp.v[4],  svld1_gather_u32index_f32(pg, &m->v[1], idx));
    svst1_f32(pg, &tmp.v[8],  svld1_gather_u32index_f32(pg, &m->v[2], idx));
    svst1_f32(pg, &tmp.v[12], svld1_gather_u32index_f32(pg, &m->v[3], idx));
    memcpy(res->v, tmp.v, sizeof(tmp.v));
}

void mat4_mul_vec4_ptr_sve2(vector4 *res, const matrix4 *m, const vector4 *v)
{
    const svbool_t pg = svwhilelt_b32((uint32_t)0, (uint32_t)4);
    svfloat32_t r = svmul_n_f32_z(pg, svld1_f32(pg, &m->v[0]), v->x);
    r = svmla_n_f32_z(pg, r, svld1_f32(pg, &m->v[4]), v->y);
    r = svmla_n_f32_z(pg, r, svld1_f32(pg, &m->v[8]), v->z);
    r = svmla_n_f32_z(pg, r, svld1_f32(pg, &m->v[12]), v->w);
    svst1_f32(pg, res->v, r);
}

void mat4_mul_vec3_ptr_sve2(vector3 *res, const matrix4 *m, const vector3 *v, vm_float_t w)
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
