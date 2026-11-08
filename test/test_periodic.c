// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <math.h>
#include <vecmat.h>
#include "unitest.h"

static vm_float_t neg_zero(void)
{
    return VECMAT_COPYSIGN(VM_F(0.0), VM_F(-1.0));
}

static vector3 unit_box(void)
{
    return vec3(VM_F(1.0), VM_F(1.0), VM_F(1.0));
}

TEST_CASE(periodic_delta_ties_and_range, "[periodic]") {
    const vector3 box = unit_box();
    const vector3 a = vec3(VM_F(0.2), VM_F(0.8), VM_F(0.5));
    const vector3 b = vec3(VM_F(0.0), VM_F(0.0), VM_F(0.0));
    vector3 d = vec3(VM_F(9.0), VM_F(9.0), VM_F(9.0));
    REQUIRE(vm_periodic_delta_ortho(&d, &a, &b, box, 1));
    REQUIRE(VECMAT_EQ(d.x, VM_F(0.2), EPSILON));
    REQUIRE(VECMAT_EQ(d.y, VM_F(-0.2), EPSILON));
    REQUIRE(VECMAT_EQ(d.x, VECMAT_REMAINDER(a.x - b.x, box.x), EPSILON));
    REQUIRE(VECMAT_EQ(d.y, VECMAT_REMAINDER(a.y - b.y, box.y), EPSILON));
    REQUIRE(d.x >= VM_F(-0.5) && d.x <= VM_F(0.5));
    REQUIRE(d.y >= VM_F(-0.5) && d.y <= VM_F(0.5));

    const vector3 ties = vec3(VM_F(0.5), VM_F(1.5), VM_F(-1.5));
    REQUIRE(vm_periodic_delta_ortho(&d, &ties, &b, box, 1));
    REQUIRE(VECMAT_EQ(d.x, VM_F(0.5), EPSILON));
    REQUIRE(VECMAT_EQ(d.y, VM_F(-0.5), EPSILON));
    REQUIRE(VECMAT_EQ(d.z, VM_F(0.5), EPSILON));

    const vector3 off = vec3(VM_F(0.2), VM_F(0.0), VM_F(0.0));
    vector3 ab, ba;
    REQUIRE(vm_periodic_delta_ortho(&ab, &off, &b, box, 1));
    REQUIRE(vm_periodic_delta_ortho(&ba, &b, &off, box, 1));
    REQUIRE(VECMAT_EQ(ab.x, -ba.x, EPSILON));
    REQUIRE(VECMAT_EQ(ab.y, -ba.y, EPSILON));
    REQUIRE(VECMAT_EQ(ab.z, -ba.z, EPSILON));
}

TEST_CASE(periodic_delta_axes_alias_and_nan, "[periodic]") {
    const vector3 box = vec3(VM_F(2.0), VM_F(4.0), VM_F(8.0));
    vector3 a[1];
    vector3 b[1];
    a[0] = vec3(VM_F(1.5), VM_F(3.0), VM_F(7.0));
    b[0] = vec3(VM_F(0.0), VM_F(0.0), VM_F(0.0));
    vector3 d[1];
    REQUIRE(vm_periodic_delta_ortho(d, a, b, box, 1));
    REQUIRE(VECMAT_EQ(d[0].x, VM_F(-0.5), EPSILON));
    REQUIRE(VECMAT_EQ(d[0].y, VM_F(-1.0), EPSILON));
    REQUIRE(VECMAT_EQ(d[0].z, VM_F(-1.0), EPSILON));

    a[0] = vec3(VM_F(1.5), VM_F(0.25), VM_F(-0.25));
    b[0] = vec3(VM_F(0.0), VM_F(0.0), VM_F(0.0));
    REQUIRE(vm_periodic_delta_ortho(a, a, b, unit_box(), 1));
    REQUIRE(VECMAT_EQ(a[0].x, VM_F(-0.5), EPSILON));
    REQUIRE(VECMAT_EQ(a[0].y, VM_F(0.25), EPSILON));
    REQUIRE(VECMAT_EQ(a[0].z, VM_F(-0.25), EPSILON));

    const vector3 nan_a = vec3(NAN, VM_F(0.2), INFINITY);
    const vector3 origin = vec3(VM_F(0.0), VM_F(0.0), VM_F(0.0));
    vector3 out = vec3(VM_F(1.0), VM_F(2.0), VM_F(3.0));
    REQUIRE(vm_periodic_delta_ortho(&out, &nan_a, &origin, unit_box(), 1));
    REQUIRE(isnan(out.x));
    REQUIRE(VECMAT_EQ(out.y, VM_F(0.2), EPSILON));
    REQUIRE(isnan(out.z));
}

