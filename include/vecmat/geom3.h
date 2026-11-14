// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_GEOM3_H
#define VECMAT_GEOM3_H

/**
 * @file
 * @brief Closest-point, gap, ray, and AABB predicates. No broadphase.
 * @ingroup vecmat_extras
 *
 * Degenerate input returns `ok = false`. A well-posed miss keeps `ok` true
 * and `hit` false. A point on the positive normal side of a triangle has a
 * positive gap. An interior query recovers the barycentric combination.
 * Rays are `origin + t * direction` with `t >= 0`. A plane is
 * `dot(normal, x) = offset`; the normal need not be unit.
 */

#include "vecmat/types.h"
#include "vecmat/abi.h"

/**
 * @brief Closest point on a triangle and its clamped barycentric coordinates.
 *
 * `bary.x + bary.y + bary.z == 1` and each component is in `[0, 1]`.
 */
typedef struct {
    bool ok;           /**< false if the triangle is degenerate. */
    vector3 closest;   /**< Closest point on the triangle. */
    vector3 bary;      /**< Clamped barycentric weights (u, v, w) for (a, b, c). */
} vm_geom3_hit;

/**
 * @brief Signed point-triangle gap and the two closest points.
 *
 * `gap` is positive when the query lies on the side of the triangle normal
 * `(b - a) × (c - a)`.
 */
typedef struct {
    bool ok;           /**< false if the triangle is degenerate. */
    vm_float_t gap;    /**< Signed gap. */
    vector3 on_tri;    /**< Closest point on the triangle. */
    vector3 query;     /**< The query point. */
} vm_geom3_gap;

/**
 * @brief Closest points and distance between two edges.
 *
 * Handles the parallel-edge case.
 */
typedef struct {
    bool ok;              /**< false if either edge has zero length. */
    vm_float_t distance;  /**< Distance between the closest points. */
    vector3 on_a;         /**< Closest point on edge a. */
    vector3 on_b;         /**< Closest point on edge b. */
} vm_geom3_edge;

/**
 * @brief Axis-aligned box. Closed interval `[min, max]` on each axis.
 */
typedef struct {
    vector3 min;
    vector3 max;
} vm_aabb;

/**
 * @brief Ray hit. `t` is the smallest nonnegative parameter.
 *
 * `bary` is the triangle weights `(u, v, w)` for `(a, b, c)`. It is zero
 * for every other primitive.
 */
typedef struct {
    bool ok;           /**< false if the query is degenerate. */
    bool hit;          /**< true when a point with `t >= 0` exists. */
    vm_float_t t;      /**< Ray parameter. Zero when `hit` is false. */
    vector3 point;     /**< `origin + t * direction` when `hit` is true. */
    vector3 bary;      /**< Triangle weights, or zero. */
} vm_geom3_ray;

/**
 * @brief Signed point-plane distance.
 *
 * `distance` is positive on the side of `normal`.
 */
typedef struct {
    bool ok;              /**< false if the normal is zero. */
    vm_float_t distance;  /**< Signed distance. */
    vector3 point;        /**< Closest point on the plane. */
} vm_geom3_plane;

/**
 * @brief Closest point on a segment `a + t (b - a)`, `t` in `[0, 1]`.
 */
typedef struct {
    bool ok;           /**< false if the segment has zero length. */
    vm_float_t t;      /**< Parameter in `[0, 1]`. */
    vector3 point;     /**< Closest point on the segment. */
} vm_geom3_segment;

/**
 * @brief Solid-ball overlap of two spheres.
 *
 * One ball inside the other without the surfaces touching is still a hit.
 */
typedef struct {
    bool ok;              /**< false if either radius is negative. */
    bool hit;             /**< true when the balls overlap or touch. */
    vm_float_t distance;  /**< Distance between centers. */
} vm_geom3_spheres;

/**
 * @brief Intersection line of two planes.
 *
 * Parallel planes, including coincident planes, are not a line: `ok` stays
 * true and `hit` is false.
 */
typedef struct {
    bool ok;            /**< false if either normal is zero. */
    bool hit;           /**< true when the planes meet in one line. */
    vector3 point;      /**< A point on the line. */
    vector3 direction;  /**< Unit direction `normalize(n0 × n1)`. */
} vm_geom3_planes;

/**
 * @brief Unclamped barycentric coordinates of a point in the triangle plane.
 *
 * The point is projected onto the plane of `(a, b, c)`. Weights need not
 * lie in `[0, 1]`.
 *
 * @return false if the triangle is degenerate.
 */
VEC_API bool vm_geom3_barycentric(vector3 *bary, const vector3 *p,
                                  const vector3 *a, const vector3 *b, const vector3 *c);

/**
 * @brief Closest point on a triangle, with clamped barycentric coordinates.
 *
 * @return false if the triangle is degenerate (`hit->ok` is also false).
 */
VEC_API bool vm_geom3_closest_triangle(vm_geom3_hit *hit, const vector3 *p,
                                       const vector3 *a, const vector3 *b, const vector3 *c);

/**
 * @brief Signed gap from a point to a triangle, and the closest points.
 *
 * @return false if the triangle is degenerate.
 */
