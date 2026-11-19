// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

/* Built only with -DVECMAT_EXPERIMENTAL=ON, which defines the macro. */
#include <vecmat.h>
#include <string.h>
#include "unitest.h"

#if !defined(_MSC_VER)
#pragma STDC FP_CONTRACT OFF
#endif

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC optimize("fp-contract=off")
#endif

static bool same_bits(const vm_float_t a, const vm_float_t b)
{
    return memcmp(&a, &b, sizeof a) == 0;
}

static bool quat_bits(const quaternion a, const quaternion b)
{
    return same_bits(a.x, b.x) && same_bits(a.y, b.y)
        && same_bits(a.z, b.z) && same_bits(a.w, b.w);
}

/**
 * @brief The printed `(w, x, y, z)` matrix times `b`, unfused left to right.
 */
static quaternion post_mul(const quaternion a, const quaternion b)
{
    const vm_float_t w = a.w;
    const vm_float_t x = a.x;
    const vm_float_t y = a.y;
    const vm_float_t z = a.z;

    const vm_float_t bw = b.w;
    const vm_float_t bx = b.x;
    const vm_float_t by = b.y;
    const vm_float_t bz = b.z;

    const vm_float_t rw = ((w * bw - x * bx) - y * by) - z * bz;
    const vm_float_t rx = ((x * bw + w * bx) - z * by) + y * bz;
    const vm_float_t ry = ((y * bw + z * bx) + w * by) - x * bz;
    const vm_float_t rz = ((z * bw - y * bx) + x * by) + w * bz;

    const quaternion r = {.x = rx, .y = ry, .z = rz, .w = rw};
    return r;
}

static quaternion mul_mat(const matrix4 *m, const quaternion q)
{
    const vector4 v_in = {.x = q.x, .y = q.y, .z = q.z, .w = q.w};
    vector4 v_out;
    mat4_mul_vec4_ptr(&v_out, m, &v_in);
    const quaternion r = {.x = v_out.x, .y = v_out.y, .z = v_out.z, .w = v_out.w};
    return r;
}

TEST_CASE(hamilton_mat_is_the_printed_matrix, "[experimental]") {
    const quaternion q = {
        .x = VM_F(0.2), .y = VM_F(-0.3), .z = VM_F(0.4), .w = VM_F(-0.5)
    };

    const vm_float_t w = q.w;
    const vm_float_t x = q.x;
    const vm_float_t y = q.y;
    const vm_float_t z = q.z;
    matrix4 m;
    m.m11 = VM_F(42.0);
    vm_x_quat_hamilton_mat(&m, &q);

    /* Columns are (x, y, z, w), each stored top to bottom. */
    REQUIRE(m.m11 == w && m.m21 == z && m.m31 == -y && m.m41 == -x);
    REQUIRE(m.m12 == -z && m.m22 == w && m.m32 == x && m.m42 == -y);
    REQUIRE(m.m13 == y && m.m23 == -x && m.m33 == w && m.m43 == -z);
    REQUIRE(m.m14 == x && m.m24 == y && m.m34 == z && m.m44 == w);

    /* Rebuild the printed row-major (w, x, y, z) matrix and compare. */
    const vm_float_t post[4][4] = {
        {w, -x, -y, -z},
        {x,  w, -z,  y},
        {y,  z,  w, -x},
        {z, -y,  x,  w}
    };

    /* wxyz index -> xyzw index: w x y z -> 3 0 1 2. */
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            const int to_xyzw[4] = {3, 0, 1, 2};
            const int rr = to_xyzw[r];
            const int cc = to_xyzw[c];
            REQUIRE(m.v[cc * 4 + rr] == post[r][c]);
        }
    }

    const quaternion p = {.x = VM_F(0.1), .y = VM_F(0.2), .z = VM_F(-0.4), .w = VM_F(0.7)};
    const quaternion via_post = post_mul(q, p);
    const quaternion via_quat = quat_mul(q, p);
    REQUIRE(quat_near(via_post, via_quat, EPSILON));
    REQUIRE(quat_near(mul_mat(&m, p), via_quat, EPSILON));

    const quaternion id = quat_identity();
    matrix4 I;
    vm_x_quat_hamilton_mat(&I, &id);
    for (int i = 0; i < 16; ++i) {
        const vm_float_t expect = (i % 5 == 0) ? VM_F(1.0) : VM_F(0.0);
        REQUIRE(I.v[i] == expect);
    }
}

