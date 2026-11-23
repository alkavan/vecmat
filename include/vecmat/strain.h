// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_STRAIN_H
#define VECMAT_STRAIN_H

/**
 * @file
 * @brief Continuum strain measures. Calls polar; does not reimplement it.
 * @ingroup vecmat_extras
 *
 * Right Cauchy–Green \f$C = F^T F\f$, Green–Lagrange \f$E = \tfrac12(C - I)\f$,
 * Almansi \f$e = \tfrac12(I - b^{-1})\f$ with \f$b = F F^T\f$, and logarithmic
 * strain from the stretches of the right stretch \f$U\f$.
 */

#include "vecmat/types.h"
#include "vecmat/abi.h"

/**
 * @brief Right Cauchy–Green tensor \f$C = F^T F\f$.
 *
 * @param F Deformation gradient.
 * @return \f$C\f$. Identity if `F` is NULL.
 */
VEC_API matrix3 vm_strain_cauchy_green(const matrix3 *F);

/**
 * @brief Green–Lagrange strain \f$E = \tfrac12(C - I)\f$.
 *
 * A pure rotation yields the zero tensor.
 *
 * @param F Deformation gradient.
 * @return \f$E\f$. Zero if `F` is NULL.
 */
VEC_API matrix3 vm_strain_green_lagrange(const matrix3 *F);

/**
 * @brief Almansi (Eulerian) strain \f$e = \tfrac12(I - b^{-1})\f$, \f$b = F F^T\f$.
 *
 * @param e Output strain. Untouched on failure.
 * @param F Deformation gradient.
 * @return false if `F` is NULL or \f$b\f$ is singular.
 */
VEC_API bool vm_strain_almansi(matrix3 *e, const matrix3 *F);

/**
 * @brief Logarithmic strain \f$\ln(U)\f$ from a symmetric positive-definite \f$U\f$.
 *
 * Spectral: \f$U = Q\,\mathrm{diag}(\lambda)\,Q^T\f$ gives
 * \f$\ln(U) = Q\,\mathrm{diag}(\ln\lambda)\,Q^T\f$.
 * For \f$U = \lambda I\f$ the result is \f$\ln(\lambda)\,I\f$.
 *
 * @param log_e Output logarithmic strain.
 * @param U Right stretch (symmetric positive definite).
 * @return false if `U` is NULL or any stretch is non-positive.
 */
VEC_API bool vm_strain_logarithmic(matrix3 *log_e, const matrix3 *U);

/**
 * @brief Logarithmic strain of the right stretch of \f$F\f$.
 *
 * Calls `mat3_polar`. Fails when polar fails (reflection, non-positive stretch).
 *
 * @param log_e Output logarithmic strain.
 * @param F Deformation gradient.
 * @return false on polar failure or non-positive stretches.
 */
VEC_API bool vm_strain_logarithmic_from_grad(matrix3 *log_e, const matrix3 *F);

/**
 * @brief Quaternion from a proper rotation matrix.
 *
 * Reuses `quat_from_mat3`. Rejects improper matrices (`det <= 0`).
 *
 * @param q Output quaternion.
 * @param R Rotation matrix.
 * @return false if `R` is NULL or improper.
 */
VEC_API bool vm_strain_rotation_to_quat(quaternion *q, const matrix3 *R);

#endif /* VECMAT_STRAIN_H */
