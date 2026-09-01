// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

/**
 * @brief Constructs the 4x4 identity matrix.
 *
 * @see mat4_identity_ptr
 *
 * @return The identity matrix4.
 */
matrix4 mat4_identity(void)
{
    matrix4 res = {0};
    mat4_identity_ptr(&res);
    return res;
}

/**
 * @brief Multiplies two 4x4 matrices (a * b).
 *
 * @see mat4_mul_ptr
 *
 * @param a The first matrix.
 * @param b The second matrix.
 * @return The resulting product matrix4.
 */
matrix4 mat4_mul(const matrix4 a, const matrix4 b)
{
    matrix4 res = {0};
    mat4_mul_ptr(&res, &a, &b);
    return res;
}

/**
 * @brief Computes the transpose of a 4x4 matrix.
 *
 * @see mat4_transpose_ptr
 *
 * @param m The input matrix.
 * @return The transposed matrix4.
 */
matrix4 mat4_transpose(const matrix4 m)
{
    matrix4 res = {0};
    mat4_transpose_ptr(&res, &m);
    return res;
}

/**
 * @brief Computes the inverse of a 4x4 matrix.
 *
 * @see mat4_inverse_ptr
 *
 * @param m The input matrix.
 * @return The inverse matrix4.
 */
matrix4 mat4_inverse(const matrix4 m)
{
    matrix4 res = {0};
    mat4_inverse_ptr(&res, &m);
    return res;
}

/**
 * @brief Inverse of an affine matrix `[R t; 0 1]`.
 *
 * @see mat4_inverse_affine_ptr
 *
 * @param m Affine 4×4 matrix.
 * @return Inverse matrix.
 */
matrix4 mat4_inverse_affine(const matrix4 m)
{
    matrix4 res = {0};
    mat4_inverse_affine_ptr(&res, &m);
    return res;
}

/**
 * @brief Constructs a translation matrix from the given vector.
 *
 * @see mat4_translate_ptr
 *
 * @param v The translation vector (x, y, z).
 * @return The translation matrix4.
 */
matrix4 mat4_translate(const vector3 v)
{
    matrix4 res = {0};
    mat4_translate_ptr(&res, &v);
    return res;
}

/**
 * @brief Constructs a scaling matrix from the given scale vector.
 *
 * @see mat4_scale_ptr
 *
 * @param v The scale vector (x, y, z factors).
 * @return The resulting scaling matrix4.
 */
matrix4 mat4_scale(const vector3 v)
{
    matrix4 res = {0};
    mat4_scale_ptr(&res, &v);
    return res;
}

/**
 * @brief Constructs a 4x4 rotation matrix.
 *
 * @see mat4_rotation_ptr
 *
 * @param axis The rotation axis.
 * @param radians The rotation angle in radians.
 * @return The rotation matrix4.
 */
matrix4 mat4_rotation(const vector3 axis, const vm_float_t radians)
{
    matrix4 res = {0};
    mat4_rotation_ptr(&res, &axis, radians);
    return res;
}

/**
 * @brief Constructs a 4x4 rotation matrix from an axis and angle in degrees.
 *
 * @param axis The rotation axis.
 * @param degrees The rotation angle in degrees.
 *
 * @return The rotation matrix4.
 */
matrix4 mat4_rotation_deg(const vector3 axis, const vm_float_t degrees)
{
    return mat4_rotation(axis, deg_to_rad(degrees));
}

/**
 * @brief Constructs a perspective projection matrix.
 *
 * @see mat4_perspective_ptr
 *
 * @param fov Field of view (radians).
 * @param aspect Aspect ratio (width/height).
 * @param near Near plane distance.
 * @param far Far plane distance.
 * @return The perspective matrix4.
 */
matrix4 mat4_perspective(const vm_float_t fov, const vm_float_t aspect, const vm_float_t near, const vm_float_t far)
{
    matrix4 res = {0};
    mat4_perspective_ptr(&res, fov, aspect, near, far);
    return res;
}

/**
 * @brief Constructs a 4x4 perspective projection matrix from vertical field of view in degrees.
 *
 * @param fov_deg Vertical field of view angle in degrees.
 * @param aspect Aspect ratio (width / height).
 * @param near Distance to the near clipping plane.
 * @param far Distance to the far clipping plane.
 * @return The perspective projection matrix.
 */
