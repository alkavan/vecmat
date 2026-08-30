// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <immintrin.h>
#include <math.h>
#include <vecmat.h>

#if defined(VECMAT_USE_F64)

static inline __m256d load4(const vector4 *v)
{
    return _mm256_loadu_pd(v->v);
}

static inline void store4(vector4 *v, const __m256d x)
{
    _mm256_storeu_pd(v->v, x);
}

static inline __m256d hadamard_div(__m256d a, __m256d b)
{
    const __m256d zero = _mm256_setzero_pd();
    const __m256d eqz = _mm256_cmp_pd(b, zero, _CMP_EQ_OQ);
    const __m256d safe = _mm256_blendv_pd(b, _mm256_set1_pd(1.0), eqz);
    return _mm256_andnot_pd(eqz, _mm256_div_pd(a, safe));
}

static inline __m256d sign4(__m256d x)
{
    const __m256d zero = _mm256_setzero_pd();
    const __m256d pos = _mm256_and_pd(_mm256_cmp_pd(x, zero, _CMP_GT_OQ), _mm256_set1_pd(1.0));
    const __m256d neg = _mm256_and_pd(_mm256_cmp_pd(x, zero, _CMP_LT_OQ), _mm256_set1_pd(-1.0));
    return _mm256_or_pd(pos, neg);
}

void vec4_add_ptr_avx(vector4 *res, const vector4 *a, const vector4 *b)
{
    store4(res, _mm256_add_pd(load4(a), load4(b)));
}

void vec4_sub_ptr_avx(vector4 *res, const vector4 *a, const vector4 *b)
{
    store4(res, _mm256_sub_pd(load4(a), load4(b)));
}

void vec4_mul_ptr_avx(vector4 *res, const vector4 *a, const vector4 *b)
{
    store4(res, _mm256_mul_pd(load4(a), load4(b)));
}

void vec4_mul_scalar_ptr_avx(vector4 *res, const vector4 *v, const vm_float_t s)
{
    store4(res, _mm256_mul_pd(load4(v), _mm256_set1_pd(s)));
}

void vec4_div_scalar_ptr_avx(vector4 *res, const vector4 *v, const vm_float_t s)
{
    if (s == 0.0) {
        store4(res, _mm256_setzero_pd());
        return;
    }
    store4(res, _mm256_div_pd(load4(v), _mm256_set1_pd(s)));
}

void vec4_neg_ptr_avx(vector4 *res, const vector4 *v)
{
    store4(res, _mm256_xor_pd(load4(v), _mm256_set1_pd(-0.0)));
}

void vec4_abs_ptr_avx(vector4 *res, const vector4 *v)
{
    const __m256d mask = _mm256_castsi256_pd(_mm256_set1_epi64x(0x7FFFFFFFFFFFFFFFLL));
    store4(res, _mm256_and_pd(load4(v), mask));
}

void vec4_normalize_ptr_avx(vector4 *res, const vector4 *v)
{
    const __m256d x = load4(v);
    const __m256d sq = _mm256_mul_pd(x, x);
    const __m128d hi = _mm256_extractf128_pd(sq, 1);
    const __m128d lo = _mm256_castpd256_pd128(sq);
    __m128d sum = _mm_add_pd(lo, hi);
    sum = _mm_add_sd(sum, _mm_unpackhi_pd(sum, sum));
    const double len2 = _mm_cvtsd_f64(sum);
    if (len2 == 0.0) {
        *res = *v;
        return;
    }
    /* rsqrtss estimate + two Newton steps in double (no rsqrt.pd in AVX). */
    double y = (double)_mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss((float)len2)));
    y = y * (1.5 - 0.5 * len2 * y * y);
    y = y * (1.5 - 0.5 * len2 * y * y);
    store4(res, _mm256_mul_pd(x, _mm256_set1_pd(y)));
}

void vec4_min_ptr_avx(vector4 *res, const vector4 *a, const vector4 *b)
{
    store4(res, _mm256_min_pd(load4(a), load4(b)));
}

void vec4_max_ptr_avx(vector4 *res, const vector4 *a, const vector4 *b)
{
    store4(res, _mm256_max_pd(load4(a), load4(b)));
}

void vec4_lerp_ptr_avx(vector4 *res, const vector4 *a, const vector4 *b, const vm_float_t t)
{
    const __m256d va = load4(a);
    const __m256d vb = load4(b);
    store4(res, _mm256_add_pd(va, _mm256_mul_pd(_mm256_set1_pd(t), _mm256_sub_pd(vb, va))));
}

