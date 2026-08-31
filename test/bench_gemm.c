// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <stdlib.h>
#include <vecmat.h>
#include "unitest.h"

static void vm_bench_gemm_fill(vm_float_t *M, const int n, const int seed)
{
    for (int i = 0; i < n; ++i) {
        M[i] = (vm_float_t)((i * 3 + seed) % 7) - VM_F(3.0);
    }
}

static vm_float_t *vm_bench_gemm_alloc(const int n)
{
    vm_float_t *p = malloc((size_t)n * sizeof(vm_float_t));
    if (!p) {
        abort();
    }
    return p;
}

BENCHMARK(vm_gemm_4x4, "iterations: 1000000 [gemm,4x4,packed]")
{
    const int iterations = 1000000;
    vm_float_t A[16], B[16], C[16];
    vm_bench_gemm_fill(A, 16, 1);
    vm_bench_gemm_fill(B, 16, 2);
    vm_bench_gemm_fill(C, 16, 3);

    for (int i = 0; i < iterations; ++i) {
        vm_gemm(C, 4, A, 4, B, 4, 4, 4, 4,
                VM_F(1.0), VM_F(0.0), false, false, VM_LAYOUT_COL_MAJOR);
        A[0] = C[0];
    }
}

BENCHMARK(vm_gemm_ref_4x4, "iterations: 1000000 [gemm,4x4,ref]")
{
    const int iterations = 1000000;
    vm_float_t A[16], B[16], C[16];
    vm_bench_gemm_fill(A, 16, 1);
    vm_bench_gemm_fill(B, 16, 2);
    vm_bench_gemm_fill(C, 16, 3);

    for (int i = 0; i < iterations; ++i) {
        vm_gemm_ref(C, 4, A, 4, B, 4, 4, 4, 4,
                    VM_F(1.0), VM_F(0.0), false, false, VM_LAYOUT_COL_MAJOR);
        A[0] = C[0];
    }
}

BENCHMARK(vm_gemm_16x16, "iterations: 20000 [gemm,16x16,packed]")
{
    const int iterations = 20000;
    const int n = 16;
    vm_float_t A[16 * 16], B[16 * 16], C[16 * 16];
    vm_bench_gemm_fill(A, n * n, 1);
    vm_bench_gemm_fill(B, n * n, 2);
    vm_bench_gemm_fill(C, n * n, 3);

    for (int i = 0; i < iterations; ++i) {
        vm_gemm(C, n, A, n, B, n, n, n, n,
                VM_F(1.0), VM_F(0.0), false, false, VM_LAYOUT_COL_MAJOR);
        A[0] = C[0];
    }
}

BENCHMARK(vm_gemm_32x32, "iterations: 4000 [gemm,32x32,packed]")
{
    const int iterations = 4000;
    const int n = 32;
    vm_float_t *A = vm_bench_gemm_alloc(n * n);
    vm_float_t *B = vm_bench_gemm_alloc(n * n);
    vm_float_t *C = vm_bench_gemm_alloc(n * n);
    vm_bench_gemm_fill(A, n * n, 1);
    vm_bench_gemm_fill(B, n * n, 2);
    vm_bench_gemm_fill(C, n * n, 3);

    for (int i = 0; i < iterations; ++i) {
        vm_gemm(C, n, A, n, B, n, n, n, n,
                VM_F(1.0), VM_F(0.0), false, false, VM_LAYOUT_COL_MAJOR);
        A[0] = C[0];
    }

    free(A);
    free(B);
    free(C);
}

BENCHMARK(vm_gemm_ref_32x32, "iterations: 4000 [gemm,32x32,ref]")
{
    const int iterations = 4000;
    const int n = 32;
    vm_float_t *A = vm_bench_gemm_alloc(n * n);
    vm_float_t *B = vm_bench_gemm_alloc(n * n);
    vm_float_t *C = vm_bench_gemm_alloc(n * n);
    vm_bench_gemm_fill(A, n * n, 1);
    vm_bench_gemm_fill(B, n * n, 2);
    vm_bench_gemm_fill(C, n * n, 3);

    for (int i = 0; i < iterations; ++i) {
        vm_gemm_ref(C, n, A, n, B, n, n, n, n,
                    VM_F(1.0), VM_F(0.0), false, false, VM_LAYOUT_COL_MAJOR);
        A[0] = C[0];
    }

    free(A);
    free(B);
    free(C);
}

BENCHMARK(vm_gemm_64x64, "iterations: 800 [gemm,64x64,packed,blocked]")
{
    const int iterations = 800;
    const int n = 64;
    vm_float_t *A = vm_bench_gemm_alloc(n * n);
    vm_float_t *B = vm_bench_gemm_alloc(n * n);
    vm_float_t *C = vm_bench_gemm_alloc(n * n);
    vm_bench_gemm_fill(A, n * n, 1);
    vm_bench_gemm_fill(B, n * n, 2);
    vm_bench_gemm_fill(C, n * n, 3);

    for (int i = 0; i < iterations; ++i) {
        vm_gemm(C, n, A, n, B, n, n, n, n,
                VM_F(1.0), VM_F(0.0), false, false, VM_LAYOUT_COL_MAJOR);
        A[0] = C[0];
    }

    free(A);
    free(B);
    free(C);
}

