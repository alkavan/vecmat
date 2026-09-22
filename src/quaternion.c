// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

quaternion quat_identity(void)
{
    quaternion res;
    quat_identity_ptr(&res);
    return res;
}

quaternion quat_mul(const quaternion a, const quaternion b)
{
    quaternion res;
    quat_mul_ptr(&res, &a, &b);
    return res;
}

quaternion quat_normalize(const quaternion q)
{
    quaternion res;
    quat_normalize_ptr(&res, &q);
    return res;
}

quaternion quat_from_euler(const vector3 euler)
{
    quaternion res;
    quat_from_euler_ptr(&res, &euler);
    return res;
}

quaternion quat_from_euler_deg(const vector3 euler_deg)
{
    quaternion res;
    quat_from_euler_deg_ptr(&res, &euler_deg);
    return res;
}

matrix4 quat_to_mat4(const quaternion q)
{
    matrix4 res;
    quat_to_mat4_ptr(&res, &q);
    return res;
}

quaternion quat_conjugate(const quaternion q)
{
    quaternion res;
    quat_conjugate_ptr(&res, &q);
    return res;
}

quaternion quat_inverse(const quaternion q)
{
    quaternion res;
    quat_inverse_ptr(&res, &q);
    return res;
}

quaternion quat_from_axis_angle(const vector3 axis, const vm_float_t radians)
{
    quaternion res;
    quat_from_axis_angle_ptr(&res, &axis, radians);
    return res;
}

quaternion quat_from_axis_angle_deg(const vector3 axis, const vm_float_t degrees)
{
    quaternion res;
    quat_from_axis_angle_deg_ptr(&res, &axis, degrees);
    return res;
}

quaternion quat_from_mat3(const matrix3 m)
{
    quaternion res;
    quat_from_mat3_ptr(&res, &m);
    return res;
}

quaternion quat_from_mat4(const matrix4 m)
{
    quaternion res;
    quat_from_mat4_ptr(&res, &m);
    return res;
}

quaternion quat_slerp(const quaternion a, const quaternion b, const vm_float_t t)
{
    quaternion res;
    quat_slerp_ptr(&res, &a, &b, t);
    return res;
}

quaternion quat_nlerp(const quaternion a, const quaternion b, const vm_float_t t)
{
    quaternion res;
    quat_nlerp_ptr(&res, &a, &b, t);
    return res;
}

vector3 quat_rotate_vec3(const quaternion q, const vector3 v)
{
    vector3 res;
    quat_rotate_vec3_ptr(&res, &q, &v);
    return res;
}

vector3 quat_to_euler(const quaternion q)
{
    vector3 res;
    quat_to_euler_ptr(&res, &q);
    return res;
}

vector3 quat_to_euler_deg(const quaternion q)
{
    vector3 res;
    quat_to_euler_deg_ptr(&res, &q);
    return res;
}

vector3 quat_to_axis_angle(const quaternion q, vm_float_t *radians)
{
    vector3 axis;
    quat_to_axis_angle_ptr(&axis, radians, &q);
    return axis;
}

vector3 quat_to_axis_angle_deg(const quaternion q, vm_float_t *degrees)
{
    vector3 axis;
    quat_to_axis_angle_deg_ptr(&axis, degrees, &q);
    return axis;
}

matrix3 quat_to_mat3(const quaternion q)
{
    matrix3 res;
    quat_to_mat3_ptr(&res, &q);
    return res;
}

vm_float_t quat_dot(const quaternion a, const quaternion b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

bool quat_near(const quaternion a, const quaternion b, const vm_float_t eps)
{
    return VECMAT_FABS(a.x - b.x) <= eps
        && VECMAT_FABS(a.y - b.y) <= eps
        && VECMAT_FABS(a.z - b.z) <= eps
        && VECMAT_FABS(a.w - b.w) <= eps;
}

quaternion quat_integrate(const quaternion q, const vector3 omega, const vm_float_t dt)
{
    quaternion res;
    quat_integrate_ptr(&res, &q, &omega, dt);
    return res;
}

quaternion quat_look(const vector3 direction, const vector3 up)
{
    quaternion res;
    quat_look_ptr(&res, &direction, &up);
    return res;
}

quaternion quat_look_clip(const vector3 direction, const vector3 up, const vm_clip_t clip)
{
    quaternion res;
    quat_look_clip_ptr(&res, &direction, &up, clip);
    return res;
}

quaternion quat_from_to(const vector3 from, const vector3 to)
{
    quaternion res;
    quat_from_to_ptr(&res, &from, &to);
    return res;
}