TEST_CASE(hamilton_mat_columns_match_quat_mul, "[experimental]") {
    const quaternion q = {.x = VM_F(0.2), .y = VM_F(-0.5), .z = VM_F(0.1), .w = VM_F(0.8)};
    const quaternion basis[4] = {
        {.x = VM_F(1.0), .y = VM_F(0.0), .z = VM_F(0.0), .w = VM_F(0.0)},
        {.x = VM_F(0.0), .y = VM_F(1.0), .z = VM_F(0.0), .w = VM_F(0.0)},
        {.x = VM_F(0.0), .y = VM_F(0.0), .z = VM_F(1.0), .w = VM_F(0.0)},
        {.x = VM_F(0.0), .y = VM_F(0.0), .z = VM_F(0.0), .w = VM_F(1.0)}
    };
    matrix4 m;
    vm_x_quat_hamilton_mat(&m, &q);
    for (int i = 0; i < 4; ++i) {
        REQUIRE(quat_near(mul_mat(&m, basis[i]), quat_mul(q, basis[i]), EPSILON));
    }

    const quaternion i = basis[0];
    const quaternion j = basis[1];
    const quaternion k = {.x = VM_F(0.0), .y = VM_F(0.0), .z = VM_F(1.0), .w = VM_F(0.0)};
    REQUIRE(quat_eq(quat_mul(i, j), k));
    REQUIRE(quat_near(mul_mat(&m, j), quat_mul(q, j), EPSILON));
}

TEST_CASE(hamilton_array_matches_quat_mul, "[experimental]") {
    const quaternion q = {.x = VM_F(0.0), .y = VM_F(0.70710678), .z = VM_F(0.0), .w = VM_F(0.70710678)};
    const quaternion p = {.x = VM_F(0.1), .y = VM_F(0.2), .z = VM_F(0.3), .w = VM_F(0.9)};
    quaternion one;
    vm_x_quat_hamilton_mul_array(&one, &q, &p, 1);

    matrix4 H;
    vm_x_quat_hamilton_mat(&H, &q);
    REQUIRE(quat_bits(one, mul_mat(&H, p)));
    REQUIRE(quat_near(one, quat_mul(q, p), EPSILON));
    REQUIRE(quat_near(one, post_mul(q, p), EPSILON));

    quaternion ins[8];
    quaternion saved[8];
    for (int i = 0; i < 8; ++i) {
        ins[i] = (quaternion){
            .x = VM_F(0.1) * (vm_float_t)i,
            .y = VM_F(0.2) * (vm_float_t)i,
            .z = VM_F(0.05) * (vm_float_t)i,
            .w = VM_F(1.0) - VM_F(0.01) * (vm_float_t)i
        };
        saved[i] = ins[i];
    }
    vm_x_quat_hamilton_mul_array(ins, &q, ins, 8);
    for (int i = 0; i < 8; ++i) {
        REQUIRE(quat_near(ins[i], quat_mul(q, saved[i]), EPSILON));
    }

    const quaternion ii = {.x = VM_F(1.0), .y = VM_F(0.0), .z = VM_F(0.0), .w = VM_F(0.0)};
    const quaternion jj = {.x = VM_F(0.0), .y = VM_F(1.0), .z = VM_F(0.0), .w = VM_F(0.0)};
    const quaternion kk = {.x = VM_F(0.0), .y = VM_F(0.0), .z = VM_F(1.0), .w = VM_F(0.0)};
    quaternion ij;
    vm_x_quat_hamilton_mul_array(&ij, &ii, &jj, 1);
    REQUIRE(quat_eq(ij, kk));
    REQUIRE(quat_eq(quat_mul(ii, jj), kk));
}

