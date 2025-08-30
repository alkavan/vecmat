// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "unitest.h"

// vector2i tests
TEST_CASE(vec2i_add_test, "[vector2i]") {
    const vector2i a = {.x = 1, .y = 2};
    const vector2i b = {.x = 3, .y = 4};
    const vector2i res = vec2i_add(a, b);
    REQUIRE(vec2i_eq(res, (vector2i){.x = 4, .y = 6}));
}

TEST_CASE(vec2i_sub_test, "[vector2i]") {
    const vector2i a = {.x = 5, .y = 6};
    const vector2i b = {.x = 3, .y = 4};
    const vector2i res = vec2i_sub(a, b);
    REQUIRE(vec2i_eq(res, (vector2i){.x = 2, .y = 2}));
}

TEST_CASE(vec2i_mul_scalar_test, "[vector2i]") {
    const vector2i v = {.x = 1, .y = 2};
    const vector2i res = vec2i_mul_scalar(v, 3);
    REQUIRE(vec2i_eq(res, (vector2i){.x = 3, .y = 6}));
}

TEST_CASE(vec2i_div_scalar_test, "[vector2i]") {
    const vector2i v = {.x = 10, .y = 20};
    const vector2i res = vec2i_div_scalar(v, 2);
    REQUIRE(vec2i_eq(res, (vector2i){.x = 5, .y = 10}));
}

TEST_CASE(vec2i_mul_test, "[vector2i]") {
    const vector2i a = {.x = 1, .y = 2};
    const vector2i b = {.x = 3, .y = 4};
    const vector2i res = vec2i_mul(a, b);
    REQUIRE(vec2i_eq(res, (vector2i){.x = 3, .y = 8}));
}

TEST_CASE(vec2i_neg_test, "[vector2i]") {
    const vector2i v = {.x = 1, .y = -2};
    const vector2i res = vec2i_neg(v);
    REQUIRE(vec2i_eq(res, (vector2i){.x = -1, .y = 2}));
}

TEST_CASE(vec2i_abs_test, "[vector2i]") {
    const vector2i v = {.x = 1, .y = -2};
    const vector2i res = vec2i_abs(v);
    REQUIRE(vec2i_eq(res, (vector2i){.x = 1, .y = 2}));
}

TEST_CASE(vec2i_zero_test, "[vector2i]") {
    const vector2i res = vec2i_zero();
    REQUIRE(vec2i_eq(res, (vector2i){.x = 0, .y = 0}));
}

TEST_CASE(vec2i_one_test, "[vector2i]") {
    const vector2i res = vec2i_one();
    REQUIRE(vec2i_eq(res, (vector2i){.x = 1, .y = 1}));
}

TEST_CASE(vec2i_x_axis_test, "[vector2i]") {
    const vector2i res = vec2i_x_axis(5);
    REQUIRE(vec2i_eq(res, (vector2i){.x = 5, .y = 0}));
}

TEST_CASE(vec2i_y_axis_test, "[vector2i]") {
    const vector2i res = vec2i_y_axis(7);
    REQUIRE(vec2i_eq(res, (vector2i){.x = 0, .y = 7}));
}

TEST_CASE(vec2i_x_scale_test, "[vector2i]") {
    const vector2i res = vec2i_x_scale(3);
    REQUIRE(vec2i_eq(res, (vector2i){.x = 3, .y = 1}));
}

TEST_CASE(vec2i_y_scale_test, "[vector2i]") {
    const vector2i res = vec2i_y_scale(4);
    REQUIRE(vec2i_eq(res, (vector2i){.x = 1, .y = 4}));
}

TEST_CASE(vec2i_dot_test, "[vector2i]") {
    const vector2i a = {.x = 1, .y = 2};
    const vector2i b = {.x = 3, .y = 4};
    const vm_int_t res = vec2i_dot(a, b);
    REQUIRE(res == 11);
}

TEST_CASE(vec2i_cross_test, "[vector2i]") {
    const vector2i a = {.x = 1, .y = 2};
    const vector2i b = {.x = 3, .y = 4};
    const vector2i res = vec2i_cross(a, b);
    REQUIRE(vec2i_eq(res, (vector2i){.x = -2, .y = 0}));
}

TEST_CASE(vec2i_length_test, "[vector2i]") {
    const vector2i v = {.x = 3, .y = 4};
    const vm_float_t res = vec2i_length(v);
    REQUIRE(VECMAT_EQ(res, 5.0f, EPSILON));
}

