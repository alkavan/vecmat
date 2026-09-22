// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

vector3 vec3_zero(void)
{
    return (vector3){.x = 0.0f, .y = 0.0f, .z = 0.0f};
}

vector3 vec3_one(void)
{
    return (vector3){.x = 1.0f, .y = 1.0f, .z = 1.0f};
}

vector3 vec3_x_axis(const vm_float_t x)
{
    return (vector3){.x = x, .y = 0.0f, .z = 0.0f};
}

vector3 vec3_y_axis(const vm_float_t y)
{
    return (vector3){.x = 0.0f, .y = y, .z = 0.0f};
}

vector3 vec3_z_axis(const vm_float_t z)
{
    return (vector3){.x = 0.0f, .y = 0.0f, .z = z};
}

vector3 vec3_x_scale(const vm_float_t x)
{
    return (vector3){.x = x, .y = 1.0f, .z = 1.0f};
}

vector3 vec3_y_scale(const vm_float_t y)
{
    return (vector3){.x = 1.0f, .y = y, .z = 1.0f};
}

vector3 vec3_z_scale(const vm_float_t z)
{
    return (vector3){.x = 1.0f, .y = 1.0f, .z = z};
}

vector3 vec3_add(const vector3 a, const vector3 b)
{
    vector3 res;
    vec3_add_ptr(&res, &a, &b);
    return res;
}

vector3 vec3_sub(const vector3 a, const vector3 b)
{
    vector3 res;
    vec3_sub_ptr(&res, &a, &b);
    return res;
}

vector3 vec3_mul_scalar(const vector3 v, const vm_float_t s)
{
    vector3 res;
    vec3_mul_scalar_ptr(&res, &v, s);
    return res;
}

vector3 vec3_div_scalar(const vector3 v, const vm_float_t s)
{
    vector3 res;
    vec3_div_scalar_ptr(&res, &v, s);
    return res;
}

vector3 vec3_mul(const vector3 a, const vector3 b)
{
    vector3 res;
    vec3_mul_ptr(&res, &a, &b);
    return res;
}

vector3 vec3_neg(const vector3 v)
{
    vector3 res;
    vec3_neg_ptr(&res, &v);
    return res;
}

vector3 vec3_abs(const vector3 v)
{
    vector3 res;
    vec3_abs_ptr(&res, &v);
    return res;
}

vector3 vec3_cross(const vector3 a, const vector3 b)
{
    vector3 res;
    vec3_cross_ptr(&res, &a, &b);
    return res;
}

vector3 vec3_normalize(const vector3 v)
{
    vector3 res;
    vec3_normalize_ptr(&res, &v);
    return res;
}

vector3 vec3_min(const vector3 a, const vector3 b)
{
    vector3 res;
    vec3_min_ptr(&res, &a, &b);
    return res;
}

vector3 vec3_max(const vector3 a, const vector3 b)
{
    vector3 res;
    vec3_max_ptr(&res, &a, &b);
    return res;
}

vector3 vec3_sign(const vector3 v)
{
    vector3 res;
    vec3_sign_ptr(&res, &v);
    return res;
}

vector3 vec3_floor(const vector3 v)
{
    vector3 res;
    vec3_floor_ptr(&res, &v);
    return res;
}

vector3 vec3_ceil(const vector3 v)
{
    vector3 res;
    vec3_ceil_ptr(&res, &v);
    return res;
}

vector3 vec3_round(const vector3 v)
{
    vector3 res;
    vec3_round_ptr(&res, &v);
    return res;
}

vector3 vec3_reflect(const vector3 incident, const vector3 normal)
{
    vector3 res;
    vec3_reflect_ptr(&res, &incident, &normal);
    return res;
}

vector3 vec3_refract(const vector3 incident, const vector3 normal, const vm_float_t eta)
{
    vector3 res;
    vec3_refract_ptr(&res, &incident, &normal, eta);
    return res;
}

vector3 vec3_lerp(const vector3 a, const vector3 b, const vm_float_t t)
{
    vector3 res;
    vec3_lerp_ptr(&res, &a, &b, t);
    return res;
}

vector3 vec3_clamp(const vector3 v, const vector3 min, const vector3 max)
{
    vector3 res;
    vec3_clamp_ptr(&res, &v, &min, &max);
    return res;
}

vm_float_t vec3_distance(const vector3 a, const vector3 b)
{
    return vec3_length(vec3_sub(a, b));
}

vm_float_t vec3_angle(const vector3 a, const vector3 b)
{
    const vm_float_t dot = vec3_dot(a, b);
    const vm_float_t len_a = vec3_length(a);
    const vm_float_t len_b = vec3_length(b);
    return VECMAT_ACOS(dot / (len_a * len_b));
}

