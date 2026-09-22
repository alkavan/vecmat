// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_VEC_H
#define VECMAT_VEC_H

/**
 * @file
 * @brief Vector algebra (value and `_ptr`), conversions, constructors.
 * @ingroup vecmat_core
 */

#include "vecmat/types.h"

/* ---- vector2 ------------------------------------------------------------- */

/**
 * @brief Adds two vectors component-wise.
 *
 * @see vec2_add_ptr
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The sum vector2.
 */
VEC_API vector2 vec2_add(vector2 a, vector2 b);

/**
 * @brief Subtracts the second vector from the first component-wise.
 *
 * @see vec2_sub_ptr
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The difference vector2.
 */
VEC_API vector2 vec2_sub(vector2 a, vector2 b);

/**
 * @brief Multiplies a vector by a scalar component-wise.
 *
 * @see vec2_mul_scalar_ptr
 *
 * @param v The input vector.
 * @param s The scalar multiplier.
 * @return The scaled vector2.
 */
VEC_API vector2 vec2_mul_scalar(vector2 v, vm_float_t s);

/**
 * @brief Divides a vector by a scalar component-wise.
 *
 * @see vec2_div_scalar_ptr
 *
 * @param v The input vector.
 * @param s The scalar divisor (non-zero).
 * @return The divided vector2.
 */
VEC_API vector2 vec2_div_scalar(vector2 v, vm_float_t s);

/**
 * @brief Multiplies two vectors component-wise (Hadamard product).
 *
 * @see vec2_mul_ptr
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The product vector2.
 */
VEC_API vector2 vec2_mul(vector2 a, vector2 b);

/**
 * @brief Negates the vector (multiplies each component by -1.0f).
 *
 * @see vec2_neg_ptr
 *
 * @param v The input vector.
 * @return The negated vector2.
 */
VEC_API vector2 vec2_neg(vector2 v);
/**
 * @brief Returns the absolute values of each component.
 *
 * @see vec2_abs_ptr
 *
 * @param v The input vector.
 * @return The absolute value vector2.
 */
VEC_API vector2 vec2_abs(vector2 v);

/**
 * @brief Returns a `vector2` with both components set to 0.0f.
 *
 * @return The zero vector2.
 */
VEC_API vector2 vec2_zero(void);
/**
 * @brief Returns a `vector2` with both components set to 1.0f.
 *
 * @return The one vector2.
 */
VEC_API vector2 vec2_one(void);

/**
 * @brief Returns a `vector2` along the x-axis (y = 0.0f).
 *
 * @param x The x component value.
 * @return The x-axis vector2.
 */
VEC_API vector2 vec2_x_axis(vm_float_t x);

/**
 * @brief Returns a `vector2` along the y-axis (x = 0.0f).
 *
 * @param y The y component value.
 * @return The y-axis vector2.
 */
VEC_API vector2 vec2_y_axis(vm_float_t y);

/**
 * @brief Returns a `vector2` representing x-axis scaling (y = 1.0f).
 *
 * @param x The x scale factor.
 * @return The x-scale vector2.
 */
VEC_API vector2 vec2_x_scale(vm_float_t x);

/**
 * @brief Returns a `vector2` representing y-axis scaling (x = 1.0f).
 *
 * @param y The y scale factor.
 * @return The y-scale vector2.
 */
VEC_API vector2 vec2_y_scale(vm_float_t y);

/**
 * @brief Scales a vector by a scalar component-wise.
 *
 * @see vec2_scale_ptr
 *
 * @param v The input vector.
 * @param s The scalar multiplier.
 * @return The scaled vector2.
 */
VEC_API vector2 vec2_scale(vector2 v, vm_float_t s);

/**
 * @brief Computes the 2D cross-product as a vector.
 *
 * @see vec2_cross_ptr
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The cross-product vector2.
 */
VEC_API vector2 vec2_cross(vector2 a, vector2 b);

/**
 * @brief Normalizes the vector to unit length.
 *
 * @see vec2_normalize_ptr
 *
 * @param v The input vector.
 * @return The normalized vector2 (unchanged if zero length).
 */
VEC_API vector2 vec2_normalize(vector2 v);

/**
 * @brief Returns the component-wise minimum of two vectors.
 *
 * @see vec2_min_ptr
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The minimum vector2.
 */
VEC_API vector2 vec2_min(vector2 a, vector2 b);

/**
 * @brief Returns the component-wise maximum of two vectors.
 *
 * @see vec2_max_ptr
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The maximum vector2.
 */
VEC_API vector2 vec2_max(vector2 a, vector2 b);

/**
 * @brief Returns the sign of each component (+1.0f, -1.0f, or 0.0f).
 *
 * @see vec2_sign_ptr
 *
 * @param v The input vector.
 * @return The sign vector2.
 */
VEC_API vector2 vec2_sign(vector2 v);

/**
 * @brief Applies floor to each component.
 *
 * @see vec2_floor_ptr
 *
 * @param v The input vector.
 * @return The floored vector2.
 */
VEC_API vector2 vec2_floor(vector2 v);

/**
 * @brief Applies ceil to each component.
 *
 * @see vec2_ceil_ptr
 *
 * @param v The input vector.
 * @return The ceiled vector2.
 */
VEC_API vector2 vec2_ceil(vector2 v);

/**
 * @brief Applies round to each component.
 *
 * @see vec2_round_ptr
 *
 * @param v The input vector.
 * @return The rounded vector2.
 */
VEC_API vector2 vec2_round(vector2 v);

/**
 * @brief Returns the perpendicular vector (90 degrees counterclockwise).
 *
 * @see vec2_perpendicular_ptr
 *
 * @param v The input vector.
 * @return The perpendicular vector2.
 */
VEC_API vector2 vec2_perpendicular(vector2 v);

/**
 * @brief Reflects vector v across the normal, storing the result in res.
 *
 * @see vec2_reflect_ptr
 *
 * @param v The incident vector.
 * @param normal The unit normal vector.
 * @return The reflected vector2.
 */
VEC_API vector2 vec2_reflect(vector2 v, vector2 normal);

/**
 * @brief Projects the first vector onto the second.
 *
 * @see vec2_project_ptr
 *
 * @param a The vector to project.
 * @param b The projection direction vector.
 * @return The projected vector2.
 */
VEC_API vector2 vec2_project(vector2 a, vector2 b);

/**
 * @brief Returns the tangent vector perpendicular to the input (90 degrees clockwise).
 *
 * @see vec2_tangent_ptr
 *
 * @param v The input vector.
 * @return The tangent vector2.
 */
VEC_API vector2 vec2_tangent(vector2 v);

/**
 * @brief Rotates the input vector counterclockwise by the given angle (radians).
 *
 * @see vec2_rotate_ptr
 *
 * @param v The input vector.
 * @param radians The rotation angle in radians.
 * @return The rotated vector2.
 */
VEC_API vector2 vec2_rotate(vector2 v, vm_float_t radians);

/**
 * @brief Rotates a `vector2` by the given angle in degrees.
 *
 * @param v The vector2 to rotate.
 * @param degrees The rotation angle in degrees.
 *
 * @return The rotated vector2.
 */
VEC_API vector2 vec2_rotate_deg(vector2 v, vm_float_t degrees);

/**
 * @brief Slides the input vector tangent to the normal (removes normal component).
 *
 * @see vec2_slide_ptr
 *
 * @param v The input vector.
 * @param normal The unit normal vector.
 * @return The slid vector2.
 */
VEC_API vector2 vec2_slide(vector2 v, vector2 normal);

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
VEC_API vector2 vec2_clamp(vector2 v, vector2 min, vector2 max);

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
VEC_API vector2 vec2_lerp(vector2 a, vector2 b, vm_float_t t);

/**
 * @brief Divides two vectors component-wise.
 *
 * @see vec2_div_ptr
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector2.
 */
VEC_API vector2 vec2_div(vector2 a, vector2 b);

/**
 * @brief Adds a scalar to each component.
 *
 * @see vec2_add_scalar_ptr
 *
 * @param v Input vector.
 * @param s Scalar value.
 * @return The resulting vector2.
 */
VEC_API vector2 vec2_add_scalar(vector2 v, vm_float_t s);

/**
 * @brief Subtracts a scalar from each component.
 *
 * @see vec2_sub_scalar_ptr
 *
 * @param v Input vector.
 * @param s Scalar value.
 * @return The resulting vector2.
 */
VEC_API vector2 vec2_sub_scalar(vector2 v, vm_float_t s);

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
VEC_API vector2 vec2_clamp_scalar(vector2 v, vm_float_t min, vm_float_t max);

/**
 * @brief Clamps each component to the range [0, 1].
 *
 * @see vec2_saturate_ptr
 *
 * @param v Input vector.
 * @return The resulting vector2.
 */
VEC_API vector2 vec2_saturate(vector2 v);

/**
 * @brief Returns the fractional part of each component.
 *
 * @see vec2_fract_ptr
 *
 * @param v Input vector.
 * @return The resulting vector2.
 */
VEC_API vector2 vec2_fract(vector2 v);

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
VEC_API vector2 vec2_refract(vector2 incident, vector2 normal, vm_float_t eta);

/**
 * @brief Returns the component of a orthogonal to b.
 *
 * @see vec2_reject_ptr
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector2.
 */
VEC_API vector2 vec2_reject(vector2 a, vector2 b);

/**
 * @brief Returns a vector with every component set to s.
 *
 * @see vec2_splat_ptr
 *
 * @param s Scalar value.
 * @return The resulting vector2.
 */
VEC_API vector2 vec2_splat(vm_float_t s);

/**
 * @brief Returns the unit vector at the given angle in radians.
 *
 * @see vec2_from_angle_ptr
 *
 * @param radians Angle in radians.
 * @return The resulting vector2.
 */
VEC_API vector2 vec2_from_angle(vm_float_t radians);

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
VEC_API vector2 vec2_from_angle_deg(vm_float_t degrees);

/**
 * @brief Rotates v around pivot by angle radians.
 *
 * @see vec2_rotate_around_ptr
 *
 * @param v Input vector.
 * @param pivot Rotation pivot.
 * @param radians Angle in radians.
 * @return The resulting vector2.
 */
VEC_API vector2 vec2_rotate_around(vector2 v, vector2 pivot, vm_float_t radians);

/**
 * @brief Rotates a vector2 around a pivot point by the given angle in degrees.
 *
 * @param v The vector2 to rotate.
 * @param pivot The vector2 to rotate around.
 * @param degrees The rotation angle in degrees.
 * @return The rotated vector2.
 */
VEC_API vector2 vec2_rotate_around_deg(vector2 v, vector2 pivot, vm_float_t degrees);

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
VEC_API vector2 vec2_move_toward(vector2 current, vector2 target, vm_float_t max_delta);

/**
 * @brief Clamps the vector length to max_len.
 *
 * @see vec2_limit_length_ptr
 *
 * @param v Input vector.
 * @param max_len Maximum length.
 * @return The resulting vector2.
 */
VEC_API vector2 vec2_limit_length(vector2 v, vm_float_t max_len);

/**
 * @brief Converts a vector2 to a vector3 using z.
 *
 * @see vec2_to_vec3_ptr
 *
 * @param v Input vector.
 * @param z Z component.
 * @return The resulting vector3.
 */
VEC_API vector3 vec2_to_vec3(vector2 v, vm_float_t z);

/**
 * @brief Computes the dot product of two vectors.
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The dot product scalar.
 */
VEC_API vm_float_t vec2_dot(vector2 a, vector2 b);

/**
 * @brief Computes the length (magnitude) of the vector.
 *
 * @param v The input vector.
 * @return The length scalar.
 */
VEC_API vm_float_t vec2_length(vector2 v);

/**
 * @brief Returns the squared Euclidean length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_float_t vec2_length_squared(vector2 v);

/**
 * @brief Returns the Manhattan (L1) length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_float_t vec2_length_manhattan(vector2 v);

/**
 * @brief Returns the Chebyshev (L-inf) length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_float_t vec2_length_chebyshev(vector2 v);

/**
 * @brief Computes the aspect ratio of the vector (x / y).
 *
 * Returns 0.0f if y == 0.0f.
 *
 * @param v The input vector.
 * @return The aspect ratio scalar.
 */
VEC_API vm_float_t vec2_aspect_ratio(vector2 v);

/**
 * @brief Computes the Euclidean distance between two vectors (treated as points).
 *
 * @param a The first point.
 * @param b The second point.
 * @return The distance scalar.
 */
VEC_API vm_float_t vec2_distance(vector2 a, vector2 b);

/**
 * @brief Returns the squared Euclidean distance between a and b.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting scalar.
 */
VEC_API vm_float_t vec2_distance_squared(vector2 a, vector2 b);

/**
 * @brief Computes the angle between two vectors (in radians, range [0, PI]).
 *
 * Returns 0.0f if either vector has zero length.
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The angle scalar.
 */
VEC_API vm_float_t vec2_angle(vector2 a, vector2 b);

/**
 * @brief Returns the 2D cross product as a scalar (a.x*b.y - a.y*b.x).
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting scalar.
 */
VEC_API vm_float_t vec2_cross_scalar(vector2 a, vector2 b);

/**
 * @brief Returns the heading angle of the vector in radians.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_float_t vec2_heading(vector2 v);

/**
 * @brief Returns the heading angle of the vector in degrees.
 *
 * Computes the angle between the positive x-axis and the vector
 * using atan2, then converts the result from radians to degrees.
 *
 * @param v The vector2 whose heading is to be calculated.
 * @return The heading angle of the vector in degrees.
 */
VEC_API vm_float_t vec2_heading_deg(vector2 v);

/**
 * @brief Returns the smallest component.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_float_t vec2_min_component(vector2 v);

/**
 * @brief Returns the largest component.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_float_t vec2_max_component(vector2 v);

/**
 * @brief Returns the sum of all components.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_float_t vec2_sum(vector2 v);

/**
 * @brief Returns true if every component is zero.
 *
 * @param v Input vector.
 * @return True if every component is zero.
 */
VEC_API bool vec2_is_zero(vector2 v);

/**
 * @brief Returns true if the vector has unit length.
 *
 * @param v Input vector.
 * @return True if the vector has unit length.
 */
VEC_API bool vec2_is_normalized(vector2 v);

/**
 * @brief Returns true if a and b are within eps of each other.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @param eps Distance tolerance.
 * @return True if a and b are within eps.
 */
VEC_API bool vec2_near(vector2 a, vector2 b, vm_float_t eps);

