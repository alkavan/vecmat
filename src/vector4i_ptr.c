// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <stdlib.h>
#include "features/abi_int_once.h"

#include <vecmat.h>

/* Integer-only: one copy (see features/abi_int_once.h). */
#if VECMAT_INT_ONCE

void vec4i_add_ptr(vector4i *res, const vector4i *a, const vector4i *b)
{
    res->x = a->x + b->x;
    res->y = a->y + b->y;
    res->z = a->z + b->z;
    res->w = a->w + b->w;
}

void vec4i_sub_ptr(vector4i *res, const vector4i *a, const vector4i *b)
{
    res->x = a->x - b->x;
    res->y = a->y - b->y;
    res->z = a->z - b->z;
    res->w = a->w - b->w;
}

void vec4i_mul_scalar_ptr(vector4i *res, const vector4i *v, const vm_int_t s)
{
    res->x = v->x * s;
    res->y = v->y * s;
    res->z = v->z * s;
    res->w = v->w * s;
}

void vec4i_div_scalar_ptr(vector4i *res, const vector4i *v, const vm_int_t s)
{
    if (s == 0) {
        res->x = 0;
        res->y = 0;
        res->z = 0;
        res->w = 0;
        return;
    }
    res->x = v->x / s;
    res->y = v->y / s;
    res->z = v->z / s;
    res->w = v->w / s;
}

void vec4i_mul_ptr(vector4i *res, const vector4i *a, const vector4i *b)
{
    res->x = a->x * b->x;
    res->y = a->y * b->y;
    res->z = a->z * b->z;
    res->w = a->w * b->w;
}

void vec4i_neg_ptr(vector4i *res, const vector4i *v)
{
    res->x = -v->x;
    res->y = -v->y;
    res->z = -v->z;
    res->w = -v->w;
}

void vec4i_abs_ptr(vector4i *res, const vector4i *v)
{
    res->x = abs(v->x);
    res->y = abs(v->y);
    res->z = abs(v->z);
    res->w = abs(v->w);
}

void vec4i_min_ptr(vector4i *res, const vector4i *a, const vector4i *b)
{
    res->x = a->x < b->x ? a->x : b->x;
    res->y = a->y < b->y ? a->y : b->y;
    res->z = a->z < b->z ? a->z : b->z;
    res->w = a->w < b->w ? a->w : b->w;
}

void vec4i_max_ptr(vector4i *res, const vector4i *a, const vector4i *b)
{
    res->x = a->x > b->x ? a->x : b->x;
    res->y = a->y > b->y ? a->y : b->y;
    res->z = a->z > b->z ? a->z : b->z;
    res->w = a->w > b->w ? a->w : b->w;
}

void vec4i_sign_ptr(vector4i *res, const vector4i *v)
{
    res->x = v->x > 0 ? 1 : v->x < 0 ? -1 : 0;
    res->y = v->y > 0 ? 1 : v->y < 0 ? -1 : 0;
    res->z = v->z > 0 ? 1 : v->z < 0 ? -1 : 0;
    res->w = v->w > 0 ? 1 : v->w < 0 ? -1 : 0;
}

void vec4i_clamp_ptr(vector4i *res, const vector4i *v, const vector4i *min, const vector4i *max)
{
    res->x = v->x < min->x ? min->x : (v->x > max->x ? max->x : v->x);
    res->y = v->y < min->y ? min->y : (v->y > max->y ? max->y : v->y);
    res->z = v->z < min->z ? min->z : (v->z > max->z ? max->z : v->z);
    res->w = v->w < min->w ? min->w : (v->w > max->w ? max->w : v->w);
}

void vec4i_to_vec3i_ptr(vector3i *res, const vector4i *v)
{
    res->x = v->x;
    res->y = v->y;
    res->z = v->z;
}

/**
 * @brief Floor division toward -inf; returns 0 if `b == 0`.
 *
 * @param a Dividend.
 * @param b Divisor.
 * @return `floor(a / b)`, or 0 if `b` is 0.
 */

static vm_int_t vm_div_floor4(const vm_int_t a, const vm_int_t b)
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
 * @brief Floor modulo matching `vm_div_floor4`.
 *
 * @param a Dividend.
 * @param b Divisor.
 * @return Floor modulus.
 */

static vm_int_t vm_mod_floor4(const vm_int_t a, const vm_int_t b)
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

void vec4i_div_ptr(vector4i *res, const vector4i *a, const vector4i *b)
{
    res->x = (b->x == 0) ? 0 : a->x / b->x;
    res->y = (b->y == 0) ? 0 : a->y / b->y;
    res->z = (b->z == 0) ? 0 : a->z / b->z;
    res->w = (b->w == 0) ? 0 : a->w / b->w;
}

void vec4i_add_scalar_ptr(vector4i *res, const vector4i *v, const vm_int_t s)
{
    res->x = v->x + s;
    res->y = v->y + s;
    res->z = v->z + s;
    res->w = v->w + s;
}

void vec4i_sub_scalar_ptr(vector4i *res, const vector4i *v, const vm_int_t s)
{
    res->x = v->x - s;
    res->y = v->y - s;
    res->z = v->z - s;
    res->w = v->w - s;
}

void vec4i_mod_ptr(vector4i *res, const vector4i *a, const vector4i *b)
{
    res->x = vm_mod_floor4(a->x, b->x);
    res->y = vm_mod_floor4(a->y, b->y);
    res->z = vm_mod_floor4(a->z, b->z);
    res->w = vm_mod_floor4(a->w, b->w);
}

void vec4i_div_floor_ptr(vector4i *res, const vector4i *a, const vector4i *b)
{
    res->x = vm_div_floor4(a->x, b->x);
    res->y = vm_div_floor4(a->y, b->y);
    res->z = vm_div_floor4(a->z, b->z);
    res->w = vm_div_floor4(a->w, b->w);
}

void vec4i_wrap_ptr(vector4i *res, const vector4i *v, const vector4i *period)
{
    res->x = vm_mod_floor4(v->x, period->x);
    res->y = vm_mod_floor4(v->y, period->y);
    res->z = vm_mod_floor4(v->z, period->z);
    res->w = vm_mod_floor4(v->w, period->w);
}

void vec4i_normalize_ptr(vector4i *res, const vector4i *v)
{
    const vm_float_t len = vec4i_length(*v);
    if (len == 0.0f) {
        *res = *v;
        return;
    }

    res->x = (vm_int_t)(v->x / len);
    res->y = (vm_int_t)(v->y / len);
    res->z = (vm_int_t)(v->z / len);
    res->w = (vm_int_t)(v->w / len);
}

#endif /* VECMAT_INT_ONCE */

/* vm_float_t in the signature: compiled per ABI width. */

void vec4i_lerp_ptr(vector4i *res, const vector4i *a, const vector4i *b, const vm_float_t t)
{
    const vm_float_t clamped_t = (t < 0.0f) ? 0.0f : (t > 1.0f) ? 1.0f : t;

    res->x = (vm_int_t)(a->x + (b->x - a->x) * clamped_t);
    res->y = (vm_int_t)(a->y + (b->y - a->y) * clamped_t);
    res->z = (vm_int_t)(a->z + (b->z - a->z) * clamped_t);
    res->w = (vm_int_t)(a->w + (b->w - a->w) * clamped_t);
}
