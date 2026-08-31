// SPDX-FileCopyrightText: Copyright (c) 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: Copyright (c) 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <stdlib.h>
#include <string.h>
#include <vecmat.h>

/**
 * @brief Sparse matrix triplet (row, column, value).
 */
typedef struct {
    int r;         /**< Row index */
    int c;         /**< Column index */
    vm_float_t v;  /**< Entry value */
} vm_trip;

/**
 * @brief Initialize a sparse matrix to empty.
 *
 * @param A Sparse matrix (may be NULL).
 */
void vm_spmat_init(vm_spmat *A)
{
    if (!A) {
        return;
    }
    A->n = 0;
    A->nnz = 0;
    A->row_ptr = NULL;
    A->col = NULL;
    A->val = NULL;
}

/**
 * @brief Free sparse matrix storage and reset it.
 *
 * @param A Sparse matrix (may be NULL).
 */
void vm_spmat_free(vm_spmat *A)
{
    if (!A) {
        return;
    }
    free(A->row_ptr);
    free(A->col);
    free(A->val);
    vm_spmat_init(A);
}

/**
 * @brief Compare triplets by (row, col) for qsort.
 *
 * @param pa First triplet.
 * @param pb Second triplet.
 * @return -1, 0, or 1.
 */
static int vm_trip_cmp(const void *pa, const void *pb)
{
    const vm_trip *a = (const vm_trip *)pa;
    const vm_trip *b = (const vm_trip *)pb;
    if (a->r != b->r) {
        return (a->r < b->r) ? -1 : 1;
    }
    if (a->c != b->c) {
        return (a->c < b->c) ? -1 : 1;
    }
    return 0;
}

/**
 * @brief Dot product of two vectors.
 *
 * @param a First vector.
 * @param b Second vector.
 * @param n Length.
 * @return a·b.
 */
static vm_float_t vm_vec_dot(const vm_float_t *a, const vm_float_t *b, const int n)
{
    vm_float_t s = VM_F(0.0);
    for (int i = 0; i < n; ++i) {
        s += a[i] * b[i];
    }
    return s;
}

/**
 * @brief Euclidean (L2) norm of a vector.
 *
 * @param a Vector.
 * @param n Length.
 * @return ||a||₂.
 */
static vm_float_t vm_vec_norm2(const vm_float_t *a, const int n)
{
    return VECMAT_SQRT(vm_vec_dot(a, a, n));
}

/**
 * @brief y := y + a * x (AXPY).
 *
 * @param y Input/output vector.
 * @param a Scalar.
 * @param x Input vector.
 * @param n Length.
 */
static void vm_vec_axpy(vm_float_t *y, const vm_float_t a, const vm_float_t *x, const int n)
{
    for (int i = 0; i < n; ++i) {
        y[i] += a * x[i];
    }
}

/**
 * @brief Copy a vector.
 *
 * @param dst Destination.
 * @param src Source.
 * @param n Length.
 */
static void vm_vec_copy(vm_float_t *dst, const vm_float_t *src, const int n)
{
    memcpy(dst, src, (size_t)n * sizeof(vm_float_t));
}

/**
 * @brief Set a vector to zero.
 *
 * @param y Vector.
 * @param n Length.
 */
