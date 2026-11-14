// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <math.h>
#include <vecmat.h>

#if !defined(_MSC_VER)
#pragma STDC FP_CONTRACT OFF
#endif

#if defined(VECMAT_ENABLE_AVX2)
void vm_array_add_avx2(vm_float_t *res, const vm_float_t *a, const vm_float_t *b, int n);
void vm_array_sub_avx2(vm_float_t *res, const vm_float_t *a, const vm_float_t *b, int n);
void vm_array_mul_avx2(vm_float_t *res, const vm_float_t *a, const vm_float_t *b, int n);
void vm_array_div_avx2(vm_float_t *res, const vm_float_t *a, const vm_float_t *b, int n);
void vm_array_fma_avx2(vm_float_t *res, const vm_float_t *a, const vm_float_t *b,
                       const vm_float_t *c, int n);
void vm_axpy_avx2(vm_float_t *y, vm_float_t alpha, const vm_float_t *x, int n);
void vm_rsqrt_array_avx2(vm_float_t *res, const vm_float_t *x, int n);
#endif

#if defined(VECMAT_ENABLE_NEON)
void vm_array_add_neon(vm_float_t *res, const vm_float_t *a, const vm_float_t *b, int n);
void vm_array_sub_neon(vm_float_t *res, const vm_float_t *a, const vm_float_t *b, int n);
void vm_array_mul_neon(vm_float_t *res, const vm_float_t *a, const vm_float_t *b, int n);
void vm_array_div_neon(vm_float_t *res, const vm_float_t *a, const vm_float_t *b, int n);
void vm_array_fma_neon(vm_float_t *res, const vm_float_t *a, const vm_float_t *b,
                       const vm_float_t *c, int n);
void vm_axpy_neon(vm_float_t *y, vm_float_t alpha, const vm_float_t *x, int n);
void vm_rsqrt_array_neon(vm_float_t *res, const vm_float_t *x, int n);
#endif

/**
 * @brief Quiet NaN in the active width.
 */
static vm_float_t array_nan(void)
{
#ifdef VECMAT_USE_F64
    return nan("");
#else
    return nanf("");
#endif
}

void vm_array_add(vm_float_t *res, const vm_float_t *a, const vm_float_t *b, const int n)
{
    if (n <= 0 || !res || !a || !b) {
        return;
    }
#if defined(VECMAT_ENABLE_AVX2)
    if (vm_cpu_has(VM_LEVEL_AVX2)) {
        vm_array_add_avx2(res, a, b, n);
        return;
    }
#endif
#if defined(VECMAT_ENABLE_NEON)
    if (vm_cpu_has(VM_LEVEL_NEON)) {
        vm_array_add_neon(res, a, b, n);
        return;
    }
#endif
    for (int i = 0; i < n; ++i) {
        res[i] = a[i] + b[i];
    }
}

void vm_array_sub(vm_float_t *res, const vm_float_t *a, const vm_float_t *b, const int n)
{
    if (n <= 0 || !res || !a || !b) {
        return;
    }
#if defined(VECMAT_ENABLE_AVX2)
    if (vm_cpu_has(VM_LEVEL_AVX2)) {
        vm_array_sub_avx2(res, a, b, n);
        return;
    }
#endif
#if defined(VECMAT_ENABLE_NEON)
    if (vm_cpu_has(VM_LEVEL_NEON)) {
        vm_array_sub_neon(res, a, b, n);
        return;
    }
#endif
    for (int i = 0; i < n; ++i) {
        res[i] = a[i] - b[i];
    }
}

void vm_array_mul(vm_float_t *res, const vm_float_t *a, const vm_float_t *b, const int n)
{
    if (n <= 0 || !res || !a || !b) {
        return;
    }
#if defined(VECMAT_ENABLE_AVX2)
    if (vm_cpu_has(VM_LEVEL_AVX2)) {
        vm_array_mul_avx2(res, a, b, n);
        return;
    }
#endif
#if defined(VECMAT_ENABLE_NEON)
    if (vm_cpu_has(VM_LEVEL_NEON)) {
        vm_array_mul_neon(res, a, b, n);
        return;
    }
#endif
    for (int i = 0; i < n; ++i) {
        res[i] = a[i] * b[i];
    }
}

void vm_array_div(vm_float_t *res, const vm_float_t *a, const vm_float_t *b, const int n)
{
    if (n <= 0 || !res || !a || !b) {
        return;
    }
#if defined(VECMAT_ENABLE_AVX2)
    if (vm_cpu_has(VM_LEVEL_AVX2)) {
        vm_array_div_avx2(res, a, b, n);
        return;
    }
#endif
#if defined(VECMAT_ENABLE_NEON)
    if (vm_cpu_has(VM_LEVEL_NEON)) {
        vm_array_div_neon(res, a, b, n);
        return;
    }
#endif
    for (int i = 0; i < n; ++i) {
        res[i] = a[i] / b[i];
    }
}

