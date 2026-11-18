// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <errno.h>
#include <stddef.h>
#include <stdint.h>
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
#include <malloc.h>
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

/**
 * @brief GEMM threading and small-batch thresholds.
 *
 * Caps the worker pool, stays serial when spawn cost would dominate, and
 * keeps tiny strided batches on the stack.
 */
enum {
    VM_GEMM_MAX_THREADS = 16,          /**< Hard cap on GEMM worker threads. */
    VM_GEMM_THREAD_MIN_FLOPS = 131072, /**< Stay serial below this many mul-adds; thread spawn is tens of microseconds. */
    VM_GEMM_THREAD_MIN_PER_WORKER = 2, /**< Do not split a batch thinner than this many problems per worker. */
    VM_GEMM_STRIDE_STACK = 32          /**< Use stack pointer arrays for a strided batch at or below this size. */
};

/**
 * @brief GEMM thread budget: `0` = auto, `1` = serial, `N` = cap at N.
 */
static int vm_gemm_thread_limit;

/**
 * @brief Cached `VECMAT_GEMM_THREADS` (`0` = unset / invalid).
 */
static int vm_gemm_env_threads;

/**
 * @brief Online logical CPU count for GEMM threading defaults.
 *
 * Uses `GetSystemInfo` on Windows and `sysconf(_SC_NPROCESSORS_ONLN)` elsewhere.
 * Falls back to 1 if the probe fails or returns a non-positive value.
 *
 * @return Hardware thread count estimate (>= 1).
 */
static int gemm_hw_threads(void)
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

int vm_gemm_threads(void)
{
    if (vm_gemm_thread_limit > 0) {
        return vm_gemm_thread_limit;
    }

    if (vm_gemm_env_threads == 0) {
        const char *env = getenv("VECMAT_GEMM_THREADS");
        int parsed = -1; /* -1 = absent / invalid → hardware default */
        if (env && env[0] != '\0') {
            char *end = NULL;
            errno = 0;
            const long n = strtol(env, &end, 10);
            if (errno == 0 && end != env && *end == '\0' && n > 0 && n <= VEC_INT_MAX) {
                parsed = (int)n;
            }
        }
        vm_gemm_env_threads = parsed;
    }

    if (vm_gemm_env_threads > 0) {
        return vm_gemm_env_threads;
    }

    return gemm_hw_threads();
}

void vm_gemm_set_threads(const int n)
{
    vm_gemm_thread_limit = n < 0 ? 0 : n;
}

/**
 * @brief Choose how many workers to use for a batched GEMM.
 *
 * Returns 1 for a single problem, insufficient items per worker, or
 * tiny total FLOPs (spawn cost dominates). Otherwise takes
 * `vm_gemm_threads()`, clamped to `[1, batch]` and `VM_GEMM_MAX_THREADS`.
 *
 * @param batch Number of independent GEMMs.
 * @param M     Rows of op(A)/C per problem.
 * @param N     Columns of op(B)/C per problem.
 * @param K     Inner-product length per problem.
 * @return Thread count to pass to `gemm_parallel_for`.
 */
