// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <stdlib.h>
#include <vecmat.h>

/**
 * @brief Adds the components of two `vector4i` and stores the result in res.
 *
 * @param res Pointer to result vector
 * @param a Pointer to first vector
 * @param b Pointer to second vector
 */
void vec4i_add_ptr(vector4i *res, const vector4i *a, const vector4i *b)
{
    res->x = a->x + b->x;
    res->y = a->y + b->y;
    res->z = a->z + b->z;
    res->w = a->w + b->w;
}

/**
 * Subtracts the components of the second vector from the first `vector4i`
 * and stores the result in res.
 *
 * @param res Pointer to result vector
 * @param a Pointer to first vector
 * @param b Pointer to second vector
 */
void vec4i_sub_ptr(vector4i *res, const vector4i *a, const vector4i *b)
{
    res->x = a->x - b->x;
    res->y = a->y - b->y;
    res->z = a->z - b->z;
    res->w = a->w - b->w;
}

/**
 * @brief Multiplies the components of a `vector4i` by a scalar and stores
 * the result in res.
 *
 * @param res Pointer to result vector
 * @param v Pointer to input vector
 * @param s Scalar value
 */
void vec4i_mul_scalar_ptr(vector4i *res, const vector4i *v, const vm_int_t s)
{
    res->x = v->x * s;
    res->y = v->y * s;
    res->z = v->z * s;
    res->w = v->w * s;
}

/**
 * @brief Divides the components of a `vector4i` by a scalar and stores the
 * result in res.
 *
 * If the scalar is zero, sets all components of res to zero.
 *
 * @param res Pointer to result vector
 * @param v Pointer to input vector
 * @param s Scalar divisor
 */
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

/**
 * @brief Multiplies the components of two `vector4i` and stores the result in res.
 *
 * @param res Pointer to result vector
 * @param a Pointer to first vector
 * @param b Pointer to second vector
 */
void vec4i_mul_ptr(vector4i *res, const vector4i *a, const vector4i *b)
{
    res->x = a->x * b->x;
    res->y = a->y * b->y;
    res->z = a->z * b->z;
    res->w = a->w * b->w;
}

/**
 * @brief Negates the components of a `vector4i` and stores the result in res.
 *
 * @param res Pointer to result vector
 * @param v Pointer to input vector
 */
void vec4i_neg_ptr(vector4i *res, const vector4i *v)
{
    res->x = -v->x;
    res->y = -v->y;
    res->z = -v->z;
    res->w = -v->w;
}

/**
 * @brief Computes the absolute value of each component of a `vector4i` and
 * stores the result in res.
 *
 * @param res Pointer to result vector
 * @param v Pointer to input vector
 */
void vec4i_abs_ptr(vector4i *res, const vector4i *v)
{
    res->x = abs(v->x);
    res->y = abs(v->y);
    res->z = abs(v->z);
    res->w = abs(v->w);
}

/**
 * @brief Normalizes a `vector4i` by dividing its components by the vector
 * length and stores the result in res.
 *
 * If the length is zero, copies the input vector to res.
 *
 * @param res Pointer to result vector
 * @param v Pointer to input vector
 */
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

/**
 * @brief Takes the component-wise minimum of two `vector4i` and stores the
 * result in res.
 *
 * @param res Pointer to result vector
 * @param a Pointer to first vector
 * @param b Pointer to second vector
 */
void vec4i_min_ptr(vector4i *res, const vector4i *a, const vector4i *b)
{
    res->x = a->x < b->x ? a->x : b->x;
    res->y = a->y < b->y ? a->y : b->y;
    res->z = a->z < b->z ? a->z : b->z;
    res->w = a->w < b->w ? a->w : b->w;
}

/**
 * @brief Computes the component-wise maximum of two `vector4i` and stores the
 * result in res.
 *
 * @param res Pointer to result vector
 * @param a Pointer to first vector
 * @param b Pointer to second vector
 */
void vec4i_max_ptr(vector4i *res, const vector4i *a, const vector4i *b)
{
    res->x = a->x > b->x ? a->x : b->x;
    res->y = a->y > b->y ? a->y : b->y;
    res->z = a->z > b->z ? a->z : b->z;
    res->w = a->w > b->w ? a->w : b->w;
}

/**
 * @brief Computes the sign (-1, 0, or 1) of each component of a `vector4i`
 * and stores the result in res.
 *
 * @param res Pointer to result vector
 * @param v Pointer to input vector
 */
