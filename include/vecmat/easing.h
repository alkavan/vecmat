// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_EASING_H
#define VECMAT_EASING_H

/**
 * @file
 * @brief Easing helper functions. Public names are `vm_*`.
 *
 * The unsuffixed names are deprecated wrappers for this 0.5 release.
 * Call `vm_smoothstep`, not `smoothstep`.
 * @ingroup vecmat_core
 */

#include "vecmat/types.h"
#include "vecmat/abi.h"

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
VEC_API vm_float_t vm_elastic_oscillation(vm_float_t f, vm_float_t exp_mult,
    vm_float_t sin_mult, vm_float_t sin_offset, vm_float_t period);

/** @brief Deprecated. Use `vm_elastic_oscillation`. */
VEC_API VM_DEPRECATED("use vm_elastic_oscillation") vm_float_t elastic_oscillation(vm_float_t f, vm_float_t exp_mult,
    vm_float_t sin_mult, vm_float_t sin_offset, vm_float_t period);

/**
 * @brief Polynomial ease-in: `f^n`.
 *
 * @param f Normalized time in [0, 1].
 * @param n Polynomial degree.
 * @return Eased value.
 */
VEC_API vm_float_t vm_poly_ease_in(vm_float_t f, vm_float_t n);

/** @brief Deprecated. Use `vm_poly_ease_in`. */
VEC_API VM_DEPRECATED("use vm_poly_ease_in") vm_float_t poly_ease_in(vm_float_t f, vm_float_t n);

/**
 * @brief Polynomial ease-out: `1 - (1-f)^n`.
 *
 * @param f Normalized time in [0, 1].
 * @param n Polynomial degree.
 * @return Eased value.
 */
VEC_API vm_float_t vm_poly_ease_out(vm_float_t f, vm_float_t n);

/** @brief Deprecated. Use `vm_poly_ease_out`. */
VEC_API VM_DEPRECATED("use vm_poly_ease_out") vm_float_t poly_ease_out(vm_float_t f, vm_float_t n);

/**
 * @brief Polynomial ease-in-out of degree `n`.
 *
 * @param f Normalized time in [0, 1].
 * @param n Polynomial degree.
 * @return Eased value.
 */
VEC_API vm_float_t vm_poly_ease_in_out(vm_float_t f, vm_float_t n);

/** @brief Deprecated. Use `vm_poly_ease_in_out`. */
VEC_API VM_DEPRECATED("use vm_poly_ease_in_out") vm_float_t poly_ease_in_out(vm_float_t f, vm_float_t n);

// Quadratic easing

/**
 * @brief Quadratic ease-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t vm_quadratic_ease_out(vm_float_t f);

/** @brief Deprecated. Use `vm_quadratic_ease_out`. */
VEC_API VM_DEPRECATED("use vm_quadratic_ease_out") vm_float_t quadratic_ease_out(vm_float_t f);

/**
 * @brief Quadratic ease-in.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t vm_quadratic_ease_in(vm_float_t f);

/** @brief Deprecated. Use `vm_quadratic_ease_in`. */
VEC_API VM_DEPRECATED("use vm_quadratic_ease_in") vm_float_t quadratic_ease_in(vm_float_t f);

/**
 * @brief Quadratic ease-in-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t vm_quadratic_ease_in_out(vm_float_t f);

/** @brief Deprecated. Use `vm_quadratic_ease_in_out`. */
VEC_API VM_DEPRECATED("use vm_quadratic_ease_in_out") vm_float_t quadratic_ease_in_out(vm_float_t f);


// Cubic easing

/**
 * @brief Cubic ease-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t vm_cubic_ease_out(vm_float_t f);

/** @brief Deprecated. Use `vm_cubic_ease_out`. */
VEC_API VM_DEPRECATED("use vm_cubic_ease_out") vm_float_t cubic_ease_out(vm_float_t f);

/**
 * @brief Cubic ease-in.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t vm_cubic_ease_in(vm_float_t f);

/** @brief Deprecated. Use `vm_cubic_ease_in`. */
VEC_API VM_DEPRECATED("use vm_cubic_ease_in") vm_float_t cubic_ease_in(vm_float_t f);

/**
 * @brief Cubic ease-in-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t vm_cubic_ease_in_out(vm_float_t f);

/** @brief Deprecated. Use `vm_cubic_ease_in_out`. */
VEC_API VM_DEPRECATED("use vm_cubic_ease_in_out") vm_float_t cubic_ease_in_out(vm_float_t f);

// Quartic easing