/* ---- vector3 ------------------------------------------------------------- */

/**
 * @brief Component-wise addition of two vectors.
 *
 * @see vec3_add_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Result vector.
 */
VEC_API vector3 vec3_add(vector3 a, vector3 b);

/**
 * @brief Component-wise subtraction of two vectors.
 *
 * @see vec3_sub_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Result vector.
 */
VEC_API vector3 vec3_sub(vector3 a, vector3 b);

/**
 * @brief Component-wise multiplication of vector by scalar.
 *
 * @see vec3_mul_scalar_ptr
 *
 * @param v The vector.
 * @param s The scalar.
 * @return Result vector.
 */
VEC_API vector3 vec3_mul_scalar(vector3 v, vm_float_t s);

/**
 * @brief Component-wise division of vector by scalar.
 *
 * @see vec3_div_scalar_ptr
 *
 * @param v The vector.
 * @param s The scalar (non-zero).
 * @return Result vector.
 */
VEC_API vector3 vec3_div_scalar(vector3 v, vm_float_t s);

/**
 * @brief Component-wise multiplication of two vectors.
 *
 * @see vec3_mul_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Result vector.
 */
VEC_API vector3 vec3_mul(vector3 a, vector3 b);

/**
 * @brief Negation of a vector.
 *
 * @see vec3_neg_ptr
 *
 * @param v The vector.
 * @return The negated vector.
 */
VEC_API vector3 vec3_neg(vector3 v);

/**
 * @brief Computes the absolute value per component of a vector3.
 *
 * @see vec3_abs_ptr
 *
 * @param v The vector.
 * @return The absolute vector3.
 */
VEC_API vector3 vec3_abs(vector3 v);

/**
 * @brief Returns a zero-initialized `vector3`.
 *
 * @return A vector3 with all components set to 0.0f.
 */
VEC_API vector3 vec3_zero(void);

/**
 * @brief Returns a `vector3` with all components set to 1.0f.
 *
 * @return A vector3 with all components set to 1.0f.
 */
VEC_API vector3 vec3_one(void);

/**
 * @brief Returns a `vector3` along the x-axis.
 *
 * @param x The x component value.
 * @return A vector3 with (x, 0.0f, 0.0f).
 */
VEC_API vector3 vec3_x_axis(vm_float_t x);

/**
 * @brief Returns a `vector3` along the y-axis.
 *
 * @param y The y component value.
 * @return A vector3 with (0.0f, y, 0.0f).
 */
VEC_API vector3 vec3_y_axis(vm_float_t y);

/**
 * @brief Returns a `vector3` along the z-axis.
 *
 * @param z The z component value.
 * @return A vector3 with (0.0f, 0.0f, z).
 */
VEC_API vector3 vec3_z_axis(vm_float_t z);

/**
 * @brief Returns a `vector3` for scaling along the x-axis.
 *
 * @param x The x scale factor.
 * @return A vector3 with (x, 1.0f, 1.0f).
 */
VEC_API vector3 vec3_x_scale(vm_float_t x);

/**
 * @brief Returns a `vector3` for scaling along the y-axis.
 *
 * @param y The y scale factor.
 * @return A vector3 with (1.0f, y, 1.0f).
 */
VEC_API vector3 vec3_y_scale(vm_float_t y);

/**
 * @brief Returns a `vector3` for scaling along the z-axis.
 *
 * @param z The z scale factor.
 * @return A vector3 with (1.0f, 1.0f, z).
 */
VEC_API vector3 vec3_z_scale(vm_float_t z);

/**
 * @brief Computes the cross-product of two vector3.
 *
 * @see vec3_cross_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return The cross-product (a × b).
 */
VEC_API vector3 vec3_cross(vector3 a, vector3 b);

/**
 * @brief Normalizes a vector3 to unit length.
 *
 * @see vec3_normalize_ptr
 *
 * @param v The vector (non-zero).
 * @return The normalized vector.
 */
VEC_API vector3 vec3_normalize(vector3 v);

/**
 * @brief Computes the component-wise minimum of two vector3.
 *
 * @see vec3_min_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Per-component min(a, b).
 */
VEC_API vector3 vec3_min(vector3 a, vector3 b);

/**
 * @brief Computes the component-wise maximum of two vector3.
 *
 * @see vec3_max_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Per-component max(a, b).
 */
VEC_API vector3 vec3_max(vector3 a, vector3 b);

/**
 * @brief Computes the sign per component of a vector3 (-1, 0, or 1).
 *
 * @see vec3_sign_ptr
 *
 * @param v The vector.
 * @return The sign vector.
 */
VEC_API vector3 vec3_sign(vector3 v);

/**
 * @brief Applies the floor per component to a vector3.
 *
 * @see vec3_floor_ptr
 *
 * @param v The vector.
 * @return The floored vector.
 */
VEC_API vector3 vec3_floor(vector3 v);

/**
 * @brief Applies ceil per component to a vector3.
 *
 * @see vec3_ceil_ptr
 *
 * @param v The vector.
 * @return The ceiling vector.
 */
VEC_API vector3 vec3_ceil(vector3 v);

/**
 * @brief Applies round per component to a vector3.
 *
 * @see vec3_round_ptr
 *
 * @param v The vector.
 * @return The rounded vector.
 */
VEC_API vector3 vec3_round(vector3 v);

/**
 * @brief Reflects an incident vector over normal.
 *
 * @see vec3_reflect_ptr
 *
 * @param incident The incident vector.
 * @param normal The surface normal.
 * @return The reflected vector.
 */
VEC_API vector3 vec3_reflect(vector3 incident, vector3 normal);

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
VEC_API vector3 vec3_refract(vector3 incident, vector3 normal, vm_float_t eta);

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
VEC_API vector3 vec3_lerp(vector3 a, vector3 b, vm_float_t t);

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
VEC_API vector3 vec3_clamp(vector3 v, vector3 min, vector3 max);

/**
 * @brief Scales the vector by a scalar.
 *
 * @see vec3_scale_ptr
 *
 * @param v Input vector.
 * @param s Scalar value.
 * @return The resulting vector3.
 */
VEC_API vector3 vec3_scale(vector3 v, vm_float_t s);

/**
 * @brief Divides two vectors component-wise.
 *
 * @see vec3_div_ptr
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector3.
 */
VEC_API vector3 vec3_div(vector3 a, vector3 b);

/**
 * @brief Adds a scalar to each component.
 *
 * @see vec3_add_scalar_ptr
 *
 * @param v Input vector.
 * @param s Scalar value.
 * @return The resulting vector3.
 */
VEC_API vector3 vec3_add_scalar(vector3 v, vm_float_t s);

/**
 * @brief Subtracts a scalar from each component.
 *
 * @see vec3_sub_scalar_ptr
 *
 * @param v Input vector.
 * @param s Scalar value.
 * @return The resulting vector3.
 */
VEC_API vector3 vec3_sub_scalar(vector3 v, vm_float_t s);

/**
 * @brief Clamps each component to the scalar range [min, max].
 *
 * @see vec3_clamp_scalar_ptr
 *
 * @param v Input vector.
 * @param min Lower bound.
 * @param max Upper bound.
 * @return The resulting vector3.
 */
VEC_API vector3 vec3_clamp_scalar(vector3 v, vm_float_t min, vm_float_t max);

/**
 * @brief Clamps each component to the range [0, 1].
 *
 * @see vec3_saturate_ptr
 *
 * @param v Input vector.
 * @return The resulting vector3.
 */
VEC_API vector3 vec3_saturate(vector3 v);

/**
 * @brief Returns the fractional part of each component.
 *
 * @see vec3_fract_ptr
 *
 * @param v Input vector.
 * @return The resulting vector3.
 */
VEC_API vector3 vec3_fract(vector3 v);

/**
 * @brief Projects a onto b.
 *
 * @see vec3_project_ptr
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector3.
 */
VEC_API vector3 vec3_project(vector3 a, vector3 b);

/**
 * @brief Removes the component of v along normal.
 *
 * @see vec3_slide_ptr
 *
 * @param v Input vector.
 * @param normal Surface normal.
 * @return The resulting vector3.
 */
VEC_API vector3 vec3_slide(vector3 v, vector3 normal);

/**
 * @brief Returns the component of a orthogonal to b.
 *
 * @see vec3_reject_ptr
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector3.
 */
VEC_API vector3 vec3_reject(vector3 a, vector3 b);

/**
 * @brief Rotates v around axis by angle radians.
 *
 * @see vec3_rotate_axis_ptr
 *
 * @param v Input vector.
 * @param axis Rotation axis.
 * @param radians Angle in radians.
 * @return The resulting vector3.
 */
VEC_API vector3 vec3_rotate_axis(vector3 v, vector3 axis, vm_float_t radians);

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
VEC_API vector3 vec3_rotate_axis_deg(vector3 v, vector3 axis, vm_float_t degrees);

/**
 * @brief Returns a vector with every component set to s.
 *
 * @see vec3_splat_ptr
 *
 * @param s Scalar value.
 * @return The resulting vector3.
 */
VEC_API vector3 vec3_splat(vm_float_t s);

/**
 * @brief Builds a vector3 from a vector2 and z.
 *
 * @see vec3_from_vec2_ptr
 *
 * @param v Input vector.
 * @param z Z component.
 * @return The resulting vector3.
 */
VEC_API vector3 vec3_from_vec2(vector2 v, vm_float_t z);

/**
 * @brief Moves current toward target by at most max_delta.
 *
 * @see vec3_move_toward_ptr
 *
 * @param current Current position.
 * @param target Target position.
 * @param max_delta Maximum distance to move.
 * @return The resulting vector3.
 */
VEC_API vector3 vec3_move_toward(vector3 current, vector3 target, vm_float_t max_delta);

/**
 * @brief Clamps the vector length to max_len.
 *
 * @see vec3_limit_length_ptr
 *
 * @param v Input vector.
 * @param max_len Maximum length.
 * @return The resulting vector3.
 */
VEC_API vector3 vec3_limit_length(vector3 v, vm_float_t max_len);

/**
 * @brief Returns the x and y components as a 2D vector.
 *
 * @see vec3_xy_ptr
 *
 * @param v Input vector.
 * @return The resulting vector2.
 */
VEC_API vector2 vec3_xy(vector3 v);

/**
 * @brief Builds a tangent and bitangent orthonormal to n.
 *
 * @param n Unit normal.
 * @param t Output tangent.
 * @param b Output bitangent.
 */
VEC_API void vec3_orthonormal_basis(vector3 n, vector3 *t, vector3 *b);

/**
 * @brief Computes the dot product of two `vector3`.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return The dot product (a.x * b.x + a.y * b.y + a.z * b.z).
 */
VEC_API vm_float_t vec3_dot(vector3 a, vector3 b);

/**
 * @brief Computes the length (magnitude) of a `vector3`.
 *
 * @param v The vector.
 * @return The Euclidean length of v.
 */
VEC_API vm_float_t vec3_length(vector3 v);

/**
 * @brief Returns the squared Euclidean length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_float_t vec3_length_squared(vector3 v);

/**
 * @brief Returns the Manhattan (L1) length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_float_t vec3_length_manhattan(vector3 v);

/**
 * @brief Returns the Chebyshev (L-inf) length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_float_t vec3_length_chebyshev(vector3 v);

/**
 * @brief Computes the Euclidean distance between two `vector3`.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return The distance between a and b.
 */
VEC_API vm_float_t vec3_distance(vector3 a, vector3 b);

/**
 * @brief Returns the squared Euclidean distance between a and b.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting scalar.
 */
VEC_API vm_float_t vec3_distance_squared(vector3 a, vector3 b);

/**
 * @brief Computes the angle between two non-zero `vector3` in radians.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return The angle between the directions of a and b.
 */
VEC_API vm_float_t vec3_angle(vector3 a, vector3 b);

/**
 * @brief Returns the signed angle from a to b around axis.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @param axis Rotation axis.
 * @return The resulting scalar.
 */
VEC_API vm_float_t vec3_signed_angle(vector3 a, vector3 b, vector3 axis);

/**
 * @brief Returns the smallest component.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_float_t vec3_min_component(vector3 v);

/**
 * @brief Returns the largest component.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_float_t vec3_max_component(vector3 v);

/**
 * @brief Returns the sum of all components.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_float_t vec3_sum(vector3 v);

/**
 * @brief Returns true if every component is zero.
 *
 * @param v Input vector.
 * @return True if every component is zero.
 */
VEC_API bool vec3_is_zero(vector3 v);

/**
 * @brief Returns true if the vector has unit length.
 *
 * @param v Input vector.
 * @return True if the vector has unit length.
 */
VEC_API bool vec3_is_normalized(vector3 v);

/**
 * @brief Returns true if a and b are within eps of each other.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @param eps Distance tolerance.
 * @return True if a and b are within eps.
 */
VEC_API bool vec3_near(vector3 a, vector3 b, vm_float_t eps);

/* ---- vector4 ------------------------------------------------------------- */

/**
 * @brief Component-wise addition of two vectors.
 *
 * @see vec4_add_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Result vector.
 */
VEC_API vector4 vec4_add(vector4 a, vector4 b);

/**
 * @brief Component-wise subtraction of two vectors.
 *
 * @see vec4_sub_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Result vector.
 */
VEC_API vector4 vec4_sub(vector4 a, vector4 b);

/**
 * @brief Component-wise multiplication of vector by scalar.
 *
 * @see vec4_mul_scalar_ptr
 *
 * @param v The vector.
 * @param s The scalar.
 * @return Result vector.
 */
VEC_API vector4 vec4_mul_scalar(vector4 v, vm_float_t s);

/**
 * @brief Component-wise division of vector by scalar.
 *
 * @see vec4_div_scalar_ptr
 *
 * @param v The vector.
 * @param s The scalar (non-zero).
 * @return Result vector.
 */
VEC_API vector4 vec4_div_scalar(vector4 v, vm_float_t s);

/**
 * @brief Component-wise multiplication of two vectors.
 *
 * @see vec4_mul_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Result vector.
 */
VEC_API vector4 vec4_mul(vector4 a, vector4 b);

/**
 * @brief Negation of a vector.
 *
 * @see vec4_neg_ptr
 *
 * @param v The vector.
 * @return The negated vector.
 */
VEC_API vector4 vec4_neg(vector4 v);

/**
 * @brief Computes the absolute value per component of a `vector4`.
 *
 * @see vec4_abs_ptr
 *
 * @param v The vector.
 * @return The absolute vector4.
 */
