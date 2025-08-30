// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "unitest.h"

// vector2i pointer-based function tests
TEST_CASE(vec2i_add_ptr_test, "[vector2i_ptr]") {
    const vector2i a = {.x = 1, .y = 2};
    const vector2i b = {.x = 3, .y = 4};
    vector2i result;
    
    vec2i_add_ptr(&result, &a, &b);
    
    REQUIRE(result.x == 4);
    REQUIRE(result.y == 6);
}

TEST_CASE(vec2i_sub_ptr_test, "[vector2i_ptr]") {
    const vector2i a = {.x = 5, .y = 6};
    const vector2i b = {.x = 3, .y = 2};
    vector2i result;
    
    vec2i_sub_ptr(&result, &a, &b);
    
    REQUIRE(result.x == 2);
    REQUIRE(result.y == 4);
}

TEST_CASE(vec2i_mul_scalar_ptr_test, "[vector2i_ptr]") {
    const vector2i v = {.x = 2, .y = 3};
    vector2i result;
    
    vec2i_mul_scalar_ptr(&result, &v, 4);
    
    REQUIRE(result.x == 8);
    REQUIRE(result.y == 12);
}

TEST_CASE(vec2i_div_scalar_ptr_test, "[vector2i_ptr]") {
    const vector2i v = {.x = 8, .y = 12};
    vector2i result;
    
    vec2i_div_scalar_ptr(&result, &v, 4);
    
    REQUIRE(result.x == 2);
    REQUIRE(result.y == 3);
}

TEST_CASE(vec2i_mul_ptr_test, "[vector2i_ptr]") {
    const vector2i a = {.x = 2, .y = 3};
    const vector2i b = {.x = 4, .y = 5};
    vector2i result;
    
    vec2i_mul_ptr(&result, &a, &b);
    
    REQUIRE(result.x == 8);
    REQUIRE(result.y == 15);
}

TEST_CASE(vec2i_neg_ptr_test, "[vector2i_ptr]") {
    const vector2i v = {.x = 5, .y = -3};
    vector2i result;
    
    vec2i_neg_ptr(&result, &v);
    
    REQUIRE(result.x == -5);
    REQUIRE(result.y == 3);
}

TEST_CASE(vec2i_abs_ptr_test, "[vector2i_ptr]") {
    const vector2i v = {.x = -5, .y = 3};
    vector2i result;
    
    vec2i_abs_ptr(&result, &v);
    
    REQUIRE(result.x == 5);
    REQUIRE(result.y == 3);
}

TEST_CASE(vec2i_min_ptr_test, "[vector2i_ptr]") {
    const vector2i a = {.x = 2, .y = 8};
    const vector2i b = {.x = 5, .y = 3};
    vector2i result;
    
    vec2i_min_ptr(&result, &a, &b);
    
    REQUIRE(result.x == 2);
    REQUIRE(result.y == 3);
}

TEST_CASE(vec2i_max_ptr_test, "[vector2i_ptr]") {
    const vector2i a = {.x = 2, .y = 8};
    const vector2i b = {.x = 5, .y = 3};
    vector2i result;
    
    vec2i_max_ptr(&result, &a, &b);
    
    REQUIRE(result.x == 5);
    REQUIRE(result.y == 8);
}

TEST_CASE(vec2i_sign_ptr_test, "[vector2i_ptr]") {
    const vector2i v = {.x = -5, .y = 0};
    vector2i result;
    
    vec2i_sign_ptr(&result, &v);
    
    REQUIRE(result.x == -1);
    REQUIRE(result.y == 0);
}

TEST_CASE(vec2i_perpendicular_ptr_test, "[vector2i_ptr]") {
    const vector2i v = {.x = 3, .y = 4};
    vector2i result;
    
    vec2i_perpendicular_ptr(&result, &v);
    
    REQUIRE(result.x == -4);
    REQUIRE(result.y == 3);
}

TEST_CASE(vec2i_cross_ptr_test, "[vector2i_ptr]") {
    const vector2i a = {.x = 1, .y = 0};
    const vector2i b = {.x = 0, .y = 1};
    vector2i result;

    vec2i_cross_ptr(&result, &a, &b);

    REQUIRE(result.x == 1);
    REQUIRE(result.y == 0);
}

