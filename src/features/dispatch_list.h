// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_DISPATCH_LIST_H
#define VECMAT_DISPATCH_LIST_H

/*
 * Dispatched kernels. X(name, params, args)
 *
 * Add a new row here, implement name_scalar in the existing _ptr file (plus a
 * public `name` trampoline that calls name##_), and name_avx / name_avx2 /
 * name_avx512 / name_sve / name_sve2. dispatch.c only binds the pointers.
 */
#define VECMAT_DISPATCH_LIST(X)                                                              \
    X(vec4_add_ptr,                                                                          \
      (vector4 *res, const vector4 *a, const vector4 *b), (res, a, b))                       \
    X(vec4_sub_ptr,                                                                          \
      (vector4 *res, const vector4 *a, const vector4 *b), (res, a, b))                       \
    X(vec4_mul_ptr,                                                                          \
      (vector4 *res, const vector4 *a, const vector4 *b), (res, a, b))                       \
    X(vec4_mul_scalar_ptr,                                                                   \
      (vector4 *res, const vector4 *v, vm_float_t s), (res, v, s))                           \
    X(vec4_div_scalar_ptr,                                                                   \
      (vector4 *res, const vector4 *v, vm_float_t s), (res, v, s))                           \
    X(vec4_neg_ptr,                                                                          \
      (vector4 *res, const vector4 *v), (res, v))                                            \
    X(vec4_abs_ptr,                                                                          \
      (vector4 *res, const vector4 *v), (res, v))                                            \
    X(vec4_normalize_ptr,                                                                    \
      (vector4 *res, const vector4 *v), (res, v))                                            \
    X(vec4_min_ptr,                                                                          \
      (vector4 *res, const vector4 *a, const vector4 *b), (res, a, b))                       \
    X(vec4_max_ptr,                                                                          \
      (vector4 *res, const vector4 *a, const vector4 *b), (res, a, b))                       \
    X(vec4_lerp_ptr,                                                                         \
      (vector4 *res, const vector4 *a, const vector4 *b, vm_float_t t), (res, a, b, t))      \
    X(vec4_clamp_ptr,                                                                        \
      (vector4 *res, const vector4 *v, const vector4 *min, const vector4 *max),              \
      (res, v, min, max))                                                                    \
    X(vec4_div_ptr,                                                                          \
      (vector4 *res, const vector4 *a, const vector4 *b), (res, a, b))                       \
    X(vec4_add_scalar_ptr,                                                                   \
      (vector4 *res, const vector4 *v, vm_float_t s), (res, v, s))                           \
    X(vec4_sub_scalar_ptr,                                                                   \
      (vector4 *res, const vector4 *v, vm_float_t s), (res, v, s))                           \
    X(vec4_clamp_scalar_ptr,                                                                 \
      (vector4 *res, const vector4 *v, vm_float_t min, vm_float_t max), (res, v, min, max))  \
    X(vec4_saturate_ptr,                                                                     \
      (vector4 *res, const vector4 *v), (res, v))                                            \
    X(vec4_sign_ptr,                                                                         \
      (vector4 *res, const vector4 *v), (res, v))                                            \
    X(vec4_floor_ptr,                                                                        \
      (vector4 *res, const vector4 *v), (res, v))                                            \
    X(vec4_ceil_ptr,                                                                         \
      (vector4 *res, const vector4 *v), (res, v))                                            \
    X(vec4_round_ptr,                                                                        \
      (vector4 *res, const vector4 *v), (res, v))                                            \
    X(vec4_fract_ptr,                                                                        \
      (vector4 *res, const vector4 *v), (res, v))                                            \
    X(vec4_homogenize_ptr,                                                                   \
      (vector4 *res, const vector4 *v), (res, v))                                            \
    X(mat4_mul_ptr,                                                                          \
      (matrix4 *res, const matrix4 *a, const matrix4 *b), (res, a, b))                       \
    X(mat4_transpose_ptr,                                                                    \
      (matrix4 *res, const matrix4 *m), (res, m))                                            \
    X(mat4_mul_vec4_ptr,                                                                     \
      (vector4 *res, const matrix4 *m, const vector4 *v), (res, m, v))                       \
    X(mat4_mul_vec3_ptr,                                                                     \
      (vector3 *res, const matrix4 *m, const vector3 *v, vm_float_t w), (res, m, v, w))      \
    X(quat_mul_ptr,                                                                          \
      (quaternion *res, const quaternion *a, const quaternion *b), (res, a, b))              \
    X(quat_normalize_ptr,                                                                    \
      (quaternion *res, const quaternion *q), (res, q))

#endif /* VECMAT_DISPATCH_LIST_H */
