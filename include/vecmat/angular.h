// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_ANGULAR_H
#define VECMAT_ANGULAR_H

/**
 * @file
 * @brief Level-symmetric sphere quadrature and phase functions.
 * @ingroup vecmat_extras
 *
 * Static tables for \f$S_2, S_4, S_6, S_8\f$. Weights sum to \f$4\pi\f$.
 * Nodes lie on the unit sphere. The isotropic phase is \f$1/(4\pi)\f$;
 * Henyey–Greenstein at \f$g = 0\f$ matches it.
 */

#include "vecmat/types.h"
#include "vecmat/abi.h"

/**
 * @brief Number of directions in the \f$S_N\f$ set.
 *
 * @param n Order; one of 2, 4, 6, 8.
 * @return Direction count, or 0 if `n` is not supported.
 */
VEC_API int vm_angular_count(int n);

/**
 * @brief Fill caller-owned arrays with the \f$S_N\f$ nodes and weights.
 *
 * Each of `mu`, `eta`, `xi`, `w` has length at least `vm_angular_count(n)`.
 *
 * @param n Order; one of 2, 4, 6, 8.
 * @param mu Output direction cosine in x.
 * @param eta Output direction cosine in y.
 * @param xi Output direction cosine in z.
 * @param w Output weight.
 * @return Number of directions written, or 0 on invalid input.
 */
VEC_API int vm_angular_sn(int n, vm_float_t *mu, vm_float_t *eta,
                          vm_float_t *xi, vm_float_t *w);

/**
 * @brief Isotropic phase function \f$1/(4\pi)\f$.
 *
 * @return Phase value.
 */
VEC_API vm_float_t vm_angular_isotropic(void);

/**
 * @brief Henyey–Greenstein phase \f$P(\mu; g)\f$.
 *
 * @param mu Cosine of the scattering angle.
 * @param g Asymmetry parameter in \f$[-1, 1]\f$.
 * @return Phase value. `g = 0` matches the isotropic phase.
 */
VEC_API vm_float_t vm_angular_hg(vm_float_t mu, vm_float_t g);

/**
 * @brief Scattering source at one direction.
 *
 * \f$\sum_m w_m P(\boldsymbol{\Omega}_m\cdot\boldsymbol{\Omega}) I_m\f$.
 * `g = 0` uses the isotropic phase.
 *
 * @param mu Ordinate cosines in x; length `n`.
 * @param eta Ordinate cosines in y; length `n`.
 * @param xi Ordinate cosines in z; length `n`.
 * @param w Ordinate weights; length `n`.
 * @param I Intensity at each ordinate; length `n`.
 * @param n Number of ordinates.
 * @param ox Target direction cosine in x.
 * @param oy Target direction cosine in y.
 * @param oz Target direction cosine in z.
 * @param g Asymmetry parameter.
 * @return Scattering source.
 */
VEC_API vm_float_t vm_angular_scatter(const vm_float_t *mu,
                                      const vm_float_t *eta,
                                      const vm_float_t *xi,
                                      const vm_float_t *w,
                                      const vm_float_t *I,
                                      int n,
                                      vm_float_t ox, vm_float_t oy, vm_float_t oz,
                                      vm_float_t g);

#endif /* VECMAT_ANGULAR_H */