matrix4 mat4_perspective_deg(const vm_float_t fov_deg, const vm_float_t aspect,
                             const vm_float_t near, const vm_float_t far)
{
    return mat4_perspective(deg_to_rad(fov_deg), aspect, near, far);
}

/**
 * @brief Constructs an orthographic projection matrix.
 *
 * @see mat4_ortho_ptr
 *
 * @param left Left clipping plane.
 * @param right Right clipping plane.
 * @param bottom Bottom clipping plane.
 * @param top Top clipping plane.
 * @param near Near plane distance.
 * @param far Far plane distance.
 * @return The orthographic matrix4.
 */
matrix4 mat4_ortho(const vm_float_t left, const vm_float_t right, const vm_float_t bottom,
    const vm_float_t top, const vm_float_t near, const vm_float_t far)
{
    matrix4 res = {0};
    mat4_ortho_ptr(&res, left, right, bottom, top, near, far);
    return res;
}

/**
 * @brief Constructs a view matrix from eye position, target, and up vector.
 *
 * @see mat4_look_at_ptr
 *
 * @param position Eye position.
 * @param target Target position.
 * @param up Up vector.
 * @return The view matrix4.
 */
matrix4 mat4_look_at(const vector3 position, const vector3 target, const vector3 up)
{
    matrix4 res = {0};
    mat4_look_at_ptr(&res, &position, &target, &up);
    return res;
}

/**
 * @brief Constructs a perspective projection matrix using FOV, width, and height.
 *
 * @see mat4_perspective_fov_ptr
 *
 * @param fov Vertical field of view (radians).
 * @param w Viewport width.
 * @param h Viewport height.
 * @param n Near plane.
 * @param f Far plane.
 *
 * @return The perspective matrix4.
 */
matrix4 mat4_perspective_fov(const vm_float_t fov, const vm_float_t w, const vm_float_t h,
    const vm_float_t n, const vm_float_t f)
{
    matrix4 res = {0};
    mat4_perspective_fov_ptr(&res, fov, w, h, n, f);
    return res;
}

/**
 * @brief Constructs a 4x4 perspective projection matrix from vertical field of view in degrees.
 *
 * @param fov_deg Vertical field of view in degrees.
 * @param w Width of the viewport.
 * @param h Height of the viewport.
 * @param n Distance to the near clipping plane.
 * @param f Distance to the far clipping plane.
 *
 * @return The perspective matrix4.
 */
matrix4 mat4_perspective_fov_deg(const vm_float_t fov_deg, const vm_float_t w, const vm_float_t h,
                                 const vm_float_t n, const vm_float_t f)
{
    return mat4_perspective_fov(deg_to_rad(fov_deg), w, h, n, f);
}

/**
 * @brief Constructs an infinite far-plane perspective projection matrix.
 *
 * @see mat4_perspective_infinite_ptr
 *
 * @param fov_y Vertical field of view (radians).
 * @param aspect Aspect ratio.
 * @param n Near plane distance.
 * @return The perspective matrix4.
 */
matrix4 mat4_perspective_infinite(const vm_float_t fov_y, const vm_float_t aspect, const vm_float_t n)
{
    matrix4 res = {0};
    mat4_perspective_infinite_ptr(&res, fov_y, aspect, n);
    return res;
}

/**
 * @brief Constructs a 4x4 infinite perspective projection matrix using vertical field of view in degrees.
 *
 * @param fov_y_deg Vertical field of view angle in degrees.
 * @param aspect Aspect ratio (width / height).
 * @param n Near plane distance.
 *
 * @return The infinite perspective matrix4.
 */
matrix4 mat4_perspective_infinite_deg(const vm_float_t fov_y_deg, const vm_float_t aspect, const vm_float_t n)
{
    return mat4_perspective_infinite(deg_to_rad(fov_y_deg), aspect, n);
}

/**
 * @brief Calculates the determinant of the given 4x4 matrix.
 *
 * Uses cofactor expansion for computation.
 *
 * @param m The matrix.
 *
 * @return The determinant value as a vm_float_t.
 */
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

/**
 * @brief Multiplies a 4x4 matrix by a vector4.
 *
 * @see mat4_mul_vec4_ptr
 *
 * @param m Input matrix.
 * @param v Input vector.
 *
 * @return The resulting vector4.
 */
vector4 mat4_mul_vec4(const matrix4 m, const vector4 v)
{
    vector4 res;
    mat4_mul_vec4_ptr(&res, &m, &v);
    return res;
}

