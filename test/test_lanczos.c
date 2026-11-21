// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <math.h>
#include <vecmat.h>
#include "unitest.h"

static vm_float_t solver_tol(void)
{
#if VECMAT_FLOAT_BITS == 64
    return VM_F(1e-9);
#else
    return VM_F(1e-5);
#endif
}

static vm_float_t match_tol(void)
{
#if VECMAT_FLOAT_BITS == 64
    return VM_F(1e-8);
#else
    return VM_F(1e-4);
#endif
}

static void symv_ctx(vm_float_t *y, const vm_float_t *x, void *ctx)
{
    vm_symv(y, (const vm_mat *)ctx, x);
}

static void dummy_matvec(vm_float_t *y, const vm_float_t *x, void *ctx)
{
    (void)y;
    (void)x;
    (void)ctx;
}

static vm_float_t bad_tol(void)
{
#if VECMAT_FLOAT_BITS == 64
    return nan("");
#else
    return nanf("");
#endif
}

/**
 * @brief Lower triangle of a 5×5 symmetric matrix. Upper entries are garbage.
 */
static vm_mat sample_lower(void)
{
    const int n = 5;
    vm_mat A = vm_mat_alloc(n, n);
    for (int j = 0; j < n; ++j) {
        for (int i = j; i < n; ++i) {
            const vm_float_t v = (i == j)
                ? (vm_float_t)(i + 1)
                : VM_F(0.15) / (vm_float_t)(i - j + 1);
            vm_mat_set(&A, i, j, v);
        }
        for (int i = 0; i < j; ++i) {
            vm_mat_set(&A, i, j, VM_F(9.0));
        }
    }
    return A;
}

static vm_float_t abs_col_dot(const vm_mat *A, const int ca, const vm_mat *B, const int cb)
{
    vm_float_t d = VM_F(0.0);
    const int n = A->rows;
    for (int r = 0; r < n; ++r) {
        d += vm_mat_get(A, r, ca) * vm_mat_get(B, r, cb);
    }
    return VECMAT_FABS(d);
}

static void require_pairs(const vm_float_t *ref, const vm_mat *Vref,
                          const vm_float_t *got, const vm_mat *Vgot, const int k,
                          const int ref0)
{
    const vm_float_t tol = match_tol();
    for (int i = 0; i < k; ++i) {
        REQUIRE(VECMAT_EQ(got[i], ref[ref0 + i], tol));
        if (Vgot) {
            REQUIRE(abs_col_dot(Vref, ref0 + i, Vgot, i) > VM_F(1.0) - tol);
        }
    }
}

static void reference_eigs(const vm_mat *A, vm_float_t *ev, vm_mat *V)
{
    vm_mat Ac = { 0 };
    REQUIRE(vm_mat_copy(&Ac, A));
    REQUIRE(vm_syev(&Ac, ev, V));
    vm_mat_free(&Ac);
}

TEST_CASE(vm_lanczos_matches_syev, "[lanczos]") {
    vm_mat A = sample_lower();
    const int n = A.rows;
    const int k = 2;
    vm_float_t ref[8];
    vm_mat Vref = vm_mat_alloc(n, n);
    reference_eigs(&A, ref, &Vref);

    const vm_float_t stol = solver_tol();
    vm_float_t got[4];
    vm_mat Vg = vm_mat_alloc(n, k);
    vm_ksp_info info;

    REQUIRE(vm_lanczos(n, k, symv_ctx, &A, VM_LANCZOS_LARGEST, n, stol, 8, got, &Vg, &info));
    REQUIRE(info.ok);
    REQUIRE(info.rel_res <= stol);
    REQUIRE(info.iters >= 0);
    REQUIRE(info.iters <= 8);
    require_pairs(ref, &Vref, got, &Vg, k, 0);

    REQUIRE(vm_lanczos(n, k, symv_ctx, &A, VM_LANCZOS_SMALLEST, n, stol, 8, got, &Vg, &info));
    REQUIRE(info.ok);
    REQUIRE(info.rel_res <= stol);
    require_pairs(ref, &Vref, got, &Vg, k, n - k);

    /* Eigenvalues only, and a null info pointer. */
    REQUIRE(vm_lanczos(n, k, symv_ctx, &A, VM_LANCZOS_LARGEST, n, stol, 4, got, NULL, NULL));
    require_pairs(ref, &Vref, got, NULL, k, 0);

    vm_mat_free(&A);
    vm_mat_free(&Vref);
    vm_mat_free(&Vg);
}

