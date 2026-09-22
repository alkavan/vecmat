// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

matrix4 mat4_identity(void)
{
    matrix4 res = {0};
    mat4_identity_ptr(&res);
    return res;
}

matrix4 mat4_mul(const matrix4 a, const matrix4 b)
{
    matrix4 res = {0};
    mat4_mul_ptr(&res, &a, &b);
    return res;
}

matrix4 mat4_transpose(const matrix4 m)
{
    matrix4 res = {0};
    mat4_transpose_ptr(&res, &m);
    return res;
}

matrix4 mat4_inverse(const matrix4 m)
{
    matrix4 res = {0};
    mat4_inverse_ptr(&res, &m);
    return res;
}

matrix4 mat4_inverse_affine(const matrix4 m)
{
    matrix4 res = {0};
    mat4_inverse_affine_ptr(&res, &m);
    return res;
}

matrix4 mat4_translate(const vector3 v)
{
    matrix4 res = {0};
    mat4_translate_ptr(&res, &v);
    return res;
}

matrix4 mat4_scale(const vector3 v)
{
    matrix4 res = {0};
    mat4_scale_ptr(&res, &v);
    return res;
}

matrix4 mat4_rotation(const vector3 axis, const vm_float_t radians)
{
    matrix4 res = {0};
    mat4_rotation_ptr(&res, &axis, radians);
    return res;
}

matrix4 mat4_rotation_deg(const vector3 axis, const vm_float_t degrees)
{
    return mat4_rotation(axis, deg_to_rad(degrees));
}

matrix4 mat4_perspective(const vm_float_t fov, const vm_float_t aspect, const vm_float_t near, const vm_float_t far)
{
    matrix4 res = {0};
    mat4_perspective_ptr(&res, fov, aspect, near, far);
    return res;
}

matrix4 mat4_perspective_deg(const vm_float_t fov_deg, const vm_float_t aspect,
                             const vm_float_t near, const vm_float_t far)
{
    return mat4_perspective(deg_to_rad(fov_deg), aspect, near, far);
}

matrix4 mat4_ortho(const vm_float_t left, const vm_float_t right, const vm_float_t bottom,
    const vm_float_t top, const vm_float_t near, const vm_float_t far)
{
    matrix4 res = {0};
    mat4_ortho_ptr(&res, left, right, bottom, top, near, far);
    return res;
}

matrix4 mat4_look_at(const vector3 position, const vector3 target, const vector3 up)
{
    matrix4 res = {0};
    mat4_look_at_ptr(&res, &position, &target, &up);
    return res;
}

matrix4 mat4_perspective_fov(const vm_float_t fov, const vm_float_t w, const vm_float_t h,
    const vm_float_t n, const vm_float_t f)
{
    matrix4 res = {0};
    mat4_perspective_fov_ptr(&res, fov, w, h, n, f);
    return res;
}

matrix4 mat4_perspective_fov_deg(const vm_float_t fov_deg, const vm_float_t w, const vm_float_t h,
                                 const vm_float_t n, const vm_float_t f)
{
    return mat4_perspective_fov(deg_to_rad(fov_deg), w, h, n, f);
}

matrix4 mat4_perspective_infinite(const vm_float_t fov_y, const vm_float_t aspect, const vm_float_t n)
{
    matrix4 res = {0};
    mat4_perspective_infinite_ptr(&res, fov_y, aspect, n);
    return res;
}

matrix4 mat4_perspective_infinite_deg(const vm_float_t fov_y_deg, const vm_float_t aspect, const vm_float_t n)
{
    return mat4_perspective_infinite(deg_to_rad(fov_y_deg), aspect, n);
}

