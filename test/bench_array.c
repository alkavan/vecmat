// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

/**
 * @file
 * @brief Benchmarks for the contiguous array kernels.
 *
 * Sizes cover a short tail, an AVX2/NEON multiple, and a longer stream.
 * A volatile sink keeps the compiler from deleting the work.
 */

#include <stdlib.h>
#include <vecmat.h>
#include "unitest.h"

static void fill(vm_float_t *p, const int n, const int seed)
{
    for (int i = 0; i < n; ++i) {
        p[i] = (vm_float_t)((i * 3 + seed) % 17) - VM_F(8.0);
    }
}

static vm_float_t *alloc_fill(const int n, const int seed)
{
    vm_float_t *p = malloc((size_t)n * sizeof(vm_float_t));
    if (!p) {
        abort();
    }
    fill(p, n, seed);
    return p;
}

static volatile vm_float_t sink;

static void consume(const vm_float_t *p, const int n)
{
    sink = p[0] + p[n - 1];
}

#define ARRAY_BENCH(op, n, iters)                                                      \
    do {                                                                               \
        vm_float_t *a = alloc_fill((n), 1);                                            \
        vm_float_t *b = alloc_fill((n), 2);                                            \
        vm_float_t *c = alloc_fill((n), 3);                                            \
        vm_float_t *r = alloc_fill((n), 4);                                            \
        for (int i = 0; i < (iters); ++i) {                                            \
            op;                                                                        \
            a[0] = r[0];                                                               \
        }                                                                              \
        consume(r, (n));                                                               \
        free(a);                                                                       \
        free(b);                                                                       \
        free(c);                                                                       \
        free(r);                                                                       \
    } while (0)

BENCHMARK(vm_array_add_64, "iterations: 100000 [array,add,n=64]")
{
    ARRAY_BENCH(vm_array_add(r, a, b, 64), 64, 100000);
}

BENCHMARK(vm_array_add_1024, "iterations: 10000 [array,add,n=1024]")
{
    ARRAY_BENCH(vm_array_add(r, a, b, 1024), 1024, 10000);
}

BENCHMARK(vm_array_add_16384, "iterations: 1000 [array,add,n=16384]")
{
    ARRAY_BENCH(vm_array_add(r, a, b, 16384), 16384, 1000);
}

BENCHMARK(vm_array_mul_1024, "iterations: 10000 [array,mul,n=1024]")
{
    ARRAY_BENCH(vm_array_mul(r, a, b, 1024), 1024, 10000);
}

BENCHMARK(vm_array_div_1024, "iterations: 10000 [array,div,n=1024]")
{
    ARRAY_BENCH(vm_array_div(r, a, b, 1024), 1024, 10000);
}

BENCHMARK(vm_array_fma_1024, "iterations: 10000 [array,fma,n=1024]")
{
    ARRAY_BENCH(vm_array_fma(r, a, b, c, 1024), 1024, 10000);
}

BENCHMARK(vm_axpy_1024, "iterations: 10000 [array,axpy,n=1024]")
{
    ARRAY_BENCH(vm_axpy(r, VM_F(1.5), a, 1024), 1024, 10000);
}

BENCHMARK(vm_rsqrt_array_1024, "iterations: 10000 [array,rsqrt,n=1024]")
{
    vm_float_t *a = alloc_fill(1024, 1);
    vm_float_t *r = alloc_fill(1024, 2);
    for (int i = 0; i < 1024; ++i) {
        a[i] = (vm_float_t)(i + 1);   /* positive, so the path is the sqrt */
    }
    for (int i = 0; i < 10000; ++i) {
        vm_rsqrt_array(r, a, 1024);
        a[0] = r[0] + VM_F(1.0);
    }
    consume(r, 1024);
    free(a);
    free(r);
}

#undef ARRAY_BENCH
