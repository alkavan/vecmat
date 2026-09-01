// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <string.h>
#include <vecmat.h>
#include "features/cpu.h"

/**
 * @brief Sets the matrix to the identity matrix.
 *
 * The identity matrix is a 4x4 matrix with 1s on the main diagonal and 0s elsewhere.
 *
 * @param res Pointer to the matrix4 to set to identity.
 */
void mat4_identity_ptr(matrix4 *res)
{
    memset(res->v, 0, sizeof(res->v));  // Zero out all 16 floats
    res->v[0] = 1.0f;
    res->v[5] = 1.0f;
    res->v[10] = 1.0f;
    res->v[15] = 1.0f;
}

/**
 * @brief Multiplies two 4x4 matrices.
 *
 * This function computes the product of two input matrices and stores the result in the provided matrix.
 * The multiplication follows standard matrix multiplication rules.
 *
 * @param res Pointer to the matrix4 where the result is stored.
 * @param a Pointer to the first matrix4 operand.
 * @param b Pointer to the second matrix4 operand.
 */
VECMAT_SCALAR_API void mat4_mul_ptr_scalar(matrix4 *res, const matrix4 *a, const matrix4 *b)
{
    /* Column-major: C's column j is A times B's column j (matches mul_vec). */
    matrix4 tmp;
    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            vm_float_t s = 0.0f;
            for (int k = 0; k < 4; k++) {
                s += a->v[k * 4 + r] * b->v[c * 4 + k];
            }
            tmp.v[c * 4 + r] = s;
        }
    }
    *res = tmp;
}

/**
 * @brief Transposes the given 4x4 matrix.
 *
 * This function computes the transpose of the input matrix, where the element at row i and column j
 * becomes the element at row j and column i in the result matrix.
 *
 * @param res Pointer to the matrix4 where the transposed result will be stored.
 * @param m Pointer to the constant matrix4 to be transposed.
 */
VECMAT_SCALAR_API void mat4_transpose_ptr_scalar(matrix4 *res, const matrix4 *m)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res->v[i * 4 + j] = m->v[j * 4 + i];
        }
    }
}

/**
 * @brief Multiplies two 4x4 matrices (dispatched).
 *
 * @see mat4_mul_ptr_scalar
 *
 * @param res Result matrix.
 * @param a Left matrix.
 * @param b Right matrix.
 */
void mat4_mul_ptr(matrix4 *res, const matrix4 *a, const matrix4 *b)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    mat4_mul_ptr_(res, a, b);
#else
    mat4_mul_ptr_scalar(res, a, b);
#endif
}

/**
 * @brief Transposes a 4x4 matrix (dispatched).
 *
 * @see mat4_transpose_ptr_scalar
 *
 * @param res Result matrix.
 * @param m Input matrix.
 */
void mat4_transpose_ptr(matrix4 *res, const matrix4 *m)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    mat4_transpose_ptr_(res, m);
#else
    mat4_transpose_ptr_scalar(res, m);
#endif
}

/**
 * @brief Computes the inverse of a 4x4 matrix.
 *
 * This function calculates the inverse of the given 4x4 matrix using the adjugate matrix and determinant.
 * If the matrix is singular (determinant is zero), the result is set to the identity matrix.
 *
 * @param res Pointer to the matrix4 where the inverse will be stored.
 * @param m Pointer to the matrix4 to invert.
 */
void mat4_inverse_ptr(matrix4 *res, const matrix4 *m)
{
    vm_float_t inv[16];

    inv[0]  =  m->v[5] * m->v[10] * m->v[15] - m->v[5] * m->v[14] * m->v[11] - m->v[9] * m->v[6] * m->v[15] + m->v[9] * m->v[14] * m->v[7] + m->v[13] * m->v[6] * m->v[11] - m->v[13] * m->v[10] * m->v[7];
    inv[4]  = -m->v[4] * m->v[10] * m->v[15] + m->v[4] * m->v[14] * m->v[11] + m->v[8] * m->v[6] * m->v[15] - m->v[8] * m->v[14] * m->v[7] - m->v[12] * m->v[6] * m->v[11] + m->v[12] * m->v[10] * m->v[7];
    inv[8]  =  m->v[4] * m->v[9]  * m->v[15] - m->v[4] * m->v[13] * m->v[11] - m->v[8] * m->v[5] * m->v[15] + m->v[8] * m->v[13] * m->v[7] + m->v[12] * m->v[5] * m->v[11] - m->v[12] * m->v[9]  * m->v[7];
    inv[12] = -m->v[4] * m->v[9]  * m->v[14] + m->v[4] * m->v[13] * m->v[10] + m->v[8] * m->v[5] * m->v[14] - m->v[8] * m->v[13] * m->v[6] - m->v[12] * m->v[5] * m->v[10] + m->v[12] * m->v[9]  * m->v[6];
    inv[1]  = -m->v[1] * m->v[10] * m->v[15] + m->v[1] * m->v[14] * m->v[11] + m->v[9] * m->v[2] * m->v[15] - m->v[9] * m->v[14] * m->v[3] - m->v[13] * m->v[2] * m->v[11] + m->v[13] * m->v[10] * m->v[3];
    inv[5]  =  m->v[0] * m->v[10] * m->v[15] - m->v[0] * m->v[14] * m->v[11] - m->v[8] * m->v[2] * m->v[15] + m->v[8] * m->v[14] * m->v[3] + m->v[12] * m->v[2] * m->v[11] - m->v[12] * m->v[10] * m->v[3];
    inv[9]  = -m->v[0] * m->v[9]  * m->v[15] + m->v[0] * m->v[13] * m->v[11] + m->v[8] * m->v[1] * m->v[15] - m->v[8] * m->v[13] * m->v[3] - m->v[12] * m->v[1] * m->v[11] + m->v[12] * m->v[9]  * m->v[3];
    inv[13] =  m->v[0] * m->v[9]  * m->v[14] - m->v[0] * m->v[13] * m->v[10] - m->v[8] * m->v[1] * m->v[14] + m->v[8] * m->v[13] * m->v[2] + m->v[12] * m->v[1] * m->v[10] - m->v[12] * m->v[9]  * m->v[2];
    inv[2]  =  m->v[1] * m->v[6]  * m->v[15] - m->v[1] * m->v[14] * m->v[7]  - m->v[5] * m->v[2] * m->v[15] + m->v[5] * m->v[14] * m->v[3] + m->v[13] * m->v[2] * m->v[7]  - m->v[13] * m->v[6]  * m->v[3];
    inv[6]  = -m->v[0] * m->v[6]  * m->v[15] + m->v[0] * m->v[14] * m->v[7]  + m->v[4] * m->v[2] * m->v[15] - m->v[4] * m->v[14] * m->v[3] - m->v[12] * m->v[2] * m->v[7]  + m->v[12] * m->v[6]  * m->v[3];
    inv[10] =  m->v[0] * m->v[5]  * m->v[15] - m->v[0] * m->v[13] * m->v[7]  - m->v[4] * m->v[1] * m->v[15] + m->v[4] * m->v[13] * m->v[3] + m->v[12] * m->v[1] * m->v[7]  - m->v[12] * m->v[5]  * m->v[3];
    inv[14] = -m->v[0] * m->v[5]  * m->v[14] + m->v[0] * m->v[13] * m->v[6]  + m->v[4] * m->v[1] * m->v[14] - m->v[4] * m->v[13] * m->v[2] - m->v[12] * m->v[1] * m->v[6]  + m->v[12] * m->v[5]  * m->v[2];
    inv[3]  = -m->v[1] * m->v[6]  * m->v[11] + m->v[1] * m->v[10] * m->v[7]  + m->v[5] * m->v[2] * m->v[11] - m->v[5] * m->v[10] * m->v[3] - m->v[9]  * m->v[2] * m->v[7]  + m->v[9]  * m->v[6]  * m->v[3];
    inv[7]  =  m->v[0] * m->v[6]  * m->v[11] - m->v[0] * m->v[10] * m->v[7]  - m->v[4] * m->v[2] * m->v[11] + m->v[4] * m->v[10] * m->v[3] + m->v[8]  * m->v[2] * m->v[7]  - m->v[8]  * m->v[6]  * m->v[3];
    inv[11] = -m->v[0] * m->v[5]  * m->v[11] + m->v[0] * m->v[9]  * m->v[7]  + m->v[4] * m->v[1] * m->v[11] - m->v[4] * m->v[9]  * m->v[3] - m->v[8]  * m->v[1] * m->v[7]  + m->v[8]  * m->v[5]  * m->v[3];
    inv[15] =  m->v[0] * m->v[5]  * m->v[10] - m->v[0] * m->v[9]  * m->v[6]  - m->v[4] * m->v[1] * m->v[10] + m->v[4] * m->v[9]  * m->v[2] + m->v[8]  * m->v[1] * m->v[6]  - m->v[8]  * m->v[5]  * m->v[2];

    vm_float_t det = m->v[0] * inv[0] + m->v[1] * inv[4] + m->v[2] * inv[8] + m->v[3] * inv[12];
    if (det == 0.0f) {
        res->v[0] = 1.0f;
        res->v[5] = 1.0f;
        res->v[10] = 1.0f;
        res->v[15] = 1.0f;
        return;
    }

    det = 1.0f / det;

    for (int i = 0; i < 16; i++) {
        res->v[i] = inv[i] * det;
    }
}

