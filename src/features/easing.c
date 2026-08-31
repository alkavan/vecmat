// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

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
vm_float_t elastic_oscillation(
    const vm_float_t f,
    const vm_float_t exp_mult,
    const vm_float_t sin_mult,
    const vm_float_t sin_offset,
    const vm_float_t period)
{
    return VECMAT_POW(2, exp_mult * f) * VECMAT_SIN((sin_mult * f - sin_offset) * (2.0f * M_PI) / period);
}

/**
 * @brief Polynomial ease-in: `f^n`.
 *
 * @param f Normalized time in [0, 1].
 * @param n Polynomial degree.
 * @return Eased value.
 */
vm_float_t poly_ease_in(const vm_float_t f, const vm_float_t n)
{
    return VECMAT_POW(f, n);
}

/**
 * @brief Polynomial ease-out: `1 - (1-f)^n`.
 *
 * @param f Normalized time in [0, 1].
 * @param n Polynomial degree.
 * @return Eased value.
 */
vm_float_t poly_ease_out(const vm_float_t f, const vm_float_t n)
{
    return 1 - VECMAT_POW(1 - f, n);
}

/**
 * @brief Polynomial ease-in-out of degree `n`.
 *
 * @param f Normalized time in [0, 1].
 * @param n Polynomial degree.
 * @return Eased value.
 */
vm_float_t poly_ease_in_out(const vm_float_t f, const vm_float_t n)
{
    return f < 0.5 ? VECMAT_POW(2 * f, n) / 2 : 1 - VECMAT_POW(2 - 2 * f, n) / 2;
}

