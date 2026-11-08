// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_PERIODIC_H
#define VECMAT_PERIODIC_H

/**
 * @file
 * @brief Orthorhombic minimum image and wrap on `vector3` arrays.
 * @ingroup vecmat_extras
 *
 * The box is axis-aligned. Lengths are in the same unit as the coordinates.
 * The scalar reference is `VECMAT_REMAINDER` (`remainder` / `remainderf`),
 * so the nearest integer uses ties-to-even. This is not a force kernel.
 */

#include "vecmat/types.h"
#include "vecmat/abi.h"

/**
 * @brief Minimum image of `a - b` on an orthorhombic box.
 *
 * Each axis is `remainder(a - b, L)` and lies in `[-L/2, L/2]`. `L/2` and
 * `3L/2` may select opposite images of equal length. A whole-box translation
 * at an exact tie need not keep the same sign. `res` may exactly alias `a`
 * or `b`.
 *
 * Returns false, and writes nothing, when `n < 0`, a box length is not
 * finite and positive, or `n > 0` and a pointer is missing. `n == 0` with a
 * valid box returns true and does not read the arrays. A nonfinite coordinate
 * is not a physical input; that axis becomes NaN.
 *
 * Away from an image boundary, and for `|coordinate/L|` at most `2^20` in
 * f32 or `2^40` in f64, the absolute error is at most
 * 16 eps × max(L, |a|, |b|).
 *
 * @param res Output images; length `n`. May exactly alias `a` or `b`.
 * @param a   First positions; length `n`.
 * @param b   Second positions; length `n`.
 * @param box Positive finite axis lengths.
 * @param n   Vector count.
 * @return false on a structural error.
 */
VEC_API bool vm_periodic_delta_ortho(vector3 *res, const vector3 *a,
                                     const vector3 *b, vector3 box, int n);

/**
 * @brief Fold each position into `[0, L)` in place.
 *
 * Uses `VECMAT_REMAINDER`, then shifts a negative image up by `L`. A result
 * that lands on `L`, and a signed zero, are stored as `+0`.
 *
 * Returns false, and writes nothing, when `n < 0`, a box length is not
 * finite and positive, or `n > 0` and `x` is missing. `n == 0` with a valid
 * box returns true and does not read `x`. A nonfinite coordinate becomes NaN.
 *
 * @param x   Positions; length `n`. Updated in place.
 * @param box Positive finite axis lengths.
 * @param n   Vector count.
 * @return false on a structural error.
 */
VEC_API bool vm_wrap_ortho(vector3 *x, vector3 box, int n);

#endif //VECMAT_PERIODIC_H
