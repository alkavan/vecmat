// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include <stdlib.h>

/**
 * @brief Computes the component-wise sum of two `vector3i`.
 *
 * @param res Pointer to the vector that will store the result.
 * @param a Pointer to the first vector.
 * @param b Pointer to the second vector.
 */
void vec3i_add_ptr(vector3i *res, const vector3i *a, const vector3i *b)
{
    res->x = a->x + b->x;
    res->y = a->y + b->y;
    res->z = a->z + b->z;
}

/**
 * @brief Computes the component-wise difference of two `vector3i` (a minus b).
 *
 * @param res Pointer to the vector that will store the result.
 * @param a Pointer to the first vector.
 * @param b Pointer to the second vector.
 */
void vec3i_sub_ptr(vector3i *res, const vector3i *a, const vector3i *b)
{
    res->x = a->x - b->x;
    res->y = a->y - b->y;
    res->z = a->z - b->z;
}

/**
 * @brief Scales a `vector3i` by an integer scalar.
 *
 * @param res Pointer to the vector that will store the result.
 * @param v Pointer to the input vector.
 * @param s The scalar multiplier.
 */
void vec3i_mul_scalar_ptr(vector3i *res, const vector3i *v, const vm_int_t s)
{
    res->x = v->x * s;
    res->y = v->y * s;
    res->z = v->z * s;
}

/**
 * @brief Scales a `vector3i` by the inverse of an integer scalar.
 *
 * If the scalar is zero, stores the zero vector.
 *
 * @param res Pointer to the vector that will store the result.
 * @param v Pointer to the input vector.
 * @param s The scalar divisor.
 */
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

/**
 * @brief Computes the component-wise product (Hadamard) of two `vector3i`.
 *
 * @param res Pointer to the vector that will store the result.
 * @param a Pointer to the first vector.
 * @param b Pointer to the second vector.
 */
void vec3i_mul_ptr(vector3i *res, const vector3i *a, const vector3i *b)
{
    res->x = a->x * b->x;
    res->y = a->y * b->y;
    res->z = a->z * b->z;
}

/**
 * @brief Negates a `vector3i` (multiplies by -1).
 *
 * @param res Pointer to the vector that will store the result.
 * @param v Pointer to the input vector.
 */
void vec3i_neg_ptr(vector3i *res, const vector3i *v)
{
    res->x = -v->x;
    res->y = -v->y;
    res->z = -v->z;
}

/**
 * @brief Computes the absolute value of each component of a `vector3i`.
 *
 * @param res Pointer to the vector that will store the result.
 * @param v Pointer to the input vector.
 */
void vec3i_abs_ptr(vector3i *res, const vector3i *v)
{
    res->x = abs(v->x);
    res->y = abs(v->y);
    res->z = abs(v->z);
}

/**
 * @brief Normalizes a `vector3i` to approximate unit length.
 *
 * If the vector length is zero, copies the input vector.
 *
 * @param res Pointer to the vector that will store the result.
 * @param v Pointer to the input vector.
 */
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

/**
 * @brief Computes the cross-product of two `vector3i`.
 *
 * @param res Pointer to the vector that will store the result.
 * @param a Pointer to the first vector.
 * @param b Pointer to the second vector.
 */
void vec3i_cross_ptr(vector3i *res, const vector3i *a, const vector3i *b)
{
    res->x = a->y * b->z - a->z * b->y;
    res->y = a->z * b->x - a->x * b->z;
    res->z = a->x * b->y - a->y * b->x;
}

/**
 * @brief Computes the component-wise minimum of two `vector3i`.
 *
 * @param res Pointer to the vector that will store the result.
 * @param a Pointer to the first vector.
 * @param b Pointer to the second vector.
 */
void vec3i_min_ptr(vector3i *res, const vector3i *a, const vector3i *b)
{
    res->x = a->x < b->x ? a->x : b->x;
    res->y = a->y < b->y ? a->y : b->y;
    res->z = a->z < b->z ? a->z : b->z;
}

/**
 * @brief Computes the component-wise maximum of two `vector3i`.
 *
 * @param res Pointer to the vector that will store the result.
 * @param a Pointer to the first vector.
 * @param b Pointer to the second vector.
 */
void vec3i_max_ptr(vector3i *res, const vector3i *a, const vector3i *b)
{
    res->x = a->x > b->x ? a->x : b->x;
    res->y = a->y > b->y ? a->y : b->y;
    res->z = a->z > b->z ? a->z : b->z;
}

/**
 * @brief Computes the sign of each component of a `vector3i` (-1, 0, or 1).
 *
 * @param res Pointer to the vector that will store the result.
 * @param v Pointer to the input vector.
 */
