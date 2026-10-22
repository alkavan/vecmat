// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "unitest.h"

TEST_CASE(eos_ideal_recovers_sound_speed, "[eos]") {
    const vm_ideal_gas gas = { .gamma = VM_F(1.4) };
    const vm_float_t rho = VM_F(1.2);
    const vm_float_t e = VM_F(2.5e5);
    vm_eos_state st;
    REQUIRE(vm_eos_ideal(&st, &gas, rho, e));
    REQUIRE(st.ok);
    const vm_float_t p = (gas.gamma - VM_F(1.0)) * rho * e;
    REQUIRE(VECMAT_EQ(st.p, p, EPSILON * p));
    const vm_float_t c = VECMAT_SQRT(gas.gamma * p / rho);
    REQUIRE(VECMAT_EQ(st.c, c, EPSILON * c * VM_F(8.0)));
}

TEST_CASE(eos_zero_density_fails, "[eos]") {
    const vm_ideal_gas gas = { .gamma = VM_F(1.4) };
    vm_eos_state st;
    REQUIRE(!vm_eos_ideal(&st, &gas, VM_F(0.0), VM_F(1.0)));
    REQUIRE(!st.ok);
    const vm_stiff_gas stiff = { .gamma = VM_F(1.4), .p_inf = VM_F(1.0e5) };
    REQUIRE(!vm_eos_stiffened(&st, &stiff, VM_F(0.0), VM_F(1.0)));
    const vm_mie mie = { .rho0 = VM_F(1000.0), .c0 = VM_F(1500.0),
                         .s = VM_F(1.5), .gamma0 = VM_F(1.0) };
    REQUIRE(!vm_eos_mie(&st, &mie, VM_F(0.0), VM_F(0.0)));
    const vm_jwl jwl = { .rho0 = VM_F(1630.0), .a = VM_F(1.0), .b = VM_F(1.0),
                         .r1 = VM_F(4.0), .r2 = VM_F(1.0), .omega = VM_F(0.3) };
    REQUIRE(!vm_eos_jwl(&st, &jwl, VM_F(0.0), VM_F(0.0)));
}

TEST_CASE(eos_stiffened_reduces_to_ideal, "[eos]") {
    const vm_stiff_gas stiff = { .gamma = VM_F(1.4), .p_inf = VM_F(0.0) };
    const vm_float_t rho = VM_F(1.2);
    const vm_float_t e = VM_F(2.5e5);
    vm_eos_state st;
    REQUIRE(vm_eos_stiffened(&st, &stiff, rho, e));
    const vm_float_t p = (stiff.gamma - VM_F(1.0)) * rho * e;
    REQUIRE(VECMAT_EQ(st.p, p, EPSILON * p));
    REQUIRE(VECMAT_EQ(st.c, VECMAT_SQRT(stiff.gamma * p / rho), EPSILON * st.c * VM_F(8.0)));
}

TEST_CASE(eos_mie_reference_density, "[eos]") {
    /* At rho = rho0, mu = 0: p = Gamma0 * rho0 * e, c^2 = c0^2 + Gamma0 * p / rho. */
    const vm_mie mie = { .rho0 = VM_F(1000.0), .c0 = VM_F(1500.0),
                             .s = VM_F(1.5), .gamma0 = VM_F(2.0) };
    const vm_float_t e = VM_F(1.0e4);
    vm_eos_state st;
    REQUIRE(vm_eos_mie(&st, &mie, mie.rho0, e));
    const vm_float_t p = mie.gamma0 * mie.rho0 * e;
    REQUIRE(VECMAT_EQ(st.p, p, EPSILON * p * VM_F(8.0)));
    const vm_float_t c2 = mie.c0 * mie.c0 + mie.gamma0 * p / mie.rho0;
    REQUIRE(VECMAT_EQ(st.c, VECMAT_SQRT(c2), EPSILON * st.c * VM_F(16.0)));
}

TEST_CASE(eos_jwl_tnt_cj_point, "[eos]") {
    /*
     * TNT, Dobratz / common tabulated set:
     *   rho0 = 1630 kg/m^3, D = 6930 m/s, p_CJ = 21.0 GPa,
     *   A = 373.8 GPa, B = 3.747 GPa, R1 = 4.15, R2 = 0.90, omega = 0.35,
     *   E0 = 6.00 GPa (energy per reference volume).
     * V_CJ from the Rayleigh line; E_CJ = E0 + 1/2 p_CJ (1 - V_CJ).
     */
    const vm_jwl tnt = {
        .rho0 = VM_F(1630.0),
        .a = VM_F(373.8e9),
        .b = VM_F(3.747e9),
        .r1 = VM_F(4.15),
        .r2 = VM_F(0.90),
        .omega = VM_F(0.35)
    };
    const vm_float_t d_cj = VM_F(6930.0);
    const vm_float_t p_cj = VM_F(21.0e9);
    const vm_float_t v_cj = VM_F(1.0) - p_cj / (tnt.rho0 * d_cj * d_cj);
    const vm_float_t e0 = VM_F(6.00e9) / tnt.rho0;
    const vm_float_t e_cj = e0 + VM_F(0.5) * p_cj * (VM_F(1.0) - v_cj) / tnt.rho0;
    const vm_float_t rho_cj = tnt.rho0 / v_cj;
    vm_eos_state st;
    REQUIRE(vm_eos_jwl(&st, &tnt, rho_cj, e_cj));
    REQUIRE(st.ok);
    REQUIRE(VECMAT_EQ(st.p, p_cj, p_cj * VM_F(1.0e-3)));
}
