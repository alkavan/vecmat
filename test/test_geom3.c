// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "unitest.h"

static vector3 v3(const vm_float_t x, const vm_float_t y, const vm_float_t z)
{
    const vector3 v = { .x = x, .y = y, .z = z };
    return v;
}

TEST_CASE(geom3_interior_matches_barycentric, "[geom3]") {
    const vector3 a = v3(VM_F(0.0), VM_F(0.0), VM_F(0.0));
    const vector3 b = v3(VM_F(2.0), VM_F(0.0), VM_F(0.0));
    const vector3 c = v3(VM_F(0.0), VM_F(2.0), VM_F(0.0));
    const vm_float_t u = VM_F(0.2);
    const vm_float_t v = VM_F(0.5);
    const vm_float_t w = VM_F(0.3);
    const vector3 p = vec3_add(vec3_add(vec3_mul_scalar(a, u), vec3_mul_scalar(b, v)),
                               vec3_mul_scalar(c, w));

    vm_geom3_hit hit;
    REQUIRE(vm_geom3_closest_triangle(&hit, &p, &a, &b, &c));
    REQUIRE(hit.ok);
    REQUIRE(VECMAT_EQ(hit.closest.x, p.x, EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(hit.closest.y, p.y, EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(hit.closest.z, p.z, EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(hit.bary.x, u, EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(hit.bary.y, v, EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(hit.bary.z, w, EPSILON * VM_F(8.0)));

    vector3 bary;
    REQUIRE(vm_geom3_barycentric(&bary, &p, &a, &b, &c));
    REQUIRE(VECMAT_EQ(bary.x, u, EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(bary.y, v, EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(bary.z, w, EPSILON * VM_F(8.0)));
}

TEST_CASE(geom3_positive_gap_on_positive_normal, "[geom3]") {
    const vector3 a = v3(VM_F(0.0), VM_F(0.0), VM_F(0.0));
    const vector3 b = v3(VM_F(1.0), VM_F(0.0), VM_F(0.0));
    const vector3 c = v3(VM_F(0.0), VM_F(1.0), VM_F(0.0));
    /* Normal is +z. Point above the interior. */
    const vector3 p = v3(VM_F(0.3), VM_F(0.3), VM_F(0.5));
    vm_geom3_gap gap;
    REQUIRE(vm_geom3_point_triangle_gap(&gap, &p, &a, &b, &c));
    REQUIRE(gap.ok);
    REQUIRE(gap.gap > VM_F(0.0));
    REQUIRE(VECMAT_EQ(gap.gap, VM_F(0.5), EPSILON * VM_F(8.0)));
}

TEST_CASE(geom3_negative_gap_on_negative_side, "[geom3]") {
    const vector3 a = v3(VM_F(0.0), VM_F(0.0), VM_F(0.0));
    const vector3 b = v3(VM_F(1.0), VM_F(0.0), VM_F(0.0));
    const vector3 c = v3(VM_F(0.0), VM_F(1.0), VM_F(0.0));
    const vector3 p = v3(VM_F(0.3), VM_F(0.3), VM_F(-0.4));
    vm_geom3_gap gap;
    REQUIRE(vm_geom3_point_triangle_gap(&gap, &p, &a, &b, &c));
    REQUIRE(gap.gap < VM_F(0.0));
    REQUIRE(VECMAT_EQ(gap.gap, VM_F(-0.4), EPSILON * VM_F(8.0)));
}

TEST_CASE(geom3_degenerate_triangle_fails, "[geom3]") {
    const vector3 a = v3(VM_F(0.0), VM_F(0.0), VM_F(0.0));
    const vector3 b = v3(VM_F(1.0), VM_F(0.0), VM_F(0.0));
    const vector3 c = v3(VM_F(2.0), VM_F(0.0), VM_F(0.0)); /* collinear */
    const vector3 p = v3(VM_F(0.5), VM_F(0.5), VM_F(0.0));
    vm_geom3_hit hit;
    REQUIRE(!vm_geom3_closest_triangle(&hit, &p, &a, &b, &c));
    REQUIRE(!hit.ok);
    vector3 bary;
    REQUIRE(!vm_geom3_barycentric(&bary, &p, &a, &b, &c));
    vm_geom3_gap gap;
    REQUIRE(!vm_geom3_point_triangle_gap(&gap, &p, &a, &b, &c));
}

TEST_CASE(geom3_edge_edge_skew, "[geom3]") {
    /* Edges along x and y, closest at the origin, distance 0. */
    const vector3 p0 = v3(VM_F(-1.0), VM_F(0.0), VM_F(0.0));
    const vector3 p1 = v3(VM_F(1.0), VM_F(0.0), VM_F(0.0));
    const vector3 q0 = v3(VM_F(0.0), VM_F(-1.0), VM_F(0.0));
    const vector3 q1 = v3(VM_F(0.0), VM_F(1.0), VM_F(0.0));
    vm_geom3_edge ee;
    REQUIRE(vm_geom3_edge_edge(&ee, &p0, &p1, &q0, &q1));
    REQUIRE(ee.ok);
    REQUIRE(VECMAT_EQ(ee.distance, VM_F(0.0), EPSILON * VM_F(8.0)));
}

TEST_CASE(geom3_edge_edge_parallel, "[geom3]") {
    /* Parallel edges, distance 1 in z. */
    const vector3 p0 = v3(VM_F(0.0), VM_F(0.0), VM_F(0.0));
    const vector3 p1 = v3(VM_F(2.0), VM_F(0.0), VM_F(0.0));
    const vector3 q0 = v3(VM_F(0.5), VM_F(0.0), VM_F(1.0));
    const vector3 q1 = v3(VM_F(1.5), VM_F(0.0), VM_F(1.0));
    vm_geom3_edge ee;
    REQUIRE(vm_geom3_edge_edge(&ee, &p0, &p1, &q0, &q1));
    REQUIRE(ee.ok);
    REQUIRE(VECMAT_EQ(ee.distance, VM_F(1.0), EPSILON * VM_F(8.0)));
}

TEST_CASE(geom3_aabb_overlap_and_triangle, "[geom3]") {
    const vector3 a = v3(VM_F(0.0), VM_F(0.0), VM_F(0.0));
    const vector3 b = v3(VM_F(2.0), VM_F(0.0), VM_F(1.0));
    const vector3 c = v3(VM_F(0.0), VM_F(3.0), VM_F(0.0));
    const vm_aabb box = vm_geom3_aabb_triangle(&a, &b, &c);
    REQUIRE(VECMAT_EQ(box.min.x, VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(box.max.x, VM_F(2.0), EPSILON));
    REQUIRE(VECMAT_EQ(box.max.y, VM_F(3.0), EPSILON));
    REQUIRE(VECMAT_EQ(box.max.z, VM_F(1.0), EPSILON));

    const vm_aabb other = { .min = v3(VM_F(1.0), VM_F(1.0), VM_F(0.0)),
                            .max = v3(VM_F(4.0), VM_F(4.0), VM_F(2.0)) };
    REQUIRE(vm_geom3_aabb_overlap(&box, &other));
    const vm_aabb far = { .min = v3(VM_F(10.0), VM_F(10.0), VM_F(10.0)),
                          .max = v3(VM_F(11.0), VM_F(11.0), VM_F(11.0)) };
    REQUIRE(!vm_geom3_aabb_overlap(&box, &far));
}

TEST_CASE(geom3_cell_index_inside_and_outside, "[geom3]") {
    const vm_grid3 g = vm_grid3_make(4, 4, 2, VM_F(1.0), VM_F(1.0), VM_F(1.0));
    const vector3 inside = v3(VM_F(1.5), VM_F(2.2), VM_F(0.1));
    int i, j, k;
    REQUIRE(vm_geom3_cell_index(&i, &j, &k, &inside, &g));
    REQUIRE(i == 1);
    REQUIRE(j == 2);
    REQUIRE(k == 0);
    REQUIRE(vm_grid_cell(g, i, j, k) == i + 4 * j + 16 * k);

    const vector3 outside = v3(VM_F(-0.1), VM_F(1.0), VM_F(0.0));
    REQUIRE(!vm_geom3_cell_index(&i, &j, &k, &outside, &g));

    /* Far boundary belongs to the last cell. */
    const vector3 far = v3(VM_F(4.0), VM_F(4.0), VM_F(2.0));
    REQUIRE(vm_geom3_cell_index(&i, &j, &k, &far, &g));
    REQUIRE(i == 3);
    REQUIRE(j == 3);
    REQUIRE(k == 1);
}