static void vm_vec_zero(vm_float_t *y, const int n)
{
    memset(y, 0, (size_t)n * sizeof(vm_float_t));
}

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
bool vm_spmat_from_triplets(vm_spmat *A, const int n, const int nnz,
                            const int *row, const int *col, const vm_float_t *val)
{
    if (!A || n <= 0 || nnz < 0 || (nnz > 0 && (!row || !col || !val))) {
        return false;
    }

    vm_trip *t = NULL;
    if (nnz > 0) {
        t = (vm_trip *)malloc((size_t)nnz * sizeof(vm_trip));
        if (!t) {
            return false;
        }
    }

    int kept = 0;
    for (int k = 0; k < nnz; ++k) {
        if (row[k] < 0 || row[k] >= n || col[k] < 0 || col[k] >= n) {
            continue;
        }
        t[kept].r = row[k];
        t[kept].c = col[k];
        t[kept].v = val[k];
        ++kept;
    }

    if (kept > 1) {
        qsort(t, (size_t)kept, sizeof(vm_trip), vm_trip_cmp);
    }

    int uniq = 0;
    for (int k = 0; k < kept; ++k) {
        if (uniq > 0 && t[k].r == t[uniq - 1].r && t[k].c == t[uniq - 1].c) {
            t[uniq - 1].v += t[k].v;
        } else {
            t[uniq++] = t[k];
        }
    }

    int *row_ptr = (int *)calloc((size_t)n + 1u, sizeof(int));
    int *cidx = NULL;
    vm_float_t *aval = NULL;

    if (uniq > 0) {
        cidx = (int *)malloc((size_t)uniq * sizeof(int));
        aval = (vm_float_t *)malloc((size_t)uniq * sizeof(vm_float_t));
    }

    if (!row_ptr || (uniq > 0 && (!cidx || !aval))) {
        free(t);
        free(row_ptr);
        free(cidx);
        free(aval);
        return false;
    }

    for (int k = 0; k < uniq; ++k) {
        cidx[k] = t[k].c;
        aval[k] = t[k].v;
        row_ptr[t[k].r + 1] += 1;
    }
    for (int i = 0; i < n; ++i) {
        row_ptr[i + 1] += row_ptr[i];
    }

    free(t);
    vm_spmat_free(A);
    A->n = n;
    A->nnz = uniq;
    A->row_ptr = row_ptr;
    A->col = cidx;
    A->val = aval;
    return true;
}

/**
 * @brief Sparse matrix–vector product y = A x.
 *
 * @param y Output vector (length A->n).
 * @param A CSR matrix.
 * @param x Input vector (length A->n).
 */
void vm_spmv(vm_float_t *y, const vm_spmat *A, const vm_float_t *x)
{
    if (!y || !A || !A->row_ptr || !x || A->n <= 0) {
        return;
    }
    const int n = A->n;
    for (int i = 0; i < n; ++i) {
        vm_float_t s = VM_F(0.0);
        const int end = A->row_ptr[i + 1];
        for (int k = A->row_ptr[i]; k < end; ++k) {
            s += A->val[k] * x[A->col[k]];
        }
        y[i] = s;
    }
}

/**
 * @brief Extract the main diagonal of A into d.
 *
 * Missing diagonal entries are set to 0.
 *
 * @param A CSR matrix.
 * @param d Output diagonal (length A->n).
 * @return true on success, false on error.
 */
bool vm_spmat_diag(const vm_spmat *A, vm_float_t *d)
{
    if (!A || !A->row_ptr || !d || A->n <= 0) {
        return false;
    }
    const int n = A->n;
    vm_vec_zero(d, n);
    for (int i = 0; i < n; ++i) {
        const int end = A->row_ptr[i + 1];
        for (int k = A->row_ptr[i]; k < end; ++k) {
            if (A->col[k] == i) {
                d[i] = A->val[k];
                break;
            }
        }
    }
    return true;
}

/**
 * @brief Apply Jacobi preconditioner: z_i = r_i / d_i.
 *
 * @param z Output vector.
 * @param d Diagonal.
 * @param r Residual.
 * @param n Length.
 */
static void vm_jacobi_apply(vm_float_t *z, const vm_float_t *d, const vm_float_t *r, const int n)
{
    for (int i = 0; i < n; ++i) {
        const vm_float_t di = d[i];
        z[i] = (VECMAT_FABS(di) > VECMAT_EPSILON) ? (r[i] / di) : r[i];
    }
}

/**
 * @brief Apply symmetric Gauss–Seidel (SSOR, ω = 1): (D+L) D⁻¹ (D+U) z = r.
 *
 * @param z Output vector.
 * @param A CSR matrix.
 * @param d Diagonal of A.
 * @param r Residual.
 * @param tmp Workspace (length n).
 */