/**
 * @brief Transforms a vector3 by a 4x4 matrix using homogeneous w.
 *
 * @see mat4_mul_vec3_ptr
 *
 * @param m Input matrix.
 * @param v Input vector.
 * @param w Homogeneous w component.
 *
 * @return The resulting vector3.
 */
vector3 mat4_mul_vec3(const matrix4 m, const vector3 v, const vm_float_t w)
{
    vector3 res;
    mat4_mul_vec3_ptr(&res, &m, &v, w);
    return res;
}

/**
 * @brief Constructs a 4x4 rotation matrix around the X axis.
 *
 * @see mat4_rotation_x_ptr
 *
 * @param radians Rotation angle in radians.
 * @return The rotation matrix4.
 */
matrix4 mat4_rotation_x(const vm_float_t radians)
{
    matrix4 res;
    mat4_rotation_x_ptr(&res, radians);
    return res;
}

/**
 * @brief Constructs a 4x4 rotation matrix around the X axis.
 *
 * The angle is specified in degrees and internally converted to radians.
 *
 * @param degrees The rotation angle in degrees.
 * @return The rotation matrix4.
 */
matrix4 mat4_rotation_x_deg(const vm_float_t degrees)
{
    return mat4_rotation_x(deg_to_rad(degrees));
}

/**
 * @brief Constructs a 4x4 rotation matrix around the Y axis.
 *
 * @see mat4_rotation_y_ptr
 *
 * @param radians The rotation angle in radians.
 * @return The rotation matrix4.
 */
matrix4 mat4_rotation_y(const vm_float_t radians)
{
    matrix4 res;
    mat4_rotation_y_ptr(&res, radians);
    return res;
}

/**
 * @brief Constructs a 4x4 rotation matrix around the Y axis.
 *
 * @param degrees Rotation angle in degrees.
 * @return The rotation matrix4.
 */
matrix4 mat4_rotation_y_deg(const vm_float_t degrees)
{
    return mat4_rotation_y(deg_to_rad(degrees));
}

/**
 * @brief Constructs a 4x4 rotation matrix around the Z axis.
 *
 * @see mat4_rotation_z_ptr
 *
 * @param radians Rotation angle in radians.
 * @return The rotation matrix4.
 */
matrix4 mat4_rotation_z(const vm_float_t radians)
{
    matrix4 res;
    mat4_rotation_z_ptr(&res, radians);
    return res;
}

/**
 * @brief Constructs a 4x4 rotation matrix around the Z axis from an angle in degrees.
 *
 * @param degrees The rotation angle in degrees.
 *
 * @return The rotation matrix4.
 */
matrix4 mat4_rotation_z_deg(const vm_float_t degrees)
{
    return mat4_rotation_z(deg_to_rad(degrees));
}

/**
 * @brief Builds a 4x4 TRS matrix from translation, rotation, and scale.
 *
 * @see mat4_trs_ptr
 *
 * @param translation Translation vector.
 * @param rotation Rotation quaternion.
 * @param scale Scale vector.
 *
 * @return The resulting matrix4.
 */
matrix4 mat4_trs(const vector3 translation, const quaternion rotation, const vector3 scale)
{
    matrix4 res;
    mat4_trs_ptr(&res, &translation, &rotation, &scale);
    return res;
}

/**
 * @brief Embeds a matrix3 into the upper-left of a matrix4.
 *
 * @see mat4_from_mat3_ptr
 *
 * @param m Input matrix.
 * @return The resulting matrix4.
 */
matrix4 mat4_from_mat3(const matrix3 m)
{
    matrix4 res;
    mat4_from_mat3_ptr(&res, &m);
    return res;
}

/**
 * @brief Extracts the translation vector from a matrix4.
 *
 * @see mat4_extract_translation_ptr
 *
 * @param m Input matrix.
 * @return The resulting vector3.
 */
vector3 mat4_extract_translation(const matrix4 m)
{
    vector3 res;
    mat4_extract_translation_ptr(&res, &m);
    return res;
}

/**
 * @brief Extracts the scale vector from a matrix4.
 *
 * @see mat4_extract_scale_ptr
 *
 * @param m Input matrix.
 * @return The resulting vector3.
 */
vector3 mat4_extract_scale(const matrix4 m)
{
    vector3 res;
    mat4_extract_scale_ptr(&res, &m);
    return res;
}

