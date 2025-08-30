// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "unitest.h"

// matrix2i ptr tests
TEST_CASE(mat2i_identity_ptr_test, "[matrix2i_ptr]")
{
    matrix2i m;
    mat2i_identity_ptr(&m);
    REQUIRE(m.v[0] == 1);
    REQUIRE(m.v[1] == 0);
    REQUIRE(m.v[2] == 0);
    REQUIRE(m.v[3] == 1);
}

TEST_CASE(mat2i_mul_ptr_test, "[matrix2i_ptr]")
{
    const matrix2i a = {.m11 = 1, .m21 = 2, .m12 = 3, .m22 = 4};
    const matrix2i b = {.m11 = 5, .m21 = 6, .m12 = 7, .m22 = 8};
    matrix2i res;
    mat2i_mul_ptr(&res, &a, &b);
    REQUIRE(res.m11 == 23);
    REQUIRE(res.m21 == 34);
    REQUIRE(res.m12 == 31);
    REQUIRE(res.m22 == 46);
}

TEST_CASE(mat2i_transpose_ptr_test, "[matrix2i_ptr]")
{
    const matrix2i m = {.m11 = 1, .m21 = 2, .m12 = 3, .m22 = 4};
    matrix2i res;
    mat2i_transpose_ptr(&res, &m);
    REQUIRE(res.v[0] == 1);
    REQUIRE(res.v[1] == 3);
    REQUIRE(res.v[2] == 2);
    REQUIRE(res.v[3] == 4);
}

TEST_CASE(mat2i_inverse_ptr_test, "[matrix2i_ptr]")
{
    const matrix2i m = {.m11 = 1, .m21 = 1, .m12 = 0, .m22 = 1};  // Determinant = 1
    matrix2i res;
    mat2i_inverse_ptr(&res, &m);
    REQUIRE(res.v[0] == 1);
    REQUIRE(res.v[1] == -1);
    REQUIRE(res.v[2] == 0);
    REQUIRE(res.v[3] == 1);
}

TEST_CASE(mat2i_inverse_ptr_singular_test, "[matrix2i_ptr]")
{
    const matrix2i m = {.m11 = 1, .m21 = 1, .m12 = 1, .m22 = 1};  // Singular matrix (determinant = 0)
    matrix2i res;
    mat2i_inverse_ptr(&res, &m);
    // Should return identity
    REQUIRE(res.v[0] == 1);
    REQUIRE(res.v[1] == 0);
    REQUIRE(res.v[2] == 0);
    REQUIRE(res.v[3] == 1);
}

// Tests for matrix3i _ptr functions
TEST_CASE(mat3i_identity_ptr_test, "[matrix3i_ptr]")
{
    matrix3i m;
    mat3i_identity_ptr(&m);
    REQUIRE(m.m11 == 1); REQUIRE(m.m12 == 0); REQUIRE(m.m13 == 0);
    REQUIRE(m.m21 == 0); REQUIRE(m.m22 == 1); REQUIRE(m.m23 == 0);
    REQUIRE(m.m31 == 0); REQUIRE(m.m32 == 0); REQUIRE(m.m33 == 1);
}

TEST_CASE(mat3i_mul_ptr_test, "[matrix3i_ptr]")
{
    const matrix3i a = {
        .m11 = 1, .m12 = 2, .m13 = 0,
        .m21 = 0, .m22 = 1, .m23 = 2,
        .m31 = 1, .m32 = 0, .m33 = 1
    };
    const matrix3i b = {
        .m11 = 1, .m12 = 0, .m13 = 1,
        .m21 = 2, .m22 = 1, .m23 = 0,
        .m31 = 0, .m32 = 2, .m33 = 1
    };
    matrix3i res;
    mat3i_mul_ptr(&res, &a, &b);

    REQUIRE(res.m11 == 5);  REQUIRE(res.m12 == 2);  REQUIRE(res.m13 == 1);
    REQUIRE(res.m21 == 2);  REQUIRE(res.m22 == 5);  REQUIRE(res.m23 == 2);
    REQUIRE(res.m31 == 1);  REQUIRE(res.m32 == 2);  REQUIRE(res.m33 == 2);
}