static void vm_ssor_apply(vm_float_t *z, const vm_spmat *A, const vm_float_t *d,
                          const vm_float_t *r, vm_float_t *tmp)
{
    const int n = A->n;
    for (int i = 0; i < n; ++i) {
        vm_float_t s = r[i];
        const int end = A->row_ptr[i + 1];
        for (int k = A->row_ptr[i]; k < end; ++k) {
            const int j = A->col[k];
            if (j < i) {
                s -= A->val[k] * tmp[j];
            }
        }
        const vm_float_t di = d[i];
        tmp[i] = (VECMAT_FABS(di) > VECMAT_EPSILON) ? (s / di) : s;
    }
    for (int i = n - 1; i >= 0; --i) {
        const vm_float_t di = d[i];
        vm_float_t s = (VECMAT_FABS(di) > VECMAT_EPSILON) ? (di * tmp[i]) : tmp[i];
        const int end = A->row_ptr[i + 1];
        for (int k = A->row_ptr[i]; k < end; ++k) {
            const int j = A->col[k];
            if (j > i) {
                s -= A->val[k] * z[j];
            }
        }
        z[i] = (VECMAT_FABS(di) > VECMAT_EPSILON) ? (s / di) : s;
    }
}

/**
 * @brief Incomplete Cholesky IC(0) factor for preconditioning.
 */
typedef struct {
    vm_spmat L;  /**< Lower factor L in CSR (A ≈ L Lᵀ) */
    int *diag;   /**< Index of diagonal entry in each row of L */
    bool ok;     /**< true if factorization succeeded */
} vm_ic0;

/**
 * @brief Free IC(0) factor data.
 *
 * @param ic IC(0) handle (may be NULL).
 */
static void vm_ic0_free(vm_ic0 *ic)
{
    if (!ic) {
        return;
    }
    vm_spmat_free(&ic->L);
    free(ic->diag);
    ic->diag = NULL;
    ic->ok = false;
}

/**
 * @brief Build incomplete Cholesky IC(0) factor of A (lower triangle).
 *
 * @param ic Output IC(0) handle.
 * @param A SPD-like CSR matrix with a full diagonal.
 * @return true on success, false on breakdown or error.
 */
static bool vm_ic0_factor(vm_ic0 *ic, const vm_spmat *A)
{
    vm_spmat_init(&ic->L);
    ic->diag = NULL;
    ic->ok = false;
    if (!A || !A->row_ptr || A->n <= 0) {
        return false;
    }

    const int n = A->n;
    int lower = 0;
    for (int i = 0; i < n; ++i) {
        const int end = A->row_ptr[i + 1];
        for (int k = A->row_ptr[i]; k < end; ++k) {
            if (A->col[k] <= i) {
                ++lower;
            }
        }
    }

    int *row_ptr = (int *)calloc((size_t)n + 1u, sizeof(int));
    int *col = (int *)malloc((size_t)lower * sizeof(int));
    vm_float_t *val = (vm_float_t *)malloc((size_t)lower * sizeof(vm_float_t));
    int *diag = (int *)malloc((size_t)n * sizeof(int));
    if (!row_ptr || (lower > 0 && (!col || !val)) || !diag) {
        free(row_ptr);
        free(col);
        free(val);
        free(diag);
        return false;
    }

    int w = 0;
    for (int i = 0; i < n; ++i) {
        row_ptr[i] = w;
        diag[i] = -1;
        const int end = A->row_ptr[i + 1];
        for (int k = A->row_ptr[i]; k < end; ++k) {
            if (A->col[k] <= i) {
                col[w] = A->col[k];
                val[w] = A->val[k];
                if (A->col[k] == i) {
                    diag[i] = w;
                }
                ++w;
            }
        }
        if (diag[i] < 0) {
            free(row_ptr);
            free(col);
            free(val);
            free(diag);
            return false;
        }
    }
    row_ptr[n] = w;

    ic->L.n = n;
    ic->L.nnz = w;
    ic->L.row_ptr = row_ptr;
    ic->L.col = col;
    ic->L.val = val;
    ic->diag = diag;

    for (int i = 0; i < n; ++i) {
        const int i_end = row_ptr[i + 1];
        for (int p = row_ptr[i]; p < i_end; ++p) {
            const int j = col[p];
            vm_float_t s = val[p];
            int pi = row_ptr[i];
            int pj = row_ptr[j];
            const int i_lim = p;
            const int j_lim = diag[j];
            while (pi < i_lim && pj < j_lim) {
                const int ci = col[pi];
                const int cj = col[pj];
                if (ci == cj) {
                    s -= val[pi] * val[pj];
                    ++pi;
                    ++pj;
                } else if (ci < cj) {
                    ++pi;
                } else {
                    ++pj;
                }
            }
            if (j < i) {
                const vm_float_t ljj = val[diag[j]];
                if (VECMAT_FABS(ljj) <= VECMAT_EPSILON) {
                    vm_ic0_free(ic);
                    return false;
                }
                val[p] = s / ljj;
            } else {
                if (s <= VECMAT_EPSILON) {
                    vm_ic0_free(ic);
                    return false;
                }
                val[p] = VECMAT_SQRT(s);
            }
        }
    }

    ic->ok = true;
    return true;
}

