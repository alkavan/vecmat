// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <arm_sve.h>
#include <math.h>
#include <vecmat.h>

#if defined(VECMAT_USE_F64)

static svbool_t pg4(void)
{
    return svwhilelt_b64((uint64_t)0, (uint64_t)4);
}

static svfloat64_t hadamard_div(svbool_t pg, svfloat64_t a, svfloat64_t b)
{
    const svfloat64_t zero = svdup_n_f64(0.0);
    const svbool_t eqz = svcmpeq_f64(pg, b, zero);
    const svfloat64_t safe = svsel_f64(eqz, svdup_n_f64(1.0), b);
    return svsel_f64(eqz, zero, svdiv_f64_z(pg, a, safe));
}

static svfloat64_t sign4(svbool_t pg, svfloat64_t x)
{
    const svfloat64_t zero = svdup_n_f64(0.0);
    const svfloat64_t one = svdup_n_f64(1.0);
    svfloat64_t r = svsel_f64(svcmpgt_f64(pg, x, zero), one, zero);
    return svsel_f64(svcmplt_f64(pg, x, zero), svneg_f64_z(pg, one), r);
}

void vec4_add_ptr_sve(vector4 *res, const vector4 *a, const vector4 *b)
{
    const svbool_t pg = pg4();
    svst1_f64(pg, res->v, svadd_f64_z(pg, svld1_f64(pg, a->v), svld1_f64(pg, b->v)));
}

void vec4_sub_ptr_sve(vector4 *res, const vector4 *a, const vector4 *b)
{
    const svbool_t pg = pg4();
    svst1_f64(pg, res->v, svsub_f64_z(pg, svld1_f64(pg, a->v), svld1_f64(pg, b->v)));
}

void vec4_mul_ptr_sve(vector4 *res, const vector4 *a, const vector4 *b)
{
    const svbool_t pg = pg4();
    svst1_f64(pg, res->v, svmul_f64_z(pg, svld1_f64(pg, a->v), svld1_f64(pg, b->v)));
}

void vec4_mul_scalar_ptr_sve(vector4 *res, const vector4 *v, vm_float_t s)
{
    const svbool_t pg = pg4();
    svst1_f64(pg, res->v, svmul_n_f64_z(pg, svld1_f64(pg, v->v), s));
}

void vec4_div_scalar_ptr_sve(vector4 *res, const vector4 *v, vm_float_t s)
{
    const svbool_t pg = pg4();
    if (s == 0.0) {
        svst1_f64(pg, res->v, svdup_n_f64(0.0));
        return;
    }
    svst1_f64(pg, res->v, svdiv_n_f64_z(pg, svld1_f64(pg, v->v), s));
}

void vec4_neg_ptr_sve(vector4 *res, const vector4 *v)
{
    const svbool_t pg = pg4();
    svst1_f64(pg, res->v, svneg_f64_z(pg, svld1_f64(pg, v->v)));
}

void vec4_abs_ptr_sve(vector4 *res, const vector4 *v)
{
    const svbool_t pg = pg4();
    svst1_f64(pg, res->v, svabs_f64_z(pg, svld1_f64(pg, v->v)));
}

void vec4_normalize_ptr_sve(vector4 *res, const vector4 *v)
{
    const svbool_t pg = pg4();
    const svfloat64_t x = svld1_f64(pg, v->v);
    const double len2 = svaddv_f64(pg, svmul_f64_z(pg, x, x));
    if (len2 == 0.0) {
        *res = *v;
        return;
    }
    const double inv = 1.0 / VECMAT_SQRT(len2);
    svst1_f64(pg, res->v, svmul_n_f64_z(pg, x, inv));
}

void vec4_min_ptr_sve(vector4 *res, const vector4 *a, const vector4 *b)
{
    const svbool_t pg = pg4();
    svst1_f64(pg, res->v, svmin_f64_z(pg, svld1_f64(pg, a->v), svld1_f64(pg, b->v)));
}

void vec4_max_ptr_sve(vector4 *res, const vector4 *a, const vector4 *b)
{
    const svbool_t pg = pg4();
    svst1_f64(pg, res->v, svmax_f64_z(pg, svld1_f64(pg, a->v), svld1_f64(pg, b->v)));
}

