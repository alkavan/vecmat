// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "unitest.h"

// Vector2 pointer-based tests
TEST_CASE(vec2_add_ptr_test, "[vector2_ptr]") {
    const vector2 a = {.x = 1.0f, .y = 2.0f};
    const vector2 b = {.x = 3.0f, .y = 4.0f};
    vector2 result;
    
    vec2_add_ptr(&result, &a, &b);
    
    REQUIRE(VECMAT_EQ(result.x, 4.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 6.0f, EPSILON));
}

TEST_CASE(vec2_sub_ptr_test, "[vector2_ptr]") {
    const vector2 a = {.x = 5.0f, .y = 8.0f};
    const vector2 b = {.x = 2.0f, .y = 3.0f};
    vector2 result;
    
    vec2_sub_ptr(&result, &a, &b);
    
    REQUIRE(VECMAT_EQ(result.x, 3.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 5.0f, EPSILON));
}

TEST_CASE(vec2_mul_scalar_ptr_test, "[vector2_ptr]") {
    const vector2 v = {.x = 2.0f, .y = 3.0f};
    const vm_float_t s = 2.5f;
    vector2 result;
    
    vec2_mul_scalar_ptr(&result, &v, s);
    
    REQUIRE(VECMAT_EQ(result.x, 5.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 7.5f, EPSILON));
}

TEST_CASE(vec2_div_scalar_ptr_test, "[vector2_ptr]") {
    const vector2 v = {.x = 10.0f, .y = 15.0f};
    const vm_float_t s = 5.0f;
    vector2 result;
    
    vec2_div_scalar_ptr(&result, &v, s);
    
    REQUIRE(VECMAT_EQ(result.x, 2.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 3.0f, EPSILON));
}

TEST_CASE(vec2_mul_ptr_test, "[vector2_ptr]") {
    const vector2 a = {.x = 2.0f, .y = 3.0f};
    const vector2 b = {.x = 4.0f, .y = 5.0f};
    vector2 result;
    
    vec2_mul_ptr(&result, &a, &b);
    
    REQUIRE(VECMAT_EQ(result.x, 8.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 15.0f, EPSILON));
}

TEST_CASE(vec2_normalize_ptr_test, "[vector2_ptr]") {
    const vector2 v = {.x = VM_F(3.0), .y = VM_F(4.0)};  // Length = 5.0
    vector2 result;

    vec2_normalize_ptr(&result, &v);

    REQUIRE(VECMAT_EQ(result.x, VM_F(3.0) / VM_F(5.0), EPSILON));
    REQUIRE(VECMAT_EQ(result.y, VM_F(4.0) / VM_F(5.0), EPSILON));
}

