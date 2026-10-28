// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <stdlib.h>
#include <vecmat.h>
#include "unitest.h"

TEST_CASE(planck_integrated_matches_sigma, "[planck]") {
    bool ok = false;
    const vm_float_t T = VM_F(1000.0);
    const vm_float_t B = vm_planck_b(T, &ok);
    REQUIRE(ok);
    const vm_float_t sigma = VM_F(5.670374419184429e-8);
    const vm_float_t expect = sigma * T * T * T * T / M_PI;
    REQUIRE(VECMAT_EQ(B, expect, EPSILON * expect));
}

TEST_CASE(planck_nonpositive_fails, "[planck]") {
    bool ok = true;
    REQUIRE(vm_planck_b(VM_F(0.0), &ok) == VM_F(0.0));
    REQUIRE(!ok);
    ok = true;
    REQUIRE(vm_planck_b(VM_F(-1.0), &ok) == VM_F(0.0));
    REQUIRE(!ok);
}

TEST_CASE(planck_group_full_spectrum, "[planck]") {
    bool ok = false;
    const vm_float_t T = VM_F(500.0);
    const vm_float_t full = vm_planck_b(T, NULL);
    const vm_float_t group = vm_planck_group(VM_F(0.0), VM_F(0.0), T, &ok);
    REQUIRE(ok);
    REQUIRE(VECMAT_EQ(group, full, EPSILON * full));
}

TEST_CASE(planck_db_dt, "[planck]") {
    bool ok = false;
    const vm_float_t T = VM_F(300.0);
    const vm_float_t d = vm_planck_db_dt(T, &ok);
    REQUIRE(ok);
    const vm_float_t sigma = VM_F(5.670374419184429e-8);
    const vm_float_t expect = VM_F(4.0) * sigma * T * T * T / M_PI;
    REQUIRE(VECMAT_EQ(d, expect, EPSILON * expect));
}

TEST_CASE(angular_weights_sum_to_4pi, "[angular]") {
    const int orders[] = { 2, 4, 6, 8 };
    for (int k = 0; k < 4; ++k) {
        const int n = orders[k];
        const int count = vm_angular_count(n);
        REQUIRE(count > 0);
        vm_float_t *mu = (vm_float_t *)malloc((size_t)count * sizeof(vm_float_t));
        vm_float_t *eta = (vm_float_t *)malloc((size_t)count * sizeof(vm_float_t));
        vm_float_t *xi = (vm_float_t *)malloc((size_t)count * sizeof(vm_float_t));
        vm_float_t *w = (vm_float_t *)malloc((size_t)count * sizeof(vm_float_t));
        REQUIRE(mu && eta && xi && w);
        REQUIRE(vm_angular_sn(n, mu, eta, xi, w) == count);
        vm_float_t sum = VM_F(0.0);
        for (int i = 0; i < count; ++i) {
            const vm_float_t r2 = mu[i] * mu[i] + eta[i] * eta[i] + xi[i] * xi[i];
            REQUIRE(VECMAT_EQ(r2, VM_F(1.0), VM_F(1e-5)));
            sum += w[i];
        }
        REQUIRE(VECMAT_EQ(sum, VM_F(4.0) * M_PI, VM_F(1e-3)));
        free(mu);
        free(eta);
        free(xi);
        free(w);
    }
}

TEST_CASE(angular_isotropic_scatter_constant, "[angular]") {
    const int count = vm_angular_count(4);
    vm_float_t *mu = (vm_float_t *)malloc((size_t)count * sizeof(vm_float_t));
    vm_float_t *eta = (vm_float_t *)malloc((size_t)count * sizeof(vm_float_t));
    vm_float_t *xi = (vm_float_t *)malloc((size_t)count * sizeof(vm_float_t));
    vm_float_t *w = (vm_float_t *)malloc((size_t)count * sizeof(vm_float_t));
    vm_float_t *I = (vm_float_t *)malloc((size_t)count * sizeof(vm_float_t));
    REQUIRE(mu && eta && xi && w && I);
    vm_angular_sn(4, mu, eta, xi, w);
    for (int i = 0; i < count; ++i) {
        I[i] = VM_F(2.5);
    }
    const vm_float_t s = vm_angular_scatter(mu, eta, xi, w, I, count,
                                            VM_F(0.0), VM_F(0.0), VM_F(1.0), VM_F(0.0));
    REQUIRE(VECMAT_EQ(s, VM_F(2.5), VM_F(1e-3)));
    free(mu);
    free(eta);
    free(xi);
    free(w);
    free(I);
}

TEST_CASE(angular_hg_zero_matches_isotropic, "[angular]") {
    const vm_float_t iso = vm_angular_isotropic();
    const vm_float_t hg = vm_angular_hg(VM_F(0.3), VM_F(0.0));
    REQUIRE(VECMAT_EQ(hg, iso, EPSILON));
}

TEST_CASE(ray_zero_tau_transmits, "[ray]") {
    REQUIRE(VECMAT_EQ(vm_ray_transmit(VM_F(0.0)), VM_F(1.0), EPSILON));
    REQUIRE(VECMAT_EQ(vm_ray_transmit(VM_F(-1.0)), VM_F(1.0), EPSILON));
}

TEST_CASE(ray_long_segment_emits_B, "[ray]") {
    const vm_float_t B = VM_F(3.0);
    const vm_float_t e = vm_ray_emit(VM_F(100.0), B);
    REQUIRE(VECMAT_EQ(e, B, EPSILON));
}

TEST_CASE(ray_piecewise_matches_sum, "[ray]") {
    const vm_float_t kappa[] = { VM_F(1.0), VM_F(2.0), VM_F(0.5) };
    const vm_float_t s[] = { VM_F(0.4), VM_F(0.3), VM_F(1.0) };
    const vm_float_t sum = vm_ray_tau_sum(kappa, s, 3);
    const vm_float_t expect = VM_F(1.0) * VM_F(0.4) + VM_F(2.0) * VM_F(0.3) + VM_F(0.5) * VM_F(1.0);
    REQUIRE(VECMAT_EQ(sum, expect, EPSILON));
}
