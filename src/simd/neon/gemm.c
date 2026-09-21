// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <arm_neon.h>
#include <vecmat.h>

#if defined(VECMAT_USE_F64)

void vm_gemm_ukernel_neon(vm_float_t *acc, const vm_float_t *Ap, const vm_float_t *Bp, const int K)
{
    float64x2_t c0a = vld1q_f64(acc + 0),  c0b = vld1q_f64(acc + 2);
    float64x2_t c0c = vld1q_f64(acc + 4),  c0d = vld1q_f64(acc + 6);
    float64x2_t c1a = vld1q_f64(acc + 8),  c1b = vld1q_f64(acc + 10);
    float64x2_t c1c = vld1q_f64(acc + 12), c1d = vld1q_f64(acc + 14);
    float64x2_t c2a = vld1q_f64(acc + 16), c2b = vld1q_f64(acc + 18);
    float64x2_t c2c = vld1q_f64(acc + 20), c2d = vld1q_f64(acc + 22);
    float64x2_t c3a = vld1q_f64(acc + 24), c3b = vld1q_f64(acc + 26);
    float64x2_t c3c = vld1q_f64(acc + 28), c3d = vld1q_f64(acc + 30);
    float64x2_t c4a = vld1q_f64(acc + 32), c4b = vld1q_f64(acc + 34);
    float64x2_t c4c = vld1q_f64(acc + 36), c4d = vld1q_f64(acc + 38);
    float64x2_t c5a = vld1q_f64(acc + 40), c5b = vld1q_f64(acc + 42);
    float64x2_t c5c = vld1q_f64(acc + 44), c5d = vld1q_f64(acc + 46);
    float64x2_t c6a = vld1q_f64(acc + 48), c6b = vld1q_f64(acc + 50);
    float64x2_t c6c = vld1q_f64(acc + 52), c6d = vld1q_f64(acc + 54);
    float64x2_t c7a = vld1q_f64(acc + 56), c7b = vld1q_f64(acc + 58);
    float64x2_t c7c = vld1q_f64(acc + 60), c7d = vld1q_f64(acc + 62);

    for (int k = 0; k < K; ++k) {
        const float64x2_t aa = vld1q_f64(Ap + k * 8 + 0);
        const float64x2_t ab = vld1q_f64(Ap + k * 8 + 2);
        const float64x2_t ac = vld1q_f64(Ap + k * 8 + 4);
        const float64x2_t ad = vld1q_f64(Ap + k * 8 + 6);
        float64x2_t b;
        b = vdupq_n_f64(Bp[k * 8 + 0]);
        c0a = vfmaq_f64(c0a, aa, b); c0b = vfmaq_f64(c0b, ab, b);
        c0c = vfmaq_f64(c0c, ac, b); c0d = vfmaq_f64(c0d, ad, b);
        b = vdupq_n_f64(Bp[k * 8 + 1]);
        c1a = vfmaq_f64(c1a, aa, b); c1b = vfmaq_f64(c1b, ab, b);
        c1c = vfmaq_f64(c1c, ac, b); c1d = vfmaq_f64(c1d, ad, b);
        b = vdupq_n_f64(Bp[k * 8 + 2]);
        c2a = vfmaq_f64(c2a, aa, b); c2b = vfmaq_f64(c2b, ab, b);
        c2c = vfmaq_f64(c2c, ac, b); c2d = vfmaq_f64(c2d, ad, b);
        b = vdupq_n_f64(Bp[k * 8 + 3]);
        c3a = vfmaq_f64(c3a, aa, b); c3b = vfmaq_f64(c3b, ab, b);
        c3c = vfmaq_f64(c3c, ac, b); c3d = vfmaq_f64(c3d, ad, b);
        b = vdupq_n_f64(Bp[k * 8 + 4]);
        c4a = vfmaq_f64(c4a, aa, b); c4b = vfmaq_f64(c4b, ab, b);
        c4c = vfmaq_f64(c4c, ac, b); c4d = vfmaq_f64(c4d, ad, b);
        b = vdupq_n_f64(Bp[k * 8 + 5]);
        c5a = vfmaq_f64(c5a, aa, b); c5b = vfmaq_f64(c5b, ab, b);
        c5c = vfmaq_f64(c5c, ac, b); c5d = vfmaq_f64(c5d, ad, b);
        b = vdupq_n_f64(Bp[k * 8 + 6]);
        c6a = vfmaq_f64(c6a, aa, b); c6b = vfmaq_f64(c6b, ab, b);
        c6c = vfmaq_f64(c6c, ac, b); c6d = vfmaq_f64(c6d, ad, b);
        b = vdupq_n_f64(Bp[k * 8 + 7]);
        c7a = vfmaq_f64(c7a, aa, b); c7b = vfmaq_f64(c7b, ab, b);
        c7c = vfmaq_f64(c7c, ac, b); c7d = vfmaq_f64(c7d, ad, b);
    }

    vst1q_f64(acc + 0, c0a);  vst1q_f64(acc + 2, c0b);
    vst1q_f64(acc + 4, c0c);  vst1q_f64(acc + 6, c0d);
    vst1q_f64(acc + 8, c1a);  vst1q_f64(acc + 10, c1b);
    vst1q_f64(acc + 12, c1c); vst1q_f64(acc + 14, c1d);
    vst1q_f64(acc + 16, c2a); vst1q_f64(acc + 18, c2b);
    vst1q_f64(acc + 20, c2c); vst1q_f64(acc + 22, c2d);
    vst1q_f64(acc + 24, c3a); vst1q_f64(acc + 26, c3b);
    vst1q_f64(acc + 28, c3c); vst1q_f64(acc + 30, c3d);
    vst1q_f64(acc + 32, c4a); vst1q_f64(acc + 34, c4b);
    vst1q_f64(acc + 36, c4c); vst1q_f64(acc + 38, c4d);
    vst1q_f64(acc + 40, c5a); vst1q_f64(acc + 42, c5b);
    vst1q_f64(acc + 44, c5c); vst1q_f64(acc + 46, c5d);
    vst1q_f64(acc + 48, c6a); vst1q_f64(acc + 50, c6b);
    vst1q_f64(acc + 52, c6c); vst1q_f64(acc + 54, c6d);
    vst1q_f64(acc + 56, c7a); vst1q_f64(acc + 58, c7b);
    vst1q_f64(acc + 60, c7c); vst1q_f64(acc + 62, c7d);
}

