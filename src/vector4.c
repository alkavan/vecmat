// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

vector4 vec4_zero(void)
{
    return (vector4){.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f};
}

vector4 vec4_one(void)
{
    return (vector4){.x = 1.0f, .y = 1.0f, .z = 1.0f, .w = 1.0f};
}

vector4 vec4_x_axis(const vm_float_t x)
{
    return (vector4){.x = x, .y = 0.0f, .z = 0.0f, .w = 0.0f};
}

vector4 vec4_y_axis(const vm_float_t y)
{
    return (vector4){.x = 0.0f, .y = y, .z = 0.0f, .w = 0.0f};
}

vector4 vec4_z_axis(const vm_float_t z)
{
    return (vector4){.x = 0.0f, .y = 0.0f, .z = z, .w = 0.0f};
}

vector4 vec4_w_axis(const vm_float_t w)
{
    return (vector4){.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = w};
}

vector4 vec4_x_scale(const vm_float_t x)
{
    return (vector4){.x = x, .y = 1.0f, .z = 1.0f, .w = 1.0f};
}

vector4 vec4_y_scale(const vm_float_t y)
{
    return (vector4){.x = 1.0f, .y = y, .z = 1.0f, .w = 1.0f};
}

vector4 vec4_z_scale(const vm_float_t z)
{
    return (vector4){.x = 1.0f, .y = 1.0f, .z = z, .w = 1.0f};
}

vector4 vec4_w_scale(const vm_float_t w)
{
    return (vector4){.x = 1.0f, .y = 1.0f, .z = 1.0f, .w = w};
}

vector4 vec4_add(const vector4 a, const vector4 b)
{
    vector4 res;
    vec4_add_ptr(&res, &a, &b);
    return res;
}

vector4 vec4_sub(const vector4 a, const vector4 b)
{
    vector4 res;
    vec4_sub_ptr(&res, &a, &b);
    return res;
}

vector4 vec4_mul_scalar(const vector4 v, const vm_float_t s)
{
    vector4 res;
    vec4_mul_scalar_ptr(&res, &v, s);
    return res;
}

vector4 vec4_div_scalar(const vector4 v, const vm_float_t s)
{
    vector4 res;
    vec4_div_scalar_ptr(&res, &v, s);
    return res;
}

vector4 vec4_mul(const vector4 a, const vector4 b)
{
    vector4 res;
    vec4_mul_ptr(&res, &a, &b);
    return res;
}

vector4 vec4_neg(const vector4 v)
{
    vector4 res;
    vec4_neg_ptr(&res, &v);
    return res;
}

vector4 vec4_abs(const vector4 v)
{
    vector4 res;
    vec4_abs_ptr(&res, &v);
    return res;
}

vector4 vec4_normalize(const vector4 v)
{
    vector4 res;
    vec4_normalize_ptr(&res, &v);
    return res;
}

vector4 vec4_min(const vector4 a, const vector4 b)
{
    vector4 res;
    vec4_min_ptr(&res, &a, &b);
    return res;
}

vector4 vec4_max(const vector4 a, const vector4 b)
{
    vector4 res;
    vec4_max_ptr(&res, &a, &b);
    return res;
}

vector4 vec4_sign(const vector4 v)
{
    vector4 res;
    vec4_sign_ptr(&res, &v);
    return res;
}

vector4 vec4_floor(const vector4 v)
{
    vector4 res;
    vec4_floor_ptr(&res, &v);
    return res;
}

vector4 vec4_ceil(const vector4 v)
{
    vector4 res;
    vec4_ceil_ptr(&res, &v);
    return res;
}

vector4 vec4_round(const vector4 v)
{
    vector4 res;
    vec4_round_ptr(&res, &v);
    return res;
}

vector4 vec4_lerp(const vector4 a, const vector4 b, const vm_float_t t)
{
    vector4 res;
    vec4_lerp_ptr(&res, &a, &b, t);
    return res;
}

vector4 vec4_clamp(const vector4 v, const vector4 min, const vector4 max)
{
    vector4 res;
    vec4_clamp_ptr(&res, &v, &min, &max);
    return res;
}

vector4 vec4_homogenize(const vector4 v)
{
    vector4 res;
    vec4_homogenize_ptr(&res, &v);
    return res;
}

vm_float_t vec4_dot(const vector4 a, const vector4 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

vm_float_t vec4_length(const vector4 v)
{
    return VECMAT_SQRT(vec4_dot(v, v));
}

vm_float_t vec4_distance(const vector4 a, const vector4 b)
{
    const vector4 diff = vec4_sub(a, b);
    return vec4_length(diff);
}

vector3 vec4_to_vec3(const vector4 v)
{
    vector3 res;
    vec4_to_vec3_ptr(&res, &v);
    return res;
}

vector4 vec4_div(const vector4 a, const vector4 b)
{
    vector4 res;
    vec4_div_ptr(&res, &a, &b);
    return res;
}

vector4 vec4_add_scalar(const vector4 v, const vm_float_t s)
{
    vector4 res;
    vec4_add_scalar_ptr(&res, &v, s);
    return res;
}

vector4 vec4_sub_scalar(const vector4 v, const vm_float_t s)
{
    vector4 res;
    vec4_sub_scalar_ptr(&res, &v, s);
    return res;
}

vector4 vec4_clamp_scalar(const vector4 v, const vm_float_t min, const vm_float_t max)
{
    vector4 res;
    vec4_clamp_scalar_ptr(&res, &v, min, max);
    return res;
}

vector4 vec4_saturate(const vector4 v)
{
    vector4 res;
    vec4_saturate_ptr(&res, &v);
    return res;
}

vector4 vec4_fract(const vector4 v)
{
    vector4 res;
    vec4_fract_ptr(&res, &v);
    return res;
}

vector4 vec4_project(const vector4 a, const vector4 b)
{
    vector4 res;
    vec4_project_ptr(&res, &a, &b);
    return res;
}

vector4 vec4_reject(const vector4 a, const vector4 b)
{
    vector4 res;
    vec4_reject_ptr(&res, &a, &b);
    return res;
}

vector4 vec4_slide(const vector4 v, const vector4 normal)
{
    vector4 res;
    vec4_slide_ptr(&res, &v, &normal);
    return res;
}

vector4 vec4_splat(const vm_float_t s)
{
    return (vector4){.x = s, .y = s, .z = s, .w = s};
}

vm_float_t vec4_length_squared(const vector4 v)
{
    return vec4_dot(v, v);
}

vm_float_t vec4_distance_squared(const vector4 a, const vector4 b)
{
    return vec4_length_squared(vec4_sub(a, b));
}

bool vec4_is_zero(const vector4 v)
{
    return vec4_length_squared(v) <= VECMAT_EPSILON * VECMAT_EPSILON;
}

bool vec4_is_normalized(const vector4 v)
{
    return VECMAT_FABS(vec4_length_squared(v) - 1.0f) < VECMAT_EPSILON;
}

bool vec4_near(const vector4 a, const vector4 b, const vm_float_t eps)
{
    return VECMAT_FABS(a.x - b.x) <= eps
        && VECMAT_FABS(a.y - b.y) <= eps
        && VECMAT_FABS(a.z - b.z) <= eps
        && VECMAT_FABS(a.w - b.w) <= eps;
}