/**
 * @brief Quartic ease-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t vm_quartic_ease_out(vm_float_t f);

/** @brief Deprecated. Use `vm_quartic_ease_out`. */
VEC_API VM_DEPRECATED("use vm_quartic_ease_out") vm_float_t quartic_ease_out(vm_float_t f);

/**
 * @brief Quartic ease-in.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t vm_quartic_ease_in(vm_float_t f);

/** @brief Deprecated. Use `vm_quartic_ease_in`. */
VEC_API VM_DEPRECATED("use vm_quartic_ease_in") vm_float_t quartic_ease_in(vm_float_t f);

/**
 * @brief Quartic ease-in-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t vm_quartic_ease_in_out(vm_float_t f);

/** @brief Deprecated. Use `vm_quartic_ease_in_out`. */
VEC_API VM_DEPRECATED("use vm_quartic_ease_in_out") vm_float_t quartic_ease_in_out(vm_float_t f);

// Quintic easing

/**
 * @brief Quintic ease-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t vm_quintic_ease_out(vm_float_t f);

/** @brief Deprecated. Use `vm_quintic_ease_out`. */
VEC_API VM_DEPRECATED("use vm_quintic_ease_out") vm_float_t quintic_ease_out(vm_float_t f);

/**
 * @brief Quintic ease-in.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t vm_quintic_ease_in(vm_float_t f);

/** @brief Deprecated. Use `vm_quintic_ease_in`. */
VEC_API VM_DEPRECATED("use vm_quintic_ease_in") vm_float_t quintic_ease_in(vm_float_t f);

/**
 * @brief Quintic ease-in-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t vm_quintic_ease_in_out(vm_float_t f);

/** @brief Deprecated. Use `vm_quintic_ease_in_out`. */
VEC_API VM_DEPRECATED("use vm_quintic_ease_in_out") vm_float_t quintic_ease_in_out(vm_float_t f);

// Sine easing

/**
 * @brief Sinusoidal ease-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t vm_sine_ease_out(vm_float_t f);

/** @brief Deprecated. Use `vm_sine_ease_out`. */
VEC_API VM_DEPRECATED("use vm_sine_ease_out") vm_float_t sine_ease_out(vm_float_t f);

/**
 * @brief Sinusoidal ease-in.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t vm_sine_ease_in(vm_float_t f);

/** @brief Deprecated. Use `vm_sine_ease_in`. */
VEC_API VM_DEPRECATED("use vm_sine_ease_in") vm_float_t sine_ease_in(vm_float_t f);

/**
 * @brief Sinusoidal ease-in-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t vm_sine_ease_in_out(vm_float_t f);

/** @brief Deprecated. Use `vm_sine_ease_in_out`. */
VEC_API VM_DEPRECATED("use vm_sine_ease_in_out") vm_float_t sine_ease_in_out(vm_float_t f);

// Circular easing

/**
 * @brief Circular ease-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t vm_circular_ease_out(vm_float_t f);

/** @brief Deprecated. Use `vm_circular_ease_out`. */
VEC_API VM_DEPRECATED("use vm_circular_ease_out") vm_float_t circular_ease_out(vm_float_t f);

/**
 * @brief Circular ease-in.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t vm_circular_ease_in(vm_float_t f);

/** @brief Deprecated. Use `vm_circular_ease_in`. */
VEC_API VM_DEPRECATED("use vm_circular_ease_in") vm_float_t circular_ease_in(vm_float_t f);

/**
 * @brief Circular ease-in-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t vm_circular_ease_in_out(vm_float_t f);

/** @brief Deprecated. Use `vm_circular_ease_in_out`. */
VEC_API VM_DEPRECATED("use vm_circular_ease_in_out") vm_float_t circular_ease_in_out(vm_float_t f);

// Exponential easing

/**
 * @brief Exponential ease-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t vm_exponential_ease_out(vm_float_t f);

/** @brief Deprecated. Use `vm_exponential_ease_out`. */
VEC_API VM_DEPRECATED("use vm_exponential_ease_out") vm_float_t exponential_ease_out(vm_float_t f);

/**
 * @brief Exponential ease-in.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t vm_exponential_ease_in(vm_float_t f);

/** @brief Deprecated. Use `vm_exponential_ease_in`. */
VEC_API VM_DEPRECATED("use vm_exponential_ease_in") vm_float_t exponential_ease_in(vm_float_t f);

/**
 * @brief Exponential ease-in-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t vm_exponential_ease_in_out(vm_float_t f);

/** @brief Deprecated. Use `vm_exponential_ease_in_out`. */
VEC_API VM_DEPRECATED("use vm_exponential_ease_in_out") vm_float_t exponential_ease_in_out(vm_float_t f);

// Elastic easing

