// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "vecmat/hydro.h"

vm_sym3 vm_hydro_viscous(const vm_sym3 *D, const vm_float_t mu, const vm_float_t lambda)
{
    vm_sym3 s;
    for (int i = 0; i < 6; ++i) {
        s.c[i] = VM_F(0.0);
    }

    if (!D) {
        return s;
    }

    const vm_float_t tr = vm_sym3_trace(D);
    const vm_float_t bulk = lambda * tr;
    const vm_float_t two_mu = VM_F(2.0) * mu;

    s.xx = two_mu * D->xx + bulk;
    s.yy = two_mu * D->yy + bulk;
    s.zz = two_mu * D->zz + bulk;
    s.xy = two_mu * D->xy;
    s.yz = two_mu * D->yz;
    s.zx = two_mu * D->zx;

    return s;
}

vm_float_t vm_hydro_q(const vm_float_t rho, const vm_float_t delta_v, const vm_float_t c0)
{
    if (rho <= VM_F(0.0) || delta_v >= VM_F(0.0) || c0 == VM_F(0.0)) {
        return VM_F(0.0);
    }

    return c0 * rho * delta_v * delta_v;
}

bool vm_hydro_rusanov(vm_hydro_flux *flux,
                      const vm_hydro_state *left,
                      const vm_hydro_state *right)
{
    if (!flux || !left || !right) {
        return false;
    }

    const vm_float_t el = left->e + VM_F(0.5) * left->vn * left->vn;
    const vm_float_t er = right->e + VM_F(0.5) * right->vn * right->vn;

    const vm_float_t ul_m = left->rho;
    const vm_float_t ul_v = left->rho * left->vn;
    const vm_float_t ul_e = left->rho * el;
    const vm_float_t ur_m = right->rho;
    const vm_float_t ur_v = right->rho * right->vn;
    const vm_float_t ur_e = right->rho * er;

    const vm_float_t fl_m = left->rho * left->vn;
    const vm_float_t fl_v = left->rho * left->vn * left->vn + left->p;
    const vm_float_t fl_e = left->rho * left->vn * el + left->p * left->vn;
    const vm_float_t fr_m = right->rho * right->vn;
    const vm_float_t fr_v = right->rho * right->vn * right->vn + right->p;
    const vm_float_t fr_e = right->rho * right->vn * er + right->p * right->vn;

    const vm_float_t sl = VECMAT_FABS(left->vn) + left->c;
    const vm_float_t sr = VECMAT_FABS(right->vn) + right->c;
    const vm_float_t s = (sl > sr) ? sl : sr;
    const vm_float_t half = VM_F(0.5);

    flux->mass = half * (fl_m + fr_m) - half * s * (ur_m - ul_m);
    flux->momentum = half * (fl_v + fr_v) - half * s * (ur_v - ul_v);
    flux->energy = half * (fl_e + fr_e) - half * s * (ur_e - ul_e);

    return true;
}

vm_float_t vm_hydro_cfl(const vm_float_t h, const vm_float_t v, const vm_float_t c,
                        const vm_float_t kappa, const vm_float_t nu)
{
    if (h <= VM_F(0.0) || kappa <= VM_F(0.0)) {
        return VM_F(0.0);
    }

    const vm_float_t speed = VECMAT_FABS(v) + c;
    vm_float_t dt = VM_F(0.0);
    if (speed > VM_F(0.0)) {
        dt = kappa * h / speed;
    }

    if (nu > VM_F(0.0)) {
        const vm_float_t viscous = h * h / nu;
        if (dt <= VM_F(0.0) || viscous < dt) {
            dt = viscous;
        }
    }

    return dt;
}