/**
 * @brief Sets the matrix to a translation matrix.
 *
 * This function initializes a 4x4 matrix to represent a translation transformation
 * based on the provided vector, with the translation components placed in the
 * last column and the rest forming an identity matrix.
 *
 * @param res Pointer to the matrix4 to set.
 * @param v Pointer to the vector3 containing the translation values.
 */
void mat4_translate_ptr(matrix4 *res, const vector3 *v)
{
    res->v[0] = 1.0f;
    res->v[1] = 0.0f;
    res->v[2] = 0.0f;
    res->v[3] = 0.0f;
    res->v[4] = 0.0f;
    res->v[5] = 1.0f;
    res->v[6] = 0.0f;
    res->v[7] = 0.0f;
    res->v[8] = 0.0f;
    res->v[9] = 0.0f;
    res->v[10] = 1.0f;
    res->v[11] = 0.0f;
    res->v[12] = v->x;
    res->v[13] = v->y;
    res->v[14] = v->z;
    res->v[15] = 1.0f;
}

/**
 * @brief Sets the matrix to a scaling matrix using the provided scale vector.
 *
 * This function constructs a 4x4 scaling matrix where the diagonal elements
 * correspond to the x, y, and z scale factors from the input vector. The
 * bottom-right element is set to 1.0f for homogeneous coordinates. All other
 * elements are implicitly zero (not set, assuming the matrix is initialized).
 *
 * @param res Pointer to the matrix4 to set as a scaling matrix.
 * @param v Pointer to the vector3 containing the scale factors (x, y, z).
 */
void mat4_scale_ptr(matrix4 *res, const vector3 *v)
{
    memset(res->v, 0, sizeof(res->v));
    res->v[0] = v->x;
    res->v[5] = v->y;
    res->v[10] = v->z;
    res->v[15] = 1.0f;
}

/**
 * @brief Sets the matrix to a rotation matrix around the given axis.
 *
 * Constructs a 4x4 rotation matrix that rotates by the specified angle
 * (in radians) around the provided axis. The axis vector is normalized
 * internally.
 *
 * @param res Pointer to the matrix4 that will receive the rotation matrix.
 * @param axis Pointer to the vector3 representing the rotation axis.
 * @param radians The rotation angle in radians.
 */
void mat4_rotation_ptr(matrix4 *res, const vector3 *axis, const vm_float_t radians)
{
    const vector3 normalized_axis = vec3_normalize(*axis);
    const vm_float_t c = VECMAT_COS(radians);
    const vm_float_t s = VECMAT_SIN(radians);
    const vm_float_t omc = 1.0f - c;
    const vm_float_t x = normalized_axis.x, y = normalized_axis.y, z = normalized_axis.z;

    memset(res->v, 0, sizeof(res->v));
    res->v[0] = x * x * omc + c;
    res->v[1] = x * y * omc + z * s;
    res->v[2] = x * z * omc - y * s;
    res->v[4] = x * y * omc - z * s;
    res->v[5] = y * y * omc + c;
    res->v[6] = y * z * omc + x * s;
    res->v[8] = x * z * omc + y * s;
    res->v[9] = y * z * omc - x * s;
    res->v[10] = z * z * omc + c;
    res->v[15] = 1.0f;
}

/**
 * @brief Sets the matrix to a rotation matrix using an axis and angle in degrees.
 *
 * @see mat4_rotation_ptr
 *
 * @param res Pointer to the matrix4 that will receive the rotation matrix.
 * @param axis Pointer to the vector3 defining the rotation axis.
 * @param degrees The rotation angle in degrees.
 */
void mat4_rotation_deg_ptr(matrix4 *res, const vector3 *axis, const vm_float_t degrees)
{
    mat4_rotation_ptr(res, axis, deg_to_rad(degrees));
}

/**
 * @brief Builds a 4x4 rotation matrix around the X axis (radians).
 *
 * @param res Result matrix.
 * @param radians Rotation angle in radians.
 */
