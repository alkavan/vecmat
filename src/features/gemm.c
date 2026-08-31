// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <vecmat.h>
#include "cpu.h"
#include "gemm_kernel.h"

#if defined(_MSC_VER)
#define VM_TLS __declspec(thread)
#else
#define VM_TLS _Thread_local
#endif

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
#endif

#if defined(VECMAT_HAS_THREADS)
#if defined(_WIN32) && defined(_MSC_VER)
/* Win32 threads via windows.h */
#else
#include <pthread.h>
#endif
#endif

enum { VM_GEMM_MAX_THREADS = 16 };

/**
 * @brief GEMM thread budget: `0` = auto, `1` = serial, `N` = cap at N.
 */
static int vm_gemm_thread_limit;

/**
 * @brief Cap or force the GEMM worker-thread budget.
 *
 * `n > 0` sets a fixed limit used by `vm_gemm_threads`; `n == 0` (or negative,
 * treated as 0) restores auto selection via env / hardware.
 *
 * @param n Thread limit (`0` = auto, `1` = serial, `N` = cap at N).
 */
void vm_gemm_set_threads(const int n)
{
    vm_gemm_thread_limit = n < 0 ? 0 : n;
}

/**
 * @brief Online logical CPU count for GEMM threading defaults.
 *
 * Uses `GetSystemInfo` on Windows and `sysconf(_SC_NPROCESSORS_ONLN)` elsewhere.
 * Falls back to 1 if the probe fails or returns a non-positive value.
 *
 * @return Hardware thread count estimate (>= 1).
 */
static int vm_gemm_hw_threads(void)
{
#if defined(_WIN32)
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return info.dwNumberOfProcessors > 0 ? (int)info.dwNumberOfProcessors : 1;
#else
    const long n = sysconf(_SC_NPROCESSORS_ONLN);
    return n > 0 ? (int)n : 1;
#endif
}

/**
 * @brief Resolve the GEMM worker-thread budget.
 *
 * Order: `vm_gemm_set_threads` limit if > 0; else a positive integer from
 * `VECMAT_GEMM_THREADS` (invalid values ignored); else online CPU count.
 *
 * @return Requested thread count (>= 1 from hardware fallback when unset).
 */
int vm_gemm_threads(void)
{
    if (vm_gemm_thread_limit > 0) {
        return vm_gemm_thread_limit;
    }
    const char *env = getenv("VECMAT_GEMM_THREADS");
    if (env && env[0] != '\0') {
        char *end = NULL;
        errno = 0;
        const long n = strtol(env, &end, 10);
        if (errno != 0 || end == env || *end != '\0' || n <= 0 || n > VEC_INT_MAX) {
            /* ignore / fall through to default */
        } else {
            return (int)n;
        }
    }
    return vm_gemm_hw_threads();
}

/**
 * @brief Choose how many workers to use for a batched GEMM.
 *
 * Returns 1 for a single problem or tiny total FLOPs. Otherwise takes
 * `vm_gemm_threads()`, clamped to `[1, batch]` and `VM_GEMM_MAX_THREADS`.
 *
 * @param batch Number of independent GEMMs.
 * @param M     Rows of op(A)/C per problem.
 * @param N     Columns of op(B)/C per problem.
 * @param K     Inner-product length per problem.
 * @return Thread count to pass to `vm_gemm_parallel_for`.
 */
static int vm_gemm_pick_threads(const int batch, const int M, const int N, const int K)
{
    if (batch <= 1) {
        return 1;
    }
    const long long flops = (long long)M * (long long)N * (long long)K * (long long)batch;
    if (flops < 4096) {
        return 1;
    }
    int t = vm_gemm_threads();
    if (t < 1) {
        t = 1;
    }
    if (t > batch) {
        t = batch;
    }
    if (t > VM_GEMM_MAX_THREADS) {
        t = VM_GEMM_MAX_THREADS;
    }
    return t;
}

/**
 * @brief One parallel-for chunk: invoke `fn(begin, end, ctx)`.
 *
 * Filled by `vm_gemm_parallel_for` and passed to worker entry points.
 *
 * @param begin First index (inclusive).
 * @param end   Last index (exclusive).
 * @param fn    Callback for this subrange.
 * @param ctx   Opaque pointer forwarded to `fn`.
 */
typedef struct {
    int begin;
    int end;
    void (*fn)(int begin, int end, void *ctx);
    void *ctx;
} vm_gemm_job;

#if defined(VECMAT_HAS_THREADS) && defined(_WIN32) && defined(_MSC_VER)
/**
 * @brief Win32 worker entry for a GEMM parallel-for job.
 *
 * @param arg Pointer to a `vm_gemm_job`.
 * @return 0.
 */
static DWORD WINAPI vm_gemm_job_win(LPVOID arg)
{
    const vm_gemm_job *j = (const vm_gemm_job *)arg;
    j->fn(j->begin, j->end, j->ctx);
    return 0;
}
#elif defined(VECMAT_HAS_THREADS)
/**
 * @brief pthreads worker entry for a GEMM parallel-for job.
 *
 * @param arg Pointer to a `vm_gemm_job`.
 * @return NULL.
 */
