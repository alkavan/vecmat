// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <arm_neon.h>
#include <math.h>
#include <vecmat.h>

#if defined(VECMAT_USE_F64)

static inline float64x2_t load2(const vm_float_t *p)
{
    return vld1q_f64(p);
}

static inline void store2(vm_float_t *p, const float64x2_t x)
{
    vst1q_f64(p, x);
}

static inline float64x2_t hadamard_div2(float64x2_t a, float64x2_t b)
{
    const float64x2_t zero = vdupq_n_f64(0.0);
    const uint64x2_t eqz = vceqq_f64(b, zero);
    const float64x2_t safe = vbslq_f64(eqz, vdupq_n_f64(1.0), b);
    const float64x2_t q = vdivq_f64(a, safe);
    return vbslq_f64(eqz, zero, q);
}

static inline float64x2_t sign2(float64x2_t x)
{
    const float64x2_t zero = vdupq_n_f64(0.0);
    const float64x2_t pos = vbslq_f64(vcgtq_f64(x, zero), vdupq_n_f64(1.0), zero);
    return vbslq_f64(vcltq_f64(x, zero), vdupq_n_f64(-1.0), pos);
}

void vec4_add_ptr_neon(vector4 *res, const vector4 *a, const vector4 *b)
{
    store2(res->v + 0, vaddq_f64(load2(a->v + 0), load2(b->v + 0)));
    store2(res->v + 2, vaddq_f64(load2(a->v + 2), load2(b->v + 2)));
}

void vec4_sub_ptr_neon(vector4 *res, const vector4 *a, const vector4 *b)
{
    store2(res->v + 0, vsubq_f64(load2(a->v + 0), load2(b->v + 0)));
    store2(res->v + 2, vsubq_f64(load2(a->v + 2), load2(b->v + 2)));
}

void vec4_mul_ptr_neon(vector4 *res, const vector4 *a, const vector4 *b)
{
    store2(res->v + 0, vmulq_f64(load2(a->v + 0), load2(b->v + 0)));
    store2(res->v + 2, vmulq_f64(load2(a->v + 2), load2(b->v + 2)));
}

void vec4_mul_scalar_ptr_neon(vector4 *res, const vector4 *v, const vm_float_t s)
{
    const float64x2_t ss = vdupq_n_f64(s);
    store2(res->v + 0, vmulq_f64(load2(v->v + 0), ss));
    store2(res->v + 2, vmulq_f64(load2(v->v + 2), ss));
}

void vec4_div_scalar_ptr_neon(vector4 *res, const vector4 *v, const vm_float_t s)
{
    if (s == 0.0) {
        store2(res->v + 0, vdupq_n_f64(0.0));
        store2(res->v + 2, vdupq_n_f64(0.0));
        return;
    }
    const float64x2_t ss = vdupq_n_f64(s);
    store2(res->v + 0, vdivq_f64(load2(v->v + 0), ss));
    store2(res->v + 2, vdivq_f64(load2(v->v + 2), ss));
}

void vec4_neg_ptr_neon(vector4 *res, const vector4 *v)
{
    store2(res->v + 0, vnegq_f64(load2(v->v + 0)));
    store2(res->v + 2, vnegq_f64(load2(v->v + 2)));
}

void vec4_abs_ptr_neon(vector4 *res, const vector4 *v)
{
    store2(res->v + 0, vabsq_f64(load2(v->v + 0)));
    store2(res->v + 2, vabsq_f64(load2(v->v + 2)));
}

void vec4_normalize_ptr_neon(vector4 *res, const vector4 *v)
{
    const float64x2_t lo = load2(v->v + 0);
    const float64x2_t hi = load2(v->v + 2);
    const float64x2_t sq = vaddq_f64(vmulq_f64(lo, lo), vmulq_f64(hi, hi));
    const double len2 = vgetq_lane_f64(sq, 0) + vgetq_lane_f64(sq, 1);
    if (len2 == 0.0) {
        *res = *v;
        return;
    }
    const double inv = 1.0 / VECMAT_SQRT(len2);
    const float64x2_t s = vdupq_n_f64(inv);
    store2(res->v + 0, vmulq_f64(lo, s));
    store2(res->v + 2, vmulq_f64(hi, s));
}