void mat4_rotation_x_ptr(matrix4 *res, const vm_float_t radians)
{
    const vm_float_t c = VECMAT_COS(radians);
    const vm_float_t s = VECMAT_SIN(radians);
    mat4_identity_ptr(res);
    res->m22 = c;
    res->m32 = s;
    res->m23 = -s;
    res->m33 = c;
}

/**
 * @brief Builds a 4x4 rotation matrix around the X axis (degrees).
 *
 * @param res Output value.
 * @param degrees Rotation angle in degrees.
 */
void mat4_rotation_x_deg_ptr(matrix4 *res, const vm_float_t degrees)
{
    mat4_rotation_x_ptr(res, deg_to_rad(degrees));
}

/**
 * @brief Sets the matrix to a rotation matrix around the Y axis.
 *
 * @param res Pointer to the matrix4 to store the result.
 * @param radians Rotation angle in radians.
 */
void mat4_rotation_y_ptr(matrix4 *res, const vm_float_t radians)
{
    const vm_float_t c = VECMAT_COS(radians);
    const vm_float_t s = VECMAT_SIN(radians);
    mat4_identity_ptr(res);
    res->m11 = c;
    res->m31 = -s;
    res->m13 = s;
    res->m33 = c;
}

/**
 * @brief Builds a 4x4 rotation matrix around the Y axis (degrees).
 *
 * @param res Output value.
 * @param degrees Rotation angle in degrees.
 */
void mat4_rotation_y_deg_ptr(matrix4 *res, const vm_float_t degrees)
{
    mat4_rotation_y_ptr(res, deg_to_rad(degrees));
}

/**
 * @brief Sets the matrix to a rotation around the Z axis.
 *
 * @param res Pointer to the matrix4 to store the result.
 * @param radians Rotation angle in radians.
 */
void mat4_rotation_z_ptr(matrix4 *res, const vm_float_t radians)
{
    const vm_float_t c = VECMAT_COS(radians);
    const vm_float_t s = VECMAT_SIN(radians);
    mat4_identity_ptr(res);
    res->m11 = c;
    res->m21 = s;
    res->m12 = -s;
    res->m22 = c;
}

/**
 * @brief Builds a 4x4 rotation matrix around the Z axis (degrees).
 *
 * @param res Output value.
 * @param degrees Rotation angle in degrees.
 */
void mat4_rotation_z_deg_ptr(matrix4 *res, const vm_float_t degrees)
{
    mat4_rotation_z_ptr(res, deg_to_rad(degrees));
}

/**
 * @brief Embeds a matrix3 into the upper-left of a matrix4.
 *
 * @param res Output value.
 * @param m Input matrix.
 */
void mat4_from_mat3_ptr(matrix4 *res, const matrix3 *m)
{
    mat4_identity_ptr(res);
    res->m11 = m->m11; res->m21 = m->m21; res->m31 = m->m31;
    res->m12 = m->m12; res->m22 = m->m22; res->m32 = m->m32;
    res->m13 = m->m13; res->m23 = m->m23; res->m33 = m->m33;
}

/**
 * @brief Builds a 4x4 TRS matrix from translation, rotation, and scale.
 *
 * @param res Output value.
 * @param translation Translation vector.
 * @param rotation Rotation quaternion.
 * @param scale Scale vector.
 */
void mat4_trs_ptr(matrix4 *res, const vector3 *translation, const quaternion *rotation, const vector3 *scale)
{
    quat_to_mat4_ptr(res, rotation);
    res->m11 *= scale->x; res->m21 *= scale->x; res->m31 *= scale->x;
    res->m12 *= scale->y; res->m22 *= scale->y; res->m32 *= scale->y;
    res->m13 *= scale->z; res->m23 *= scale->z; res->m33 *= scale->z;
    res->m14 = translation->x;
    res->m24 = translation->y;
    res->m34 = translation->z;
}

/**
 * @brief Extracts the translation vector from a matrix4.
 *
 * @param res Output value.
 * @param m Input matrix.
 */
void mat4_extract_translation_ptr(vector3 *res, const matrix4 *m)
{
    res->x = m->m14;
    res->y = m->m24;
    res->z = m->m34;
}

/**
 * @brief Extracts the scale vector from a matrix4.
 *
 * @param res Output value.
 * @param m Input matrix.
 */
void mat4_extract_scale_ptr(vector3 *res, const matrix4 *m)
{
    res->x = VECMAT_SQRT(m->m11 * m->m11 + m->m21 * m->m21 + m->m31 * m->m31);
    res->y = VECMAT_SQRT(m->m12 * m->m12 + m->m22 * m->m22 + m->m32 * m->m32);
    res->z = VECMAT_SQRT(m->m13 * m->m13 + m->m23 * m->m23 + m->m33 * m->m33);
}

/**
 * @brief Extracts the rotation quaternion from a matrix4.
 *
 * @param res Output value.
 * @param m Input matrix.
 */
void mat4_extract_rotation_ptr(quaternion *res, const matrix4 *m)
{
    vector3 s;
    mat4_extract_scale_ptr(&s, m);
    matrix3 r;
    r.m11 = (s.x != 0.0f) ? m->m11 / s.x : 0.0f;
    r.m21 = (s.x != 0.0f) ? m->m21 / s.x : 0.0f;
    r.m31 = (s.x != 0.0f) ? m->m31 / s.x : 0.0f;
    r.m12 = (s.y != 0.0f) ? m->m12 / s.y : 0.0f;
    r.m22 = (s.y != 0.0f) ? m->m22 / s.y : 0.0f;
    r.m32 = (s.y != 0.0f) ? m->m32 / s.y : 0.0f;
    r.m13 = (s.z != 0.0f) ? m->m13 / s.z : 0.0f;
    r.m23 = (s.z != 0.0f) ? m->m23 / s.z : 0.0f;
    r.m33 = (s.z != 0.0f) ? m->m33 / s.z : 0.0f;
    quat_from_mat3_ptr(res, &r);
}

/**
 * @brief Creates a perspective projection matrix.
 *
 * This function computes a right-handed perspective projection matrix based on the given field of view,
 * aspect ratio, and near and far clipping planes. The matrix is stored in the provided result pointer.
 *
 * @param res Pointer to the matrix4 to store the perspective projection matrix.
 * @param fov Field of view in radians.
 * @param aspect Aspect ratio of the viewport (width divided by height).
 * @param near Distance to the near clipping plane.
 * @param far Distance to the far clipping plane.
 */
void mat4_perspective_ptr(matrix4 *res, const vm_float_t fov, const vm_float_t aspect,
                          const vm_float_t near, const vm_float_t far)
{
    const vm_float_t tan_half_fov = VECMAT_TAN(fov * VM_F(0.5));

    for (int i = 0; i < 16; i++) res->v[i] = 0.0f;
    res->v[0] = 1.0f / (aspect * tan_half_fov);
    res->v[5] = 1.0f / tan_half_fov;
    res->v[10] = -(far + near) / (far - near);
    res->v[11] = -1.0f;
    res->v[14] = -2.0f * far * near / (far - near);
}