void vec3i_sign_ptr(vector3i *res, const vector3i *v)
{
    res->x = v->x > 0 ? 1 : v->x < 0 ? -1 : 0;
    res->y = v->y > 0 ? 1 : v->y < 0 ? -1 : 0;
    res->z = v->z > 0 ? 1 : v->z < 0 ? -1 : 0;
}

/**
 * @brief Performs linear interpolation between two `vector3i`.
 *
 * @param res Pointer to the vector that will store the result.
 * @param a Pointer to the start vector.
 * @param b Pointer to the end vector.
 * @param t Interpolation factor (typically between 0.0 and 1.0).
 */
void vec3i_lerp_ptr(vector3i *res, const vector3i *a, const vector3i *b, const vm_float_t t)
{
    res->x = (vm_int_t)((1.0f - t) * (vm_float_t)a->x + t * (vm_float_t)b->x);
    res->y = (vm_int_t)((1.0f - t) * (vm_float_t)a->y + t * (vm_float_t)b->y);
    res->z = (vm_int_t)((1.0f - t) * (vm_float_t)a->z + t * (vm_float_t)b->z);
}

/**
 * @brief Clamps each component of a `vector3i` between corresponding min and
 * max values.
 *
 * @param res Pointer to the vector that will store the result.
 * @param v Pointer to the input vector.
 * @param min Pointer to the minimum bounds vector.
 * @param max Pointer to the maximum bounds vector.
 */
void vec3i_clamp_ptr(vector3i *res, const vector3i *v, const vector3i *min, const vector3i *max)
{
    res->x = v->x < min->x ? min->x : (v->x > max->x ? max->x : v->x);
    res->y = v->y < min->y ? min->y : (v->y > max->y ? max->y : v->y);
    res->z = v->z < min->z ? min->z : (v->z > max->z ? max->z : v->z);
}

// Floor division: toward -inf; returns 0 if b == 0.
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

// Floor modulo: result in [0, b) for b > 0 (or (b, 0] for b < 0); 0 if b == 0.
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

/**
 * @brief Divides two vectors component-wise.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
void vec3i_div_ptr(vector3i *res, const vector3i *a, const vector3i *b)
{
    res->x = (b->x == 0) ? 0 : a->x / b->x;
    res->y = (b->y == 0) ? 0 : a->y / b->y;
    res->z = (b->z == 0) ? 0 : a->z / b->z;
}

/**
 * @brief Adds a scalar to each component.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param s Scalar value.
 */
void vec3i_add_scalar_ptr(vector3i *res, const vector3i *v, const vm_int_t s)
{
    res->x = v->x + s;
    res->y = v->y + s;
    res->z = v->z + s;
}

/**
 * @brief Subtracts a scalar from each component.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param s Scalar value.
 */
void vec3i_sub_scalar_ptr(vector3i *res, const vector3i *v, const vm_int_t s)
{
    res->x = v->x - s;
    res->y = v->y - s;
    res->z = v->z - s;
}

/**
 * @brief Component-wise floor modulo of a by b.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
void vec3i_mod_ptr(vector3i *res, const vector3i *a, const vector3i *b)
{
    res->x = vm_mod_floor3(a->x, b->x);
    res->y = vm_mod_floor3(a->y, b->y);
    res->z = vm_mod_floor3(a->z, b->z);
}

/**
 * @brief Component-wise floored division of a by b.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
void vec3i_div_floor_ptr(vector3i *res, const vector3i *a, const vector3i *b)
{
    res->x = vm_div_floor3(a->x, b->x);
    res->y = vm_div_floor3(a->y, b->y);
    res->z = vm_div_floor3(a->z, b->z);
}

/**
 * @brief Wraps each component of v into [0, period).
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param period Wrap period per component.
 */
void vec3i_wrap_ptr(vector3i *res, const vector3i *v, const vector3i *period)
{
    res->x = vm_mod_floor3(v->x, period->x);
    res->y = vm_mod_floor3(v->y, period->y);
    res->z = vm_mod_floor3(v->z, period->z);
}

/**
 * @brief Builds a vector3i from a vector2i and z.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param z Z component.
 */
void vec3i_from_vec2i_ptr(vector3i *res, const vector2i *v, const vm_int_t z)
{
    res->x = v->x;
    res->y = v->y;
    res->z = z;
}

/**
 * @brief Returns the x and y components as a 2D vector.
 *
 * @param res Output vector.
 * @param v Input vector.
 */
void vec3i_xy_ptr(vector2i *res, const vector3i *v)
{
    res->x = v->x;
    res->y = v->y;
}

/**
 * @brief Converts to a unit-length vector3.
 *
 * @param res Output vector.
 * @param v Input vector.
 */
void vec3i_normalize_to_vec3_ptr(vector3 *res, const vector3i *v)
{
    const vector3 f = {
        .x = (vm_float_t)v->x,
        .y = (vm_float_t)v->y,
        .z = (vm_float_t)v->z
    };
    vec3_normalize_ptr(res, &f);
}
