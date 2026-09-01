// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

/**
 * @brief Returns a `vector2i` with both components set to 0.
 *
 * @return The zero vector2i.
 */
vector2i vec2i_zero(void)
{
    return (vector2i){.x = 0, .y = 0};
}

/**
 * @brief Returns a `vector2i` with both components set to 1.
 *
 * @return The one vector2i.
 */
vector2i vec2i_one(void)
{
    return (vector2i){.x = 1, .y = 1};
}

/**
 * @brief Returns a `vector2i` along the x-axis (y = 0).
 *
 * @param x The x component value.
 * @return The x-axis vector2i.
 */
vector2i vec2i_x_axis(const vm_int_t x)
{
    return (vector2i){.x = x, .y = 0};
}

/**
 * @brief Returns a `vector2i` along the y-axis (x = 0).
 *
 * @param y The y component value.
 * @return The y-axis vector2i.
 */
vector2i vec2i_y_axis(const vm_int_t y)
{
    return (vector2i){.x = 0, .y = y};
}

/**
 * @brief Returns a `vector2i` representing x-axis scaling (y = 1).
 *
 * @param x The x scale factor.
 * @return The x-scale vector2i.
 */
vector2i vec2i_x_scale(const vm_int_t x)
{
    return (vector2i){.x = x, .y = 1};
}

/**
 * @brief Returns a `vector2i` representing y-axis scaling (x = 1).
 *
 * @param y The y scale factor.
 * @return The y-scale vector2i.
 */
vector2i vec2i_y_scale(const vm_int_t y)
{
    return (vector2i){.x = 1, .y = y};
}

/**
 * @brief Adds two vectors component-wise.
 *
 * @see vec2i_add_ptr
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The sum vector2i.
 */