static int gemm_pick_threads(const int batch, const int M, const int N, const int K)
{
    if (batch <= 1) {
        return 1;
    }

    const long long flops = (long long)M * (long long)N * (long long)K * (long long)batch;
    if (flops < (long long)VM_GEMM_THREAD_MIN_FLOPS) {
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

    if (t > 1 && batch < t * VM_GEMM_THREAD_MIN_PER_WORKER) {
        t = batch / VM_GEMM_THREAD_MIN_PER_WORKER;
    }

    while (t > 1 && flops / (long long)t < (long long)(VM_GEMM_THREAD_MIN_FLOPS / 4)) {
        t--;
    }

    if (t < 1) {
        t = 1;
    }

    return t;
}

/**
 * @brief One parallel-for chunk: invoke `fn(begin, end, ctx)`.
 *
 * Filled by `gemm_parallel_for` and passed to worker entry points.
 */
typedef struct {
    int begin; /**< First index (inclusive). */
    int end;   /**< Last index (exclusive). */
    void (*fn)(int begin, int end, void *ctx); /**< Callback for this subrange. */
    void *ctx; /**< Opaque pointer forwarded to `fn`. */
} vm_gemm_job;

#if defined(VECMAT_HAS_THREADS)

/**
 * @brief Persistent GEMM worker pool (create once, wait on condvars).
 *
 * Slot 0 is reserved for the caller thread. Workers 1..MAX-1 sleep until a
 * generation is posted, run their slot, then signal completion.
 */
static vm_gemm_job vm_gemm_pool_jobs[VM_GEMM_MAX_THREADS];
static int vm_gemm_pool_parts;
static int vm_gemm_pool_finished;
static unsigned vm_gemm_pool_gen;
static int vm_gemm_pool_ready;
static int vm_gemm_pool_workers;

#if defined(_WIN32) && defined(_MSC_VER)
static SRWLOCK vm_gemm_pool_lock = SRWLOCK_INIT;
static CONDITION_VARIABLE vm_gemm_pool_work = CONDITION_VARIABLE_INIT;
static CONDITION_VARIABLE vm_gemm_pool_idle = CONDITION_VARIABLE_INIT;
static HANDLE vm_gemm_pool_handles[VM_GEMM_MAX_THREADS];

/**
 * @brief Win32 pool worker: wait for a generation, run slot `idx`.
 *
 * @param arg Worker index as `INT_PTR` (1 .. MAX-1).
 * @return 0.
 */
static DWORD WINAPI gemm_pool_worker_win(LPVOID arg)
{
    const int idx = (int)(INT_PTR)arg;
    unsigned seen = 0;
    for (;;) {
        AcquireSRWLockExclusive(&vm_gemm_pool_lock);
        while (vm_gemm_pool_gen == seen) {
            SleepConditionVariableSRW(&vm_gemm_pool_work, &vm_gemm_pool_lock, INFINITE, 0);
        }

        seen = vm_gemm_pool_gen;
        const vm_gemm_job job = vm_gemm_pool_jobs[idx];
        const int active = (idx < vm_gemm_pool_parts);
        ReleaseSRWLockExclusive(&vm_gemm_pool_lock);
        if (active && job.fn) {
            job.fn(job.begin, job.end, job.ctx);
        }

        AcquireSRWLockExclusive(&vm_gemm_pool_lock);
        vm_gemm_pool_finished++;
        WakeConditionVariable(&vm_gemm_pool_idle);
        ReleaseSRWLockExclusive(&vm_gemm_pool_lock);
    }
}

/**
 * @brief Start Win32 pool workers on first use.
 */
static void gemm_pool_init(void)
{
    if (vm_gemm_pool_ready) {
        return;
    }

    AcquireSRWLockExclusive(&vm_gemm_pool_lock);
    if (!vm_gemm_pool_ready) {
        int started = 0;
        for (int i = 0; i < VM_GEMM_MAX_THREADS - 1; ++i) {
            HANDLE h = CreateThread(NULL, 0, gemm_pool_worker_win,
                                    (LPVOID)(INT_PTR)(i + 1), 0, NULL);
            if (h) {
                vm_gemm_pool_handles[started++] = h;
            }
        }
        vm_gemm_pool_workers = started;
        vm_gemm_pool_ready = 1;
    }
    ReleaseSRWLockExclusive(&vm_gemm_pool_lock);
}

#else /* pthreads */

static pthread_mutex_t vm_gemm_pool_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t vm_gemm_pool_work = PTHREAD_COND_INITIALIZER;
static pthread_cond_t vm_gemm_pool_idle = PTHREAD_COND_INITIALIZER;
static pthread_t vm_gemm_pool_ids[VM_GEMM_MAX_THREADS];

/**
 * @brief pthreads pool worker: wait for a generation, run slot `idx`.
 *
 * @param arg Worker index as `intptr_t` (1 .. MAX-1).
 * @return NULL.
 */
static void *gemm_pool_worker_pthread(void *arg)
{
    const int idx = (int)(intptr_t)arg;
    unsigned seen = 0;
    for (;;) {
        pthread_mutex_lock(&vm_gemm_pool_lock);
        while (vm_gemm_pool_gen == seen) {
            pthread_cond_wait(&vm_gemm_pool_work, &vm_gemm_pool_lock);
        }

        seen = vm_gemm_pool_gen;
        const vm_gemm_job job = vm_gemm_pool_jobs[idx];
        const int active = (idx < vm_gemm_pool_parts);
        pthread_mutex_unlock(&vm_gemm_pool_lock);

        if (active && job.fn) {
            job.fn(job.begin, job.end, job.ctx);
        }

        pthread_mutex_lock(&vm_gemm_pool_lock);
        vm_gemm_pool_finished++;
        pthread_cond_signal(&vm_gemm_pool_idle);
        pthread_mutex_unlock(&vm_gemm_pool_lock);
    }
}

/**
 * @brief Start pthreads pool workers on first use.
 */
static void gemm_pool_init(void)
{
    if (vm_gemm_pool_ready) {
        return;
    }

    pthread_mutex_lock(&vm_gemm_pool_lock);
    if (!vm_gemm_pool_ready) {
        vm_gemm_pool_workers = VM_GEMM_MAX_THREADS - 1;
        int started = 0;
        for (int i = 0; i < vm_gemm_pool_workers; ++i) {
            if (pthread_create(&vm_gemm_pool_ids[i], NULL, gemm_pool_worker_pthread,
                               (void *)(intptr_t)(i + 1)) == 0) {
                started++;
            }
        }
        vm_gemm_pool_workers = started;
        vm_gemm_pool_ready = 1;
    }

    pthread_mutex_unlock(&vm_gemm_pool_lock);
}

#endif /* pthreads vs Win32 */

/**
 * @brief Publish slots and wake sleeping pool workers.
 *
 * The caller runs slot 0 itself after this returns. Workers whose index
 * is >= `parts` still observe the generation (so they do not stay stuck
 * on a stale `seen`) and count as finished without calling `fn`.
 *
 * @param parts Number of job slots filled in `vm_gemm_pool_jobs`.
 */
static void gemm_pool_post(const int parts)
{
    gemm_pool_init();
#if defined(_WIN32) && defined(_MSC_VER)
    AcquireSRWLockExclusive(&vm_gemm_pool_lock);
    vm_gemm_pool_parts = parts;
    vm_gemm_pool_finished = 0;
    vm_gemm_pool_gen++;
    WakeAllConditionVariable(&vm_gemm_pool_work);
    ReleaseSRWLockExclusive(&vm_gemm_pool_lock);
#else
    pthread_mutex_lock(&vm_gemm_pool_lock);
    vm_gemm_pool_parts = parts;
    vm_gemm_pool_finished = 0;
    vm_gemm_pool_gen++;
    pthread_cond_broadcast(&vm_gemm_pool_work);
    pthread_mutex_unlock(&vm_gemm_pool_lock);
#endif
}

/**
 * @brief Block until every pool worker has consumed the current generation.
 */
static void gemm_pool_wait(void)
{
#if defined(_WIN32) && defined(_MSC_VER)
    AcquireSRWLockExclusive(&vm_gemm_pool_lock);
    while (vm_gemm_pool_finished < vm_gemm_pool_workers) {
        SleepConditionVariableSRW(&vm_gemm_pool_idle, &vm_gemm_pool_lock, INFINITE, 0);
    }
    ReleaseSRWLockExclusive(&vm_gemm_pool_lock);
#else
    pthread_mutex_lock(&vm_gemm_pool_lock);
    while (vm_gemm_pool_finished < vm_gemm_pool_workers) {
        pthread_cond_wait(&vm_gemm_pool_idle, &vm_gemm_pool_lock);
    }
    pthread_mutex_unlock(&vm_gemm_pool_lock);
#endif
}

#endif /* VECMAT_HAS_THREADS */

/**
 * @brief Run `fn(begin, end, ctx)` over `[0, n)` using up to `threads` workers.
 *
 * Splits the range into roughly equal chunks (capped by `n` and
 * `VM_GEMM_MAX_THREADS`). Runs serial when threading is off, `threads <= 1`,
 * or the range is tiny. Persistent pool workers handle extra chunks; the
 * caller thread always runs the first chunk.
 *
 * @param n       Iteration count (no-op if <= 0).
 * @param fn      Callback for a half-open subrange `[begin, end)`.
 * @param ctx     Opaque pointer passed through to `fn`.
 * @param threads Requested worker count (clamped; <= 1 forces serial).
 */
static void gemm_parallel_for(const int n, void (*fn)(int begin, int end, void *ctx),
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

        vm_gemm_pool_jobs[parts].begin = begin;
        vm_gemm_pool_jobs[parts].end = end;
        vm_gemm_pool_jobs[parts].fn = fn;
        vm_gemm_pool_jobs[parts].ctx = ctx;
        parts++;
    }

    if (parts <= 1) {
        fn(0, n, ctx);
        return;
    }

    gemm_pool_post(parts);
    fn(vm_gemm_pool_jobs[0].begin, vm_gemm_pool_jobs[0].end, ctx);
    gemm_pool_wait();

    for (int i = vm_gemm_pool_workers + 1; i < parts; ++i) {
        fn(vm_gemm_pool_jobs[i].begin, vm_gemm_pool_jobs[i].end, ctx);
    }
#endif
}

