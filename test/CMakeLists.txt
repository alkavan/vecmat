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
        ${CMAKE_BINARY_DIR}/vecmat_module_register.c
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
        test_extended.c
        test_physics.c
        test_abi.c
        ${VECMAT_MODULE_TEST_SOURCES}
)
if(VECMAT_ENABLE_FP16)
    target_sources(vecmat_tests PRIVATE test_fp16.c)
endif()
if(VECMAT_USE_F64)
    target_compile_definitions(vecmat_tests PRIVATE VECMAT_USE_F64)
endif()

if(MSVC)
    target_link_libraries(vecmat_tests PRIVATE vecmat_static)
else()
    target_link_libraries(vecmat_tests PRIVATE vecmat_static m)
endif()

find_package(Threads)
if(Threads_FOUND)
    target_link_libraries(vecmat_tests PRIVATE Threads::Threads)
    target_compile_definitions(vecmat_tests PRIVATE VECMAT_TEST_THREADS)
endif()

# Exit test: an f32 TU and an f64 TU link the same BOTH library and run.
if(VECMAT_BUILD_ABI_32 AND VECMAT_BUILD_ABI_64)
    add_executable(vecmat_abi_both
            abi_tu32.c
            abi_tu64.c
            abi_both_main.c)
    # abi_tu64.c defines VECMAT_USE_F64 itself so it does not inherit the test TU.
    if(MSVC)
        target_link_libraries(vecmat_abi_both PRIVATE vecmat_static)
    else()
        target_link_libraries(vecmat_abi_both PRIVATE vecmat_static m)
    endif()
endif()

# Exit test: an f32 TU against a 64-only vm_compiled_float_bits trips.
add_executable(vecmat_abi_mismatch abi_mismatch.c
        ${CMAKE_SOURCE_DIR}/src/features/cpu.c)
target_include_directories(vecmat_abi_mismatch PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src)
target_compile_definitions(vecmat_abi_mismatch PRIVATE VECMAT_HAVE_ABI_64)
if(NOT MSVC)
    target_link_libraries(vecmat_abi_mismatch PRIVATE m)
endif()

# enable if you want to print out successful tests, only failed are printed by default
#target_compile_definitions(vecmat_tests PRIVATE UNITEST_PRINT_SUCCESS)

# Benchmarks
add_executable(vecmat_benchmarks
        bench_main.c
        ${CMAKE_BINARY_DIR}/vecmat_module_register.c
        bench_vector_float.c
        bench_matrix_float.c
        bench_quaternion.c
        bench_gemm.c
        ${VECMAT_MODULE_BENCH_SOURCES}
)
if(VECMAT_USE_F64)
    target_compile_definitions(vecmat_benchmarks PRIVATE VECMAT_USE_F64)
endif()

if(MSVC)
    target_link_libraries(vecmat_benchmarks PRIVATE vecmat_static)
else()
    target_link_libraries(vecmat_benchmarks PRIVATE vecmat_static m)
endif()
