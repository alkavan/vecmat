// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_GEMM_H
#define VECMAT_GEMM_H

/**
 * @file
 * @brief Packed dense GEMM (`C = α op(A) op(B) + β C`), batch helpers, and `vm_im2col`. Not a BLAS.
 * @ingroup vecmat_extras
 */

#include "vecmat/types.h"
#include "vecmat/abi.h"

/**
 * @brief C = alpha * op(A) * op(B) + beta * C for dense float panels.
 *
 * Blocked packed kernel with thread-local A/B workspaces. Equivalent to
 * `vm_gemm_ex` with `op == VM_GEMM_OP_NONE`.
 *
 * @param C      Output matrix (M×N), updated in place.
 * @param ldc    Leading dimension of C.
 * @param A      Left input matrix; ignored when alpha == 0 or K <= 0.
 * @param lda    Leading dimension of A.
 * @param B      Right input matrix; ignored when alpha == 0 or K <= 0.
 * @param ldb    Leading dimension of B.
 * @param M      Number of rows of op(A) and C.
 * @param N      Number of columns of op(B) and C.
 * @param K      Inner product length.
 * @param alpha  Scale factor for the A*B product.
 * @param beta   Scale factor for the existing C values.
 * @param transA If true, use A^T; otherwise A.
 * @param transB If true, use B^T; otherwise B.
 * @param layout Memory layout for A, B, and C.
 */
VEC_API void vm_gemm(vm_float_t *C, int ldc, const vm_float_t *A, int lda,
    const vm_float_t *B, int ldb, int M, int N, int K,
    vm_float_t alpha, vm_float_t beta, bool transA, bool transB, vm_layout_t layout);

/**
 * @brief Triple-loop reference GEMM. Useful for tests and tiny/fallback paths.
 *
 * Computes `C = alpha * op(A) * op(B) + beta * C`.
 * `op(X) = X` or `X^T` according to the matching transpose flag. Layout
 * selects row-major or column-major indexing for A, B and C together.
 *
 * @param C      Output matrix (M×N), updated in place.
 * @param ldc    Leading dimension of C.
 * @param A      Left input matrix; ignored when alpha == 0 or K <= 0.
 * @param lda    Leading dimension of A.
 * @param B      Right input matrix; ignored when alpha == 0 or K <= 0.
 * @param ldb    Leading dimension of B.
 * @param M      Number of rows of op(A) and C.
 * @param N      Number of columns of op(B) and C.
 * @param K      Inner product length (columns of op(A), rows of op(B)).
 * @param alpha  Scale factor for the A*B product.
 * @param beta   Scale factor for the existing C values (0 skips reading C).
 * @param transA If true, use A^T; otherwise A.
 * @param transB If true, use B^T; otherwise B.
 * @param layout Memory layout for A, B, and C.
 */
VEC_API void vm_gemm_ref(vm_float_t *C, int ldc, const vm_float_t *A, int lda,
    const vm_float_t *B, int ldb, int M, int N, int K,
    vm_float_t alpha, vm_float_t beta, bool transA, bool transB, vm_layout_t layout);

/**
 * @brief Batched GEMM: `batch` independent GEMMs with shared shape.
 *
 * `A[p]`, `B[p]`, `C[p]` are the p-th problem. If every `B[p]` aliases the
 * same buffer, uses a shared-B pack path; otherwise runs per-item `vm_gemm`.
 * The batch range is split across worker threads when the work is large enough.
 *
 * @param C      Array of `batch` pointers to output matrices (each M×N).
 * @param ldc    Leading dimension shared by every C[p].
 * @param A      Array of `batch` pointers to left matrices, or NULL.
 * @param lda    Leading dimension shared by every A[p].
 * @param B      Array of `batch` pointers to right matrices, or NULL.
 * @param ldb    Leading dimension shared by every B[p].
 * @param M      Number of rows of op(A) and C (same for all problems).
 * @param N      Number of columns of op(B) and C (same for all problems).
 * @param K      Inner product length (same for all problems).
 * @param alpha  Scale factor for each A*B product.
 * @param beta   Scale factor for each existing C[p].
 * @param transA If true, use A^T for every problem; otherwise A.
 * @param transB If true, use B^T for every problem; otherwise B.
 * @param layout Memory layout for all A, B, and C panels.
 * @param batch  Number of independent GEMM problems.
 */
