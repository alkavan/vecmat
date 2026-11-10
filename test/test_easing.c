// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "unitest.h"

TEST_CASE(test_smoothstep_edges_and_mid, "[easing][smoothstep]") {
    REQUIRE(VECMAT_EQ(smoothstep(0.0f, 1.0f, 0.0f), 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(smoothstep(0.0f, 1.0f, 1.0f), 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(smoothstep(0.0f, 1.0f, 0.5f), 0.5f, EPSILON));
    REQUIRE(VECMAT_EQ(smoothstep(0.0f, 1.0f, -0.5f), 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(smoothstep(0.0f, 1.0f, 1.5f), 1.0f, EPSILON));
}

TEST_CASE(test_smoothstep_known_value, "[easing][smoothstep]") {
    // t=0.25 → 0.25^2 * (3 - 0.5) = 0.0625 * 2.5 = 0.15625
    REQUIRE(VECMAT_EQ(smoothstep(0.0f, 1.0f, 0.25f), 0.15625f, EPSILON));
}

TEST_CASE(test_smootherstep_edges_and_mid, "[easing][smootherstep]") {
    REQUIRE(VECMAT_EQ(smootherstep(0.0f, 1.0f, 0.0f), 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(smootherstep(0.0f, 1.0f, 1.0f), 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(smootherstep(0.0f, 1.0f, 0.5f), 0.5f, EPSILON));
    REQUIRE(VECMAT_EQ(smootherstep(0.0f, 1.0f, -1.0f), 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(smootherstep(0.0f, 1.0f, 2.0f), 1.0f, EPSILON));
}

TEST_CASE(test_smootherstep_known_value, "[easing][smootherstep]") {
    // t=0.25 → 0.25^3 * (0.25*(0.25*6 - 15) + 10) = 0.015625 * (0.25*(-13.5) + 10)
    // = 0.015625 * (-3.375 + 10) = 0.015625 * 6.625 = 0.103515625
    REQUIRE(VECMAT_EQ(smootherstep(0.0f, 1.0f, 0.25f), 0.103515625f, EPSILON));
}

TEST_CASE(test_smoothstep_equal_edges, "[easing][smoothstep]") {
    REQUIRE(VECMAT_EQ(smoothstep(2.0f, 2.0f, 1.0f), 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(smoothstep(2.0f, 2.0f, 2.0f), 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(smoothstep(2.0f, 2.0f, 3.0f), 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(smootherstep(2.0f, 2.0f, 1.0f), 0.0f, EPSILON));
}