vector2i vec2i_add(const vector2i a, const vector2i b)
{
    vector2i res;
    vec2i_add_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Subtracts the second vector from the first component-wise.
 *
 * @see vec2i_sub_ptr
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The difference vector2i.
 */
vector2i vec2i_sub(const vector2i a, const vector2i b)
{
    vector2i res;
    vec2i_sub_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Multiplies a vector by a scalar component-wise.
 *
 * @see vec2i_mul_scalar_ptr
 *
 * @param v The input vector.
 * @param s The scalar multiplier.
 * @return The scaled vector2i.
 */
vector2i vec2i_mul_scalar(const vector2i v, const vm_int_t s)
{
    vector2i res;
    vec2i_mul_scalar_ptr(&res, &v, s);
    return res;
}

/**
 * @brief Divides a vector by a scalar component-wise (float division,
 * truncated to int).
 *
 * @see vec2i_div_scalar_ptr
 *
 * @param v The input vector.
 * @param s The scalar divisor (sets to zero vector if zero).
 * @return The divided vector2i.
 */
vector2i vec2i_div_scalar(const vector2i v, const vm_int_t s)
{
    vector2i res;
    vec2i_div_scalar_ptr(&res, &v, s);
    return res;
}

/**
 * @brief Multiplies two vectors component-wise (Hadamard product).
 *
 * @see vec2i_mul_ptr
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The product vector2i.
 */
vector2i vec2i_mul(const vector2i a, const vector2i b)
{
    vector2i res;
    vec2i_mul_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Negates the vector (multiplies each component by -1).
 *
 * @see vec2i_neg_ptr
 *
 * @param v The input vector.
 * @return The negated vector2i.
 */
vector2i vec2i_neg(const vector2i v)
{
    vector2i res;
    vec2i_neg_ptr(&res, &v);
    return res;
}

/**
 * @brief Returns the absolute values of each component.
 *
 * @see vec2i_abs_ptr
 *
 * @param v The input vector.
 * @return The absolute value vector2i.
 */
vector2i vec2i_abs(const vector2i v)
{
    vector2i res;
    vec2i_abs_ptr(&res, &v);
    return res;
}

/**
 * @brief Normalizes the vector to approximate unit length (float length, truncated to int).
 *
 * @see vec2i_normalize_ptr
 *
 * @param v The input vector.
 * @return The normalized vector2i (unchanged if zero length).
 */
vector2i vec2i_normalize(const vector2i v)
{
    vector2i res;
    vec2i_normalize_ptr(&res, &v);
    return res;
}

/**
 * @brief Returns the component-wise minimum of two vectors.
 *
 * @see vec2i_min_ptr
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The minimum vector2i.
 */
vector2i vec2i_min(const vector2i a, const vector2i b)
{
    vector2i res;
    vec2i_min_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Returns the component-wise maximum of two vectors.
 *
 * @see vec2i_max_ptr
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The maximum vector2i.
 */
vector2i vec2i_max(const vector2i a, const vector2i b)
{
    vector2i res;
    vec2i_max_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Returns the sign of each component (+1, -1, or 0).
 *
 * @see vec2i_sign_ptr
 *
 * @param v The input vector.
 * @return The sign vector2i.
 */
vector2i vec2i_sign(const vector2i v)
{
    vector2i res;
    vec2i_sign_ptr(&res, &v);
    return res;
}

/**
 * @brief Returns the perpendicular vector (90 degrees counterclockwise).
 *
 * @see vec2i_perpendicular_ptr
 *
 * @param v The input vector.
 * @return The perpendicular vector2i.
 */
vector2i vec2i_perpendicular(const vector2i v)
{
    vector2i res;
    vec2i_perpendicular_ptr(&res, &v);
    return res;
}

/**
 * @brief Computes the 2D cross-product as a vector.
 *
 * @see vec2i_cross_ptr
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The cross-product vector2i.
 */
vector2i vec2i_cross(const vector2i a, const vector2i b)
{
    vector2i res;
    vec2i_cross_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Computes the dot product of two vectors.
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The dot product (vm_int_t; may overflow).
 */
vm_int_t vec2i_dot(const vector2i a, const vector2i b)
{
    return a.x * b.x + a.y * b.y;
}

/**
 * @brief Computes the length (magnitude) of the vector.
 *
 * @param v The input vector.
 * @return The length scalar.
 */
vm_float_t vec2i_length(const vector2i v)
{
    return VECMAT_SQRT(vec2i_dot(v, v));
}

/**
 * @brief Computes the Euclidean distance between two vectors (treated as points).
 *
 * @param a The first point.
 * @param b The second point.
 * @return The distance scalar.
 */
vm_float_t vec2i_distance(const vector2i a, const vector2i b)
{
    const vm_int_t dx = a.x - b.x;
    const vm_int_t dy = a.y - b.y;
    return VECMAT_SQRT(dx * dx + dy * dy);
}

/**
 * @brief Computes the signed angle between two vectors (in radians,
 * range [-PI, PI]) using atan2(cross, dot).
 *
 * Returns 0.0f if either vector has zero length.
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The signed angle scalar.
 */
vm_float_t vec2i_angle(const vector2i a, const vector2i b)
{
    const vm_float_t dot = vec2i_dot(a, b);
    const vm_float_t len_a = vec2i_length(a);
    const vm_float_t len_b = vec2i_length(b);
    if (len_a == 0.0f || len_b == 0.0f) return 0.0f;

    return VECMAT_ATAN2(a.x * b.y - a.y * b.x, dot); // atan2 for signed angle
}

/**
 * @brief Computes the aspect ratio of the vector (x / y).
 *
 * Returns 0.0f if y == 0.
 *
 * @param v The input vector.
 * @return The aspect ratio scalar.
 */
vm_float_t vec2i_aspect_ratio(const vector2i v)
{
    if (v.y == 0) return 0.0f;
    return (vm_float_t)v.x / (vm_float_t)v.y;
}

/**
 * @brief Linearly interpolates from a to b by t.
 *
 * @see vec2i_lerp_ptr
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @param t Interpolation factor.
 * @return The resulting vector2i.
 */
vector2i vec2i_lerp(const vector2i a, const vector2i b, const vm_float_t t)
{
    vector2i res;
    vec2i_lerp_ptr(&res, &a, &b, t);
    return res;
}

/**
 * @brief Clamps each component between min and max.
 *
 * @see vec2i_clamp_ptr
 *
 * @param v Input vector.
 * @param min Lower bound.
 * @param max Upper bound.
 * @return The resulting vector2i.
 */
vector2i vec2i_clamp(const vector2i v, const vector2i min, const vector2i max)
{
    vector2i res;
    vec2i_clamp_ptr(&res, &v, &min, &max);
    return res;
}

/**
 * @brief Divides two vectors component-wise.
 *
 * @see vec2i_div_ptr
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector2i.
 */
vector2i vec2i_div(const vector2i a, const vector2i b)
{
    vector2i res;
    vec2i_div_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Adds a scalar to each component.
 *
 * @see vec2i_add_scalar_ptr
 *
 * @param v Input vector.
 * @param s Scalar value.
 * @return The resulting vector2i.
 */
vector2i vec2i_add_scalar(const vector2i v, const vm_int_t s)
{
    vector2i res;
    vec2i_add_scalar_ptr(&res, &v, s);
    return res;
}

/**
 * @brief Subtracts a scalar from each component.
 *
 * @see vec2i_sub_scalar_ptr
 *
 * @param v Input vector.
 * @param s Scalar value.
 * @return The resulting vector2i.
 */
vector2i vec2i_sub_scalar(const vector2i v, const vm_int_t s)
{
    vector2i res;
    vec2i_sub_scalar_ptr(&res, &v, s);
    return res;
}

/**
 * @brief Component-wise floor modulo of a by b.
 *
 * @see vec2i_mod_ptr
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector2i.
 */
vector2i vec2i_mod(const vector2i a, const vector2i b)
{
    vector2i res;
    vec2i_mod_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Component-wise floored division of a by b.
 *
 * @see vec2i_div_floor_ptr
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector2i.
 */
vector2i vec2i_div_floor(const vector2i a, const vector2i b)
{
    vector2i res;
    vec2i_div_floor_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Wraps each component of v into [0, period).
 *
 * @see vec2i_wrap_ptr
 *
 * @param v Input vector.
 * @param period Wrap period per component.
 * @return The resulting vector2i.
 */
vector2i vec2i_wrap(const vector2i v, const vector2i period)
{
    vector2i res;
    vec2i_wrap_ptr(&res, &v, &period);
    return res;
}

/**
 * @brief Returns a vector with every component set to s.
 *
 * @see vec2i_splat_ptr
 *
 * @param s Scalar value.
 * @return The resulting vector2i.
 */
vector2i vec2i_splat(const vm_int_t s)
{
    return (vector2i){.x = s, .y = s};
}

/**
 * @brief Converts a vector2i to a vector3i using z.
 *
 * @see vec2i_to_vec3i_ptr
 *
 * @param v Input vector.
 * @param z Z component.
 * @return The resulting vector3i.
 */
vector3i vec2i_to_vec3i(const vector2i v, const vm_int_t z)
{
    vector3i res;
    vec2i_to_vec3i_ptr(&res, &v, z);
    return res;
}

/**
 * @brief Converts to a unit-length vector2.
 *
 * @see vec2i_normalize_to_vec2_ptr
 *
 * @param v Input vector.
 * @return The resulting vector2.
 */
vector2 vec2i_normalize_to_vec2(const vector2i v)
{
    vector2 res;
    vec2i_normalize_to_vec2_ptr(&res, &v);
    return res;
}

/**
 * @brief Returns the 2D cross product as a scalar (a.x*b.y - a.y*b.x).
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting scalar.
 */
vm_int_t vec2i_cross_scalar(const vector2i a, const vector2i b)
{
    return a.x * b.y - a.y * b.x;
}

/**
 * @brief Returns the squared Euclidean length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_int_t vec2i_length_squared(const vector2i v)
{
    return vec2i_dot(v, v);
}

/**
 * @brief Returns the squared Euclidean distance between a and b.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting scalar.
 */
vm_int_t vec2i_distance_squared(const vector2i a, const vector2i b)
{
    const vm_int_t dx = a.x - b.x;
    const vm_int_t dy = a.y - b.y;
    return dx * dx + dy * dy;
}

/**
 * @brief Returns the Manhattan (L1) length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_int_t vec2i_length_manhattan(const vector2i v)
{
    const vm_int_t ax = v.x < 0 ? -v.x : v.x;
    const vm_int_t ay = v.y < 0 ? -v.y : v.y;
    return ax + ay;
}

/**
 * @brief Returns the Chebyshev (L-inf) length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_int_t vec2i_length_chebyshev(const vector2i v)
{
    const vm_int_t ax = v.x < 0 ? -v.x : v.x;
    const vm_int_t ay = v.y < 0 ? -v.y : v.y;
    return ax > ay ? ax : ay;
}

/**
 * @brief Returns the smallest component.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_int_t vec2i_min_component(const vector2i v)
{
    return v.x < v.y ? v.x : v.y;
}

/**
 * @brief Returns the largest component.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_int_t vec2i_max_component(const vector2i v)
{
    return v.x > v.y ? v.x : v.y;
}

/**
 * @brief Returns the sum of all components.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_int_t vec2i_sum(const vector2i v)
{
    return v.x + v.y;
}

/**
 * @brief Returns true if every component is zero.
 *
 * @param v Input vector.
 * @return True if every component is zero.
 */
bool vec2i_is_zero(const vector2i v)
{
    return v.x == 0 && v.y == 0;
}