void vec4i_sign_ptr(vector4i *res, const vector4i *v)
{
    res->x = v->x > 0 ? 1 : v->x < 0 ? -1 : 0;
    res->y = v->y > 0 ? 1 : v->y < 0 ? -1 : 0;
    res->z = v->z > 0 ? 1 : v->z < 0 ? -1 : 0;
    res->w = v->w > 0 ? 1 : v->w < 0 ? -1 : 0;
}

/**
 * @brief Linearly interpolates between two `vector4i` using an interpolation
 * factor t (clamped to [0,1]) and stores the result in res.
 *
 * @param res Pointer to result vector
 * @param a Pointer to first vector
 * @param b Pointer to second vector
 * @param t Interpolation factor
 */
void vec4i_lerp_ptr(vector4i *res, const vector4i *a, const vector4i *b, const vm_float_t t)
{
    const vm_float_t clamped_t = (t < 0.0f) ? 0.0f : (t > 1.0f) ? 1.0f : t;

    res->x = (vm_int_t)(a->x + (b->x - a->x) * clamped_t);
    res->y = (vm_int_t)(a->y + (b->y - a->y) * clamped_t);
    res->z = (vm_int_t)(a->z + (b->z - a->z) * clamped_t);
    res->w = (vm_int_t)(a->w + (b->w - a->w) * clamped_t);
}

/**
 * @brief Clamps the components of a `vector4i` to the range [min, max] and
 * stores the result in res.
 *
 * @param res Pointer to result vector
 * @param v Pointer to input vector
 * @param min Pointer to minimum bounds vector
 * @param max Pointer to maximum bounds vector
 */
void vec4i_clamp_ptr(vector4i *res, const vector4i *v, const vector4i *min, const vector4i *max)
{
    res->x = v->x < min->x ? min->x : (v->x > max->x ? max->x : v->x);
    res->y = v->y < min->y ? min->y : (v->y > max->y ? max->y : v->y);
    res->z = v->z < min->z ? min->z : (v->z > max->z ? max->z : v->z);
    res->w = v->w < min->w ? min->w : (v->w > max->w ? max->w : v->w);
}

/**
 * @brief Copies the x, y, z components of a `vector4i` and stores the result
 * in a `vector3i`.
 *
 * @param res Pointer to result `vector3i`
 * @param v Pointer to source `vector4i`
 */
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

/**
 * @brief Divides two vectors component-wise.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
void vec4i_div_ptr(vector4i *res, const vector4i *a, const vector4i *b)
{
    res->x = (b->x == 0) ? 0 : a->x / b->x;
    res->y = (b->y == 0) ? 0 : a->y / b->y;
    res->z = (b->z == 0) ? 0 : a->z / b->z;
    res->w = (b->w == 0) ? 0 : a->w / b->w;
}

/**
 * @brief Adds a scalar to each component.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param s Scalar value.
 */
void vec4i_add_scalar_ptr(vector4i *res, const vector4i *v, const vm_int_t s)
{
    res->x = v->x + s;
    res->y = v->y + s;
    res->z = v->z + s;
    res->w = v->w + s;
}

/**
 * @brief Subtracts a scalar from each component.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param s Scalar value.
 */
void vec4i_sub_scalar_ptr(vector4i *res, const vector4i *v, const vm_int_t s)
{
    res->x = v->x - s;
    res->y = v->y - s;
    res->z = v->z - s;
    res->w = v->w - s;
}

/**
 * @brief Component-wise floor modulo of a by b.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
void vec4i_mod_ptr(vector4i *res, const vector4i *a, const vector4i *b)
{
    res->x = vm_mod_floor4(a->x, b->x);
    res->y = vm_mod_floor4(a->y, b->y);
    res->z = vm_mod_floor4(a->z, b->z);
    res->w = vm_mod_floor4(a->w, b->w);
}

/**
 * @brief Component-wise floored division of a by b.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
void vec4i_div_floor_ptr(vector4i *res, const vector4i *a, const vector4i *b)
{
    res->x = vm_div_floor4(a->x, b->x);
    res->y = vm_div_floor4(a->y, b->y);
    res->z = vm_div_floor4(a->z, b->z);
    res->w = vm_div_floor4(a->w, b->w);
}

/**
 * @brief Wraps each component of v into [0, period).
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param period Wrap period per component.
 */
void vec4i_wrap_ptr(vector4i *res, const vector4i *v, const vector4i *period)
{
    res->x = vm_mod_floor4(v->x, period->x);
    res->y = vm_mod_floor4(v->y, period->y);
    res->z = vm_mod_floor4(v->z, period->z);
    res->w = vm_mod_floor4(v->w, period->w);
}