vm_float_t vec3_dot(const vector3 a, const vector3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vm_float_t vec3_length(const vector3 v)
{
    return VECMAT_SQRT(vec3_dot(v, v));
}

vector3 vec3_scale(const vector3 v, const vm_float_t s)
{
    vector3 res;
    vec3_scale_ptr(&res, &v, s);
    return res;
}

vector3 vec3_div(const vector3 a, const vector3 b)
{
    vector3 res;
    vec3_div_ptr(&res, &a, &b);
    return res;
}

vector3 vec3_add_scalar(const vector3 v, const vm_float_t s)
{
    vector3 res;
    vec3_add_scalar_ptr(&res, &v, s);
    return res;
}

vector3 vec3_sub_scalar(const vector3 v, const vm_float_t s)
{
    vector3 res;
    vec3_sub_scalar_ptr(&res, &v, s);
    return res;
}

vector3 vec3_clamp_scalar(const vector3 v, const vm_float_t min, const vm_float_t max)
{
    vector3 res;
    vec3_clamp_scalar_ptr(&res, &v, min, max);
    return res;
}

vector3 vec3_saturate(const vector3 v)
{
    vector3 res;
    vec3_saturate_ptr(&res, &v);
    return res;
}

vector3 vec3_fract(const vector3 v)
{
    vector3 res;
    vec3_fract_ptr(&res, &v);
    return res;
}

vector3 vec3_project(const vector3 a, const vector3 b)
{
    vector3 res;
    vec3_project_ptr(&res, &a, &b);
    return res;
}

vector3 vec3_slide(const vector3 v, const vector3 normal)
{
    vector3 res;
    vec3_slide_ptr(&res, &v, &normal);
    return res;
}

vector3 vec3_reject(const vector3 a, const vector3 b)
{
    vector3 res;
    vec3_reject_ptr(&res, &a, &b);
    return res;
}

vector3 vec3_rotate_axis(const vector3 v, const vector3 axis, const vm_float_t radians)
{
    vector3 res;
    vec3_rotate_axis_ptr(&res, &v, &axis, radians);
    return res;
}

vector3 vec3_rotate_axis_deg(const vector3 v, const vector3 axis, const vm_float_t degrees)
{
    return vec3_rotate_axis(v, axis, deg_to_rad(degrees));
}

vector3 vec3_splat(const vm_float_t s)
{
    return (vector3){.x = s, .y = s, .z = s};
}

vector3 vec3_from_vec2(const vector2 v, const vm_float_t z)
{
    vector3 res;
    vec3_from_vec2_ptr(&res, &v, z);
    return res;
}

vector3 vec3_move_toward(const vector3 current, const vector3 target, const vm_float_t max_delta)
{
    vector3 res;
    vec3_move_toward_ptr(&res, &current, &target, max_delta);
    return res;
}

vector3 vec3_limit_length(const vector3 v, const vm_float_t max_len)
{
    vector3 res;
    vec3_limit_length_ptr(&res, &v, max_len);
    return res;
}

vector2 vec3_xy(const vector3 v)
{
    vector2 res;
    vec3_xy_ptr(&res, &v);
    return res;
}

void vec3_orthonormal_basis(const vector3 n, vector3 *t, vector3 *b)
{
    vec3_orthonormal_basis_ptr(&n, t, b);
}

vm_float_t vec3_length_squared(const vector3 v)
{
    return vec3_dot(v, v);
}

vm_float_t vec3_length_manhattan(const vector3 v)
{
    return VECMAT_FABS(v.x) + VECMAT_FABS(v.y) + VECMAT_FABS(v.z);
}

vm_float_t vec3_length_chebyshev(const vector3 v)
{
    return VECMAT_FMAX(VECMAT_FABS(v.x), fmaxf(VECMAT_FABS(v.y), VECMAT_FABS(v.z)));
}

vm_float_t vec3_distance_squared(const vector3 a, const vector3 b)
{
    const vm_float_t dx = a.x - b.x;
    const vm_float_t dy = a.y - b.y;
    const vm_float_t dz = a.z - b.z;
    return dx * dx + dy * dy + dz * dz;
}

vm_float_t vec3_signed_angle(const vector3 a, const vector3 b, const vector3 axis)
{
    const vector3 c = vec3_cross(a, b);
    return VECMAT_ATAN2(vec3_dot(axis, c), vec3_dot(a, b));
}

vm_float_t vec3_min_component(const vector3 v)
{
    return VECMAT_FMIN(v.x, VECMAT_FMIN(v.y, v.z));
}

vm_float_t vec3_max_component(const vector3 v)
{
    return VECMAT_FMAX(v.x, VECMAT_FMAX(v.y, v.z));
}

vm_float_t vec3_sum(const vector3 v)
{
    return v.x + v.y + v.z;
}

bool vec3_is_zero(const vector3 v)
{
    return VECMAT_FABS(v.x) < VECMAT_EPSILON
        && VECMAT_FABS(v.y) < VECMAT_EPSILON
        && VECMAT_FABS(v.z) < VECMAT_EPSILON;
}

bool vec3_is_normalized(const vector3 v)
{
    return VECMAT_FABS(vec3_length_squared(v) - 1.0f) < VECMAT_EPSILON;
}

bool vec3_near(const vector3 a, const vector3 b, const vm_float_t eps)
{
    return VECMAT_FABS(a.x - b.x) < eps
        && VECMAT_FABS(a.y - b.y) < eps
        && VECMAT_FABS(a.z - b.z) < eps;
}
