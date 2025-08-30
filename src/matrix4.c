// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

/**
 * @brief Constructs the 4x4 identity matrix.
 *
 * See `mat4_identity_ptr` instead.
 *
 * @return The identity matrix4.
 */
matrix4 mat4_identity(void)
{
    matrix4 res = {0};
    mat4_identity_ptr(&res);
    return res;
}

/**
 * @brief Multiplies two 4x4 matrices (a * b).
 *
 * See `mat4_mul_ptr` instead.
 *
 * @param a The first matrix.
 * @param b The second matrix.
 * @return The resulting product matrix4.
 */
matrix4 mat4_mul(const matrix4 a, const matrix4 b)
{
    matrix4 res = {0};
    mat4_mul_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Computes the transpose of a 4x4 matrix.
 *
 * See `mat4_transpose_ptr` instead.
 *
 * @param m The input matrix.
 * @return The transposed matrix4.
 */
matrix4 mat4_transpose(const matrix4 m)
{
    matrix4 res = {0};
    mat4_transpose_ptr(&res, &m);
    return res;
}

/**
 * @brief Computes the inverse of a 4x4 matrix.
 *
 * See `mat4_inverse_ptr` instead.
 *
 * @param m The input matrix.
 * @return The inverse matrix4.
 */
matrix4 mat4_inverse(const matrix4 m)
{
    matrix4 res = {0};
    mat4_inverse_ptr(&res, &m);
    return res;
}

/**
 * @brief Constructs a translation matrix from the given vector.
 *
 * See `mat4_translate_ptr` instead.
 *
 * @param v The translation vector (x, y, z).
 * @return The translation matrix4.
 */
matrix4 mat4_translate(const vector3 v)
{
    matrix4 res = {0};
    mat4_translate_ptr(&res, &v);
    return res;
}

/**
 * @brief Constructs a scaling matrix from the given scale vector.
 *
 * See `mat4_scale_ptr` instead.
 *
 * @param v The scale vector (x, y, z factors).
 * @return The resulting scaling matrix4.
 */
matrix4 mat4_scale(const vector3 v)
{
    matrix4 res = {0};
    mat4_scale_ptr(&res, &v);
    return res;
}

/**
 * @brief Constructs a rotation matrix around the given axis by the specified angle (in radians).
 *
 * See `mat4_rotation_ptr` instead.
 *
 * @param axis The rotation axis vector.
 * @param angle The rotation angle in radians.
 * @return The rotation matrix4.
 */
matrix4 mat4_rotation(const vector3 axis, const vm_float_t angle)
{
    matrix4 res = {0};
    mat4_rotation_ptr(&res, &axis, angle);
    return res;
}

/**
 * @brief Constructs a perspective projection matrix.
 *
 * See `mat4_perspective_ptr` instead.
 *
 * @param fov Field of view (radians).
 * @param aspect Aspect ratio (width/height).
 * @param near Near plane distance.
 * @param far Far plane distance.
 * @return The perspective matrix4.
 */
matrix4 mat4_perspective(const vm_float_t fov, const vm_float_t aspect, const vm_float_t near, const vm_float_t far)
{
    matrix4 res = {0};
    mat4_perspective_ptr(&res, fov, aspect, near, far);
    return res;
}

/**
 * @brief Constructs an orthographic projection matrix.
 *
 * See `mat4_ortho_ptr` instead.
 *
 * @param left Left clipping plane.
 * @param right Right clipping plane.
 * @param bottom Bottom clipping plane.
 * @param top Top clipping plane.
 * @param near Near plane distance.
 * @param far Far plane distance.
 * @return The orthographic matrix4.
 */
matrix4 mat4_ortho(const vm_float_t left, const vm_float_t right, const vm_float_t bottom,
    const vm_float_t top, const vm_float_t near, const vm_float_t far)
{
    matrix4 res = {0};
    mat4_ortho_ptr(&res, left, right, bottom, top, near, far);
    return res;
}

/**
 * @brief Constructs a view matrix from eye position, target, and up vector.
 *
 * See `mat4_look_at_ptr` instead.
 *
 * @param position Eye position.
 * @param target Target position.
 * @param up Up vector.
 * @return The view matrix4.
 */
matrix4 mat4_look_at(const vector3 position, const vector3 target, const vector3 up)
{
    matrix4 res = {0};
    mat4_look_at_ptr(&res, &position, &target, &up);
    return res;
}

/**
 * @brief Constructs a perspective projection matrix using FOV, width, and height.
 *
 * See `mat4_perspective_fov_ptr` instead.
 *
 * @param fov Vertical field of view (radians).
 * @param w Viewport width.
 * @param h Viewport height.
 * @param n Near plane.
 * @param f Far plane.
 * @return The perspective matrix4.
 */
matrix4 mat4_perspective_fov(const vm_float_t fov, const vm_float_t w, const vm_float_t h,
    const vm_float_t n, const vm_float_t f)
{
    matrix4 res = {0};
    mat4_perspective_fov_ptr(&res, fov, w, h, n, f);
    return res;
}

/**
 * @brief Constructs an infinite far-plane perspective projection matrix.
 *
 * See `mat4_perspective_infinite_ptr` instead.
 *
 * @param fov_y Vertical field of view (radians).
 * @param aspect Aspect ratio.
 * @param n Near plane distance.
 * @return The perspective matrix4.
 */
matrix4 mat4_perspective_infinite(const vm_float_t fov_y, const vm_float_t aspect, const vm_float_t n)
{
    matrix4 res = {0};
    mat4_perspective_infinite_ptr(&res, fov_y, aspect, n);
    return res;
}

/**
 * @brief Calculates the determinant of the given 4x4 matrix.
 *
 * Uses cofactor expansion for computation.
 *
 * @param m The matrix.
 * @return The determinant value as a vm_float_t.
 */
vm_float_t mat4_determinant(const matrix4 m)
{
    const vm_float_t cofactor_1 =  m.v[5] * m.v[10] * m.v[15] - m.v[5] * m.v[14] * m.v[11] - m.v[9] * m.v[6] * m.v[15] +
                                   m.v[9] * m.v[14] * m.v[7] + m.v[13] * m.v[6] * m.v[11] - m.v[13] * m.v[10] * m.v[7];

    const vm_float_t cofactor_2 = -m.v[4] * m.v[10] * m.v[15] + m.v[4] * m.v[14] * m.v[11] + m.v[8] * m.v[6] * m.v[15] -
                                   m.v[8] * m.v[14] * m.v[7] - m.v[12] * m.v[6] * m.v[11] + m.v[12] * m.v[10] * m.v[7];

    const vm_float_t cofactor_3 =  m.v[4] * m.v[9]  * m.v[15] - m.v[4] * m.v[13] * m.v[11] - m.v[8] * m.v[5] * m.v[15] +
                                   m.v[8] * m.v[13] * m.v[7] + m.v[12] * m.v[5] * m.v[11] - m.v[12] * m.v[9]  * m.v[7];

    const vm_float_t cofactor_4 = -m.v[4] * m.v[9]  * m.v[14] + m.v[4] * m.v[13] * m.v[10] + m.v[8] * m.v[5] * m.v[14] -
                                   m.v[8] * m.v[13] * m.v[6] - m.v[12] * m.v[5] * m.v[10] + m.v[12] * m.v[9]  * m.v[6];

    return m.v[0] * cofactor_1 + m.v[1] * cofactor_2 + m.v[2] * cofactor_3 + m.v[3] * cofactor_4;
}

/**
 * @brief Multiplies a 4x4 matrix by a vector4.
 *
 * See `mat4_mul_vec4_ptr` instead.
 *
 * @param m Input matrix.
 * @param v Input vector.
 * @return The resulting vector4.
 */
vector4 mat4_mul_vec4(const matrix4 m, const vector4 v)
{
    vector4 res;
    mat4_mul_vec4_ptr(&res, &m, &v);
    return res;
}

/**
 * @brief Transforms a vector3 by a 4x4 matrix using homogeneous w.
 *
 * See `mat4_mul_vec3_ptr` instead.
 *
 * @param m Input matrix.
 * @param v Input vector.
 * @param w Homogeneous w component.
 * @return The resulting vector3.
 */
vector3 mat4_mul_vec3(const matrix4 m, const vector3 v, const vm_float_t w)
{
    vector3 res;
    mat4_mul_vec3_ptr(&res, &m, &v, w);
    return res;
}

/**
 * @brief Builds a 4x4 rotation matrix around the X axis (degrees).
 *
 * See `mat4_rotation_x_ptr` instead.
 *
 * @param degrees Rotation angle in degrees.
 * @return The resulting matrix4.
 */
matrix4 mat4_rotation_x(const vm_float_t degrees)
{
    matrix4 res;
    mat4_rotation_x_ptr(&res, degrees);
    return res;
}

/**
 * @brief Builds a 4x4 rotation matrix around the Y axis (degrees).
 *
 * See `mat4_rotation_y_ptr` instead.
 *
 * @param degrees Rotation angle in degrees.
 * @return The resulting matrix4.
 */
matrix4 mat4_rotation_y(const vm_float_t degrees)
{
    matrix4 res;
    mat4_rotation_y_ptr(&res, degrees);
    return res;
}

/**
 * @brief Builds a 4x4 rotation matrix around the Z axis (degrees).
 *
 * See `mat4_rotation_z_ptr` instead.
 *
 * @param degrees Rotation angle in degrees.
 * @return The resulting matrix4.
 */
matrix4 mat4_rotation_z(const vm_float_t degrees)
{
    matrix4 res;
    mat4_rotation_z_ptr(&res, degrees);
    return res;
}

/**
 * @brief Builds a 4x4 TRS matrix from translation, rotation, and scale.
 *
 * See `mat4_trs_ptr` instead.
 *
 * @param translation Translation vector.
 * @param rotation Rotation quaternion.
 * @param scale Scale vector.
 * @return The resulting matrix4.
 */
matrix4 mat4_trs(const vector3 translation, const quaternion rotation, const vector3 scale)
{
    matrix4 res;
    mat4_trs_ptr(&res, &translation, &rotation, &scale);
    return res;
}

/**
 * @brief Embeds a matrix3 into the upper-left of a matrix4.
 *
 * See `mat4_from_mat3_ptr` instead.
 *
 * @param m Input matrix.
 * @return The resulting matrix4.
 */
matrix4 mat4_from_mat3(const matrix3 m)
{
    matrix4 res;
    mat4_from_mat3_ptr(&res, &m);
    return res;
}

/**
 * @brief Extracts the translation vector from a matrix4.
 *
 * See `mat4_extract_translation_ptr` instead.
 *
 * @param m Input matrix.
 * @return The resulting vector3.
 */
vector3 mat4_extract_translation(const matrix4 m)
{
    vector3 res;
    mat4_extract_translation_ptr(&res, &m);
    return res;
}

/**
 * @brief Extracts the scale vector from a matrix4.
 *
 * See `mat4_extract_scale_ptr` instead.
 *
 * @param m Input matrix.
 * @return The resulting vector3.
 */
vector3 mat4_extract_scale(const matrix4 m)
{
    vector3 res;
    mat4_extract_scale_ptr(&res, &m);
    return res;
}

/**
 * @brief Extracts the rotation quaternion from a matrix4.
 *
 * See `mat4_extract_rotation_ptr` instead.
 *
 * @param m Input matrix.
 * @return The resulting quaternion.
 */
quaternion mat4_extract_rotation(const matrix4 m)
{
    quaternion res;
    mat4_extract_rotation_ptr(&res, &m);
    return res;
}
