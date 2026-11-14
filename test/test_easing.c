// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "unitest.h"

TEST_CASE(test_smoothstep_edges_and_mid, "[easing][smoothstep]") {
    REQUIRE(VECMAT_EQ(vm_smoothstep(0.0f, 1.0f, 0.0f), 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(vm_smoothstep(0.0f, 1.0f, 1.0f), 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(vm_smoothstep(0.0f, 1.0f, 0.5f), 0.5f, EPSILON));
    REQUIRE(VECMAT_EQ(vm_smoothstep(0.0f, 1.0f, -0.5f), 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(vm_smoothstep(0.0f, 1.0f, 1.5f), 1.0f, EPSILON));
}

TEST_CASE(test_smoothstep_known_value, "[easing][smoothstep]") {
    // t=0.25 → 0.25^2 * (3 - 0.5) = 0.0625 * 2.5 = 0.15625
    REQUIRE(VECMAT_EQ(vm_smoothstep(0.0f, 1.0f, 0.25f), 0.15625f, EPSILON));
}

TEST_CASE(test_smootherstep_edges_and_mid, "[easing][smootherstep]") {
    REQUIRE(VECMAT_EQ(vm_smootherstep(0.0f, 1.0f, 0.0f), 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(vm_smootherstep(0.0f, 1.0f, 1.0f), 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(vm_smootherstep(0.0f, 1.0f, 0.5f), 0.5f, EPSILON));
    REQUIRE(VECMAT_EQ(vm_smootherstep(0.0f, 1.0f, -1.0f), 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(vm_smootherstep(0.0f, 1.0f, 2.0f), 1.0f, EPSILON));
}

TEST_CASE(test_smootherstep_known_value, "[easing][smootherstep]") {
    // t=0.25 → 0.25^3 * (0.25*(0.25*6 - 15) + 10) = 0.015625 * (0.25*(-13.5) + 10)
    // = 0.015625 * (-3.375 + 10) = 0.015625 * 6.625 = 0.103515625
    REQUIRE(VECMAT_EQ(vm_smootherstep(0.0f, 1.0f, 0.25f), 0.103515625f, EPSILON));
}

TEST_CASE(test_smoothstep_equal_edges, "[easing][smoothstep]") {
    REQUIRE(VECMAT_EQ(vm_smoothstep(2.0f, 2.0f, 1.0f), 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(vm_smoothstep(2.0f, 2.0f, 2.0f), 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(vm_smoothstep(2.0f, 2.0f, 3.0f), 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(vm_smootherstep(2.0f, 2.0f, 1.0f), 0.0f, EPSILON));
}

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4996)
#endif

TEST_CASE(test_easing_old_names_match_vm, "[easing]") {
    const vm_float_t samples[] = { VM_F(0.0), VM_F(0.25), VM_F(0.5), VM_F(1.0) };
    for (int i = 0; i < 4; ++i) {
        const vm_float_t f = samples[i];
        REQUIRE(VECMAT_EQ(elastic_oscillation(f, VM_F(-10.0), VM_F(10.0), VM_F(0.75), VM_F(3.0)),
                          vm_elastic_oscillation(f, VM_F(-10.0), VM_F(10.0), VM_F(0.75), VM_F(3.0)), EPSILON));
        REQUIRE(VECMAT_EQ(poly_ease_in(f, VM_F(2.0)), vm_poly_ease_in(f, VM_F(2.0)), EPSILON));
        REQUIRE(VECMAT_EQ(poly_ease_out(f, VM_F(3.0)), vm_poly_ease_out(f, VM_F(3.0)), EPSILON));
        REQUIRE(VECMAT_EQ(poly_ease_in_out(f, VM_F(2.0)), vm_poly_ease_in_out(f, VM_F(2.0)), EPSILON));
        REQUIRE(VECMAT_EQ(quadratic_ease_in(f), vm_quadratic_ease_in(f), EPSILON));
        REQUIRE(VECMAT_EQ(quadratic_ease_out(f), vm_quadratic_ease_out(f), EPSILON));
        REQUIRE(VECMAT_EQ(quadratic_ease_in_out(f), vm_quadratic_ease_in_out(f), EPSILON));
        REQUIRE(VECMAT_EQ(cubic_ease_in(f), vm_cubic_ease_in(f), EPSILON));
        REQUIRE(VECMAT_EQ(cubic_ease_out(f), vm_cubic_ease_out(f), EPSILON));
        REQUIRE(VECMAT_EQ(cubic_ease_in_out(f), vm_cubic_ease_in_out(f), EPSILON));
        REQUIRE(VECMAT_EQ(quartic_ease_in(f), vm_quartic_ease_in(f), EPSILON));
        REQUIRE(VECMAT_EQ(quartic_ease_out(f), vm_quartic_ease_out(f), EPSILON));
        REQUIRE(VECMAT_EQ(quartic_ease_in_out(f), vm_quartic_ease_in_out(f), EPSILON));
        REQUIRE(VECMAT_EQ(quintic_ease_in(f), vm_quintic_ease_in(f), EPSILON));
        REQUIRE(VECMAT_EQ(quintic_ease_out(f), vm_quintic_ease_out(f), EPSILON));
        REQUIRE(VECMAT_EQ(quintic_ease_in_out(f), vm_quintic_ease_in_out(f), EPSILON));
        REQUIRE(VECMAT_EQ(sine_ease_in(f), vm_sine_ease_in(f), EPSILON));
        REQUIRE(VECMAT_EQ(sine_ease_out(f), vm_sine_ease_out(f), EPSILON));
        REQUIRE(VECMAT_EQ(sine_ease_in_out(f), vm_sine_ease_in_out(f), EPSILON));
        REQUIRE(VECMAT_EQ(circular_ease_in(f), vm_circular_ease_in(f), EPSILON));
        REQUIRE(VECMAT_EQ(circular_ease_out(f), vm_circular_ease_out(f), EPSILON));
        REQUIRE(VECMAT_EQ(circular_ease_in_out(f), vm_circular_ease_in_out(f), EPSILON));
        REQUIRE(VECMAT_EQ(exponential_ease_in(f), vm_exponential_ease_in(f), EPSILON));
        REQUIRE(VECMAT_EQ(exponential_ease_out(f), vm_exponential_ease_out(f), EPSILON));
        REQUIRE(VECMAT_EQ(exponential_ease_in_out(f), vm_exponential_ease_in_out(f), EPSILON));
        REQUIRE(VECMAT_EQ(elastic_ease_in(f), vm_elastic_ease_in(f), EPSILON));
        REQUIRE(VECMAT_EQ(elastic_ease_out(f), vm_elastic_ease_out(f), EPSILON));
        REQUIRE(VECMAT_EQ(elastic_ease_in_out(f), vm_elastic_ease_in_out(f), EPSILON));
        REQUIRE(VECMAT_EQ(back_ease_in(f), vm_back_ease_in(f), EPSILON));
        REQUIRE(VECMAT_EQ(back_ease_out(f), vm_back_ease_out(f), EPSILON));
        REQUIRE(VECMAT_EQ(back_ease_in_out(f), vm_back_ease_in_out(f), EPSILON));
        REQUIRE(VECMAT_EQ(bounce_ease_in(f), vm_bounce_ease_in(f), EPSILON));
        REQUIRE(VECMAT_EQ(bounce_ease_out(f), vm_bounce_ease_out(f), EPSILON));
        REQUIRE(VECMAT_EQ(bounce_ease_in_out(f), vm_bounce_ease_in_out(f), EPSILON));
        REQUIRE(VECMAT_EQ(smoothstep(VM_F(0.0), VM_F(1.0), f), vm_smoothstep(VM_F(0.0), VM_F(1.0), f), EPSILON));
        REQUIRE(VECMAT_EQ(smootherstep(VM_F(0.0), VM_F(1.0), f), vm_smootherstep(VM_F(0.0), VM_F(1.0), f), EPSILON));
    }
    REQUIRE(VECMAT_EQ(vm_quadratic_ease_in(VM_F(0.0)), VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(vm_quadratic_ease_in(VM_F(1.0)), VM_F(1.0), EPSILON));
    REQUIRE(VECMAT_EQ(vm_quadratic_ease_out(VM_F(0.0)), VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(vm_quadratic_ease_out(VM_F(1.0)), VM_F(1.0), EPSILON));
}

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif
#if defined(_MSC_VER)
#pragma warning(pop)
#endif
