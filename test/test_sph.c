// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <stdlib.h>
#include <vecmat.h>
#include "unitest.h"

static vm_float_t integrate_1d(const int kernel)
{
    const vm_float_t h = VM_F(1.0);
    const int n = 4000;
    const vm_float_t lo = VM_F(-2.0) * h;
    const vm_float_t dx = (VM_F(4.0) * h) / (vm_float_t)n;
    vm_float_t acc = VM_F(0.0);
    for (int i = 0; i < n; ++i) {
        const vm_float_t r = lo + (vm_float_t)i * dx + VM_F(0.5) * dx;
        acc += vm_sph_w(r, h, 1, kernel) * dx;
    }
    return acc;
}

static vm_float_t integrate_2d(const int kernel)
{
    const vm_float_t h = VM_F(1.0);
    const int n = 200;
    const vm_float_t lo = VM_F(-2.0) * h;
    const vm_float_t dx = (VM_F(4.0) * h) / (vm_float_t)n;
    vm_float_t acc = VM_F(0.0);
    for (int j = 0; j < n; ++j) {
        const vm_float_t y = lo + (vm_float_t)j * dx + VM_F(0.5) * dx;
        for (int i = 0; i < n; ++i) {
            const vm_float_t x = lo + (vm_float_t)i * dx + VM_F(0.5) * dx;
            const vm_float_t r = VECMAT_SQRT(x * x + y * y);
            acc += vm_sph_w(r, h, 2, kernel) * dx * dx;
        }
    }
    return acc;
}

TEST_CASE(sph_kernel_integrates_to_one, "[sph]") {
    REQUIRE(VECMAT_EQ(integrate_1d(VM_SPH_CUBIC), VM_F(1.0), VM_F(1e-3)));
    REQUIRE(VECMAT_EQ(integrate_1d(VM_SPH_WENDLAND), VM_F(1.0), VM_F(1e-3)));
    REQUIRE(VECMAT_EQ(integrate_2d(VM_SPH_CUBIC), VM_F(1.0), VM_F(2e-2)));
    REQUIRE(VECMAT_EQ(integrate_2d(VM_SPH_WENDLAND), VM_F(1.0), VM_F(2e-2)));
}