// vector3i pointer-based function tests
TEST_CASE(vec3i_add_ptr_test, "[vector3i_ptr]") {
    const vector3i a = {.x = 1, .y = 2, .z = 3};
    const vector3i b = {.x = 4, .y = 5, .z = 6};
    vector3i result;
    
    vec3i_add_ptr(&result, &a, &b);
    
    REQUIRE(result.x == 5);
    REQUIRE(result.y == 7);
    REQUIRE(result.z == 9);
}

TEST_CASE(vec3i_cross_ptr_test, "[vector3i_ptr]") {
    const vector3i a = {.x = 1, .y = 0, .z = 0};
    const vector3i b = {.x = 0, .y = 1, .z = 0};
    vector3i result;
    
    vec3i_cross_ptr(&result, &a, &b);
    
    REQUIRE(result.x == 0);
    REQUIRE(result.y == 0);
    REQUIRE(result.z == 1);
}

TEST_CASE(vec3i_lerp_ptr_test, "[vector3i_ptr]") {
    const vector3i a = {.x = 0, .y = 0, .z = 0};
    const vector3i b = {.x = 10, .y = 20, .z = 30};
    vector3i result;
    
    vec3i_lerp_ptr(&result, &a, &b, 0.5f);
    
    REQUIRE(result.x == 5);
    REQUIRE(result.y == 10);
    REQUIRE(result.z == 15);
}

TEST_CASE(vec3i_clamp_ptr_test, "[vector3i_ptr]") {
    const vector3i v = {.x = -5, .y = 15, .z = 8};
    const vector3i min_val = {.x = 0, .y = 0, .z = 0};
    const vector3i max_val = {.x = 10, .y = 10, .z = 10};
    vector3i result;
    
    vec3i_clamp_ptr(&result, &v, &min_val, &max_val);
    
    REQUIRE(result.x == 0);
    REQUIRE(result.y == 10);
    REQUIRE(result.z == 8);
}

// vector4i pointer-based function tests
TEST_CASE(vec4i_add_ptr_test, "[vector4i_ptr]") {
    const vector4i a = {.x = 1, .y = 2, .z = 3, .w = 4};
    const vector4i b = {.x = 5, .y = 6, .z = 7, .w = 8};
    vector4i result;
    
    vec4i_add_ptr(&result, &a, &b);
    
    REQUIRE(result.x == 6);
    REQUIRE(result.y == 8);
    REQUIRE(result.z == 10);
    REQUIRE(result.w == 12);
}

TEST_CASE(vec4i_normalize_ptr_test, "[vector4i_ptr]") {
    const vector4i v = {.x = 5, .y = 0, .z = 0, .w = 0};  // Length = 5.0
    vector4i result;
    
    vec4i_normalize_ptr(&result, &v);
    
    REQUIRE(result.x == 1);
    REQUIRE(result.y == 0);
    REQUIRE(result.z == 0);
    REQUIRE(result.w == 0);
}

TEST_CASE(vec4i_lerp_ptr_test, "[vector4i_ptr]") {
    const vector4i a = {.x = 0, .y = 0, .z = 0, .w = 0};
    const vector4i b = {.x = 10, .y = 20, .z = 30, .w = 40};
    vector4i result;
    
    vec4i_lerp_ptr(&result, &a, &b, 0.25f);
    
    REQUIRE(result.x == 2);
    REQUIRE(result.y == 5);
    REQUIRE(result.z == 7);
    REQUIRE(result.w == 10);
}

TEST_CASE(vec4i_clamp_ptr_test, "[vector4i_ptr]") {
    const vector4i v = {.x = -5, .y = 15, .z = 8, .w = 25};
    const vector4i min_val = {.x = 0, .y = 0, .z = 0, .w = 0};
    const vector4i max_val = {.x = 10, .y = 10, .z = 10, .w = 20};
    vector4i result;
    
    vec4i_clamp_ptr(&result, &v, &min_val, &max_val);
    
    REQUIRE(result.x == 0);
    REQUIRE(result.y == 10);
    REQUIRE(result.z == 8);
    REQUIRE(result.w == 20);
}