void vec4_lerp_ptr_sve(vector4 *res, const vector4 *a, const vector4 *b, vm_float_t t)
{
    const svbool_t pg = pg4();
    const svfloat64_t va = svld1_f64(pg, a->v);
    const svfloat64_t d = svsub_f64_z(pg, svld1_f64(pg, b->v), va);
    svst1_f64(pg, res->v, svmla_n_f64_z(pg, va, d, t));
}

void vec4_clamp_ptr_sve(vector4 *res, const vector4 *v,
                        const vector4 *min, const vector4 *max)
{
    const svbool_t pg = pg4();
    svfloat64_t x = svld1_f64(pg, v->v);
    x = svmax_f64_z(pg, svld1_f64(pg, min->v), x);
    x = svmin_f64_z(pg, svld1_f64(pg, max->v), x);
    svst1_f64(pg, res->v, x);
}

void vec4_div_ptr_sve(vector4 *res, const vector4 *a, const vector4 *b)
{
    const svbool_t pg = pg4();
    svst1_f64(pg, res->v, hadamard_div(pg, svld1_f64(pg, a->v), svld1_f64(pg, b->v)));
}

void vec4_add_scalar_ptr_sve(vector4 *res, const vector4 *v, vm_float_t s)
{
    const svbool_t pg = pg4();
    svst1_f64(pg, res->v, svadd_n_f64_z(pg, svld1_f64(pg, v->v), s));
}

void vec4_sub_scalar_ptr_sve(vector4 *res, const vector4 *v, vm_float_t s)
{
    const svbool_t pg = pg4();
    svst1_f64(pg, res->v, svsub_n_f64_z(pg, svld1_f64(pg, v->v), s));
}

void vec4_clamp_scalar_ptr_sve(vector4 *res, const vector4 *v, vm_float_t min, vm_float_t max)
{
    const svbool_t pg = pg4();
    svfloat64_t x = svmax_n_f64_z(pg, svld1_f64(pg, v->v), min);
    svst1_f64(pg, res->v, svmin_n_f64_z(pg, x, max));
}

void vec4_saturate_ptr_sve(vector4 *res, const vector4 *v)
{
    const svbool_t pg = pg4();
    svfloat64_t x = svmax_n_f64_z(pg, svld1_f64(pg, v->v), 0.0);
    svst1_f64(pg, res->v, svmin_n_f64_z(pg, x, 1.0));
}

void vec4_sign_ptr_sve(vector4 *res, const vector4 *v)
{
    const svbool_t pg = pg4();
    svst1_f64(pg, res->v, sign4(pg, svld1_f64(pg, v->v)));
}

void vec4_floor_ptr_sve(vector4 *res, const vector4 *v)
{
    const svbool_t pg = pg4();
    svst1_f64(pg, res->v, svrintm_f64_z(pg, svld1_f64(pg, v->v)));
}

void vec4_ceil_ptr_sve(vector4 *res, const vector4 *v)
{
    const svbool_t pg = pg4();
    svst1_f64(pg, res->v, svrintp_f64_z(pg, svld1_f64(pg, v->v)));
}

void vec4_round_ptr_sve(vector4 *res, const vector4 *v)
{
    const svbool_t pg = pg4();
    svst1_f64(pg, res->v, svrinta_f64_z(pg, svld1_f64(pg, v->v)));
}

void vec4_fract_ptr_sve(vector4 *res, const vector4 *v)
{
    const svbool_t pg = pg4();
    const svfloat64_t x = svld1_f64(pg, v->v);
    svst1_f64(pg, res->v, svsub_f64_z(pg, x, svrintm_f64_z(pg, x)));
}

void vec4_homogenize_ptr_sve(vector4 *res, const vector4 *v)
{
    const svbool_t pg = pg4();
    if (VECMAT_FABS(v->w) > VECMAT_EPSILON) {
        svst1_f64(pg, res->v, svmul_n_f64_z(pg, svld1_f64(pg, v->v), 1.0 / v->w));
        res->w = 1.0;
    } else {
        svst1_f64(pg, res->v, svdup_n_f64(0.0));
    }
}

