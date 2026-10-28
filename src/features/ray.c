// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <math.h>
#include <stddef.h>
#include <vecmat.h>

#include "vecmat/ray.h"

vm_float_t vm_ray_tau(const vm_float_t kappa, const vm_float_t s)
{
    if (kappa <= VM_F(0.0) || s <= VM_F(0.0)) {
        return VM_F(0.0);
    }

    return kappa * s;
}

vm_float_t vm_ray_tau_sum(const vm_float_t *kappa, const vm_float_t *s, const int n)
{
    if (!kappa || !s || n <= 0) {
        return VM_F(0.0);
    }

    vm_float_t acc = VM_F(0.0);
    for (int i = 0; i < n; ++i) {
        acc += vm_ray_tau(kappa[i], s[i]);
    }

    return acc;
}

vm_float_t vm_ray_transmit(const vm_float_t tau)
{
    if (tau <= VM_F(0.0)) {
        return VM_F(1.0);
    }

    if (tau > VM_RAY_TAU_CUTOFF) {
        return VM_F(0.0);
    }

    return VECMAT_EXP(-tau);
}

vm_float_t vm_ray_emit(const vm_float_t tau, const vm_float_t B)
{
    return (VM_F(1.0) - vm_ray_transmit(tau)) * B;
}
