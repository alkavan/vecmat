// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "unitest.h"

// matrix2 tests
TEST_CASE(mat2_identity_test, "[matrix2]") {
    const matrix2 m = mat2_identity();
    REQUIRE(mat2_eq(m, (matrix2){.m11 = 1.0f, .m21 = 0.0f, .m12 = 0.0f, .m22 = 1.0f}));
}

TEST_CASE(mat2_mul_test, "[matrix2]") {
    const matrix2 a = {.m11 = 1.0f, .m21 = 2.0f, .m12 = 3.0f, .m22 = 4.0f};
    const matrix2 b = {.m11 = 5.0f, .m21 = 6.0f, .m12 = 7.0f, .m22 = 8.0f};
    const matrix2 res = mat2_mul(a, b);
    REQUIRE(mat2_eq(res, (matrix2){.m11 = 23.0f, .m21 = 34.0f, .m12 = 31.0f, .m22 = 46.0f}));
}

TEST_CASE(mat2_mul_associative_test, "[matrix2]") {
    const matrix2 a = {.m11 = 1.0f, .m21 = 2.0f, .m12 = 3.0f, .m22 = 4.0f};
    const matrix2 b = {.m11 = 5.0f, .m21 = 6.0f, .m12 = 7.0f, .m22 = 8.0f};
    const vector2 v = {.x = 1.0f, .y = 0.0f};
    const vector2 left = mat2_mul_vec2(mat2_mul(a, b), v);
    const vector2 right = mat2_mul_vec2(a, mat2_mul_vec2(b, v));
    REQUIRE(vec2_eq(left, right));
    REQUIRE(vec2_eq(left, (vector2){.x = 23.0f, .y = 34.0f}));
}

TEST_CASE(mat2_transpose_test, "[matrix2]") {
    const matrix2 m = {.m11 = 1.0f, .m21 = 2.0f, .m12 = 3.0f, .m22 = 4.0f};
    const matrix2 res = mat2_transpose(m);
    REQUIRE(mat2_eq(res, (matrix2){.m11 = 1.0f, .m21 = 3.0f, .m12 = 2.0f, .m22 = 4.0f}));
}

TEST_CASE(mat2_inverse_test, "[matrix2]") {
    const matrix2 m = {.m11 = 1.0f, .m21 = 2.0f, .m12 = 3.0f, .m22 = 4.0f};
    const matrix2 inv = mat2_inverse(m);
    const matrix2 res = mat2_mul(m, inv);
    REQUIRE(mat2_eq(res, mat2_identity()));
}