TEST_CASE(sph_gradient_zero_at_origin, "[sph]") {
    REQUIRE(VECMAT_EQ(vm_sph_dw(VM_F(0.0), VM_F(1.0), 1, VM_SPH_CUBIC), VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(vm_sph_dw(VM_F(0.0), VM_F(1.0), 2, VM_SPH_CUBIC), VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(vm_sph_dw(VM_F(0.0), VM_F(1.0), 3, VM_SPH_WENDLAND), VM_F(0.0), EPSILON));
}

TEST_CASE(sph_empty_neighbor_is_self, "[sph]") {
    const vm_float_t x[] = { VM_F(0.0) };
    const vm_float_t m[] = { VM_F(2.0) };
    const vm_float_t h = VM_F(1.0);
    const vm_float_t rho = vm_sph_density(x, NULL, NULL, m, 0, NULL, 0, h, 1, VM_SPH_CUBIC);
    const vm_float_t expect = m[0] * vm_sph_w(VM_F(0.0), h, 1, VM_SPH_CUBIC);
    REQUIRE(VECMAT_EQ(rho, expect, EPSILON * expect));
}

TEST_CASE(sph_coincident_particles, "[sph]") {
    const vm_float_t x[] = { VM_F(0.0), VM_F(0.0) };
    const vm_float_t m[] = { VM_F(1.5), VM_F(2.5) };
    const int nb[] = { 1 };
    const vm_float_t h = VM_F(1.0);
    const vm_float_t rho = vm_sph_density(x, NULL, NULL, m, 0, nb, 1, h, 1, VM_SPH_WENDLAND);
    const vm_float_t w0 = vm_sph_w(VM_F(0.0), h, 1, VM_SPH_WENDLAND);
    REQUIRE(VECMAT_EQ(rho, (m[0] + m[1]) * w0, EPSILON * rho));
}

static vm_float_t expected_density(const vm_float_t *x, const vm_float_t *y,
                                   const vm_float_t *z, const vm_float_t *m,
                                   const int i, const int *neighbors, const int nn,
                                   const vm_float_t h, const int dim, const int kernel)
{
    vm_float_t rho = m[i] * vm_sph_w(VM_F(0.0), h, dim, kernel);
    for (int k = 0; k < nn; ++k) {
        const int j = neighbors[k];
        const vm_float_t dx = x[i] - x[j];
        const vm_float_t dy = (dim >= 2) ? (y[i] - y[j]) : VM_F(0.0);
        const vm_float_t dz = (dim >= 3) ? (z[i] - z[j]) : VM_F(0.0);
        const vm_float_t r = VECMAT_SQRT(dx * dx + dy * dy + dz * dz);
        rho += m[j] * vm_sph_w(r, h, dim, kernel);
    }
    return rho;
}

TEST_CASE(sph_density_multi_neighbor, "[sph]") {
    const vm_float_t h = VM_F(1.0);
    const int kernels[] = { VM_SPH_CUBIC, VM_SPH_WENDLAND };
    for (int ki = 0; ki < 2; ++ki) {
        const int kernel = kernels[ki];
        /* 1-D, nn = 5 (remainder) and 9 (lanes + rem for f32) */
        const vm_float_t x1[] = { VM_F(0.0), VM_F(0.3), VM_F(0.6), VM_F(0.9), VM_F(1.2),
                                  VM_F(-0.4), VM_F(-0.8), VM_F(1.5), VM_F(-1.1), VM_F(0.1) };
        const vm_float_t m1[] = { VM_F(1.0), VM_F(1.1), VM_F(0.9), VM_F(1.2), VM_F(0.8),
                                  VM_F(1.0), VM_F(1.3), VM_F(0.7), VM_F(1.1), VM_F(0.95) };
        const int nb5[] = { 1, 2, 3, 4, 5 };
        const int nb9[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        vm_float_t rho = vm_sph_density(x1, NULL, NULL, m1, 0, nb5, 5, h, 1, kernel);
        REQUIRE(VECMAT_EQ(rho, expected_density(x1, NULL, NULL, m1, 0, nb5, 5, h, 1, kernel), EPSILON * rho));
        rho = vm_sph_density(x1, NULL, NULL, m1, 0, nb9, 9, h, 1, kernel);
        REQUIRE(VECMAT_EQ(rho, expected_density(x1, NULL, NULL, m1, 0, nb9, 9, h, 1, kernel), EPSILON * rho));

        /* 2-D */
        const vm_float_t x2[] = { VM_F(0.0), VM_F(0.4), VM_F(-0.3), VM_F(0.2), VM_F(-0.5),
                                  VM_F(0.6), VM_F(-0.1), VM_F(0.7), VM_F(-0.4) };
        const vm_float_t y2[] = { VM_F(0.0), VM_F(0.2), VM_F(0.5), VM_F(-0.4), VM_F(0.1),
                                  VM_F(-0.3), VM_F(0.6), VM_F(0.0), VM_F(-0.2) };
        const vm_float_t m2[] = { VM_F(1.0), VM_F(1.0), VM_F(1.1), VM_F(0.9), VM_F(1.2),
                                  VM_F(0.8), VM_F(1.0), VM_F(1.3), VM_F(0.7) };
        const int nb2[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
        rho = vm_sph_density(x2, y2, NULL, m2, 0, nb2, 8, h, 2, kernel);
        REQUIRE(VECMAT_EQ(rho, expected_density(x2, y2, NULL, m2, 0, nb2, 8, h, 2, kernel), EPSILON * rho));

        /* 3-D */
        const vm_float_t x3[] = { VM_F(0.0), VM_F(0.3), VM_F(-0.2), VM_F(0.4), VM_F(-0.1) };
        const vm_float_t y3[] = { VM_F(0.0), VM_F(0.1), VM_F(0.4), VM_F(-0.3), VM_F(0.2) };
        const vm_float_t z3[] = { VM_F(0.0), VM_F(-0.2), VM_F(0.3), VM_F(0.1), VM_F(-0.4) };
        const vm_float_t m3[] = { VM_F(1.0), VM_F(1.1), VM_F(0.9), VM_F(1.2), VM_F(0.8) };
        const int nb3[] = { 1, 2, 3, 4 };
        rho = vm_sph_density(x3, y3, z3, m3, 0, nb3, 4, h, 3, kernel);
        REQUIRE(VECMAT_EQ(rho, expected_density(x3, y3, z3, m3, 0, nb3, 4, h, 3, kernel), EPSILON * rho));
    }
}

static void expected_pressure(vm_float_t *gx, vm_float_t *gy, vm_float_t *gz,
                              const vm_float_t *x, const vm_float_t *y, const vm_float_t *z,
                              const vm_float_t *m, const vm_float_t *rho, const vm_float_t *p,
                              const int i, const int *neighbors, const int nn,
                              const vm_float_t h, const int dim, const int kernel)
{
    *gx = VM_F(0.0);
    *gy = VM_F(0.0);
    if (gz) *gz = VM_F(0.0);
    const vm_float_t pi_term = p[i] / (rho[i] * rho[i]);
    for (int k = 0; k < nn; ++k) {
        const int j = neighbors[k];
        const vm_float_t dx = x[i] - x[j];
        const vm_float_t dy = (dim >= 2) ? (y[i] - y[j]) : VM_F(0.0);
        const vm_float_t dz = (dim >= 3) ? (z[i] - z[j]) : VM_F(0.0);
        const vm_float_t r2 = dx * dx + dy * dy + dz * dz;
        if (r2 <= VM_F(0.0)) continue;
        const vm_float_t r = VECMAT_SQRT(r2);
        const vm_float_t dw = vm_sph_dw(r, h, dim, kernel);
        const vm_float_t scale = m[j] * (pi_term + p[j] / (rho[j] * rho[j])) * dw / r;
        *gx += scale * dx;
        if (dim >= 2) *gy += scale * dy;
        if (dim >= 3 && gz) *gz += scale * dz;
    }
}

TEST_CASE(sph_pressure_grad_multi_neighbor, "[sph]") {
    const vm_float_t h = VM_F(1.0);
    const int kernels[] = { VM_SPH_CUBIC, VM_SPH_WENDLAND };
    for (int ki = 0; ki < 2; ++ki) {
        const int kernel = kernels[ki];
        const vm_float_t x[] = { VM_F(0.0), VM_F(0.5), VM_F(-0.4), VM_F(0.3), VM_F(-0.6),
                                 VM_F(0.2), VM_F(-0.1), VM_F(0.7), VM_F(-0.3) };
        const vm_float_t y[] = { VM_F(0.0), VM_F(0.1), VM_F(0.3), VM_F(-0.2), VM_F(0.4),
                                 VM_F(-0.5), VM_F(0.2), VM_F(0.0), VM_F(-0.1) };
        const vm_float_t z[] = { VM_F(0.0), VM_F(-0.2), VM_F(0.1), VM_F(0.4), VM_F(-0.3),
                                 VM_F(0.2), VM_F(-0.4), VM_F(0.1), VM_F(0.3) };
        const vm_float_t m[] = { VM_F(1.0), VM_F(1.1), VM_F(0.9), VM_F(1.2), VM_F(0.8),
                                 VM_F(1.0), VM_F(1.3), VM_F(0.7), VM_F(1.1) };
        const vm_float_t rho[] = { VM_F(1.2), VM_F(1.1), VM_F(1.3), VM_F(1.0), VM_F(1.4),
                                   VM_F(1.1), VM_F(1.2), VM_F(0.9), VM_F(1.3) };
        const vm_float_t p[] = { VM_F(0.5), VM_F(0.4), VM_F(0.6), VM_F(0.3), VM_F(0.7),
                                 VM_F(0.5), VM_F(0.4), VM_F(0.6), VM_F(0.3) };
        const int nb[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
        vm_float_t gx, gy, gz, egx, egy, egz;
        bool ok = vm_sph_pressure_grad(&gx, &gy, &gz, x, y, z, m, rho, p, 0, nb, 8, h, 3, kernel);
        REQUIRE(ok);
        expected_pressure(&egx, &egy, &egz, x, y, z, m, rho, p, 0, nb, 8, h, 3, kernel);
        REQUIRE(VECMAT_EQ(gx, egx, EPSILON * VECMAT_FABS(egx) + EPSILON));
        REQUIRE(VECMAT_EQ(gy, egy, EPSILON * VECMAT_FABS(egy) + EPSILON));
        REQUIRE(VECMAT_EQ(gz, egz, EPSILON * VECMAT_FABS(egz) + EPSILON));

        /* 1-D case with remainder */
        ok = vm_sph_pressure_grad(&gx, &gy, NULL, x, NULL, NULL, m, rho, p, 0, nb, 5, h, 1, kernel);
        REQUIRE(ok);
        expected_pressure(&egx, &egy, NULL, x, NULL, NULL, m, rho, p, 0, nb, 5, h, 1, kernel);
        REQUIRE(VECMAT_EQ(gx, egx, EPSILON * VECMAT_FABS(egx) + EPSILON));
    }
}

TEST_CASE(sph_pressure_bad_density, "[sph]") {
    const vm_float_t x[] = { VM_F(0.0), VM_F(0.5), VM_F(0.3), VM_F(0.4), VM_F(0.2),
                             VM_F(0.6), VM_F(0.1), VM_F(0.7), VM_F(0.8) };
    const vm_float_t m[] = { VM_F(1.0), VM_F(1.0), VM_F(1.0), VM_F(1.0), VM_F(1.0),
                             VM_F(1.0), VM_F(1.0), VM_F(1.0), VM_F(1.0) };
    const vm_float_t rho[] = { VM_F(1.0), VM_F(1.1), VM_F(1.2), VM_F(0.0), VM_F(1.0),
                               VM_F(1.3), VM_F(1.1), VM_F(1.0), VM_F(1.2) };
    const vm_float_t p[] = { VM_F(1.0), VM_F(1.0), VM_F(1.0), VM_F(1.0), VM_F(1.0),
                             VM_F(1.0), VM_F(1.0), VM_F(1.0), VM_F(1.0) };
    const int nb[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    vm_float_t gx, gy;
    const bool ok = vm_sph_pressure_grad(&gx, &gy, NULL, x, NULL, NULL, m, rho, p,
                                         0, nb, 8, VM_F(1.0), 1, VM_SPH_CUBIC);
    REQUIRE(!ok);
}
