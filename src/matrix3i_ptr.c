// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

/**
 * @brief Fills the given 3x3 integer matrix with the identity matrix.
 *
 * @param res Pointer to the result matrix.
 */
void mat3i_identity_ptr(matrix3i *res)
{
    res->m11 = 1; res->m12 = 0; res->m13 = 0;
    res->m21 = 0; res->m22 = 1; res->m23 = 0;
    res->m31 = 0; res->m32 = 0; res->m33 = 1;
}

/**
 * @brief Multiplies two 3x3 integer matrices (standard matrix multiplication).
 *
 * @param res Pointer to the result matrix.
 * @param a   Pointer to the first input matrix.
 * @param b   Pointer to the second input matrix.
 */
void mat3i_mul_ptr(matrix3i *res, const matrix3i *a, const matrix3i *b)
{
    matrix3i temp;
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
 * @brief Computes the transpose of a 3x3 integer matrix.
 *
 * @param res Pointer to the result matrix.
 * @param m   Pointer to the input matrix.
 */
void mat3i_transpose_ptr(matrix3i *res, const matrix3i *m)
{
    matrix3i temp;
    temp.m11 = m->m11; temp.m12 = m->m21; temp.m13 = m->m31;
    temp.m21 = m->m12; temp.m22 = m->m22; temp.m23 = m->m32;
    temp.m31 = m->m13; temp.m32 = m->m23; temp.m33 = m->m33;
    *res = temp;
}

/**
 * @brief Computes the inverse of a 3x3 integer matrix using adjugate over determinant.
 *
 * If the determinant is zero (singular), sets the result to the identity matrix.
 *
 * @param res Pointer to the result matrix.
 * @param m   Pointer to the input matrix.
 */
void mat3i_inverse_ptr(matrix3i *res, const matrix3i *m)
{
    const vm_int_t det = mat3i_determinant(*m);
    if (det == 0) {
        mat3i_identity_ptr(res);
        return;
    }

    matrix3i temp;
    temp.m11 = (m->m22 * m->m33 - m->m23 * m->m32) / det;
    temp.m12 = (m->m13 * m->m32 - m->m12 * m->m33) / det;
    temp.m13 = (m->m12 * m->m23 - m->m13 * m->m22) / det;

    temp.m21 = (m->m23 * m->m31 - m->m21 * m->m33) / det;
    temp.m22 = (m->m11 * m->m33 - m->m13 * m->m31) / det;
    temp.m23 = (m->m13 * m->m21 - m->m11 * m->m23) / det;

    temp.m31 = (m->m21 * m->m32 - m->m22 * m->m31) / det;
    temp.m32 = (m->m12 * m->m31 - m->m11 * m->m32) / det;
    temp.m33 = (m->m11 * m->m22 - m->m12 * m->m21) / det;

    *res = temp;
}

/**
 * @brief Multiplies a 3x3 integer matrix by a vector3i.
 *
 * @param res Output value.
 * @param m Input matrix.
 * @param v Input vector.
 */
void mat3i_mul_vec3i_ptr(vector3i *res, const matrix3i *m, const vector3i *v)
{
    const vm_int_t x = v->x;
    const vm_int_t y = v->y;
    const vm_int_t z = v->z;
    res->x = m->m11 * x + m->m12 * y + m->m13 * z;
    res->y = m->m21 * x + m->m22 * y + m->m23 * z;
    res->z = m->m31 * x + m->m32 * y + m->m33 * z;
}

/**
 * @brief Applies a 3x3 integer affine transform to a vector2i.
 *
 * @param res Output value.
 * @param m Input matrix.
 * @param v Input vector.
 */
void mat3i_mul_vec2i_ptr(vector2i *res, const matrix3i *m, const vector2i *v)
{
    const vm_int_t x = v->x;
    const vm_int_t y = v->y;
    res->x = m->m11 * x + m->m12 * y + m->m13;
    res->y = m->m21 * x + m->m22 * y + m->m23;
}