TEST_CASE(vm_lanczos_restart_matches_syev, "[lanczos]") {
    vm_mat A = sample_lower();
    const int n = A.rows;
    const int k = 2;
    const int m = 3;
    vm_float_t ref[8];
    vm_mat Vref = vm_mat_alloc(n, n);
    reference_eigs(&A, ref, &Vref);

    const vm_float_t stol = solver_tol();
    vm_float_t got[4];
    vm_mat Vg = vm_mat_alloc(n, k);
    vm_ksp_info info;

    REQUIRE(vm_lanczos_dense(&A, k, VM_LANCZOS_LARGEST, m, stol, 30, got, &Vg, &info));
    REQUIRE(info.ok);
    REQUIRE(info.rel_res <= stol);
    REQUIRE(info.iters >= 1);
    require_pairs(ref, &Vref, got, &Vg, k, 0);

    REQUIRE(vm_lanczos_dense(&A, k, VM_LANCZOS_SMALLEST, m, stol, 30, got, &Vg, &info));
    REQUIRE(info.ok);
    REQUIRE(info.rel_res <= stol);
    REQUIRE(info.iters >= 1);
    require_pairs(ref, &Vref, got, &Vg, k, n - k);

    vm_mat_free(&A);
    vm_mat_free(&Vref);
    vm_mat_free(&Vg);
}

static vm_mat path_laplacian(const int n)
{
    vm_mat Adj = vm_mat_alloc(n, n);
    for (int i = 0; i < n - 1; ++i) {
        vm_mat_set(&Adj, i, i + 1, VM_F(1.0));
        vm_mat_set(&Adj, i + 1, i, VM_F(1.0));
    }
    vm_mat L = { 0 };
    REQUIRE(vm_laplacian(&L, &Adj));
    vm_mat_free(&Adj);
    return L;
}

static bool path_laplacian_csr(vm_spmat *S, const int n)
{
    const int cap = 3 * n;
    int *row = (int *)malloc((size_t)cap * sizeof(int));
    int *col = (int *)malloc((size_t)cap * sizeof(int));
    vm_float_t *val = (vm_float_t *)malloc((size_t)cap * sizeof(vm_float_t));
    if (!row || !col || !val) {
        free(row);
        free(col);
        free(val);
        return false;
    }
    int nnz = 0;
    for (int i = 0; i < n; ++i) {
        const int deg = (i > 0 ? 1 : 0) + (i + 1 < n ? 1 : 0);
        row[nnz] = i;
        col[nnz] = i;
        val[nnz] = (vm_float_t)deg;
        ++nnz;
        if (i > 0) {
            row[nnz] = i;
            col[nnz] = i - 1;
            val[nnz] = VM_F(-1.0);
            ++nnz;
        }
        if (i + 1 < n) {
            row[nnz] = i;
            col[nnz] = i + 1;
            val[nnz] = VM_F(-1.0);
            ++nnz;
        }
    }
    vm_spmat_init(S);
    const bool ok = vm_spmat_from_triplets(S, n, nnz, row, col, val);
    free(row);
    free(col);
    free(val);
    return ok;
}

