// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <stdlib.h>
#include <string.h>
#include <vecmat.h>
#include "unitest.h"

TEST_CASE(vm_spmat_from_triplets_sums_duplicates, "[sparse]") {
    const int row[5] = { 0, 0, 1, 1, 0 };
    const int col[5] = { 0, 1, 0, 1, 0 };
    const vm_float_t val[5] = { 2, 3, 4, 5, 1 };
    vm_spmat A;
    vm_spmat_init(&A);
    REQUIRE(vm_spmat_from_triplets(&A, 2, 5, row, col, val));
    REQUIRE(A.n == 2);
    REQUIRE(A.nnz == 4);
    vm_float_t y[2];
    const vm_float_t x[2] = { 1, 1 };
    vm_spmv(y, &A, x);
    REQUIRE(VECMAT_EQ(y[0], VM_F(6.0), EPSILON)); /* (2+1)+3 */
    REQUIRE(VECMAT_EQ(y[1], VM_F(9.0), EPSILON));
    vm_spmat_free(&A);
}

TEST_CASE(vm_cg_solves_spd_dense_as_csr, "[sparse]") {
    /* A = [[4,1],[1,3]], b = [1,2] → x = [1/11, 7/11] */
    const int row[4] = { 0, 0, 1, 1 };
    const int col[4] = { 0, 1, 0, 1 };
    const vm_float_t val[4] = { 4, 1, 1, 3 };
    vm_spmat A;
    vm_spmat_init(&A);
    REQUIRE(vm_spmat_from_triplets(&A, 2, 4, row, col, val));

    const vm_float_t b[2] = { 1, 2 };
    vm_float_t x[2] = { 0, 0 };
    vm_ksp_info info;
    REQUIRE(vm_cg(&A, b, x, VM_F(1e-8), 16, VM_KSP_PREC_NONE, &info));
    REQUIRE(info.ok);
    REQUIRE(VECMAT_EQ(x[0], VM_F(1.0) / VM_F(11.0), EPSILON * VM_F(10.0)));
    REQUIRE(VECMAT_EQ(x[1], VM_F(7.0) / VM_F(11.0), EPSILON * VM_F(10.0)));
    vm_spmat_free(&A);
}

TEST_CASE(vm_cg_jacobi_poisson_2d, "[sparse]") {
    const vm_grid3 g = vm_grid3_make(8, 8, 1, VM_F(1.0), VM_F(1.0), VM_F(1.0));
    vm_spmat A;
    vm_spmat_init(&A);
    REQUIRE(vm_grid_laplacian(&A, g, VM_BC_DIRICHLET));
    const int n = vm_grid_ncells(g);
    vm_float_t *b = (vm_float_t *)calloc((size_t)n, sizeof(vm_float_t));
    vm_float_t *x = (vm_float_t *)calloc((size_t)n, sizeof(vm_float_t));
    REQUIRE(b && x);
    for (int i = 0; i < n; ++i) {
        b[i] = VM_F(1.0);
    }
    /* Dirichlet rows are identity: set rhs to 0 on the boundary. */
    for (int j = 0; j < 8; ++j) {
        for (int i = 0; i < 8; ++i) {
            if (i == 0 || i == 7 || j == 0 || j == 7) {
                b[vm_grid_cell(g, i, j, 0)] = VM_F(0.0);
            }
        }
    }
    vm_ksp_info info;
    REQUIRE(vm_cg(&A, b, x, VM_F(1e-5), 200, VM_KSP_PREC_JACOBI, &info));
    REQUIRE(info.ok);
    vm_float_t *ax = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    REQUIRE(ax);
    vm_spmv(ax, &A, x);
    vm_float_t max_err = VM_F(0.0);
    for (int i = 0; i < n; ++i) {
        const vm_float_t e = VECMAT_FABS(ax[i] - b[i]);
        if (e > max_err) {
            max_err = e;
        }
    }
    REQUIRE(max_err < VM_F(1e-3));
    free(ax);
    free(b);
    free(x);
    vm_spmat_free(&A);
}

