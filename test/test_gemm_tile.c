// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <stdlib.h>
#include <string.h>
#include <vecmat.h>
#include "unitest.h"

static void tile_fill(vm_float_t *m, const int n, const int stride, const int seed)
{
    for (int i = 0; i < n; ++i) {
        m[i] = (vm_float_t)((i * 3 + seed) % stride) - VM_F(3.0);
    }
}

static void tile_check(const int M, const int N, const int K,
                       const vm_float_t alpha, const vm_float_t beta,
                       const bool transA, const bool transB,
                       const vm_layout_t layout)
{
    const int a_rows = transA ? K : M;
    const int a_cols = transA ? M : K;
    const int b_rows = transB ? N : K;
    const int b_cols = transB ? K : N;
    const int lda = (layout == VM_LAYOUT_ROW_MAJOR) ? a_cols : a_rows;
    const int ldb = (layout == VM_LAYOUT_ROW_MAJOR) ? b_cols : b_rows;
    const int ldc = (layout == VM_LAYOUT_ROW_MAJOR) ? N : M;
    const int a_n = a_rows * a_cols;
    const int b_n = b_rows * b_cols;
    const int c_n = M * N;
    vm_float_t *A = malloc((size_t)a_n * sizeof(vm_float_t));
    vm_float_t *B = malloc((size_t)b_n * sizeof(vm_float_t));
    vm_float_t *C_fast = malloc((size_t)c_n * sizeof(vm_float_t));
    vm_float_t *C_ref = malloc((size_t)c_n * sizeof(vm_float_t));

    REQUIRE(A && B && C_fast && C_ref);
    tile_fill(A, a_n, 7, 1);
    tile_fill(B, b_n, 5, 2);
    tile_fill(C_fast, c_n, 11, 3);
    memcpy(C_ref, C_fast, (size_t)c_n * sizeof(vm_float_t));
    vm_gemm(C_fast, ldc, A, lda, B, ldb, M, N, K, alpha, beta, transA, transB, layout);
    vm_gemm_ref(C_ref, ldc, A, lda, B, ldb, M, N, K, alpha, beta, transA, transB, layout);
    for (int i = 0; i < c_n; ++i) {
        REQUIRE(VECMAT_EQ(C_fast[i], C_ref[i], EPSILON));
    }
    free(A);
    free(B);
    free(C_fast);
    free(C_ref);
}

static void tile_band(void)
{
    tile_check(3, 3, 3, VM_F(1.0), VM_F(0.0), false, false, VM_LAYOUT_COL_MAJOR);
    tile_check(4, 5, 6, VM_F(1.25), VM_F(0.5), false, false, VM_LAYOUT_COL_MAJOR);
    tile_check(13, 9, 7, VM_F(-0.5), VM_F(1.0), true, false, VM_LAYOUT_ROW_MAJOR);
    tile_check(12, 8, 11, VM_F(1.5), VM_F(0.25), false, true, VM_LAYOUT_ROW_MAJOR);
    tile_check(15, 14, 5, VM_F(0.75), VM_F(-1.0), true, true, VM_LAYOUT_COL_MAJOR);
    tile_check(11, 10, 1, VM_F(2.0), VM_F(0.0), false, false, VM_LAYOUT_ROW_MAJOR);
}

static void ukernel_6x2(vm_float_t *acc, const vm_float_t *Ap, const vm_float_t *Bp, const int K)
{
    vm_float_t c[12];
    for (int t = 0; t < 12; ++t) {
        c[t] = acc[t];
    }
    for (int k = 0; k < K; ++k) {
        const vm_float_t *a = Ap + k * 6;
        const vm_float_t *b = Bp + k * 2;
        for (int j = 0; j < 2; ++j) {
            for (int i = 0; i < 6; ++i) {
                c[i + j * 6] += a[i] * b[j];
            }
        }
    }
    for (int t = 0; t < 12; ++t) {
        acc[t] = c[t];
    }
}

