// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

/**
 * @brief Returns a zero-initialized `vector4i`.
 *
 * @return A `vector4i` with all components set to 0.
 */
vector4i vec4i_zero(void)
{
    return (vector4i){.x = 0, .y = 0, .z = 0, .w = 0};
}

/**
 * @brief Returns a `vector4i` with all components set to 1.
 *
 * @return A `vector4i` with all components set to 1.
 */
vector4i vec4i_one(void)
{
    return (vector4i){.x = 1, .y = 1, .z = 1, .w = 1};
}

/**
 * @brief Returns a `vector4i` along the x-axis.
 *
 * @param x The x component value.
 * @return A `vector4i` with (x, 0, 0, 0).
 */
vector4i vec4i_x_axis(const vm_int_t x)
{
    return (vector4i){.x = x, .y = 0, .z = 0, .w = 0};
}

/**
 * @brief Returns a `vector4i` along the y-axis.
 *
 * @param y The y component value.
 * @return A `vector4i` with (0, y, 0, 0).
 */
vector4i vec4i_y_axis(const vm_int_t y)
{
    return (vector4i){.x = 0, .y = y, .z = 0, .w = 0};
}

/**
 * @brief Returns a `vector4i` along the z-axis.
 *
 * @param z The z component value.
 * @return A `vector4i` with (0, 0, z, 0).
 */
vector4i vec4i_z_axis(const vm_int_t z) {
    return (vector4i){.x = 0, .y = 0, .z = z, .w = 0};
}

/**
 * @brief Returns a `vector4i` along the w-axis.
 *
 * @param w The w component value.
 * @return A `vector4i` with (0, 0, 0, w).
 */
vector4i vec4i_w_axis(const vm_int_t w)
{
    return (vector4i){.x = 0, .y = 0, .z = 0, .w = w};
}

/**
 * @brief Returns a `vector4i` for scaling along the x-axis.
 *
 * @param x The x scale factor.
 * @return A `vector4i` with (x, 1, 1, 1).
 */
vector4i vec4i_x_scale(const vm_int_t x)
{
    return (vector4i){.x = x, .y = 1, .z = 1, .w = 1};
}

/**
 * @brief Returns a `vector4i` for scaling along the y-axis.
 *
 * @param y The y scale factor.
 * @return A `vector4i` with (1, y, 1, 1).
 */
vector4i vec4i_y_scale(const vm_int_t y)
{
    return (vector4i){.x = 1, .y = y, .z = 1, .w = 1};
}

/**
 * @brief Returns a `vector4i` for scaling along the z-axis.
 *
 * @param z The z scale factor.
 * @return A `vector4i` with (1, 1, z, 1).
 */
vector4i vec4i_z_scale(const vm_int_t z)
{
    return (vector4i){.x = 1, .y = 1, .z = z, .w = 1};
}

/**
 * @brief Returns a `vector4i` for scaling along the w-axis.
 *
 * @param w The w scale factor.
 * @return A `vector4i` with (1, 1, 1, w).
 */
vector4i vec4i_w_scale(const vm_int_t w)
{
    return (vector4i){.x = 1, .y = 1, .z = 1, .w = w};
}

/**
 * @brief Component-wise addition of two vectors.
 *
 * See `vec4i_add_ptr` for the out-parameter version.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Result vector.
 */