VEC_API void vm_gemm_batch(vm_float_t * const *C, int ldc, const vm_float_t * const *A,
    int lda, const vm_float_t * const *B, int ldb, int M, int N, int K,
    vm_float_t alpha, vm_float_t beta, bool transA, bool transB, vm_layout_t layout, int batch);

/**
 * @brief Strided batched GEMM: problems live `strideX` elements apart.
 *
 * Problem `p` uses `A + p * strideA`, `B + p * strideB`, `C + p * strideC`.
 * When `strideB == 0` (shared B), reuses the shared-B batch path.
 *
 * @param C       Base pointer for output matrices; problem p at C + p*strideC.
 * @param ldc     Leading dimension shared by every C panel.
 * @param strideC Element stride between consecutive C problems.
 * @param A       Base pointer for left matrices, or NULL; problem p at A + p*strideA.
 * @param lda     Leading dimension shared by every A panel.
 * @param strideA Element stride between consecutive A problems.
 * @param B       Base pointer for right matrices, or NULL; problem p at B + p*strideB.
 * @param ldb     Leading dimension shared by every B panel.
 * @param strideB Element stride between B problems (0 means one shared B).
 * @param M       Number of rows of op(A) and C (same for all problems).
 * @param N       Number of columns of op(B) and C (same for all problems).
 * @param K       Inner product length (same for all problems).
 * @param alpha   Scale factor for each A*B product.
 * @param beta    Scale factor for each existing C panel.
 * @param transA  If true, use A^T for every problem; otherwise A.
 * @param transB  If true, use B^T for every problem; otherwise B.
 * @param layout  Memory layout for all A, B, and C panels.
 * @param batch   Number of independent GEMM problems.
 */
VEC_API void vm_gemm_strided_batch(vm_float_t *C, int ldc, int strideC, const vm_float_t *A,
    int lda, int strideA, const vm_float_t *B, int ldb, int strideB, int M, int N, int K,
    vm_float_t alpha, vm_float_t beta, bool transA, bool transB, vm_layout_t layout, int batch);

/**
 * @brief C = alpha * op(A) * op(B) + beta * C with optional fused epilogue.
 *
 * Same blocked packed path as `vm_gemm`, plus last-K bias add and/or ReLU
 * when requested via `op`.
 *
 * @param C      Output matrix (M×N), updated in place.
 * @param ldc    Leading dimension of C.
 * @param A      Left input matrix; ignored when alpha == 0 or K <= 0.
 * @param lda    Leading dimension of A.
 * @param B      Right input matrix; ignored when alpha == 0 or K <= 0.
 * @param ldb    Leading dimension of B.
 * @param M      Number of rows of op(A) and C.
 * @param N      Number of columns of op(B) and C.
 * @param K      Inner product length.
 * @param alpha  Scale factor for the A*B product.
 * @param beta   Scale factor for the existing C values.
 * @param transA If true, use A^T; otherwise A.
 * @param transB If true, use B^T; otherwise B.
 * @param layout Memory layout for A, B, and C.
 * @param op     Epilogue flags (`VM_GEMM_OP_*`).
 * @param bias   Optional length-N bias vector (may be NULL).
 */
VEC_API void vm_gemm_ex(vm_float_t *C, int ldc, const vm_float_t *A, int lda, const vm_float_t *B,
    int ldb, int M, int N, int K, vm_float_t alpha, vm_float_t beta, bool transA, bool transB,
    vm_layout_t layout, int op, const vm_float_t *bias);

/**
 * @brief Resolve the GEMM worker-thread budget.
 *
 * Order: `vm_gemm_set_threads` limit if > 0; else a positive integer from
 * `VECMAT_GEMM_THREADS` (invalid values ignored); else online CPU count.
 *
 * @return Requested thread count (>= 1 from hardware fallback when unset).
 */
VEC_API int vm_gemm_threads(void);

