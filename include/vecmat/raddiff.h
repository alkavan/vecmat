// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_RADDIFF_H
#define VECMAT_RADDIFF_H

/**
 * @file
 * @brief Rosseland diffusion coefficient and flux limiter.
 * @ingroup vecmat_extras
 *
 * The diffusion coefficient is \f$D = 1/(3\kappa_R)\f$. The Levermore–Pomraning
 * limiter is \f$\lambda(R) = (\coth R - 1/R)/R\f$ with \f$R = |\nabla E|/(\kappa E)\f$.
 * In the optically thick limit \f$\lambda(0) = 1/3\f$, and \f$\lambda\f$ stays in
 * \f$(0, 1/3]\f$. The limited coefficient is \f$D\lambda(R)\f$.
 *
 * The variable-D operator fills a caller-owned CSR. Constant \f$D = 1\f$ matches
 * the assembled grid Laplacian. The solve stays `vm_cg`.
 */

#include "vecmat/types.h"
#include "vecmat/abi.h"
#include "vecmat/ode.h"
#include "vecmat/sparse.h"

/**
 * @brief Rosseland diffusion coefficient \f$D = 1/(3\kappa_R)\f$.
 *
 * @param D Output coefficient; length `n`.
 * @param kappa Rosseland opacity; length `n`.
 * @param n Number of cells.
 * @return false if a pointer is NULL or any opacity is non-positive.
 */
VEC_API bool vm_raddiff_d(vm_float_t *D, const vm_float_t *kappa, int n);

/**
 * @brief Levermore–Pomraning flux limiter \f$\lambda(R)\f$.
 *
 * \f$\lambda(0) = 1/3\f$ and \f$\lambda\f$ decreases toward 0 as \f$R\f$ grows.
 *
 * @param R Dimensionless gradient \f$R = |\nabla E|/(\kappa E)\f$. Negative
 *          values are taken in absolute value.
 * @return Limiter in \f$(0, 1/3]\f$.
 */
VEC_API vm_float_t vm_raddiff_limiter(vm_float_t R);

/**
 * @brief Dimensionless gradient \f$R = |\nabla E|/(\kappa E)\f$.
 *
 * @param grad_e Magnitude of the energy gradient.
 * @param kappa Rosseland opacity.
 * @param E Energy density.
 * @return R, or 0 if the denominator is non-positive.
 */
VEC_API vm_float_t vm_raddiff_ratio(vm_float_t grad_e, vm_float_t kappa,
                                    vm_float_t E);

/**
 * @brief Gray emission–absorption residual \f$\kappa_a (E - a T^4)\f$.
 *
 * @param r Output residual; length `n`.
 * @param kappa_a Absorption opacity; length `n`.
 * @param E Energy density; length `n`.
 * @param aT4 Radiation constant times \f$T^4\f$; length `n`.
 * @param n Number of cells.
 * @return false if a pointer is NULL.
 */
VEC_API bool vm_raddiff_residual(vm_float_t *r,
                                 const vm_float_t *kappa_a,
                                 const vm_float_t *E,
                                 const vm_float_t *aT4,
                                 int n);

/**
 * @brief Fill a caller-owned CSR with \f$-\nabla\cdot(D\nabla)\f$.
 *
 * Same boundary treatment as `vm_grid_laplacian`. Face coefficient is the
 * arithmetic mean of the two cell values; a domain boundary face uses the
 * cell value. Constant `D == 1` matches the assembled Laplacian. NULL `D`
 * means coefficient 1 everywhere.
 *
 * @param A Output sparse matrix; size `vm_grid_ncells(g)`.
 * @param D Cell-centered diffusion coefficient, length `vm_grid_ncells(g)`,
 *          or NULL.
 * @param g Grid dimensions and spacing.
 * @param bc Boundary condition (`VM_BC_DIRICHLET` or `VM_BC_NEUMANN`).
 * @return false if a pointer is NULL, the grid is invalid, or a coefficient
 *         is non-positive.
 */
VEC_API bool vm_raddiff_laplacian(vm_spmat *A,
                                  const vm_float_t *D,
                                  vm_grid3 g,
                                  vm_bc_t bc);

#endif //VECMAT_RADDIFF_H