void vec4_clamp_ptr_avx(vector4 *res, const vector4 *v,
                        const vector4 *min, const vector4 *max)
{
    store4(res, _mm256_min_pd(load4(max), _mm256_max_pd(load4(min), load4(v))));
}

void vec4_div_ptr_avx(vector4 *res, const vector4 *a, const vector4 *b)
{
    store4(res, hadamard_div(load4(a), load4(b)));
}

void vec4_add_scalar_ptr_avx(vector4 *res, const vector4 *v, const vm_float_t s)
{
    store4(res, _mm256_add_pd(load4(v), _mm256_set1_pd(s)));
}

void vec4_sub_scalar_ptr_avx(vector4 *res, const vector4 *v, const vm_float_t s)
{
    store4(res, _mm256_sub_pd(load4(v), _mm256_set1_pd(s)));
}

void vec4_clamp_scalar_ptr_avx(vector4 *res, const vector4 *v, const vm_float_t min, const vm_float_t max)
{
    store4(res, _mm256_min_pd(_mm256_set1_pd(max), _mm256_max_pd(_mm256_set1_pd(min), load4(v))));
}

void vec4_saturate_ptr_avx(vector4 *res, const vector4 *v)
{
    store4(res, _mm256_min_pd(_mm256_set1_pd(1.0), _mm256_max_pd(_mm256_setzero_pd(), load4(v))));
}

void vec4_sign_ptr_avx(vector4 *res, const vector4 *v)
{
    store4(res, sign4(load4(v)));
}

void vec4_floor_ptr_avx(vector4 *res, const vector4 *v)
{
    store4(res, _mm256_floor_pd(load4(v)));
}

void vec4_ceil_ptr_avx(vector4 *res, const vector4 *v)
{
    store4(res, _mm256_ceil_pd(load4(v)));
}

void vec4_round_ptr_avx(vector4 *res, const vector4 *v)
{
    const __m256d x = load4(v);
    const __m256d abs_mask = _mm256_castsi256_pd(_mm256_set1_epi64x(0x7FFFFFFFFFFFFFFFLL));
    const __m256d sign = _mm256_andnot_pd(abs_mask, x);
    const __m256d mag = _mm256_floor_pd(_mm256_add_pd(_mm256_and_pd(x, abs_mask), _mm256_set1_pd(0.5)));
    store4(res, _mm256_or_pd(mag, sign));
}

void vec4_fract_ptr_avx(vector4 *res, const vector4 *v)
{
    const __m256d x = load4(v);
    store4(res, _mm256_sub_pd(x, _mm256_floor_pd(x)));
}

void vec4_homogenize_ptr_avx(vector4 *res, const vector4 *v)
{
    if (VECMAT_FABS(v->w) > VECMAT_EPSILON) {
        store4(res, _mm256_mul_pd(load4(v), _mm256_set1_pd(1.0 / v->w)));
        res->w = 1.0;
    } else {
        store4(res, _mm256_setzero_pd());
    }
}

static inline __m256d quat_mul_avx(__m256d a, __m256d b)
{
    const __m128d a_lo = _mm256_castpd256_pd128(a);
    const __m128d a_hi = _mm256_extractf128_pd(a, 1);
    const double ax = _mm_cvtsd_f64(a_lo);
    const double ay = _mm_cvtsd_f64(_mm_unpackhi_pd(a_lo, a_lo));
    const double az = _mm_cvtsd_f64(a_hi);
    const double aw = _mm_cvtsd_f64(_mm_unpackhi_pd(a_hi, a_hi));

    const __m128d b_lo = _mm256_castpd256_pd128(b);
    const __m128d b_hi = _mm256_extractf128_pd(b, 1);
    const double bx = _mm_cvtsd_f64(b_lo);
    const double by = _mm_cvtsd_f64(_mm_unpackhi_pd(b_lo, b_lo));
    const double bz = _mm_cvtsd_f64(b_hi);
    const double bw = _mm_cvtsd_f64(_mm_unpackhi_pd(b_hi, b_hi));

    return _mm256_setr_pd(
        aw * bx + ax * bw + ay * bz - az * by,
        aw * by - ax * bz + ay * bw + az * bx,
        aw * bz + ax * by - ay * bx + az * bw,
        aw * bw - ax * bx - ay * by - az * bz);
}

