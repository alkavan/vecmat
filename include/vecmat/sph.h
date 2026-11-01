// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_SPH_H
#define VECMAT_SPH_H

/**
 * @file
 * @brief SPH kernels, density sum, and symmetric pressure gradient.
 * @ingroup vecmat_extras
 *
 * Cubic spline and Wendland C2, both supported on \(r < 2h\) and normalized
 * so the integral is 1. The neighbor list is an array of indices. There is
 * no search structure here; a caller bins with the `geom3` cell index and an
 * AABB.
 */

#include "vecmat/types.h"
#include "vecmat/abi.h"

/**
 * @brief Kernel selector.
 */
typedef enum {
    VM_SPH_CUBIC = 0,     /**< Cubic spline. */
    VM_SPH_WENDLAND = 1   /**< Wendland C2. */
} vm_sph_kernel_t;

/**
 * @brief Kernel value \(W(r, h)\).
 *
 * @param r Separation.
 * @param h Smoothing length.
 * @param dim Dimension, 1, 2, or 3.
 * @param kernel `VM_SPH_CUBIC` or `VM_SPH_WENDLAND`.
 * @return Kernel value. Zero outside the support or on invalid input.
 */
VEC_API vm_float_t vm_sph_w(vm_float_t r, vm_float_t h, int dim, int kernel);

/**
 * @brief Radial derivative \(dW/dr\).
 *
 * @param r Separation.
 * @param h Smoothing length.
 * @param dim Dimension, 1, 2, or 3.
 * @param kernel `VM_SPH_CUBIC` or `VM_SPH_WENDLAND`.
 * @return Radial derivative. Zero at \(r = 0\).
 */
VEC_API vm_float_t vm_sph_dw(vm_float_t r, vm_float_t h, int dim, int kernel);

/**
 * @brief Density sum \(\rho_i = m_i W(0, h) + \sum_j m_j W_{ij}\).
 *
 * An empty neighbor list returns the self term only. `z` may be NULL in 1-D
 * and 2-D.
 *
 * @param x Particle x coordinates; length at least the indices used.
 * @param y Particle y coordinates.
 * @param z Particle z coordinates, or NULL below 3-D.
 * @param m Particle masses.
 * @param i Index of the particle being summed.
 * @param neighbors Neighbor indices; length `nn`.
 * @param nn Number of neighbors.
 * @param h Smoothing length.
 * @param dim Dimension, 1, 2, or 3.
 * @param kernel Kernel selector.
 * @return Density. Zero on invalid input.
 */
VEC_API vm_float_t vm_sph_density(const vm_float_t *x, const vm_float_t *y,
                                  const vm_float_t *z, const vm_float_t *m,
                                  int i, const int *neighbors, int nn,
                                  vm_float_t h, int dim, int kernel);

/**
 * @brief Symmetric pressure gradient.
 *
 * \(\sum_j m_j (p_i/\rho_i^2 + p_j/\rho_j^2) \nabla W_{ij}\).
 *
 * @param gx Output x component.
 * @param gy Output y component.
 * @param gz Output z component; may be NULL below 3-D.
 * @param x Particle x coordinates.
 * @param y Particle y coordinates.
 * @param z Particle z coordinates, or NULL below 3-D.
 * @param m Particle masses.
 * @param rho Particle densities.
 * @param p Particle pressures.
 * @param i Index of the particle.
 * @param neighbors Neighbor indices; length `nn`.
 * @param nn Number of neighbors.
 * @param h Smoothing length.
 * @param dim Dimension, 1, 2, or 3.
 * @param kernel Kernel selector.
 * @return false if a required pointer is NULL or a density is non-positive.
 */
VEC_API bool vm_sph_pressure_grad(vm_float_t *gx, vm_float_t *gy, vm_float_t *gz,
                                  const vm_float_t *x, const vm_float_t *y,
                                  const vm_float_t *z, const vm_float_t *m,
                                  const vm_float_t *rho, const vm_float_t *p,
                                  int i, const int *neighbors, int nn,
                                  vm_float_t h, int dim, int kernel);

#endif //VECMAT_SPH_H
