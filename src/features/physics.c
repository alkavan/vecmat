// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <stdlib.h>
#include <string.h>
#include <vecmat.h>

void vm_euler_semi(vm_float_t *x, vm_float_t *v, const vm_float_t *a, const int n, const vm_float_t dt)
{
    if (!x || !v || !a || n <= 0) {
        return;
    }
    for (int i = 0; i < n; ++i) {
        v[i] += a[i] * dt;
        x[i] += v[i] * dt;
    }
}

void vm_verlet(const vm_acc_fn acc, vm_float_t *x, vm_float_t *v, vm_float_t *a,
               const int n, const vm_float_t dt, void *ctx)
{
    if (!acc || !x || !v || !a || n <= 0) {
        return;
    }

    const vm_float_t half = dt * VM_F(0.5);
    for (int i = 0; i < n; ++i) {
        v[i] += a[i] * half;
        x[i] += v[i] * dt;
    }

    acc(x, a, ctx);
    for (int i = 0; i < n; ++i) {
        v[i] += a[i] * half;
    }
}

void vm_rk2(const vm_ode_fn f, vm_float_t *y, const int n, const vm_float_t dt, void *ctx)
{
    if (!f || !y || n <= 0) {
        return;
    }

    vm_float_t *k1 = malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *yt = malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *k2 = malloc((size_t)n * sizeof(vm_float_t));
    if (!k1 || !yt || !k2) {
        free(k1);
        free(yt);
        free(k2);
        return;
    }

    f(y, k1, ctx);
    for (int i = 0; i < n; ++i) {
        yt[i] = y[i] + dt * k1[i];
    }

    f(yt, k2, ctx);
    for (int i = 0; i < n; ++i) {
        y[i] += dt * VM_F(0.5) * (k1[i] + k2[i]);
    }

    free(k1);
    free(yt);
    free(k2);
}

void vm_rk4(const vm_ode_fn f, vm_float_t *y, const int n, const vm_float_t dt, void *ctx)
{
    if (!f || !y || n <= 0) {
        return;
    }
    vm_float_t *k1 = malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *k2 = malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *k3 = malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *k4 = malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *yt = malloc((size_t)n * sizeof(vm_float_t));
    if (!k1 || !k2 || !k3 || !k4 || !yt) {
        free(k1);
        free(k2);
        free(k3);
        free(k4);
        free(yt);
        return;
    }

    const vm_float_t h2 = dt * VM_F(0.5);

    f(y, k1, ctx);
    for (int i = 0; i < n; ++i) {
        yt[i] = y[i] + h2 * k1[i];
    }

    f(yt, k2, ctx);
    for (int i = 0; i < n; ++i) {
        yt[i] = y[i] + h2 * k2[i];
    }

    f(yt, k3, ctx);
    for (int i = 0; i < n; ++i) {
        yt[i] = y[i] + dt * k3[i];
    }

    f(yt, k4, ctx);
    const vm_float_t s = dt / VM_F(6.0);
    for (int i = 0; i < n; ++i) {
        y[i] += s * (k1[i] + VM_F(2.0) * k2[i] + VM_F(2.0) * k3[i] + k4[i]);
    }

    free(k1);
    free(k2);
    free(k3);
    free(k4);
    free(yt);
}

vm_float_t vm_cfl_dt(const vm_float_t cfl, const vm_float_t dx, const vm_float_t speed)
{
    const vm_float_t den = VECMAT_FABS(speed) + VECMAT_EPSILON;
    return cfl * dx / den;
}

