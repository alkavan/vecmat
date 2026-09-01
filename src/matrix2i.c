// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

/**
 * @brief Constructs the 2x2 integer identity matrix.
 *
 * @see mat2i_identity_ptr
 *
 * @return The identity matrix2i.
 */
matrix2i mat2i_identity(void)
{
    matrix2i res;
    mat2i_identity_ptr(&res);
    return res;
}

/**
 * @brief Multiplies two 2x2 integer matrices (a * b).
 *
 * @see mat2i_mul_ptr
 *
 * @param a The first matrix.
 * @param b The second matrix.
 * @return The resulting product matrix2i.
 */
matrix2i mat2i_mul(const matrix2i a, const matrix2i b)
{
    matrix2i res;
    mat2i_mul_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Computes the transpose of a 2x2 integer matrix.
 *
 * @see mat2i_transpose_ptr
 *
 * @param m The input matrix.
 * @return The transposed matrix2i.
 */
matrix2i mat2i_transpose(const matrix2i m)
{
    matrix2i res;
    mat2i_transpose_ptr(&res, &m);
    return res;
}

/**
 * @brief Computes the inverse of a 2x2 integer matrix.
 *
 * @see mat2i_inverse_ptr
 *
 * @param m The input matrix.
 * @return The inverse matrix2i.
 */
matrix2i mat2i_inverse(const matrix2i m)
{
    matrix2i res;
    mat2i_inverse_ptr(&res, &m);
    return res;
}

/**
 * @brief Calculates the determinant of the given 2x2 integer matrix.
 *
 * @param m The matrix.
 * @return The determinant value (m.v[0]*m.v[3] - m.v[1]*m.v[2]).
 */
vm_int_t mat2i_determinant(const matrix2i m)
{
    return m.v[0] * m.v[3] - m.v[1] * m.v[2];
}

/**
 * @brief Multiplies a 2x2 integer matrix by a vector2i.
 *
 * @see mat2i_mul_vec2i_ptr
 *
 * @param m Input matrix.
 * @param v Input vector.
 * @return The resulting vector2i.
 */
vector2i mat2i_mul_vec2i(const matrix2i m, const vector2i v)
{
    vector2i res;
    mat2i_mul_vec2i_ptr(&res, &m, &v);
    return res;
}
