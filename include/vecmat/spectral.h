// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_SPECTRAL_H
#define VECMAT_SPECTRAL_H

/**
 * @file
 * @brief Dense spectral primitives: matvec, gemv, ger, symmetric eigensolve, graph Laplacian.
 * @ingroup vecmat_extras
 */

#include "vecmat/types.h"
#include "vecmat/abi.h"

/**
 * @brief Dense matrix-vector product `y = A x`.
 *
 * `A` is `rows × cols`. `x` has length `cols`, `y` has length `rows`.
 *
 * @param y Output vector.
 * @param A Input matrix.
 * @param x Input vector.
 */
VEC_API void vm_matvec(vm_float_t *y, const vm_mat *A, const vm_float_t *x);

/**
 * @brief `y = alpha * op(A) * x + beta * y`.
 *
 * `A` is column-major. `trans` false uses `A` (`x` length `cols`, `y` length
 * `rows`). `trans` true uses `Aᵀ` (`x` length `rows`, `y` length `cols`).
 * A null pointer, or a matrix with a non-positive dimension, does not read
 * or write. `beta == 0` does not read `y`, so a NaN already in `y` is
 * replaced. `alpha == 0` does not read `x`. `y` may exactly alias `x` when
 * the buffer is long enough for both lengths; a partial overlap is caller
 * error. The exact-alias case allocates one temporary and frees it.
 * Allocation failure leaves `y` unchanged. `vm_matvec` stays the
 * no-transpose, `alpha = 1`, `beta = 0` path.
 *
 * @param y Output. Length is `rows`, or `cols` when `trans` is true.
 * @param alpha Scale on `op(A) x`.
 * @param A Matrix. Not modified.
 * @param x Input. May exactly alias `y`.
 * @param beta Scale on the incoming `y`. Zero skips the read.
 * @param trans True to apply `Aᵀ`.
 */
VEC_API void vm_gemv(vm_float_t *y, vm_float_t alpha, const vm_mat *A, const vm_float_t *x,
                     vm_float_t beta, bool trans);

/**
 * @brief Rank-1 update `A += alpha * x * yᵀ`.
 *
 * `x` has length `A->rows` and `y` has length `A->cols`. `x` may exactly
 * alias `y`. `A` must not overlap either vector. No allocation. `alpha == 0`
 * or an empty matrix does not read `x` or `y` and does not write `A`.
 *
 * @param A Matrix updated in place.
 * @param alpha Scale on the outer product.
 * @param x Left vector, length `rows`.
 * @param y Right vector, length `cols`.
 */
VEC_API void vm_ger(vm_mat *A, vm_float_t alpha, const vm_float_t *x, const vm_float_t *y);

/**
 * @brief Symmetric matrix-vector product.
 *
 * Only the lower triangle of `A` is read. `A` must be square.
 *
 * @param y Output vector.
 * @param A Symmetric matrix (lower triangle used).
 * @param x Input vector.
 */
VEC_API void vm_symv(vm_float_t *y, const vm_mat *A, const vm_float_t *x);

/**
 * @brief Symmetric real eigenvalue decomposition (Jacobi).
 *
 * On success, eigenvalues are written in descending order. Eigenvector
 * columns are written to `V` if non-NULL (`V` must be `n × n`). `A` is
 * overwritten. Returns false on non-square, bad sizes, or numerical failure.
 *
 * @param A Symmetric matrix, overwritten on success.
 * @param eigenvalues Output eigenvalues (length `n`), descending.
 * @param V Optional `n × n` eigenvector matrix (columns).
 * @return true on success.
 */
VEC_API bool vm_syev(vm_mat *A, vm_float_t *eigenvalues, vm_mat *V);

/**
 * @brief Unnormalized graph Laplacian `L = D − A`.
 *
 * From a dense symmetric adjacency matrix `A`. `L` is allocated or resized
 * as needed.
 *
 * @param L Output Laplacian.
 * @param A Symmetric adjacency (non-negative entries expected).
 * @return true on success.
 */
VEC_API bool vm_laplacian(vm_mat *L, const vm_mat *A);

/**
 * @brief Top-k eigenvectors of a symmetric matrix.
 *
 * Thin wrapper around `vm_syev`. Eigenvalues descending. `eigenvectors` is
 * `n × k` on success.
 *
 * @param A Symmetric matrix (not overwritten).
 * @param k Number of leading eigenpairs (`1 ≤ k ≤ n`).
 * @param eigenvalues Output eigenvalues (length `k`).
 * @param eigenvectors Output `n × k` eigenvectors (columns).
 * @return true on success.
 */
VEC_API bool vm_syev_topk(const vm_mat *A, int k,
                          vm_float_t *eigenvalues, vm_mat *eigenvectors);

#endif //VECMAT_SPECTRAL_H
