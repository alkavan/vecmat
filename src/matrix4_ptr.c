// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <string.h>
#include <vecmat.h>
#include "features/cpu.h"

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

void mat4_scale_ptr(matrix4 *res, const vector3 *v)
{
    memset(res->v, 0, sizeof(res->v));
    res->v[0] = v->x;
    res->v[5] = v->y;
    res->v[10] = v->z;
    res->v[15] = 1.0f;
}

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

void mat4_rotation_deg_ptr(matrix4 *res, const vector3 *axis, const vm_float_t degrees)
{
    mat4_rotation_ptr(res, axis, deg_to_rad(degrees));
}

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

void mat4_rotation_x_deg_ptr(matrix4 *res, const vm_float_t degrees)
{
    mat4_rotation_x_ptr(res, deg_to_rad(degrees));
}

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

void mat4_rotation_y_deg_ptr(matrix4 *res, const vm_float_t degrees)
{
    mat4_rotation_y_ptr(res, deg_to_rad(degrees));
}

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

void mat4_rotation_z_deg_ptr(matrix4 *res, const vm_float_t degrees)
{
    mat4_rotation_z_ptr(res, deg_to_rad(degrees));
}

void mat4_from_mat3_ptr(matrix4 *res, const matrix3 *m)
{
    mat4_identity_ptr(res);
    res->m11 = m->m11; res->m21 = m->m21; res->m31 = m->m31;
    res->m12 = m->m12; res->m22 = m->m22; res->m32 = m->m32;
    res->m13 = m->m13; res->m23 = m->m23; res->m33 = m->m33;
}

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

void mat4_extract_translation_ptr(vector3 *res, const matrix4 *m)
{
    res->x = m->m14;
    res->y = m->m24;
    res->z = m->m34;
}

void mat4_extract_scale_ptr(vector3 *res, const matrix4 *m)
{
    res->x = VECMAT_SQRT(m->m11 * m->m11 + m->m21 * m->m21 + m->m31 * m->m31);
    res->y = VECMAT_SQRT(m->m12 * m->m12 + m->m22 * m->m22 + m->m32 * m->m32);
    res->z = VECMAT_SQRT(m->m13 * m->m13 + m->m23 * m->m23 + m->m33 * m->m33);
}

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

void mat4_perspective_deg_ptr(matrix4 *res, const vm_float_t fov_deg, const vm_float_t aspect,
                              const vm_float_t near, const vm_float_t far)
{
    mat4_perspective_ptr(res, deg_to_rad(fov_deg), aspect, near, far);
}

void mat4_ortho_ptr(matrix4 *res, const vm_float_t left, const vm_float_t right, const vm_float_t bottom,
                    const vm_float_t top, const vm_float_t near, const vm_float_t far)
{
    mat4_ortho_clip_ptr(res, left, right, bottom, top, near, far, VM_CLIP_RH_NO);
}

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

void mat4_perspective_fov_deg_ptr(matrix4 *res, const vm_float_t fov_deg, const vm_float_t w, const vm_float_t h,
                                  const vm_float_t n, const vm_float_t f)
{
    mat4_perspective_fov_ptr(res, deg_to_rad(fov_deg), w, h, n, f);
}

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

void mat4_perspective_clip_deg_ptr(matrix4 *res, const vm_float_t fov_y_deg, const vm_float_t aspect,
                                   const vm_float_t near, const vm_float_t far, const vm_clip_t clip)
{
    mat4_perspective_clip_ptr(res, deg_to_rad(fov_y_deg), aspect, near, far, clip);
}

void mat4_ortho_clip_ptr(matrix4 *res, const vm_float_t left, const vm_float_t right, const vm_float_t bottom,
                         const vm_float_t top, const vm_float_t near, const vm_float_t far, const vm_clip_t clip)
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

void mat4_normal_ptr(matrix4 *res, const matrix4 *m)
{
    matrix3 a;
    matrix3 n;
    mat3_from_mat4_ptr(&a, m);
    mat3_normal_ptr(&n, &a);
    mat4_from_mat3_ptr(res, &n);
}

void mat4_look_from_dir_ptr(matrix4 *res, const vector3 *position, const vector3 *direction, const vector3 *up)
{
    mat4_look_from_dir_clip_ptr(res, position, direction, up, VM_CLIP_RH_NO);
}

void mat4_look_from_dir_clip_ptr(matrix4 *res, const vector3 *position, const vector3 *direction,
                                 const vector3 *up, const vm_clip_t clip)
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

void mat4_viewport_ptr(matrix4 *res, const vm_float_t x, const vm_float_t y,
                       const vm_float_t width, const vm_float_t height)
{
    mat4_viewport_depth_ptr(res, x, y, width, height, VM_F(0.0), VM_F(1.0));
}

void mat4_viewport_depth_ptr(matrix4 *res, const vm_float_t x, const vm_float_t y, const vm_float_t width,
                             const vm_float_t height, const vm_float_t n, const vm_float_t f)
{
    mat4_identity_ptr(res);
    res->m11 = width * VM_F(0.5);
    res->m22 = height * VM_F(0.5);
    res->m33 = (f - n) * VM_F(0.5);
    res->m14 = x + width * VM_F(0.5);
    res->m24 = y + height * VM_F(0.5);
    res->m34 = (n + f) * VM_F(0.5);
}

void mat4_perspective_infinite_clip_ptr(matrix4 *res, const vm_float_t fov_y, const vm_float_t aspect,
                                        const vm_float_t n, const vm_clip_t clip)
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

void mat4_perspective_infinite_clip_deg_ptr(matrix4 *res, const vm_float_t fov_y_deg, const vm_float_t aspect,
                                            const vm_float_t n, const vm_clip_t clip)
{
    mat4_perspective_infinite_clip_ptr(res, deg_to_rad(fov_y_deg), aspect, n, clip);
}

void mat4_infinite_reverse_z_ptr(matrix4 *res, const vm_float_t fov_y, const vm_float_t aspect, const vm_float_t n)
{
    mat4_infinite_reverse_z_clip_ptr(res, fov_y, aspect, n, VM_CLIP_RH_ZO);
}

void mat4_infinite_reverse_z_clip_ptr(matrix4 *res, const vm_float_t fov_y, const vm_float_t aspect,
                                      const vm_float_t n, const vm_clip_t clip)
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

void mat4_infinite_reverse_z_deg_ptr(matrix4 *res, const vm_float_t fov_y_deg,
                                     const vm_float_t aspect, const vm_float_t n)
{
    mat4_infinite_reverse_z_ptr(res, deg_to_rad(fov_y_deg), aspect, n);
}

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

void vec3_world_to_window_clip_ptr(vector3 *res, const vector3 *world, const matrix4 *model,
                                   const matrix4 *projection, const vector4 *viewport, const vm_clip_t clip)
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

void vec3_world_to_window_ptr(vector3 *res, const vector3 *world, const matrix4 *model,
                              const matrix4 *projection, const vector4 *viewport)
{
    vec3_world_to_window_clip_ptr(res, world, model, projection, viewport, VM_CLIP_RH_NO);
}

void vec3_window_to_world_clip_ptr(vector3 *res, const vector3 *window, const matrix4 *model,
                                   const matrix4 *projection, const vector4 *viewport, const vm_clip_t clip)
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

void vec3_window_to_world_ptr(vector3 *res, const vector3 *window, const matrix4 *model,
                              const matrix4 *projection, const vector4 *viewport)
{
    vec3_window_to_world_clip_ptr(res, window, model, projection, viewport, VM_CLIP_RH_NO);
}
