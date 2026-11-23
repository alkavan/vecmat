// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_LANCZOS_H
#define VECMAT_LANCZOS_H

/**
 * @file
 * @brief Real-symmetric Lanczos eigensolver with full reorthogonalization.
 * @ingroup vecmat_extras
 */

#include "vecmat/types.h"
#include "vecmat/abi.h"

/**
 * @brief Which end of the projected spectrum to return.
 *
 * `vm_syev` orders eigenvalues descending. Largest keeps that leading end.
 * Smallest keeps the trailing end, still descending, so the smallest value
 * is last.
 */
typedef enum {
    VM_LANCZOS_LARGEST = 0,  /**< Largest algebraic eigenvalues. */
    VM_LANCZOS_SMALLEST = 1  /**< Smallest algebraic eigenvalues. */
} vm_lanczos_which;

/**
 * @brief Implicitly restarted Lanczos for a real symmetric operator.
 *
 * Builds a Krylov subspace of length `m` with full reorthogonalization,
 * solves the projected tridiagonal with `vm_syev`, and keeps the `k`
 * eigenvalues at the end selected by `which`. Unwanted Ritz values are
 * implicit shifts. The start vector is deterministic.
 *
 * `k <= m <= n`. `tol` is the relative residual
 * `||A u − λ u|| / max(|λ|, ||T||, ε)`, where `||T||` is the max absolute
 * row sum of the projected tridiagonal and `u` is a unit Ritz vector.
 * `max_restarts < 0` is rejected. Zero restarts is one Krylov build.
 *
 * On success the `k` eigenvalues are descending within the selected end.
 * Eigenvector columns, when requested, are unit length with a positive
 * entry of largest magnitude. `eigenvectors` NULL skips the vectors.
 * `info` may be NULL. `iters` is the number of restarts performed (zero
 * when the first build is enough), `rel_res` is the worst residual of the
 * returned pairs, and `ok` matches the bool result.
 *
 * A bad `k`, `m`, or `tol` (`tol <= 0` or NaN), a null `matvec` or
 * `eigenvalues`, or an `eigenvectors` matrix that is not `n × k` returns
 * false and does not write the eigenvalues. If the iteration stops without
 * meeting `tol` and at least `k` steps exist, the best pairs are written
 * and the return is false.
 *
 * Real symmetric only. No shift-invert, no block Lanczos, no complex
 * Hermitian path, and no device path.
 *
 * @param n Matrix order. `n >= 1`.
 * @param k Number of eigenpairs. `1 <= k <= m`.
 * @param matvec Applies the symmetric operator. `y` and `x` have length `n`.
 * @param ctx Passed through to `matvec`. May be NULL.
 * @param which Largest or smallest end of the projected spectrum.
 * @param m Krylov length. `k <= m <= n`.
 * @param tol Relative residual tolerance. Must be finite and positive.
 * @param max_restarts Restart cap. Zero is a single build. Negative is an error.
 * @param eigenvalues Output eigenvalues, length `k`.
 * @param eigenvectors Optional `n × k` eigenvectors (columns). NULL skips them.
 * @param info Optional iteration stats. May be NULL.
 *
 * @return true when the worst returned residual is within `tol`.
 */
VEC_API bool vm_lanczos(int n, int k,
                        void (*matvec)(vm_float_t *y, const vm_float_t *x, void *ctx),
                        void *ctx, vm_lanczos_which which,
                        int m, vm_float_t tol, int max_restarts,
                        vm_float_t *eigenvalues, vm_mat *eigenvectors,
                        vm_ksp_info *info);

/**
 * @brief Lanczos on a dense symmetric matrix.
 *
 * Calls `vm_symv`, so only the lower triangle of `A` is read. `A` is not
 * modified. `A` must be square. The remaining arguments match `vm_lanczos`.
 *
 * @param A Symmetric matrix. Lower triangle only.
 * @param k Number of eigenpairs.
 * @param which Largest or smallest end.
 * @param m Krylov length. `k <= m <= n`.
 * @param tol Relative residual tolerance.
 * @param max_restarts Restart cap.
 * @param eigenvalues Output eigenvalues, length `k`.
 * @param eigenvectors Optional `n × k` eigenvectors. NULL skips them.
 * @param info Optional iteration stats. May be NULL.
 *
 * @return true when the worst returned residual is within `tol`.
 */
VEC_API bool vm_lanczos_dense(const vm_mat *A, int k, vm_lanczos_which which,
                              int m, vm_float_t tol, int max_restarts,
                              vm_float_t *eigenvalues, vm_mat *eigenvectors,
                              vm_ksp_info *info);

/**
 * @brief Lanczos on a square CSR matrix.
 *
 * Calls `vm_spmv`. The matrix must already store the operator the product
 * should apply. A nonsymmetric CSR is applied as stored; the eigenvalues
 * are meaningful when that operator is symmetric.
 *
 * @param A CSR operator.
 * @param k Number of eigenpairs.
 * @param which Largest or smallest end.
 * @param m Krylov length. `k <= m <= n`.
 * @param tol Relative residual tolerance.
 * @param max_restarts Restart cap.
 * @param eigenvalues Output eigenvalues, length `k`.
 * @param eigenvectors Optional `n × k` eigenvectors. NULL skips them.
 * @param info Optional iteration stats. May be NULL.
 *
 * @return true when the worst returned residual is within `tol`.
 */
VEC_API bool vm_lanczos_sparse(const vm_spmat *A, int k, vm_lanczos_which which,
                               int m, vm_float_t tol, int max_restarts,
                               vm_float_t *eigenvalues, vm_mat *eigenvectors,
                               vm_ksp_info *info);

#endif /* VECMAT_LANCZOS_H */
