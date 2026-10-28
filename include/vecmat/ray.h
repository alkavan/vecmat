// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_RAY_H
#define VECMAT_RAY_H

/**
 * @file
 * @brief Optical depth and Beer attenuation on one segment.
 * @ingroup vecmat_extras
 *
 * A segment, not a tracer. Transmission is \(e^{-\tau}\). Optical depth
 * above the documented cutoff returns 0 transmission without a denormal.
 */

#include "vecmat/types.h"
#include "vecmat/abi.h"

/**
 * @brief Cutoff above which transmission is returned as 0.
 *
 * `exp(-tau)` underflows for large `tau`. Values above this cutoff return
 * 0 transmission.
 */
#define VM_RAY_TAU_CUTOFF VM_F(80.0)

/**
 * @brief Optical depth of a uniform segment, \(\tau = \kappa s\).
 *
 * @param kappa Opacity.
 * @param s Segment length.
 * @return Optical depth. Negative inputs return 0.
 */
VEC_API vm_float_t vm_ray_tau(vm_float_t kappa, vm_float_t s);

/**
 * @brief Piecewise optical depth, sum of \(\kappa_k s_k\).
 *
 * @param kappa Opacity of each piece; length `n`.
 * @param s Length of each piece; length `n`.
 * @param n Number of pieces.
 * @return Sum of the piece optical depths.
 */
VEC_API vm_float_t vm_ray_tau_sum(const vm_float_t *kappa,
                                  const vm_float_t *s, int n);

/**
 * @brief Beer transmission \(e^{-\tau}\).
 *
 * @param tau Optical depth.
 * @return Transmission. `tau <= 0` returns 1. `tau` above
 *         `VM_RAY_TAU_CUTOFF` returns 0.
 */
VEC_API vm_float_t vm_ray_transmit(vm_float_t tau);

/**
 * @brief Emission integral along a uniform segment, \((1 - e^{-\tau}) B\).
 *
 * @param tau Optical depth.
 * @param B Source radiance.
 * @return Emission. A long segment approaches `B`.
 */
VEC_API vm_float_t vm_ray_emit(vm_float_t tau, vm_float_t B);

#endif //VECMAT_RAY_H