void vec4_min_ptr_neon(vector4 *res, const vector4 *a, const vector4 *b)
{
    store2(res->v + 0, vminq_f64(load2(a->v + 0), load2(b->v + 0)));
    store2(res->v + 2, vminq_f64(load2(a->v + 2), load2(b->v + 2)));
}

void vec4_max_ptr_neon(vector4 *res, const vector4 *a, const vector4 *b)
{
    store2(res->v + 0, vmaxq_f64(load2(a->v + 0), load2(b->v + 0)));
    store2(res->v + 2, vmaxq_f64(load2(a->v + 2), load2(b->v + 2)));
}

void vec4_lerp_ptr_neon(vector4 *res, const vector4 *a, const vector4 *b, const vm_float_t t)
{
    const float64x2_t tt = vdupq_n_f64(t);
    const float64x2_t a0 = load2(a->v + 0);
    const float64x2_t a1 = load2(a->v + 2);
    store2(res->v + 0, vfmaq_f64(a0, vsubq_f64(load2(b->v + 0), a0), tt));
    store2(res->v + 2, vfmaq_f64(a1, vsubq_f64(load2(b->v + 2), a1), tt));
}

void vec4_clamp_ptr_neon(vector4 *res, const vector4 *v,
                         const vector4 *min, const vector4 *max)
{
    store2(res->v + 0, vminq_f64(load2(max->v + 0), vmaxq_f64(load2(min->v + 0), load2(v->v + 0))));
    store2(res->v + 2, vminq_f64(load2(max->v + 2), vmaxq_f64(load2(min->v + 2), load2(v->v + 2))));
}

void vec4_div_ptr_neon(vector4 *res, const vector4 *a, const vector4 *b)
{
    store2(res->v + 0, hadamard_div2(load2(a->v + 0), load2(b->v + 0)));
    store2(res->v + 2, hadamard_div2(load2(a->v + 2), load2(b->v + 2)));
}

void vec4_add_scalar_ptr_neon(vector4 *res, const vector4 *v, const vm_float_t s)
{
    const float64x2_t ss = vdupq_n_f64(s);
    store2(res->v + 0, vaddq_f64(load2(v->v + 0), ss));
    store2(res->v + 2, vaddq_f64(load2(v->v + 2), ss));
}

void vec4_sub_scalar_ptr_neon(vector4 *res, const vector4 *v, const vm_float_t s)
{
    const float64x2_t ss = vdupq_n_f64(s);
    store2(res->v + 0, vsubq_f64(load2(v->v + 0), ss));
    store2(res->v + 2, vsubq_f64(load2(v->v + 2), ss));
}

void vec4_clamp_scalar_ptr_neon(vector4 *res, const vector4 *v, const vm_float_t min, const vm_float_t max)
{
    store2(res->v + 0, vminq_f64(vdupq_n_f64(max), vmaxq_f64(vdupq_n_f64(min), load2(v->v + 0))));
    store2(res->v + 2, vminq_f64(vdupq_n_f64(max), vmaxq_f64(vdupq_n_f64(min), load2(v->v + 2))));
}

void vec4_saturate_ptr_neon(vector4 *res, const vector4 *v)
{
    const float64x2_t z = vdupq_n_f64(0.0);
    const float64x2_t o = vdupq_n_f64(1.0);
    store2(res->v + 0, vminq_f64(o, vmaxq_f64(z, load2(v->v + 0))));
    store2(res->v + 2, vminq_f64(o, vmaxq_f64(z, load2(v->v + 2))));
}

void vec4_sign_ptr_neon(vector4 *res, const vector4 *v)
{
    store2(res->v + 0, sign2(load2(v->v + 0)));
    store2(res->v + 2, sign2(load2(v->v + 2)));
}

void vec4_floor_ptr_neon(vector4 *res, const vector4 *v)
{
    store2(res->v + 0, vrndmq_f64(load2(v->v + 0)));
    store2(res->v + 2, vrndmq_f64(load2(v->v + 2)));
}

void vec4_ceil_ptr_neon(vector4 *res, const vector4 *v)
{
    store2(res->v + 0, vrndpq_f64(load2(v->v + 0)));
    store2(res->v + 2, vrndpq_f64(load2(v->v + 2)));
}

void vec4_round_ptr_neon(vector4 *res, const vector4 *v)
{
    /* FRINTA: nearest, ties away from zero (matches libm round). */
    store2(res->v + 0, vrndaq_f64(load2(v->v + 0)));
    store2(res->v + 2, vrndaq_f64(load2(v->v + 2)));
}

