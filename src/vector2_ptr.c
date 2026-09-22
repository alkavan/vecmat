// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

void vec2_add_ptr(vector2 *res, const vector2 *a, const vector2 *b)
{
    res->x = a->x + b->x;
    res->y = a->y + b->y;
}

void vec2_sub_ptr(vector2 *res, const vector2 *a, const vector2 *b)
{
    res->x = a->x - b->x;
    res->y = a->y - b->y;
}

void vec2_mul_scalar_ptr(vector2 *res, const vector2 *v, const vm_float_t s)
{
    res->x = v->x * s;
    res->y = v->y * s;
}

void vec2_div_scalar_ptr(vector2 *res, const vector2 *v, const vm_float_t s)
{
    if (s == 0.0f) {
        *res = *v;
        return;
    }
    res->x = v->x / s;
    res->y = v->y / s;
}

void vec2_mul_ptr(vector2 *res, const vector2 *a, const vector2 *b)
{
    res->x = a->x * b->x;
    res->y = a->y * b->y;
}

void vec2_neg_ptr(vector2 *res, const vector2 *v)
{
    res->x = -v->x;
    res->y = -v->y;
}

void vec2_abs_ptr(vector2 *res, const vector2 *v)
{
    res->x = VECMAT_FABS(v->x);
    res->y = VECMAT_FABS(v->y);
}

void vec2_normalize_ptr(vector2 *res, const vector2 *v)
{
    const vm_float_t len = vec2_length(*v);
    if (len == 0.0f) {
        *res = *v;
        return;
    }
    const vm_float_t inv_len = 1.0f / len;
    res->x = v->x * inv_len;
    res->y = v->y * inv_len;
}

void vec2_min_ptr(vector2 *res, const vector2 *a, const vector2 *b)
{
    res->x = VECMAT_FMIN(a->x, b->x);
    res->y = VECMAT_FMIN(a->y, b->y);
}

void vec2_max_ptr(vector2 *res, const vector2 *a, const vector2 *b)
{
    res->x = VECMAT_FMAX(a->x, b->x);
    res->y = VECMAT_FMAX(a->y, b->y);
}

void vec2_sign_ptr(vector2 *res, const vector2 *v)
{
    res->x = VECMAT_COPYSIGN(1.0f, v->x);
    res->y = VECMAT_COPYSIGN(1.0f, v->y);
}

void vec2_floor_ptr(vector2 *res, const vector2 *v)
{
    res->x = VECMAT_FLOOR(v->x);
    res->y = VECMAT_FLOOR(v->y);
}

void vec2_ceil_ptr(vector2 *res, const vector2 *v)
{
    res->x = VECMAT_CEIL(v->x);
    res->y = VECMAT_CEIL(v->y);
}

void vec2_round_ptr(vector2 *res, const vector2 *v)
{
    res->x = VECMAT_ROUND(v->x);
    res->y = VECMAT_ROUND(v->y);
}

void vec2_perpendicular_ptr(vector2 *res, const vector2 *v)
{
    res->x = -v->y;
    res->y = v->x;
}

void vec2_cross_ptr(vector2 *res, const vector2 *a, const vector2 *b)
{

    res->x = a->x * b->y - a->y * b->x;
    res->y = 0.f;
}

void vec2_scale_ptr(vector2 *res, const vector2 *v, const vm_float_t s)
{
    res->x = v->x * s;
    res->y = v->y * s;
}

void vec2_reflect_ptr(vector2 *res, const vector2 *v, const vector2 *normal)
{
    const vm_float_t d = 2.0f * vec2_dot(*v, *normal);
    res->x = v->x - normal->x * d;
    res->y = v->y - normal->y * d;
}

void vec2_project_ptr(vector2 *res, const vector2 *a, const vector2 *b)
{
    const vm_float_t denom = vec2_dot(*b, *b);
    if (denom == 0.0f) {
        res->x = 0.0f;
        res->y = 0.0f;
        return;
    }
    const vm_float_t scale = vec2_dot(*a, *b) / denom;
    res->x = b->x * scale;
    res->y = b->y * scale;
}

void vec2_tangent_ptr(vector2 *res, const vector2 *v)
{
    res->x = v->y;
    res->y = -v->x;
}

void vec2_rotate_ptr(vector2 *result, const vector2 *v, const vm_float_t radians)
{
    const vm_float_t cs = VECMAT_COS(radians);
    const vm_float_t sn = VECMAT_SIN(radians);
    const vm_float_t x = v->x;
    const vm_float_t y = v->y;
    result->x = x * cs - y * sn;
    result->y = x * sn + y * cs;
}

void vec2_rotate_deg_ptr(vector2 *result, const vector2 *v, const vm_float_t degrees)
{
    vec2_rotate_ptr(result, v, deg_to_rad(degrees));
}