BENCHMARK(vm_gemm_odd_packed, "iterations: 600 [gemm,65x67x63,packed,fringe]")
{
    const int iterations = 600;
    const int M = 65, N = 67, K = 63;
    vm_float_t *A = vm_bench_gemm_alloc(M * K);
    vm_float_t *B = vm_bench_gemm_alloc(K * N);
    vm_float_t *C = vm_bench_gemm_alloc(M * N);
    vm_bench_gemm_fill(A, M * K, 1);
    vm_bench_gemm_fill(B, K * N, 2);
    vm_bench_gemm_fill(C, M * N, 3);

    for (int i = 0; i < iterations; ++i) {
        vm_gemm(C, M, A, M, B, K, M, N, K,
                VM_F(1.25), VM_F(0.5), false, false, VM_LAYOUT_COL_MAJOR);
        A[0] = C[0];
    }

    free(A);
    free(B);
    free(C);
}

BENCHMARK(vm_gemm_trans_32, "iterations: 2000 [gemm,32x32,transA,transB]")
{
    const int iterations = 2000;
    const int n = 32;
    vm_float_t *A = vm_bench_gemm_alloc(n * n);
    vm_float_t *B = vm_bench_gemm_alloc(n * n);
    vm_float_t *C = vm_bench_gemm_alloc(n * n);
    vm_bench_gemm_fill(A, n * n, 1);
    vm_bench_gemm_fill(B, n * n, 2);
    vm_bench_gemm_fill(C, n * n, 3);

    for (int i = 0; i < iterations; ++i) {
        vm_gemm(C, n, A, n, B, n, n, n, n,
                VM_F(1.0), VM_F(0.0), true, true, VM_LAYOUT_COL_MAJOR);
        A[0] = C[0];
    }

    free(A);
    free(B);
    free(C);
}

BENCHMARK(vm_gemm_row_major_32, "iterations: 2000 [gemm,32x32,row-major]")
{
    const int iterations = 2000;
    const int n = 32;
    vm_float_t *A = vm_bench_gemm_alloc(n * n);
    vm_float_t *B = vm_bench_gemm_alloc(n * n);
    vm_float_t *C = vm_bench_gemm_alloc(n * n);
    vm_bench_gemm_fill(A, n * n, 1);
    vm_bench_gemm_fill(B, n * n, 2);
    vm_bench_gemm_fill(C, n * n, 3);

    for (int i = 0; i < iterations; ++i) {
        vm_gemm(C, n, A, n, B, n, n, n, n,
                VM_F(1.0), VM_F(0.0), false, false, VM_LAYOUT_ROW_MAJOR);
        A[0] = C[0];
    }

    free(A);
    free(B);
    free(C);
}

BENCHMARK(vm_gemm_batch_8x16, "iterations: 8000 [gemm,batch,8x8x16]")
{
    const int iterations = 8000;
    const int M = 8, N = 8, K = 16, batch = 8;
    vm_float_t *A = vm_bench_gemm_alloc(batch * M * K);
    vm_float_t *B = vm_bench_gemm_alloc(batch * K * N);
    vm_float_t *C = vm_bench_gemm_alloc(batch * M * N);
    vm_float_t *Ap[8], *Bp[8], *Cp[8];
    vm_bench_gemm_fill(A, batch * M * K, 1);
    vm_bench_gemm_fill(B, batch * K * N, 2);
    vm_bench_gemm_fill(C, batch * M * N, 3);
    for (int p = 0; p < batch; ++p) {
        Ap[p] = A + p * M * K;
        Bp[p] = B + p * K * N;
        Cp[p] = C + p * M * N;
    }

    for (int i = 0; i < iterations; ++i) {
        vm_gemm_batch(Cp, M, (const vm_float_t * const *)Ap, M,
                      (const vm_float_t * const *)Bp, K,
                      M, N, K, VM_F(1.0), VM_F(0.0),
                      false, false, VM_LAYOUT_COL_MAJOR, batch);
        A[0] = C[0];
    }

    free(A);
    free(B);
    free(C);
}

BENCHMARK(vm_gemm_strided_batch_8x16, "iterations: 8000 [gemm,strided-batch,8x8x16]")
{
    const int iterations = 8000;
    const int M = 8, N = 8, K = 16, batch = 8;
    const int strideA = M * K;
    const int strideB = K * N;
    const int strideC = M * N;
    vm_float_t *A = vm_bench_gemm_alloc(batch * strideA);
    vm_float_t *B = vm_bench_gemm_alloc(batch * strideB);
    vm_float_t *C = vm_bench_gemm_alloc(batch * strideC);
    vm_bench_gemm_fill(A, batch * strideA, 1);
    vm_bench_gemm_fill(B, batch * strideB, 2);
    vm_bench_gemm_fill(C, batch * strideC, 3);

    for (int i = 0; i < iterations; ++i) {
        vm_gemm_strided_batch(C, M, strideC, A, M, strideA, B, K, strideB,
                              M, N, K, VM_F(1.0), VM_F(0.0),
                              false, false, VM_LAYOUT_COL_MAJOR, batch);
        A[0] = C[0];
    }

    free(A);
    free(B);
    free(C);
}
