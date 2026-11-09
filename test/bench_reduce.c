// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

/**
 * @file
 * @brief Benchmarks for the compensated reductions and extrema.
 *
 * Sizes cover a short tail, a medium buffer, and a longer stream. A volatile
 * sink takes every result so the calls stay. No speed target; the numbers
 * are for comparing builds and ISAs.
 */

#include <stdlib.h>
#include <vecmat.h>
#include "unitest.h"

static vm_float_t *alloc_fill(const int n, const int seed)
{
    vm_float_t *p = malloc((size_t)n * sizeof(vm_float_t));
    if (!p) {
        abort();
    }
    for (int i = 0; i < n; ++i) {
        p[i] = (vm_float_t)((i * 3 + seed) % 17) - VM_F(8.0);
    }
    return p;
}

static vm_float32_t *alloc_fill32(const int n, const int seed)
{
    vm_float32_t *p = malloc((size_t)n * sizeof(vm_float32_t));
    if (!p) {
        abort();
    }
    for (int i = 0; i < n; ++i) {
        p[i] = (vm_float32_t)((i * 3 + seed) % 17) - 8.0f;
    }
    return p;
}

static volatile vm_float_t sink;
static volatile vm_float64_t sink64;

#define REDUCE_BENCH(expr, n, iters)                                                   \
    do {                                                                               \
        vm_float_t *a = alloc_fill((n), 1);                                            \
        vm_float_t *b = alloc_fill((n), 2);                                            \
        for (int i = 0; i < (iters); ++i) {                                            \
            sink = (expr);                                                             \
        }                                                                              \
        free(a);                                                                       \
        free(b);                                                                       \
    } while (0)

#define REDUCE64_BENCH(expr, n, iters)                                                 \
    do {                                                                               \
        vm_float32_t *a = alloc_fill32((n), 1);                                        \
        vm_float32_t *b = alloc_fill32((n), 2);                                        \
        for (int i = 0; i < (iters); ++i) {                                            \
            sink64 = (expr);                                                           \
        }                                                                              \
        free(a);                                                                       \
        free(b);                                                                       \
    } while (0)

BENCHMARK(vm_sum_compensated_64, "iterations: 100000 [reduce,sum,n=64]")
{
    REDUCE_BENCH(vm_sum_compensated(a, 64), 64, 100000);
}

BENCHMARK(vm_sum_compensated_1024, "iterations: 10000 [reduce,sum,n=1024]")
{
    REDUCE_BENCH(vm_sum_compensated(a, 1024), 1024, 10000);
}

BENCHMARK(vm_sum_compensated_16384, "iterations: 1000 [reduce,sum,n=16384]")
{
    REDUCE_BENCH(vm_sum_compensated(a, 16384), 16384, 1000);
}

BENCHMARK(vm_dot_compensated_64, "iterations: 100000 [reduce,dot,n=64]")
{
    REDUCE_BENCH(vm_dot_compensated(a, b, 64), 64, 100000);
}

BENCHMARK(vm_dot_compensated_1024, "iterations: 10000 [reduce,dot,n=1024]")
{
    REDUCE_BENCH(vm_dot_compensated(a, b, 1024), 1024, 10000);
}

BENCHMARK(vm_dot_compensated_16384, "iterations: 1000 [reduce,dot,n=16384]")
{
    REDUCE_BENCH(vm_dot_compensated(a, b, 16384), 16384, 1000);
}

BENCHMARK(vm_sum_f32f64_1024, "iterations: 10000 [reduce,sum_f32f64,n=1024]")
{
    REDUCE64_BENCH(vm_sum_f32f64(a, 1024), 1024, 10000);
}

BENCHMARK(vm_dot_f32f64_1024, "iterations: 10000 [reduce,dot_f32f64,n=1024]")
{
    REDUCE64_BENCH(vm_dot_f32f64(a, b, 1024), 1024, 10000);
}

BENCHMARK(vm_array_minmax_1024, "iterations: 10000 [reduce,minmax,n=1024]")
{
    vm_float_t *a = alloc_fill(1024, 1);
    vm_float_t lo = VM_F(0.0);
    vm_float_t hi = VM_F(0.0);
    for (int i = 0; i < 10000; ++i) {
        (void)vm_array_minmax(&lo, &hi, a, 1024);
        sink = lo + hi;
    }
    free(a);
}

#undef REDUCE_BENCH
#undef REDUCE64_BENCH