TEST_CASE(vec2_min_max_ptr_test, "[vector2_ptr]") {
    const vector2 a = {.x = 1.0f, .y = 5.0f};
    const vector2 b = {.x = 3.0f, .y = 2.0f};
    vector2 min_result, max_result;
    
    vec2_min_ptr(&min_result, &a, &b);
    vec2_max_ptr(&max_result, &a, &b);
    
    REQUIRE(VECMAT_EQ(min_result.x, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(min_result.y, 2.0f, EPSILON));
    REQUIRE(VECMAT_EQ(max_result.x, 3.0f, EPSILON));
    REQUIRE(VECMAT_EQ(max_result.y, 5.0f, EPSILON));
}

TEST_CASE(vec2_neg_ptr_test, "[vector2_ptr]") {
    const vector2 v = {.x = 1.0f, .y = -2.0f};
    vector2 result;

    vec2_neg_ptr(&result, &v);

    REQUIRE(VECMAT_EQ(result.x, -1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 2.0f, EPSILON));
}

TEST_CASE(vec2_abs_ptr_test, "[vector2_ptr]") {
    const vector2 v = {.x = -1.0f, .y = 2.0f};
    vector2 result;

    vec2_abs_ptr(&result, &v);

    REQUIRE(VECMAT_EQ(result.x, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 2.0f, EPSILON));
}

TEST_CASE(vec2_sign_ptr_test, "[vector2_ptr]") {
    const vector2 v = {.x = 1.0f, .y = -2.0f};
    vector2 result;

    vec2_sign_ptr(&result, &v);

    REQUIRE(VECMAT_EQ(result.x, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, -1.0f, EPSILON));
}

TEST_CASE(vec2_floor_ptr_test, "[vector2_ptr]") {
    const vector2 v = {.x = 1.9f, .y = -1.1f};
    vector2 result;

    vec2_floor_ptr(&result, &v);

    REQUIRE(VECMAT_EQ(result.x, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, -2.0f, EPSILON));
}

TEST_CASE(vec2_ceil_ptr_test, "[vector2_ptr]") {
    const vector2 v = {.x = 1.1f, .y = -1.9f};
    vector2 result;

    vec2_ceil_ptr(&result, &v);

    REQUIRE(VECMAT_EQ(result.x, 2.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, -1.0f, EPSILON));
}

TEST_CASE(vec2_round_ptr_test, "[vector2_ptr]") {
    const vector2 v = {.x = 1.3f, .y = 1.7f};
    vector2 result;

    vec2_round_ptr(&result, &v);

    REQUIRE(VECMAT_EQ(result.x, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 2.0f, EPSILON));
}

TEST_CASE(vec2_perpendicular_ptr_test, "[vector2_ptr]") {
    const vector2 v = {.x = 1.0f, .y = 2.0f};
    vector2 result;

    vec2_perpendicular_ptr(&result, &v);

    REQUIRE(VECMAT_EQ(result.x, -2.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 1.0f, EPSILON));
}

TEST_CASE(vec2_cross_ptr_test, "[vector2_ptr]") {
    const vector2 a = {.x = 1.0f, .y = 0.0f};
    const vector2 b = {.x = 0.0f, .y = 1.0f};
    vector2 result;

    vec2_cross_ptr(&result, &a, &b);

    REQUIRE(VECMAT_EQ(result.x, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 0.0f, EPSILON));
}

TEST_CASE(vec2_scale_ptr_test, "[vector2_ptr]") {
    const vector2 v = {.x = 1.0f, .y = 2.0f};
    vector2 result;

    vec2_scale_ptr(&result, &v, 3.0f);

    REQUIRE(VECMAT_EQ(result.x, 3.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 6.0f, EPSILON));
}

TEST_CASE(vec2_reflect_ptr_test, "[vector2_ptr]") {
    const vector2 v = {.x = 1.0f, .y = -1.0f};
    const vector2 normal = {.x = 0.0f, .y = 1.0f};
    vector2 result;

    vec2_reflect_ptr(&result, &v, &normal);

    REQUIRE(VECMAT_EQ(result.x, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 1.0f, EPSILON));

    // Length preserved
    REQUIRE(VECMAT_EQ(vec2_length(result), vec2_length(v), EPSILON));

    // Normal component sign flipped
    REQUIRE(VECMAT_EQ(vec2_dot(result, normal), -vec2_dot(v, normal), EPSILON));
}

TEST_CASE(vec2_project_ptr_test, "[vector2_ptr]") {
    const vector2 a = {.x = 3.0f, .y = 4.0f};
    const vector2 b = {.x = 1.0f, .y = 0.0f};
    vector2 result;

    vec2_project_ptr(&result, &a, &b);

    REQUIRE(VECMAT_EQ(result.x, 3.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 0.0f, EPSILON));
}

TEST_CASE(vec2_tangent_ptr_test, "[vector2_ptr]") {
    const vector2 v = {.x = 3.0f, .y = 4.0f};
    vector2 result;

    vec2_tangent_ptr(&result, &v);

    REQUIRE(VECMAT_EQ(result.x, 4.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, -3.0f, EPSILON));
    // Length preserved
    REQUIRE(VECMAT_EQ(vec2_length(result), vec2_length(v), EPSILON));
    // Perpendicular to input
    REQUIRE(VECMAT_EQ(vec2_dot(result, v), 0.0f, EPSILON));
}

TEST_CASE(vec2_rotate_ptr_test, "[vector2_ptr]") {
    const vector2 v = {.x = 1.0f, .y = 0.0f};
    const vm_float_t angle = (vm_float_t)M_PI_2;
    vector2 result;

    vec2_rotate_ptr(&result, &v, angle);

    REQUIRE(VECMAT_EQ(result.x, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 1.0f, EPSILON));
    // Length preserved
    REQUIRE(VECMAT_EQ(vec2_length(result), 1.0f, EPSILON));
}

TEST_CASE(vec2_slide_ptr_test, "[vector2_ptr]") {
    const vector2 v = {.x = 1.0f, .y = -1.0f};
    const vector2 normal = {.x = 0.0f, .y = 1.0f};
    vector2 result;

    vec2_slide_ptr(&result, &v, &normal);

    REQUIRE(VECMAT_EQ(result.x, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 0.0f, EPSILON));

    // Perpendicular to normal
    REQUIRE(VECMAT_EQ(vec2_dot(result, normal), 0.0f, EPSILON));
}

TEST_CASE(vec2_clamp_ptr_test, "[vector2_ptr]") {
    const vector2 v = {.x = -5.0f, .y = 15.0f};
    const vector2 min = {.x = 0.0f, .y = 0.0f};
    const vector2 max = {.x = 10.0f, .y = 10.0f};
    vector2 result;

    vec2_clamp_ptr(&result, &v, &min, &max);

    REQUIRE(VECMAT_EQ(result.x, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 10.0f, EPSILON));
}

// Vector3 pointer-based tests
TEST_CASE(vec3_add_ptr_test, "[vector3_ptr]") {
    const vector3 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f};
    const vector3 b = {.x = 4.0f, .y = 5.0f, .z = 6.0f};
    vector3 result;
    
    vec3_add_ptr(&result, &a, &b);
    
    REQUIRE(VECMAT_EQ(result.x, 5.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 7.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, 9.0f, EPSILON));
}

