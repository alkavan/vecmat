// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

/**
 * @brief Returns a zero-initialized `vector3i`.
 *
 * @return A vector3i with all components set to 0.
 */
vector3i vec3i_zero(void)
{
    return (vector3i){.x = 0, .y = 0, .z = 0};
}

/**
 * @brief Returns a `vector3i` with all components set to 1.
 *
 * @return A vector3i with all components set to 1.
 */
vector3i vec3i_one(void)
{
    return (vector3i){.x = 1, .y = 1, .z = 1};
}

/**
 * @brief Returns a `vector3i` along the x-axis.
 *
 * @param x The x component value.
 * @return A vector3i with (x, 0, 0).
 */
vector3i vec3i_x_axis(const vm_int_t x)
{
    return (vector3i){.x = x, .y = 0, .z = 0};
}

/**
 * @brief Returns a `vector3i` along the y-axis.
 *
 * @param y The y component value.
 * @return A vector3i with (0, y, 0).
 */
vector3i vec3i_y_axis(const vm_int_t y)
{
    return (vector3i){.x = 0, .y = y, .z = 0};
}

/**
 * @brief Returns a `vector3i` along the z-axis.
 *
 * @param z The z component value.
 * @return A vector3i with (0, 0, z).
 */
vector3i vec3i_z_axis(const vm_int_t z)
{
    return (vector3i){.x = 0, .y = 0, .z = z};
}

/**
 * @brief Returns a `vector3i` for scaling along the x-axis.
 *
 * @param x The x scale factor.
 * @return A vector3i with (x, 1, 1).
 */
vector3i vec3i_x_scale(const vm_int_t x)
{
    return (vector3i){.x = x, .y = 1, .z = 1};
}

/**
 * @brief Returns a `vector3i` for scaling along the y-axis.
 *
 * @param y The y scale factor.
 * @return A vector3i with (1, y, 1).
 */
vector3i vec3i_y_scale(const vm_int_t y)
{
    return (vector3i){.x = 1, .y = y, .z = 1};
}

/**
 * @brief Returns a `vector3i` for scaling along the z-axis.
 *
 * @param z The z scale factor.
 * @return A vector3i with (1, 1, z).
 */
vector3i vec3i_z_scale(const vm_int_t z)
{
    return (vector3i){.x = 1, .y = 1, .z = z};
}

/**
 * @brief Component-wise addition of two vectors.
 *
 * @see vec3i_add_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Result vector.
 */
