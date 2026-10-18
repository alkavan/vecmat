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
 * Right Cauchy–Green \(C = F^T F\), Green–Lagrange \(E = \tfrac12(C - I)\),
 * Almansi \(e = \tfrac12(I - b^{-1})\) with \(b = F F^T\), and logarithmic
 * strain from the stretches of the right stretch \(U\).
 */

#include "vecmat/types.h"
#include "vecmat/abi.h"

/**
 * @brief Right Cauchy–Green tensor \(C = F^T F\).
 *
 * @param F Deformation gradient.
 * @return \(C\). Identity if `F` is NULL.
 */
VEC_API matrix3 vm_strain_cauchy_green(const matrix3 *F);

/**
 * @brief Green–Lagrange strain \(E = \tfrac12(C - I)\).
 *
 * A pure rotation yields the zero tensor.
 *
 * @param F Deformation gradient.
 * @return \(E\). Zero if `F` is NULL.
 */
VEC_API matrix3 vm_strain_green_lagrange(const matrix3 *F);

/**
 * @brief Almansi (Eulerian) strain \(e = \tfrac12(I - b^{-1})\), \(b = F F^T\).
 *
 * @param e Output strain. Untouched on failure.
 * @param F Deformation gradient.
 * @return false if `F` is NULL or \(b\) is singular.
 */
VEC_API bool vm_strain_almansi(matrix3 *e, const matrix3 *F);

/**
 * @brief Logarithmic strain \(\ln(U)\) from a symmetric positive-definite \(U\).
 *
 * Spectral: \(U = Q\,\mathrm{diag}(\lambda)\,Q^T\) gives
 * \(\ln(U) = Q\,\mathrm{diag}(\ln\lambda)\,Q^T\).
 * For \(U = \lambda I\) the result is \(\ln(\lambda)\,I\).
 *
 * @param log_e Output logarithmic strain.
 * @param U Right stretch (symmetric positive definite).
 * @return false if `U` is NULL or any stretch is non-positive.
 */
VEC_API bool vm_strain_logarithmic(matrix3 *log_e, const matrix3 *U);

/**
 * @brief Logarithmic strain of the right stretch of \(F\).
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

#endif //VECMAT_STRAIN_H
