// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <stdlib.h>
#include <vecmat.h>
#include "unitest.h"

TEST_CASE(project_residual_constant_is_zero, "[project]") {
    const vm_grid3 g = vm_grid3_make(4, 3, 1, VM_F(1.0), VM_F(1.0), VM_F(1.0));
    const int n = vm_grid_ncells(g);
    vm_float_t *u = (vm_float_t *)malloc((size_t)vm_mac_nu(g) * sizeof(vm_float_t));
    vm_float_t *v = (vm_float_t *)malloc((size_t)vm_mac_nv(g) * sizeof(vm_float_t));
    vm_float_t *r = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    REQUIRE(u && v && r);
    for (int i = 0; i < vm_mac_nu(g); ++i) {
        u[i] = VM_F(3.0);
    }
    for (int i = 0; i < vm_mac_nv(g); ++i) {
        v[i] = VM_F(-2.0);
    }
    vm_project_residual(r, u, v, NULL, g);
    for (int i = 0; i < n; ++i) {
        REQUIRE(VECMAT_EQ(r[i], VM_F(0.0), EPSILON));
    }
    free(u);
    free(v);
    free(r);
}

TEST_CASE(project_constant_density_matches_laplacian, "[project]") {
    const vm_grid3 g = vm_grid3_make(5, 4, 1, VM_F(0.5), VM_F(0.5), VM_F(1.0));
    const int n = vm_grid_ncells(g);
    vm_spmat L;
    vm_spmat P;
    vm_spmat_init(&L);
    vm_spmat_init(&P);
    REQUIRE(vm_grid_laplacian(&L, g, VM_BC_DIRICHLET));
    REQUIRE(vm_project_laplacian(&P, NULL, g, VM_BC_DIRICHLET));

    vm_float_t *x = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *yl = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *yp = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    REQUIRE(x && yl && yp);
    for (int i = 0; i < n; ++i) {
        x[i] = VM_F(0.3) * (vm_float_t)(i + 1);
    }
    vm_spmv(yl, &L, x);
    vm_spmv(yp, &P, x);
    for (int i = 0; i < n; ++i) {
        REQUIRE(VECMAT_EQ(yp[i], yl[i], EPSILON * (VECMAT_FABS(yl[i]) + VM_F(1.0))));
    }
    free(x);
    free(yl);
    free(yp);
    vm_spmat_free(&L);
    vm_spmat_free(&P);
}

TEST_CASE(project_cg_manufactured_pressure, "[project]") {
    const vm_grid3 g = vm_grid3_make(6, 5, 1, VM_F(1.0), VM_F(1.0), VM_F(1.0));
    const int n = vm_grid_ncells(g);
    vm_float_t *rho = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *p = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *b = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *x = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    REQUIRE(rho && p && b && x);
    for (int j = 0; j < g.ny; ++j) {
        for (int i = 0; i < g.nx; ++i) {
            const int c = vm_grid_cell(g, i, j, 0);
            rho[c] = VM_F(1.0) + VM_F(0.2) * (vm_float_t)i;
            const bool boundary = (i == 0 || i == g.nx - 1 || j == 0 || j == g.ny - 1);
            p[c] = boundary ? VM_F(0.0) : VM_F(0.5) * (vm_float_t)(i * j);
            x[c] = VM_F(0.0);
        }
    }

    vm_spmat A;
    vm_spmat_init(&A);
    REQUIRE(vm_project_laplacian(&A, rho, g, VM_BC_DIRICHLET));
    vm_spmv(b, &A, p);

    vm_ksp_info info;
    REQUIRE(vm_cg(&A, b, x, VM_F(1e-8), 200, VM_KSP_PREC_JACOBI, &info));
    for (int i = 0; i < n; ++i) {
        REQUIRE(VECMAT_EQ(x[i], p[i], EPSILON * VM_F(64.0) + VM_F(1e-5)));
    }
    free(rho);
    free(p);
    free(b);
    free(x);
    vm_spmat_free(&A);
}
