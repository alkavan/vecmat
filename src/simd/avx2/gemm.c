// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <immintrin.h>
#include <vecmat.h>

#if defined(VECMAT_USE_F64)

void vm_gemm_ukernel_avx2(vm_float_t *acc, const vm_float_t *Ap, const vm_float_t *Bp, const int K)
{
    __m256d c0l = _mm256_loadu_pd(acc + 0),  c0h = _mm256_loadu_pd(acc + 4);
    __m256d c1l = _mm256_loadu_pd(acc + 8),  c1h = _mm256_loadu_pd(acc + 12);
    __m256d c2l = _mm256_loadu_pd(acc + 16), c2h = _mm256_loadu_pd(acc + 20);
    __m256d c3l = _mm256_loadu_pd(acc + 24), c3h = _mm256_loadu_pd(acc + 28);
    __m256d c4l = _mm256_loadu_pd(acc + 32), c4h = _mm256_loadu_pd(acc + 36);
    __m256d c5l = _mm256_loadu_pd(acc + 40), c5h = _mm256_loadu_pd(acc + 44);
    __m256d c6l = _mm256_loadu_pd(acc + 48), c6h = _mm256_loadu_pd(acc + 52);
    __m256d c7l = _mm256_loadu_pd(acc + 56), c7h = _mm256_loadu_pd(acc + 60);
    int k = 0;
    for (; k + 1 < K; k += 2) {
        const __m256d a0l = _mm256_loadu_pd(Ap + k * 8);
        const __m256d a0h = _mm256_loadu_pd(Ap + k * 8 + 4);
        const __m256d a1l = _mm256_loadu_pd(Ap + (k + 1) * 8);
        const __m256d a1h = _mm256_loadu_pd(Ap + (k + 1) * 8 + 4);
        const vm_float_t *b0 = Bp + k * 8;
        const vm_float_t *b1 = Bp + (k + 1) * 8;
        c0l = _mm256_fmadd_pd(a0l, _mm256_broadcast_sd(b0 + 0), c0l);
        c0h = _mm256_fmadd_pd(a0h, _mm256_broadcast_sd(b0 + 0), c0h);
        c1l = _mm256_fmadd_pd(a0l, _mm256_broadcast_sd(b0 + 1), c1l);
        c1h = _mm256_fmadd_pd(a0h, _mm256_broadcast_sd(b0 + 1), c1h);
        c2l = _mm256_fmadd_pd(a0l, _mm256_broadcast_sd(b0 + 2), c2l);
        c2h = _mm256_fmadd_pd(a0h, _mm256_broadcast_sd(b0 + 2), c2h);
        c3l = _mm256_fmadd_pd(a0l, _mm256_broadcast_sd(b0 + 3), c3l);
        c3h = _mm256_fmadd_pd(a0h, _mm256_broadcast_sd(b0 + 3), c3h);
        c4l = _mm256_fmadd_pd(a0l, _mm256_broadcast_sd(b0 + 4), c4l);
        c4h = _mm256_fmadd_pd(a0h, _mm256_broadcast_sd(b0 + 4), c4h);
        c5l = _mm256_fmadd_pd(a0l, _mm256_broadcast_sd(b0 + 5), c5l);
        c5h = _mm256_fmadd_pd(a0h, _mm256_broadcast_sd(b0 + 5), c5h);
        c6l = _mm256_fmadd_pd(a0l, _mm256_broadcast_sd(b0 + 6), c6l);
        c6h = _mm256_fmadd_pd(a0h, _mm256_broadcast_sd(b0 + 6), c6h);
        c7l = _mm256_fmadd_pd(a0l, _mm256_broadcast_sd(b0 + 7), c7l);
        c7h = _mm256_fmadd_pd(a0h, _mm256_broadcast_sd(b0 + 7), c7h);
        c0l = _mm256_fmadd_pd(a1l, _mm256_broadcast_sd(b1 + 0), c0l);
        c0h = _mm256_fmadd_pd(a1h, _mm256_broadcast_sd(b1 + 0), c0h);
        c1l = _mm256_fmadd_pd(a1l, _mm256_broadcast_sd(b1 + 1), c1l);
        c1h = _mm256_fmadd_pd(a1h, _mm256_broadcast_sd(b1 + 1), c1h);
        c2l = _mm256_fmadd_pd(a1l, _mm256_broadcast_sd(b1 + 2), c2l);
        c2h = _mm256_fmadd_pd(a1h, _mm256_broadcast_sd(b1 + 2), c2h);
        c3l = _mm256_fmadd_pd(a1l, _mm256_broadcast_sd(b1 + 3), c3l);
        c3h = _mm256_fmadd_pd(a1h, _mm256_broadcast_sd(b1 + 3), c3h);
        c4l = _mm256_fmadd_pd(a1l, _mm256_broadcast_sd(b1 + 4), c4l);
        c4h = _mm256_fmadd_pd(a1h, _mm256_broadcast_sd(b1 + 4), c4h);
        c5l = _mm256_fmadd_pd(a1l, _mm256_broadcast_sd(b1 + 5), c5l);
        c5h = _mm256_fmadd_pd(a1h, _mm256_broadcast_sd(b1 + 5), c5h);
        c6l = _mm256_fmadd_pd(a1l, _mm256_broadcast_sd(b1 + 6), c6l);
        c6h = _mm256_fmadd_pd(a1h, _mm256_broadcast_sd(b1 + 6), c6h);
        c7l = _mm256_fmadd_pd(a1l, _mm256_broadcast_sd(b1 + 7), c7l);
        c7h = _mm256_fmadd_pd(a1h, _mm256_broadcast_sd(b1 + 7), c7h);
    }
    for (; k < K; ++k) {
        const __m256d al = _mm256_loadu_pd(Ap + k * 8);
        const __m256d ah = _mm256_loadu_pd(Ap + k * 8 + 4);
        const vm_float_t *b = Bp + k * 8;
        c0l = _mm256_fmadd_pd(al, _mm256_broadcast_sd(b + 0), c0l);
        c0h = _mm256_fmadd_pd(ah, _mm256_broadcast_sd(b + 0), c0h);
        c1l = _mm256_fmadd_pd(al, _mm256_broadcast_sd(b + 1), c1l);
        c1h = _mm256_fmadd_pd(ah, _mm256_broadcast_sd(b + 1), c1h);
        c2l = _mm256_fmadd_pd(al, _mm256_broadcast_sd(b + 2), c2l);
        c2h = _mm256_fmadd_pd(ah, _mm256_broadcast_sd(b + 2), c2h);
        c3l = _mm256_fmadd_pd(al, _mm256_broadcast_sd(b + 3), c3l);
        c3h = _mm256_fmadd_pd(ah, _mm256_broadcast_sd(b + 3), c3h);
        c4l = _mm256_fmadd_pd(al, _mm256_broadcast_sd(b + 4), c4l);
        c4h = _mm256_fmadd_pd(ah, _mm256_broadcast_sd(b + 4), c4h);
        c5l = _mm256_fmadd_pd(al, _mm256_broadcast_sd(b + 5), c5l);
        c5h = _mm256_fmadd_pd(ah, _mm256_broadcast_sd(b + 5), c5h);
        c6l = _mm256_fmadd_pd(al, _mm256_broadcast_sd(b + 6), c6l);
        c6h = _mm256_fmadd_pd(ah, _mm256_broadcast_sd(b + 6), c6h);
        c7l = _mm256_fmadd_pd(al, _mm256_broadcast_sd(b + 7), c7l);
        c7h = _mm256_fmadd_pd(ah, _mm256_broadcast_sd(b + 7), c7h);
    }
    _mm256_storeu_pd(acc + 0, c0l);  _mm256_storeu_pd(acc + 4, c0h);
    _mm256_storeu_pd(acc + 8, c1l);  _mm256_storeu_pd(acc + 12, c1h);
    _mm256_storeu_pd(acc + 16, c2l); _mm256_storeu_pd(acc + 20, c2h);
    _mm256_storeu_pd(acc + 24, c3l); _mm256_storeu_pd(acc + 28, c3h);
    _mm256_storeu_pd(acc + 32, c4l); _mm256_storeu_pd(acc + 36, c4h);
    _mm256_storeu_pd(acc + 40, c5l); _mm256_storeu_pd(acc + 44, c5h);
    _mm256_storeu_pd(acc + 48, c6l); _mm256_storeu_pd(acc + 52, c6h);
    _mm256_storeu_pd(acc + 56, c7l); _mm256_storeu_pd(acc + 60, c7h);
}