void quat_mul_ptr_avx(quaternion *res, const quaternion *a, const quaternion *b)
{
    const __m256d r = quat_mul_avx(_mm256_loadu_pd(a->v), _mm256_loadu_pd(b->v));
    _mm256_storeu_pd(res->v, r);
}

void quat_normalize_ptr_avx(quaternion *res, const quaternion *q)
{
    vec4_normalize_ptr_avx((vector4 *)res, (const vector4 *)q);
}

#else /* float */

static inline __m128 load4(const vector4 *v)
{
    return _mm_loadu_ps(v->v);
}

static inline void store4(vector4 *v, const __m128 x)
{
    _mm_storeu_ps(v->v, x);
}

static inline __m128 hadamard_div(__m128 a, __m128 b)
{
    const __m128 zero = _mm_setzero_ps();
    const __m128 eqz = _mm_cmpeq_ps(b, zero);
    const __m128 safe = _mm_blendv_ps(b, _mm_set1_ps(1.0f), eqz);
    return _mm_andnot_ps(eqz, _mm_div_ps(a, safe));
}

static inline __m128 sign4(__m128 x)
{
    const __m128 zero = _mm_setzero_ps();
    const __m128 pos = _mm_and_ps(_mm_cmpgt_ps(x, zero), _mm_set1_ps(1.0f));
    const __m128 neg = _mm_and_ps(_mm_cmplt_ps(x, zero), _mm_set1_ps(-1.0f));
    return _mm_or_ps(pos, neg);
}

void vec4_add_ptr_avx(vector4 *res, const vector4 *a, const vector4 *b)
{
    store4(res, _mm_add_ps(load4(a), load4(b)));
}

void vec4_sub_ptr_avx(vector4 *res, const vector4 *a, const vector4 *b)
{
    store4(res, _mm_sub_ps(load4(a), load4(b)));
}

void vec4_mul_ptr_avx(vector4 *res, const vector4 *a, const vector4 *b)
{
    store4(res, _mm_mul_ps(load4(a), load4(b)));
}

void vec4_mul_scalar_ptr_avx(vector4 *res, const vector4 *v, const vm_float_t s)
{
    store4(res, _mm_mul_ps(load4(v), _mm_set1_ps(s)));
}

void vec4_div_scalar_ptr_avx(vector4 *res, const vector4 *v, const vm_float_t s)
{
    if (s == 0.0f) {
        store4(res, _mm_setzero_ps());
        return;
    }
    store4(res, _mm_div_ps(load4(v), _mm_set1_ps(s)));
}

void vec4_neg_ptr_avx(vector4 *res, const vector4 *v)
{
    store4(res, _mm_xor_ps(load4(v), _mm_set1_ps(-0.0f)));
}

void vec4_abs_ptr_avx(vector4 *res, const vector4 *v)
{
    const __m128 mask = _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF));
    store4(res, _mm_and_ps(load4(v), mask));
}

void vec4_normalize_ptr_avx(vector4 *res, const vector4 *v)
{
    const __m128 x = load4(v);
    const __m128 len2 = _mm_dp_ps(x, x, 0xFF);
    if (_mm_cvtss_f32(len2) == 0.0f) {
        *res = *v;
        return;
    }
    const __m128 y = _mm_rsqrt_ps(len2);
    const __m128 half = _mm_set1_ps(0.5f);
    const __m128 three_halves = _mm_set1_ps(1.5f);
    const __m128 inv = _mm_mul_ps(y, _mm_sub_ps(three_halves,
        _mm_mul_ps(_mm_mul_ps(half, len2), _mm_mul_ps(y, y))));
    store4(res, _mm_mul_ps(x, inv));
}

void vec4_min_ptr_avx(vector4 *res, const vector4 *a, const vector4 *b)
{
    store4(res, _mm_min_ps(load4(a), load4(b)));
}

void vec4_max_ptr_avx(vector4 *res, const vector4 *a, const vector4 *b)
{
    store4(res, _mm_max_ps(load4(a), load4(b)));
}

void vec4_lerp_ptr_avx(vector4 *res, const vector4 *a, const vector4 *b, const vm_float_t t)
{
    const __m128 va = load4(a);
    const __m128 vb = load4(b);
    store4(res, _mm_add_ps(va, _mm_mul_ps(_mm_set1_ps(t), _mm_sub_ps(vb, va))));
}

