// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_SPECTRAL_H
#define VECMAT_SPECTRAL_H

/**
 * @file
 * @brief Dense spectral primitives: matvec, symmetric eigensolve, graph Laplacian.
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