vm_float_t mat4_determinant(const matrix4 m)
{
    const vm_float_t cofactor_1 =  m.v[5] * m.v[10] * m.v[15] - m.v[5] * m.v[14] * m.v[11] - m.v[9] * m.v[6] * m.v[15] +
                                   m.v[9] * m.v[14] * m.v[7] + m.v[13] * m.v[6] * m.v[11] - m.v[13] * m.v[10] * m.v[7];

    const vm_float_t cofactor_2 = -m.v[4] * m.v[10] * m.v[15] + m.v[4] * m.v[14] * m.v[11] + m.v[8] * m.v[6] * m.v[15] -
                                   m.v[8] * m.v[14] * m.v[7] - m.v[12] * m.v[6] * m.v[11] + m.v[12] * m.v[10] * m.v[7];

    const vm_float_t cofactor_3 =  m.v[4] * m.v[9]  * m.v[15] - m.v[4] * m.v[13] * m.v[11] - m.v[8] * m.v[5] * m.v[15] +
                                   m.v[8] * m.v[13] * m.v[7] + m.v[12] * m.v[5] * m.v[11] - m.v[12] * m.v[9]  * m.v[7];

    const vm_float_t cofactor_4 = -m.v[4] * m.v[9]  * m.v[14] + m.v[4] * m.v[13] * m.v[10] + m.v[8] * m.v[5] * m.v[14] -
                                   m.v[8] * m.v[13] * m.v[6] - m.v[12] * m.v[5] * m.v[10] + m.v[12] * m.v[9]  * m.v[6];

    return m.v[0] * cofactor_1 + m.v[1] * cofactor_2 + m.v[2] * cofactor_3 + m.v[3] * cofactor_4;
}

vector4 mat4_mul_vec4(const matrix4 m, const vector4 v)
{
    vector4 res;
    mat4_mul_vec4_ptr(&res, &m, &v);
    return res;
}

vector3 mat4_mul_vec3(const matrix4 m, const vector3 v, const vm_float_t w)
{
    vector3 res;
    mat4_mul_vec3_ptr(&res, &m, &v, w);
    return res;
}

matrix4 mat4_rotation_x(const vm_float_t radians)
{
    matrix4 res;
    mat4_rotation_x_ptr(&res, radians);
    return res;
}

matrix4 mat4_rotation_x_deg(const vm_float_t degrees)
{
    return mat4_rotation_x(deg_to_rad(degrees));
}

matrix4 mat4_rotation_y(const vm_float_t radians)
{
    matrix4 res;
    mat4_rotation_y_ptr(&res, radians);
    return res;
}

matrix4 mat4_rotation_y_deg(const vm_float_t degrees)
{
    return mat4_rotation_y(deg_to_rad(degrees));
}

matrix4 mat4_rotation_z(const vm_float_t radians)
{
    matrix4 res;
    mat4_rotation_z_ptr(&res, radians);
    return res;
}

matrix4 mat4_rotation_z_deg(const vm_float_t degrees)
{
    return mat4_rotation_z(deg_to_rad(degrees));
}

matrix4 mat4_trs(const vector3 translation, const quaternion rotation, const vector3 scale)
{
    matrix4 res;
    mat4_trs_ptr(&res, &translation, &rotation, &scale);
    return res;
}

matrix4 mat4_from_mat3(const matrix3 m)
{
    matrix4 res;
    mat4_from_mat3_ptr(&res, &m);
    return res;
}

vector3 mat4_extract_translation(const matrix4 m)
{
    vector3 res;
    mat4_extract_translation_ptr(&res, &m);
    return res;
}

vector3 mat4_extract_scale(const matrix4 m)
{
    vector3 res;
    mat4_extract_scale_ptr(&res, &m);
    return res;
}

quaternion mat4_extract_rotation(const matrix4 m)
{
    quaternion res;
    mat4_extract_rotation_ptr(&res, &m);
    return res;
}

matrix4 mat4_perspective_clip(const vm_float_t fov_y, const vm_float_t aspect,
                              const vm_float_t near, const vm_float_t far, const vm_clip_t clip)
{
    matrix4 res;
    mat4_perspective_clip_ptr(&res, fov_y, aspect, near, far, clip);
    return res;
}

matrix4 mat4_perspective_clip_deg(const vm_float_t fov_y_deg, const vm_float_t aspect,
                                  const vm_float_t near, const vm_float_t far, const vm_clip_t clip)
{
    return mat4_perspective_clip(deg_to_rad(fov_y_deg), aspect, near, far, clip);
}

matrix4 mat4_perspective_rh_no(const vm_float_t fov_y, const vm_float_t aspect,
                               const vm_float_t near, const vm_float_t far)
{
    return mat4_perspective_clip(fov_y, aspect, near, far, VM_CLIP_RH_NO);
}

matrix4 mat4_perspective_rh_zo(const vm_float_t fov_y, const vm_float_t aspect,
                               const vm_float_t near, const vm_float_t far)
{
    return mat4_perspective_clip(fov_y, aspect, near, far, VM_CLIP_RH_ZO);
}