/**
 * @brief Return the smaller of two integers (panel edge clamp).
 *
 * @param a First value.
 * @param b Second value.
 * @return The minimum of `a` and `b`.
 */
static int gemm_min(const int a, const int b)
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
static vm_float_t panel_at(const vm_float_t *M, const int ld, const int r, const int c,
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
static vm_float_t panel_get_c(const vm_float_t *C, const int ldc, const int r, const int c,
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
static void panel_set_c(vm_float_t *C, const int ldc, const int r, const int c,
                           const vm_float_t v, const vm_layout_t layout)
{
    if (layout == VM_LAYOUT_ROW_MAJOR) {
        C[c + r * ldc] = v;
    } else {
        C[r + c * ldc] = v;
    }
}

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
                    acc += panel_at(A, lda, i, k, transA, layout)
                         * panel_at(B, ldb, k, j, transB, layout);
                }
                acc *= alpha;
            }

            if (beta == VM_F(0.0)) {
                panel_set_c(C, ldc, i, j, acc, layout);
            } else {
                panel_set_c(C, ldc, i, j,
                    acc + beta * panel_get_c(C, ldc, i, j, layout), layout);
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
 * @brief Ensure runtime CPU feature detection has run (no-op without dispatch).
 *
 * When built with `VECMAT_RUNTIME_DISPATCH`, calls `vm_cpu_init()` so the
 * selected GEMM micro-kernel is ready. Otherwise, does nothing.
 */
static void gemm_ensure_dispatch(void)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
#endif
}

/**
 * @brief Invoke the dispatched 8×8 micro-kernel.
 *
 * With `VECMAT_RUNTIME_DISPATCH`, calls `vm_gemm_ukernel_`. Otherwise, calls
 * `vm_gemm_ukernel_scalar`. A selected tile other than 8×8 does not use this
 * function; it calls that tile's own kernel.
 *
 * @param acc Accumulator tile (8×8), updated in place.
 * @param Ap  Packed A micro-panel, 8 values per k.
 * @param Bp  Packed B micro-panel, 8 values per k.
 * @param K   Inner-dimension length for this tile.
 */
static void gemm_ukernel_call(vm_float_t *acc, const vm_float_t *Ap, const vm_float_t *Bp, const int K)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_gemm_ukernel_(acc, Ap, Bp, K);
#else
    vm_gemm_ukernel_scalar(acc, Ap, Bp, K);
#endif
}

/**
 * @brief 8×8 micro-kernel through the dispatch slot.
 *
 * Calls `vm_gemm_ukernel_` when runtime dispatch is on, so a registered
 * backend's kernel is used. Otherwise, calls `vm_gemm_ukernel_scalar`.
 * This is always the 8×8 packing, not the selected tile.
 *
 * @param acc Accumulator tile (8×8), updated in place.
 * @param Ap  Packed A panel, 8 values per k.
 * @param Bp  Packed B panel, 8 values per k.
 * @param K   Inner-dimension length for this tile.
 */
static void gemm_ukernel_8x8(vm_float_t *acc, const vm_float_t *Ap, const vm_float_t *Bp, const int K)
{
    gemm_ukernel_call(acc, Ap, Bp, K);
}

/**
 * @brief Scalar 4×4 packed micro-kernel.
 *
 * Same update as `vm_gemm_ukernel_scalar`, with MR = NR = 4.
 *
 * @param acc Accumulator tile (4×4), updated in place.
 * @param Ap  Packed A panel, 4 values per k.
 * @param Bp  Packed B panel, 4 values per k.
 * @param K   Number of packed inner-dimension steps.
 */
static void gemm_ukernel_scalar_4x4(vm_float_t *acc, const vm_float_t *Ap,
                                    const vm_float_t *Bp, const int K)
{
    vm_float_t c[4 * 4];
    for (int t = 0; t < 16; ++t) {
        c[t] = acc[t];
    }

    for (int k = 0; k < K; ++k) {
        const vm_float_t *a = Ap + k * 4;
        const vm_float_t *b = Bp + k * 4;
        for (int j = 0; j < 4; ++j) {
            const vm_float_t bj = b[j];
            vm_float_t *col = c + j * 4;
            for (int i = 0; i < 4; ++i) {
                col[i] += a[i] * bj;
            }
        }
    }

    for (int t = 0; t < 16; ++t) {
        acc[t] = c[t];
    }
}

#if defined(VECMAT_ENABLE_AVX2)
/* Defined and documented in src/simd/avx2/gemm_12x8.c. */
void vm_gemm_ukernel_avx2_12x8(vm_float_t *acc, const vm_float_t *Ap,
                               const vm_float_t *Bp, int K);
#endif
#if defined(VECMAT_ENABLE_AVX512F)
/* Defined and documented in src/simd/avx512/gemm_16x14.c. */
void vm_gemm_ukernel_avx512_16x14(vm_float_t *acc, const vm_float_t *Ap,
                                  const vm_float_t *Bp, int K);
#endif

enum { VM_GEMM_TILE_SLOTS = 4 };

typedef struct {
    int mr;                     /**< Packed rows, including zero padding. */
    int nr;                     /**< Packed columns, including zero padding. */
    vm_gemm_ukernel_fn ukernel; /**< Kernel for this `mr`×`nr` packing. */
} gemm_tile;

