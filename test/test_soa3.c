// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <float.h>
#include <math.h>
#include <string.h>
#include <vecmat.h>
#include "unitest.h"

/* The local reference uses the library's unfused order. */
#if !defined(_MSC_VER)
#pragma STDC FP_CONTRACT OFF
#endif

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC optimize("fp-contract=off")
#endif

#ifdef VECMAT_USE_F64
#define MACH_EPS DBL_EPSILON
#else
#define MACH_EPS FLT_EPSILON
#endif

/* Longest batch in these tests; covers every AVX2/NEON remainder. */
#define SOA_N 19
/* Component stride inside one buffer, with padding after each component. */
#define SOA_PAD 24
#define SOA_SENTINEL VM_F(-777.0)

static vm_float_t ref_dot(const vm_float_t ax, const vm_float_t ay, const vm_float_t az,
                          const vm_float_t bx, const vm_float_t by, const vm_float_t bz)
{
    const vm_float_t xx = ax * bx;
    const vm_float_t yy = ay * by;
    const vm_float_t zz = az * bz;
    return (xx + yy) + zz;
}

static bool same_bits(const vm_float_t a, const vm_float_t b)
{
    return memcmp(&a, &b, sizeof a) == 0;
}

/** Fill a padded buffer: three components of SOA_PAD, sentinel past n.
 *  Values are not short dyadics, so a fused multiply-add would differ. */
static void fill_padded(vm_float_t *buf, const int n, const int seed)
{
    for (int c = 0; c < 3; ++c) {
        for (int i = 0; i < SOA_PAD; ++i) {
            buf[c * SOA_PAD + i] = i < n
                ? VM_F(3.0) / (vm_float_t)((i * 7 + c * 5 + seed) % 23 + 3) - VM_F(0.37)
                : SOA_SENTINEL;
        }
    }
}

static vm_soa3 view(vm_float_t *buf)
{
    const vm_soa3 v = { .x = buf, .y = buf + SOA_PAD, .z = buf + 2 * SOA_PAD };
    return v;
}

static vm_csoa3 cview(const vm_float_t *buf)
{
    const vm_csoa3 v = { .x = buf, .y = buf + SOA_PAD, .z = buf + 2 * SOA_PAD };
    return v;
}

static bool padding_intact(const vm_float_t *buf, const int n)
{
    for (int c = 0; c < 3; ++c) {
        for (int i = n; i < SOA_PAD; ++i) {
            if (!same_bits(buf[c * SOA_PAD + i], SOA_SENTINEL)) {
                return false;
            }
        }
    }
    return true;
}

TEST_CASE(soa3_batch_of_one_matches_vec3, "[soa3]") {
    const vector3 p = vec3(VM_F(1.5), VM_F(-2.0), VM_F(0.25));
    const vector3 q = vec3(VM_F(-0.5), VM_F(4.0), VM_F(3.0));
    const vm_float_t px = p.x, py = p.y, pz = p.z;
    const vm_float_t qx = q.x, qy = q.y, qz = q.z;
    const vm_csoa3 a = { .x = &px, .y = &py, .z = &pz };
    const vm_csoa3 b = { .x = &qx, .y = &qy, .z = &qz };

    vm_float_t r = VM_F(0.0);
    vm_soa3_dot(&r, a, b, 1);
    REQUIRE(same_bits(r, ref_dot(px, py, pz, qx, qy, qz)));
    REQUIRE(VECMAT_EQ(r, vec3_dot(p, q), EPSILON));

    vm_soa3_length_squared(&r, a, 1);
    REQUIRE(same_bits(r, ref_dot(px, py, pz, px, py, pz)));
    REQUIRE(VECMAT_EQ(r, vec3_length_squared(p), EPSILON));

    vm_soa3_distance_squared(&r, a, b, 1);
    REQUIRE(VECMAT_EQ(r, vec3_distance_squared(p, q), EPSILON));

    vm_float_t cx, cy, cz;
    const vm_soa3 c = { .x = &cx, .y = &cy, .z = &cz };
    vm_soa3_cross(c, a, b, 1);
    const vector3 e = vec3_cross(p, q);
    REQUIRE(VECMAT_EQ(cx, e.x, EPSILON));
    REQUIRE(VECMAT_EQ(cy, e.y, EPSILON));
    REQUIRE(VECMAT_EQ(cz, e.z, EPSILON));
}

