// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_PLANCK_H
#define VECMAT_PLANCK_H

/**
 * @file
 * @brief Blackbody source and group integrals.
 * @ingroup vecmat_extras
 *
 * The integrated blackbody is the isotropic radiance
 * \(B(T) = \sigma T^4 / \pi\), which equals \(a c T^4 / 4\pi\).
 * A group with `nu_hi <= 0` spans the whole spectrum and returns that
 * integral. `T <= 0` returns `ok = false`.
 */

#include "vecmat/types.h"
#include "vecmat/abi.h"

/**
 * @brief Integrated blackbody radiance \(B(T) = \sigma T^4 / \pi\).
 *
 * @param T Temperature.
 * @param ok Set false when `T <= 0`; may be NULL.
 * @return Radiance, or 0 when `T <= 0`.
 */
VEC_API vm_float_t vm_planck_b(vm_float_t T, bool *ok);

/**
 * @brief Spectral blackbody radiance \(B_\nu(T)\).
 *
 * @param nu Frequency.
 * @param T Temperature.
 * @param ok Set false when `T <= 0`; may be NULL.
 * @return Spectral radiance, or 0 when `T <= 0` or `nu <= 0`.
 */
VEC_API vm_float_t vm_planck_bnu(vm_float_t nu, vm_float_t T, bool *ok);

/**
 * @brief Group integral of \(B_\nu\) between two frequencies.
 *
 * `nu_hi <= 0` means the upper limit is infinity and returns the analytic
 * integrated radiance. A finite band uses a fixed Gauss quadrature in the
 * dimensionless frequency.
 *
 * @param nu_lo Lower frequency bound.
 * @param nu_hi Upper frequency bound, or `<= 0` for infinity.
 * @param T Temperature.
 * @param ok Set false when `T <= 0` or the band is empty; may be NULL.
 * @return Group radiance.
 */
VEC_API vm_float_t vm_planck_group(vm_float_t nu_lo, vm_float_t nu_hi,
                                   vm_float_t T, bool *ok);

/**
 * @brief Temperature derivative \(dB/dT = 4 \sigma T^3 / \pi\).
 *
 * @param T Temperature.
 * @param ok Set false when `T <= 0`; may be NULL.
 * @return Derivative, or 0 when `T <= 0`.
 */
VEC_API vm_float_t vm_planck_db_dt(vm_float_t T, bool *ok);

#endif //VECMAT_PLANCK_H
