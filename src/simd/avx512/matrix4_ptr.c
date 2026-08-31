// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <immintrin.h>
#include <string.h>
#include <vecmat.h>

#if defined(VECMAT_USE_F64)

void mat4_mul_ptr_avx512(matrix4 *res, const matrix4 *a, const matrix4 *b)
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

void mat4_transpose_ptr_avx512(matrix4 *res, const matrix4 *m)
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

void mat4_mul_vec4_ptr_avx512(vector4 *res, const matrix4 *m, const vector4 *v)
{
    __m256d r = _mm256_mul_pd(_mm256_loadu_pd(&m->v[0]), _mm256_set1_pd(v->x));
    r = _mm256_fmadd_pd(_mm256_loadu_pd(&m->v[4]), _mm256_set1_pd(v->y), r);
    r = _mm256_fmadd_pd(_mm256_loadu_pd(&m->v[8]), _mm256_set1_pd(v->z), r);
    r = _mm256_fmadd_pd(_mm256_loadu_pd(&m->v[12]), _mm256_set1_pd(v->w), r);
    _mm256_storeu_pd(res->v, r);
}

void mat4_mul_vec3_ptr_avx512(vector3 *res, const matrix4 *m, const vector3 *v, const vm_float_t w)
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

#else /* float: one ZMM holds the whole matrix */

void mat4_mul_ptr_avx512(matrix4 *res, const matrix4 *a, const matrix4 *b)
{
    const __m512 A = _mm512_loadu_ps(a->v);
    const __m512 B = _mm512_loadu_ps(b->v);

    /* C.col[j] += A.col[k] * B[k,j]  for k = 0..3, all four columns at once */
    __m512 acc = _mm512_mul_ps(
        _mm512_permutexvar_ps(_mm512_setr_epi32(
            0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3), A),
        _mm512_permutexvar_ps(_mm512_setr_epi32(
            0, 0, 0, 0, 4, 4, 4, 4, 8, 8, 8, 8, 12, 12, 12, 12), B));

    acc = _mm512_fmadd_ps(
        _mm512_permutexvar_ps(_mm512_setr_epi32(
            4, 5, 6, 7, 4, 5, 6, 7, 4, 5, 6, 7, 4, 5, 6, 7), A),
        _mm512_permutexvar_ps(_mm512_setr_epi32(
            1, 1, 1, 1, 5, 5, 5, 5, 9, 9, 9, 9, 13, 13, 13, 13), B),
        acc);

    acc = _mm512_fmadd_ps(
        _mm512_permutexvar_ps(_mm512_setr_epi32(
            8, 9, 10, 11, 8, 9, 10, 11, 8, 9, 10, 11, 8, 9, 10, 11), A),
        _mm512_permutexvar_ps(_mm512_setr_epi32(
            2, 2, 2, 2, 6, 6, 6, 6, 10, 10, 10, 10, 14, 14, 14, 14), B),
        acc);

    acc = _mm512_fmadd_ps(
        _mm512_permutexvar_ps(_mm512_setr_epi32(
            12, 13, 14, 15, 12, 13, 14, 15, 12, 13, 14, 15, 12, 13, 14, 15), A),
        _mm512_permutexvar_ps(_mm512_setr_epi32(
            3, 3, 3, 3, 7, 7, 7, 7, 11, 11, 11, 11, 15, 15, 15, 15), B),
        acc);

    matrix4 tmp;
    _mm512_storeu_ps(tmp.v, acc);
    memcpy(res->v, tmp.v, sizeof(tmp.v));
}

void mat4_transpose_ptr_avx512(matrix4 *res, const matrix4 *m)
{
    const __m512 t = _mm512_permutexvar_ps(
        _mm512_setr_epi32(0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15),
        _mm512_loadu_ps(m->v));
    matrix4 tmp;
    _mm512_storeu_ps(tmp.v, t);
    memcpy(res->v, tmp.v, sizeof(tmp.v));
}

void mat4_mul_vec4_ptr_avx512(vector4 *res, const matrix4 *m, const vector4 *v)
{
    __m128 r = _mm_mul_ps(_mm_loadu_ps(&m->v[0]), _mm_set1_ps(v->x));
    r = _mm_fmadd_ps(_mm_loadu_ps(&m->v[4]), _mm_set1_ps(v->y), r);
    r = _mm_fmadd_ps(_mm_loadu_ps(&m->v[8]), _mm_set1_ps(v->z), r);
    r = _mm_fmadd_ps(_mm_loadu_ps(&m->v[12]), _mm_set1_ps(v->w), r);
    _mm_storeu_ps(res->v, r);
}

void mat4_mul_vec3_ptr_avx512(vector3 *res, const matrix4 *m, const vector3 *v, const vm_float_t w)
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