TEST_CASE(vec3_cross_ptr_test, "[vector3_ptr]") {
    const vector3 a = {.x = 1.0f, .y = 0.0f, .z = 0.0f};
    const vector3 b = {.x = 0.0f, .y = 1.0f, .z = 0.0f};
    vector3 result;
    
    vec3_cross_ptr(&result, &a, &b);
    
    REQUIRE(VECMAT_EQ(result.x, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, 1.0f, EPSILON));
}

TEST_CASE(vec3_normalize_ptr_test, "[vector3_ptr]") {
    const vector3 v = {.x = VM_F(3.0), .y = VM_F(4.0), .z = VM_F(0.0)};  // Length = 5.0
    vector3 result;

    vec3_normalize_ptr(&result, &v);

    REQUIRE(VECMAT_EQ(result.x, VM_F(3.0) / VM_F(5.0), EPSILON));
    REQUIRE(VECMAT_EQ(result.y, VM_F(4.0) / VM_F(5.0), EPSILON));
    REQUIRE(VECMAT_EQ(result.z, VM_F(0.0), EPSILON));
}

TEST_CASE(vec3_lerp_ptr_test, "[vector3_ptr]") {
    const vector3 a = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
    const vector3 b = {.x = 10.0f, .y = 20.0f, .z = 30.0f};
    vector3 result;
    
    vec3_lerp_ptr(&result, &a, &b, 0.5f);
    
    REQUIRE(VECMAT_EQ(result.x, 5.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 10.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, 15.0f, EPSILON));
}

TEST_CASE(vec3_clamp_ptr_test, "[vector3_ptr]") {
    const vector3 v = {.x = -5.0f, .y = 15.0f, .z = 5.0f};
    const vector3 min = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
    const vector3 max = {.x = 10.0f, .y = 10.0f, .z = 10.0f};
    vector3 result;
    
    vec3_clamp_ptr(&result, &v, &min, &max);
    
    REQUIRE(VECMAT_EQ(result.x, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 10.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, 5.0f, EPSILON));
}

TEST_CASE(vec3_sub_ptr_test, "[vector3_ptr]") {
    const vector3 a = {.x = 5.0f, .y = 6.0f, .z = 7.0f};
    const vector3 b = {.x = 1.0f, .y = 2.0f, .z = 3.0f};
    vector3 result;
    
    vec3_sub_ptr(&result, &a, &b);
    
    REQUIRE(VECMAT_EQ(result.x, 4.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 4.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, 4.0f, EPSILON));
}

TEST_CASE(vec3_mul_scalar_ptr_test, "[vector3_ptr]") {
    const vector3 v = {.x = 2.0f, .y = 3.0f, .z = 4.0f};
    const vm_float_t s = 2.0f;
    vector3 result;
    
    vec3_mul_scalar_ptr(&result, &v, s);
    
    REQUIRE(VECMAT_EQ(result.x, 4.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 6.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, 8.0f, EPSILON));
}