static void *vm_gemm_job_pthread(void *arg)
{
    const vm_gemm_job *j = (const vm_gemm_job *)arg;
    j->fn(j->begin, j->end, j->ctx);
    return NULL;
}
#endif


/**
 * @brief Run `fn(begin, end, ctx)` over `[0, n)` using up to `threads` workers.
 *
 * Splits the range into roughly equal chunks (capped by `n` and
 * `VM_GEMM_MAX_THREADS`). Runs serial when threading is off, `threads <= 1`,
 * or the range is tiny. The caller thread handles the first chunk; on worker
 * create failure, remaining work is finished on the caller.
 *
 * @param n       Iteration count (no-op if <= 0).
 * @param fn      Callback for a half-open subrange `[begin, end)`.
 * @param ctx     Opaque pointer passed through to `fn`.
 * @param threads Requested worker count (clamped; <= 1 forces serial).
 */
static void vm_gemm_parallel_for(const int n, void (*fn)(int begin, int end, void *ctx),
                                void *ctx, const int threads)
{
    if (n <= 0) {
        return;
    }
    if (threads <= 1 || n == 1) {
        fn(0, n, ctx);
        return;
    }

#if !defined(VECMAT_HAS_THREADS)
    fn(0, n, ctx);
#else
    int t = threads;
    if (t > n) {
        t = n;
    }
    if (t > VM_GEMM_MAX_THREADS) {
        t = VM_GEMM_MAX_THREADS;
    }

    vm_gemm_job jobs[VM_GEMM_MAX_THREADS];
    const int chunk = (n + t - 1) / t;
    int parts = 0;
    for (int i = 0; i < t; ++i) {
        const int begin = i * chunk;
        if (begin >= n) {
            break;
        }
        int end = begin + chunk;
        if (end > n) {
            end = n;
        }
        jobs[parts].begin = begin;
        jobs[parts].end = end;
        jobs[parts].fn = fn;
        jobs[parts].ctx = ctx;
        parts++;
    }
    if (parts <= 1) {
        fn(0, n, ctx);
        return;
    }

#if defined(_WIN32) && defined(_MSC_VER)
    HANDLE handles[VM_GEMM_MAX_THREADS];
    int launched = 0;
    int fail_from = -1;
    for (int i = 1; i < parts; ++i) {
        handles[launched] = CreateThread(NULL, 0, vm_gemm_job_win, &jobs[i], 0, NULL);
        if (!handles[launched]) {
            fail_from = i;
            break;
        }
        launched++;
    }
    fn(jobs[0].begin, jobs[0].end, ctx);
    if (launched > 0) {
        WaitForMultipleObjects((DWORD)launched, handles, TRUE, INFINITE);
        for (int i = 0; i < launched; ++i) {
            CloseHandle(handles[i]);
        }
    }
    if (fail_from > 0) {
        fn(jobs[fail_from].begin, n, ctx);
    }
#else
    pthread_t thread_ids[VM_GEMM_MAX_THREADS];
    int launched = 0;
    int fail_from = -1;
    for (int i = 1; i < parts; ++i) {
        if (pthread_create(&thread_ids[launched], NULL, vm_gemm_job_pthread, &jobs[i]) != 0) {
            fail_from = i;
            break;
        }
        launched++;
    }
    fn(jobs[0].begin, jobs[0].end, ctx);
    for (int i = 0; i < launched; ++i) {
        pthread_join(thread_ids[i], NULL);
    }
    if (fail_from > 0) {
        fn(jobs[fail_from].begin, n, ctx);
    }
#endif
#endif
}

/**
 * @brief Return the smaller of two integers (panel edge clamp).
 *
 * @param a First value.
 * @param b Second value.
 * @return The minimum of `a` and `b`.
 */
static int vm_gemm_min(const int a, const int b)
{
    return a < b ? a : b;
}

/**
 * @brief Read one entry from A or B with optional transpose and layout.
 *
 * Logical indices `(r, c)` are in op(M) space; `trans` swaps them before
 * applying row- or column-major addressing.
 *
 * @param M      Matrix base pointer.
 * @param ld     Leading dimension of M.
 * @param r      Logical row in op(M).
 * @param c      Logical column in op(M).
 * @param trans  If true, treat M as transposed.
 * @param layout Row-major or column-major storage.
 * @return The value at the mapped location.
 */
static vm_float_t vm_panel_at(const vm_float_t *M, const int ld, const int r, const int c,
                              const bool trans, const vm_layout_t layout)
{
    const int rr = trans ? c : r;
    const int cc = trans ? r : c;
    if (layout == VM_LAYOUT_ROW_MAJOR) {
        return M[cc + rr * ld];
    }
    return M[rr + cc * ld];
}

/**
 * @brief Read C(r, c) according to layout (no transpose).
 *
 * @param C      Output matrix base pointer.
 * @param ldc    Leading dimension of C.
 * @param r      Row index.
 * @param c      Column index.
 * @param layout Row-major or column-major storage.
 * @return C at (r, c).
 */
static vm_float_t vm_panel_get_c(const vm_float_t *C, const int ldc, const int r, const int c,
                                 const vm_layout_t layout)
{
    if (layout == VM_LAYOUT_ROW_MAJOR) {
        return C[c + r * ldc];
    }
    return C[r + c * ldc];
}