VEC_API vector4 vec4_abs(vector4 v);

/**
 * @brief Returns a zero-initialized `vector4`.
 *
 * @return A vector4 with all components set to 0.0f.
 */
VEC_API vector4 vec4_zero(void);

/**
 * Returns a `vector4` with all components set to 1.0f.
 *
 * @return A `vector4` with all components set to 1.0f.
 */
VEC_API vector4 vec4_one(void);

/**
 * @brief Returns a `vector4` along the x-axis.
 *
 * @param x The x component value.
 * @return A vector4 with (x, 0.0f, 0.0f, 0.0f).
 */
VEC_API vector4 vec4_x_axis(vm_float_t x);

/**
 * @brief Returns a `vector4` along the y-axis.
 *
 * @param y The y component value.
 * @return A vector4 with (0.0f, y, 0.0f, 0.0f).
 */
VEC_API vector4 vec4_y_axis(vm_float_t y);

/**
 * @brief Returns a `vector4` along the z-axis.
 *
 * @param z The z component value.
 * @return A vector4 with (0.0f, 0.0f, z, 0.0f).
 */
VEC_API vector4 vec4_z_axis(vm_float_t z);

/**
 * @brief Returns a `vector4` along the w-axis.
 *
 * @param w The w component value.
 * @return A vector4 with (0.0f, 0.0f, 0.0f, w).
 */
VEC_API vector4 vec4_w_axis(vm_float_t w);

/**
 * @brief Returns a`vector4` for scaling along the x-axis.
 *
 * @param x The x scale factor.
 * @return A vector4 with (x, 1.0f, 1.0f, 1.0f).
 */
VEC_API vector4 vec4_x_scale(vm_float_t x);

/**
 * @brief Returns a `vector4` for scaling along the y-axis.
 *
 * @param y The y scale factor.
 * @return A vector4 with (1.0f, y, 1.0f, 1.0f).
 */
VEC_API vector4 vec4_y_scale(vm_float_t y);

/**
 * @brief Returns a `vector4`for scaling along the z-axis.
 *
 * @param z The z scale factor.
 * @return A vector4 with (1.0f, 1.0f, z, 1.0f).
 */
VEC_API vector4 vec4_z_scale(vm_float_t z);

/**
 * @brief Returns a vector4 for scaling along the w-axis.
 *
 * @param w The w scale factor.
 * @return A vector4 with (1.0f, 1.0f, 1.0f, w).
 */
VEC_API vector4 vec4_w_scale(vm_float_t w);

/**
 * @brief Normalizes a `vector4` to unit length.
 *
 * @see vec4_normalize_ptr
 *
 * @param v The vector (non-zero).
 * @return The normalized vector.
 */
VEC_API vector4 vec4_normalize(vector4 v);

/**
 * @brief Computes the component-wise minimum of two `vector4`.
 *
 * @see vec4_min_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Per-component min(a, b).
 */
VEC_API vector4 vec4_min(vector4 a, vector4 b);

/**
 * @brief Computes the component-wise maximum of two `vector4`.
 *
 * @see vec4_max_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Per-component max(a, b).
 */
VEC_API vector4 vec4_max(vector4 a, vector4 b);

/**
 * @brief Computes the sign per component of a `vector4` (-1, 0, or 1).
 *
 * @see vec4_sign_ptr
 *
 * @param v The vector.
 * @return The sign vector.
 */
VEC_API vector4 vec4_sign(vector4 v);

/**
 * @brief Applies the floor per component to a `vector4`.
 *
 * @see vec4_floor_ptr
 *
 * @param v The vector.
 * @return The floored vector.
 */
VEC_API vector4 vec4_floor(vector4 v);

/**
 * @brief Applies ceil per component to a `vector4`.
 *
 * @see vec4_ceil_ptr
 *
 * @param v The vector.
 * @return The ceiling vector.
 */
VEC_API vector4 vec4_ceil(vector4 v);

/**
 * @brief Applies round per component to a `vector4`.
 *
 * @see vec4_round_ptr
 *
 * @param v The vector.
 * @return The rounded vector.
 */
VEC_API vector4 vec4_round(vector4 v);

/**
 * @brief Linearly interpolates between two `vector4`.
 *
 * @see vec4_lerp_ptr
 *
 * @param a Start vector.
 * @param b End vector.
 * @param t Interpolation factor [0, 1].
 * @return The interpolated vector (a * (1 - t) + b * t).
 */
VEC_API vector4 vec4_lerp(vector4 a, vector4 b, vm_float_t t);

/**
 * @brief Clamps a `vector4` between min and max per component.
 *
 * @see vec4_clamp_ptr
 *
 * @param v The vector.
 * @param min Minimum bounds.
 * @param max Maximum bounds.
 * @return The clamped vector.
 */
VEC_API vector4 vec4_clamp(vector4 v, vector4 min, vector4 max);

/**
 * @brief Homogenizes a `vector4` (divides x, y, z by w).
 *
 * @see vec4_homogenize_ptr
 *
 * @param v The homogeneous vector (w != 0.0f).
 * @return The normalized cartesian vector4.
 */
VEC_API vector4 vec4_homogenize(vector4 v);

/**
 * @brief Divides two vectors component-wise.
 *
 * @see vec4_div_ptr
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector4.
 */
VEC_API vector4 vec4_div(vector4 a, vector4 b);

/**
 * @brief Adds a scalar to each component.
 *
 * @see vec4_add_scalar_ptr
 *
 * @param v Input vector.
 * @param s Scalar value.
 * @return The resulting vector4.
 */
VEC_API vector4 vec4_add_scalar(vector4 v, vm_float_t s);
/**
 * @brief Subtracts a scalar from each component.
 *
 * @see vec4_sub_scalar_ptr
 *
 * @param v Input vector.
 * @param s Scalar value.
 * @return The resulting vector4.
 */
VEC_API vector4 vec4_sub_scalar(vector4 v, vm_float_t s);

/**
 * @brief Clamps each component to the scalar range [min, max].
 *
 * @see vec4_clamp_scalar_ptr
 *
 * @param v Input vector.
 * @param min Lower bound.
 * @param max Upper bound.
 * @return The resulting vector4.
 */
VEC_API vector4 vec4_clamp_scalar(vector4 v, vm_float_t min, vm_float_t max);

/**
 * @brief Clamps each component to the range [0, 1].
 *
 * @see vec4_saturate_ptr
 *
 * @param v Input vector.
 * @return The resulting vector4.
 */
VEC_API vector4 vec4_saturate(vector4 v);

/**
 * @brief Returns the fractional part of each component.
 *
 * @see vec4_fract_ptr
 *
 * @param v Input vector.
 * @return The resulting vector4.
 */
VEC_API vector4 vec4_fract(vector4 v);

/**
 * @brief Projects a onto b.
 *
 * @see vec4_project_ptr
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector4.
 */
VEC_API vector4 vec4_project(vector4 a, vector4 b);

/**
 * @brief Returns the component of a orthogonal to b.
 *
 * @see vec4_reject_ptr
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector4.
 */
VEC_API vector4 vec4_reject(vector4 a, vector4 b);

/**
 * @brief Removes the component of v along normal.
 *
 * @see vec4_slide_ptr
 *
 * @param v Input vector.
 * @param normal Surface normal.
 * @return The resulting vector4.
 */
VEC_API vector4 vec4_slide(vector4 v, vector4 normal);

/**
 * @brief Returns a vector with every component set to s.
 *
 * @see vec4_splat_ptr
 *
 * @param s Scalar value.
 * @return The resulting vector4.
 */
VEC_API vector4 vec4_splat(vm_float_t s);

/**
 * @brief Converts a `vector4` to a `vector3` (discards the w component).
 *
 * @param v The source vector4.
 * @return A vector3 {v.x, v.y, v.z}.
 */
VEC_API vector3 vec4_to_vec3(vector4 v);

/**
 * @brief Computes the dot product of two `vector4`.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return The dot product (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w).
 */
VEC_API vm_float_t vec4_dot(vector4 a, vector4 b);

/**
 * @brief Computes the length (magnitude) of a `vector4`.
 *
 * @param v The vector.
 * @return The Euclidean length of v.
 */
VEC_API vm_float_t vec4_length(vector4 v);

/**
 * @brief Returns the squared Euclidean length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_float_t vec4_length_squared(vector4 v);

/**
 * @brief Computes the Euclidean distance between two `vector4`.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return The distance between a and b.
 */
VEC_API vm_float_t vec4_distance(vector4 a, vector4 b);

/**
 * @brief Returns the squared Euclidean distance between a and b.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting scalar.
 */
VEC_API vm_float_t vec4_distance_squared(vector4 a, vector4 b);

/**
 * @brief Returns true if every component is zero.
 *
 * @param v Input vector.
 * @return True if every component is zero.
 */
VEC_API bool vec4_is_zero(vector4 v);

/**
 * @brief Returns true if the vector has unit length.
 *
 * @param v Input vector.
 * @return True if the vector has unit length.
 */
VEC_API bool vec4_is_normalized(vector4 v);

/**
 * @brief Returns true if a and b are within eps of each other.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @param eps Distance tolerance.
 * @return True if a and b are within eps.
 */
VEC_API bool vec4_near(vector4 a, vector4 b, vm_float_t eps);

/*******************************************************************************
 * Floating-point vector pointer-based performance functions
 ******************************************************************************/

/* ---- vector2 pointer-based operations ------------------------------------ */

/**
 * @brief Adds vectors a and b component-wise, storing the result in res.
 *
 * @param res Output vector.
 * @param a Input vector a.
 * @param b Input vector b.
 */
VEC_API void vec2_add_ptr(vector2 *res, const vector2 *a, const vector2 *b);

/**
 * @brief Subtracts vector b from vector a component-wise, storing the result in res.
 *
 * @param res Output vector.
 * @param a Input vector a.
 * @param b Input vector b.
 */
VEC_API void vec2_sub_ptr(vector2 *res, const vector2 *a, const vector2 *b);

/**
 * @brief Multiplies vector v by scalar s component-wise, storing the result in res.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param s Input scalar.
 */
VEC_API void vec2_mul_scalar_ptr(vector2 *res, const vector2 *v, vm_float_t s);

/**
 * @brief Divides vector v by scalar s component-wise, storing the result in res.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param s Input scalar.
 */
VEC_API void vec2_div_scalar_ptr(vector2 *res, const vector2 *v, vm_float_t s);

/**
 * @brief Multiplies vectors a and b component-wise, storing the result in res.
 *
 * @param res Output vector.
 * @param a Input vector a.
 * @param b Input vector b.
 */
VEC_API void vec2_mul_ptr(vector2 *res, const vector2 *a, const vector2 *b);

/**
 * @brief Negates the components of vector v, storing the result in res.
 *
 * @param res Output vector.
 * @param v Input vector.
 */
VEC_API void vec2_neg_ptr(vector2 *res, const vector2 *v);

/**
 * @brief Computes the absolute values of the components of vector v, storing
 * the result in res.
 *
 * @param res Output vector.
 * @param v Input vector.
 */
VEC_API void vec2_abs_ptr(vector2 *res, const vector2 *v);

/**
 * @brief Normalizes vector v to unit length, storing the result in res.
 *
 * @param res Output vector.
 * @param v Input vector.
 */
VEC_API void vec2_normalize_ptr(vector2 *res, const vector2 *v);

/**
 * @brief Computes the component-wise minimum of vectors a and b, storing the
 * result in res.
 *
 * @param res Output vector.
 * @param a Input vector a.
 * @param b Input vector b.
 */
VEC_API void vec2_min_ptr(vector2 *res, const vector2 *a, const vector2 *b);

/**
 * @brief Computes the component-wise maximum of vectors a and b, storing the
 * result in res.
 *
 * @param res Output vector.
 * @param a Input vector a.
 * @param b Input vector b.
 */
VEC_API void vec2_max_ptr(vector2 *res, const vector2 *a, const vector2 *b);

/**
 * @brief Sets each component of res to the sign of the corresponding component
 * in v (+1, -1).
 *
 * @param res Output vector.
 * @param v Input vector.
 */
VEC_API void vec2_sign_ptr(vector2 *res, const vector2 *v);

/**
 * @brief Applies the floor function to each component of vector v, storing the
 * result in res.
 *
 * @param res Output vector.
 * @param v Input vector.
 */
VEC_API void vec2_floor_ptr(vector2 *res, const vector2 *v);

/**
 * @brief Applies the ceil function to each component of vector v, storing the
 * result in res.
 *
 * @param res Output vector.
 * @param v Input vector.
 */
VEC_API void vec2_ceil_ptr(vector2 *res, const vector2 *v);

/**
 * @brief Applies the round function to each component of vector v, storing the
 * result in res.
 *
 * @param res Output vector.
 * @param v Input vector.
 */
VEC_API void vec2_round_ptr(vector2 *res, const vector2 *v);

/**
 * @brief Computes the perpendicular vector to v (90 degrees counterclockwise
 * rotation), storing the result in res.
 *
 * @param res Output vector.
 * @param v Input vector.
 */
VEC_API void vec2_perpendicular_ptr(vector2 *res, const vector2 *v);

/**
 * @brief Scales a vector by a scalar component-wise, storing the result in res.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param s Input scalar.
 */
VEC_API void vec2_scale_ptr(vector2 *res, const vector2 *v, vm_float_t s);

/**
 * @brief Computes the 2D cross-product of a and b, storing the scalar value in
 * res->x and 0 in res->y.
 *
 * @param res Output vector.
 * @param a Input vector a.
 * @param b Input vector b.
 */
VEC_API void vec2_cross_ptr(vector2 *res, const vector2 *a, const vector2 *b);

/**
 * @brief Reflects vector v across the normal, storing the result in res.
 *
 * @param res Output vector.
 * @param v Input incident vector.
 * @param normal Input surface normal.
 */
VEC_API void vec2_reflect_ptr(vector2 *res, const vector2 *v, const vector2 *normal);

/**
 * @brief Projects vector `a` onto vector `b` (scalar projection scaled by `b`).
 *
 * @param res The output projected vector.
 * @param a The vector to project.
 * @param b The direction vector (non-zero length recommended).
 */
VEC_API void vec2_project_ptr(vector2 *res, const vector2 *a, const vector2 *b);

/**
 * @brief Computes a tangent vector perpendicular to the input (90 degrees clockwise).
 *
 * Equivalent to `(v.y, -v.x)`.
 *
 * @param res The output tangent vector.
 * @param v The input vector.
 */
