// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "unitest.h"

// matrix2i tests
TEST_CASE(mat2i_identity_test, "[matrix2i]") {
    const matrix2i m = mat2i_identity();
    REQUIRE(mat2i_eq(m, (matrix2i){.m11 = 1, .m21 = 0, .m12 = 0, .m22 = 1}));
}

TEST_CASE(mat2i_mul_test, "[matrix2i]") {
    const matrix2i a = {.m11 = 1, .m21 = 2, .m12 = 3, .m22 = 4};
    const matrix2i b = {.m11 = 5, .m21 = 6, .m12 = 7, .m22 = 8};
    const matrix2i res = mat2i_mul(a, b);
    REQUIRE(mat2i_eq(res, (matrix2i){.m11 = 23, .m21 = 34, .m12 = 31, .m22 = 46}));
}

TEST_CASE(mat2i_transpose_test, "[matrix2i]") {
    const matrix2i m = {.m11 = 1, .m21 = 2, .m12 = 3, .m22 = 4};
    const matrix2i res = mat2i_transpose(m);
    REQUIRE(mat2i_eq(res, (matrix2i){.m11 = 1, .m21 = 3, .m12 = 2, .m22 = 4}));
}

TEST_CASE(mat2i_determinant_test, "[matrix2i]") {
    const matrix2i m = {.m11 = 1, .m21 = 2, .m12 = 3, .m22 = 4};
    const int det = mat2i_determinant(m);
    REQUIRE(det == -2);
}

TEST_CASE(mat2i_inverse_test, "[matrix2i]") {
    const matrix2i m = mat2i_identity();
    REQUIRE(mat2i_eq(mat2i_inverse(m), m));
}

TEST_CASE(mat2i_mul_vec2i_test, "[matrix2i]") {
    const matrix2i m = {.m11 = 1, .m21 = 2, .m12 = 3, .m22 = 4};
    REQUIRE(vec2i_eq(mat2i_mul_vec2i(m, (vector2i){.x = 1, .y = 0}), (vector2i){.x = 1, .y = 2}));
    REQUIRE(vec2i_eq(mat2i_mul_vec2i(m, (vector2i){.x = 0, .y = 1}), (vector2i){.x = 3, .y = 4}));
    REQUIRE(vec2i_eq(mat2i_mul_vec2i(m, (vector2i){.x = 1, .y = 1}), (vector2i){.x = 4, .y = 6}));
    REQUIRE(vec2i_eq(mat2i_mul_vec2i(mat2i_identity(), (vector2i){.x = 3, .y = 4}),
        (vector2i){.x = 3, .y = 4}));
}

// matrix3i tests
TEST_CASE(mat3i_identity_test, "[matrix3i]") {
    const matrix3i m = mat3i_identity();
    REQUIRE(mat3i_eq(m, (matrix3i){
        .m11 = 1, .m21 = 0, .m31 = 0,
        .m12 = 0, .m22 = 1, .m32 = 0,
        .m13 = 0, .m23 = 0, .m33 = 1
        }));
}

TEST_CASE(mat3i_mul_test, "[matrix3i]") {
    const matrix3i a = {
        .m11 = 1, .m21 = 2, .m31 = 3,
        .m12 = 4, .m22 = 5, .m32 = 6,
        .m13 = 7, .m23 = 8, .m33 = 9
    };
    const matrix3i b = {
        .m11 = 9, .m21 = 8, .m31 = 7,
        .m12 = 6, .m22 = 5, .m32 = 4,
        .m13 = 3, .m23 = 2, .m33 = 1
    };
    const matrix3i res = mat3i_mul(a, b);
    REQUIRE(mat3i_eq(res, (matrix3i){
        .m11 = 90,  .m21 = 114, .m31 = 138,
        .m12 = 54,  .m22 = 69,  .m32 = 84,
        .m13 = 18,  .m23 = 24,  .m33 = 30
        }));
}

