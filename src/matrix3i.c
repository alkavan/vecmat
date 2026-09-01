// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

/**
 * @brief Constructs the 3x3 identity matrix.
 *
 * @see mat3i_identity_ptr
 *
 * @return The identity matrix3i.
 */
matrix3i mat3i_identity(void)
{
    matrix3i m;
    mat3i_identity_ptr(&m);
    return m;
}

/**
 * @brief Multiplies two 3x3 integer matrices.
 *
 * @see mat3i_mul_ptr
 *
 * @param a The first matrix.
 * @param b The second matrix.
 * @return The product matrix.
 */
matrix3i mat3i_mul(const matrix3i a, const matrix3i b)
{
    matrix3i res;
    mat3i_mul_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Computes the transpose of a 3x3 integer matrix.
 *
 * @see mat3i_transpose_ptr
 *
 * @param m The input matrix.
 * @return The transposed matrix.
 */
matrix3i mat3i_transpose(const matrix3i m)
{
    matrix3i res;
    mat3i_transpose_ptr(&res, &m);
    return res;
}

/**
 * @brief Computes the determinant of a 3x3 integer matrix.
 *
 * @param m The input matrix.
 * @return The determinant value.
 */
vm_int_t mat3i_determinant(const matrix3i m)
{
    return m.v[0] * (m.v[4] * m.v[8] - m.v[5] * m.v[7]) -
           m.v[1] * (m.v[3] * m.v[8] - m.v[5] * m.v[6]) +
           m.v[2] * (m.v[3] * m.v[7] - m.v[4] * m.v[6]);
}

/**
 * @brief Computes the inverse of a 3x3 integer matrix.
 *
 * @see mat3i_inverse_ptr
 *
 * @param m The input matrix.
 * @return The inverse matrix.
 */
matrix3i mat3i_inverse(const matrix3i m)
{
    matrix3i res;
    mat3i_inverse_ptr(&res, &m);
    return res;
}

/**
 * @brief Multiplies a 3x3 integer matrix by a vector3i.
 *
 * @see mat3i_mul_vec3i_ptr
 *
 * @param m Input matrix.
 * @param v Input vector.
 * @return The resulting vector3i.
 */
vector3i mat3i_mul_vec3i(const matrix3i m, const vector3i v)
{
    vector3i res;
    mat3i_mul_vec3i_ptr(&res, &m, &v);
    return res;
}

/**
 * @brief Applies a 3x3 integer affine transform to a vector2i.
 *
 * @see mat3i_mul_vec2i_ptr
 *
 * @param m Input matrix.
 * @param v Input vector.
 * @return The resulting vector2i.
 */
vector2i mat3i_mul_vec2i(const matrix3i m, const vector2i v)
{
    vector2i res;
    mat3i_mul_vec2i_ptr(&res, &m, &v);
    return res;
}
