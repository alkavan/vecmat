// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

/**
 * @brief Returns a zero-initialized `vector4`.
 *
 * @return A vector4 with all components set to 0.0f.
 */
vector4 vec4_zero(void)
{
    return (vector4){.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f};
}

/**
 * Returns a `vector4` with all components set to 1.0f.
 *
 * @return A `vector4` with all components set to 1.0f.
 */
vector4 vec4_one(void)
{
    return (vector4){.x = 1.0f, .y = 1.0f, .z = 1.0f, .w = 1.0f};
}

/**
 * @brief Returns a `vector4` along the x-axis.
 *
 * @param x The x component value.
 * @return A vector4 with (x, 0.0f, 0.0f, 0.0f).
 */
vector4 vec4_x_axis(const vm_float_t x)
{
    return (vector4){.x = x, .y = 0.0f, .z = 0.0f, .w = 0.0f};
}

/**
 * @brief Returns a `vector4` along the y-axis.
 *
 * @param y The y component value.
 * @return A vector4 with (0.0f, y, 0.0f, 0.0f).
 */
vector4 vec4_y_axis(const vm_float_t y)
{
    return (vector4){.x = 0.0f, .y = y, .z = 0.0f, .w = 0.0f};
}

/**
 * @brief Returns a `vector4` along the z-axis.
 *
 * @param z The z component value.
 * @return A vector4 with (0.0f, 0.0f, z, 0.0f).
 */
vector4 vec4_z_axis(const vm_float_t z)
{
    return (vector4){.x = 0.0f, .y = 0.0f, .z = z, .w = 0.0f};
}

/**
 * @brief Returns a `vector4` along the w-axis.
 *
 * @param w The w component value.
 * @return A vector4 with (0.0f, 0.0f, 0.0f, w).
 */
vector4 vec4_w_axis(const vm_float_t w)
{
    return (vector4){.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = w};
}

/**
 * @brief Returns a`vector4` for scaling along the x-axis.
 *
 * @param x The x scale factor.
 * @return A vector4 with (x, 1.0f, 1.0f, 1.0f).
 */
vector4 vec4_x_scale(const vm_float_t x)
{
    return (vector4){.x = x, .y = 1.0f, .z = 1.0f, .w = 1.0f};
}

/**
 * @brief Returns a `vector4` for scaling along the y-axis.
 *
 * @param y The y scale factor.
 * @return A vector4 with (1.0f, y, 1.0f, 1.0f).
 */
vector4 vec4_y_scale(const vm_float_t y)
{
    return (vector4){.x = 1.0f, .y = y, .z = 1.0f, .w = 1.0f};
}

/**
 * @brief Returns a `vector4`for scaling along the z-axis.
 *
 * @param z The z scale factor.
 * @return A vector4 with (1.0f, 1.0f, z, 1.0f).
 */
vector4 vec4_z_scale(const vm_float_t z)
{
    return (vector4){.x = 1.0f, .y = 1.0f, .z = z, .w = 1.0f};
}

/**
 * @brief Returns a vector4 for scaling along the w-axis.
 *
 * @param w The w scale factor.
 * @return A vector4 with (1.0f, 1.0f, 1.0f, w).
 */
vector4 vec4_w_scale(const vm_float_t w)
{
    return (vector4){.x = 1.0f, .y = 1.0f, .z = 1.0f, .w = w};
}

/**
 * @brief Component-wise addition of two vectors.
 *
 * See `vec4_add_ptr` for the out-parameter version.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Result vector.
 */
