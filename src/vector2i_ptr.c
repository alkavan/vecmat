// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <stdlib.h>
#include <vecmat.h>

/**
 * @brief Adds vectors a and b component-wise, storing the result in res.
 *
 * @param res Pointer to result vector2i.
 * @param a Pointer to first vector2i.
 * @param b Pointer to second vector2i.
 */
void vec2i_add_ptr(vector2i *res, const vector2i *a, const vector2i *b)
{
    res->x = a->x + b->x;
    res->y = a->y + b->y;
}

/**
 * @brief Subtracts vector b from vector a component-wise, storing the result in res.
 *
 * @param res Pointer to result vector2i.
 * @param a Pointer to first vector2i.
 * @param b Pointer to second vector2i.
 */
void vec2i_sub_ptr(vector2i *res, const vector2i *a, const vector2i *b)
{
    res->x = a->x - b->x;
    res->y = a->y - b->y;
}

/**
 * @brief Multiplies vector v by scalar s component-wise, storing the result in res.
 *
 * @param res Pointer to result vector2i.
 * @param v Pointer to vector2i.
 * @param s Scalar.
 */
void vec2i_mul_scalar_ptr(vector2i *res, const vector2i *v, const vm_int_t s)
{
    res->x = v->x * s;
    res->y = v->y * s;
}

/**
 * @brief Divides vector v by scalar s component-wise (float division, truncated
 * to int), storing the result in res.
 *
 * Sets vector to zero if s == 0.
 *
 * @param res Pointer to result vector2i.
 * @param v Pointer to vector2i.
 * @param s Scalar.
 */
void vec2i_div_scalar_ptr(vector2i *res, const vector2i *v, const vm_int_t s)
{
    if (s == 0) {
        res->x = 0;
        res->y = 0;
        return;
    }
    res->x = (vm_int_t)((vm_float_t)v->x / s);
    res->y = (vm_int_t)((vm_float_t)v->y / s);
}

/**
 * @brief Multiplies vectors a and b component-wise (Hadamard product), storing
 * the result in res.
 *
 * @param res Pointer to result vector2i.
 * @param a Pointer to first vector2i.
 * @param b Pointer to second vector2i.
 */
void vec2i_mul_ptr(vector2i *res, const vector2i *a, const vector2i *b)
{
    res->x = a->x * b->x;
    res->y = a->y * b->y;
}

/**
 * @brief Negates the components of vector v, storing the result in res.
 *
 * @param res Pointer to result vector2i.
 * @param v Pointer to vector2i.
 */
void vec2i_neg_ptr(vector2i *res, const vector2i *v)
{
    res->x = -v->x;
    res->y = -v->y;
}

/**
 * @brief Computes the absolute values of the components of vector v (using int
 * abs), storing the result in res.
 *
 * @param res Pointer to result vector2i.
 * @param v Pointer to vector2i.
 */
void vec2i_abs_ptr(vector2i *res, const vector2i *v)
{
    res->x = abs(v->x);
    res->y = abs(v->y);
}

/**
 * @brief Normalizes vector v to approximate unit length (float length
 * computation, truncated to int), storing the result in res.
 *
 * Copies v unchanged if the length is zero.
 *
 * @param res Pointer to result vector2i.
 * @param v Pointer to vector2i.
 */
void vec2i_normalize_ptr(vector2i *res, const vector2i *v)
{
    const vm_float_t len = vec2i_length(*v);
    if (len == 0.0f) {
        *res = *v;
        return;
    }
    res->x = (vm_int_t)(v->x / len);
    res->y = (vm_int_t)(v->y / len);
}

/**
 * @brief Computes the component-wise minimum of vectors a and b, storing the
 * result in res.
 *
 * @param res Pointer to result vector2i.
 * @param a Pointer to first vector2i.
 * @param b Pointer to second vector2i.
 */
void vec2i_min_ptr(vector2i *res, const vector2i *a, const vector2i *b)
{
    res->x = a->x < b->x ? a->x : b->x;
    res->y = a->y < b->y ? a->y : b->y;
}

/**
 * @brief Computes the component-wise maximum of vectors a and b, storing the
 * result in res.
 *
 * @param res Pointer to result vector2i.
 * @param a Pointer to first vector2i.
 * @param b Pointer to second vector2i.
 */
void vec2i_max_ptr(vector2i *res, const vector2i *a, const vector2i *b)
{
    res->x = a->x > b->x ? a->x : b->x;
    res->y = a->y > b->y ? a->y : b->y;
}

/**
 * @brief Sets each component of res to the sign of the corresponding component
 * in v (+1, -1, or 0).
 *
 * @param res Pointer to result vector2i.
 * @param v Pointer to vector2i.
 */
void vec2i_sign_ptr(vector2i *res, const vector2i *v)
{
    res->x = v->x > 0 ? 1 : v->x < 0 ? -1 : 0;
    res->y = v->y > 0 ? 1 : v->y < 0 ? -1 : 0;
}

