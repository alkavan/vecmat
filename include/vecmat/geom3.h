// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_GEOM3_H
#define VECMAT_GEOM3_H

/**
 * @file
 * @brief Closest-point, gap, edge-edge, and AABB predicates. No broadphase.
 * @ingroup vecmat_extras
 *
 * Degenerate triangles return `ok = false`. A point on the positive normal
 * side of a triangle has a positive gap. An interior query recovers the
 * barycentric combination.
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

#endif //VECMAT_GEOM3_H