vector4 vec4_add(const vector4 a, const vector4 b)
{
    vector4 res;
    vec4_add_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Component-wise subtraction of two vectors.
 *
 * See `vec4_sub_ptr` for the out-parameter version.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Result vector.
 */
vector4 vec4_sub(const vector4 a, const vector4 b)
{
    vector4 res;
    vec4_sub_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Component-wise multiplication of vector by scalar.
 *
 * See `vec4_mul_scalar_ptr` for the out-parameter version.
 *
 * @param v The vector.
 * @param s The scalar.
 * @return Result vector.
 */
vector4 vec4_mul_scalar(const vector4 v, const vm_float_t s)
{
    vector4 res;
    vec4_mul_scalar_ptr(&res, &v, s);
    return res;
}

/**
 * @brief Component-wise division of vector by scalar.
 *
 * See `vec4_div_scalar_ptr` for the out-parameter version.
 *
 * @param v The vector.
 * @param s The scalar (non-zero).
 * @return Result vector.
 */
vector4 vec4_div_scalar(const vector4 v, const vm_float_t s)
{
    vector4 res;
    vec4_div_scalar_ptr(&res, &v, s);
    return res;
}

/**
 * @brief Component-wise multiplication of two vectors.
 *
 * See `vec4_mul_ptr` for the out-parameter version.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Result vector.
 */
vector4 vec4_mul(const vector4 a, const vector4 b)
{
    vector4 res;
    vec4_mul_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Negation of a vector.
 *
 * See `vec4_neg_ptr` for the out-parameter version.
 *
 * @param v The vector.
 * @return The negated vector.
 */
vector4 vec4_neg(const vector4 v)
{
    vector4 res;
    vec4_neg_ptr(&res, &v);
    return res;
}

/**
 * @brief Computes the absolute value per component of a `vector4`.
 *
 * See `vec4_abs_ptr` for the out-parameter version.
 *
 * @param v The vector.
 * @return The absolute vector4.
 */
vector4 vec4_abs(const vector4 v)
{
    vector4 res;
    vec4_abs_ptr(&res, &v);
    return res;
}

/**
 * @brief Normalizes a `vector4` to unit length.
 *
 * See `vec4_normalize_ptr` for the out-parameter version.
 *
 * @param v The vector (non-zero).
 * @return The normalized vector.
 */
vector4 vec4_normalize(const vector4 v)
{
    vector4 res;
    vec4_normalize_ptr(&res, &v);
    return res;
}

/**
 * @brief Computes the component-wise minimum of two `vector4`.
 *
 * See `vec4_min_ptr` for the out-parameter version.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Per-component min(a, b).
 */
vector4 vec4_min(const vector4 a, const vector4 b)
{
    vector4 res;
    vec4_min_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Computes the component-wise maximum of two `vector4`.
 *
 * See `vec4_max_ptr` for the out-parameter version.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Per-component max(a, b).
 */
vector4 vec4_max(const vector4 a, const vector4 b)
{
    vector4 res;
    vec4_max_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Computes the sign per component of a `vector4` (-1, 0, or 1).
 *
 * See `vec4_sign_ptr` for the out-parameter version.
 *
 * @param v The vector.
 * @return The sign vector.
 */
vector4 vec4_sign(const vector4 v)
{
    vector4 res;
    vec4_sign_ptr(&res, &v);
    return res;
}

/**
 * @brief Applies the floor per component to a `vector4`.
 *
 * See `vec4_floor_ptr` for the out-parameter version.
 *
 * @param v The vector.
 * @return The floored vector.
 */
vector4 vec4_floor(const vector4 v)
{
    vector4 res;
    vec4_floor_ptr(&res, &v);
    return res;
}

/**
 * @brief Applies ceil per component to a `vector4`.
 *
 * See `vec4_ceil_ptr` for the out-parameter version.
 *
 * @param v The vector.
 * @return The ceiling vector.
 */
vector4 vec4_ceil(const vector4 v)
{
    vector4 res;
    vec4_ceil_ptr(&res, &v);
    return res;
}

/**
 * @brief Applies round per component to a `vector4`.
 *
 * See `vec4_round_ptr` for the out-parameter version.
 *
 * @param v The vector.
 * @return The rounded vector.
 */
vector4 vec4_round(const vector4 v)
{
    vector4 res;
    vec4_round_ptr(&res, &v);
    return res;
}

/**
 * @brief Linearly interpolates between two `vector4`.
 *
 * See `vec4_lerp_ptr` for the out-parameter version.
 *
 * @param a Start vector.
 * @param b End vector.
 * @param t Interpolation factor [0, 1].
 * @return The interpolated vector (a * (1 - t) + b * t).
 */
vector4 vec4_lerp(const vector4 a, const vector4 b, const vm_float_t t)
{
    vector4 res;
    vec4_lerp_ptr(&res, &a, &b, t);
    return res;
}

/**
 * @brief Clamps a `vector4` between min and max per component.
 *
 * See `vec4_clamp_ptr` for the out-parameter version.
 *
 * @param v The vector.
 * @param min Minimum bounds.
 * @param max Maximum bounds.
 * @return The clamped vector.
 */
vector4 vec4_clamp(const vector4 v, const vector4 min, const vector4 max)
{
    vector4 res;
    vec4_clamp_ptr(&res, &v, &min, &max);
    return res;
}

/**
 * @brief Homogenizes a `vector4` (divides x, y, z by w).
 *
 * See `vec4_homogenize_ptr` for the out-parameter version.
 *
 * @param v The homogeneous vector (w != 0.0f).
 * @return The normalized cartesian vector4.
 */
vector4 vec4_homogenize(const vector4 v)
{
    vector4 res;
    vec4_homogenize_ptr(&res, &v);
    return res;
}

/**
 * @brief Computes the dot product of two `vector4`.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return The dot product (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w).
 */
vm_float_t vec4_dot(const vector4 a, const vector4 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

/**
 * @brief Computes the length (magnitude) of a `vector4`.
 *
 * @param v The vector.
 * @return The Euclidean length of v.
 */
vm_float_t vec4_length(const vector4 v)
{
    return VECMAT_SQRT(vec4_dot(v, v));
}

/**
 * @brief Computes the Euclidean distance between two `vector4`.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return The distance between a and b.
 */
vm_float_t vec4_distance(const vector4 a, const vector4 b)
{
    const vector4 diff = vec4_sub(a, b);
    return vec4_length(diff);
}

/**
 * @brief Converts a `vector4` to a `vector3` (discards the w component).
 *
 * @param v The source vector4.
 * @return A vector3 {v.x, v.y, v.z}.
 */
vector3 vec4_to_vec3(const vector4 v)
{
    vector3 res;
    vec4_to_vec3_ptr(&res, &v);
    return res;
}

/**
 * @brief Divides two vectors component-wise.
 *
 * See `vec4_div_ptr` instead.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector4.
 */
vector4 vec4_div(const vector4 a, const vector4 b)
{
    vector4 res;
    vec4_div_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Adds a scalar to each component.
 *
 * See `vec4_add_scalar_ptr` instead.
 *
 * @param v Input vector.
 * @param s Scalar value.
 * @return The resulting vector4.
 */
vector4 vec4_add_scalar(const vector4 v, const vm_float_t s)
{
    vector4 res;
    vec4_add_scalar_ptr(&res, &v, s);
    return res;
}

/**
 * @brief Subtracts a scalar from each component.
 *
 * See `vec4_sub_scalar_ptr` instead.
 *
 * @param v Input vector.
 * @param s Scalar value.
 * @return The resulting vector4.
 */
vector4 vec4_sub_scalar(const vector4 v, const vm_float_t s)
{
    vector4 res;
    vec4_sub_scalar_ptr(&res, &v, s);
    return res;
}

/**
 * @brief Clamps each component to the scalar range [min, max].
 *
 * See `vec4_clamp_scalar_ptr` instead.
 *
 * @param v Input vector.
 * @param min Lower bound.
 * @param max Upper bound.
 * @return The resulting vector4.
 */
vector4 vec4_clamp_scalar(const vector4 v, const vm_float_t min, const vm_float_t max)
{
    vector4 res;
    vec4_clamp_scalar_ptr(&res, &v, min, max);
    return res;
}

/**
 * @brief Clamps each component to the range [0, 1].
 *
 * See `vec4_saturate_ptr` instead.
 *
 * @param v Input vector.
 * @return The resulting vector4.
 */
vector4 vec4_saturate(const vector4 v)
{
    vector4 res;
    vec4_saturate_ptr(&res, &v);
    return res;
}

/**
 * @brief Returns the fractional part of each component.
 *
 * See `vec4_fract_ptr` instead.
 *
 * @param v Input vector.
 * @return The resulting vector4.
 */
vector4 vec4_fract(const vector4 v)
{
    vector4 res;
    vec4_fract_ptr(&res, &v);
    return res;
}

/**
 * @brief Projects a onto b.
 *
 * See `vec4_project_ptr` instead.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector4.
 */
vector4 vec4_project(const vector4 a, const vector4 b)
{
    vector4 res;
    vec4_project_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Returns the component of a orthogonal to b.
 *
 * See `vec4_reject_ptr` instead.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector4.
 */
vector4 vec4_reject(const vector4 a, const vector4 b)
{
    vector4 res;
    vec4_reject_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Removes the component of v along normal.
 *
 * See `vec4_slide_ptr` instead.
 *
 * @param v Input vector.
 * @param normal Surface normal.
 * @return The resulting vector4.
 */
vector4 vec4_slide(const vector4 v, const vector4 normal)
{
    vector4 res;
    vec4_slide_ptr(&res, &v, &normal);
    return res;
}

/**
 * @brief Returns a vector with every component set to s.
 *
 * See `vec4_splat_ptr` instead.
 *
 * @param s Scalar value.
 * @return The resulting vector4.
 */
vector4 vec4_splat(const vm_float_t s)
{
    return (vector4){.x = s, .y = s, .z = s, .w = s};
}

/**
 * @brief Returns the squared Euclidean length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_float_t vec4_length_squared(const vector4 v)
{
    return vec4_dot(v, v);
}

/**
 * @brief Returns the squared Euclidean distance between a and b.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting scalar.
 */
vm_float_t vec4_distance_squared(const vector4 a, const vector4 b)
{
    return vec4_length_squared(vec4_sub(a, b));
}

/**
 * @brief Returns true if every component is zero.
 *
 * @param v Input vector.
 * @return True if every component is zero.
 */
bool vec4_is_zero(const vector4 v)
{
    return vec4_length_squared(v) <= VECMAT_EPSILON * VECMAT_EPSILON;
}

/**
 * @brief Returns true if the vector has unit length.
 *
 * @param v Input vector.
 * @return True if the vector has unit length.
 */
bool vec4_is_normalized(const vector4 v)
{
    return VECMAT_FABS(vec4_length_squared(v) - 1.0f) < VECMAT_EPSILON;
}

/**
 * @brief Returns true if a and b are within eps of each other.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @param eps Distance tolerance.
 * @return True if a and b are within eps.
 */
bool vec4_near(const vector4 a, const vector4 b, const vm_float_t eps)
{
    return VECMAT_FABS(a.x - b.x) <= eps
        && VECMAT_FABS(a.y - b.y) <= eps
        && VECMAT_FABS(a.z - b.z) <= eps
        && VECMAT_FABS(a.w - b.w) <= eps;
}
