// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <stdlib.h>
#include <vecmat.h>

/**
 * @brief Column-major element pointer `data[r + c * rows]`.
 *
 * @param m Matrix.
 * @param r Row index.
 * @param c Column index.
 * @return Pointer to element (r, c).
 */
static vm_float_t *vm_mat_at(const vm_mat *m, const int r, const int c)
{
    return &m->data[r + c * m->rows];
}

/**
 * @brief Largest absolute entry of `m`.
 *
 * @param m Matrix.
 * @return Max |m_ij|, or 0 if empty.
 */
static vm_float_t vm_mat_max_abs(const vm_mat *m)
{
    vm_float_t s = VM_F(0.0);
    const int n = m->rows * m->cols;
    for (int i = 0; i < n; ++i) {
        const vm_float_t a = VECMAT_FABS(m->data[i]);
        if (a > s) {
            s = a;
        }
    }
    return s;
}

/**
 * @brief Scale-aware cutoff `n * eps * scale` (at least `eps`).
 *
 * @param scale Typical magnitude of the matrix.
 * @param n Matrix order (clamped to at least 1).
 * @return Factorization / rank tolerance.
 */
static vm_float_t vm_factor_tol(const vm_float_t scale, const int n)
{
    if (scale <= VM_F(0.0)) {
        return VECMAT_EPSILON;
    }
    const int dim = (n > 1) ? n : 1;
    return VECMAT_EPSILON * (vm_float_t)dim * scale;
}

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
bool vm_lu_factor(vm_mat *A, int *pivot, int *sign)
{
    if (!A || !A->data || !pivot || A->rows <= 0 || A->rows != A->cols) {
        return false;
    }

    const int n = A->rows;
    int sgn = 1;
    const vm_float_t tol = vm_factor_tol(vm_mat_max_abs(A), n);

    for (int i = 0; i < n; ++i) {
        pivot[i] = i;
    }

    for (int k = 0; k < n; ++k) {
        int piv = k;
        vm_float_t max_abs = VECMAT_FABS(*vm_mat_at(A, k, k));
        for (int i = k + 1; i < n; ++i) {
            const vm_float_t a = VECMAT_FABS(*vm_mat_at(A, i, k));
            if (a > max_abs) {
                max_abs = a;
                piv = i;
            }
        }
        if (max_abs <= tol) {
            return false;
        }
        if (piv != k) {
            for (int j = 0; j < n; ++j) {
                const vm_float_t tmp = *vm_mat_at(A, k, j);
                *vm_mat_at(A, k, j) = *vm_mat_at(A, piv, j);
                *vm_mat_at(A, piv, j) = tmp;
            }
            const int tp = pivot[k];
            pivot[k] = pivot[piv];
            pivot[piv] = tp;
            sgn = -sgn;
        }

        const vm_float_t akk = *vm_mat_at(A, k, k);
        for (int i = k + 1; i < n; ++i) {
            *vm_mat_at(A, i, k) /= akk;
            const vm_float_t lik = *vm_mat_at(A, i, k);
            for (int j = k + 1; j < n; ++j) {
                *vm_mat_at(A, i, j) -= lik * *vm_mat_at(A, k, j);
            }
        }
    }

    if (sign) {
        *sign = sgn;
    }
    return true;
}

/**
 * @brief Solves `A x = b` from a factored LU.
 *
 * @param LU Factored matrix from `vm_lu_factor`.
 * @param pivot Row permutation from `vm_lu_factor`.
 * @param b Right-hand side, length `n`.
 * @param x Solution, length `n`.
 * @return True on success.
 */