void vec4_clamp_ptr_avx(vector4 *res, const vector4 *v,
                        const vector4 *min, const vector4 *max)
{
    store4(res, _mm_min_ps(load4(max), _mm_max_ps(load4(min), load4(v))));
}

void vec4_div_ptr_avx(vector4 *res, const vector4 *a, const vector4 *b)
{
    store4(res, hadamard_div(load4(a), load4(b)));
}

void vec4_add_scalar_ptr_avx(vector4 *res, const vector4 *v, const vm_float_t s)
{
    store4(res, _mm_add_ps(load4(v), _mm_set1_ps(s)));
}

void vec4_sub_scalar_ptr_avx(vector4 *res, const vector4 *v, const vm_float_t s)
{
    store4(res, _mm_sub_ps(load4(v), _mm_set1_ps(s)));
}

void vec4_clamp_scalar_ptr_avx(vector4 *res, const vector4 *v, const vm_float_t min, const vm_float_t max)
{
    store4(res, _mm_min_ps(_mm_set1_ps(max), _mm_max_ps(_mm_set1_ps(min), load4(v))));
}

void vec4_saturate_ptr_avx(vector4 *res, const vector4 *v)
{
    store4(res, _mm_min_ps(_mm_set1_ps(1.0f), _mm_max_ps(_mm_setzero_ps(), load4(v))));
}

void vec4_sign_ptr_avx(vector4 *res, const vector4 *v)
{
    store4(res, sign4(load4(v)));
}

void vec4_floor_ptr_avx(vector4 *res, const vector4 *v)
{
    store4(res, _mm_floor_ps(load4(v)));
}

void vec4_ceil_ptr_avx(vector4 *res, const vector4 *v)
{
    store4(res, _mm_ceil_ps(load4(v)));
}

void vec4_round_ptr_avx(vector4 *res, const vector4 *v)
{
    const __m128 x = load4(v);
    const __m128 abs_mask = _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF));
    const __m128 sign = _mm_andnot_ps(abs_mask, x);
    const __m128 mag = _mm_floor_ps(_mm_add_ps(_mm_and_ps(x, abs_mask), _mm_set1_ps(0.5f)));
    store4(res, _mm_or_ps(mag, sign));
}

void vec4_fract_ptr_avx(vector4 *res, const vector4 *v)
{
    const __m128 x = load4(v);
    store4(res, _mm_sub_ps(x, _mm_floor_ps(x)));
}

void vec4_homogenize_ptr_avx(vector4 *res, const vector4 *v)
{
    if (VECMAT_FABS(v->w) > VECMAT_EPSILON) {
        store4(res, _mm_mul_ps(load4(v), _mm_set1_ps(1.0f / v->w)));
        res->w = 1.0f;
    } else {
        store4(res, _mm_setzero_ps());
    }
}

static inline __m128 quat_mul_f32(__m128 a, __m128 b)
{
    const __m128 aw = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 3, 3, 3));
    const __m128 ax = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 0, 0));
    const __m128 ay = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 1, 1, 1));
    const __m128 az = _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 2, 2, 2));

    const __m128 t1 = _mm_xor_ps(_mm_shuffle_ps(b, b, _MM_SHUFFLE(0, 1, 2, 3)),
                                 _mm_set_ps(-0.0f, 0.0f, -0.0f, 0.0f));
    const __m128 t2 = _mm_xor_ps(_mm_shuffle_ps(b, b, _MM_SHUFFLE(1, 0, 3, 2)),
                                 _mm_set_ps(-0.0f, -0.0f, 0.0f, 0.0f));
    const __m128 t3 = _mm_xor_ps(_mm_shuffle_ps(b, b, _MM_SHUFFLE(2, 3, 0, 1)),
                                 _mm_set_ps(-0.0f, 0.0f, 0.0f, -0.0f));

    __m128 r = _mm_mul_ps(aw, b);
    r = _mm_add_ps(r, _mm_mul_ps(ax, t1));
    r = _mm_add_ps(r, _mm_mul_ps(ay, t2));
    r = _mm_add_ps(r, _mm_mul_ps(az, t3));
    return r;
}

void quat_mul_ptr_avx(quaternion *res, const quaternion *a, const quaternion *b)
{
    _mm_storeu_ps(res->v, quat_mul_f32(_mm_loadu_ps(a->v), _mm_loadu_ps(b->v)));
}

void quat_normalize_ptr_avx(quaternion *res, const quaternion *q)
{
    vec4_normalize_ptr_avx((vector4 *)res, (const vector4 *)q);
}

#endif
