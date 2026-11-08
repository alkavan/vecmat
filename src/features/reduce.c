// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <math.h>
#include <vecmat.h>

#if !defined(_MSC_VER)
#pragma STDC FP_CONTRACT OFF
#endif

/**
 * @brief One Neumaier step. `t` is stored so the lost low part survives optimization.
 */
static vm_float_t neumaier_step(const vm_float_t sum, vm_float_t *comp, const vm_float_t x)
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

vm_float_t vm_sum_compensated(const vm_float_t *x, const int n)
{
    if (n <= 0 || !x) {
        return VM_F(0.0);
    }
    vm_float_t sum = VM_F(0.0);
    vm_float_t comp = VM_F(0.0);
    for (int i = 0; i < n; ++i) {
        sum = neumaier_step(sum, &comp, x[i]);
    }
    volatile vm_float_t total = sum + comp;
    return total;
}

vm_float_t vm_dot_compensated(const vm_float_t *a, const vm_float_t *b, const int n)
{
    if (n <= 0 || !a || !b) {
        return VM_F(0.0);
    }

    vm_float_t sum = VM_F(0.0);
    vm_float_t comp = VM_F(0.0);

    for (int i = 0; i < n; ++i) {
        const vm_float_t term = a[i] * b[i];
        sum = neumaier_step(sum, &comp, term);
    }

    volatile vm_float_t total = sum + comp;
    return total;
}

bool vm_array_minmax(vm_float_t *min_value, vm_float_t *max_value,
                     const vm_float_t *x, const int n)
{
    if (n <= 0 || !min_value || !max_value || !x) {
        return false;
    }

    vm_float_t lo = VM_F(0.0);
    vm_float_t hi = VM_F(0.0);
    bool any = false;
    for (int i = 0; i < n; ++i) {
        const vm_float_t v = x[i];
        if (isnan(v)) {
            continue;
        }

        if (!any) {
            lo = v;
            hi = v;
            any = true;
            continue;
        }

        if (v < lo || (v == VM_F(0.0) && lo == VM_F(0.0) && signbit(v))) {
            lo = v;
        }

        if (v > hi || (v == VM_F(0.0) && hi == VM_F(0.0) && !signbit(v))) {
            hi = v;
        }
    }

    if (!any) {
        return false;
    }

    *min_value = lo;
    *max_value = hi;
    return true;
}

/*
 * f32 acc. f64 reductions. Fixed names, not remapped in abi.h.
 * Emitted from only one float object library so the symbols appear once:
 * the f32 objects when ABI 32 is built, otherwise the f64 objects.
 */
#if !defined(VECMAT_USE_F64) || !defined(VECMAT_HAVE_ABI_32)

/**
 * @brief One f64 Neumaier step.
 */
static double neumaier_f64(const double sum, double *comp, const double x)
{
    volatile double t = sum + x;
    if (fabs(sum) >= fabs(x)) {
        const volatile double lost = sum - t;
        *comp += lost + x;
    } else {
        const volatile double lost = x - t;
        *comp += lost + sum;
    }
    return t;
}

vm_float64_t vm_sum_f32f64(const vm_float32_t *x, const int n)
{
    if (n <= 0 || !x) {
        return 0.0;
    }

    double sum = 0.0;
    double comp = 0.0;
    for (int i = 0; i < n; ++i) {
        sum = neumaier_f64(sum, &comp, (double)x[i]);
    }

    volatile double total = sum + comp;
    return total;
}

vm_float64_t vm_dot_f32f64(const vm_float32_t *a, const vm_float32_t *b, const int n)
{
    if (n <= 0 || !a || !b) {
        return 0.0;
    }

    double sum = 0.0;
    double comp = 0.0;
    for (int i = 0; i < n; ++i) {
        const double term = (double)a[i] * (double)b[i];
        sum = neumaier_f64(sum, &comp, term);
    }

    volatile double total = sum + comp;
    return total;
}

#endif