void vm_array_fma(vm_float_t *res, const vm_float_t *a, const vm_float_t *b,
                  const vm_float_t *c, const int n)
{
    if (n <= 0 || !res || !a || !b || !c) {
        return;
    }
#if defined(VECMAT_ENABLE_AVX2)
    if (vm_cpu_has(VM_LEVEL_AVX2)) {
        vm_array_fma_avx2(res, a, b, c, n);
        return;
    }
#endif
#if defined(VECMAT_ENABLE_NEON)
    if (vm_cpu_has(VM_LEVEL_NEON)) {
        vm_array_fma_neon(res, a, b, c, n);
        return;
    }
#endif
    for (int i = 0; i < n; ++i) {
        res[i] = VECMAT_FMA(a[i], b[i], c[i]);
    }
}

void vm_axpy(vm_float_t *y, const vm_float_t alpha, const vm_float_t *x, const int n)
{
    if (n <= 0 || !y || !x) {
        return;
    }
#if defined(VECMAT_ENABLE_AVX2)
    if (vm_cpu_has(VM_LEVEL_AVX2)) {
        vm_axpy_avx2(y, alpha, x, n);
        return;
    }
#endif
#if defined(VECMAT_ENABLE_NEON)
    if (vm_cpu_has(VM_LEVEL_NEON)) {
        vm_axpy_neon(y, alpha, x, n);
        return;
    }
#endif
    for (int i = 0; i < n; ++i) {
        const vm_float_t term = alpha * x[i];
        y[i] = y[i] + term;
    }
}

static vm_float_t asum_step(const vm_float_t sum, vm_float_t *comp, const vm_float_t x)
{
    volatile vm_float_t t = sum + x;
    if (VECMAT_FABS(sum) >= VECMAT_FABS(x)) {
        const volatile vm_float_t lost = sum - t;
        *comp += lost + x;
    } else {
        const volatile vm_float_t lost = x - t;
        *comp += lost + sum;
    }
    return t;
}

vm_float_t vm_nrm2(const vm_float_t *x, const int n)
{
    if (n <= 0 || !x) {
        return VM_F(0.0);
    }

    vm_float_t scale = VM_F(0.0);
    vm_float_t ssq = VM_F(1.0);
    bool saw_nan = false;
    bool saw_inf = false;
    for (int i = 0; i < n; ++i) {
        const vm_float_t xi = x[i];
        if (isnan(xi)) {
            saw_nan = true;
            continue;
        }
        if (isinf(xi)) {
            saw_inf = true;
            continue;
        }
        const vm_float_t absxi = VECMAT_FABS(xi);
        if (absxi == VM_F(0.0)) {
            continue;
        }
        if (scale < absxi) {
            const vm_float_t ratio = scale / absxi;
            ssq = VM_F(1.0) + ssq * ratio * ratio;
            scale = absxi;
        } else {
            const vm_float_t ratio = absxi / scale;
            ssq += ratio * ratio;
        }
    }
    if (saw_nan) {
        return array_nan();
    }
    if (saw_inf) {
        return INFINITY;
    }
    return scale * VECMAT_SQRT(ssq);
}

vm_float_t vm_asum(const vm_float_t *x, const int n)
{
    if (n <= 0 || !x) {
        return VM_F(0.0);
    }

    vm_float_t sum = VM_F(0.0);
    vm_float_t comp = VM_F(0.0);
    for (int i = 0; i < n; ++i) {
        sum = asum_step(sum, &comp, VECMAT_FABS(x[i]));
    }
    volatile vm_float_t total = sum + comp;
    return total;
}

void vm_rsqrt_array(vm_float_t *res, const vm_float_t *x, const int n)
{
    if (n <= 0 || !res || !x) {
        return;
    }
#if defined(VECMAT_ENABLE_AVX2)
    if (vm_cpu_has(VM_LEVEL_AVX2)) {
        vm_rsqrt_array_avx2(res, x, n);
        return;
    }
#endif
#if defined(VECMAT_ENABLE_NEON)
    if (vm_cpu_has(VM_LEVEL_NEON)) {
        vm_rsqrt_array_neon(res, x, n);
        return;
    }
#endif
    for (int i = 0; i < n; ++i) {
        const vm_float_t v = x[i];
        if (v < VM_F(0.0)) {
            res[i] = array_nan();
        } else {
            res[i] = VM_F(1.0) / VECMAT_SQRT(v);
        }
    }
}
