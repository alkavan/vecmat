// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <arm_sve.h>
#include <vecmat.h>

#if defined(VECMAT_USE_F64)

void vm_gemm_ukernel_sve(vm_float_t *acc, const vm_float_t *Ap, const vm_float_t *Bp, const int K)
{
    for (uint64_t i = 0; i < 8; i += svcntd()) {
        const svbool_t pg = svwhilelt_b64(i, (uint64_t)8);
        svfloat64_t c0 = svld1_f64(pg, acc + 0 + i);
        svfloat64_t c1 = svld1_f64(pg, acc + 8 + i);
        svfloat64_t c2 = svld1_f64(pg, acc + 16 + i);
        svfloat64_t c3 = svld1_f64(pg, acc + 24 + i);
        svfloat64_t c4 = svld1_f64(pg, acc + 32 + i);
        svfloat64_t c5 = svld1_f64(pg, acc + 40 + i);
        svfloat64_t c6 = svld1_f64(pg, acc + 48 + i);
        svfloat64_t c7 = svld1_f64(pg, acc + 56 + i);
        for (int k = 0; k < K; ++k) {
            const svfloat64_t a = svld1_f64(pg, Ap + k * 8 + i);
            c0 = svmla_n_f64_x(pg, c0, a, Bp[k * 8 + 0]);
            c1 = svmla_n_f64_x(pg, c1, a, Bp[k * 8 + 1]);
            c2 = svmla_n_f64_x(pg, c2, a, Bp[k * 8 + 2]);
            c3 = svmla_n_f64_x(pg, c3, a, Bp[k * 8 + 3]);
            c4 = svmla_n_f64_x(pg, c4, a, Bp[k * 8 + 4]);
            c5 = svmla_n_f64_x(pg, c5, a, Bp[k * 8 + 5]);
            c6 = svmla_n_f64_x(pg, c6, a, Bp[k * 8 + 6]);
            c7 = svmla_n_f64_x(pg, c7, a, Bp[k * 8 + 7]);
        }
        svst1_f64(pg, acc + 0 + i, c0);
        svst1_f64(pg, acc + 8 + i, c1);
        svst1_f64(pg, acc + 16 + i, c2);
        svst1_f64(pg, acc + 24 + i, c3);
        svst1_f64(pg, acc + 32 + i, c4);
        svst1_f64(pg, acc + 40 + i, c5);
        svst1_f64(pg, acc + 48 + i, c6);
        svst1_f64(pg, acc + 56 + i, c7);
    }
}

#else /* float */

void vm_gemm_ukernel_sve(vm_float_t *acc, const vm_float_t *Ap, const vm_float_t *Bp, const int K)
{
    for (uint32_t i = 0; i < 8; i += svcntw()) {
        const svbool_t pg = svwhilelt_b32(i, (uint32_t)8);
        svfloat32_t c0 = svld1_f32(pg, acc + 0 + i);
        svfloat32_t c1 = svld1_f32(pg, acc + 8 + i);
        svfloat32_t c2 = svld1_f32(pg, acc + 16 + i);
        svfloat32_t c3 = svld1_f32(pg, acc + 24 + i);
        svfloat32_t c4 = svld1_f32(pg, acc + 32 + i);
        svfloat32_t c5 = svld1_f32(pg, acc + 40 + i);
        svfloat32_t c6 = svld1_f32(pg, acc + 48 + i);
        svfloat32_t c7 = svld1_f32(pg, acc + 56 + i);
        for (int k = 0; k < K; ++k) {
            const svfloat32_t a = svld1_f32(pg, Ap + k * 8 + i);
            c0 = svmla_n_f32_x(pg, c0, a, Bp[k * 8 + 0]);
            c1 = svmla_n_f32_x(pg, c1, a, Bp[k * 8 + 1]);
            c2 = svmla_n_f32_x(pg, c2, a, Bp[k * 8 + 2]);
            c3 = svmla_n_f32_x(pg, c3, a, Bp[k * 8 + 3]);
            c4 = svmla_n_f32_x(pg, c4, a, Bp[k * 8 + 4]);
            c5 = svmla_n_f32_x(pg, c5, a, Bp[k * 8 + 5]);
            c6 = svmla_n_f32_x(pg, c6, a, Bp[k * 8 + 6]);
            c7 = svmla_n_f32_x(pg, c7, a, Bp[k * 8 + 7]);
        }
        svst1_f32(pg, acc + 0 + i, c0);
        svst1_f32(pg, acc + 8 + i, c1);
        svst1_f32(pg, acc + 16 + i, c2);
        svst1_f32(pg, acc + 24 + i, c3);
        svst1_f32(pg, acc + 32 + i, c4);
        svst1_f32(pg, acc + 40 + i, c5);
        svst1_f32(pg, acc + 48 + i, c6);
        svst1_f32(pg, acc + 56 + i, c7);
    }
}

#endif
