// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

/**
 * @brief Constructs the 3x3 identity matrix.
 *
 * See `mat3_identity_ptr` instead.
 *
 * @return The identity matrix3.
 */
matrix3 mat3_identity(void)
{
    matrix3 m;
    mat3_identity_ptr(&m);
    return m;
}

/**
 * @brief Multiplies two 3x3 matrices.
 *
 * See `mat3_mul_ptr` instead.
 *
 * @param a The first matrix.
 * @param b The second matrix.
 * @return The product matrix.
 */
matrix3 mat3_mul(const matrix3 a, const matrix3 b)
{
    matrix3 res;
    mat3_mul_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Computes the transpose of a 3x3 matrix.
 *
 * See `mat3_transpose_ptr` instead.
 *
 * @param m The input matrix.
 * @return The transposed matrix.
 */
matrix3 mat3_transpose(const matrix3 m)
{
    matrix3 res;
    mat3_transpose_ptr(&res, &m);
    return res;
}

/**
 * @brief Computes the inverse of a 3x3 matrix.
 *
 * See `mat3_inverse_ptr` instead.
 *
 * @param m The input matrix.
 * @return The inverse matrix.
 */
matrix3 mat3_inverse(const matrix3 m)
{
    matrix3 res;
    mat3_inverse_ptr(&res, &m);
    return res;
}

/**
 * @brief Constructs the 3x3 rotation matrix around the Z-axis.
 *
 * See `mat3_rotation_z_ptr` instead.
 *
 * @param degrees The rotation angle in degrees.
 * @return The rotation matrix3.
 */
matrix3 mat3_rotation_z(const vm_float_t degrees)
{
    matrix3 m;
    mat3_rotation_z_ptr(&m, degrees);
    return m;
}

/**
 * @brief Computes the determinant of a 3x3 matrix.
 *
 * @param m The input matrix.
 * @return The determinant value.
 */
vm_float_t mat3_determinant(const matrix3 m)
{
    return m.v[0] * (m.v[4] * m.v[8] - m.v[5] * m.v[7]) -
           m.v[1] * (m.v[3] * m.v[8] - m.v[5] * m.v[6]) +
           m.v[2] * (m.v[3] * m.v[7] - m.v[4] * m.v[6]);
}

/**
 * @brief Multiplies a 3x3 matrix by a vector3.
 *
 * See `mat3_mul_vec3_ptr` instead.
 *
 * @param m Input matrix.
 * @param v Input vector.
 * @return The resulting vector3.
 */
vector3 mat3_mul_vec3(const matrix3 m, const vector3 v)
{
    vector3 res;
    mat3_mul_vec3_ptr(&res, &m, &v);
    return res;
}

/**
 * @brief Applies a 3x3 affine transform to a vector2.
 *
 * See `mat3_mul_vec2_ptr` instead.
 *
 * @param m Input matrix.
 * @param v Input vector.
 * @return The resulting vector2.
 */
vector2 mat3_mul_vec2(const matrix3 m, const vector2 v)
{
    vector2 res;
    mat3_mul_vec2_ptr(&res, &m, &v);
    return res;
}

/**
 * @brief Builds a 3x3 rotation matrix around the X axis (degrees).
 *
 * See `mat3_rotation_x_ptr` instead.
 *
 * @param degrees Rotation angle in degrees.
 * @return The resulting matrix3.
 */
matrix3 mat3_rotation_x(const vm_float_t degrees)
{
    matrix3 res;
    mat3_rotation_x_ptr(&res, degrees);
    return res;
}

/**
 * @brief Builds a 3x3 rotation matrix around the Y axis (degrees).
 *
 * See `mat3_rotation_y_ptr` instead.
 *
 * @param degrees Rotation angle in degrees.
 * @return The resulting matrix3.
 */
matrix3 mat3_rotation_y(const vm_float_t degrees)
{
    matrix3 res;
    mat3_rotation_y_ptr(&res, degrees);
    return res;
}

/**
 * @brief Builds a 3x3 2D translation matrix.
 *
 * See `mat3_translate_ptr` instead.
 *
 * @param t Translation vector.
 * @return The resulting matrix3.
 */
matrix3 mat3_translate(const vector2 t)
{
    matrix3 res;
    mat3_translate_ptr(&res, &t);
    return res;
}

/**
 * @brief Builds a 3x3 2D scaling matrix.
 *
 * See `mat3_scale_ptr` instead.
 *
 * @param s Scale vector.
 * @return The resulting matrix3.
 */
matrix3 mat3_scale(const vector2 s)
{
    matrix3 res;
    mat3_scale_ptr(&res, &s);
    return res;
}

/**
 * @brief Copies the upper-left 3x3 of a matrix4.
 *
 * See `mat3_from_mat4_ptr` instead.
 *
 * @param m Input matrix.
 * @return The resulting matrix3.
 */
matrix3 mat3_from_mat4(const matrix4 m)
{
    matrix3 res;
    mat3_from_mat4_ptr(&res, &m);
    return res;
}
