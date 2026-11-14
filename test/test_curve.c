// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "unitest.h"

TEST_CASE(curve_bezier_endpoints_and_midpoint, "[curve]") {
    REQUIRE(VECMAT_EQ(vm_bezier2(VM_F(2.0), VM_F(8.0), VM_F(4.0), VM_F(0.0)), VM_F(2.0), EPSILON));
    REQUIRE(VECMAT_EQ(vm_bezier2(VM_F(2.0), VM_F(8.0), VM_F(4.0), VM_F(1.0)), VM_F(4.0), EPSILON));
    /* 0.25*2 + 0.5*8 + 0.25*4 = 5.5 */
    REQUIRE(VECMAT_EQ(vm_bezier2(VM_F(2.0), VM_F(8.0), VM_F(4.0), VM_F(0.5)), VM_F(5.5), EPSILON * VM_F(8.0)));
    /* t is not clamped: u = -1, result = 4 * p2 when p0 = p1 = 0. */
    REQUIRE(VECMAT_EQ(vm_bezier2(VM_F(0.0), VM_F(0.0), VM_F(1.0), VM_F(2.0)), VM_F(4.0), EPSILON));

    REQUIRE(VECMAT_EQ(vm_bezier3(VM_F(1.0), VM_F(9.0), VM_F(-3.0), VM_F(5.0), VM_F(0.0)), VM_F(1.0), EPSILON));
    REQUIRE(VECMAT_EQ(vm_bezier3(VM_F(1.0), VM_F(9.0), VM_F(-3.0), VM_F(5.0), VM_F(1.0)), VM_F(5.0), EPSILON));
    /* (1 + 27 - 9 + 5) / 8 = 3 */
    REQUIRE(VECMAT_EQ(vm_bezier3(VM_F(1.0), VM_F(9.0), VM_F(-3.0), VM_F(5.0), VM_F(0.5)), VM_F(3.0),
                      EPSILON * VM_F(8.0)));
}

TEST_CASE(curve_hermite_and_catmull, "[curve]") {
    REQUIRE(VECMAT_EQ(vm_hermite(VM_F(0.0), VM_F(1.0), VM_F(0.0), VM_F(0.0), VM_F(0.0)), VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(vm_hermite(VM_F(0.0), VM_F(1.0), VM_F(0.0), VM_F(0.0), VM_F(1.0)), VM_F(1.0), EPSILON));
    REQUIRE(VECMAT_EQ(vm_hermite(VM_F(0.0), VM_F(1.0), VM_F(0.0), VM_F(0.0), VM_F(0.5)), VM_F(0.5),
                      EPSILON * VM_F(8.0)));

    REQUIRE(VECMAT_EQ(vm_catmull_rom(VM_F(4.0), VM_F(1.0), VM_F(2.0), VM_F(-3.0), VM_F(0.0)), VM_F(1.0), EPSILON));
    REQUIRE(VECMAT_EQ(vm_catmull_rom(VM_F(4.0), VM_F(1.0), VM_F(2.0), VM_F(-3.0), VM_F(1.0)), VM_F(2.0), EPSILON));
    REQUIRE(VECMAT_EQ(vm_catmull_rom(VM_F(0.0), VM_F(0.0), VM_F(1.0), VM_F(1.0), VM_F(0.5)), VM_F(0.5),
                      EPSILON * VM_F(8.0)));
}

TEST_CASE(curve_vector_matches_scalar, "[curve]") {
    const vector2 a2 = vec2(VM_F(2.0), VM_F(-1.0));
    const vector2 b2 = vec2(VM_F(8.0), VM_F(3.0));
    const vector2 c2 = vec2(VM_F(4.0), VM_F(0.5));
    const vector2 d2 = vec2(VM_F(5.0), VM_F(7.0));
    const vector2 q2 = vec2_bezier2(a2, b2, c2, VM_F(0.5));
    REQUIRE(VECMAT_EQ(q2.x, vm_bezier2(a2.x, b2.x, c2.x, VM_F(0.5)), EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(q2.y, vm_bezier2(a2.y, b2.y, c2.y, VM_F(0.5)), EPSILON * VM_F(8.0)));

    const vector2 c3 = vec2_bezier3(a2, b2, c2, d2, VM_F(0.25));
    REQUIRE(VECMAT_EQ(c3.x, vm_bezier3(a2.x, b2.x, c2.x, d2.x, VM_F(0.25)), EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(c3.y, vm_bezier3(a2.y, b2.y, c2.y, d2.y, VM_F(0.25)), EPSILON * VM_F(8.0)));

    const vector3 a3 = vec3(VM_F(0.0), VM_F(1.0), VM_F(2.0));
    const vector3 b3 = vec3(VM_F(3.0), VM_F(-1.0), VM_F(4.0));
    const vector3 c3v = vec3(VM_F(5.0), VM_F(2.0), VM_F(-2.0));
    const vector3 d3 = vec3(VM_F(1.0), VM_F(0.0), VM_F(6.0));
    const vector3 m0 = vec3(VM_F(1.0), VM_F(0.0), VM_F(-1.0));
    const vector3 m1 = vec3(VM_F(0.5), VM_F(2.0), VM_F(0.0));
    const vector3 h = vec3_hermite(a3, b3, m0, m1, VM_F(0.5));
    REQUIRE(VECMAT_EQ(h.x, vm_hermite(a3.x, b3.x, m0.x, m1.x, VM_F(0.5)), EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(h.y, vm_hermite(a3.y, b3.y, m0.y, m1.y, VM_F(0.5)), EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(h.z, vm_hermite(a3.z, b3.z, m0.z, m1.z, VM_F(0.5)), EPSILON * VM_F(8.0)));

    const vector3 cr = vec3_catmull_rom(a3, b3, c3v, d3, VM_F(0.5));
    REQUIRE(VECMAT_EQ(cr.x, vm_catmull_rom(a3.x, b3.x, c3v.x, d3.x, VM_F(0.5)), EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(cr.y, vm_catmull_rom(a3.y, b3.y, c3v.y, d3.y, VM_F(0.5)), EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(cr.z, vm_catmull_rom(a3.z, b3.z, c3v.z, d3.z, VM_F(0.5)), EPSILON * VM_F(8.0)));

    const vector2 cr2 = vec2_catmull_rom(a2, b2, c2, d2, VM_F(0.0));
    REQUIRE(VECMAT_EQ(cr2.x, b2.x, EPSILON));
    REQUIRE(VECMAT_EQ(cr2.y, b2.y, EPSILON));
    const vector2 end = vec2_hermite(a2, b2, c2, d2, VM_F(1.0));
    REQUIRE(VECMAT_EQ(end.x, b2.x, EPSILON));
    REQUIRE(VECMAT_EQ(end.y, b2.y, EPSILON));
}