VEC_API void vec2_tangent_ptr(vector2 *res, const vector2 *v);

/**
 * @brief Rotates the input vector counterclockwise by the given angle (radians).
 *
 * Uses standard 2D rotation matrix.
 *
 * @param result The output rotated vector.
 * @param v The input vector.
 * @param radians The rotation angle in radians.
 */
VEC_API void vec2_rotate_ptr(vector2 *result, const vector2 *v, vm_float_t radians);

/**
 * @brief Rotates vector v by the given angle in degrees, storing the result in result.
 *
 * @param result Output vector.
 * @param v Input vector.
 * @param degrees Rotation angle in degrees.
 */
VEC_API void vec2_rotate_deg_ptr(vector2 *result, const vector2 *v, vm_float_t degrees);

/**
 * @brief Slides the input vector tangent to the normal (removes normal component).
 *
 * Formula: v - dot(v, normal) * normal (assumes unit normal).
 *
 * @param result The output slid vector.
 * @param v The input vector.
 * @param normal The unit normal vector.
 */
VEC_API void vec2_slide_ptr(vector2 *result, const vector2 *v, const vector2 *normal);

/**
 * @brief Clamps vector v component-wise between min and max.
 *
 * @param res Output vector.
 * @param v Input vector to clamp.
 * @param min Minimum bounds vector.
 * @param max Maximum bounds vector.
 */
VEC_API void vec2_clamp_ptr(vector2 *res, const vector2 *v, const vector2 *min, const vector2 *max);

/**
 * @brief Linearly interpolates from a to b by t.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 * @param t Interpolation factor.
 */
VEC_API void vec2_lerp_ptr(vector2 *res, const vector2 *a, const vector2 *b, vm_float_t t);

/**
 * @brief Divides two vectors component-wise.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
VEC_API void vec2_div_ptr(vector2 *res, const vector2 *a, const vector2 *b);

/**
 * @brief Adds a scalar to each component.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param s Scalar value.
 */
VEC_API void vec2_add_scalar_ptr(vector2 *res, const vector2 *v, vm_float_t s);

/**
 * @brief Subtracts a scalar from each component.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param s Scalar value.
 */
VEC_API void vec2_sub_scalar_ptr(vector2 *res, const vector2 *v, vm_float_t s);

/**
 * @brief Clamps each component to the scalar range [min, max].
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param min Lower bound.
 * @param max Upper bound.
 */
VEC_API void vec2_clamp_scalar_ptr(vector2 *res, const vector2 *v, vm_float_t min, vm_float_t max);

/**
 * @brief Clamps each component to the range [0, 1].
 *
 * @param res Output vector.
 * @param v Input vector.
 */
VEC_API void vec2_saturate_ptr(vector2 *res, const vector2 *v);

/**
 * @brief Returns the fractional part of each component.
 *
 * @param res Output vector.
 * @param v Input vector.
 */
VEC_API void vec2_fract_ptr(vector2 *res, const vector2 *v);

/**
 * @brief Computes the refraction of incident across normal with ratio eta.
 *
 * @param res Output vector.
 * @param incident Incident vector.
 * @param normal Surface normal.
 * @param eta Ratio of indices of refraction.
 */
VEC_API void vec2_refract_ptr(vector2 *res, const vector2 *incident, const vector2 *normal, vm_float_t eta);

/**
 * @brief Returns the component of a orthogonal to b.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
VEC_API void vec2_reject_ptr(vector2 *res, const vector2 *a, const vector2 *b);

/**
 * @brief Rotates v around pivot by angle radians.
 *
 * @see vec2_rotate_ptr
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param pivot Rotation pivot.
 * @param radians Angle in radians.
 */
VEC_API void vec2_rotate_around_ptr(vector2 *res, const vector2 *v, const vector2 *pivot, vm_float_t radians);

/**
 * @brief Rotates vector v around the given pivot point by the specified angle in degrees.
 *
 * This function converts the input angle from degrees to radians and then
 * delegates to the radian-based rotation routine.
 *
 * @param res Output vector storing the rotated result.
 * @param v Input vector to rotate.
 * @param pivot Pivot point around which to rotate.
 * @param degrees Rotation angle in degrees.
 */
VEC_API void vec2_rotate_around_deg_ptr(vector2 *res, const vector2 *v, const vector2 *pivot, vm_float_t degrees);

/**
 * @brief Moves current toward target by at most max_delta.
 *
 * @param res Output vector.
 * @param current Current position.
 * @param target Target position.
 * @param max_delta Maximum distance to move.
 */
VEC_API void vec2_move_toward_ptr(vector2 *res, const vector2 *current, const vector2 *target, vm_float_t max_delta);

/**
 * @brief Clamps the vector length to max_len.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param max_len Maximum length.
 */
VEC_API void vec2_limit_length_ptr(vector2 *res, const vector2 *v, vm_float_t max_len);

/**
 * @brief Converts a vector2 to a vector3 with the given z.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param z Z component.
 */
VEC_API void vec2_to_vec3_ptr(vector3 *res, const vector2 *v, vm_float_t z);

/* ---- vector3 pointer-based operations ------------------------------------ */

/**
 * @brief Component-wise addition of two vectors.
 *
 * @param res Result vector.
 * @param a First vector.
 * @param b Second vector.
 */
VEC_API void vec3_add_ptr(vector3 *res, const vector3 *a, const vector3 *b);

/**
 * @brief Component-wise subtraction of two vectors.
 *
 * @param res Result vector.
 * @param a First vector.
 * @param b Second vector.
 */
VEC_API void vec3_sub_ptr(vector3 *res, const vector3 *a, const vector3 *b);

/**
 * @brief Component-wise multiplication of vector by scalar.
 *
 * @param res Result vector.
 * @param v Vector.
 * @param s Scalar.
 */
VEC_API void vec3_mul_scalar_ptr(vector3 *res, const vector3 *v, vm_float_t s);

/**
 * @brief Component-wise division of vector by scalar.
 *
 * @param res Result vector.
 * @param v Vector.
 * @param s Scalar.
 */
VEC_API void vec3_div_scalar_ptr(vector3 *res, const vector3 *v, vm_float_t s);

/**
 * @brief Component-wise multiplication of two vectors.
 *
 * @param res Result vector.
 * @param a First vector.
 * @param b Second vector.
 */
VEC_API void vec3_mul_ptr(vector3 *res, const vector3 *a, const vector3 *b);

/**
 * @brief Negation of a vector.
 *
 * @param res Result vector.
 * @param v Vector.
 */
VEC_API void vec3_neg_ptr(vector3 *res, const vector3 *v);

/**
 * @brief Computes the absolute value per component of a `vector3`.
 *
 * @param res Result vector.
 * @param v Vector.
 */
VEC_API void vec3_abs_ptr(vector3 *res, const vector3 *v);

/**
 * @brief Normalize a vector to unit length.
 *
 * @param res Result vector.
 * @param v Vector.
 */
VEC_API void vec3_normalize_ptr(vector3 *res, const vector3 *v);

/**
 * @brief Compute cross-product of two vectors.
 *
 * @param res Result vector.
 * @param a First vector.
 * @param b Second vector.
 */
VEC_API void vec3_cross_ptr(vector3 *res, const vector3 *a, const vector3 *b);

/**
 * @brief Component-wise minimum of two vectors.
 *
 * @param res Result vector.
 * @param a First vector.
 * @param b Second vector.
 */
VEC_API void vec3_min_ptr(vector3 *res, const vector3 *a, const vector3 *b);

/**
 * @brief Component-wise maximum of two vectors.
 *
 * @param res Result vector.
 * @param a First vector.
 * @param b Second vector.
 */
VEC_API void vec3_max_ptr(vector3 *res, const vector3 *a, const vector3 *b);

/**
 * @brief Component-wise sign of a vector.
 *
 * @param res Result vector.
 * @param v Vector.
 */
VEC_API void vec3_sign_ptr(vector3 *res, const vector3 *v);

/**
 * @brief Component-wise floor of a vector.
 *
 * @param res Result vector.
 * @param v Vector.
 */
VEC_API void vec3_floor_ptr(vector3 *res, const vector3 *v);

/**
 * @brief Component-wise ceil of a vector.
 *
 * @param res Result vector.
 * @param v Vector.
 */
VEC_API void vec3_ceil_ptr(vector3 *res, const vector3 *v);

/**
 * @brief Component-wise round of a vector.
 *
 * @param res Result vector.
 * @param v Vector.
 */
VEC_API void vec3_round_ptr(vector3 *res, const vector3 *v);

/**
 * @brief Compute reflection of the incident vector over normal.
 *
 * @param res Result vector.
 * @param incident Incident vector.
 * @param normal Normal vector.
 */
VEC_API void vec3_reflect_ptr(vector3 *res, const vector3 *incident, const vector3 *normal);

/**
 * @brief Compute refraction of the incident vector through normal with eta.
 *
 * @param res Result vector.
 * @param incident Incident vector.
 * @param normal Normal vector.
 * @param eta Refraction index ratio.
 */
VEC_API void vec3_refract_ptr(vector3 *res, const vector3 *incident, const vector3 *normal, vm_float_t eta);

/**
 * @brief Linear interpolation between two vectors.
 *
 * @param res Result vector.
 * @param a Start vector.
 * @param b End vector.
 * @param t Interpolation factor.
 */
VEC_API void vec3_lerp_ptr(vector3 *res, const vector3 *a, const vector3 *b, vm_float_t t);

/**
 * @brief Clamp vector components between min and max.
 *
 * @param res Result vector.
 * @param v Vector.
 * @param min Minimum bounds.
 * @param max Maximum bounds.
 */
VEC_API void vec3_clamp_ptr(vector3 *res, const vector3 *v, const vector3 *min, const vector3 *max);

/**
 * @brief Scales the vector by a scalar.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param s Scalar value.
 */
VEC_API void vec3_scale_ptr(vector3 *res, const vector3 *v, vm_float_t s);

/**
 * @brief Divides two vectors component-wise.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
VEC_API void vec3_div_ptr(vector3 *res, const vector3 *a, const vector3 *b);

/**
 * @brief Adds a scalar to each component.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param s Scalar value.
 */
VEC_API void vec3_add_scalar_ptr(vector3 *res, const vector3 *v, vm_float_t s);

/**
 * @brief Subtracts a scalar from each component.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param s Scalar value.
 */
VEC_API void vec3_sub_scalar_ptr(vector3 *res, const vector3 *v, vm_float_t s);

/**
 * @brief Clamps each component to the scalar range [min, max].
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param min Lower bound.
 * @param max Upper bound.
 */
VEC_API void vec3_clamp_scalar_ptr(vector3 *res, const vector3 *v, vm_float_t min, vm_float_t max);

/**
 * @brief Clamps each component to the range [0, 1].
 *
 * @param res Output vector.
 * @param v Input vector.
 */
VEC_API void vec3_saturate_ptr(vector3 *res, const vector3 *v);

/**
 * @brief Returns the fractional part of each component.
 *
 * @param res Output vector.
 * @param v Input vector.
 */
VEC_API void vec3_fract_ptr(vector3 *res, const vector3 *v);

/**
 * @brief Projects a onto b.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
VEC_API void vec3_project_ptr(vector3 *res, const vector3 *a, const vector3 *b);

/**
 * @brief Removes the component of v along normal.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param normal Surface normal.
 */
VEC_API void vec3_slide_ptr(vector3 *res, const vector3 *v, const vector3 *normal);

/**
 * @brief Returns the component of a orthogonal to b.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
VEC_API void vec3_reject_ptr(vector3 *res, const vector3 *a, const vector3 *b);

/**
 * @brief Rotates v around axis by angle radians.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param axis Rotation axis.
 * @param radians Angle in radians.
 */
VEC_API void vec3_rotate_axis_ptr(vector3 *res, const vector3 *v, const vector3 *axis, vm_float_t radians);

/**
 * @brief Component-wise rotation of a vector around an axis by a given angle in degrees.
 *
 * @param res Result vector.
 * @param v Vector to rotate.
 * @param axis Rotation axis.
 * @param degrees Angle in degrees.
 */
VEC_API void vec3_rotate_axis_deg_ptr(vector3 *res, const vector3 *v, const vector3 *axis, vm_float_t degrees);

/**
 * @brief Builds a higher-dimension vector from a vector2.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param z Z component.
 */
VEC_API void vec3_from_vec2_ptr(vector3 *res, const vector2 *v, vm_float_t z);

/**
 * @brief Returns the x and y components as a 2D vector.
 *
 * @param res Output vector.
 * @param v Input vector.
 */
VEC_API void vec3_xy_ptr(vector2 *res, const vector3 *v);

/**
 * @brief Moves current toward target by at most max_delta.
 *
 * @param res Output vector.
 * @param current Current position.
 * @param target Target position.
 * @param max_delta Maximum distance to move.
 */
VEC_API void vec3_move_toward_ptr(vector3 *res, const vector3 *current, const vector3 *target, vm_float_t max_delta);

/**
 * @brief Clamps the vector length to max_len.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param max_len Maximum length.
 */
VEC_API void vec3_limit_length_ptr(vector3 *res, const vector3 *v, vm_float_t max_len);

/**
 * @brief Builds a tangent and bitangent orthonormal to n.
 *
 * @param n Unit normal.
 * @param t Output tangent.
 * @param b Output bitangent.
 */
VEC_API void vec3_orthonormal_basis_ptr(const vector3 *n, vector3 *t, vector3 *b);

/* ---- vector4 pointer-based operations ------------------------------------ */

/**
 * @brief Component-wise addition (dispatched).
 *
 * @see vec4_add_ptr_scalar
 *
 * @param res Result vector.
 * @param a First vector.
 * @param b Second vector.
 */
VEC_API void vec4_add_ptr(vector4 *res, const vector4 *a, const vector4 *b);

/**
 * @brief Component-wise subtraction (dispatched).
 *
 * @see vec4_sub_ptr_scalar
 *
 * @param res Result vector.
 * @param a First vector.
 * @param b Second vector.
 */
VEC_API void vec4_sub_ptr(vector4 *res, const vector4 *a, const vector4 *b);

/**
 * @brief Multiplies a vector by a scalar (dispatched).
 *
 * @see vec4_mul_scalar_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 * @param s Scalar.
 */
VEC_API void vec4_mul_scalar_ptr(vector4 *res, const vector4 *v, vm_float_t s);