void vec4_fract_ptr_neon(vector4 *res, const vector4 *v)
{
    const float64x2_t lo = load2(v->v + 0);
    const float64x2_t hi = load2(v->v + 2);
    store2(res->v + 0, vsubq_f64(lo, vrndmq_f64(lo)));
    store2(res->v + 2, vsubq_f64(hi, vrndmq_f64(hi)));
}

void vec4_homogenize_ptr_neon(vector4 *res, const vector4 *v)
{
    if (VECMAT_FABS(v->w) > VECMAT_EPSILON) {
        const float64x2_t s = vdupq_n_f64(1.0 / v->w);
        store2(res->v + 0, vmulq_f64(load2(v->v + 0), s));
        store2(res->v + 2, vmulq_f64(load2(v->v + 2), s));
        res->w = 1.0;
    } else {
        store2(res->v + 0, vdupq_n_f64(0.0));
        store2(res->v + 2, vdupq_n_f64(0.0));
    }
}

void quat_mul_ptr_neon(quaternion *res, const quaternion *a, const quaternion *b)
{
    const double ax = a->x, ay = a->y, az = a->z, aw = a->w;
    const double bx = b->x, by = b->y, bz = b->z, bw = b->w;
    res->x = aw * bx + ax * bw + ay * bz - az * by;
    res->y = aw * by - ax * bz + ay * bw + az * bx;
    res->z = aw * bz + ax * by - ay * bx + az * bw;
    res->w = aw * bw - ax * bx - ay * by - az * bz;
}

void quat_normalize_ptr_neon(quaternion *res, const quaternion *q)
{
    vec4_normalize_ptr_neon((vector4 *)res, (const vector4 *)q);
}

#else /* float */

static inline float32x4_t load4(const vector4 *v)
{
    return vld1q_f32(v->v);
}

static inline void store4(vector4 *v, const float32x4_t x)
{
    vst1q_f32(v->v, x);
}

static inline float32x4_t hadamard_div(float32x4_t a, float32x4_t b)
{
    const float32x4_t zero = vdupq_n_f32(0.0f);
    const uint32x4_t eqz = vceqq_f32(b, zero);
    const float32x4_t safe = vbslq_f32(eqz, vdupq_n_f32(1.0f), b);
    const float32x4_t q = vdivq_f32(a, safe);
    return vbslq_f32(eqz, zero, q);
}

static inline float32x4_t sign4(float32x4_t x)
{
    const float32x4_t zero = vdupq_n_f32(0.0f);
    const float32x4_t pos = vbslq_f32(vcgtq_f32(x, zero), vdupq_n_f32(1.0f), zero);
    return vbslq_f32(vcltq_f32(x, zero), vdupq_n_f32(-1.0f), pos);
}

void vec4_add_ptr_neon(vector4 *res, const vector4 *a, const vector4 *b)
{
    store4(res, vaddq_f32(load4(a), load4(b)));
}

void vec4_sub_ptr_neon(vector4 *res, const vector4 *a, const vector4 *b)
{
    store4(res, vsubq_f32(load4(a), load4(b)));
}

void vec4_mul_ptr_neon(vector4 *res, const vector4 *a, const vector4 *b)
{
    store4(res, vmulq_f32(load4(a), load4(b)));
}

void vec4_mul_scalar_ptr_neon(vector4 *res, const vector4 *v, const vm_float_t s)
{
    store4(res, vmulq_n_f32(load4(v), s));
}

void vec4_div_scalar_ptr_neon(vector4 *res, const vector4 *v, const vm_float_t s)
{
    if (s == 0.0f) {
        store4(res, vdupq_n_f32(0.0f));
        return;
    }
    store4(res, vdivq_f32(load4(v), vdupq_n_f32(s)));
}

void vec4_neg_ptr_neon(vector4 *res, const vector4 *v)
{
    store4(res, vnegq_f32(load4(v)));
}

void vec4_abs_ptr_neon(vector4 *res, const vector4 *v)
{
    store4(res, vabsq_f32(load4(v)));
}