typedef struct {
    int mr;
    int nr;
    vm_gemm_ukernel_fn fn;
} gemm_tile_slot;

static gemm_tile_slot gemm_user_tiles[VM_GEMM_TILE_SLOTS];
static int gemm_user_tile_n;
static int gemm_pin_mr;
static int gemm_pin_nr;

/**
 * @brief Find the micro-kernel registered for one tile shape.
 *
 * A user registration wins. Otherwise, the built-ins are 8×8 (the dispatch
 * slot), scalar 4×4, AVX2 12×8, and AVX-512 16×14 when that kernel was compiled.
 *
 * @param mr  Tile rows.
 * @param nr  Tile columns.
 * @param out Receives the kernel. Unchanged when the shape is unknown.
 * @return 1 when `out` was set, otherwise 0.
 */
static int gemm_tile_lookup(const int mr, const int nr, vm_gemm_ukernel_fn *out)
{
    for (int i = 0; i < gemm_user_tile_n; ++i) {
        if (gemm_user_tiles[i].mr == mr && gemm_user_tiles[i].nr == nr) {
            *out = gemm_user_tiles[i].fn;
            return 1;
        }
    }

    if (mr == VM_GEMM_MR && nr == VM_GEMM_NR) {
        *out = gemm_ukernel_8x8;
        return 1;
    }

    if (mr == 4 && nr == 4) {
        *out = gemm_ukernel_scalar_4x4;
        return 1;
    }

#if defined(VECMAT_ENABLE_AVX2)
    if (mr == 12 && nr == 8) {
        *out = vm_gemm_ukernel_avx2_12x8;
        return 1;
    }
#endif
#if defined(VECMAT_ENABLE_AVX512F)
    if (mr == 16 && nr == 14) {
        *out = vm_gemm_ukernel_avx512_16x14;
        return 1;
    }
#endif

    return 0;
}

/**
 * @brief Build a tile descriptor.
 *
 * @param mr Packed rows.
 * @param nr Packed columns.
 * @param fn Micro-kernel for that packing.
 * @return The descriptor.
 */
static gemm_tile gemm_tile_make(const int mr, const int nr, const vm_gemm_ukernel_fn fn)
{
    gemm_tile tile;
    tile.mr = mr;
    tile.nr = nr;
    tile.ukernel = fn;
    return tile;
}

/**
 * @brief Tile the next `vm_gemm` call will pack for.
 *
 * A `vm_gemm_tile_select` pin wins. Otherwise a registered backend keeps the
 * dispatched 8×8 kernel. With no backend, AVX-512 selects 16×14 and AVX2
 * selects 12×8 when that kernel was compiled. Every other case is 8×8.
 *
 * @return The active tile.
 */
static gemm_tile gemm_tile_now(void)
{
    vm_gemm_ukernel_fn fn = gemm_ukernel_8x8;

    gemm_ensure_dispatch();
    if (gemm_pin_mr != 0) {
        if (gemm_tile_lookup(gemm_pin_mr, gemm_pin_nr, &fn)) {
            return gemm_tile_make(gemm_pin_mr, gemm_pin_nr, fn);
        }
        gemm_pin_mr = 0;
        gemm_pin_nr = 0;
    }

#if defined(VECMAT_RUNTIME_DISPATCH)
    if (vm_backend_best() != NULL) {
        return gemm_tile_make(VM_GEMM_MR, VM_GEMM_NR, gemm_ukernel_8x8);
    }
#endif

    if (vm_cpu_selected_features() == VM_CPU_AVX512F && gemm_tile_lookup(16, 14, &fn)) {
        return gemm_tile_make(16, 14, fn);
    }

    if (vm_cpu_selected_features() == VM_CPU_AVX2 && gemm_tile_lookup(12, 8, &fn)) {
        return gemm_tile_make(12, 8, fn);
    }

    return gemm_tile_make(VM_GEMM_MR, VM_GEMM_NR, gemm_ukernel_8x8);
}

int vm_gemm_tile_register(const int mr, const int nr, const vm_gemm_ukernel_fn ukernel)
{
    if (!ukernel || mr < 1 || nr < 1 || mr > VM_GEMM_MR_MAX || nr > VM_GEMM_NR_MAX) {
        return -1;
    }

    if (mr == VM_GEMM_MR && nr == VM_GEMM_NR) {
        return -1;
    }

    for (int i = 0; i < gemm_user_tile_n; ++i) {
        if (gemm_user_tiles[i].mr == mr && gemm_user_tiles[i].nr == nr) {
            gemm_user_tiles[i].fn = ukernel;
            return 0;
        }
    }

    if (gemm_user_tile_n >= VM_GEMM_TILE_SLOTS) {
        return -1;
    }

    gemm_user_tiles[gemm_user_tile_n].mr = mr;
    gemm_user_tiles[gemm_user_tile_n].nr = nr;
    gemm_user_tiles[gemm_user_tile_n].fn = ukernel;
    gemm_user_tile_n++;

    return 0;
}

int vm_gemm_tile_select(const int mr, const int nr)
{
    vm_gemm_ukernel_fn fn;

    if (mr == 0 && nr == 0) {
        gemm_pin_mr = 0;
        gemm_pin_nr = 0;
        return 0;
    }

    if (!gemm_tile_lookup(mr, nr, &fn)) {
        return -1;
    }
    (void)fn;

    gemm_pin_mr = mr;
    gemm_pin_nr = nr;

    return 0;
}

void vm_gemm_tile_shape(int *mr, int *nr)
{
    const gemm_tile tile = gemm_tile_now();

    if (mr) {
        *mr = tile.mr;
    }

    if (nr) {
        *nr = tile.nr;
    }
}

/**
 * @brief Pack an A panel into contiguous micro-panels (zero-padded).
 *
 * Layout is k-major within each `tile_mr` block so the micro-kernel streams
 * linearly. A short block is padded with zeros out to `tile_mr`.
 *
 * @param Ap      Destination pack buffer.
 * @param A       Source matrix A.
 * @param lda     Leading dimension of A.
 * @param i0      Starting row of the panel in op(A).
 * @param k0      Starting K index of the panel.
 * @param ib      Number of rows to pack.
 * @param kb      Number of K steps to pack.
 * @param transA  If true, read A as transposed.
 * @param layout  Row-major or column-major storage of A.
 * @param tile_mr Rows in each packed micro-panel, including zero padding.
 */
