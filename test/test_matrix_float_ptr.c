// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "unitest.h"

// matrix2 ptr tests
TEST_CASE(mat2_identity_ptr_test, "[matrix2_ptr]") {
    matrix2 res;
    mat2_identity_ptr(&res);
    REQUIRE(mat2_eq(res, (matrix2){.m11 = 1.0f, .m21 = 0.0f, .m12 = 0.0f, .m22 = 1.0f}));
}

TEST_CASE(mat2_mul_ptr_test, "[matrix2_ptr]") {
    matrix2 res;
    const matrix2 a = {.m11 = 1.0f, .m21 = 2.0f, .m12 = 3.0f, .m22 = 4.0f};
    const matrix2 b = {.m11 = 5.0f, .m21 = 6.0f, .m12 = 7.0f, .m22 = 8.0f};
    mat2_mul_ptr(&res, &a, &b);
    REQUIRE(mat2_eq(res, (matrix2){.m11 = 19.0f, .m21 = 22.0f, .m12 = 43.0f, .m22 = 50.0f}));
}

TEST_CASE(mat2_transpose_ptr_test, "[matrix2_ptr]") {
    matrix2 res;
    const matrix2 m = {.m11 = 1.0f, .m21 = 2.0f, .m12 = 3.0f, .m22 = 4.0f};
    mat2_transpose_ptr(&res, &m);
    REQUIRE(mat2_eq(res, (matrix2){.m11 = 1.0f, .m21 = 3.0f, .m12 = 2.0f, .m22 = 4.0f}));
}

TEST_CASE(mat2_inverse_ptr_test, "[matrix2_ptr]") {
    matrix2 inv;
    const matrix2 m = {.m11 = 1.0f, .m21 = 2.0f, .m12 = 3.0f, .m22 = 4.0f};
    mat2_inverse_ptr(&inv, &m);
    matrix2 res;
    mat2_mul_ptr(&res, &m, &inv);
    REQUIRE(mat2_eq(res, mat2_identity()));
}

