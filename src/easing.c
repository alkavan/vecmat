// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

vm_float_t elastic_oscillation(
    const vm_float_t f,
    const vm_float_t exp_mult,
    const vm_float_t sin_mult,
    const vm_float_t sin_offset,
    const vm_float_t period)
{
    return VECMAT_POW(2, exp_mult * f) * VECMAT_SIN((sin_mult * f - sin_offset) * (2.0f * M_PI) / period);
}

vm_float_t poly_ease_in(const vm_float_t f, const vm_float_t n)
{
    return VECMAT_POW(f, n);
}

vm_float_t poly_ease_out(const vm_float_t f, const vm_float_t n)
{
    return 1 - VECMAT_POW(1 - f, n);
}

vm_float_t poly_ease_in_out(const vm_float_t f, const vm_float_t n)
{
    return f < 0.5 ? VECMAT_POW(2 * f, n) / 2 : 1 - VECMAT_POW(2 - 2 * f, n) / 2;
}

vm_float_t quadratic_ease_out(const vm_float_t f)
{
    return poly_ease_out(f, 2);
}

vm_float_t quadratic_ease_in(const vm_float_t f)
{
    return poly_ease_in(f, 2);
}

vm_float_t quadratic_ease_in_out(const vm_float_t f)
{
    return poly_ease_in_out(f, 2);
}

vm_float_t cubic_ease_out(const vm_float_t f)
{
    return poly_ease_out(f, 3);
}

vm_float_t cubic_ease_in(const vm_float_t f)
{
    return poly_ease_in(f, 3);
}

vm_float_t cubic_ease_in_out(const vm_float_t f)
{
    return poly_ease_in_out(f, 3);
}

vm_float_t quartic_ease_out(const vm_float_t f)
{
    return poly_ease_out(f, 4);
}

vm_float_t quartic_ease_in(const vm_float_t f)
{
    return poly_ease_in(f, 4);
}

vm_float_t quartic_ease_in_out(const vm_float_t f)
{
    return poly_ease_in_out(f, 4);
}

vm_float_t quintic_ease_out(const vm_float_t f)
{
    return poly_ease_out(f, 5);
}

vm_float_t quintic_ease_in(const vm_float_t f)
{
    return poly_ease_in(f, 5);
}

vm_float_t quintic_ease_in_out(const vm_float_t f)
{
    return poly_ease_in_out(f, 5);
}

vm_float_t sine_ease_out(const vm_float_t f)
{
    return VECMAT_SIN(f * M_PI / 2);
}

vm_float_t sine_ease_in(const vm_float_t f)
{
    return 1 - VECMAT_COS(f * M_PI / 2);
}

vm_float_t sine_ease_in_out(const vm_float_t f)
{
    return -(VECMAT_COS(M_PI * f) - 1) / 2;
}

vm_float_t circular_ease_out(const vm_float_t f)
{
    return VECMAT_SQRT(1 - (1 - f) * (1 - f));
}

vm_float_t circular_ease_in(const vm_float_t f)
{
    return 1.f - VECMAT_SQRT(1 - f * f);
}

vm_float_t circular_ease_in_out(const vm_float_t f)
{
    return f < 0.5f ? (1.0f - VECMAT_SQRT(1.0f - 4.0f * f * f)) / 2.0f :
        (VECMAT_SQRT(1.0f - (-2.0f * f + 2.0f) * (-2.0f * f + 2.0f)) + 1.0f) / 2.0f;
}

vm_float_t exponential_ease_out(const vm_float_t f)
{
    return 1 - VECMAT_POW(2, -10 * f);
}

vm_float_t exponential_ease_in(const vm_float_t f)
{
    return VECMAT_POW(2, 10 * (f - 1));
}

vm_float_t exponential_ease_in_out(const vm_float_t f)
{
    return f == 0.0f ? 0.f : f == 1.f ? 1.f :
        f < 0.5f ? VECMAT_POW(2, 20 * f - 10) / 2.0f : (2 - VECMAT_POW(2, -20 * f + 10)) / 2.0f;
}

vm_float_t elastic_ease_out(const vm_float_t f)
{
    return f == 0.0f ? 0.f : f == 1.0f ? 1.f :
        elastic_oscillation(f, -10, 10, 0.75f, 3.f) + 1;
}

vm_float_t elastic_ease_in(const vm_float_t f)
{
    return f == 0.0f ? 0.f : f == 1.0f ? 1.f :
        -elastic_oscillation(f, 10, 10, 10.75f, 3.f);
}

vm_float_t elastic_ease_in_out(const vm_float_t f)
{
    return f == 0.0f ? 0.f :
        f == 1.0f ? 1.f : f < 0.5f ?
            -elastic_oscillation(f, 20, 20, 11.125f, 4.5f) / 2.f :
            elastic_oscillation(f, -20, 20, 11.125f, 4.5f) / 2.f + 1.f;
}

vm_float_t back_ease_out(const vm_float_t f)
{
    return 1.f + 1.70158f * VECMAT_POW(f - 1, 3) + 1.70158f * VECMAT_POW(f - 1.f, 2.f);
}

vm_float_t back_ease_in(const vm_float_t f)
{
    return 1.70158f * f * f * f - 1.70158f * f * f;
}

vm_float_t back_ease_in_out(const vm_float_t f)
{
    return f < 0.5f ? (VECMAT_POW(2 * f, 2) * ((2.5949095f + 1.0f)
        * 2.0f * f - 2.5949095f)) / 2.0f :
    (VECMAT_POW(2 * f - 2, 2) * ((2.5949095f + 1.0f)
        * (f * 2.0f - 2.0f) + 2.5949095f) + 2.0f) / 2.0f;
}

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

vm_float_t bounce_ease_in(const vm_float_t f)
{
    return 1 - bounce_ease_out(1 - f);
}

vm_float_t bounce_ease_in_out(const vm_float_t f)
{
    return f < 0.5 ? (1 - bounce_ease_out(1 - 2 * f)) / 2 : (1 + bounce_ease_out(2 * f - 1)) / 2;
}