matrix4 mat4_perspective_lh_zo(const vm_float_t fov_y, const vm_float_t aspect,
                               const vm_float_t near, const vm_float_t far)
{
    return mat4_perspective_clip(fov_y, aspect, near, far, VM_CLIP_LH_ZO);
}

matrix4 mat4_perspective_lh_no(const vm_float_t fov_y, const vm_float_t aspect,
                               const vm_float_t near, const vm_float_t far)
{
    return mat4_perspective_clip(fov_y, aspect, near, far, VM_CLIP_LH_NO);
}

matrix4 mat4_perspective_rh_no_deg(const vm_float_t fov_y_deg, const vm_float_t aspect,
                                   const vm_float_t near, const vm_float_t far)
{
    return mat4_perspective_rh_no(deg_to_rad(fov_y_deg), aspect, near, far);
}

matrix4 mat4_perspective_rh_zo_deg(const vm_float_t fov_y_deg, const vm_float_t aspect,
                                   const vm_float_t near, const vm_float_t far)
{
    return mat4_perspective_rh_zo(deg_to_rad(fov_y_deg), aspect, near, far);
}

matrix4 mat4_perspective_lh_zo_deg(const vm_float_t fov_y_deg, const vm_float_t aspect,
                                   const vm_float_t near, const vm_float_t far)
{
    return mat4_perspective_lh_zo(deg_to_rad(fov_y_deg), aspect, near, far);
}

matrix4 mat4_perspective_lh_no_deg(const vm_float_t fov_y_deg, const vm_float_t aspect,
                                   const vm_float_t near, const vm_float_t far)
{
    return mat4_perspective_lh_no(deg_to_rad(fov_y_deg), aspect, near, far);
}

matrix4 mat4_ortho_clip(const vm_float_t left, const vm_float_t right, const vm_float_t bottom,
                        const vm_float_t top, const vm_float_t near, const vm_float_t far,
                        const vm_clip_t clip)
{
    matrix4 res;
    mat4_ortho_clip_ptr(&res, left, right, bottom, top, near, far, clip);
    return res;
}

matrix4 mat4_ortho_rh_no(const vm_float_t left, const vm_float_t right, const vm_float_t bottom,
                         const vm_float_t top, const vm_float_t near, const vm_float_t far)
{
    return mat4_ortho_clip(left, right, bottom, top, near, far, VM_CLIP_RH_NO);
}

matrix4 mat4_ortho_rh_zo(const vm_float_t left, const vm_float_t right, const vm_float_t bottom,
                         const vm_float_t top, const vm_float_t near, const vm_float_t far)
{
    return mat4_ortho_clip(left, right, bottom, top, near, far, VM_CLIP_RH_ZO);
}

matrix4 mat4_ortho_lh_zo(const vm_float_t left, const vm_float_t right, const vm_float_t bottom,
                         const vm_float_t top, const vm_float_t near, const vm_float_t far)
{
    return mat4_ortho_clip(left, right, bottom, top, near, far, VM_CLIP_LH_ZO);
}

matrix4 mat4_ortho_lh_no(const vm_float_t left, const vm_float_t right, const vm_float_t bottom,
                         const vm_float_t top, const vm_float_t near, const vm_float_t far)
{
    return mat4_ortho_clip(left, right, bottom, top, near, far, VM_CLIP_LH_NO);
}

matrix4 mat4_look_at_clip(const vector3 position, const vector3 target, const vector3 up,
                          const vm_clip_t clip)
{
    matrix4 res;
    mat4_look_at_clip_ptr(&res, &position, &target, &up, clip);
    return res;
}

matrix4 mat4_look_at_rh(const vector3 position, const vector3 target, const vector3 up)
{
    return mat4_look_at_clip(position, target, up, VM_CLIP_RH_NO);
}

matrix4 mat4_look_at_lh(const vector3 position, const vector3 target, const vector3 up)
{
    return mat4_look_at_clip(position, target, up, VM_CLIP_LH_ZO);
}

matrix4 mat4_look_from_dir(const vector3 position, const vector3 direction, const vector3 up)
{
    matrix4 res = {0};
    mat4_look_from_dir_ptr(&res, &position, &direction, &up);
    return res;
}