TEST_CASE(vec2i_normalize_test, "[vector2i]") {
    const vector2i v = {.x = 5, .y = 0};
    const vector2i res = vec2i_normalize(v);
    REQUIRE(VECMAT_EQ(vec2i_length(res), 1.0f, EPSILON));
}

TEST_CASE(vec2i_distance_test, "[vector2i]") {
    const vector2i a = {.x = 0, .y = 0};
    const vector2i b = {.x = 3, .y = 4};
    const vm_float_t res = vec2i_distance(a, b);
    REQUIRE(VECMAT_EQ(res, 5.0f, EPSILON));
}

TEST_CASE(vec2i_angle_test, "[vector2i]") {
    const vector2i a = {.x = 1, .y = 0};
    const vector2i b = {.x = 0, .y = 1};
    const vm_float_t res = vec2i_angle(a, b);
    REQUIRE(VECMAT_EQ(res, (vm_float_t)M_PI_2, EPSILON));
}

TEST_CASE(vec2i_min_test, "[vector2i]") {
    const vector2i a = {.x = 1, .y = 5};
    const vector2i b = {.x = 3, .y = 2};
    const vector2i res = vec2i_min(a, b);
    REQUIRE(vec2i_eq(res, (vector2i){.x = 1, .y = 2}));
}

TEST_CASE(vec2i_max_test, "[vector2i]") {
    const vector2i a = {.x = 1, .y = 5};
    const vector2i b = {.x = 3, .y = 2};
    const vector2i res = vec2i_max(a, b);
    REQUIRE(vec2i_eq(res, (vector2i){.x = 3, .y = 5}));
}

TEST_CASE(vec2i_sign_test, "[vector2i]") {
    const vector2i v = {.x = 1, .y = -2};
    const vector2i res = vec2i_sign(v);
    REQUIRE(vec2i_eq(res, (vector2i){.x = 1, .y = -1}));
}

TEST_CASE(vec2i_perpendicular_test, "[vector2i]") {
    const vector2i v = {.x = 3, .y = 4};
    const vector2i res = vec2i_perpendicular(v);
    REQUIRE(vec2i_eq(res, (vector2i){.x = -4, .y = 3}));
}

TEST_CASE(vec2i_aspect_ratio_test, "[vector2i]") {
    const vector2i v = {.x = 8, .y = 4};
    const vm_float_t res = vec2i_aspect_ratio(v);
    REQUIRE(VECMAT_EQ(res, 2.0f, EPSILON));
}

// vector3i tests
TEST_CASE(vec3i_add_test, "[vector3i]") {
    const vector3i a = {.x = 1, .y = 2, .z = 3};
    const vector3i b = {.x = 4, .y = 5, .z = 6};
    const vector3i res = vec3i_add(a, b);
    REQUIRE(vec3i_eq(res, (vector3i){.x = 5, .y = 7, .z = 9}));
}

TEST_CASE(vec3i_sub_test, "[vector3i]") {
    const vector3i a = {.x = 5, .y = 6, .z = 7};
    const vector3i b = {.x = 2, .y = 3, .z = 4};
    const vector3i res = vec3i_sub(a, b);
    REQUIRE(vec3i_eq(res, (vector3i){.x = 3, .y = 3, .z = 3}));
}

TEST_CASE(vec3i_mul_scalar_test, "[vector3i]") {
    const vector3i v = {.x = 1, .y = 2, .z = 3};
    const vector3i res = vec3i_mul_scalar(v, 2);
    REQUIRE(vec3i_eq(res, (vector3i){.x = 2, .y = 4, .z = 6}));
}

TEST_CASE(vec3i_div_scalar_test, "[vector3i]") {
    const vector3i v = {.x = 10, .y = 20, .z = 30};
    const vector3i res = vec3i_div_scalar(v, 2);
    REQUIRE(vec3i_eq(res, (vector3i){.x = 5, .y = 10, .z = 15}));
}

TEST_CASE(vec3i_mul_test, "[vector3i]") {
    const vector3i a = {.x = 1, .y = 2, .z = 3};
    const vector3i b = {.x = 4, .y = 5, .z = 6};
    const vector3i res = vec3i_mul(a, b);
    REQUIRE(vec3i_eq(res, (vector3i){.x = 4, .y = 10, .z = 18}));
}