TEST_CASE(vec3_div_scalar_ptr_test, "[vector3_ptr]") {
    const vector3 v = {.x = 10.0f, .y = 20.0f, .z = 30.0f};
    const vm_float_t s = 2.0f;
    vector3 result;
    
    vec3_div_scalar_ptr(&result, &v, s);
    
    REQUIRE(VECMAT_EQ(result.x, 5.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 10.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, 15.0f, EPSILON));
}

TEST_CASE(vec3_mul_ptr_test, "[vector3_ptr]") {
    const vector3 a = {.x = 2.0f, .y = 3.0f, .z = 4.0f};
    const vector3 b = {.x = 1.0f, .y = 2.0f, .z = 3.0f};
    vector3 result;
    
    vec3_mul_ptr(&result, &a, &b);
    
    REQUIRE(VECMAT_EQ(result.x, 2.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 6.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, 12.0f, EPSILON));
}

TEST_CASE(vec3_neg_ptr_test, "[vector3_ptr]") {
    const vector3 v = {.x = 1.0f, .y = -2.0f, .z = 3.0f};
    vector3 result;
    
    vec3_neg_ptr(&result, &v);
    
    REQUIRE(VECMAT_EQ(result.x, -1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 2.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, -3.0f, EPSILON));
}

TEST_CASE(vec3_abs_ptr_test, "[vector3_ptr]") {
    const vector3 v = {.x = -1.0f, .y = 2.0f, .z = -3.0f};
    vector3 result;
    
    vec3_abs_ptr(&result, &v);
    
    REQUIRE(VECMAT_EQ(result.x, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 2.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, 3.0f, EPSILON));
}

TEST_CASE(vec3_min_max_ptr_test, "[vector3_ptr]") {
    const vector3 a = {.x = 1.0f, .y = 5.0f, .z = 3.0f};
    const vector3 b = {.x = 2.0f, .y = 4.0f, .z = 6.0f};
    vector3 min_result, max_result;
    
    vec3_min_ptr(&min_result, &a, &b);
    vec3_max_ptr(&max_result, &a, &b);
    
    REQUIRE(VECMAT_EQ(min_result.x, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(min_result.y, 4.0f, EPSILON));
    REQUIRE(VECMAT_EQ(min_result.z, 3.0f, EPSILON));
    REQUIRE(VECMAT_EQ(max_result.x, 2.0f, EPSILON));
    REQUIRE(VECMAT_EQ(max_result.y, 5.0f, EPSILON));
    REQUIRE(VECMAT_EQ(max_result.z, 6.0f, EPSILON));
}

TEST_CASE(vec3_sign_ptr_test, "[vector3_ptr]") {
    const vector3 v = {.x = 1.0f, .y = -2.0f, .z = 0.0f};
    vector3 result;
    
    vec3_sign_ptr(&result, &v);
    
    REQUIRE(VECMAT_EQ(result.x, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, -1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, 0.0f, EPSILON));
}

TEST_CASE(vec3_floor_ptr_test, "[vector3_ptr]") {
    const vector3 v = {.x = 1.9f, .y = 2.1f, .z = -1.9f};
    vector3 result;
    
    vec3_floor_ptr(&result, &v);
    
    REQUIRE(VECMAT_EQ(result.x, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 2.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, -2.0f, EPSILON));
}

TEST_CASE(vec3_ceil_ptr_test, "[vector3_ptr]") {
    const vector3 v = {.x = 1.1f, .y = 2.9f, .z = -1.1f};
    vector3 result;
    
    vec3_ceil_ptr(&result, &v);
    
    REQUIRE(VECMAT_EQ(result.x, 2.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 3.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, -1.0f, EPSILON));
}

TEST_CASE(vec3_round_ptr_test, "[vector3_ptr]") {
    const vector3 v = {.x = 1.3f, .y = 1.7f, .z = -1.3f};
    vector3 result;
    
    vec3_round_ptr(&result, &v);
    
    REQUIRE(VECMAT_EQ(result.x, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 2.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, -1.0f, EPSILON));
}

TEST_CASE(vec3_reflect_ptr_test, "[vector3_ptr]") {
    const vector3 incident = {.x = 1.0f, .y = -1.0f, .z = 0.0f};
    const vector3 normal = {.x = 0.0f, .y = 1.0f, .z = 0.0f};
    vector3 result;
    
    vec3_reflect_ptr(&result, &incident, &normal);
    
    REQUIRE(VECMAT_EQ(result.x, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, 0.0f, EPSILON));
}

TEST_CASE(vec3_refract_ptr_test, "[vector3_ptr]") {
    const vector3 incident = {.x = 0.0f, .y = -1.0f, .z = 0.0f};
    const vector3 normal = {.x = 0.0f, .y = 1.0f, .z = 0.0f};
    const vm_float_t eta = 1.0f;
    vector3 result;
    
    vec3_refract_ptr(&result, &incident, &normal, eta);
    
    REQUIRE(VECMAT_EQ(result.x, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, -1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, 0.0f, EPSILON));
}

// Vector4 pointer-based tests
TEST_CASE(vec4_add_ptr_test, "[vector4_ptr]") {
    const vector4 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f};
    const vector4 b = {.x = 5.0f, .y = 6.0f, .z = 7.0f, .w = 8.0f};
    vector4 result;
    
    vec4_add_ptr(&result, &a, &b);
    
    REQUIRE(VECMAT_EQ(result.x, 6.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 8.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, 10.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.w, 12.0f, EPSILON));
}