static void gemm_pack_a(vm_float_t *Ap,
                           const vm_float_t *A, const int lda,
                           const int i0, const int k0,
                           const int ib, const int kb,
                           const bool transA, const vm_layout_t layout,
                           const int tile_mr)
{
    /* op(A)(i,k) is contiguous in i for col-major A or row-major A^T. */
    const int contig_i = (!transA && layout != VM_LAYOUT_ROW_MAJOR)
                      || (transA && layout == VM_LAYOUT_ROW_MAJOR);
    if (contig_i) {
        for (int ir = 0; ir < ib; ir += tile_mr) {
            const int mr = gemm_min(tile_mr, ib - ir);
            for (int k = 0; k < kb; ++k) {
                const vm_float_t *src = A + (i0 + ir) + (k0 + k) * lda;
                int i = 0;
                for (; i < mr; ++i) {
                    *Ap++ = src[i];
                }
                for (; i < tile_mr; ++i) {
                    *Ap++ = VM_F(0.0);
                }
            }
        }
        return;
    }

    for (int ir = 0; ir < ib; ir += tile_mr) {
        const int mr = gemm_min(tile_mr, ib - ir);
        for (int k = 0; k < kb; ++k) {
            int i = 0;
            for (; i < mr; ++i) {
                *Ap++ = panel_at(A, lda, i0 + ir + i, k0 + k, transA, layout);
            }
            for (; i < tile_mr; ++i) {
                *Ap++ = VM_F(0.0);
            }
        }
    }
}

/**
 * @brief Pack a B panel into contiguous micro-panels (zero-padded).
 *
 * Layout is k-major within each `tile_nr` block so the micro-kernel streams
 * linearly. A short block is padded with zeros out to `tile_nr`.
 *
 * @param Bp      Destination pack buffer.
 * @param B       Source matrix B.
 * @param ldb     Leading dimension of B.
 * @param k0      Starting K index of the panel.
 * @param j0      Starting column of the panel in op(B).
 * @param kb      Number of K steps to pack.
 * @param jb      Number of columns to pack.
 * @param transB  If true, read B as transposed.
 * @param layout  Row-major or column-major storage of B.
 * @param tile_nr Columns in each packed micro-panel, including zero padding.
 */
static void gemm_pack_b(vm_float_t *Bp,
                           const vm_float_t *B, const int ldb,
                           const int k0, const int j0,
                           const int kb, const int jb,
                           const bool transB, const vm_layout_t layout,
                           const int tile_nr)
{
    /* op(B)(k,j) is contiguous in j for row-major B or col-major B^T. */
    const int contig_j = (!transB && layout == VM_LAYOUT_ROW_MAJOR)
                      || (transB && layout != VM_LAYOUT_ROW_MAJOR);
    if (contig_j) {
        for (int jr = 0; jr < jb; jr += tile_nr) {
            const int nr = gemm_min(tile_nr, jb - jr);
            for (int k = 0; k < kb; ++k) {
                const vm_float_t *src = B + (j0 + jr) + (k0 + k) * ldb;
                int j = 0;
                for (; j < nr; ++j) {
                    *Bp++ = src[j];
                }
                for (; j < tile_nr; ++j) {
                    *Bp++ = VM_F(0.0);
                }
            }
        }
        return;
    }

    for (int jr = 0; jr < jb; jr += tile_nr) {
        const int nr = gemm_min(tile_nr, jb - jr);
        for (int k = 0; k < kb; ++k) {
            int j = 0;
            for (; j < nr; ++j) {
                *Bp++ = panel_at(B, ldb, k0 + k, j0 + jr + j, transB, layout);
            }
            for (; j < tile_nr; ++j) {
                *Bp++ = VM_F(0.0);
            }
        }
    }
}

/**
 * @brief Write a micro-kernel tile into C with alpha/beta and optional epilogue.
 *
 * Computes `C = alpha * acc + beta * C`. When `op != VM_GEMM_OP_NONE`, also
 * adds per-column `bias` and/or applies ReLU as requested. Callers pass
 * `VM_GEMM_OP_NONE` on non-final K panels so the epilogue runs only once.
 *
 * @param C      Output matrix.
 * @param ldc    Leading dimension of C.
 * @param acc    Accumulator from the micro-kernel, indexed `i + j*acc_mr`.
 * @param i0     Destination row origin in C.
 * @param j0     Destination column origin in C.
 * @param mr     Active rows to store (`<= acc_mr`).
 * @param nr     Active columns to store.
 * @param acc_mr Row stride of `acc`. This is the kernel tile's row count,
 *               which is larger than `mr` on a padded edge.
 * @param alpha  Scale for the accumulator.
 * @param beta   Scale for existing C (0 skips the read).
 * @param layout Row-major or column-major storage of C.
 * @param op     Epilogue flags (`VM_GEMM_OP_*`); `VM_GEMM_OP_NONE` skips bias/ReLU.
 * @param bias   Optional length-N bias vector (used with `VM_GEMM_OP_BIAS`).
 */
