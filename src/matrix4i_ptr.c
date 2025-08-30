// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

/**
 * @brief Sets the given 4x4 integer matrix to the identity matrix.
 *
 * @param res Pointer to the matrix structure to set to identity.
 */
void mat4i_identity_ptr(matrix4i *res)
{
    matrix4i m = {0};
    m.v[0] = 1;
    m.v[5] = 1;
    m.v[10] = 1;
    m.v[15] = 1;
    *res = m;
}

/**
 * @brief Performs matrix multiplication of two 4x4 matrices and stores the result.
 *
 * @param res Pointer to the matrix structure to store the result of the multiplication.
 * @param a Pointer to the first matrix operand.
 * @param b Pointer to the second matrix operand.
 */
void mat4i_mul_ptr(matrix4i *res, const matrix4i *a, const matrix4i *b)
{
    matrix4i r;
    r.m11 = a->m11 * b->m11 + a->m12 * b->m21 + a->m13 * b->m31 + a->m14 * b->m41;
    r.m21 = a->m21 * b->m11 + a->m22 * b->m21 + a->m23 * b->m31 + a->m24 * b->m41;
    r.m31 = a->m31 * b->m11 + a->m32 * b->m21 + a->m33 * b->m31 + a->m34 * b->m41;
    r.m41 = a->m41 * b->m11 + a->m42 * b->m21 + a->m43 * b->m31 + a->m44 * b->m41;

    r.m12 = a->m11 * b->m12 + a->m12 * b->m22 + a->m13 * b->m32 + a->m14 * b->m42;
    r.m22 = a->m21 * b->m12 + a->m22 * b->m22 + a->m23 * b->m32 + a->m24 * b->m42;
    r.m32 = a->m31 * b->m12 + a->m32 * b->m22 + a->m33 * b->m32 + a->m34 * b->m42;
    r.m42 = a->m41 * b->m12 + a->m42 * b->m22 + a->m43 * b->m32 + a->m44 * b->m42;

    r.m13 = a->m11 * b->m13 + a->m12 * b->m23 + a->m13 * b->m33 + a->m14 * b->m43;
    r.m23 = a->m21 * b->m13 + a->m22 * b->m23 + a->m23 * b->m33 + a->m24 * b->m43;
    r.m33 = a->m31 * b->m13 + a->m32 * b->m23 + a->m33 * b->m33 + a->m34 * b->m43;
    r.m43 = a->m41 * b->m13 + a->m42 * b->m23 + a->m43 * b->m33 + a->m44 * b->m43;

    r.m14 = a->m11 * b->m14 + a->m12 * b->m24 + a->m13 * b->m34 + a->m14 * b->m44;
    r.m24 = a->m21 * b->m14 + a->m22 * b->m24 + a->m23 * b->m34 + a->m24 * b->m44;
    r.m34 = a->m31 * b->m14 + a->m32 * b->m24 + a->m33 * b->m34 + a->m34 * b->m44;
    r.m44 = a->m41 * b->m14 + a->m42 * b->m24 + a->m43 * b->m34 + a->m44 * b->m44;
    *res = r;
}

/**
 * @brief Transposes the given 4x4 integer matrix and stores the result in the
 * provided matrix structure.
 *
 * @param res Pointer to the matrix structure to store the transposed matrix.
 * @param m Pointer to the input matrix to be transposed.
 */
void mat4i_transpose_ptr(matrix4i *res, const matrix4i *m)
{
    matrix4i r;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            r.v[i * 4 + j] = m->v[j * 4 + i];
        }
    }
    *res = r;
}

/**
 * @brief Computes the inverse of the given 4x4 integer matrix and stores the
 * result in the specified matrix.
 *
 * If the matrix is singular (determinant is zero), the result is set to the identity matrix.
 *
 * @param res Pointer to the matrix structure to store the inverse matrix.
 * @param m Pointer to the constant matrix to be inverted.
 */