/**
 * @brief Divides a vector by a scalar (dispatched).
 *
 * @see vec4_div_scalar_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 * @param s Scalar.
 */
VEC_API void vec4_div_scalar_ptr(vector4 *res, const vector4 *v, vm_float_t s);

/**
 * @brief Component-wise multiplication (dispatched).
 *
 * @see vec4_mul_ptr_scalar
 *
 * @param res Result vector.
 * @param a First vector.
 * @param b Second vector.
 */
VEC_API void vec4_mul_ptr(vector4 *res, const vector4 *a, const vector4 *b);

/**
 * @brief Negates a vector (dispatched).
 *
 * @see vec4_neg_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 */
VEC_API void vec4_neg_ptr(vector4 *res, const vector4 *v);

/**
 * @brief Component-wise absolute value (dispatched).
 *
 * @see vec4_abs_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 */
VEC_API void vec4_abs_ptr(vector4 *res, const vector4 *v);

/**
 * @brief Normalizes a vector (dispatched).
 *
 * @see vec4_normalize_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 */
VEC_API void vec4_normalize_ptr(vector4 *res, const vector4 *v);

/**
 * @brief Component-wise minimum (dispatched).
 *
 * @see vec4_min_ptr_scalar
 *
 * @param res Result vector.
 * @param a First vector.
 * @param b Second vector.
 */
VEC_API void vec4_min_ptr(vector4 *res, const vector4 *a, const vector4 *b);

/**
 * @brief Component-wise maximum (dispatched).
 *
 * @see vec4_max_ptr_scalar
 *
 * @param res Result vector.
 * @param a First vector.
 * @param b Second vector.
 */
VEC_API void vec4_max_ptr(vector4 *res, const vector4 *a, const vector4 *b);

/**
 * @brief Component-wise sign (dispatched).
 *
 * @see vec4_sign_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 */
VEC_API void vec4_sign_ptr(vector4 *res, const vector4 *v);

/**
 * @brief Component-wise floor (dispatched).
 *
 * @see vec4_floor_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 */
VEC_API void vec4_floor_ptr(vector4 *res, const vector4 *v);

/**
 * @brief Component-wise ceil (dispatched).
 *
 * @see vec4_ceil_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 */
VEC_API void vec4_ceil_ptr(vector4 *res, const vector4 *v);

/**
 * @brief Component-wise round (dispatched).
 *
 * @see vec4_round_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 */
VEC_API void vec4_round_ptr(vector4 *res, const vector4 *v);

/**
 * @brief Linear interpolation (dispatched).
 *
 * @see vec4_lerp_ptr_scalar
 *
 * @param res Result vector.
 * @param a First vector.
 * @param b Second vector.
 * @param t Interpolation factor.
 */
VEC_API void vec4_lerp_ptr(vector4 *res, const vector4 *a, const vector4 *b, vm_float_t t);

/**
 * @brief Component-wise clamp (dispatched).
 *
 * @see vec4_clamp_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 * @param min Lower bound.
 * @param max Upper bound.
 */
VEC_API void vec4_clamp_ptr(vector4 *res, const vector4 *v, const vector4 *min, const vector4 *max);

/**
 * @brief Divides xyz by w (dispatched).
 *
 * @see vec4_homogenize_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 */
VEC_API void vec4_homogenize_ptr(vector4 *res, const vector4 *v);

/**
 * @brief Component-wise division (dispatched).
 *
 * @see vec4_div_ptr_scalar
 *
 * @param res Result vector.
 * @param a First vector.
 * @param b Second vector.
 */
VEC_API void vec4_div_ptr(vector4 *res, const vector4 *a, const vector4 *b);

/**
 * @brief Adds a scalar to each component (dispatched).
 *
 * @see vec4_add_scalar_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 * @param s Scalar.
 */
VEC_API void vec4_add_scalar_ptr(vector4 *res, const vector4 *v, vm_float_t s);

/**
 * @brief Subtracts a scalar from each component (dispatched).
 *
 * @see vec4_sub_scalar_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 * @param s Scalar.
 */
VEC_API void vec4_sub_scalar_ptr(vector4 *res, const vector4 *v, vm_float_t s);

/**
 * @brief Clamps each component to a scalar range (dispatched).
 *
 * @see vec4_clamp_scalar_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 * @param min Lower bound.
 * @param max Upper bound.
 */
VEC_API void vec4_clamp_scalar_ptr(vector4 *res, const vector4 *v, vm_float_t min, vm_float_t max);

/**
 * @brief Clamps each component to [0, 1] (dispatched).
 *
 * @see vec4_saturate_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 */
VEC_API void vec4_saturate_ptr(vector4 *res, const vector4 *v);

/**
 * @brief Component-wise fractional part (dispatched).
 *
 * @see vec4_fract_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 */
VEC_API void vec4_fract_ptr(vector4 *res, const vector4 *v);

/**
 * @brief Projects a onto b.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
VEC_API void vec4_project_ptr(vector4 *res, const vector4 *a, const vector4 *b);

/**
 * @brief Returns the component of a orthogonal to b.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
VEC_API void vec4_reject_ptr(vector4 *res, const vector4 *a, const vector4 *b);

/**
 * @brief Removes the component of v along normal.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param normal Surface normal.
 */
VEC_API void vec4_slide_ptr(vector4 *res, const vector4 *v, const vector4 *normal);

/**
 * @brief Copies the x, y, z components from a `vector4` to a `vector3`.
 *
 * @param res Pointer to result `vector3`.
 * @param v Pointer to input `vector4`.
 */
VEC_API void vec4_to_vec3_ptr(vector3 *res, const vector4 *v);

/* ---- vector2i ------------------------------------------------------------ */

/**
 * @brief Adds two vectors component-wise.
 *
 * @see vec2i_add_ptr
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The sum vector2i.
 */
VEC_API vector2i vec2i_add(vector2i a, vector2i b);

/**
 * @brief Subtracts the second vector from the first component-wise.
 *
 * @see vec2i_sub_ptr
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The difference vector2i.
 */
VEC_API vector2i vec2i_sub(vector2i a, vector2i b);

/**
 * @brief Multiplies a vector by a scalar component-wise.
 *
 * @see vec2i_mul_scalar_ptr
 *
 * @param v The input vector.
 * @param s The scalar multiplier.
 * @return The scaled vector2i.
 */
VEC_API vector2i vec2i_mul_scalar(vector2i v, vm_int_t s);

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
VEC_API vector2i vec2i_div_scalar(vector2i v, vm_int_t s);

/**
 * @brief Multiplies two vectors component-wise (Hadamard product).
 *
 * @see vec2i_mul_ptr
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The product vector2i.
 */
VEC_API vector2i vec2i_mul(vector2i a, vector2i b);

/**
 * @brief Negates the vector (multiplies each component by -1).
 *
 * @see vec2i_neg_ptr
 *
 * @param v The input vector.
 * @return The negated vector2i.
 */
VEC_API vector2i vec2i_neg(vector2i v);

/**
 * @brief Returns the absolute values of each component.
 *
 * @see vec2i_abs_ptr
 *
 * @param v The input vector.
 * @return The absolute value vector2i.
 */
VEC_API vector2i vec2i_abs(vector2i v);

/**
 * @brief Returns a `vector2i` with both components set to 0.
 *
 * @return The zero vector2i.
 */
VEC_API vector2i vec2i_zero(void);

/**
 * @brief Returns a `vector2i` with both components set to 1.
 *
 * @return The one vector2i.
 */
VEC_API vector2i vec2i_one(void);

/**
 * @brief Returns a `vector2i` along the x-axis (y = 0).
 *
 * @param x The x component value.
 * @return The x-axis vector2i.
 */
VEC_API vector2i vec2i_x_axis(vm_int_t x);

/**
 * @brief Returns a `vector2i` along the y-axis (x = 0).
 *
 * @param y The y component value.
 * @return The y-axis vector2i.
 */
VEC_API vector2i vec2i_y_axis(vm_int_t y);

/**
 * @brief Returns a `vector2i` representing x-axis scaling (y = 1).
 *
 * @param x The x scale factor.
 * @return The x-scale vector2i.
 */
VEC_API vector2i vec2i_x_scale(vm_int_t x);

/**
 * @brief Returns a `vector2i` representing y-axis scaling (x = 1).
 *
 * @param y The y scale factor.
 * @return The y-scale vector2i.
 */
VEC_API vector2i vec2i_y_scale(vm_int_t y);

/**
 * @brief Computes the 2D cross-product as a vector.
 *
 * @see vec2i_cross_ptr
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The cross-product vector2i.
 */
VEC_API vector2i vec2i_cross(vector2i a, vector2i b);

/**
 * @brief Normalizes the vector to approximate unit length (float length, truncated to int).
 *
 * @see vec2i_normalize_ptr
 *
 * @param v The input vector.
 * @return The normalized vector2i (unchanged if zero length).
 */
VEC_API vector2i vec2i_normalize(vector2i v);

/**
 * @brief Returns the component-wise minimum of two vectors.
 *
 * @see vec2i_min_ptr
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The minimum vector2i.
 */
VEC_API vector2i vec2i_min(vector2i a, vector2i b);

/**
 * @brief Returns the component-wise maximum of two vectors.
 *
 * @see vec2i_max_ptr
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The maximum vector2i.
 */
VEC_API vector2i vec2i_max(vector2i a, vector2i b);

/**
 * @brief Returns the sign of each component (+1, -1, or 0).
 *
 * @see vec2i_sign_ptr
 *
 * @param v The input vector.
 * @return The sign vector2i.
 */
VEC_API vector2i vec2i_sign(vector2i v);

/**
 * @brief Returns the perpendicular vector (90 degrees counterclockwise).
 *
 * @see vec2i_perpendicular_ptr
 *
 * @param v The input vector.
 * @return The perpendicular vector2i.
 */
VEC_API vector2i vec2i_perpendicular(vector2i v);

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
VEC_API vector2i vec2i_lerp(vector2i a, vector2i b, vm_float_t t);

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
VEC_API vector2i vec2i_clamp(vector2i v, vector2i min, vector2i max);

/**
 * @brief Divides two vectors component-wise.
 *
 * @see vec2i_div_ptr
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector2i.
 */
VEC_API vector2i vec2i_div(vector2i a, vector2i b);

/**
 * @brief Adds a scalar to each component.
 *
 * @see vec2i_add_scalar_ptr
 *
 * @param v Input vector.
 * @param s Scalar value.
 * @return The resulting vector2i.
 */
VEC_API vector2i vec2i_add_scalar(vector2i v, vm_int_t s);

/**
 * @brief Subtracts a scalar from each component.
 *
 * @see vec2i_sub_scalar_ptr
 *
 * @param v Input vector.
 * @param s Scalar value.
 * @return The resulting vector2i.
 */
VEC_API vector2i vec2i_sub_scalar(vector2i v, vm_int_t s);

/**
 * @brief Component-wise floor modulo of a by b.
 *
 * @see vec2i_mod_ptr
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector2i.
 */
VEC_API vector2i vec2i_mod(vector2i a, vector2i b);

/**
 * @brief Component-wise floored division of a by b.
 *
 * @see vec2i_div_floor_ptr
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector2i.
 */
VEC_API vector2i vec2i_div_floor(vector2i a, vector2i b);

/**
 * @brief Wraps each component of v into [0, period).
 *
 * @see vec2i_wrap_ptr
 *
 * @param v Input vector.
 * @param period Wrap period per component.
 * @return The resulting vector2i.
 */
VEC_API vector2i vec2i_wrap(vector2i v, vector2i period);

/**
 * @brief Returns a vector with every component set to s.
 *
 * @see vec2i_splat_ptr
 *
 * @param s Scalar value.
 * @return The resulting vector2i.
 */
VEC_API vector2i vec2i_splat(vm_int_t s);

/**
 * @brief Converts a vector2i to a vector3i using z.
 *
 * @see vec2i_to_vec3i_ptr
 *
 * @param v Input vector.
 * @param z Z component.
 * @return The resulting vector3i.
 */
VEC_API vector3i vec2i_to_vec3i(vector2i v, vm_int_t z);

/**
 * @brief Converts to a unit-length vector2.
 *
 * @see vec2i_normalize_to_vec2_ptr
 *
 * @param v Input vector.
 * @return The resulting vector2.
 */
VEC_API vector2 vec2i_normalize_to_vec2(vector2i v);


/**
 * @brief Computes the dot product of two vectors.
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The dot product (vm_int_t; may overflow).
 */
VEC_API vm_int_t vec2i_dot(vector2i a, vector2i b);

/**
 * @brief Returns the 2D cross product as a scalar (a.x*b.y - a.y*b.x).
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting scalar.
 */
VEC_API vm_int_t vec2i_cross_scalar(vector2i a, vector2i b);

/**
 * @brief Returns the squared Euclidean length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_int_t vec2i_length_squared(vector2i v);

/**
 * @brief Returns the squared Euclidean distance between a and b.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting scalar.
 */
VEC_API vm_int_t vec2i_distance_squared(vector2i a, vector2i b);

/**
 * @brief Returns the Manhattan (L1) length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_int_t vec2i_length_manhattan(vector2i v);

/**
 * @brief Returns the Chebyshev (L-inf) length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_int_t vec2i_length_chebyshev(vector2i v);

/**
 * @brief Returns the smallest component.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_int_t vec2i_min_component(vector2i v);

/**
 * @brief Returns the largest component.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_int_t vec2i_max_component(vector2i v);

/**
 * @brief Returns the sum of all components.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_int_t vec2i_sum(vector2i v);

/**
 * @brief Computes the length (magnitude) of the vector.
 *
 * @param v The input vector.
 * @return The length scalar.
 */
VEC_API vm_float_t vec2i_length(vector2i v);

/**
 * @brief Computes the Euclidean distance between two vectors (treated as points).
 *
 * @param a The first point.
 * @param b The second point.
 * @return The distance scalar.
 */
VEC_API vm_float_t vec2i_distance(vector2i a, vector2i b);

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
VEC_API vm_float_t vec2i_angle(vector2i a, vector2i b);

/**
 * @brief Computes the aspect ratio of the vector (x / y).
 *
 * Returns 0.0f if y == 0.
 *
 * @param v The input vector.
 * @return The aspect ratio scalar.
 */
VEC_API vm_float_t vec2i_aspect_ratio(vector2i v);

/**
 * @brief Returns true if every component is zero.
 *
 * @param v Input vector.
 * @return True if every component is zero.
 */
