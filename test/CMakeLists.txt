# SPDX-FileCopyrightText: 2025-2026 Igal Alkon
# SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.21)
project(vecmat_tests C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Tests
add_executable(vecmat_tests
        test_main.c
        test_vector_float.c
        test_vector_int.c
        test_matrix_float.c
        test_matrix_int.c
        test_quaternion.c
        test_vector_float_ptr.c
        test_vector_int_ptr.c
        test_matrix_float_ptr.c
        test_matrix_int_ptr.c
        test_quaternion_ptr.c
        test_common_usage.c
        test_cpu.c
)
target_link_libraries(vecmat_tests PRIVATE vecmat_static m)

# enable if you want to print out successful tests, only failed are printed by default
#target_compile_definitions(vecmat_tests PRIVATE UNITEST_PRINT_SUCCESS)

# Benchmarks
add_executable(vecmat_benchmarks
        bench_main.c
        bench_vector_float.c
        bench_matrix_float.c
        bench_quaternion.c
)
target_link_libraries(vecmat_benchmarks PRIVATE vecmat_static m)
