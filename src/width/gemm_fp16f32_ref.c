// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <stddef.h>

#include "vecmat/fp16.h"

static vm_fp16_gemm_fn gemm_slot;
static const char *fp16_note = "fp16=scalar";

void vm_fp16_modules_register(void);

static void vm_gemm_fp16f32_scalar(
    float *c, const int ldc,
    const vm_fp16_t *a, const int lda,
    const vm_fp16_t *b, const int ldb,
    const int m, const int n, const int k,
    const float alpha, const float beta)
{
    if (m <= 0 || n <= 0 || !c)
        return;

    for (int i = 0; i < m; ++i) {
        float *row = c + (size_t)i * (size_t)ldc;
        if (beta == 0.0f) {
            for (int j = 0; j < n; ++j)
                row[j] = 0.0f;
        } else if (beta != 1.0f) {
            for (int j = 0; j < n; ++j)
                row[j] *= beta;
        }
    }
    if (alpha == 0.0f || k <= 0 || !a || !b)
        return;

    for (int i = 0; i < m; ++i) {
        const vm_fp16_t *arow = a + (size_t)i * (size_t)lda;
        float *crow = c + (size_t)i * (size_t)ldc;
        for (int p = 0; p < k; ++p) {
            const float av = vm_f32_from_fp16(arow[p]);
            if (av == 0.0f)
                continue;
            const vm_fp16_t *brow = b + (size_t)p * (size_t)ldb;
            const float scale = alpha * av;
            for (int j = 0; j < n; ++j)
                crow[j] += scale * vm_f32_from_fp16(brow[j]);
        }
    }
}

void vm_fp16_init(void)
{
    if (!gemm_slot) {
        gemm_slot = vm_gemm_fp16f32_scalar;
        fp16_note = "fp16=scalar";
    }
    vm_fp16_modules_register();
}

void vm_fp16_set_gemm(const vm_fp16_gemm_fn fn, const char *note)
{
    if (!fn)
        return;
    gemm_slot = fn;
    if (note)
        fp16_note = note;
}

const char *vm_fp16_note(void)
{
    return fp16_note;
}

void vm_gemm_fp16f32(
    float *c, const int ldc,
    const vm_fp16_t *a, const int lda,
    const vm_fp16_t *b, const int ldb,
    const int m, const int n, const int k,
    const float alpha, const float beta)
{
    if (!gemm_slot)
        vm_fp16_init();
    gemm_slot(c, ldc, a, lda, b, ldb, m, n, k, alpha, beta);
}