VEC_API bool vec2i_is_zero(vector2i v);

/* ---- vector3i ------------------------------------------------------------ */

/**
 * @brief Component-wise addition of two vectors.
 *
 * @see vec3i_add_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Result vector.
 */
VEC_API vector3i vec3i_add(vector3i a, vector3i b);

/**
 * @brief Component-wise subtraction of two vectors.
 *
 * @see vec3i_sub_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Result vector.
 */
VEC_API vector3i vec3i_sub(vector3i a, vector3i b);

/**
 * @brief Component-wise multiplication of vector by scalar.
 *
 * @see vec3i_mul_scalar_ptr
 *
 * @param v The vector.
 * @param s The scalar.
 * @return Result vector.
 */
VEC_API vector3i vec3i_mul_scalar(vector3i v, vm_int_t s);

/**
 * @brief Component-wise division of vector by scalar.
 *
 * @see vec3i_div_scalar_ptr
 *
 * @param v The vector.
 * @param s The scalar (non-zero).
 * @return Result vector.
 */
VEC_API vector3i vec3i_div_scalar(vector3i v, vm_int_t s);

/**
 * @brief Component-wise multiplication of two vectors.
 *
 * @see vec3i_mul_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Result vector.
 */
VEC_API vector3i vec3i_mul(vector3i a, vector3i b);

/**
 * @brief Negation of a vector.
 *
 * @see vec3i_neg_ptr
 *
 * @param v The vector.
 * @return The negated vector.
 */
VEC_API vector3i vec3i_neg(vector3i v);

/**
 * @brief Computes the absolute value per component of a `vector3i`.
 *
 * @see vec3i_abs_ptr
 *
 * @param v The vector.
 * @return The absolute vector3i.
 */
VEC_API vector3i vec3i_abs(vector3i v);

/**
 * @brief Returns a zero-initialized `vector3i`.
 *
 * @return A vector3i with all components set to 0.
 */
VEC_API vector3i vec3i_zero(void);

/**
 * @brief Returns a `vector3i` with all components set to 1.
 *
 * @return A vector3i with all components set to 1.
 */
VEC_API vector3i vec3i_one(void);

/**
 * @brief Returns a `vector3i` along the x-axis.
 *
 * @param x The x component value.
 * @return A vector3i with (x, 0, 0).
 */
VEC_API vector3i vec3i_x_axis(vm_int_t x);

/**
 * @brief Returns a `vector3i` along the y-axis.
 *
 * @param y The y component value.
 * @return A vector3i with (0, y, 0).
 */
VEC_API vector3i vec3i_y_axis(vm_int_t y);

/**
 * @brief Returns a `vector3i` along the z-axis.
 *
 * @param z The z component value.
 * @return A vector3i with (0, 0, z).
 */
VEC_API vector3i vec3i_z_axis(vm_int_t z);

/**
 * @brief Returns a `vector3i` for scaling along the x-axis.
 *
 * @param x The x scale factor.
 * @return A vector3i with (x, 1, 1).
 */
VEC_API vector3i vec3i_x_scale(vm_int_t x);

/**
 * @brief Returns a `vector3i` for scaling along the y-axis.
 *
 * @param y The y scale factor.
 * @return A vector3i with (1, y, 1).
 */
VEC_API vector3i vec3i_y_scale(vm_int_t y);

/**
 * @brief Returns a `vector3i` for scaling along the z-axis.
 *
 * @param z The z scale factor.
 * @return A vector3i with (1, 1, z).
 */
VEC_API vector3i vec3i_z_scale(vm_int_t z);

/**
 * @brief Computes the cross-product of two `vector3i`.
 *
 * @see vec3i_cross_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return The cross-product (a × b).
 */
VEC_API vector3i vec3i_cross(vector3i a, vector3i b);

/**
 * @brief Normalizes a `vector3i` to unit length.
 *
 * @see vec3i_normalize_ptr
 *
 * @param v The vector (non-zero).
 * @return The normalized vector.
 */
VEC_API vector3i vec3i_normalize(vector3i v);

/**
 * @brief Computes the component-wise minimum of two `vector3i`.
 *
 * @see vec3i_min_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Per-component min(a, b).
 */
VEC_API vector3i vec3i_min(vector3i a, vector3i b);

/**
 * @brief Computes the component-wise maximum of two `vector3i`.
 *
 * @see vec3i_max_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Per-component max(a, b).
 */
VEC_API vector3i vec3i_max(vector3i a, vector3i b);

/**
 * @brief Computes the sign per component of a `vector3i` (-1, 0, or 1).
 *
 * @see vec3i_sign_ptr
 *
 * @param v The vector.
 * @return The sign vector.
 */
VEC_API vector3i vec3i_sign(vector3i v);

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
VEC_API vector3i vec3i_lerp(vector3i a, vector3i b, vm_float_t t);

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
VEC_API vector3i vec3i_clamp(vector3i v, vector3i min, vector3i max);

/**
 * @brief Divides two vectors component-wise.
 *
 * @see vec3i_div_ptr
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector3i.
 */
VEC_API vector3i vec3i_div(vector3i a, vector3i b);

/**
 * @brief Adds a scalar to each component.
 *
 * @see vec3i_add_scalar_ptr
 *
 * @param v Input vector.
 * @param s Scalar value.
 * @return The resulting vector3i.
 */
VEC_API vector3i vec3i_add_scalar(vector3i v, vm_int_t s);

/**
 * @brief Subtracts a scalar from each component.
 *
 * @see vec3i_sub_scalar_ptr
 *
 * @param v Input vector.
 * @param s Scalar value.
 * @return The resulting vector3i.
 */
VEC_API vector3i vec3i_sub_scalar(vector3i v, vm_int_t s);

/**
 * @brief Component-wise floor modulo of a by b.
 *
 * @see vec3i_mod_ptr
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector3i.
 */
VEC_API vector3i vec3i_mod(vector3i a, vector3i b);

/**
 * @brief Component-wise floored division of a by b.
 *
 * @see vec3i_div_floor_ptr
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector3i.
 */
VEC_API vector3i vec3i_div_floor(vector3i a, vector3i b);

/**
 * @brief Wraps each component of v into [0, period).
 *
 * @see vec3i_wrap_ptr
 *
 * @param v Input vector.
 * @param period Wrap period per component.
 * @return The resulting vector3i.
 */
VEC_API vector3i vec3i_wrap(vector3i v, vector3i period);

/**
 * @brief Returns a vector with every component set to s.
 *
 * @see vec3i_splat_ptr
 *
 * @param s Scalar value.
 * @return The resulting vector3i.
 */
VEC_API vector3i vec3i_splat(vm_int_t s);

/**
 * @brief Builds a vector3i from a vector2i and z.
 *
 * @see vec3i_from_vec2i_ptr
 *
 * @param v Input vector.
 * @param z Z component.
 * @return The resulting vector3i.
 */
VEC_API vector3i vec3i_from_vec2i(vector2i v, vm_int_t z);

/**
 * @brief Returns the x and y components as a 2D vector.
 *
 * @see vec3i_xy_ptr
 *
 * @param v Input vector.
 * @return The resulting vector2i.
 */
VEC_API vector2i vec3i_xy(vector3i v);

/**
 * @brief Converts to a unit-length vector3.
 *
 * @see vec3i_normalize_to_vec3_ptr
 *
 * @param v Input vector.
 * @return The resulting vector3.
 */
VEC_API vector3 vec3i_normalize_to_vec3(vector3i v);

/**
 * @brief Computes the dot product of two `vector3i`.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return The dot product (a.x * b.x + a.y * b.y + a.z * b.z).
 */
VEC_API vm_int_t vec3i_dot(vector3i a, vector3i b);

/**
 * @brief Returns the squared Euclidean length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_int_t vec3i_length_squared(vector3i v);

/**
 * @brief Returns the squared Euclidean distance between a and b.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting scalar.
 */
VEC_API vm_int_t vec3i_distance_squared(vector3i a, vector3i b);

/**
 * @brief Returns the Manhattan (L1) length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_int_t vec3i_length_manhattan(vector3i v);

/**
 * @brief Returns the Chebyshev (L-inf) length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_int_t vec3i_length_chebyshev(vector3i v);

/**
 * @brief Returns the smallest component.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_int_t vec3i_min_component(vector3i v);

/**
 * @brief Returns the largest component.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_int_t vec3i_max_component(vector3i v);

/**
 * @brief Returns the sum of all components.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_int_t vec3i_sum(vector3i v);

/**
 * @brief Computes the length (magnitude) of a `vector3i`.
 *
 * @param v The vector.
 * @return The Euclidean length of v.
 */
VEC_API vm_float_t vec3i_length(vector3i v);

/**
 * @brief Computes the Euclidean distance between two `vector3i`.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return The distance between a and b.
 */
VEC_API vm_float_t vec3i_distance(vector3i a, vector3i b);

/**
 * @brief Computes the angle between two non-zero `vector3i` in radians.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return The angle between the directions of a and b.
 */
VEC_API vm_float_t vec3i_angle(vector3i a, vector3i b);

/**
 * @brief Returns true if every component is zero.
 *
 * @param v Input vector.
 * @return True if every component is zero.
 */
VEC_API bool vec3i_is_zero(vector3i v);

/* ---- vector4i ------------------------------------------------------------ */

/**
 * @brief Component-wise addition of two vectors.
 *
 * @see vec4i_add_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Result vector.
 */
VEC_API vector4i vec4i_add(vector4i a, vector4i b);

/**
 * @brief Component-wise subtraction of two vectors.
 *
 * @see vec4i_sub_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Result vector.
 */
VEC_API vector4i vec4i_sub(vector4i a, vector4i b);

/**
 * @brief Component-wise multiplication of vector by scalar.
 *
 * @see vec4i_mul_scalar_ptr
 *
 * @param v The vector.
 * @param s The scalar.
 * @return Result vector.
 */
VEC_API vector4i vec4i_mul_scalar(vector4i v, vm_int_t s);

/**
 * @brief Component-wise division of vector by scalar.
 *
 * @see vec4i_div_scalar_ptr
 *
 * @param v The vector.
 * @param s The scalar (non-zero).
 * @return Result vector.
 */
VEC_API vector4i vec4i_div_scalar(vector4i v, vm_int_t s);

/**
 * @brief Component-wise multiplication of two vectors.
 *
 * @see vec4i_mul_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Result vector.
 */
VEC_API vector4i vec4i_mul(vector4i a, vector4i b);

/**
 * @brief Negation of a vector.
 *
 * @see vec4i_neg_ptr
 *
 * @param v The vector.
 * @return The negated vector.
 */
VEC_API vector4i vec4i_neg(vector4i v);

/**
 * @brief Computes the absolute value per component of a `vector4i`.
 *
 * @see vec4i_abs_ptr
 *
 * @param v The vector.
 * @return The absolute `vector4i`.
 */
VEC_API vector4i vec4i_abs(vector4i v);


/**
 * @brief Returns a zero-initialized `vector4i`.
 *
 * @return A `vector4i` with all components set to 0.
 */
VEC_API vector4i vec4i_zero(void);

/**
 * @brief Returns a `vector4i` with all components set to 1.
 *
 * @return A `vector4i` with all components set to 1.
 */
VEC_API vector4i vec4i_one(void);

/**
 * @brief Returns a `vector4i` along the x-axis.
 *
 * @param x The x component value.
 * @return A `vector4i` with (x, 0, 0, 0).
 */
VEC_API vector4i vec4i_x_axis(vm_int_t x);

/**
 * @brief Returns a `vector4i` along the y-axis.
 *
 * @param y The y component value.
 * @return A `vector4i` with (0, y, 0, 0).
 */
VEC_API vector4i vec4i_y_axis(vm_int_t y);

/**
 * @brief Returns a `vector4i` along the z-axis.
 *
 * @param z The z component value.
 * @return A `vector4i` with (0, 0, z, 0).
 */
VEC_API vector4i vec4i_z_axis(vm_int_t z);

/**
 * @brief Returns a `vector4i` along the w-axis.
 *
 * @param w The w component value.
 * @return A `vector4i` with (0, 0, 0, w).
 */
VEC_API vector4i vec4i_w_axis(vm_int_t w);

/**
 * @brief Returns a `vector4i` for scaling along the x-axis.
 *
 * @param x The x scale factor.
 * @return A `vector4i` with (x, 1, 1, 1).
 */
VEC_API vector4i vec4i_x_scale(vm_int_t x);

/**
 * @brief Returns a `vector4i` for scaling along the y-axis.
 *
 * @param y The y scale factor.
 * @return A `vector4i` with (1, y, 1, 1).
 */
VEC_API vector4i vec4i_y_scale(vm_int_t y);

/**
 * @brief Returns a `vector4i` for scaling along the z-axis.
 *
 * @param z The z scale factor.
 * @return A `vector4i` with (1, 1, z, 1).
 */
VEC_API vector4i vec4i_z_scale(vm_int_t z);

/**
 * @brief Returns a `vector4i` for scaling along the w-axis.
 *
 * @param w The w scale factor.
 * @return A `vector4i` with (1, 1, 1, w).
 */
VEC_API vector4i vec4i_w_scale(vm_int_t w);

/**
 * @brief Normalizes a `vector4i` to unit length.
 *
 * @see vec4i_normalize_ptr
 *
 * @param v The vector (non-zero).
 * @return The normalized vector.
 */
VEC_API vector4i vec4i_normalize(vector4i v);

/**
 * @brief Computes the component-wise minimum of two `vector4i`.
 *
 * @see vec4i_min_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Per-component min(a, b).
 */
VEC_API vector4i vec4i_min(vector4i a, vector4i b);

/**
 * @brief Computes the component-wise maximum of two vector4i.
 *
 * @see vec4i_max_ptr
 *
 * @param a First vector.
 * @param b Second vector.
 * @return Per-component max(a, b).
 */
VEC_API vector4i vec4i_max(vector4i a, vector4i b);

/**
 * @brief Computes the sign per component of a `vector4i` (-1, 0, or 1).
 *
 * @see vec4i_sign_ptr
 *
 * @param v The vector.
 * @return The sign vector.
 */
VEC_API vector4i vec4i_sign(vector4i v);

/**
 * @brief Linearly interpolates between two `vector4i`.
 *
 * @see vec4i_lerp_ptr
 *
 * @param a Start vector.
 * @param b End vector.
 * @param t Interpolation factor [0, 1].
 * @return The interpolated vector (a * (1 - t) + b * t).
 */
