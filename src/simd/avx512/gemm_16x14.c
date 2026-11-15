// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <immintrin.h>
#include <vecmat.h>

#if defined(VECMAT_USE_F64)

/**
 * @brief AVX-512 16×14 packed micro-kernel (f64).
 *
 * `acc` is column-major with stride 16. `Ap` has 16 values per k and `Bp`
 * has 14. Rows 0–7 and rows 8–15 are separate passes.
 *
 * @param acc Accumulator tile (16×14), updated in place.
 * @param Ap  Packed A panel, 16 values per k.
 * @param Bp  Packed B panel, 14 values per k.
 * @param K   Number of packed inner-dimension steps.
 */
void vm_gemm_ukernel_avx512_16x14(vm_float_t *acc, const vm_float_t *Ap,
                                  const vm_float_t *Bp, const int K)
{
    __m512d c0 = _mm512_loadu_pd(acc + 0);
    __m512d c1 = _mm512_loadu_pd(acc + 16);
    __m512d c2 = _mm512_loadu_pd(acc + 32);
    __m512d c3 = _mm512_loadu_pd(acc + 48);
    __m512d c4 = _mm512_loadu_pd(acc + 64);
    __m512d c5 = _mm512_loadu_pd(acc + 80);
    __m512d c6 = _mm512_loadu_pd(acc + 96);
    __m512d c7 = _mm512_loadu_pd(acc + 112);
    __m512d c8 = _mm512_loadu_pd(acc + 128);
    __m512d c9 = _mm512_loadu_pd(acc + 144);
    __m512d c10 = _mm512_loadu_pd(acc + 160);
    __m512d c11 = _mm512_loadu_pd(acc + 176);
    __m512d c12 = _mm512_loadu_pd(acc + 192);
    __m512d c13 = _mm512_loadu_pd(acc + 208);
    int k = 0;
    for (; k < K; ++k) {
        const __m512d a = _mm512_loadu_pd(Ap + k * 16);
        const vm_float_t *b = Bp + k * 14;
        c0 = _mm512_fmadd_pd(a, _mm512_set1_pd(b[0]), c0);
        c1 = _mm512_fmadd_pd(a, _mm512_set1_pd(b[1]), c1);
        c2 = _mm512_fmadd_pd(a, _mm512_set1_pd(b[2]), c2);
        c3 = _mm512_fmadd_pd(a, _mm512_set1_pd(b[3]), c3);
        c4 = _mm512_fmadd_pd(a, _mm512_set1_pd(b[4]), c4);
        c5 = _mm512_fmadd_pd(a, _mm512_set1_pd(b[5]), c5);
        c6 = _mm512_fmadd_pd(a, _mm512_set1_pd(b[6]), c6);
        c7 = _mm512_fmadd_pd(a, _mm512_set1_pd(b[7]), c7);
        c8 = _mm512_fmadd_pd(a, _mm512_set1_pd(b[8]), c8);
        c9 = _mm512_fmadd_pd(a, _mm512_set1_pd(b[9]), c9);
        c10 = _mm512_fmadd_pd(a, _mm512_set1_pd(b[10]), c10);
        c11 = _mm512_fmadd_pd(a, _mm512_set1_pd(b[11]), c11);
        c12 = _mm512_fmadd_pd(a, _mm512_set1_pd(b[12]), c12);
        c13 = _mm512_fmadd_pd(a, _mm512_set1_pd(b[13]), c13);
    }
    _mm512_storeu_pd(acc + 0, c0);
    _mm512_storeu_pd(acc + 16, c1);
    _mm512_storeu_pd(acc + 32, c2);
    _mm512_storeu_pd(acc + 48, c3);
    _mm512_storeu_pd(acc + 64, c4);
    _mm512_storeu_pd(acc + 80, c5);
    _mm512_storeu_pd(acc + 96, c6);
    _mm512_storeu_pd(acc + 112, c7);
    _mm512_storeu_pd(acc + 128, c8);
    _mm512_storeu_pd(acc + 144, c9);
    _mm512_storeu_pd(acc + 160, c10);
    _mm512_storeu_pd(acc + 176, c11);
    _mm512_storeu_pd(acc + 192, c12);
    _mm512_storeu_pd(acc + 208, c13);

    c0 = _mm512_loadu_pd(acc + 8);
    c1 = _mm512_loadu_pd(acc + 24);
    c2 = _mm512_loadu_pd(acc + 40);
    c3 = _mm512_loadu_pd(acc + 56);
    c4 = _mm512_loadu_pd(acc + 72);
    c5 = _mm512_loadu_pd(acc + 88);
    c6 = _mm512_loadu_pd(acc + 104);
    c7 = _mm512_loadu_pd(acc + 120);
    c8 = _mm512_loadu_pd(acc + 136);
    c9 = _mm512_loadu_pd(acc + 152);
    c10 = _mm512_loadu_pd(acc + 168);
    c11 = _mm512_loadu_pd(acc + 184);
    c12 = _mm512_loadu_pd(acc + 200);
    c13 = _mm512_loadu_pd(acc + 216);
    for (k = 0; k < K; ++k) {
        const __m512d a = _mm512_loadu_pd(Ap + k * 16 + 8);
        const vm_float_t *b = Bp + k * 14;
        c0 = _mm512_fmadd_pd(a, _mm512_set1_pd(b[0]), c0);
        c1 = _mm512_fmadd_pd(a, _mm512_set1_pd(b[1]), c1);
        c2 = _mm512_fmadd_pd(a, _mm512_set1_pd(b[2]), c2);
        c3 = _mm512_fmadd_pd(a, _mm512_set1_pd(b[3]), c3);
        c4 = _mm512_fmadd_pd(a, _mm512_set1_pd(b[4]), c4);
        c5 = _mm512_fmadd_pd(a, _mm512_set1_pd(b[5]), c5);
        c6 = _mm512_fmadd_pd(a, _mm512_set1_pd(b[6]), c6);
        c7 = _mm512_fmadd_pd(a, _mm512_set1_pd(b[7]), c7);
        c8 = _mm512_fmadd_pd(a, _mm512_set1_pd(b[8]), c8);
        c9 = _mm512_fmadd_pd(a, _mm512_set1_pd(b[9]), c9);
        c10 = _mm512_fmadd_pd(a, _mm512_set1_pd(b[10]), c10);
        c11 = _mm512_fmadd_pd(a, _mm512_set1_pd(b[11]), c11);
        c12 = _mm512_fmadd_pd(a, _mm512_set1_pd(b[12]), c12);
        c13 = _mm512_fmadd_pd(a, _mm512_set1_pd(b[13]), c13);
    }
    _mm512_storeu_pd(acc + 8, c0);
    _mm512_storeu_pd(acc + 24, c1);
    _mm512_storeu_pd(acc + 40, c2);
    _mm512_storeu_pd(acc + 56, c3);
    _mm512_storeu_pd(acc + 72, c4);
    _mm512_storeu_pd(acc + 88, c5);
    _mm512_storeu_pd(acc + 104, c6);
    _mm512_storeu_pd(acc + 120, c7);
    _mm512_storeu_pd(acc + 136, c8);
    _mm512_storeu_pd(acc + 152, c9);
    _mm512_storeu_pd(acc + 168, c10);
    _mm512_storeu_pd(acc + 184, c11);
    _mm512_storeu_pd(acc + 200, c12);
    _mm512_storeu_pd(acc + 216, c13);
}

