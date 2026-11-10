// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

#include "vecmat/eos.h"
#include "vecmat/cpu.h"

#if defined(VECMAT_ENABLE_AVX2)
bool vm_eos_ideal_n_avx2(vm_float_t *p, vm_float_t *c,
                         const vm_ideal_gas *eos,
                         const vm_float_t *rho, int rho_stride,
                         const vm_float_t *e, int e_stride, int n);
bool vm_eos_stiffened_n_avx2(vm_float_t *p, vm_float_t *c,
                             const vm_stiff_gas *eos,
                             const vm_float_t *rho, int rho_stride,
                             const vm_float_t *e, int e_stride, int n);
#endif
#if defined(VECMAT_ENABLE_NEON)
bool vm_eos_ideal_n_neon(vm_float_t *p, vm_float_t *c,
                         const vm_ideal_gas *eos,
                         const vm_float_t *rho, int rho_stride,
                         const vm_float_t *e, int e_stride, int n);
bool vm_eos_stiffened_n_neon(vm_float_t *p, vm_float_t *c,
                             const vm_stiff_gas *eos,
                             const vm_float_t *rho, int rho_stride,
                             const vm_float_t *e, int e_stride, int n);
#endif

static bool eos_fail_item(const vm_eos_state *st)
{
    return !st->ok;
}

static bool finish(vm_eos_state *out, const vm_float_t p, const vm_float_t c2)
{
    if (c2 < VM_F(0.0)) {
        out->ok = false;
        return false;
    }

    out->ok = true;
    out->p = p;
    out->c = VECMAT_SQRT(c2);

    return true;
}

bool vm_eos_ideal(vm_eos_state *out, const vm_ideal_gas *eos,
                  const vm_float_t rho, const vm_float_t e)
{
    if (!out) {
        return false;
    }

    out->ok = false;
    if (!eos || rho <= VM_F(0.0) || eos->gamma <= VM_F(1.0)) {
        return false;
    }

    const vm_float_t p = (eos->gamma - VM_F(1.0)) * rho * e;
    const vm_float_t c2 = eos->gamma * p / rho;

    return finish(out, p, c2);
}

bool vm_eos_stiffened(vm_eos_state *out, const vm_stiff_gas *eos,
                      const vm_float_t rho, const vm_float_t e)
{
    if (!out) {
        return false;
    }

    out->ok = false;
    if (!eos || rho <= VM_F(0.0) || eos->gamma <= VM_F(1.0)) {
        return false;
    }

    const vm_float_t p = (eos->gamma - VM_F(1.0)) * rho * e - eos->gamma * eos->p_inf;
    const vm_float_t c2 = eos->gamma * (p + eos->p_inf) / rho;

    return finish(out, p, c2);
}

bool vm_eos_mie(vm_eos_state *out, const vm_mie *eos,
                const vm_float_t rho, const vm_float_t e)
{
    if (!out) {
        return false;
    }

    out->ok = false;
    if (!eos || rho <= VM_F(0.0) || eos->rho0 <= VM_F(0.0)
        || eos->c0 <= VM_F(0.0) || eos->gamma0 < VM_F(0.0)) {
        return false;
    }

    const vm_float_t mu = rho / eos->rho0 - VM_F(1.0);
    const vm_float_t k = eos->rho0 * eos->c0 * eos->c0;

    vm_float_t p_h;
    vm_float_t dp_h_drho;
    vm_float_t e_h;
    vm_float_t de_h_drho;

    if (mu > VM_F(0.0)) {
        const vm_float_t sm1 = eos->s - VM_F(1.0);
        const vm_float_t denom = VM_F(1.0) - sm1 * mu;
        if (denom <= VM_F(0.0)) {
            return false;
        }

        const vm_float_t mu1 = VM_F(1.0) + mu;
        p_h = k * mu * mu1 / (denom * denom);
        e_h = p_h * mu / (VM_F(2.0) * eos->rho0 * mu1);

        const vm_float_t num = (VM_F(1.0) + VM_F(2.0) * mu) * denom
                             + VM_F(2.0) * sm1 * mu * mu1;
        const vm_float_t dp_h_dmu = k * num / (denom * denom * denom);
        dp_h_drho = dp_h_dmu / eos->rho0;

        const vm_float_t f = mu / mu1;
        const vm_float_t df_drho = (VM_F(1.0) / (mu1 * mu1)) / eos->rho0;
        de_h_drho = (dp_h_drho * f + p_h * df_drho) / (VM_F(2.0) * eos->rho0);
    } else {
        p_h = k * mu;
        e_h = VM_F(0.0);
        dp_h_drho = eos->c0 * eos->c0;
        de_h_drho = VM_F(0.0);
    }

    const vm_float_t p = p_h + eos->gamma0 * eos->rho0 * (e - e_h);
    const vm_float_t dp_de = eos->gamma0 * eos->rho0;
    const vm_float_t dp_drho = dp_h_drho - dp_de * de_h_drho;
    const vm_float_t c2 = dp_drho + (p / (rho * rho)) * dp_de;

    return finish(out, p, c2);
}

