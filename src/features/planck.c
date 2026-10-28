// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <math.h>
#include <stddef.h>
#include <vecmat.h>

#include "vecmat/planck.h"

/* Stefan-Boltzmann constant, CODATA 2018. */
#define SIGMA VM_F(5.670374419184429e-8)
#define PLANCK_H VM_F(6.62607015e-34)
#define BOLTZMANN_K VM_F(1.380649e-23)
#define LIGHT_C VM_F(2.99792458e8)

static void set_ok(bool *ok, const bool value)
{
    if (ok) {
        *ok = value;
    }
}

vm_float_t vm_planck_b(const vm_float_t T, bool *ok)
{
    if (T <= VM_F(0.0)) {
        set_ok(ok, false);
        return VM_F(0.0);
    }

    set_ok(ok, true);
    const vm_float_t T2 = T * T;

    return SIGMA * T2 * T2 / M_PI;
}

vm_float_t vm_planck_bnu(const vm_float_t nu, const vm_float_t T, bool *ok)
{
    if (T <= VM_F(0.0)) {
        set_ok(ok, false);
        return VM_F(0.0);
    }
    set_ok(ok, true);

    if (nu <= VM_F(0.0)) {
        return VM_F(0.0);
    }

    const vm_float_t x = PLANCK_H * nu / (BOLTZMANN_K * T);
    if (x > VM_F(80.0)) {
        return VM_F(0.0);
    }

    const vm_float_t nu2 = nu * nu;
    const vm_float_t num = VM_F(2.0) * PLANCK_H * nu2 * nu;
    const vm_float_t den = LIGHT_C * LIGHT_C * (VECMAT_EXP(x) - VM_F(1.0));

    return num / den;
}

/* 16-point Gauss-Legendre nodes and weights on [-1, 1]. */
static const vm_float_t gl_x[16] = {
    VM_F(-0.9894009349916499), VM_F(-0.9445750230732326),
    VM_F(-0.8656312023878318), VM_F(-0.7554044083550030),
    VM_F(-0.6178762444026438), VM_F(-0.4580167776572274),
    VM_F(-0.2816035507792589), VM_F(-0.0950125098376374),
    VM_F(0.0950125098376374), VM_F(0.2816035507792589),
    VM_F(0.4580167776572274), VM_F(0.6178762444026438),
    VM_F(0.7554044083550030), VM_F(0.8656312023878318),
    VM_F(0.9445750230732326), VM_F(0.9894009349916499)
};
static const vm_float_t gl_w[16] = {
    VM_F(0.0271524594117541), VM_F(0.0622535239386479),
    VM_F(0.0951585116824928), VM_F(0.1246289712555339),
    VM_F(0.1495959888165767), VM_F(0.1691565193950025),
    VM_F(0.1826034150449236), VM_F(0.1894506104550685),
    VM_F(0.1894506104550685), VM_F(0.1826034150449236),
    VM_F(0.1691565193950025), VM_F(0.1495959888165767),
    VM_F(0.1246289712555339), VM_F(0.0951585116824928),
    VM_F(0.0622535239386479), VM_F(0.0271524594117541)
};

static vm_float_t planck_integrand(const vm_float_t x)
{
    if (x <= VM_F(0.0)) {
        return VM_F(0.0);
    }

    if (x < VM_F(1e-4)) {
        return x * x;
    }

    if (x > VM_F(80.0)) {
        return VM_F(0.0);
    }

    const vm_float_t x2 = x * x;
    return x2 * x / (VECMAT_EXP(x) - VM_F(1.0));
}

vm_float_t vm_planck_group(const vm_float_t nu_lo, const vm_float_t nu_hi,
                           const vm_float_t T, bool *ok)
{
    if (T <= VM_F(0.0)) {
        set_ok(ok, false);
        return VM_F(0.0);
    }

    const vm_float_t full = vm_planck_b(T, NULL);
    if (nu_hi <= VM_F(0.0)) {
        set_ok(ok, true);
        return full;
    }

    if (nu_hi <= nu_lo) {
        set_ok(ok, false);
        return VM_F(0.0);
    }
    set_ok(ok, true);

    const vm_float_t scale = PLANCK_H / (BOLTZMANN_K * T);
    const vm_float_t x1 = (nu_lo > VM_F(0.0)) ? nu_lo * scale : VM_F(0.0);
    const vm_float_t x2 = nu_hi * scale;
    const vm_float_t half = VM_F(0.5) * (x2 - x1);
    const vm_float_t mid = VM_F(0.5) * (x2 + x1);

    vm_float_t acc = VM_F(0.0);
    for (int i = 0; i < 16; ++i) {
        const vm_float_t x = mid + half * gl_x[i];
        acc += gl_w[i] * planck_integrand(x);
    }
    acc *= half;

    /* Prefactor (2 k^4 / h^3 c^2) * T^4 = full * 15 / pi^4. */
    const vm_float_t pi2 = M_PI * M_PI;
    const vm_float_t pi4 = pi2 * pi2;

    return full * (VM_F(15.0) / pi4) * acc;
}

vm_float_t vm_planck_db_dt(const vm_float_t T, bool *ok)
{
    if (T <= VM_F(0.0)) {
        set_ok(ok, false);
        return VM_F(0.0);
    }
    set_ok(ok, true);

    return VM_F(4.0) * SIGMA * T * T * T / M_PI;
}