VEC_API vector4i vec4i_lerp(vector4i a, vector4i b, vm_float_t t);

/**
 * @brief Clamps each component of the input vector to the range defined by the
 * minimum and maximum vectors.
 *
 * @see vec4i_clamp_ptr
 *
 * @param v The input vector.
 * @param min The minimum value for each component.
 * @param max The maximum value for each component.
 * @return A new `vector4i` with each component clamped to [min, max].
 */
VEC_API vector4i vec4i_clamp(vector4i v, vector4i min, vector4i max);

/**
 * @brief Divides two vectors component-wise.
 *
 * @see vec4i_div_ptr
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector4i.
 */
VEC_API vector4i vec4i_div(vector4i a, vector4i b);

/**
 * @brief Adds a scalar to each component.
 *
 * @see vec4i_add_scalar_ptr
 *
 * @param v Input vector.
 * @param s Scalar value.
 * @return The resulting vector4i.
 */
VEC_API vector4i vec4i_add_scalar(vector4i v, vm_int_t s);

/**
 * @brief Subtracts a scalar from each component.
 *
 * @see vec4i_sub_scalar_ptr
 *
 * @param v Input vector.
 * @param s Scalar value.
 * @return The resulting vector4i.
 */
VEC_API vector4i vec4i_sub_scalar(vector4i v, vm_int_t s);

/**
 * @brief Component-wise floor modulo of a by b.
 *
 * @see vec4i_mod_ptr
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector4i.
 */
VEC_API vector4i vec4i_mod(vector4i a, vector4i b);

/**
 * @brief Component-wise floored division of a by b.
 *
 * @see vec4i_div_floor_ptr
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting vector4i.
 */
VEC_API vector4i vec4i_div_floor(vector4i a, vector4i b);

/**
 * @brief Wraps each component of v into [0, period).
 *
 * @see vec4i_wrap_ptr
 *
 * @param v Input vector.
 * @param period Wrap period per component.
 * @return The resulting vector4i.
 */
VEC_API vector4i vec4i_wrap(vector4i v, vector4i period);

/**
 * @brief Returns a vector with every component set to s.
 *
 * @see vec4i_splat_ptr
 *
 * @param s Scalar value.
 * @return The resulting vector4i.
 */
VEC_API vector4i vec4i_splat(vm_int_t s);

/**
 * @brief Converts a `vector4i` to a `vector3i` (discards the w component).
 *
 * @param v The source `vector4i`.
 * @return A `vector3i` {v.x, v.y, v.z}.
 */
VEC_API vector3i vec4i_to_vec3i(vector4i v);

/**
 * @brief Computes the dot product of two `vector4i`.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return The dot product.
 */
VEC_API vm_int_t vec4i_dot(vector4i a, vector4i b);

/**
 * @brief Returns the squared Euclidean length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_int_t vec4i_length_squared(vector4i v);

/**
 * @brief Returns the squared Euclidean distance between a and b.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return The resulting scalar.
 */
VEC_API vm_int_t vec4i_distance_squared(vector4i a, vector4i b);

/**
 * @brief Returns the Manhattan (L1) length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_int_t vec4i_length_manhattan(vector4i v);

/**
 * @brief Returns the Chebyshev (L-inf) length.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_int_t vec4i_length_chebyshev(vector4i v);

/**
 * @brief Returns the smallest component.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_int_t vec4i_min_component(vector4i v);

/**
 * @brief Returns the largest component.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_int_t vec4i_max_component(vector4i v);

/**
 * @brief Returns the sum of all components.
 *
 * @param v Input vector.
 * @return The resulting scalar.
 */
VEC_API vm_int_t vec4i_sum(vector4i v);

/**
 * @brief Computes the length (magnitude) of a `vector4i`.
 *
 * @param v The vector.
 * @return The Euclidean length of v.
 */
VEC_API vm_float_t vec4i_length(vector4i v);

/**
 * @brief Computes the Euclidean distance between two `vector4i`.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return The distance between a and b.
 */
VEC_API vm_float_t vec4i_distance(vector4i a, vector4i b);

/**
 * @brief Returns true if every component is zero.
 *
 * @param v Input vector.
 * @return True if every component is zero.
 */
VEC_API bool vec4i_is_zero(vector4i v);

/*******************************************************************************
 * Integer vector (pointer-based) performance functions
 ******************************************************************************/

/* ---- vector2i pointer-based operations ----------------------------------- */

/**
 * @brief Adds vectors a and b component-wise, storing the result in res.
 *
 * @param res Pointer to result vector2i.
 * @param a Pointer to first vector2i.
 * @param b Pointer to second vector2i.
 */
VEC_API void vec2i_add_ptr(vector2i *res, const vector2i *a, const vector2i *b);

/**
 * @brief Subtracts vector b from vector a component-wise, storing the result in res.
 *
 * @param res Pointer to result vector2i.
 * @param a Pointer to first vector2i.
 * @param b Pointer to second vector2i.
 */
VEC_API void vec2i_sub_ptr(vector2i *res, const vector2i *a, const vector2i *b);

/**
 * @brief Multiplies vector v by scalar s component-wise, storing the result in res.
 *
 * @param res Pointer to result vector2i.
 * @param v Pointer to vector2i.
 * @param s Scalar.
 */
VEC_API void vec2i_mul_scalar_ptr(vector2i *res, const vector2i *v, vm_int_t s);

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
VEC_API void vec2i_div_scalar_ptr(vector2i *res, const vector2i *v, vm_int_t s);

/**
 * @brief Multiplies vectors a and b component-wise (Hadamard product), storing
 * the result in res.
 *
 * @param res Pointer to result vector2i.
 * @param a Pointer to first vector2i.
 * @param b Pointer to second vector2i.
 */
VEC_API void vec2i_mul_ptr(vector2i *res, const vector2i *a, const vector2i *b);

/**
 * @brief Negates the components of vector v, storing the result in res.
 *
 * @param res Pointer to result vector2i.
 * @param v Pointer to vector2i.
 */
VEC_API void vec2i_neg_ptr(vector2i *res, const vector2i *v);

/**
 * @brief Computes the absolute values of the components of vector v (using int
 * abs), storing the result in res.
 *
 * @param res Pointer to result vector2i.
 * @param v Pointer to vector2i.
 */
VEC_API void vec2i_abs_ptr(vector2i *res, const vector2i *v);

/**
 * @brief Normalizes vector v to approximate unit length (float length
 * computation, truncated to int), storing the result in res.
 *
 * Copies v unchanged if the length is zero.
 *
 * @param res Pointer to result vector2i.
 * @param v Pointer to vector2i.
 */
VEC_API void vec2i_normalize_ptr(vector2i *res, const vector2i *v);

/**
 * @brief Computes the component-wise minimum of vectors a and b, storing the
 * result in res.
 *
 * @param res Pointer to result vector2i.
 * @param a Pointer to first vector2i.
 * @param b Pointer to second vector2i.
 */
VEC_API void vec2i_min_ptr(vector2i *res, const vector2i *a, const vector2i *b);

/**
 * @brief Computes the component-wise maximum of vectors a and b, storing the
 * result in res.
 *
 * @param res Pointer to result vector2i.
 * @param a Pointer to first vector2i.
 * @param b Pointer to second vector2i.
 */
VEC_API void vec2i_max_ptr(vector2i *res, const vector2i *a, const vector2i *b);

/**
 * @brief Sets each component of res to the sign of the corresponding component
 * in v (+1, -1, or 0).
 *
 * @param res Pointer to result vector2i.
 * @param v Pointer to vector2i.
 */
VEC_API void vec2i_sign_ptr(vector2i *res, const vector2i *v);

/**
 * @brief Computes the perpendicular vector to v (90 degrees counterclockwise
 * rotation), storing the result in res.
 *
 * @param res Pointer to result vector2i.
 * @param v Pointer to vector2i.
 */
VEC_API void vec2i_perpendicular_ptr(vector2i *res, const vector2i *v);

/**
 * @brief Computes the 2D cross-product of a and b, storing the scalar value in
 * res->x and 0 in res->y.
 *
 * @param res Pointer to result vector2i.
 * @param a Pointer to first vector2i.
 * @param b Pointer to second vector2i.
 */
VEC_API void vec2i_cross_ptr(vector2i *res, const vector2i *a, const vector2i *b);

/**
 * @brief Linearly interpolates from a to b by t.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 * @param t Interpolation factor.
 */
VEC_API void vec2i_lerp_ptr(vector2i *res, const vector2i *a, const vector2i *b, vm_float_t t);

/**
 * @brief Clamps each component between min and max.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param min Lower bound.
 * @param max Upper bound.
 */
VEC_API void vec2i_clamp_ptr(vector2i *res, const vector2i *v, const vector2i *min, const vector2i *max);

/**
 * @brief Divides two vectors component-wise.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
VEC_API void vec2i_div_ptr(vector2i *res, const vector2i *a, const vector2i *b);

/**
 * @brief Adds a scalar to each component.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param s Scalar value.
 */
VEC_API void vec2i_add_scalar_ptr(vector2i *res, const vector2i *v, vm_int_t s);

/**
 * @brief Subtracts a scalar from each component.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param s Scalar value.
 */
VEC_API void vec2i_sub_scalar_ptr(vector2i *res, const vector2i *v, vm_int_t s);

/**
 * @brief Component-wise floor modulo of a by b.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
VEC_API void vec2i_mod_ptr(vector2i *res, const vector2i *a, const vector2i *b);

/**
 * @brief Component-wise floored division of a by b.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
VEC_API void vec2i_div_floor_ptr(vector2i *res, const vector2i *a, const vector2i *b);

/**
 * @brief Wraps each component of v into [0, period).
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param period Wrap period per component.
 */
VEC_API void vec2i_wrap_ptr(vector2i *res, const vector2i *v, const vector2i *period);

/**
 * @brief Converts a `vector4i` to a `vector3i` by dropping w.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param z Z component.
 */
VEC_API void vec2i_to_vec3i_ptr(vector3i *res, const vector2i *v, vm_int_t z);

/**
 * @brief Converts to a unit-length vector2.
 *
 * @param res Output vector.
 * @param v Input vector.
 */
VEC_API void vec2i_normalize_to_vec2_ptr(vector2 *res, const vector2i *v);

/* ---- vector3i pointer-based operations ----------------------------------- */

/**
 * @brief Computes the component-wise sum of two `vector3i`.
 *
 * @param res Pointer to the vector that will store the result.
 * @param a Pointer to the first vector.
 * @param b Pointer to the second vector.
 */
VEC_API void vec3i_add_ptr(vector3i *res, const vector3i *a, const vector3i *b);

/**
 * @brief Computes the component-wise difference of two `vector3i` (a minus b).
 *
 * @param res Pointer to the vector that will store the result.
 * @param a Pointer to the first vector.
 * @param b Pointer to the second vector.
 */
VEC_API void vec3i_sub_ptr(vector3i *res, const vector3i *a, const vector3i *b);

/**
 * @brief Scales a `vector3i` by an integer scalar.
 *
 * @param res Pointer to the vector that will store the result.
 * @param v Pointer to the input vector.
 * @param s The scalar multiplier.
 */
VEC_API void vec3i_mul_scalar_ptr(vector3i *res, const vector3i *v, vm_int_t s);

/**
 * @brief Scales a `vector3i` by the inverse of an integer scalar.
 *
 * If the scalar is zero, stores the zero vector.
 *
 * @param res Pointer to the vector that will store the result.
 * @param v Pointer to the input vector.
 * @param s The scalar divisor.
 */
VEC_API void vec3i_div_scalar_ptr(vector3i *res, const vector3i *v, vm_int_t s);

/**
 * @brief Computes the component-wise product (Hadamard) of two `vector3i`.
 *
 * @param res Pointer to the vector that will store the result.
 * @param a Pointer to the first vector.
 * @param b Pointer to the second vector.
 */
VEC_API void vec3i_mul_ptr(vector3i *res, const vector3i *a, const vector3i *b);

/**
 * @brief Negates a `vector3i` (multiplies by -1).
 *
 * @param res Pointer to the vector that will store the result.
 * @param v Pointer to the input vector.
 */
VEC_API void vec3i_neg_ptr(vector3i *res, const vector3i *v);

/**
 * @brief Computes the absolute value of each component of a `vector3i`.
 *
 * @param res Pointer to the vector that will store the result.
 * @param v Pointer to the input vector.
 */
VEC_API void vec3i_abs_ptr(vector3i *res, const vector3i *v);

/**
 * @brief Normalizes a `vector3i` to approximate unit length.
 *
 * If the vector length is zero, copies the input vector.
 *
 * @param res Pointer to the vector that will store the result.
 * @param v Pointer to the input vector.
 */
VEC_API void vec3i_normalize_ptr(vector3i *res, const vector3i *v);

/**
 * @brief Computes the cross-product of two `vector3i`.
 *
 * @param res Pointer to the vector that will store the result.
 * @param a Pointer to the first vector.
 * @param b Pointer to the second vector.
 */
VEC_API void vec3i_cross_ptr(vector3i *res, const vector3i *a, const vector3i *b);

/**
 * @brief Computes the component-wise minimum of two `vector3i`.
 *
 * @param res Pointer to the vector that will store the result.
 * @param a Pointer to the first vector.
 * @param b Pointer to the second vector.
 */
VEC_API void vec3i_min_ptr(vector3i *res, const vector3i *a, const vector3i *b);

/**
 * @brief Computes the component-wise maximum of two `vector3i`.
 *
 * @param res Pointer to the vector that will store the result.
 * @param a Pointer to the first vector.
 * @param b Pointer to the second vector.
 */
VEC_API void vec3i_max_ptr(vector3i *res, const vector3i *a, const vector3i *b);

/**
 * @brief Computes the sign of each component of a `vector3i` (-1, 0, or 1).
 *
 * @param res Pointer to the vector that will store the result.
 * @param v Pointer to the input vector.
 */
VEC_API void vec3i_sign_ptr(vector3i *res, const vector3i *v);

/**
 * @brief Performs linear interpolation between two `vector3i`.
 *
 * @param res Pointer to the vector that will store the result.
 * @param a Pointer to the start vector.
 * @param b Pointer to the end vector.
 * @param t Interpolation factor (typically between 0.0 and 1.0).
 */
VEC_API void vec3i_lerp_ptr(vector3i *res, const vector3i *a, const vector3i *b, vm_float_t t);