/**
 * @brief Apply IC(0): solve L Lᵀ z = r.
 *
 * @param z Output vector.
 * @param ic IC(0) factor.
 * @param r Residual.
 * @param tmp Workspace (length n).
 */
static void vm_ic0_apply(vm_float_t *z, const vm_ic0 *ic, const vm_float_t *r, vm_float_t *tmp)
{
    const vm_spmat *L = &ic->L;
    const int n = L->n;

    for (int i = 0; i < n; ++i) {
        vm_float_t s = r[i];
        const int end = L->row_ptr[i + 1];
        for (int k = L->row_ptr[i]; k < end; ++k) {
            const int j = L->col[k];
            if (j < i) {
                s -= L->val[k] * tmp[j];
            }
        }
        tmp[i] = s / L->val[ic->diag[i]];
    }

    /* Lᵀ z = tmp, scatter off-diagonals of each row of L. */
    vm_vec_copy(z, tmp, n);
    for (int i = n - 1; i >= 0; --i) {
        z[i] /= L->val[ic->diag[i]];
        const int end = L->row_ptr[i + 1];
        for (int k = L->row_ptr[i]; k < end; ++k) {
            const int j = L->col[k];
            if (j < i) {
                z[j] -= L->val[k] * z[i];
            }
        }
    }
}

/**
 * @brief Left preconditioner for iterative solvers (CG / BiCGSTAB).
 */
typedef struct {
    vm_ksp_prec_t kind;  /**< NONE, JACOBI, SSOR, or IC0 */
    const vm_spmat *A;   /**< matrix being preconditioned (not owned) */
    vm_float_t *diag;    /**< diagonal of A (Jacobi / SSOR / IC0 fallback) */
    vm_float_t *tmp;     /**< workspace for SSOR / IC0 (length A->n) */
    vm_ic0 ic;           /**< IC(0) factor when kind is IC0 */
} vm_prec;

/**
 * @brief Free preconditioner data.
 *
 * @param p Preconditioner (may be NULL).
 */
static void vm_prec_free(vm_prec *p)
{
    if (!p) {
        return;
    }
    free(p->diag);
    free(p->tmp);
    vm_ic0_free(&p->ic);
    p->diag = NULL;
    p->tmp = NULL;
}

/**
 * @brief Set up a left preconditioner for A.
 *
 * IC(0) breakdown falls back to Jacobi.
 *
 * @param p Output preconditioner.
 * @param A CSR matrix.
 * @param kind NONE, JACOBI, SSOR, or IC0.
 * @return true on success, false on allocation/setup failure.
 */
