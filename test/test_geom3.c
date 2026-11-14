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

TEST_CASE(geom3_point_plane_signed_distance, "[geom3]") {
    const vector3 p = v3(VM_F(0.0), VM_F(0.0), VM_F(2.0));
    const vector3 n = v3(VM_F(0.0), VM_F(0.0), VM_F(2.0));
    vm_geom3_plane hit;
    REQUIRE(vm_geom3_point_plane(&hit, &p, &n, VM_F(0.0)));
    REQUIRE(hit.ok);
    REQUIRE(VECMAT_EQ(hit.distance, VM_F(2.0), EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(hit.point.z, VM_F(0.0), EPSILON * VM_F(8.0)));

    /* Plane 2 z = 2, so z = 1. Point at z = -1.5 is 2.5 below the unit normal. */
    const vector3 below = v3(VM_F(1.0), VM_F(-3.0), VM_F(-1.5));
    REQUIRE(vm_geom3_point_plane(&hit, &below, &n, VM_F(2.0)));
    REQUIRE(VECMAT_EQ(hit.distance, VM_F(-2.5), EPSILON * VM_F(8.0)));

    const vector3 zero = vec3_zero();
    REQUIRE(!vm_geom3_point_plane(&hit, &p, &zero, VM_F(0.0)));
    REQUIRE(!hit.ok);
}

TEST_CASE(geom3_closest_segment_clamps, "[geom3]") {
    const vector3 a = v3(VM_F(0.0), VM_F(0.0), VM_F(0.0));
    const vector3 b = v3(VM_F(2.0), VM_F(0.0), VM_F(0.0));
    const vector3 mid = v3(VM_F(1.0), VM_F(1.0), VM_F(0.0));
    vm_geom3_segment seg;
    REQUIRE(vm_geom3_closest_segment(&seg, &mid, &a, &b));
    REQUIRE(seg.ok);
    REQUIRE(VECMAT_EQ(seg.t, VM_F(0.5), EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(seg.point.x, VM_F(1.0), EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(seg.point.y, VM_F(0.0), EPSILON));

    const vector3 before = v3(VM_F(-1.0), VM_F(4.0), VM_F(0.0));
    REQUIRE(vm_geom3_closest_segment(&seg, &before, &a, &b));
    REQUIRE(VECMAT_EQ(seg.t, VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(seg.point.x, VM_F(0.0), EPSILON));

    REQUIRE(!vm_geom3_closest_segment(&seg, &mid, &a, &a));
    REQUIRE(!seg.ok);
}

TEST_CASE(geom3_ray_triangle_hit_and_miss, "[geom3]") {
    const vector3 a = v3(VM_F(0.0), VM_F(0.0), VM_F(0.0));
    const vector3 b = v3(VM_F(1.0), VM_F(0.0), VM_F(0.0));
    const vector3 c = v3(VM_F(0.0), VM_F(1.0), VM_F(0.0));
    const vector3 origin = v3(VM_F(0.25), VM_F(0.25), VM_F(1.0));
    const vector3 dir = v3(VM_F(0.0), VM_F(0.0), VM_F(-1.0));
    vm_geom3_ray hit;
    REQUIRE(vm_geom3_ray_triangle(&hit, &origin, &dir, &a, &b, &c));
    REQUIRE(hit.ok);
    REQUIRE(hit.hit);
    REQUIRE(VECMAT_EQ(hit.t, VM_F(1.0), EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(hit.point.x, VM_F(0.25), EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(hit.point.y, VM_F(0.25), EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(hit.point.z, VM_F(0.0), EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(hit.bary.x, VM_F(0.5), EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(hit.bary.y, VM_F(0.25), EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(hit.bary.z, VM_F(0.25), EPSILON * VM_F(8.0)));

    const vector3 back = v3(VM_F(0.0), VM_F(0.0), VM_F(1.0));
    REQUIRE(vm_geom3_ray_triangle(&hit, &origin, &back, &a, &b, &c));
    REQUIRE(hit.ok);
    REQUIRE(!hit.hit);

    const vector3 beside = v3(VM_F(2.0), VM_F(2.0), VM_F(1.0));
    REQUIRE(vm_geom3_ray_triangle(&hit, &beside, &dir, &a, &b, &c));
    REQUIRE(hit.ok);
    REQUIRE(!hit.hit);

    const vector3 col = v3(VM_F(2.0), VM_F(0.0), VM_F(0.0));
    REQUIRE(!vm_geom3_ray_triangle(&hit, &origin, &dir, &a, &b, &col));
    REQUIRE(!hit.ok);
    REQUIRE(!vm_geom3_ray_triangle(&hit, &origin, &a, &a, &b, &c));
}

TEST_CASE(geom3_ray_aabb_entry_and_inside, "[geom3]") {
    const vm_aabb box = { .min = v3(VM_F(0.0), VM_F(0.0), VM_F(0.0)),
                          .max = v3(VM_F(1.0), VM_F(1.0), VM_F(1.0)) };
    const vector3 origin = v3(VM_F(-1.0), VM_F(0.5), VM_F(0.5));
    const vector3 dir = v3(VM_F(1.0), VM_F(0.0), VM_F(0.0));
    vm_geom3_ray hit;
    REQUIRE(vm_geom3_ray_aabb(&hit, &origin, &dir, &box));
    REQUIRE(hit.hit);
    REQUIRE(VECMAT_EQ(hit.t, VM_F(1.0), EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(hit.point.x, VM_F(0.0), EPSILON * VM_F(8.0)));

    const vector3 inside = v3(VM_F(0.25), VM_F(0.25), VM_F(0.25));
    REQUIRE(vm_geom3_ray_aabb(&hit, &inside, &dir, &box));
    REQUIRE(hit.hit);
    REQUIRE(VECMAT_EQ(hit.t, VM_F(0.0), EPSILON));

    const vector3 miss = v3(VM_F(-1.0), VM_F(2.0), VM_F(0.5));
    REQUIRE(vm_geom3_ray_aabb(&hit, &miss, &dir, &box));
    REQUIRE(hit.ok);
    REQUIRE(!hit.hit);

    const vector3 behind = v3(VM_F(2.0), VM_F(0.5), VM_F(0.5));
    REQUIRE(vm_geom3_ray_aabb(&hit, &behind, &dir, &box));
    REQUIRE(hit.ok);
    REQUIRE(!hit.hit);

    const vm_aabb bad = { .min = v3(VM_F(1.0), VM_F(0.0), VM_F(0.0)),
                          .max = v3(VM_F(0.0), VM_F(1.0), VM_F(1.0)) };
    REQUIRE(!vm_geom3_ray_aabb(&hit, &origin, &dir, &bad));
    REQUIRE(!hit.ok);
}

TEST_CASE(geom3_ray_sphere_near_and_exit, "[geom3]") {
    const vector3 center = vec3_zero();
    const vector3 origin = v3(VM_F(-5.0), VM_F(0.0), VM_F(0.0));
    const vector3 dir = v3(VM_F(1.0), VM_F(0.0), VM_F(0.0));
    vm_geom3_ray hit;
    REQUIRE(vm_geom3_ray_sphere(&hit, &origin, &dir, &center, VM_F(1.0)));
    REQUIRE(hit.hit);
    REQUIRE(VECMAT_EQ(hit.t, VM_F(4.0), EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(hit.point.x, VM_F(-1.0), EPSILON * VM_F(8.0)));

    const vector3 inside = vec3_zero();
    REQUIRE(vm_geom3_ray_sphere(&hit, &inside, &dir, &center, VM_F(1.0)));
    REQUIRE(hit.hit);
    REQUIRE(VECMAT_EQ(hit.t, VM_F(1.0), EPSILON * VM_F(8.0)));

    const vector3 miss_o = v3(VM_F(-5.0), VM_F(3.0), VM_F(0.0));
    REQUIRE(vm_geom3_ray_sphere(&hit, &miss_o, &dir, &center, VM_F(1.0)));
    REQUIRE(hit.ok);
    REQUIRE(!hit.hit);
    REQUIRE(!vm_geom3_ray_sphere(&hit, &origin, &dir, &center, VM_F(-1.0)));
    REQUIRE(!hit.ok);
}

TEST_CASE(geom3_ray_plane_hit_and_parallel, "[geom3]") {
    const vector3 n = v3(VM_F(0.0), VM_F(0.0), VM_F(1.0));
    const vector3 origin = v3(VM_F(1.0), VM_F(2.0), VM_F(4.0));
    const vector3 dir = v3(VM_F(0.0), VM_F(0.0), VM_F(-2.0));
    vm_geom3_ray hit;
    REQUIRE(vm_geom3_ray_plane(&hit, &origin, &dir, &n, VM_F(0.0)));
    REQUIRE(hit.hit);
    REQUIRE(VECMAT_EQ(hit.t, VM_F(2.0), EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(hit.point.z, VM_F(0.0), EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(hit.point.x, VM_F(1.0), EPSILON * VM_F(8.0)));

    const vector3 along = v3(VM_F(1.0), VM_F(0.0), VM_F(0.0));
    REQUIRE(vm_geom3_ray_plane(&hit, &origin, &along, &n, VM_F(0.0)));
    REQUIRE(hit.ok);
    REQUIRE(!hit.hit);

    const vector3 on = v3(VM_F(3.0), VM_F(0.0), VM_F(0.0));
    REQUIRE(vm_geom3_ray_plane(&hit, &on, &along, &n, VM_F(0.0)));
    REQUIRE(hit.hit);
    REQUIRE(VECMAT_EQ(hit.t, VM_F(0.0), EPSILON));

    const vector3 zero = vec3_zero();
    REQUIRE(!vm_geom3_ray_plane(&hit, &origin, &dir, &zero, VM_F(0.0)));
}

TEST_CASE(geom3_sphere_sphere_solid, "[geom3]") {
    const vector3 a = vec3_zero();
    const vector3 b = v3(VM_F(3.0), VM_F(0.0), VM_F(0.0));
    vm_geom3_spheres hit;
    REQUIRE(vm_geom3_sphere_sphere(&hit, &a, VM_F(1.0), &b, VM_F(1.0)));
    REQUIRE(hit.ok);
    REQUIRE(!hit.hit);
    REQUIRE(VECMAT_EQ(hit.distance, VM_F(3.0), EPSILON * VM_F(8.0)));

    const vector3 c = v3(VM_F(1.5), VM_F(0.0), VM_F(0.0));
    REQUIRE(vm_geom3_sphere_sphere(&hit, &a, VM_F(1.0), &c, VM_F(1.0)));
    REQUIRE(hit.hit);
    REQUIRE(VECMAT_EQ(hit.distance, VM_F(1.5), EPSILON * VM_F(8.0)));

    REQUIRE(vm_geom3_sphere_sphere(&hit, &a, VM_F(2.0), &a, VM_F(0.5)));
    REQUIRE(hit.hit);
    REQUIRE(VECMAT_EQ(hit.distance, VM_F(0.0), EPSILON));

    REQUIRE(!vm_geom3_sphere_sphere(&hit, &a, VM_F(-0.1), &b, VM_F(1.0)));
    REQUIRE(!hit.ok);
}

TEST_CASE(geom3_plane_plane_line_and_parallel, "[geom3]") {
    const vector3 nz = v3(VM_F(0.0), VM_F(0.0), VM_F(1.0));
    const vector3 ny = v3(VM_F(0.0), VM_F(0.0), VM_F(0.0));
    const vector3 ny_axis = v3(VM_F(0.0), VM_F(1.0), VM_F(0.0));
    vm_geom3_planes line;
    REQUIRE(vm_geom3_plane_plane(&line, &nz, VM_F(0.0), &ny_axis, VM_F(0.0)));
    REQUIRE(line.ok);
    REQUIRE(line.hit);
    REQUIRE(VECMAT_EQ(VECMAT_FABS(line.direction.x), VM_F(1.0), EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(line.direction.y, VM_F(0.0), EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(line.point.y, VM_F(0.0), EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(line.point.z, VM_F(0.0), EPSILON * VM_F(8.0)));

    REQUIRE(vm_geom3_plane_plane(&line, &nz, VM_F(0.0), &nz, VM_F(1.0)));
    REQUIRE(line.ok);
    REQUIRE(!line.hit);

    REQUIRE(!vm_geom3_plane_plane(&line, &nz, VM_F(0.0), &ny, VM_F(0.0)));
    REQUIRE(!line.ok);
}

TEST_CASE(geom3_aabb_contains_union_expand, "[geom3]") {
    const vm_aabb box = { .min = v3(VM_F(0.0), VM_F(0.0), VM_F(0.0)),
                          .max = v3(VM_F(1.0), VM_F(2.0), VM_F(3.0)) };
    const vector3 on_face = v3(VM_F(1.0), VM_F(2.0), VM_F(3.0));
    const vector3 out = v3(VM_F(1.1), VM_F(0.0), VM_F(0.0));
    REQUIRE(vm_geom3_aabb_contains(&box, &on_face));
    REQUIRE(!vm_geom3_aabb_contains(&box, &out));

    const vm_aabb inverted = { .min = v3(VM_F(1.0), VM_F(0.0), VM_F(0.0)),
                               .max = v3(VM_F(0.0), VM_F(1.0), VM_F(1.0)) };
    REQUIRE(!vm_geom3_aabb_contains(&inverted, &on_face));

    const vm_aabb other = { .min = v3(VM_F(-2.0), VM_F(1.0), VM_F(4.0)),
                            .max = v3(VM_F(0.5), VM_F(5.0), VM_F(4.0)) };
    const vm_aabb uni = vm_geom3_aabb_union(&box, &other);
    REQUIRE(VECMAT_EQ(uni.min.x, VM_F(-2.0), EPSILON));
    REQUIRE(VECMAT_EQ(uni.min.y, VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(uni.max.y, VM_F(5.0), EPSILON));
    REQUIRE(VECMAT_EQ(uni.max.z, VM_F(4.0), EPSILON));
    REQUIRE(VECMAT_EQ(uni.min.z, VM_F(0.0), EPSILON));

    const vm_aabb dropped = vm_geom3_aabb_union(&inverted, &box);
    REQUIRE(VECMAT_EQ(dropped.max.y, box.max.y, EPSILON));

    const vector3 extra = v3(VM_F(3.0), VM_F(-1.0), VM_F(1.0));
    const vm_aabb grown = vm_geom3_aabb_expand(&box, &extra);
    REQUIRE(vm_geom3_aabb_contains(&grown, &extra));
    REQUIRE(vm_geom3_aabb_contains(&grown, &box.min));
    REQUIRE(VECMAT_EQ(grown.min.y, VM_F(-1.0), EPSILON));
    REQUIRE(VECMAT_EQ(grown.max.x, VM_F(3.0), EPSILON));

    const vm_aabb from_empty = vm_geom3_aabb_expand(&inverted, &extra);
    REQUIRE(VECMAT_EQ(from_empty.min.x, extra.x, EPSILON));
    REQUIRE(VECMAT_EQ(from_empty.max.z, extra.z, EPSILON));
}