/**
 * @brief Sets the matrix to a perspective projection matrix using field of view in degrees.
 *
 * @see mat4_perspective_ptr
 *
 * @param res Pointer to the matrix4 to receive the perspective projection.
 * @param fov_deg Vertical field of view angle in degrees.
 * @param aspect Aspect ratio (width / height).
 * @param near Distance to the near clipping plane.
 * @param far Distance to the far clipping plane.
 */
void mat4_perspective_deg_ptr(matrix4 *res, const vm_float_t fov_deg, const vm_float_t aspect,
                              const vm_float_t near, const vm_float_t far)
{
    mat4_perspective_ptr(res, deg_to_rad(fov_deg), aspect, near, far);
}

/**
 * @brief Sets the matrix to an orthographic projection matrix.
 *
 * @see mat4_ortho_clip_ptr
 *
 * @param res Pointer to the matrix4 to set to the orthographic projection matrix.
 * @param left The left clipping plane coordinate.
 * @param right The right clipping plane coordinate.
 * @param bottom The bottom clipping plane coordinate.
 * @param top The top clipping plane coordinate.
 * @param near The near clipping plane coordinate.
 * @param far The far clipping plane coordinate.
 */
void mat4_ortho_ptr(matrix4 *res, const vm_float_t left, const vm_float_t right, const vm_float_t bottom,
                    const vm_float_t top, const vm_float_t near, const vm_float_t far)
{
    mat4_ortho_clip_ptr(res, left, right, bottom, top, near, far, VM_CLIP_RH_NO);
}

/**
 * @brief Constructs a view matrix for a camera positioned at the given location,
 *        looking towards a target, with a specified up direction.
 *
 * This function computes a 4x4 view matrix that orients the camera at the position vector,
 * directing it towards the target vector, while aligning the up direction.
 * The matrix is stored in column-major order.
 *
 * @param res Pointer to the matrix4 where the result is stored.
 * @param position Pointer to the vector3 representing the camera's position.
 * @param target Pointer to the vector3 representing the point the camera is looking at.
 * @param up Pointer to the vector3 representing the up direction for the camera.
 */
void mat4_look_at_ptr(matrix4 *res, const vector3 *position, const vector3 *target, const vector3 *up)
{
    const vector3 forward = vec3_normalize(vec3_sub(*target, *position));
    const vector3 right = vec3_normalize(vec3_cross(forward, *up));
    const vector3 true_up = vec3_cross(right, forward);

    *res = mat4_identity();
    res->v[0] = right.x;
    res->v[1] = true_up.x;
    res->v[2] = -forward.x;
    res->v[4] = right.y;
    res->v[5] = true_up.y;
    res->v[6] = -forward.y;
    res->v[8] = right.z;
    res->v[9] = true_up.z;
    res->v[10] = -forward.z;
    res->v[12] = -vec3_dot(right, *position);
    res->v[13] = -vec3_dot(true_up, *position);
    res->v[14] = vec3_dot(forward, *position);
}

/**
 * @brief Sets the matrix to a perspective projection matrix.
 *
 * This function constructs a right-handed perspective projection matrix using the specified field of view,
 * viewport width and height, near clipping plane, and far clipping plane.
 *
 * @param res Pointer to the matrix4 to set to the perspective projection matrix.
 * @param fov Field of view angle in radians.
 * @param w Viewport width.
 * @param h Viewport height.
 * @param n Near clipping plane distance.
 * @param f Far clipping plane distance.
 */
void mat4_perspective_fov_ptr(matrix4 *res, const vm_float_t fov, const vm_float_t w, const vm_float_t h,
                              const vm_float_t n, const vm_float_t f)
{
    const vm_float_t tan_half_fov = VECMAT_TAN(fov * VM_F(0.5));
    const vm_float_t aspect = w / h;

    for (int i = 0; i < 16; i++) res->v[i] = 0.0f;
    res->v[0] = 1.0f / (aspect * tan_half_fov);
    res->v[5] = 1.0f / tan_half_fov;
    res->v[10] = -(f + n) / (f - n);
    res->v[11] = -1.0f;
    res->v[14] = -2.0f * f * n / (f - n);
}

/**
 * @brief Sets the matrix to a perspective projection matrix using vertical field of view in degrees.
 *
 * @see mat4_perspective_fov_ptr
 *
 * @param res Pointer to the matrix4 to be set.
 * @param fov_deg Vertical field of view angle in degrees.
 * @param w Width of the viewport.
 * @param h Height of the viewport.
 * @param n Distance to the near clipping plane.
 * @param f Distance to the far clipping plane.
 */
void mat4_perspective_fov_deg_ptr(matrix4 *res, const vm_float_t fov_deg, const vm_float_t w, const vm_float_t h,
                                  const vm_float_t n, const vm_float_t f)
{
    mat4_perspective_fov_ptr(res, deg_to_rad(fov_deg), w, h, n, f);
}

/**
 * @brief Sets the matrix to an infinite perspective projection matrix.
 *
 * This function constructs a perspective projection matrix with an infinite
 * far plane, which is useful for rendering scenes where depth precision is less
 * critical beyond the near plane. The matrix is set such that the field of view
 * and aspect ratio are applied, with the near plane at distance n.
 *
 * @param res Pointer to the matrix4 to set to the infinite perspective projection.
 * @param fov_y Vertical field of view in radians.
 * @param aspect Aspect ratio of the viewport (width / height).
 * @param n Distance to the near clipping plane.
 */
void mat4_perspective_infinite_ptr(matrix4 *res, vm_float_t const fov_y, vm_float_t const aspect, vm_float_t const n)
{
    const vm_float_t tan_half_fov = VECMAT_TAN(fov_y * VM_F(0.5));
    for (int i = 0; i < 16; i++) res->v[i] = 0.0f;
    res->v[0] = 1.0f / (aspect * tan_half_fov);
    res->v[5] = 1.0f / tan_half_fov;
    res->v[10] = -1.0f;
    res->v[11] = -1.0f;
    res->v[14] = -2.0f * n;
}

/**
 * @brief Sets the matrix to an infinite perspective projection matrix using
 *        vertical field of view in degrees.
 *
 * @param res Pointer to the matrix4 to store the resulting projection matrix.
 * @param fov_y_deg Vertical field of view angle in degrees.
 * @param aspect Aspect ratio (width / height).
 * @param n Near clipping plane distance.
 */