TEST_CASE(mat2_rotation_z_test, "[matrix2]") {
    const vm_float_t degrees = 90.0f;
    const matrix2 rot = mat2_rotation_z(degrees);
    // For 90 degrees: cos(90)=0, sin(90)=1
    // Matrix should be: [0, -1; 1, 0]
    REQUIRE(VECMAT_EQ(rot.m11, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(rot.m12, -1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(rot.m21, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(rot.m22, 0.0f, EPSILON));
}

TEST_CASE(mat2_mul_vec2_test, "[matrix2]") {
    const matrix2 m = {.m11 = 1.0f, .m21 = 2.0f, .m12 = 3.0f, .m22 = 4.0f};
    REQUIRE(vec2_eq(mat2_mul_vec2(m, (vector2){.x = 1.0f, .y = 0.0f}), (vector2){.x = 1.0f, .y = 2.0f}));
    REQUIRE(vec2_eq(mat2_mul_vec2(m, (vector2){.x = 0.0f, .y = 1.0f}), (vector2){.x = 3.0f, .y = 4.0f}));
    REQUIRE(vec2_eq(mat2_mul_vec2(m, (vector2){.x = 1.0f, .y = 1.0f}), (vector2){.x = 4.0f, .y = 6.0f}));
    REQUIRE(vec2_eq(mat2_mul_vec2(mat2_identity(), (vector2){.x = 3.0f, .y = 4.0f}), (vector2){.x = 3.0f, .y = 4.0f}));

    const vector2 rotated = mat2_mul_vec2(mat2_rotation_z(90.0f), (vector2){.x = 1.0f, .y = 0.0f});
    REQUIRE(vec2_near(rotated, (vector2){.x = 0.0f, .y = 1.0f}, EPSILON));
}

// matrix3 tests
TEST_CASE(mat3_identity_test, "[matrix3]") {
    const matrix3 m = mat3_identity();
    REQUIRE(mat3_eq(m, (matrix3){
        .m11 = 1, .m21 = 0, .m31 = 0,
        .m12 = 0, .m22 = 1, .m32 = 0,
        .m13 = 0, .m23 = 0, .m33 = 1
        }));
}

TEST_CASE(mat3_mul_test, "[matrix3]") {
    const matrix3 a = {
        .m11 = 1, .m21 = 2, .m31 = 3,
        .m12 = 4, .m22 = 5, .m32 = 6,
        .m13 = 7, .m23 = 8, .m33 = 9
    };
    const matrix3 b = {
        .m11 = 9, .m21 = 8, .m31 = 7,
        .m12 = 6, .m22 = 5, .m32 = 4,
        .m13 = 3, .m23 = 2, .m33 = 1
    };
    const matrix3 res = mat3_mul(a, b);
    REQUIRE(mat3_eq(res, (matrix3){
        .m11 = 90,  .m21 = 114, .m31 = 138,
        .m12 = 54,  .m22 = 69,  .m32 = 84,
        .m13 = 18,  .m23 = 24,  .m33 = 30
        }));
}

TEST_CASE(mat3_mul_associative_test, "[matrix3]") {
    const matrix3 a = {
        .m11 = 1, .m21 = 2, .m31 = 3,
        .m12 = 4, .m22 = 5, .m32 = 6,
        .m13 = 7, .m23 = 8, .m33 = 9
    };
    const matrix3 b = {
        .m11 = 9, .m21 = 8, .m31 = 7,
        .m12 = 6, .m22 = 5, .m32 = 4,
        .m13 = 3, .m23 = 2, .m33 = 1
    };
    const vector3 v = {.x = 1.0f, .y = 0.0f, .z = 0.0f};
    const vector3 left = mat3_mul_vec3(mat3_mul(a, b), v);
    const vector3 right = mat3_mul_vec3(a, mat3_mul_vec3(b, v));
    REQUIRE(vec3_eq(left, right));
    REQUIRE(vec3_eq(left, (vector3){.x = 90.0f, .y = 114.0f, .z = 138.0f}));
}

TEST_CASE(mat3_transpose_test, "[matrix3]") {
    const matrix3 m = {
        .m11 = 1, .m21 = 2, .m31 = 3,
        .m12 = 4, .m22 = 5, .m32 = 6,
        .m13 = 7, .m23 = 8, .m33 = 9
    };
    const matrix3 res = mat3_transpose(m);
    REQUIRE(mat3_eq(res, (matrix3){
        .m11 = 1, .m21 = 4, .m31 = 7,
        .m12 = 2, .m22 = 5, .m32 = 8,
        .m13 = 3, .m23 = 6, .m33 = 9
        }));
}

TEST_CASE(mat3_inverse_test, "[matrix3]") {
    // Identity
    const matrix3 m = {
        .m11 = 1, .m21 = 0, .m31 = 0,
        .m12 = 0, .m22 = 1, .m32 = 0,
        .m13 = 0, .m23 = 0, .m33 = 1
    };
    const matrix3 inv = mat3_inverse(m);
    REQUIRE(mat3_eq(inv, m));

    const matrix3 m2 = {
        .m11 = 1, .m21 = 2, .m31 = 3,
        .m12 = 0, .m22 = 1, .m32 = 4,
        .m13 = 5, .m23 = 6, .m33 = 0
    };
    const matrix3 inv2 = mat3_inverse(m2);
    const matrix3 res = mat3_mul(m2, inv2);
    REQUIRE(mat3_eq(res, mat3_identity()));
}

TEST_CASE(mat3_rotation_z_test, "[matrix3]") {
    const vm_float_t degrees = 90.0f;
    const matrix3 rot = mat3_rotation_z(degrees);
    // For 90 degrees: cos(90)=0, sin(90)=1
    // Matrix: [[0, -1, 0], [1, 0, 0], [0, 0, 1]]
    REQUIRE(VECMAT_EQ(rot.m11, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(rot.m12, -1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(rot.m13, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(rot.m21, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(rot.m22, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(rot.m23, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(rot.m31, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(rot.m32, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(rot.m33, 1.0f, EPSILON));
}

TEST_CASE(mat3_mul_vec3_test, "[matrix3]") {
    const matrix3 m = {
        .m11 = 1, .m21 = 0, .m31 = 0,
        .m12 = 0, .m22 = 1, .m32 = 0,
        .m13 = 0, .m23 = 0, .m33 = 1
    };
    const vector3 v = {.x = 2.0f, .y = 3.0f, .z = 4.0f};
    REQUIRE(vec3_eq(mat3_mul_vec3(m, v), v));

    const matrix3 a = {
        .m11 = 1, .m21 = 2, .m31 = 3,
        .m12 = 4, .m22 = 5, .m32 = 6,
        .m13 = 7, .m23 = 8, .m33 = 9
    };
    REQUIRE(vec3_eq(mat3_mul_vec3(a, (vector3){.x = 1.0f, .y = 0.0f, .z = 0.0f}),
        (vector3){.x = 1.0f, .y = 2.0f, .z = 3.0f}));
    REQUIRE(vec3_eq(mat3_mul_vec3(a, (vector3){.x = 0.0f, .y = 1.0f, .z = 0.0f}),
        (vector3){.x = 4.0f, .y = 5.0f, .z = 6.0f}));
    REQUIRE(vec3_eq(mat3_mul_vec3(a, (vector3){.x = 0.0f, .y = 0.0f, .z = 1.0f}),
        (vector3){.x = 7.0f, .y = 8.0f, .z = 9.0f}));

    const vector3 rotated = mat3_mul_vec3(mat3_rotation_z(90.0f),
        (vector3){.x = 1.0f, .y = 0.0f, .z = 5.0f});
    REQUIRE(vec3_near(rotated, (vector3){.x = 0.0f, .y = 1.0f, .z = 5.0f}, EPSILON));
}

TEST_CASE(mat3_mul_vec2_test, "[matrix3]") {
    matrix3 m = mat3_identity();
    m.m13 = 5.0f;
    m.m23 = 7.0f;
    REQUIRE(vec2_eq(mat3_mul_vec2(m, (vector2){.x = 1.0f, .y = 2.0f}),
        (vector2){.x = 6.0f, .y = 9.0f}));
    REQUIRE(vec2_eq(mat3_mul_vec2(mat3_identity(), (vector2){.x = 3.0f, .y = 4.0f}),
        (vector2){.x = 3.0f, .y = 4.0f}));
}

// matrix4 tests
TEST_CASE(mat4_identity_test, "[matrix4]") {
    const matrix4 m = mat4_identity();
    REQUIRE(mat4_eq(m, (matrix4){
        .m11 = 1, .m21 = 0, .m31 = 0, .m41 = 0,
        .m12 = 0, .m22 = 1, .m32 = 0, .m42 = 0,
        .m13 = 0, .m23 = 0, .m33 = 1, .m43 = 0,
        .m14 = 0, .m24 = 0, .m34 = 0, .m44 = 1
        }));
}

TEST_CASE(mat4_mul_test, "[matrix4]") {
    const matrix4 a = mat4_identity();
    const matrix4 b = mat4_identity();
    const matrix4 res = mat4_mul(a, b);
    REQUIRE(mat4_eq(res, mat4_identity()));

    const matrix4 a2 = {
        .m11 = 1.0f, .m21 = 2.0f, .m31 = 0.0f, .m41 = 0.0f,
        .m12 = 3.0f, .m22 = 4.0f, .m32 = 0.0f, .m42 = 0.0f,
        .m13 = 0.0f, .m23 = 0.0f, .m33 = 1.0f, .m43 = 0.0f,
        .m14 = 0.0f, .m24 = 0.0f, .m34 = 0.0f, .m44 = 1.0f
    };
    const matrix4 b2 = {
        .m11 = 5.0f, .m21 = 6.0f, .m31 = 0.0f, .m41 = 0.0f,
        .m12 = 7.0f, .m22 = 8.0f, .m32 = 0.0f, .m42 = 0.0f,
        .m13 = 0.0f, .m23 = 0.0f, .m33 = 1.0f, .m43 = 0.0f,
        .m14 = 0.0f, .m24 = 0.0f, .m34 = 0.0f, .m44 = 1.0f
    };
    REQUIRE(mat4_eq(mat4_mul(a2, b2), (matrix4){
        .m11 = 23.0f, .m21 = 34.0f, .m31 = 0.0f, .m41 = 0.0f,
        .m12 = 31.0f, .m22 = 46.0f, .m32 = 0.0f, .m42 = 0.0f,
        .m13 = 0.0f,  .m23 = 0.0f,  .m33 = 1.0f, .m43 = 0.0f,
        .m14 = 0.0f,  .m24 = 0.0f,  .m34 = 0.0f, .m44 = 1.0f
    }));
}

TEST_CASE(mat4_mul_associative_test, "[matrix4]") {
    const matrix4 r = mat4_rotation_z(90.0f);
    const matrix4 t = mat4_translate((vector3){.x = 1.0f, .y = 2.0f, .z = 3.0f});
    const vector4 v = {.x = 1.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f};
    const vector4 left = mat4_mul_vec4(mat4_mul(t, r), v);
    const vector4 right = mat4_mul_vec4(t, mat4_mul_vec4(r, v));
    REQUIRE(vec4_near(left, right, EPSILON));
    REQUIRE(vec4_near(left, (vector4){.x = 1.0f, .y = 3.0f, .z = 3.0f, .w = 1.0f}, EPSILON));
}

TEST_CASE(mat4_transpose_test, "[matrix4]") {
    const matrix4 m = {
        .m11 = 1,  .m21 = 2,  .m31 = 3,  .m41 = 4,
        .m12 = 5,  .m22 = 6,  .m32 = 7,  .m42 = 8,
        .m13 = 9,  .m23 = 10, .m33 = 11, .m43 = 12,
        .m14 = 13, .m24 = 14, .m34 = 15, .m44 = 16
    };

    const matrix4 res = mat4_transpose(m);

    REQUIRE(mat4_eq(res, (matrix4){
        .m11 = 1, .m21 = 5, .m31 = 9,  .m41 = 13,
        .m12 = 2, .m22 = 6, .m32 = 10, .m42 = 14,
        .m13 = 3, .m23 = 7, .m33 = 11, .m43 = 15,
        .m14 = 4, .m24 = 8, .m34 = 12, .m44 = 16
        }));
}

TEST_CASE(mat4_inverse_test, "[matrix4]") {
    const matrix4 m = mat4_identity();
    const matrix4 inv = mat4_inverse(m);
    REQUIRE(mat4_eq(inv, m));

    // Test a translation matrix
    const matrix4 trans = mat4_translate((vector3){.x = 1,.y = 2,.z = 3});
    const matrix4 inv_trans = mat4_inverse(trans);
    const matrix4 res = mat4_mul(trans, inv_trans);
    REQUIRE(mat4_eq(res, mat4_identity()));
}

TEST_CASE(mat4_translate_test, "[matrix4]") {
    const matrix4 m = mat4_translate((vector3){.x = 1.0f, .y = 2.0f, .z = 3.0f});
    REQUIRE(VECMAT_EQ(m.m14, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(m.m24, 2.0f, EPSILON));
    REQUIRE(VECMAT_EQ(m.m34, 3.0f, EPSILON));
    REQUIRE(VECMAT_EQ(m.m44, 1.0f, EPSILON));
}

TEST_CASE(mat4_scale_test, "[matrix4]") {
    const matrix4 m = mat4_scale((vector3){.x = 2.0f, .y = 3.0f, .z = 4.0f});
    REQUIRE(VECMAT_EQ(m.m11, 2.0f, EPSILON));
    REQUIRE(VECMAT_EQ(m.m22, 3.0f, EPSILON));
    REQUIRE(VECMAT_EQ(m.m33, 4.0f, EPSILON));
    REQUIRE(VECMAT_EQ(m.m44, 1.0f, EPSILON));
}

TEST_CASE(mat4_rotate_test, "[matrix4]") {
    const vector3 axis = {.x = 0.0f, .y = 0.0f, .z = 1.0f};
    const matrix4 m = mat4_rotation(axis, 90.0f);
    REQUIRE(VECMAT_EQ(m.m11, 0.0f, EPSILON)); // cos(90)=0, sin(90)=1
    REQUIRE(VECMAT_EQ(m.m21, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(m.m12, -1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(m.m22, 0.0f, EPSILON));

    const vector3 rotated = mat4_mul_vec3(m, (vector3){.x = 1.0f, .y = 0.0f, .z = 0.0f}, 1.0f);
    REQUIRE(vec3_near(rotated, (vector3){.x = 0.0f, .y = 1.0f, .z = 0.0f}, EPSILON));
}

TEST_CASE(mat4_mul_vec4_test, "[matrix4]") {
    const vector4 v = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f};
    REQUIRE(vec4_eq(mat4_mul_vec4(mat4_identity(), v), v));

    const matrix4 s = mat4_scale((vector3){.x = 2.0f, .y = 3.0f, .z = 4.0f});
    REQUIRE(vec4_eq(mat4_mul_vec4(s, (vector4){.x = 1.0f, .y = 1.0f, .z = 1.0f, .w = 1.0f}),
                    (vector4){.x = 2.0f, .y = 3.0f, .z = 4.0f, .w = 1.0f}));

    const matrix4 t = mat4_translate((vector3){.x = 1.0f, .y = 2.0f, .z = 3.0f});
    REQUIRE(vec4_eq(mat4_mul_vec4(t, (vector4){.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f}),
                    (vector4){.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 1.0f}));
}

TEST_CASE(mat4_mul_vec3_test, "[matrix4]") {
    const matrix4 t = mat4_translate((vector3){.x = 1.0f, .y = 2.0f, .z = 3.0f});
    REQUIRE(vec3_eq(mat4_mul_vec3(t, (vector3){.x = 0.0f, .y = 0.0f, .z = 0.0f}, 1.0f),
                    (vector3){.x = 1.0f, .y = 2.0f, .z = 3.0f}));
    REQUIRE(vec3_eq(mat4_mul_vec3(t, (vector3){.x = 4.0f, .y = 5.0f, .z = 6.0f}, 0.0f),
                    (vector3){.x = 4.0f, .y = 5.0f, .z = 6.0f}));

    const matrix4 s = mat4_scale((vector3){.x = 2.0f, .y = 3.0f, .z = 4.0f});
    REQUIRE(vec3_eq(mat4_mul_vec3(s, (vector3){.x = 1.0f, .y = 1.0f, .z = 1.0f}, 1.0f),
                    (vector3){.x = 2.0f, .y = 3.0f, .z = 4.0f}));
}

TEST_CASE(mat2_scale_from_mat3_test, "[matrix2]") {
    const matrix2 s2 = mat2_scale((vector2){.x = 2.0f, .y = 3.0f});
    REQUIRE(vec2_eq(mat2_mul_vec2(s2, (vector2){.x = 1.0f, .y = 1.0f}),
        (vector2){.x = 2.0f, .y = 3.0f}));
    const matrix2 r = mat2_from_mat3(mat3_rotation_z(90.0f));
    REQUIRE(vec2_near(mat2_mul_vec2(r, (vector2){.x = 1.0f, .y = 0.0f}),
        (vector2){.x = 0.0f, .y = 1.0f}, EPSILON));
}

TEST_CASE(mat3_affine_test, "[matrix3]") {
    const matrix3 t = mat3_translate((vector2){.x = 5.0f, .y = 7.0f});
    REQUIRE(vec2_eq(mat3_mul_vec2(t, (vector2){.x = 1.0f, .y = 2.0f}),
        (vector2){.x = 6.0f, .y = 9.0f}));
    const matrix3 sc = mat3_scale((vector2){.x = 2.0f, .y = 3.0f});
    REQUIRE(vec2_eq(mat3_mul_vec2(sc, (vector2){.x = 1.0f, .y = 1.0f}),
        (vector2){.x = 2.0f, .y = 3.0f}));
    const vector3 rx = mat3_mul_vec3(mat3_rotation_x(90.0f), (vector3){.x = 0.0f, .y = 1.0f, .z = 0.0f});
    REQUIRE(vec3_near(rx, (vector3){.x = 0.0f, .y = 0.0f, .z = 1.0f}, EPSILON));
    const vector3 ry = mat3_mul_vec3(mat3_rotation_y(90.0f), (vector3){.x = 0.0f, .y = 0.0f, .z = 1.0f});
    REQUIRE(vec3_near(ry, (vector3){.x = 1.0f, .y = 0.0f, .z = 0.0f}, EPSILON));
}

TEST_CASE(mat4_named_rotations_test, "[matrix4]") {
    REQUIRE(vec3_near(
        mat4_mul_vec3(mat4_rotation_z(90.0f), (vector3){.x = 1.0f, .y = 0.0f, .z = 0.0f}, 1.0f),
        (vector3){.x = 0.0f, .y = 1.0f, .z = 0.0f}, EPSILON));
    REQUIRE(vec3_near(
        mat4_mul_vec3(mat4_rotation_x(90.0f), (vector3){.x = 0.0f, .y = 1.0f, .z = 0.0f}, 1.0f),
        (vector3){.x = 0.0f, .y = 0.0f, .z = 1.0f}, EPSILON));
    REQUIRE(vec3_near(
        mat4_mul_vec3(mat4_rotation_y(90.0f), (vector3){.x = 0.0f, .y = 0.0f, .z = 1.0f}, 1.0f),
        (vector3){.x = 1.0f, .y = 0.0f, .z = 0.0f}, EPSILON));
}

TEST_CASE(mat4_trs_extract_test, "[matrix4]") {
    const vector3 t = {.x = 1.0f, .y = 2.0f, .z = 3.0f};
    const vector3 sc = {.x = 2.0f, .y = 3.0f, .z = 4.0f};
    const quaternion r = quat_from_axis_angle((vector3){.x = 0.0f, .y = 0.0f, .z = 1.0f}, 0.0f);
    const matrix4 m = mat4_trs(t, r, sc);
    REQUIRE(vec3_eq(mat4_extract_translation(m), t));
    REQUIRE(vec3_near(mat4_extract_scale(m), sc, EPSILON));
    REQUIRE(vec3_eq(mat4_mul_vec3(m, (vector3){.x = 1.0f, .y = 1.0f, .z = 1.0f}, 1.0f),
                    (vector3){.x = 3.0f, .y = 5.0f, .z = 7.0f}));
    const matrix3 upper = mat3_from_mat4(m);
    REQUIRE(VECMAT_EQ(upper.m11, 2.0f, EPSILON));
    REQUIRE(VECMAT_EQ(upper.m22, 3.0f, EPSILON));
    REQUIRE(VECMAT_EQ(upper.m33, 4.0f, EPSILON));
    const matrix4 back = mat4_from_mat3(upper);
    REQUIRE(VECMAT_EQ(back.m11, 2.0f, EPSILON));
    REQUIRE(VECMAT_EQ(back.m44, 1.0f, EPSILON));
    const quaternion extracted = mat4_extract_rotation(m);
    REQUIRE(quat_near(extracted, r, EPSILON) ||
            quat_near(extracted, (quaternion){.x = -r.x, .y = -r.y, .z = -r.z, .w = -r.w}, EPSILON));
}

TEST_CASE(mat4_perspective_test, "[matrix4]") {
    const vm_float_t fov = 90.0f;
    const vm_float_t aspect = 1.0f;
    const vm_float_t near = 0.1f;
    const vm_float_t far = 100.0f;
    const matrix4 m = mat4_perspective(fov, aspect, near, far);
    const vm_float_t rad = deg_to_rad(fov / 2.0f);
    const vm_float_t tan_half_fov = VECMAT_TAN(rad);
    REQUIRE(VECMAT_EQ(m.m11, 1.0f / (aspect * tan_half_fov), EPSILON));
    REQUIRE(VECMAT_EQ(m.m22, 1.0f / tan_half_fov, EPSILON));
    REQUIRE(VECMAT_EQ(m.m33, -(far + near) / (far - near), EPSILON));
    REQUIRE(VECMAT_EQ(m.m43, -1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(m.m34, -2.0f * far * near / (far - near), EPSILON));
}

TEST_CASE(mat4_ortho_test, "[matrix4]") {
    const matrix4 m = mat4_ortho(
        -VM_F(1.0), VM_F(1.0),
        -VM_F(1.0), VM_F(1.0),
        VM_F(0.1), VM_F(100.0));
    REQUIRE(VECMAT_EQ(m.m11, VM_F(1.0), EPSILON));
    REQUIRE(VECMAT_EQ(m.m22, VM_F(1.0), EPSILON));
    REQUIRE(VECMAT_EQ(m.m33, -VM_F(2.0) / (VM_F(100.0) - VM_F(0.1)), EPSILON));
    REQUIRE(VECMAT_EQ(m.m34, -(VM_F(100.0) + VM_F(0.1)) / (VM_F(100.0) - VM_F(0.1)), EPSILON));
}

TEST_CASE(mat2_determinant_test, "[matrix2]") {
    const matrix2 m = {.m11 = 1.0f, .m21 = 2.0f, .m12 = 3.0f, .m22 = 4.0f};
    const vm_float_t det = mat2_determinant(m);
    REQUIRE(VECMAT_EQ(det, -2.0f, EPSILON));
}

TEST_CASE(mat3_determinant_test, "[matrix3]") {
    const matrix3 m = {
        .m11 = 1.0f, .m21 = 2.0f, .m31 = 3.0f,
        .m12 = 0.0f, .m22 = 1.0f, .m32 = 4.0f,
        .m13 = 5.0f, .m23 = 6.0f, .m33 = 0.0f
    };
    const vm_float_t det = mat3_determinant(m);
    REQUIRE(VECMAT_EQ(det, 1.0f, EPSILON));
}

TEST_CASE(mat4_determinant_test, "[matrix4]") {
    const matrix4 m = mat4_identity();
    const vm_float_t det = mat4_determinant(m);
    REQUIRE(VECMAT_EQ(det, 1.0f, EPSILON));
}

TEST_CASE(mat4_look_at_test, "[matrix4]") {
    const vector3 position = {.x = 0.0f, .y = 0.0f, .z = 5.0f};
    const vector3 target = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
    const vector3 up = {.x = 0.0f, .y = 1.0f, .z = 0.0f};
    const matrix4 m = mat4_look_at(position, target, up);
    // Verify some key elements: the matrix positions the camera correctly with the target at negative Z
    REQUIRE(VECMAT_EQ(m.m33, 1.0f, EPSILON));  // Z-column reflects the view orientation
    REQUIRE(VECMAT_EQ(m.m34, -5.0f, EPSILON)); // Translation component for Z
}

TEST_CASE(mat4_perspective_fov_test, "[matrix4]") {
    const vm_float_t fov = 90.0f;
    const vm_float_t w = 800.0f;
    const vm_float_t h = 600.0f;
    const vm_float_t n = 0.1f;
    const vm_float_t f = 100.0f;
    const matrix4 m = mat4_perspective_fov(fov, w, h, n, f);
    const vm_float_t aspect = w / h;
    const vm_float_t rad = deg_to_rad(fov / 2.0f);
    const vm_float_t tan_half_fov = VECMAT_TAN(rad);
    REQUIRE(VECMAT_EQ(m.m11, 1.0f / (aspect * tan_half_fov), EPSILON));
    REQUIRE(VECMAT_EQ(m.m22, 1.0f / tan_half_fov, EPSILON));
}

TEST_CASE(mat4_perspective_infinite_test, "[matrix4]") {
    const vm_float_t fov_y = 90.0f;
    const vm_float_t aspect = 1.0f;
    const vm_float_t n = 0.1f;
    const matrix4 m = mat4_perspective_infinite(fov_y, aspect, n);
    const vm_float_t rad = deg_to_rad(fov_y / 2.0f);
    const vm_float_t tan_half_fov = VECMAT_TAN(rad);
    REQUIRE(VECMAT_EQ(m.m11, 1.0f / (aspect * tan_half_fov), EPSILON));
    REQUIRE(VECMAT_EQ(m.m22, 1.0f / tan_half_fov, EPSILON));
    REQUIRE(VECMAT_EQ(m.m33, -1.0f, EPSILON));     // For infinite far plane, m33 should be -1
    REQUIRE(VECMAT_EQ(m.m34, -2.0f * n, EPSILON)); // Adjusted for infinite far
}
