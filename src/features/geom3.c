// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <math.h>
#include <vecmat.h>
#include "vecmat/geom3.h"

// TODO: should be refactored as `vm_clamp` (scalar version), part of the core library.
static vm_float_t clampf(const vm_float_t x, const vm_float_t lo, const vm_float_t hi)
{
    if (x < lo) {
        return lo;
    }

    if (x > hi) {
        return hi;
    }

    return x;
}

static bool triangle_degenerate(const vector3 *a, const vector3 *b, const vector3 *c)
{
    const vector3 ab = vec3_sub(*b, *a);
    const vector3 ac = vec3_sub(*c, *a);
    const vector3 n = vec3_cross(ab, ac);

    const vm_float_t area2 = vec3_dot(n, n);
    const vm_float_t scale = vec3_length_squared(ab) + vec3_length_squared(ac) + VM_F(1.0);

    return area2 <= VECMAT_EPSILON * scale;
}

bool vm_geom3_barycentric(vector3 *bary, const vector3 *p,
                          const vector3 *a, const vector3 *b, const vector3 *c)
{
    if (!bary || !p || !a || !b || !c) {
        return false;
    }

    if (triangle_degenerate(a, b, c)) {
        return false;
    }

    const vector3 ab = vec3_sub(*b, *a);
    const vector3 ac = vec3_sub(*c, *a);
    const vector3 n = vec3_cross(ab, ac);
    const vm_float_t nn = vec3_dot(n, n);
    const vector3 ap = vec3_sub(*p, *a);

    const vm_float_t v = vec3_dot(vec3_cross(ap, ac), n) / nn;
    const vm_float_t w = vec3_dot(vec3_cross(ab, ap), n) / nn;
    const vm_float_t u = VM_F(1.0) - v - w;

    bary->x = u;
    bary->y = v;
    bary->z = w;

    return true;
}

bool vm_geom3_closest_triangle(vm_geom3_hit *hit, const vector3 *p,
                               const vector3 *a, const vector3 *b, const vector3 *c)
{
    if (!hit) {
        return false;
    }

    hit->ok = false;
    if (!p || !a || !b || !c || triangle_degenerate(a, b, c)) {
        return false;
    }

    const vector3 ab = vec3_sub(*b, *a);
    const vector3 ac = vec3_sub(*c, *a);
    const vector3 ap = vec3_sub(*p, *a);
    const vm_float_t d1 = vec3_dot(ab, ap);
    const vm_float_t d2 = vec3_dot(ac, ap);
    if (d1 <= VM_F(0.0) && d2 <= VM_F(0.0)) {
        hit->closest = *a;
        hit->bary = vec3_x_axis(VM_F(1.0));
        hit->ok = true;
        return true;
    }

    const vector3 bp = vec3_sub(*p, *b);
    const vm_float_t d3 = vec3_dot(ab, bp);
    const vm_float_t d4 = vec3_dot(ac, bp);
    if (d3 >= VM_F(0.0) && d4 <= d3) {
        hit->closest = *b;
        hit->bary = vec3_y_axis(VM_F(1.0));
        hit->ok = true;
        return true;
    }

    const vm_float_t vc = d1 * d4 - d3 * d2;
    if (vc <= VM_F(0.0) && d1 >= VM_F(0.0) && d3 <= VM_F(0.0)) {
        const vm_float_t v = d1 / (d1 - d3);
        hit->closest = vec3_add(*a, vec3_mul_scalar(ab, v));
        hit->bary.x = VM_F(1.0) - v;
        hit->bary.y = v;
        hit->bary.z = VM_F(0.0);
        hit->ok = true;
        return true;
    }

    const vector3 cp = vec3_sub(*p, *c);
    const vm_float_t d5 = vec3_dot(ab, cp);
    const vm_float_t d6 = vec3_dot(ac, cp);
    if (d6 >= VM_F(0.0) && d5 <= d6) {
        hit->closest = *c;
        hit->bary = vec3_z_axis(VM_F(1.0));
        hit->ok = true;
        return true;
    }

    const vm_float_t vb = d5 * d2 - d1 * d6;
    if (vb <= VM_F(0.0) && d2 >= VM_F(0.0) && d6 <= VM_F(0.0)) {
        const vm_float_t w = d2 / (d2 - d6);
        hit->closest = vec3_add(*a, vec3_mul_scalar(ac, w));
        hit->bary.x = VM_F(1.0) - w;
        hit->bary.y = VM_F(0.0);
        hit->bary.z = w;
        hit->ok = true;
        return true;
    }

    const vm_float_t va = d3 * d6 - d5 * d4;
    if (va <= VM_F(0.0) && (d4 - d3) >= VM_F(0.0) && (d5 - d6) >= VM_F(0.0)) {
        const vm_float_t w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
        hit->closest = vec3_add(*b, vec3_mul_scalar(vec3_sub(*c, *b), w));
        hit->bary.x = VM_F(0.0);
        hit->bary.y = VM_F(1.0) - w;
        hit->bary.z = w;
        hit->ok = true;
        return true;
    }

    const vm_float_t denom = VM_F(1.0) / (va + vb + vc);
    const vm_float_t v = vb * denom;
    const vm_float_t w = vc * denom;
    const vm_float_t u = VM_F(1.0) - v - w;
    hit->closest = vec3_add(*a, vec3_add(vec3_mul_scalar(ab, v), vec3_mul_scalar(ac, w)));
    hit->bary.x = u;
    hit->bary.y = v;
    hit->bary.z = w;
    hit->ok = true;
    return true;
}

