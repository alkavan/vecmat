// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

/**
 * @brief Constructs the identity quaternion.
 *
 * See `quat_identity_ptr` instead.
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
 * See `quat_mul_ptr` instead.
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
 * See `quat_normalize_ptr` instead.
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
 * @brief Constructs a quaternion from Euler angles (in radians, XYZ order).
 *
 * See `quat_from_euler_ptr` instead.
 *
 * @param euler The Euler angles vector (x=pitch, y=yaw, z=roll).
 * @return The resulting quaternion.
 */
quaternion quat_from_euler(const vector3 euler)
{
    quaternion res;
    quat_from_euler_ptr(&res, &euler);
    return res;
}

/**
 * @brief Converts a quaternion to a 4x4 rotation matrix.
 *
 * See `quat_to_mat4_ptr` instead.
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
 * See `quat_conjugate_ptr` instead.
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
 * See `quat_inverse_ptr` instead.
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
 * @brief Builds a quaternion from an axis and an angle in degrees.
 *
 * See `quat_from_axis_angle_ptr` instead.
 *
 * @param axis Rotation axis.
 * @param degrees Rotation angle in degrees.
 * @return The resulting quaternion.
 */
quaternion quat_from_axis_angle(const vector3 axis, const vm_float_t degrees)
{
    quaternion res;
    quat_from_axis_angle_ptr(&res, &axis, degrees);
    return res;
}

/**
 * @brief Builds a quaternion from a 3x3 rotation matrix.
 *
 * See `quat_from_mat3_ptr` instead.
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
 * See `quat_from_mat4_ptr` instead.
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
 * See `quat_slerp_ptr` instead.
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
 * See `quat_nlerp_ptr` instead.
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
 * See `quat_rotate_vec3_ptr` instead.
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
 * See `quat_to_euler_ptr` instead.
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
 * @brief Converts a quaternion to an axis and an angle in degrees.
 *
 * See `quat_to_axis_angle_ptr` instead.
 *
 * @param q Input quaternion.
 * @param degrees Optional output angle in degrees.
 * @return The rotation axis.
 */
vector3 quat_to_axis_angle(const quaternion q, vm_float_t *degrees)
{
    vector3 axis;
    quat_to_axis_angle_ptr(&axis, degrees, &q);
    return axis;
}

/**
 * @brief Converts a quaternion to a 3x3 rotation matrix.
 *
 * See `quat_to_mat3_ptr` instead.
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