static void gemm_store_tile(vm_float_t *C, const int ldc,
                               const vm_float_t *acc,
                               const int i0, const int j0,
                               const int mr, const int nr,
                               const int acc_mr,
                               const vm_float_t alpha, const vm_float_t beta,
                               const vm_layout_t layout,
                               const int op, const vm_float_t *bias)
{
    const int last_k = (op != VM_GEMM_OP_NONE);
    for (int j = 0; j < nr; ++j) {
        const vm_float_t bj = (bias && (op & VM_GEMM_OP_BIAS) && last_k)
                            ? bias[j0 + j] : VM_F(0.0);

        for (int i = 0; i < mr; ++i) {
            vm_float_t v = alpha * acc[i + j * acc_mr];
            if (beta == VM_F(0.0)) {
                v += bj;
            } else {
                v += beta * panel_get_c(C, ldc, i0 + i, j0 + j, layout) + bj;
            }
            if ((op & VM_GEMM_OP_RELU) && last_k && v < VM_F(0.0)) {
                v = VM_F(0.0);
            }
            panel_set_c(C, ldc, i0 + i, j0 + j, v, layout);
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
static void gemm_scale_c(vm_float_t *C, const int ldc,
                            const int M, const int N,
                            const vm_float_t beta, const vm_layout_t layout)
{
    for (int j = 0; j < N; ++j) {
        for (int i = 0; i < M; ++i) {
            if (beta == VM_F(0.0)) {
                panel_set_c(C, ldc, i, j, VM_F(0.0), layout);
            } else {
                panel_set_c(C, ldc, i, j,
                               beta * panel_get_c(C, ldc, i, j, layout),
                               layout);
            }
        }
    }
}

/**
 * @brief Thread-local pack workspace for A/B GEMM panels.
 *
 * Holds the packed A and B buffers for one thread. Buffers grow on demand
 * and are never shrunk; capacities are in elements.
 */
typedef struct {
    vm_float_t *Ap;  /* Packed A buffer. */
    vm_float_t *Bp;  /* Packed B buffer. */
    size_t cap_a;    /* Capacity of Ap in elements. */
    size_t cap_b;    /* Capacity of Bp in elements. */
} vm_gemm_ws;

static VM_TLS vm_gemm_ws vm_gemm_tls;

/**
 * @brief Allocate a 32-byte-aligned pack buffer for at least `need` elements.
 *
 * Rounds the byte size up to a multiple of 32 (minimum 32 bytes).
 *
 * @param need Required element count.
 * @return Aligned buffer pointer, or NULL on allocation failure.
 */
static vm_float_t *gemm_alloc_pack(const size_t need)
{
    size_t bytes = need * sizeof(vm_float_t);
    if (bytes < 32u) {
        bytes = 32u;
    }
    bytes = (bytes + 31u) & ~(size_t)31u;
#if defined(_WIN32)
    return (vm_float_t *)_aligned_malloc(bytes, 32);
#else
    return (vm_float_t *)aligned_alloc(32, bytes);
#endif
}

/**
 * @brief Free a pack buffer from `gemm_alloc_pack`.
 *
 * @param p Buffer pointer returned by `gemm_alloc_pack` (or NULL).
 */
static void gemm_free_pack(vm_float_t *p)
{
#if defined(_WIN32)
    _aligned_free(p);
#else
    free(p);
#endif
}

/**
 * @brief Grow a pack buffer so it holds at least `need` elements.
 *
 * Reuses `*buf` when `*cap >= need`; otherwise allocates a new aligned
 * buffer, frees the old one, and updates `*buf` / `*cap`.
 *
 * @param buf  In/out pointer to the buffer (may be reallocated).
 * @param cap  In/out capacity in elements.
 * @param need Required element count.
 * @return Pointer to a buffer with capacity >= need, or NULL on OOM.
 */
static vm_float_t *gemm_ws_fit(vm_float_t **buf, size_t *cap, const size_t need)
{
    if (*cap >= need) {
        return *buf;
    }

    vm_float_t *n = gemm_alloc_pack(need);
    if (!n) {
        return NULL;
    }
    gemm_free_pack(*buf);

    *buf = n;
    *cap = need;

    return n;
}

/**
 * @brief Element count for an A pack covering `ib` rows and `kb` K-steps.
 *
 * @param ib      Rows in the A panel.
 * @param kb      K-panel width.
 * @param tile_mr Rows per micro-panel, including zero padding.
 * @return Number of `vm_float_t` elements required (`tile_mr`-padded).
 */
static size_t gemm_pack_a_elems(const int ib, const int kb, const int tile_mr)
{
    return ((size_t)((ib + tile_mr - 1) / tile_mr)) * (size_t)kb * (size_t)tile_mr;
}

/**
 * @brief Element count for a B pack covering `jb` columns and `kb` K-steps.
 *
 * @param jb      Columns in the B panel.
 * @param kb      K-panel width.
 * @param tile_nr Columns per micro-panel, including zero padding.
 * @return Number of `vm_float_t` elements required (`tile_nr`-padded).
 */
static size_t gemm_pack_b_elems(const int jb, const int kb, const int tile_nr)
{
    return ((size_t)((jb + tile_nr - 1) / tile_nr)) * (size_t)kb * (size_t)tile_nr;
}

/**
 * @brief Multiply A panels against a pre-packed B panel and store into C.
 *
 * Walks M in MC blocks, packs each A slab at `tile.mr`, runs `tile.ukernel`,
 * and writes tiles. `Bp` must already be packed at `tile.nr`. Epilogue ops
 * run only when `last_k` is set. On thread-local A-pack allocation failure,
 * returns without updating C (no fallback).
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
 * @param tile       Active packed tile (`mr`, `nr`, and its micro-kernel).
 * @param Ap_slabs   Packed A for this whole M, one MC slab after another.
 *                   NULL packs each slab into the thread-local buffer.
 */
static void gemm_apply_packed(vm_float_t *C, const int ldc,
                                 const vm_float_t *A, const int lda,
                                 const vm_float_t *Bp,
                                 const int M, const int jb, const int kb,
                                 const int ic0, const int jc, const int kc,
                                 const vm_float_t alpha, const vm_float_t beta_panel,
                                 const bool transA, const vm_layout_t layout,
                                 const int last_k, const int op, const vm_float_t *bias,
                                 const gemm_tile tile, const vm_float_t *Ap_slabs)
{
    const int tile_n = tile.mr * tile.nr;

    if ((!A && !Ap_slabs) || !Bp) {
        return;
    }

    vm_float_t *Ap = NULL;
    if (!Ap_slabs) {
        Ap = gemm_ws_fit(&vm_gemm_tls.Ap, &vm_gemm_tls.cap_a,
                         gemm_pack_a_elems(gemm_min(VM_GEMM_MC, M), kb, tile.mr));
        if (!Ap) {
            return;
        }
    }

    const vm_float_t *slab = Ap_slabs;
    for (int ic = 0; ic < M; ic += VM_GEMM_MC) {
        const int ib = gemm_min(VM_GEMM_MC, M - ic);
        const vm_float_t *Ap_use;
        if (Ap_slabs) {
            Ap_use = slab;
            slab += gemm_pack_a_elems(ib, kb, tile.mr);
        } else {
            gemm_pack_a(Ap, A, lda, ic0 + ic, kc, ib, kb, transA, layout, tile.mr);
            Ap_use = Ap;
        }

        for (int jr = 0; jr < jb; jr += tile.nr) {
            const int nr = gemm_min(tile.nr, jb - jr);

            const vm_float_t *Bp_tile = Bp + (jr / tile.nr) * (kb * tile.nr);
            for (int ir = 0; ir < ib; ir += tile.mr) {
                const int mr = gemm_min(tile.mr, ib - ir);
                const vm_float_t *Ap_tile = Ap_use + (ir / tile.mr) * (kb * tile.mr);

                vm_float_t acc[VM_GEMM_MR_MAX * VM_GEMM_NR_MAX];
                for (int t = 0; t < tile_n; ++t) {
                    acc[t] = VM_F(0.0);
                }

                tile.ukernel(acc, Ap_tile, Bp_tile, kb);
                gemm_store_tile(C, ldc, acc, ic0 + ic + ir, jc + jr,
                                mr, nr, tile.mr, alpha, beta_panel, layout,
                                last_k ? op : VM_GEMM_OP_NONE, bias);
            }
        }
    }
}

/**
 * @brief Core blocked GEMM with optional bias/ReLU epilogue.
 *
 * Packs B by NC×KC panels, then applies packed A×B tiles. When N spans more
 * than one NC panel and one KC slice of packed A fits in `VM_GEMM_PACK_CACHE`,
 * that slice is packed once and reused across the NC panels. The panel width
 * is the active tile from `gemm_tile_now()`. On allocation failure falls
 * back to `vm_gemm_ref` (without epilogue). When the product is empty
 * (alpha==0 / missing inputs / K==0), scales C and still applies op.
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
static void gemm_ex_body(vm_float_t *C, const int ldc,
                            const vm_float_t *A, const int lda,
                            const vm_float_t *B, const int ldb,
                            const int M, const int N, const int K,
                            const vm_float_t alpha, const vm_float_t beta,
                            const bool transA, const bool transB,
                            const vm_layout_t layout,
                            const int op, const vm_float_t *bias)
{
    gemm_ensure_dispatch();
    if (!C || M <= 0 || N <= 0) {
        return;
    }

    if (alpha == VM_F(0.0) || !A || !B || K <= 0) {
        gemm_scale_c(C, ldc, M, N, beta, layout);
        if (op != VM_GEMM_OP_NONE) {
            for (int j = 0; j < N; ++j) {
                const vm_float_t bj = (bias && (op & VM_GEMM_OP_BIAS)) ? bias[j] : VM_F(0.0);
                for (int i = 0; i < M; ++i) {
                    vm_float_t v = panel_get_c(C, ldc, i, j, layout) + bj;
                    if ((op & VM_GEMM_OP_RELU) && v < VM_F(0.0)) {
                        v = VM_F(0.0);
                    }
                    panel_set_c(C, ldc, i, j, v, layout);
                }
            }
        }
        return;
    }

    const gemm_tile tile = gemm_tile_now();
    const int tile_n = tile.mr * tile.nr;

    /* One tile: pack once per K-panel, accumulate, store once. */
    if (M <= tile.mr && N <= tile.nr) {
        vm_float_t *Ap = gemm_ws_fit(&vm_gemm_tls.Ap, &vm_gemm_tls.cap_a,
                                        gemm_pack_a_elems(M, gemm_min(VM_GEMM_KC, K), tile.mr));
        vm_float_t *Bp = gemm_ws_fit(&vm_gemm_tls.Bp, &vm_gemm_tls.cap_b,
                                        gemm_pack_b_elems(N, gemm_min(VM_GEMM_KC, K), tile.nr));

        if (Ap && Bp) {
            vm_float_t acc[VM_GEMM_MR_MAX * VM_GEMM_NR_MAX];
            for (int t = 0; t < tile_n; ++t) {
                acc[t] = VM_F(0.0);
            }

            for (int kc = 0; kc < K; kc += VM_GEMM_KC) {
                const int kb = gemm_min(VM_GEMM_KC, K - kc);
                gemm_pack_a(Ap, A, lda, 0, kc, M, kb, transA, layout, tile.mr);
                gemm_pack_b(Bp, B, ldb, kc, 0, kb, N, transB, layout, tile.nr);
                tile.ukernel(acc, Ap, Bp, kb);
            }

            gemm_store_tile(C, ldc, acc, 0, 0, M, N, tile.mr, alpha, beta, layout, op, bias);

            return;
        }
    }

    vm_float_t *Ap = gemm_ws_fit(&vm_gemm_tls.Ap, &vm_gemm_tls.cap_a,
        gemm_pack_a_elems(gemm_min(VM_GEMM_MC, M), gemm_min(VM_GEMM_KC, K), tile.mr));

    vm_float_t *Bp = gemm_ws_fit(&vm_gemm_tls.Bp, &vm_gemm_tls.cap_b,
        gemm_pack_b_elems(gemm_min(VM_GEMM_NC, N), gemm_min(VM_GEMM_KC, K), tile.nr));

    if (!Ap || !Bp) {
        vm_gemm_ref(C, ldc, A, lda, B, ldb, M, N, K, alpha, beta, transA, transB, layout);
        return;
    }

    size_t a_slice = 0;
    const int kb_span = gemm_min(VM_GEMM_KC, K);
    for (int ic = 0; ic < M; ic += VM_GEMM_MC) {
        a_slice += gemm_pack_a_elems(gemm_min(VM_GEMM_MC, M - ic), kb_span, tile.mr);
    }
    const int reuse_a = N > VM_GEMM_NC
        && a_slice * sizeof(vm_float_t) <= (size_t)VM_GEMM_PACK_CACHE;

    /* One KC slice of A is packed once and reused across NC panels when the
       slice fits in 4 MiB. Larger shapes keep the old loop. */
    if (reuse_a) {
        vm_float_t *Ap_all = gemm_ws_fit(&vm_gemm_tls.Ap, &vm_gemm_tls.cap_a, a_slice);
        if (!Ap_all) {
            vm_gemm_ref(C, ldc, A, lda, B, ldb, M, N, K, alpha, beta, transA, transB, layout);
            return;
        }
        for (int kc = 0; kc < K; kc += VM_GEMM_KC) {
            const int kb = gemm_min(VM_GEMM_KC, K - kc);
            const vm_float_t beta_panel = (kc == 0) ? beta : VM_F(1.0);
            const int last_k = (kc + kb >= K);
            vm_float_t *cursor = Ap_all;
            for (int ic = 0; ic < M; ic += VM_GEMM_MC) {
                const int ib = gemm_min(VM_GEMM_MC, M - ic);
                gemm_pack_a(cursor, A, lda, ic, kc, ib, kb, transA, layout, tile.mr);
                cursor += gemm_pack_a_elems(ib, kb, tile.mr);
            }
            for (int jc = 0; jc < N; jc += VM_GEMM_NC) {
                const int jb = gemm_min(VM_GEMM_NC, N - jc);
                gemm_pack_b(Bp, B, ldb, kc, jc, kb, jb, transB, layout, tile.nr);
                gemm_apply_packed(C, ldc, A, lda, Bp, M, jb, kb, 0, jc, kc,
                                     alpha, beta_panel, transA, layout,
                                     last_k, op, bias, tile, Ap_all);
            }
        }
    } else {
        for (int jc = 0; jc < N; jc += VM_GEMM_NC) {
            const int jb = gemm_min(VM_GEMM_NC, N - jc);
            for (int kc = 0; kc < K; kc += VM_GEMM_KC) {
                const int kb = gemm_min(VM_GEMM_KC, K - kc);
                const vm_float_t beta_panel = (kc == 0) ? beta : VM_F(1.0);
                const int last_k = (kc + kb >= K);
                gemm_pack_b(Bp, B, ldb, kc, jc, kb, jb, transB, layout, tile.nr);
                gemm_apply_packed(C, ldc, A, lda, Bp, M, jb, kb, 0, jc, kc,
                                     alpha, beta_panel, transA, layout,
                                     last_k, op, bias, tile, NULL);
            }
        }
    }
}

void vm_gemm_ex(vm_float_t *C, const int ldc,
                const vm_float_t *A, const int lda,
                const vm_float_t *B, const int ldb,
                const int M, const int N, const int K,
                const vm_float_t alpha, const vm_float_t beta,
                const bool transA, const bool transB,
                const vm_layout_t layout,
                const int op, const vm_float_t *bias)
{
    gemm_ex_body(C, ldc, A, lda, B, ldb, M, N, K,
                    alpha, beta, transA, transB, layout, op, bias);
}

void vm_gemm(vm_float_t *C, const int ldc,
             const vm_float_t *A, const int lda,
             const vm_float_t *B, const int ldb,
             const int M, const int N, const int K,
             const vm_float_t alpha, const vm_float_t beta,
             const bool transA, const bool transB,
             const vm_layout_t layout)
{
    gemm_ex_body(C, ldc, A, lda, B, ldb, M, N, K,
                    alpha, beta, transA, transB, layout,
                    VM_GEMM_OP_NONE, NULL);
}

/**
 * @brief Batched GEMM with one shared B, packing B once per KC/NC panel.
 *
 * Heap-allocates a B pack for this batch index range. Each KC×NC panel of the
 * shared B is packed once at the active tile's column count, then applied to
 * every item in `[begin, end)`. Each item still packs its own A via
 * thread-local workspace. On pack allocation failure, falls back to per-item
 * `vm_gemm`.
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
static void gemm_batch_shared_b(vm_float_t * const *C, const int ldc,
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

    const gemm_tile tile = gemm_tile_now();
    const size_t nb = gemm_pack_b_elems(gemm_min(VM_GEMM_NC, N),
                                        gemm_min(VM_GEMM_KC, K), tile.nr);

    vm_float_t *Bp = (vm_float_t *)malloc(nb * sizeof(vm_float_t));
    if (!Bp) {
        for (int p = begin; p < end; ++p) {
            vm_gemm(C[p], ldc, A ? A[p] : NULL, lda, B, ldb, M, N, K,
                    alpha, beta, transA, transB, layout);
        }
        return;
    }

    for (int jc = 0; jc < N; jc += VM_GEMM_NC) {
        const int jb = gemm_min(VM_GEMM_NC, N - jc);
        for (int kc = 0; kc < K; kc += VM_GEMM_KC) {
            const int kb = gemm_min(VM_GEMM_KC, K - kc);
            const vm_float_t beta_panel = (kc == 0) ? beta : VM_F(1.0);
            gemm_pack_b(Bp, B, ldb, kc, jc, kb, jb, transB, layout, tile.nr);
            for (int p = begin; p < end; ++p) {
                if (!C[p]) {
                    continue;
                }
                gemm_apply_packed(C[p], ldc, A ? A[p] : NULL, lda, Bp,
                                     M, jb, kb, 0, jc, kc,
                                     alpha, beta_panel, transA, layout,
                                     0, VM_GEMM_OP_NONE, NULL, tile, NULL);
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
static void gemm_batch_range(const int begin, const int end, void *arg)
{
    const vm_gemm_batch_ctx *c = arg;
    if (c->shared) {
        gemm_batch_shared_b(c->C, c->ldc, c->A, c->lda, c->B_shared, c->ldb,
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
    gemm_ensure_dispatch();

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
    gemm_parallel_for(batch, gemm_batch_range, &ctx,
                         gemm_pick_threads(batch, M, N, K));
}

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

    if (batch <= VM_GEMM_STRIDE_STACK) {
        vm_float_t *Cp_s[VM_GEMM_STRIDE_STACK];
        const vm_float_t *Ap_s[VM_GEMM_STRIDE_STACK];
        const vm_float_t *Bp_s[VM_GEMM_STRIDE_STACK];
        for (int p = 0; p < batch; ++p) {
            Cp_s[p] = C + (size_t)p * (size_t)strideC;
            Ap_s[p] = A ? A + (size_t)p * (size_t)strideA : NULL;
            Bp_s[p] = B ? B + (size_t)p * (size_t)strideB : NULL;
        }
        vm_gemm_batch(Cp_s, ldc, Ap_s, lda, Bp_s, ldb, M, N, K,
                      alpha, beta, transA, transB, layout, batch);
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