TEST_CASE(hamilton_right_and_chain_match_quat_mul, "[experimental]") {
    const quaternion q = {.x = VM_F(0.0), .y = VM_F(0.70710678), .z = VM_F(0.0), .w = VM_F(0.70710678)};
    const quaternion p = {.x = VM_F(0.1), .y = VM_F(0.2), .z = VM_F(0.3), .w = VM_F(0.9)};
    quaternion left;
    quaternion right;
    vm_x_quat_hamilton_mul_array(&left, &q, &p, 1);
    vm_x_quat_hamilton_mul_array_right(&right, &q, &p, 1);
    REQUIRE(quat_near(right, quat_mul(p, q), EPSILON));
    REQUIRE(!quat_near(left, right, EPSILON));

    quaternion ins[8];
    quaternion saved[8];
    for (int i = 0; i < 8; ++i) {
        ins[i] = (quaternion){
            .x = VM_F(-0.2) * (vm_float_t)i,
            .y = VM_F(0.15) * (vm_float_t)i,
            .z = VM_F(0.07) * (vm_float_t)i,
            .w = VM_F(0.4) + VM_F(0.02) * (vm_float_t)i
        };
        saved[i] = ins[i];
    }
    vm_x_quat_hamilton_mul_array_right(ins, &q, ins, 8);
    for (int i = 0; i < 8; ++i) {
        REQUIRE(quat_near(ins[i], quat_mul(saved[i], q), EPSILON));
    }

    const quaternion chain[3] = {q, p, q};
    quaternion acc = quat_identity();
    vm_x_quat_hamilton_mul_chain(&acc, chain, 3);
    quaternion ref = quat_identity();
    ref = quat_mul(ref, q);
    ref = quat_mul(ref, p);
    ref = quat_mul(ref, q);
    REQUIRE(quat_near(acc, ref, EPSILON));
}

TEST_CASE(hamilton_edges_do_not_write, "[experimental]") {
    const quaternion q = {.x = VM_F(1.0), .y = VM_F(2.0), .z = VM_F(3.0), .w = VM_F(4.0)};
    const quaternion p = {.x = VM_F(5.0), .y = VM_F(6.0), .z = VM_F(7.0), .w = VM_F(8.0)};
    quaternion out = q;
    vm_x_quat_hamilton_mul_array(&out, &q, &p, 0);
    vm_x_quat_hamilton_mul_array(&out, &q, &p, -2);
    vm_x_quat_hamilton_mul_array_right(&out, &q, &p, 0);
    REQUIRE(quat_eq(out, q));

    vm_x_quat_hamilton_mul_array(NULL, NULL, NULL, 0);
    vm_x_quat_hamilton_mul_array(NULL, NULL, NULL, -1);
    vm_x_quat_hamilton_mul_array(NULL, &q, &p, 1);
    vm_x_quat_hamilton_mul_array_right(NULL, &q, &p, 1);
    vm_x_quat_hamilton_mul_chain(NULL, NULL, 0);
    vm_x_quat_hamilton_mul_chain(NULL, &q, 1);

    quaternion acc = q;
    vm_x_quat_hamilton_mul_chain(&acc, &p, 0);
    vm_x_quat_hamilton_mul_chain(&acc, NULL, 3);
    REQUIRE(quat_eq(acc, q));

    matrix4 m;
    m.m11 = VM_F(9.0);
    vm_x_quat_hamilton_mat(NULL, &q);
    vm_x_quat_hamilton_mat(&m, NULL);
    REQUIRE(m.m11 == VM_F(9.0));
}
