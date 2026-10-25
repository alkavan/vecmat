// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "unitest.h"

static vm_sym3 sym_zero(void)
{
    vm_sym3 s;
    for (int i = 0; i < 6; ++i) {
        s.c[i] = VM_F(0.0);
    }
    return s;
}

TEST_CASE(hydro_viscous_pure_spin_is_zero, "[hydro]") {
    /* Pure spin has D = 0. */
    const vm_sym3 D = sym_zero();
    const vm_sym3 s = vm_hydro_viscous(&D, VM_F(1.5), VM_F(0.4));
    REQUIRE(VECMAT_EQ(s.xx, VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(s.yy, VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(s.zz, VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(s.xy, VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(s.yz, VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(s.zx, VM_F(0.0), EPSILON));
}

TEST_CASE(hydro_viscous_uniaxial, "[hydro]") {
    /* D = diag(1, 0, 0): σ_xx = 2μ + λ, shears zero. */
    vm_sym3 D = sym_zero();
    D.xx = VM_F(1.0);
    const vm_float_t mu = VM_F(2.0);
    const vm_float_t lambda = VM_F(3.0);
    const vm_sym3 s = vm_hydro_viscous(&D, mu, lambda);
    REQUIRE(VECMAT_EQ(s.xx, VM_F(2.0) * mu + lambda, EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(s.yy, lambda, EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(s.zz, lambda, EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(s.xy, VM_F(0.0), EPSILON));
}

TEST_CASE(hydro_q_zero_in_expansion, "[hydro]") {
    REQUIRE(VECMAT_EQ(vm_hydro_q(VM_F(1.2), VM_F(0.5), VM_F(2.0)), VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(vm_hydro_q(VM_F(1.2), VM_F(0.0), VM_F(2.0)), VM_F(0.0), EPSILON));
}

TEST_CASE(hydro_q_compression, "[hydro]") {
    const vm_float_t q = vm_hydro_q(VM_F(1.2), VM_F(-0.5), VM_F(2.0));
    REQUIRE(VECMAT_EQ(q, VM_F(2.0) * VM_F(1.2) * VM_F(0.25), EPSILON * VM_F(8.0)));
}

TEST_CASE(hydro_rusanov_symmetric_zero_mass, "[hydro]") {
    /* Equal density, pressure, sound speed; opposite normal velocity. */
    const vm_hydro_state left = {
        .rho = VM_F(1.2), .vn = VM_F(3.0), .e = VM_F(1.0e5),
        .p = VM_F(1.0e5), .c = VM_F(340.0)
    };
    const vm_hydro_state right = {
        .rho = VM_F(1.2), .vn = VM_F(-3.0), .e = VM_F(1.0e5),
        .p = VM_F(1.0e5), .c = VM_F(340.0)
    };
    vm_hydro_flux flux;
    REQUIRE(vm_hydro_rusanov(&flux, &left, &right));
    REQUIRE(VECMAT_EQ(flux.mass, VM_F(0.0), EPSILON * VM_F(64.0)));
}

TEST_CASE(hydro_rusanov_identical_no_dissipation, "[hydro]") {
    const vm_hydro_state st = {
        .rho = VM_F(1.0), .vn = VM_F(2.0), .e = VM_F(10.0),
        .p = VM_F(5.0), .c = VM_F(3.0)
    };
    vm_hydro_flux flux;
    REQUIRE(vm_hydro_rusanov(&flux, &st, &st));
    const vm_float_t et = st.e + VM_F(0.5) * st.vn * st.vn;
    REQUIRE(VECMAT_EQ(flux.mass, st.rho * st.vn, EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(flux.momentum, st.rho * st.vn * st.vn + st.p, EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(flux.energy, st.rho * st.vn * et + st.p * st.vn, EPSILON * VM_F(8.0)));
}

TEST_CASE(hydro_cfl_acoustic_and_viscous, "[hydro]") {
    const vm_float_t h = VM_F(0.1);
    const vm_float_t acoustic = vm_hydro_cfl(h, VM_F(2.0), VM_F(3.0), VM_F(0.5), VM_F(0.0));
    REQUIRE(VECMAT_EQ(acoustic, VM_F(0.5) * h / VM_F(5.0), EPSILON * acoustic));

    /* Viscous scale h^2/nu = 0.01/10 = 0.001, tighter than acoustic 0.01. */
    const vm_float_t clamped = vm_hydro_cfl(h, VM_F(2.0), VM_F(3.0), VM_F(0.5), VM_F(10.0));
    REQUIRE(VECMAT_EQ(clamped, h * h / VM_F(10.0), EPSILON * clamped));
}