TEST_CASE(vm_lanczos_path_laplacian, "[lanczos]") {
    const int n = 6;
    const int k = 2;
    vm_mat L = path_laplacian(n);
    vm_float_t ref[8];
    vm_mat Vref = vm_mat_alloc(n, n);
    reference_eigs(&L, ref, &Vref);

    const vm_float_t stol = solver_tol();
    vm_float_t got[4];
    vm_mat Vg = vm_mat_alloc(n, k);
    vm_ksp_info info;

    REQUIRE(vm_lanczos_dense(&L, k, VM_LANCZOS_LARGEST, n, stol, 6, got, &Vg, &info));
    REQUIRE(info.ok);
    require_pairs(ref, &Vref, got, &Vg, k, 0);

    REQUIRE(vm_lanczos_dense(&L, k, VM_LANCZOS_SMALLEST, n, stol, 6, got, &Vg, &info));
    REQUIRE(info.ok);
    require_pairs(ref, &Vref, got, &Vg, k, n - k);
    /* Path Laplacian is singular. The smallest eigenvalue is the constant mode. */
    REQUIRE(VECMAT_EQ(got[k - 1], VM_F(0.0), match_tol()));

    vm_spmat S;
    REQUIRE(path_laplacian_csr(&S, n));
    REQUIRE(vm_lanczos_sparse(&S, k, VM_LANCZOS_LARGEST, n, stol, 6, got, &Vg, &info));
    REQUIRE(info.ok);
    require_pairs(ref, &Vref, got, &Vg, k, 0);

    REQUIRE(vm_lanczos_sparse(&S, k, VM_LANCZOS_SMALLEST, 4, stol, 20, got, &Vg, &info));
    REQUIRE(info.ok);
    require_pairs(ref, &Vref, got, &Vg, k, n - k);

    vm_spmat_free(&S);
    vm_mat_free(&L);
    vm_mat_free(&Vref);
    vm_mat_free(&Vg);
}

TEST_CASE(vm_lanczos_rejects_bad_arguments, "[lanczos]") {
    vm_float_t ev[4];
    ev[0] = VM_F(1.0);
    vm_ksp_info info;
    info.ok = true;
    info.iters = 3;
    info.rel_res = VM_F(1.0);

    REQUIRE(!vm_lanczos(4, 0, dummy_matvec, NULL, VM_LANCZOS_LARGEST, 3, VM_F(1e-4), 2, ev, NULL, &info));
    REQUIRE(!info.ok);
    REQUIRE(!vm_lanczos(4, 3, dummy_matvec, NULL, VM_LANCZOS_LARGEST, 2, VM_F(1e-4), 2, ev, NULL, &info));
    REQUIRE(!vm_lanczos(4, 2, dummy_matvec, NULL, VM_LANCZOS_LARGEST, 5, VM_F(1e-4), 2, ev, NULL, &info));
    REQUIRE(!vm_lanczos(4, 2, dummy_matvec, NULL, VM_LANCZOS_LARGEST, 3, VM_F(0.0), 2, ev, NULL, &info));
    REQUIRE(!vm_lanczos(4, 2, dummy_matvec, NULL, VM_LANCZOS_LARGEST, 3, VM_F(-1.0), 2, ev, NULL, &info));
    REQUIRE(!vm_lanczos(4, 2, dummy_matvec, NULL, VM_LANCZOS_LARGEST, 3, bad_tol(), 2, ev, NULL, &info));
    REQUIRE(!vm_lanczos(4, 1, NULL, NULL, VM_LANCZOS_LARGEST, 2, VM_F(1e-4), 2, ev, NULL, &info));
    REQUIRE(!vm_lanczos(4, 1, dummy_matvec, NULL, VM_LANCZOS_LARGEST, 2, VM_F(1e-4), 2, NULL, NULL, &info));
    REQUIRE(!vm_lanczos(4, 1, dummy_matvec, NULL, VM_LANCZOS_LARGEST, 2, VM_F(1e-4), -1, ev, NULL, &info));

    vm_mat bad = vm_mat_alloc(3, 2);
    REQUIRE(!vm_lanczos_dense(&bad, 1, VM_LANCZOS_LARGEST, 1, VM_F(1e-4), 1, ev, NULL, &info));
    REQUIRE(!vm_lanczos_dense(NULL, 1, VM_LANCZOS_LARGEST, 1, VM_F(1e-4), 1, ev, NULL, &info));
    REQUIRE(!vm_lanczos_sparse(NULL, 1, VM_LANCZOS_LARGEST, 1, VM_F(1e-4), 1, ev, NULL, &info));

    vm_mat A = sample_lower();
    vm_mat wrong = vm_mat_alloc(A.rows, 1);
    REQUIRE(!vm_lanczos_dense(&A, 2, VM_LANCZOS_LARGEST, A.rows, VM_F(1e-4), 2, ev, &wrong, &info));
    REQUIRE(!info.ok);

    vm_mat_free(&bad);
    vm_mat_free(&A);
    vm_mat_free(&wrong);
}
