// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <stddef.h>
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
 * @brief Copy `in^T` into `out`, resizing as needed.
 *
 * @param out Destination transpose.
 * @param in Source matrix.
 * @return True on success.
 */
static bool vm_mat_transpose_copy(vm_mat *out, const vm_mat *in)
{
    if (!vm_mat_resize(out, in->cols, in->rows)) {
        return false;
    }
    for (int c = 0; c < in->cols; ++c) {
        for (int r = 0; r < in->rows; ++r) {
            *vm_mat_at(out, c, r) = *vm_mat_at(in, r, c);
        }
    }
    return true;
}

/**
 * @brief Dot product of columns `p` and `q`.
 *
 * @param A Matrix.
 * @param p First column.
 * @param q Second column.
 * @return Column inner product.
 */
static vm_float_t vm_col_dot(const vm_mat *A, const int p, const int q)
{
    vm_float_t s = VM_F(0.0);
    for (int r = 0; r < A->rows; ++r) {
        s += *vm_mat_at(A, r, p) * *vm_mat_at(A, r, q);
    }
    return s;
}

/**
 * @brief Swap columns `p` and `q` in place.
 *
 * @param A Matrix (storage is mutated).
 * @param p First column.
 * @param q Second column.
 */
static void vm_swap_columns(const vm_mat *A, const int p, const int q)
{
    if (p == q) {
        return;
    }
    for (int r = 0; r < A->rows; ++r) {
        const vm_float_t t = *vm_mat_at(A, r, p);
        *vm_mat_at(A, r, p) = *vm_mat_at(A, r, q);
        *vm_mat_at(A, r, q) = t;
    }
}

/**
 * @brief Sort singular values descending and permute U/V columns to match.
 *
 * @param U Left singular vectors.
 * @param s Singular values, length `k`.
 * @param V Right singular vectors.
 * @param k Number of values / columns.
 */
static void vm_sort_singular(const vm_mat *U, vm_float_t *s, const vm_mat *V, const int k)
{
    for (int i = 0; i < k; ++i) {
        int best = i;
        for (int j = i + 1; j < k; ++j) {
            if (s[j] > s[best]) {
                best = j;
            }
        }
        if (best == i) {
            continue;
        }
        const vm_float_t tmp = s[i];
        s[i] = s[best];
        s[best] = tmp;
        vm_swap_columns(U, i, best);
        vm_swap_columns(V, i, best);
    }
}

/**
 * @brief One-sided Jacobi SVD of a tall-or-square copy `B` (m x n, m >= n).
 *
 * On exit columns of `B` are u_j * s_j and `V` is n x n (then trimmed to n x k).
 *
 * @param B Tall working copy, overwritten.
 * @param V Right factor, resized to n x n.
 * @return True on success.
 */