/**
 * @brief Extracts the rotation quaternion from a matrix4.
 *
 * @see mat4_extract_rotation_ptr
 *
 * @param m Input matrix.
 * @return The resulting quaternion.
 */
quaternion mat4_extract_rotation(const matrix4 m)
{
    quaternion res;
    mat4_extract_rotation_ptr(&res, &m);
    return res;
}

/**
 * @brief Constructs a 4x4 perspective projection matrix.
 *
 * @see mat4_perspective_clip_ptr
 *
 * @param fov_y Vertical field of view in radians.
 * @param aspect Aspect ratio (width / height).
 * @param near Distance to the near clipping plane.
 * @param far Distance to the far clipping plane.
 * @param clip Clip space convention.
 * @return The perspective matrix4.
 */
matrix4 mat4_perspective_clip(const vm_float_t fov_y, const vm_float_t aspect,
                              const vm_float_t near, const vm_float_t far, const vm_clip_t clip)
{
    matrix4 res;
    mat4_perspective_clip_ptr(&res, fov_y, aspect, near, far, clip);
    return res;
}

/**
 * @brief Perspective projection from a vertical FOV in degrees.
 *
 * @see mat4_perspective_clip_ptr
 *
 * @param fov_y_deg Vertical field of view in degrees.
 * @param aspect Aspect ratio (width / height).
 * @param near Near plane distance.
 * @param far Far plane distance.
 * @param clip Clip-space convention.
 * @return Perspective matrix4.
 */
matrix4 mat4_perspective_clip_deg(const vm_float_t fov_y_deg, const vm_float_t aspect,
                                  const vm_float_t near, const vm_float_t far, const vm_clip_t clip)
{
    return mat4_perspective_clip(deg_to_rad(fov_y_deg), aspect, near, far, clip);
}

/**
 * @brief OpenGL-style RH perspective, clip z in [-1, 1].
 *
 * @see mat4_perspective_clip_ptr
 *
 * @param fov_y Vertical field of view in radians.
 * @param aspect Aspect ratio (width / height).
 * @param near Near plane distance.
 * @param far Far plane distance.
 * @return Perspective matrix4.
 */
matrix4 mat4_perspective_rh_no(const vm_float_t fov_y, const vm_float_t aspect,
                               const vm_float_t near, const vm_float_t far)
{
    return mat4_perspective_clip(fov_y, aspect, near, far, VM_CLIP_RH_NO);
}

/**
 * @brief Vulkan-style RH perspective, clip z in [0, 1].
 *
 * @see mat4_perspective_clip_ptr
 *
 * @param fov_y Vertical field of view in radians.
 * @param aspect Aspect ratio (width / height).
 * @param near Near plane distance.
 * @param far Far plane distance.
 * @return Perspective matrix4.
 */
matrix4 mat4_perspective_rh_zo(const vm_float_t fov_y, const vm_float_t aspect,
                               const vm_float_t near, const vm_float_t far)
{
    return mat4_perspective_clip(fov_y, aspect, near, far, VM_CLIP_RH_ZO);
}

/**
 * @brief Direct3D-style LH perspective, clip z in [0, 1].
 *
 * @see mat4_perspective_clip_ptr
 *
 * @param fov_y Vertical field of view in radians.
 * @param aspect Aspect ratio (width / height).
 * @param near Near plane distance.
 * @param far Far plane distance.
 * @return Perspective matrix4.
 */
matrix4 mat4_perspective_lh_zo(const vm_float_t fov_y, const vm_float_t aspect,
                               const vm_float_t near, const vm_float_t far)
{
    return mat4_perspective_clip(fov_y, aspect, near, far, VM_CLIP_LH_ZO);
}

/**
 * @brief Left-handed perspective, clip z in [-1, 1].
 *
 * @see mat4_perspective_clip_ptr
 *
 * @param fov_y Vertical field of view in radians.
 * @param aspect Aspect ratio (width / height).
 * @param near Near plane distance.
 * @param far Far plane distance.
 * @return Perspective matrix4.
 */
matrix4 mat4_perspective_lh_no(const vm_float_t fov_y, const vm_float_t aspect,
                               const vm_float_t near, const vm_float_t far)
{
    return mat4_perspective_clip(fov_y, aspect, near, far, VM_CLIP_LH_NO);
}