TEST_CASE(mat3i_transpose_ptr_test, "[matrix3i_ptr]")
{
    const matrix3i m = {
        .m11=1, .m12=2, .m13=3,
        .m21=4, .m22=5, .m23=6,
        .m31=7, .m32=8, .m33=9
    };
    matrix3i res;
    mat3i_transpose_ptr(&res, &m);
    REQUIRE(res.m11 == 1); REQUIRE(res.m12 == 4); REQUIRE(res.m13 == 7);
    REQUIRE(res.m21 == 2); REQUIRE(res.m22 == 5); REQUIRE(res.m23 == 8);
    REQUIRE(res.m31 == 3); REQUIRE(res.m32 == 6); REQUIRE(res.m33 == 9);
}

TEST_CASE(mat3i_inverse_ptr_test, "[matrix3i_ptr]")
{
    const matrix3i m = {
        .m11=1, .m12=0, .m13=0,
        .m21=0, .m22=1, .m23=0,
        .m31=0, .m32=0, .m33=1
    };  // Identity, determinant = 1
    matrix3i res;
    mat3i_inverse_ptr(&res, &m);
    REQUIRE(res.m11 == 1); REQUIRE(res.m12 == 0); REQUIRE(res.m13 == 0);
    REQUIRE(res.m21 == 0); REQUIRE(res.m22 == 1); REQUIRE(res.m23 == 0);
    REQUIRE(res.m31 == 0); REQUIRE(res.m32 == 0); REQUIRE(res.m33 == 1);
}

TEST_CASE(mat3i_inverse_ptr_singular_test, "[matrix3i_ptr]")
{
    const matrix3i m = {
        .m11=1, .m12=1, .m13=1,
        .m21=1, .m22=1, .m23=1,
        .m31=1, .m32=1, .m33=1
    };  // Singular matrix (determinant = 0)
    matrix3i res;
    mat3i_inverse_ptr(&res, &m);
    // Should return identity
    REQUIRE(res.m11 == 1); REQUIRE(res.m12 == 0); REQUIRE(res.m13 == 0);
    REQUIRE(res.m21 == 0); REQUIRE(res.m22 == 1); REQUIRE(res.m23 == 0);
    REQUIRE(res.m31 == 0); REQUIRE(res.m32 == 0); REQUIRE(res.m33 == 1);
}

// Tests for matrix4i _ptr functions
TEST_CASE(mat4i_identity_ptr_test, "[matrix4i_ptr]")
{
    matrix4i m;
    mat4i_identity_ptr(&m);
    REQUIRE(m.v[0] == 1);  REQUIRE(m.v[1] == 0);  REQUIRE(m.v[2] == 0);  REQUIRE(m.v[3] == 0);
    REQUIRE(m.v[4] == 0);  REQUIRE(m.v[5] == 1);  REQUIRE(m.v[6] == 0);  REQUIRE(m.v[7] == 0);
    REQUIRE(m.v[8] == 0);  REQUIRE(m.v[9] == 0);  REQUIRE(m.v[10] == 1); REQUIRE(m.v[11] == 0);
    REQUIRE(m.v[12] == 0); REQUIRE(m.v[13] == 0); REQUIRE(m.v[14] == 0); REQUIRE(m.v[15] == 1);
}

TEST_CASE(mat4i_mul_ptr_test, "[matrix4i_ptr]")
{
    const matrix4i a = {
        .m11 = 1,  .m21 = 0,  .m31 = 0,  .m41 = 0,
        .m12 = 2,  .m22 = 1,  .m32 = 0,  .m42 = 0,
        .m13 = 0,  .m23 = 2,  .m33 = 1,  .m43 = 0,
        .m14 = 0,  .m24 = 0,  .m34 = 2,  .m44 = 1
    };
    const matrix4i b = {
        .m11 = 1,  .m21 = 2,  .m31 = 0,  .m41 = 0,
        .m12 = 0,  .m22 = 1,  .m32 = 2,  .m42 = 0,
        .m13 = 0,  .m23 = 0,  .m33 = 1,  .m43 = 2,
        .m14 = 1,  .m24 = 0,  .m34 = 0,  .m44 = 1
    };
    matrix4i res;
    mat4i_mul_ptr(&res, &a, &b);

    REQUIRE(res.m11 == 5); REQUIRE(res.m21 == 2); REQUIRE(res.m31 == 0); REQUIRE(res.m41 == 0);
    REQUIRE(res.m12 == 2); REQUIRE(res.m22 == 5); REQUIRE(res.m32 == 2); REQUIRE(res.m42 == 0);
    REQUIRE(res.m13 == 0); REQUIRE(res.m23 == 2); REQUIRE(res.m33 == 5); REQUIRE(res.m43 == 2);
    REQUIRE(res.m14 == 1); REQUIRE(res.m24 == 0); REQUIRE(res.m34 == 2); REQUIRE(res.m44 == 1);
}