static svfloat64_t quat_mul4(svbool_t pg, svfloat64_t a, svfloat64_t b)
{
    const uint64_t i1[4] = {3, 2, 1, 0};
    const uint64_t i2[4] = {2, 3, 0, 1};
    const uint64_t i3[4] = {1, 0, 3, 2};
    const svuint64_t lane = svindex_u64(0, 1);
    const svbool_t n13 = svorr_z(pg, svcmpeq_n_u64(pg, lane, 1), svcmpeq_n_u64(pg, lane, 3));
    const svbool_t n23 = svorr_z(pg, svcmpeq_n_u64(pg, lane, 2), svcmpeq_n_u64(pg, lane, 3));
    const svbool_t n03 = svorr_z(pg, svcmpeq_n_u64(pg, lane, 0), svcmpeq_n_u64(pg, lane, 3));

    svfloat64_t t1 = svtbl_f64(b, svld1_u64(pg, i1));
    svfloat64_t t2 = svtbl_f64(b, svld1_u64(pg, i2));
    svfloat64_t t3 = svtbl_f64(b, svld1_u64(pg, i3));
    t1 = svneg_f64_m(t1, n13, t1);
    t2 = svneg_f64_m(t2, n23, t2);
    t3 = svneg_f64_m(t3, n03, t3);

    double aa[4];
    svst1_f64(pg, aa, a);
    svfloat64_t r = svmul_n_f64_z(pg, b, aa[3]);
    r = svmla_n_f64_z(pg, r, t1, aa[0]);
    r = svmla_n_f64_z(pg, r, t2, aa[1]);
    r = svmla_n_f64_z(pg, r, t3, aa[2]);
    return r;
}

void quat_mul_ptr_sve(quaternion *res, const quaternion *a, const quaternion *b)
{
    const svbool_t pg = pg4();
    svst1_f64(pg, res->v, quat_mul4(pg, svld1_f64(pg, a->v), svld1_f64(pg, b->v)));
}

void quat_normalize_ptr_sve(quaternion *res, const quaternion *q)
{
    vec4_normalize_ptr_sve((vector4 *)res, (const vector4 *)q);
}

#else /* float */

static svbool_t pg4(void)
{
    return svwhilelt_b32((uint32_t)0, (uint32_t)4);
}

static svfloat32_t hadamard_div(svbool_t pg, svfloat32_t a, svfloat32_t b)
{
    const svfloat32_t zero = svdup_n_f32(0.0f);
    const svbool_t eqz = svcmpeq_f32(pg, b, zero);
    const svfloat32_t safe = svsel_f32(eqz, svdup_n_f32(1.0f), b);
    return svsel_f32(eqz, zero, svdiv_f32_z(pg, a, safe));
}

static svfloat32_t sign4(svbool_t pg, svfloat32_t x)
{
    const svfloat32_t zero = svdup_n_f32(0.0f);
    const svfloat32_t one = svdup_n_f32(1.0f);
    svfloat32_t r = svsel_f32(svcmpgt_f32(pg, x, zero), one, zero);
    return svsel_f32(svcmplt_f32(pg, x, zero), svneg_f32_z(pg, one), r);
}

void vec4_add_ptr_sve(vector4 *res, const vector4 *a, const vector4 *b)
{
    const svbool_t pg = pg4();
    svst1_f32(pg, res->v, svadd_f32_z(pg, svld1_f32(pg, a->v), svld1_f32(pg, b->v)));
}

void vec4_sub_ptr_sve(vector4 *res, const vector4 *a, const vector4 *b)
{
    const svbool_t pg = pg4();
    svst1_f32(pg, res->v, svsub_f32_z(pg, svld1_f32(pg, a->v), svld1_f32(pg, b->v)));
}

void vec4_mul_ptr_sve(vector4 *res, const vector4 *a, const vector4 *b)
{
    const svbool_t pg = pg4();
    svst1_f32(pg, res->v, svmul_f32_z(pg, svld1_f32(pg, a->v), svld1_f32(pg, b->v)));
}

void vec4_mul_scalar_ptr_sve(vector4 *res, const vector4 *v, vm_float_t s)
{
    const svbool_t pg = pg4();
    svst1_f32(pg, res->v, svmul_n_f32_z(pg, svld1_f32(pg, v->v), s));
}

