// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include "unitest.h"
#include "module_register.h"
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
    printf("Vecmat %s — A simple math and linear algebra library.\n", VECMAT_VERSION);
    vm_backend_modules_register();
    vm_cpu_init();
    if (vm_cpu_note())
        printf("%s\n", vm_cpu_note());
    return run_tests();
}
