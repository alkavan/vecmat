// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <math.h>
#include <stdint.h>
#include <string.h>
#include <vecmat.h>

#include "unitest.h"

static void fill_mat(vm_mat *A, const vm_float_t v)
{
    for (int c = 0; c < A->cols; ++c) {
        for (int r = 0; r < A->rows; ++r) {
            vm_mat_set(A, r, c, v);
        }
    }
}

static int all_equal(const vm_mat *A, const vm_float_t v)
{
    for (int c = 0; c < A->cols; ++c) {
        for (int r = 0; r < A->rows; ++r) {
            if (vm_mat_get(A, r, c) != v) {
                return 0;
            }
        }
    }
    return 1;
}

static int symmetric(const vm_mat *A)
{
    const int n = A->rows;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (vm_mat_get(A, i, j) != vm_mat_get(A, j, i)) {
                return 0;
            }
        }
    }
    return 1;
}

static int hermitian(const vm_mat *Ar, const vm_mat *Ai)
{
    const int n = Ar->rows;
    for (int i = 0; i < n; ++i) {
        if (vm_mat_get(Ai, i, i) != VM_F(0.0)) {
            return 0;
        }
        if (!isfinite((double)vm_mat_get(Ar, i, i))) {
            return 0;
        }
        for (int j = i + 1; j < n; ++j) {
            if (vm_mat_get(Ar, i, j) != vm_mat_get(Ar, j, i)) {
                return 0;
            }
            if (vm_mat_get(Ai, j, i) != -vm_mat_get(Ai, i, j)) {
                return 0;
            }
            if (!isfinite((double)vm_mat_get(Ar, i, j)) || !isfinite((double)vm_mat_get(Ai, i, j))) {
                return 0;
            }
        }
    }
    return 1;
}

static int bits_equal(const vm_float_t x, const uint64_t expect)
{
#if VECMAT_FLOAT_BITS == 64
    uint64_t bits = 0;
    memcpy(&bits, &x, sizeof bits);
    return bits == expect;
#else
    uint32_t bits = 0;
    memcpy(&bits, &x, sizeof bits);
    return (uint64_t)bits == expect;
#endif
}

/* Seed 0, 1×1. IEEE bits of the single draw from random.c. */
#if VECMAT_FLOAT_BITS == 64
#define GOE_11_BITS 0x3ffceea311629643ULL
#define GUE_11_BITS 0x3ff47548823b8b03ULL
#else
#define GOE_11_BITS 0x3fe77519ULL
#define GUE_11_BITS 0x3fa3aa44ULL
#endif

TEST_CASE(rng_seed_advances_once, "[random]") {
    vm_rng rng;
    vm_rng_seed(&rng, 0);
    REQUIRE(rng.state == 0x9E3779B97F4A7C15ULL);
    vm_rng_seed(&rng, 1);
    REQUIRE(rng.state == 0x9E3779B97F4A7C15ULL + 1ULL);
    vm_rng_seed(NULL, 0);
}

TEST_CASE(goe_rejects_bad_arguments, "[random]") {
    vm_rng rng;
    vm_rng_seed(&rng, 3);
    const uint64_t state = rng.state;
    vm_float_t buf[6];
    for (int i = 0; i < 6; ++i) {
        buf[i] = VM_F(7.0);
    }

    vm_mat wide = vm_mat_alloc(2, 3);
    REQUIRE(wide.data != NULL);
    fill_mat(&wide, VM_F(7.0));
    REQUIRE(!vm_goe(&wide, &rng));
    REQUIRE(all_equal(&wide, VM_F(7.0)));
    REQUIRE(rng.state == state);
    REQUIRE(!vm_goe(&wide, NULL));
    REQUIRE(all_equal(&wide, VM_F(7.0)));

    const vm_mat empty = {.rows = 0, .cols = 0, .data = buf};
    const vm_mat negative = {.rows = -2, .cols = -2, .data = buf};
    const vm_mat no_data = {.rows = 2, .cols = 2, .data = NULL};
    REQUIRE(!vm_goe(&empty, &rng));
    REQUIRE(!vm_goe(&negative, &rng));
    REQUIRE(!vm_goe(&no_data, &rng));
    REQUIRE(!vm_goe(NULL, &rng));
    REQUIRE(buf[0] == VM_F(7.0));
    REQUIRE(buf[5] == VM_F(7.0));
    REQUIRE(rng.state == state);
    vm_mat_free(&wide);
}

