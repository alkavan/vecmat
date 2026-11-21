// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <stdlib.h>
#include <string.h>
#include <vecmat.h>

#if VECMAT_FLOAT_BITS == 64
static const vm_float_t lanczos_macheps = VM_F(2.2204460492503131e-16);
#else
static const vm_float_t lanczos_macheps = VM_F(1.1920928955078125e-7);
#endif

/**
 * @brief Lanczos workspace. `V` holds `m + 1` columns of length `n`.
 *
 * After `basis` steps, columns `0 .. basis-1` are the Lanczos vectors,
 * `alpha[0 .. basis)` and `beta[0 .. basis-2]` are the tridiagonal, and
 * `beta[basis-1]` with column `basis` is the residual term.
 */
typedef struct {
    int n;
    int m;
    int basis;
    int attempt;
    int stopped; /* No direction left outside the current basis. */
    vm_float_t anorm;
    vm_float_t *V;
    vm_float_t *alpha;
    vm_float_t *beta;
    vm_float_t *work;
    vm_float_t *work2;
} lanczos_ws;

static void info_set(vm_ksp_info *info, const int iters, const vm_float_t rel_res, const bool ok)
{
    if (!info) {
        return;
    }
    info->iters = iters;
    info->rel_res = rel_res;
    info->ok = ok;
}

static vm_float_t lanczos_dot(const vm_float_t *a, const vm_float_t *b, const int n)
{
    vm_float_t s = VM_F(0.0);
    for (int i = 0; i < n; ++i) {
        s += a[i] * b[i];
    }
    return s;
}

static vm_float_t *lanczos_col(const lanczos_ws *ws, const int j)
{
    return ws->V + (size_t)j * (size_t)ws->n;
}

static vm_float_t tridiag_anorm(const vm_float_t *alpha, const vm_float_t *beta, const int basis)
{
    vm_float_t anorm = VM_F(0.0);
    for (int j = 0; j < basis; ++j) {
        vm_float_t row = VECMAT_FABS(alpha[j]);
        if (j > 0) {
            row += VECMAT_FABS(beta[j - 1]);
        }

        if (j + 1 < basis) {
            row += VECMAT_FABS(beta[j]);
        }

        if (row > anorm) {
            anorm = row;
        }
    }

    return anorm;
}

static vm_float_t lanczos_break_tol(const vm_float_t anorm, const int n)
{
    vm_float_t scale = anorm;
    if (!(scale > VM_F(1.0))) {
        scale = VM_F(1.0);
    }
    return VM_F(10.0) * lanczos_macheps * VECMAT_SQRT((vm_float_t)n) * scale;
}

static bool vec_finite(const vm_float_t *x, const int n)
{
    for (int i = 0; i < n; ++i) {
        if (!isfinite(x[i])) {
            return false;
        }
    }
    return true;
}

static void ws_free(lanczos_ws *ws)
{
    if (!ws) {
        return;
    }
    free(ws->V);
    free(ws->alpha);
    free(ws->beta);
    free(ws->work);
    free(ws->work2);
    memset(ws, 0, sizeof(*ws));
}

static bool ws_alloc(lanczos_ws *ws, const int n, const int m)
{
    memset(ws, 0, sizeof(*ws));

    ws->n = n;
    ws->m = m;
    ws->V = (vm_float_t *)calloc((size_t)(m + 1) * (size_t)n, sizeof(vm_float_t));
    ws->alpha = (vm_float_t *)calloc((size_t)m, sizeof(vm_float_t));
    ws->beta = (vm_float_t *)calloc((size_t)m, sizeof(vm_float_t));
    ws->work = (vm_float_t *)calloc((size_t)n, sizeof(vm_float_t));
    ws->work2 = (vm_float_t *)calloc((size_t)n, sizeof(vm_float_t));

    if (!ws->V || !ws->alpha || !ws->beta || !ws->work || !ws->work2) {
        ws_free(ws);
        return false;
    }

    return true;
}

/**
 * @brief Unit vector in column `col`, orthogonal to columns `0 .. nprev-1`.
 */