#else /* float: 8-wide AVX2 + FMA */

void vm_gemm_ukernel_avx2(vm_float_t *acc, const vm_float_t *Ap, const vm_float_t *Bp, const int K)
{
    __m256 c0 = _mm256_loadu_ps(acc + 0);
    __m256 c1 = _mm256_loadu_ps(acc + 8);
    __m256 c2 = _mm256_loadu_ps(acc + 16);
    __m256 c3 = _mm256_loadu_ps(acc + 24);
    __m256 c4 = _mm256_loadu_ps(acc + 32);
    __m256 c5 = _mm256_loadu_ps(acc + 40);
    __m256 c6 = _mm256_loadu_ps(acc + 48);
    __m256 c7 = _mm256_loadu_ps(acc + 56);
    int k = 0;
    for (; k + 1 < K; k += 2) {
        const __m256 a0 = _mm256_loadu_ps(Ap + k * 8);
        const __m256 a1 = _mm256_loadu_ps(Ap + (k + 1) * 8);
        const vm_float_t *b0 = Bp + k * 8;
        const vm_float_t *b1 = Bp + (k + 1) * 8;
        c0 = _mm256_fmadd_ps(a0, _mm256_broadcast_ss(b0 + 0), c0);
        c1 = _mm256_fmadd_ps(a0, _mm256_broadcast_ss(b0 + 1), c1);
        c2 = _mm256_fmadd_ps(a0, _mm256_broadcast_ss(b0 + 2), c2);
        c3 = _mm256_fmadd_ps(a0, _mm256_broadcast_ss(b0 + 3), c3);
        c4 = _mm256_fmadd_ps(a0, _mm256_broadcast_ss(b0 + 4), c4);
        c5 = _mm256_fmadd_ps(a0, _mm256_broadcast_ss(b0 + 5), c5);
        c6 = _mm256_fmadd_ps(a0, _mm256_broadcast_ss(b0 + 6), c6);
        c7 = _mm256_fmadd_ps(a0, _mm256_broadcast_ss(b0 + 7), c7);
        c0 = _mm256_fmadd_ps(a1, _mm256_broadcast_ss(b1 + 0), c0);
        c1 = _mm256_fmadd_ps(a1, _mm256_broadcast_ss(b1 + 1), c1);
        c2 = _mm256_fmadd_ps(a1, _mm256_broadcast_ss(b1 + 2), c2);
        c3 = _mm256_fmadd_ps(a1, _mm256_broadcast_ss(b1 + 3), c3);
        c4 = _mm256_fmadd_ps(a1, _mm256_broadcast_ss(b1 + 4), c4);
        c5 = _mm256_fmadd_ps(a1, _mm256_broadcast_ss(b1 + 5), c5);
        c6 = _mm256_fmadd_ps(a1, _mm256_broadcast_ss(b1 + 6), c6);
        c7 = _mm256_fmadd_ps(a1, _mm256_broadcast_ss(b1 + 7), c7);
    }
    for (; k < K; ++k) {
        const __m256 a = _mm256_loadu_ps(Ap + k * 8);
        const vm_float_t *b = Bp + k * 8;
        c0 = _mm256_fmadd_ps(a, _mm256_broadcast_ss(b + 0), c0);
        c1 = _mm256_fmadd_ps(a, _mm256_broadcast_ss(b + 1), c1);
        c2 = _mm256_fmadd_ps(a, _mm256_broadcast_ss(b + 2), c2);
        c3 = _mm256_fmadd_ps(a, _mm256_broadcast_ss(b + 3), c3);
        c4 = _mm256_fmadd_ps(a, _mm256_broadcast_ss(b + 4), c4);
        c5 = _mm256_fmadd_ps(a, _mm256_broadcast_ss(b + 5), c5);
        c6 = _mm256_fmadd_ps(a, _mm256_broadcast_ss(b + 6), c6);
        c7 = _mm256_fmadd_ps(a, _mm256_broadcast_ss(b + 7), c7);
    }
    _mm256_storeu_ps(acc + 0, c0);
    _mm256_storeu_ps(acc + 8, c1);
    _mm256_storeu_ps(acc + 16, c2);
    _mm256_storeu_ps(acc + 24, c3);
    _mm256_storeu_ps(acc + 32, c4);
    _mm256_storeu_ps(acc + 40, c5);
    _mm256_storeu_ps(acc + 48, c6);
    _mm256_storeu_ps(acc + 56, c7);
}

#endif
