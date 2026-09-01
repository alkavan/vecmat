// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

/**
 * @brief Constructs the 2x2 identity matrix.
 *
 * @see mat2_identity_ptr
 *
 * @return The identity matrix2.
 */
matrix2 mat2_identity(void)
{
    matrix2 res;
    mat2_identity_ptr(&res);
    return res;
}

/**
 * @brief Multiplies two 2x2 matrices (a * b).
 *
 * @see mat2_mul_ptr
 *
 * @param a The first matrix.
 * @param b The second matrix.
 * @return The resulting product matrix2.
 */
matrix2 mat2_mul(const matrix2 a, const matrix2 b)
{
    matrix2 res;
    mat2_mul_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Computes the transpose of a 2x2 matrix.
 *
 * @see mat2_transpose_ptr
 *
 * @param m The input matrix.
 * @return The transposed matrix2.
 */
matrix2 mat2_transpose(const matrix2 m)
{
    matrix2 res;
    mat2_transpose_ptr(&res, &m);
    return res;
}

/**
  * @brief Computes the inverse of a 2x2 matrix.
  *
  * @see mat2_inverse_ptr
  *
  * @param m The input matrix.
  * @return The inverse matrix2.
  */
matrix2 mat2_inverse(const matrix2 m)
{
    matrix2 res;
    mat2_inverse_ptr(&res, &m);
    return res;
}

/**
 * @brief Constructs a 2x2 rotation matrix around the Z axis.
 *
 * @see mat2_rotation_z_ptr
 *
 * @param radians Rotation angle in radians.
 * @return The rotation matrix2.
 */
matrix2 mat2_rotation_z(const vm_float_t radians)
{
    matrix2 res;
    mat2_rotation_z_ptr(&res, radians);
    return res;
}

/**
 * @brief Constructs a 2x2 rotation matrix from an angle in degrees around the Z axis.
 *
 * @param degrees The rotation angle in degrees.
 * @return The rotation matrix2.
 */
matrix2 mat2_rotation_z_deg(const vm_float_t degrees)
{
    return mat2_rotation_z(deg_to_rad(degrees));
}

/**
 * @brief Calculates the determinant of the given 2x2 matrix.
 *
 * @param m The matrix.
 * @return The determinant value (m.v[0]*m.v[3] - m.v[1]*m.v[2]).
 */
vm_float_t mat2_determinant(const matrix2 m)
{
    return m.v[0] * m.v[3] - m.v[1] * m.v[2];
}

/**
 * @brief Multiplies a 2x2 matrix by a vector2.
 *
 * @see mat2_mul_vec2_ptr
 *
 * @param m Input matrix.
 * @param v Input vector.
 * @return The resulting vector2.
 */
vector2 mat2_mul_vec2(const matrix2 m, const vector2 v)
{
    vector2 res;
    mat2_mul_vec2_ptr(&res, &m, &v);
    return res;
}

/**
 * @brief Builds a 2x2 scaling matrix from a vector2.
 *
 * @see mat2_scale_ptr
 *
 * @param s Scale vector.
 * @return The resulting matrix2.
 */
matrix2 mat2_scale(const vector2 s)
{
    matrix2 res;
    mat2_scale_ptr(&res, &s);
    return res;
}

/**
 * @brief Copies the upper-left 2x2 of a matrix3.
 *
 * @see mat2_from_mat3_ptr
 *
 * @param m Input matrix.
 * @return The resulting matrix2.
 */
matrix2 mat2_from_mat3(const matrix3 m)
{
    matrix2 res;
    mat2_from_mat3_ptr(&res, &m);
    return res;
}