bool vm_geom3_point_triangle_gap(vm_geom3_gap *out, const vector3 *p,
                                 const vector3 *a, const vector3 *b, const vector3 *c)
{
    if (!out) {
        return false;
    }

    out->ok = false;
    if (!p || !a || !b || !c || triangle_degenerate(a, b, c)) {
        return false;
    }

    vm_geom3_hit hit;
    if (!vm_geom3_closest_triangle(&hit, p, a, b, c)) {
        return false;
    }

    const vector3 ab = vec3_sub(*b, *a);
    const vector3 ac = vec3_sub(*c, *a);
    const vector3 n = vec3_normalize(vec3_cross(ab, ac));
    const vector3 diff = vec3_sub(*p, hit.closest);

    out->ok = true;
    out->gap = vec3_dot(diff, n);
    out->on_tri = hit.closest;
    out->query = *p;

    return true;
}

bool vm_geom3_edge_edge(vm_geom3_edge *out,
                        const vector3 *p0, const vector3 *p1,
                        const vector3 *q0, const vector3 *q1)
{
    if (!out) {
        return false;
    }
    out->ok = false;

    if (!p0 || !p1 || !q0 || !q1) {
        return false;
    }

    const vector3 d1 = vec3_sub(*p1, *p0);
    const vector3 d2 = vec3_sub(*q1, *q0);
    const vm_float_t a = vec3_dot(d1, d1);
    const vm_float_t e = vec3_dot(d2, d2);

    if (a <= VECMAT_EPSILON || e <= VECMAT_EPSILON) {
        return false;
    }

    const vector3 r = vec3_sub(*p0, *q0);
    const vm_float_t f = vec3_dot(d2, r);
    const vm_float_t b = vec3_dot(d1, d2);
    const vm_float_t c = vec3_dot(d1, r);
    const vm_float_t denom = a * e - b * b;

    vm_float_t s;
    if (denom > VECMAT_EPSILON * (a * e + VM_F(1.0))) {
        s = clampf((b * f - c * e) / denom, VM_F(0.0), VM_F(1.0));
    } else {
        s = VM_F(0.0);
    }

    vm_float_t t = (b * s + f) / e;
    if (t < VM_F(0.0)) {
        t = VM_F(0.0);
        s = clampf(-c / a, VM_F(0.0), VM_F(1.0));
    } else if (t > VM_F(1.0)) {
        t = VM_F(1.0);
        s = clampf((b - c) / a, VM_F(0.0), VM_F(1.0));
    }

    out->on_a = vec3_add(*p0, vec3_mul_scalar(d1, s));
    out->on_b = vec3_add(*q0, vec3_mul_scalar(d2, t));
    out->distance = vec3_distance(out->on_a, out->on_b);
    out->ok = true;

    return true;
}

vm_aabb vm_geom3_aabb_triangle(const vector3 *a, const vector3 *b, const vector3 *c)
{
    vm_aabb box;
    if (!a || !b || !c) {
        box.min = vec3_zero();
        box.max = vec3_zero();
        return box;
    }

    box.min = vec3_min(vec3_min(*a, *b), *c);
    box.max = vec3_max(vec3_max(*a, *b), *c);

    return box;
}

bool vm_geom3_aabb_overlap(const vm_aabb *a, const vm_aabb *b)
{
    if (!a || !b) {
        return false;
    }

    return a->min.x <= b->max.x && b->min.x <= a->max.x
        && a->min.y <= b->max.y && b->min.y <= a->max.y
        && a->min.z <= b->max.z && b->min.z <= a->max.z;
}