#else

/**
 * @brief AVX-512 16×14 packed micro-kernel (f32).
 *
 * `acc` is column-major with stride 16. One zmm holds a column. `Ap` has 16
 * values per k and `Bp` has 14.
 *
 * @param acc Accumulator tile (16×14), updated in place.
 * @param Ap  Packed A panel, 16 values per k.
 * @param Bp  Packed B panel, 14 values per k.
 * @param K   Number of packed inner-dimension steps.
 */
void vm_gemm_ukernel_avx512_16x14(vm_float_t *acc, const vm_float_t *Ap,
                                  const vm_float_t *Bp, const int K)
{
    __m512 c0 = _mm512_loadu_ps(acc + 0);
    __m512 c1 = _mm512_loadu_ps(acc + 16);
    __m512 c2 = _mm512_loadu_ps(acc + 32);
    __m512 c3 = _mm512_loadu_ps(acc + 48);
    __m512 c4 = _mm512_loadu_ps(acc + 64);
    __m512 c5 = _mm512_loadu_ps(acc + 80);
    __m512 c6 = _mm512_loadu_ps(acc + 96);
    __m512 c7 = _mm512_loadu_ps(acc + 112);
    __m512 c8 = _mm512_loadu_ps(acc + 128);
    __m512 c9 = _mm512_loadu_ps(acc + 144);
    __m512 c10 = _mm512_loadu_ps(acc + 160);
    __m512 c11 = _mm512_loadu_ps(acc + 176);
    __m512 c12 = _mm512_loadu_ps(acc + 192);
    __m512 c13 = _mm512_loadu_ps(acc + 208);
    for (int k = 0; k < K; ++k) {
        const __m512 a = _mm512_loadu_ps(Ap + k * 16);
        const vm_float_t *b = Bp + k * 14;
        c0 = _mm512_fmadd_ps(a, _mm512_set1_ps(b[0]), c0);
        c1 = _mm512_fmadd_ps(a, _mm512_set1_ps(b[1]), c1);
        c2 = _mm512_fmadd_ps(a, _mm512_set1_ps(b[2]), c2);
        c3 = _mm512_fmadd_ps(a, _mm512_set1_ps(b[3]), c3);
        c4 = _mm512_fmadd_ps(a, _mm512_set1_ps(b[4]), c4);
        c5 = _mm512_fmadd_ps(a, _mm512_set1_ps(b[5]), c5);
        c6 = _mm512_fmadd_ps(a, _mm512_set1_ps(b[6]), c6);
        c7 = _mm512_fmadd_ps(a, _mm512_set1_ps(b[7]), c7);
        c8 = _mm512_fmadd_ps(a, _mm512_set1_ps(b[8]), c8);
        c9 = _mm512_fmadd_ps(a, _mm512_set1_ps(b[9]), c9);
        c10 = _mm512_fmadd_ps(a, _mm512_set1_ps(b[10]), c10);
        c11 = _mm512_fmadd_ps(a, _mm512_set1_ps(b[11]), c11);
        c12 = _mm512_fmadd_ps(a, _mm512_set1_ps(b[12]), c12);
        c13 = _mm512_fmadd_ps(a, _mm512_set1_ps(b[13]), c13);
    }
    _mm512_storeu_ps(acc + 0, c0);
    _mm512_storeu_ps(acc + 16, c1);
    _mm512_storeu_ps(acc + 32, c2);
    _mm512_storeu_ps(acc + 48, c3);
    _mm512_storeu_ps(acc + 64, c4);
    _mm512_storeu_ps(acc + 80, c5);
    _mm512_storeu_ps(acc + 96, c6);
    _mm512_storeu_ps(acc + 112, c7);
    _mm512_storeu_ps(acc + 128, c8);
    _mm512_storeu_ps(acc + 144, c9);
    _mm512_storeu_ps(acc + 160, c10);
    _mm512_storeu_ps(acc + 176, c11);
    _mm512_storeu_ps(acc + 192, c12);
    _mm512_storeu_ps(acc + 208, c13);
}

#endif
