// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <stdlib.h>
#include <vecmat.h>
#include "unitest.h"

TEST_CASE(raddiff_limiter_zero_is_one_third, "[raddiff]") {
    const vm_float_t lam = vm_raddiff_limiter(VM_F(0.0));
    REQUIRE(VECMAT_EQ(lam, VM_F(1.0) / VM_F(3.0), EPSILON));
}

TEST_CASE(raddiff_limiter_bounded, "[raddiff]") {
    const vm_float_t samples[] = { VM_F(0.0), VM_F(0.1), VM_F(1.0), VM_F(5.0), VM_F(50.0) };
    for (int i = 0; i < 5; ++i) {
        const vm_float_t lam = vm_raddiff_limiter(samples[i]);
        REQUIRE(lam > VM_F(0.0));
        REQUIRE(lam <= VM_F(1.0) / VM_F(3.0) + EPSILON);
    }
}

TEST_CASE(raddiff_d_and_residual, "[raddiff]") {
    const vm_float_t kappa[] = { VM_F(2.0), VM_F(4.0) };
    vm_float_t D[2];
    REQUIRE(vm_raddiff_d(D, kappa, 2));
    REQUIRE(VECMAT_EQ(D[0], VM_F(1.0) / VM_F(6.0), EPSILON));
    REQUIRE(VECMAT_EQ(D[1], VM_F(1.0) / VM_F(12.0), EPSILON));
    REQUIRE(!vm_raddiff_d(D, (vm_float_t[]){ VM_F(0.0) }, 1));

    const vm_float_t ka[] = { VM_F(1.5) };
    const vm_float_t E[] = { VM_F(10.0) };
    const vm_float_t aT4[] = { VM_F(4.0) };
    vm_float_t r[1];
    REQUIRE(vm_raddiff_residual(r, ka, E, aT4, 1));
    REQUIRE(VECMAT_EQ(r[0], VM_F(1.5) * VM_F(6.0), EPSILON));
}

TEST_CASE(raddiff_constant_d_matches_laplacian, "[raddiff]") {
    const vm_grid3 g = vm_grid3_make(5, 4, 1, VM_F(0.5), VM_F(0.5), VM_F(1.0));
    const int n = vm_grid_ncells(g);
    vm_spmat L;
    vm_spmat R;
    vm_spmat_init(&L);
    vm_spmat_init(&R);
    REQUIRE(vm_grid_laplacian(&L, g, VM_BC_DIRICHLET));
    REQUIRE(vm_raddiff_laplacian(&R, NULL, g, VM_BC_DIRICHLET));

    vm_float_t *x = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *yl = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *yr = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    REQUIRE(x && yl && yr);
    for (int i = 0; i < n; ++i) {
        x[i] = VM_F(0.3) * (vm_float_t)(i + 1);
    }
    vm_spmv(yl, &L, x);
    vm_spmv(yr, &R, x);
    for (int i = 0; i < n; ++i) {
        REQUIRE(VECMAT_EQ(yr[i], yl[i], EPSILON * (VECMAT_FABS(yl[i]) + VM_F(1.0))));
    }
    free(x);
    free(yl);
    free(yr);
    vm_spmat_free(&L);
    vm_spmat_free(&R);
}

TEST_CASE(raddiff_cg_manufactured, "[raddiff]") {
    const vm_grid3 g = vm_grid3_make(6, 5, 1, VM_F(1.0), VM_F(1.0), VM_F(1.0));
    const int n = vm_grid_ncells(g);
    vm_float_t *D = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *E = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *b = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *x = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    REQUIRE(D && E && b && x);
    for (int j = 0; j < g.ny; ++j) {
        for (int i = 0; i < g.nx; ++i) {
            const int c = vm_grid_cell(g, i, j, 0);
            D[c] = VM_F(1.0) + VM_F(0.1) * (vm_float_t)i;
            const bool boundary = (i == 0 || i == g.nx - 1 || j == 0 || j == g.ny - 1);
            E[c] = boundary ? VM_F(0.0) : VM_F(0.25) * (vm_float_t)(i + j);
            x[c] = VM_F(0.0);
        }
    }

    vm_spmat A;
    vm_spmat_init(&A);
    REQUIRE(vm_raddiff_laplacian(&A, D, g, VM_BC_DIRICHLET));
    vm_spmv(b, &A, E);

    vm_ksp_info info;
    REQUIRE(vm_cg(&A, b, x, VM_F(1e-5), 200, VM_KSP_PREC_JACOBI, &info));
    for (int i = 0; i < n; ++i) {
        REQUIRE(VECMAT_EQ(x[i], E[i], EPSILON * VM_F(64.0) + VM_F(1e-5)));
    }
    free(D);
    free(E);
    free(b);
    free(x);
    vm_spmat_free(&A);
}