bool mat3_chol(const matrix3 a, matrix3 *L)
{
    if (!L) {
        return false;
    }
    const vm_float_t a11 = a.m11;
    if (a11 <= VECMAT_EPSILON) {
        return false;
    }
    const vm_float_t l11 = VECMAT_SQRT(a11);
    const vm_float_t l21 = a.m21 / l11;
    const vm_float_t l31 = a.m31 / l11;
    const vm_float_t t22 = a.m22 - l21 * l21;
    if (t22 <= VECMAT_EPSILON) {
        return false;
    }
    const vm_float_t l22 = VECMAT_SQRT(t22);
    const vm_float_t l32 = (a.m32 - l31 * l21) / l22;
    const vm_float_t t33 = a.m33 - l31 * l31 - l32 * l32;
    if (t33 <= VECMAT_EPSILON) {
        return false;
    }
    *L = (matrix3){
        .m11 = l11, .m21 = l21, .m31 = l31,
        .m12 = VM_F(0.0), .m22 = l22, .m32 = l32,
        .m13 = VM_F(0.0), .m23 = VM_F(0.0), .m33 = VECMAT_SQRT(t33)
    };
    return true;
}

/**
 * @brief Solve `L Lᵀ x = b` given a lower-triangular Cholesky factor `L`.
 *
 * Performs forward substitution for `L y = b`, then back substitution for
 * `Lᵀ x = y`.
 *
 * @param L Lower-triangular Cholesky factor of an SPD 3×3 matrix.
 * @param b Right-hand side vector.
 * @param x Solution vector (out).
 * @return `true` on success.
 */
static bool mat3_chol_solve(const matrix3 L, const vector3 b, vector3 *x)
{
    const vm_float_t y1 = b.x / L.m11;
    const vm_float_t y2 = (b.y - L.m21 * y1) / L.m22;
    const vm_float_t y3 = (b.z - L.m31 * y1 - L.m32 * y2) / L.m33;
    const vm_float_t z3 = y3 / L.m33;
    const vm_float_t z2 = (y2 - L.m32 * z3) / L.m22;
    const vm_float_t z1 = (y1 - L.m21 * z2 - L.m31 * z3) / L.m11;
    x->x = z1;
    x->y = z2;
    x->z = z3;
    return true;
}

bool mat3_spd_solve(const matrix3 a, const vector3 b, vector3 *x)
{
    if (!x) {
        return false;
    }
    matrix3 L;
    if (!mat3_chol(a, &L)) {
        return false;
    }
    return mat3_chol_solve(L, b, x);
}

matrix3 vm_inertia_world(const matrix3 ib, const quaternion q)
{
    const matrix3 R = quat_to_mat3(q);
    return mat3_mul(mat3_mul(R, ib), mat3_transpose(R));
}

vector3 vm_omega_from_angmom(const matrix3 I, const vector3 L)
{
    vector3 w = { 0 };
    if (!mat3_spd_solve(I, L, &w)) {
        return (vector3){ 0 };
    }
    return w;
}

vm_float_t vm_rigid_energy(const vm_float_t mass, const vector3 v, const matrix3 I, const vector3 w)
{
    const vector3 Iw = mat3_mul_vec3(I, w);
    return VM_F(0.5) * mass * vec3_length_squared(v) + VM_F(0.5) * vec3_dot(w, Iw);
}

void vm_rigid_step(vector3 *x, vector3 *v, quaternion *q, vector3 *w,
                   const vector3 F, const vector3 tau, const vm_float_t mass,
                   const matrix3 I_body, const vm_float_t dt)
{
    if (!x || !v || !q || !w) {
        return;
    }
    const vm_float_t inv_m = (VECMAT_FABS(mass) > VECMAT_EPSILON) ? (VM_F(1.0) / mass) : VM_F(0.0);
    *v = vec3_add(*v, vec3_mul_scalar(F, inv_m * dt));
    *x = vec3_add(*x, vec3_mul_scalar(*v, dt));

    const vector3 Iw = mat3_mul_vec3(I_body, *w);
    const vector3 gyro = vec3_cross(*w, Iw);
    const vector3 rhs = vec3_sub(tau, gyro);
    vector3 alpha;
    if (mat3_spd_solve(I_body, rhs, &alpha)) {
        *w = vec3_add(*w, vec3_mul_scalar(alpha, dt));
    }
    *q = quat_integrate(*q, *w, dt);
}

