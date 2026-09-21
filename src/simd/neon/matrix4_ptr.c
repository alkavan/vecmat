// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <arm_neon.h>
#include <string.h>
#include <vecmat.h>

#if defined(VECMAT_USE_F64)

void mat4_mul_ptr_neon(matrix4 *res, const matrix4 *a, const matrix4 *b)
{
    matrix4 tmp;
    const float64x2_t a0l = vld1q_f64(&a->v[0]);
    const float64x2_t a0h = vld1q_f64(&a->v[2]);
    const float64x2_t a1l = vld1q_f64(&a->v[4]);
    const float64x2_t a1h = vld1q_f64(&a->v[6]);
    const float64x2_t a2l = vld1q_f64(&a->v[8]);
    const float64x2_t a2h = vld1q_f64(&a->v[10]);
    const float64x2_t a3l = vld1q_f64(&a->v[12]);
    const float64x2_t a3h = vld1q_f64(&a->v[14]);
    for (int c = 0; c < 4; c++) {
        float64x2_t col_l = vmulq_n_f64(a0l, b->v[c * 4 + 0]);
        float64x2_t col_h = vmulq_n_f64(a0h, b->v[c * 4 + 0]);
        col_l = vfmaq_n_f64(col_l, a1l, b->v[c * 4 + 1]);
        col_h = vfmaq_n_f64(col_h, a1h, b->v[c * 4 + 1]);
        col_l = vfmaq_n_f64(col_l, a2l, b->v[c * 4 + 2]);
        col_h = vfmaq_n_f64(col_h, a2h, b->v[c * 4 + 2]);
        col_l = vfmaq_n_f64(col_l, a3l, b->v[c * 4 + 3]);
        col_h = vfmaq_n_f64(col_h, a3h, b->v[c * 4 + 3]);
        vst1q_f64(&tmp.v[c * 4 + 0], col_l);
        vst1q_f64(&tmp.v[c * 4 + 2], col_h);
    }
    memcpy(res->v, tmp.v, sizeof(tmp.v));
}

void mat4_transpose_ptr_neon(matrix4 *res, const matrix4 *m)
{
    /* Column-major 4x4: treat stored columns as 2x2 blocks of f64. */
    float64x2_t c0l = vld1q_f64(&m->v[0]);
    float64x2_t c0h = vld1q_f64(&m->v[2]);
    float64x2_t c1l = vld1q_f64(&m->v[4]);
    float64x2_t c1h = vld1q_f64(&m->v[6]);
    float64x2_t c2l = vld1q_f64(&m->v[8]);
    float64x2_t c2h = vld1q_f64(&m->v[10]);
    float64x2_t c3l = vld1q_f64(&m->v[12]);
    float64x2_t c3h = vld1q_f64(&m->v[14]);

    const float64x2_t t0 = vtrn1q_f64(c0l, c1l);
    const float64x2_t t1 = vtrn2q_f64(c0l, c1l);
    const float64x2_t t2 = vtrn1q_f64(c2l, c3l);
    const float64x2_t t3 = vtrn2q_f64(c2l, c3l);
    const float64x2_t t4 = vtrn1q_f64(c0h, c1h);
    const float64x2_t t5 = vtrn2q_f64(c0h, c1h);
    const float64x2_t t6 = vtrn1q_f64(c2h, c3h);
    const float64x2_t t7 = vtrn2q_f64(c2h, c3h);

    matrix4 tmp;
    vst1q_f64(&tmp.v[0], t0);
    vst1q_f64(&tmp.v[2], t2);
    vst1q_f64(&tmp.v[4], t1);
    vst1q_f64(&tmp.v[6], t3);
    vst1q_f64(&tmp.v[8], t4);
    vst1q_f64(&tmp.v[10], t6);
    vst1q_f64(&tmp.v[12], t5);
    vst1q_f64(&tmp.v[14], t7);
    memcpy(res->v, tmp.v, sizeof(tmp.v));
}

void mat4_mul_vec4_ptr_neon(vector4 *res, const matrix4 *m, const vector4 *v)
{
    float64x2_t rl = vmulq_n_f64(vld1q_f64(&m->v[0]), v->x);
    float64x2_t rh = vmulq_n_f64(vld1q_f64(&m->v[2]), v->x);
    rl = vfmaq_n_f64(rl, vld1q_f64(&m->v[4]), v->y);
    rh = vfmaq_n_f64(rh, vld1q_f64(&m->v[6]), v->y);
    rl = vfmaq_n_f64(rl, vld1q_f64(&m->v[8]), v->z);
    rh = vfmaq_n_f64(rh, vld1q_f64(&m->v[10]), v->z);
    rl = vfmaq_n_f64(rl, vld1q_f64(&m->v[12]), v->w);
    rh = vfmaq_n_f64(rh, vld1q_f64(&m->v[14]), v->w);
    vst1q_f64(&res->v[0], rl);
    vst1q_f64(&res->v[2], rh);
}