TEST_CASE(vec3i_neg_test, "[vector3i]") {
    const vector3i v = {.x = 1, .y = -2, .z = 3};
    const vector3i res = vec3i_neg(v);
    REQUIRE(vec3i_eq(res, (vector3i){.x = -1, .y = 2, .z = -3}));
}

TEST_CASE(vec3i_abs_test, "[vector3i]") {
    const vector3i v = {.x = 1, .y = -2, .z = -3};
    const vector3i res = vec3i_abs(v);
    REQUIRE(vec3i_eq(res, (vector3i){.x = 1, .y = 2, .z = 3}));
}

TEST_CASE(vec3i_zero_test, "[vector3i]") {
    const vector3i res = vec3i_zero();
    REQUIRE(vec3i_eq(res, (vector3i){.x = 0, .y = 0, .z = 0}));
}

TEST_CASE(vec3i_one_test, "[vector3i]") {
    const vector3i res = vec3i_one();
    REQUIRE(vec3i_eq(res, (vector3i){.x = 1, .y = 1, .z = 1}));
}

TEST_CASE(vec3i_x_axis_test, "[vector3i]") {
    const vector3i res = vec3i_x_axis(5);
    REQUIRE(vec3i_eq(res, (vector3i){.x = 5, .y = 0, .z = 0}));
}

TEST_CASE(vec3i_y_axis_test, "[vector3i]") {
    const vector3i res = vec3i_y_axis(7);
    REQUIRE(vec3i_eq(res, (vector3i){.x = 0, .y = 7, .z = 0}));
}

TEST_CASE(vec3i_z_axis_test, "[vector3i]") {
    const vector3i res = vec3i_z_axis(9);
    REQUIRE(vec3i_eq(res, (vector3i){.x = 0, .y = 0, .z = 9}));
}

TEST_CASE(vec3i_x_scale_test, "[vector3i]") {
    const vector3i res = vec3i_x_scale(3);
    REQUIRE(vec3i_eq(res, (vector3i){.x = 3, .y = 1, .z = 1}));
}

TEST_CASE(vec3i_y_scale_test, "[vector3i]") {
    const vector3i res = vec3i_y_scale(4);
    REQUIRE(vec3i_eq(res, (vector3i){.x = 1, .y = 4, .z = 1}));
}

TEST_CASE(vec3i_z_scale_test, "[vector3i]") {
    const vector3i res = vec3i_z_scale(5);
    REQUIRE(vec3i_eq(res, (vector3i){.x = 1, .y = 1, .z = 5}));
}

TEST_CASE(vec3i_dot_test, "[vector3i]") {
    const vector3i a = {.x = 1, .y = 2, .z = 3};
    const vector3i b = {.x = 4, .y = 5, .z = 6};
    const vm_int_t res = vec3i_dot(a, b);
    REQUIRE(res == 32);
}

TEST_CASE(vec3i_cross_test, "[vector3i]") {
    const vector3i a = {.x = 1, .y = 0, .z = 0};
    const vector3i b = {.x = 0, .y = 1, .z = 0};
    const vector3i res = vec3i_cross(a, b);
    REQUIRE(vec3i_eq(res, (vector3i){.x = 0, .y = 0, .z = 1}));
}

TEST_CASE(vec3i_length_test, "[vector3i]") {
    const vector3i v = {.x = 3, .y = 4, .z = 0};
    const vm_float_t res = vec3i_length(v);
    REQUIRE(VECMAT_EQ(res, 5.0f, EPSILON));
}

TEST_CASE(vec3i_normalize_test, "[vector3i]") {
    const vector3i v = {.x = 5, .y = 0, .z = 0};
    const vector3i res = vec3i_normalize(v);
    REQUIRE(VECMAT_EQ(vec3i_length(res), 1.0f, EPSILON));
}

TEST_CASE(vec3i_distance_test, "[vector3i]") {
    const vector3i a = {.x = 0, .y = 0, .z = 0};
    const vector3i b = {.x = 3, .y = 4, .z = 0};
    const vm_float_t res = vec3i_distance(a, b);
    REQUIRE(VECMAT_EQ(res, 5.0f, EPSILON));
}