bool vm_geom3_cell_index(int *i, int *j, int *k,
                         const vector3 *p, const vm_grid3 *g)
{
    if (!i || !j || !k || !p || !g) {
        return false;
    }

    if (g->nx <= 0 || g->ny <= 0 || g->nz <= 0
        || g->dx <= VM_F(0.0) || g->dy <= VM_F(0.0) || g->dz <= VM_F(0.0)) {
        return false;
    }

    const vm_float_t x_max = (vm_float_t)g->nx * g->dx;
    const vm_float_t y_max = (vm_float_t)g->ny * g->dy;
    const vm_float_t z_max = (vm_float_t)g->nz * g->dz;
    if (p->x < VM_F(0.0) || p->y < VM_F(0.0) || p->z < VM_F(0.0)
        || p->x > x_max || p->y > y_max || p->z > z_max) {
        return false;
    }

    int ii = (int)VECMAT_FLOOR(p->x / g->dx);
    int jj = (int)VECMAT_FLOOR(p->y / g->dy);
    int kk = (int)VECMAT_FLOOR(p->z / g->dz);

    if (ii >= g->nx) {
        ii = g->nx - 1;
    }
    if (jj >= g->ny) {
        jj = g->ny - 1;
    }
    if (kk >= g->nz) {
        kk = g->nz - 1;
    }

    *i = ii;
    *j = jj;
    *k = kk;

    return true;
}

static bool vec3_degenerate(const vector3 v)
{
    return vec3_length_squared(v) <= VECMAT_EPSILON;
}

static bool aabb_inverted(const vm_aabb *box)
{
    return box->min.x > box->max.x
        || box->min.y > box->max.y
        || box->min.z > box->max.z;
}

static vm_aabb aabb_empty(void)
{
    vm_aabb box;
    box.min = vec3_one();
    box.max = vec3_zero();
    return box;
}

static void ray_reset(vm_geom3_ray *out)
{
    out->ok = false;
    out->hit = false;
    out->t = VM_F(0.0);
    out->point = vec3_zero();
    out->bary = vec3_zero();
}

static vector3 ray_at(const vector3 *origin, const vector3 *direction, const vm_float_t t)
{
    return vec3_add(*origin, vec3_mul_scalar(*direction, t));
}

bool vm_geom3_point_plane(vm_geom3_plane *out, const vector3 *p,
                          const vector3 *normal, const vm_float_t offset)
{
    if (!out) {
        return false;
    }

    out->ok = false;
    out->distance = VM_F(0.0);
    out->point = vec3_zero();
    if (!p || !normal || vec3_degenerate(*normal)) {
        return false;
    }

    const vector3 n = vec3_normalize(*normal);
    const vm_float_t signed_off = offset / vec3_length(*normal);
    out->distance = vec3_dot(n, *p) - signed_off;
    out->point = vec3_sub(*p, vec3_mul_scalar(n, out->distance));
    out->ok = true;
    return true;
}

bool vm_geom3_closest_segment(vm_geom3_segment *out, const vector3 *p,
                              const vector3 *a, const vector3 *b)
{
    if (!out) {
        return false;
    }

    out->ok = false;
    out->t = VM_F(0.0);
    out->point = vec3_zero();
    if (!p || !a || !b) {
        return false;
    }

    const vector3 ab = vec3_sub(*b, *a);
    const vm_float_t ab2 = vec3_dot(ab, ab);
    if (ab2 <= VECMAT_EPSILON) {
        return false;
    }

    const vm_float_t t = clampf(vec3_dot(vec3_sub(*p, *a), ab) / ab2, VM_F(0.0), VM_F(1.0));
    out->t = t;
    out->point = vec3_add(*a, vec3_mul_scalar(ab, t));
    out->ok = true;
    return true;
}