TEST_CASE(vec4_sub_ptr_test, "[vector4_ptr]") {
    const vector4 a = {.x = 5.0f, .y = 6.0f, .z = 7.0f, .w = 8.0f};
    const vector4 b = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f};
    vector4 result;
    
    vec4_sub_ptr(&result, &a, &b);
    
    REQUIRE(VECMAT_EQ(result.x, 4.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 4.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, 4.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.w, 4.0f, EPSILON));
}

TEST_CASE(vec4_mul_scalar_ptr_test, "[vector4_ptr]") {
    const vector4 v = {.x = 2.0f, .y = 3.0f, .z = 4.0f, .w = 5.0f};
    const vm_float_t s = 2.0f;
    vector4 result;
    
    vec4_mul_scalar_ptr(&result, &v, s);
    
    REQUIRE(VECMAT_EQ(result.x, 4.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 6.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, 8.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.w, 10.0f, EPSILON));
}

TEST_CASE(vec4_div_scalar_ptr_test, "[vector4_ptr]") {
    const vector4 v = {.x = 10.0f, .y = 20.0f, .z = 30.0f, .w = 40.0f};
    const vm_float_t s = 2.0f;
    vector4 result;
    
    vec4_div_scalar_ptr(&result, &v, s);
    
    REQUIRE(VECMAT_EQ(result.x, 5.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 10.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, 15.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.w, 20.0f, EPSILON));
}

TEST_CASE(vec4_mul_ptr_test, "[vector4_ptr]") {
    const vector4 a = {.x = 2.0f, .y = 3.0f, .z = 4.0f, .w = 5.0f};
    const vector4 b = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f};
    vector4 result;
    
    vec4_mul_ptr(&result, &a, &b);
    
    REQUIRE(VECMAT_EQ(result.x, 2.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 6.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, 12.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.w, 20.0f, EPSILON));
}

TEST_CASE(vec4_neg_ptr_test, "[vector4_ptr]") {
    const vector4 v = {.x = 1.0f, .y = -2.0f, .z = 3.0f, .w = -4.0f};
    vector4 result;
    
    vec4_neg_ptr(&result, &v);
    
    REQUIRE(VECMAT_EQ(result.x, -1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 2.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, -3.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.w, 4.0f, EPSILON));
}

TEST_CASE(vec4_abs_ptr_test, "[vector4_ptr]") {
    const vector4 v = {.x = -1.0f, .y = 2.0f, .z = -3.0f, .w = 4.0f};
    vector4 result;
    
    vec4_abs_ptr(&result, &v);
    
    REQUIRE(VECMAT_EQ(result.x, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 2.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, 3.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.w, 4.0f, EPSILON));
}