void mat4_perspective_infinite_deg_ptr(matrix4 *res, const vm_float_t fov_y_deg,
                                       const vm_float_t aspect, const vm_float_t n)
{
    mat4_perspective_infinite_ptr(res, deg_to_rad(fov_y_deg), aspect, n);
}

/**
 * @brief Multiplies a 4x4 matrix by a vector4.
 *
 * @param res Output value.
 * @param m Input matrix.
 * @param v Input vector.
 */
VECMAT_SCALAR_API void mat4_mul_vec4_ptr_scalar(vector4 *res, const matrix4 *m, const vector4 *v)
{
    const vm_float_t x = v->x;
    const vm_float_t y = v->y;
    const vm_float_t z = v->z;
    const vm_float_t w = v->w;
    res->x = m->m11 * x + m->m12 * y + m->m13 * z + m->m14 * w;
    res->y = m->m21 * x + m->m22 * y + m->m23 * z + m->m24 * w;
    res->z = m->m31 * x + m->m32 * y + m->m33 * z + m->m34 * w;
    res->w = m->m41 * x + m->m42 * y + m->m43 * z + m->m44 * w;
}

/**
 * @brief Transforms a vector4 by a 4x4 matrix (dispatched).
 *
 * @see mat4_mul_vec4_ptr_scalar
 *
 * @param res Result vector.
 * @param m Input matrix.
 * @param v Input vector.
 */
void mat4_mul_vec4_ptr(vector4 *res, const matrix4 *m, const vector4 *v)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    mat4_mul_vec4_ptr_(res, m, v);
#else
    mat4_mul_vec4_ptr_scalar(res, m, v);
#endif
}

/**
 * @brief Transforms a vector3 by a 4x4 matrix using homogeneous w.
 *
 * @param res Output value.
 * @param m Input matrix.
 * @param v Input vector.
 * @param w Homogeneous w component.
 */
VECMAT_SCALAR_API void mat4_mul_vec3_ptr_scalar(vector3 *res, const matrix4 *m, const vector3 *v, const vm_float_t w)
{
    const vm_float_t x = v->x;
    const vm_float_t y = v->y;
    const vm_float_t z = v->z;
    res->x = m->m11 * x + m->m12 * y + m->m13 * z + m->m14 * w;
    res->y = m->m21 * x + m->m22 * y + m->m23 * z + m->m24 * w;
    res->z = m->m31 * x + m->m32 * y + m->m33 * z + m->m34 * w;
}

/**
 * @brief Transforms a vector3 by a 4x4 matrix using homogeneous w (dispatched).
 *
 * @see mat4_mul_vec3_ptr_scalar
 *
 * @param res Result vector.
 * @param m Input matrix.
 * @param v Input vector.
 * @param w Homogeneous w component.
 */
void mat4_mul_vec3_ptr(vector3 *res, const matrix4 *m, const vector3 *v, const vm_float_t w)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    mat4_mul_vec3_ptr_(res, m, v, w);
#else
    mat4_mul_vec3_ptr_scalar(res, m, v, w);
#endif
}

/**
 * @brief Constructs a perspective projection matrix with configurable clip space.
 *
 * Computes a 4x4 perspective projection matrix based on vertical field of view,
 * aspect ratio, near and far planes, and the desired clip space convention.
 *
 * @param res Pointer to the matrix4 to receive the computed projection matrix.
 * @param fov_y Vertical field of view in radians.
 * @param aspect Aspect ratio (width / height).
 * @param near Distance to the near clipping plane.
 * @param far Distance to the far clipping plane.
 * @param clip Clip space convention to use (VM_CLIP_RH_NO, VM_CLIP_RH_ZO, VM_CLIP_LH_ZO or VM_CLIP_LH_NO).
 */
void mat4_perspective_clip_ptr(matrix4 *res, const vm_float_t fov_y, const vm_float_t aspect,
                               const vm_float_t near, const vm_float_t far, const vm_clip_t clip)
{
    const vm_float_t tan_half = VECMAT_TAN(fov_y * VM_F(0.5));
    const vm_float_t a = VM_F(1.0) / (aspect * tan_half);
    const vm_float_t b = VM_F(1.0) / tan_half;
    const vm_float_t fn = far - near;

    memset(res->v, 0, sizeof(res->v));
    res->m11 = a;
    res->m22 = b;

    switch (clip) {
    case VM_CLIP_RH_ZO:
        res->m33 = -far / fn;
        res->m43 = VM_F(-1.0);
        res->m34 = -(far * near) / fn;
        break;
    case VM_CLIP_LH_ZO:
        res->m33 = far / fn;
        res->m43 = VM_F(1.0);
        res->m34 = -(far * near) / fn;
        break;
    case VM_CLIP_LH_NO:
        res->m33 = (far + near) / fn;
        res->m43 = VM_F(1.0);
        res->m34 = VM_F(-2.0) * far * near / fn;
        break;
    case VM_CLIP_RH_NO:
    default:
        res->m33 = -(far + near) / fn;
        res->m43 = VM_F(-1.0);
        res->m34 = VM_F(-2.0) * far * near / fn;
        break;
    }
}

/**
 * @brief Sets a perspective projection matrix using vertical field of view in degrees.
 *
 * @see mat4_perspective_clip_ptr
 *
 * @param res Pointer to the matrix4 that will receive the computed projection matrix.
 * @param fov_y_deg Vertical field of view angle in degrees.
 * @param aspect Aspect ratio (width / height).
 * @param near Distance to the near clipping plane.
 * @param far Distance to the far clipping plane.
 * @param clip Clip space convention to use.
 */
void mat4_perspective_clip_deg_ptr(matrix4 *res, const vm_float_t fov_y_deg, const vm_float_t aspect,
                                   const vm_float_t near, const vm_float_t far, const vm_clip_t clip)
{
    mat4_perspective_clip_ptr(res, deg_to_rad(fov_y_deg), aspect, near, far, clip);
}

/**
 * @brief Sets the matrix to an orthographic projection matrix.
 *
 * Constructs a 4x4 orthographic projection matrix based on the provided frustum bounds
 * and the selected clip space convention.
 *
 * @param res Pointer to the matrix4 to store the result.
 * @param left Left clipping plane.
 * @param right Right clipping plane.
 * @param bottom Bottom clipping plane.
 * @param top Top clipping plane.
 * @param near Near clipping plane.
 * @param far Far clipping plane.
 * @param clip Clip space convention to use (one of the VM_CLIP_* values).
 */