TEST_CASE(mat3i_transpose_test, "[matrix3i]") {
    const matrix3i m = {
        .m11 = 1, .m21 = 2, .m31 = 3,
        .m12 = 4, .m22 = 5, .m32 = 6,
        .m13 = 7, .m23 = 8, .m33 = 9
    };
    const matrix3i res = mat3i_transpose(m);
    REQUIRE(mat3i_eq(res, (matrix3i){
        .m11 = 1, .m21 = 4, .m31 = 7,
        .m12 = 2, .m22 = 5, .m32 = 8,
        .m13 = 3, .m23 = 6, .m33 = 9
        }));
}

TEST_CASE(mat3i_determinant_test, "[matrix3i]") {
    const matrix3i m = {
        .m11 = 1, .m21 = 2, .m31 = 3,
        .m12 = 0, .m22 = 1, .m32 = 4,
        .m13 = 5, .m23 = 6, .m33 = 0
    };
    const int det = mat3i_determinant(m);
    REQUIRE(det == 1);
}

TEST_CASE(mat3i_inverse_test, "[matrix3i]") {
    // Identity
    const matrix3i m = {
        .m11 = 1, .m21 = 0, .m31 = 0,
        .m12 = 0, .m22 = 1, .m32 = 0,
        .m13 = 0, .m23 = 0, .m33 = 1
    };
    const matrix3i inv = mat3i_inverse(m);
    REQUIRE(mat3i_eq(inv, m));

    const matrix3i m2 = {
        .m11 = 1, .m21 = 2, .m31 = 3,
        .m12 = 0, .m22 = 1, .m32 = 4,
        .m13 = 5, .m23 = 6, .m33 = 0
    };
    const matrix3i inv2 = mat3i_inverse(m2);
    const matrix3i res = mat3i_mul(m2, inv2);
    REQUIRE(mat3i_eq(res, mat3i_identity()));
}

TEST_CASE(mat3i_mul_vec3i_test, "[matrix3i]") {
    const matrix3i id = mat3i_identity();
    const vector3i v = {.x = 2, .y = 3, .z = 4};
    REQUIRE(vec3i_eq(mat3i_mul_vec3i(id, v), v));

    const matrix3i a = {
        .m11 = 1, .m21 = 2, .m31 = 3,
        .m12 = 4, .m22 = 5, .m32 = 6,
        .m13 = 7, .m23 = 8, .m33 = 9
    };

    REQUIRE(vec3i_eq(mat3i_mul_vec3i(a, (vector3i){.x = 1, .y = 0, .z = 0}),
        (vector3i){.x = 1, .y = 2, .z = 3}));
    REQUIRE(vec3i_eq(mat3i_mul_vec3i(a, (vector3i){.x = 0, .y = 1, .z = 0}),
        (vector3i){.x = 4, .y = 5, .z = 6}));
    REQUIRE(vec3i_eq(mat3i_mul_vec3i(a, (vector3i){.x = 0, .y = 0, .z = 1}),
        (vector3i){.x = 7, .y = 8, .z = 9}));
}

TEST_CASE(mat3i_mul_vec2i_test, "[matrix3i]") {
    matrix3i m = mat3i_identity();
    m.m13 = 5;
    m.m23 = 7;

    REQUIRE(vec2i_eq(mat3i_mul_vec2i(m, (vector2i){.x = 1, .y = 2}),
        (vector2i){.x = 6, .y = 9}));
    REQUIRE(vec2i_eq(mat3i_mul_vec2i(mat3i_identity(), (vector2i){.x = 3, .y = 4}),
        (vector2i){.x = 3, .y = 4}));
}

// matrix4i tests
TEST_CASE(mat4i_identity_test, "[matrix4i]") {
    const matrix4i m = mat4i_identity();
    REQUIRE(mat4i_eq(m, (matrix4i){
        .m11 = 1, .m21 = 0, .m31 = 0, .m41 = 0,
        .m12 = 0, .m22 = 1, .m32 = 0, .m42 = 0,
        .m13 = 0, .m23 = 0, .m33 = 1, .m43 = 0,
        .m14 = 0, .m24 = 0, .m34 = 0, .m44 = 1
        }));
}