static bool vm_prec_setup(vm_prec *p, const vm_spmat *A, const vm_ksp_prec_t kind)
{
    memset(p, 0, sizeof(*p));
    p->kind = VM_KSP_PREC_NONE;
    p->A = A;
    vm_spmat_init(&p->ic.L);
    if (kind == VM_KSP_PREC_NONE) {
        return true;
    }

    const int n = A->n;
    p->diag = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    if (!p->diag || !vm_spmat_diag(A, p->diag)) {
        vm_prec_free(p);
        return false;
    }

    if (kind == VM_KSP_PREC_JACOBI) {
        p->kind = VM_KSP_PREC_JACOBI;
        return true;
    }

    p->tmp = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    if (!p->tmp) {
        vm_prec_free(p);
        return false;
    }

    if (kind == VM_KSP_PREC_SSOR) {
        p->kind = VM_KSP_PREC_SSOR;
        return true;
    }

    if (kind == VM_KSP_PREC_IC0 && vm_ic0_factor(&p->ic, A)) {
        p->kind = VM_KSP_PREC_IC0;
        return true;
    }

    /* IC(0) breakdown → Jacobi */
    p->kind = VM_KSP_PREC_JACOBI;
    return true;
}

/**
 * @brief Apply preconditioner: z ≈ M⁻¹ r.
 *
 * @param z Output vector.
 * @param p Preconditioner.
 * @param r Residual.
 */
static void vm_prec_apply(vm_float_t *z, const vm_prec *p, const vm_float_t *r)
{
    const int n = p->A->n;
    switch (p->kind) {
    case VM_KSP_PREC_JACOBI:
        vm_jacobi_apply(z, p->diag, r, n);
        break;
    case VM_KSP_PREC_SSOR:
        vm_ssor_apply(z, p->A, p->diag, r, p->tmp);
        break;
    case VM_KSP_PREC_IC0:
        vm_ic0_apply(z, &p->ic, r, p->tmp);
        break;
    default:
        vm_vec_copy(z, r, n);
        break;
    }
}

/**
 * @brief Fill KSP result info.
 *
 * @param info Output info (may be NULL).
 * @param iters Iteration count.
 * @param relres Relative residual.
 * @param ok Convergence flag.
 */
static void vm_ksp_set(vm_ksp_info *info, const int iters, const vm_float_t relres, const bool ok)
{
    if (!info) {
        return;
    }
    info->iters = iters;
    info->rel_res = relres;
    info->ok = ok;
}

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
 */
bool vm_cg(const vm_spmat *A, const vm_float_t *b, vm_float_t *x,
           vm_float_t tol, int max_iter, const vm_ksp_prec_t pre_cond, vm_ksp_info *info)
{
    vm_ksp_set(info, 0, VM_F(0.0), false);
    if (!A || !A->row_ptr || !b || !x || A->n <= 0) {
        return false;
    }

    const int n = A->n;
    if (max_iter <= 0) {
        max_iter = n;
    }

    if (tol <= VM_F(0.0)) {
        tol = VECMAT_EPSILON * (vm_float_t)n;
    }

    vm_float_t *r = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *z = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *p = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *ap = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));

    if (!r || !z || !p || !ap) {
        free(r);
        free(z);
        free(p);
        free(ap);
        return false;
    }

    vm_prec P;
    if (!vm_prec_setup(&P, A, pre_cond)) {
        free(r);
        free(z);
        free(p);
        free(ap);
        return false;
    }

    vm_spmv(ap, A, x);
    for (int i = 0; i < n; ++i) {
        r[i] = b[i] - ap[i];
    }

    const vm_float_t rhs_norm = vm_vec_norm2(b, n);
    const vm_float_t scale = (rhs_norm > VECMAT_EPSILON) ? rhs_norm : VM_F(1.0);
    vm_float_t res_norm = vm_vec_norm2(r, n);

    if (res_norm / scale <= tol) {
        vm_ksp_set(info, 0, res_norm / scale, true);
        vm_prec_free(&P);
        free(r);
        free(z);
        free(p);
        free(ap);
        return true;
    }

    vm_prec_apply(z, &P, r);
    vm_vec_copy(p, z, n);
    vm_float_t rz = vm_vec_dot(r, z, n);

    int it = 0;
    bool ok = false;
    for (it = 1; it <= max_iter; ++it) {
        vm_spmv(ap, A, p);
        const vm_float_t pap = vm_vec_dot(p, ap, n);
        const vm_float_t pnorm2 = vm_vec_dot(p, p, n);
        if (VECMAT_FABS(pap) <= VECMAT_EPSILON * (pnorm2 + VECMAT_EPSILON)) {
            break;
        }
        const vm_float_t alpha = rz / pap;
        vm_vec_axpy(x, alpha, p, n);
        vm_vec_axpy(r, -alpha, ap, n);
        res_norm = vm_vec_norm2(r, n);
        if (res_norm / scale <= tol) {
            ok = true;
            break;
        }
        vm_prec_apply(z, &P, r);
        const vm_float_t rz_new = vm_vec_dot(r, z, n);
        if (VECMAT_FABS(rz) <= VECMAT_EPSILON * (VECMAT_FABS(rz_new) + VECMAT_EPSILON)) {
            break;
        }
        const vm_float_t beta = rz_new / rz;
        for (int i = 0; i < n; ++i) {
            p[i] = z[i] + beta * p[i];
        }
        rz = rz_new;
    }

    vm_ksp_set(info, it, res_norm / scale, ok);
    vm_prec_free(&P);
    free(r);
    free(z);
    free(p);
    free(ap);
    return ok;
}

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
 */
