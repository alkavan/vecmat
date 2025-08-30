// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

/**
 * @brief Constructs the 4x4 identity matrix.
 *
 * See `mat4i_identity_ptr` instead.
 *
 * @return The identity matrix4i.
 */
matrix4i mat4i_identity(void)
{
    matrix4i res;
    mat4i_identity_ptr(&res);
    return res;
}

/**
 * @brief Multiplies two 4x4 integer matrices.
 *
 * See `mat4i_mul_ptr` instead.
 *
 * @param a The first matrix.
 * @param b The second matrix.
 * @return The product matrix.
 */
matrix4i mat4i_mul(const matrix4i a, const matrix4i b)
{
    matrix4i res;
    mat4i_mul_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Computes the transpose of a 4x4 integer matrix.
 *
 * See `mat4i_transpose_ptr` instead.
 *
 * @param m The input matrix.
 * @return The transposed matrix.
 */
matrix4i mat4i_transpose(const matrix4i m)
{
    matrix4i res;
    mat4i_transpose_ptr(&res, &m);
    return res;
}

/**
 * @brief Computes the inverse of a 4x4 integer matrix.
 *
 * See `mat4i_inverse_ptr` instead.
 *
 * @param m The input matrix.
 * @return The inverse matrix.
 */
matrix4i mat4i_inverse(const matrix4i m)
{
    matrix4i res;
    mat4i_inverse_ptr(&res, &m);
    return res;
}

/**
 * @brief  Computes the determinant of a 4x4 integer matrix (Laplace expansion along first row).
 *
 * @param m The input matrix.
 * @return The determinant value.
 */
vm_int_t mat4i_determinant(const matrix4i m)
{
    const vm_int_t cofactor_1 =  m.v[5] * m.v[10] * m.v[15] - m.v[5] * m.v[14] * m.v[11] - m.v[9] * m.v[6] * m.v[15] +
                                 m.v[9] * m.v[14] * m.v[7] + m.v[13] * m.v[6] * m.v[11] - m.v[13] * m.v[10] * m.v[7];

    const vm_int_t cofactor_2 = -m.v[4] * m.v[10] * m.v[15] + m.v[4] * m.v[14] * m.v[11] + m.v[8] * m.v[6] * m.v[15] -
                                 m.v[8] * m.v[14] * m.v[7] - m.v[12] * m.v[6] * m.v[11] + m.v[12] * m.v[10] * m.v[7];

    const vm_int_t cofactor_3 =  m.v[4] * m.v[9]  * m.v[15] - m.v[4] * m.v[13] * m.v[11] - m.v[8] * m.v[5] * m.v[15] +
                                 m.v[8] * m.v[13] * m.v[7] + m.v[12] * m.v[5] * m.v[11] - m.v[12] * m.v[9]  * m.v[7];

    const vm_int_t cofactor_4 = -m.v[4] * m.v[9]  * m.v[14] + m.v[4] * m.v[13] * m.v[10] + m.v[8] * m.v[5] * m.v[14] -
                                 m.v[8] * m.v[13] * m.v[6] - m.v[12] * m.v[5] * m.v[10] + m.v[12] * m.v[9]  * m.v[6];

    return m.v[0] * cofactor_1 + m.v[1] * cofactor_2 + m.v[2] * cofactor_3 + m.v[3] * cofactor_4;
}

/**
 * @brief Multiplies a 4x4 integer matrix by a vector4i.
 *
 * See `mat4i_mul_vec4i_ptr` instead.
 *
 * @param m Input matrix.
 * @param v Input vector.
 * @return The resulting vector4i.
 */
vector4i mat4i_mul_vec4i(const matrix4i m, const vector4i v)
{
    vector4i res;
    mat4i_mul_vec4i_ptr(&res, &m, &v);
    return res;
}

/**
 * @brief Transforms a vector3i by a 4x4 integer matrix using homogeneous w.
 *
 * See `mat4i_mul_vec3i_ptr` instead.
 *
 * @param m Input matrix.
 * @param v Input vector.
 * @param w Homogeneous w component.
 * @return The resulting vector3i.
 */
vector3i mat4i_mul_vec3i(const matrix4i m, const vector3i v, const vm_int_t w)
{
    vector3i res;
    mat4i_mul_vec3i_ptr(&res, &m, &v, w);
    return res;
}
