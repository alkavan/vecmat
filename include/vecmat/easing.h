// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_EASING_H
#define VECMAT_EASING_H

/**
 * @file
 * @brief Easing helper functions.
 * @ingroup vecmat_core
 */

#include "vecmat/types.h"

/**
 * @brief Damped sine used by the elastic easing curves.
 *
 * @param f Normalized time in [0, 1].
 * @param exp_mult Exponent scale on `2^(exp_mult * f)`.
 * @param sin_mult Frequency scale of the sine wave.
 * @param sin_offset Phase offset of the sine wave.
 * @param period Oscillation period.
 * @return Combined exponential-sine value.
 */
VEC_API vm_float_t elastic_oscillation(vm_float_t f, vm_float_t exp_mult,
    vm_float_t sin_mult, vm_float_t sin_offset, vm_float_t period);

/**
 * @brief Polynomial ease-in: `f^n`.
 *
 * @param f Normalized time in [0, 1].
 * @param n Polynomial degree.
 * @return Eased value.
 */
VEC_API vm_float_t poly_ease_in(vm_float_t f, vm_float_t n);

/**
 * @brief Polynomial ease-out: `1 - (1-f)^n`.
 *
 * @param f Normalized time in [0, 1].
 * @param n Polynomial degree.
 * @return Eased value.
 */
VEC_API vm_float_t poly_ease_out(vm_float_t f, vm_float_t n);

/**
 * @brief Polynomial ease-in-out of degree `n`.
 *
 * @param f Normalized time in [0, 1].
 * @param n Polynomial degree.
 * @return Eased value.
 */
VEC_API vm_float_t poly_ease_in_out(vm_float_t f, vm_float_t n);

// Quadratic easing

/**
 * @brief Quadratic ease-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t quadratic_ease_out(vm_float_t f);

/**
 * @brief Quadratic ease-in.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t quadratic_ease_in(vm_float_t f);

/**
 * @brief Quadratic ease-in-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t quadratic_ease_in_out(vm_float_t f);


// Cubic easing

/**
 * @brief Cubic ease-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t cubic_ease_out(vm_float_t f);

/**
 * @brief Cubic ease-in.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t cubic_ease_in(vm_float_t f);

/**
 * @brief Cubic ease-in-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t cubic_ease_in_out(vm_float_t f);

// Quartic easing

/**
 * @brief Quartic ease-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t quartic_ease_out(vm_float_t f);

/**
 * @brief Quartic ease-in.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t quartic_ease_in(vm_float_t f);

/**
 * @brief Quartic ease-in-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t quartic_ease_in_out(vm_float_t f);

// Quintic easing

/**
 * @brief Quintic ease-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t quintic_ease_out(vm_float_t f);

/**
 * @brief Quintic ease-in.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t quintic_ease_in(vm_float_t f);

/**
 * @brief Quintic ease-in-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t quintic_ease_in_out(vm_float_t f);

// Sine easing

/**
 * @brief Sinusoidal ease-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t sine_ease_out(vm_float_t f);

/**
 * @brief Sinusoidal ease-in.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t sine_ease_in(vm_float_t f);

/**
 * @brief Sinusoidal ease-in-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t sine_ease_in_out(vm_float_t f);

// Circular easing

/**
 * @brief Circular ease-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t circular_ease_out(vm_float_t f);

/**
 * @brief Circular ease-in.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t circular_ease_in(vm_float_t f);

/**
 * @brief Circular ease-in-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t circular_ease_in_out(vm_float_t f);

// Exponential easing

/**
 * @brief Exponential ease-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t exponential_ease_out(vm_float_t f);

/**
 * @brief Exponential ease-in.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t exponential_ease_in(vm_float_t f);

/**
 * @brief Exponential ease-in-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t exponential_ease_in_out(vm_float_t f);

// Elastic easing

/**
 * @brief Elastic ease-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t elastic_ease_out(vm_float_t f);

/**
 * @brief Elastic ease-in.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t elastic_ease_in(vm_float_t f);

/**
 * @brief Elastic ease-in-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t elastic_ease_in_out(vm_float_t f);

// Back easing

/**
 * @brief Overshooting "back" ease-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t back_ease_out(vm_float_t f);

/**
 * @brief Overshooting "back" ease-in.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t back_ease_in(vm_float_t f);

/**
 * @brief Overshooting "back" ease-in-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t back_ease_in_out(vm_float_t f);

// Bounce easing

/**
 * @brief Bounce ease-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t bounce_ease_out(vm_float_t f);

/**
 * @brief Bounce ease-in.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t bounce_ease_in(vm_float_t f);

/**
 * @brief Bounce ease-in-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t bounce_ease_in_out(vm_float_t f);

#endif //VECMAT_EASING_H