void vec4_div_scalar_ptr_sve(vector4 *res, const vector4 *v, vm_float_t s)
{
    const svbool_t pg = pg4();
    if (s == 0.0f) {
        svst1_f32(pg, res->v, svdup_n_f32(0.0f));
        return;
    }
    svst1_f32(pg, res->v, svdiv_n_f32_z(pg, svld1_f32(pg, v->v), s));
}

void vec4_neg_ptr_sve(vector4 *res, const vector4 *v)
{
    const svbool_t pg = pg4();
    svst1_f32(pg, res->v, svneg_f32_z(pg, svld1_f32(pg, v->v)));
}

void vec4_abs_ptr_sve(vector4 *res, const vector4 *v)
{
    const svbool_t pg = pg4();
    svst1_f32(pg, res->v, svabs_f32_z(pg, svld1_f32(pg, v->v)));
}

void vec4_normalize_ptr_sve(vector4 *res, const vector4 *v)
{
    const svbool_t pg = pg4();
    const svfloat32_t x = svld1_f32(pg, v->v);
    const float len2 = svaddv_f32(pg, svmul_f32_z(pg, x, x));
    if (len2 == 0.0f) {
        *res = *v;
        return;
    }
    const float inv = 1.0f / VECMAT_SQRT(len2);
    svst1_f32(pg, res->v, svmul_n_f32_z(pg, x, inv));
}

void vec4_min_ptr_sve(vector4 *res, const vector4 *a, const vector4 *b)
{
    const svbool_t pg = pg4();
    svst1_f32(pg, res->v, svmin_f32_z(pg, svld1_f32(pg, a->v), svld1_f32(pg, b->v)));
}

void vec4_max_ptr_sve(vector4 *res, const vector4 *a, const vector4 *b)
{
    const svbool_t pg = pg4();
    svst1_f32(pg, res->v, svmax_f32_z(pg, svld1_f32(pg, a->v), svld1_f32(pg, b->v)));
}

void vec4_lerp_ptr_sve(vector4 *res, const vector4 *a, const vector4 *b, vm_float_t t)
{
    const svbool_t pg = pg4();
    const svfloat32_t va = svld1_f32(pg, a->v);
    const svfloat32_t d = svsub_f32_z(pg, svld1_f32(pg, b->v), va);
    svst1_f32(pg, res->v, svmla_n_f32_z(pg, va, d, t));
}

void vec4_clamp_ptr_sve(vector4 *res, const vector4 *v, const vector4 *min, const vector4 *max)
{
    const svbool_t pg = pg4();
    svfloat32_t x = svld1_f32(pg, v->v);
    x = svmax_f32_z(pg, svld1_f32(pg, min->v), x);
    x = svmin_f32_z(pg, svld1_f32(pg, max->v), x);
    svst1_f32(pg, res->v, x);
}

void vec4_div_ptr_sve(vector4 *res, const vector4 *a, const vector4 *b)
{
    const svbool_t pg = pg4();
    svst1_f32(pg, res->v, hadamard_div(pg, svld1_f32(pg, a->v), svld1_f32(pg, b->v)));
}

void vec4_add_scalar_ptr_sve(vector4 *res, const vector4 *v, vm_float_t s)
{
    const svbool_t pg = pg4();
    svst1_f32(pg, res->v, svadd_n_f32_z(pg, svld1_f32(pg, v->v), s));
}

void vec4_sub_scalar_ptr_sve(vector4 *res, const vector4 *v, vm_float_t s)
{
    const svbool_t pg = pg4();
    svst1_f32(pg, res->v, svsub_n_f32_z(pg, svld1_f32(pg, v->v), s));
}

void vec4_clamp_scalar_ptr_sve(vector4 *res, const vector4 *v, vm_float_t min, vm_float_t max)
{
    const svbool_t pg = pg4();
    svfloat32_t x = svmax_n_f32_z(pg, svld1_f32(pg, v->v), min);
    svst1_f32(pg, res->v, svmin_n_f32_z(pg, x, max));
}