/**
 * @brief Write C(r, c) according to layout (no transpose).
 *
 * @param C      Output matrix base pointer.
 * @param ldc    Leading dimension of C.
 * @param r      Row index.
 * @param c      Column index.
 * @param v      Value to store.
 * @param layout Row-major or column-major storage.
 */
static void vm_panel_set_c(vm_float_t *C, const int ldc, const int r, const int c,
                           const vm_float_t v, const vm_layout_t layout)
{
    if (layout == VM_LAYOUT_ROW_MAJOR) {
        C[c + r * ldc] = v;
    } else {
        C[r + c * ldc] = v;
    }
}

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
void vm_gemm_ref(vm_float_t *C, const int ldc,
                 const vm_float_t *A, const int lda,
                 const vm_float_t *B, const int ldb,
                 const int M, const int N, const int K,
                 const vm_float_t alpha, const vm_float_t beta,
                 const bool transA, const bool transB,
                 const vm_layout_t layout)
{
    if (!C || M <= 0 || N <= 0) {
        return;
    }

    for (int j = 0; j < N; ++j) {
        for (int i = 0; i < M; ++i) {
            vm_float_t acc = VM_F(0.0);
            if (alpha != VM_F(0.0) && A && B && K > 0) {
                for (int k = 0; k < K; ++k) {
                    acc += vm_panel_at(A, lda, i, k, transA, layout)
                         * vm_panel_at(B, ldb, k, j, transB, layout);
                }
                acc *= alpha;
            }
            if (beta == VM_F(0.0)) {
                vm_panel_set_c(C, ldc, i, j, acc, layout);
            } else {
                vm_panel_set_c(C, ldc, i, j,
                               acc + beta * vm_panel_get_c(C, ldc, i, j, layout),
                               layout);
            }
        }
    }
}

/**
 * @brief Scalar packed MR×NR micro-kernel.
 *
 * Updates `acc[i + j*MR] += Ap[k*MR+i] * Bp[k*NR+j]` for k in [0, K).
 * `Ap` / `Bp` must already be packed with MR/NR padding.
 *
 * @param acc Accumulator tile (MR×NR), updated in place.
 * @param Ap  Packed A panel columns (MR values per k).
 * @param Bp  Packed B panel rows (NR values per k).
 * @param K   Number of packed inner-dimension steps.
 */
VECMAT_SCALAR_API void vm_gemm_ukernel_scalar(vm_float_t *acc,
                                              const vm_float_t *Ap,
                                              const vm_float_t *Bp,
                                              const int K)
{
    vm_float_t c[VM_GEMM_MR * VM_GEMM_NR];
    for (int t = 0; t < VM_GEMM_MR * VM_GEMM_NR; ++t) {
        c[t] = acc[t];
    }
    for (int k = 0; k < K; ++k) {
        const vm_float_t *a = Ap + k * VM_GEMM_MR;
        const vm_float_t *b = Bp + k * VM_GEMM_NR;
        for (int j = 0; j < VM_GEMM_NR; ++j) {
            const vm_float_t bj = b[j];
            vm_float_t *col = c + j * VM_GEMM_MR;
            for (int i = 0; i < VM_GEMM_MR; ++i) {
                col[i] += a[i] * bj;
            }
        }
    }
    for (int t = 0; t < VM_GEMM_MR * VM_GEMM_NR; ++t) {
        acc[t] = c[t];
    }
}

/**
 * @brief Invoke the best available micro-kernel for a packed MR×NR tile.
 *
 * Uses runtime CPU dispatch when built with `VECMAT_RUNTIME_DISPATCH`;
 * otherwise calls the scalar kernel.
 *
 * @param acc Accumulator tile (MR×NR), updated in place.
 * @param Ap  Packed A micro-panel.
 * @param Bp  Packed B micro-panel.
 * @param K   Inner-dimension length for this tile.
 */
static void vm_gemm_ukernel_call(vm_float_t *acc, const vm_float_t *Ap, const vm_float_t *Bp, const int K)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    vm_gemm_ukernel_(acc, Ap, Bp, K);
#else
    vm_gemm_ukernel_scalar(acc, Ap, Bp, K);
#endif
}

/**
 * @brief Pack an A panel into contiguous MR-wide micro-panels (zero-padded).
 *
 * Layout is k-major within each MR block so the micro-kernel streams linearly.
 *
 * @param Ap     Destination pack buffer.
 * @param A      Source matrix A.
 * @param lda    Leading dimension of A.
 * @param i0     Starting row of the panel in op(A).
 * @param k0     Starting K index of the panel.
 * @param ib     Number of rows to pack.
 * @param kb     Number of K steps to pack.
 * @param transA If true, read A as transposed.
 * @param layout Row-major or column-major storage of A.
 */
