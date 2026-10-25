// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_HYDRO_H
#define VECMAT_HYDRO_H

/**
 * @file
 * @brief One-cell hydro primitives. No grid loop.
 * @ingroup vecmat_extras
 *
 * Viscous stress, von Neumann–Richtmyer artificial viscosity, a Rusanov
 * face flux, and a cell CFL. Sound speed comes from the equation of state.
 */

#include "vecmat/types.h"
#include "vecmat/abi.h"
#include "vecmat/sym3.h"

/**
 * @brief Primitive state on one side of a face.
 *
 * `e` is specific internal energy. Total energy is `e + 0.5 vn * vn`.
 */
typedef struct {
    vm_float_t rho;  /**< Density. */
    vm_float_t vn;   /**< Velocity normal to the face. */
    vm_float_t e;    /**< Specific internal energy. */
    vm_float_t p;    /**< Pressure. */
    vm_float_t c;    /**< Sound speed. */
} vm_hydro_state;

/**
 * @brief Conservative face flux `(mass, normal momentum, energy)`.
 */
typedef struct {
    vm_float_t mass;      /**< Mass flux. */
    vm_float_t momentum;  /**< Normal-momentum flux. */
    vm_float_t energy;    /**< Total-energy flux. */
} vm_hydro_flux;

/**
 * @brief Newtonian viscous stress \(\sigma = 2\mu D + \lambda (\operatorname{tr} D)\,I\).
 *
 * A pure spin has \(D = 0\), so the stress is zero.
 *
 * @param D Rate-of-deformation, Voigt-6.
 * @param mu Dynamic viscosity.
 * @param lambda Second viscosity.
 * @return Viscous stress as Voigt-6.
 */
VEC_API vm_sym3 vm_hydro_viscous(const vm_sym3 *D, vm_float_t mu, vm_float_t lambda);

/**
 * @brief von Neumann–Richtmyer artificial viscosity.
 *
 * \(q = c_0 \rho (\Delta v)^2\) in compression (\(\Delta v < 0\)), zero in
 * expansion. No clamping.
 *
 * @param rho Density.
 * @param delta_v Velocity jump. Negative is compression.
 * @param c0 Coefficient.
 * @return Artificial viscosity. Zero if expanding or density is non-positive.
 */
VEC_API vm_float_t vm_hydro_q(vm_float_t rho, vm_float_t delta_v, vm_float_t c0);

/**
 * @brief Rusanov flux between two face states.
 *
 * Spectral radius is \(\max(|v_n| + c)\) of the two sides. A symmetric
 * pair (equal density, pressure, sound speed, opposite normal velocity)
 * has zero mass flux.
 *
 * @param flux Output flux.
 * @param left Left state.
 * @param right Right state.
 * @return false if any pointer is NULL.
 */
VEC_API bool vm_hydro_rusanov(vm_hydro_flux *flux,
                              const vm_hydro_state *left,
                              const vm_hydro_state *right);

/**
 * @brief Cell CFL time step.
 *
 * \(\Delta t = \kappa h / (|v| + c)\), clamped by the viscous scale
 * \(h^2 / \nu\) when \(\nu > 0\).
 *
 * @param h Cell size.
 * @param v Velocity magnitude.
 * @param c Sound speed.
 * @param kappa CFL number.
 * @param nu Kinematic viscosity. Non-positive skips the viscous clamp.
 * @return Time step, or 0 if the inputs cannot produce one.
 */
VEC_API vm_float_t vm_hydro_cfl(vm_float_t h, vm_float_t v, vm_float_t c,
                                vm_float_t kappa, vm_float_t nu);

#endif //VECMAT_HYDRO_H