TEST_CASE(vec3i_angle_test, "[vector3i]") {
    const vector3i a = {.x = 1, .y = 0, .z = 0};
    const vector3i b = {.x = 0, .y = 1, .z = 0};
    const vm_float_t res = vec3i_angle(a, b);
    REQUIRE(VECMAT_EQ(res, (vm_float_t)M_PI_2, EPSILON));
}

TEST_CASE(vec3i_min_test, "[vector3i]") {
    const vector3i a = {.x = 1, .y = 5, .z = 3};
    const vector3i b = {.x = 3, .y = 2, .z = 4};
    const vector3i res = vec3i_min(a, b);
    REQUIRE(vec3i_eq(res, (vector3i){.x = 1, .y = 2, .z = 3}));
}

TEST_CASE(vec3i_max_test, "[vector3i]") {
    const vector3i a = {.x = 1, .y = 5, .z = 3};
    const vector3i b = {.x = 3, .y = 2, .z = 4};
    const vector3i res = vec3i_max(a, b);
    REQUIRE(vec3i_eq(res, (vector3i){.x = 3, .y = 5, .z = 4}));
}

TEST_CASE(vec3i_sign_test, "[vector3i]") {
    const vector3i v = {.x = 1, .y = -2, .z = 0};
    const vector3i res = vec3i_sign(v);
    REQUIRE(vec3i_eq(res, (vector3i){.x = 1, .y = -1, .z = 0}));
}

TEST_CASE(vec3i_lerp_test, "[vector3i]") {
    const vector3i a = {.x = 0, .y = 0, .z = 0};
    const vector3i b = {.x = 10, .y = 20, .z = 30};
    const vector3i res = vec3i_lerp(a, b, 0.5f);
    REQUIRE(vec3i_eq(res, (vector3i){.x = 5, .y = 10, .z = 15}));
}

TEST_CASE(vec3i_clamp_test, "[vector3i]") {
    const vector3i v = {.x = 5, .y = -5, .z = 15};
    const vector3i min = {.x = 0, .y = 0, .z = 0};
    const vector3i max = {.x = 10, .y = 10, .z = 10};
    const vector3i res = vec3i_clamp(v, min, max);
    REQUIRE(vec3i_eq(res, (vector3i){.x = 5, .y = 0, .z = 10}));
}

// vector4i tests
TEST_CASE(vec4i_add_test, "[vector4i]") {
    const vector4i a = {.x = 1, .y = 2, .z = 3, .w = 4};
    const vector4i b = {.x = 5, .y = 6, .z = 7, .w = 8};
    const vector4i res = vec4i_add(a, b);
    REQUIRE(vec4i_eq(res, (vector4i){.x = 6, .y = 8, .z = 10, .w = 12}));
}

TEST_CASE(vec4i_sub_test, "[vector4i]") {
    const vector4i a = {.x = 5, .y = 6, .z = 7, .w = 8};
    const vector4i b = {.x = 1, .y = 2, .z = 3, .w = 4};
    const vector4i res = vec4i_sub(a, b);
    REQUIRE(vec4i_eq(res, (vector4i){.x = 4, .y = 4, .z = 4, .w = 4}));
}

TEST_CASE(vec4i_mul_scalar_test, "[vector4i]") {
    const vector4i v = {.x = 1, .y = 2, .z = 3, .w = 4};
    const vector4i res = vec4i_mul_scalar(v, 2);
    REQUIRE(vec4i_eq(res, (vector4i){.x = 2, .y = 4, .z = 6, .w = 8}));
}

TEST_CASE(vec4i_div_scalar_test, "[vector4i]") {
    const vector4i v = {.x = 10, .y = 20, .z = 30, .w = 40};
    const vector4i res = vec4i_div_scalar(v, 2);
    REQUIRE(vec4i_eq(res, (vector4i){.x = 5, .y = 10, .z = 15, .w = 20}));
}

TEST_CASE(vec4i_mul_test, "[vector4i]") {
    const vector4i a = {.x = 1, .y = 2, .z = 3, .w = 4};
    const vector4i b = {.x = 5, .y = 6, .z = 7, .w = 8};
    const vector4i res = vec4i_mul(a, b);
    REQUIRE(vec4i_eq(res, (vector4i){.x = 5, .y = 12, .z = 21, .w = 32}));
}

