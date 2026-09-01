// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <immintrin.h>
#include <string.h>
#include <vecmat.h>

#if defined(VECMAT_USE_F64)

/* One f64 column is 256 bits. Duplicate it into both halves of a ZMM so two
 * result columns can FMA in one instruction. */
static inline __m512d vm_avx512_dupcol(const __m256d col)
{
    return _mm512_insertf64x4(_mm512_castpd256_pd512(col), col, 1);
}

static inline __m512d vm_avx512_dup2(const double s0, const double s1)
{
    return _mm512_setr_pd(s0, s0, s0, s0, s1, s1, s1, s1);
}

void mat4_mul_ptr_avx512(matrix4 *res, const matrix4 *a, const matrix4 *b)
{
    matrix4 tmp;
    const __m512d a0 = vm_avx512_dupcol(_mm256_loadu_pd(&a->v[0]));
    const __m512d a1 = vm_avx512_dupcol(_mm256_loadu_pd(&a->v[4]));
    const __m512d a2 = vm_avx512_dupcol(_mm256_loadu_pd(&a->v[8]));
    const __m512d a3 = vm_avx512_dupcol(_mm256_loadu_pd(&a->v[12]));

    /* C.col[j] and C.col[j+1] packed in one ZMM. */
    for (int c = 0; c < 4; c += 2) {
        __m512d acc = _mm512_mul_pd(a0, vm_avx512_dup2(b->v[c * 4 + 0], b->v[(c + 1) * 4 + 0]));
        acc = _mm512_fmadd_pd(a1, vm_avx512_dup2(b->v[c * 4 + 1], b->v[(c + 1) * 4 + 1]), acc);
        acc = _mm512_fmadd_pd(a2, vm_avx512_dup2(b->v[c * 4 + 2], b->v[(c + 1) * 4 + 2]), acc);
        acc = _mm512_fmadd_pd(a3, vm_avx512_dup2(b->v[c * 4 + 3], b->v[(c + 1) * 4 + 3]), acc);
        _mm512_storeu_pd(&tmp.v[c * 4], acc);
    }
    memcpy(res->v, tmp.v, sizeof(tmp.v));
}

void mat4_transpose_ptr_avx512(matrix4 *res, const matrix4 *m)
{
    /* Two ZMMs hold the whole 4×4. Index bit 3 picks lo vs hi. */
    const __m512d lo = _mm512_loadu_pd(&m->v[0]);
    const __m512d hi = _mm512_loadu_pd(&m->v[8]);
    const __m512d out_lo = _mm512_permutex2var_pd(
        lo, _mm512_setr_epi64(0, 4, 8, 12, 1, 5, 9, 13), hi);
    const __m512d out_hi = _mm512_permutex2var_pd(
        lo, _mm512_setr_epi64(2, 6, 10, 14, 3, 7, 11, 15), hi);
    matrix4 tmp;
    _mm512_storeu_pd(&tmp.v[0], out_lo);
    _mm512_storeu_pd(&tmp.v[8], out_hi);
    memcpy(res->v, tmp.v, sizeof(tmp.v));
}

/* 4 f64 lanes = one YMM; a ZMM would be half unused. */
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
