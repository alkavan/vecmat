// SPDX-FileCopyrightText: Copyright (c) 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: Copyright (c) 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <stdlib.h>
#include <string.h>
#include <vecmat.h>

/**
 * @brief Semi-implicit Euler: `v += a dt`, then `x += v dt`.
 *
 * @param x  Position vector (in/out); length `n`.
 * @param v  Velocity vector (in/out); length `n`.
 * @param a  Acceleration vector; length `n`.
 * @param n  State dimension.
 * @param dt Timestep.
 */
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

/**
 * @brief Velocity Verlet with an acceleration callback.
 *
 * Uses the incoming `a` at `x`, advances `x`, re-evaluates `acc`, then
 * completes the velocity half-kick.
 *
 * @param acc Acceleration callback `acc(x, a, ctx)`.
 * @param x   Position vector (in/out); length `n`.
 * @param v   Velocity vector (in/out); length `n`.
 * @param a   Acceleration vector (in/out); length `n`.
 * @param n   State dimension.
 * @param dt  Timestep.
 * @param ctx User context passed to `acc`.
 */
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

/**
 * @brief Explicit midpoint RK2 for `y' = f(y)`.
 *
 * @param f   ODE right-hand side `f(y, dy, ctx)`.
 * @param y   State vector (in/out); length `n`.
 * @param n   State dimension.
 * @param dt  Timestep.
 * @param ctx User context passed to `f`.
 */
