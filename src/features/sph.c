// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <math.h>
#include <stddef.h>
#include <vecmat.h>

#include "vecmat/sph.h"
#include "vecmat/cpu.h"

#if defined(VECMAT_ENABLE_AVX2)
vm_float_t vm_sph_density_avx2(const vm_float_t *x, const vm_float_t *y,
                               const vm_float_t *z, const vm_float_t *m,
                               int i, const int *neighbors, int nn,
                               vm_float_t h, int dim, int kernel);
bool vm_sph_pressure_grad_avx2(vm_float_t *gx, vm_float_t *gy, vm_float_t *gz,
                               const vm_float_t *x, const vm_float_t *y,
                               const vm_float_t *z, const vm_float_t *m,
                               const vm_float_t *rho, const vm_float_t *p,
                               int i, const int *neighbors, int nn,
                               vm_float_t h, int dim, int kernel);
#endif
#if defined(VECMAT_ENABLE_NEON)
vm_float_t vm_sph_density_neon(const vm_float_t *x, const vm_float_t *y,
                               const vm_float_t *z, const vm_float_t *m,
                               int i, const int *neighbors, int nn,
                               vm_float_t h, int dim, int kernel);
bool vm_sph_pressure_grad_neon(vm_float_t *gx, vm_float_t *gy, vm_float_t *gz,
                               const vm_float_t *x, const vm_float_t *y,
                               const vm_float_t *z, const vm_float_t *m,
                               const vm_float_t *rho, const vm_float_t *p,
                               int i, const int *neighbors, int nn,
                               vm_float_t h, int dim, int kernel);
#endif

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

#if defined(VECMAT_ENABLE_AVX2)
    if ((vm_cpu_runtime_features() & VM_CPU_AVX2) != 0) {
        return vm_sph_density_avx2(x, y, z, m, i, neighbors, nn, h, dim, kernel);
    }
#endif
#if defined(VECMAT_ENABLE_NEON)
    if ((vm_cpu_runtime_features() & VM_CPU_NEON) != 0) {
        return vm_sph_density_neon(x, y, z, m, i, neighbors, nn, h, dim, kernel);
    }
#endif

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
#if defined(VECMAT_ENABLE_AVX2)
    if ((vm_cpu_runtime_features() & VM_CPU_AVX2) != 0) {
        return vm_sph_pressure_grad_avx2(gx, gy, gz, x, y, z, m, rho, p,
                                         i, neighbors, nn, h, dim, kernel);
    }
#endif
#if defined(VECMAT_ENABLE_NEON)
    if ((vm_cpu_runtime_features() & VM_CPU_NEON) != 0) {
        return vm_sph_pressure_grad_neon(gx, gy, gz, x, y, z, m, rho, p,
                                         i, neighbors, nn, h, dim, kernel);
    }