static void vm_gemm_pack_a(vm_float_t *Ap,
                           const vm_float_t *A, const int lda,
                           const int i0, const int k0,
                           const int ib, const int kb,
                           const bool transA, const vm_layout_t layout)
{
    for (int ir = 0; ir < ib; ir += VM_GEMM_MR) {
        const int mr = vm_gemm_min(VM_GEMM_MR, ib - ir);
        for (int k = 0; k < kb; ++k) {
            int i = 0;
            for (; i < mr; ++i) {
                *Ap++ = vm_panel_at(A, lda, i0 + ir + i, k0 + k, transA, layout);
            }
            for (; i < VM_GEMM_MR; ++i) {
                *Ap++ = VM_F(0.0);
            }
        }
    }
}

/**
 * @brief Pack a B panel into contiguous NR-wide micro-panels (zero-padded).
 *
 * Layout is k-major within each NR block so the micro-kernel streams linearly.
 *
 * @param Bp     Destination pack buffer.
 * @param B      Source matrix B.
 * @param ldb    Leading dimension of B.
 * @param k0     Starting K index of the panel.
 * @param j0     Starting column of the panel in op(B).
 * @param kb     Number of K steps to pack.
 * @param jb     Number of columns to pack.
 * @param transB If true, read B as transposed.
 * @param layout Row-major or column-major storage of B.
 */
static void vm_gemm_pack_b(vm_float_t *Bp,
                           const vm_float_t *B, const int ldb,
                           const int k0, const int j0,
                           const int kb, const int jb,
                           const bool transB, const vm_layout_t layout)
{
    for (int jr = 0; jr < jb; jr += VM_GEMM_NR) {
        const int nr = vm_gemm_min(VM_GEMM_NR, jb - jr);
        for (int k = 0; k < kb; ++k) {
            int j = 0;
            for (; j < nr; ++j) {
                *Bp++ = vm_panel_at(B, ldb, k0 + k, j0 + jr + j, transB, layout);
            }
            for (; j < VM_GEMM_NR; ++j) {
                *Bp++ = VM_F(0.0);
            }
        }
    }
}

/**
 * @brief Write a micro-kernel tile into C with alpha/beta and optional epilogue.
 *
 * Computes `C = alpha * acc + beta * C`, then on the last K panel may add
 * per-column `bias` and/or apply ReLU when `op` requests them.
 *
 * @param C      Output matrix.
 * @param ldc    Leading dimension of C.
 * @param acc    MR×NR accumulator from the micro-kernel.
 * @param i0     Destination row origin in C.
 * @param j0     Destination column origin in C.
 * @param mr     Active rows in this tile (<= MR).
 * @param nr     Active columns in this tile (<= NR).
 * @param alpha  Scale for the accumulator.
 * @param beta   Scale for existing C (0 skips the read).
 * @param layout Row-major or column-major storage of C.
 * @param op     Epilogue flags (`VM_GEMM_OP_*`); ignored unless this is last K.
 * @param bias   Optional length-N bias vector (used with `VM_GEMM_OP_BIAS`).
 */
static void vm_gemm_store_tile(vm_float_t *C, const int ldc,
                               const vm_float_t *acc,
                               const int i0, const int j0,
                               const int mr, const int nr,
                               const vm_float_t alpha, const vm_float_t beta,
                               const vm_layout_t layout,
                               const int op, const vm_float_t *bias)
{
    const int last_k = (op != VM_GEMM_OP_NONE);
    for (int j = 0; j < nr; ++j) {
        const vm_float_t bj = (bias && (op & VM_GEMM_OP_BIAS) && last_k)
                            ? bias[j0 + j] : VM_F(0.0);
        for (int i = 0; i < mr; ++i) {
            vm_float_t v = alpha * acc[i + j * VM_GEMM_MR];
            if (beta == VM_F(0.0)) {
                v += bj;
            } else {
                v += beta * vm_panel_get_c(C, ldc, i0 + i, j0 + j, layout) + bj;
            }
            if ((op & VM_GEMM_OP_RELU) && last_k && v < VM_F(0.0)) {
                v = VM_F(0.0);
            }
            vm_panel_set_c(C, ldc, i0 + i, j0 + j, v, layout);
        }
    }
}

/**
 * @brief Apply C = beta * C over an M×N panel (alpha==0 / empty product path).
 *
 * @param C      Output matrix to scale in place.
 * @param ldc    Leading dimension of C.
 * @param M      Number of rows.
 * @param N      Number of columns.
 * @param beta   Scale factor (0 zeros the panel).
 * @param layout Row-major or column-major storage of C.
 */
static void vm_gemm_scale_c(vm_float_t *C, const int ldc,
                            const int M, const int N,
                            const vm_float_t beta, const vm_layout_t layout)
{
    for (int j = 0; j < N; ++j) {
        for (int i = 0; i < M; ++i) {
            if (beta == VM_F(0.0)) {
                vm_panel_set_c(C, ldc, i, j, VM_F(0.0), layout);
            } else {
                vm_panel_set_c(C, ldc, i, j,
                               beta * vm_panel_get_c(C, ldc, i, j, layout),
                               layout);
            }
        }
    }
}

/** Thread-local pack workspace for A/B panels (grows on demand). */
typedef struct {
    vm_float_t *Ap;  /* Packed A buffer. */
    vm_float_t *Bp;  /* Packed B buffer. */
    size_t cap_a;    /* Capacity of Ap in elements. */
    size_t cap_b;    /* Capacity of Bp in elements. */
} vm_gemm_ws;

