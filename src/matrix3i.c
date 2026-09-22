// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

matrix3i mat3i_identity(void)
{
    matrix3i m;
    mat3i_identity_ptr(&m);
    return m;
}

matrix3i mat3i_mul(const matrix3i a, const matrix3i b)
{
    matrix3i res;
    mat3i_mul_ptr(&res, &a, &b);
    return res;
}

matrix3i mat3i_transpose(const matrix3i m)
{
    matrix3i res;
    mat3i_transpose_ptr(&res, &m);
    return res;
}

vm_int_t mat3i_determinant(const matrix3i m)
{
    return m.v[0] * (m.v[4] * m.v[8] - m.v[5] * m.v[7]) -
           m.v[1] * (m.v[3] * m.v[8] - m.v[5] * m.v[6]) +
           m.v[2] * (m.v[3] * m.v[7] - m.v[4] * m.v[6]);
}

matrix3i mat3i_inverse(const matrix3i m)
{
    matrix3i res;
    mat3i_inverse_ptr(&res, &m);
    return res;
}

vector3i mat3i_mul_vec3i(const matrix3i m, const vector3i v)
{
    vector3i res;
    mat3i_mul_vec3i_ptr(&res, &m, &v);
    return res;
}

vector2i mat3i_mul_vec2i(const matrix3i m, const vector2i v)
{
    vector2i res;
    mat3i_mul_vec2i_ptr(&res, &m, &v);
    return res;
}