#endif

    *gx = VM_F(0.0);
    *gy = VM_F(0.0);
    if (gz) {
        *gz = VM_F(0.0);
    }

    if (!neighbors || nn <= 0) {
        return true;
    }

    const vm_float_t pi_term = p[i] / (rho[i] * rho[i]);
    for (int k = 0; k < nn; ++k) {
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

/* Per-particle discretizations. Each returns the term; the caller integrates. */

static void sph_delta(const vm_float_t *x, const vm_float_t *y, const vm_float_t *z,
                      const int dim, const int i, const int j,
                      vm_float_t *dx, vm_float_t *dy, vm_float_t *dz)
{
    *dx = x[i] - x[j];
    *dy = (dim >= 2) ? (y[i] - y[j]) : VM_F(0.0);
    *dz = (dim >= 3) ? (z[i] - z[j]) : VM_F(0.0);
}

vm_float_t vm_sph_continuity(const vm_float_t *x, const vm_float_t *y,
                             const vm_float_t *z,
                             const vm_float_t *vx, const vm_float_t *vy,
                             const vm_float_t *vz, const vm_float_t *m,
                             const int i, const int *neighbors, const int nn,
                             const vm_float_t h, const int dim, const int kernel)
{
    if (!x || !vx || !m || (dim >= 2 && (!y || !vy)) || (dim >= 3 && (!z || !vz))
        || h <= VM_F(0.0) || dim < 1 || dim > 3) {
        return VM_F(0.0);
    }

    if (!neighbors || nn <= 0) {
        return VM_F(0.0);
    }

    vm_float_t rate = VM_F(0.0);
    for (int k = 0; k < nn; ++k) {
        const int j = neighbors[k];
        vm_float_t dx, dy, dz;
        sph_delta(x, y, z, dim, i, j, &dx, &dy, &dz);

        const vm_float_t r = separation(dx, dy, dz, dim);
        if (r == VM_F(0.0)) {
            continue;
        }

        const vm_float_t dw = vm_sph_dw(r, h, dim, kernel);
        const vm_float_t vxij = vx[i] - vx[j];
        const vm_float_t vyij = (dim >= 2) ? (vy[i] - vy[j]) : VM_F(0.0);
        const vm_float_t vzij = (dim >= 3) ? (vz[i] - vz[j]) : VM_F(0.0);
        const vm_float_t v_dot_r = vxij * dx + vyij * dy + vzij * dz;

        rate += m[j] * v_dot_r * dw / r;
    }

    return rate;
}

bool vm_sph_viscosity(vm_float_t *ax, vm_float_t *ay, vm_float_t *az,
                      const vm_float_t *x, const vm_float_t *y,
                      const vm_float_t *z,
                      const vm_float_t *vx, const vm_float_t *vy,
                      const vm_float_t *vz,
                      const vm_float_t *m, const vm_float_t *rho,
                      const vm_float_t *mu,
                      const int i, const int *neighbors, const int nn,
                      const vm_float_t h, const vm_float_t eta,
                      const int dim, const int kernel)
{
    if (!ax || !ay || !x || !vx || !m || !rho || !mu
        || (dim >= 2 && (!y || !vy)) || (dim >= 3 && (!z || !vz || !az))
        || h <= VM_F(0.0) || dim < 1 || dim > 3 || rho[i] <= VM_F(0.0)) {
        return false;
    }

    *ax = VM_F(0.0);
    *ay = VM_F(0.0);

    if (az) {
        *az = VM_F(0.0);
    }

    if (!neighbors || nn <= 0) {
        return true;
    }

    const vm_float_t floor2 = (eta * h) * (eta * h);
    for (int k = 0; k < nn; ++k) {
        const int j = neighbors[k];
        if (rho[j] <= VM_F(0.0)) {
            return false;
        }

        vm_float_t dx, dy, dz;
        sph_delta(x, y, z, dim, i, j, &dx, &dy, &dz);
        const vm_float_t r = separation(dx, dy, dz, dim);

        if (r == VM_F(0.0)) {
            continue;
        }

        const vm_float_t vxij = vx[i] - vx[j];
        const vm_float_t vyij = (dim >= 2) ? (vy[i] - vy[j]) : VM_F(0.0);
        const vm_float_t vzij = (dim >= 3) ? (vz[i] - vz[j]) : VM_F(0.0);
        const vm_float_t v_dot_r = vxij * dx + vyij * dy + vzij * dz;
        const vm_float_t r2 = r * r;
        const vm_float_t dw = vm_sph_dw(r, h, dim, kernel);
        const vm_float_t coeff = m[j] * (mu[i] + mu[j]) / (rho[i] * rho[j])
                               * v_dot_r / (r2 + floor2) * dw / r;
        *ax += coeff * dx;
        if (dim >= 2) {
            *ay += coeff * dy;
        }
        if (dim >= 3 && az) {
            *az += coeff * dz;
        }
    }
    return true;
}

vm_float_t vm_sph_pi(const vm_float_t r, const vm_float_t v_dot_r,
                     const vm_float_t rho_i, const vm_float_t rho_j,
                     const vm_float_t c_i, const vm_float_t c_j,
                     const vm_float_t h, const vm_float_t alpha,
                     const vm_float_t beta, const vm_float_t eta)
{
    if (v_dot_r >= VM_F(0.0) || rho_i <= VM_F(0.0) || rho_j <= VM_F(0.0)
        || h <= VM_F(0.0) || r < VM_F(0.0)) {
        return VM_F(0.0);
    }

    const vm_float_t r2 = r * r;
    const vm_float_t mu = h * v_dot_r / (r2 + (eta * h) * (eta * h));
    const vm_float_t c_bar = VM_F(0.5) * (c_i + c_j);
    const vm_float_t rho_bar = VM_F(0.5) * (rho_i + rho_j);

    if (rho_bar == VM_F(0.0)) {
        return VM_F(0.0);
    }

    return (-alpha * c_bar * mu + beta * mu * mu) / rho_bar;
}

bool vm_sph_artvisc(vm_float_t *ax, vm_float_t *ay, vm_float_t *az,
                    const vm_float_t *x, const vm_float_t *y,
                    const vm_float_t *z,
                    const vm_float_t *vx, const vm_float_t *vy,
                    const vm_float_t *vz,
                    const vm_float_t *m, const vm_float_t *rho,
                    const vm_float_t *c,
                    const int i, const int *neighbors, const int nn,
                    const vm_float_t h, const vm_float_t alpha,
                    const vm_float_t beta, const vm_float_t eta,
                    const int dim, const int kernel)
{
    if (!ax || !ay || !x || !vx || !m || !rho || !c
        || (dim >= 2 && (!y || !vy)) || (dim >= 3 && (!z || !vz || !az))
        || h <= VM_F(0.0) || dim < 1 || dim > 3 || rho[i] <= VM_F(0.0)) {
        return false;
    }

    *ax = VM_F(0.0);
    *ay = VM_F(0.0);
    if (az) {
        *az = VM_F(0.0);
    }

    if (!neighbors || nn <= 0) {
        return true;
    }

    for (int k = 0; k < nn; ++k) {
        const int j = neighbors[k];
        if (rho[j] <= VM_F(0.0)) {
            return false;
        }

        vm_float_t dx, dy, dz;
        sph_delta(x, y, z, dim, i, j, &dx, &dy, &dz);
        const vm_float_t r = separation(dx, dy, dz, dim);

        if (r == VM_F(0.0)) {
            continue;
        }

        const vm_float_t vxij = vx[i] - vx[j];
        const vm_float_t vyij = (dim >= 2) ? (vy[i] - vy[j]) : VM_F(0.0);
        const vm_float_t vzij = (dim >= 3) ? (vz[i] - vz[j]) : VM_F(0.0);

        const vm_float_t v_dot_r = vxij * dx + vyij * dy + vzij * dz;
        const vm_float_t pi = vm_sph_pi(r, v_dot_r, rho[i], rho[j], c[i], c[j],
                                        h, alpha, beta, eta);
        const vm_float_t dw = vm_sph_dw(r, h, dim, kernel);
        const vm_float_t coeff = -m[j] * pi * dw / r;

        *ax += coeff * dx;
        if (dim >= 2) {
            *ay += coeff * dy;
        }
        if (dim >= 3 && az) {
            *az += coeff * dz;
        }
    }

    return true;
}

bool vm_sph_xsph(vm_float_t *dvx, vm_float_t *dvy, vm_float_t *dvz,
                 const vm_float_t *x, const vm_float_t *y,
                 const vm_float_t *z,
                 const vm_float_t *vx, const vm_float_t *vy,
                 const vm_float_t *vz,
                 const vm_float_t *m, const vm_float_t *rho,
                 const int i, const int *neighbors, const int nn,
                 const vm_float_t h, const vm_float_t epsilon,
                 const int dim, const int kernel)
{
    if (!dvx || !dvy || !x || !vx || !m || !rho
        || (dim >= 2 && (!y || !vy)) || (dim >= 3 && (!z || !vz || !dvz))
        || h <= VM_F(0.0) || dim < 1 || dim > 3) {
        return false;
    }

    *dvx = VM_F(0.0);
    *dvy = VM_F(0.0);
    if (dvz) {
        *dvz = VM_F(0.0);
    }

    if (!neighbors || nn <= 0 || epsilon == VM_F(0.0)) {
        return true;
    }

    for (int k = 0; k < nn; ++k) {
        const int j = neighbors[k];
        if (rho[j] <= VM_F(0.0)) {
            return false;
        }

        vm_float_t dx, dy, dz;
        sph_delta(x, y, z, dim, i, j, &dx, &dy, &dz);

        const vm_float_t r = separation(dx, dy, dz, dim);
        const vm_float_t w = vm_sph_w(r, h, dim, kernel);
        const vm_float_t coeff = epsilon * m[j] / rho[j] * w;

        *dvx += coeff * (vx[j] - vx[i]);
        if (dim >= 2) {
            *dvy += coeff * (vy[j] - vy[i]);
        }
        if (dim >= 3 && dvz) {
            *dvz += coeff * (vz[j] - vz[i]);
        }
    }
    return true;
}

static vm_float_t sph_tension(const vm_float_t p, const vm_float_t rho,
                              const vm_float_t epsilon)
{
    if (p >= VM_F(0.0) || rho <= VM_F(0.0)) {
        return VM_F(0.0);
    }

    return epsilon * VECMAT_FABS(p) / (rho * rho);
}

bool vm_sph_tensile(vm_float_t *ax, vm_float_t *ay, vm_float_t *az,
                    const vm_float_t *x, const vm_float_t *y,
                    const vm_float_t *z,
                    const vm_float_t *m, const vm_float_t *rho,
                    const vm_float_t *p,
                    const int i, const int *neighbors, const int nn,
                    const vm_float_t h, const vm_float_t delta,
                    const vm_float_t epsilon, const int nexp,
                    const int dim, const int kernel)
{
    if (!ax || !ay || !x || !m || !rho || !p
        || (dim >= 2 && !y) || (dim >= 3 && (!z || !az))
        || h <= VM_F(0.0) || delta <= VM_F(0.0) || nexp < 0
        || dim < 1 || dim > 3 || rho[i] <= VM_F(0.0)) {
        return false;
    }

    const vm_float_t w_delta = vm_sph_w(delta, h, dim, kernel);

    if (w_delta == VM_F(0.0)) {
        return false;
    }
    *ax = VM_F(0.0);
    *ay = VM_F(0.0);
    if (az) {
        *az = VM_F(0.0);
    }

    if (!neighbors || nn <= 0 || epsilon == VM_F(0.0)) {
        return true;
    }

    const vm_float_t ri = sph_tension(p[i], rho[i], epsilon);
    for (int k = 0; k < nn; ++k) {
        const int j = neighbors[k];
        if (rho[j] <= VM_F(0.0)) {
            return false;
        }

        const vm_float_t rj = sph_tension(p[j], rho[j], epsilon);
        if (ri == VM_F(0.0) && rj == VM_F(0.0)) {
            continue;
        }

        vm_float_t dx, dy, dz;
        sph_delta(x, y, z, dim, i, j, &dx, &dy, &dz);

        const vm_float_t r = separation(dx, dy, dz, dim);
        if (r == VM_F(0.0)) {
            continue;
        }

        const vm_float_t f = vm_sph_w(r, h, dim, kernel) / w_delta;
        vm_float_t fn = VM_F(1.0);
        for (int e = 0; e < nexp; ++e) {
            fn *= f;
        }

        const vm_float_t dw = vm_sph_dw(r, h, dim, kernel);
        const vm_float_t coeff = -m[j] * (ri + rj) * fn * dw / r;

        *ax += coeff * dx;
        if (dim >= 2) {
            *ay += coeff * dy;
        }
        if (dim >= 3 && az) {
            *az += coeff * dz;
        }
    }

    return true;
}

bool vm_sph_wall(vm_float_t *ax, vm_float_t *ay, vm_float_t *az,
                 const vm_float_t px, const vm_float_t py, const vm_float_t pz,
                 const vm_float_t qx, const vm_float_t qy, const vm_float_t qz,
                 const vm_float_t nx, const vm_float_t ny, const vm_float_t nz,
                 const vm_float_t cutoff, const vm_float_t strength)
{
    if (!ax || !ay || !az || cutoff <= VM_F(0.0)) {
        return false;
    }

    *ax = VM_F(0.0);
    *ay = VM_F(0.0);
    *az = VM_F(0.0);

    const vm_float_t gap = (px - qx) * nx + (py - qy) * ny + (pz - qz) * nz;
    if (gap <= VM_F(0.0) || gap >= cutoff || strength == VM_F(0.0)) {
        return true;
    }

    const vm_float_t ratio = cutoff / gap;
    const vm_float_t ratio2 = ratio * ratio;
    const vm_float_t mag = strength * (ratio2 * ratio2 - ratio2);

    *ax = mag * nx;
    *ay = mag * ny;
    *az = mag * nz;

    return true;
}

bool vm_sph_boundary(vm_float_t *ax, vm_float_t *ay, vm_float_t *az,
                     const vm_float_t *x, const vm_float_t *y,
                     const vm_float_t *z, const vm_float_t *m,
                     const int i, const int *boundary, const int nb,
                     const vm_float_t r0, const vm_float_t strength,
                     const int dim)
{
    if (!ax || !ay || !x || !m || (dim >= 2 && !y) || (dim >= 3 && (!z || !az))
        || r0 <= VM_F(0.0) || dim < 1 || dim > 3) {
        return false;
    }

    *ax = VM_F(0.0);
    *ay = VM_F(0.0);
    if (az) {
        *az = VM_F(0.0);
    }

    if (!boundary || nb <= 0 || strength == VM_F(0.0)) {
        return true;
    }

    const vm_float_t r02 = r0 * r0;
    for (int k = 0; k < nb; ++k) {
        const int j = boundary[k];
        if (j == i) {
            continue;
        }

        vm_float_t dx, dy, dz;
        sph_delta(x, y, z, dim, i, j, &dx, &dy, &dz);

        const vm_float_t r2 = dx * dx + dy * dy + dz * dz;
        if (r2 == VM_F(0.0) || r2 >= r02) {
            continue;
        }

        const vm_float_t r = VECMAT_SQRT(r2);
        const vm_float_t ratio = r0 / r;
        const vm_float_t ratio2 = ratio * ratio;
        const vm_float_t mag = m[j] * strength * (ratio2 * ratio2 - ratio2) / r2;

        *ax += mag * dx;
        if (dim >= 2) {
            *ay += mag * dy;
        }
        if (dim >= 3 && az) {
            *az += mag * dz;
        }
    }

    return true;
}

/* Linked-cell bin and CSR neighbor lists. Caller owns every buffer. */

static int sph_ncells_raw(const vm_sph_grid *g)
{
    if (!g || g->nx <= 0 || g->ny <= 0 || g->nz <= 0 || g->cell <= VM_F(0.0)) {
        return 0;
    }

    return g->nx * g->ny * g->nz;
}

int vm_sph_ncells(const vm_sph_grid *g)
{
    return sph_ncells_raw(g);
}

static int axis_cells(const vm_float_t lo, const vm_float_t hi, const vm_float_t cell)
{
    const vm_float_t extent = hi - lo;

    int n = (extent > VM_F(0.0)) ? (int)VECMAT_CEIL(extent / cell) : 1;
    if (n < 1) {
        n = 1;
    }

    return n;
}

bool vm_sph_grid_fit(vm_sph_grid *g,
                     const vm_float_t *x, const vm_float_t *y,
                     const vm_float_t *z, const int n,
                     const vm_float_t cell, const int dim)
{
    if (!g || !x || n <= 0 || cell <= VM_F(0.0) || dim < 1 || dim > 3
        || (dim >= 2 && !y) || (dim >= 3 && !z)) {
        return false;
    }

    vm_float_t xlo = x[0], xhi = x[0];
    vm_float_t ylo = VM_F(0.0), yhi = VM_F(0.0);
    vm_float_t zlo = VM_F(0.0), zhi = VM_F(0.0);

    if (dim >= 2) {
        ylo = y[0];
        yhi = y[0];
    }
    if (dim >= 3) {
        zlo = z[0];
        zhi = z[0];
    }

    for (int i = 1; i < n; ++i) {
        if (x[i] < xlo) xlo = x[i];
        if (x[i] > xhi) xhi = x[i];

        if (dim >= 2) {
            if (y[i] < ylo) ylo = y[i];
            if (y[i] > yhi) yhi = y[i];
        }
        if (dim >= 3) {
            if (z[i] < zlo) zlo = z[i];
            if (z[i] > zhi) zhi = z[i];
        }
    }

    g->x0 = xlo;
    g->y0 = (dim >= 2) ? ylo : VM_F(0.0);
    g->z0 = (dim >= 3) ? zlo : VM_F(0.0);
    g->cell = cell;
    g->nx = axis_cells(xlo, xhi, cell);
    g->ny = (dim >= 2) ? axis_cells(ylo, yhi, cell) : 1;
    g->nz = (dim >= 3) ? axis_cells(zlo, zhi, cell) : 1;

    return sph_ncells_raw(g) > 0;
}

int vm_sph_prefix(int *offsets, const int *counts, const int n)
{
    if (!offsets || !counts || n < 0) {
        return -1;
    }

    int sum = 0;
    offsets[0] = 0;
    for (int i = 0; i < n; ++i) {
        if (counts[i] < 0) {
            return -1;
        }
        sum += counts[i];
        offsets[i + 1] = sum;
    }

    return sum;
}

static int sph_cell(const vm_sph_grid *g, const int dim,
                    const vm_float_t px, const vm_float_t py, const vm_float_t pz)
{
    const vm_float_t cell = g->cell;
    const vm_float_t dx = px - g->x0;

    if (dx < VM_F(0.0)) {
        return -1;
    }

    int i = (int)VECMAT_FLOOR(dx / cell);
    if (i >= g->nx) {
        if (dx > (vm_float_t)g->nx * cell) {
            return -1;
        }
        i = g->nx - 1;
    }

    int j = 0;
    if (dim >= 2) {
        const vm_float_t dy = py - g->y0;
        if (dy < VM_F(0.0)) {
            return -1;
        }

        j = (int)VECMAT_FLOOR(dy / cell);
        if (j >= g->ny) {
            if (dy > (vm_float_t)g->ny * cell) {
                return -1;
            }
            j = g->ny - 1;
        }
    }

    int k = 0;
    if (dim >= 3) {
        const vm_float_t dz = pz - g->z0;
        if (dz < VM_F(0.0)) {
            return -1;
        }

        k = (int)VECMAT_FLOOR(dz / cell);
        if (k >= g->nz) {
            if (dz > (vm_float_t)g->nz * cell) {
                return -1;
            }
            k = g->nz - 1;
        }
    }

    return (k * g->ny + j) * g->nx + i;
}

bool vm_sph_bin(int *cell_of, int *cell_offsets, int *cell_particles,
                const vm_float_t *x, const vm_float_t *y,
                const vm_float_t *z, const int n,
                const vm_sph_grid *g, const int dim)
{
    const int ncells = sph_ncells_raw(g);
    if (!cell_of || !cell_offsets || !cell_particles || !x || n <= 0 || ncells <= 0
        || dim < 1 || dim > 3 || (dim >= 2 && !y) || (dim >= 3 && !z)) {
        return false;
    }

    for (int c = 0; c <= ncells; ++c) {
        cell_offsets[c] = 0;
    }

    for (int i = 0; i < n; ++i) {
        const vm_float_t py = (dim >= 2) ? y[i] : VM_F(0.0);
        const vm_float_t pz = (dim >= 3) ? z[i] : VM_F(0.0);

        const int c = sph_cell(g, dim, x[i], py, pz);
        cell_of[i] = c;

        if (c >= 0) {
            cell_offsets[c] += 1;
        }
    }

    int sum = 0;
    for (int c = 0; c < ncells; ++c) {
        const int cnt = cell_offsets[c];
        cell_offsets[c] = sum;
        sum += cnt;
    }
    cell_offsets[ncells] = sum;

    for (int i = 0; i < n; ++i) {
        const int c = cell_of[i];
        if (c < 0) {
            continue;
        }

        cell_particles[cell_offsets[c]] = i;
        cell_offsets[c] += 1;
    }

    for (int c = ncells; c > 0; --c) {
        cell_offsets[c] = cell_offsets[c - 1];
    }
    cell_offsets[0] = 0;

    return true;
}

static vm_float_t sph_r2(const vm_float_t *x, const vm_float_t *y, const vm_float_t *z,
                         const int dim, const int i, const int j)
{
    const vm_float_t dx = x[i] - x[j];
    vm_float_t r2 = dx * dx;

    if (dim >= 2) {
        const vm_float_t dy = y[i] - y[j];
        r2 += dy * dy;
    }
    if (dim >= 3) {
        const vm_float_t dz = z[i] - z[j];
        r2 += dz * dz;
    }

    return r2;
}

static int sph_visit(const vm_float_t *x, const vm_float_t *y, const vm_float_t *z,
                     const int n, const vm_sph_grid *g,
                     const int *cell_offsets, const int *cell_particles,
                     const vm_float_t support2, const int dim, const int i,
                     int *out, const int out_n, int *written)
{
    const int c = sph_cell(g, dim,
                           x[i],
                           (dim >= 2) ? y[i] : VM_F(0.0),
                           (dim >= 3) ? z[i] : VM_F(0.0));
    if (c < 0) {
        return 0;
    }

    const int ci = c % g->nx;
    const int cj = (c / g->nx) % g->ny;
    const int ck = c / (g->nx * g->ny);
    const int j0 = (dim >= 2) ? -1 : 0;
    const int j1 = (dim >= 2) ? 1 : 0;
    const int k0 = (dim >= 3) ? -1 : 0;
    const int k1 = (dim >= 3) ? 1 : 0;

    int count = 0;
    for (int dk = k0; dk <= k1; ++dk) {
        const int nk = ck + dk;
        if (nk < 0 || nk >= g->nz) {
            continue;
        }

        for (int dj = j0; dj <= j1; ++dj) {
            const int nj = cj + dj;
            if (nj < 0 || nj >= g->ny) {
                continue;
            }

            for (int di = -1; di <= 1; ++di) {
                const int ni = ci + di;
                if (ni < 0 || ni >= g->nx) {
                    continue;
                }

                const int nc = (nk * g->ny + nj) * g->nx + ni;
                for (int p = cell_offsets[nc]; p < cell_offsets[nc + 1]; ++p) {
                    const int j = cell_particles[p];
                    if (j == i || j < 0 || j >= n) {
                        continue;
                    }

                    if (sph_r2(x, y, z, dim, i, j) >= support2) {
                        continue;
                    }

                    if (out && count < out_n) {
                        out[(*written)++] = j;
                    }

                    count += 1;
                }
            }
        }
    }
    return count;
}

int vm_sph_neighbor_count(int *nn,
                          const vm_float_t *x, const vm_float_t *y,
                          const vm_float_t *z, const int n,
                          const vm_sph_grid *g,
                          const int *cell_offsets, const int *cell_particles,
                          const vm_float_t support, const int dim)
{
    if (!nn || !x || !cell_offsets || !cell_particles || n <= 0
        || sph_ncells_raw(g) <= 0 || support <= VM_F(0.0)
        || dim < 1 || dim > 3 || (dim >= 2 && !y) || (dim >= 3 && !z)) {
        return -1;
    }

    const vm_float_t support2 = support * support;
    int total = 0;
    for (int i = 0; i < n; ++i) {
        const int c = sph_visit(x, y, z, n, g, cell_offsets, cell_particles,
                                support2, dim, i, NULL, 0, NULL);
        nn[i] = c;
        total += c;
    }

    return total;
}

bool vm_sph_neighbor_fill(int *neighbors, const int *offsets,
                          const vm_float_t *x, const vm_float_t *y,
                          const vm_float_t *z, const int n,
                          const vm_sph_grid *g,
                          const int *cell_offsets, const int *cell_particles,
                          const vm_float_t support, const int dim)
{
    if (!neighbors || !offsets || !x || !cell_offsets || !cell_particles || n <= 0
        || sph_ncells_raw(g) <= 0 || support <= VM_F(0.0)
        || dim < 1 || dim > 3 || (dim >= 2 && !y) || (dim >= 3 && !z)
        || offsets[0] != 0) {
        return false;
    }

    const vm_float_t support2 = support * support;
    for (int i = 0; i < n; ++i) {
        const int begin = offsets[i];
        const int end = offsets[i + 1];
        if (end < begin) {
            return false;
        }

        int written = begin;
        const int count = sph_visit(x, y, z, n, g, cell_offsets, cell_particles,
                                    support2, dim, i, neighbors, end - begin, &written);

        if (count != end - begin || written != end) {
            return false;
        }
    }

    return true;
}
