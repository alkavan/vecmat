// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <immintrin.h>
#include <string.h>
#include <vecmat.h>

#if defined(VECMAT_USE_F64)

void mat4_mul_ptr_avx2(matrix4 *res, const matrix4 *a, const matrix4 *b)
{
    matrix4 tmp;
    const __m256d a0 = _mm256_loadu_pd(&a->v[0]);
    const __m256d a1 = _mm256_loadu_pd(&a->v[4]);
    const __m256d a2 = _mm256_loadu_pd(&a->v[8]);
    const __m256d a3 = _mm256_loadu_pd(&a->v[12]);
    for (int c = 0; c < 4; c++) {
        __m256d col = _mm256_mul_pd(a0, _mm256_set1_pd(b->v[c * 4 + 0]));
        col = _mm256_fmadd_pd(a1, _mm256_set1_pd(b->v[c * 4 + 1]), col);
        col = _mm256_fmadd_pd(a2, _mm256_set1_pd(b->v[c * 4 + 2]), col);
        col = _mm256_fmadd_pd(a3, _mm256_set1_pd(b->v[c * 4 + 3]), col);
        _mm256_storeu_pd(&tmp.v[c * 4], col);
    }
    memcpy(res->v, tmp.v, sizeof(tmp.v));
}

void mat4_transpose_ptr_avx2(matrix4 *res, const matrix4 *m)
{
    __m256d r0 = _mm256_loadu_pd(&m->v[0]);
    __m256d r1 = _mm256_loadu_pd(&m->v[4]);
    __m256d r2 = _mm256_loadu_pd(&m->v[8]);
    __m256d r3 = _mm256_loadu_pd(&m->v[12]);

    __m256d t0 = _mm256_unpacklo_pd(r0, r1);
    __m256d t1 = _mm256_unpackhi_pd(r0, r1);
    __m256d t2 = _mm256_unpacklo_pd(r2, r3);
    __m256d t3 = _mm256_unpackhi_pd(r2, r3);

    r0 = _mm256_permute2f128_pd(t0, t2, 0x20);
    r1 = _mm256_permute2f128_pd(t1, t3, 0x20);
    r2 = _mm256_permute2f128_pd(t0, t2, 0x31);
    r3 = _mm256_permute2f128_pd(t1, t3, 0x31);

    matrix4 tmp;
    _mm256_storeu_pd(&tmp.v[0], r0);
    _mm256_storeu_pd(&tmp.v[4], r1);
    _mm256_storeu_pd(&tmp.v[8], r2);
    _mm256_storeu_pd(&tmp.v[12], r3);
    memcpy(res->v, tmp.v, sizeof(tmp.v));
}

void mat4_mul_vec4_ptr_avx2(vector4 *res, const matrix4 *m, const vector4 *v)
{
    __m256d r = _mm256_mul_pd(_mm256_loadu_pd(&m->v[0]), _mm256_set1_pd(v->x));
    r = _mm256_fmadd_pd(_mm256_loadu_pd(&m->v[4]), _mm256_set1_pd(v->y), r);
    r = _mm256_fmadd_pd(_mm256_loadu_pd(&m->v[8]), _mm256_set1_pd(v->z), r);
    r = _mm256_fmadd_pd(_mm256_loadu_pd(&m->v[12]), _mm256_set1_pd(v->w), r);
    _mm256_storeu_pd(res->v, r);
}

void mat4_mul_vec3_ptr_avx2(vector3 *res, const matrix4 *m, const vector3 *v, const vm_float_t w)
{
    __m256d r = _mm256_mul_pd(_mm256_loadu_pd(&m->v[0]), _mm256_set1_pd(v->x));
    r = _mm256_fmadd_pd(_mm256_loadu_pd(&m->v[4]), _mm256_set1_pd(v->y), r);
    r = _mm256_fmadd_pd(_mm256_loadu_pd(&m->v[8]), _mm256_set1_pd(v->z), r);
    r = _mm256_fmadd_pd(_mm256_loadu_pd(&m->v[12]), _mm256_set1_pd(w), r);
    double t[4];
    _mm256_storeu_pd(t, r);
    res->x = t[0];
    res->y = t[1];
    res->z = t[2];
}

#else /* float */

void mat4_mul_ptr_avx2(matrix4 *res, const matrix4 *a, const matrix4 *b)
{
    matrix4 tmp;
    const __m128 a0 = _mm_loadu_ps(&a->v[0]);
    const __m128 a1 = _mm_loadu_ps(&a->v[4]);
    const __m128 a2 = _mm_loadu_ps(&a->v[8]);
    const __m128 a3 = _mm_loadu_ps(&a->v[12]);
    for (int c = 0; c < 4; c++) {
        __m128 col = _mm_mul_ps(a0, _mm_set1_ps(b->v[c * 4 + 0]));
        col = _mm_fmadd_ps(a1, _mm_set1_ps(b->v[c * 4 + 1]), col);
        col = _mm_fmadd_ps(a2, _mm_set1_ps(b->v[c * 4 + 2]), col);
        col = _mm_fmadd_ps(a3, _mm_set1_ps(b->v[c * 4 + 3]), col);
        _mm_storeu_ps(&tmp.v[c * 4], col);
    }
    memcpy(res->v, tmp.v, sizeof(tmp.v));
}

void mat4_transpose_ptr_avx2(matrix4 *res, const matrix4 *m)
{
    __m128 c0 = _mm_loadu_ps(&m->v[0]);
    __m128 c1 = _mm_loadu_ps(&m->v[4]);
    __m128 c2 = _mm_loadu_ps(&m->v[8]);
    __m128 c3 = _mm_loadu_ps(&m->v[12]);
    _MM_TRANSPOSE4_PS(c0, c1, c2, c3);
    matrix4 tmp;
    _mm_storeu_ps(&tmp.v[0], c0);
    _mm_storeu_ps(&tmp.v[4], c1);
    _mm_storeu_ps(&tmp.v[8], c2);
    _mm_storeu_ps(&tmp.v[12], c3);
    memcpy(res->v, tmp.v, sizeof(tmp.v));
}

void mat4_mul_vec4_ptr_avx2(vector4 *res, const matrix4 *m, const vector4 *v)
{
    __m128 r = _mm_mul_ps(_mm_loadu_ps(&m->v[0]), _mm_set1_ps(v->x));
    r = _mm_fmadd_ps(_mm_loadu_ps(&m->v[4]), _mm_set1_ps(v->y), r);
    r = _mm_fmadd_ps(_mm_loadu_ps(&m->v[8]), _mm_set1_ps(v->z), r);
    r = _mm_fmadd_ps(_mm_loadu_ps(&m->v[12]), _mm_set1_ps(v->w), r);
    _mm_storeu_ps(res->v, r);
}

void mat4_mul_vec3_ptr_avx2(vector3 *res, const matrix4 *m, const vector3 *v, const vm_float_t w)
{
    __m128 r = _mm_mul_ps(_mm_loadu_ps(&m->v[0]), _mm_set1_ps(v->x));
    r = _mm_fmadd_ps(_mm_loadu_ps(&m->v[4]), _mm_set1_ps(v->y), r);
    r = _mm_fmadd_ps(_mm_loadu_ps(&m->v[8]), _mm_set1_ps(v->z), r);
    r = _mm_fmadd_ps(_mm_loadu_ps(&m->v[12]), _mm_set1_ps(w), r);
    float t[4];
    _mm_storeu_ps(t, r);
    res->x = t[0];
    res->y = t[1];
    res->z = t[2];
}

#endif
