// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_BATCH3_H
#define VECMAT_BATCH3_H

/**
 * @file
 * @brief Strided batch reference for 3×3 and Voigt-6 kernels.
 * @ingroup vecmat_extras
 *
 * Scalar reference. Strides are in elements, so packed and padded layouts
 * both work. A batch of one matches the scalar call. SIMD backends register
 * through the existing mechanism and must match this result.
 */

#include "vecmat/types.h"
#include "vecmat/abi.h"

/**
 * @brief Batched 3×3 multiply, \(C_i = A_i B_i\).
 *
 * @param C Output matrices; item `i` at `C + i * c_stride`.
 * @param c_stride Element stride between output matrices.
 * @param A Left matrices; item `i` at `A + i * a_stride`.
 * @param a_stride Element stride between left matrices.
 * @param B Right matrices; item `i` at `B + i * b_stride`.
 * @param b_stride Element stride between right matrices.
 * @param n Number of items.
 * @return false if a pointer is NULL or `n` is non-positive.
 */
VEC_API bool vm_batch3_mul(vm_float_t *C, int c_stride,
                           const vm_float_t *A, int a_stride,
                           const vm_float_t *B, int b_stride,
                           int n);

/**
 * @brief Batched 3×3 inverse.
 *
 * @param C Output matrices; item `i` at `C + i * c_stride`.
 * @param c_stride Element stride between output matrices.
 * @param A Input matrices; item `i` at `A + i * a_stride`.
 * @param a_stride Element stride between input matrices.
 * @param n Number of items.
 * @return false if a pointer is NULL, `n` is non-positive, or any inverse fails.
 */
VEC_API bool vm_batch3_inverse(vm_float_t *C, int c_stride,
                               const vm_float_t *A, int a_stride,
                               int n);

/**
 * @brief Batched polar decomposition, \(F_i = R_i U_i\).
 *
 * @param R Output rotations; item `i` at `R + i * r_stride`.
 * @param r_stride Element stride between rotations.
 * @param U Output stretches; item `i` at `U + i * u_stride`.
 * @param u_stride Element stride between stretches.
 * @param F Input gradients; item `i` at `F + i * f_stride`.
 * @param f_stride Element stride between gradients.
 * @param n Number of items.
 * @return false if a pointer is NULL, `n` is non-positive, or any polar fails.
 */
VEC_API bool vm_batch3_polar(vm_float_t *R, int r_stride,
                             vm_float_t *U, int u_stride,
                             const vm_float_t *F, int f_stride,
                             int n);

/**
 * @brief Batched Voigt-6 double contraction, \(d_i = a_i : b_i\).
 *
 * @param d Output scalars; length `n`.
 * @param a Left tensors; item `i` at `a + i * a_stride`.
 * @param a_stride Element stride between left tensors.
 * @param b Right tensors; item `i` at `b + i * b_stride`.
 * @param b_stride Element stride between right tensors.
 * @param n Number of items.
 * @return false if a pointer is NULL or `n` is non-positive.
 */
VEC_API bool vm_batch3_ddot(vm_float_t *d,
                            const vm_float_t *a, int a_stride,
                            const vm_float_t *b, int b_stride,
                            int n);

#endif //VECMAT_BATCH3_H
