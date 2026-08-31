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

void mat4_mul_ptr(matrix4 *res, const matrix4 *a, const matrix4 *b)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    mat4_mul_ptr_(res, a, b);
#else
    mat4_mul_ptr_scalar(res, a, b);
#endif
}

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
 * @brief Sets the matrix to a rotation matrix around the specified axis by the given angle.
 *
 * The rotation is performed around the normalized axis vector by the specified angle in degrees.
 * The resulting matrix is a 4x4 rotation matrix stored in column-major order.
 *
 * @param res Pointer to the matrix4 to store the resulting rotation matrix.
 * @param axis Pointer to the vector3 representing the axis of rotation.
 * @param angle The rotation angle in degrees.
 */
void mat4_rotation_ptr(matrix4 *res, const vector3 *axis, const vm_float_t angle)
{
    const vector3 normalized_axis = vec3_normalize(*axis);
    const vm_float_t rad = deg_to_rad(angle);
    const vm_float_t c = VECMAT_COS(rad);
    const vm_float_t s = VECMAT_SIN(rad);
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
 * @brief Builds a 4x4 rotation matrix around the X axis (degrees).
 *
 * @param res Output value.
 * @param degrees Rotation angle in degrees.
 */
void mat4_rotation_x_ptr(matrix4 *res, const vm_float_t degrees)
{
    const vm_float_t rad = deg_to_rad(degrees);
    const vm_float_t c = VECMAT_COS(rad);
    const vm_float_t s = VECMAT_SIN(rad);
    mat4_identity_ptr(res);
    res->m22 = c;
    res->m32 = s;
    res->m23 = -s;
    res->m33 = c;
}

/**
 * @brief Builds a 4x4 rotation matrix around the Y axis (degrees).
 *
 * @param res Output value.
 * @param degrees Rotation angle in degrees.
 */
void mat4_rotation_y_ptr(matrix4 *res, const vm_float_t degrees)
{
    const vm_float_t rad = deg_to_rad(degrees);
    const vm_float_t c = VECMAT_COS(rad);
    const vm_float_t s = VECMAT_SIN(rad);
    mat4_identity_ptr(res);
    res->m11 = c;
    res->m31 = -s;
    res->m13 = s;
    res->m33 = c;
}

/**
 * @brief Builds a 4x4 rotation matrix around the Z axis (degrees).
 *
 * @param res Output value.
 * @param degrees Rotation angle in degrees.
 */
void mat4_rotation_z_ptr(matrix4 *res, const vm_float_t degrees)
{
    const vm_float_t rad = deg_to_rad(degrees);
    const vm_float_t c = VECMAT_COS(rad);
    const vm_float_t s = VECMAT_SIN(rad);
    mat4_identity_ptr(res);
    res->m11 = c;
    res->m21 = s;
    res->m12 = -s;
    res->m22 = c;
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
 * @param fov Field of view in degrees.
 * @param aspect Aspect ratio of the viewport (width divided by height).
 * @param near Distance to the near clipping plane.
 * @param far Distance to the far clipping plane.
 */
void mat4_perspective_ptr(matrix4 *res, const vm_float_t fov, const vm_float_t aspect,
                          const vm_float_t near, const vm_float_t far)
{
    const vm_float_t rad = deg_to_rad(fov / 2.0f);
    const vm_float_t tan_half_fov = VECMAT_TAN(rad);

    for (int i = 0; i < 16; i++) res->v[i] = 0.0f;
    res->v[0] = 1.0f / (aspect * tan_half_fov);
    res->v[5] = 1.0f / tan_half_fov;
    res->v[10] = -(far + near) / (far - near);
    res->v[11] = -1.0f;
    res->v[14] = -2.0f * far * near / (far - near);
}

/**
 * @brief Sets the matrix to an orthographic projection matrix.
 *
 * This function configures the matrix for an orthographic projection,
 * mapping a rectangular frustum defined by the clipping planes to the canonical view volume.
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
    res->v[0]  = 2.0f / (right - left);
    res->v[5]  = 2.0f / (top - bottom);
    res->v[10] = -2.0f / (far - near);
    res->v[12] = -(right + left) / (right - left);
    res->v[13] = -(top + bottom) / (top - bottom);
    res->v[14] = -(far + near) / (far - near);
    res->v[15] = 1.0f;
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
 * @param fov Field of view angle in degrees.
 * @param w Viewport width.
 * @param h Viewport height.
 * @param n Near clipping plane distance.
 * @param f Far clipping plane distance.
 */
void mat4_perspective_fov_ptr(matrix4 *res, const vm_float_t fov, const vm_float_t w, const vm_float_t h,
                              const vm_float_t n, const vm_float_t f)
{
    const vm_float_t rad = deg_to_rad(fov / 2.0f);
    const vm_float_t tan_half_fov = VECMAT_TAN(rad);
    const vm_float_t aspect = w / h;

    for (int i = 0; i < 16; i++) res->v[i] = 0.0f;
    res->v[0] = 1.0f / (aspect * tan_half_fov);
    res->v[5] = 1.0f / tan_half_fov;
    res->v[10] = -(f + n) / (f - n);
    res->v[11] = -1.0f;
    res->v[14] = -2.0f * f * n / (f - n);
}

/**
 * @brief Sets the matrix to an infinite perspective projection matrix.
 *
 * This function constructs a perspective projection matrix with an infinite far plane,
 * which is useful for rendering scenes where depth precision is less critical beyond the near plane.
 * The matrix is set such that the field of view and aspect ratio are applied, with the near plane at distance n.
 *
 * @param res Pointer to the matrix4 to set to the infinite perspective projection.
 * @param fov_y Vertical field of view in degrees.
 * @param aspect Aspect ratio of the viewport (width / height).
 * @param n Distance to the near clipping plane.
 */
void mat4_perspective_infinite_ptr(matrix4 *res, vm_float_t const fov_y, vm_float_t const aspect, vm_float_t const n)
{
    const vm_float_t rad = deg_to_rad(fov_y / 2.0f);
    const vm_float_t tan_half_fov = VECMAT_TAN(rad);
    for (int i = 0; i < 16; i++) res->v[i] = 0.0f;
    res->v[0] = 1.0f / (aspect * tan_half_fov);
    res->v[5] = 1.0f / tan_half_fov;
    res->v[10] = -1.0f;
    res->v[11] = -1.0f;
    res->v[14] = -2.0f * n;
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

void mat4_mul_vec3_ptr(vector3 *res, const matrix4 *m, const vector3 *v, const vm_float_t w)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    mat4_mul_vec3_ptr_(res, m, v, w);
#else
    mat4_mul_vec3_ptr_scalar(res, m, v, w);
#endif
}