VEC_API bool vm_geom3_point_triangle_gap(vm_geom3_gap *out, const vector3 *p,
                                         const vector3 *a, const vector3 *b, const vector3 *c);

/**
 * @brief Closest points and distance between two line segments.
 *
 * Includes the parallel-edge case.
 *
 * @return false if either segment has zero length.
 */
VEC_API bool vm_geom3_edge_edge(vm_geom3_edge *out,
                                const vector3 *p0, const vector3 *p1,
                                const vector3 *q0, const vector3 *q1);

/**
 * @brief AABB of a triangle.
 */
VEC_API vm_aabb vm_geom3_aabb_triangle(const vector3 *a, const vector3 *b, const vector3 *c);

/**
 * @brief Closed AABB overlap test.
 */
VEC_API bool vm_geom3_aabb_overlap(const vm_aabb *a, const vm_aabb *b);

/**
 * @brief Cell index of a point in an existing grid extent.
 *
 * Grid origin is the origin. Cell `(i, j, k)` covers
 * `[i dx, (i+1) dx) × [j dy, (j+1) dy) × [k dz, (k+1) dz)`.
 * A point on the far boundary is assigned to the last cell.
 * No broadphase object is created. Use `vm_grid_cell` for the linear index.
 *
 * @return false if the grid is invalid or the point is outside the extent.
 */
VEC_API bool vm_geom3_cell_index(int *i, int *j, int *k,
                                 const vector3 *p, const vm_grid3 *g);

/**
 * @brief Signed distance from a point to the plane `dot(normal, x) = offset`.
 *
 * @return false if `normal` is zero (`out->ok` is also false).
 */
VEC_API bool vm_geom3_point_plane(vm_geom3_plane *out, const vector3 *p,
                                  const vector3 *normal, vm_float_t offset);

/**
 * @brief Closest point on the segment from `a` to `b`.
 *
 * @return false if the segment has zero length.
 */
VEC_API bool vm_geom3_closest_segment(vm_geom3_segment *out, const vector3 *p,
                                      const vector3 *a, const vector3 *b);

/**
 * @brief Two-sided ray-triangle hit (Möller–Trumbore).
 *
 * A degenerate triangle or a zero direction returns false. A miss of a
 * real triangle returns true with `hit` false.
 *
 * @return false if the query is degenerate.
 */
VEC_API bool vm_geom3_ray_triangle(vm_geom3_ray *out,
                                   const vector3 *origin, const vector3 *direction,
                                   const vector3 *a, const vector3 *b, const vector3 *c);

/**
 * @brief Ray against a closed AABB (slab method).
 *
 * An inverted box (`min` greater than `max` on any axis) or a zero
 * direction returns false. A ray that starts inside hits at `t = 0`.
 *
 * @return false if the query is degenerate.
 */
VEC_API bool vm_geom3_ray_aabb(vm_geom3_ray *out,
                               const vector3 *origin, const vector3 *direction,
                               const vm_aabb *box);

/**
 * @brief Nearest sphere-surface hit with `t >= 0`.
 *
 * A negative radius or a zero direction returns false. A ray that starts
 * inside the ball hits the exit point.
 *
 * @return false if the query is degenerate.
 */
VEC_API bool vm_geom3_ray_sphere(vm_geom3_ray *out,
                                 const vector3 *origin, const vector3 *direction,
                                 const vector3 *center, vm_float_t radius);

/**
 * @brief Ray against the plane `dot(normal, x) = offset`.
 *
 * A zero normal or a zero direction returns false. A ray lying in the
 * plane hits at `t = 0`.
 *
 * @return false if the query is degenerate.
 */
VEC_API bool vm_geom3_ray_plane(vm_geom3_ray *out,
                                const vector3 *origin, const vector3 *direction,
                                const vector3 *normal, vm_float_t offset);

/**
 * @brief Solid overlap of two spheres.
 *
 * @return false if either radius is negative.
 */
VEC_API bool vm_geom3_sphere_sphere(vm_geom3_spheres *out,
                                    const vector3 *c0, vm_float_t r0,
                                    const vector3 *c1, vm_float_t r1);

/**
 * @brief Intersection line of two planes `dot(n, x) = offset`.
 *
 * @return false if either normal is zero.
 */
VEC_API bool vm_geom3_plane_plane(vm_geom3_planes *out,
                                  const vector3 *n0, vm_float_t offset0,
                                  const vector3 *n1, vm_float_t offset1);

/**
 * @brief Closed AABB contains a point.
 *
 * An inverted box contains nothing.
 */
VEC_API bool vm_geom3_aabb_contains(const vm_aabb *box, const vector3 *p);

/**
 * @brief Union of two closed AABBs.
 *
 * An inverted box is empty and is dropped. Two empty inputs return an
 * inverted box.
 */
VEC_API vm_aabb vm_geom3_aabb_union(const vm_aabb *a, const vm_aabb *b);

/**
 * @brief Grow a closed AABB so it contains `p`.
 *
 * An inverted or null box becomes the point box.
 */
VEC_API vm_aabb vm_geom3_aabb_expand(const vm_aabb *box, const vector3 *p);

#endif //VECMAT_GEOM3_H
