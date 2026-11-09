// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

/**
 * @file
 * @brief Benchmarks for the orthorhombic minimum image and wrap.
 *
 * Sizes cover a short tail, a medium buffer, and a longer stream of
 * `vector3`. Positions span several box lengths on both sides, so images and
 * folds take both signs. A volatile sink keeps the compiler from deleting
 * the work. No speed target.
 */

#include <stdlib.h>
#include <string.h>
#include <vecmat.h>
#include "unitest.h"

static const vector3 bench_box = {.x = VM_F(10.0), .y = VM_F(12.5), .z = VM_F(7.25)};

static vector3 *alloc_positions(const int n, const int seed)
{
    vector3 *p = malloc((size_t)n * sizeof(vector3));
    if (!p) {
        abort();
    }
    for (int i = 0; i < n; ++i) {
        p[i].x = VM_F(0.75) * (vm_float_t)((i * 7 + seed) % 61) - VM_F(22.0);
        p[i].y = VM_F(0.5) * (vm_float_t)((i * 5 + seed) % 97) - VM_F(24.0);
        p[i].z = VM_F(0.25) * (vm_float_t)((i * 3 + seed) % 89) - VM_F(11.0);
    }
    return p;
}

static volatile vm_float_t sink;

static void consume(const vector3 *p, const int n)
{
    sink = p[0].x + p[n - 1].z;
}

#define DELTA_BENCH(n, iters)                                                          \
    do {                                                                               \
        vector3 *a = alloc_positions((n), 1);                                          \
        vector3 *b = alloc_positions((n), 2);                                          \
        vector3 *r = alloc_positions((n), 3);                                          \
        for (int i = 0; i < (iters); ++i) {                                            \
            (void)vm_periodic_delta_ortho(r, a, b, bench_box, (n));                    \
            a[0].x = r[0].x;                                                           \
        }                                                                              \
        consume(r, (n));                                                               \
        free(a);                                                                       \
        free(b);                                                                       \
        free(r);                                                                       \
    } while (0)

/* Wrap folds in place, so each pass first restores the unfolded input; the
 * timing includes that one copy of `n` vectors. */
#define WRAP_BENCH(n, iters)                                                           \
    do {                                                                               \
        vector3 *src = alloc_positions((n), 4);                                        \
        vector3 *x = alloc_positions((n), 4);                                          \
        for (int i = 0; i < (iters); ++i) {                                            \
            memcpy(x, src, (size_t)(n) * sizeof(vector3));                             \
            (void)vm_wrap_ortho(x, bench_box, (n));                                    \
            src[0].x = x[0].x - VM_F(30.0);                                            \
        }                                                                              \
        consume(x, (n));                                                               \
        free(src);                                                                     \
        free(x);                                                                       \
    } while (0)

BENCHMARK(vm_periodic_delta_ortho_64, "iterations: 100000 [periodic,delta,n=64]")
{
    DELTA_BENCH(64, 100000);
}

BENCHMARK(vm_periodic_delta_ortho_1024, "iterations: 10000 [periodic,delta,n=1024]")
{
    DELTA_BENCH(1024, 10000);
}

BENCHMARK(vm_periodic_delta_ortho_16384, "iterations: 1000 [periodic,delta,n=16384]")
{
    DELTA_BENCH(16384, 1000);
}

BENCHMARK(vm_wrap_ortho_64, "iterations: 100000 [periodic,wrap,n=64]")
{
    WRAP_BENCH(64, 100000);
}

BENCHMARK(vm_wrap_ortho_1024, "iterations: 10000 [periodic,wrap,n=1024]")
{
    WRAP_BENCH(1024, 10000);
}

BENCHMARK(vm_wrap_ortho_16384, "iterations: 1000 [periodic,wrap,n=16384]")
{
    WRAP_BENCH(16384, 1000);
}

#undef DELTA_BENCH
#undef WRAP_BENCH