static VM_TLS vm_gemm_ws vm_gemm_tls;

/**
 * @brief Grow a pack buffer so it holds at least `need` elements.
 *
 * @param buf  In/out pointer to the buffer (may be reallocated).
 * @param cap  In/out capacity in elements.
 * @param need Required element count.
 * @return Pointer to a buffer with capacity >= need, or NULL on OOM.
 */
static vm_float_t *vm_gemm_ws_fit(vm_float_t **buf, size_t *cap, const size_t need)
{
    if (*cap >= need) {
        return *buf;
    }
    vm_float_t *n = (vm_float_t *)realloc(*buf, need * sizeof(vm_float_t));
    if (!n) {
        return NULL;
    }
    *buf = n;
    *cap = need;
    return n;
}

/**
 * @brief Element count for an A pack covering `ib` rows and `kb` K-steps.
 *
 * @param ib Rows in the A panel.
 * @param kb K-panel width.
 * @return Number of `vm_float_t` elements required (MR-padded).
 */
static size_t vm_gemm_pack_a_elems(const int ib, const int kb)
{
    return ((size_t)((ib + VM_GEMM_MR - 1) / VM_GEMM_MR)) * (size_t)kb * VM_GEMM_MR;
}

/**
 * @brief Element count for a B pack covering `jb` columns and `kb` K-steps.
 *
 * @param jb Columns in the B panel.
 * @param kb K-panel width.
 * @return Number of `vm_float_t` elements required (NR-padded).
 */
static size_t vm_gemm_pack_b_elems(const int jb, const int kb)
{
    return ((size_t)((jb + VM_GEMM_NR - 1) / VM_GEMM_NR)) * (size_t)kb * VM_GEMM_NR;
}

/**
 * @brief Multiply A panels against a pre-packed B panel and store into C.
 *
 * Walks M in MC blocks, packs each A slab, runs the MR×NR micro-kernel, and
 * writes tiles. Epilogue ops run only when `last_k` is set.
 *
 * @param C          Output matrix.
 * @param ldc        Leading dimension of C.
 * @param A          Left matrix (or NULL to skip).
 * @param lda        Leading dimension of A.
 * @param Bp         Pre-packed B panel for this (jc, kc) block.
 * @param M          Rows of op(A)/C to process from `ic0`.
 * @param jb         Columns in the packed B panel.
 * @param kb         K width of the packed panel.
 * @param ic0        Row origin of this A/C slab in the full problem.
 * @param jc         Column origin in C / op(B).
 * @param kc         K origin used when packing A.
 * @param alpha      Scale for the product tiles.
 * @param beta_panel Beta for this K panel (beta on first panel, else 1).
 * @param transA     If true, read A as transposed.
 * @param layout     Storage layout for A and C.
 * @param last_k     Nonzero if this is the final K panel (enable epilogue).
 * @param op         Epilogue flags applied on last K (`VM_GEMM_OP_*`).
 * @param bias       Optional bias vector for `VM_GEMM_OP_BIAS`.
 */
static void vm_gemm_apply_packed(vm_float_t *C, const int ldc,
                                 const vm_float_t *A, const int lda,
                                 const vm_float_t *Bp,
                                 const int M, const int jb, const int kb,
                                 const int ic0, const int jc, const int kc,
                                 const vm_float_t alpha, const vm_float_t beta_panel,
                                 const bool transA, const vm_layout_t layout,
                                 const int last_k, const int op, const vm_float_t *bias)
{
    if (!A || !Bp) {
        return;
    }
    vm_float_t *Ap = vm_gemm_ws_fit(&vm_gemm_tls.Ap, &vm_gemm_tls.cap_a,
                                    vm_gemm_pack_a_elems(vm_gemm_min(VM_GEMM_MC, M), kb));
    if (!Ap) {
        return;
    }

    for (int ic = 0; ic < M; ic += VM_GEMM_MC) {
        const int ib = vm_gemm_min(VM_GEMM_MC, M - ic);
        vm_gemm_pack_a(Ap, A, lda, ic0 + ic, kc, ib, kb, transA, layout);
        for (int jr = 0; jr < jb; jr += VM_GEMM_NR) {
            const int nr = vm_gemm_min(VM_GEMM_NR, jb - jr);
            const vm_float_t *Bp_tile = Bp + (jr / VM_GEMM_NR) * (kb * VM_GEMM_NR);
            for (int ir = 0; ir < ib; ir += VM_GEMM_MR) {
                const int mr = vm_gemm_min(VM_GEMM_MR, ib - ir);
                const vm_float_t *Ap_tile = Ap + (ir / VM_GEMM_MR) * (kb * VM_GEMM_MR);
                vm_float_t acc[VM_GEMM_MR * VM_GEMM_NR];
                for (int t = 0; t < VM_GEMM_MR * VM_GEMM_NR; ++t) {
                    acc[t] = VM_F(0.0);
                }
                vm_gemm_ukernel_call(acc, Ap_tile, Bp_tile, kb);
                vm_gemm_store_tile(C, ldc, acc, ic0 + ic + ir, jc + jr,
                                   mr, nr, alpha, beta_panel, layout,
                                   last_k ? op : VM_GEMM_OP_NONE, bias);
            }
        }
    }
}