bool vm_lu_solve(const vm_mat *LU, const int *pivot, const vm_float_t *b, vm_float_t *x)
{
    if (!LU || !LU->data || !pivot || !b || !x || LU->rows != LU->cols) {
        return false;
    }

    const int n = LU->rows;
    vm_float_t uscale = VM_F(0.0);
    for (int i = 0; i < n; ++i) {
        const vm_float_t a = VECMAT_FABS(*vm_mat_at(LU, i, i));
        if (a > uscale) {
            uscale = a;
        }
    }
    const vm_float_t tol = vm_factor_tol(uscale, n);
    for (int i = 0; i < n; ++i) {
        x[i] = b[pivot[i]];
    }

    for (int i = 0; i < n; ++i) {
        vm_float_t s = x[i];
        for (int j = 0; j < i; ++j) {
            s -= *vm_mat_at(LU, i, j) * x[j];
        }
        x[i] = s;
    }

    for (int i = n - 1; i >= 0; --i) {
        vm_float_t s = x[i];
        for (int j = i + 1; j < n; ++j) {
            s -= *vm_mat_at(LU, i, j) * x[j];
        }
        const vm_float_t uii = *vm_mat_at(LU, i, i);
        if (VECMAT_FABS(uii) <= tol) {
            return false;
        }
        x[i] = s / uii;
    }
    return true;
}

/**
 * @brief Determinant of a square matrix via LU.
 *
 * @param A Square matrix (not modified).
 * @return det(A), or 0 on failure.
 */
vm_float_t vm_mat_det(const vm_mat *A)
{
    if (!A || !A->data || A->rows != A->cols || A->rows <= 0) {
        return VM_F(0.0);
    }

    vm_mat lu = { 0 };
    if (!vm_mat_copy(&lu, A)) {
        return VM_F(0.0);
    }

    int *pivot = (int *)malloc((size_t)lu.rows * sizeof(int));
    if (!pivot) {
        vm_mat_free(&lu);
        return VM_F(0.0);
    }

    int sign = 1;
    vm_float_t det = VM_F(0.0);
    if (vm_lu_factor(&lu, pivot, &sign)) {
        det = (vm_float_t)sign;
        for (int i = 0; i < lu.rows; ++i) {
            det *= *vm_mat_at(&lu, i, i);
        }
    }

    free(pivot);
    vm_mat_free(&lu);
    return det;
}

/**
 * @brief Inverse of a square matrix via LU.
 *
 * Allocates or resizes `out` when it is not already `n` x `n`.
 *
 * @param out Inverse on success.
 * @param A Square matrix (not modified).
 * @return True on success.
 */
bool vm_mat_inverse(vm_mat *out, const vm_mat *A)
{
    if (!out || !A || !A->data || A->rows != A->cols || A->rows <= 0) {
        return false;
    }

    vm_mat lu = { 0 };
    if (!vm_mat_copy(&lu, A)) {
        return false;
    }

    const int n = A->rows;
    int *pivot = (int *)malloc((size_t)n * sizeof(int));
    vm_float_t *b = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *x = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    if (!pivot || !b || !x) {
        free(pivot);
        free(b);
        free(x);
        vm_mat_free(&lu);
        return false;
    }

    if (!vm_lu_factor(&lu, pivot, NULL)) {
        free(pivot);
        free(b);
        free(x);
        vm_mat_free(&lu);
        return false;
    }

    if (!out->data || out->rows != n || out->cols != n) {
        vm_mat_free(out);
        *out = vm_mat_alloc(n, n);
        if (!out->data) {
            free(pivot);
            free(b);
            free(x);
            vm_mat_free(&lu);
            return false;
        }
    }

    for (int col = 0; col < n; ++col) {
        for (int i = 0; i < n; ++i) {
            b[i] = (i == col) ? VM_F(1.0) : VM_F(0.0);
        }
        if (!vm_lu_solve(&lu, pivot, b, x)) {
            free(pivot);
            free(b);
            free(x);
            vm_mat_free(&lu);
            return false;
        }
        for (int i = 0; i < n; ++i) {
            vm_mat_set(out, i, col, x[i]);
        }
    }

    free(pivot);
    free(b);
    free(x);
    vm_mat_free(&lu);
    return true;
}
