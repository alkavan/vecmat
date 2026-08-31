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
 * @brief Integer minimum.
 *
 * @param a First value.
 * @param b Second value.
 * @return The smaller of `a` and `b`.
 */
static int vm_min_int(const int a, const int b)
{
    return a < b ? a : b;
}

/**
 * @brief Ensure `m` is `rows` x `cols`, zeroed.
 *
 * Reuses the buffer when the size already matches.
 *
 * @param m Matrix to resize.
 * @param rows Desired rows.
 * @param cols Desired columns.
 * @return True if `m->data` is valid.
 */
static bool vm_mat_resize(vm_mat *m, const int rows, const int cols)
{
    if (m && m->data && m->rows == rows && m->cols == cols) {
        vm_mat_zero(m);
        return true;
    }
    vm_mat_free(m);
    *m = vm_mat_alloc(rows, cols);
    return m->data != NULL;
}

/**
 * @brief Euclidean norm of column `c` from row `r0` downward.
 *
 * @param A Matrix.
 * @param r0 First row of the slice.
 * @param c Column index.
 * @return 2-norm of the slice.
 */
static vm_float_t vm_householder_norm(const vm_mat *A, const int r0, const int c)
{
    vm_float_t s = VM_F(0.0);
    for (int r = r0; r < A->rows; ++r) {
        const vm_float_t v = *vm_mat_at(A, r, c);
        s += v * v;
    }
    return VECMAT_SQRT(s);
}

/**
 * @brief Apply the k-th stored Householder reflector to rows k:m of a panel.
 *
 * `n_cols` consecutive columns starting at `c0` are updated in place.
 *
 * @param M Panel being updated (may alias `QR`).
 * @param QR Factored matrix holding Householder vectors below the diagonal.
 * @param tau Householder scale for column `k`.
 * @param k Reflector index.
 * @param c0 First column of the panel.
 * @param n_cols Number of columns to update.
 */
static void vm_apply_householder(const vm_mat *M, const vm_mat *QR, const vm_float_t tau,
                                 const int k, const int c0, const int n_cols)
{
    if (tau == VM_F(0.0)) {
        return;
    }
    for (int j = 0; j < n_cols; ++j) {
        const int c = c0 + j;
        vm_float_t dot = *vm_mat_at(M, k, c);
        for (int r = k + 1; r < QR->rows; ++r) {
            dot += *vm_mat_at(QR, r, k) * *vm_mat_at(M, r, c);
        }
        const vm_float_t w = tau * dot;
        *vm_mat_at(M, k, c) -= w;
        for (int r = k + 1; r < QR->rows; ++r) {
            *vm_mat_at(M, r, c) -= w * *vm_mat_at(QR, r, k);
        }
    }
}

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
bool vm_qr_factor(vm_mat *A, vm_float_t *tau)
{
    if (!A || !A->data || !tau || A->rows <= 0 || A->cols <= 0) {
        return false;
    }

    const int m = A->rows;
    const int n = A->cols;
    const int kmax = vm_min_int(m, n);
    const vm_float_t tol = vm_factor_tol(vm_mat_max_abs(A), vm_min_int(m, n));

    for (int k = 0; k < kmax; ++k) {
        const vm_float_t nrm = vm_householder_norm(A, k, k);
        const vm_float_t akk = *vm_mat_at(A, k, k);

        if (nrm <= tol) {
            tau[k] = VM_F(0.0);
            continue;
        }

        const vm_float_t beta = -VECMAT_COPYSIGN(nrm, akk);
        tau[k] = (beta - akk) / beta;
        const vm_float_t scale = VM_F(1.0) / (akk - beta);
        for (int r = k + 1; r < m; ++r) {
            *vm_mat_at(A, r, k) *= scale;
        }
        *vm_mat_at(A, k, k) = beta;

        if (k + 1 < n) {
            vm_apply_householder(A, A, tau[k], k, k + 1, n - (k + 1));
        }
    }
    return true;
}

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
bool vm_qr_unpack(vm_mat *Q, vm_mat *R, const vm_mat *QR, const vm_float_t *tau)
{
    if (!Q || !R || !QR || !QR->data || !tau) {
        return false;
    }

    const int m = QR->rows;
    const int n = QR->cols;
    const int k = vm_min_int(m, n);

    if (!vm_mat_resize(Q, m, k) || !vm_mat_resize(R, k, n)) {
        return false;
    }

    for (int j = 0; j < k; ++j) {
        vm_mat_set(Q, j, j, VM_F(1.0));
    }

    for (int i = k - 1; i >= 0; --i) {
        vm_apply_householder(Q, QR, tau[i], i, 0, k);
    }

    for (int c = 0; c < n; ++c) {
        const int rmax = vm_min_int(c + 1, k);
        for (int r = 0; r < rmax; ++r) {
            vm_mat_set(R, r, c, *vm_mat_at(QR, r, c));
        }
    }
    return true;
}

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
bool vm_qr_solve(const vm_mat *QR, const vm_float_t *tau, const vm_float_t *b, vm_float_t *x)
{
    if (!QR || !QR->data || !tau || !b || !x || QR->rows < QR->cols) {
        return false;
    }

    const int m = QR->rows;
    const int n = QR->cols;
    vm_float_t r_diag_max = VM_F(0.0);
    for (int i = 0; i < n; ++i) {
        const vm_float_t a = VECMAT_FABS(*vm_mat_at(QR, i, i));
        if (a > r_diag_max) {
            r_diag_max = a;
        }
    }
    const vm_float_t tol = vm_factor_tol(r_diag_max, n);
    vm_float_t *y = (vm_float_t *)malloc((size_t)m * sizeof(vm_float_t));
    if (!y) {
        return false;
    }
    for (int i = 0; i < m; ++i) {
        y[i] = b[i];
    }

    /* y <- Q^T b, applied as the same reflectors in forward order. */
    for (int k = 0; k < n; ++k) {
        if (tau[k] == VM_F(0.0)) {
            continue;
        }
        vm_float_t dot = y[k];
        for (int r = k + 1; r < m; ++r) {
            dot += *vm_mat_at(QR, r, k) * y[r];
        }
        const vm_float_t w = tau[k] * dot;
        y[k] -= w;
        for (int r = k + 1; r < m; ++r) {
            y[r] -= w * *vm_mat_at(QR, r, k);
        }
    }

    for (int i = n - 1; i >= 0; --i) {
        vm_float_t s = y[i];
        for (int j = i + 1; j < n; ++j) {
            s -= *vm_mat_at(QR, i, j) * x[j];
        }
        const vm_float_t rii = *vm_mat_at(QR, i, i);
        if (VECMAT_FABS(rii) <= tol) {
            free(y);
            return false;
        }
        x[i] = s / rii;
    }

    free(y);
    return true;
}
