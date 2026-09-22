// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_ODE_H
#define VECMAT_ODE_H

/**
 * @file
 * @brief Time integrators, rigid-body helpers, and regular-grid / MAC operators. Primitives, not a simulator.
 * @ingroup vecmat_extras
 */

#include "vecmat/types.h"
#include "vecmat/abi.h"

/**
 * @brief Semi-implicit Euler: `v += a dt`, then `x += v dt`.
 *
 * @param x  Position vector (in/out); length `n`.
 * @param v  Velocity vector (in/out); length `n`.
 * @param a  Acceleration vector; length `n`.
 * @param n  State dimension.
 * @param dt Timestep.
 */
VEC_API void vm_euler_semi(vm_float_t *x, vm_float_t *v, const vm_float_t *a, int n, vm_float_t dt);

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
VEC_API void vm_verlet(vm_acc_fn acc, vm_float_t *x, vm_float_t *v, vm_float_t *a, int n, vm_float_t dt, void *ctx);

/**
 * @brief Explicit midpoint RK2 for `y' = f(y)`.
 *
 * @param f   ODE right-hand side `f(y, dy, ctx)`.
 * @param y   State vector (in/out); length `n`.
 * @param n   State dimension.
 * @param dt  Timestep.
 * @param ctx User context passed to `f`.
 */
VEC_API void vm_rk2(vm_ode_fn f, vm_float_t *y, int n, vm_float_t dt, void *ctx);

/**
 * @brief Classic RK4 for `y' = f(y)`.
 *
 * @param f   ODE right-hand side `f(y, dy, ctx)`.
 * @param y   State vector (in/out); length `n`.
 * @param n   State dimension.
 * @param dt  Timestep.
 * @param ctx User context passed to `f`.
 */
VEC_API void vm_rk4(vm_ode_fn f, vm_float_t *y, int n, vm_float_t dt, void *ctx);

/**
 * @brief CFL timestep `dt = cfl * dx / (|u| + ε)`.
 *
 * @param cfl   CFL number (typically in `(0, 1]`).
 * @param dx    Characteristic cell size.
 * @param speed Characteristic speed (e.g. `|u|`).
 * @return Stable timestep estimate.
 */
VEC_API vm_float_t vm_cfl_dt(vm_float_t cfl, vm_float_t dx, vm_float_t speed);

/**
 * @brief 3×3 Cholesky `A = L Lᵀ`. `L` is lower; the upper triangle is zeroed.
 *
 * @param a SPD coefficient matrix.
 * @param L Lower-triangular Cholesky factor (out); upper triangle set to 0.
 * @return `true` on success, `false` if `L` is NULL or `a` is not SPD.
 */
VEC_API bool mat3_chol(matrix3 a, matrix3 *L);

/**
 * @brief Solve the 3×3 SPD system `A x = b` via Cholesky.
 *
 * @param a SPD coefficient matrix.
 * @param b Right-hand side vector.
 * @param x Solution vector (out).
 * @return `true` on success, `false` if `x` is NULL or factorization fails.
 */
VEC_API bool mat3_spd_solve(matrix3 a, vector3 b, vector3 *x);

/**
 * @brief World-frame inertia `I_w = R I_b Rᵀ` from a body tensor and orientation.
 *
 * @param ib  Body-frame inertia tensor.
 * @param q   Orientation quaternion.
 * @return World-frame inertia tensor.
 */
VEC_API matrix3 vm_inertia_world(matrix3 ib, quaternion q);

/**
 * @brief Recover `ω` from angular momentum `L = I ω`.
 *
 * @param I Inertia tensor (same frame as `L`).
 * @param L Angular momentum.
 * @return Angular velocity, or the zero vector if the solve fails.
 */
VEC_API vector3 vm_omega_from_angmom(matrix3 I, vector3 L);

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
VEC_API vm_float_t vm_rigid_energy(vm_float_t mass, vector3 v, matrix3 I, vector3 w);

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
VEC_API void vm_rigid_step(vector3 *x, vector3 *v, quaternion *q, vector3 *w, vector3 F,
    vector3 tau, vm_float_t mass, matrix3 I_body, vm_float_t dt);

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
VEC_API void vm_baumgarte_correct(vector3 *x, vector3 *v, vector3 n, vm_float_t C,
    vm_float_t beta, vm_float_t gamma, vm_float_t dt);

/*******************************************************************************
 * Regular grid operators (MAC + assembled Laplacian)
 ******************************************************************************/

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
VEC_API vm_grid3 vm_grid3_make(int nx, int ny, int nz, vm_float_t dx, vm_float_t dy, vm_float_t dz);

/**
 * @brief Returns the total number of cells in the grid.
 *
 * @param g 3D grid descriptor.
 * @return Total cell count (`nx * ny * nz`), or `0` if `nx` or `ny` is non-positive.
 */
VEC_API int vm_grid_ncells(vm_grid3 g);

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
VEC_API int vm_grid_cell(vm_grid3 g, int i, int j, int k);

/**
 * @brief Returns the number of u-velocity MAC face values for the grid.
 *
 * @param g Grid definition.
 * @return Number of u-faces, or zero if the grid is invalid.
 */
VEC_API int vm_mac_nu(vm_grid3 g);

/**
 * @brief Returns the number of MAC grid v-velocity components.
 *
 * @param g Grid dimensions and spacing.
 * @return Number of v-velocity samples.
 */
VEC_API int vm_mac_nv(vm_grid3 g);

/**
 * @brief Returns the number of MAC grid faces in the z (vertical) direction.
 *
 * @param g Grid dimensions and spacing.
 * @return Number of vertical MAC faces, or 0 if the grid is invalid.
 */
VEC_API int vm_mac_nw(vm_grid3 g);

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
VEC_API int vm_mac_u(vm_grid3 g, int i, int j, int k);

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
VEC_API int vm_mac_v(vm_grid3 g, int i, int j, int k);

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
VEC_API int vm_mac_w(vm_grid3 g, int i, int j, int k);

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
VEC_API bool vm_grid_laplacian(vm_spmat *A, vm_grid3 g, vm_bc_t bc);

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
VEC_API void vm_mac_div(vm_float_t *div, const vm_float_t *u, const vm_float_t *v, const vm_float_t *w, vm_grid3 g);

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
VEC_API void vm_mac_grad(vm_float_t *gu, vm_float_t *gv, vm_float_t *gw, const vm_float_t *p, vm_grid3 g);

/**
 * @brief Cell-centered z-vorticity `(∂v/∂x − ∂u/∂y)` from MAC `u, v`.
 *
 * @param cz Output cell-centered vorticity; length `vm_grid_ncells(g)`.
 * @param u  MAC face-centered x-velocity.
 * @param v  MAC face-centered y-velocity.
 * @param g  Grid dimensions and spacing.
 */
VEC_API void vm_mac_curl_z(vm_float_t *cz, const vm_float_t *u, const vm_float_t *v, vm_grid3 g);

#endif //VECMAT_ODE_H
