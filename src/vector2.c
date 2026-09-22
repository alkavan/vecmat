// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

vector2 vec2_zero(void)
{
    return (vector2){.x = 0.0f, .y = 0.0f};
}

vector2 vec2_one(void)
{
    return (vector2){.x = 1.0f, .y = 1.0f};
}

vector2 vec2_x_axis(const vm_float_t x)
{
    return (vector2){.x = x, .y = 0.0f};
}

vector2 vec2_y_axis(const vm_float_t y)
{
    return (vector2){.x = 0.0f, .y = y};
}

vector2 vec2_x_scale(const vm_float_t x)
{
    return (vector2){.x = x, .y = 1.0f};
}

vector2 vec2_y_scale(const vm_float_t y)
{
    return (vector2){.x = 1.0f, .y = y};
}

vector2 vec2_scale(const vector2 v, const vm_float_t s)
{
    vector2 res;
    vec2_scale_ptr(&res, &v, s);
    return res;
}

vector2 vec2_add(const vector2 a, const vector2 b)
{
    vector2 res;
    vec2_add_ptr(&res, &a, &b);
    return res;
}

vector2 vec2_sub(const vector2 a, const vector2 b)
{
    vector2 res;
    vec2_sub_ptr(&res, &a, &b);
    return res;
}

vector2 vec2_mul_scalar(const vector2 v, const vm_float_t s)
{
    vector2 res;
    vec2_mul_scalar_ptr(&res, &v, s);
    return res;
}

vector2 vec2_div_scalar(const vector2 v, const vm_float_t s)
{
    vector2 res;
    vec2_div_scalar_ptr(&res, &v, s);
    return res;
}

vector2 vec2_mul(const vector2 a, const vector2 b)
{
    vector2 res;
    vec2_mul_ptr(&res, &a, &b);
    return res;
}

vector2 vec2_neg(const vector2 v)
{
    vector2 res;
    vec2_neg_ptr(&res, &v);
    return res;
}

vector2 vec2_abs(const vector2 v)
{
    vector2 res;
    vec2_abs_ptr(&res, &v);
    return res;
}

vector2 vec2_cross(const vector2 a, const vector2 b)
{
    vector2 res;
    vec2_cross_ptr(&res, &a, &b);
    return res;
}

vector2 vec2_normalize(const vector2 v)
{
    vector2 res;
    vec2_normalize_ptr(&res, &v);
    return res;
}

vector2 vec2_min(const vector2 a, const vector2 b)
{
    vector2 res;
    vec2_min_ptr(&res, &a, &b);
    return res;
}

vector2 vec2_max(const vector2 a, const vector2 b)
{
    vector2 res;
    vec2_max_ptr(&res, &a, &b);
    return res;
}

vector2 vec2_sign(const vector2 v)
{
    vector2 res;
    vec2_sign_ptr(&res, &v);
    return res;
}

vector2 vec2_floor(const vector2 v)
{
    vector2 res;
    vec2_floor_ptr(&res, &v);
    return res;
}

vector2 vec2_ceil(const vector2 v)
{
    vector2 res;
    vec2_ceil_ptr(&res, &v);
    return res;
}

vector2 vec2_round(const vector2 v)
{
    vector2 res;
    vec2_round_ptr(&res, &v);
    return res;
}

vector2 vec2_perpendicular(const vector2 v)
{
    vector2 res;
    vec2_perpendicular_ptr(&res, &v);
    return res;
}

vector2 vec2_reflect(const vector2 v, const vector2 normal)
{
    vector2 res;
    vec2_reflect_ptr(&res, &v, &normal);
    return res;
}

vector2 vec2_project(const vector2 a, const vector2 b)
{
    vector2 res;
    vec2_project_ptr(&res, &a, &b);
    return res;
}

vector2 vec2_tangent(const vector2 v)
{
    vector2 res;
    vec2_tangent_ptr(&res, &v);
    return res;
}

vector2 vec2_rotate(const vector2 v, const vm_float_t radians)
{
    vector2 res;
    vec2_rotate_ptr(&res, &v, radians);
    return res;
}

vector2 vec2_rotate_deg(const vector2 v, const vm_float_t degrees)
{
    return vec2_rotate(v, deg_to_rad(degrees));
}

vector2 vec2_slide(const vector2 v, const vector2 normal)
{
    vector2 res;
    vec2_slide_ptr(&res, &v, &normal);
    return res;
}

vector2 vec2_clamp(const vector2 v, const vector2 min, const vector2 max)
{
    vector2 res;
    vec2_clamp_ptr(&res, &v, &min, &max);
    return res;
}

vm_float_t vec2_dot(const vector2 a, const vector2 b)
{
    return a.x * b.x + a.y * b.y;
}

vm_float_t vec2_length(const vector2 v)
{
    return VECMAT_SQRT(vec2_dot(v, v));
}

vm_float_t vec2_aspect_ratio(const vector2 v)
{
    if (v.y == 0.0f) return 0.0f;
    return v.x / v.y;
}

vm_float_t vec2_distance(const vector2 a, const vector2 b)
{
    const vm_float_t dx = a.x - b.x;
    const vm_float_t dy = a.y - b.y;
    return VECMAT_SQRT(dx * dx + dy * dy);
}

