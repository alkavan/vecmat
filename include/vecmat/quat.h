// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_QUAT_H
#define VECMAT_QUAT_H

/**
 * @file
 * @brief Rotation quaternions (value and `_ptr`). Unsuffixed angles are radians.
 * @ingroup vecmat_core
 */

#include "vecmat/types.h"

/**
 * @brief Constructs the identity quaternion.
 *
 * @see quat_identity_ptr
 *
 * @return The identity quaternion.
 */
VEC_API quaternion quat_identity(void);

/**
 * @brief Multiplies two quaternions (a * b).
 *
 * @see quat_mul_ptr
 *
 * @param a The first quaternion.
 * @param b The second quaternion.
 * @return The resulting product quaternion.
 */
VEC_API quaternion quat_mul(quaternion a, quaternion b);

/**
 * @brief Normalizes a quaternion.
 *
 * @see quat_normalize_ptr
 *
 * @param q The input quaternion.
 * @return The normalized quaternion.
 */
VEC_API quaternion quat_normalize(quaternion q);

/**
 * @brief Returns the conjugate of a quaternion.
 *
 * @see quat_conjugate_ptr
 *
 * @param q Input quaternion.
 * @return The resulting quaternion.
 */
VEC_API quaternion quat_conjugate(quaternion q);

/**
 * @brief Returns the inverse of a quaternion.
 *
 * @see quat_inverse_ptr
 *
 * @param q Input quaternion.
 * @return The resulting quaternion.
 */
VEC_API quaternion quat_inverse(quaternion q);

/**
 * @brief Constructs a quaternion from Euler angles.
 *
 * @see quat_from_euler_ptr
 *
 * @param euler Euler angles in radians.
 * @return The resulting quaternion.
 */
VEC_API quaternion quat_from_euler(vector3 euler);

/**
 * @brief Constructs a quaternion from Euler angles in degrees.
 *
 * @see quat_from_euler_deg_ptr
 *
 * @param euler_deg Euler angles in degrees.
 * @return The resulting quaternion.
 */
VEC_API quaternion quat_from_euler_deg(vector3 euler_deg);

/**
 * @brief Constructs a quaternion from a rotation axis and angle.
 *
 * @see quat_from_axis_angle_ptr
 *
 * @param axis The rotation axis. Must be normalized.
 * @param radians The rotation angle in radians.
 * @return The resulting rotation quaternion.
 */
VEC_API quaternion quat_from_axis_angle(vector3 axis, vm_float_t radians);

/**
 * @brief Constructs a quaternion from an axis and an angle in degrees.
 *
 * @see quat_from_axis_angle_deg_ptr
 *
 * @param axis The rotation axis.
 * @param degrees The rotation angle in degrees.
 * @return The resulting quaternion.
 */
VEC_API quaternion quat_from_axis_angle_deg(vector3 axis, vm_float_t degrees);

/**
 * @brief Builds a quaternion from a 3x3 rotation matrix.
 *
 * @see quat_from_mat3_ptr
 *
 * @param m Input matrix.
 * @return The resulting quaternion.
 */
VEC_API quaternion quat_from_mat3(matrix3 m);

/**
 * @brief Builds a quaternion from the rotation of a 4x4 matrix.
 *
 * @see quat_from_mat4_ptr
 *
 * @param m Input matrix.
 * @return The resulting quaternion.
 */
VEC_API quaternion quat_from_mat4(matrix4 m);

/**
 * @brief Constructs a quaternion that rotates from the negative Z axis towards
 * the given direction, with the specified up vector.
 *
 * @see quat_look_ptr
 *
 * @param direction The target direction vector to look towards.
 * @param up The up vector.
 *
 * @return The look rotation quaternion.
 */
VEC_API quaternion quat_look(vector3 direction, vector3 up);

/**
 * @brief Constructs a quaternion representing a look-at rotation with specified clip space.
 *
 * @see quat_look_clip_ptr
 *
 * @param direction The forward direction vector.
 * @param up The up direction vector.
 * @param clip The clip space convention.
 *
 * @return The look-at quaternion.
 */
VEC_API quaternion quat_look_clip(vector3 direction, vector3 up, vm_clip_t clip);

/**
 * @brief Constructs a quaternion representing the shortest rotation from one vector to another.
 *
 * @see quat_from_to_ptr
 *
 * @param from The source direction vector.
 * @param to The target direction vector.
 *
 * @return Quaternion that rotates @p from into @p to.
 */
