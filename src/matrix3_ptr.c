// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

/**
 * @brief Initializes the 3x3 matrix to identity (diagonal 1.0, others 0.0).
 *
 * @param res Pointer to the output matrix3.
 */
void mat3_identity_ptr(matrix3 *res)
{
    res->v[0] = 1.0f;
    res->v[1] = 0.0f;
    res->v[2] = 0.0f;
    res->v[3] = 0.0f;
    res->v[4] = 1.0f;
    res->v[5] = 0.0f;
    res->v[6] = 0.0f;
    res->v[7] = 0.0f;
    res->v[8] = 1.0f;
}

/**
 * @brief Multiplies two 3x3 matrices (a * b) in column-major / column-vector convention.
 *
 * Accumulates into a temporary matrix.
 *
 * @param res Pointer to the output matrix3.
 * @param a Pointer to the first matrix.
 * @param b Pointer to the second matrix.
 */
void mat3_mul_ptr(matrix3 *res, const matrix3 *a, const matrix3 *b)
{
    matrix3 temp;
    temp.m11 = a->m11 * b->m11 + a->m12 * b->m21 + a->m13 * b->m31;
    temp.m12 = a->m11 * b->m12 + a->m12 * b->m22 + a->m13 * b->m32;
    temp.m13 = a->m11 * b->m13 + a->m12 * b->m23 + a->m13 * b->m33;

    temp.m21 = a->m21 * b->m11 + a->m22 * b->m21 + a->m23 * b->m31;
    temp.m22 = a->m21 * b->m12 + a->m22 * b->m22 + a->m23 * b->m32;
    temp.m23 = a->m21 * b->m13 + a->m22 * b->m23 + a->m23 * b->m33;

    temp.m31 = a->m31 * b->m11 + a->m32 * b->m21 + a->m33 * b->m31;
    temp.m32 = a->m31 * b->m12 + a->m32 * b->m22 + a->m33 * b->m32;
    temp.m33 = a->m31 * b->m13 + a->m32 * b->m23 + a->m33 * b->m33;

    *res = temp;
}

/**
 * @brief Computes the transpose of the input 3x3 matrix and stores in res.
 *
 * @param res Pointer to the output matrix3.
 * @param m Pointer to the input matrix.
 */
void mat3_transpose_ptr(matrix3 *res, const matrix3 *m)
{
    matrix3 temp;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            temp.v[i * 3 + j] = m->v[j * 3 + i];
        }
    }
    *res = temp;
}

/**
 * @brief Computes the inverse of the input 3x3 matrix using the adjugate method
 * and stores in res.
 *
 * If the determinant is zero, sets res to identity.
 *
 * @param res Pointer to the output matrix3.
 * @param m Pointer to the input matrix.
 */
void mat3_inverse_ptr(matrix3 *res, const matrix3 *m)
{
    // Compute determinant
    const vm_float_t det = m->v[0] * (m->v[4] * m->v[8] - m->v[5] * m->v[7]) -
                           m->v[1] * (m->v[3] * m->v[8] - m->v[5] * m->v[6]) +
                           m->v[2] * (m->v[3] * m->v[7] - m->v[4] * m->v[6]);

    if (det == 0.0f) {
        mat3_identity_ptr(res);
        return;
    }

    const vm_float_t inv_det = 1.0f / det;

    matrix3 inv;
    inv.v[0] = (m->v[4] * m->v[8] - m->v[5] * m->v[7]) * inv_det;
    inv.v[1] = (m->v[2] * m->v[7] - m->v[1] * m->v[8]) * inv_det;
    inv.v[2] = (m->v[1] * m->v[5] - m->v[2] * m->v[4]) * inv_det;
    inv.v[3] = (m->v[5] * m->v[6] - m->v[3] * m->v[8]) * inv_det;
    inv.v[4] = (m->v[0] * m->v[8] - m->v[2] * m->v[6]) * inv_det;
    inv.v[5] = (m->v[2] * m->v[3] - m->v[0] * m->v[5]) * inv_det;
    inv.v[6] = (m->v[3] * m->v[7] - m->v[4] * m->v[6]) * inv_det;
    inv.v[7] = (m->v[1] * m->v[6] - m->v[0] * m->v[7]) * inv_det;
    inv.v[8] = (m->v[0] * m->v[4] - m->v[1] * m->v[3]) * inv_det;
    *res = inv;
}

