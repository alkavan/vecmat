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
 * so the integral is 1. Neighbor lists are arrays of indices. A uniform
 * linked-cell bin and a CSR neighbor build sit next to the kernels; the
 * caller owns every buffer.
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

/**
 * @brief Uniform cell grid for SPH neighbor search.
 *
 * Origin is the lower corner. `cell` should be the kernel support (`2h`).
 * `ny == nz == 1` is 1-D; `nz == 1` is 2-D. Cell `(i, j, k)` covers
 * `[x0 + i cell, x0 + (i+1) cell)` and the same in y and z. A particle on
 * the far boundary is assigned to the last cell.
 */
typedef struct {
    vm_float_t x0;    /**< Lower corner, x. */
    vm_float_t y0;    /**< Lower corner, y. */
    vm_float_t z0;    /**< Lower corner, z. */
    vm_float_t cell;  /**< Cell size. */
    int nx;           /**< Cells in x. */
    int ny;           /**< Cells in y. */
    int nz;           /**< Cells in z. */
} vm_sph_grid;

/**
 * @brief Number of cells, `nx * ny * nz`.
 *
 * @return 0 if the grid is NULL or a dimension is non-positive.
 */
VEC_API int vm_sph_ncells(const vm_sph_grid *g);

/**
 * @brief Fit a grid that covers the particles.
 *
 * Origin is the minimum coordinate. Each axis has at least one cell.
 * Unused axes (`dim < 2` for y, `dim < 3` for z) are a single cell at 0.
 *
 * @param g Output grid.
 * @param x Particle x coordinates; length `n`.
 * @param y Particle y coordinates, or NULL below 2-D.
 * @param z Particle z coordinates, or NULL below 3-D.
 * @param n Number of particles.
 * @param cell Cell size. Must be positive.
 * @param dim Dimension, 1, 2, or 3.
 * @return false on invalid input.
 */
VEC_API bool vm_sph_grid_fit(vm_sph_grid *g,
                             const vm_float_t *x, const vm_float_t *y,
                             const vm_float_t *z, int n,
                             vm_float_t cell, int dim);

/**
 * @brief Exclusive prefix sum.
 *
 * `offsets[0] = 0`, `offsets[i+1] = offsets[i] + counts[i]`.
 *
 * @param offsets Output; length `n + 1`.
 * @param counts Input counts; length `n`.
 * @param n Number of counts.
 * @return `offsets[n]`, or -1 on invalid input.
 */
VEC_API int vm_sph_prefix(int *offsets, const int *counts, int n);

/**
 * @brief Bin particles into cells.
 *
 * `cell_of[i]` is the linear cell of particle `i`, or -1 if it is outside
 * the grid. `cell_offsets` / `cell_particles` is the CSR of particles per
 * cell: cell `c` holds `cell_particles[cell_offsets[c] .. cell_offsets[c+1])`,
 * in increasing particle index. Particles outside the grid are omitted.
 *
 * @param cell_of Output cell per particle; length `n`.
 * @param cell_offsets Output; length `vm_sph_ncells(g) + 1`.
 * @param cell_particles Output; length `n`.
 * @return false on invalid input.
 */
VEC_API bool vm_sph_bin(int *cell_of, int *cell_offsets, int *cell_particles,
                        const vm_float_t *x, const vm_float_t *y,
                        const vm_float_t *z, int n,
                        const vm_sph_grid *g, int dim);

/**
 * @brief Count neighbors inside the support, excluding self.
 *
 * A neighbor is a different particle in the 3^dim stencil of cells with
 * separation strictly less than `support`. Particles outside the grid have
 * a count of 0.
 *
 * @param nn Output counts; length `n`.
 * @param support Kernel support, typically `2h`.
 * @return Total neighbor count, or -1 on invalid input.
 */
VEC_API int vm_sph_neighbor_count(int *nn,
                                  const vm_float_t *x, const vm_float_t *y,
                                  const vm_float_t *z, int n,
                                  const vm_sph_grid *g,
                                  const int *cell_offsets,
                                  const int *cell_particles,
                                  vm_float_t support, int dim);

/**
 * @brief Fill CSR neighbor lists.
 *
 * `offsets` is the exclusive prefix of the counts from
 * `vm_sph_neighbor_count`. `neighbors` has length `offsets[n]`. Self is
 * excluded. Order is cell-stencil order, particles within a cell in index
 * order.
 *
 * @param neighbors Output indices; length `offsets[n]`.
 * @param offsets Exclusive prefix; length `n + 1`.
 * @return false on invalid input.
 */
VEC_API bool vm_sph_neighbor_fill(int *neighbors, const int *offsets,
                                  const vm_float_t *x, const vm_float_t *y,
                                  const vm_float_t *z, int n,
                                  const vm_sph_grid *g,
                                  const int *cell_offsets,
                                  const int *cell_particles,
                                  vm_float_t support, int dim);

#endif //VECMAT_SPH_H