/**
 * @brief Core blocked GEMM with optional bias/ReLU epilogue.
 *
 * Packs B by NC×KC panels, then applies packed A×B tiles. On allocation
 * failure falls back to `vm_gemm_ref` (without epilogue). When the product
 * is empty (alpha==0 / missing inputs / K==0), scales C and still applies op.
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
 * @param beta   Scale factor for existing C on the first K panel.
 * @param transA If true, use A^T; otherwise A.
 * @param transB If true, use B^T; otherwise B.
 * @param layout Memory layout for A, B, and C.
 * @param op     Epilogue flags (`VM_GEMM_OP_NONE`, bias, ReLU, …).
 * @param bias   Optional length-N bias; used when `op` includes bias.
 */
static void vm_gemm_ex_body(vm_float_t *C, const int ldc,
                            const vm_float_t *A, const int lda,
                            const vm_float_t *B, const int ldb,
                            const int M, const int N, const int K,
                            const vm_float_t alpha, const vm_float_t beta,
                            const bool transA, const bool transB,
                            const vm_layout_t layout,
                            const int op, const vm_float_t *bias)
{
    if (!C || M <= 0 || N <= 0) {
        return;
    }

    if (alpha == VM_F(0.0) || !A || !B || K <= 0) {
        vm_gemm_scale_c(C, ldc, M, N, beta, layout);
        if (op != VM_GEMM_OP_NONE) {
            for (int j = 0; j < N; ++j) {
                const vm_float_t bj = (bias && (op & VM_GEMM_OP_BIAS)) ? bias[j] : VM_F(0.0);
                for (int i = 0; i < M; ++i) {
                    vm_float_t v = vm_panel_get_c(C, ldc, i, j, layout) + bj;
                    if ((op & VM_GEMM_OP_RELU) && v < VM_F(0.0)) {
                        v = VM_F(0.0);
                    }
                    vm_panel_set_c(C, ldc, i, j, v, layout);
                }
            }
        }
        return;
    }

    vm_float_t *Ap = vm_gemm_ws_fit(&vm_gemm_tls.Ap, &vm_gemm_tls.cap_a,
                                    vm_gemm_pack_a_elems(vm_gemm_min(VM_GEMM_MC, M),
                                                        vm_gemm_min(VM_GEMM_KC, K)));
    vm_float_t *Bp = vm_gemm_ws_fit(&vm_gemm_tls.Bp, &vm_gemm_tls.cap_b,
                                    vm_gemm_pack_b_elems(vm_gemm_min(VM_GEMM_NC, N),
                                                        vm_gemm_min(VM_GEMM_KC, K)));
    if (!Ap || !Bp) {
        vm_gemm_ref(C, ldc, A, lda, B, ldb, M, N, K, alpha, beta, transA, transB, layout);
        return;
    }

    for (int jc = 0; jc < N; jc += VM_GEMM_NC) {
        const int jb = vm_gemm_min(VM_GEMM_NC, N - jc);
        for (int kc = 0; kc < K; kc += VM_GEMM_KC) {
            const int kb = vm_gemm_min(VM_GEMM_KC, K - kc);
            const vm_float_t beta_panel = (kc == 0) ? beta : VM_F(1.0);
            const int last_k = (kc + kb >= K);
            vm_gemm_pack_b(Bp, B, ldb, kc, jc, kb, jb, transB, layout);
            vm_gemm_apply_packed(C, ldc, A, lda, Bp, M, jb, kb, 0, jc, kc,
                                 alpha, beta_panel, transA, layout,
                                 last_k, op, bias);
        }
    }
}

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
void vm_gemm_ex(vm_float_t *C, const int ldc,
                const vm_float_t *A, const int lda,
                const vm_float_t *B, const int ldb,
                const int M, const int N, const int K,
                const vm_float_t alpha, const vm_float_t beta,
                const bool transA, const bool transB,
                const vm_layout_t layout,
                const int op, const vm_float_t *bias)
{
    vm_gemm_ex_body(C, ldc, A, lda, B, ldb, M, N, K,
                    alpha, beta, transA, transB, layout, op, bias);
}

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
void vm_gemm(vm_float_t *C, const int ldc,
             const vm_float_t *A, const int lda,
             const vm_float_t *B, const int ldb,
             const int M, const int N, const int K,
             const vm_float_t alpha, const vm_float_t beta,
             const bool transA, const bool transB,
             const vm_layout_t layout)
{
    vm_gemm_ex_body(C, ldc, A, lda, B, ldb, M, N, K,
                    alpha, beta, transA, transB, layout,
                    VM_GEMM_OP_NONE, NULL);
}