TEST_CASE(vec4i_neg_test, "[vector4i]") {
    const vector4i v = {.x = 1, .y = -2, .z = 3, .w = -4};
    const vector4i res = vec4i_neg(v);
    REQUIRE(vec4i_eq(res, (vector4i){.x = -1, .y = 2, .z = -3, .w = 4}));
}

TEST_CASE(vec4i_abs_test, "[vector4i]") {
    const vector4i v = {.x = 1, .y = -2, .z = -3, .w = 4};
    const vector4i res = vec4i_abs(v);
    REQUIRE(vec4i_eq(res, (vector4i){.x = 1, .y = 2, .z = 3, .w = 4}));
}

TEST_CASE(vec4i_zero_test, "[vector4i]") {
    const vector4i res = vec4i_zero();
    REQUIRE(vec4i_eq(res, (vector4i){.x = 0, .y = 0, .z = 0, .w = 0}));
}

TEST_CASE(vec4i_one_test, "[vector4i]") {
    const vector4i res = vec4i_one();
    REQUIRE(vec4i_eq(res, (vector4i){.x = 1, .y = 1, .z = 1, .w = 1}));
}

TEST_CASE(vec4i_x_axis_test, "[vector4i]") {
    const vector4i res = vec4i_x_axis(5);
    REQUIRE(vec4i_eq(res, (vector4i){.x = 5, .y = 0, .z = 0, .w = 0}));
}

TEST_CASE(vec4i_y_axis_test, "[vector4i]") {
    const vector4i res = vec4i_y_axis(7);
    REQUIRE(vec4i_eq(res, (vector4i){.x = 0, .y = 7, .z = 0, .w = 0}));
}

TEST_CASE(vec4i_z_axis_test, "[vector4i]") {
    const vector4i res = vec4i_z_axis(9);
    REQUIRE(vec4i_eq(res, (vector4i){.x = 0, .y = 0, .z = 9, .w = 0}));
}

TEST_CASE(vec4i_w_axis_test, "[vector4i]") {
    const vector4i res = vec4i_w_axis(11);
    REQUIRE(vec4i_eq(res, (vector4i){.x = 0, .y = 0, .z = 0, .w = 11}));
}

TEST_CASE(vec4i_x_scale_test, "[vector4i]") {
    const vector4i res = vec4i_x_scale(3);
    REQUIRE(vec4i_eq(res, (vector4i){.x = 3, .y = 1, .z = 1, .w = 1}));
}

TEST_CASE(vec4i_y_scale_test, "[vector4i]") {
    const vector4i res = vec4i_y_scale(4);
    REQUIRE(vec4i_eq(res, (vector4i){.x = 1, .y = 4, .z = 1, .w = 1}));
}

TEST_CASE(vec4i_z_scale_test, "[vector4i]") {
    const vector4i res = vec4i_z_scale(5);
    REQUIRE(vec4i_eq(res, (vector4i){.x = 1, .y = 1, .z = 5, .w = 1}));
}

TEST_CASE(vec4i_w_scale_test, "[vector4i]") {
    const vector4i res = vec4i_w_scale(6);
    REQUIRE(vec4i_eq(res, (vector4i){.x = 1, .y = 1, .z = 1, .w = 6}));
}

TEST_CASE(vec4i_dot_test, "[vector4i]") {
    const vector4i a = {.x = 1, .y = 2, .z = 3, .w = 4};
    const vector4i b = {.x = 5, .y = 6, .z = 7, .w = 8};
    const vm_int_t res = vec4i_dot(a, b);
    REQUIRE(res == 70);
}

TEST_CASE(vec4i_length_test, "[vector4i]") {
    const vector4i v = {.x = 1, .y = 2, .z = 2, .w = 4};
    const vm_float_t res = vec4i_length(v);
    REQUIRE(VECMAT_EQ(res, 5.0f, EPSILON));
}

TEST_CASE(vec4i_normalize_test, "[vector4i]") {
    const vector4i v = {.x = 5, .y = 0, .z = 0, .w = 0};
    const vector4i res = vec4i_normalize(v);
    REQUIRE(VECMAT_EQ(vec4i_length(res), 1.0f, EPSILON));
}

TEST_CASE(vec4i_distance_test, "[vector4i]") {
    const vector4i a = {.x = 0, .y = 0, .z = 0, .w = 0};
    const vector4i b = {.x = 1, .y = 2, .z = 2, .w = 4};
    const vm_float_t res = vec4i_distance(a, b);
    REQUIRE(VECMAT_EQ(res, 5.0f, EPSILON));
}

