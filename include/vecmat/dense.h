// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_DENSE_H
#define VECMAT_DENSE_H

/**
 * @file
 * @brief Heap `vm_mat` plus LU / QR / SVD / Cholesky. Rank-deficient work returns `false`, not a silent NaN.
 * @ingroup vecmat_extras
 */

#include "vecmat/types.h"
#include "vecmat/abi.h"

/**
 * @brief Allocates a new matrix with the specified dimensions.
 *
 * Memory is allocated using calloc to initialize all elements to zero.
 * If allocation fails or if dimensions are invalid, a matrix with
 * zero dimensions and NULL data pointer is returned.
 *
 * @param rows number of rows in the matrix
 * @param cols number of columns in the matrix
 * @return allocated matrix or zero matrix on failure
 */
VEC_API vm_mat vm_mat_alloc(int rows, int cols);

/**
 * @brief Frees the memory allocated for a matrix and resets its state.
 *
 * If the matrix pointer is NULL, the function returns immediately.
 * The data pointer is freed (if allocated), then set to NULL and
 * both dimensions are reset to zero.
 *
 * @param m pointer to the matrix to free
 */
VEC_API void vm_mat_free(vm_mat *m);

/**
 * @brief Sets all elements of the matrix to zero.
 *
 * If the matrix pointer or its data pointer is NULL, the function returns
 * immediately without performing any operation.
 *
 * @param m pointer to the matrix to be zeroed
 */
VEC_API void vm_mat_zero(vm_mat *m);

/**
 * @brief Returns the value of the matrix element at the specified row and column.
 *
 * If the matrix pointer is NULL, the data pointer is NULL, or the row or
 * column indices are out of bounds, the function returns 0.0f without
 * accessing memory.
 *
 * @param m pointer to the matrix
 * @param r zero-based row index
 * @param c zero-based column index
 * @return value at (r, c) or 0.0f on invalid input
 */
VEC_API vm_float_t vm_mat_get(const vm_mat *m, int r, int c);

/**
 * @brief Sets the element at the specified row and column in the matrix to the given value.
 *
 * The function performs bounds checking and validates the matrix pointer and
 * its data buffer before performing the assignment. If any validation fails,
 * the function returns without modifying any data.
 *
 * @param m pointer to the matrix to modify
 * @param r row index
 * @param c column index
 * @param v value to set at the specified position
 */
VEC_API void vm_mat_set(vm_mat *m, int r, int c, vm_float_t v);

/**
 * @brief Copies the contents of one matrix to another.
 *
 * If the destination matrix has incompatible dimensions or unallocated
 * data it is freed and reallocated to match the source dimensions.
 * The source matrix must be valid and have allocated data.
 *
 * @param dst pointer to the destination matrix
 * @param src pointer to the source matrix
 * @return true on success, false if either pointer is NULL, source data
 *         is NULL or memory allocation fails
 */
VEC_API bool vm_mat_copy(vm_mat *dst, const vm_mat *src);

/**
 * @brief Determinant of a square matrix via LU.
 *
 * @param A Square matrix (not modified).
 * @return det(A), or 0 on failure.
 */
VEC_API vm_float_t vm_mat_det(const vm_mat *A);

/**
 * @brief Inverse of a square matrix via LU.
 *
 * Allocates or resizes `out` when it is not already `n` x `n`.
 *
 * @param out Inverse on success.
 * @param A Square matrix (not modified).
 * @return True on success.
 */
VEC_API bool vm_mat_inverse(vm_mat *out, const vm_mat *A);

/**
 * @brief In-place LU factorization with partial pivoting.
 *
 * On success `A` holds L (unit diagonal, strictly below) and U (on and
 * above the diagonal). `pivot[i]` is the original row now at position `i`.
 *
 * @param A Square matrix, overwritten with L and U.
 * @param pivot Row permutation; length `A->rows`.
 * @param sign Optional; set to +1 or -1 for the permutation sign.
 * @return True on success.
 */
VEC_API bool vm_lu_factor(vm_mat *A, int *pivot, int *sign);

/**
 * @brief Solves `A x = b` from a factored LU.
 *
 * @param LU Factored matrix from `vm_lu_factor`.
 * @param pivot Row permutation from `vm_lu_factor`.
 * @param b Right-hand side, length `n`.
 * @param x Solution, length `n`.
 * @return True on success.
 */
VEC_API bool vm_lu_solve(const vm_mat *LU, const int *pivot, const vm_float_t *b, vm_float_t *x);

/**
 * @brief In-place Householder QR. `A` is m x n.
 *
 * On success the upper triangle of `A` is R and the strict lower part
 * stores Householder vectors. `tau` must hold `min(m, n)` scalars.
 *
 * @param A Matrix overwritten with R and Householder vectors.
 * @param tau Householder scales, length `min(m, n)`.
 * @return True on success.
 */
VEC_API bool vm_qr_factor(vm_mat *A, vm_float_t *tau);

/**
 * @brief Thin factors: Q is m x k, R is k x n, k = min(m, n).
 *
 * Allocates or resizes `Q` and `R` when they do not already match.
 *
 * @param Q Orthonormal factor on success.
 * @param R Upper-triangular factor on success.
 * @param QR Factored matrix from `vm_qr_factor`.
 * @param tau Householder scales from `vm_qr_factor`.
 * @return True on success.
 */
VEC_API bool vm_qr_unpack(vm_mat *Q, vm_mat *R, const vm_mat *QR, const vm_float_t *tau);

/**
 * @brief Least-squares solve `min ||A x - b||` from a factored QR.
 *
 * Requires `m >= n`. `b` has length `m`, `x` has length `n`.
 *
 * @param QR Factored matrix from `vm_qr_factor`.
 * @param tau Householder scales from `vm_qr_factor`.
 * @param b Right-hand side, length `m`.
 * @param x Solution, length `n`.
 * @return True on success.
 */
VEC_API bool vm_qr_solve(const vm_mat *QR, const vm_float_t *tau, const vm_float_t *b, vm_float_t *x);

/**
 * @brief Thin SVD `A = U diag(s) V^T`.
 *
 * `s` has length `k = min(m, n)` (descending). `U` is m x k, `V` is n x k
 * (columns are singular vectors). Allocates or resizes `U` and `V`.
 *
 * @param A Input matrix (not modified).
 * @param U Left singular vectors on success.
 * @param s Singular values, length `min(m, n)`.
 * @param V Right singular vectors on success.
 * @return True on success.
 */
VEC_API bool vm_svd_factor(const vm_mat *A, vm_mat *U, vm_float_t *s, vm_mat *V);

/**
 * @brief In-place dense Cholesky A = L Lᵀ (lower triangle overwritten).
 *
 * `A` must be square SPD. The strict upper triangle is left untouched.
 *
 * @param A Square dense matrix (destroyed / factored in place).
 * @return true on success, false if not SPD or invalid input.
 */
VEC_API bool vm_chol_factor(vm_mat *A);

/**
 * @brief Solve L Lᵀ x = b after `vm_chol_factor`.
 *
 * @param L Factored lower triangle from `vm_chol_factor`.
 * @param b Right-hand side (length n).
 * @param x Solution (length n).
 * @return true on success, false on error.
 */
VEC_API bool vm_chol_solve(const vm_mat *L, const vm_float_t *b, vm_float_t *x);

#endif //VECMAT_DENSE_H
