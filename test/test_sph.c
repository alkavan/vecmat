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