TEST_CASE(vm_cg_ic0_beats_unpreconditioned_iters, "[sparse]") {
    const vm_grid3 g = vm_grid3_make(10, 10, 1, VM_F(1.0), VM_F(1.0), VM_F(1.0));
    vm_spmat A;
    vm_spmat_init(&A);
    REQUIRE(vm_grid_laplacian(&A, g, VM_BC_DIRICHLET));
    const int n = vm_grid_ncells(g);
    vm_float_t *b = (vm_float_t *)calloc((size_t)n, sizeof(vm_float_t));
    vm_float_t *x0 = (vm_float_t *)calloc((size_t)n, sizeof(vm_float_t));
    vm_float_t *x1 = (vm_float_t *)calloc((size_t)n, sizeof(vm_float_t));
    REQUIRE(b && x0 && x1);
    for (int j = 1; j < 9; ++j) {
        for (int i = 1; i < 9; ++i) {
            b[vm_grid_cell(g, i, j, 0)] = VM_F(1.0);
        }
    }
    vm_ksp_info none_info;
    vm_ksp_info ic_info;
    REQUIRE(vm_cg(&A, b, x0, VM_F(1e-6), 400, VM_KSP_PREC_NONE, &none_info));
    REQUIRE(vm_cg(&A, b, x1, VM_F(1e-6), 400, VM_KSP_PREC_IC0, &ic_info));
    REQUIRE(none_info.ok && ic_info.ok);
    REQUIRE(ic_info.iters <= none_info.iters);
    free(b);
    free(x0);
    free(x1);
    vm_spmat_free(&A);
}

TEST_CASE(vm_bicgstab_nonsymmetric, "[sparse]") {
    /* Convection–diffusion style: diag 4, right -1, left -1 + extra 0.5. */
    const int row[6] = { 0, 0, 1, 1, 1, 2 };
    const int col[6] = { 0, 1, 0, 1, 2, 2 };
    const vm_float_t val[6] = { 4, -1, -0.5, 4, -1, 3 };
    vm_spmat A;
    vm_spmat_init(&A);
    REQUIRE(vm_spmat_from_triplets(&A, 3, 6, row, col, val));
    const vm_float_t b[3] = { 3, 2.5, 3 };
    vm_float_t x[3] = { 0, 0, 0 };
    vm_ksp_info info;
    REQUIRE(vm_bicgstab(&A, b, x, VM_F(1e-8), 32, VM_KSP_PREC_JACOBI, &info));
    REQUIRE(info.ok);
    vm_float_t ax[3];
    vm_spmv(ax, &A, x);
    REQUIRE(VECMAT_EQ(ax[0], b[0], EPSILON * VM_F(20.0)));
    REQUIRE(VECMAT_EQ(ax[1], b[1], EPSILON * VM_F(20.0)));
    REQUIRE(VECMAT_EQ(ax[2], b[2], EPSILON * VM_F(20.0)));
    vm_spmat_free(&A);
}

TEST_CASE(vm_chol_dense_roundtrip, "[solvers]") {
    vm_mat A = vm_mat_alloc(3, 3);
    vm_mat_set(&A, 0, 0, 4); vm_mat_set(&A, 1, 0, 2); vm_mat_set(&A, 2, 0, 0);
    vm_mat_set(&A, 0, 1, 2); vm_mat_set(&A, 1, 1, 5); vm_mat_set(&A, 2, 1, 1);
    vm_mat_set(&A, 0, 2, 0); vm_mat_set(&A, 1, 2, 1); vm_mat_set(&A, 2, 2, 3);
    REQUIRE(vm_chol_factor(&A));
    const vm_float_t b[3] = { 8, 13, 5 };
    vm_float_t x[3];
    REQUIRE(vm_chol_solve(&A, b, x));
    /* A_orig x = b: x = (1, 2, 1) */
    REQUIRE(VECMAT_EQ(x[0], VM_F(1.0), EPSILON * VM_F(20.0)));
    REQUIRE(VECMAT_EQ(x[1], VM_F(2.0), EPSILON * VM_F(20.0)));
    REQUIRE(VECMAT_EQ(x[2], VM_F(1.0), EPSILON * VM_F(20.0)));
    vm_mat_free(&A);
}

TEST_CASE(mat3_spd_solve_identity, "[physics]") {
    const matrix3 I = mat3_identity();
    const vector3 b = { .x = 3, .y = -2, .z = 4 };
    vector3 x;
    REQUIRE(mat3_spd_solve(I, b, &x));
    REQUIRE(vec3_near(x, b, EPSILON));
}