void vec4_normalize_ptr_neon(vector4 *res, const vector4 *v)
{
    const float32x4_t x = load4(v);
    const float32x4_t sq = vmulq_f32(x, x);
    const float32x2_t sum2 = vadd_f32(vget_low_f32(sq), vget_high_f32(sq));
    const float32x2_t sum1 = vpadd_f32(sum2, sum2);
    const float len2 = vget_lane_f32(sum1, 0);
    if (len2 == 0.0f) {
        *res = *v;
        return;
    }
    /*
     * Exact rsqrt. ASIMD URSQRTE is ~8 bits; one Newton step is ~16 bits
     * and misses VECMAT_EPS_F32 on Windows ARM64 (Clang and MSVC).
     * SVE F32 already uses 1/sqrt for the same reason.
     */
    store4(res, vmulq_n_f32(x, 1.0f / VECMAT_SQRT(len2)));
}

void vec4_min_ptr_neon(vector4 *res, const vector4 *a, const vector4 *b)
{
    store4(res, vminq_f32(load4(a), load4(b)));
}

void vec4_max_ptr_neon(vector4 *res, const vector4 *a, const vector4 *b)
{
    store4(res, vmaxq_f32(load4(a), load4(b)));
}

void vec4_lerp_ptr_neon(vector4 *res, const vector4 *a, const vector4 *b, const vm_float_t t)
{
    const float32x4_t va = load4(a);
    store4(res, vfmaq_n_f32(va, vsubq_f32(load4(b), va), t));
}

void vec4_clamp_ptr_neon(vector4 *res, const vector4 *v,
                         const vector4 *min, const vector4 *max)
{
    store4(res, vminq_f32(load4(max), vmaxq_f32(load4(min), load4(v))));
}

void vec4_div_ptr_neon(vector4 *res, const vector4 *a, const vector4 *b)
{
    store4(res, hadamard_div(load4(a), load4(b)));
}

void vec4_add_scalar_ptr_neon(vector4 *res, const vector4 *v, const vm_float_t s)
{
    store4(res, vaddq_f32(load4(v), vdupq_n_f32(s)));
}

void vec4_sub_scalar_ptr_neon(vector4 *res, const vector4 *v, const vm_float_t s)
{
    store4(res, vsubq_f32(load4(v), vdupq_n_f32(s)));
}

void vec4_clamp_scalar_ptr_neon(vector4 *res, const vector4 *v, const vm_float_t min, const vm_float_t max)
{
    store4(res, vminq_f32(vdupq_n_f32(max), vmaxq_f32(vdupq_n_f32(min), load4(v))));
}

void vec4_saturate_ptr_neon(vector4 *res, const vector4 *v)
{
    store4(res, vminq_f32(vdupq_n_f32(1.0f), vmaxq_f32(vdupq_n_f32(0.0f), load4(v))));
}

void vec4_sign_ptr_neon(vector4 *res, const vector4 *v)
{
    store4(res, sign4(load4(v)));
}

void vec4_floor_ptr_neon(vector4 *res, const vector4 *v)
{
    store4(res, vrndmq_f32(load4(v)));
}

void vec4_ceil_ptr_neon(vector4 *res, const vector4 *v)
{
    store4(res, vrndpq_f32(load4(v)));
}

void vec4_round_ptr_neon(vector4 *res, const vector4 *v)
{
    store4(res, vrndaq_f32(load4(v)));
}

void vec4_fract_ptr_neon(vector4 *res, const vector4 *v)
{
    const float32x4_t x = load4(v);
    store4(res, vsubq_f32(x, vrndmq_f32(x)));
}

void vec4_homogenize_ptr_neon(vector4 *res, const vector4 *v)
{
    if (VECMAT_FABS(v->w) > VECMAT_EPSILON) {
        store4(res, vmulq_n_f32(load4(v), 1.0f / v->w));
        res->w = 1.0f;
    } else {
        store4(res, vdupq_n_f32(0.0f));
    }
}

void quat_mul_ptr_neon(quaternion *res, const quaternion *a, const quaternion *b)
{
    const float ax = a->x, ay = a->y, az = a->z, aw = a->w;
    const float bx = b->x, by = b->y, bz = b->z, bw = b->w;
    res->x = aw * bx + ax * bw + ay * bz - az * by;
    res->y = aw * by - ax * bz + ay * bw + az * bx;
    res->z = aw * bz + ax * by - ay * bx + az * bw;
    res->w = aw * bw - ax * bx - ay * by - az * bz;
}

void quat_normalize_ptr_neon(quaternion *res, const quaternion *q)
{
    vec4_normalize_ptr_neon((vector4 *)res, (const vector4 *)q);
}

#endif