void mat4i_inverse_ptr(matrix4i *res, const matrix4i *m)
{
    int inv[16];

    inv[0]  =  m->v[5] * m->v[10] * m->v[15] - m->v[5] * m->v[14] * m->v[11] - m->v[9] * m->v[6] * m->v[15] + m->v[9] * m->v[14] * m->v[7] + m->v[13] * m->v[6] * m->v[11] - m->v[13] * m->v[10] * m->v[7];
    inv[4]  = -m->v[4] * m->v[10] * m->v[15] + m->v[4] * m->v[14] * m->v[11] + m->v[8] * m->v[6] * m->v[15] - m->v[8] * m->v[14] * m->v[7] - m->v[12] * m->v[6] * m->v[11] + m->v[12] * m->v[10] * m->v[7];
    inv[8]  =  m->v[4] * m->v[9]  * m->v[15] - m->v[4] * m->v[13] * m->v[11] - m->v[8] * m->v[5] * m->v[15] + m->v[8] * m->v[13] * m->v[7] + m->v[12] * m->v[5] * m->v[11] - m->v[12] * m->v[9]  * m->v[7];
    inv[12] = -m->v[4] * m->v[9]  * m->v[14] + m->v[4] * m->v[13] * m->v[10] + m->v[8] * m->v[5] * m->v[14] - m->v[8] * m->v[13] * m->v[6] - m->v[12] * m->v[5] * m->v[10] + m->v[12] * m->v[9]  * m->v[6];
    inv[1]  = -m->v[1] * m->v[10] * m->v[15] + m->v[1] * m->v[14] * m->v[11] + m->v[9] * m->v[2] * m->v[15] - m->v[9] * m->v[14] * m->v[3] - m->v[13] * m->v[2] * m->v[11] + m->v[13] * m->v[10] * m->v[3];
    inv[5]  =  m->v[0] * m->v[10] * m->v[15] - m->v[0] * m->v[14] * m->v[11] - m->v[8] * m->v[2] * m->v[15] + m->v[8] * m->v[14] * m->v[3] + m->v[12] * m->v[2] * m->v[11] - m->v[12] * m->v[10] * m->v[3];
    inv[9]  = -m->v[0] * m->v[9]  * m->v[15] + m->v[0] * m->v[13] * m->v[11] + m->v[8] * m->v[1] * m->v[15] - m->v[8] * m->v[13] * m->v[3] - m->v[12] * m->v[1] * m->v[11] + m->v[12] * m->v[9]  * m->v[3];
    inv[13] =  m->v[0] * m->v[9]  * m->v[14] - m->v[0] * m->v[13] * m->v[10] - m->v[8] * m->v[1] * m->v[14] + m->v[8] * m->v[13] * m->v[2] + m->v[12] * m->v[1] * m->v[10] - m->v[12] * m->v[9]  * m->v[2];
    inv[2]  =  m->v[1] * m->v[6]  * m->v[15] - m->v[1] * m->v[14] * m->v[7]  - m->v[5] * m->v[2] * m->v[15] + m->v[5] * m->v[14] * m->v[3] + m->v[13] * m->v[2] * m->v[7]  - m->v[13] * m->v[6]  * m->v[3];
    inv[6]  = -m->v[0] * m->v[6]  * m->v[15] + m->v[0] * m->v[14] * m->v[7]  + m->v[4] * m->v[2] * m->v[15] - m->v[4] * m->v[14] * m->v[3] - m->v[12] * m->v[2] * m->v[7]  + m->v[12] * m->v[6]  * m->v[3];
    inv[10] =  m->v[0] * m->v[5]  * m->v[15] - m->v[0] * m->v[13] * m->v[7]  - m->v[4] * m->v[1] * m->v[15] + m->v[4] * m->v[13] * m->v[3] + m->v[12] * m->v[1] * m->v[7]  - m->v[12] * m->v[5]  * m->v[3];
    inv[14] = -m->v[0] * m->v[5]  * m->v[14] + m->v[0] * m->v[13] * m->v[6]  + m->v[4] * m->v[1] * m->v[14] - m->v[4] * m->v[13] * m->v[2] - m->v[12] * m->v[1] * m->v[6]  + m->v[12] * m->v[5]  * m->v[2];
    inv[3]  = -m->v[1] * m->v[6]  * m->v[11] + m->v[1] * m->v[10] * m->v[7]  + m->v[5] * m->v[2] * m->v[11] - m->v[5] * m->v[10] * m->v[3] - m->v[9]  * m->v[2] * m->v[7]  + m->v[9]  * m->v[6]  * m->v[3];
    inv[7]  =  m->v[0] * m->v[6]  * m->v[11] - m->v[0] * m->v[10] * m->v[7]  - m->v[4] * m->v[2] * m->v[11] + m->v[4] * m->v[10] * m->v[3] + m->v[8]  * m->v[2] * m->v[7]  - m->v[8]  * m->v[6]  * m->v[3];
    inv[11] = -m->v[0] * m->v[5]  * m->v[11] + m->v[0] * m->v[9]  * m->v[7]  + m->v[4] * m->v[1] * m->v[11] - m->v[4] * m->v[9]  * m->v[3] - m->v[8]  * m->v[1] * m->v[7]  + m->v[8]  * m->v[5]  * m->v[3];
    inv[15] =  m->v[0] * m->v[5]  * m->v[10] - m->v[0] * m->v[9]  * m->v[6]  - m->v[4] * m->v[1] * m->v[10] + m->v[4] * m->v[9]  * m->v[2] + m->v[8]  * m->v[1] * m->v[6]  - m->v[8]  * m->v[5]  * m->v[2];

    const vm_int_t det = m->v[0] * inv[0] + m->v[1] * inv[4] + m->v[2] * inv[8] + m->v[3] * inv[12];
    if (det == 0) {
        matrix4i id = {0};
        id.v[0] = 1;
        id.v[5] = 1;
        id.v[10] = 1;
        id.v[15] = 1;
        *res = id;
        return;
    }

    const double fdet = 1.0f / (vm_float_t)det;

    matrix4i result;
    for (int i = 0; i < 16; i++) {
        result.v[i] = (int)(inv[i] * fdet);
    }
    *res = result;
}

