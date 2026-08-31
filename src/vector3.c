// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

/**
 * @brief Returns a zero-initialized `vector3`.
 *
 * @return A vector3 with all components set to 0.0f.
 */
vector3 vec3_zero(void)
{
    return (vector3){.x = 0.0f, .y = 0.0f, .z = 0.0f};
}

/**
 * @brief Returns a `vector3` with all components set to 1.0f.
 *
 * @return A vector3 with all components set to 1.0f.
 */
vector3 vec3_one(void)
{
    return (vector3){.x = 1.0f, .y = 1.0f, .z = 1.0f};
}

/**
 * @brief Returns a `vector3` along the x-axis.
 *
 * @param x The x component value.
 * @return A vector3 with (x, 0.0f, 0.0f).
 */
vector3 vec3_x_axis(const vm_float_t x)
{
    return (vector3){.x = x, .y = 0.0f, .z = 0.0f};
}

/**
 * @brief Returns a `vector3` along the y-axis.
 *
 * @param y The y component value.
 * @return A vector3 with (0.0f, y, 0.0f).
 */
vector3 vec3_y_axis(const vm_float_t y)
{
    return (vector3){.x = 0.0f, .y = y, .z = 0.0f};
}

/**
 * @brief Returns a `vector3` along the z-axis.
 *
 * @param z The z component value.
 * @return A vector3 with (0.0f, 0.0f, z).
 */
vector3 vec3_z_axis(const vm_float_t z)
{
    return (vector3){.x = 0.0f, .y = 0.0f, .z = z};
}

/**
 * @brief Returns a `vector3` for scaling along the x-axis.
 *
 * @param x The x scale factor.
 * @return A vector3 with (x, 1.0f, 1.0f).
 */
vector3 vec3_x_scale(const vm_float_t x)
{
    return (vector3){.x = x, .y = 1.0f, .z = 1.0f};
}

/**
 * @brief Returns a `vector3` for scaling along the y-axis.
 *
 * @param y The y scale factor.
 * @return A vector3 with (1.0f, y, 1.0f).
 */
vector3 vec3_y_scale(const vm_float_t y)
{
    return (vector3){.x = 1.0f, .y = y, .z = 1.0f};
}

/**
 * @brief Returns a `vector3` for scaling along the z-axis.
 *
 * @param z The z scale factor.
 * @return A vector3 with (1.0f, 1.0f, z).
 */
vector3 vec3_z_scale(const vm_float_t z)
{
    return (vector3){.x = 1.0f, .y = 1.0f, .z = z};
}

/**
 * @brief Component-wise addition of two vectors.
 *
 * @see vec3_add_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Result vector.
 */