void vec4_saturate_ptr_sve(vector4 *res, const vector4 *v)
{
    const svbool_t pg = pg4();
    svfloat32_t x = svmax_n_f32_z(pg, svld1_f32(pg, v->v), 0.0f);
    svst1_f32(pg, res->v, svmin_n_f32_z(pg, x, 1.0f));
}

void vec4_sign_ptr_sve(vector4 *res, const vector4 *v)
{
    const svbool_t pg = pg4();
    svst1_f32(pg, res->v, sign4(pg, svld1_f32(pg, v->v)));
}

void vec4_floor_ptr_sve(vector4 *res, const vector4 *v)
{
    const svbool_t pg = pg4();
    svst1_f32(pg, res->v, svrintm_f32_z(pg, svld1_f32(pg, v->v)));
}

void vec4_ceil_ptr_sve(vector4 *res, const vector4 *v)
{
    const svbool_t pg = pg4();
    svst1_f32(pg, res->v, svrintp_f32_z(pg, svld1_f32(pg, v->v)));
}

void vec4_round_ptr_sve(vector4 *res, const vector4 *v)
{
    const svbool_t pg = pg4();
    svst1_f32(pg, res->v, svrinta_f32_z(pg, svld1_f32(pg, v->v)));
}

void vec4_fract_ptr_sve(vector4 *res, const vector4 *v)
{
    const svbool_t pg = pg4();
    const svfloat32_t x = svld1_f32(pg, v->v);
    svst1_f32(pg, res->v, svsub_f32_z(pg, x, svrintm_f32_z(pg, x)));
}

void vec4_homogenize_ptr_sve(vector4 *res, const vector4 *v)
{
    const svbool_t pg = pg4();
    if (VECMAT_FABS(v->w) > VECMAT_EPSILON) {
        svst1_f32(pg, res->v, svmul_n_f32_z(pg, svld1_f32(pg, v->v), 1.0f / v->w));
        res->w = 1.0f;
    } else {
        svst1_f32(pg, res->v, svdup_n_f32(0.0f));
    }
}

static svfloat32_t quat_mul4(svbool_t pg, svfloat32_t a, svfloat32_t b)
{
    const uint32_t i1[4] = {3, 2, 1, 0};
    const uint32_t i2[4] = {2, 3, 0, 1};
    const uint32_t i3[4] = {1, 0, 3, 2};
    const svuint32_t lane = svindex_u32(0, 1);
    const svbool_t n13 = svorr_z(pg, svcmpeq_n_u32(pg, lane, 1), svcmpeq_n_u32(pg, lane, 3));
    const svbool_t n23 = svorr_z(pg, svcmpeq_n_u32(pg, lane, 2), svcmpeq_n_u32(pg, lane, 3));
    const svbool_t n03 = svorr_z(pg, svcmpeq_n_u32(pg, lane, 0), svcmpeq_n_u32(pg, lane, 3));

    svfloat32_t t1 = svtbl_f32(b, svld1_u32(pg, i1));
    svfloat32_t t2 = svtbl_f32(b, svld1_u32(pg, i2));
    svfloat32_t t3 = svtbl_f32(b, svld1_u32(pg, i3));
    t1 = svneg_f32_m(t1, n13, t1);
    t2 = svneg_f32_m(t2, n23, t2);
    t3 = svneg_f32_m(t3, n03, t3);

    float aa[4];
    svst1_f32(pg, aa, a);
    svfloat32_t r = svmul_n_f32_z(pg, b, aa[3]);
    r = svmla_n_f32_z(pg, r, t1, aa[0]);
    r = svmla_n_f32_z(pg, r, t2, aa[1]);
    r = svmla_n_f32_z(pg, r, t3, aa[2]);
    return r;
}

void quat_mul_ptr_sve(quaternion *res, const quaternion *a, const quaternion *b)
{
    const svbool_t pg = pg4();
    svst1_f32(pg, res->v, quat_mul4(pg, svld1_f32(pg, a->v), svld1_f32(pg, b->v)));
}

void quat_normalize_ptr_sve(quaternion *res, const quaternion *q)
{
    vec4_normalize_ptr_sve((vector4 *)res, (const vector4 *)q);
}

#endif