static bool seed_column(lanczos_ws *ws, const int col, const int nprev)
{
    const int n = ws->n;
    vm_float_t *v = lanczos_col(ws, col);
    const int attempt = ws->attempt++;

    if (attempt == 0) {
        for (int i = 0; i < n; ++i) {
            v[i] = (vm_float_t)(i + 1);
        }
    } else if (attempt <= n) {
        for (int i = 0; i < n; ++i) {
            v[i] = (i + 1 == attempt) ? VM_F(1.0) : VM_F(0.0);
        }
    } else {
        for (int i = 0; i < n; ++i) {
            v[i] = VECMAT_SIN((vm_float_t)(i + 1) * (vm_float_t)attempt);
        }
    }

    for (int pass = 0; pass < 2; ++pass) {
        for (int i = 0; i < nprev; ++i) {
            const vm_float_t *vi = lanczos_col(ws, i);
            const vm_float_t t = lanczos_dot(vi, v, n);
            vm_axpy(v, -t, vi, n);
        }
    }

    const vm_float_t b = vm_nrm2(v, n);
    if (!(b > lanczos_break_tol(ws->anorm, n))) {
        return false;
    }

    const vm_float_t inv = VM_F(1.0) / b;
    for (int i = 0; i < n; ++i) {
        v[i] *= inv;
    }

    return true;
}

