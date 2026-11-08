// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_ARRAY_H
#define VECMAT_ARRAY_H

/**
 * @file
 * @brief Scalar loops on contiguous `vm_float_t` buffers.
 * @ingroup vecmat_extras
 *
 * `n <= 0` reads nothing, and null pointers are then allowed. For `n > 0`
 * the pointers must be non-null and address `n` elements. An output may
 * exactly alias an input; a partial overlap is caller error. No allocation.
 */

#include "vecmat/types.h"
#include "vecmat/abi.h"

/**
 * @brief `res[i] = a[i] + b[i]`.
 *
 * One rounding. Finite results are within 1 ULP of the correctly rounded sum.
 *
 * @param res Output; length `n`. May exactly alias `a` or `b`.
 * @param a   Left input; length `n`.
 * @param b   Right input; length `n`.
 * @param n   Element count. `n <= 0` is a no-op.
 */
VEC_API void vm_array_add(vm_float_t *res, const vm_float_t *a,
                          const vm_float_t *b, int n);

/**
 * @brief `res[i] = a[i] - b[i]`.
 *
 * One rounding. Finite results are within 1 ULP of the correctly rounded difference.
 *
 * @param res Output; length `n`. May exactly alias `a` or `b`.
 * @param a   Left input; length `n`.
 * @param b   Right input; length `n`.
 * @param n   Element count. `n <= 0` is a no-op.
 */
VEC_API void vm_array_sub(vm_float_t *res, const vm_float_t *a,
                          const vm_float_t *b, int n);

/**
 * @brief `res[i] = a[i] * b[i]`.
 *
 * One rounding. Finite results are within 1 ULP of the correctly rounded product.
 *
 * @param res Output; length `n`. May exactly alias `a` or `b`.
 * @param a   Left input; length `n`.
 * @param b   Right input; length `n`.
 * @param n   Element count. `n <= 0` is a no-op.
 */
VEC_API void vm_array_mul(vm_float_t *res, const vm_float_t *a,
                          const vm_float_t *b, int n);

/**
 * @brief `res[i] = fma(a[i], b[i], c[i])`.
 *
 * One rounding, through `VECMAT_FMA` (`fma` / `fmaf`). This is not a license
 * to contract other expressions. Finite results are within 1 ULP of the
 * correctly rounded fused operation.
 *
 * @param res Output; length `n`. May exactly alias `a`, `b`, or `c`.
 * @param a   Factor; length `n`.
 * @param b   Factor; length `n`.
 * @param c   Addend; length `n`.
 * @param n   Element count. `n <= 0` is a no-op.
 */
VEC_API void vm_array_fma(vm_float_t *res, const vm_float_t *a,
                          const vm_float_t *b, const vm_float_t *c, int n);

/**
 * @brief `y[i] += alpha * x[i]`.
 *
 * A multiply, then an add. Not a fused multiply-add. `x` may exactly alias
 * `y`. For finite inputs the forward error is at most
 * 8 eps × `(|y_old| + |alpha * x|)`.
 *
 * @param y     Input and output; length `n`.
 * @param alpha Scale applied to `x`.
 * @param x     Input; length `n`. May exactly alias `y`.
 * @param n     Element count. `n <= 0` is a no-op.
 */
VEC_API void vm_axpy(vm_float_t *y, vm_float_t alpha, const vm_float_t *x, int n);

/**
 * @brief `res[i] = 1 / sqrt(x[i])`.
 *
 * Strict reciprocal square root. No estimate and no Newton iteration.
 * `+0` → `+inf`, `−0` → `−inf`, `+inf` → `+0`, a negative nonzero
 * (including `−inf`) → NaN, and a NaN stays a NaN. Positive finite normals
 * whose reciprocal square root is finite are within 2 ULP of the correctly
 * rounded value. `res` may exactly alias `x`.
 *
 * @param res Output; length `n`.
 * @param x   Input; length `n`.
 * @param n   Element count. `n <= 0` is a no-op.
 */
VEC_API void vm_rsqrt_array(vm_float_t *res, const vm_float_t *x, int n);

#endif //VECMAT_ARRAY_H
