// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include "unitest.h"
#include <vecmat.h>

// Epsilon
#ifdef VECMAT_USE_F64
const double EPSILON = VECMAT_EPS_F64;
#else
const float EPSILON = VECMAT_EPS_F32;
#endif

// Test node
struct test_node* test_head = NULL;

// Test exception buffer
jmp_buf _except_buf;

// Benchmark node
struct benchmark_node* benchmark_head = NULL;

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    vm_cpu_init();
    printf("Vecmat %s Benchmarks (selected=%s  compiled=%s  runtime=%s  precision=%s)\n",
        VECMAT_VERSION,
        vm_cpu_name(vm_cpu_selected_features()),
        vm_cpu_name(vm_cpu_compiled_features()),
        vm_cpu_name(vm_cpu_runtime_features()),
#ifdef VECMAT_USE_F64
           "f64"
#else
           "f32"
#endif
    );

    const double total_time = run_benchmarks();
    printf("All benchmarks completed in %.9lf seconds \n", total_time);

    return EXIT_SUCCESS;
}
