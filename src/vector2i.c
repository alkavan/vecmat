// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

vector2i vec2i_zero(void)
{
    return (vector2i){.x = 0, .y = 0};
}

vector2i vec2i_one(void)
{
    return (vector2i){.x = 1, .y = 1};
}

vector2i vec2i_x_axis(const vm_int_t x)
{
    return (vector2i){.x = x, .y = 0};
}

vector2i vec2i_y_axis(const vm_int_t y)
{
    return (vector2i){.x = 0, .y = y};
}

vector2i vec2i_x_scale(const vm_int_t x)
{
    return (vector2i){.x = x, .y = 1};
}

vector2i vec2i_y_scale(const vm_int_t y)
{
    return (vector2i){.x = 1, .y = y};
}

vector2i vec2i_add(const vector2i a, const vector2i b)
{
    vector2i res;
    vec2i_add_ptr(&res, &a, &b);
    return res;
}

vector2i vec2i_sub(const vector2i a, const vector2i b)
{
    vector2i res;
    vec2i_sub_ptr(&res, &a, &b);
    return res;
}

vector2i vec2i_mul_scalar(const vector2i v, const vm_int_t s)
{
    vector2i res;
    vec2i_mul_scalar_ptr(&res, &v, s);
    return res;
}

vector2i vec2i_div_scalar(const vector2i v, const vm_int_t s)
{
    vector2i res;
    vec2i_div_scalar_ptr(&res, &v, s);
    return res;
}

vector2i vec2i_mul(const vector2i a, const vector2i b)
{
    vector2i res;
    vec2i_mul_ptr(&res, &a, &b);
    return res;
}

vector2i vec2i_neg(const vector2i v)
{
    vector2i res;
    vec2i_neg_ptr(&res, &v);
    return res;
}

vector2i vec2i_abs(const vector2i v)
{
    vector2i res;
    vec2i_abs_ptr(&res, &v);
    return res;
}

vector2i vec2i_normalize(const vector2i v)
{
    vector2i res;
    vec2i_normalize_ptr(&res, &v);
    return res;
}

vector2i vec2i_min(const vector2i a, const vector2i b)
{
    vector2i res;
    vec2i_min_ptr(&res, &a, &b);
    return res;
}

vector2i vec2i_max(const vector2i a, const vector2i b)
{
    vector2i res;
    vec2i_max_ptr(&res, &a, &b);
    return res;
}

vector2i vec2i_sign(const vector2i v)
{
    vector2i res;
    vec2i_sign_ptr(&res, &v);
    return res;
}

vector2i vec2i_perpendicular(const vector2i v)
{
    vector2i res;
    vec2i_perpendicular_ptr(&res, &v);
    return res;
}

vector2i vec2i_cross(const vector2i a, const vector2i b)
{
    vector2i res;
    vec2i_cross_ptr(&res, &a, &b);
    return res;
}

vm_int_t vec2i_dot(const vector2i a, const vector2i b)
{
    return a.x * b.x + a.y * b.y;
}

vm_float_t vec2i_length(const vector2i v)
{
    return VECMAT_SQRT(vec2i_dot(v, v));
}

vm_float_t vec2i_distance(const vector2i a, const vector2i b)
{
    const vm_int_t dx = a.x - b.x;
    const vm_int_t dy = a.y - b.y;
    return VECMAT_SQRT(dx * dx + dy * dy);
}

vm_float_t vec2i_angle(const vector2i a, const vector2i b)
{
    const vm_float_t dot = vec2i_dot(a, b);
    const vm_float_t len_a = vec2i_length(a);
    const vm_float_t len_b = vec2i_length(b);
    if (len_a == 0.0f || len_b == 0.0f) return 0.0f;

    return VECMAT_ATAN2(a.x * b.y - a.y * b.x, dot); // atan2 for signed angle
}

vm_float_t vec2i_aspect_ratio(const vector2i v)
{
    if (v.y == 0) return 0.0f;
    return (vm_float_t)v.x / (vm_float_t)v.y;
}

vector2i vec2i_lerp(const vector2i a, const vector2i b, const vm_float_t t)
{
    vector2i res;
    vec2i_lerp_ptr(&res, &a, &b, t);
    return res;
}

vector2i vec2i_clamp(const vector2i v, const vector2i min, const vector2i max)
{
    vector2i res;
    vec2i_clamp_ptr(&res, &v, &min, &max);
    return res;
}

vector2i vec2i_div(const vector2i a, const vector2i b)
{
    vector2i res;
    vec2i_div_ptr(&res, &a, &b);
    return res;
}

vector2i vec2i_add_scalar(const vector2i v, const vm_int_t s)
{
    vector2i res;
    vec2i_add_scalar_ptr(&res, &v, s);
    return res;
}

vector2i vec2i_sub_scalar(const vector2i v, const vm_int_t s)
{
    vector2i res;
    vec2i_sub_scalar_ptr(&res, &v, s);
    return res;
}

vector2i vec2i_mod(const vector2i a, const vector2i b)
{
    vector2i res;
    vec2i_mod_ptr(&res, &a, &b);
    return res;
}

vector2i vec2i_div_floor(const vector2i a, const vector2i b)
{
    vector2i res;
    vec2i_div_floor_ptr(&res, &a, &b);
    return res;
}

vector2i vec2i_wrap(const vector2i v, const vector2i period)
{
    vector2i res;
    vec2i_wrap_ptr(&res, &v, &period);
    return res;
}

vector2i vec2i_splat(const vm_int_t s)
{
    return (vector2i){.x = s, .y = s};
}

vector3i vec2i_to_vec3i(const vector2i v, const vm_int_t z)
{
    vector3i res;
    vec2i_to_vec3i_ptr(&res, &v, z);
    return res;
}

vector2 vec2i_normalize_to_vec2(const vector2i v)
{
    vector2 res;
    vec2i_normalize_to_vec2_ptr(&res, &v);
    return res;
}

vm_int_t vec2i_cross_scalar(const vector2i a, const vector2i b)
{
    return a.x * b.y - a.y * b.x;
}

vm_int_t vec2i_length_squared(const vector2i v)
{
    return vec2i_dot(v, v);
}

vm_int_t vec2i_distance_squared(const vector2i a, const vector2i b)
{
    const vm_int_t dx = a.x - b.x;
    const vm_int_t dy = a.y - b.y;
    return dx * dx + dy * dy;
}

vm_int_t vec2i_length_manhattan(const vector2i v)
{
    const vm_int_t ax = v.x < 0 ? -v.x : v.x;
    const vm_int_t ay = v.y < 0 ? -v.y : v.y;
    return ax + ay;
}

vm_int_t vec2i_length_chebyshev(const vector2i v)
{
    const vm_int_t ax = v.x < 0 ? -v.x : v.x;
    const vm_int_t ay = v.y < 0 ? -v.y : v.y;
    return ax > ay ? ax : ay;
}

vm_int_t vec2i_min_component(const vector2i v)
{
    return v.x < v.y ? v.x : v.y;
}

vm_int_t vec2i_max_component(const vector2i v)
{
    return v.x > v.y ? v.x : v.y;
}

vm_int_t vec2i_sum(const vector2i v)
{
    return v.x + v.y;
}

bool vec2i_is_zero(const vector2i v)
{
    return v.x == 0 && v.y == 0;
}
