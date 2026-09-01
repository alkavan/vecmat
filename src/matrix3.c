// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

/**
 * @brief Constructs the 3x3 identity matrix.
 *
 * @see mat3_identity_ptr
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
 * @see mat3_mul_ptr
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
 * @see mat3_transpose_ptr
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
 * @see mat3_inverse_ptr
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
 * @brief Constructs a 3x3 rotation matrix around the Z axis.
 *
 * @see mat3_rotation_z_ptr
 *
 * @param radians Rotation angle in radians.
 * @return The rotation matrix3.
 */
matrix3 mat3_rotation_z(const vm_float_t radians)
{
    matrix3 m;
    mat3_rotation_z_ptr(&m, radians);
    return m;
}

/**
 * @brief Constructs a 3x3 rotation matrix around the Z axis.
 *
 * The angle is given in degrees and internally converted to radians.
 *
 * @param degrees The rotation angle in degrees.
 * @return The resulting rotation matrix3.
 */
matrix3 mat3_rotation_z_deg(const vm_float_t degrees)
{
    return mat3_rotation_z(deg_to_rad(degrees));
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
 * @see mat3_mul_vec3_ptr
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
 * @see mat3_mul_vec2_ptr
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
 * @brief Constructs a 3x3 rotation matrix around the X axis.
 *
 * @see mat3_rotation_x_ptr
 *
 * @param radians Rotation angle in radians.
 * @return The rotation matrix3.
 */
matrix3 mat3_rotation_x(const vm_float_t radians)
{
    matrix3 res;
    mat3_rotation_x_ptr(&res, radians);
    return res;
}

/**
 * @brief Constructs a 3x3 rotation matrix around the X axis.
 *
 * The angle is provided in degrees and converted to radians internally.
 *
 * @param degrees The rotation angle in degrees.
 * @return The rotation matrix3.
 */
matrix3 mat3_rotation_x_deg(const vm_float_t degrees)
{
    return mat3_rotation_x(deg_to_rad(degrees));
}

/**
 * @brief Constructs a 3x3 rotation matrix around the Y axis.
 *
 * @see `mat3_rotation_y_ptr`
 *
 * @param radians The rotation angle in radians.
 * @return The rotation matrix3.
 */
matrix3 mat3_rotation_y(const vm_float_t radians)
{
    matrix3 res;
    mat3_rotation_y_ptr(&res, radians);
    return res;
}
/**
 * @brief 3x3 rotation about Y from an angle in degrees.
 *
 * @see mat3_rotation_y_ptr
 *
 * @param degrees Rotation angle in degrees.
 * @return Rotation matrix3.
 */
matrix3 mat3_rotation_y_deg(const vm_float_t degrees)
{
    return mat3_rotation_y(deg_to_rad(degrees));
}

/**
 * @brief Builds a 3x3 2D translation matrix.
 *
 * @see mat3_translate_ptr
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
 * @see mat3_scale_ptr
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
 * @see mat3_from_mat4_ptr
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

/**
 * @brief Inverse-transpose of a 3x3 (normal matrix).
 *
 * @see mat3_normal_ptr
 *
 * @param m Linear part of a model transform.
 * @return The normal matrix3.
 */
matrix3 mat3_normal(const matrix3 m)
{
    matrix3 res;
    mat3_normal_ptr(&res, &m);
    return res;
}

/**
 * @brief Symmetric 3x3 eigensolve (Jacobi). `m` is symmetrized as (A+A^T)/2.
 *
 * Eigenvalues are unsorted principal moments. Eigenvectors are the columns
 * of the returned rotation (`axes * diag(moments) * axes^T ~= m`).
 *
 * @see mat3_sym_eigen_ptr
 *
 * @param m Input matrix (symmetrized internally).
 * @param axes Optional; receives eigenvector columns. May be NULL.
 * @return Eigenvalues as a vector3.
 */
vector3 mat3_sym_eigen(const matrix3 m, matrix3 *axes)
{
    vector3 ev;
    matrix3 local_axes;
    matrix3 *out_axes = axes ? axes : &local_axes;
    mat3_sym_eigen_ptr(&ev, out_axes, &m);
    return ev;
}
