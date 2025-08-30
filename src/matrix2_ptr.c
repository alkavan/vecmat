// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

/**
 * @brief Initializes the 2x2 matrix to identity (diagonal 1.0, others 0.0).
 *
 * @param res Pointer to the output matrix2.
 */
void mat2_identity_ptr(matrix2 *res)
{
    *res = (matrix2){0};
    res->v[0] = 1.0f;
    res->v[3] = 1.0f;
}

/**
 * @brief Multiplies two 2x2 matrices (a * b) using explicit loops and stores the result in res.
 *
 * @param res Pointer to the output matrix2.
 * @param a Pointer to the first matrix.
 * @param b Pointer to the second matrix.
 */
void mat2_mul_ptr(matrix2 *res, const matrix2 *a, const matrix2 *b)
{
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            res->v[i * 2 + j] = 0.0f;
            for (int k = 0; k < 2; k++) {
                res->v[i * 2 + j] += a->v[i * 2 + k] * b->v[k * 2 + j];
            }
        }
    }
}

/**
 * @brief Computes the transpose of the input 2x2 matrix and stores in res.
 *
 * @param res Pointer to the output matrix2.
 * @param m Pointer to the input matrix.
 */
void mat2_transpose_ptr(matrix2 *res, const matrix2 *m)
{
    *res = (matrix2){
        .m11 = m->v[0], .m21 = m->v[2],
        .m12 = m->v[1], .m22 = m->v[3]
    };
}


/**
 * @brief Computes the inverse of the input 2x2 matrix using determinant and stores in res.
 *
 * If the determinant is zero, sets res to identity matrix.
 *
 * @param res Pointer to the output matrix2.
 * @param m Pointer to the input matrix.
 */
void mat2_inverse_ptr(matrix2 *res, const matrix2 *m)
{
    const vm_float_t det = mat2_determinant(*m);
    if (det == 0.0f) {
        mat2_identity_ptr(res);
        return;
    }
    const vm_float_t inv_det = 1.0f / det;
    *res = (matrix2){
        .m11 =  m->v[3] * inv_det, .m21 = -m->v[1] * inv_det,
        .m12 = -m->v[2] * inv_det, .m22 =  m->v[0] * inv_det
    };
}

/**
 * @brief Multiplies a 2x2 matrix by a vector2.
 *
 * @param res Output value.
 * @param m Input matrix.
 * @param v Input vector.
 */
void mat2_mul_vec2_ptr(vector2 *res, const matrix2 *m, const vector2 *v)
{
    const vm_float_t x = v->x;
    const vm_float_t y = v->y;
    res->x = m->m11 * x + m->m12 * y;
    res->y = m->m21 * x + m->m22 * y;
}

/**
 * @brief  Sets res to a 2x2 rotation matrix for rotation around the Z-axis.
 *
 * Converts degrees to radians; positive is counter-clockwise.
 *
 * @param res Pointer to the output matrix2.
 * @param degrees Rotation angle in degrees.
 */
void mat2_rotation_z_ptr(matrix2 *res, const vm_float_t degrees)
{
    const vm_float_t radians = deg_to_rad(degrees);
    const vm_float_t cos_theta = VECMAT_COS(radians);
    const vm_float_t sin_theta = VECMAT_SIN(radians);
    *res = (matrix2){
        .m11 = cos_theta,  .m21 = sin_theta,
        .m12 = -sin_theta, .m22 = cos_theta
    };
}

/**
 * @brief Builds a 2x2 scaling matrix from a vector2.
 *
 * @param res Output value.
 * @param s Scale vector.
 */
void mat2_scale_ptr(matrix2 *res, const vector2 *s)
{
    *res = (matrix2){
        .m11 = s->x, .m21 = 0.0f,
        .m12 = 0.0f, .m22 = s->y
    };
}

/**
 * @brief Copies the upper-left 2x2 of a matrix3.
 *
 * @param res Output value.
 * @param m Input matrix.
 */
void mat2_from_mat3_ptr(matrix2 *res, const matrix3 *m)
{
    *res = (matrix2){
        .m11 = m->m11, .m21 = m->m21,
        .m12 = m->m12, .m22 = m->m22
    };
}