TEST_CASE(gue_rejects_bad_arguments, "[random]") {
    vm_rng rng;
    vm_rng_seed(&rng, 5);
    const uint64_t state = rng.state;

    vm_mat a = vm_mat_alloc(2, 2);
    vm_mat b = vm_mat_alloc(3, 3);
    vm_mat wide = vm_mat_alloc(2, 4);
    REQUIRE(a.data && b.data && wide.data);
    fill_mat(&a, VM_F(7.0));
    fill_mat(&b, VM_F(7.0));
    fill_mat(&wide, VM_F(7.0));

    REQUIRE(!vm_gue(&a, &b, &rng));
    REQUIRE(!vm_gue(&a, &wide, &rng));
    REQUIRE(!vm_gue(&wide, &a, &rng));
    REQUIRE(!vm_gue(NULL, &a, &rng));
    REQUIRE(!vm_gue(&a, NULL, &rng));
    REQUIRE(!vm_gue(&a, &b, NULL));
    const vm_mat no_data = {.rows = 2, .cols = 2, .data = NULL};
    REQUIRE(!vm_gue(&a, &no_data, &rng));
    REQUIRE(!vm_gue(&no_data, &a, &rng));

    REQUIRE(all_equal(&a, VM_F(7.0)));
    REQUIRE(all_equal(&b, VM_F(7.0)));
    REQUIRE(all_equal(&wide, VM_F(7.0)));
    REQUIRE(rng.state == state);
    vm_mat_free(&a);
    vm_mat_free(&b);
    vm_mat_free(&wide);
}

TEST_CASE(goe_1x1_and_2x2, "[random]") {
    vm_rng rng;
    vm_rng_seed(&rng, 0);
    vm_mat a = vm_mat_alloc(1, 1);
    REQUIRE(a.data != NULL);
    REQUIRE(vm_goe(&a, &rng));
    const vm_float_t a00 = vm_mat_get(&a, 0, 0);
    REQUIRE(isfinite((double)a00));
    REQUIRE(bits_equal(a00, GOE_11_BITS));

    vm_rng_seed(&rng, 4);
    vm_mat b = vm_mat_alloc(2, 2);
    REQUIRE(b.data != NULL);
    REQUIRE(vm_goe(&b, &rng));
    REQUIRE(symmetric(&b));
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            REQUIRE(isfinite((double)vm_mat_get(&b, i, j)));
        }
    }
    REQUIRE(vm_mat_get(&b, 0, 1) == vm_mat_get(&b, 1, 0));
    vm_mat_free(&a);
    vm_mat_free(&b);
}

TEST_CASE(gue_1x1_and_2x2, "[random]") {
    vm_rng rng;
    vm_rng_seed(&rng, 0);
    vm_mat ar = vm_mat_alloc(1, 1);
    vm_mat ai = vm_mat_alloc(1, 1);
    REQUIRE(ar.data && ai.data);
    vm_mat_set(&ai, 0, 0, VM_F(4.0));
    REQUIRE(vm_gue(&ar, &ai, &rng));
    const vm_float_t real = vm_mat_get(&ar, 0, 0);
    REQUIRE(isfinite((double)real));
    REQUIRE(bits_equal(real, GUE_11_BITS));
    REQUIRE(vm_mat_get(&ai, 0, 0) == VM_F(0.0));

    vm_rng_seed(&rng, 8);
    vm_mat br = vm_mat_alloc(2, 2);
    vm_mat bi = vm_mat_alloc(2, 2);
    REQUIRE(br.data && bi.data);
    REQUIRE(vm_gue(&br, &bi, &rng));
    REQUIRE(hermitian(&br, &bi));
    REQUIRE(vm_mat_get(&bi, 0, 0) == VM_F(0.0));
    REQUIRE(vm_mat_get(&bi, 1, 1) == VM_F(0.0));
    REQUIRE(vm_mat_get(&br, 0, 1) == vm_mat_get(&br, 1, 0));
    REQUIRE(vm_mat_get(&bi, 1, 0) == -vm_mat_get(&bi, 0, 1));
    vm_mat_free(&ar);
    vm_mat_free(&ai);
    vm_mat_free(&br);
    vm_mat_free(&bi);
}

static int matrices_equal(const vm_mat *A, const vm_mat *B)
{
    if (A->rows != B->rows || A->cols != B->cols) {
        return 0;
    }

    for (int c = 0; c < A->cols; ++c) {
        for (int r = 0; r < A->rows; ++r) {
            if (vm_mat_get(A, r, c) != vm_mat_get(B, r, c)) {
                return 0;
            }
        }
    }

    return 1;
}

