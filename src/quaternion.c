// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

/**
 * @brief Constructs the identity quaternion.
 *
 * @see quat_identity_ptr
 *
 * @return The identity quaternion.
 */
quaternion quat_identity(void)
{
    quaternion res;
    quat_identity_ptr(&res);
    return res;
}

/**
 * @brief Multiplies two quaternions (a * b).
 *
 * @see quat_mul_ptr
 *
 * @param a The first quaternion.
 * @param b The second quaternion.
 * @return The resulting product quaternion.
 */
quaternion quat_mul(const quaternion a, const quaternion b)
{
    quaternion res;
    quat_mul_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Normalizes a quaternion.
 *
 * @see quat_normalize_ptr
 *
 * @param q The input quaternion.
 * @return The normalized quaternion.
 */
quaternion quat_normalize(const quaternion q)
{
    quaternion res;
    quat_normalize_ptr(&res, &q);
    return res;
}

/**
 * @brief Constructs a quaternion from Euler angles.
 *
 * @see quat_from_euler_ptr
 *
 * @param euler Euler angles in radians.
 * @return The resulting quaternion.
 */
quaternion quat_from_euler(const vector3 euler)
{
    quaternion res;
    quat_from_euler_ptr(&res, &euler);
    return res;
}

/**
 * @brief Constructs a quaternion from Euler angles in degrees.
 *
 * @see quat_from_euler_deg_ptr
 *
 * @param euler_deg Euler angles in degrees.
 * @return The resulting quaternion.
 */
quaternion quat_from_euler_deg(const vector3 euler_deg)
{
    quaternion res;
    quat_from_euler_deg_ptr(&res, &euler_deg);
    return res;
}

/**
 * @brief Converts a quaternion to a 4x4 rotation matrix.
 *
 * @see quat_to_mat4_ptr
 *
 * @param q The input quaternion.
 * @return The equivalent matrix4.
 */
matrix4 quat_to_mat4(const quaternion q)
{
    matrix4 res;
    quat_to_mat4_ptr(&res, &q);
    return res;
}

/**
 * @brief Returns the conjugate of a quaternion.
 *
 * @see quat_conjugate_ptr
 *
 * @param q Input quaternion.
 * @return The resulting quaternion.
 */
quaternion quat_conjugate(const quaternion q)
{
    quaternion res;
    quat_conjugate_ptr(&res, &q);
    return res;
}

/**
 * @brief Returns the inverse of a quaternion.
 *
 * @see quat_inverse_ptr
 *
 * @param q Input quaternion.
 * @return The resulting quaternion.
 */
quaternion quat_inverse(const quaternion q)
{
    quaternion res;
    quat_inverse_ptr(&res, &q);
    return res;
}

/**
 * @brief Constructs a quaternion from a rotation axis and angle.
 *
 * @see quat_from_axis_angle_ptr
 *
 * @param axis The rotation axis. Must be normalized.
 * @param radians The rotation angle in radians.
 * @return The resulting rotation quaternion.
 */
quaternion quat_from_axis_angle(const vector3 axis, const vm_float_t radians)
{
    quaternion res;
    quat_from_axis_angle_ptr(&res, &axis, radians);
    return res;
}

/**
 * @brief Constructs a quaternion from an axis and an angle in degrees.
 *
 * @see quat_from_axis_angle_deg_ptr
 *
 * @param axis The rotation axis.
 * @param degrees The rotation angle in degrees.
 * @return The resulting quaternion.
 */
quaternion quat_from_axis_angle_deg(const vector3 axis, const vm_float_t degrees)
{
    quaternion res;
    quat_from_axis_angle_deg_ptr(&res, &axis, degrees);
    return res;
}

/**
 * @brief Builds a quaternion from a 3x3 rotation matrix.
 *
 * @see quat_from_mat3_ptr
 *
 * @param m Input matrix.
 * @return The resulting quaternion.
 */
quaternion quat_from_mat3(const matrix3 m)
{
    quaternion res;
    quat_from_mat3_ptr(&res, &m);
    return res;
}

/**
 * @brief Builds a quaternion from the rotation of a 4x4 matrix.
 *
 * @see quat_from_mat4_ptr
 *
 * @param m Input matrix.
 * @return The resulting quaternion.
 */
quaternion quat_from_mat4(const matrix4 m)
{
    quaternion res;
    quat_from_mat4_ptr(&res, &m);
    return res;
}

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
quaternion quat_slerp(const quaternion a, const quaternion b, const vm_float_t t)
{
    quaternion res;
    quat_slerp_ptr(&res, &a, &b, t);
    return res;
}

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
quaternion quat_nlerp(const quaternion a, const quaternion b, const vm_float_t t)
{
    quaternion res;
    quat_nlerp_ptr(&res, &a, &b, t);
    return res;
}

/**
 * @brief Rotates a vector3 by a quaternion.
 *
 * @see quat_rotate_vec3_ptr
 *
 * @param q Input quaternion.
 * @param v Input vector.
 * @return The resulting vector3.
 */
vector3 quat_rotate_vec3(const quaternion q, const vector3 v)
{
    vector3 res;
    quat_rotate_vec3_ptr(&res, &q, &v);
    return res;
}

/**
 * @brief Converts a quaternion to Euler angles in degrees (XYZ).
 *
 * @see quat_to_euler_ptr
 *
 * @param q Input quaternion.
 * @return The resulting vector3.
 */
vector3 quat_to_euler(const quaternion q)
{
    vector3 res;
    quat_to_euler_ptr(&res, &q);
    return res;
}

/**
 * @brief Converts a quaternion to Euler angles in degrees.
 *
 * @see quat_to_euler_deg_ptr
 *
 * @param q The quaternion to convert.
 * @return The Euler angles in degrees as a vector3.
 */
vector3 quat_to_euler_deg(const quaternion q)
{
    vector3 res;
    quat_to_euler_deg_ptr(&res, &q);
    return res;
}

/**
 * @brief Converts a quaternion to axis-angle representation.
 *
 * @see quat_to_axis_angle_ptr
 *
 * @param q The input quaternion.
 * @param radians Pointer to a float that receives the rotation angle in radians.
 * @return The rotation axis vector.
 */
vector3 quat_to_axis_angle(const quaternion q, vm_float_t *radians)
{
    vector3 axis;
    quat_to_axis_angle_ptr(&axis, radians, &q);
    return axis;
}

/**
 * @brief Converts a quaternion to axis-angle representation in degrees.
 *
 * @see quat_to_axis_angle_deg_ptr
 *
 * @param q Input quaternion.
 * @param degrees Pointer to a float that receives the rotation angle in degrees.
 * @return The rotation axis vector.
 */
vector3 quat_to_axis_angle_deg(const quaternion q, vm_float_t *degrees)
{
    vector3 axis;
    quat_to_axis_angle_deg_ptr(&axis, degrees, &q);
    return axis;
}

/**
 * @brief Converts a quaternion to a 3x3 rotation matrix.
 *
 * @see quat_to_mat3_ptr
 *
 * @param q Input quaternion.
 * @return The resulting matrix3.
 */
matrix3 quat_to_mat3(const quaternion q)
{
    matrix3 res;
    quat_to_mat3_ptr(&res, &q);
    return res;
}

/**
 * @brief Returns the dot product of two quaternions.
 *
 * @param a First input quaternion.
 * @param b Second input quaternion.
 * @return The resulting vm_float_t.
 */
vm_float_t quat_dot(const quaternion a, const quaternion b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

/**
 * @brief Returns true if a and b are within eps of each other.
 *
 * @param a First input quaternion.
 * @param b Second input quaternion.
 * @param eps Distance tolerance.
 * @return True if a and b are within eps.
 */
bool quat_near(const quaternion a, const quaternion b, const vm_float_t eps)
{
    return VECMAT_FABS(a.x - b.x) <= eps
        && VECMAT_FABS(a.y - b.y) <= eps
        && VECMAT_FABS(a.z - b.z) <= eps
        && VECMAT_FABS(a.w - b.w) <= eps;
}

/**
 * @brief Integrates a quaternion using angular velocity.
 *
 * @param q The starting quaternion.
 * @param omega Angular velocity vector.
 * @param dt Time delta.
 *
 * @return The integrated quaternion.
 */
quaternion quat_integrate(const quaternion q, const vector3 omega, const vm_float_t dt)
{
    quaternion res;
    quat_integrate_ptr(&res, &q, &omega, dt);
    return res;
}

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
quaternion quat_look(const vector3 direction, const vector3 up)
{
    quaternion res;
    quat_look_ptr(&res, &direction, &up);
    return res;
}

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
quaternion quat_look_clip(const vector3 direction, const vector3 up, const vm_clip_t clip)
{
    quaternion res;
    quat_look_clip_ptr(&res, &direction, &up, clip);
    return res;
}

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
quaternion quat_from_to(const vector3 from, const vector3 to)
{
    quaternion res;
    quat_from_to_ptr(&res, &from, &to);
    return res;
}