/**
 * @brief Clamps each component of a `vector3i` between corresponding min and
 * max values.
 *
 * @param res Pointer to the vector that will store the result.
 * @param v Pointer to the input vector.
 * @param min Pointer to the minimum bounds vector.
 * @param max Pointer to the maximum bounds vector.
 */
VEC_API void vec3i_clamp_ptr(vector3i *res, const vector3i *v, const vector3i *min, const vector3i *max);

/**
 * @brief Divides two vectors component-wise.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
VEC_API void vec3i_div_ptr(vector3i *res, const vector3i *a, const vector3i *b);

/**
 * @brief Adds a scalar to each component.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param s Scalar value.
 */
VEC_API void vec3i_add_scalar_ptr(vector3i *res, const vector3i *v, vm_int_t s);

/**
 * @brief Subtracts a scalar from each component.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param s Scalar value.
 */
VEC_API void vec3i_sub_scalar_ptr(vector3i *res, const vector3i *v, vm_int_t s);

/**
 * @brief Component-wise floor modulo of a by b.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
VEC_API void vec3i_mod_ptr(vector3i *res, const vector3i *a, const vector3i *b);

/**
 * @brief Component-wise floored division of a by b.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
VEC_API void vec3i_div_floor_ptr(vector3i *res, const vector3i *a, const vector3i *b);

/**
 * @brief Wraps each component of v into [0, period).
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param period Wrap period per component.
 */
VEC_API void vec3i_wrap_ptr(vector3i *res, const vector3i *v, const vector3i *period);

/**
 * @brief Builds a vector3i from a vector2i and z.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param z Z component.
 */
VEC_API void vec3i_from_vec2i_ptr(vector3i *res, const vector2i *v, vm_int_t z);

/**
 * @brief Returns the x and y components as a 2D vector.
 *
 * @param res Output vector.
 * @param v Input vector.
 */
VEC_API void vec3i_xy_ptr(vector2i *res, const vector3i *v);

/**
 * @brief Converts to a unit-length vector3.
 *
 * @param res Output vector.
 * @param v Input vector.
 */
VEC_API void vec3i_normalize_to_vec3_ptr(vector3 *res, const vector3i *v);


// vector4i pointer-based operations
/**
 * @brief Adds the components of two `vector4i` and stores the result in res.
 *
 * @param res Pointer to result vector
 * @param a Pointer to first vector
 * @param b Pointer to second vector
 */
VEC_API void vec4i_add_ptr(vector4i *res, const vector4i *a, const vector4i *b);

/**
 * @brief Subtracts the components of the second vector from the first `vector4i`
 * and stores the result in res.
 *
 * @param res Pointer to result vector
 * @param a Pointer to first vector
 * @param b Pointer to second vector
 */
VEC_API void vec4i_sub_ptr(vector4i *res, const vector4i *a, const vector4i *b);

/**
 * @brief Multiplies the components of a `vector4i` by a scalar and stores
 * the result in res.
 *
 * @param res Pointer to result vector
 * @param v Pointer to input vector
 * @param s Scalar value
 */
VEC_API void vec4i_mul_scalar_ptr(vector4i *res, const vector4i *v, vm_int_t s);

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
VEC_API void vec4i_div_scalar_ptr(vector4i *res, const vector4i *v, vm_int_t s);

/**
 * @brief Multiplies the components of two `vector4i` and stores the result in res.
 *
 * @param res Pointer to result vector
 * @param a Pointer to first vector
 * @param b Pointer to second vector
 */
VEC_API void vec4i_mul_ptr(vector4i *res, const vector4i *a, const vector4i *b);

/**
 * @brief Negates the components of a `vector4i` and stores the result in res.
 *
 * @param res Pointer to result vector
 * @param v Pointer to input vector
 */
VEC_API void vec4i_neg_ptr(vector4i *res, const vector4i *v);

/**
 * @brief Computes the absolute value of each component of a `vector4i` and
 * stores the result in res.
 *
 * @param res Pointer to result vector
 * @param v Pointer to input vector
 */
VEC_API void vec4i_abs_ptr(vector4i *res, const vector4i *v);

/**
 * @brief Normalizes a `vector4i` by dividing its components by the vector
 * length and stores the result in res.
 *
 * If the length is zero, copies the input vector to res.
 *
 * @param res Pointer to result vector
 * @param v Pointer to input vector
 */
VEC_API void vec4i_normalize_ptr(vector4i *res, const vector4i *v);

/**
 * @brief Takes the component-wise minimum of two `vector4i` and stores the
 * result in res.
 *
 * @param res Pointer to result vector
 * @param a Pointer to first vector
 * @param b Pointer to second vector
 */
VEC_API void vec4i_min_ptr(vector4i *res, const vector4i *a, const vector4i *b);

/**
 * @brief Computes the component-wise maximum of two `vector4i` and stores the
 * result in res.
 *
 * @param res Pointer to result vector
 * @param a Pointer to first vector
 * @param b Pointer to second vector
 */
VEC_API void vec4i_max_ptr(vector4i *res, const vector4i *a, const vector4i *b);

/**
 * @brief Computes the sign (-1, 0, or 1) of each component of a `vector4i`
 * and stores the result in res.
 *
 * @param res Pointer to result vector
 * @param v Pointer to input vector
 */
VEC_API void vec4i_sign_ptr(vector4i *res, const vector4i *v);

/**
 * @brief Linearly interpolates between two `vector4i` using an interpolation
 * factor t (clamped to [0,1]) and stores the result in res.
 *
 * @param res Pointer to result vector
 * @param a Pointer to first vector
 * @param b Pointer to second vector
 * @param t Interpolation factor
 */
VEC_API void vec4i_lerp_ptr(vector4i *res, const vector4i *a, const vector4i *b, vm_float_t t);

/**
 * @brief Clamps the components of a `vector4i` to the range [min, max] and
 * stores the result in res.
 *
 * @param res Pointer to result vector
 * @param v Pointer to input vector
 * @param min Pointer to minimum bounds vector
 * @param max Pointer to maximum bounds vector
 */
VEC_API void vec4i_clamp_ptr(vector4i *res, const vector4i *v, const vector4i *min, const vector4i *max);

/**
 * @brief Divides two vectors component-wise.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
VEC_API void vec4i_div_ptr(vector4i *res, const vector4i *a, const vector4i *b);

/**
 * @brief Adds a scalar to each component.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param s Scalar value.
 */
VEC_API void vec4i_add_scalar_ptr(vector4i *res, const vector4i *v, vm_int_t s);

/**
 * @brief Subtracts a scalar from each component.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param s Scalar value.
 */
VEC_API void vec4i_sub_scalar_ptr(vector4i *res, const vector4i *v, vm_int_t s);

/**
 * @brief Component-wise floor modulo of a by b.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
VEC_API void vec4i_mod_ptr(vector4i *res, const vector4i *a, const vector4i *b);

/**
 * @brief Component-wise floored division of a by b.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
VEC_API void vec4i_div_floor_ptr(vector4i *res, const vector4i *a, const vector4i *b);

/**
 * @brief Wraps each component of v into [0, period).
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param period Wrap period per component.
 */
VEC_API void vec4i_wrap_ptr(vector4i *res, const vector4i *v, const vector4i *period);

/**
 * @brief Copies the x, y, z components of a `vector4i` and stores the result
 * in a `vector3i`.
 *
 * @param res Pointer to result `vector3i`
 * @param v Pointer to source `vector4i`
 */
VEC_API void vec4i_to_vec3i_ptr(vector3i *res, const vector4i *v);

/**
 * @brief Converts a `vector2i` to a `vector2`.
 *
 * @param v Input integer vector.
 * @return The resulting vector2.
 */
VEC_API vector2 vec2_from(const vector2i *v);

/**
 * @brief Converts a `vector2` to a `vector2i` by truncation.
 *
 * @param v Input vector.
 * @return The resulting vector2i.
 */
VEC_API vector2i vec2i_from(const vector2 *v);

/**
 * @brief Converts a `vector2` to a `vector2i` by flooring each component.
 *
 * @param v Input vector.
 * @return The resulting vector2i.
 */
VEC_API vector2i vec2i_from_floored(const vector2 *v);

/**
 * @brief Converts a `vector2` to a `vector2i` by rounding each component.
 *
 * @param v Input vector.
 * @return The resulting vector2i.
 */
VEC_API vector2i vec2i_from_rounded(const vector2 *v);

/**
 * @brief Converts a vector3i to a vector3.
 *
 * @param v Input integer vector.
 * @return The resulting vector3.
 */
VEC_API vector3 vec3_from(const vector3i *v);

/**
 * @brief Converts a `vector3` to a `vector3i` by truncation.
 *
 * @param v Input vector.
 * @return The resulting vector3i.
 */
VEC_API vector3i vec3i_from(const vector3 *v);

/**
 * @brief Converts a `vector3` to a `vector3i` by flooring each component.
 *
 * @param v Input vector.
 * @return The resulting vector3i.
 */
VEC_API vector3i vec3i_from_floored(const vector3 *v);

/**
 * @brief Converts a `vector3` to a `vector3i` by rounding each component.
 *
 * @param v Input vector.
 * @return The resulting vector3i.
 */
VEC_API vector3i vec3i_from_rounded(const vector3 *v);

/**
 * @brief Converts degrees to radians.
 *
 * @param degrees Angle in degrees.
 * @return The angle in radians.
 */
VEC_API vm_float_t deg_to_rad(vm_float_t degrees);

/**
 * @brief Converts radians to degrees.
 *
 * @param radians Angle in radians.
 * @return The angle in degrees.
 */
VEC_API vm_float_t rad_to_deg(vm_float_t radians);

/**
 * @brief Constructs a vector2 from x and y.
 *
 * @param x X component.
 * @param y Y component.
 * @return The resulting vector2.
 */
VEC_API vector2 vec2(vm_float_t x, vm_float_t y);

/**
 * @brief Copies src into dest.
 *
 * @param dest Destination vector.
 * @param src Source vector.
 */
VEC_API void vec2_assign(vector2 *dest, const vector2 *src);

/**
 * @brief Assigns x and y to dest.
 *
 * @param dest Destination vector.
 * @param x X component.
 * @param y Y component.
 */
VEC_API void vec2_assign_xy(vector2 *dest, vm_float_t x, vm_float_t y);

/**
 * @brief Adds src to dest in place.
 *
 * @param dest Destination vector.
 * @param src Vector to add.
 */
VEC_API void vec2_add_assign(vector2 *dest, const vector2 *src);

/**
 * @brief Constructs a `vector3` from x, y, and z.
 *
 * @param x X component.
 * @param y Y component.
 * @param z Z component.
 * @return The resulting vector3.
 */
VEC_API vector3 vec3(vm_float_t x, vm_float_t y, vm_float_t z);

/**
 * @brief Copies src into dest.
 *
 * @param dest Destination vector.
 * @param src Source vector.
 */
VEC_API void vec3_assign(vector3 *dest, const vector3 *src);

/**
 * @brief Assigns x, y, and z to dest.
 *
 * @param dest Destination vector.
 * @param x X component.
 * @param y Y component.
 * @param z Z component.
 */
VEC_API void vec3_assign_xyz(vector3 *dest, vm_float_t x, vm_float_t y, vm_float_t z);

/**
 * @brief Adds src to dest in place.
 *
 * @param dest Destination vector.
 * @param src Vector to add.
 */
VEC_API void vec3_add_assign(vector3 *dest, const vector3 *src);

/**
 * @brief Constructs a `vector2i` from x and y.
 *
 * @param x X component.
 * @param y Y component.
 * @return The resulting vector2i.
 */
VEC_API vector2i vec2i(vm_int_t x, vm_int_t y);

/**
 * @brief Copies src into dest.
 *
 * @param dest Destination vector.
 * @param src Source vector.
 */
VEC_API void vec2i_assign(vector2i *dest, const vector2i *src);

/**
 * @brief Assigns x and y to dest.
 *
 * @param dest Destination vector.
 * @param x X component.
 * @param y Y component.
 */
VEC_API void vec2i_assign_xy(vector2i *dest, vm_int_t x, vm_int_t y);

/**
 * @brief Adds src to dest in place.
 *
 * @param dest Destination vector.
 * @param src Vector to add.
 */
VEC_API void vec2i_add_assign(vector2i *dest, const vector2i *src);

/**
 * @brief Constructs a `vector3i` from x, y, and z.
 *
 * @param x X component.
 * @param y Y component.
 * @param z Z component.
 * @return The resulting vector3i.
 */
VEC_API vector3i vec3i(vm_int_t x, vm_int_t y, vm_int_t z);

/**
 * @brief Copies src into dest.
 *
 * @param dest Destination vector.
 * @param src Source vector.
 */
VEC_API void vec3i_assign(vector3i *dest, const vector3i *src);

/**
 * @brief Assigns x, y, and z to dest.
 *
 * @param dest Destination vector.
 * @param x X component.
 * @param y Y component.
 * @param z Z component.
 */
VEC_API void vec3i_assign_xyz(vector3i *dest, vm_int_t x, vm_int_t y, vm_int_t z);

/**
 * @brief Adds src to dest in place.
 *
 * @param dest Destination vector.
 * @param src Vector to add.
 */
VEC_API void vec3i_add_assign(vector3i *dest, const vector3i *src);

/*******************************************************************************
 * Vector equality
 ******************************************************************************/

/**
 * @brief Returns true if two vectors are equal within `VECMAT_EPSILON`.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return True if `a` and `b` are approximately equal.
 */
VEC_API bool vec2_eq(vector2 a, vector2 b);

/**
 * @brief Returns true if two vectors are equal within `VECMAT_EPSILON`.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return True if a and b are approximately equal.
 */
VEC_API bool vec3_eq(vector3 a, vector3 b);

/**
 * @brief Returns true if two vectors are equal within `VECMAT_EPSILON`.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return True if a and b are approximately equal.
 */
VEC_API bool vec4_eq(vector4 a, vector4 b);

/**
 * @brief Returns true if two vectors are exactly equal.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return True if a and b are equal.
 */
VEC_API bool vec2i_eq(vector2i a, vector2i b);

/**
 * @brief Returns true if two vectors are exactly equal.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return True if a and b are equal.
 */
VEC_API bool vec3i_eq(vector3i a, vector3i b);

/**
 * @brief Returns true if two vectors are exactly equal.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return True if a and b are equal.
 */
VEC_API bool vec4i_eq(vector4i a, vector4i b);

#endif //VECMAT_VEC_H