bool vm_bicgstab(const vm_spmat *A, const vm_float_t *b, vm_float_t *x,
                 vm_float_t tol, int max_iter, const vm_ksp_prec_t pre_cond, vm_ksp_info *info)
{
    vm_ksp_set(info, 0, VM_F(0.0), false);
    if (!A || !A->row_ptr || !b || !x || A->n <= 0) {
        return false;
    }
    const int n = A->n;
    if (max_iter <= 0) {
        max_iter = 2 * n;
    }
    if (tol <= VM_F(0.0)) {
        tol = VECMAT_EPSILON * (vm_float_t)n;
    }

    vm_float_t *r  =  (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *r0 =  (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *p  =  (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *v  =  (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *s  =  (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *t  =  (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *y  =  (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *z  =  (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    if (!r || !r0 || !p || !v || !s || !t || !y || !z) {
        free(r);
        free(r0);
        free(p);
        free(v);
        free(s);
        free(t);
        free(y);
        free(z);
        return false;
    }

    vm_prec P;
    if (!vm_prec_setup(&P, A, pre_cond)) {
        free(r);
        free(r0);
        free(p);
        free(v);
        free(s);
        free(t);
        free(y);
        free(z);
        return false;
    }

    vm_spmv(v, A, x);
    for (int i = 0; i < n; ++i) {
        r[i] = b[i] - v[i];
    }
    vm_vec_copy(r0, r, n);
    vm_vec_zero(p, n);
    vm_vec_zero(v, n);

    const vm_float_t rhs_norm = vm_vec_norm2(b, n);
    const vm_float_t scale = (rhs_norm > VECMAT_EPSILON) ? rhs_norm : VM_F(1.0);
    vm_float_t res_norm = vm_vec_norm2(r, n);
    if (res_norm / scale <= tol) {
        vm_ksp_set(info, 0, res_norm / scale, true);
        vm_prec_free(&P);
        free(r);
        free(r0);
        free(p);
        free(v);
        free(s);
        free(t);
        free(y);
        free(z);
        return true;
    }

    vm_float_t rho = VM_F(1.0);
    vm_float_t alpha = VM_F(1.0);
    vm_float_t omega = VM_F(1.0);
    int it = 0;
    bool ok = false;

    for (it = 1; it <= max_iter; ++it) {
        const vm_float_t rho_new = vm_vec_dot(r0, r, n);
        if (VECMAT_FABS(rho_new) <= VECMAT_EPSILON) {
            break;
        }
        const vm_float_t beta = (rho_new / rho) * (alpha / omega);
        for (int i = 0; i < n; ++i) {
            p[i] = r[i] + beta * (p[i] - omega * v[i]);
        }
        vm_prec_apply(y, &P, p);
        vm_spmv(v, A, y);
        const vm_float_t r0v = vm_vec_dot(r0, v, n);
        if (VECMAT_FABS(r0v) <= VECMAT_EPSILON) {
            break;
        }
        alpha = rho_new / r0v;
        for (int i = 0; i < n; ++i) {
            s[i] = r[i] - alpha * v[i];
        }
        if (vm_vec_norm2(s, n) / scale <= tol) {
            vm_vec_axpy(x, alpha, y, n);
            res_norm = vm_vec_norm2(s, n);
            ok = true;
            break;
        }
        vm_prec_apply(z, &P, s);
        vm_spmv(t, A, z);
        const vm_float_t tt = vm_vec_dot(t, t, n);
        if (VECMAT_FABS(tt) <= VECMAT_EPSILON) {
            break;
        }
        omega = vm_vec_dot(t, s, n) / tt;
        for (int i = 0; i < n; ++i) {
            x[i] += alpha * y[i] + omega * z[i];
            r[i] = s[i] - omega * t[i];
        }
        res_norm = vm_vec_norm2(r, n);
        if (res_norm / scale <= tol) {
            ok = true;
            break;
        }
        if (VECMAT_FABS(omega) <= VECMAT_EPSILON) {
            break;
        }
        rho = rho_new;
    }

    vm_ksp_set(info, it, res_norm / scale, ok);
    vm_prec_free(&P);
    free(r);
    free(r0);
    free(p);
    free(v);
    free(s);
    free(t);
    free(y);
    free(z);
    return ok;
}

/**
 * @brief Pointer to entry A(r, c) in column-major dense storage.
 *
 * @param m Dense matrix.
 * @param r Row index.
 * @param c Column index.
 * @return Pointer to the entry.
 */
static vm_float_t *vm_mat_at(const vm_mat *m, const int r, const int c)
{
    return &m->data[r + c * m->rows];
}

/**
 * @brief In-place dense Cholesky A = L Lᵀ (lower triangle overwritten).
 *
 * `A` must be square SPD. The strict upper triangle is left untouched.
 *
 * @param A Square dense matrix (destroyed / factored in place).
 * @return true on success, false if not SPD or invalid input.
 */
bool vm_chol_factor(vm_mat *A)
{
    if (!A || !A->data || A->rows <= 0 || A->rows != A->cols) {
        return false;
    }
    const int n = A->rows;
    for (int j = 0; j < n; ++j) {
        vm_float_t d = *vm_mat_at(A, j, j);
        for (int k = 0; k < j; ++k) {
            const vm_float_t ljk = *vm_mat_at(A, j, k);
            d -= ljk * ljk;
        }
        if (d <= VECMAT_EPSILON) {
            return false;
        }
        d = VECMAT_SQRT(d);
        *vm_mat_at(A, j, j) = d;
        for (int i = j + 1; i < n; ++i) {
            vm_float_t s = *vm_mat_at(A, i, j);
            for (int k = 0; k < j; ++k) {
                s -= *vm_mat_at(A, i, k) * *vm_mat_at(A, j, k);
            }
            *vm_mat_at(A, i, j) = s / d;
        }
    }
    return true;
}

/**
 * @brief Solve L Lᵀ x = b after `vm_chol_factor`.
 *
 * @param L Factored lower triangle from `vm_chol_factor`.
 * @param b Right-hand side (length n).
 * @param x Solution (length n).
 * @return true on success, false on error.
 */
bool vm_chol_solve(const vm_mat *L, const vm_float_t *b, vm_float_t *x)
{
    if (!L || !L->data || !b || !x || L->rows <= 0 || L->rows != L->cols) {
        return false;
    }
    const int n = L->rows;
    for (int i = 0; i < n; ++i) {
        vm_float_t s = b[i];
        for (int k = 0; k < i; ++k) {
            s -= *vm_mat_at(L, i, k) * x[k];
        }
        const vm_float_t d = *vm_mat_at(L, i, i);
        if (VECMAT_FABS(d) <= VECMAT_EPSILON) {
            return false;
        }
        x[i] = s / d;
    }
    for (int i = n - 1; i >= 0; --i) {
        vm_float_t s = x[i];
        for (int k = i + 1; k < n; ++k) {
            s -= *vm_mat_at(L, k, i) * x[k];
        }
        const vm_float_t d = *vm_mat_at(L, i, i);
        if (VECMAT_FABS(d) <= VECMAT_EPSILON) {
            return false;
        }
        x[i] = s / d;
    }
    return true;
}