void vec2_slide_ptr(vector2 *result, const vector2 *v, const vector2 *normal)
{
    const vm_float_t d = vec2_dot(*v, *normal);
    result->x = v->x - normal->x * d;
    result->y = v->y - normal->y * d;
}

void vec2_clamp_ptr(vector2 *res, const vector2 *v, const vector2 *min, const vector2 *max)
{
    res->x = VECMAT_FMIN(VECMAT_FMAX(v->x, min->x), max->x);
    res->y = VECMAT_FMIN(VECMAT_FMAX(v->y, min->y), max->y);
}

void vec2_lerp_ptr(vector2 *res, const vector2 *a, const vector2 *b, const vm_float_t t)
{
    res->x = a->x + t * (b->x - a->x);
    res->y = a->y + t * (b->y - a->y);
}

void vec2_div_ptr(vector2 *res, const vector2 *a, const vector2 *b)
{
    res->x = (b->x == 0.0f) ? 0.0f : a->x / b->x;
    res->y = (b->y == 0.0f) ? 0.0f : a->y / b->y;
}

void vec2_add_scalar_ptr(vector2 *res, const vector2 *v, const vm_float_t s)
{
    res->x = v->x + s;
    res->y = v->y + s;
}

void vec2_sub_scalar_ptr(vector2 *res, const vector2 *v, const vm_float_t s)
{
    res->x = v->x - s;
    res->y = v->y - s;
}

void vec2_clamp_scalar_ptr(vector2 *res, const vector2 *v, const vm_float_t min, const vm_float_t max)
{
    res->x = VECMAT_FMIN(VECMAT_FMAX(v->x, min), max);
    res->y = VECMAT_FMIN(VECMAT_FMAX(v->y, min), max);
}

void vec2_saturate_ptr(vector2 *res, const vector2 *v)
{
    vec2_clamp_scalar_ptr(res, v, 0.0f, 1.0f);
}

void vec2_fract_ptr(vector2 *res, const vector2 *v)
{
    res->x = v->x - VECMAT_FLOOR(v->x);
    res->y = v->y - VECMAT_FLOOR(v->y);
}

void vec2_refract_ptr(vector2 *res, const vector2 *incident, const vector2 *normal, const vm_float_t eta)
{
    const vm_float_t dot = vec2_dot(*incident, *normal);
    const vm_float_t k = 1.0f - eta * eta * (1.0f - dot * dot);
    if (k < 0.0f) {
        res->x = 0.0f;
        res->y = 0.0f;
        return;
    }
    const vm_float_t factor = eta * dot + VECMAT_SQRT(k);
    res->x = eta * incident->x - factor * normal->x;
    res->y = eta * incident->y - factor * normal->y;
}

void vec2_reject_ptr(vector2 *res, const vector2 *a, const vector2 *b)
{
    vector2 projected;
    vec2_project_ptr(&projected, a, b);
    res->x = a->x - projected.x;
    res->y = a->y - projected.y;
}

void vec2_rotate_around_ptr(vector2 *res, const vector2 *v, const vector2 *pivot, const vm_float_t radians)
{
    vector2 offset;
    offset.x = v->x - pivot->x;
    offset.y = v->y - pivot->y;
    vec2_rotate_ptr(res, &offset, radians);
    res->x += pivot->x;
    res->y += pivot->y;
}

void vec2_rotate_around_deg_ptr(vector2 *res, const vector2 *v, const vector2 *pivot, const vm_float_t degrees)
{
    vec2_rotate_around_ptr(res, v, pivot, deg_to_rad(degrees));
}

void vec2_move_toward_ptr(vector2 *res, const vector2 *current, const vector2 *target, const vm_float_t max_delta)
{
    const vm_float_t dx = target->x - current->x;
    const vm_float_t dy = target->y - current->y;
    const vm_float_t dist = VECMAT_SQRT(dx * dx + dy * dy);
    if (dist <= max_delta || dist == 0.0f) {
        *res = *target;
        return;
    }
    const vm_float_t scale = max_delta / dist;
    res->x = current->x + dx * scale;
    res->y = current->y + dy * scale;
}

void vec2_limit_length_ptr(vector2 *res, const vector2 *v, const vm_float_t max_len)
{
    const vm_float_t len = vec2_length(*v);
    if (len <= max_len || len == 0.0f) {
        *res = *v;
        return;
    }
    const vm_float_t scale = max_len / len;
    res->x = v->x * scale;
    res->y = v->y * scale;
}

void vec2_to_vec3_ptr(vector3 *res, const vector2 *v, const vm_float_t z)
{
    res->x = v->x;
    res->y = v->y;
    res->z = z;
}