VEC_API quaternion quat_from_to(vector3 from, vector3 to);

/**
 * @brief Spherical-linearly interpolates from a to b by t.
 *
 * @see quat_slerp_ptr
 *
 * @param a First input quaternion.
 * @param b Second input quaternion.
 * @param t Interpolation factor.
 * @return The resulting quaternion.
 */
VEC_API quaternion quat_slerp(quaternion a, quaternion b, vm_float_t t);

/**
 * @brief Normalized-linearly interpolates from a to b by t.
 *
 * @see quat_nlerp_ptr
 *
 * @param a First input quaternion.
 * @param b Second input quaternion.
 * @param t Interpolation factor.
 * @return The resulting quaternion.
 */
VEC_API quaternion quat_nlerp(quaternion a, quaternion b, vm_float_t t);

/**
 * @brief Integrates a quaternion using angular velocity.
 *
 * @param q The starting quaternion.
 * @param omega Angular velocity vector.
 * @param dt Time delta.
 *
 * @return The integrated quaternion.
 */
VEC_API quaternion quat_integrate(quaternion q, vector3 omega, vm_float_t dt);


/**
 * @brief Rotates a vector3 by a quaternion.
 *
 * @see quat_rotate_vec3_ptr
 *
 * @param q Input quaternion.
 * @param v Input vector.
 * @return The resulting vector3.
 */
VEC_API vector3 quat_rotate_vec3(quaternion q, vector3 v);

/**
 * @brief Converts a quaternion to Euler angles in degrees (XYZ).
 *
 * @see quat_to_euler_ptr
 *
 * @param q Input quaternion.
 * @return The resulting vector3.
 */
VEC_API vector3 quat_to_euler(quaternion q);

/**
 * @brief Converts a quaternion to Euler angles in degrees.
 *
 * @see quat_to_euler_deg_ptr
 *
 * @param q The quaternion to convert.
 * @return The Euler angles in degrees as a vector3.
 */
VEC_API vector3 quat_to_euler_deg(quaternion q);

/**
 * @brief Converts a quaternion to axis-angle representation.
 *
 * @see quat_to_axis_angle_ptr
 *
 * @param q The input quaternion.
 * @param radians Pointer to a float that receives the rotation angle in radians.
 * @return The rotation axis vector.
 */
VEC_API vector3 quat_to_axis_angle(quaternion q, vm_float_t *radians);

/**
 * @brief Converts a quaternion to axis-angle representation in degrees.
 *
 * @see quat_to_axis_angle_deg_ptr
 *
 * @param q Input quaternion.
 * @param degrees Pointer to a float that receives the rotation angle in degrees.
 * @return The rotation axis vector.
 */
VEC_API vector3 quat_to_axis_angle_deg(quaternion q, vm_float_t *degrees);

/**
 * @brief Converts a quaternion to a 4x4 rotation matrix.
 *
 * @see quat_to_mat4_ptr
 *
 * @param q The input quaternion.
 * @return The equivalent matrix4.
 */
VEC_API matrix4 quat_to_mat4(quaternion q);

/**
 * @brief Converts a quaternion to a 3x3 rotation matrix.
 *
 * @see quat_to_mat3_ptr
 *
 * @param q Input quaternion.
 * @return The resulting matrix3.
 */
VEC_API matrix3 quat_to_mat3(quaternion q);

/**
 * @brief Returns the dot product of two quaternions.
 *
 * @param a First input quaternion.
 * @param b Second input quaternion.
 * @return The resulting vm_float_t.
 */
VEC_API vm_float_t quat_dot(quaternion a, quaternion b);

/**
 * @brief Returns true if a and b are within eps of each other.
 *
 * @param a First input quaternion.
 * @param b Second input quaternion.
 * @param eps Distance tolerance.
 * @return True if a and b are within eps.
 */
VEC_API bool quat_near(quaternion a, quaternion b, vm_float_t eps);

/*******************************************************************************
 * Quaternion pointer-based performance functions
 ******************************************************************************/

VEC_API void quat_identity_ptr(quaternion *res);
/**
 * @brief Hamilton product `a * b` (dispatched).
 *
 * @see quat_mul_ptr_scalar
 *
 * @param res Result quaternion.
 * @param a Left quaternion.
 * @param b Right quaternion.
 */