void mat4_ortho_clip_ptr(matrix4 *res, const vm_float_t left, const vm_float_t right,
                         const vm_float_t bottom, const vm_float_t top,
                         const vm_float_t near, const vm_float_t far, const vm_clip_t clip)
{
    const vm_float_t rl = right - left;
    const vm_float_t tb = top - bottom;
    const vm_float_t fn = far - near;

    memset(res->v, 0, sizeof(res->v));
    res->m11 = VM_F(2.0) / rl;
    res->m22 = VM_F(2.0) / tb;
    res->m14 = -(right + left) / rl;
    res->m24 = -(top + bottom) / tb;
    res->m44 = VM_F(1.0);

    switch (clip) {
    case VM_CLIP_RH_ZO:
        res->m33 = VM_F(-1.0) / fn;
        res->m34 = -near / fn;
        break;
    case VM_CLIP_LH_ZO:
        res->m33 = VM_F(1.0) / fn;
        res->m34 = -near / fn;
        break;
    case VM_CLIP_LH_NO:
        res->m33 = VM_F(2.0) / fn;
        res->m34 = -(far + near) / fn;
        break;
    case VM_CLIP_RH_NO:
    default:
        res->m33 = VM_F(-2.0) / fn;
        res->m34 = -(far + near) / fn;
        break;
    }
}

/**
 * @brief Constructs a look-at view matrix with support for different clip space conventions.
 *
 * Computes a 4x4 view matrix that positions the camera at @p position looking towards @p target,
 * with @p up defining the world up direction. The resulting matrix is affected by the selected
 * clip space handedness and depth range via the @p clip parameter.
 *
 * For right-handed clip conventions the function delegates to mat4_look_at_ptr. For left-handed
 * conventions a dedicated basis is constructed where the camera forward direction points along
 * positive Z.
 *
 * @param res Pointer to the matrix4 that will receive the computed view matrix.
 * @param position Camera position in world space.
 * @param target Target point in world space the camera is looking at.
 * @param up World up direction vector.
 * @param clip Clip space convention that determines handedness and depth range.
 */
void mat4_look_at_clip_ptr(matrix4 *res, const vector3 *position, const vector3 *target,
                           const vector3 *up, const vm_clip_t clip)
{
    const int left_handed = (clip == VM_CLIP_LH_ZO || clip == VM_CLIP_LH_NO);

    if (!left_handed) {
        mat4_look_at_ptr(res, position, target, up);
        return;
    }

    const vector3 z_axis = vec3_normalize(vec3_sub(*target, *position));
    const vector3 x_axis = vec3_normalize(vec3_cross(*up, z_axis));
    const vector3 y_axis = vec3_cross(z_axis, x_axis);

    mat4_identity_ptr(res);
    res->v[0] = x_axis.x;
    res->v[1] = y_axis.x;
    res->v[2] = z_axis.x;
    res->v[4] = x_axis.y;
    res->v[5] = y_axis.y;
    res->v[6] = z_axis.y;
    res->v[8] = x_axis.z;
    res->v[9] = y_axis.z;
    res->v[10] = z_axis.z;
    res->v[12] = -vec3_dot(x_axis, *position);
    res->v[13] = -vec3_dot(y_axis, *position);
    res->v[14] = -vec3_dot(z_axis, *position);
}

/**
 * @brief Inverse of an affine matrix `[R t; 0 1]`.
 *
 * Inverts the upper-left 3×3 and corrects the translation. Last row is fixed
 * as `[0 0 0 1]`. For a full 4×4 inverse, use `mat4_inverse`. Singular 3×3
 * yields identity.
 *
 * @param res Output inverse matrix.
 * @param m   Affine 4×4 matrix.
 */
void mat4_inverse_affine_ptr(matrix4 *res, const matrix4 *m)
{
    matrix3 a;
    matrix3 ai;
    mat3_from_mat4_ptr(&a, m);
    mat3_inverse_ptr(&ai, &a);

    const vector3 t = { .x = m->m14, .y = m->m24, .z = m->m34 };
    const vector3 tin_v = mat3_mul_vec3(ai, t);

    mat4_from_mat3_ptr(res, &ai);
    res->m14 = -tin_v.x;
    res->m24 = -tin_v.y;
    res->m34 = -tin_v.z;
    res->m41 = VM_F(0.0);
    res->m42 = VM_F(0.0);
    res->m43 = VM_F(0.0);
    res->m44 = VM_F(1.0);
}

/**
 * @brief Inverse-transpose of the upper-left 3x3, embedded in a matrix4.
 *
 * Used to transform surface normals under a (possibly non-uniformly scaled)
 * model matrix. Last row/column stay identity.
 *
 * @param res Normal matrix as a matrix4.
 * @param m Model matrix.
 */
void mat4_normal_ptr(matrix4 *res, const matrix4 *m)
{
    matrix3 a;
    matrix3 n;
    mat3_from_mat4_ptr(&a, m);
    mat3_normal_ptr(&n, &a);
    mat4_from_mat3_ptr(res, &n);
}

/**
 * @brief View matrix from eye position and a look direction (no target point).
 *
 * Equivalent to `mat4_look_at(position, position + direction, up)` with a
 * default forward if @p direction is near zero.
 *
 * @param res View matrix.
 * @param position Eye position.
 * @param direction Look direction.
 * @param up World up direction.
 */
void mat4_look_from_dir_ptr(matrix4 *res, const vector3 *position,
                            const vector3 *direction, const vector3 *up)
{
    mat4_look_from_dir_clip_ptr(res, position, direction, up, VM_CLIP_RH_NO);
}

/**
 * @brief Clip-aware look-from-direction view matrix.
 *
 * @param res View matrix.
 * @param position Eye position.
 * @param direction World-space look direction (need not be unit).
 * @param up World up hint.
 * @param clip Handedness / depth convention.
 */
void mat4_look_from_dir_clip_ptr(matrix4 *res, const vector3 *position,
                                 const vector3 *direction, const vector3 *up,
                                 const vm_clip_t clip)
{
    vector3 dir = *direction;
    if (vec3_length_squared(dir) <= VECMAT_EPSILON * VECMAT_EPSILON) {
        const int left_handed = (clip == VM_CLIP_LH_ZO || clip == VM_CLIP_LH_NO);
        dir.x = VM_F(0.0);
        dir.y = VM_F(0.0);
        dir.z = left_handed ? VM_F(1.0) : VM_F(-1.0);
    }
    const vector3 target = vec3_add(*position, dir);
    mat4_look_at_clip_ptr(res, position, &target, up, clip);
}

/**
 * @brief Viewport matrix from origin `(x, y)` and size `(width, height)`.
 *
 * Maps NDC to window coordinates with depth in `[0, 1]`.
 * @see mat4_viewport_depth_ptr
 *
 * @param res    Output 4×4 matrix.
 * @param x      Viewport origin x.
 * @param y      Viewport origin y.
 * @param width  Viewport width.
 * @param height Viewport height.
 */