#else /* float: 8-wide as two 128-bit ASIMD registers */

void vm_gemm_ukernel_neon(vm_float_t *acc, const vm_float_t *Ap, const vm_float_t *Bp, const int K)
{
    float32x4_t c0l = vld1q_f32(acc + 0),  c0h = vld1q_f32(acc + 4);
    float32x4_t c1l = vld1q_f32(acc + 8),  c1h = vld1q_f32(acc + 12);
    float32x4_t c2l = vld1q_f32(acc + 16), c2h = vld1q_f32(acc + 20);
    float32x4_t c3l = vld1q_f32(acc + 24), c3h = vld1q_f32(acc + 28);
    float32x4_t c4l = vld1q_f32(acc + 32), c4h = vld1q_f32(acc + 36);
    float32x4_t c5l = vld1q_f32(acc + 40), c5h = vld1q_f32(acc + 44);
    float32x4_t c6l = vld1q_f32(acc + 48), c6h = vld1q_f32(acc + 52);
    float32x4_t c7l = vld1q_f32(acc + 56), c7h = vld1q_f32(acc + 60);

    for (int k = 0; k < K; ++k) {
        const float32x4_t al = vld1q_f32(Ap + k * 8);
        const float32x4_t ah = vld1q_f32(Ap + k * 8 + 4);
        float32x4_t b;
        b = vdupq_n_f32(Bp[k * 8 + 0]); c0l = vfmaq_f32(c0l, al, b); c0h = vfmaq_f32(c0h, ah, b);
        b = vdupq_n_f32(Bp[k * 8 + 1]); c1l = vfmaq_f32(c1l, al, b); c1h = vfmaq_f32(c1h, ah, b);
        b = vdupq_n_f32(Bp[k * 8 + 2]); c2l = vfmaq_f32(c2l, al, b); c2h = vfmaq_f32(c2h, ah, b);
        b = vdupq_n_f32(Bp[k * 8 + 3]); c3l = vfmaq_f32(c3l, al, b); c3h = vfmaq_f32(c3h, ah, b);
        b = vdupq_n_f32(Bp[k * 8 + 4]); c4l = vfmaq_f32(c4l, al, b); c4h = vfmaq_f32(c4h, ah, b);
        b = vdupq_n_f32(Bp[k * 8 + 5]); c5l = vfmaq_f32(c5l, al, b); c5h = vfmaq_f32(c5h, ah, b);
        b = vdupq_n_f32(Bp[k * 8 + 6]); c6l = vfmaq_f32(c6l, al, b); c6h = vfmaq_f32(c6h, ah, b);
        b = vdupq_n_f32(Bp[k * 8 + 7]); c7l = vfmaq_f32(c7l, al, b); c7h = vfmaq_f32(c7h, ah, b);
    }

    vst1q_f32(acc + 0, c0l);  vst1q_f32(acc + 4, c0h);
    vst1q_f32(acc + 8, c1l);  vst1q_f32(acc + 12, c1h);
    vst1q_f32(acc + 16, c2l); vst1q_f32(acc + 20, c2h);
    vst1q_f32(acc + 24, c3l); vst1q_f32(acc + 28, c3h);
    vst1q_f32(acc + 32, c4l); vst1q_f32(acc + 36, c4h);
    vst1q_f32(acc + 40, c5l); vst1q_f32(acc + 44, c5h);
    vst1q_f32(acc + 48, c6l); vst1q_f32(acc + 52, c6h);
    vst1q_f32(acc + 56, c7l); vst1q_f32(acc + 60, c7h);
}

#endif
