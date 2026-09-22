// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "features/abi_int_once.h"

/* Integer-only: one copy (see features/abi_int_once.h). */
#if VECMAT_INT_ONCE

vector4i vec4i_zero(void)
{
    return (vector4i){.x = 0, .y = 0, .z = 0, .w = 0};
}

vector4i vec4i_one(void)
{
    return (vector4i){.x = 1, .y = 1, .z = 1, .w = 1};
}

vector4i vec4i_x_axis(const vm_int_t x)
{
    return (vector4i){.x = x, .y = 0, .z = 0, .w = 0};
}

vector4i vec4i_y_axis(const vm_int_t y)
{
    return (vector4i){.x = 0, .y = y, .z = 0, .w = 0};
}

vector4i vec4i_z_axis(const vm_int_t z) {
    return (vector4i){.x = 0, .y = 0, .z = z, .w = 0};
}

vector4i vec4i_w_axis(const vm_int_t w)
{
    return (vector4i){.x = 0, .y = 0, .z = 0, .w = w};
}

vector4i vec4i_x_scale(const vm_int_t x)
{
    return (vector4i){.x = x, .y = 1, .z = 1, .w = 1};
}

vector4i vec4i_y_scale(const vm_int_t y)
{
    return (vector4i){.x = 1, .y = y, .z = 1, .w = 1};
}

vector4i vec4i_z_scale(const vm_int_t z)
{
    return (vector4i){.x = 1, .y = 1, .z = z, .w = 1};
}

vector4i vec4i_w_scale(const vm_int_t w)
{
    return (vector4i){.x = 1, .y = 1, .z = 1, .w = w};
}

vector4i vec4i_add(const vector4i a, const vector4i b)
{
    vector4i res;
    vec4i_add_ptr(&res, &a, &b);
    return res;
}

vector4i vec4i_sub(const vector4i a, const vector4i b)
{
    vector4i res;
    vec4i_sub_ptr(&res, &a, &b);
    return res;
}

vector4i vec4i_mul_scalar(const vector4i v, const vm_int_t s)
{
    vector4i res;
    vec4i_mul_scalar_ptr(&res, &v, s);
    return res;
}

vector4i vec4i_div_scalar(const vector4i v, const vm_int_t s)
{
    vector4i res;
    vec4i_div_scalar_ptr(&res, &v, s);
    return res;
}

vector4i vec4i_mul(const vector4i a, const vector4i b)
{
    vector4i res;
    vec4i_mul_ptr(&res, &a, &b);
    return res;
}

vector4i vec4i_neg(const vector4i v)
{
    vector4i res;
    vec4i_neg_ptr(&res, &v);
    return res;
}

vector4i vec4i_abs(const vector4i v)
{
    vector4i res;
    vec4i_abs_ptr(&res, &v);
    return res;
}

vector4i vec4i_min(const vector4i a, const vector4i b)
{
    vector4i res;
    vec4i_min_ptr(&res, &a, &b);
    return res;
}

vector4i vec4i_max(const vector4i a, const vector4i b)
{
    vector4i res;
    vec4i_max_ptr(&res, &a, &b);
    return res;
}

vector4i vec4i_sign(const vector4i v)
{
    vector4i res;
    vec4i_sign_ptr(&res, &v);
    return res;
}

vector4i vec4i_clamp(const vector4i v, const vector4i min, const vector4i max)
{
    vector4i res;
    vec4i_clamp_ptr(&res, &v, &min, &max);
    return res;
}

vm_int_t vec4i_dot(const vector4i a, const vector4i b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

vector3i vec4i_to_vec3i(const vector4i v)
{
    vector3i res;
    vec4i_to_vec3i_ptr(&res, &v);
    return res;
}

vector4i vec4i_div(const vector4i a, const vector4i b)
{
    vector4i res;
    vec4i_div_ptr(&res, &a, &b);
    return res;
}

vector4i vec4i_add_scalar(const vector4i v, const vm_int_t s)
{
    vector4i res;
    vec4i_add_scalar_ptr(&res, &v, s);
    return res;
}

vector4i vec4i_sub_scalar(const vector4i v, const vm_int_t s)
{
    vector4i res;
    vec4i_sub_scalar_ptr(&res, &v, s);
    return res;
}

vector4i vec4i_mod(const vector4i a, const vector4i b)
{
    vector4i res;
    vec4i_mod_ptr(&res, &a, &b);
    return res;
}

vector4i vec4i_div_floor(const vector4i a, const vector4i b)
{
    vector4i res;
    vec4i_div_floor_ptr(&res, &a, &b);
    return res;
}

vector4i vec4i_wrap(const vector4i v, const vector4i period)
{
    vector4i res;
    vec4i_wrap_ptr(&res, &v, &period);
    return res;
}

vector4i vec4i_splat(const vm_int_t s)
{
    return (vector4i){.x = s, .y = s, .z = s, .w = s};
}

vm_int_t vec4i_length_squared(const vector4i v)
{
    return vec4i_dot(v, v);
}

vm_int_t vec4i_distance_squared(const vector4i a, const vector4i b)
{
    return vec4i_length_squared(vec4i_sub(a, b));
}

vm_int_t vec4i_length_manhattan(const vector4i v)
{
    const vm_int_t ax = v.x < 0 ? -v.x : v.x;
    const vm_int_t ay = v.y < 0 ? -v.y : v.y;
    const vm_int_t az = v.z < 0 ? -v.z : v.z;
    const vm_int_t aw = v.w < 0 ? -v.w : v.w;
    return ax + ay + az + aw;
}

vm_int_t vec4i_length_chebyshev(const vector4i v)
{
    vm_int_t m = v.x < 0 ? -v.x : v.x;
    const vm_int_t ay = v.y < 0 ? -v.y : v.y;
    const vm_int_t az = v.z < 0 ? -v.z : v.z;
    const vm_int_t aw = v.w < 0 ? -v.w : v.w;
    if (ay > m) m = ay;
    if (az > m) m = az;
    if (aw > m) m = aw;
    return m;
}

vm_int_t vec4i_min_component(const vector4i v)
{
    vm_int_t m = v.x;
    if (v.y < m) m = v.y;
    if (v.z < m) m = v.z;
    if (v.w < m) m = v.w;
    return m;
}

vm_int_t vec4i_max_component(const vector4i v)
{
    vm_int_t m = v.x;
    if (v.y > m) m = v.y;
    if (v.z > m) m = v.z;
    if (v.w > m) m = v.w;
    return m;
}

vm_int_t vec4i_sum(const vector4i v)
{
    return v.x + v.y + v.z + v.w;
}

bool vec4i_is_zero(const vector4i v)
{
    return v.x == 0 && v.y == 0 && v.z == 0 && v.w == 0;
}

vector4i vec4i_normalize(const vector4i v)
{
    vector4i res;
    vec4i_normalize_ptr(&res, &v);
    return res;
}

#endif /* VECMAT_INT_ONCE */

/* vm_float_t in the signature: compiled per ABI width. */

vector4i vec4i_lerp(const vector4i a, const vector4i b, const vm_float_t t)
{
    vector4i res;
    vec4i_lerp_ptr(&res, &a, &b, t);
    return res;
}

vm_float_t vec4i_length(const vector4i v)
{
    return VECMAT_SQRT(vec4i_dot(v, v));
}

vm_float_t vec4i_distance(const vector4i a, const vector4i b)
{
    const vector4i diff = vec4i_sub(a, b);
    return vec4i_length(diff);
}
