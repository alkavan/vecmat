// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_PROJECT_H
#define VECMAT_PROJECT_H

/**
 * @file
 * @brief Projection residual for an incompressible pressure solve.
 * @ingroup vecmat_extras
 *
 * The residual is the cell-centered divergence of a MAC velocity. The
 * variable-density operator is the 5- or 7-point stencil with face weight
 * scaled by the inverse face density. Constant density 1 matches the
 * assembled grid Laplacian. The solve stays `vm_cg`; this module does not
 * solve.
 */

#include "vecmat/types.h"
#include "vecmat/abi.h"
#include "vecmat/ode.h"
#include "vecmat/sparse.h"

/**
 * @brief Projection residual \(r = \nabla\cdot v^\*\).
 *
 * Cell-centered divergence of a MAC velocity. `w` may be NULL when
 * `g.nz <= 1`. `r` has length `vm_grid_ncells(g)`.
 *
 * @param r Output residual; length `vm_grid_ncells(g)`.
 * @param u MAC face-centered x-velocity; length `vm_mac_nu(g)`.
 * @param v MAC face-centered y-velocity; length `vm_mac_nv(g)`.
 * @param w MAC face-centered z-velocity, or NULL when `g.nz <= 1`.
 * @param g Grid dimensions and spacing.
 */
VEC_API void vm_project_residual(vm_float_t *r,
                                 const vm_float_t *u,
                                 const vm_float_t *v,
                                 const vm_float_t *w,
                                 vm_grid3 g);

/**
 * @brief Fill a caller-owned CSR with \(-\nabla\cdot((1/\rho)\nabla)\).
 *
 * Same boundary treatment as `vm_grid_laplacian`: Dirichlet boundary cells
 * become identity rows, homogeneous Neumann drops the missing neighbour.
 * Face density is the arithmetic mean of the two cell densities; a domain
 * boundary face uses the cell density. Constant `rho == 1` matches the
 * assembled Laplacian. NULL `rho` means density 1 everywhere.
 *
 * @param A Output sparse matrix; size `vm_grid_ncells(g)`.
 * @param rho Cell-centered density, length `vm_grid_ncells(g)`, or NULL.
 * @param g Grid dimensions and spacing.
 * @param bc Boundary condition (`VM_BC_DIRICHLET` or `VM_BC_NEUMANN`).
 * @return false if a pointer is NULL, the grid is invalid, or a density is
 *         non-positive.
 */
VEC_API bool vm_project_laplacian(vm_spmat *A,
                                  const vm_float_t *rho,
                                  vm_grid3 g,
                                  vm_bc_t bc);

#endif //VECMAT_PROJECT_H