VEC_API void quat_mul_ptr(quaternion *res, const quaternion *a, const quaternion *b);

/**
 * @brief Normalizes a quaternion (dispatched).
 *
 * @see quat_normalize_ptr_scalar
 *
 * @param res Result quaternion.
 * @param q Input quaternion.
 */
VEC_API void quat_normalize_ptr(quaternion *res, const quaternion *q);

/**
 * @brief Writes the conjugate of a quaternion.
 *
 * @param res Output value.
 * @param q Input quaternion.
 */
VEC_API void quat_conjugate_ptr(quaternion *res, const quaternion *q);

/**
 * @brief Writes the inverse of a quaternion.
 *
 * @param res Output value.
 * @param q Input quaternion.
 */
VEC_API void quat_inverse_ptr(quaternion *res, const quaternion *q);

/**
 * @brief Converts Euler angles (in radians) to a normalized quaternion.
 *
 * Computes a quaternion from the given Euler rotation vector using the
 * XYZ (Tait-Bryan) convention. The resulting quaternion is guaranteed
 * to be normalized.
 *
 * @param res Pointer to the quaternion that will receive the result.
 * @param euler Pointer to a vector3 containing the Euler angles in radians (x, y, z).
 */
VEC_API void quat_from_euler_ptr(quaternion *res, const vector3 *euler);

/**
 * @brief Converts Euler angles in degrees to a quaternion.
 *
 * @see quat_from_euler_ptr
 *
 * @param res Pointer to the quaternion to store the result.
 * @param euler_deg Pointer to the vector containing Euler angles in degrees.
 */
VEC_API void quat_from_euler_deg_ptr(quaternion *res, const vector3 *euler_deg);

/**
 * @brief Converts an axis-angle rotation to a quaternion.
 *
 * The axis vector is normalized internally. The angle is given in radians.
 *
 * @param res Pointer to the quaternion that will receive the result.
 * @param axis Pointer to the rotation axis vector.
 * @param radians Rotation angle around the axis in radians.
 */
VEC_API void quat_from_axis_angle_ptr(quaternion *res, const vector3 *axis, vm_float_t radians);

/**
 * @brief Converts an axis-angle rotation (in degrees) to a quaternion.
 *
 * @param res Pointer to the quaternion that will receive the result.
 * @param axis Pointer to the rotation axis vector.
 * @param degrees Rotation angle around the axis in degrees.
 */
VEC_API void quat_from_axis_angle_deg_ptr(quaternion *res, const vector3 *axis, vm_float_t degrees);

/**
 * @brief Builds a quaternion from a 3x3 rotation matrix.
 *
 * @param res Output value.
 * @param m Input matrix.
 */
VEC_API void quat_from_mat3_ptr(quaternion *res, const matrix3 *m);

/**
 * @brief Builds a quaternion from the rotation of a 4x4 matrix.
 *
 * @param res Output value.
 * @param m Input matrix.
 */
VEC_API void quat_from_mat4_ptr(quaternion *res, const matrix4 *m);

/**
 * @brief Orientation that aims local -Z along @p direction (RH / FPS camera).
 *
 * Matches the rotation part of `mat4_look_from_dir` inverted (view-to-world).
 *
 * @param res Result quaternion.
 * @param direction Look direction.
 * @param up World up hint.
 */
VEC_API void quat_look_ptr(quaternion *res, const vector3 *direction, const vector3 *up);

/**
 * @brief Orientation that aims along @p direction.
 *
 * RH: local -Z maps to @p direction. LH: local +Z maps to @p direction.
 *
 * @param res Result quaternion.
 * @param direction Look direction.
 * @param up World up hint.
 * @param clip Clip-space convention selecting handedness.
 */
VEC_API void quat_look_clip_ptr(quaternion *res, const vector3 *direction, const vector3 *up, vm_clip_t clip);

/**
 * @brief Shortest rotation taking @p from onto @p to.
 *
 * Opposite vectors pick a stable orthogonal axis (180 deg). Near-parallel
 * vectors return identity.
 *
 * @param res Result quaternion.
 * @param from Source direction.
 * @param to Target direction.
 */
VEC_API void quat_from_to_ptr(quaternion *res, const vector3 *from, const vector3 *to);