TEST_CASE(mat3_spd_solve_spd, "[physics]") {
    const matrix3 A = {
        .m11 = 4, .m21 = 1, .m31 = 0,
        .m12 = 1, .m22 = 3, .m32 = 0,
        .m13 = 0, .m23 = 0, .m33 = 2
    };
    const vector3 b = { .x = 5, .y = 5, .z = 2 };
    vector3 x;
    REQUIRE(mat3_spd_solve(A, b, &x));
    const vector3 ax = mat3_mul_vec3(A, x);
    REQUIRE(vec3_near(ax, b, EPSILON * VM_F(20.0)));
}

static void ode_decay(const vm_float_t *y, vm_float_t *ydot, void *ctx)
{
    (void)ctx;
    ydot[0] = -y[0];
}

TEST_CASE(vm_rk4_exponential_decay, "[integrate]") {
    vm_float_t y = VM_F(1.0);
    const vm_float_t dt = VM_F(0.1);
    for (int i = 0; i < 10; ++i) {
        vm_rk4(ode_decay, &y, 1, dt, NULL);
    }
    const vm_float_t expect = VECMAT_POW(M_E, VM_F(-1.0));
    REQUIRE(VECMAT_EQ(y, expect, VM_F(1e-4)));
}

TEST_CASE(vm_rk2_exponential_decay, "[integrate]") {
    vm_float_t y = VM_F(1.0);
    const vm_float_t dt = VM_F(0.05);
    for (int i = 0; i < 20; ++i) {
        vm_rk2(ode_decay, &y, 1, dt, NULL);
    }
    const vm_float_t expect = VECMAT_POW(M_E, VM_F(-1.0));
    REQUIRE(VECMAT_EQ(y, expect, VM_F(2e-3)));
}

static void hooke(const vm_float_t *x, vm_float_t *a, void *ctx)
{
    (void)ctx;
    a[0] = -x[0];
}

TEST_CASE(vm_verlet_harmonic_bounded, "[integrate]") {
    vm_float_t x = VM_F(1.0);
    vm_float_t v = VM_F(0.0);
    vm_float_t a = -x;
    const vm_float_t dt = VM_F(0.05);
    vm_float_t max_e = VM_F(0.0);
    const vm_float_t e0 = VM_F(0.5) * v * v + VM_F(0.5) * x * x;
    for (int i = 0; i < 200; ++i) {
        vm_verlet(hooke, &x, &v, &a, 1, dt, NULL);
        const vm_float_t e = VM_F(0.5) * v * v + VM_F(0.5) * x * x;
        const vm_float_t de = VECMAT_FABS(e - e0);
        if (de > max_e) {
            max_e = de;
        }
    }
    REQUIRE(max_e < VM_F(0.05));
}

TEST_CASE(vm_euler_semi_advances, "[integrate]") {
    vm_float_t x = VM_F(0.0);
    vm_float_t v = VM_F(0.0);
    const vm_float_t a = VM_F(2.0);
    vm_euler_semi(&x, &v, &a, 1, VM_F(1.0));
    REQUIRE(VECMAT_EQ(v, VM_F(2.0), EPSILON));
    REQUIRE(VECMAT_EQ(x, VM_F(2.0), EPSILON));
}

TEST_CASE(vm_cfl_dt_scales, "[integrate]") {
    const vm_float_t dt = vm_cfl_dt(VM_F(0.5), VM_F(0.1), VM_F(2.0));
    REQUIRE(dt > VM_F(0.0));
    REQUIRE(dt < VM_F(0.1));
}

TEST_CASE(vm_rigid_step_rest_stays_rest, "[physics]") {
    vector3 x = { .x = 1, .y = 2, .z = 3 };
    vector3 v = { 0 };
    quaternion q = quat_identity();
    vector3 w = { 0 };
    const matrix3 I = mat3_identity();
    vm_rigid_step(&x, &v, &q, &w, (vector3){ 0 }, (vector3){ 0 }, VM_F(2.0), I, VM_F(0.016));
    REQUIRE(vec3_near(x, (vector3){ .x = 1, .y = 2, .z = 3 }, EPSILON));
    REQUIRE(vec3_near(v, (vector3){ 0 }, EPSILON));
    REQUIRE(quat_near(q, quat_identity(), EPSILON));
}

