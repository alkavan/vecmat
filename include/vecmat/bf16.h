// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_BF16_H
#define VECMAT_BF16_H

/**
 * @file
 * @brief bfloat16 storage and `vm_gemm_bf16f32`. Not a `vm_float_t` width.
 * @ingroup vecmat_extras
 *
 * Compiled into the library only when `VECMAT_ENABLE_BF16=ON`. Not part of
 * `vecmat.h`. Symbols are not in `abi.h`.
 */

#include <stdint.h>

#include "vecmat/config.h"

typedef uint16_t vm_bf16_t; /**< bfloat16 bits. Not a C type. */

typedef struct vm_vec4_bf16 {
    union {
        struct {
            vm_bf16_t x; /**< X component. */
            vm_bf16_t y; /**< Y component. */
            vm_bf16_t z; /**< Z component. */
            vm_bf16_t w; /**< W component. */
        };
        vm_bf16_t v[4];
    };
} vm_vec4_bf16;

typedef struct vm_mat4_bf16 {
    union {
        struct {
            vm_bf16_t m11; /**< Column 0, row 0. */
            vm_bf16_t m21; /**< Column 0, row 1. */
            vm_bf16_t m31; /**< Column 0, row 2. */
            vm_bf16_t m41; /**< Column 0, row 3. */
            vm_bf16_t m12; /**< Column 1, row 0. */
            vm_bf16_t m22; /**< Column 1, row 1. */
            vm_bf16_t m32; /**< Column 1, row 2. */
            vm_bf16_t m42; /**< Column 1, row 3. */
            vm_bf16_t m13; /**< Column 2, row 0. */
            vm_bf16_t m23; /**< Column 2, row 1. */
            vm_bf16_t m33; /**< Column 2, row 2. */
            vm_bf16_t m43; /**< Column 2, row 3. */
            vm_bf16_t m14; /**< Column 3, row 0. */
            vm_bf16_t m24; /**< Column 3, row 1. */
            vm_bf16_t m34; /**< Column 3, row 2. */
            vm_bf16_t m44; /**< Column 3, row 3. */
        };
        vm_bf16_t m[16]; /**< Elements in column-major order. */
    };
} vm_mat4_bf16;

/**
 * @brief Row-major `C = alpha * A * B + beta * C` with bf16 inputs and an f32 accumulator.
 *
 * No transpose and no layout flag. This is not `vm_gemm`. `lda` / `ldb` are in
 * `vm_bf16_t` elements; `ldc` is in `float` elements.
 */
typedef void (*vm_bf16_gemm_fn)(
    float *c, int ldc,
    const vm_bf16_t *a, int lda,
    const vm_bf16_t *b, int ldb,
    int m, int n, int k,
    float alpha, float beta);

/**
 * @brief Round a binary32 value to bfloat16, ties to even.
 *
 * Inf maps to inf. Every NaN maps to the canonical quiet NaN `0x7fc0`.
 */
VEC_API vm_bf16_t vm_bf16_from_f32(float x);

/** @brief Expand bfloat16 bits to binary32. */
VEC_API float vm_f32_from_bf16(vm_bf16_t h);

/**
 * @brief Install the scalar GEMM, then run `vm_bf16_modules_register()`.
 *
 * Call before the first `vm_gemm_bf16f32` if a module must override the slot.
 * A second call does not replace an already installed slot.
 */
VEC_API void vm_bf16_init(void);

/**
 * @brief Replace the GEMM slot. `fn == NULL` is ignored.
 *
 * `note` is printed by the caller via `vm_bf16_note()`. It is not `vm_cpu_note()`.
 */
VEC_API void vm_bf16_set_gemm(vm_bf16_gemm_fn fn, const char *note);

/** @brief `bf16=scalar` until a module overrides the slot. */
VEC_API const char *vm_bf16_note(void);

/** @brief See `vm_bf16_gemm_fn`. Dispatches the installed slot. */
VEC_API void vm_gemm_bf16f32(
    float *c, int ldc,
    const vm_bf16_t *a, int lda,
    const vm_bf16_t *b, int ldb,
    int m, int n, int k,
    float alpha, float beta);

#endif //VECMAT_BF16_H