TEST_CASE(periodic_rejects_bad_box_without_writes, "[periodic]") {
    const vector3 a = vec3(VM_F(0.2), VM_F(0.2), VM_F(0.2));
    const vector3 b = vec3(VM_F(0.0), VM_F(0.0), VM_F(0.0));
    vector3 d = vec3(VM_F(1.0), VM_F(2.0), VM_F(3.0));
    REQUIRE(!vm_periodic_delta_ortho(&d, &a, &b, vec3(VM_F(0.0), VM_F(1.0), VM_F(1.0)), 1));
    REQUIRE(!vm_periodic_delta_ortho(&d, &a, &b, vec3(VM_F(-1.0), VM_F(1.0), VM_F(1.0)), 1));
    REQUIRE(!vm_periodic_delta_ortho(&d, &a, &b, vec3(NAN, VM_F(1.0), VM_F(1.0)), 1));
    REQUIRE(!vm_periodic_delta_ortho(&d, &a, &b, unit_box(), -1));
    REQUIRE(VECMAT_EQ(d.x, VM_F(1.0), EPSILON));
    REQUIRE(VECMAT_EQ(d.y, VM_F(2.0), EPSILON));
    REQUIRE(VECMAT_EQ(d.z, VM_F(3.0), EPSILON));
    REQUIRE(vm_periodic_delta_ortho(NULL, NULL, NULL, unit_box(), 0));
    REQUIRE(!vm_periodic_delta_ortho(NULL, NULL, NULL, unit_box(), 1));

    vector3 w = vec3(VM_F(1.25), VM_F(0.0), VM_F(0.0));
    REQUIRE(!vm_wrap_ortho(&w, vec3(VM_F(1.0), VM_F(0.0), VM_F(1.0)), 1));
    REQUIRE(VECMAT_EQ(w.x, VM_F(1.25), EPSILON));
    REQUIRE(vm_wrap_ortho(NULL, unit_box(), 0));
    REQUIRE(!vm_wrap_ortho(NULL, unit_box(), 2));
}

TEST_CASE(wrap_ortho_range_and_signed_zero, "[periodic]") {
    vector3 w[2];
    w[0] = vec3(VM_F(1.5), VM_F(-0.25), VM_F(1.0));
    w[1] = vec3(neg_zero(), VM_F(5.25), VM_F(-5.25));
    REQUIRE(vm_wrap_ortho(w, unit_box(), 2));
    REQUIRE(VECMAT_EQ(w[0].x, VM_F(0.5), EPSILON));
    REQUIRE(VECMAT_EQ(w[0].y, VM_F(0.75), EPSILON));
    REQUIRE(w[0].z == VM_F(0.0) && !signbit(w[0].z));
    REQUIRE(w[1].x == VM_F(0.0) && !signbit(w[1].x));
    REQUIRE(VECMAT_EQ(w[1].y, VM_F(0.25), EPSILON));
    REQUIRE(VECMAT_EQ(w[1].z, VM_F(0.75), EPSILON));

    vector3 wide[1];
    wide[0] = vec3(VM_F(3.5), VM_F(-1.0), VM_F(8.0));
    const vector3 box = vec3(VM_F(2.0), VM_F(4.0), VM_F(8.0));
    REQUIRE(vm_wrap_ortho(wide, box, 1));
    REQUIRE(VECMAT_EQ(wide[0].x, VM_F(1.5), EPSILON));
    REQUIRE(VECMAT_EQ(wide[0].y, VM_F(3.0), EPSILON));
    REQUIRE(wide[0].z == VM_F(0.0) && !signbit(wide[0].z));
    REQUIRE(wide[0].x >= VM_F(0.0) && wide[0].x < box.x);
    REQUIRE(wide[0].y >= VM_F(0.0) && wide[0].y < box.y);
}
