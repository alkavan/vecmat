// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include <stdlib.h>

void vec3i_add_ptr(vector3i *res, const vector3i *a, const vector3i *b)
{
    res->x = a->x + b->x;
    res->y = a->y + b->y;
    res->z = a->z + b->z;
}

void vec3i_sub_ptr(vector3i *res, const vector3i *a, const vector3i *b)
{
    res->x = a->x - b->x;
    res->y = a->y - b->y;
    res->z = a->z - b->z;
}

void vec3i_mul_scalar_ptr(vector3i *res, const vector3i *v, const vm_int_t s)
{
    res->x = v->x * s;
    res->y = v->y * s;
    res->z = v->z * s;
}

void vec3i_div_scalar_ptr(vector3i *res, const vector3i *v, const vm_int_t s)
{
    if (s == 0) {
        res->x = 0;
        res->y = 0;
        res->z = 0;
        return;
    }
    res->x = v->x / s;
    res->y = v->y / s;
    res->z = v->z / s;
}

void vec3i_mul_ptr(vector3i *res, const vector3i *a, const vector3i *b)
{
    res->x = a->x * b->x;
    res->y = a->y * b->y;
    res->z = a->z * b->z;
}

void vec3i_neg_ptr(vector3i *res, const vector3i *v)
{
    res->x = -v->x;
    res->y = -v->y;
    res->z = -v->z;
}

void vec3i_abs_ptr(vector3i *res, const vector3i *v)
{
    res->x = abs(v->x);
    res->y = abs(v->y);
    res->z = abs(v->z);
}

void vec3i_normalize_ptr(vector3i *res, const vector3i *v)
{
    const vm_float_t len = vec3i_length(*v);
    if (len == 0.0f) {
        *res = *v;
        return;
    }

    res->x = (vm_int_t)(v->x / len);
    res->y = (vm_int_t)(v->y / len);
    res->z = (vm_int_t)(v->z / len);
}

void vec3i_cross_ptr(vector3i *res, const vector3i *a, const vector3i *b)
{
    res->x = a->y * b->z - a->z * b->y;
    res->y = a->z * b->x - a->x * b->z;
    res->z = a->x * b->y - a->y * b->x;
}

void vec3i_min_ptr(vector3i *res, const vector3i *a, const vector3i *b)
{
    res->x = a->x < b->x ? a->x : b->x;
    res->y = a->y < b->y ? a->y : b->y;
    res->z = a->z < b->z ? a->z : b->z;
}

void vec3i_max_ptr(vector3i *res, const vector3i *a, const vector3i *b)
{
    res->x = a->x > b->x ? a->x : b->x;
    res->y = a->y > b->y ? a->y : b->y;
    res->z = a->z > b->z ? a->z : b->z;
}

void vec3i_sign_ptr(vector3i *res, const vector3i *v)
{
    res->x = v->x > 0 ? 1 : v->x < 0 ? -1 : 0;
    res->y = v->y > 0 ? 1 : v->y < 0 ? -1 : 0;
    res->z = v->z > 0 ? 1 : v->z < 0 ? -1 : 0;
}

void vec3i_lerp_ptr(vector3i *res, const vector3i *a, const vector3i *b, const vm_float_t t)
{
    res->x = (vm_int_t)((1.0f - t) * (vm_float_t)a->x + t * (vm_float_t)b->x);
    res->y = (vm_int_t)((1.0f - t) * (vm_float_t)a->y + t * (vm_float_t)b->y);
    res->z = (vm_int_t)((1.0f - t) * (vm_float_t)a->z + t * (vm_float_t)b->z);
}

void vec3i_clamp_ptr(vector3i *res, const vector3i *v, const vector3i *min, const vector3i *max)
{
    res->x = v->x < min->x ? min->x : (v->x > max->x ? max->x : v->x);
    res->y = v->y < min->y ? min->y : (v->y > max->y ? max->y : v->y);
    res->z = v->z < min->z ? min->z : (v->z > max->z ? max->z : v->z);
}

/**
 * @brief Floor division toward -inf; returns 0 if `b == 0`.
 *
 * @param a Dividend.
 * @param b Divisor.
 * @return `floor(a / b)`, or 0 if `b` is 0.
 */
static vm_int_t vm_div_floor3(const vm_int_t a, const vm_int_t b)
{
    if (b == 0) {
        return 0;
    }
    const vm_int_t q = a / b;
    const vm_int_t r = a % b;
    if (r != 0 && ((a < 0) != (b < 0))) {
        return q - 1;
    }
    return q;
}

/**
 * @brief Floor modulo matching `vm_div_floor3`.
 *
 * @param a Dividend.
 * @param b Divisor.
 * @return Floor modulus.
 */
static vm_int_t vm_mod_floor3(const vm_int_t a, const vm_int_t b)
{
    if (b == 0) {
        return 0;
    }
    const vm_int_t r = a % b;
    if (r != 0 && ((a < 0) != (b < 0))) {
        return r + b;
    }
    return r;
}

void vec3i_div_ptr(vector3i *res, const vector3i *a, const vector3i *b)
{
    res->x = (b->x == 0) ? 0 : a->x / b->x;
    res->y = (b->y == 0) ? 0 : a->y / b->y;
    res->z = (b->z == 0) ? 0 : a->z / b->z;
}

void vec3i_add_scalar_ptr(vector3i *res, const vector3i *v, const vm_int_t s)
{
    res->x = v->x + s;
    res->y = v->y + s;
    res->z = v->z + s;
}

void vec3i_sub_scalar_ptr(vector3i *res, const vector3i *v, const vm_int_t s)
{
    res->x = v->x - s;
    res->y = v->y - s;
    res->z = v->z - s;
}

void vec3i_mod_ptr(vector3i *res, const vector3i *a, const vector3i *b)
{
    res->x = vm_mod_floor3(a->x, b->x);
    res->y = vm_mod_floor3(a->y, b->y);
    res->z = vm_mod_floor3(a->z, b->z);
}

void vec3i_div_floor_ptr(vector3i *res, const vector3i *a, const vector3i *b)
{
    res->x = vm_div_floor3(a->x, b->x);
    res->y = vm_div_floor3(a->y, b->y);
    res->z = vm_div_floor3(a->z, b->z);
}

void vec3i_wrap_ptr(vector3i *res, const vector3i *v, const vector3i *period)
{
    res->x = vm_mod_floor3(v->x, period->x);
    res->y = vm_mod_floor3(v->y, period->y);
    res->z = vm_mod_floor3(v->z, period->z);
}

void vec3i_from_vec2i_ptr(vector3i *res, const vector2i *v, const vm_int_t z)
{
    res->x = v->x;
    res->y = v->y;
    res->z = z;
}

void vec3i_xy_ptr(vector2i *res, const vector3i *v)
{
    res->x = v->x;
    res->y = v->y;
}

void vec3i_normalize_to_vec3_ptr(vector3 *res, const vector3i *v)
{
    const vector3 f = {
        .x = (vm_float_t)v->x,
        .y = (vm_float_t)v->y,
        .z = (vm_float_t)v->z
    };
    vec3_normalize_ptr(res, &f);
}
