// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

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
