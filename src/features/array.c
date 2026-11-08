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
void vm_array_fma_avx2(vm_float_t *res, const vm_float_t *a, const vm_float_t *b,
                       const vm_float_t *c, int n);
void vm_axpy_avx2(vm_float_t *y, vm_float_t alpha, const vm_float_t *x, int n);
#endif

#if defined(VECMAT_ENABLE_NEON)
void vm_array_add_neon(vm_float_t *res, const vm_float_t *a, const vm_float_t *b, int n);
void vm_array_sub_neon(vm_float_t *res, const vm_float_t *a, const vm_float_t *b, int n);
void vm_array_mul_neon(vm_float_t *res, const vm_float_t *a, const vm_float_t *b, int n);
void vm_array_fma_neon(vm_float_t *res, const vm_float_t *a, const vm_float_t *b,
                       const vm_float_t *c, int n);
void vm_axpy_neon(vm_float_t *y, vm_float_t alpha, const vm_float_t *x, int n);
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
    if ((vm_cpu_runtime_features() & VM_CPU_AVX2) != 0) {
        vm_array_add_avx2(res, a, b, n);
        return;
    }
#endif
#if defined(VECMAT_ENABLE_NEON)
    if ((vm_cpu_runtime_features() & VM_CPU_NEON) != 0) {
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
    if ((vm_cpu_runtime_features() & VM_CPU_AVX2) != 0) {
        vm_array_sub_avx2(res, a, b, n);
        return;
    }
#endif
#if defined(VECMAT_ENABLE_NEON)
    if ((vm_cpu_runtime_features() & VM_CPU_NEON) != 0) {
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
    if ((vm_cpu_runtime_features() & VM_CPU_AVX2) != 0) {
        vm_array_mul_avx2(res, a, b, n);
        return;
    }
#endif
#if defined(VECMAT_ENABLE_NEON)
    if ((vm_cpu_runtime_features() & VM_CPU_NEON) != 0) {
        vm_array_mul_neon(res, a, b, n);
        return;
    }
#endif
    for (int i = 0; i < n; ++i) {
        res[i] = a[i] * b[i];
    }
}

void vm_array_fma(vm_float_t *res, const vm_float_t *a, const vm_float_t *b,
                  const vm_float_t *c, const int n)
{
    if (n <= 0 || !res || !a || !b || !c) {
        return;
    }
#if defined(VECMAT_ENABLE_AVX2)
    if ((vm_cpu_runtime_features() & VM_CPU_AVX2) != 0) {
        vm_array_fma_avx2(res, a, b, c, n);
        return;
    }
#endif
#if defined(VECMAT_ENABLE_NEON)
    if ((vm_cpu_runtime_features() & VM_CPU_NEON) != 0) {
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
    if ((vm_cpu_runtime_features() & VM_CPU_AVX2) != 0) {
        vm_axpy_avx2(y, alpha, x, n);
        return;
    }
#endif
#if defined(VECMAT_ENABLE_NEON)
    if ((vm_cpu_runtime_features() & VM_CPU_NEON) != 0) {
        vm_axpy_neon(y, alpha, x, n);
        return;
    }
#endif
    for (int i = 0; i < n; ++i) {
        const vm_float_t term = alpha * x[i];
        y[i] = y[i] + term;
    }
}

void vm_rsqrt_array(vm_float_t *res, const vm_float_t *x, const int n)
{
    if (n <= 0 || !res || !x) {
        return;
    }
    for (int i = 0; i < n; ++i) {
        const vm_float_t v = x[i];
        if (v < VM_F(0.0)) {
            res[i] = array_nan();
        } else {
            res[i] = VM_F(1.0) / VECMAT_SQRT(v);
        }
    }
}