static bool vm_svd_jacobi_tall(const vm_mat *B, vm_mat *V)
{
    const int m = B->rows;
    const int n = B->cols;
    (void)m;

    if (!vm_mat_resize(V, n, n)) {
        return false;
    }
    for (int i = 0; i < n; ++i) {
        vm_mat_set(V, i, i, VM_F(1.0));
    }

    const vm_float_t scale = vm_mat_max_abs(B);
    const vm_float_t off_tol = vm_factor_tol(scale * scale, n);

    for (int sweep = 0; sweep < 64; ++sweep) {
        vm_float_t off = VM_F(0.0);
        for (int p = 0; p < n - 1; ++p) {
            for (int q = p + 1; q < n; ++q) {
                const vm_float_t a = vm_col_dot(B, p, p);
                const vm_float_t b = vm_col_dot(B, q, q);
                const vm_float_t c = vm_col_dot(B, p, q);
                off += VECMAT_FABS(c);
                const vm_float_t pair_scale = VECMAT_FMAX(a + b, scale * scale);
                if (VECMAT_FABS(c) <= VECMAT_EPSILON * (VM_F(1.0) + pair_scale)) {
                    continue;
                }

                const vm_float_t tau = (b - a) / (VM_F(2.0) * c);
                const vm_float_t t = (tau >= 0)
                    ? VM_F(1.0) / (tau + VECMAT_SQRT(VM_F(1.0) + tau * tau))
                    : VM_F(-1.0) / (-tau + VECMAT_SQRT(VM_F(1.0) + tau * tau));
                const vm_float_t cs = VM_F(1.0) / VECMAT_SQRT(VM_F(1.0) + t * t);
                const vm_float_t sn = t * cs;

                for (int r = 0; r < B->rows; ++r) {
                    const vm_float_t bp = *vm_mat_at(B, r, p);
                    const vm_float_t bq = *vm_mat_at(B, r, q);
                    *vm_mat_at(B, r, p) = cs * bp - sn * bq;
                    *vm_mat_at(B, r, q) = sn * bp + cs * bq;
                }
                for (int r = 0; r < n; ++r) {
                    const vm_float_t vp = *vm_mat_at(V, r, p);
                    const vm_float_t vq = *vm_mat_at(V, r, q);
                    *vm_mat_at(V, r, p) = cs * vp - sn * vq;
                    *vm_mat_at(V, r, q) = sn * vp + cs * vq;
                }
            }
        }
        if (off <= off_tol) {
            break;
        }
    }
    return true;
}

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
bool vm_svd_factor(const vm_mat *A, vm_mat *U, vm_float_t *s, vm_mat *V)
{
    if (!A || !A->data || !U || !s || !V || A->rows <= 0 || A->cols <= 0) {
        return false;
    }

    const int m = A->rows;
    const int n = A->cols;
    const int k = vm_min_int(m, n);
    const int tall = (m >= n);

    vm_mat B = { 0 };
    vm_mat V_full = { 0 };
    if (tall) {
        if (!vm_mat_copy(&B, A)) {
            return false;
        }
    } else if (!vm_mat_transpose_copy(&B, A)) {
        return false;
    }

    if (!vm_svd_jacobi_tall(&B, &V_full)) {
        vm_mat_free(&B);
        vm_mat_free(&V_full);
        return false;
    }

    /* B is now (tall ? m : n) × k_work, k_work = min side of original as cols. */
    const int work_n = B.cols;
    if (!vm_mat_resize(U, m, k) || !vm_mat_resize(V, n, k)) {
        vm_mat_free(&B);
        vm_mat_free(&V_full);
        return false;
    }

    vm_float_t smax = VM_F(0.0);
    for (int j = 0; j < work_n; ++j) {
        s[j] = VECMAT_SQRT(vm_col_dot(&B, j, j));
        if (s[j] > smax) {
            smax = s[j];
        }
    }
    const vm_float_t zero_tol = vm_factor_tol(smax, work_n);

    for (int j = 0; j < work_n; ++j) {
        vm_float_t nrm = s[j];
        if (nrm <= zero_tol) {
            s[j] = VM_F(0.0);
            nrm = VM_F(1.0);
        }
        if (tall) {
            for (int r = 0; r < m; ++r) {
                vm_mat_set(U, r, j, *vm_mat_at(&B, r, j) / nrm);
            }
            for (int r = 0; r < n; ++r) {
                vm_mat_set(V, r, j, *vm_mat_at(&V_full, r, j));
            }
        } else {
            /* SVD(A^T) = V_A Σ U_A^T, so U comes from V_full and V from B. */
            for (int r = 0; r < n; ++r) {
                vm_mat_set(V, r, j, *vm_mat_at(&B, r, j) / nrm);
            }
            for (int r = 0; r < m; ++r) {
                vm_mat_set(U, r, j, *vm_mat_at(&V_full, r, j));
            }
        }
    }

    vm_sort_singular(U, s, V, k);
    vm_mat_free(&B);
    vm_mat_free(&V_full);
    return true;
}