TEST_CASE(vm_rigid_step_force_moves, "[physics]") {
    vector3 x = { 0 };
    vector3 v = { 0 };
    quaternion q = quat_identity();
    vector3 w = { 0 };
    const matrix3 I = {
        .m11 = 2, .m21 = 0, .m31 = 0,
        .m12 = 0, .m22 = 2, .m32 = 0,
        .m13 = 0, .m23 = 0, .m33 = 2
    };
    const vector3 F = { .x = 4, .y = 0, .z = 0 };
    vm_rigid_step(&x, &v, &q, &w, F, (vector3){ 0 }, VM_F(2.0), I, VM_F(1.0));
    REQUIRE(VECMAT_EQ(v.x, VM_F(2.0), EPSILON));
    REQUIRE(VECMAT_EQ(x.x, VM_F(2.0), EPSILON));
}

TEST_CASE(vm_inertia_world_identity_rotation, "[physics]") {
    const matrix3 Ib = {
        .m11 = 2, .m21 = 0, .m31 = 0,
        .m12 = 0, .m22 = 3, .m32 = 0,
        .m13 = 0, .m23 = 0, .m33 = 4
    };
    const matrix3 Iw = vm_inertia_world(Ib, quat_identity());
    REQUIRE(VECMAT_EQ(Iw.m11, VM_F(2.0), EPSILON));
    REQUIRE(VECMAT_EQ(Iw.m22, VM_F(3.0), EPSILON));
    REQUIRE(VECMAT_EQ(Iw.m33, VM_F(4.0), EPSILON));
}

TEST_CASE(vm_omega_from_L_matches, "[physics]") {
    const matrix3 I = {
        .m11 = 2, .m21 = 0, .m31 = 0,
        .m12 = 0, .m22 = 4, .m32 = 0,
        .m13 = 0, .m23 = 0, .m33 = 8
    };
    const vector3 w0 = { .x = 1, .y = 2, .z = 3 };
    const vector3 L = mat3_mul_vec3(I, w0);
    const vector3 w = vm_omega_from_L(I, L);
    REQUIRE(vec3_near(w, w0, EPSILON * VM_F(20.0)));
}

TEST_CASE(vm_rigid_energy_positive, "[physics]") {
    const matrix3 I = mat3_identity();
    const vm_float_t e = vm_rigid_energy(VM_F(2.0), (vector3){ .x = 1, .y = 0, .z = 0 },
                                         I, (vector3){ .x = 0, .y = 2, .z = 0 });
    REQUIRE(VECMAT_EQ(e, VM_F(3.0), EPSILON)); /* 0.5*2*1 + 0.5*2*2 */
}

TEST_CASE(vm_baumgarte_pulls_to_plane, "[physics]") {
    vector3 x = { .x = 0, .y = 2, .z = 0 };
    vector3 v = { .x = 0, .y = 1, .z = 0 };
    const vector3 n = { .x = 0, .y = 1, .z = 0 };
    vm_baumgarte_correct(&x, &v, n, x.y, VM_F(1.0), VM_F(0.0), VM_F(0.016));
    REQUIRE(VECMAT_EQ(x.y, VM_F(0.0), EPSILON));
}

TEST_CASE(vm_mac_div_constant_is_zero, "[grid]") {
    const vm_grid3 g = vm_grid3_make(4, 3, 1, VM_F(1.0), VM_F(1.0), VM_F(1.0));
    const int nu = vm_mac_nu(g);
    const int nv = vm_mac_nv(g);
    const int n = vm_grid_ncells(g);
    vm_float_t *u = (vm_float_t *)malloc((size_t)nu * sizeof(vm_float_t));
    vm_float_t *v = (vm_float_t *)malloc((size_t)nv * sizeof(vm_float_t));
    vm_float_t *div = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    REQUIRE(u && v && div);
    for (int i = 0; i < nu; ++i) {
        u[i] = VM_F(3.0);
    }
    for (int i = 0; i < nv; ++i) {
        v[i] = VM_F(-2.0);
    }
    vm_mac_div(div, u, v, NULL, g);
    for (int i = 0; i < n; ++i) {
        REQUIRE(VECMAT_EQ(div[i], VM_F(0.0), EPSILON));
    }
    free(u);
    free(v);
    free(div);
}