TEST_CASE(same_seed_same_output, "[random]") {
    vm_rng a;
    vm_rng b;
    vm_rng_seed(&a, 42);
    vm_rng_seed(&b, 42);
    vm_mat g0 = vm_mat_alloc(4, 4);
    vm_mat g1 = vm_mat_alloc(4, 4);
    vm_mat r0 = vm_mat_alloc(4, 4);
    vm_mat i0 = vm_mat_alloc(4, 4);
    vm_mat r1 = vm_mat_alloc(4, 4);
    vm_mat i1 = vm_mat_alloc(4, 4);
    REQUIRE(g0.data && g1.data && r0.data && i0.data && r1.data && i1.data);
    REQUIRE(vm_goe(&g0, &a));
    REQUIRE(vm_goe(&g1, &b));
    REQUIRE(matrices_equal(&g0, &g1));
    REQUIRE(a.state == b.state);

    vm_rng_seed(&a, 0);
    vm_rng_seed(&b, 0);
    REQUIRE(vm_gue(&r0, &i0, &a));
    REQUIRE(vm_gue(&r1, &i1, &b));
    REQUIRE(matrices_equal(&r0, &r1));
    REQUIRE(matrices_equal(&i0, &i1));
    REQUIRE(a.state == b.state);
    REQUIRE(hermitian(&r0, &i0));

    vm_mat_free(&g0);
    vm_mat_free(&g1);
    vm_mat_free(&r0);
    vm_mat_free(&i0);
    vm_mat_free(&r1);
    vm_mat_free(&i1);
}

/*
 * Seed 1, order 256. Tolerance is five standard errors of the sample mean
 * and of the unbiased sample variance (divisor count - 1). Diagonal and
 * strict upper triangle are separate samples. The lower triangle is the
 * copy written with the upper entry, so it is not a second draw.
 */
static int upper_moments(const vm_mat *A, const int diagonal, double *mean, double *var)
{
    const int n = A->rows;
    double sum = 0.0;
    int count = 0;
    for (int i = 0; i < n; ++i) {
        const int j0 = diagonal ? i : i + 1;
        const int j1 = diagonal ? i + 1 : n;
        for (int j = j0; j < j1; ++j) {
            sum += (double)vm_mat_get(A, i, j);
            count += 1;
        }
    }

    const double m = sum / (double)count;
    double ss = 0.0;
    for (int i = 0; i < n; ++i) {
        const int j0 = diagonal ? i : i + 1;
        const int j1 = diagonal ? i + 1 : n;
        for (int j = j0; j < j1; ++j) {
            const double d = (double)vm_mat_get(A, i, j) - m;
            ss += d * d;
        }
    }
    *mean = m;
    *var = ss / (double)(count - 1);

    return count;
}

static void require_normal(const double mean, const double var, const double target, const int count)
{
    const double mean_tol = 5.0 * sqrt(target / (double)count);
    const double var_tol = 5.0 * target * sqrt(2.0 / (double)(count - 1));
    if (fabs(mean) > mean_tol || fabs(var - target) > var_tol) {
        FAIL("mean %.8g tol %.8g var %.8g target %.8g tol %.8g\n",
             mean, mean_tol, var, target, var_tol);
    }
}

TEST_CASE(seeded_mean_and_variance, "[random]") {
    const int n = 256;
    vm_rng rng;
    vm_rng_seed(&rng, 1);
    vm_mat goe = vm_mat_alloc(n, n);
    REQUIRE(goe.data != NULL);
    REQUIRE(vm_goe(&goe, &rng));
    REQUIRE(symmetric(&goe));
    double mean = 0.0;
    double var = 0.0;
    int count = upper_moments(&goe, 1, &mean, &var);
    REQUIRE(count == n);
    require_normal(mean, var, 2.0, count);
    count = upper_moments(&goe, 0, &mean, &var);
    REQUIRE(count == n * (n - 1) / 2);
    require_normal(mean, var, 1.0, count);

    vm_rng_seed(&rng, 1);
    vm_mat ar = vm_mat_alloc(n, n);
    vm_mat ai = vm_mat_alloc(n, n);
    REQUIRE(ar.data && ai.data);
    REQUIRE(vm_gue(&ar, &ai, &rng));
    REQUIRE(hermitian(&ar, &ai));
    count = upper_moments(&ar, 1, &mean, &var);
    REQUIRE(count == n);
    require_normal(mean, var, 1.0, count);
    count = upper_moments(&ar, 0, &mean, &var);
    REQUIRE(count == n * (n - 1) / 2);
    require_normal(mean, var, 0.5, count);
    count = upper_moments(&ai, 0, &mean, &var);
    REQUIRE(count == n * (n - 1) / 2);
    require_normal(mean, var, 0.5, count);
    for (int i = 0; i < n; ++i) {
        REQUIRE(vm_mat_get(&ai, i, i) == VM_F(0.0));
    }

    vm_mat_free(&goe);
    vm_mat_free(&ar);
    vm_mat_free(&ai);
}
