// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

matrix2i mat2i_identity(void)
{
    matrix2i res;
    mat2i_identity_ptr(&res);
    return res;
}

matrix2i mat2i_mul(const matrix2i a, const matrix2i b)
{
    matrix2i res;
    mat2i_mul_ptr(&res, &a, &b);
    return res;
}

matrix2i mat2i_transpose(const matrix2i m)
{
    matrix2i res;
    mat2i_transpose_ptr(&res, &m);
    return res;
}

matrix2i mat2i_inverse(const matrix2i m)
{
    matrix2i res;
    mat2i_inverse_ptr(&res, &m);
    return res;
}

vm_int_t mat2i_determinant(const matrix2i m)
{
    return m.v[0] * m.v[3] - m.v[1] * m.v[2];
}

vector2i mat2i_mul_vec2i(const matrix2i m, const vector2i v)
{
    vector2i res;
    mat2i_mul_vec2i_ptr(&res, &m, &v);
    return res;
}
