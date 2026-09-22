// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_BACKEND_H
#define VECMAT_BACKEND_H

/**
 * @file
 * @brief Runtime `_ptr` table (`VECMAT_DISPATCH_LIST`) and `vm_backend_register()`.
 * @ingroup vecmat_fastpath
 */

#include "vecmat/types.h"

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
      (quaternion *res, const quaternion *q), (res, q))                                      \
    X(vm_gemm_ukernel,                                                                       \
      (vm_float_t *acc, const vm_float_t *Ap, const vm_float_t *Bp, int K),                  \
      (acc, Ap, Bp, K))

/**
 * @brief Function table for a registered backend.
 *
 * Layout follows `VECMAT_DISPATCH_LIST` below. New slots are appended;
 * existing slots are never reordered.
 */
typedef struct vm_backend_ops {
#define VECMAT_BACKEND_OP(name, params, args) void (*name) params;
    VECMAT_DISPATCH_LIST(VECMAT_BACKEND_OP)
#undef VECMAT_BACKEND_OP
} vm_backend_ops;

/**
 * @brief External SIMD backend.
 *
 * Register before the first `vm_cpu_init()` (or from it). The implementation
 * must only register when the kernels can run on this CPU. Public code does
 * not `dlopen`.
 */
typedef struct vm_backend {
    const char *name;            /**< Non-NULL stable name (`neon_a55`, ...) */
    uint64_t features;           /**< ISA bits and/or private feature bits. */
    int priority;                /**< Higher wins among registered backends. */
    const vm_backend_ops *ops;   /**< Complete table; no NULL slots. */
} vm_backend;

enum {
    VM_BACKEND_OK = 0,
    VM_BACKEND_ERR_INVAL = -1,
    VM_BACKEND_ERR_NODISPATCH = -2,
    VM_BACKEND_ERR_FULL = -3,
    VM_BACKEND_ERR_CONFLICT = -4
};

#define VM_BACKEND_MAX 8

/**
 * @brief Register an external backend.
 *
 * Idempotent for the same `backend` pointer. Rejects a NULL name, NULL
 * `ops`, or any NULL slot. Same name with a different pointer is a conflict.
 *
 * @param backend Backend descriptor; must outlive the process.
 * @return `VM_BACKEND_OK` or a negative `VM_BACKEND_ERR_*` code.
 */
VEC_API int vm_backend_register(const vm_backend *backend);

#endif //VECMAT_BACKEND_H
