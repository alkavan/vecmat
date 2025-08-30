// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

/**
 * @brief Initializes the integer 2x2 matrix to identity (diagonal 1, others 0).
 *
 * @param res Pointer to the output matrix2i.
 */
void mat2i_identity_ptr(matrix2i *res)
{
    *res = (matrix2i){0};
    res->v[0] = 1;
    res->v[3] = 1;
}

/**
 * @brief Multiplies two integer 2x2 matrices (a * b) using explicit loops and
 * stores the result in res.
 *
 * @param res Pointer to the output matrix2i.
 * @param a Pointer to the first matrix.
 * @param b Pointer to the second matrix.
 */
void mat2i_mul_ptr(matrix2i *res, const matrix2i *a, const matrix2i *b)
{
    const matrix2i tmp = {
        .m11 = a->m11 * b->m11 + a->m12 * b->m21,
        .m21 = a->m21 * b->m11 + a->m22 * b->m21,
        .m12 = a->m11 * b->m12 + a->m12 * b->m22,
        .m22 = a->m21 * b->m12 + a->m22 * b->m22
    };
    *res = tmp;
}

/**
 * @brief Computes the transpose of the input integer 2x2 matrix and stores in res.
 *
 * @param res Pointer to the output matrix2i.
 * @param m Pointer to the input matrix.
 */
void mat2i_transpose_ptr(matrix2i *res, const matrix2i *m)
{
    *res = (matrix2i){
        .m11 = m->v[0], .m21 = m->v[2],
        .m12 = m->v[1], .m22 = m->v[3]
    };
}

/**
 * @brief Computes the inverse of the input integer 2x2 matrix and stores in res.
 *
 * Uses double precision for inversion, truncates to int; sets to identity if det=0.
 *
 * @param res Pointer to the output matrix2i.
 * @param m Pointer to the input matrix.
 */
void mat2i_inverse_ptr(matrix2i *res, const matrix2i *m)
{
    const vm_int_t det = mat2i_determinant(*m);
    if (det == 0) {
        mat2i_identity_ptr(res);
        return;
    }
    const double inv_det = 1.0 / (double)det;
    *res = (matrix2i){
        .m11 = (int)(m->v[3] * inv_det),  .m21 = (int)(-m->v[1] * inv_det),
        .m12 = (int)(-m->v[2] * inv_det), .m22 = (int)(m->v[0] * inv_det)
    };
}

/**
 * @brief Multiplies a 2x2 integer matrix by a vector2i.
 *
 * @param res Output value.
 * @param m Input matrix.
 * @param v Input vector.
 */
void mat2i_mul_vec2i_ptr(vector2i *res, const matrix2i *m, const vector2i *v)
{
    const vm_int_t x = v->x;
    const vm_int_t y = v->y;
    res->x = m->m11 * x + m->m12 * y;
    res->y = m->m21 * x + m->m22 * y;
}