/**
 * @brief OpenGL-style RH perspective from a FOV in degrees.
 *
 * @see mat4_perspective_clip_ptr
 *
 * @param fov_y_deg Vertical field of view in degrees.
 * @param aspect Aspect ratio (width / height).
 * @param near Near plane distance.
 * @param far Far plane distance.
 * @return Perspective matrix4.
 */
matrix4 mat4_perspective_rh_no_deg(const vm_float_t fov_y_deg, const vm_float_t aspect,
                                   const vm_float_t near, const vm_float_t far)
{
    return mat4_perspective_rh_no(deg_to_rad(fov_y_deg), aspect, near, far);
}

/**
 * @brief Vulkan-style RH perspective from a FOV in degrees.
 *
 * @see mat4_perspective_clip_ptr
 *
 * @param fov_y_deg Vertical field of view in degrees.
 * @param aspect Aspect ratio (width / height).
 * @param near Near plane distance.
 * @param far Far plane distance.
 * @return Perspective matrix4.
 */
matrix4 mat4_perspective_rh_zo_deg(const vm_float_t fov_y_deg, const vm_float_t aspect,
                                   const vm_float_t near, const vm_float_t far)
{
    return mat4_perspective_rh_zo(deg_to_rad(fov_y_deg), aspect, near, far);
}

/**
 * @brief Direct3D-style LH perspective from a FOV in degrees.
 *
 * @see mat4_perspective_clip_ptr
 *
 * @param fov_y_deg Vertical field of view in degrees.
 * @param aspect Aspect ratio (width / height).
 * @param near Near plane distance.
 * @param far Far plane distance.
 * @return Perspective matrix4.
 */
matrix4 mat4_perspective_lh_zo_deg(const vm_float_t fov_y_deg, const vm_float_t aspect,
                                   const vm_float_t near, const vm_float_t far)
{
    return mat4_perspective_lh_zo(deg_to_rad(fov_y_deg), aspect, near, far);
}

/**
 * @brief Left-handed NO perspective from a FOV in degrees.
 *
 * @see mat4_perspective_clip_ptr
 *
 * @param fov_y_deg Vertical field of view in degrees.
 * @param aspect Aspect ratio (width / height).
 * @param near Near plane distance.
 * @param far Far plane distance.
 * @return Perspective matrix4.
 */
matrix4 mat4_perspective_lh_no_deg(const vm_float_t fov_y_deg, const vm_float_t aspect,
                                   const vm_float_t near, const vm_float_t far)
{
    return mat4_perspective_lh_no(deg_to_rad(fov_y_deg), aspect, near, far);
}

/**
 * @brief Orthographic projection with an explicit clip convention.
 *
 * @see mat4_ortho_clip_ptr
 *
 * @param left Left plane.
 * @param right Right plane.
 * @param bottom Bottom plane.
 * @param top Top plane.
 * @param near Near plane.
 * @param far Far plane.
 * @param clip Clip-space convention.
 * @return Orthographic matrix4.
 */
matrix4 mat4_ortho_clip(const vm_float_t left, const vm_float_t right, const vm_float_t bottom,
                        const vm_float_t top, const vm_float_t near, const vm_float_t far,
                        const vm_clip_t clip)
{
    matrix4 res;
    mat4_ortho_clip_ptr(&res, left, right, bottom, top, near, far, clip);
    return res;
}

/**
 * @brief OpenGL-style RH orthographic projection.
 *
 * @see mat4_ortho_clip_ptr
 *
 * @param left Left plane.
 * @param right Right plane.
 * @param bottom Bottom plane.
 * @param top Top plane.
 * @param near Near plane.
 * @param far Far plane.
 * @return Orthographic matrix4.
 */
matrix4 mat4_ortho_rh_no(const vm_float_t left, const vm_float_t right, const vm_float_t bottom,
                         const vm_float_t top, const vm_float_t near, const vm_float_t far)
{
    return mat4_ortho_clip(left, right, bottom, top, near, far, VM_CLIP_RH_NO);
}

/**
 * @brief Vulkan-style RH orthographic projection.
 *
 * @see mat4_ortho_clip_ptr
 *
 * @param left Left plane.
 * @param right Right plane.
 * @param bottom Bottom plane.
 * @param top Top plane.
 * @param near Near plane.
 * @param far Far plane.
 * @return Orthographic matrix4.
 */
