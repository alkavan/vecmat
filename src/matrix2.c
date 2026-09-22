// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

matrix2 mat2_identity(void)
{
    matrix2 res;
    mat2_identity_ptr(&res);
    return res;
}

matrix2 mat2_mul(const matrix2 a, const matrix2 b)
{
    matrix2 res;
    mat2_mul_ptr(&res, &a, &b);
    return res;
}

matrix2 mat2_transpose(const matrix2 m)
{
    matrix2 res;
    mat2_transpose_ptr(&res, &m);
    return res;
}

matrix2 mat2_inverse(const matrix2 m)
{
    matrix2 res;
    mat2_inverse_ptr(&res, &m);
    return res;
}

matrix2 mat2_rotation_z(const vm_float_t radians)
{
    matrix2 res;
    mat2_rotation_z_ptr(&res, radians);
    return res;
}

matrix2 mat2_rotation_z_deg(const vm_float_t degrees)
{
    return mat2_rotation_z(deg_to_rad(degrees));
}

vm_float_t mat2_determinant(const matrix2 m)
{
    return m.v[0] * m.v[3] - m.v[1] * m.v[2];
}

vector2 mat2_mul_vec2(const matrix2 m, const vector2 v)
{
    vector2 res;
    mat2_mul_vec2_ptr(&res, &m, &v);
    return res;
}

matrix2 mat2_scale(const vector2 s)
{
    matrix2 res;
    mat2_scale_ptr(&res, &s);
    return res;
}

matrix2 mat2_from_mat3(const matrix3 m)
{
    matrix2 res;
    mat2_from_mat3_ptr(&res, &m);
    return res;
}