void mat4_viewport_ptr(matrix4 *res, const vm_float_t x, const vm_float_t y,
                       const vm_float_t width, const vm_float_t height)
{
    mat4_viewport_depth_ptr(res, x, y, width, height, VM_F(0.0), VM_F(1.0));
}

/**
 * @brief Viewport matrix from origin, size, and depth range `[n, f]`.
 *
 * Maps NDC to window coordinates with depth in `[n, f]`.
 *
 * @param res    Output 4×4 matrix.
 * @param x      Viewport origin x.
 * @param y      Viewport origin y.
 * @param width  Viewport width.
 * @param height Viewport height.
 * @param n      Near depth.
 * @param f      Far depth.
 */
void mat4_viewport_depth_ptr(matrix4 *res, const vm_float_t x, const vm_float_t y,
                             const vm_float_t width, const vm_float_t height,
                             const vm_float_t n, const vm_float_t f)
{
    mat4_identity_ptr(res);
    res->m11 = width * VM_F(0.5);
    res->m22 = height * VM_F(0.5);
    res->m33 = (f - n) * VM_F(0.5);
    res->m14 = x + width * VM_F(0.5);
    res->m24 = y + height * VM_F(0.5);
    res->m34 = (n + f) * VM_F(0.5);
}

/**
 * @brief Infinite perspective matrix for a chosen clip convention.
 *
 * No far plane. `fov_y` is in radians.
 *
 * @param res    Output 4×4 matrix.
 * @param fov_y  Vertical field of view in radians.
 * @param aspect Aspect ratio (width / height).
 * @param n      Near plane distance.
 * @param clip   Clip space convention (handedness and ZO/NO depth).
 */
void mat4_perspective_infinite_clip_ptr(matrix4 *res, const vm_float_t fov_y,
                                        const vm_float_t aspect, const vm_float_t n,
                                        const vm_clip_t clip)
{
    const vm_float_t tan_half = VECMAT_TAN(fov_y * VM_F(0.5));
    memset(res->v, 0, sizeof(res->v));
    res->m11 = VM_F(1.0) / (aspect * tan_half);
    res->m22 = VM_F(1.0) / tan_half;

    switch (clip) {
    case VM_CLIP_RH_ZO:
        res->m33 = VM_F(-1.0);
        res->m43 = VM_F(-1.0);
        res->m34 = -n;
        break;
    case VM_CLIP_LH_ZO:
        res->m33 = VM_F(1.0);
        res->m43 = VM_F(1.0);
        res->m34 = -n;
        break;
    case VM_CLIP_LH_NO:
        res->m33 = VM_F(1.0);
        res->m43 = VM_F(1.0);
        res->m34 = VM_F(-2.0) * n;
        break;
    case VM_CLIP_RH_NO:
    default:
        res->m33 = VM_F(-1.0);
        res->m43 = VM_F(-1.0);
        res->m34 = VM_F(-2.0) * n;
        break;
    }
}

/**
 * @brief Infinite perspective matrix from FOV in degrees and clip space.
 *
 * @see mat4_perspective_infinite_clip_ptr
 *
 * @param res       Output 4×4 matrix.
 * @param fov_y_deg Vertical field of view in degrees.
 * @param aspect    Aspect ratio (width / height).
 * @param n         Near plane distance.
 * @param clip      Clip space convention (handedness and ZO/NO depth).
 */
void mat4_perspective_infinite_clip_deg_ptr(matrix4 *res, const vm_float_t fov_y_deg,
                                            const vm_float_t aspect, const vm_float_t n,
                                            const vm_clip_t clip)
{
    mat4_perspective_infinite_clip_ptr(res, deg_to_rad(fov_y_deg), aspect, n, clip);
}

/**
 * @brief Infinite reverse-Z perspective matrix (RH, [0, 1] depth).
 *
 * @see mat4_infinite_reverse_z_clip_ptr
 *
 * @param res    Output 4×4 matrix.
 * @param fov_y  Vertical field of view in radians.
 * @param aspect Aspect ratio (width / height).
 * @param n      Near plane distance.
 */
void mat4_infinite_reverse_z_ptr(matrix4 *res, const vm_float_t fov_y,
                                 const vm_float_t aspect, const vm_float_t n)
{
    mat4_infinite_reverse_z_clip_ptr(res, fov_y, aspect, n, VM_CLIP_RH_ZO);
}

/**
 * @brief Infinite reverse-Z perspective matrix for a chosen clip convention.
 *
 * No far plane; depth is reversed for better precision. `fov_y` is in radians.
 *
 * @param res    Output 4×4 matrix.
 * @param fov_y  Vertical field of view in radians.
 * @param aspect Aspect ratio (width / height).
 * @param n      Near plane distance.
 * @param clip   Clip space convention (handedness and ZO/NO depth).
 */
void mat4_infinite_reverse_z_clip_ptr(matrix4 *res, const vm_float_t fov_y,
                                      const vm_float_t aspect, const vm_float_t n,
                                      const vm_clip_t clip)
{
    const vm_float_t tan_half = VECMAT_TAN(fov_y * VM_F(0.5));
    memset(res->v, 0, sizeof(res->v));
    res->m11 = VM_F(1.0) / (aspect * tan_half);
    res->m22 = VM_F(1.0) / tan_half;

    switch (clip) {
    case VM_CLIP_LH_ZO:
        res->m33 = VM_F(0.0);
        res->m43 = VM_F(1.0);
        res->m34 = n;
        break;
    case VM_CLIP_LH_NO:
        res->m33 = VM_F(-1.0);
        res->m43 = VM_F(1.0);
        res->m34 = VM_F(2.0) * n;
        break;
    case VM_CLIP_RH_NO:
        res->m33 = VM_F(1.0);
        res->m43 = VM_F(-1.0);
        res->m34 = VM_F(2.0) * n;
        break;
    case VM_CLIP_RH_ZO:
    default:
        res->m33 = VM_F(0.0);
        res->m43 = VM_F(-1.0);
        res->m34 = n;
        break;
    }
}

/**
 * @brief Infinite reverse-Z perspective matrix from a vertical FOV in degrees.
 *
 * @see mat4_infinite_reverse_z_ptr
 *
 * @param res       Output 4×4 matrix.
 * @param fov_y_deg Vertical field of view in degrees.
 * @param aspect    Aspect ratio (width / height).
 * @param n         Near plane distance.
 */
void mat4_infinite_reverse_z_deg_ptr(matrix4 *res, const vm_float_t fov_y_deg,
                                     const vm_float_t aspect, const vm_float_t n)
{
    mat4_infinite_reverse_z_ptr(res, deg_to_rad(fov_y_deg), aspect, n);
}