matrix4 mat4_ortho_rh_zo(const vm_float_t left, const vm_float_t right, const vm_float_t bottom,
                         const vm_float_t top, const vm_float_t near, const vm_float_t far)
{
    return mat4_ortho_clip(left, right, bottom, top, near, far, VM_CLIP_RH_ZO);
}

/**
 * @brief Direct3D-style LH orthographic projection.
 *
 * @see mat4_ortho_clip_ptr
 *
 * @param left Left plane.
 * @param right Right plane.
 * @param bottom Bottom plane.
 * @param top Top plane.
 * @param near Near plane.
 * @param far Far plane.
 * @return Orthographic matrix4.
 */
matrix4 mat4_ortho_lh_zo(const vm_float_t left, const vm_float_t right, const vm_float_t bottom,
                         const vm_float_t top, const vm_float_t near, const vm_float_t far)
{
    return mat4_ortho_clip(left, right, bottom, top, near, far, VM_CLIP_LH_ZO);
}

/**
 * @brief Left-handed NO orthographic projection.
 *
 * @see mat4_ortho_clip_ptr
 *
 * @param left Left plane.
 * @param right Right plane.
 * @param bottom Bottom plane.
 * @param top Top plane.
 * @param near Near plane.
 * @param far Far plane.
 * @return Orthographic matrix4.
 */
matrix4 mat4_ortho_lh_no(const vm_float_t left, const vm_float_t right, const vm_float_t bottom,
                         const vm_float_t top, const vm_float_t near, const vm_float_t far)
{
    return mat4_ortho_clip(left, right, bottom, top, near, far, VM_CLIP_LH_NO);
}

/**
 * @brief Look-at view matrix with an explicit clip convention.
 *
 * @see mat4_look_at_clip_ptr
 *
 * @param position Eye position.
 * @param target Point the camera looks at.
 * @param up World up direction.
 * @param clip Clip-space convention.
 * @return View matrix4.
 */
matrix4 mat4_look_at_clip(const vector3 position, const vector3 target, const vector3 up,
                          const vm_clip_t clip)
{
    matrix4 res;
    mat4_look_at_clip_ptr(&res, &position, &target, &up, clip);
    return res;
}

/**
 * @brief Right-handed look-at view matrix.
 *
 * @see mat4_look_at_clip_ptr
 *
 * @param position Eye position.
 * @param target Point the camera looks at.
 * @param up World up direction.
 * @return View matrix4.
 */
matrix4 mat4_look_at_rh(const vector3 position, const vector3 target, const vector3 up)
{
    return mat4_look_at_clip(position, target, up, VM_CLIP_RH_NO);
}

/**
 * @brief Left-handed look-at view matrix.
 *
 * @see mat4_look_at_clip_ptr
 *
 * @param position Eye position.
 * @param target Point the camera looks at.
 * @param up World up direction.
 * @return View matrix4.
 */
matrix4 mat4_look_at_lh(const vector3 position, const vector3 target, const vector3 up)
{
    return mat4_look_at_clip(position, target, up, VM_CLIP_LH_ZO);
}

/**
 * @brief View matrix from eye position and a look direction.
 *
 * @see mat4_look_from_dir_ptr
 *
 * @param position Eye position.
 * @param direction Look direction.
 * @param up World up direction.
 * @return View matrix4.
 */
matrix4 mat4_look_from_dir(const vector3 position, const vector3 direction, const vector3 up)
{
    matrix4 res = {0};
    mat4_look_from_dir_ptr(&res, &position, &direction, &up);
    return res;
}

/**
 * @brief Look-from-direction view matrix with an explicit clip convention.
 *
 * @see mat4_look_from_dir_clip_ptr
 *
 * @param position Eye position.
 * @param direction Look direction.
 * @param up World up direction.
 * @param clip Clip-space convention.
 * @return View matrix4.
 */
matrix4 mat4_look_from_dir_clip(const vector3 position, const vector3 direction,
                                const vector3 up, const vm_clip_t clip)
{
    matrix4 res = {0};
    mat4_look_from_dir_clip_ptr(&res, &position, &direction, &up, clip);
    return res;
}

/**
 * @brief Right-handed look-from-direction view matrix.
 *
 * @see mat4_look_from_dir_clip_ptr
 *
 * @param position Eye position.
 * @param direction Look direction.
 * @param up World up direction.
 * @return View matrix4.
 */
matrix4 mat4_look_from_dir_rh(const vector3 position, const vector3 direction, const vector3 up)
{
    return mat4_look_from_dir_clip(position, direction, up, VM_CLIP_RH_NO);
}

