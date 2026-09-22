// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "features/abi_int_once.h"

/* Integer-only: one copy (see features/abi_int_once.h). */
#if VECMAT_INT_ONCE

vector3i vec3i_zero(void)
{
    return (vector3i){.x = 0, .y = 0, .z = 0};
}

vector3i vec3i_one(void)
{
    return (vector3i){.x = 1, .y = 1, .z = 1};
}

vector3i vec3i_x_axis(const vm_int_t x)
{
    return (vector3i){.x = x, .y = 0, .z = 0};
}

vector3i vec3i_y_axis(const vm_int_t y)
{
    return (vector3i){.x = 0, .y = y, .z = 0};
}

vector3i vec3i_z_axis(const vm_int_t z)
{
    return (vector3i){.x = 0, .y = 0, .z = z};
}

vector3i vec3i_x_scale(const vm_int_t x)
{
    return (vector3i){.x = x, .y = 1, .z = 1};
}

vector3i vec3i_y_scale(const vm_int_t y)
{
    return (vector3i){.x = 1, .y = y, .z = 1};
}

vector3i vec3i_z_scale(const vm_int_t z)
{
    return (vector3i){.x = 1, .y = 1, .z = z};
}

vector3i vec3i_add(const vector3i a, const vector3i b)
{
    vector3i res;
    vec3i_add_ptr(&res, &a, &b);
    return res;
}

vector3i vec3i_sub(const vector3i a, const vector3i b)
{
    vector3i res;
    vec3i_sub_ptr(&res, &a, &b);
    return res;
}

vector3i vec3i_mul_scalar(const vector3i v, const vm_int_t s)
{
    vector3i res;
    vec3i_mul_scalar_ptr(&res, &v, s);
    return res;
}

vector3i vec3i_div_scalar(const vector3i v, const vm_int_t s)
{
    vector3i res;
    vec3i_div_scalar_ptr(&res, &v, s);
    return res;
}

vector3i vec3i_mul(const vector3i a, const vector3i b)
{
    vector3i res;
    vec3i_mul_ptr(&res, &a, &b);
    return res;
}

vector3i vec3i_neg(const vector3i v)
{
    vector3i res;
    vec3i_neg_ptr(&res, &v);
    return res;
}

vector3i vec3i_abs(const vector3i v)
{
    vector3i res;
    vec3i_abs_ptr(&res, &v);
    return res;
}

vector3i vec3i_cross(const vector3i a, const vector3i b)
{
    vector3i res;
    vec3i_cross_ptr(&res, &a, &b);
    return res;
}

vector3i vec3i_min(const vector3i a, const vector3i b)
{
    vector3i res;
    vec3i_min_ptr(&res, &a, &b);
    return res;
}

vector3i vec3i_max(const vector3i a, const vector3i b)
{
    vector3i res;
    vec3i_max_ptr(&res, &a, &b);
    return res;
}

vector3i vec3i_sign(const vector3i v)
{
    vector3i res;
    vec3i_sign_ptr(&res, &v);
    return res;
}

vector3i vec3i_clamp(const vector3i v, const vector3i min, const vector3i max)
{
    vector3i res;
    vec3i_clamp_ptr(&res, &v, &min, &max);
    return res;
}