/**
 * @brief Batched GEMM with one shared B, packing B once per KC/NC panel.
 *
 * Heap-allocates the shared B pack so OpenMP workers can read it concurrently.
 * Each batch item still packs its own A via thread-local workspace.
 * On pack allocation failure, falls back to per-item `vm_gemm`.
 *
 * @param C      Array of `batch` output matrix pointers.
 * @param ldc    Leading dimension shared by every C[p].
 * @param A      Array of `batch` left matrix pointers (per-item A).
 * @param lda    Leading dimension shared by every A[p].
 * @param B      Shared right matrix for all batch items.
 * @param ldb    Leading dimension of B.
 * @param M      Rows of op(A) and C (same for all problems).
 * @param N      Columns of op(B) and C (same for all problems).
 * @param K      Inner product length (same for all problems).
 * @param alpha  Scale factor for each A*B product.
 * @param beta   Scale factor for each existing C[p].
 * @param transA If true, use A^T for every problem.
 * @param transB If true, use B^T for the shared B.
 * @param layout Memory layout for all panels.
 * @param begin  First batch index (inclusive).
 * @param end    Last batch index (exclusive).
 */
static void vm_gemm_batch_shared_b(vm_float_t * const *C, const int ldc,
                                   const vm_float_t * const *A, const int lda,
                                   const vm_float_t *B, const int ldb,
                                   const int M, const int N, const int K,
                                   const vm_float_t alpha, const vm_float_t beta,
                                   const bool transA, const bool transB,
                                   const vm_layout_t layout,
                                   const int begin, const int end)
{
    if (begin >= end) {
        return;
    }
    const size_t nb = vm_gemm_pack_b_elems(vm_gemm_min(VM_GEMM_NC, N),
                                          vm_gemm_min(VM_GEMM_KC, K));
    vm_float_t *Bp = (vm_float_t *)malloc(nb * sizeof(vm_float_t));
    if (!Bp) {
        for (int p = begin; p < end; ++p) {
            vm_gemm(C[p], ldc, A ? A[p] : NULL, lda, B, ldb, M, N, K,
                    alpha, beta, transA, transB, layout);
        }
        return;
    }

    for (int jc = 0; jc < N; jc += VM_GEMM_NC) {
        const int jb = vm_gemm_min(VM_GEMM_NC, N - jc);
        for (int kc = 0; kc < K; kc += VM_GEMM_KC) {
            const int kb = vm_gemm_min(VM_GEMM_KC, K - kc);
            const vm_float_t beta_panel = (kc == 0) ? beta : VM_F(1.0);
            vm_gemm_pack_b(Bp, B, ldb, kc, jc, kb, jb, transB, layout);
            for (int p = begin; p < end; ++p) {
                if (!C[p]) {
                    continue;
                }
                vm_gemm_apply_packed(C[p], ldc, A ? A[p] : NULL, lda, Bp,
                                     M, jb, kb, 0, jc, kc,
                                     alpha, beta_panel, transA, layout,
                                     0, VM_GEMM_OP_NONE, NULL);
            }
        }
    }
    free(Bp);
}

/**
 * @brief Context for a batched GEMM parallel-for range.
 */
typedef struct {
    vm_float_t * const *C;       /**< Output panels. */
    int ldc;                     /**< Leading dimension of each C. */
    const vm_float_t * const *A; /**< Input A panels (or NULL). */
    int lda;                     /**< Leading dimension of each A. */
    const vm_float_t * const *B; /**< Per-batch B panels (or NULL). */
    const vm_float_t *B_shared;  /**< Shared B when `shared` is set. */
    int ldb;                     /**< Leading dimension of B. */
    int M, N, K;                 /**< Panel sizes. */
    vm_float_t alpha, beta;      /**< GEMM scales. */
    bool transA, transB;         /**< Transpose flags. */
    vm_layout_t layout;          /**< Memory layout. */
    int shared;                  /**< Non-zero when B is shared. */
} vm_gemm_batch_ctx;

/**
 * @brief Runs GEMM for batch indices `[begin, end)`.
 *
 * @param begin First batch index (inclusive).
 * @param end Last batch index (exclusive).
 * @param arg Pointer to a `vm_gemm_batch_ctx`.
 */