TEST_CASE(vm_mac_grad_then_div_is_laplacian_interior, "[grid]") {
    const vm_grid3 g = vm_grid3_make(5, 5, 1, VM_F(1.0), VM_F(1.0), VM_F(1.0));
    const int n = vm_grid_ncells(g);
    vm_float_t *p = (vm_float_t *)calloc((size_t)n, sizeof(vm_float_t));
    vm_float_t *gu = (vm_float_t *)calloc((size_t)vm_mac_nu(g), sizeof(vm_float_t));
    vm_float_t *gv = (vm_float_t *)calloc((size_t)vm_mac_nv(g), sizeof(vm_float_t));
    vm_float_t *div = (vm_float_t *)calloc((size_t)n, sizeof(vm_float_t));
    REQUIRE(p && gu && gv && div);
    p[vm_grid_cell(g, 2, 2, 0)] = VM_F(1.0);
    vm_mac_grad(gu, gv, NULL, p, g);
    vm_mac_div(div, gu, gv, NULL, g);
    /* div(grad p) is the (negative definite) Laplacian: centre -4, neighbour +1 */
    REQUIRE(VECMAT_EQ(div[vm_grid_cell(g, 2, 2, 0)], VM_F(-4.0), EPSILON));
    REQUIRE(VECMAT_EQ(div[vm_grid_cell(g, 3, 2, 0)], VM_F(1.0), EPSILON));
    free(p);
    free(gu);
    free(gv);
    free(div);
}

TEST_CASE(vm_project_mac_field, "[grid]") {
    const vm_grid3 g = vm_grid3_make(8, 8, 1, VM_F(1.0), VM_F(1.0), VM_F(1.0));
    const int n = vm_grid_ncells(g);
    const int nu = vm_mac_nu(g);
    const int nv = vm_mac_nv(g);
    vm_float_t *u = (vm_float_t *)calloc((size_t)nu, sizeof(vm_float_t));
    vm_float_t *v = (vm_float_t *)calloc((size_t)nv, sizeof(vm_float_t));
    vm_float_t *div = (vm_float_t *)calloc((size_t)n, sizeof(vm_float_t));
    vm_float_t *p = (vm_float_t *)calloc((size_t)n, sizeof(vm_float_t));
    vm_float_t *rhs = (vm_float_t *)calloc((size_t)n, sizeof(vm_float_t));
    REQUIRE(u && v && div && p && rhs);

    for (int j = 0; j < g.ny; ++j) {
        for (int i = 0; i <= g.nx; ++i) {
            u[vm_mac_u(g, i, j, 0)] = (vm_float_t)i;
        }
    }

    vm_mac_div(div, u, v, NULL, g);
    vm_spmat A;
    vm_spmat_init(&A);
    REQUIRE(vm_grid_laplacian(&A, g, VM_BC_DIRICHLET));
    for (int j = 0; j < g.ny; ++j) {
        for (int i = 0; i < g.nx; ++i) {
            const int c = vm_grid_cell(g, i, j, 0);
            rhs[c] = (i == 0 || i == g.nx - 1 || j == 0 || j == g.ny - 1) ? VM_F(0.0) : -div[c];
        }
    }
    vm_ksp_info info;
    REQUIRE(vm_cg(&A, rhs, p, VM_F(1e-5), 300, VM_KSP_PREC_JACOBI, &info));

    vm_float_t *gu = (vm_float_t *)calloc((size_t)nu, sizeof(vm_float_t));
    vm_float_t *gv = (vm_float_t *)calloc((size_t)nv, sizeof(vm_float_t));
    REQUIRE(gu && gv);
    vm_mac_grad(gu, gv, NULL, p, g);
    for (int i = 0; i < nu; ++i) {
        u[i] -= gu[i];
    }
    for (int i = 0; i < nv; ++i) {
        v[i] -= gv[i];
    }
    vm_mac_div(div, u, v, NULL, g);
    vm_float_t max_div = VM_F(0.0);
    for (int j = 1; j < g.ny - 1; ++j) {
        for (int i = 1; i < g.nx - 1; ++i) {
            const vm_float_t d = VECMAT_FABS(div[vm_grid_cell(g, i, j, 0)]);
            if (d > max_div) {
                max_div = d;
            }
        }
    }
    REQUIRE(max_div < VM_F(5e-3));

    free(u);
    free(v);
    free(div);
    free(p);
    free(rhs);
    free(gu);
    free(gv);
    vm_spmat_free(&A);
}
