// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

/**
 * @brief Returns a `vector2` with both components set to 0.0f.
 *
 * @return The zero vector2.
 */
vector2 vec2_zero(void)
{
    return (vector2){.x = 0.0f, .y = 0.0f};
}

/**
 * @brief Returns a `vector2` with both components set to 1.0f.
 *
 * @return The one vector2.
 */
vector2 vec2_one(void)
{
    return (vector2){.x = 1.0f, .y = 1.0f};
}

/**
 * @brief Returns a `vector2` along the x-axis (y = 0.0f).
 *
 * @param x The x component value.
 * @return The x-axis vector2.
 */
vector2 vec2_x_axis(const vm_float_t x)
{
    return (vector2){.x = x, .y = 0.0f};
}

/**
 * @brief Returns a `vector2` along the y-axis (x = 0.0f).
 *
 * @param y The y component value.
 * @return The y-axis vector2.
 */
vector2 vec2_y_axis(const vm_float_t y)
{
    return (vector2){.x = 0.0f, .y = y};
}

/**
 * @brief Returns a `vector2` representing x-axis scaling (y = 1.0f).
 *
 * @param x The x scale factor.
 * @return The x-scale vector2.
 */
vector2 vec2_x_scale(const vm_float_t x)
{
    return (vector2){.x = x, .y = 1.0f};
}

/**
 * @brief Returns a `vector2` representing y-axis scaling (x = 1.0f).
 *
 * @param y The y scale factor.
 * @return The y-scale vector2.
 */
vector2 vec2_y_scale(const vm_float_t y)
{
    return (vector2){.x = 1.0f, .y = y};
}

/**
 * @brief Scales a vector by a scalar component-wise.
 *
 * @see vec2_scale_ptr
 *
 * @param v The input vector.
 * @param s The scalar multiplier.
 * @return The scaled vector2.
 */
vector2 vec2_scale(const vector2 v, const vm_float_t s)
{
    vector2 res;
    vec2_scale_ptr(&res, &v, s);
    return res;
}

/**
 * @brief Adds two vectors component-wise.
 *
 * @see vec2_add_ptr
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The sum vector2.
 */