static void vm_gemm_batch_range(const int begin, const int end, void *arg)
{
    const vm_gemm_batch_ctx *c = (const vm_gemm_batch_ctx *)arg;
    if (c->shared) {
        vm_gemm_batch_shared_b(c->C, c->ldc, c->A, c->lda, c->B_shared, c->ldb,
                               c->M, c->N, c->K, c->alpha, c->beta,
                               c->transA, c->transB, c->layout, begin, end);
        return;
    }
    for (int p = begin; p < end; ++p) {
        const vm_float_t *Ap = c->A ? c->A[p] : NULL;
        const vm_float_t *Bp = c->B ? c->B[p] : NULL;
        vm_gemm(c->C[p], c->ldc, Ap, c->lda, Bp, c->ldb, c->M, c->N, c->K,
                c->alpha, c->beta, c->transA, c->transB, c->layout);
    }
}

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
void vm_gemm_batch(vm_float_t * const *C, const int ldc,
                   const vm_float_t * const *A, const int lda,
                   const vm_float_t * const *B, const int ldb,
                   const int M, const int N, const int K,
                   const vm_float_t alpha, const vm_float_t beta,
                   const bool transA, const bool transB,
                   const vm_layout_t layout,
                   const int batch)
{
    if (!C || batch <= 0) {
        return;
    }

    int shared_b = (B != NULL && B[0] != NULL);
    for (int p = 1; shared_b && p < batch; ++p) {
        if (B[p] != B[0]) {
            shared_b = 0;
        }
    }

    vm_gemm_batch_ctx ctx = {
        .C = C, .ldc = ldc, .A = A, .lda = lda, .B = B,
        .B_shared = (shared_b && B) ? B[0] : NULL,
        .ldb = ldb, .M = M, .N = N, .K = K,
        .alpha = alpha, .beta = beta,
        .transA = transA, .transB = transB, .layout = layout,
        .shared = (shared_b && A && alpha != VM_F(0.0) && K > 0 && M > 0 && N > 0)
    };
    vm_gemm_parallel_for(batch, vm_gemm_batch_range, &ctx,
                         vm_gemm_pick_threads(batch, M, N, K));
}

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
void vm_gemm_strided_batch(vm_float_t *C, const int ldc, const int strideC,
                           const vm_float_t *A, const int lda, const int strideA,
                           const vm_float_t *B, const int ldb, const int strideB,
                           const int M, const int N, const int K,
                           const vm_float_t alpha, const vm_float_t beta,
                           const bool transA, const bool transB,
                           const vm_layout_t layout,
                           const int batch)
{
    if (!C || batch <= 0) {
        return;
    }

    vm_float_t **Cp = (vm_float_t **)malloc((size_t)batch * sizeof(vm_float_t *));
    const vm_float_t **Ap = (const vm_float_t **)malloc((size_t)batch * sizeof(vm_float_t *));
    const vm_float_t **Bp = (const vm_float_t **)malloc((size_t)batch * sizeof(vm_float_t *));
    if (Cp && Ap && Bp) {
        for (int p = 0; p < batch; ++p) {
            Cp[p] = C + (size_t)p * (size_t)strideC;
            Ap[p] = A ? A + (size_t)p * (size_t)strideA : NULL;
            Bp[p] = B ? B + (size_t)p * (size_t)strideB : NULL;
        }
        vm_gemm_batch(Cp, ldc, Ap, lda, Bp, ldb, M, N, K,
                      alpha, beta, transA, transB, layout, batch);
        free(Cp);
        free((void *)Ap);
        free((void *)Bp);
        return;
    }
    free(Cp);
    free((void *)Ap);
    free((void *)Bp);
    for (int p = 0; p < batch; ++p) {
        const vm_float_t *Api = A ? A + (size_t)p * (size_t)strideA : NULL;
        const vm_float_t *Bpi = B ? B + (size_t)p * (size_t)strideB : NULL;
        vm_gemm(C + (size_t)p * (size_t)strideC, ldc, Api, lda, Bpi, ldb,
                M, N, K, alpha, beta, transA, transB, layout);
    }
}

/**
 * @brief NCHW im2col into a GEMM-ready panel.
 *
 * `img` is N×C×H×W packed as `((n*C+c)*H+y)*W+x`. Output has `C*kH*kW` rows
 * and `N*outH*outW` columns (zero-filled off-image taps). `outH`/`outW` use
 * standard floor division with the given pads and strides.
 *
 * @param col      Destination matrix for unfolded patches.
 * @param ld_col    Leading dimension of `col` under `layout`.
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
void vm_im2col(vm_float_t *col, const int ld_col,
               const vm_float_t *img,
               const int n, const int c, const int h, const int w,
               const int kh, const int kw,
               const int pad_h, const int pad_w,
               const int stride_h, const int stride_w,
               const vm_layout_t layout)
{
    if (!col || !img || n <= 0 || c <= 0 || h <= 0 || w <= 0 || kh <= 0 || kw <= 0) {
        return;
    }
    const int sh = stride_h > 0 ? stride_h : 1;
    const int sw = stride_w > 0 ? stride_w : 1;
    const int out_h = (h + 2 * pad_h - kh) / sh + 1;
    const int out_w = (w + 2 * pad_w - kw) / sw + 1;
    if (out_h <= 0 || out_w <= 0) {
        return;
    }

    /* img is NCHW, element (ni,ci,yi,xi) at img[((ni*c+ci)*h+yi)*w+xi]. */
    const int rows = c * kh * kw;
    const int cols = n * out_h * out_w;
    for (int col_j = 0; col_j < cols; ++col_j) {
        const int ow = col_j % out_w;
        const int tmp = col_j / out_w;
        const int oh = tmp % out_h;
        const int ni = tmp / out_h;
        for (int row = 0; row < rows; ++row) {
            const int kx = row % kw;
            const int t2 = row / kw;
            const int ky = t2 % kh;
            const int ci = t2 / kh;
            const int ih = oh * sh + ky - pad_h;
            const int iw = ow * sw + kx - pad_w;
            vm_float_t v = VM_F(0.0);
            if (ih >= 0 && ih < h && iw >= 0 && iw < w) {
                v = img[((ni * c + ci) * h + ih) * w + iw];
            }
            if (layout == VM_LAYOUT_ROW_MAJOR) {
                col[row * ld_col + col_j] = v;
            } else {
                col[row + col_j * ld_col] = v;
            }
        }
    }
}