TEST_CASE(soa3_padded_stride_matches_reference, "[soa3]") {
    vm_float_t a_buf[3 * SOA_PAD];
    vm_float_t b_buf[3 * SOA_PAD];
    vm_float_t c_buf[3 * SOA_PAD];
    vm_float_t r[SOA_PAD];

    for (int n = 1; n <= SOA_N; ++n) {
        fill_padded(a_buf, n, 1);
        fill_padded(b_buf, n, 4);
        fill_padded(c_buf, 0, 0);

        const vm_csoa3 a = cview(a_buf);
        const vm_csoa3 b = cview(b_buf);

        for (int i = 0; i < SOA_PAD; ++i) {
            r[i] = SOA_SENTINEL;
        }

        vm_soa3_dot(r, a, b, n);
        for (int i = 0; i < n; ++i) {
            REQUIRE(same_bits(r[i], ref_dot(a.x[i], a.y[i], a.z[i], b.x[i], b.y[i], b.z[i])));
        }
        REQUIRE(same_bits(r[n], SOA_SENTINEL));

        vm_soa3_length_squared(r, a, n);
        for (int i = 0; i < n; ++i) {
            REQUIRE(same_bits(r[i], ref_dot(a.x[i], a.y[i], a.z[i], a.x[i], a.y[i], a.z[i])));
        }
        REQUIRE(same_bits(r[n], SOA_SENTINEL));

        vm_soa3_distance_squared(r, a, b, n);
        for (int i = 0; i < n; ++i) {
            const vm_float_t dx = a.x[i] - b.x[i];
            const vm_float_t dy = a.y[i] - b.y[i];
            const vm_float_t dz = a.z[i] - b.z[i];
            REQUIRE(same_bits(r[i], ref_dot(dx, dy, dz, dx, dy, dz)));
        }
        REQUIRE(same_bits(r[n], SOA_SENTINEL));

        vm_soa3_cross(view(c_buf), a, b, n);
        const vm_csoa3 c = cview(c_buf);
        for (int i = 0; i < n; ++i) {
            REQUIRE(same_bits(c.x[i], a.y[i] * b.z[i] - a.z[i] * b.y[i]));
            REQUIRE(same_bits(c.y[i], a.z[i] * b.x[i] - a.x[i] * b.z[i]));
            REQUIRE(same_bits(c.z[i], a.x[i] * b.y[i] - a.y[i] * b.x[i]));
        }

        REQUIRE(padding_intact(c_buf, n));
        REQUIRE(padding_intact(a_buf, n));
        REQUIRE(padding_intact(b_buf, n));
    }
}

TEST_CASE(soa3_cross_exact_alias, "[soa3]") {
    for (int n = 1; n <= SOA_N; ++n) {
        vm_float_t abuf[3 * SOA_PAD];
        vm_float_t bbuf[3 * SOA_PAD];
        vm_float_t expect[3 * SOA_PAD];

        fill_padded(abuf, n, 2);
        fill_padded(bbuf, n, 9);
        fill_padded(expect, 0, 0);

        vm_soa3_cross(view(expect), cview(abuf), cview(bbuf), n);

        /* res == a */
        vm_float_t work[3 * SOA_PAD];
        memcpy(work, abuf, sizeof work);
        vm_soa3_cross(view(work), cview(work), cview(bbuf), n);
        for (int c = 0; c < 3; ++c) {
            for (int i = 0; i < n; ++i) {
                REQUIRE(same_bits(work[c * SOA_PAD + i], expect[c * SOA_PAD + i]));
            }
        }
        REQUIRE(padding_intact(work, n));

        /* res == b */
        memcpy(work, bbuf, sizeof work);
        vm_soa3_cross(view(work), cview(abuf), cview(work), n);
        for (int c = 0; c < 3; ++c) {
            for (int i = 0; i < n; ++i) {
                REQUIRE(same_bits(work[c * SOA_PAD + i], expect[c * SOA_PAD + i]));
            }
        }

        /* res == a == b gives exact zeros */
        memcpy(work, abuf, sizeof work);
        vm_soa3_cross(view(work), cview(work), cview(work), n);
        for (int c = 0; c < 3; ++c) {
            for (int i = 0; i < n; ++i) {
                REQUIRE(work[c * SOA_PAD + i] == VM_F(0.0));
            }
        }
    }
}

