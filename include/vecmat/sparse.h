// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_SPARSE_H
#define VECMAT_SPARSE_H

/**
 * @file
 * @brief CSR `vm_spmat` and Krylov solvers (CG / BiCGSTAB) with Jacobi / SSOR / IC(0).
 * @ingroup vecmat_extras
 */

#include "vecmat/types.h"
#include "vecmat/abi.h"

/**
 * @brief Sparse matrix triplet (row, column, value).
 *
 * Initialize a sparse matrix to empty.
 *
 * @param A Sparse matrix (may be NULL).
 */
VEC_API void vm_spmat_init(vm_spmat *A);

/**
 * @brief Free sparse matrix storage and reset it.
 *
 * @param A Sparse matrix (may be NULL).
 */
VEC_API void vm_spmat_free(vm_spmat *A);

/**
 * @brief Build a square CSR matrix from unsorted (row, col, val) triplets.
 *
 * Duplicate (i, j) entries are summed. Out-of-range indices are skipped.
 * On success, existing storage in `A` is freed and replaced.
 *
 * @param A Output sparse matrix.
 * @param n Matrix size (n×n).
 * @param nnz Number of input triplets.
 * @param row Row indices (length nnz).
 * @param col Column indices (length nnz).
 * @param val Values (length nnz).
 *
 * @return true on success, false on error.
 */
VEC_API bool vm_spmat_from_triplets(vm_spmat *A, int n, int nnz, const int *row, const int *col, const vm_float_t *val);

/**
 * @brief Sparse matrix–vector product y = A x.
 *
 * @param y Output vector (length A->n).
 * @param A CSR matrix.
 * @param x Input vector (length A->n).
 */
VEC_API void vm_spmv(vm_float_t *y, const vm_spmat *A, const vm_float_t *x);

/**
 * @brief Extract the main diagonal of A into d.
 *
 * Missing diagonal entries are set to 0.
 *
 * @param A CSR matrix.
 * @param d Output diagonal (length A->n).
 * @return true on success, false on error.
 */
VEC_API bool vm_spmat_diag(const vm_spmat *A, vm_float_t *d);

/**
 * @brief Conjugate gradient for SPD A x = b.
 *
 * `x` is the initial guess and the solution. `tol` is relative residual
 * ||r|| / max(||b||, ε).
 *
 * @param A SPD CSR matrix.
 * @param b Right-hand side (length A->n).
 * @param x Initial guess / solution (length A->n).
 * @param tol Relative residual tolerance (≤0 picks a default).
 * @param max_iter Max iterations (≤0 defaults to n).
 * @param pre_cond NONE, JACOBI, SSOR, or IC0.
 * @param info Optional solver stats (may be NULL).
 * @return true if converged, false otherwise.
 *
 * Relative residual is `||r|| / max(||b||, ε)`.
 */
VEC_API bool vm_cg(const vm_spmat *A, const vm_float_t *b, vm_float_t *x,
    vm_float_t tol, int max_iter, vm_ksp_prec_t pre_cond, vm_ksp_info *info);

/**
 * @brief BiCGSTAB for general (possibly nonsymmetric) A x = b.
 *
 * Same calling convention as `vm_cg`. Jacobi / SSOR / IC0 are left
 * preconditioners; IC0 still expects an SPD-like diagonal.
 *
 * @param A CSR matrix.
 * @param b Right-hand side (length A->n).
 * @param x Initial guess / solution (length A->n).
 * @param tol Relative residual tolerance (≤0 picks a default).
 * @param max_iter Max iterations (≤0 defaults to 2n).
 * @param pre_cond NONE, JACOBI, SSOR, or IC0.
 * @param info Optional solver stats (may be NULL).
 *
 * @return true if converged, false otherwise.
 *
 * @see https://www.ctcms.nist.gov/~langer/oof2man/RegisteredClass-StabilizedBiConjugateGradient.html
 *
 * Relative residual is `||r|| / max(||b||, ε)`.
 */
VEC_API bool vm_bicgstab(const vm_spmat *A, const vm_float_t *b, vm_float_t *x,
    vm_float_t tol, int max_iter, vm_ksp_prec_t pre_cond, vm_ksp_info *info);

#endif //VECMAT_SPARSE_H
