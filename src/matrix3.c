// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

matrix3 mat3_identity(void)
{
    matrix3 m;
    mat3_identity_ptr(&m);
    return m;
}

matrix3 mat3_mul(const matrix3 a, const matrix3 b)
{
    matrix3 res;
    mat3_mul_ptr(&res, &a, &b);
    return res;
}

matrix3 mat3_transpose(const matrix3 m)
{
    matrix3 res;
    mat3_transpose_ptr(&res, &m);
    return res;
}

matrix3 mat3_inverse(const matrix3 m)
{
    matrix3 res;
    mat3_inverse_ptr(&res, &m);
    return res;
}

matrix3 mat3_rotation_z(const vm_float_t radians)
{
    matrix3 m;
    mat3_rotation_z_ptr(&m, radians);
    return m;
}

matrix3 mat3_rotation_z_deg(const vm_float_t degrees)
{
    return mat3_rotation_z(deg_to_rad(degrees));
}

vm_float_t mat3_determinant(const matrix3 m)
{
    return m.v[0] * (m.v[4] * m.v[8] - m.v[5] * m.v[7]) -
           m.v[1] * (m.v[3] * m.v[8] - m.v[5] * m.v[6]) +
           m.v[2] * (m.v[3] * m.v[7] - m.v[4] * m.v[6]);
}

vector3 mat3_mul_vec3(const matrix3 m, const vector3 v)
{
    vector3 res;
    mat3_mul_vec3_ptr(&res, &m, &v);
    return res;
}

vector2 mat3_mul_vec2(const matrix3 m, const vector2 v)
{
    vector2 res;
    mat3_mul_vec2_ptr(&res, &m, &v);
    return res;
}

matrix3 mat3_rotation_x(const vm_float_t radians)
{
    matrix3 res;
    mat3_rotation_x_ptr(&res, radians);
    return res;
}

matrix3 mat3_rotation_x_deg(const vm_float_t degrees)
{
    return mat3_rotation_x(deg_to_rad(degrees));
}

matrix3 mat3_rotation_y(const vm_float_t radians)
{
    matrix3 res;
    mat3_rotation_y_ptr(&res, radians);
    return res;
}

matrix3 mat3_rotation_y_deg(const vm_float_t degrees)
{
    return mat3_rotation_y(deg_to_rad(degrees));
}

matrix3 mat3_translate(const vector2 t)
{
    matrix3 res;
    mat3_translate_ptr(&res, &t);
    return res;
}

matrix3 mat3_scale(const vector2 s)
{
    matrix3 res;
    mat3_scale_ptr(&res, &s);
    return res;
}

matrix3 mat3_from_mat4(const matrix4 m)
{
    matrix3 res;
    mat3_from_mat4_ptr(&res, &m);
    return res;
}

matrix3 mat3_normal(const matrix3 m)
{
    matrix3 res;
    mat3_normal_ptr(&res, &m);
    return res;
}

vector3 mat3_sym_eigen(const matrix3 m, matrix3 *axes)
{
    vector3 ev;
    matrix3 local_axes;
    matrix3 *out_axes = axes ? axes : &local_axes;
    mat3_sym_eigen_ptr(&ev, out_axes, &m);
    return ev;
}