static bool seed_any(lanczos_ws *ws, const int col, const int n_prev)
{
    const int tries = n_prev + 2;
    for (int t = 0; t < tries; ++t) {
        if (seed_column(ws, col, n_prev)) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Extend the factorization until `target` steps or an invariant subspace.
 *
 * Returns false on a non-finite matvec. A short return with `basis < target`
 * means the Krylov space stopped early and `beta[basis - 1] == 0`.
 */
static bool extend_to(lanczos_ws *ws,
                      void (*matvec)(vm_float_t *y, const vm_float_t *x, void *ctx),
                      void *ctx, const int target)
{
    const int n = ws->n;
    if (ws->stopped) {
        return true;
    }
    if (ws->basis == 0) {
        if (!seed_column(ws, 0, 0)) {
            return false;
        }
    }

    int guard = 0;
    while (ws->basis < target) {
        if (guard++ > target + n + 2) {
            return false;
        }
        const int j = ws->basis;
        const vm_float_t *v = lanczos_col(ws, j);
        matvec(ws->work, v, ctx);
        if (!vec_finite(ws->work, n)) {
            return false;
        }
        if (j > 0) {
            vm_axpy(ws->work, -ws->beta[j - 1], lanczos_col(ws, j - 1), n);
        }

        vm_float_t alpha = lanczos_dot(v, ws->work, n);
        vm_axpy(ws->work, -alpha, v, n);
        for (int pass = 0; pass < 2; ++pass) {
            for (int i = 0; i <= j; ++i) {
                const vm_float_t *vi = lanczos_col(ws, i);
                const vm_float_t t = lanczos_dot(vi, ws->work, n);
                vm_axpy(ws->work, -t, vi, n);
                if (i == j) {
                    alpha += t;
                } else if (i == j - 1) {
                    ws->beta[j - 1] += t;
                }
            }
        }
        if (!isfinite(alpha)) {
            return false;
        }
        ws->alpha[j] = alpha;
        ws->anorm = tridiag_anorm(ws->alpha, ws->beta, j + 1);

        const vm_float_t b = vm_nrm2(ws->work, n);
        if (!(b > lanczos_break_tol(ws->anorm, n))) {
            ws->beta[j] = VM_F(0.0);
            ws->basis = j + 1;
            if (!seed_any(ws, j + 1, j + 1)) {
                return true;
            }
            continue;
        }

        const vm_float_t inv = VM_F(1.0) / b;
        vm_float_t *vnext = lanczos_col(ws, j + 1);
        for (int i = 0; i < n; ++i) {
            vnext[i] = ws->work[i] * inv;
        }
        ws->beta[j] = b;
        ws->basis = j + 1;
    }
    return true;
}

static bool solve_tridiag(const vm_float_t *alpha, const vm_float_t *beta, const int basis,
                          vm_float_t *ritz, vm_mat *Y)
{
    vm_mat T = vm_mat_alloc(basis, basis);
    if (!T.data) {
        return false;
    }
    for (int j = 0; j < basis; ++j) {
        vm_mat_set(&T, j, j, alpha[j]);
        if (j + 1 < basis) {
            vm_mat_set(&T, j + 1, j, beta[j]);
        }
    }
    *Y = vm_mat_alloc(basis, basis);
    if (!Y->data) {
        vm_mat_free(&T);
        return false;
    }
    const bool ok = vm_syev(&T, ritz, Y);
    vm_mat_free(&T);
    if (!ok) {
        return false;
    }
    for (int i = 0; i < basis; ++i) {
        if (!isfinite(ritz[i])) {
            return false;
        }
    }
    return true;
}

static int wanted_column(const vm_lanczos_which which, const int basis, const int k, const int i)
{
    if (which == VM_LANCZOS_LARGEST) {
        return i;
    }
    return basis - k + i;
}

static vm_float_t residual_scale(const vm_float_t lambda, const vm_float_t anorm)
{
    vm_float_t scale = VECMAT_FMAX(VECMAT_FABS(lambda), anorm);
    if (!(scale > lanczos_macheps)) {
        scale = VM_F(1.0);
    }
    return scale;
}

static vm_float_t worst_estimate(const lanczos_ws *ws, const vm_mat *Y, const vm_float_t *ritz,
                                 const vm_lanczos_which which, const int k)
{
    vm_float_t worst = VM_F(0.0);
    const vm_float_t beta = ws->beta[ws->basis - 1];
    for (int i = 0; i < k; ++i) {
        const int col = wanted_column(which, ws->basis, k, i);
        const vm_float_t resid = VECMAT_FABS(beta * vm_mat_get(Y, ws->basis - 1, col));
        const vm_float_t rel = resid / residual_scale(ritz[col], ws->anorm);
        if (rel > worst) {
            worst = rel;
        }
    }
    return worst;
}

static void orient_positive(vm_float_t *u, const int n)
{
    int pivot = 0;
    vm_float_t mag = VM_F(0.0);
    for (int i = 0; i < n; ++i) {
        const vm_float_t a = VECMAT_FABS(u[i]);
        if (a > mag) {
            mag = a;
            pivot = i;
        }
    }
    if (u[pivot] < VM_F(0.0)) {
        for (int i = 0; i < n; ++i) {
            u[i] = -u[i];
        }
    }
}

/**
 * @brief Write the selected Ritz pairs and the worst true relative residual.
 */
static bool export_pairs(const lanczos_ws *ws, const vm_mat *Y, const vm_float_t *ritz,
                         const vm_lanczos_which which, const int k,
                         vm_float_t *eigenvalues, vm_mat *eigenvectors,
                         void (*matvec)(vm_float_t *y, const vm_float_t *x, void *ctx),
                         void *ctx, vm_float_t *rel_out)
{
    const int n = ws->n;
    const int basis = ws->basis;
    vm_float_t worst = VM_F(0.0);

    for (int i = 0; i < k; ++i) {
        const int col = wanted_column(which, basis, k, i);
        vm_float_t *u = ws->work;
        for (int r = 0; r < n; ++r) {
            u[r] = VM_F(0.0);
        }
        for (int j = 0; j < basis; ++j) {
            vm_axpy(u, vm_mat_get(Y, j, col), lanczos_col(ws, j), n);
        }
        const vm_float_t nrm = vm_nrm2(u, n);
        if (!(nrm > VM_F(0.0))) {
            return false;
        }
        const vm_float_t inv = VM_F(1.0) / nrm;
        for (int r = 0; r < n; ++r) {
            u[r] *= inv;
        }
        orient_positive(u, n);

        const vm_float_t lambda = ritz[col];
        eigenvalues[i] = lambda;
        if (eigenvectors) {
            for (int r = 0; r < n; ++r) {
                vm_mat_set(eigenvectors, r, i, u[r]);
            }
        }

        matvec(ws->work2, u, ctx);
        if (!vec_finite(ws->work2, n)) {
            return false;
        }
        for (int r = 0; r < n; ++r) {
            ws->work2[r] -= lambda * u[r];
        }
        const vm_float_t rel = vm_nrm2(ws->work2, n) / residual_scale(lambda, ws->anorm);
        if (rel > worst) {
            worst = rel;
        }
    }

    *rel_out = worst;
    return true;
}

static void fill_tridiag(vm_mat *T, const vm_float_t *alpha, const vm_float_t *beta, const int n)
{
    vm_mat_zero(T);
    for (int j = 0; j < n; ++j) {
        vm_mat_set(T, j, j, alpha[j]);
        if (j + 1 < n) {
            vm_mat_set(T, j + 1, j, beta[j]);
            vm_mat_set(T, j, j + 1, beta[j]);
        }
    }
}

/**
 * @brief One implicit QR shift: `T <- Qᵀ T Q`, and `Q_total <- Q_total Q`.
 *
 * `T` stays symmetric tridiagonal. Fill outside that band is roundoff and
 * is dropped, which is the bulge-chase update.
 */
static bool qr_shift(vm_mat *T, const vm_float_t sigma, vm_mat *Q_total)
{
    const int n = T->rows;
    vm_mat M = vm_mat_alloc(n, n);
    vm_mat Q = vm_mat_alloc(n, n);
    vm_mat Prod = vm_mat_alloc(n, n);
    vm_float_t *v = (vm_float_t *)calloc((size_t)n, sizeof(vm_float_t));
    vm_float_t *w = (vm_float_t *)calloc((size_t)n, sizeof(vm_float_t));
    vm_float_t *diag = (vm_float_t *)calloc((size_t)n, sizeof(vm_float_t));
    vm_float_t *off = (vm_float_t *)calloc((size_t)n, sizeof(vm_float_t));
    if (!M.data || !Q.data || !Prod.data || !v || !w || !diag || !off) {
        vm_mat_free(&M);
        vm_mat_free(&Q);
        vm_mat_free(&Prod);
        free(v);
        free(w);
        free(diag);
        free(off);
        return false;
    }

    for (int j = 0; j < n; ++j) {
        for (int i = 0; i < n; ++i) {
            vm_float_t tij = vm_mat_get(T, i, j);
            if (i == j) {
                tij -= sigma;
            }
            vm_mat_set(&M, i, j, tij);
            vm_mat_set(&Q, i, j, (i == j) ? VM_F(1.0) : VM_F(0.0));
        }
    }

    for (int k = 0; k < n - 1; ++k) {
        vm_float_t nrm2 = VM_F(0.0);
        for (int i = k; i < n; ++i) {
            const vm_float_t x = vm_mat_get(&M, i, k);
            nrm2 += x * x;
        }

        const vm_float_t nrm = VECMAT_SQRT(nrm2);
        if (!(nrm > VM_F(0.0))) {
            continue;
        }

        const vm_float_t x0 = vm_mat_get(&M, k, k);
        const vm_float_t signed_nrm = (x0 >= VM_F(0.0)) ? -nrm : nrm;
        for (int i = k; i < n; ++i) {
            v[i] = vm_mat_get(&M, i, k);
        }
        v[k] = x0 - signed_nrm;

        vm_float_t vv = VM_F(0.0);
        for (int i = k; i < n; ++i) {
            vv += v[i] * v[i];
        }

        if (!(vv > VM_F(0.0))) {
            continue;
        }

        const vm_float_t tau = VM_F(2.0) / vv;

        /* Left apply on every column. Columns before k are already zero in these rows. */
        for (int j = 0; j < n; ++j) {
            vm_float_t dot = VM_F(0.0);
            for (int i = k; i < n; ++i) {
                dot += v[i] * vm_mat_get(&M, i, j);
            }

            const vm_float_t beta = tau * dot;
            for (int i = k; i < n; ++i) {
                vm_mat_set(&M, i, j, vm_mat_get(&M, i, j) - beta * v[i]);
            }
        }

        for (int i = 0; i < n; ++i) {
            vm_float_t dot = VM_F(0.0);
            for (int j = k; j < n; ++j) {
                dot += vm_mat_get(&Q, i, j) * v[j];
            }
            w[i] = dot;
        }

        for (int j = k; j < n; ++j) {
            for (int i = 0; i < n; ++i) {
                vm_mat_set(&Q, i, j, vm_mat_get(&Q, i, j) - tau * w[i] * v[j]);
            }
        }
    }

    for (int j = 0; j < n; ++j) {
        for (int i = j + 1; i < n; ++i) {
            vm_mat_set(&M, i, j, VM_F(0.0));
        }
    }

    for (int j = 0; j < n; ++j) {
        for (int i = 0; i < n; ++i) {
            vm_float_t s = (i == j) ? sigma : VM_F(0.0);
            for (int p = 0; p < n; ++p) {
                s += vm_mat_get(&M, i, p) * vm_mat_get(&Q, p, j);
            }
            vm_mat_set(&Prod, i, j, s);
        }
    }

    for (int j = 0; j < n; ++j) {
        for (int i = 0; i < n; ++i) {
            vm_float_t s = VM_F(0.0);
            for (int p = 0; p < n; ++p) {
                s += vm_mat_get(Q_total, i, p) * vm_mat_get(&Q, p, j);
            }
            vm_mat_set(&M, i, j, s);
        }
    }
    for (int j = 0; j < n; ++j) {
        for (int i = 0; i < n; ++i) {
            vm_mat_set(Q_total, i, j, vm_mat_get(&M, i, j));
        }
    }

    for (int i = 0; i < n; ++i) {
        diag[i] = vm_mat_get(&Prod, i, i);
        if (i + 1 < n) {
            off[i] = VM_F(0.5) * (vm_mat_get(&Prod, i + 1, i) + vm_mat_get(&Prod, i, i + 1));
        }
    }

    vm_mat_zero(T);
    for (int i = 0; i < n; ++i) {
        vm_mat_set(T, i, i, diag[i]);
        if (i + 1 < n) {
            vm_mat_set(T, i + 1, i, off[i]);
            vm_mat_set(T, i, i + 1, off[i]);
        }
    }

    vm_mat_free(&M);
    vm_mat_free(&Q);
    vm_mat_free(&Prod);
    free(v);
    free(w);
    free(diag);
    free(off);

    return true;
}

/**
 * @brief Purge `m - k` unwanted Ritz values and keep a k-step factorization.
 */
static bool implicit_restart(lanczos_ws *ws, const vm_lanczos_which which, const int k,
                             const vm_float_t *ritz)
{
    const int m = ws->basis;
    const int n = ws->n;
    const int p = m - k;
    if (p <= 0 || k < 1) {
        return false;
    }

    vm_mat T = vm_mat_alloc(m, m);
    vm_mat Q_total = vm_mat_alloc(m, m);
    vm_float_t *V_new = (vm_float_t *)malloc((size_t)n * (size_t)m * sizeof(vm_float_t));
    vm_float_t *v_next = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *f = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    if (!T.data || !Q_total.data || !V_new || !v_next || !f) {
        vm_mat_free(&T);
        vm_mat_free(&Q_total);
        free(V_new);
        free(v_next);
        free(f);
        return false;
    }

    fill_tridiag(&T, ws->alpha, ws->beta, m);
    for (int j = 0; j < m; ++j) {
        for (int i = 0; i < m; ++i) {
            vm_mat_set(&Q_total, i, j, (i == j) ? VM_F(1.0) : VM_F(0.0));
        }
    }

    const vm_float_t beta_m = ws->beta[m - 1];
    if (VECMAT_FABS(beta_m) > VM_F(0.0)) {
        memcpy(v_next, lanczos_col(ws, m), (size_t)n * sizeof(vm_float_t));
    } else {
        memset(v_next, 0, (size_t)n * sizeof(vm_float_t));
    }

    bool shifts_ok = true;
    for (int s = 0; s < p; ++s) {
        const int shift_index = (which == VM_LANCZOS_LARGEST) ? (k + s) : s;
        if (!qr_shift(&T, ritz[shift_index], &Q_total)) {
            shifts_ok = false;
            break;
        }
    }
    if (!shifts_ok) {
        vm_mat_free(&T);
        vm_mat_free(&Q_total);
        free(V_new);
        free(v_next);
        free(f);
        return false;
    }

    for (int col = 0; col < m; ++col) {
        for (int r = 0; r < n; ++r) {
            vm_float_t sum = VM_F(0.0);
            for (int i = 0; i < m; ++i) {
                sum += lanczos_col(ws, i)[r] * vm_mat_get(&Q_total, i, col);
            }
            V_new[(size_t)col * (size_t)n + (size_t)r] = sum;
        }
    }

    const vm_float_t sub = vm_mat_get(&T, k, k - 1);
    const vm_float_t q_last = vm_mat_get(&Q_total, m - 1, k - 1);
    const vm_float_t *purged = V_new + (size_t)k * (size_t)n;
    for (int r = 0; r < n; ++r) {
        f[r] = sub * purged[r] + (beta_m * q_last) * v_next[r];
    }

    for (int j = 0; j < k; ++j) {
        memcpy(lanczos_col(ws, j), V_new + (size_t)j * (size_t)n, (size_t)n * sizeof(vm_float_t));
        ws->alpha[j] = vm_mat_get(&T, j, j);
        if (j + 1 < k) {
            ws->beta[j] = vm_mat_get(&T, j + 1, j);
        }
    }

    for (int pass = 0; pass < 2; ++pass) {
        for (int j = 0; j < k; ++j) {
            const vm_float_t t = lanczos_dot(lanczos_col(ws, j), f, n);
            vm_axpy(f, -t, lanczos_col(ws, j), n);
        }
    }

    const vm_float_t b_new = vm_nrm2(f, n);
    ws->anorm = tridiag_anorm(ws->alpha, ws->beta, k);
    if (!(b_new > lanczos_break_tol(ws->anorm, n))) {
        ws->beta[k - 1] = VM_F(0.0);
        ws->basis = k;
        if (!seed_any(ws, k, k)) {
            ws->stopped = 1;
        }
    } else {
        const vm_float_t inv = VM_F(1.0) / b_new;
        vm_float_t *dest = lanczos_col(ws, k);
        for (int r = 0; r < n; ++r) {
            dest[r] = f[r] * inv;
        }
        ws->beta[k - 1] = b_new;
        ws->basis = k;
    }

    vm_mat_free(&T);
    vm_mat_free(&Q_total);
    free(V_new);
    free(v_next);
    free(f);

    return true;
}

static bool give_up(const lanczos_ws *ws, const vm_mat *Y, const vm_float_t *ritz,
                    const vm_lanczos_which which, const int k,
                    vm_float_t *eigenvalues, vm_mat *eigenvectors,
                    void (*matvec)(vm_float_t *y, const vm_float_t *x, void *ctx),
                    void *ctx, const vm_float_t tol, vm_float_t *rel, bool *ok)
{
    if (!export_pairs(ws, Y, ritz, which, k, eigenvalues, eigenvectors, matvec, ctx, rel)) {
        return false;
    }
    *ok = (*rel <= tol);
    return true;
}

bool vm_lanczos(const int n, const int k,
                void (*matvec)(vm_float_t *y, const vm_float_t *x, void *ctx),
                void *ctx, const vm_lanczos_which which,
                const int m, const vm_float_t tol, const int max_restarts,
                vm_float_t *eigenvalues, vm_mat *eigenvectors,
                vm_ksp_info *info)
{
    info_set(info, 0, VM_F(0.0), false);

    if (n < 1 || k < 1 || m < k || m > n || max_restarts < 0) {
        return false;
    }

    if (!(tol > VM_F(0.0))) {
        return false;
    }

    if (!matvec || !eigenvalues) {
        return false;
    }

    if (which != VM_LANCZOS_LARGEST && which != VM_LANCZOS_SMALLEST) {
        return false;
    }

    if (eigenvectors &&
        (eigenvectors->rows != n
            || eigenvectors->cols != k
            || !eigenvectors->data)) {
        return false;
    }

    lanczos_ws ws;
    if (!ws_alloc(&ws, n, m)) {
        return false;
    }

    vm_float_t *ritz = (vm_float_t *)malloc((size_t)m * sizeof(vm_float_t));
    vm_mat Y = { 0 };
    if (!ritz) {
        ws_free(&ws);
        return false;
    }

    bool ok = false;
    vm_float_t rel = VM_F(0.0);
    int iters = 0;

    if (extend_to(&ws, matvec, ctx, m)) {
        while (ws.basis >= k) {
            vm_mat_free(&Y);
            if (!solve_tridiag(ws.alpha, ws.beta, ws.basis, ritz, &Y)) {
                break;
            }
            ws.anorm = tridiag_anorm(ws.alpha, ws.beta, ws.basis);

            const vm_float_t est = worst_estimate(&ws, &Y, ritz, which, k);
            if (est <= tol) {
                if (!export_pairs(&ws, &Y, ritz, which, k, eigenvalues, eigenvectors, matvec, ctx, &rel)) {
                    break;
                }
                if (rel <= tol) {
                    ok = true;
                    break;
                }
            }

            if (iters >= max_restarts) {
                if (!give_up(&ws, &Y, ritz, which, k, eigenvalues, eigenvectors, matvec, ctx, tol, &rel, &ok)) {
                    break;
                }
                break;
            }

            if (ws.basis < m) {
                const int before = ws.basis;
                if (!extend_to(&ws, matvec, ctx, m)) {
                    break;
                }
                if (ws.basis == before) {
                    if (!give_up(&ws, &Y, ritz, which, k, eigenvalues, eigenvectors, matvec, ctx, tol, &rel, &ok)) {
                        break;
                    }
                    break;
                }
                continue;
            }

            if (ws.basis == k) {
                if (!give_up(&ws, &Y, ritz, which, k, eigenvalues, eigenvectors, matvec, ctx, tol, &rel, &ok)) {
                    break;
                }
                break;
            }

            if (!implicit_restart(&ws, which, k, ritz)) {
                break;
            }

            ++iters;
            if (!extend_to(&ws, matvec, ctx, m)) {
                break;
            }
        }
    }

    info_set(info, iters, rel, ok);
    vm_mat_free(&Y);
    free(ritz);
    ws_free(&ws);
    return ok;
}

typedef struct {
    const vm_mat *A;
} lanczos_dense_ctx;

static void lanczos_dense_matvec(vm_float_t *y, const vm_float_t *x, void *ctx)
{
    const lanczos_dense_ctx *c = (const lanczos_dense_ctx *)ctx;
    vm_symv(y, c->A, x);
}

bool vm_lanczos_dense(const vm_mat *A, const int k, const vm_lanczos_which which,
                      const int m, const vm_float_t tol, const int max_restarts,
                      vm_float_t *eigenvalues, vm_mat *eigenvectors,
                      vm_ksp_info *info)
{
    info_set(info, 0, VM_F(0.0), false);
    if (!A || !A->data || A->rows <= 0 || A->rows != A->cols) {
        return false;
    }
    lanczos_dense_ctx ctx;
    ctx.A = A;
    return vm_lanczos(A->rows, k, lanczos_dense_matvec, &ctx, which, m, tol, max_restarts,
                      eigenvalues, eigenvectors, info);
}

static void lanczos_sparse_matvec(vm_float_t *y, const vm_float_t *x, void *ctx)
{
    vm_spmv(y, (const vm_spmat *)ctx, x);
}

bool vm_lanczos_sparse(const vm_spmat *A, const int k, const vm_lanczos_which which,
                       const int m, const vm_float_t tol, const int max_restarts,
                       vm_float_t *eigenvalues, vm_mat *eigenvectors,
                       vm_ksp_info *info)
{
    info_set(info, 0, VM_F(0.0), false);
    if (!A || !A->row_ptr || A->n <= 0) {
        return false;
    }
    return vm_lanczos(A->n, k, lanczos_sparse_matvec, (void *)A, which, m, tol, max_restarts,
                      eigenvalues, eigenvectors, info);
}