TEST_CASE(vec4i_min_test, "[vector4i]") {
    const vector4i a = {.x = 1, .y = 5, .z = 3, .w = 7};
    const vector4i b = {.x = 3, .y = 2, .z = 4, .w = 6};
    const vector4i res = vec4i_min(a, b);
    REQUIRE(vec4i_eq(res, (vector4i){.x = 1, .y = 2, .z = 3, .w = 6}));
}

TEST_CASE(vec4i_max_test, "[vector4i]") {
    const vector4i a = {.x = 1, .y = 5, .z = 3, .w = 7};
    const vector4i b = {.x = 3, .y = 2, .z = 4, .w = 6};
    const vector4i res = vec4i_max(a, b);
    REQUIRE(vec4i_eq(res, (vector4i){.x = 3, .y = 5, .z = 4, .w = 7}));
}

TEST_CASE(vec4i_sign_test, "[vector4i]") {
    const vector4i v = {.x = 1, .y = -2, .z = 0, .w = -1};
    const vector4i res = vec4i_sign(v);
    REQUIRE(vec4i_eq(res, (vector4i){.x = 1, .y = -1, .z = 0, .w = -1}));
}

TEST_CASE(vec4i_lerp_test, "[vector4i]") {
    const vector4i a = {.x = 0, .y = 0, .z = 0, .w = 0};
    const vector4i b = {.x = 10, .y = 20, .z = 30, .w = 40};
    const vector4i res = vec4i_lerp(a, b, 0.5f);
    REQUIRE(vec4i_eq(res, (vector4i){.x = 5, .y = 10, .z = 15, .w = 20}));
}

TEST_CASE(vec4i_clamp_test, "[vector4i]") {
    const vector4i v = {.x = 5, .y = -5, .z = 15, .w = 25};
    const vector4i min = {.x = 0, .y = 0, .z = 0, .w = 0};
    const vector4i max = {.x = 10, .y = 10, .z = 10, .w = 20};
    const vector4i res = vec4i_clamp(v, min, max);
    REQUIRE(vec4i_eq(res, (vector4i){.x = 5, .y = 0, .z = 10, .w = 20}));
}

// Conversions
TEST_CASE(vec4i_to_vec3i_test, "[vector4i]") {
    const vector4i v = {.x = 2, .y = 4, .z = 6, .w = 2};
    const vector3i res = vec4i_to_vec3i(v);
    REQUIRE(vec3i_eq(res, (vector3i){.x = 2, .y = 4, .z = 6}));
}

TEST_CASE(vec4i_parity, "[vector4i]") {
    REQUIRE(
        vec4i_eq(vec4i_div(
            (vector4i){.x = 8, .y = 9, .z = 10, .w = 12},(vector4i){.x = 2, .y = 3, .z = 5, .w = 4}),
            (vector4i){.x = 4, .y = 3, .z = 2, .w = 3}));
    REQUIRE(
        vec4i_eq(vec4i_add_scalar((vector4i){.x = 1, .y = 2, .z = 3, .w = 4}, 1),
            (vector4i){.x = 2, .y = 3, .z = 4, .w = 5}));
    REQUIRE(
        vec4i_eq(vec4i_mod((vector4i){.x = -1, .y = 7, .z = 8, .w = -5},
            (vector4i){.x = 4, .y = 4, .z = 4, .w = 4}), (vector4i){.x = 3, .y = 3, .z = 0, .w = 3}));
    REQUIRE(
        vec4i_eq(vec4i_wrap(
            (vector4i){.x = -1, .y = 8, .z = 4, .w = 9},(vector4i){.x = 4, .y = 4, .z = 4, .w = 4}
            ),(vector4i){.x = 3, .y = 0, .z = 0, .w = 1}));
    REQUIRE(vec4i_length_squared((vector4i){.x = 1, .y = 2, .z = 2, .w = 4}) == 25);
    REQUIRE(vec4i_length_manhattan((vector4i){.x = -1, .y = 2, .z = -3, .w = 4}) == 10);
    REQUIRE(vec4i_length_chebyshev((vector4i){.x = -1, .y = 2, .z = -3, .w = 4}) == 4);
    REQUIRE(vec4i_is_zero(vec4i_zero()));
    REQUIRE(vec4i_eq(vec4i_splat(7), (vector4i){.x = 7, .y = 7, .z = 7, .w = 7}));
    REQUIRE(
        vec4i_eq(vec4i_div_floor((vector4i){.x = -5, .y = 7, .z = 8, .w = -3}, (vector4i){.x = 2, .y = 2, .z = 3, .w = 2
            }), (vector4i){.x = -3, .y = 3, .z = 2, .w = -2}));
    REQUIRE(
        vec4i_distance_squared((vector4i){.x = 1, .y = 2, .z = 3, .w = 4}, (vector4i){.x = 1, .y = 2, .z = 3, .w = 4})
        == 0);
    REQUIRE(vec4i_min_component((vector4i){.x = 4, .y = -1, .z = 3, .w = 2}) == -1);
    REQUIRE(vec4i_max_component((vector4i){.x = 4, .y = -1, .z = 3, .w = 2}) == 4);
    REQUIRE(vec4i_sum((vector4i){.x = 1, .y = 2, .z = 3, .w = 4}) == 10);
}