/**
 * @brief Elastic ease-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t vm_elastic_ease_out(vm_float_t f);

/** @brief Deprecated. Use `vm_elastic_ease_out`. */
VEC_API VM_DEPRECATED("use vm_elastic_ease_out") vm_float_t elastic_ease_out(vm_float_t f);

/**
 * @brief Elastic ease-in.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t vm_elastic_ease_in(vm_float_t f);

/** @brief Deprecated. Use `vm_elastic_ease_in`. */
VEC_API VM_DEPRECATED("use vm_elastic_ease_in") vm_float_t elastic_ease_in(vm_float_t f);

/**
 * @brief Elastic ease-in-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t vm_elastic_ease_in_out(vm_float_t f);

/** @brief Deprecated. Use `vm_elastic_ease_in_out`. */
VEC_API VM_DEPRECATED("use vm_elastic_ease_in_out") vm_float_t elastic_ease_in_out(vm_float_t f);

// Back easing

/**
 * @brief Overshooting "back" ease-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t vm_back_ease_out(vm_float_t f);

/** @brief Deprecated. Use `vm_back_ease_out`. */
VEC_API VM_DEPRECATED("use vm_back_ease_out") vm_float_t back_ease_out(vm_float_t f);

/**
 * @brief Overshooting "back" ease-in.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t vm_back_ease_in(vm_float_t f);

/** @brief Deprecated. Use `vm_back_ease_in`. */
VEC_API VM_DEPRECATED("use vm_back_ease_in") vm_float_t back_ease_in(vm_float_t f);

/**
 * @brief Overshooting "back" ease-in-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t vm_back_ease_in_out(vm_float_t f);

/** @brief Deprecated. Use `vm_back_ease_in_out`. */
VEC_API VM_DEPRECATED("use vm_back_ease_in_out") vm_float_t back_ease_in_out(vm_float_t f);

// Bounce easing

/**
 * @brief Bounce ease-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t vm_bounce_ease_out(vm_float_t f);

/** @brief Deprecated. Use `vm_bounce_ease_out`. */
VEC_API VM_DEPRECATED("use vm_bounce_ease_out") vm_float_t bounce_ease_out(vm_float_t f);

/**
 * @brief Bounce ease-in.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t vm_bounce_ease_in(vm_float_t f);

/** @brief Deprecated. Use `vm_bounce_ease_in`. */
VEC_API VM_DEPRECATED("use vm_bounce_ease_in") vm_float_t bounce_ease_in(vm_float_t f);

/**
 * @brief Bounce ease-in-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
VEC_API vm_float_t vm_bounce_ease_in_out(vm_float_t f);

/** @brief Deprecated. Use `vm_bounce_ease_in_out`. */
VEC_API VM_DEPRECATED("use vm_bounce_ease_in_out") vm_float_t bounce_ease_in_out(vm_float_t f);


// Hermite interpolation (smoothstep family)


/**
 * @brief Smooth Hermite interpolation (cubic).
 *
 * Returns 0 when x <= edge0, 1 when x >= edge1, and the cubic Hermite
 * polynomial 3t^2 - 2t^3 in between, where t is the clamped normalized
 * position. Matches GLSL / HLSL smoothstep. First derivative is zero at
 * both edges.
 *
 * @param edge0 Lower edge.
 * @param edge1 Upper edge.
 * @param x Value to interpolate.
 * @return Interpolated value in [0, 1].
 */
VEC_API vm_float_t vm_smoothstep(vm_float_t edge0, vm_float_t edge1, vm_float_t x);

/** @brief Deprecated. Use `vm_smoothstep`. */
VEC_API VM_DEPRECATED("use vm_smoothstep") vm_float_t smoothstep(vm_float_t edge0, vm_float_t edge1, vm_float_t x);

/**
 * @brief Smoother Hermite interpolation (quintic).
 *
 * Returns 0 when x <= edge0, 1 when x >= edge1, and the quintic Hermite
 * polynomial 6t^5 - 15t^4 + 10t^3 in between. First and second derivatives
 * are zero at both edges (Perlin's improved fade). Preferred for noise
 * and bump mapping.
 *
 * @param edge0 Lower edge.
 * @param edge1 Upper edge.
 * @param x Value to interpolate.
 * @return Interpolated value in [0, 1].
 */
VEC_API vm_float_t vm_smootherstep(vm_float_t edge0, vm_float_t edge1, vm_float_t x);

/** @brief Deprecated. Use `vm_smootherstep`. */
VEC_API VM_DEPRECATED("use vm_smootherstep") vm_float_t smootherstep(vm_float_t edge0, vm_float_t edge1, vm_float_t x);

#endif //VECMAT_EASING_H