bool vm_geom3_ray_triangle(vm_geom3_ray *out,
                           const vector3 *origin, const vector3 *direction,
                           const vector3 *a, const vector3 *b, const vector3 *c)
{
    if (!out) {
        return false;
    }
    ray_reset(out);
    if (!origin || !direction || !a || !b || !c || vec3_degenerate(*direction)
        || triangle_degenerate(a, b, c)) {
        return false;
    }

    const vector3 ab = vec3_sub(*b, *a);
    const vector3 ac = vec3_sub(*c, *a);
    const vector3 p_vec = vec3_cross(*direction, ac);
    const vm_float_t det = vec3_dot(ab, p_vec);
    const vm_float_t scale = vec3_length(*direction) * (vec3_length(ab) + vec3_length(ac)) + VM_F(1.0);
    if (VECMAT_FABS(det) <= VECMAT_EPSILON * scale) {
        out->ok = true;
        return true;
    }

    const vm_float_t inv_det = VM_F(1.0) / det;
    const vector3 t_vec = vec3_sub(*origin, *a);
    const vm_float_t u = vec3_dot(t_vec, p_vec) * inv_det;
    if (u < VM_F(0.0) || u > VM_F(1.0)) {
        out->ok = true;
        return true;
    }

    const vector3 q_vec = vec3_cross(t_vec, ab);
    const vm_float_t v = vec3_dot(*direction, q_vec) * inv_det;
    if (v < VM_F(0.0) || u + v > VM_F(1.0)) {
        out->ok = true;
        return true;
    }

    const vm_float_t t = vec3_dot(ac, q_vec) * inv_det;
    out->ok = true;
    if (t < VM_F(0.0)) {
        return true;
    }

    out->hit = true;
    out->t = t;
    out->point = ray_at(origin, direction, t);
    out->bary.x = VM_F(1.0) - u - v;
    out->bary.y = u;
    out->bary.z = v;

    return true;
}

bool vm_geom3_ray_aabb(vm_geom3_ray *out,
                       const vector3 *origin, const vector3 *direction,
                       const vm_aabb *box)
{
    if (!out) {
        return false;
    }
    ray_reset(out);

    if (!origin || !direction || !box || vec3_degenerate(*direction) || aabb_inverted(box)) {
        return false;
    }

    const vm_float_t o[3] = { origin->x, origin->y, origin->z };
    const vm_float_t d[3] = { direction->x, direction->y, direction->z };
    const vm_float_t lo[3] = { box->min.x, box->min.y, box->min.z };
    const vm_float_t hi[3] = { box->max.x, box->max.y, box->max.z };

    vm_float_t tmin = VM_F(0.0);
    vm_float_t tmax = INFINITY;
    for (int axis = 0; axis < 3; ++axis) {
        if (VECMAT_FABS(d[axis]) <= VECMAT_EPSILON) {
            if (o[axis] < lo[axis] || o[axis] > hi[axis]) {
                out->ok = true;
                return true;
            }
            continue;
        }

        vm_float_t t1 = (lo[axis] - o[axis]) / d[axis];
        vm_float_t t2 = (hi[axis] - o[axis]) / d[axis];
        if (t1 > t2) {
            const vm_float_t swap = t1;
            t1 = t2;
            t2 = swap;
        }

        if (t1 > tmin) {
            tmin = t1;
        }

        if (t2 < tmax) {
            tmax = t2;
        }

        if (tmin > tmax) {
            out->ok = true;
            return true;
        }
    }

    out->ok = true;
    if (tmax < VM_F(0.0)) {
        return true;
    }

    const vm_float_t t = tmin >= VM_F(0.0) ? tmin : VM_F(0.0);
    out->hit = true;
    out->t = t;
    out->point = ray_at(origin, direction, t);

    return true;
}

bool vm_geom3_ray_sphere(vm_geom3_ray *out,
                         const vector3 *origin, const vector3 *direction,
                         const vector3 *center, const vm_float_t radius)
{
    if (!out) {
        return false;
    }
    ray_reset(out);

    if (!origin || !direction || !center || radius < VM_F(0.0) || vec3_degenerate(*direction)) {
        return false;
    }

    const vector3 oc = vec3_sub(*origin, *center);
    const vm_float_t a = vec3_dot(*direction, *direction);
    const vm_float_t half_b = vec3_dot(oc, *direction);
    const vm_float_t c = vec3_dot(oc, oc) - radius * radius;
    const vm_float_t disc = half_b * half_b - a * c;
    out->ok = true;
    if (disc < VM_F(0.0)) {
        return true;
    }

    const vm_float_t root = VECMAT_SQRT(disc);
    const vm_float_t t0 = (-half_b - root) / a;
    const vm_float_t t1 = (-half_b + root) / a;
    vm_float_t t;
    if (t0 >= VM_F(0.0)) {
        t = t0;
    } else if (t1 >= VM_F(0.0)) {
        t = t1;
    } else {
        return true;
    }

    out->hit = true;
    out->t = t;
    out->point = ray_at(origin, direction, t);

    return true;
}