TEST_CASE(vec4_min_max_ptr_test, "[vector4_ptr]") {
    const vector4 a = {.x = 1.0f, .y = 5.0f, .z = 3.0f, .w = 7.0f};
    const vector4 b = {.x = 2.0f, .y = 4.0f, .z = 6.0f, .w = 8.0f};
    vector4 min_result, max_result;
    
    vec4_min_ptr(&min_result, &a, &b);
    vec4_max_ptr(&max_result, &a, &b);
    
    REQUIRE(VECMAT_EQ(min_result.x, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(min_result.y, 4.0f, EPSILON));
    REQUIRE(VECMAT_EQ(min_result.z, 3.0f, EPSILON));
    REQUIRE(VECMAT_EQ(min_result.w, 7.0f, EPSILON));
    REQUIRE(VECMAT_EQ(max_result.x, 2.0f, EPSILON));
    REQUIRE(VECMAT_EQ(max_result.y, 5.0f, EPSILON));
    REQUIRE(VECMAT_EQ(max_result.z, 6.0f, EPSILON));
    REQUIRE(VECMAT_EQ(max_result.w, 8.0f, EPSILON));
}

TEST_CASE(vec4_sign_ptr_test, "[vector4_ptr]") {
    const vector4 v = {.x = 1.0f, .y = -2.0f, .z = 0.0f, .w = 3.0f};
    vector4 result;
    
    vec4_sign_ptr(&result, &v);
    
    REQUIRE(VECMAT_EQ(result.x, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, -1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.w, 1.0f, EPSILON));
}

TEST_CASE(vec4_floor_ptr_test, "[vector4_ptr]") {
    const vector4 v = {.x = 1.9f, .y = 2.1f, .z = -1.9f, .w = -2.1f};
    vector4 result;
    
    vec4_floor_ptr(&result, &v);
    
    REQUIRE(VECMAT_EQ(result.x, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 2.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, -2.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.w, -3.0f, EPSILON));
}

TEST_CASE(vec4_ceil_ptr_test, "[vector4_ptr]") {
    const vector4 v = {.x = 1.1f, .y = 2.9f, .z = -1.1f, .w = -2.9f};
    vector4 result;
    
    vec4_ceil_ptr(&result, &v);
    
    REQUIRE(VECMAT_EQ(result.x, 2.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 3.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, -1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.w, -2.0f, EPSILON));
}

TEST_CASE(vec4_normalize_ptr_test, "[vector4_ptr]") {
    const vector4 v = {.x = 2.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f};  // Length = 2.0
    vector4 result;
    
    vec4_normalize_ptr(&result, &v);
    
    REQUIRE(VECMAT_EQ(result.x, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.w, 0.0f, EPSILON));
}

TEST_CASE(vec4_homogenize_ptr_test, "[vector4_ptr]") {
    const vector4 v = {.x = 4.0f, .y = 8.0f, .z = 12.0f, .w = 2.0f};
    vector4 result;
    
    vec4_homogenize_ptr(&result, &v);
    
    REQUIRE(VECMAT_EQ(result.x, 2.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 4.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, 6.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.w, 1.0f, EPSILON));
}

TEST_CASE(vec4_lerp_ptr_test, "[vector4_ptr]") {
    const vector4 a = {.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f};
    const vector4 b = {.x = 10.0f, .y = 20.0f, .z = 30.0f, .w = 40.0f};
    vector4 result;
    
    vec4_lerp_ptr(&result, &a, &b, 0.25f);
    
    REQUIRE(VECMAT_EQ(result.x, 2.5f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 5.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, 7.5f, EPSILON));
    REQUIRE(VECMAT_EQ(result.w, 10.0f, EPSILON));
}

TEST_CASE(vec4_clamp_ptr_test, "[vector4_ptr]") {
    const vector4 v = {.x = -2.0f, .y = 12.0f, .z = 5.0f, .w = -1.0f};
    const vector4 min = {.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f};
    const vector4 max = {.x = 10.0f, .y = 10.0f, .z = 10.0f, .w = 10.0f};
    vector4 result;
    
    vec4_clamp_ptr(&result, &v, &min, &max);
    
    REQUIRE(VECMAT_EQ(result.x, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 10.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, 5.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.w, 0.0f, EPSILON));
}

TEST_CASE(vec4_round_ptr_test, "[vector4_ptr]") {
    const vector4 v = {.x = 1.3f, .y = 1.7f, .z = -1.3f, .w = -1.7f};
    vector4 result;
    
    vec4_round_ptr(&result, &v);
    
    REQUIRE(VECMAT_EQ(result.x, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 2.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.z, -1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.w, -2.0f, EPSILON));
}

// Conversions
TEST_CASE(vec4_to_vec3_ptr, "[vector4_ptr]") {
    const vector4 v = {.x = 0.5f, .y = 1.0f, .z = 1.5f, .w = 3.0f};
    const vector3 res = vec4_to_vec3(v);
    REQUIRE(vec3_eq(res, (vector3){.x = 0.5f, .y = 1.0f, .z = 1.5f}));
}

TEST_CASE(vec4_parity_ptr, "[vector4_ptr]") {
    const vector4 a = {.x = 2.0f, .y = 4.0f, .z = 6.0f, .w = 8.0f};
    const vector4 b = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f};
    vector4 res;
    vec4_div_ptr(&res, &a, &b);
    REQUIRE(vec4_eq(res, (vector4){.x = 2.0f, .y = 2.0f, .z = 2.0f, .w = 2.0f}));
    vec4_add_scalar_ptr(&res, &b, 1.0f);
    REQUIRE(vec4_eq(res, (vector4){.x = 2.0f, .y = 3.0f, .z = 4.0f, .w = 5.0f}));
    vec4_project_ptr(&res, &a, &b);
    REQUIRE(vec4_eq(res, a));
    vec4_reject_ptr(&res, &a, &b);
    REQUIRE(vec4_eq(res, vec4_zero()));
    const vector4 f = {.x = 1.25f, .y = -1.25f, .z = 2.0f, .w = 0.5f};
    vec4_fract_ptr(&res, &f);
    REQUIRE(vec4_eq(res, (vector4){.x = 0.25f, .y = 0.75f, .z = 0.0f, .w = 0.5f}));
    const vector4 v = {.x = 1.0f, .y = 1.0f, .z = 0.0f, .w = 0.0f};
    const vector4 n = {.x = 0.0f, .y = 1.0f, .z = 0.0f, .w = 0.0f};
    vec4_slide_ptr(&res, &v, &n);
    REQUIRE(vec4_eq(res, (vector4){.x = 1.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f}));
}