TEST_CASE(vm_gemm_tile_scalar_4x4, "[gemm]")
{
    int mr = 0, nr = 0;

    vm_gemm_tile_select(0, 0);
    REQUIRE(vm_gemm_tile_select(4, 4) == 0);
    vm_gemm_tile_shape(&mr, &nr);
    REQUIRE(mr == 4 && nr == 4);
    tile_band();
    REQUIRE(vm_gemm_tile_select(8, 8) == 0);
    vm_gemm_tile_shape(&mr, &nr);
    REQUIRE(mr == 8 && nr == 8);
    tile_check(9, 7, 5, VM_F(1.0), VM_F(0.5), false, false, VM_LAYOUT_COL_MAJOR);
    vm_gemm_tile_select(0, 0);
}

TEST_CASE(vm_gemm_tile_register_and_reject, "[gemm]")
{
    int mr = 0, nr = 0;

    vm_gemm_tile_select(0, 0);
    REQUIRE(vm_gemm_tile_register(0, 2, ukernel_6x2) == -1);
    REQUIRE(vm_gemm_tile_register(8, 8, ukernel_6x2) == -1);
    REQUIRE(vm_gemm_tile_register(17, 1, ukernel_6x2) == -1);
    REQUIRE(vm_gemm_tile_select(6, 2) == -1);
    REQUIRE(vm_gemm_tile_register(6, 2, ukernel_6x2) == 0);
    REQUIRE(vm_gemm_tile_select(6, 2) == 0);
    vm_gemm_tile_shape(&mr, &nr);
    REQUIRE(mr == 6 && nr == 2);
    tile_check(6, 2, 4, VM_F(1.5), VM_F(0.25), false, true, VM_LAYOUT_ROW_MAJOR);
    tile_check(7, 5, 3, VM_F(1.0), VM_F(0.0), true, false, VM_LAYOUT_COL_MAJOR);
    vm_gemm_tile_select(0, 0);
}

TEST_CASE(vm_gemm_tile_avx2_12x8, "[gemm]")
{
    int mr = 0, nr = 0;

    vm_gemm_tile_select(0, 0);
    vm_gemm_tile_shape(&mr, &nr);
    if (vm_cpu_selected_features() == VM_CPU_AVX512F) {
        REQUIRE(mr == 16 && nr == 14);
    } else if (vm_cpu_selected_features() == VM_CPU_AVX2) {
        REQUIRE(mr == 12 && nr == 8);
    } else {
        REQUIRE(mr == 8 && nr == 8);
    }
    if (vm_cpu_has(VM_LEVEL_AVX2) && vm_gemm_tile_select(12, 8) == 0) {
        vm_gemm_tile_shape(&mr, &nr);
        REQUIRE(mr == 12 && nr == 8);
        tile_band();
        tile_check(128, 96, 64, VM_F(1.1), VM_F(0.3), false, false, VM_LAYOUT_COL_MAJOR);
    }
    vm_gemm_tile_select(0, 0);
}

TEST_CASE(vm_gemm_tile_avx512_16x14, "[gemm]")
{
    int mr = 0, nr = 0;

    vm_gemm_tile_select(0, 0);
    if (vm_cpu_has(VM_CPU_AVX512F) && vm_gemm_tile_select(16, 14) == 0) {
        vm_gemm_tile_shape(&mr, &nr);
        REQUIRE(mr == 16 && nr == 14);
        tile_band();
        tile_check(16, 14, 8, VM_F(1.0), VM_F(0.0), false, false, VM_LAYOUT_COL_MAJOR);
        tile_check(17, 15, 9, VM_F(1.25), VM_F(0.5), true, false, VM_LAYOUT_ROW_MAJOR);
        tile_check(31, 29, 13, VM_F(-0.5), VM_F(1.0), false, true, VM_LAYOUT_COL_MAJOR);
    }
    vm_gemm_tile_select(0, 0);
}
