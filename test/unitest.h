// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef UNITEST_H
#define UNITEST_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#undef near
#undef far
#endif

#ifdef __APPLE__
#include <mach/mach_time.h>
#endif

#include "except.h"

#ifdef VECMAT_USE_F64
extern const double EPSILON;
#else
extern const float EPSILON;
#endif

// Printing
#ifdef UNITEST_PRINT_SUCCESS
#define PRINT_TEST_PASS(name, tags) printf("PASS: %s %s\n", tags, name)
#else
#define PRINT_TEST_PASS(name, tags) /* do nothing */
#endif
#define PRINT_TEST_FAIL(name, tags) printf("FAIL: %s %s\n", tags, name)

// Token pasting helpers
#define CONCAT_(x, y) x ## y
#define CONCAT(x, y) CONCAT_(x, y)

// Portable initializer macro
#ifdef _MSC_VER
#pragma section(".CRT$XCU", read)
#ifdef _WIN64
#define INITIALIZER(f) INITIALIZER_I(f, "")
#else
#define INITIALIZER(f) INITIALIZER_I(f, "_")
#endif
#define INITIALIZER_I(f, p) \
    static void f(void); \
    __declspec(allocate(".CRT$XCU")) void (*f##_)(void) = f; \
    __pragma(comment(linker,"/include:" p #f "_")) \
    static void f(void)
#else
#define INITIALIZER(f) \
    static void f(void) __attribute__((constructor)); \
    static void f(void)
#endif

// Forward declaration of the test node structure
struct test_node {
    const char* name;
    const char* tags;
    void (*func)(void);
    struct test_node* next;
};

// Global head of the test list (declaration)
extern struct test_node* test_head;

// Test Case
#define TEST_CASE(name, tags) \
    static void test_func_##name(void); \
    static struct test_node test_node_##name = { #name, tags, test_func_##name, NULL }; \
    INITIALIZER(register_##name) { \
        test_node_##name.next = test_head; \
        test_head = &test_node_##name; \
    } \
    static void test_func_##name(void)

// Forward declaration of the benchmark node structure
struct benchmark_node {
    const char* name;   // display name
    const char* tags;   // filterable tags
    void (*func)(void);
    struct benchmark_node* next;
};

// Global head of the benchmark list
extern struct benchmark_node* benchmark_head;

#define BENCHMARK(name, tags) \
    static void bench_func_##name(void); \
    static struct benchmark_node bench_node_##name = { #name, tags, bench_func_##name, NULL }; \
    INITIALIZER(register_bench_##name) { \
        bench_node_##name.next = benchmark_head; \
        benchmark_head = &bench_node_##name; \
    } \
    static void bench_func_##name(void)

/*******************************************************************************
 * Assertion Macros
 ******************************************************************************/

#define REQUIRE(cond) do { \
    if (!(cond)) { \
        printf("REQUIRE failed: %s at %s:%d\n", #cond, __FILE__, __LINE__); \
        THROW; \
    } \
} while (0)

#define FAIL(fmt, ...) do { \
    printf("FAIL: " fmt " at %s:%d\n", ##__VA_ARGS__, __FILE__, __LINE__); \
    THROW; \
} while (0)

/**
 * @brief Function to run all registered tests
 * @return Returns the number of failures (0 for success)
 */
static int run_tests(void) {
    int passed = 0;
    int total = 0;
    const struct test_node* current = test_head;

    // Traverse the list (order may be reversed due to registration, but often doesn't matter)
    while (current != NULL) {
        total++;

        // Set up a jump point for this test
        TRY {
            // Run the test function
            current->func();
            // If we get here, the test passed (no THROW)
            PRINT_TEST_PASS(current->name, current->tags);
            passed++;
        } CATCH {
            // THROW occurred, test failed
            PRINT_TEST_FAIL(current->name, current->tags);
        } END_TRY;

        current = current->next;
    }

    printf("All tests completed: %d/%d passed\n", passed, total);
    return total - passed;
}

/**
 * @brief Function to run all registered benchmarks
 * @return Returns the total seconds of all benchmarks summed up
 */
static inline double run_benchmarks(void) {
    double total_time = 0.0;

    // Reverse the benchmark list to run in registration order (first registered)
    struct benchmark_node* prev = NULL;
    struct benchmark_node* current = benchmark_head;
    while (current != NULL) {
        struct benchmark_node* next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    benchmark_head = prev;

    const struct benchmark_node* current_run = benchmark_head;

    // Traverse the list in corrected order
    while (current_run != NULL) {
#ifdef _WIN32
        LARGE_INTEGER freq, start, end;
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&start);
        current_run->func();
        QueryPerformanceCounter(&end);
        const double time = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
#elif defined(__APPLE__)
        static mach_timebase_info_data_t timebase_info = {0, 0};
        if (timebase_info.denom == 0) {
            mach_timebase_info(&timebase_info);
        }
        uint64_t start = mach_absolute_time();
        current_run->func();
        uint64_t end = mach_absolute_time();
        uint64_t elapsed = end - start;
        uint64_t nano = elapsed * timebase_info.numer / timebase_info.denom;
        double time = (double)nano / 1e9;
#else
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        current_run->func();
        clock_gettime(CLOCK_MONOTONIC, &end);
        const double time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
#endif
        printf("BENCHMARK %s: %.9lf seconds\n", current_run->name, time);
        total_time += time;
        current_run = current_run->next;
    }

    return total_time;
}

#endif // UNITEST_H