TEST_CASE(soa3_axes_and_identities, "[soa3]") {
    const vm_float_t ex[3] = { VM_F(1.0), VM_F(0.0), VM_F(0.0) };
    const vm_float_t ey[3] = { VM_F(0.0), VM_F(1.0), VM_F(0.0) };
    const vm_float_t ez[3] = { VM_F(0.0), VM_F(0.0), VM_F(1.0) };
    /* a = (x̂, ŷ, ẑ), b = (ŷ, ẑ, x̂) */
    const vm_csoa3 a = { .x = ex, .y = ey, .z = ez };
    const vm_csoa3 b = { .x = ez, .y = ex, .z = ey };
    vm_float_t cx[3], cy[3], cz[3];
    vm_soa3_cross((vm_soa3){ .x = cx, .y = cy, .z = cz }, a, b, 3);
    /* x̂ × ŷ = ẑ, ŷ × ẑ = x̂, ẑ × x̂ = ŷ */
    REQUIRE(cx[0] == VM_F(0.0) && cy[0] == VM_F(0.0) && cz[0] == VM_F(1.0));
    REQUIRE(cx[1] == VM_F(1.0) && cy[1] == VM_F(0.0) && cz[1] == VM_F(0.0));
    REQUIRE(cx[2] == VM_F(0.0) && cy[2] == VM_F(1.0) && cz[2] == VM_F(0.0));

    /* anti-commutativity and dot symmetry on a longer batch */
    vm_float_t a_buf[3 * SOA_PAD];
    vm_float_t b_buf[3 * SOA_PAD];

    vm_float_t ab[3 * SOA_PAD];
    vm_float_t ba[3 * SOA_PAD];

    fill_padded(a_buf, SOA_N, 3);
    fill_padded(b_buf, SOA_N, 11);

    vm_soa3_cross(view(ab), cview(a_buf), cview(b_buf), SOA_N);
    vm_soa3_cross(view(ba), cview(b_buf), cview(a_buf), SOA_N);

    for (int c = 0; c < 3; ++c) {
        for (int i = 0; i < SOA_N; ++i) {
            REQUIRE(ab[c * SOA_PAD + i] == -ba[c * SOA_PAD + i]);
        }
    }

    vm_float_t d1[SOA_N];
    vm_float_t d2[SOA_N];
    vm_soa3_dot(d1, cview(a_buf), cview(b_buf), SOA_N);
    vm_soa3_dot(d2, cview(b_buf), cview(a_buf), SOA_N);
    for (int i = 0; i < SOA_N; ++i) {
        REQUIRE(same_bits(d1[i], d2[i]));
    }

    /* a · (a × b) is zero within the absolute bound */
    vm_soa3_dot(d1, cview(a_buf), cview(ab), SOA_N);
    for (int i = 0; i < SOA_N; ++i) {
        const double ax = a_buf[i], ay = a_buf[SOA_PAD + i], az = a_buf[2 * SOA_PAD + i];
        const double terms = fabs(ax * ab[i]) + fabs(ay * ab[SOA_PAD + i])
                           + fabs(az * ab[2 * SOA_PAD + i]);
        const double bx = b_buf[i], by = b_buf[SOA_PAD + i], bz = b_buf[2 * SOA_PAD + i];
        const double cross_terms = fabs(ay * bz) + fabs(az * by) + fabs(az * bx)
                                 + fabs(ax * bz) + fabs(ax * by) + fabs(ay * bx);
        const double scale = terms + (fabs(ax) + fabs(ay) + fabs(az)) * cross_terms;
        REQUIRE(fabs((double)d1[i]) <= 16.0 * (double)MACH_EPS * scale);
    }
}

