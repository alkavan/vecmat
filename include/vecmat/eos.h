// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_EOS_H
#define VECMAT_EOS_H

/**
 * @file
 * @brief Local equations of state. Two scalars in, pressure and sound speed out.
 * @ingroup vecmat_extras
 *
 * Each call returns `ok = false` on non-positive density or on a negative
 * \(c^2\). No clamping. f64 is the contract; f32 uses the same formula.
 */

#include "vecmat/types.h"
#include "vecmat/abi.h"

/**
 * @brief Pressure and sound speed from an equation of state.
 */
typedef struct {
    bool ok;           /**< false on bad density or negative \(c^2\). */
    vm_float_t p;      /**< Pressure. */
    vm_float_t c;      /**< Sound speed. */
} vm_eos_state;

/**
 * @brief Ideal gas: \(p = (\gamma - 1)\rho e\), \(c = \sqrt{\gamma p / \rho}\).
 */
typedef struct {
    vm_float_t gamma;  /**< Ratio of specific heats, must be > 1. */
} vm_ideal_gas;

/**
 * @brief Stiffened gas: \(p = (\gamma - 1)\rho e - \gamma p_\infty\).
 *
 * Sound speed from the same derivative: \(c = \sqrt{\gamma (p + p_\infty) / \rho}\).
 */
typedef struct {
    vm_float_t gamma;  /**< Ratio of specific heats, must be > 1. */
    vm_float_t p_inf;  /**< Stiffening pressure \(p_\infty\). */
} vm_stiff_gas;

/**
 * @brief Mie–Grüneisen on a linear Hugoniot \(U_s = c_0 + s U_p\).
 */
typedef struct {
    vm_float_t rho0;    /**< Reference density. */
    vm_float_t c0;      /**< Bulk sound speed. */
    vm_float_t s;       /**< Hugoniot slope. */
    vm_float_t gamma0;  /**< Grüneisen coefficient \(\Gamma_0\). */
} vm_mie;

/**
 * @brief Jones–Wilkins–Lee. \(V = \rho_0 / \rho\).
 *
 * \(p = A(1 - \omega/(R_1 V))e^{-R_1 V} + B(1 - \omega/(R_2 V))e^{-R_2 V} + \omega \rho_0 e / V\).
 */
typedef struct {
    vm_float_t rho0;   /**< Reference density. */
    vm_float_t a;      /**< \(A\). */
    vm_float_t b;      /**< \(B\). */
    vm_float_t r1;     /**< \(R_1\). */
    vm_float_t r2;     /**< \(R_2\). */
    vm_float_t omega;  /**< \(\omega\). */
} vm_jwl;

/**
 * @brief Ideal-gas pressure and sound speed.
 *
 * @return false if density is non-positive, gamma is not > 1, or \(c^2 < 0\).
 */
VEC_API bool vm_eos_ideal(vm_eos_state *out, const vm_ideal_gas *eos,
                          vm_float_t rho, vm_float_t e);

/**
 * @brief Stiffened-gas pressure and sound speed.
 *
 * @return false if density is non-positive, gamma is not > 1, or \(c^2 < 0\).
 */
VEC_API bool vm_eos_stiffened(vm_eos_state *out, const vm_stiff_gas *eos,
                              vm_float_t rho, vm_float_t e);

/**
 * @brief Mie–Grüneisen pressure and sound speed on a linear Hugoniot.
 *
 * @return false if density or reference density is non-positive, or \(c^2 < 0\).
 */
VEC_API bool vm_eos_mie(vm_eos_state *out, const vm_mie *eos,
                        vm_float_t rho, vm_float_t e);

/**
 * @brief Jones–Wilkins–Lee pressure and sound speed.
 *
 * @return false if density or reference density is non-positive, or \(c^2 < 0\).
 */
VEC_API bool vm_eos_jwl(vm_eos_state *out, const vm_jwl *eos,
                        vm_float_t rho, vm_float_t e);

#endif //VECMAT_EOS_H