// In-place operation tests
TEST_CASE(vec2_in_place_operations, "[vector2_ptr]") {
    vector2 v = {.x = 2.0f, .y = 3.0f};
    const vector2 add_val = {.x = 1.0f, .y = 1.0f};
    
    // Test in-place addition (result and input are the same)
    vec2_add_ptr(&v, &v, &add_val);
    
    REQUIRE(VECMAT_EQ(v.x, 3.0f, EPSILON));
    REQUIRE(VECMAT_EQ(v.y, 4.0f, EPSILON));
}

TEST_CASE(vec3_in_place_operations, "[vector3_ptr]") {
    vector3 v = {.x = 1.0f, .y = 2.0f, .z = 3.0f};
    
    // Test in-place scalar multiplication
    vec3_mul_scalar_ptr(&v, &v, 2.0f);
    
    REQUIRE(VECMAT_EQ(v.x, 2.0f, EPSILON));
    REQUIRE(VECMAT_EQ(v.y, 4.0f, EPSILON));
    REQUIRE(VECMAT_EQ(v.z, 6.0f, EPSILON));
}

// Edge case tests
TEST_CASE(vec_ptr_edge_cases, "[vector_ptr]") {
    // Test division by zero
    const vector2 v2 = {.x = 5.0f, .y = 10.0f};
    vector2 result2;
    vec2_div_scalar_ptr(&result2, &v2, 0.0f);
    REQUIRE(vec2_eq(result2, v2)); // Should return the original vector
    
    // Test normalization of a zero vector
    const vector3 zero3 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
    vector3 result3;
    vec3_normalize_ptr(&result3, &zero3);
    REQUIRE(vec3_eq(result3, zero3)); // Should return a zero vector
    
    // Test homogenization with zero w
    const vector4 v4_zero_w = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 0.0f};
    vector4 result4;
    vec4_homogenize_ptr(&result4, &v4_zero_w);
    REQUIRE(VECMAT_EQ(result4.x, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result4.y, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result4.z, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result4.w, 0.0f, EPSILON));
}