TEST_CASE(vec2i_lerp_clamp, "[vector2i]") {
    REQUIRE(vec2i_eq(vec2i_lerp((vector2i){.x = 0, .y = 0}, (vector2i){.x = 10, .y = 20}, 0.5f),
        (vector2i){.x = 5, .y = 10}));
    REQUIRE(vec2i_eq(vec2i_clamp((vector2i){.x = -2, .y = 9},
        (vector2i){.x = 0, .y = 0}, (vector2i){.x = 5, .y = 5}), (vector2i){.x = 0, .y = 5}));
}

TEST_CASE(vec2i_mod_wrap, "[vector2i]") {
    REQUIRE(vec2i_eq(vec2i_mod((vector2i){.x = -1, .y = 7},
        (vector2i){.x = 4, .y = 4}), (vector2i){.x = 3, .y = 3}));
    REQUIRE(vec2i_eq(vec2i_wrap((vector2i){.x = -1, .y = 8}, (vector2i){.x = 4, .y = 4}),
        (vector2i){.x = 3, .y = 0}));
    REQUIRE(vec2i_eq(vec2i_div_floor((vector2i){.x = -5, .y = 7}, (vector2i){.x = 2, .y = 2}),
        (vector2i){.x = -3, .y = 3}));
}

TEST_CASE(vec2i_length_metrics, "[vector2i]") {
    REQUIRE(vec2i_length_squared((vector2i){.x = 3, .y = 4}) == 25);
    REQUIRE(vec2i_length_manhattan((vector2i){.x = -3, .y = 4}) == 7);
    REQUIRE(vec2i_length_chebyshev((vector2i){.x = -3, .y = 4}) == 4);
    REQUIRE(vec2i_cross_scalar((vector2i){.x = 1, .y = 2}, (vector2i){.x = 3, .y = 4}) == -2);
}

TEST_CASE(vec2i_normalize_to_vec2, "[vector2i]") {
    const vector2 n = vec2i_normalize_to_vec2((vector2i){.x = 3, .y = 4});
    REQUIRE(VECMAT_EQ(vec2_length(n), 1.0f, EPSILON));
}

TEST_CASE(vec3i_grid, "[vector3i]") {
    REQUIRE(vec3i_eq(vec3i(1, 2, 3), (vector3i){.x = 1, .y = 2, .z = 3}));
    REQUIRE(vec3i_eq(vec3i_from_vec2i((vector2i){.x = 1, .y = 2}, 3), (vector3i){.x = 1, .y = 2, .z = 3}));
    REQUIRE(vec2i_eq(vec3i_xy((vector3i){.x = 1, .y = 2, .z = 3}), (vector2i){.x = 1, .y = 2}));
    REQUIRE(vec3i_length_manhattan((vector3i){.x = -1, .y = 2, .z = -3}) == 6);
    REQUIRE(vec3i_is_zero(vec3i_zero()));
}

TEST_CASE(vec3i_from_float, "[vector3i]") {
    const vector3 v = {.x = -1.7f, .y = 2.3f, .z = 3.5f};
    REQUIRE(vec3i_eq(vec3i_from_floored(&v), (vector3i){.x = -2, .y = 2, .z = 3}));
    REQUIRE(vec3i_eq(vec3i_from_rounded(&v), (vector3i){.x = -2, .y = 2, .z = 4}));
}