vm_int_t vec3i_dot(const vector3i a, const vector3i b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vector3i vec3i_div(const vector3i a, const vector3i b)
{
    vector3i res;
    vec3i_div_ptr(&res, &a, &b);
    return res;
}

vector3i vec3i_add_scalar(const vector3i v, const vm_int_t s)
{
    vector3i res;
    vec3i_add_scalar_ptr(&res, &v, s);
    return res;
}

vector3i vec3i_sub_scalar(const vector3i v, const vm_int_t s)
{
    vector3i res;
    vec3i_sub_scalar_ptr(&res, &v, s);
    return res;
}

vector3i vec3i_mod(const vector3i a, const vector3i b)
{
    vector3i res;
    vec3i_mod_ptr(&res, &a, &b);
    return res;
}

vector3i vec3i_div_floor(const vector3i a, const vector3i b)
{
    vector3i res;
    vec3i_div_floor_ptr(&res, &a, &b);
    return res;
}

vector3i vec3i_wrap(const vector3i v, const vector3i period)
{
    vector3i res;
    vec3i_wrap_ptr(&res, &v, &period);
    return res;
}

vector3i vec3i_splat(const vm_int_t s)
{
    return (vector3i){.x = s, .y = s, .z = s};
}

vector3i vec3i_from_vec2i(const vector2i v, const vm_int_t z)
{
    vector3i res;
    vec3i_from_vec2i_ptr(&res, &v, z);
    return res;
}

vector2i vec3i_xy(const vector3i v)
{
    vector2i res;
    vec3i_xy_ptr(&res, &v);
    return res;
}

vm_int_t vec3i_length_squared(const vector3i v)
{
    return vec3i_dot(v, v);
}

vm_int_t vec3i_distance_squared(const vector3i a, const vector3i b)
{
    const vm_int_t dx = a.x - b.x;
    const vm_int_t dy = a.y - b.y;
    const vm_int_t dz = a.z - b.z;
    return dx * dx + dy * dy + dz * dz;
}

vm_int_t vec3i_length_manhattan(const vector3i v)
{
    const vm_int_t ax = v.x < 0 ? -v.x : v.x;
    const vm_int_t ay = v.y < 0 ? -v.y : v.y;
    const vm_int_t az = v.z < 0 ? -v.z : v.z;
    return ax + ay + az;
}

vm_int_t vec3i_length_chebyshev(const vector3i v)
{
    const vm_int_t ax = v.x < 0 ? -v.x : v.x;
    const vm_int_t ay = v.y < 0 ? -v.y : v.y;
    const vm_int_t az = v.z < 0 ? -v.z : v.z;
    const vm_int_t m = ax > ay ? ax : ay;
    return m > az ? m : az;
}

vm_int_t vec3i_min_component(const vector3i v)
{
    const vm_int_t m = v.x < v.y ? v.x : v.y;
    return m < v.z ? m : v.z;
}

vm_int_t vec3i_max_component(const vector3i v)
{
    const vm_int_t m = v.x > v.y ? v.x : v.y;
    return m > v.z ? m : v.z;
}

vm_int_t vec3i_sum(const vector3i v)
{
    return v.x + v.y + v.z;
}

bool vec3i_is_zero(const vector3i v)
{
    return v.x == 0 && v.y == 0 && v.z == 0;
}

vector3i vec3i_normalize(const vector3i v)
{
    vector3i res;
    vec3i_normalize_ptr(&res, &v);
    return res;
}

#endif /* VECMAT_INT_ONCE */

/* vm_float_t in the signature: compiled per ABI width. */

vector3i vec3i_lerp(const vector3i a, const vector3i b, const vm_float_t t)
{
    vector3i res;
    vec3i_lerp_ptr(&res, &a, &b, t);
    return res;
}

vm_float_t vec3i_length(const vector3i v) {
    return VECMAT_SQRT(vec3i_dot(v, v));
}

vm_float_t vec3i_distance(const vector3i a, const vector3i b)
{
    return vec3i_length(vec3i_sub(a, b));
}

vm_float_t vec3i_angle(const vector3i a, const vector3i b)
{
    const vm_float_t dot = vec3i_dot(a, b);
    const vm_float_t len_a = vec3i_length(a);
    const vm_float_t len_b = vec3i_length(b);
    if (len_a == 0.0f || len_b == 0.0f) return 0.0f;
    return VECMAT_ACOS(dot / (len_a * len_b));
}

vector3 vec3i_normalize_to_vec3(const vector3i v)
{
    vector3 res;
    vec3i_normalize_to_vec3_ptr(&res, &v);
    return res;
}
