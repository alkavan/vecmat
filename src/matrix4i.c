// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

matrix4i mat4i_identity(void)
{
    matrix4i res;
    mat4i_identity_ptr(&res);
    return res;
}

matrix4i mat4i_mul(const matrix4i a, const matrix4i b)
{
    matrix4i res;
    mat4i_mul_ptr(&res, &a, &b);
    return res;
}

matrix4i mat4i_transpose(const matrix4i m)
{
    matrix4i res;
    mat4i_transpose_ptr(&res, &m);
    return res;
}

matrix4i mat4i_inverse(const matrix4i m)
{
    matrix4i res;
    mat4i_inverse_ptr(&res, &m);
    return res;
}

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

vector4i mat4i_mul_vec4i(const matrix4i m, const vector4i v)
{
    vector4i res;
    mat4i_mul_vec4i_ptr(&res, &m, &v);
    return res;
}

vector3i mat4i_mul_vec3i(const matrix4i m, const vector3i v, const vm_int_t w)
{
    vector3i res;
    mat4i_mul_vec3i_ptr(&res, &m, &v, w);
    return res;
}