vector2 vec2_add(const vector2 a, const vector2 b)
{
    vector2 res;
    vec2_add_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Subtracts the second vector from the first component-wise.
 *
 * @see vec2_sub_ptr
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The difference vector2.
 */
vector2 vec2_sub(const vector2 a, const vector2 b)
{
    vector2 res;
    vec2_sub_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Multiplies a vector by a scalar component-wise.
 *
 * @see vec2_mul_scalar_ptr
 *
 * @param v The input vector.
 * @param s The scalar multiplier.
 * @return The scaled vector2.
 */
vector2 vec2_mul_scalar(const vector2 v, const vm_float_t s)
{
    vector2 res;
    vec2_mul_scalar_ptr(&res, &v, s);
    return res;
}

/**
 * @brief Divides a vector by a scalar component-wise.
 *
 * @see vec2_div_scalar_ptr
 *
 * @param v The input vector.
 * @param s The scalar divisor (non-zero).
 * @return The divided vector2.
 */
vector2 vec2_div_scalar(const vector2 v, const vm_float_t s)
{
    vector2 res;
    vec2_div_scalar_ptr(&res, &v, s);
    return res;
}

/**
 * @brief Multiplies two vectors component-wise (Hadamard product).
 *
 * @see vec2_mul_ptr
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The product vector2.
 */
vector2 vec2_mul(const vector2 a, const vector2 b)
{
    vector2 res;
    vec2_mul_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Negates the vector (multiplies each component by -1.0f).
 *
 * @see vec2_neg_ptr
 *
 * @param v The input vector.
 * @return The negated vector2.
 */
vector2 vec2_neg(const vector2 v)
{
    vector2 res;
    vec2_neg_ptr(&res, &v);
    return res;
}

/**
 * @brief Returns the absolute values of each component.
 *
 * @see vec2_abs_ptr
 *
 * @param v The input vector.
 * @return The absolute value vector2.
 */
vector2 vec2_abs(const vector2 v)
{
    vector2 res;
    vec2_abs_ptr(&res, &v);
    return res;
}

/**
 * @brief Computes the 2D cross-product as a vector.
 *
 * @see vec2_cross_ptr
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The cross-product vector2.
 */
vector2 vec2_cross(const vector2 a, const vector2 b)
{
    vector2 res;
    vec2_cross_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Normalizes the vector to unit length.
 *
 * @see vec2_normalize_ptr
 *
 * @param v The input vector.
 * @return The normalized vector2 (unchanged if zero length).
 */
vector2 vec2_normalize(const vector2 v)
{
    vector2 res;
    vec2_normalize_ptr(&res, &v);
    return res;
}

/**
 * @brief Returns the component-wise minimum of two vectors.
 *
 * @see vec2_min_ptr
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The minimum vector2.
 */
vector2 vec2_min(const vector2 a, const vector2 b)
{
    vector2 res;
    vec2_min_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Returns the component-wise maximum of two vectors.
 *
 * @see vec2_max_ptr
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The maximum vector2.
 */
vector2 vec2_max(const vector2 a, const vector2 b)
{
    vector2 res;
    vec2_max_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Returns the sign of each component (+1.0f, -1.0f, or 0.0f).
 *
 * @see vec2_sign_ptr
 *
 * @param v The input vector.
 * @return The sign vector2.
 */
vector2 vec2_sign(const vector2 v)
{
    vector2 res;
    vec2_sign_ptr(&res, &v);
    return res;
}

/**
 * @brief Applies floor to each component.
 *
 * @see vec2_floor_ptr
 *
 * @param v The input vector.
 * @return The floored vector2.
 */
vector2 vec2_floor(const vector2 v)
{
    vector2 res;
    vec2_floor_ptr(&res, &v);
    return res;
}

/**
 * @brief Applies ceil to each component.
 *
 * @see vec2_ceil_ptr
 *
 * @param v The input vector.
 * @return The ceiled vector2.
 */
vector2 vec2_ceil(const vector2 v)
{
    vector2 res;
    vec2_ceil_ptr(&res, &v);
    return res;
}

/**
 * @brief Applies round to each component.
 *
 * @see vec2_round_ptr
 *
 * @param v The input vector.
 * @return The rounded vector2.
 */
vector2 vec2_round(const vector2 v)
{
    vector2 res;
    vec2_round_ptr(&res, &v);
    return res;
}


/**
 * @brief Returns the perpendicular vector (90 degrees counterclockwise).
 *
 * @see vec2_perpendicular_ptr
 *
 * @param v The input vector.
 * @return The perpendicular vector2.
 */
vector2 vec2_perpendicular(const vector2 v)
{
    vector2 res;
    vec2_perpendicular_ptr(&res, &v);
    return res;
}

/**
 * @brief Reflects vector v across the normal, storing the result in res.
 *
 * @see vec2_reflect_ptr
 *
 * @param v The incident vector.
 * @param normal The unit normal vector.
 * @return The reflected vector2.
 */
vector2 vec2_reflect(const vector2 v, const vector2 normal)
{
    vector2 res;
    vec2_reflect_ptr(&res, &v, &normal);
    return res;
}

/**
 * @brief Projects the first vector onto the second.
 *
 * @see vec2_project_ptr
 *
 * @param a The vector to project.
 * @param b The projection direction vector.
 * @return The projected vector2.
 */
vector2 vec2_project(const vector2 a, const vector2 b)
{
    vector2 res;
    vec2_project_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Returns the tangent vector perpendicular to the input (90 degrees clockwise).
 *
 * @see vec2_tangent_ptr
 *
 * @param v The input vector.
 * @return The tangent vector2.
 */
vector2 vec2_tangent(const vector2 v)
{
    vector2 res;
    vec2_tangent_ptr(&res, &v);
    return res;
}

/**
 * @brief Rotates the input vector counterclockwise by the given angle (radians).
 * @see vec2_rotate_ptr
 *
 * @param v The input vector.
 * @param radians The rotation angle in radians.
 * @return The rotated vector2.
 */
vector2 vec2_rotate(const vector2 v, const vm_float_t radians)
{
    vector2 res;
    vec2_rotate_ptr(&res, &v, radians);
    return res;
}

/**
 * @brief Rotates a `vector2` by the given angle in degrees.
 *
 * @param v The vector2 to rotate.
 * @param degrees The rotation angle in degrees.
 *
 * @return The rotated vector2.
 */
vector2 vec2_rotate_deg(const vector2 v, const vm_float_t degrees)
{
    return vec2_rotate(v, deg_to_rad(degrees));
}

/**
 * @brief Slides the input vector tangent to the normal (removes normal component).
 *
 * @see vec2_slide_ptr
 *
 * @param v The input vector.
 * @param normal The unit normal vector.
 * @return The slid vector2.
 */
vector2 vec2_slide(const vector2 v, const vector2 normal)
{
    vector2 res;
    vec2_slide_ptr(&res, &v, &normal);
    return res;
}

/**
 * @brief Clamps vector v component-wise between min and max.
 *
 * @see vec2_clamp_ptr
 *
 * @param v The input vector.
 * @param min The minimum bounds vector.
 * @param max The maximum bounds vector.
 * @return The clamped vector2.
 */
vector2 vec2_clamp(const vector2 v, const vector2 min, const vector2 max)
{
    vector2 res;
    vec2_clamp_ptr(&res, &v, &min, &max);
    return res;
}

/**
 * @brief Computes the dot product of two vectors.
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The dot product scalar.
 */
vm_float_t vec2_dot(const vector2 a, const vector2 b)
{
    return a.x * b.x + a.y * b.y;
}

/**
 * @brief Computes the length (magnitude) of the vector.
 *
 * @param v The input vector.
 * @return The length scalar.
 */
vm_float_t vec2_length(const vector2 v)
{
    return VECMAT_SQRT(vec2_dot(v, v));
}

/**
 * @brief Computes the aspect ratio of the vector (x / y).
 *
 * Returns 0.0f if y == 0.0f.
 *
 * @param v The input vector.
 * @return The aspect ratio scalar.
 */
vm_float_t vec2_aspect_ratio(const vector2 v)
{
    if (v.y == 0.0f) return 0.0f;
    return v.x / v.y;
}

/**
 * @brief Computes the Euclidean distance between two vectors (treated as points).
 *
 * @param a The first point.
 * @param b The second point.
 * @return The distance scalar.
 */
vm_float_t vec2_distance(const vector2 a, const vector2 b)
{
    const vm_float_t dx = a.x - b.x;
    const vm_float_t dy = a.y - b.y;
    return VECMAT_SQRT(dx * dx + dy * dy);
}

/**
 * @brief Computes the angle between two vectors (in radians, range [0, PI]).
 *
 * Returns 0.0f if either vector has zero length.
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The angle scalar.
 */
vm_float_t vec2_angle(const vector2 a, const vector2 b)
{
    const vm_float_t dot = vec2_dot(a, b);
    const vm_float_t len_a = vec2_length(a);
    const vm_float_t len_b = vec2_length(b);
    if (len_a == 0.0f || len_b == 0.0f) return 0.0f;
    return VECMAT_ACOS(dot / (len_a * len_b));
}

/**
 * @brief Linearly interpolates from a to b by t.
 *
 * @see vec2_lerp_ptr
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @param t Interpolation factor.
 * @return The resulting vector2.
 */
vector2 vec2_lerp(const vector2 a, const vector2 b, const vm_float_t t)
{
    vector2 res;
    vec2_lerp_ptr(&res, &a, &b, t);
    return res;
}

/**
 * @brief Divides two vectors component-wise.
 *
 * @see vec2_div_ptr
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector2.
 */
vector2 vec2_div(const vector2 a, const vector2 b)
{
    vector2 res;
    vec2_div_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Adds a scalar to each component.
 *
 * @see vec2_add_scalar_ptr
 *
 * @param v Input vector.
 * @param s Scalar value.
 * @return The resulting vector2.
 */
vector2 vec2_add_scalar(const vector2 v, const vm_float_t s)
{
    vector2 res;
    vec2_add_scalar_ptr(&res, &v, s);
    return res;
}

/**
 * @brief Subtracts a scalar from each component.
 *
 * @see vec2_sub_scalar_ptr
 *
 * @param v Input vector.
 * @param s Scalar value.
 * @return The resulting vector2.
 */
vector2 vec2_sub_scalar(const vector2 v, const vm_float_t s)
{
    vector2 res;
    vec2_sub_scalar_ptr(&res, &v, s);
    return res;
}

/**
 * @brief Clamps each component to the scalar range [min, max].
 *
 * @see vec2_clamp_scalar_ptr
 *
 * @param v Input vector.
 * @param min Lower bound.
 * @param max Upper bound.
 * @return The resulting vector2.
 */
vector2 vec2_clamp_scalar(const vector2 v, const vm_float_t min, const vm_float_t max)
{
    vector2 res;
    vec2_clamp_scalar_ptr(&res, &v, min, max);
    return res;
}

/**
 * @brief Clamps each component to the range [0, 1].
 *
 * @see vec2_saturate_ptr
 *
 * @param v Input vector.
 * @return The resulting vector2.
 */
vector2 vec2_saturate(const vector2 v)
{
    vector2 res;
    vec2_saturate_ptr(&res, &v);
    return res;
}

/**
 * @brief Returns the fractional part of each component.
 *
 * @see vec2_fract_ptr
 *
 * @param v Input vector.
 * @return The resulting vector2.
 */
vector2 vec2_fract(const vector2 v)
{
    vector2 res;
    vec2_fract_ptr(&res, &v);
    return res;
}

/**
 * @brief Computes the refraction of incident across normal with ratio eta.
 *
 * @see vec2_refract_ptr
 *
 * @param incident Incident vector.
 * @param normal Surface normal.
 * @param eta Ratio of indices of refraction.
 * @return The resulting vector2.
 */
vector2 vec2_refract(const vector2 incident, const vector2 normal, const vm_float_t eta)
{
    vector2 res;
    vec2_refract_ptr(&res, &incident, &normal, eta);
    return res;
}

/**
 * @brief Returns the component of a orthogonal to b.
 *
 * @see vec2_reject_ptr
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector2.
 */
vector2 vec2_reject(const vector2 a, const vector2 b)
{
    vector2 res;
    vec2_reject_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Returns a vector with every component set to s.
 *
 * @see vec2_splat_ptr
 *
 * @param s Scalar value.
 * @return The resulting vector2.
 */
vector2 vec2_splat(const vm_float_t s)
{
    return (vector2){.x = s, .y = s};
}

/**
 * @brief Returns the unit vector at the given angle in radians.
 * @see vec2_from_angle_ptr
 *
 * @param radians Angle in radians.
 * @return The resulting vector2.
 */
vector2 vec2_from_angle(const vm_float_t radians)
{
    return (vector2){.x = VECMAT_COS(radians), .y = VECMAT_SIN(radians)};
}

/**
 * @brief Creates a `vector2` from an angle given in degrees.
 *
 * The resulting vector has unit length and points in the direction
 * specified by the angle. The angle is converted to radians internally
 * before computing the cosine and sine.
 *
 * @param degrees The angle in degrees.
 * @return A unit vector2 representing the given angle.
 */
vector2 vec2_from_angle_deg(const vm_float_t degrees)
{
    return vec2_from_angle(deg_to_rad(degrees));
}

/**
 * @brief Rotates v around pivot by angle radians.
 * @see vec2_rotate_around_ptr
 *
 * @param v Input vector.
 * @param pivot Rotation pivot.
 * @param radians Angle in radians.
 * @return The resulting vector2.
 */
vector2 vec2_rotate_around(const vector2 v, const vector2 pivot, const vm_float_t radians)
{
    vector2 res;
    vec2_rotate_around_ptr(&res, &v, &pivot, radians);
    return res;
}

/**
 * @brief Rotates a vector2 around a pivot point by the given angle in degrees.
 *
 * @param v The vector2 to rotate.
 * @param pivot The vector2 to rotate around.
 * @param degrees The rotation angle in degrees.
 * @return The rotated vector2.
 */
vector2 vec2_rotate_around_deg(const vector2 v, const vector2 pivot, const vm_float_t degrees)
{
    return vec2_rotate_around(v, pivot, deg_to_rad(degrees));
}

/**
 * @brief Moves current toward target by at most max_delta.
 *
 * @see vec2_move_toward_ptr
 *
 * @param current Current position.
 * @param target Target position.
 * @param max_delta Maximum distance to move.
 * @return The resulting vector2.
 */
vector2 vec2_move_toward(const vector2 current, const vector2 target, const vm_float_t max_delta)
{
    vector2 res;
    vec2_move_toward_ptr(&res, &current, &target, max_delta);
    return res;
}

/**
 * @brief Clamps the vector length to max_len.
 *
 * @see vec2_limit_length_ptr
 *
 * @param v Input vector.
 * @param max_len Maximum length.
 * @return The resulting vector2.
 */
vector2 vec2_limit_length(const vector2 v, const vm_float_t max_len)
{
    vector2 res;
    vec2_limit_length_ptr(&res, &v, max_len);
    return res;
}

/**
 * @brief Converts a vector2 to a vector3 using z.
 *
 * @see vec2_to_vec3_ptr
 *
 * @param v Input vector.
 * @param z Z component.
 * @return The resulting vector3.
 */
vector3 vec2_to_vec3(const vector2 v, const vm_float_t z)
{
    vector3 res;
    vec2_to_vec3_ptr(&res, &v, z);
    return res;
}

/**
 * @brief Returns the squared Euclidean length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_float_t vec2_length_squared(const vector2 v)
{
    return vec2_dot(v, v);
}

/**
 * @brief Returns the Manhattan (L1) length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_float_t vec2_length_manhattan(const vector2 v)
{
    return VECMAT_FABS(v.x) + VECMAT_FABS(v.y);
}

/**
 * @brief Returns the Chebyshev (L-inf) length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_float_t vec2_length_chebyshev(const vector2 v)
{
    return VECMAT_FMAX(VECMAT_FABS(v.x), VECMAT_FABS(v.y));
}

/**
 * @brief Returns the squared Euclidean distance between a and b.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting scalar.
 */
vm_float_t vec2_distance_squared(const vector2 a, const vector2 b)
{
    const vm_float_t dx = a.x - b.x;
    const vm_float_t dy = a.y - b.y;
    return dx * dx + dy * dy;
}

/**
 * @brief Returns the 2D cross product as a scalar (a.x*b.y - a.y*b.x).
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting scalar.
 */
vm_float_t vec2_cross_scalar(const vector2 a, const vector2 b)
{
    return a.x * b.y - a.y * b.x;
}

/**
 * @brief Returns the heading angle of the vector in radians.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_float_t vec2_heading(const vector2 v)
{
    return VECMAT_ATAN2(v.y, v.x);
}

/**
 * @brief Returns the heading angle of the vector in degrees.
 *
 * Computes the angle between the positive x-axis and the vector
 * using atan2, then converts the result from radians to degrees.
 *
 * @param v The vector2 whose heading is to be calculated.
 * @return The heading angle of the vector in degrees.
 */
vm_float_t vec2_heading_deg(const vector2 v)
{
    return rad_to_deg(vec2_heading(v));
}

/**
 * @brief Returns the smallest component.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_float_t vec2_min_component(const vector2 v)
{
    return VECMAT_FMIN(v.x, v.y);
}

/**
 * @brief Returns the largest component.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_float_t vec2_max_component(const vector2 v)
{
    return VECMAT_FMAX(v.x, v.y);
}

/**
 * @brief Returns the sum of all components.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
vm_float_t vec2_sum(const vector2 v)
{
    return v.x + v.y;
}

/**
 * @brief Returns true if every component is zero.
 *
 * @param v Input vector.
 * @return True if every component is zero.
 */
bool vec2_is_zero(const vector2 v)
{
    return VECMAT_FABS(v.x) < VECMAT_EPSILON && VECMAT_FABS(v.y) < VECMAT_EPSILON;
}

/**
 * @brief Returns true if the vector has unit length.
 *
 * @param v Input vector.
 * @return True if the vector has unit length.
 */
bool vec2_is_normalized(const vector2 v)
{
    return VECMAT_FABS(vec2_length_squared(v) - 1.0f) < VECMAT_EPSILON;
}

/**
 * @brief Returns true if a and b are within eps of each other.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @param eps Distance tolerance.
 * @return True if a and b are within eps.
 */
bool vec2_near(const vector2 a, const vector2 b, const vm_float_t eps)
{
    return VECMAT_FABS(a.x - b.x) < eps && VECMAT_FABS(a.y - b.y) < eps;
}