TEST_CASE(soa3_absolute_error_bound, "[soa3]") {
    /* scaled coordinates: large and small magnitudes in one batch */
    vm_float_t a_buf[3 * SOA_PAD];
    vm_float_t b_buf[3 * SOA_PAD];

    fill_padded(a_buf, SOA_N, 5);
    fill_padded(b_buf, SOA_N, 6);

    for (int i = 0; i < SOA_N; ++i) {
        const vm_float_t s = (i % 3 == 0) ? VM_F(1.0e6) : (i % 3 == 1) ? VM_F(1.0e-6) : VM_F(1.0);
        for (int c = 0; c < 3; ++c) {
            a_buf[c * SOA_PAD + i] *= s;
            b_buf[c * SOA_PAD + i] *= s;
        }
    }

    const vm_csoa3 a = cview(a_buf);
    const vm_csoa3 b = cview(b_buf);
    vm_float_t r[SOA_N];

    vm_soa3_dot(r, a, b, SOA_N);
    for (int i = 0; i < SOA_N; ++i) {
        const long double xx = (long double)a.x[i] * b.x[i];
        const long double yy = (long double)a.y[i] * b.y[i];
        const long double zz = (long double)a.z[i] * b.z[i];
        const long double terms = fabsl(xx) + fabsl(yy) + fabsl(zz);
        REQUIRE(fabsl((long double)r[i] - (xx + yy + zz)) <= 16.0L * MACH_EPS * terms);
    }

    vm_soa3_distance_squared(r, a, b, SOA_N);
    for (int i = 0; i < SOA_N; ++i) {
        const long double dx = (long double)a.x[i] - b.x[i];
        const long double dy = (long double)a.y[i] - b.y[i];
        const long double dz = (long double)a.z[i] - b.z[i];
        const long double exact = dx * dx + dy * dy + dz * dz;
        REQUIRE(fabsl((long double)r[i] - exact) <= 16.0L * MACH_EPS * exact);
    }

    vm_soa3_length_squared(r, a, SOA_N);
    for (int i = 0; i < SOA_N; ++i) {
        const long double exact = (long double)a.x[i] * a.x[i]
                                + (long double)a.y[i] * a.y[i]
                                + (long double)a.z[i] * a.z[i];
        REQUIRE(fabsl((long double)r[i] - exact) <= 16.0L * MACH_EPS * exact);
    }
}

TEST_CASE(soa3_empty_is_noop, "[soa3]") {
    const vm_csoa3 none = {.x = NULL, .y = NULL, .z = NULL};
    const vm_soa3 out = {.x = NULL, .y = NULL, .z = NULL};
    vm_soa3_dot(NULL, none, none, 0);
    vm_soa3_cross(out, none, none, 0);
    vm_soa3_length_squared(NULL, none, -1);
    vm_soa3_distance_squared(NULL, none, none, -5);

    const vm_float_t one = VM_F(1.0);
    const vm_csoa3 a = { .x = &one, .y = &one, .z = &one };
    vm_float_t r = SOA_SENTINEL;
    vm_soa3_dot(&r, a, a, 0);
    vm_soa3_length_squared(&r, a, -1);
    REQUIRE(same_bits(r, SOA_SENTINEL));
}

TEST_CASE(soa3_suffix_matches_unsuffixed, "[soa3]") {
    const vm_float_t x = VM_F(2.0), y = VM_F(3.0), z = VM_F(6.0);
    const vm_csoa3 a = { .x = &x, .y = &y, .z = &z };
    vm_float_t r, s;
    vm_soa3_length_squared(&r, a, 1);
#if VECMAT_FLOAT_BITS == 64
    vm_soa3_length_squared64(&s, a, 1);
#else
    vm_soa3_length_squared32(&s, a, 1);
#endif
    REQUIRE(same_bits(r, s));
    REQUIRE(r == VM_F(49.0));
}