void vm_baumgarte_correct(vector3 *x, vector3 *v, const vector3 n, const vm_float_t C,
                          const vm_float_t beta, const vm_float_t gamma, const vm_float_t dt)
{
    if (!x || !v) {
        return;
    }

    const vm_float_t inv_dt = (VECMAT_FABS(dt) > VECMAT_EPSILON) ? (VM_F(1.0) / dt) : VM_F(0.0);
    *x = vec3_sub(*x, vec3_mul_scalar(n, beta * C));
    *v = vec3_sub(*v, vec3_mul_scalar(n, gamma * C * inv_dt));
}

vm_grid3 vm_grid3_make(const int nx, const int ny, const int nz,
                       const vm_float_t dx, const vm_float_t dy, const vm_float_t dz)
{
    const vm_grid3 g = {
        .nx = nx,
        .ny = ny,
        .nz = (nz > 0) ? nz : 1,
        .dx = dx,
        .dy = dy,
        .dz = dz
    };
    return g;
}

int vm_grid_ncells(const vm_grid3 g)
{
    const int nz = (g.nz > 0) ? g.nz : 1;
    if (g.nx <= 0 || g.ny <= 0) {
        return 0;
    }
    return g.nx * g.ny * nz;
}

int vm_grid_cell(const vm_grid3 g, const int i, const int j, const int k)
{
    return i + g.nx * (j + g.ny * k);
}

int vm_mac_nu(const vm_grid3 g)
{
    const int nz = (g.nz > 0) ? g.nz : 1;
    if (g.nx <= 0 || g.ny <= 0) {
        return 0;
    }
    return (g.nx + 1) * g.ny * nz;
}

int vm_mac_nv(const vm_grid3 g)
{
    const int nz = (g.nz > 0) ? g.nz : 1;
    if (g.nx <= 0 || g.ny <= 0) {
        return 0;
    }
    return g.nx * (g.ny + 1) * nz;
}

int vm_mac_nw(const vm_grid3 g)
{
    if (g.nz <= 1 || g.nx <= 0 || g.ny <= 0) {
        return 0;
    }
    return g.nx * g.ny * (g.nz + 1);
}

int vm_mac_u(const vm_grid3 g, const int i, const int j, const int k)
{
    return i + (g.nx + 1) * (j + g.ny * k);
}

int vm_mac_v(const vm_grid3 g, const int i, const int j, const int k)
{
    return i + g.nx * (j + (g.ny + 1) * k);
}

int vm_mac_w(const vm_grid3 g, const int i, const int j, const int k)
{
    return i + g.nx * (j + g.ny * k);
}

/**
 * @brief Test if a grid cell is on the domain boundary.
 *
 * @param g 3-D grid descriptor.
 * @param i cell index in x direction.
 * @param j cell index in y direction.
 * @param k cell index in z direction.
 *
 * @return true if the cell lies on any face of the grid, false otherwise.
 */
static bool vm_grid_on_boundary(const vm_grid3 g, const int i, const int j, const int k)
{
    const int nz = (g.nz > 0) ? g.nz : 1;
    if (i == 0 || i == g.nx - 1 || j == 0 || j == g.ny - 1) {
        return true;
    }
    if (nz > 1 && (k == 0 || k == nz - 1)) {
        return true;
    }
    return false;
}

