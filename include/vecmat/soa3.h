// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_SOA3_H
#define VECMAT_SOA3_H

/**
 * @file
 * @brief Minimal 3-vector geometry on structure-of-arrays views.
 * @ingroup vecmat_extras
 *
 * A view is three borrowed component pointers; vector `i` is
 * `(x[i], y[i], z[i])`. The view carries no size, ownership, padding, or
 * SIMD width. Each component array has unit stride and needs only natural
 * alignment. Component arrays must not overlap each other.
 *
 * Every call batches independent vectors, unlike the single-vector
 * `vec3_dot` / `vec3_cross`. `n <= 0` is a no-op and reads nothing; null
 * pointers are then allowed. For `n > 0` every pointer must be non-null and
 * address `n` elements. No allocation.
 *
 * The views hold selected-width pointers, so use them only with the TU's
 * width (`VECMAT_USE_F64` or not).
 *
 * Accuracy is an absolute bound, not a ULP claim: for normal finite
 * intermediates the error is at most 16 eps × the sum of the absolute
 * intermediate terms (for distances, the terms after the subtraction).
 * Cancellation can make the relative error unbounded.
 */

#include "vecmat/types.h"
#include "vecmat/abi.h"

/**
 * @brief Writable borrowed SoA view of 3-vectors.
 */
typedef struct vm_soa3 {
    vm_float_t *x; /**< x components; unit stride. */
    vm_float_t *y; /**< y components; unit stride. */
    vm_float_t *z; /**< z components; unit stride. */
} vm_soa3;

/**
 * @brief Read-only borrowed SoA view of 3-vectors.
 */
typedef struct vm_csoa3 {
    const vm_float_t *x; /**< x components; unit stride. */
    const vm_float_t *y; /**< y components; unit stride. */
    const vm_float_t *z; /**< z components; unit stride. */
} vm_csoa3;

/**
 * @brief `res[i] = a[i] · b[i]`.
 *
 * Evaluated as `(ax*bx + ay*by) + az*bz`, unfused. `a` may exactly alias
 * `b`. `res` must not overlap any input component.
 *
 * @param res Output; length `n`.
 * @param a   Left vectors; length `n`.
 * @param b   Right vectors; length `n`. May exactly alias `a`.
 * @param n   Vector count. `n <= 0` is a no-op.
 */
VEC_API void vm_soa3_dot(vm_float_t *res, vm_csoa3 a, vm_csoa3 b, int n);

/**
 * @brief `res[i] = a[i] × b[i]`.
 *
 * Each component is `p*q - r*s`, unfused. All six input components of
 * vector `i` are loaded before any store, so `res` may exactly equal the
 * `a` or `b` view (same three pointers). A partial overlap, or a `res`
 * component that aliases a different input component, is caller error.
 *
 * @param res Output vectors; length `n`. May exactly equal `a` or `b`.
 * @param a   Left vectors; length `n`.
 * @param b   Right vectors; length `n`.
 * @param n   Vector count. `n <= 0` is a no-op.
 */
VEC_API void vm_soa3_cross(vm_soa3 res, vm_csoa3 a, vm_csoa3 b, int n);

/**
 * @brief `res[i] = |a[i]|²`.
 *
 * Evaluated as `(x*x + y*y) + z*z`, unfused. `res` must not overlap any
 * input component.
 *
 * @param res Output; length `n`.
 * @param a   Vectors; length `n`.
 * @param n   Vector count. `n <= 0` is a no-op.
 */
VEC_API void vm_soa3_length_squared(vm_float_t *res, vm_csoa3 a, int n);

/**
 * @brief `res[i] = |a[i] - b[i]|²`.
 *
 * Each axis difference is rounded once, then squared and summed as in
 * `vm_soa3_length_squared`. `a` may exactly alias `b`. `res` must not
 * overlap any input component.
 *
 * @param res Output; length `n`.
 * @param a   First vectors; length `n`.
 * @param b   Second vectors; length `n`. May exactly alias `a`.
 * @param n   Vector count. `n <= 0` is a no-op.
 */
VEC_API void vm_soa3_distance_squared(vm_float_t *res, vm_csoa3 a, vm_csoa3 b, int n);

#endif //VECMAT_SOA3_H