TEST_CASE(mat4i_transpose_ptr_test, "[matrix4i_ptr]")
{
    const matrix4i m = {
        .m11 = 1,  .m21 = 2,  .m31 = 3,  .m41 = 4,
        .m12 = 5,  .m22 = 6,  .m32 = 7,  .m42 = 8,
        .m13 = 9,  .m23 = 10, .m33 = 11, .m43 = 12,
        .m14 = 13, .m24 = 14, .m34 = 15, .m44 = 16
    };
    matrix4i res;
    mat4i_transpose_ptr(&res, &m);
    REQUIRE(res.v[0] == 1);  REQUIRE(res.v[1] == 5);  REQUIRE(res.v[2] == 9);   REQUIRE(res.v[3] == 13);
    REQUIRE(res.v[4] == 2);  REQUIRE(res.v[5] == 6);  REQUIRE(res.v[6] == 10);  REQUIRE(res.v[7] == 14);
    REQUIRE(res.v[8] == 3);  REQUIRE(res.v[9] == 7);  REQUIRE(res.v[10] == 11); REQUIRE(res.v[11] == 15);
    REQUIRE(res.v[12] == 4); REQUIRE(res.v[13] == 8); REQUIRE(res.v[14] == 12); REQUIRE(res.v[15] == 16);
}

TEST_CASE(mat4i_inverse_ptr_test, "[matrix4i_ptr]")
{
    const matrix4i m = {
        .m11 = 1, .m21 = 0, .m31 = 0, .m41 = 0,
        .m12 = 0, .m22 = 1, .m32 = 0, .m42 = 0,
        .m13 = 0, .m23 = 0, .m33 = 1, .m43 = 0,
        .m14 = 0, .m24 = 0, .m34 = 0, .m44 = 1
    };  // Identity, determinant = 1
    matrix4i res;
    mat4i_inverse_ptr(&res, &m);
    REQUIRE(res.v[0] == 1);  REQUIRE(res.v[1] == 0);  REQUIRE(res.v[2] == 0);  REQUIRE(res.v[3] == 0);
    REQUIRE(res.v[4] == 0);  REQUIRE(res.v[5] == 1);  REQUIRE(res.v[6] == 0);  REQUIRE(res.v[7] == 0);
    REQUIRE(res.v[8] == 0);  REQUIRE(res.v[9] == 0);  REQUIRE(res.v[10] == 1); REQUIRE(res.v[11] == 0);
    REQUIRE(res.v[12] == 0); REQUIRE(res.v[13] == 0); REQUIRE(res.v[14] == 0); REQUIRE(res.v[15] == 1);
}

TEST_CASE(mat4i_inverse_ptr_singular_test, "[matrix4i_ptr]")
{
    const matrix4i m = {
        .m11 = 1, .m21 = 1, .m31 = 1, .m41 = 1,
        .m12 = 1, .m22 = 1, .m32 = 1, .m42 = 1,
        .m13 = 1, .m23 = 1, .m33 = 1, .m43 = 1,
        .m14 = 1, .m24 = 1, .m34 = 1, .m44 = 1
    };  // Singular matrix (determinant = 0)
    matrix4i res;
    mat4i_inverse_ptr(&res, &m);
    // Should return identity
    REQUIRE(res.v[0] == 1);  REQUIRE(res.v[1] == 0);  REQUIRE(res.v[2] == 0);  REQUIRE(res.v[3] == 0);
    REQUIRE(res.v[4] == 0);  REQUIRE(res.v[5] == 1);  REQUIRE(res.v[6] == 0);  REQUIRE(res.v[7] == 0);
    REQUIRE(res.v[8] == 0);  REQUIRE(res.v[9] == 0);  REQUIRE(res.v[10] == 1); REQUIRE(res.v[11] == 0);
    REQUIRE(res.v[12] == 0); REQUIRE(res.v[13] == 0); REQUIRE(res.v[14] == 0); REQUIRE(res.v[15] == 1);
}