bool vm_grid_laplacian(vm_spmat *A, const vm_grid3 g, const vm_bc_t bc)
{
    const int nz = (g.nz > 0) ? g.nz : 1;
    const int n = vm_grid_ncells(g);
    if (!A || n <= 0 || g.dx == VM_F(0.0) || g.dy == VM_F(0.0) || (nz > 1 && g.dz == VM_F(0.0))) {
        return false;
    }

    const int cap = n * 7;
    int *row = malloc((size_t)cap * sizeof(int));
    int *col = malloc((size_t)cap * sizeof(int));
    vm_float_t *val = malloc((size_t)cap * sizeof(vm_float_t));

    if (!row || !col || !val) {
        free(row);
        free(col);
        free(val);
        return false;
    }

    const vm_float_t ix = VM_F(1.0) / (g.dx * g.dx);
    const vm_float_t iy = VM_F(1.0) / (g.dy * g.dy);
    const vm_float_t iz = (nz > 1) ? (VM_F(1.0) / (g.dz * g.dz)) : VM_F(0.0);

    int nnz = 0;
    for (int k = 0; k < nz; ++k) {
        for (int j = 0; j < g.ny; ++j) {
            for (int i = 0; i < g.nx; ++i) {
                const int p = vm_grid_cell(g, i, j, k);
                if (bc == VM_BC_DIRICHLET && vm_grid_on_boundary(g, i, j, k)) {
                    row[nnz] = p;
                    col[nnz] = p;
                    val[nnz] = VM_F(1.0);
                    ++nnz;
                    continue;
                }

                vm_float_t diag = VM_F(0.0);
                const int nb[6][3] = {
                    { i - 1, j, k }, { i + 1, j, k },
                    { i, j - 1, k }, { i, j + 1, k },
                    { i, j, k - 1 }, { i, j, k + 1 }
                };
                const vm_float_t w[6] = { ix, ix, iy, iy, iz, iz };
                const int nnb = (nz > 1) ? 6 : 4;
                for (int t = 0; t < nnb; ++t) {
                    const int ni = nb[t][0];
                    const int nj = nb[t][1];
                    const int nk = nb[t][2];
                    const bool inside = ni >= 0 && ni < g.nx && nj >= 0 && nj < g.ny && nk >= 0 && nk < nz;
                    if (!inside) {
                        if (bc != VM_BC_NEUMANN) {
                            diag += w[t];
                        }
                        continue;
                    }
                    /* Homogeneous Dirichlet: keep the weight on the diagonal
                     * and omit the boundary column so A stays SPD. */
                    if (bc == VM_BC_DIRICHLET && vm_grid_on_boundary(g, ni, nj, nk)) {
                        diag += w[t];
                        continue;
                    }
                    row[nnz] = p;
                    col[nnz] = vm_grid_cell(g, ni, nj, nk);
                    val[nnz] = -w[t];
                    ++nnz;
                    diag += w[t];
                }
                row[nnz] = p;
                col[nnz] = p;
                val[nnz] = diag;
                ++nnz;
            }
        }
    }

    const bool ok = vm_spmat_from_triplets(A, n, nnz, row, col, val);
    free(row);
    free(col);
    free(val);
    return ok;
}

void vm_mac_div(vm_float_t *div, const vm_float_t *u, const vm_float_t *v,
    const vm_float_t *w, const vm_grid3 g)
{
    const int nz = (g.nz > 0) ? g.nz : 1;
    if (!div || !u || !v || g.nx <= 0 || g.ny <= 0) {
        return;
    }
    const vm_float_t inv_x = VM_F(1.0) / g.dx;
    const vm_float_t inv_y = VM_F(1.0) / g.dy;
    const vm_float_t inv_z = (nz > 1 && w && g.dz != VM_F(0.0)) ? (VM_F(1.0) / g.dz) : VM_F(0.0);
    for (int k = 0; k < nz; ++k) {
        for (int j = 0; j < g.ny; ++j) {
            for (int i = 0; i < g.nx; ++i) {
                vm_float_t d = (u[vm_mac_u(g, i + 1, j, k)] - u[vm_mac_u(g, i, j, k)]) * inv_x
                             + (v[vm_mac_v(g, i, j + 1, k)] - v[vm_mac_v(g, i, j, k)]) * inv_y;
                if (inv_z != VM_F(0.0)) {
                    d += (w[vm_mac_w(g, i, j, k + 1)] - w[vm_mac_w(g, i, j, k)]) * inv_z;
                }
                div[vm_grid_cell(g, i, j, k)] = d;
            }
        }
    }
}