TEST_CASE(mat2_rotation_z_ptr_test, "[matrix2_ptr]") {
    const vm_float_t degrees = 90.0f;
    matrix2 rot;
    mat2_rotation_z_ptr(&rot, degrees);
    // For 90 degrees: cos(90)=0, sin(90)=1
    // Matrix should be: [0, -1; 1, 0]
    REQUIRE(VECMAT_EQ(rot.m11,  0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(rot.m12, -1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(rot.m21,  1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(rot.m22,  0.0f, EPSILON));
}

TEST_CASE(mat2_mul_vec2_ptr_test, "[matrix2_ptr]") {
    vector2 res;
    const matrix2 m = {.m11 = 1.0f, .m21 = 2.0f, .m12 = 3.0f, .m22 = 4.0f};
    const vector2 v = {.x = 1.0f, .y = 1.0f};
    mat2_mul_vec2_ptr(&res, &m, &v);
    REQUIRE(vec2_eq(res, (vector2){.x = 4.0f, .y = 6.0f}));

    const matrix2 id = mat2_identity();
    const vector2 u = {.x = 3.0f, .y = 4.0f};
    mat2_mul_vec2_ptr(&res, &id, &u);
    REQUIRE(vec2_eq(res, u));
}

// matrix3 ptr tests
TEST_CASE(mat3_identity_ptr_test, "[matrix3_ptr]") {
    matrix3 res;
    mat3_identity_ptr(&res);
    REQUIRE(mat3_eq(res, (matrix3){
        .m11 = 1, .m21 = 0, .m31 = 0,
        .m12 = 0, .m22 = 1, .m32 = 0,
        .m13 = 0, .m23 = 0, .m33 = 1
        }));
}

TEST_CASE(mat3_mul_ptr_test, "[matrix3_ptr]") {
    matrix3 res;
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
    mat3_mul_ptr(&res, &a, &b);
    REQUIRE(mat3_eq(res, (matrix3){
        .m11 = 30,  .m21 = 24,  .m31 = 18,
        .m12 = 84,  .m22 = 69,  .m32 = 54,
        .m13 = 138, .m23 = 114, .m33 = 90
        }));
}

TEST_CASE(mat3_transpose_ptr_test, "[matrix3_ptr]") {
    matrix3 res;
    const matrix3 m = {
        .m11 = 1, .m21 = 2, .m31 = 3,
        .m12 = 4, .m22 = 5, .m32 = 6,
        .m13 = 7, .m23 = 8, .m33 = 9
    };
    mat3_transpose_ptr(&res, &m);
    REQUIRE(mat3_eq(res, (matrix3){
        .m11 = 1, .m21 = 4, .m31 = 7,
        .m12 = 2, .m22 = 5, .m32 = 8,
        .m13 = 3, .m23 = 6, .m33 = 9
        }));
}

TEST_CASE(mat3_inverse_ptr_test, "[matrix3_ptr]") {
    // Identity
    matrix3 res;
    const matrix3 m = {
        .m11 = 1, .m21 = 0, .m31 = 0,
        .m12 = 0, .m22 = 1, .m32 = 0,
        .m13 = 0, .m23 = 0, .m33 = 1
    };
    mat3_inverse_ptr(&res, &m);
    REQUIRE(mat3_eq(res, m));

    const matrix3 m2 = {
        .m11 = 1, .m21 = 2, .m31 = 3,
        .m12 = 0, .m22 = 1, .m32 = 4,
        .m13 = 5, .m23 = 6, .m33 = 0
    };
    matrix3 inv2;
    mat3_inverse_ptr(&inv2, &m2);
    matrix3 res2;
    mat3_mul_ptr(&res2, &m2, &inv2);
    REQUIRE(mat3_eq(res2, mat3_identity()));
}

TEST_CASE(mat3_rotation_z_ptr_test, "[matrix3_ptr]") {
    const vm_float_t degrees = 90.0f;
    matrix3 rot;
    mat3_rotation_z_ptr(&rot, degrees);
    // For 90 degrees: cos(90°)=~0, sin(90°)=~1
    // Matrix should be: [0, -1, 0; 1, 0, 0; 0, 0, 1]
    REQUIRE(VECMAT_EQ(rot.m11,  0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(rot.m12, -1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(rot.m13,  0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(rot.m21,  1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(rot.m22,  0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(rot.m23,  0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(rot.m31,  0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(rot.m32,  0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(rot.m33,  1.0f, EPSILON));
}

TEST_CASE(mat3_mul_vec3_ptr_test, "[matrix3_ptr]") {
    vector3 res;
    const matrix3 a = {
        .m11 = 1, .m21 = 2, .m31 = 3,
        .m12 = 4, .m22 = 5, .m32 = 6,
        .m13 = 7, .m23 = 8, .m33 = 9
    };
    const vector3 v = {.x = 1.0f, .y = 0.0f, .z = 0.0f};
    mat3_mul_vec3_ptr(&res, &a, &v);
    REQUIRE(vec3_eq(res, (vector3){.x = 1.0f, .y = 2.0f, .z = 3.0f}));

    const matrix3 id = mat3_identity();
    const vector3 u = {.x = 2.0f, .y = 3.0f, .z = 4.0f};
    mat3_mul_vec3_ptr(&res, &id, &u);
    REQUIRE(vec3_eq(res, u));
}

TEST_CASE(mat3_mul_vec2_ptr_test, "[matrix3_ptr]") {
    vector2 res;
    matrix3 m = mat3_identity();
    m.m13 = 5.0f;
    m.m23 = 7.0f;
    const vector2 v = {.x = 1.0f, .y = 2.0f};
    mat3_mul_vec2_ptr(&res, &m, &v);
    REQUIRE(vec2_eq(res, (vector2){.x = 6.0f, .y = 9.0f}));
}

// matrix4 ptr tests
TEST_CASE(mat4_identity_ptr_test, "[matrix4_ptr]") {
    matrix4 res;
    mat4_identity_ptr(&res);
    REQUIRE(mat4_eq(res, (matrix4){
        .m11 = 1, .m21 = 0, .m31 = 0, .m41 = 0,
        .m12 = 0, .m22 = 1, .m32 = 0, .m42 = 0,
        .m13 = 0, .m23 = 0, .m33 = 1, .m43 = 0,
        .m14 = 0, .m24 = 0, .m34 = 0, .m44 = 1
        }));
}

TEST_CASE(mat4_mul_ptr_test, "[matrix4_ptr]") {
    matrix4 res;
    const matrix4 a = mat4_identity();
    const matrix4 b = mat4_identity();
    mat4_mul_ptr(&res, &a, &b);
    REQUIRE(mat4_eq(res, mat4_identity()));
}

TEST_CASE(mat4_transpose_ptr_test, "[matrix4_ptr]") {
    matrix4 res;
    const matrix4 m = {
        .m11 = 1,  .m21 = 2,  .m31 = 3,  .m41 = 4,
        .m12 = 5,  .m22 = 6,  .m32 = 7,  .m42 = 8,
        .m13 = 9,  .m23 = 10, .m33 = 11, .m43 = 12,
        .m14 = 13, .m24 = 14, .m34 = 15, .m44 = 16
    };
    mat4_transpose_ptr(&res, &m);
    REQUIRE(mat4_eq(res, (matrix4){
        .m11 = 1, .m21 = 5, .m31 = 9,  .m41 = 13,
        .m12 = 2, .m22 = 6, .m32 = 10, .m42 = 14,
        .m13 = 3, .m23 = 7, .m33 = 11, .m43 = 15,
        .m14 = 4, .m24 = 8, .m34 = 12, .m44 = 16
        }));
}

TEST_CASE(mat4_inverse_ptr_test, "[matrix4_ptr]") {
    matrix4 res;
    const matrix4 m = mat4_identity();
    mat4_inverse_ptr(&res, &m);
    REQUIRE(mat4_eq(res, m));

    // Test a translation matrix
    const vector3 v = {.x = 1,.y = 2,.z = 3};
    matrix4 trans;
    mat4_translate_ptr(&trans, &v);
    matrix4 inv_trans;
    mat4_inverse_ptr(&inv_trans, &trans);
    matrix4 res2;
    mat4_mul_ptr(&res2, &trans, &inv_trans);
    REQUIRE(mat4_eq(res2, mat4_identity()));
}

TEST_CASE(mat4_translate_ptr_test, "[matrix4_ptr]") {
    matrix4 res;
    const vector3 v = {.x = 1.0f, .y = 2.0f, .z = 3.0f};
    mat4_translate_ptr(&res, &v);
    REQUIRE(VECMAT_EQ(res.m14, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(res.m24, 2.0f, EPSILON));
    REQUIRE(VECMAT_EQ(res.m34, 3.0f, EPSILON));
    REQUIRE(VECMAT_EQ(res.m44, 1.0f, EPSILON));
}

TEST_CASE(mat4_scale_ptr_test, "[matrix4_ptr]") {
    matrix4 res;
    const vector3 v = {.x = 2.0f, .y = 3.0f, .z = 4.0f};
    mat4_scale_ptr(&res, &v);
    REQUIRE(VECMAT_EQ(res.m11, 2.0f, EPSILON));
    REQUIRE(VECMAT_EQ(res.m22, 3.0f, EPSILON));
    REQUIRE(VECMAT_EQ(res.m33, 4.0f, EPSILON));
    REQUIRE(VECMAT_EQ(res.m44, 1.0f, EPSILON));
}

TEST_CASE(mat4_rotation_ptr_test, "[matrix4_ptr]") {
    matrix4 res;
    const vector3 axis = {.x = 0.0f, .y = 0.0f, .z = 1.0f};
    mat4_rotation_ptr(&res, &axis, 90.0f);
    REQUIRE(VECMAT_EQ(res.m11, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(res.m21, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(res.m12, -1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(res.m22, 0.0f, EPSILON));

    vector3 rotated;
    const vector3 p = {.x = 1.0f, .y = 0.0f, .z = 0.0f};
    mat4_mul_vec3_ptr(&rotated, &res, &p, 1.0f);
    REQUIRE(vec3_near(rotated, (vector3){.x = 0.0f, .y = 1.0f, .z = 0.0f}, EPSILON));
}

TEST_CASE(mat4_mul_vec4_ptr_test, "[matrix4_ptr]") {
    vector4 res;
    const matrix4 id = mat4_identity();
    const vector4 v = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f};
    mat4_mul_vec4_ptr(&res, &id, &v);
    REQUIRE(vec4_eq(res, v));

    const matrix4 s = mat4_scale((vector3){.x = 2.0f, .y = 3.0f, .z = 4.0f});
    const vector4 u = {.x = 1.0f, .y = 1.0f, .z = 1.0f, .w = 1.0f};
    mat4_mul_vec4_ptr(&res, &s, &u);
    REQUIRE(vec4_eq(res, (vector4){.x = 2.0f, .y = 3.0f, .z = 4.0f, .w = 1.0f}));
}

TEST_CASE(mat4_mul_vec3_ptr_test, "[matrix4_ptr]") {
    vector3 res;
    const matrix4 t = mat4_translate((vector3){.x = 1.0f, .y = 2.0f, .z = 3.0f});
    const vector3 origin = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
    mat4_mul_vec3_ptr(&res, &t, &origin, 1.0f);
    REQUIRE(vec3_eq(res, (vector3){.x = 1.0f, .y = 2.0f, .z = 3.0f}));

    const vector3 dir = {.x = 4.0f, .y = 5.0f, .z = 6.0f};
    mat4_mul_vec3_ptr(&res, &t, &dir, 0.0f);
    REQUIRE(vec3_eq(res, dir));
}

TEST_CASE(mat4_perspective_ptr_test, "[matrix4_ptr]") {
    matrix4 res;
    const vm_float_t fov = 90.0f;
    const vm_float_t aspect = 1.0f;
    const vm_float_t near = 0.1f;
    const vm_float_t far = 100.0f;
    mat4_perspective_ptr(&res, fov, aspect, near, far);
    const vm_float_t rad = deg_to_rad(fov / 2.0f);
    const vm_float_t tan_half_fov = VECMAT_TAN(rad);
    REQUIRE(VECMAT_EQ(res.m11, 1.0f / (aspect * tan_half_fov), EPSILON));
    REQUIRE(VECMAT_EQ(res.m22, 1.0f / tan_half_fov, EPSILON));
    REQUIRE(VECMAT_EQ(res.m33, -(far + near) / (far - near), EPSILON));
    REQUIRE(VECMAT_EQ(res.m43, -1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(res.m34, -2.0f * far * near / (far - near), EPSILON));
}

TEST_CASE(mat4_ortho_ptr_test, "[matrix4_ptr]") {
    matrix4 res;
    mat4_ortho_ptr(&res,
        -VM_F(1.0), VM_F(1.0),
        -VM_F(1.0), VM_F(1.0),
         VM_F(0.1), VM_F(100.0));
    REQUIRE(VECMAT_EQ(res.m11, VM_F(1.0), EPSILON));
    REQUIRE(VECMAT_EQ(res.m22, VM_F(1.0), EPSILON));
    REQUIRE(VECMAT_EQ(res.m33, -VM_F(2.0) / (VM_F(100.0) - VM_F(0.1)), EPSILON));
    REQUIRE(VECMAT_EQ(res.m34, -(VM_F(100.0) + VM_F(0.1)) / (VM_F(100.0) - VM_F(0.1)), EPSILON));
}

TEST_CASE(mat4_look_at_ptr_test, "[matrix4_ptr]") {
    matrix4 res;
    const vector3 position = {.x = 0.0f, .y = 0.0f, .z = 5.0f};
    const vector3 target = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
    const vector3 up = {.x = 0.0f, .y = 1.0f, .z = 0.0f};
    mat4_look_at_ptr(&res, &position, &target, &up);
    REQUIRE(VECMAT_EQ(res.m33, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(res.m34, -5.0f, EPSILON));
}

TEST_CASE(mat4_perspective_fov_ptr_test, "[matrix4_ptr]") {
    matrix4 res;
    const vm_float_t fov = 90.0f;
    const vm_float_t w = 800.0f;
    const vm_float_t h = 600.0f;
    const vm_float_t n = 0.1f;
    const vm_float_t f = 100.0f;
    mat4_perspective_fov_ptr(&res, fov, w, h, n, f);
    const vm_float_t aspect = w / h;
    const vm_float_t rad = deg_to_rad(fov / 2.0f);
    const vm_float_t tan_half_fov = VECMAT_TAN(rad);
    REQUIRE(VECMAT_EQ(res.m11, 1.0f / (aspect * tan_half_fov), EPSILON));
    REQUIRE(VECMAT_EQ(res.m22, 1.0f / tan_half_fov, EPSILON));
}

TEST_CASE(mat4_perspective_infinite_ptr_test, "[matrix4_ptr]") {
    matrix4 res;
    const vm_float_t fov_y = 90.0f;
    const vm_float_t aspect = 1.0f;
    const vm_float_t n = 0.1f;
    mat4_perspective_infinite_ptr(&res, fov_y, aspect, n);
    const vm_float_t rad = deg_to_rad(fov_y / 2.0f);
    const vm_float_t tan_half_fov = VECMAT_TAN(rad);
    REQUIRE(VECMAT_EQ(res.m11, 1.0f / (aspect * tan_half_fov), EPSILON));
    REQUIRE(VECMAT_EQ(res.m22, 1.0f / tan_half_fov, EPSILON));
    REQUIRE(VECMAT_EQ(res.m33, -1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(res.m34, -2.0f * n, EPSILON));
}

TEST_CASE(mat2_scale_from_mat3_ptr_test, "[matrix2_ptr]") {
    matrix2 s;
    const vector2 sv = {.x = 2.0f, .y = 3.0f};
    mat2_scale_ptr(&s, &sv);
    REQUIRE(vec2_eq(mat2_mul_vec2(s, (vector2){.x = 1.0f, .y = 1.0f}), (vector2){.x = 2.0f, .y = 3.0f}));
    matrix3 rot;
    mat3_rotation_z_ptr(&rot, 90.0f);
    matrix2 r;
    mat2_from_mat3_ptr(&r, &rot);
    REQUIRE(vec2_near(mat2_mul_vec2(r, (vector2){.x = 1.0f, .y = 0.0f}), (vector2){.x = 0.0f, .y = 1.0f}, EPSILON));
}

TEST_CASE(mat3_affine_ptr_test, "[matrix3_ptr]") {
    matrix3 t;
    const vector2 tv = {.x = 5.0f, .y = 7.0f};
    mat3_translate_ptr(&t, &tv);
    REQUIRE(vec2_eq(mat3_mul_vec2(t, (vector2){.x = 1.0f, .y = 2.0f}), (vector2){.x = 6.0f, .y = 9.0f}));
    matrix3 rx;
    mat3_rotation_x_ptr(&rx, 90.0f);
    REQUIRE(vec3_near(mat3_mul_vec3(rx, (vector3){.x = 0.0f, .y = 1.0f, .z = 0.0f}),
                      (vector3){.x = 0.0f, .y = 0.0f, .z = 1.0f}, EPSILON));
    const matrix4 m4 = mat4_scale((vector3){.x = 2.0f, .y = 3.0f, .z = 4.0f});
    matrix3 m3;
    mat3_from_mat4_ptr(&m3, &m4);
    REQUIRE(VECMAT_EQ(m3.m11, 2.0f, EPSILON));
    REQUIRE(VECMAT_EQ(m3.m22, 3.0f, EPSILON));
    REQUIRE(VECMAT_EQ(m3.m33, 4.0f, EPSILON));
}

TEST_CASE(mat4_trs_extract_ptr_test, "[matrix4_ptr]") {
    matrix4 rot;
    mat4_rotation_y_ptr(&rot, 90.0f);
    REQUIRE(vec3_near(mat4_mul_vec3(rot, (vector3){.x = 0.0f, .y = 0.0f, .z = 1.0f}, 1.0f),
                      (vector3){.x = 1.0f, .y = 0.0f, .z = 0.0f}, EPSILON));

    const vector3 t = {.x = 1.0f, .y = 2.0f, .z = 3.0f};
    const vector3 s = {.x = 2.0f, .y = 3.0f, .z = 4.0f};
    const quaternion q = quat_identity();
    matrix4 m;
    mat4_trs_ptr(&m, &t, &q, &s);
    vector3 et, es;
    quaternion er;
    mat4_extract_translation_ptr(&et, &m);
    mat4_extract_scale_ptr(&es, &m);
    mat4_extract_rotation_ptr(&er, &m);
    REQUIRE(vec3_eq(et, t));
    REQUIRE(vec3_near(es, s, EPSILON));
    REQUIRE(quat_near(er, q, EPSILON) || quat_near(er, (quaternion){.x = -q.x, .y = -q.y, .z = -q.z, .w = -q.w}, EPSILON));
}