vector3 vec3_add(const vector3 a, const vector3 b)
{
    vector3 res;
    vec3_add_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Component-wise subtraction of two vectors.
 *
 * @see vec3_sub_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Result vector.
 */
vector3 vec3_sub(const vector3 a, const vector3 b)
{
    vector3 res;
    vec3_sub_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Component-wise multiplication of vector by scalar.
 *
 * @see vec3_mul_scalar_ptr
 *
 * @param v The vector.
 * @param s The scalar.
 * @return Result vector.
 */
vector3 vec3_mul_scalar(const vector3 v, const vm_float_t s)
{
    vector3 res;
    vec3_mul_scalar_ptr(&res, &v, s);
    return res;
}

/**
 * @brief Component-wise division of vector by scalar.
 *
 * @see vec3_div_scalar_ptr
 *
 * @param v The vector.
 * @param s The scalar (non-zero).
 * @return Result vector.
 */
vector3 vec3_div_scalar(const vector3 v, const vm_float_t s)
{
    vector3 res;
    vec3_div_scalar_ptr(&res, &v, s);
    return res;
}

/**
 * @brief Component-wise multiplication of two vectors.
 *
 * @see vec3_mul_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Result vector.
 */
vector3 vec3_mul(const vector3 a, const vector3 b)
{
    vector3 res;
    vec3_mul_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Negation of a vector.
 *
 * @see vec3_neg_ptr
 *
 * @param v The vector.
 * @return The negated vector.
 */
vector3 vec3_neg(const vector3 v)
{
    vector3 res;
    vec3_neg_ptr(&res, &v);
    return res;
}

/**
 * @brief Computes the absolute value per component of a vector3.
 *
 * @see vec3_abs_ptr
 *
 * @param v The vector.
 * @return The absolute vector3.
 */
vector3 vec3_abs(const vector3 v)
{
    vector3 res;
    vec3_abs_ptr(&res, &v);
    return res;
}

/**
 * @brief Computes the cross-product of two vector3.
 *
 * @see vec3_cross_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return The cross-product (a × b).
 */
vector3 vec3_cross(const vector3 a, const vector3 b)
{
    vector3 res;
    vec3_cross_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Normalizes a vector3 to unit length.
 *
 * @see vec3_normalize_ptr
 *
 * @param v The vector (non-zero).
 * @return The normalized vector.
 */
vector3 vec3_normalize(const vector3 v)
{
    vector3 res;
    vec3_normalize_ptr(&res, &v);
    return res;
}

/**
 * @brief Computes the component-wise minimum of two vector3.
 *
 * @see vec3_min_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Per-component min(a, b).
 */
vector3 vec3_min(const vector3 a, const vector3 b)
{
    vector3 res;
    vec3_min_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Computes the component-wise maximum of two vector3.
 *
 * @see vec3_max_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Per-component max(a, b).
 */
vector3 vec3_max(const vector3 a, const vector3 b)
{
    vector3 res;
    vec3_max_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Computes the sign per component of a vector3 (-1, 0, or 1).
 *
 * @see vec3_sign_ptr
 *
 * @param v The vector.
 * @return The sign vector.
 */
vector3 vec3_sign(const vector3 v)
{
    vector3 res;
    vec3_sign_ptr(&res, &v);
    return res;
}

/**
 * @brief Applies the floor per component to a vector3.
 *
 * @see vec3_floor_ptr
 *
 * @param v The vector.
 * @return The floored vector.
 */
vector3 vec3_floor(const vector3 v)
{
    vector3 res;
    vec3_floor_ptr(&res, &v);
    return res;
}

/**
 * @brief Applies ceil per component to a vector3.
 *
 * @see vec3_ceil_ptr
 *
 * @param v The vector.
 * @return The ceiling vector.
 */
vector3 vec3_ceil(const vector3 v)
{
    vector3 res;
    vec3_ceil_ptr(&res, &v);
    return res;
}

/**
 * @brief Applies round per component to a vector3.
 *
 * @see vec3_round_ptr
 *
 * @param v The vector.
 * @return The rounded vector.
 */
vector3 vec3_round(const vector3 v)
{
    vector3 res;
    vec3_round_ptr(&res, &v);
    return res;
}

/**
 * @brief Reflects an incident vector over normal.
 *
 * @see vec3_reflect_ptr
 *
 * @param incident The incident vector.
 * @param normal The surface normal.
 * @return The reflected vector.
 */
vector3 vec3_reflect(const vector3 incident, const vector3 normal)
{
    vector3 res;
    vec3_reflect_ptr(&res, &incident, &normal);
    return res;
}

/**
 * @brief Refracts an incident vector across an interface with a given normal
 * and ratio of refraction eta.
 *
 * @see vec3_refract_ptr
 *
 * @param incident The incident vector.
 * @param normal The surface normal.
 * @param eta The ratio of refraction (eta = n1 / n2).
 * @return The refracted vector, or incident if total internal reflection.
 */
vector3 vec3_refract(const vector3 incident, const vector3 normal, const vm_float_t eta)
{
    vector3 res;
    vec3_refract_ptr(&res, &incident, &normal, eta);
    return res;
}

/**
 * @brief Linearly interpolates between two `vector3`.
 *
 * @see vec3_lerp_ptr
 *
 * @param a Start vector.
 * @param b End vector.
 * @param t Interpolation factor [0, 1].
 * @return The interpolated vector (a * (1 - t) + b * t).
 */
vector3 vec3_lerp(const vector3 a, const vector3 b, const vm_float_t t)
{
    vector3 res;
    vec3_lerp_ptr(&res, &a, &b, t);
    return res;
}

/**
 * @brief Clamps a `vector3` between min and max per component.
 *
 * @see vec3_clamp_ptr
 *
 * @param v The vector.
 * @param min Minimum bounds.
 * @param max Maximum bounds.
 * @return The clamped vector.
 */
vector3 vec3_clamp(const vector3 v, const vector3 min, const vector3 max)
{
    vector3 res;
    vec3_clamp_ptr(&res, &v, &min, &max);
    return res;
}

/**
 * @brief Computes the Euclidean distance between two `vector3`.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return The distance between a and b.
 */
vm_float_t vec3_distance(const vector3 a, const vector3 b)
{
    return vec3_length(vec3_sub(a, b));
}

/**
 * @brief Computes the angle between two non-zero `vector3` in radians.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return The angle between the directions of a and b.
 */
vm_float_t vec3_angle(const vector3 a, const vector3 b)
{
    const vm_float_t dot = vec3_dot(a, b);
    const vm_float_t len_a = vec3_length(a);
    const vm_float_t len_b = vec3_length(b);
    return VECMAT_ACOS(dot / (len_a * len_b));
}

/**
 * @brief Computes the dot product of two `vector3`.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return The dot product (a.x * b.x + a.y * b.y + a.z * b.z).
 */
vm_float_t vec3_dot(const vector3 a, const vector3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

/**
 * @brief Computes the length (magnitude) of a `vector3`.
 *
 * @param v The vector.
 * @return The Euclidean length of v.
 */
vm_float_t vec3_length(const vector3 v)
{
    return VECMAT_SQRT(vec3_dot(v, v));
}

/**
 * @brief Scales the vector by a scalar.
 *
 * See `vec3_scale_ptr` instead.
 *
 * @param v Input vector.
 * @param s Scalar value.
 * @return The resulting vector3.
 */
vector3 vec3_scale(const vector3 v, const vm_float_t s)
{
    vector3 res;
    vec3_scale_ptr(&res, &v, s);
    return res;
}

/**
 * @brief Divides two vectors component-wise.
 *
 * See `vec3_div_ptr` instead.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector3.
 */
vector3 vec3_div(const vector3 a, const vector3 b)
{
    vector3 res;
    vec3_div_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Adds a scalar to each component.
 *
 * See `vec3_add_scalar_ptr` instead.
 *
 * @param v Input vector.
 * @param s Scalar value.
 * @return The resulting vector3.
 */
vector3 vec3_add_scalar(const vector3 v, const vm_float_t s)
{
    vector3 res;
    vec3_add_scalar_ptr(&res, &v, s);
    return res;
}

/**
 * @brief Subtracts a scalar from each component.
 *
 * See `vec3_sub_scalar_ptr` instead.
 *
 * @param v Input vector.
 * @param s Scalar value.
 * @return The resulting vector3.
 */
vector3 vec3_sub_scalar(const vector3 v, const vm_float_t s)
{
    vector3 res;
    vec3_sub_scalar_ptr(&res, &v, s);
    return res;
}

/**
 * @brief Clamps each component to the scalar range [min, max].
 *
 * See `vec3_clamp_scalar_ptr` instead.
 *
 * @param v Input vector.
 * @param min Lower bound.
 * @param max Upper bound.
 * @return The resulting vector3.
 */
vector3 vec3_clamp_scalar(const vector3 v, const vm_float_t min, const vm_float_t max)
{
    vector3 res;
    vec3_clamp_scalar_ptr(&res, &v, min, max);
    return res;
}

/**
 * @brief Clamps each component to the range [0, 1].
 *
 * See `vec3_saturate_ptr` instead.
 *
 * @param v Input vector.
 * @return The resulting vector3.
 */
vector3 vec3_saturate(const vector3 v)
{
    vector3 res;
    vec3_saturate_ptr(&res, &v);
    return res;
}

/**
 * @brief Returns the fractional part of each component.
 *
 * See `vec3_fract_ptr` instead.
 *
 * @param v Input vector.
 * @return The resulting vector3.
 */
vector3 vec3_fract(const vector3 v)
{
    vector3 res;
    vec3_fract_ptr(&res, &v);
    return res;
}

/**
 * @brief Projects a onto b.
 *
 * See `vec3_project_ptr` instead.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector3.
 */
vector3 vec3_project(const vector3 a, const vector3 b)
{
    vector3 res;
    vec3_project_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Removes the component of v along normal.
 *
 * See `vec3_slide_ptr` instead.
 *
 * @param v Input vector.
 * @param normal Surface normal.
 * @return The resulting vector3.
 */
vector3 vec3_slide(const vector3 v, const vector3 normal)
{
    vector3 res;
    vec3_slide_ptr(&res, &v, &normal);
    return res;
}

/**
 * @brief Returns the component of a orthogonal to b.
 *
 * See `vec3_reject_ptr` instead.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector3.
 */
vector3 vec3_reject(const vector3 a, const vector3 b)
{
    vector3 res;
    vec3_reject_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Rotates v around axis by angle radians.
 * @see vec3_rotate_axis_ptr
 *
 * @param v Input vector.
 * @param axis Rotation axis.
 * @param radians Angle in radians.
 * @return The resulting vector3.
 */
vector3 vec3_rotate_axis(const vector3 v, const vector3 axis, const vm_float_t radians)
{
    vector3 res;
    vec3_rotate_axis_ptr(&res, &v, &axis, radians);
    return res;
}

/**
 * @brief Rotates a vector around an arbitrary axis by a given angle in degrees.
 *
 * This is a convenience wrapper that converts @p degrees to radians and
 * delegates to @ref vec3_rotate_axis.
 *
 * @param v The vector to rotate.
 * @param axis The axis of rotation. The axis vector is assumed to be normalized.
 * @param degrees The rotation angle in degrees.
 *
 * @return The rotated vector.
 */
vector3 vec3_rotate_axis_deg(const vector3 v, const vector3 axis, const vm_float_t degrees)
{
    return vec3_rotate_axis(v, axis, deg_to_rad(degrees));
}

/**
 * @brief Returns a vector with every component set to s.
 *
 * See `vec3_splat_ptr` instead.
 *
 * @param s Scalar value.
 * @return The resulting vector3.
 */
vector3 vec3_splat(const vm_float_t s)
{
    return (vector3){.x = s, .y = s, .z = s};
}

/**
 * @brief Builds a vector3 from a vector2 and z.
 *
 * See `vec3_from_vec2_ptr` instead.
 *
 * @param v Input vector.
 * @param z Z component.
 * @return The resulting vector3.
 */
vector3 vec3_from_vec2(const vector2 v, const vm_float_t z)
{
    vector3 res;
    vec3_from_vec2_ptr(&res, &v, z);
    return res;
}

/**
 * @brief Moves current toward target by at most max_delta.
 *
 * See `vec3_move_toward_ptr` instead.
 *
 * @param current Current position.
 * @param target Target position.
 * @param max_delta Maximum distance to move.
 * @return The resulting vector3.
 */
vector3 vec3_move_toward(const vector3 current, const vector3 target, const vm_float_t max_delta)
{
    vector3 res;
    vec3_move_toward_ptr(&res, &current, &target, max_delta);
    return res;
}

/**
 * @brief Clamps the vector length to max_len.
 *
 * See `vec3_limit_length_ptr` instead.
 *
 * @param v Input vector.
 * @param max_len Maximum length.
 * @return The resulting vector3.
 */
vector3 vec3_limit_length(const vector3 v, const vm_float_t max_len)
{
    vector3 res;
    vec3_limit_length_ptr(&res, &v, max_len);
    return res;
}

/**
 * @brief Returns the x and y components as a 2D vector.
 *
 * See `vec3_xy_ptr` instead.
 *
 * @param v Input vector.
 * @return The resulting vector2.
 */
vector2 vec3_xy(const vector3 v)
{
    vector2 res;
    vec3_xy_ptr(&res, &v);
    return res;
}

/**
 * @brief Builds a tangent and bitangent orthonormal to n.
 *
 * @param n Unit normal.
 * @param t Output tangent.
 * @param b Output bitangent.
 */
void vec3_orthonormal_basis(const vector3 n, vector3 *t, vector3 *b)
{
    vec3_orthonormal_basis_ptr(&n, t, b);
}

/**
 * @brief Returns the squared Euclidean length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_float_t vec3_length_squared(const vector3 v)
{
    return vec3_dot(v, v);
}

/**
 * @brief Returns the Manhattan (L1) length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_float_t vec3_length_manhattan(const vector3 v)
{
    return VECMAT_FABS(v.x) + VECMAT_FABS(v.y) + VECMAT_FABS(v.z);
}

/**
 * @brief Returns the Chebyshev (L-inf) length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_float_t vec3_length_chebyshev(const vector3 v)
{
    return VECMAT_FMAX(VECMAT_FABS(v.x), fmaxf(VECMAT_FABS(v.y), VECMAT_FABS(v.z)));
}

/**
 * @brief Returns the squared Euclidean distance between a and b.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting scalar.
 */
vm_float_t vec3_distance_squared(const vector3 a, const vector3 b)
{
    const vm_float_t dx = a.x - b.x;
    const vm_float_t dy = a.y - b.y;
    const vm_float_t dz = a.z - b.z;
    return dx * dx + dy * dy + dz * dz;
}

/**
 * @brief Returns the signed angle from a to b around axis.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @param axis Rotation axis.
 * @return The resulting scalar.
 */
vm_float_t vec3_signed_angle(const vector3 a, const vector3 b, const vector3 axis)
{
    const vector3 c = vec3_cross(a, b);
    return VECMAT_ATAN2(vec3_dot(axis, c), vec3_dot(a, b));
}

/**
 * @brief Returns the smallest component.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_float_t vec3_min_component(const vector3 v)
{
    return VECMAT_FMIN(v.x, VECMAT_FMIN(v.y, v.z));
}

/**
 * @brief Returns the largest component.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_float_t vec3_max_component(const vector3 v)
{
    return VECMAT_FMAX(v.x, VECMAT_FMAX(v.y, v.z));
}

/**
 * @brief Returns the sum of all components.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_float_t vec3_sum(const vector3 v)
{
    return v.x + v.y + v.z;
}

/**
 * @brief Returns true if every component is zero.
 *
 * @param v Input vector.
 * @return True if every component is zero.
 */
bool vec3_is_zero(const vector3 v)
{
    return VECMAT_FABS(v.x) < VECMAT_EPSILON
        && VECMAT_FABS(v.y) < VECMAT_EPSILON
        && VECMAT_FABS(v.z) < VECMAT_EPSILON;
}

/**
 * @brief Returns true if the vector has unit length.
 *
 * @param v Input vector.
 * @return True if the vector has unit length.
 */
bool vec3_is_normalized(const vector3 v)
{
    return VECMAT_FABS(vec3_length_squared(v) - 1.0f) < VECMAT_EPSILON;
}

/**
 * @brief Returns true if a and b are within eps of each other.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @param eps Distance tolerance.
 * @return True if a and b are within eps.
 */
bool vec3_near(const vector3 a, const vector3 b, const vm_float_t eps)
{
    return VECMAT_FABS(a.x - b.x) < eps
        && VECMAT_FABS(a.y - b.y) < eps
        && VECMAT_FABS(a.z - b.z) < eps;
}