vector4i vec4i_add(const vector4i a, const vector4i b)
{
    vector4i res;
    vec4i_add_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Component-wise subtraction of two vectors.
 *
 * See `vec4i_sub_ptr` for the out-parameter version.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Result vector.
 */
vector4i vec4i_sub(const vector4i a, const vector4i b)
{
    vector4i res;
    vec4i_sub_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Component-wise multiplication of vector by scalar.
 *
 * See `vec4i_mul_scalar_ptr` for the out-parameter version.
 *
 * @param v The vector.
 * @param s The scalar.
 * @return Result vector.
 */
vector4i vec4i_mul_scalar(const vector4i v, const vm_int_t s)
{
    vector4i res;
    vec4i_mul_scalar_ptr(&res, &v, s);
    return res;
}

/**
 * @brief Component-wise division of vector by scalar.
 *
 * See `vec4i_div_scalar_ptr` for the out-parameter version.
 *
 * @param v The vector.
 * @param s The scalar (non-zero).
 * @return Result vector.
 */
vector4i vec4i_div_scalar(const vector4i v, const vm_int_t s)
{
    vector4i res;
    vec4i_div_scalar_ptr(&res, &v, s);
    return res;
}

/**
 * @brief Component-wise multiplication of two vectors.
 *
 * See `vec4i_mul_ptr` for the out-parameter version.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Result vector.
 */
vector4i vec4i_mul(const vector4i a, const vector4i b)
{
    vector4i res;
    vec4i_mul_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Negation of a vector.
 *
 * See `vec4i_neg_ptr` for the out-parameter version.
 *
 * @param v The vector.
 * @return The negated vector.
 */
vector4i vec4i_neg(const vector4i v)
{
    vector4i res;
    vec4i_neg_ptr(&res, &v);
    return res;
}

/**
 * @brief Computes the absolute value per component of a `vector4i`.
 *
 * See `vec4i_abs_ptr` for the out-parameter version.
 *
 * @param v The vector.
 * @return The absolute `vector4i`.
 */
vector4i vec4i_abs(const vector4i v)
{
    vector4i res;
    vec4i_abs_ptr(&res, &v);
    return res;
}

/**
 * @brief Normalizes a `vector4i` to unit length.
 *
 * See `vec4i_normalize_ptr` for the out-parameter version.
 *
 * @param v The vector (non-zero).
 * @return The normalized vector.
 */
vector4i vec4i_normalize(const vector4i v)
{
    vector4i res;
    vec4i_normalize_ptr(&res, &v);
    return res;
}

/**
 * @brief Computes the component-wise minimum of two `vector4i`.
 *
 * See `vec4i_min_ptr` for the out-parameter version.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Per-component min(a, b).
 */
vector4i vec4i_min(const vector4i a, const vector4i b)
{
    vector4i res;
    vec4i_min_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Computes the component-wise maximum of two vector4i.
 *
 * See `vec4i_max_ptr` for the out-parameter version.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Per-component max(a, b).
 */
vector4i vec4i_max(const vector4i a, const vector4i b)
{
    vector4i res;
    vec4i_max_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Computes the sign per component of a `vector4i` (-1, 0, or 1).
 *
 * See `vec4i_sign_ptr` for the out-parameter version.
 *
 * @param v The vector.
 * @return The sign vector.
 */
vector4i vec4i_sign(const vector4i v)
{
    vector4i res;
    vec4i_sign_ptr(&res, &v);
    return res;
}

/**
 * @brief Linearly interpolates between two `vector4i`.
 *
 * See `vec4i_lerp_ptr` for the out-parameter version.
 *
 * @param a Start vector.
 * @param b End vector.
 * @param t Interpolation factor [0, 1].
 * @return The interpolated vector (a * (1 - t) + b * t).
 */
vector4i vec4i_lerp(const vector4i a, const vector4i b, const vm_float_t t)
{
    vector4i res;
    vec4i_lerp_ptr(&res, &a, &b, t);
    return res;
}

/**
 * @brief Clamps each component of the input vector to the range defined by the
 * minimum and maximum vectors.
 *
 * See `vec4i_clamp_ptr` for the out-parameter version.
 *
 * @param v The input vector.
 * @param min The minimum value for each component.
 * @param max The maximum value for each component.
 * @return A new `vector4i` with each component clamped to [min, max].
 */
vector4i vec4i_clamp(const vector4i v, const vector4i min, const vector4i max)
{
    vector4i res;
    vec4i_clamp_ptr(&res, &v, &min, &max);
    return res;
}

/**
 * @brief Computes the dot product of two `vector4i`.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return The dot product.
 */
vm_int_t vec4i_dot(const vector4i a, const vector4i b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

/**
 * @brief Computes the length (magnitude) of a `vector4i`.
 *
 * @param v The vector.
 * @return The Euclidean length of v.
 */
vm_float_t vec4i_length(const vector4i v)
{
    return VECMAT_SQRT(vec4i_dot(v, v));
}

/**
 * @brief Computes the Euclidean distance between two `vector4i`.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return The distance between a and b.
 */
vm_float_t vec4i_distance(const vector4i a, const vector4i b)
{
    const vector4i diff = vec4i_sub(a, b);
    return vec4i_length(diff);
}

/**
 * @brief Converts a `vector4i` to a `vector3i` (discards the w component).
 *
 * @param v The source `vector4i`.
 * @return A `vector3i` {v.x, v.y, v.z}.
 */
vector3i vec4i_to_vec3i(const vector4i v)
{
    vector3i res;
    vec4i_to_vec3i_ptr(&res, &v);
    return res;
}

/**
 * @brief Divides two vectors component-wise.
 *
 * See `vec4i_div_ptr` instead.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector4i.
 */
vector4i vec4i_div(const vector4i a, const vector4i b)
{
    vector4i res;
    vec4i_div_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Adds a scalar to each component.
 *
 * See `vec4i_add_scalar_ptr` instead.
 *
 * @param v Input vector.
 * @param s Scalar value.
 * @return The resulting vector4i.
 */
vector4i vec4i_add_scalar(const vector4i v, const vm_int_t s)
{
    vector4i res;
    vec4i_add_scalar_ptr(&res, &v, s);
    return res;
}

/**
 * @brief Subtracts a scalar from each component.
 *
 * See `vec4i_sub_scalar_ptr` instead.
 *
 * @param v Input vector.
 * @param s Scalar value.
 * @return The resulting vector4i.
 */
vector4i vec4i_sub_scalar(const vector4i v, const vm_int_t s)
{
    vector4i res;
    vec4i_sub_scalar_ptr(&res, &v, s);
    return res;
}

/**
 * @brief Component-wise floor modulo of a by b.
 *
 * See `vec4i_mod_ptr` instead.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector4i.
 */
vector4i vec4i_mod(const vector4i a, const vector4i b)
{
    vector4i res;
    vec4i_mod_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Component-wise floored division of a by b.
 *
 * See `vec4i_div_floor_ptr` instead.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector4i.
 */
vector4i vec4i_div_floor(const vector4i a, const vector4i b)
{
    vector4i res;
    vec4i_div_floor_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Wraps each component of v into [0, period).
 *
 * See `vec4i_wrap_ptr` instead.
 *
 * @param v Input vector.
 * @param period Wrap period per component.
 * @return The resulting vector4i.
 */
vector4i vec4i_wrap(const vector4i v, const vector4i period)
{
    vector4i res;
    vec4i_wrap_ptr(&res, &v, &period);
    return res;
}

/**
 * @brief Returns a vector with every component set to s.
 *
 * See `vec4i_splat_ptr` instead.
 *
 * @param s Scalar value.
 * @return The resulting vector4i.
 */
vector4i vec4i_splat(const vm_int_t s)
{
    return (vector4i){.x = s, .y = s, .z = s, .w = s};
}

/**
 * @brief Returns the squared Euclidean length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_int_t vec4i_length_squared(const vector4i v)
{
    return vec4i_dot(v, v);
}

/**
 * @brief Returns the squared Euclidean distance between a and b.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting scalar.
 */
vm_int_t vec4i_distance_squared(const vector4i a, const vector4i b)
{
    return vec4i_length_squared(vec4i_sub(a, b));
}

/**
 * @brief Returns the Manhattan (L1) length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_int_t vec4i_length_manhattan(const vector4i v)
{
    const vm_int_t ax = v.x < 0 ? -v.x : v.x;
    const vm_int_t ay = v.y < 0 ? -v.y : v.y;
    const vm_int_t az = v.z < 0 ? -v.z : v.z;
    const vm_int_t aw = v.w < 0 ? -v.w : v.w;
    return ax + ay + az + aw;
}

/**
 * @brief Returns the Chebyshev (L-inf) length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
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

/**
 * @brief Returns the smallest component.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_int_t vec4i_min_component(const vector4i v)
{
    vm_int_t m = v.x;
    if (v.y < m) m = v.y;
    if (v.z < m) m = v.z;
    if (v.w < m) m = v.w;
    return m;
}

/**
 * @brief Returns the largest component.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_int_t vec4i_max_component(const vector4i v)
{
    vm_int_t m = v.x;
    if (v.y > m) m = v.y;
    if (v.z > m) m = v.z;
    if (v.w > m) m = v.w;
    return m;
}

/**
 * @brief Returns the sum of all components.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_int_t vec4i_sum(const vector4i v)
{
    return v.x + v.y + v.z + v.w;
}

/**
 * @brief Returns true if every component is zero.
 *
 * @param v Input vector.
 * @return True if every component is zero.
 */
bool vec4i_is_zero(const vector4i v)
{
    return v.x == 0 && v.y == 0 && v.z == 0 && v.w == 0;
}