matrix4 mat4_look_from_dir_clip(const vector3 position, const vector3 direction,
                                const vector3 up, const vm_clip_t clip)
{
    matrix4 res = {0};
    mat4_look_from_dir_clip_ptr(&res, &position, &direction, &up, clip);
    return res;
}

matrix4 mat4_look_from_dir_rh(const vector3 position, const vector3 direction, const vector3 up)
{
    return mat4_look_from_dir_clip(position, direction, up, VM_CLIP_RH_NO);
}

matrix4 mat4_look_from_dir_lh(const vector3 position, const vector3 direction, const vector3 up)
{
    return mat4_look_from_dir_clip(position, direction, up, VM_CLIP_LH_ZO);
}

matrix4 mat4_viewport(const vm_float_t x, const vm_float_t y,
                      const vm_float_t width, const vm_float_t height)
{
    matrix4 res = {0};
    mat4_viewport_ptr(&res, x, y, width, height);
    return res;
}

matrix4 mat4_viewport_depth(const vm_float_t x, const vm_float_t y,
                            const vm_float_t width, const vm_float_t height,
                            const vm_float_t n, const vm_float_t f)
{
    matrix4 res = {0};
    mat4_viewport_depth_ptr(&res, x, y, width, height, n, f);
    return res;
}

matrix4 mat4_normal(const matrix4 m)
{
    matrix4 res = {0};
    mat4_normal_ptr(&res, &m);
    return res;
}

matrix4 mat4_perspective_infinite_clip(const vm_float_t fov_y, const vm_float_t aspect,
                                       const vm_float_t n, const vm_clip_t clip)
{
    matrix4 res = {0};
    mat4_perspective_infinite_clip_ptr(&res, fov_y, aspect, n, clip);
    return res;
}

matrix4 mat4_perspective_infinite_clip_deg(const vm_float_t fov_y_deg, const vm_float_t aspect,
                                           const vm_float_t n, const vm_clip_t clip)
{
    return mat4_perspective_infinite_clip(deg_to_rad(fov_y_deg), aspect, n, clip);
}

matrix4 mat4_infinite_reverse_z(const vm_float_t fov_y, const vm_float_t aspect, const vm_float_t n)
{
    matrix4 res = {0};
    mat4_infinite_reverse_z_ptr(&res, fov_y, aspect, n);
    return res;
}

matrix4 mat4_infinite_reverse_z_clip(const vm_float_t fov_y, const vm_float_t aspect,
                                     const vm_float_t n, const vm_clip_t clip)
{
    matrix4 res = {0};
    mat4_infinite_reverse_z_clip_ptr(&res, fov_y, aspect, n, clip);
    return res;
}

matrix4 mat4_infinite_reverse_z_deg(const vm_float_t fov_y_deg, const vm_float_t aspect,
                                    const vm_float_t n)
{
    return mat4_infinite_reverse_z(deg_to_rad(fov_y_deg), aspect, n);
}

matrix4 mat4_infinite_reverse_z_clip_deg(const vm_float_t fov_y_deg, const vm_float_t aspect,
                                         const vm_float_t n, const vm_clip_t clip)
{
    return mat4_infinite_reverse_z_clip(deg_to_rad(fov_y_deg), aspect, n, clip);
}

vector3 vec3_world_to_window(const vector3 world, const matrix4 model,
                            const matrix4 projection, const vector4 viewport)
{
    vector3 res;
    vec3_world_to_window_ptr(&res, &world, &model, &projection, &viewport);
    return res;
}

vector3 vec3_window_to_world(const vector3 window, const matrix4 model,
                              const matrix4 projection, const vector4 viewport)
{
    vector3 res;
    vec3_window_to_world_ptr(&res, &window, &model, &projection, &viewport);
    return res;
}

vector3 vec3_world_to_window_clip(const vector3 world, const matrix4 model,
                                  const matrix4 projection, const vector4 viewport,
                                  const vm_clip_t clip)
{
    vector3 res;
    vec3_world_to_window_clip_ptr(&res, &world, &model, &projection, &viewport, clip);
    return res;
}

vector3 vec3_window_to_world_clip(const vector3 window, const matrix4 model,
                                  const matrix4 projection, const vector4 viewport,
                                  const vm_clip_t clip)
{
    vector3 res;
    vec3_window_to_world_clip_ptr(&res, &window, &model, &projection, &viewport, clip);
    return res;
}