vector3i vec3i_add(const vector3i a, const vector3i b)
{
    vector3i res;
    vec3i_add_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Component-wise subtraction of two vectors.
 *
 * @see vec3i_sub_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Result vector.
 */
vector3i vec3i_sub(const vector3i a, const vector3i b)
{
    vector3i res;
    vec3i_sub_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Component-wise multiplication of vector by scalar.
 *
 * @see vec3i_mul_scalar_ptr
 *
 * @param v The vector.
 * @param s The scalar.
 * @return Result vector.
 */
vector3i vec3i_mul_scalar(const vector3i v, const vm_int_t s)
{
    vector3i res;
    vec3i_mul_scalar_ptr(&res, &v, s);
    return res;
}

/**
 * @brief Component-wise division of vector by scalar.
 *
 * @see vec3i_div_scalar_ptr
 *
 * @param v The vector.
 * @param s The scalar (non-zero).
 * @return Result vector.
 */
vector3i vec3i_div_scalar(const vector3i v, const vm_int_t s)
{
    vector3i res;
    vec3i_div_scalar_ptr(&res, &v, s);
    return res;
}

/**
 * @brief Component-wise multiplication of two vectors.
 *
 * @see vec3i_mul_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Result vector.
 */
vector3i vec3i_mul(const vector3i a, const vector3i b)
{
    vector3i res;
    vec3i_mul_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Negation of a vector.
 *
 * @see vec3i_neg_ptr
 *
 * @param v The vector.
 * @return The negated vector.
 */
vector3i vec3i_neg(const vector3i v)
{
    vector3i res;
    vec3i_neg_ptr(&res, &v);
    return res;
}

/**
 * @brief Computes the absolute value per component of a `vector3i`.
 *
 * @see vec3i_abs_ptr
 *
 * @param v The vector.
 * @return The absolute vector3i.
 */
vector3i vec3i_abs(const vector3i v)
{
    vector3i res;
    vec3i_abs_ptr(&res, &v);
    return res;
}

/**
 * @brief Computes the cross-product of two `vector3i`.
 *
 * @see vec3i_cross_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return The cross-product (a × b).
 */
vector3i vec3i_cross(const vector3i a, const vector3i b)
{
    vector3i res;
    vec3i_cross_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Normalizes a `vector3i` to unit length.
 *
 * @see vec3i_normalize_ptr
 *
 * @param v The vector (non-zero).
 * @return The normalized vector.
 */
vector3i vec3i_normalize(const vector3i v)
{
    vector3i res;
    vec3i_normalize_ptr(&res, &v);
    return res;
}

/**
 * @brief Computes the component-wise minimum of two `vector3i`.
 *
 * @see vec3i_min_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Per-component min(a, b).
 */
vector3i vec3i_min(const vector3i a, const vector3i b)
{
    vector3i res;
    vec3i_min_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Computes the component-wise maximum of two `vector3i`.
 *
 * @see vec3i_max_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Per-component max(a, b).
 */
vector3i vec3i_max(const vector3i a, const vector3i b)
{
    vector3i res;
    vec3i_max_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Computes the sign per component of a `vector3i` (-1, 0, or 1).
 *
 * @see vec3i_sign_ptr
 *
 * @param v The vector.
 * @return The sign vector.
 */
vector3i vec3i_sign(const vector3i v)
{
    vector3i res;
    vec3i_sign_ptr(&res, &v);
    return res;
}

/**
 * @brief Linearly interpolates between two `vector3i`.
 *
 * @see vec3i_lerp_ptr
 *
 * @param a Start vector.
 * @param b End vector.
 * @param t Interpolation factor [0, 1].
 * @return The interpolated vector (a * (1 - t) + b * t).
 */
vector3i vec3i_lerp(const vector3i a, const vector3i b, const vm_float_t t)
{
    vector3i res;
    vec3i_lerp_ptr(&res, &a, &b, t);
    return res;
}

/**
 * @brief Clamps a `vector3i` between min and max per component.
 *
 * @see vec3i_clamp_ptr
 *
 * @param v The vector.
 * @param min Minimum bounds.
 * @param max Maximum bounds.
 * @return The clamped vector.
 */
vector3i vec3i_clamp(const vector3i v, const vector3i min, const vector3i max)
{
    vector3i res;
    vec3i_clamp_ptr(&res, &v, &min, &max);
    return res;
}

/**
 * @brief Computes the dot product of two `vector3i`.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return The dot product (a.x * b.x + a.y * b.y + a.z * b.z).
 */
vm_int_t vec3i_dot(const vector3i a, const vector3i b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

/**
 * @brief Computes the length (magnitude) of a `vector3i`.
 *
 * @param v The vector.
 * @return The Euclidean length of v.
 */
vm_float_t vec3i_length(const vector3i v) {
    return VECMAT_SQRT(vec3i_dot(v, v));
}

/**
 * @brief Computes the Euclidean distance between two `vector3i`.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return The distance between a and b.
 */
vm_float_t vec3i_distance(const vector3i a, const vector3i b)
{
    return vec3i_length(vec3i_sub(a, b));
}

/**
 * @brief Computes the angle between two non-zero `vector3i` in radians.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return The angle between the directions of a and b.
 */
vm_float_t vec3i_angle(const vector3i a, const vector3i b)
{
    const vm_float_t dot = vec3i_dot(a, b);
    const vm_float_t len_a = vec3i_length(a);
    const vm_float_t len_b = vec3i_length(b);
    if (len_a == 0.0f || len_b == 0.0f) return 0.0f;
    return VECMAT_ACOS(dot / (len_a * len_b));
}

/**
 * @brief Divides two vectors component-wise.
 *
 * @see vec3i_div_ptr
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector3i.
 */
vector3i vec3i_div(const vector3i a, const vector3i b)
{
    vector3i res;
    vec3i_div_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Adds a scalar to each component.
 *
 * @see vec3i_add_scalar_ptr
 *
 * @param v Input vector.
 * @param s Scalar value.
 * @return The resulting vector3i.
 */
vector3i vec3i_add_scalar(const vector3i v, const vm_int_t s)
{
    vector3i res;
    vec3i_add_scalar_ptr(&res, &v, s);
    return res;
}

/**
 * @brief Subtracts a scalar from each component.
 *
 * @see vec3i_sub_scalar_ptr
 *
 * @param v Input vector.
 * @param s Scalar value.
 * @return The resulting vector3i.
 */
vector3i vec3i_sub_scalar(const vector3i v, const vm_int_t s)
{
    vector3i res;
    vec3i_sub_scalar_ptr(&res, &v, s);
    return res;
}

/**
 * @brief Component-wise floor modulo of a by b.
 *
 * @see vec3i_mod_ptr
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector3i.
 */
vector3i vec3i_mod(const vector3i a, const vector3i b)
{
    vector3i res;
    vec3i_mod_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Component-wise floored division of a by b.
 *
 * @see vec3i_div_floor_ptr
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector3i.
 */
vector3i vec3i_div_floor(const vector3i a, const vector3i b)
{
    vector3i res;
    vec3i_div_floor_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Wraps each component of v into [0, period).
 *
 * @see vec3i_wrap_ptr
 *
 * @param v Input vector.
 * @param period Wrap period per component.
 * @return The resulting vector3i.
 */
vector3i vec3i_wrap(const vector3i v, const vector3i period)
{
    vector3i res;
    vec3i_wrap_ptr(&res, &v, &period);
    return res;
}

/**
 * @brief Returns a vector with every component set to s.
 *
 * @see vec3i_splat_ptr
 *
 * @param s Scalar value.
 * @return The resulting vector3i.
 */
vector3i vec3i_splat(const vm_int_t s)
{
    return (vector3i){.x = s, .y = s, .z = s};
}

/**
 * @brief Builds a vector3i from a vector2i and z.
 *
 * @see vec3i_from_vec2i_ptr
 *
 * @param v Input vector.
 * @param z Z component.
 * @return The resulting vector3i.
 */
vector3i vec3i_from_vec2i(const vector2i v, const vm_int_t z)
{
    vector3i res;
    vec3i_from_vec2i_ptr(&res, &v, z);
    return res;
}

/**
 * @brief Returns the x and y components as a 2D vector.
 *
 * @see vec3i_xy_ptr
 *
 * @param v Input vector.
 * @return The resulting vector2i.
 */
vector2i vec3i_xy(const vector3i v)
{
    vector2i res;
    vec3i_xy_ptr(&res, &v);
    return res;
}

/**
 * @brief Converts to a unit-length vector3.
 *
 * @see vec3i_normalize_to_vec3_ptr
 *
 * @param v Input vector.
 * @return The resulting vector3.
 */
vector3 vec3i_normalize_to_vec3(const vector3i v)
{
    vector3 res;
    vec3i_normalize_to_vec3_ptr(&res, &v);
    return res;
}

/**
 * @brief Returns the squared Euclidean length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_int_t vec3i_length_squared(const vector3i v)
{
    return vec3i_dot(v, v);
}

/**
 * @brief Returns the squared Euclidean distance between a and b.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting scalar.
 */
vm_int_t vec3i_distance_squared(const vector3i a, const vector3i b)
{
    const vm_int_t dx = a.x - b.x;
    const vm_int_t dy = a.y - b.y;
    const vm_int_t dz = a.z - b.z;
    return dx * dx + dy * dy + dz * dz;
}

/**
 * @brief Returns the Manhattan (L1) length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_int_t vec3i_length_manhattan(const vector3i v)
{
    const vm_int_t ax = v.x < 0 ? -v.x : v.x;
    const vm_int_t ay = v.y < 0 ? -v.y : v.y;
    const vm_int_t az = v.z < 0 ? -v.z : v.z;
    return ax + ay + az;
}

/**
 * @brief Returns the Chebyshev (L-inf) length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_int_t vec3i_length_chebyshev(const vector3i v)
{
    const vm_int_t ax = v.x < 0 ? -v.x : v.x;
    const vm_int_t ay = v.y < 0 ? -v.y : v.y;
    const vm_int_t az = v.z < 0 ? -v.z : v.z;
    const vm_int_t m = ax > ay ? ax : ay;
    return m > az ? m : az;
}

/**
 * @brief Returns the smallest component.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_int_t vec3i_min_component(const vector3i v)
{
    const vm_int_t m = v.x < v.y ? v.x : v.y;
    return m < v.z ? m : v.z;
}

/**
 * @brief Returns the largest component.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_int_t vec3i_max_component(const vector3i v)
{
    const vm_int_t m = v.x > v.y ? v.x : v.y;
    return m > v.z ? m : v.z;
}

/**
 * @brief Returns the sum of all components.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_int_t vec3i_sum(const vector3i v)
{
    return v.x + v.y + v.z;
}

/**
 * @brief Returns true if every component is zero.
 *
 * @param v Input vector.
 * @return True if every component is zero.
 */
bool vec3i_is_zero(const vector3i v)
{
    return v.x == 0 && v.y == 0 && v.z == 0;
}