/**
 * @brief Cap or force the GEMM worker-thread budget.
 *
 * `n > 0` sets a fixed limit used by `vm_gemm_threads`; `n == 0` (or negative,
 * treated as 0) restores auto selection via env / hardware.
 *
 * @param n Thread limit (`0` = auto, `1` = serial, `N` = cap at N).
 */
VEC_API void vm_gemm_set_threads(int n);

/**
 * @brief Micro-kernel for one packed tile: `acc[i + j*mr] += Ap[k*mr+i] * Bp[k*nr+j]`.
 *
 * `mr` and `nr` are the tile the kernel was registered with. They are not
 * arguments: the packer pads to that shape before the call.
 *
 * @param acc Accumulator tile, updated in place. Column `j` starts at `j*mr`.
 * @param Ap  Packed A panel, `mr` values per k, including zero padding.
 * @param Bp  Packed B panel, `nr` values per k, including zero padding.
 * @param K   Number of packed inner-dimension steps.
 */
typedef void (*vm_gemm_ukernel_fn)(vm_float_t *acc, const vm_float_t *Ap,
                                   const vm_float_t *Bp, int K);

/**
 * @brief Register an extra packed GEMM tile.
 *
 * The built-in 8×8 kernel stays the `vm_gemm_ukernel` dispatch slot, including
 * a registered backend, so `(8, 8)` is rejected. The same pair replaces a
 * previous registration. `ukernel` must outlive the process and must use
 * that pair's packing.
 *
 * @param mr      Tile rows, in `[1, 16]`.
 * @param nr      Tile columns, in `[1, 16]`.
 * @param ukernel Micro-kernel for this `mr`×`nr` packing.
 * @return 0, or -1 when the arguments are unusable or the table is full.
 */
VEC_API int vm_gemm_tile_register(int mr, int nr, vm_gemm_ukernel_fn ukernel);

/**
 * @brief Choose the packed tile for later `vm_gemm` calls in this process.
 *
 * `(0, 0)` restores the automatic choice: 12×8 when the selected ISA is AVX2,
 * that kernel was compiled, and no backend is registered; otherwise 8×8.
 * `(8, 8)` forces the dispatched 8×8 kernel. Any other pair must be built in
 * (scalar 4×4, AVX2 12×8) or registered.
 *
 * @param mr Tile rows. `0` together with `nr == 0` restores the automatic choice.
 * @param nr Tile columns.
 * @return 0, or -1 when that shape is not available. A failed call leaves the
 *         previous choice in place.
 */
VEC_API int vm_gemm_tile_select(int mr, int nr);

/**
 * @brief Write the tile `vm_gemm` will use now.
 *
 * @param mr Out: rows of the packed tile. May be NULL.
 * @param nr Out: columns of the packed tile. May be NULL.
 */
VEC_API void vm_gemm_tile_shape(int *mr, int *nr);

/**
 * @brief NCHW im2col into a GEMM-ready panel.
 *
 * `img` is N×C×H×W packed as `((n*C+c)*H+y)*W+x`. Output has `C*kH*kW` rows
 * and `N*outH*outW` columns (zero-filled off-image taps). `outH`/`outW` use
 * standard floor division with the given pads and strides; non-positive
 * `stride_h` / `stride_w` are treated as 1.
 *
 * @param col      Destination matrix for unfolded patches.
 * @param ld_col   Leading dimension of `col` under `layout`.
 * @param img      Source image tensor in NCHW order.
 * @param n        Batch size (N).
 * @param c        Channel count (C).
 * @param h        Input height (H).
 * @param w        Input width (W).
 * @param kh       Kernel height.
 * @param kw       Kernel width.
 * @param pad_h    Top/bottom padding in pixels.
 * @param pad_w    Left/right padding in pixels.
 * @param stride_h Vertical kernel stride (<= 0 treated as 1).
 * @param stride_w Horizontal kernel stride (<= 0 treated as 1).
 * @param layout   Row-major or column-major storage for `col`.
 */
VEC_API void vm_im2col(vm_float_t *col, int ld_col, const vm_float_t *img,
    int n, int c, int h, int w, int kh, int kw, int pad_h, int pad_w,
    int stride_h, int stride_w, vm_layout_t layout);

#endif //VECMAT_GEMM_H