/**
 * @brief Left-handed look-from-direction view matrix.
 *
 * @see mat4_look_from_dir_clip_ptr
 *
 * @param position Eye position.
 * @param direction Look direction.
 * @param up World up direction.
 * @return View matrix4.
 */
matrix4 mat4_look_from_dir_lh(const vector3 position, const vector3 direction, const vector3 up)
{
    return mat4_look_from_dir_clip(position, direction, up, VM_CLIP_LH_ZO);
}

/**
 * @brief Pixel-box viewport matrix (NDC xy/z [-1,1] to window + depth [0,1]).
 *
 * @see mat4_viewport_ptr
 *
 * @param x Viewport origin X.
 * @param y Viewport origin Y.
 * @param width Viewport width.
 * @param height Viewport height.
 * @return Viewport matrix4.
 */
matrix4 mat4_viewport(const vm_float_t x, const vm_float_t y,
                      const vm_float_t width, const vm_float_t height)
{
    matrix4 res = {0};
    mat4_viewport_ptr(&res, x, y, width, height);
    return res;
}

/**
 * @brief Viewport matrix with an explicit depth range.
 *
 * @see mat4_viewport_depth_ptr
 *
 * @param x Viewport origin X.
 * @param y Viewport origin Y.
 * @param width Viewport width.
 * @param height Viewport height.
 * @param n Near depth mapped from clip z.
 * @param f Far depth mapped from clip z.
 * @return Viewport matrix4.
 */
matrix4 mat4_viewport_depth(const vm_float_t x, const vm_float_t y,
                            const vm_float_t width, const vm_float_t height,
                            const vm_float_t n, const vm_float_t f)
{
    matrix4 res = {0};
    mat4_viewport_depth_ptr(&res, x, y, width, height, n, f);
    return res;
}

/**
 * @brief Inverse-transpose of the upper 3x3, embedded in a matrix4.
 *
 * @see mat4_normal_ptr
 *
 * @param m Model matrix.
 * @return Normal matrix as a matrix4.
 */
matrix4 mat4_normal(const matrix4 m)
{
    matrix4 res = {0};
    mat4_normal_ptr(&res, &m);
    return res;
}

/**
 * @brief Infinite-far perspective with an explicit clip convention.
 *
 * @see mat4_perspective_infinite_clip_ptr
 *
 * @param fov_y Vertical field of view in radians.
 * @param aspect Aspect ratio (width / height).
 * @param n Near plane distance.
 * @param clip Clip-space convention.
 * @return Perspective matrix4.
 */
matrix4 mat4_perspective_infinite_clip(const vm_float_t fov_y, const vm_float_t aspect,
                                       const vm_float_t n, const vm_clip_t clip)
{
    matrix4 res = {0};
    mat4_perspective_infinite_clip_ptr(&res, fov_y, aspect, n, clip);
    return res;
}

/**
 * @brief Infinite-far perspective from a FOV in degrees.
 *
 * @see mat4_perspective_infinite_clip_ptr
 *
 * @param fov_y_deg Vertical field of view in degrees.
 * @param aspect Aspect ratio (width / height).
 * @param n Near plane distance.
 * @param clip Clip-space convention.
 * @return Perspective matrix4.
 */
matrix4 mat4_perspective_infinite_clip_deg(const vm_float_t fov_y_deg, const vm_float_t aspect,
                                           const vm_float_t n, const vm_clip_t clip)
{
    return mat4_perspective_infinite_clip(deg_to_rad(fov_y_deg), aspect, n, clip);
}

/**
 * @brief Infinite reverse-Z perspective (RH + ZO by default).
 *
 * @see mat4_infinite_reverse_z_ptr
 *
 * @param fov_y Vertical field of view in radians.
 * @param aspect Aspect ratio (width / height).
 * @param n Near plane distance.
 * @return Perspective matrix4.
 */
matrix4 mat4_infinite_reverse_z(const vm_float_t fov_y, const vm_float_t aspect, const vm_float_t n)
{
    matrix4 res = {0};
    mat4_infinite_reverse_z_ptr(&res, fov_y, aspect, n);
    return res;
}

/**
 * @brief Infinite reverse-Z perspective with an explicit clip convention.
 *
 * @see mat4_infinite_reverse_z_clip_ptr
 *
 * @param fov_y Vertical field of view in radians.
 * @param aspect Aspect ratio (width / height).
 * @param n Near plane distance.
 * @param clip Clip-space convention.
 * @return Perspective matrix4.
 */