TEST_CASE(mat2i_mul_vec2i_ptr_test, "[matrix2i_ptr]")
{
    const matrix2i m = {.m11 = 1, .m21 = 2, .m12 = 3, .m22 = 4};
    const vector2i v = {.x = 1, .y = 1};
    vector2i res;
    mat2i_mul_vec2i_ptr(&res, &m, &v);
    REQUIRE(vec2i_eq(res, (vector2i){.x = 4, .y = 6}));

    const matrix2i id = mat2i_identity();
    const vector2i u = {.x = 3, .y = 4};
    mat2i_mul_vec2i_ptr(&res, &id, &u);
    REQUIRE(vec2i_eq(res, u));
}

TEST_CASE(mat3i_mul_vec3i_ptr_test, "[matrix3i_ptr]")
{
    const matrix3i a = {
        .m11 = 1, .m21 = 2, .m31 = 3,
        .m12 = 4, .m22 = 5, .m32 = 6,
        .m13 = 7, .m23 = 8, .m33 = 9
    };
    const vector3i v = {.x = 1, .y = 0, .z = 0};
    vector3i res;
    mat3i_mul_vec3i_ptr(&res, &a, &v);
    REQUIRE(vec3i_eq(res, (vector3i){.x = 1, .y = 2, .z = 3}));

    const matrix3i id = mat3i_identity();
    const vector3i u = {.x = 2, .y = 3, .z = 4};
    mat3i_mul_vec3i_ptr(&res, &id, &u);
    REQUIRE(vec3i_eq(res, u));
}

TEST_CASE(mat3i_mul_vec2i_ptr_test, "[matrix3i_ptr]")
{
    matrix3i m = mat3i_identity();
    m.m13 = 5;
    m.m23 = 7;
    const vector2i v = {.x = 1, .y = 2};
    vector2i res;
    mat3i_mul_vec2i_ptr(&res, &m, &v);
    REQUIRE(vec2i_eq(res, (vector2i){.x = 6, .y = 9}));
}

TEST_CASE(mat4i_mul_vec4i_ptr_test, "[matrix4i_ptr]")
{
    const matrix4i id = mat4i_identity();
    const vector4i v = {.x = 1, .y = 2, .z = 3, .w = 4};
    vector4i res;
    mat4i_mul_vec4i_ptr(&res, &id, &v);
    REQUIRE(vec4i_eq(res, v));

    matrix4i s = mat4i_identity();
    s.m11 = 2;
    s.m22 = 3;
    s.m33 = 4;
    const vector4i u = {.x = 1, .y = 1, .z = 1, .w = 1};
    mat4i_mul_vec4i_ptr(&res, &s, &u);
    REQUIRE(vec4i_eq(res, (vector4i){.x = 2, .y = 3, .z = 4, .w = 1}));
}

TEST_CASE(mat4i_mul_vec3i_ptr_test, "[matrix4i_ptr]")
{
    matrix4i t = mat4i_identity();
    t.m14 = 1;
    t.m24 = 2;
    t.m34 = 3;
    const vector3i origin = {.x = 0, .y = 0, .z = 0};
    vector3i res;
    mat4i_mul_vec3i_ptr(&res, &t, &origin, 1);
    REQUIRE(vec3i_eq(res, (vector3i){.x = 1, .y = 2, .z = 3}));

    const vector3i dir = {.x = 4, .y = 5, .z = 6};
    mat4i_mul_vec3i_ptr(&res, &t, &dir, 0);
    REQUIRE(vec3i_eq(res, dir));
}
