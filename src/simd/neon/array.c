// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

/**
 * @file
 * @brief NEON elementwise array kernels.
 *
 * f32 is 4-wide, f64 is 2-wide. Unaligned loads and stores. Exact alias is
 * safe because each element is written in order. `vm_axpy` is a multiply
 * then an add, not a fused multiply-add.
 */

#include <arm_neon.h>
#include <vecmat.h>

#if defined(VECMAT_USE_F64)
#define LANES 2
typedef float64x2_t vec_t;
#define vload(p)   vld1q_f64((p))
#define vstore(p, v) vst1q_f64((p), (v))
#define vadd(a, b) vaddq_f64((a), (b))
#define vsub(a, b) vsubq_f64((a), (b))
#define vmul(a, b) vmulq_f64((a), (b))
#define vdiv(a, b) vdivq_f64((a), (b))
#define vfma(a, b, c) vfmaq_f64((c), (a), (b))
#define vset1(x)   vdupq_n_f64((x))
#else
#define LANES 4
typedef float32x4_t vec_t;
#define vload(p)   vld1q_f32((p))
#define vstore(p, v) vst1q_f32((p), (v))
#define vadd(a, b) vaddq_f32((a), (b))
#define vsub(a, b) vsubq_f32((a), (b))
#define vmul(a, b) vmulq_f32((a), (b))
#define vdiv(a, b) vdivq_f32((a), (b))
#define vfma(a, b, c) vfmaq_f32((c), (a), (b))
#define vset1(x)   vdupq_n_f32((x))
#endif

void vm_array_add_neon(vm_float_t *res, const vm_float_t *a, const vm_float_t *b, const int n)
{
    int i = 0;
    for (; i + LANES <= n; i += LANES) {
        vstore(res + i, vadd(vload(a + i), vload(b + i)));
    }
    for (; i < n; ++i) {
        res[i] = a[i] + b[i];
    }
}

void vm_array_sub_neon(vm_float_t *res, const vm_float_t *a, const vm_float_t *b, const int n)
{
    int i = 0;
    for (; i + LANES <= n; i += LANES) {
        vstore(res + i, vsub(vload(a + i), vload(b + i)));
    }
    for (; i < n; ++i) {
        res[i] = a[i] - b[i];
    }
}

void vm_array_mul_neon(vm_float_t *res, const vm_float_t *a, const vm_float_t *b, const int n)
{
    int i = 0;
    for (; i + LANES <= n; i += LANES) {
        vstore(res + i, vmul(vload(a + i), vload(b + i)));
    }
    for (; i < n; ++i) {
        res[i] = a[i] * b[i];
    }
}

void vm_array_div_neon(vm_float_t *res, const vm_float_t *a, const vm_float_t *b, const int n)
{
    int i = 0;
    for (; i + LANES <= n; i += LANES) {
        vstore(res + i, vdiv(vload(a + i), vload(b + i)));
    }
    for (; i < n; ++i) {
        res[i] = a[i] / b[i];
    }
}

void vm_array_fma_neon(vm_float_t *res, const vm_float_t *a, const vm_float_t *b,
                       const vm_float_t *c, const int n)
{
    int i = 0;
    for (; i + LANES <= n; i += LANES) {
        vstore(res + i, vfma(vload(a + i), vload(b + i), vload(c + i)));
    }
    for (; i < n; ++i) {
        res[i] = VECMAT_FMA(a[i], b[i], c[i]);
    }
}

void vm_axpy_neon(vm_float_t *y, const vm_float_t alpha, const vm_float_t *x, const int n)
{
    const vec_t scale = vset1(alpha);
    int i = 0;
    for (; i + LANES <= n; i += LANES) {
        const vec_t term = vmul(scale, vload(x + i));
        vstore(y + i, vadd(vload(y + i), term));
    }
    for (; i < n; ++i) {
        y[i] = y[i] + alpha * x[i];
    }
}