vm_float_t vec2_angle(const vector2 a, const vector2 b)
{
    const vm_float_t dot = vec2_dot(a, b);
    const vm_float_t len_a = vec2_length(a);
    const vm_float_t len_b = vec2_length(b);
    if (len_a == 0.0f || len_b == 0.0f) return 0.0f;
    return VECMAT_ACOS(dot / (len_a * len_b));
}

vector2 vec2_lerp(const vector2 a, const vector2 b, const vm_float_t t)
{
    vector2 res;
    vec2_lerp_ptr(&res, &a, &b, t);
    return res;
}

vector2 vec2_div(const vector2 a, const vector2 b)
{
    vector2 res;
    vec2_div_ptr(&res, &a, &b);
    return res;
}

vector2 vec2_add_scalar(const vector2 v, const vm_float_t s)
{
    vector2 res;
    vec2_add_scalar_ptr(&res, &v, s);
    return res;
}

vector2 vec2_sub_scalar(const vector2 v, const vm_float_t s)
{
    vector2 res;
    vec2_sub_scalar_ptr(&res, &v, s);
    return res;
}

vector2 vec2_clamp_scalar(const vector2 v, const vm_float_t min, const vm_float_t max)
{
    vector2 res;
    vec2_clamp_scalar_ptr(&res, &v, min, max);
    return res;
}

vector2 vec2_saturate(const vector2 v)
{
    vector2 res;
    vec2_saturate_ptr(&res, &v);
    return res;
}

vector2 vec2_fract(const vector2 v)
{
    vector2 res;
    vec2_fract_ptr(&res, &v);
    return res;
}

vector2 vec2_refract(const vector2 incident, const vector2 normal, const vm_float_t eta)
{
    vector2 res;
    vec2_refract_ptr(&res, &incident, &normal, eta);
    return res;
}

vector2 vec2_reject(const vector2 a, const vector2 b)
{
    vector2 res;
    vec2_reject_ptr(&res, &a, &b);
    return res;
}

vector2 vec2_splat(const vm_float_t s)
{
    return (vector2){.x = s, .y = s};
}

vector2 vec2_from_angle(const vm_float_t radians)
{
    return (vector2){.x = VECMAT_COS(radians), .y = VECMAT_SIN(radians)};
}

vector2 vec2_from_angle_deg(const vm_float_t degrees)
{
    return vec2_from_angle(deg_to_rad(degrees));
}

vector2 vec2_rotate_around(const vector2 v, const vector2 pivot, const vm_float_t radians)
{
    vector2 res;
    vec2_rotate_around_ptr(&res, &v, &pivot, radians);
    return res;
}

vector2 vec2_rotate_around_deg(const vector2 v, const vector2 pivot, const vm_float_t degrees)
{
    return vec2_rotate_around(v, pivot, deg_to_rad(degrees));
}

vector2 vec2_move_toward(const vector2 current, const vector2 target, const vm_float_t max_delta)
{
    vector2 res;
    vec2_move_toward_ptr(&res, &current, &target, max_delta);
    return res;
}

vector2 vec2_limit_length(const vector2 v, const vm_float_t max_len)
{
    vector2 res;
    vec2_limit_length_ptr(&res, &v, max_len);
    return res;
}

vector3 vec2_to_vec3(const vector2 v, const vm_float_t z)
{
    vector3 res;
    vec2_to_vec3_ptr(&res, &v, z);
    return res;
}

vm_float_t vec2_length_squared(const vector2 v)
{
    return vec2_dot(v, v);
}

vm_float_t vec2_length_manhattan(const vector2 v)
{
    return VECMAT_FABS(v.x) + VECMAT_FABS(v.y);
}

vm_float_t vec2_length_chebyshev(const vector2 v)
{
    return VECMAT_FMAX(VECMAT_FABS(v.x), VECMAT_FABS(v.y));
}

vm_float_t vec2_distance_squared(const vector2 a, const vector2 b)
{
    const vm_float_t dx = a.x - b.x;
    const vm_float_t dy = a.y - b.y;
    return dx * dx + dy * dy;
}

vm_float_t vec2_cross_scalar(const vector2 a, const vector2 b)
{
    return a.x * b.y - a.y * b.x;
}

vm_float_t vec2_heading(const vector2 v)
{
    return VECMAT_ATAN2(v.y, v.x);
}

vm_float_t vec2_heading_deg(const vector2 v)
{
    return rad_to_deg(vec2_heading(v));
}

vm_float_t vec2_min_component(const vector2 v)
{
    return VECMAT_FMIN(v.x, v.y);
}

vm_float_t vec2_max_component(const vector2 v)
{
    return VECMAT_FMAX(v.x, v.y);
}

vm_float_t vec2_sum(const vector2 v)
{
    return v.x + v.y;
}

bool vec2_is_zero(const vector2 v)
{
    return VECMAT_FABS(v.x) < VECMAT_EPSILON && VECMAT_FABS(v.y) < VECMAT_EPSILON;
}

bool vec2_is_normalized(const vector2 v)
{
    return VECMAT_FABS(vec2_length_squared(v) - 1.0f) < VECMAT_EPSILON;
}

bool vec2_near(const vector2 a, const vector2 b, const vm_float_t eps)
{
    return VECMAT_FABS(a.x - b.x) < eps && VECMAT_FABS(a.y - b.y) < eps;
}