/**
 * @brief Sets the 3x3 matrix to a rotation around the Z-axis by the given angle
 * in degrees.
 *
 * @param res Pointer to the output matrix3.
 * @param degrees Rotation angle in degrees.
 */
void mat3_rotation_z_ptr(matrix3 *res, const vm_float_t degrees)
{
    const vm_float_t rad = degrees * (vm_float_t)(M_PI / 180.0);
    const vm_float_t c = VECMAT_COS(rad);
    const vm_float_t s = VECMAT_SIN(rad);

    res->m11 = c;
    res->m12 = -s;
    res->m13 = 0.0f;

    res->m21 = s;
    res->m22 = c;
    res->m23 = 0.0f;

    res->m31 = 0.0f;
    res->m32 = 0.0f;
    res->m33 = 1.0f;
}

/**
 * @brief Builds a 3x3 rotation matrix around the X axis (degrees).
 *
 * @param res Output value.
 * @param degrees Rotation angle in degrees.
 */
void mat3_rotation_x_ptr(matrix3 *res, const vm_float_t degrees)
{
    const vm_float_t rad = deg_to_rad(degrees);
    const vm_float_t c = VECMAT_COS(rad);
    const vm_float_t s = VECMAT_SIN(rad);
    *res = (matrix3){
        .m11 = 1.0f, .m21 = 0.0f, .m31 = 0.0f,
        .m12 = 0.0f, .m22 = c,    .m32 = s,
        .m13 = 0.0f, .m23 = -s,   .m33 = c
    };
}

/**
 * @brief Builds a 3x3 rotation matrix around the Y axis (degrees).
 *
 * @param res Output value.
 * @param degrees Rotation angle in degrees.
 */
void mat3_rotation_y_ptr(matrix3 *res, const vm_float_t degrees)
{
    const vm_float_t rad = deg_to_rad(degrees);
    const vm_float_t c = VECMAT_COS(rad);
    const vm_float_t s = VECMAT_SIN(rad);
    *res = (matrix3){
        .m11 = c,    .m21 = 0.0f, .m31 = -s,
        .m12 = 0.0f, .m22 = 1.0f, .m32 = 0.0f,
        .m13 = s,    .m23 = 0.0f, .m33 = c
    };
}

/**
 * @brief Builds a 3x3 2D translation matrix.
 *
 * @param res Output value.
 * @param t Translation vector.
 */
void mat3_translate_ptr(matrix3 *res, const vector2 *t)
{
    mat3_identity_ptr(res);
    res->m13 = t->x;
    res->m23 = t->y;
}

/**
 * @brief Builds a 3x3 2D scaling matrix.
 *
 * @param res Output value.
 * @param s Scale vector.
 */
void mat3_scale_ptr(matrix3 *res, const vector2 *s)
{
    mat3_identity_ptr(res);
    res->m11 = s->x;
    res->m22 = s->y;
}

/**
 * @brief Copies the upper-left 3x3 of a matrix4.
 *
 * @param res Output value.
 * @param m Input matrix.
 */
void mat3_from_mat4_ptr(matrix3 *res, const matrix4 *m)
{
    res->m11 = m->m11; res->m21 = m->m21; res->m31 = m->m31;
    res->m12 = m->m12; res->m22 = m->m22; res->m32 = m->m32;
    res->m13 = m->m13; res->m23 = m->m23; res->m33 = m->m33;
}

/**
 * @brief Multiplies a 3x3 matrix by a vector3.
 *
 * @param res Output value.
 * @param m Input matrix.
 * @param v Input vector.
 */
void mat3_mul_vec3_ptr(vector3 *res, const matrix3 *m, const vector3 *v)
{
    const vm_float_t x = v->x;
    const vm_float_t y = v->y;
    const vm_float_t z = v->z;
    res->x = m->m11 * x + m->m12 * y + m->m13 * z;
    res->y = m->m21 * x + m->m22 * y + m->m23 * z;
    res->z = m->m31 * x + m->m32 * y + m->m33 * z;
}

/**
 * @brief Applies a 3x3 affine transform to a vector2.
 *
 * @param res Output value.
 * @param m Input matrix.
 * @param v Input vector.
 */
void mat3_mul_vec2_ptr(vector2 *res, const matrix3 *m, const vector2 *v)
{
    const vm_float_t x = v->x;
    const vm_float_t y = v->y;
    res->x = m->m11 * x + m->m12 * y + m->m13;
    res->y = m->m21 * x + m->m22 * y + m->m23;
}