void vm_rk2(const vm_ode_fn f, vm_float_t *y, const int n, const vm_float_t dt, void *ctx)
{
    if (!f || !y || n <= 0) {
        return;
    }

    vm_float_t *k1 = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *yt = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *k2 = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
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

/**
 * @brief Classic RK4 for `y' = f(y)`.
 *
 * @param f   ODE right-hand side `f(y, dy, ctx)`.
 * @param y   State vector (in/out); length `n`.
 * @param n   State dimension.
 * @param dt  Timestep.
 * @param ctx User context passed to `f`.
 */
void vm_rk4(const vm_ode_fn f, vm_float_t *y, const int n, const vm_float_t dt, void *ctx)
{
    if (!f || !y || n <= 0) {
        return;
    }
    vm_float_t *k1 = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *k2 = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *k3 = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *k4 = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    vm_float_t *yt = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
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

/**
 * @brief CFL timestep `dt = cfl * dx / (|u| + ε)`.
 *
 * @param cfl   CFL number (typically in `(0, 1]`).
 * @param dx    Characteristic cell size.
 * @param speed Characteristic speed (e.g. `|u|`).
 * @return Stable timestep estimate.
 */
vm_float_t vm_cfl_dt(const vm_float_t cfl, const vm_float_t dx, const vm_float_t speed)
{
    const vm_float_t den = VECMAT_FABS(speed) + VECMAT_EPSILON;
    return cfl * dx / den;
}

/**
 * @brief 3×3 Cholesky `A = L Lᵀ`. `L` is lower; the upper triangle is zeroed.
 *
 * @param a SPD coefficient matrix.
 * @param L Lower-triangular Cholesky factor (out); upper triangle set to 0.
 * @return `true` on success, `false` if `L` is NULL or `a` is not SPD.
 */
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

/**
 * @brief Solve the 3×3 SPD system `A x = b` via Cholesky.
 *
 * @param a SPD coefficient matrix.
 * @param b Right-hand side vector.
 * @param x Solution vector (out).
 * @return `true` on success, `false` if `x` is NULL or factorization fails.
 */
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

/**
 * @brief World-frame inertia `I_w = R I_b Rᵀ` from a body tensor and orientation.
 *
 * @param ib  Body-frame inertia tensor.
 * @param q   Orientation quaternion.
 * @return World-frame inertia tensor.
 */
matrix3 vm_inertia_world(const matrix3 ib, const quaternion q)
{
    const matrix3 R = quat_to_mat3(q);
    return mat3_mul(mat3_mul(R, ib), mat3_transpose(R));
}

/**
 * @brief Recover `ω` from angular momentum `L = I ω`.
 *
 * @param I Inertia tensor (same frame as `L`).
 * @param L Angular momentum.
 * @return Angular velocity, or the zero vector if the solve fails.
 */
vector3 vm_omega_from_angmom(const matrix3 I, const vector3 L)
{
    vector3 w = { 0 };
    if (!mat3_spd_solve(I, L, &w)) {
        return (vector3){ 0 };
    }
    return w;
}

/**
 * @brief Rigid kinetic energy `½ m |v|² + ½ ω · (I ω)`.
 *
 * `I` and `ω` must share a frame.
 *
 * @param mass Mass.
 * @param v    Linear velocity.
 * @param I    Inertia tensor (same frame as `w`).
 * @param w    Angular velocity (same frame as `I`).
 *
 * @return Kinetic energy.
 */
vm_float_t vm_rigid_energy(const vm_float_t mass, const vector3 v, const matrix3 I, const vector3 w)
{
    const vector3 Iw = mat3_mul_vec3(I, w);
    return VM_F(0.5) * mass * vec3_length_squared(v) + VM_F(0.5) * vec3_dot(w, Iw);
}

/**
 * @brief One symplectic-Euler rigid step.
 *
 * `x`, `v`, `F` are world-frame. `w` and `tau` are body-frame. `I_body` is
 * the body inertia (any SPD 3×3). Orientation is advanced with
 * `quat_integrate`.
 *
 * @param x      World-frame position (in/out).
 * @param v      World-frame linear velocity (in/out).
 * @param q      Orientation quaternion (in/out).
 * @param w      Body-frame angular velocity (in/out).
 * @param F      World-frame force.
 * @param tau    Body-frame torque.
 * @param mass   Mass.
 * @param I_body Body-frame inertia tensor (SPD 3×3).
 * @param dt     Timestep.
 */
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

/**
 * @brief Single-constraint Baumgarte correction along a unit normal.
 *
 * `C` is the signed constraint value (0 at contact). Position is moved by
 * `-beta C n`; velocity by `-gamma C / dt n`.
 *
 * @param x     Position to correct (in/out).
 * @param v     Velocity to correct (in/out).
 * @param n     Unit constraint normal.
 * @param C     Signed constraint value (`0` at contact).
 * @param beta  Position Baumgarte coefficient.
 * @param gamma Velocity Baumgarte coefficient.
 * @param dt    Timestep used for the velocity correction scale.
 */
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

/**
 * @brief Build a 3-D grid descriptor.
 *
 * @param nx Number of cells in x.
 * @param ny Number of cells in y.
 * @param nz Number of cells in z; values `<= 0` are treated as `1`.
 * @param dx Cell spacing in x.
 * @param dy Cell spacing in y.
 * @param dz Cell spacing in z.
 *
 * @return Grid descriptor with the given dimensions and spacing.
 */
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

/**
 * @brief Returns the total number of cells in the grid.
 *
 * @param g 3D grid descriptor.
 * @return Total cell count (`nx * ny * nz`), or `0` if `nx` or `ny` is non-positive.
 */
int vm_grid_ncells(const vm_grid3 g)
{
    const int nz = (g.nz > 0) ? g.nz : 1;
    if (g.nx <= 0 || g.ny <= 0) {
        return 0;
    }
    return g.nx * g.ny * nz;
}

/**
 * @brief Compute linear index of a cell in a 3-D grid stored in row-major order.
 *
 * @param g grid dimensions and spacing
 * @param i cell index along x
 * @param j cell index along y
 * @param k cell index along z
 *
 * @return flattened 1-D index of the cell
 */
int vm_grid_cell(const vm_grid3 g, const int i, const int j, const int k)
{
    return i + g.nx * (j + g.ny * k);
}

/**
 * @brief Returns the number of u-velocity MAC face values for the grid.
 *
 * @param g Grid definition.
 * @return Number of u-faces, or zero if the grid is invalid.
 */
int vm_mac_nu(const vm_grid3 g)
{
    const int nz = (g.nz > 0) ? g.nz : 1;
    if (g.nx <= 0 || g.ny <= 0) {
        return 0;
    }
    return (g.nx + 1) * g.ny * nz;
}

/**
 * @brief Returns the number of MAC grid v-velocity components.
 *
 * @param g Grid dimensions and spacing.
 * @return Number of v-velocity samples.
 */
int vm_mac_nv(const vm_grid3 g)
{
    const int nz = (g.nz > 0) ? g.nz : 1;
    if (g.nx <= 0 || g.ny <= 0) {
        return 0;
    }
    return g.nx * (g.ny + 1) * nz;
}

/**
 * @brief Returns the number of MAC grid faces in the z (vertical) direction.
 *
 * @param g Grid dimensions and spacing.
 * @return Number of vertical MAC faces, or 0 if the grid is invalid.
 */
int vm_mac_nw(const vm_grid3 g)
{
    if (g.nz <= 1 || g.nx <= 0 || g.ny <= 0) {
        return 0;
    }
    return g.nx * g.ny * (g.nz + 1);
}

/**
 * @brief Compute linear index of u-velocity on MAC grid.
 *
 * @param g Grid descriptor.
 * @param i Cell index in x.
 * @param j Cell index in y.
 * @param k Cell index in z.
 *
 * @return Linear array index for the u-face value.
 */
int vm_mac_u(const vm_grid3 g, const int i, const int j, const int k)
{
    return i + (g.nx + 1) * (j + g.ny * k);
}

/**
 * @brief Computes the linear index for the v-component of a MAC grid velocity.
 *
 * @param g Grid descriptor.
 * @param i Cell index in x.
 * @param j Cell index in y.
 * @param k Cell index in z.
 *
 * @return Linear array index for v at the given staggered location.
 */
int vm_mac_v(const vm_grid3 g, const int i, const int j, const int k)
{
    return i + g.nx * (j + (g.ny + 1) * k);
}

/**
 * @brief Compute linear index of MAC grid w-component at cell (i,j,k).
 *
 * @param g Grid descriptor.
 * @param i Cell index in x.
 * @param j Cell index in y.
 * @param k Cell index in z.
 *
 * @return Linear array index for the w velocity component.
 */
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

/**
 * @brief Assemble the SPD operator `-∇²` on a cell-centered grid.
 *
 * 5-point in 2-D (`nz == 1`), 7-point in 3-D. Dirichlet boundary cells become
 * identity rows. Homogeneous Neumann drops the missing neighbour (singular
 * constant nullspace).
 *
 * @param A  Output sparse matrix; size `vm_grid_ncells(g)`.
 * @param g  Grid dimensions and spacing.
 * @param bc Boundary condition (`VM_BC_DIRICHLET` or `VM_BC_NEUMANN`).
 * @return `true` on success, `false` on invalid input or allocation failure.
 */
bool vm_grid_laplacian(vm_spmat *A, const vm_grid3 g, const vm_bc_t bc)
{
    const int nz = (g.nz > 0) ? g.nz : 1;
    const int n = vm_grid_ncells(g);
    if (!A || n <= 0 || g.dx == VM_F(0.0) || g.dy == VM_F(0.0) || (nz > 1 && g.dz == VM_F(0.0))) {
        return false;
    }

    const int cap = n * 7;
    int *row = (int *)malloc((size_t)cap * sizeof(int));
    int *col = (int *)malloc((size_t)cap * sizeof(int));
    vm_float_t *val = (vm_float_t *)malloc((size_t)cap * sizeof(vm_float_t));

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

/**
 * @brief Cell-centered divergence of a MAC velocity field.
 *
 * `w` may be NULL when `g.nz <= 1`. `div` has length `vm_grid_ncells(g)`.
 *
 * @param div Output cell-centered divergence; length `vm_grid_ncells(g)`.
 * @param u   MAC face-centered x-velocity; length `vm_mac_nu(g)`.
 * @param v   MAC face-centered y-velocity; length `vm_mac_nv(g)`.
 * @param w   MAC face-centered z-velocity, or NULL when `g.nz <= 1`; length `vm_mac_nw(g)`.
 * @param g   Grid dimensions and spacing.
 */
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

/**
 * @brief MAC face gradient of a cell-centered scalar (pressure).
 *
 * Boundary faces are left at 0. `gw` may be NULL when `g.nz <= 1`.
 *
 * @param gu Output MAC face gradient in x; length `vm_mac_nu(g)`.
 * @param gv Output MAC face gradient in y; length `vm_mac_nv(g)`.
 * @param gw Output MAC face gradient in z, or NULL when `g.nz <= 1`; length `vm_mac_nw(g)`.
 * @param p  Cell-centered scalar field; length `vm_grid_ncells(g)`.
 * @param g  Grid dimensions and spacing.
 */
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

/**
 * @brief Cell-centered z-vorticity `(∂v/∂x − ∂u/∂y)` from MAC `u, v`.
 *
 * @param cz Output cell-centered vorticity; length `vm_grid_ncells(g)`.
 * @param u  MAC face-centered x-velocity.
 * @param v  MAC face-centered y-velocity.
 * @param g  Grid dimensions and spacing.
 */
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