/**
 * @brief Infinite reverse-Z perspective matrix from FOV in degrees and clip space.
 *
 * @see mat4_infinite_reverse_z_clip_ptr
 *
 * @param res       Output 4×4 matrix.
 * @param fov_y_deg Vertical field of view in degrees.
 * @param aspect    Aspect ratio (width / height).
 * @param n         Near plane distance.
 * @param clip      Clip space convention (handedness and ZO/NO depth).
 */
void mat4_infinite_reverse_z_clip_deg_ptr(matrix4 *res, const vm_float_t fov_y_deg,
                                          const vm_float_t aspect, const vm_float_t n,
                                          const vm_clip_t clip)
{
    mat4_infinite_reverse_z_clip_ptr(res, deg_to_rad(fov_y_deg), aspect, n, clip);
}

/**
 * @brief Return true if clip uses a [0, 1] (ZO) depth range.
 * @param clip Clip space convention.
 */
static int vm_clip_is_zo(const vm_clip_t clip)
{
    return clip == VM_CLIP_RH_ZO || clip == VM_CLIP_LH_ZO;
}

/**
 * @brief Projects a world-space point into window coordinates using model,
 *        projection, viewport, and clip depth range.
 *
 * Applies MVP, perspective-divides to NDC, then maps x/y into the viewport.
 * Depth (`res->z`) is stored in the selected clip convention (ZO or NO).
 * If `w` is near zero, writes `(0, 0, 0)`.
 *
 * @param res        Output window-space point (x, y, depth).
 * @param world      World-space position.
 * @param model      Model matrix.
 * @param projection Projection matrix.
 * @param viewport   Viewport rectangle (x, y, width, height).
 * @param clip       Clip depth range (ZO or NO).
 */
void vec3_world_to_window_clip_ptr(vector3 *res, const vector3 *world,
                                   const matrix4 *model, const matrix4 *projection,
                                   const vector4 *viewport, const vm_clip_t clip)
{
    const matrix4 mvp = mat4_mul(*projection, *model);
    const vector4 obj = { .x = world->x, .y = world->y, .z = world->z, VM_F(.w = 1.0)};
    const vector4 clip_v = mat4_mul_vec4(mvp, obj);
    if (VECMAT_FABS(clip_v.w) <= VECMAT_EPSILON) {
        res->x = VM_F(0.0);
        res->y = VM_F(0.0);
        res->z = VM_F(0.0);
        return;
    }
    const vm_float_t inv_w = VM_F(1.0) / clip_v.w;
    const vm_float_t ndc_x = clip_v.x * inv_w;
    const vm_float_t ndc_y = clip_v.y * inv_w;
    const vm_float_t ndc_z = clip_v.z * inv_w;
    res->x = viewport->x + viewport->z * (ndc_x + VM_F(1.0)) * VM_F(0.5);
    res->y = viewport->y + viewport->w * (ndc_y + VM_F(1.0)) * VM_F(0.5);
    res->z = vm_clip_is_zo(clip) ? ndc_z : (ndc_z + VM_F(1.0)) * VM_F(0.5);
}

/**
 * @brief Projects a world-space point to window coordinates (OpenGL clip).
 *
 * @see vec3_world_to_window_clip_ptr
 *
 * @param res Window-space point.
 * @param world World-space position.
 * @param model Model matrix.
 * @param projection Projection matrix.
 * @param viewport Viewport rectangle (x, y, width, height).
 */
void vec3_world_to_window_ptr(vector3 *res, const vector3 *world,
                              const matrix4 *model, const matrix4 *projection,
                              const vector4 *viewport)
{
    vec3_world_to_window_clip_ptr(res, world, model, projection, viewport, VM_CLIP_RH_NO);
}

/**
 * @brief Un-projects a window-space point to world coordinates using model,
 *        projection, viewport, and clip depth range.
 *
 * Maps window x/y into NDC, converts depth with the selected clip convention
 * (ZO or NO), then applies the inverse MVP and perspective-divides.
 * If the viewport size or `w` is near zero, writes `(0, 0, 0)`.
 *
 * @param res        Output world-space point.
 * @param window     Window-space point (x, y, depth).
 * @param model      Model matrix.
 * @param projection Projection matrix.
 * @param viewport   Viewport rectangle (x, y, width, height).
 * @param clip       Clip depth range (ZO or NO).
 */
void vec3_window_to_world_clip_ptr(vector3 *res, const vector3 *window,
                                   const matrix4 *model, const matrix4 *projection,
                                   const vector4 *viewport, const vm_clip_t clip)
{
    const vm_float_t w = viewport->z;
    const vm_float_t h = viewport->w;
    if (VECMAT_FABS(w) <= VECMAT_EPSILON || VECMAT_FABS(h) <= VECMAT_EPSILON) {
        res->x = VM_F(0.0);
        res->y = VM_F(0.0);
        res->z = VM_F(0.0);
        return;
    }

    const vm_float_t ndc_x = VM_F(2.0) * (window->x - viewport->x) / w - VM_F(1.0);
    const vm_float_t ndc_y = VM_F(2.0) * (window->y - viewport->y) / h - VM_F(1.0);
    const vm_float_t ndc_z = vm_clip_is_zo(clip) ? window->z
                                                 : VM_F(2.0) * window->z - VM_F(1.0);

    const matrix4 mvp = mat4_mul(*projection, *model);
    const matrix4 inv = mat4_inverse(mvp);
    const vector4 ndc = { .x = ndc_x, .y = ndc_y, .z = ndc_z, VM_F(.w = 1.0)};
    const vector4 obj = mat4_mul_vec4(inv, ndc);
    if (VECMAT_FABS(obj.w) <= VECMAT_EPSILON) {
        res->x = VM_F(0.0);
        res->y = VM_F(0.0);
        res->z = VM_F(0.0);
        return;
    }
    const vm_float_t inv_w = VM_F(1.0) / obj.w;
    res->x = obj.x * inv_w;
    res->y = obj.y * inv_w;
    res->z = obj.z * inv_w;
}

/**
 * @brief Un-projects a window-space point to world coordinates using model,
 *        projection, and viewport.
 *
 * Convenience wrapper around `vec3_window_to_world_clip_ptr` with
 * `VM_CLIP_RH_NO` (right-handed, [-1, 1] depth).
 *
 * @param res        Output world-space point.
 * @param window     Window-space point (x, y, depth).
 * @param model      Model matrix.
 * @param projection Projection matrix.
 * @param viewport   Viewport rectangle (x, y, width, height).
 */
void vec3_window_to_world_ptr(vector3 *res, const vector3 *window,
                              const matrix4 *model, const matrix4 *projection,
                              const vector4 *viewport)
{
    vec3_window_to_world_clip_ptr(res, window, model, projection, viewport, VM_CLIP_RH_NO);
}