matrix4 mat4_infinite_reverse_z_clip(const vm_float_t fov_y, const vm_float_t aspect,
                                     const vm_float_t n, const vm_clip_t clip)
{
    matrix4 res = {0};
    mat4_infinite_reverse_z_clip_ptr(&res, fov_y, aspect, n, clip);
    return res;
}

/**
 * @brief Infinite reverse-Z perspective from a FOV in degrees.
 *
 * @see mat4_infinite_reverse_z_ptr
 *
 * @param fov_y_deg Vertical field of view in degrees.
 * @param aspect Aspect ratio (width / height).
 * @param n Near plane distance.
 * @return Perspective matrix4.
 */
matrix4 mat4_infinite_reverse_z_deg(const vm_float_t fov_y_deg, const vm_float_t aspect,
                                    const vm_float_t n)
{
    return mat4_infinite_reverse_z(deg_to_rad(fov_y_deg), aspect, n);
}

/**
 * @brief Infinite reverse-Z perspective (degrees + clip convention).
 *
 * @see mat4_infinite_reverse_z_clip_ptr
 *
 * @param fov_y_deg Vertical field of view in degrees.
 * @param aspect Aspect ratio (width / height).
 * @param n Near plane distance.
 * @param clip Clip-space convention.
 * @return Perspective matrix4.
 */
matrix4 mat4_infinite_reverse_z_clip_deg(const vm_float_t fov_y_deg, const vm_float_t aspect,
                                         const vm_float_t n, const vm_clip_t clip)
{
    return mat4_infinite_reverse_z_clip(deg_to_rad(fov_y_deg), aspect, n, clip);
}

/**
 * @brief Projects a world space point to window coordinates.
 *
 * @see vec3_world_to_window_ptr
 *
 * @param world World space position to project.
 * @param model Model matrix.
 * @param projection Projection matrix.
 * @param viewport Viewport rectangle (x, y, width, height).
 *
 * @return The projected window space vector3.
 */
vector3 vec3_world_to_window(const vector3 world, const matrix4 model,
                            const matrix4 projection, const vector4 viewport)
{
    vector3 res;
    vec3_world_to_window_ptr(&res, &world, &model, &projection, &viewport);
    return res;
}

/**
 * @brief Un-projects a window coordinate back to world space.
 *
 * @see vec3_window_to_world_ptr
 *
 * @param window The window coordinate to un-project.
 * @param model The model matrix.
 * @param projection The projection matrix.
 * @param viewport The viewport (x, y, width, height).
 * @return The unprojected world space vector3.
 */
vector3 vec3_window_to_world(const vector3 window, const matrix4 model,
                              const matrix4 projection, const vector4 viewport)
{
    vector3 res;
    vec3_window_to_world_ptr(&res, &window, &model, &projection, &viewport);
    return res;
}

/**
 * @brief Projects a world-space point into window coordinates using model,
 * projection and viewport.
 *
 * @see vec3_world_to_window_clip_ptr.
 *
 * @param world World-space position to project.
 * @param model Model matrix.
 * @param projection Projection matrix.
 * @param viewport Viewport rectangle (x, y, width, height).
 * @param clip Clip-space convention to use.
 * @return The projected window-space coordinates.
 */
vector3 vec3_world_to_window_clip(const vector3 world, const matrix4 model,
                                  const matrix4 projection, const vector4 viewport,
                                  const vm_clip_t clip)
{
    vector3 res;
    vec3_world_to_window_clip_ptr(&res, &world, &model, &projection, &viewport, clip);
    return res;
}

/**
 * @brief Un-projects a window coordinate using model, projection,
 * viewport and clip space.
 *
 * @see vec3_window_to_world_clip_ptr
 *
 * @param window The window coordinate to un-project.
 * @param model The model matrix.
 * @param projection The projection matrix.
 * @param viewport The viewport (x, y, width, height).
 * @param clip The clip space convention.
 * @return The unprojected vector3.
 */
vector3 vec3_window_to_world_clip(const vector3 window, const matrix4 model,
                                  const matrix4 projection, const vector4 viewport,
                                  const vm_clip_t clip)
{
    vector3 res;
    vec3_window_to_world_clip_ptr(&res, &window, &model, &projection, &viewport, clip);
    return res;
}