void mat4_mul_vec3_ptr_neon(vector3 *res, const matrix4 *m, const vector3 *v, const vm_float_t w)
{
    float64x2_t rl = vmulq_n_f64(vld1q_f64(&m->v[0]), v->x);
    float64x2_t rh = vmulq_n_f64(vld1q_f64(&m->v[2]), v->x);
    rl = vfmaq_n_f64(rl, vld1q_f64(&m->v[4]), v->y);
    rh = vfmaq_n_f64(rh, vld1q_f64(&m->v[6]), v->y);
    rl = vfmaq_n_f64(rl, vld1q_f64(&m->v[8]), v->z);
    rh = vfmaq_n_f64(rh, vld1q_f64(&m->v[10]), v->z);
    rl = vfmaq_n_f64(rl, vld1q_f64(&m->v[12]), w);
    rh = vfmaq_n_f64(rh, vld1q_f64(&m->v[14]), w);
    double t[4];
    vst1q_f64(t + 0, rl);
    vst1q_f64(t + 2, rh);
    res->x = t[0];
    res->y = t[1];
    res->z = t[2];
}

#else /* float */

void mat4_mul_ptr_neon(matrix4 *res, const matrix4 *a, const matrix4 *b)
{
    matrix4 tmp;
    const float32x4_t a0 = vld1q_f32(&a->v[0]);
    const float32x4_t a1 = vld1q_f32(&a->v[4]);
    const float32x4_t a2 = vld1q_f32(&a->v[8]);
    const float32x4_t a3 = vld1q_f32(&a->v[12]);
    for (int c = 0; c < 4; c++) {
        float32x4_t col = vmulq_n_f32(a0, b->v[c * 4 + 0]);
        col = vfmaq_n_f32(col, a1, b->v[c * 4 + 1]);
        col = vfmaq_n_f32(col, a2, b->v[c * 4 + 2]);
        col = vfmaq_n_f32(col, a3, b->v[c * 4 + 3]);
        vst1q_f32(&tmp.v[c * 4], col);
    }
    memcpy(res->v, tmp.v, sizeof(tmp.v));
}

void mat4_transpose_ptr_neon(matrix4 *res, const matrix4 *m)
{
    float32x4_t c0 = vld1q_f32(&m->v[0]);
    float32x4_t c1 = vld1q_f32(&m->v[4]);
    float32x4_t c2 = vld1q_f32(&m->v[8]);
    float32x4_t c3 = vld1q_f32(&m->v[12]);

    const float32x4x2_t t01 = vtrnq_f32(c0, c1);
    const float32x4x2_t t23 = vtrnq_f32(c2, c3);
    const float32x4_t r0 = vcombine_f32(vget_low_f32(t01.val[0]), vget_low_f32(t23.val[0]));
    const float32x4_t r1 = vcombine_f32(vget_low_f32(t01.val[1]), vget_low_f32(t23.val[1]));
    const float32x4_t r2 = vcombine_f32(vget_high_f32(t01.val[0]), vget_high_f32(t23.val[0]));
    const float32x4_t r3 = vcombine_f32(vget_high_f32(t01.val[1]), vget_high_f32(t23.val[1]));

    matrix4 tmp;
    vst1q_f32(&tmp.v[0], r0);
    vst1q_f32(&tmp.v[4], r1);
    vst1q_f32(&tmp.v[8], r2);
    vst1q_f32(&tmp.v[12], r3);
    memcpy(res->v, tmp.v, sizeof(tmp.v));
}

void mat4_mul_vec4_ptr_neon(vector4 *res, const matrix4 *m, const vector4 *v)
{
    float32x4_t r = vmulq_n_f32(vld1q_f32(&m->v[0]), v->x);
    r = vfmaq_n_f32(r, vld1q_f32(&m->v[4]), v->y);
    r = vfmaq_n_f32(r, vld1q_f32(&m->v[8]), v->z);
    r = vfmaq_n_f32(r, vld1q_f32(&m->v[12]), v->w);
    vst1q_f32(res->v, r);
}

void mat4_mul_vec3_ptr_neon(vector3 *res, const matrix4 *m, const vector3 *v, const vm_float_t w)
{
    float32x4_t r = vmulq_n_f32(vld1q_f32(&m->v[0]), v->x);
    r = vfmaq_n_f32(r, vld1q_f32(&m->v[4]), v->y);
    r = vfmaq_n_f32(r, vld1q_f32(&m->v[8]), v->z);
    r = vfmaq_n_f32(r, vld1q_f32(&m->v[12]), w);
    float t[4];
    vst1q_f32(t, r);
    res->x = t[0];
    res->y = t[1];
    res->z = t[2];
}

#endif
