// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

/**
 * @file
 * @brief AVX2 elementwise array kernels.
 *
 * f32 is 8-wide, f64 is 4-wide. Unaligned loads and stores. Exact alias is
 * safe because each element is written in order. `vm_axpy` is a multiply
 * then an add, not a fused multiply-add.
 */

#include <immintrin.h>
#include <vecmat.h>

#if defined(VECMAT_USE_F64)
#define LANES 4
typedef __m256d vec_t;
#define vload(p)   _mm256_loadu_pd((p))
#define vstore(p, v) _mm256_storeu_pd((p), (v))
#define vadd(a, b) _mm256_add_pd((a), (b))
#define vsub(a, b) _mm256_sub_pd((a), (b))
#define vmul(a, b) _mm256_mul_pd((a), (b))
#define vdiv(a, b) _mm256_div_pd((a), (b))
#define vfma(a, b, c) _mm256_fmadd_pd((a), (b), (c))
#define vset1(x)   _mm256_set1_pd((x))
#else
#define LANES 8
typedef __m256 vec_t;
#define vload(p)   _mm256_loadu_ps((p))
#define vstore(p, v) _mm256_storeu_ps((p), (v))
#define vadd(a, b) _mm256_add_ps((a), (b))
#define vsub(a, b) _mm256_sub_ps((a), (b))
#define vmul(a, b) _mm256_mul_ps((a), (b))
#define vdiv(a, b) _mm256_div_ps((a), (b))
#define vfma(a, b, c) _mm256_fmadd_ps((a), (b), (c))
#define vset1(x)   _mm256_set1_ps((x))
#endif

void vm_array_add_avx2(vm_float_t *res, const vm_float_t *a, const vm_float_t *b, const int n)
{
    int i = 0;
    for (; i + LANES <= n; i += LANES) {
        vstore(res + i, vadd(vload(a + i), vload(b + i)));
    }
    for (; i < n; ++i) {
        res[i] = a[i] + b[i];
    }
}

void vm_array_sub_avx2(vm_float_t *res, const vm_float_t *a, const vm_float_t *b, const int n)
{
    int i = 0;
    for (; i + LANES <= n; i += LANES) {
        vstore(res + i, vsub(vload(a + i), vload(b + i)));
    }
    for (; i < n; ++i) {
        res[i] = a[i] - b[i];
    }
}

void vm_array_mul_avx2(vm_float_t *res, const vm_float_t *a, const vm_float_t *b, const int n)
{
    int i = 0;
    for (; i + LANES <= n; i += LANES) {
        vstore(res + i, vmul(vload(a + i), vload(b + i)));
    }
    for (; i < n; ++i) {
        res[i] = a[i] * b[i];
    }
}

void vm_array_div_avx2(vm_float_t *res, const vm_float_t *a, const vm_float_t *b, const int n)
{
    int i = 0;
    for (; i + LANES <= n; i += LANES) {
        vstore(res + i, vdiv(vload(a + i), vload(b + i)));
    }
    for (; i < n; ++i) {
        res[i] = a[i] / b[i];
    }
}

void vm_array_fma_avx2(vm_float_t *res, const vm_float_t *a, const vm_float_t *b,
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

void vm_axpy_avx2(vm_float_t *y, const vm_float_t alpha, const vm_float_t *x, const int n)
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