/**
 * @brief Spherical-linearly interpolates from a to b by t.
 *
 * @param res Output value.
 * @param a First input quaternion.
 * @param b Second input quaternion.
 * @param t Interpolation factor.
 */
VEC_API void quat_slerp_ptr(quaternion *res, const quaternion *a, const quaternion *b, vm_float_t t);

/**
 * @brief Normalized-linearly interpolates from a to b by t.
 *
 * @param res Output value.
 * @param a First input quaternion.
 * @param b Second input quaternion.
 * @param t Interpolation factor.
 */
VEC_API void quat_nlerp_ptr(quaternion *res, const quaternion *a, const quaternion *b, vm_float_t t);

/**
 * @brief Rotates a vector3 by a quaternion.
 *
 * @param res Output value.
 * @param q Input quaternion.
 * @param v Input vector.
 */
VEC_API void quat_rotate_vec3_ptr(vector3 *res, const quaternion *q, const vector3 *v);

/**
 * @brief Converts a quaternion to Euler angles in degrees (XYZ).
 *
 * @param res Output value.
 * @param q Input quaternion.
 */
VEC_API void quat_to_euler_ptr(vector3 *res, const quaternion *q);

/**
 * @brief Converts a quaternion to XYZ Euler angles in degrees.
 *
 * @see quat_to_euler_ptr
 *
 * @param res Euler angles in degrees (x, y, z).
 * @param q Input quaternion.
 */
VEC_API void quat_to_euler_deg_ptr(vector3 *res, const quaternion *q);

/**
 * @brief Converts a quaternion to an axis-angle representation.
 *
 * The quaternion is first normalized. The axis is stored in the provided
 * vector3 pointer. The rotation angle in radians is optionally written to
 * the radians pointer if it is not NULL. When the axis cannot be uniquely
 * determined (near zero rotation) the axis is set to (1, 0, 0).
 *
 * @param axis Pointer to the vector3 that will receive the rotation axis.
 * @param radians Pointer to a float that will receive the rotation angle in radians, or NULL.
 * @param q Pointer to the input quaternion.
 */
VEC_API void quat_to_axis_angle_ptr(vector3 *axis, vm_float_t *radians, const quaternion *q);

/**
 * @brief Converts a quaternion to an axis-angle representation, with the angle in degrees.
 *
 * Extracts the rotation axis and rotation angle (in degrees) equivalent to the
 * given quaternion. The quaternion is first normalized internally. If the
 * quaternion represents no rotation, the axis is set to (1, 0, 0).
 *
 * @param axis Pointer to the vector3 that will receive the normalized rotation axis.
 * @param degrees Pointer to a float that will receive the rotation angle in degrees (may be NULL).
 * @param q Pointer to the source quaternion.
 */
VEC_API void quat_to_axis_angle_deg_ptr(vector3 *axis, vm_float_t *degrees, const quaternion *q);

/**
 * @brief Converts a unit quaternion to a 4x4 rotation matrix and stores in res.
 *
 * Starts with the identity matrix and applies rotation components.
 *
 * @param res Pointer to the output matrix4.
 * @param q Pointer to the input quaternion (should be normalized).
 */
VEC_API void quat_to_mat4_ptr(matrix4 *res, const quaternion *q);

/**
 * @brief Converts a quaternion to a 3x3 rotation matrix.
 *
 * @param res Output value.
 * @param q Input quaternion.
 */
VEC_API void quat_to_mat3_ptr(matrix3 *res, const quaternion *q);

/**
 * @brief Integrates angular velocity over a time step and applies the
 * resulting rotation to the input quaternion.
 *
 * Computes a delta quaternion from the angular velocity and timestep,
 * multiplies it with the source quaternion and normalizes the result.
 *
 * @param res Pointer to the quaternion where the integrated result is stored.
 * @param q Pointer to the source quaternion.
 * @param omega Pointer to the angular velocity vector (in radians per second).
 * @param dt Time step (in seconds).
 */
VEC_API void quat_integrate_ptr(quaternion *res, const quaternion *q, const vector3 *omega, vm_float_t dt);

/**
 * @brief Returns true if two quaternions are equal within `VECMAT_EPSILON`.
 *
 * @param a First quaternion.
 * @param b Second quaternion.
 * @return True if a and b are approximately equal.
 */
VEC_API bool quat_eq(quaternion a, quaternion b);

#endif //VECMAT_QUAT_H
