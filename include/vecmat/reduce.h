// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_REDUCE_H
#define VECMAT_REDUCE_H

/**
 * @file
 * @brief Compensated reductions and extrema on contiguous buffers.
 * @ingroup vecmat_extras
 *
 * Sum and dot use Neumaier compensation in the accumulation type.
 * `vm_sum_f32f64` and `vm_dot_f32f64` are fixed names, compiled once, and
 * are not remapped in `abi.h`. The mixed dot widens each factor to f64
 * before the multiply. `n <= 0` returns `+0` and does not read.
 */

#include "vecmat/types.h"
#include "vecmat/abi.h"

/**
 * @brief Neumaier sum of `x[0..n)`.
 *
 * `n <= 0` returns `+0` and does not read. On finite non-overflow inputs
 * with `n <= 10^6`, the absolute error is at most
 * 16 eps × sum(|terms|). The same binary and input repeat.
 *
 * @param x Input; length `n`. Ignored when `n <= 0`.
 * @param n Element count.
 * @return Compensated sum, or `+0` when `n <= 0`.
 */
VEC_API vm_float_t vm_sum_compensated(const vm_float_t *x, int n);

/**
 * @brief Neumaier dot product of `a` and `b`.
 *
 * The product is formed in `vm_float_t`, then compensated. `a` may exactly
 * alias `b`. `n <= 0` returns `+0` and does not read. The absolute-error
 * target matches `vm_sum_compensated`.
 *
 * @param a Left input; length `n`. Ignored when `n <= 0`.
 * @param b Right input; length `n`. May exactly alias `a`.
 * @param n Element count.
 * @return Compensated dot, or `+0` when `n <= 0`.
 */
VEC_API vm_float_t vm_dot_compensated(const vm_float_t *a, const vm_float_t *b, int n);

/**
 * @brief f64 Neumaier sum of f32 values.
 *
 * Each term is widened before it is accumulated. Compiled once; the name
 * is not remapped. `n <= 0` returns `+0` and does not read.
 *
 * @param x Input; length `n`. Ignored when `n <= 0`.
 * @param n Element count.
 * @return Compensated f64 sum, or `+0` when `n <= 0`.
 */
VEC_API vm_float64_t vm_sum_f32f64(const vm_float32_t *x, int n);

/**
 * @brief f64 Neumaier dot of f32 values, widened before the multiply.
 *
 * `a[i]` and `b[i]` are converted to f64, then multiplied. Compiled once;
 * the name is not remapped. `a` may exactly alias `b`. `n <= 0` returns
 * `+0` and does not read.
 *
 * @param a Left input; length `n`. Ignored when `n <= 0`.
 * @param b Right input; length `n`. May exactly alias `a`.
 * @param n Element count.
 * @return Compensated f64 dot, or `+0` when `n <= 0`.
 */
VEC_API vm_float64_t vm_dot_f32f64(const vm_float32_t *a, const vm_float32_t *b, int n);

/**
 * @brief Minimum and maximum, ignoring NaNs.
 *
 * `n <= 0`, a missing pointer, or an all-NaN input returns false and leaves
 * `*min_value` and `*max_value` unchanged. Infinities count. The minimum of
 * `±0` is `−0` and the maximum is `+0`. A selected finite or infinite value
 * is exact.
 *
 * @param min_value Output minimum. Unchanged on failure.
 * @param max_value Output maximum. Unchanged on failure.
 * @param x         Input; length `n`.
 * @param n         Element count.
 * @return true when at least one non-NaN value was seen.
 */
VEC_API bool vm_array_minmax(vm_float_t *min_value, vm_float_t *max_value,
                             const vm_float_t *x, int n);

#endif //VECMAT_REDUCE_H