bool vm_geom3_ray_plane(vm_geom3_ray *out,
                        const vector3 *origin, const vector3 *direction,
                        const vector3 *normal, const vm_float_t offset)
{
    if (!out) {
        return false;
    }
    ray_reset(out);

    if (!origin || !direction || !normal || vec3_degenerate(*normal) || vec3_degenerate(*direction)) {
        return false;
    }

    const vm_float_t denom = vec3_dot(*normal, *direction);
    const vm_float_t scale = vec3_length(*normal) * vec3_length(*direction) + VM_F(1.0);
    const vm_float_t at_origin = vec3_dot(*normal, *origin) - offset;
    out->ok = true;
    if (VECMAT_FABS(denom) <= VECMAT_EPSILON * scale) {
        if (VECMAT_FABS(at_origin) <= VECMAT_EPSILON * scale) {
            out->hit = true;
            out->t = VM_F(0.0);
            out->point = *origin;
        }
        return true;
    }

    const vm_float_t t = -at_origin / denom;
    if (t < VM_F(0.0)) {
        return true;
    }

    out->hit = true;
    out->t = t;
    out->point = ray_at(origin, direction, t);

    return true;
}

bool vm_geom3_sphere_sphere(vm_geom3_spheres *out,
                            const vector3 *c0, const vm_float_t r0,
                            const vector3 *c1, const vm_float_t r1)
{
    if (!out) {
        return false;
    }

    out->ok = false;
    out->hit = false;
    out->distance = VM_F(0.0);
    if (!c0 || !c1 || r0 < VM_F(0.0) || r1 < VM_F(0.0)) {
        return false;
    }

    out->distance = vec3_distance(*c0, *c1);
    out->hit = out->distance <= r0 + r1;
    out->ok = true;

    return true;
}

bool vm_geom3_plane_plane(vm_geom3_planes *out,
                          const vector3 *n0, const vm_float_t offset0,
                          const vector3 *n1, const vm_float_t offset1)
{
    if (!out) {
        return false;
    }

    out->ok = false;
    out->hit = false;
    out->point = vec3_zero();
    out->direction = vec3_zero();
    if (!n0 || !n1 || vec3_degenerate(*n0) || vec3_degenerate(*n1)) {
        return false;
    }

    const vector3 cross = vec3_cross(*n0, *n1);
    const vm_float_t n00 = vec3_dot(*n0, *n0);
    const vm_float_t n11 = vec3_dot(*n1, *n1);
    const vm_float_t n01 = vec3_dot(*n0, *n1);
    const vm_float_t det = n00 * n11 - n01 * n01;
    out->ok = true;
    if (det <= VECMAT_EPSILON * (n00 * n11 + VM_F(1.0))) {
        return true;
    }

    const vm_float_t a = (offset0 * n11 - offset1 * n01) / det;
    const vm_float_t b = (offset1 * n00 - offset0 * n01) / det;
    out->hit = true;
    out->point = vec3_add(vec3_mul_scalar(*n0, a), vec3_mul_scalar(*n1, b));
    out->direction = vec3_normalize(cross);

    return true;
}

bool vm_geom3_aabb_contains(const vm_aabb *box, const vector3 *p)
{
    if (!box || !p || aabb_inverted(box)) {
        return false;
    }

    return p->x >= box->min.x && p->x <= box->max.x
        && p->y >= box->min.y && p->y <= box->max.y
        && p->z >= box->min.z && p->z <= box->max.z;
}

vm_aabb vm_geom3_aabb_union(const vm_aabb *a, const vm_aabb *b)
{
    const bool a_ok = a && !aabb_inverted(a);
    const bool b_ok = b && !aabb_inverted(b);
    if (!a_ok && !b_ok) {
        return aabb_empty();
    }
    if (!a_ok) {
        return *b;
    }
    if (!b_ok) {
        return *a;
    }

    vm_aabb box;
    box.min = vec3_min(a->min, b->min);
    box.max = vec3_max(a->max, b->max);
    return box;
}

vm_aabb vm_geom3_aabb_expand(const vm_aabb *box, const vector3 *p)
{
    if (!p) {
        if (box && !aabb_inverted(box)) {
            return *box;
        }
        return aabb_empty();
    }
    if (!box || aabb_inverted(box)) {
        vm_aabb point;
        point.min = *p;
        point.max = *p;
        return point;
    }

    vm_aabb grown;
    grown.min = vec3_min(box->min, *p);
    grown.max = vec3_max(box->max, *p);
    return grown;
}