void vm_mac_grad(vm_float_t *gu, vm_float_t *gv, vm_float_t *gw,
                 const vm_float_t *p, const vm_grid3 g)
{
    const int nz = (g.nz > 0) ? g.nz : 1;
    if (!gu || !gv || !p || g.nx <= 0 || g.ny <= 0) {
        return;
    }

    memset(gu, 0, (size_t)vm_mac_nu(g) * sizeof(vm_float_t));
    memset(gv, 0, (size_t)vm_mac_nv(g) * sizeof(vm_float_t));
    if (gw && vm_mac_nw(g) > 0) {
        memset(gw, 0, (size_t)vm_mac_nw(g) * sizeof(vm_float_t));
    }
    const vm_float_t inv_x = VM_F(1.0) / g.dx;
    const vm_float_t inv_y = VM_F(1.0) / g.dy;
    const vm_float_t inv_z = (nz > 1 && gw && g.dz != VM_F(0.0)) ? (VM_F(1.0) / g.dz) : VM_F(0.0);

    for (int k = 0; k < nz; ++k) {
        for (int j = 0; j < g.ny; ++j) {
            for (int i = 1; i < g.nx; ++i) {
                gu[vm_mac_u(g, i, j, k)] =
                    (p[vm_grid_cell(g, i, j, k)] - p[vm_grid_cell(g, i - 1, j, k)]) * inv_x;
            }
        }
    }

    for (int k = 0; k < nz; ++k) {
        for (int j = 1; j < g.ny; ++j) {
            for (int i = 0; i < g.nx; ++i) {
                gv[vm_mac_v(g, i, j, k)] =
                    (p[vm_grid_cell(g, i, j, k)] - p[vm_grid_cell(g, i, j - 1, k)]) * inv_y;
            }
        }
    }

    if (inv_z != VM_F(0.0)) {
        for (int k = 1; k < nz; ++k) {
            for (int j = 0; j < g.ny; ++j) {
                for (int i = 0; i < g.nx; ++i) {
                    gw[vm_mac_w(g, i, j, k)] =
                        (p[vm_grid_cell(g, i, j, k)] - p[vm_grid_cell(g, i, j, k - 1)]) * inv_z;
                }
            }
        }
    }
}

void vm_mac_curl_z(vm_float_t *cz, const vm_float_t *u, const vm_float_t *v, const vm_grid3 g)
{
    const int nz = (g.nz > 0) ? g.nz : 1;
    if (!cz || !u || !v || g.nx <= 0 || g.ny <= 0) {
        return;
    }

    const vm_float_t inv_x = VM_F(1.0) / g.dx;
    const vm_float_t inv_y = VM_F(1.0) / g.dy;

    for (int k = 0; k < nz; ++k) {
        for (int j = 0; j < g.ny; ++j) {
            for (int i = 0; i < g.nx; ++i) {
                /* average neighbouring face differences onto the cell */
                const vm_float_t dv_dx = VM_F(0.5) * (
                    (v[vm_mac_v(g, (i + 1 < g.nx) ? i + 1 : i, j, k)]
                     - v[vm_mac_v(g, (i > 0) ? i - 1 : i, j, k)]) * inv_x
                    + (v[vm_mac_v(g, (i + 1 < g.nx) ? i + 1 : i, j + 1, k)]
                       - v[vm_mac_v(g, (i > 0) ? i - 1 : i, j + 1, k)]) * inv_x);
                const vm_float_t du_dy = VM_F(0.5) * (
                    (u[vm_mac_u(g, i, (j + 1 < g.ny) ? j + 1 : j, k)]
                     - u[vm_mac_u(g, i, (j > 0) ? j - 1 : j, k)]) * inv_y
                    + (u[vm_mac_u(g, i + 1, (j + 1 < g.ny) ? j + 1 : j, k)]
                       - u[vm_mac_u(g, i + 1, (j > 0) ? j - 1 : j, k)]) * inv_y);
                cz[vm_grid_cell(g, i, j, k)] = dv_dx - du_dy;
            }
        }
    }
}