// Conversions
TEST_CASE(vec4i_to_vec3i_ptr_test, "[vector4i_ptr]") {
    const vector4i v = {.x = 1, .y = 2, .z = 4, .w = 8};
    const vector3i res = vec4i_to_vec3i(v);
    REQUIRE(vec3i_eq(res, (vector3i){.x = 1, .y = 2, .z = 4}));
}

// In-place operation tests
TEST_CASE(vec2i_in_place_operations_test, "[vector2i_ptr]") {
    vector2i v = {.x = 2, .y = 3};
    const vector2i add_val = {.x = 1, .y = 1};
    
    // Test in-place addition (result and input are same)
    vec2i_add_ptr(&v, &v, &add_val);
    
    REQUIRE(v.x == 3);
    REQUIRE(v.y == 4);
}

TEST_CASE(vec3i_in_place_operations_test, "[vector3i_ptr]") {
    vector3i v = {.x = 1, .y = 2, .z = 3};
    
    // Test in-place scalar multiplication
    vec3i_mul_scalar_ptr(&v, &v, 2);
    
    REQUIRE(v.x == 2);
    REQUIRE(v.y == 4);
    REQUIRE(v.z == 6);
}

// Edge case tests
TEST_CASE(vec_int_ptr_edge_cases_test, "[vector_int_ptr]") {
    // Test division by zero
    const vector2i v = {.x = 10, .y = 20};
    vector2i result;
    vec2i_div_scalar_ptr(&result, &v, 0);
    
    REQUIRE(result.x == 0);
    REQUIRE(result.y == 0);
    
    // Test normalize zero vector
    const vector3i zero_vec = {.x = 0, .y = 0, .z = 0};
    vector3i norm_result;
    vec3i_normalize_ptr(&norm_result, &zero_vec);
    
    REQUIRE(norm_result.x == 0);
    REQUIRE(norm_result.y == 0);
    REQUIRE(norm_result.z == 0);
}

TEST_CASE(vec4i_div_scalar_ptr_edge_case_test, "[vector4i_ptr]") {
    const vector4i v = {.x = 8, .y = 12, .z = 16, .w = 20};
    vector4i result;
    
    // Test division by zero
    vec4i_div_scalar_ptr(&result, &v, 0);
    
    REQUIRE(result.x == 0);
    REQUIRE(result.y == 0);
    REQUIRE(result.z == 0);
    REQUIRE(result.w == 0);
}

TEST_CASE(vec3i_cross_ptr_orthogonal_vectors_test, "[vector3i_ptr]") {
    const vector3i a = {.x = 2, .y = 0, .z = 0};
    const vector3i b = {.x = 0, .y = 3, .z = 0};
    vector3i result;
    
    vec3i_cross_ptr(&result, &a, &b);
    
    REQUIRE(result.x == 0);
    REQUIRE(result.y == 0);
    REQUIRE(result.z == 6);
}

TEST_CASE(vec4i_parity_ptr, "[vector4i_ptr]") {
    const vector4i a = {.x = 8, .y = 9, .z = 10, .w = 12};
    const vector4i b = {.x = 2, .y = 3, .z = 5, .w = 4};
    vector4i res;
    vec4i_div_ptr(&res, &a, &b);
    REQUIRE(vec4i_eq(res, (vector4i){.x = 4, .y = 3, .z = 2, .w = 3}));
    const vector4i n = {.x = -1, .y = 7, .z = 8, .w = -5};
    const vector4i p = {.x = 4, .y = 4, .z = 4, .w = 4};
    vec4i_mod_ptr(&res, &n, &p);
    REQUIRE(vec4i_eq(res, (vector4i){.x = 3, .y = 3, .z = 0, .w = 3}));
    vec4i_div_floor_ptr(&res, &(vector4i){.x = -5, .y = 7, .z = 8, .w = -3}, &(vector4i){.x = 2, .y = 2, .z = 3, .w = 2});
    REQUIRE(vec4i_eq(res, (vector4i){.x = -3, .y = 3, .z = 2, .w = -2}));
    vec4i_add_scalar_ptr(&res, &b, 1);
    REQUIRE(vec4i_eq(res, (vector4i){.x = 3, .y = 4, .z = 6, .w = 5}));
}
