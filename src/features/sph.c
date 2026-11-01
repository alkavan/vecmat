// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <math.h>
#include <vecmat.h>
#include "vecmat/sph.h"

/* Cubic spline shape and derivative, q = r/h, support q < 2. */
static vm_float_t cubic_f(const vm_float_t q)
{
    if (q < VM_F(0.0) || q >= VM_F(2.0)) {
        return VM_F(0.0);
    }

    if (q <= VM_F(1.0)) {
        return VM_F(1.0) - VM_F(1.5) * q * q + VM_F(0.75) * q * q * q;
    }
    const vm_float_t t = VM_F(2.0) - q;

    return VM_F(0.25) * t * t * t;
}

static vm_float_t cubic_df(const vm_float_t q)
{
    if (q <= VM_F(0.0) || q >= VM_F(2.0)) {
        return VM_F(0.0);
    }

    if (q <= VM_F(1.0)) {
        return VM_F(-3.0) * q + VM_F(2.25) * q * q;
    }
    const vm_float_t t = VM_F(2.0) - q;

    return VM_F(-0.75) * t * t;
}

/* Wendland C2 shape, q = r/h, support q < 2. */
static vm_float_t wendland_f(const vm_float_t q)
{
    if (q < VM_F(0.0) || q >= VM_F(2.0)) {
        return VM_F(0.0);
    }

    const vm_float_t u = VM_F(1.0) - VM_F(0.5) * q;
    const vm_float_t u2 = u * u;

    return u2 * u2 * (VM_F(1.0) + VM_F(2.0) * q);
}

static vm_float_t wendland_df(const vm_float_t q)
{
    if (q <= VM_F(0.0) || q >= VM_F(2.0)) {
        return VM_F(0.0);
    }
    const vm_float_t u = VM_F(1.0) - VM_F(0.5) * q;

    return VM_F(-5.0) * q * u * u * u;
}

static vm_float_t kernel_sigma(const int dim, const int kernel)
{
    if (kernel == VM_SPH_WENDLAND) {
        if (dim == 1) return VM_F(0.75);
        if (dim == 2) return VM_F(7.0) / (VM_F(4.0) * M_PI);
        if (dim == 3) return VM_F(21.0) / (VM_F(16.0) * M_PI);
        return VM_F(0.0);
    }

    if (dim == 1) return VM_F(2.0) / VM_F(3.0);
    if (dim == 2) return VM_F(10.0) / (VM_F(7.0) * M_PI);
    if (dim == 3) return VM_F(1.0) / M_PI;

    return VM_F(0.0);
}

vm_float_t vm_sph_w(const vm_float_t r, const vm_float_t h, const int dim, const int kernel)
{
    if (h <= VM_F(0.0) || dim < 1 || dim > 3) {
        return VM_F(0.0);
    }

    const vm_float_t q = VECMAT_FABS(r) / h;
    const vm_float_t f = (kernel == VM_SPH_WENDLAND) ? wendland_f(q) : cubic_f(q);

    vm_float_t hd = h;
    for (int d = 1; d < dim; ++d) {
        hd *= h;
    }

    return kernel_sigma(dim, kernel) * f / hd;
}

vm_float_t vm_sph_dw(const vm_float_t r, const vm_float_t h, const int dim, const int kernel)
{
    if (h <= VM_F(0.0) || dim < 1 || dim > 3 || r == VM_F(0.0)) {
        return VM_F(0.0);
    }

    const vm_float_t q = VECMAT_FABS(r) / h;
    const vm_float_t df = (kernel == VM_SPH_WENDLAND) ? wendland_df(q) : cubic_df(q);

    vm_float_t hd = h;
    for (int d = 0; d < dim; ++d) {
        hd *= h;
    }

    const vm_float_t sign = (r < VM_F(0.0)) ? VM_F(-1.0) : VM_F(1.0);

    return sign * kernel_sigma(dim, kernel) * df / hd;
}

static vm_float_t separation(const vm_float_t dx, const vm_float_t dy,
                             const vm_float_t dz, const int dim)
{
    vm_float_t r2 = dx * dx;
    if (dim >= 2) {
        r2 += dy * dy;
    }

    if (dim >= 3) {
        r2 += dz * dz;
    }

    return VECMAT_SQRT(r2);
}

vm_float_t vm_sph_density(const vm_float_t *x, const vm_float_t *y,
                          const vm_float_t *z, const vm_float_t *m,
                          const int i, const int *neighbors, const int nn,
                          const vm_float_t h, const int dim, const int kernel)
{
    if (!x || !m || (dim >= 2 && !y) || (dim >= 3 && !z) || h <= VM_F(0.0)) {
        return VM_F(0.0);
    }

    vm_float_t rho = m[i] * vm_sph_w(VM_F(0.0), h, dim, kernel);
    if (!neighbors || nn <= 0) {
        return rho;
    }

    for (int k = 0; k < nn; ++k) {
        const int j = neighbors[k];
        const vm_float_t dx = x[i] - x[j];
        const vm_float_t dy = (dim >= 2) ? (y[i] - y[j]) : VM_F(0.0);
        const vm_float_t dz = (dim >= 3) ? (z[i] - z[j]) : VM_F(0.0);
        const vm_float_t r = separation(dx, dy, dz, dim);
        rho += m[j] * vm_sph_w(r, h, dim, kernel);
    }

    return rho;
}

bool vm_sph_pressure_grad(vm_float_t *gx, vm_float_t *gy, vm_float_t *gz,
                          const vm_float_t *x, const vm_float_t *y,
                          const vm_float_t *z, const vm_float_t *m,
                          const vm_float_t *rho, const vm_float_t *p,
                          const int i, const int *neighbors, const int nn,
                          const vm_float_t h, const int dim, const int kernel)
{
    if (!gx || !gy || !x || !m || !rho || !p || (dim >= 2 && !y) ||
        (dim >= 3 && (!z || !gz)) || h <= VM_F(0.0) || rho[i] <= VM_F(0.0)) {
        return false;
    }

    *gx = VM_F(0.0);
    *gy = VM_F(0.0);
    if (gz) {
        *gz = VM_F(0.0);
    }

    if (!neighbors || nn <= 0) {
        return true;
    }

    const vm_float_t pi_term = p[i] / (rho[i] * rho[i]);
    for (int k = 0; k < nn; ++k)
    {
        const int j = neighbors[k];
        if (rho[j] <= VM_F(0.0)) {
            return false;
        }

        const vm_float_t dx = x[i] - x[j];
        const vm_float_t dy = (dim >= 2) ? (y[i] - y[j]) : VM_F(0.0);
        const vm_float_t dz = (dim >= 3) ? (z[i] - z[j]) : VM_F(0.0);

        const vm_float_t r = separation(dx, dy, dz, dim);
        if (r == VM_F(0.0)) {
            continue;
        }

        const vm_float_t dw = vm_sph_dw(r, h, dim, kernel);
        const vm_float_t scale = m[j] * (pi_term + p[j] / (rho[j] * rho[j])) * dw / r;

        *gx += scale * dx;
        if (dim >= 2) {
            *gy += scale * dy;
        }

        if (dim >= 3 && gz) {
            *gz += scale * dz;
        }
    }
    return true;
}