/**
 * @brief Computes the perpendicular vector to v (90 degrees counterclockwise
 * rotation), storing the result in res.
 *
 * @param res Pointer to result vector2i.
 * @param v Pointer to vector2i.
 */
void vec2i_perpendicular_ptr(vector2i *res, const vector2i *v)
{
    res->x = -v->y;
    res->y = v->x;
}

/**
 * @brief Computes the 2D cross-product of a and b, storing the scalar value in
 * res->x and 0 in res->y.
 *
 * @param res Pointer to result vector2i.
 * @param a Pointer to first vector2i.
 * @param b Pointer to second vector2i.
 */
void vec2i_cross_ptr(vector2i *res, const vector2i *a, const vector2i *b)
{
    res->x = a->x * b->y - a->y * b->x;
    res->y = 0;
}

/**
 * @brief Floor division toward -inf; returns 0 if `b == 0`.
 *
 * @param a Dividend.
 * @param b Divisor.
 * @return `floor(a / b)`, or 0 if `b` is 0.
 */
static vm_int_t vm_div_floor(const vm_int_t a, const vm_int_t b)
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
 * @brief Floor modulo matching `vm_div_floor`.
 *
 * Result is in [0, b) for b > 0 (or (b, 0] for b < 0); 0 if b == 0.
 *
 * @param a Dividend.
 * @param b Divisor.
 * @return Floor modulus.
 */
static vm_int_t vm_mod_floor(const vm_int_t a, const vm_int_t b)
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
 * @brief Linearly interpolates from a to b by t.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 * @param t Interpolation factor.
 */
void vec2i_lerp_ptr(vector2i *res, const vector2i *a, const vector2i *b, const vm_float_t t)
{
    res->x = (vm_int_t)((1.0f - t) * (vm_float_t)a->x + t * (vm_float_t)b->x);
    res->y = (vm_int_t)((1.0f - t) * (vm_float_t)a->y + t * (vm_float_t)b->y);
}

/**
 * @brief Clamps each component between min and max.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param min Lower bound.
 * @param max Upper bound.
 */
void vec2i_clamp_ptr(vector2i *res, const vector2i *v, const vector2i *min, const vector2i *max)
{
    res->x = v->x < min->x ? min->x : (v->x > max->x ? max->x : v->x);
    res->y = v->y < min->y ? min->y : (v->y > max->y ? max->y : v->y);
}

/**
 * @brief Divides two vectors component-wise.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
void vec2i_div_ptr(vector2i *res, const vector2i *a, const vector2i *b)
{
    res->x = (b->x == 0) ? 0 : a->x / b->x;
    res->y = (b->y == 0) ? 0 : a->y / b->y;
}

/**
 * @brief Adds a scalar to each component.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param s Scalar value.
 */
void vec2i_add_scalar_ptr(vector2i *res, const vector2i *v, const vm_int_t s)
{
    res->x = v->x + s;
    res->y = v->y + s;
}

/**
 * @brief Subtracts a scalar from each component.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param s Scalar value.
 */
void vec2i_sub_scalar_ptr(vector2i *res, const vector2i *v, const vm_int_t s)
{
    res->x = v->x - s;
    res->y = v->y - s;
}

/**
 * @brief Component-wise floor modulo of a by b.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
void vec2i_mod_ptr(vector2i *res, const vector2i *a, const vector2i *b)
{
    res->x = vm_mod_floor(a->x, b->x);
    res->y = vm_mod_floor(a->y, b->y);
}

/**
 * @brief Component-wise floored division of a by b.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
void vec2i_div_floor_ptr(vector2i *res, const vector2i *a, const vector2i *b)
{
    res->x = vm_div_floor(a->x, b->x);
    res->y = vm_div_floor(a->y, b->y);
}

/**
 * @brief Wraps each component of v into [0, period).
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param period Wrap period per component.
 */
void vec2i_wrap_ptr(vector2i *res, const vector2i *v, const vector2i *period)
{
    res->x = vm_mod_floor(v->x, period->x);
    res->y = vm_mod_floor(v->y, period->y);
}

/**
 * @brief Converts a `vector4i` to a `vector3i` by dropping w.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param z Z component.
 */
void vec2i_to_vec3i_ptr(vector3i *res, const vector2i *v, const vm_int_t z)
{
    res->x = v->x;
    res->y = v->y;
    res->z = z;
}

/**
 * @brief Converts to a unit-length vector2.
 *
 * @param res Output vector.
 * @param v Input vector.
 */
void vec2i_normalize_to_vec2_ptr(vector2 *res, const vector2i *v)
{
    const vector2 f = {.x = (vm_float_t)v->x, .y = (vm_float_t)v->y};
    vec2_normalize_ptr(res, &f);
}