TEST_CASE(mat4i_mul_test, "[matrix4i]") {
    const matrix4i a = mat4i_identity();
    const matrix4i b = mat4i_identity();
    const matrix4i res = mat4i_mul(a, b);
    REQUIRE(mat4i_eq(res, mat4i_identity()));
}

TEST_CASE(mat4i_transpose_test, "[matrix4i]") {
    const matrix4i m = {
        .m11 = 1,  .m21 = 2,  .m31 = 3,  .m41 = 4,
        .m12 = 5,  .m22 = 6,  .m32 = 7,  .m42 = 8,
        .m13 = 9,  .m23 = 10, .m33 = 11, .m43 = 12,
        .m14 = 13, .m24 = 14, .m34 = 15, .m44 = 16
    };

    const matrix4i res = mat4i_transpose(m);

    REQUIRE(mat4i_eq(res, (matrix4i){
        .m11 = 1, .m21 = 5, .m31 = 9,  .m41 = 13,
        .m12 = 2, .m22 = 6, .m32 = 10, .m42 = 14,
        .m13 = 3, .m23 = 7, .m33 = 11, .m43 = 15,
        .m14 = 4, .m24 = 8, .m34 = 12, .m44 = 16
        }));
}

TEST_CASE(mat4i_determinant_test, "[matrix4i]") {
    const matrix4i m = mat4i_identity();
    const int det = mat4i_determinant(m);
    REQUIRE(det == 1);
}

TEST_CASE(mat4i_inverse_test, "[matrix4i]") {
    const matrix4i m = mat4i_identity();
    const matrix4i inv = mat4i_inverse(m);
    REQUIRE(mat4i_eq(inv, m));

    // Test a simple invertible matrix
    const matrix4i m2 = {
        .m11 = 1, .m21 = 0, .m31 = 0, .m41 = 0,
        .m12 = 0, .m22 = 1, .m32 = 0, .m42 = 0,
        .m13 = 0, .m23 = 0, .m33 = 1, .m43 = 0,
        .m14 = 0, .m24 = 0, .m34 = 0, .m44 = 1
    }; // Identity again

    const matrix4i inv2 = mat4i_inverse(m2);
    REQUIRE(mat4i_eq(inv2, m2));
}

TEST_CASE(mat4i_mul_vec4i_test, "[matrix4i]") {
    const vector4i v = {.x = 1, .y = 2, .z = 3, .w = 4};
    REQUIRE(vec4i_eq(mat4i_mul_vec4i(mat4i_identity(), v), v));

    matrix4i s = mat4i_identity();
    s.m11 = 2;
    s.m22 = 3;
    s.m33 = 4;

    REQUIRE(vec4i_eq(mat4i_mul_vec4i(s, (vector4i){.x = 1, .y = 1, .z = 1, .w = 1}),
        (vector4i){.x = 2, .y = 3, .z = 4, .w = 1}));

    matrix4i t = mat4i_identity();
    t.m14 = 1;
    t.m24 = 2;
    t.m34 = 3;

    REQUIRE(vec4i_eq(mat4i_mul_vec4i(t, (vector4i){.x = 0, .y = 0, .z = 0, .w = 1}),
        (vector4i){.x = 1, .y = 2, .z = 3, .w = 1}));
}

TEST_CASE(mat4i_mul_vec3i_test, "[matrix4i]") {
    matrix4i t = mat4i_identity();
    t.m14 = 1;
    t.m24 = 2;
    t.m34 = 3;

    REQUIRE(vec3i_eq(mat4i_mul_vec3i(t, (vector3i){.x = 0, .y = 0, .z = 0}, 1),
        (vector3i){.x = 1, .y = 2, .z = 3}));
    REQUIRE(vec3i_eq(mat4i_mul_vec3i(t, (vector3i){.x = 4, .y = 5, .z = 6}, 0),
        (vector3i){.x = 4, .y = 5, .z = 6}));

    matrix4i s = mat4i_identity();
    s.m11 = 2;
    s.m22 = 3;
    s.m33 = 4;

    REQUIRE(vec3i_eq(mat4i_mul_vec3i(s, (vector3i){.x = 1, .y = 1, .z = 1}, 1),
        (vector3i){.x = 2, .y = 3, .z = 4}));
}
