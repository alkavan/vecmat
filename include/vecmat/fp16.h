// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_FP16_H
#define VECMAT_FP16_H

/**
 * @file
 * @brief IEEE binary16 storage and `vm_gemm_fp16f32`. Not a `vm_float_t` width.
 * @ingroup vecmat_extras
 *
 * Compiled into the library only when `VECMAT_ENABLE_FP16=ON`. Not part of
 * `vecmat.h`. Symbols are not in `abi.h`.
 */

#include <stdint.h>

#include "vecmat/config.h"

typedef uint16_t vm_fp16_t; /**< IEEE binary16 bits. Not a C `_Float16`. */

typedef struct vm_vec4_fp16 {
    vm_fp16_t x, y, z, w;
} vm_vec4_fp16;

typedef struct vm_mat4_fp16 {
    vm_fp16_t m[16]; /**< Storage only. Not a half-precision `matrix4`. */
} vm_mat4_fp16;

/**
 * @brief Row-major `C = alpha * A * B + beta * C` with fp16 inputs and an f32 accumulator.
 *
 * No transpose and no layout flag. This is not `vm_gemm` (that default is
 * column-major and takes transpose flags). `lda` / `ldb` are in `vm_fp16_t`
 * elements; `ldc` is in `float` elements.
 */
typedef void (*vm_fp16_gemm_fn)(
    float *c, int ldc,
    const vm_fp16_t *a, int lda,
    const vm_fp16_t *b, int ldb,
    int m, int n, int k,
    float alpha, float beta);

/**
 * @brief Round a binary32 value to binary16, ties to even.
 *
 * Inf maps to inf. Every NaN maps to the canonical quiet NaN `0x7e00`.
 * Subnormals are kept.
 */
VEC_API vm_fp16_t vm_fp16_from_f32(float x);

/** @brief Expand binary16 bits to binary32. Subnormals are kept. */
VEC_API float vm_f32_from_fp16(vm_fp16_t h);

/**
 * @brief Install the scalar GEMM, then run `vm_fp16_modules_register()`.
 *
 * Call before the first `vm_gemm_fp16f32` if a module must override the slot.
 * A second call does not replace an already installed slot.
 */
VEC_API void vm_fp16_init(void);

/**
 * @brief Replace the GEMM slot. `fn == NULL` is ignored.
 *
 * `note` is printed by the caller via `vm_fp16_note()`. It is not `vm_cpu_note()`.
 */
VEC_API void vm_fp16_set_gemm(vm_fp16_gemm_fn fn, const char *note);

/** @brief `fp16=scalar` until a module overrides the slot. */
VEC_API const char *vm_fp16_note(void);

/** @brief See `vm_fp16_gemm_fn`. Dispatches the installed slot. */
VEC_API void vm_gemm_fp16f32(
    float *c, int ldc,
    const vm_fp16_t *a, int lda,
    const vm_fp16_t *b, int ldb,
    int m, int n, int k,
    float alpha, float beta);

#endif //VECMAT_FP16_H