/**
 * @brief Quadratic ease-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
vm_float_t quadratic_ease_out(const vm_float_t f)
{
    return poly_ease_out(f, 2);
}

/**
 * @brief Quadratic ease-in.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
vm_float_t quadratic_ease_in(const vm_float_t f)
{
    return poly_ease_in(f, 2);
}

/**
 * @brief Quadratic ease-in-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
vm_float_t quadratic_ease_in_out(const vm_float_t f)
{
    return poly_ease_in_out(f, 2);
}

/**
 * @brief Cubic ease-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
vm_float_t cubic_ease_out(const vm_float_t f)
{
    return poly_ease_out(f, 3);
}

/**
 * @brief Cubic ease-in.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
vm_float_t cubic_ease_in(const vm_float_t f)
{
    return poly_ease_in(f, 3);
}

/**
 * @brief Cubic ease-in-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
vm_float_t cubic_ease_in_out(const vm_float_t f)
{
    return poly_ease_in_out(f, 3);
}

/**
 * @brief Quartic ease-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
vm_float_t quartic_ease_out(const vm_float_t f)
{
    return poly_ease_out(f, 4);
}

/**
 * @brief Quartic ease-in.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
vm_float_t quartic_ease_in(const vm_float_t f)
{
    return poly_ease_in(f, 4);
}

/**
 * @brief Quartic ease-in-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
vm_float_t quartic_ease_in_out(const vm_float_t f)
{
    return poly_ease_in_out(f, 4);
}

/**
 * @brief Quintic ease-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
vm_float_t quintic_ease_out(const vm_float_t f)
{
    return poly_ease_out(f, 5);
}

/**
 * @brief Quintic ease-in.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
vm_float_t quintic_ease_in(const vm_float_t f)
{
    return poly_ease_in(f, 5);
}

/**
 * @brief Quintic ease-in-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
vm_float_t quintic_ease_in_out(const vm_float_t f)
{
    return poly_ease_in_out(f, 5);
}

/**
 * @brief Sinusoidal ease-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
vm_float_t sine_ease_out(const vm_float_t f)
{
    return VECMAT_SIN(f * M_PI / 2);
}

/**
 * @brief Sinusoidal ease-in.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
vm_float_t sine_ease_in(const vm_float_t f)
{
    return 1 - VECMAT_COS(f * M_PI / 2);
}

/**
 * @brief Sinusoidal ease-in-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
vm_float_t sine_ease_in_out(const vm_float_t f)
{
    return -(VECMAT_COS(M_PI * f) - 1) / 2;
}

/**
 * @brief Circular ease-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
vm_float_t circular_ease_out(const vm_float_t f)
{
    return VECMAT_SQRT(1 - (1 - f) * (1 - f));
}

/**
 * @brief Circular ease-in.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
vm_float_t circular_ease_in(const vm_float_t f)
{
    return 1.f - VECMAT_SQRT(1 - f * f);
}

/**
 * @brief Circular ease-in-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
vm_float_t circular_ease_in_out(const vm_float_t f)
{
    return f < 0.5f ? (1.0f - VECMAT_SQRT(1.0f - 4.0f * f * f)) / 2.0f :
        (VECMAT_SQRT(1.0f - (-2.0f * f + 2.0f) * (-2.0f * f + 2.0f)) + 1.0f) / 2.0f;
}

/**
 * @brief Exponential ease-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
vm_float_t exponential_ease_out(const vm_float_t f)
{
    return 1 - VECMAT_POW(2, -10 * f);
}

/**
 * @brief Exponential ease-in.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
vm_float_t exponential_ease_in(const vm_float_t f)
{
    return VECMAT_POW(2, 10 * (f - 1));
}

/**
 * @brief Exponential ease-in-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
vm_float_t exponential_ease_in_out(const vm_float_t f)
{
    return f == 0.0f ? 0.f : f == 1.f ? 1.f :
        f < 0.5f ? VECMAT_POW(2, 20 * f - 10) / 2.0f : (2 - VECMAT_POW(2, -20 * f + 10)) / 2.0f;
}

/**
 * @brief Elastic ease-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
vm_float_t elastic_ease_out(const vm_float_t f)
{
    return f == 0.0f ? 0.f : f == 1.0f ? 1.f :
        elastic_oscillation(f, -10, 10, 0.75f, 3.f) + 1;
}

/**
 * @brief Elastic ease-in.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
vm_float_t elastic_ease_in(const vm_float_t f)
{
    return f == 0.0f ? 0.f : f == 1.0f ? 1.f :
        -elastic_oscillation(f, 10, 10, 10.75f, 3.f);
}

/**
 * @brief Elastic ease-in-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
vm_float_t elastic_ease_in_out(const vm_float_t f)
{
    return f == 0.0f ? 0.f :
        f == 1.0f ? 1.f : f < 0.5f ?
            -elastic_oscillation(f, 20, 20, 11.125f, 4.5f) / 2.f :
            elastic_oscillation(f, -20, 20, 11.125f, 4.5f) / 2.f + 1.f;
}

/**
 * @brief Overshooting "back" ease-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
vm_float_t back_ease_out(const vm_float_t f)
{
    return 1.f + 1.70158f * VECMAT_POW(f - 1, 3) + 1.70158f * VECMAT_POW(f - 1.f, 2.f);
}

/**
 * @brief Overshooting "back" ease-in.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
vm_float_t back_ease_in(const vm_float_t f)
{
    return 1.70158f * f * f * f - 1.70158f * f * f;
}

/**
 * @brief Overshooting "back" ease-in-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
vm_float_t back_ease_in_out(const vm_float_t f)
{
    return f < 0.5f ? (VECMAT_POW(2 * f, 2) * ((2.5949095f + 1.0f)
        * 2.0f * f - 2.5949095f)) / 2.0f :
    (VECMAT_POW(2 * f - 2, 2) * ((2.5949095f + 1.0f)
        * (f * 2.0f - 2.0f) + 2.5949095f) + 2.0f) / 2.0f;
}

/**
 * @brief Bounce ease-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
vm_float_t bounce_ease_out(const vm_float_t f)
{
    if (f < 1.0f / 2.75f) return 7.5625f * f * f;

    if (f < 2.0f / 2.75f) {
        const vm_float_t t = f - 1.5f / 2.75f;
        return 7.5625f * t * t + 0.75f;
    }

    if (f < 2.5f / 2.75f) {
        const vm_float_t t = f - 2.25f / 2.75f;
        return 7.5625f * t * t + 0.9375f;
    }

    const vm_float_t t = f - 2.625f / 2.75f;
    return 7.5625f * t * t + 0.984375f;
}

/**
 * @brief Bounce ease-in.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
vm_float_t bounce_ease_in(const vm_float_t f)
{
    return 1 - bounce_ease_out(1 - f);
}

/**
 * @brief Bounce ease-in-out.
 *
 * @param f Normalized time in [0, 1].
 * @return Eased value.
 */
vm_float_t bounce_ease_in_out(const vm_float_t f)
{
    return f < 0.5 ? (1 - bounce_ease_out(1 - 2 * f)) / 2 : (1 + bounce_ease_out(2 * f - 1)) / 2;
}