/**
 * @brief Multiplies a 4x4 integer matrix by a vector4i.
 *
 * @param res Output value.
 * @param m Input matrix.
 * @param v Input vector.
 */
void mat4i_mul_vec4i_ptr(vector4i *res, const matrix4i *m, const vector4i *v)
{
    const vm_int_t x = v->x;
    const vm_int_t y = v->y;
    const vm_int_t z = v->z;
    const vm_int_t w = v->w;
    res->x = m->m11 * x + m->m12 * y + m->m13 * z + m->m14 * w;
    res->y = m->m21 * x + m->m22 * y + m->m23 * z + m->m24 * w;
    res->z = m->m31 * x + m->m32 * y + m->m33 * z + m->m34 * w;
    res->w = m->m41 * x + m->m42 * y + m->m43 * z + m->m44 * w;
}

/**
 * @brief Transforms a vector3i by a 4x4 integer matrix using homogeneous w.
 *
 * @param res Output value.
 * @param m Input matrix.
 * @param v Input vector.
 * @param w Homogeneous w component.
 */
void mat4i_mul_vec3i_ptr(vector3i *res, const matrix4i *m, const vector3i *v, const vm_int_t w)
{
    const vm_int_t x = v->x;
    const vm_int_t y = v->y;
    const vm_int_t z = v->z;
    res->x = m->m11 * x + m->m12 * y + m->m13 * z + m->m14 * w;
    res->y = m->m21 * x + m->m22 * y + m->m23 * z + m->m24 * w;
    res->z = m->m31 * x + m->m32 * y + m->m33 * z + m->m34 * w;
}