bool vm_eos_jwl(vm_eos_state *out, const vm_jwl *eos,
                const vm_float_t rho, const vm_float_t e)
{
    if (!out) {
        return false;
    }

    out->ok = false;
    if (!eos || rho <= VM_F(0.0) || eos->rho0 <= VM_F(0.0)
        || eos->r1 <= VM_F(0.0) || eos->r2 <= VM_F(0.0)) {
        return false;
    }

    const vm_float_t v = eos->rho0 / rho;
    const vm_float_t er1 = VECMAT_EXP(-eos->r1 * v);
    const vm_float_t er2 = VECMAT_EXP(-eos->r2 * v);
    const vm_float_t t1 = (VM_F(1.0) - eos->omega / (eos->r1 * v)) * er1;
    const vm_float_t t2 = (VM_F(1.0) - eos->omega / (eos->r2 * v)) * er2;
    /* Last term uses energy per reference volume: E = rho0 * e. */
    const vm_float_t p = eos->a * t1 + eos->b * t2 + eos->omega * eos->rho0 * e / v;

    /* d/dV of (1 - ω/(R V)) exp(-R V) */
    const vm_float_t d1 = er1 * (-eos->r1
        + eos->omega / (eos->r1 * v * v)
        + eos->omega / v);
    const vm_float_t d2 = er2 * (-eos->r2
        + eos->omega / (eos->r2 * v * v)
        + eos->omega / v);
    const vm_float_t dp_dv = eos->a * d1 + eos->b * d2
        - eos->omega * eos->rho0 * e / (v * v);
    const vm_float_t dp_drho = dp_dv * (-v / rho);
    const vm_float_t dp_de = eos->omega * eos->rho0 / v;
    const vm_float_t c2 = dp_drho + (p / (rho * rho)) * dp_de;

    return finish(out, p, c2);
}

bool vm_eos_ideal_n(vm_float_t *p, vm_float_t *c, const vm_ideal_gas *eos,
                    const vm_float_t *rho, const int rho_stride,
                    const vm_float_t *e, const int e_stride, const int n)
{
    if (!p || !c || !eos || !rho || !e || n <= 0 || eos->gamma <= VM_F(1.0)) {
        return false;
    }

#if defined(VECMAT_ENABLE_AVX2)
    if (vm_cpu_has(VM_LEVEL_AVX2)) {
        return vm_eos_ideal_n_avx2(p, c, eos, rho, rho_stride, e, e_stride, n);
    }
#endif
#if defined(VECMAT_ENABLE_NEON)
    if (vm_cpu_has(VM_LEVEL_NEON)) {
        return vm_eos_ideal_n_neon(p, c, eos, rho, rho_stride, e, e_stride, n);
    }
#endif

    for (int i = 0; i < n; ++i) {
        vm_eos_state st;
        if (!vm_eos_ideal(&st, eos, rho[i * rho_stride], e[i * e_stride]) || eos_fail_item(&st)) {
            return false;
        }
        p[i] = st.p;
        c[i] = st.c;
    }

    return true;
}

bool vm_eos_stiffened_n(vm_float_t *p, vm_float_t *c, const vm_stiff_gas *eos,
                        const vm_float_t *rho, const int rho_stride,
                        const vm_float_t *e, const int e_stride, const int n)
{
    if (!p || !c || !eos || !rho || !e || n <= 0 || eos->gamma <= VM_F(1.0)) {
        return false;
    }

#if defined(VECMAT_ENABLE_AVX2)
    if (vm_cpu_has(VM_LEVEL_AVX2)) {
        return vm_eos_stiffened_n_avx2(p, c, eos, rho, rho_stride, e, e_stride, n);
    }
#endif
#if defined(VECMAT_ENABLE_NEON)
    if (vm_cpu_has(VM_LEVEL_NEON)) {
        return vm_eos_stiffened_n_neon(p, c, eos, rho, rho_stride, e, e_stride, n);
    }
#endif

    for (int i = 0; i < n; ++i) {
        vm_eos_state st;
        if (!vm_eos_stiffened(&st, eos, rho[i * rho_stride], e[i * e_stride]) || eos_fail_item(&st)) {
            return false;
        }
        p[i] = st.p;
        c[i] = st.c;
    }

    return true;
}

bool vm_eos_mie_n(vm_float_t *p, vm_float_t *c, const vm_mie *eos,
                  const vm_float_t *rho, const int rho_stride,
                  const vm_float_t *e, const int e_stride, const int n)
{
    if (!p || !c || !eos || !rho || !e || n <= 0) {
        return false;
    }

    for (int i = 0; i < n; ++i) {
        vm_eos_state st;
        if (!vm_eos_mie(&st, eos, rho[i * rho_stride], e[i * e_stride]) || eos_fail_item(&st)) {
            return false;
        }
        p[i] = st.p;
        c[i] = st.c;
    }

    return true;
}

bool vm_eos_jwl_n(vm_float_t *p, vm_float_t *c, const vm_jwl *eos,
                  const vm_float_t *rho, const int rho_stride,
                  const vm_float_t *e, const int e_stride, const int n)
{
    if (!p || !c || !eos || !rho || !e || n <= 0) {
        return false;
    }

    for (int i = 0; i < n; ++i) {
        vm_eos_state st;
        if (!vm_eos_jwl(&st, eos, rho[i * rho_stride], e[i * e_stride]) || eos_fail_item(&st)) {
            return false;
        }
        p[i] = st.p;
        c[i] = st.c;
    }

    return true;
}
