// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "unitest.h"

// vector2 tests
TEST_CASE(vec2_add, "[vector2]") {
    const vector2 a = {.x = 1.0f, .y = 2.0f};
    const vector2 b = {.x = 3.0f, .y = 4.0f};
    const vector2 res = vec2_add(a, b);
    REQUIRE(vec2_eq(res, (vector2){.x = 4.0f, .y = 6.0f}));
}

TEST_CASE(vec2_sub, "[vector2]") {
    const vector2 a = {.x = 5.0f, .y = 6.0f};
    const vector2 b = {.x = 3.0f, .y = 4.0f};
    const vector2 res = vec2_sub(a, b);
    REQUIRE(vec2_eq(res, (vector2){.x = 2.0f, .y = 2.0f}));
}

TEST_CASE(vec2_mul_scalar, "[vector2]") {
    const vector2 v = {.x = 1.0f, .y = 2.0f};
    const vector2 res = vec2_mul_scalar(v, 3.0f);
    REQUIRE(vec2_eq(res, (vector2){.x = 3.0f, .y = 6.0f}));
}

TEST_CASE(vec2_dot, "[vector2]") {
    const vector2 a = {.x = 1.0f, .y = 2.0f};
    const vector2 b = {.x = 3.0f, .y = 4.0f};
    const vm_float_t res = vec2_dot(a, b);
    REQUIRE(VECMAT_EQ(res, 11.0f, EPSILON));
}

TEST_CASE(vec2_length, "[vector2]") {
    const vector2 v = {.x = 3.0f, .y = 4.0f};
    const vm_float_t res = vec2_length(v);
    REQUIRE(VECMAT_EQ(res, 5.0f, EPSILON));
}

TEST_CASE(vec2_normalize, "[vector2]") {
    const vector2 v = {.x = VM_F(3.0), .y = VM_F(4.0)};
    const vector2 res = vec2_normalize(v);
    REQUIRE(VECMAT_EQ(vec2_length(res), VM_F(1.0), EPSILON));
    REQUIRE(vec2_eq(res, (vector2){
        .x = VM_F(3.0) / VM_F(5.0),
        .y = VM_F(4.0) / VM_F(5.0)
    }));
}

TEST_CASE(vec2_x_axis, "[vector2]") {
    const vector2 res = vec2_x_axis(5.0f);
    REQUIRE(vec2_eq(res, (vector2){.x = 5.0f, .y = 0.0f}));
}

TEST_CASE(vec2_y_axis, "[vector2]") {
    const vector2 res = vec2_y_axis(7.0f);
    REQUIRE(vec2_eq(res, (vector2){.x = 0.0f, .y = 7.0f}));
}

TEST_CASE(vec2_x_scale, "[vector2]") {
    const vector2 res = vec2_x_scale(3.0f);
    REQUIRE(vec2_eq(res, (vector2){.x = 3.0f, .y = 1.0f}));
}

TEST_CASE(vec2_y_scale, "[vector2]") {
    const vector2 res = vec2_y_scale(4.0f);
    REQUIRE(vec2_eq(res, (vector2){.x = 1.0f, .y = 4.0f}));
}

TEST_CASE(vec2_perpendicular, "[vector2]") {
    const vector2 v = {.x = 3.0f, .y = 4.0f};
    const vector2 res = vec2_perpendicular(v);
    REQUIRE(vec2_eq(res, (vector2){.x = -4.0f, .y = 3.0f}));
}

TEST_CASE(vec2_aspect_ratio, "[vector2]") {
    const vector2 v = {.x = 8.0f, .y = 4.0f};
    const vm_float_t res = vec2_aspect_ratio(v);
    REQUIRE(VECMAT_EQ(res, 2.0f, EPSILON));
}

TEST_CASE(vec2_div_scalar, "[vector2]") {
    const vector2 v = {.x = 10.0f, .y = 20.0f};
    const vector2 res = vec2_div_scalar(v, 2.0f);
    REQUIRE(vec2_eq(res, (vector2){.x = 5.0f, .y = 10.0f}));
}

TEST_CASE(vec2_mul, "[vector2]") {
    const vector2 a = {.x = 1.0f, .y = 2.0f};
    const vector2 b = {.x = 3.0f, .y = 4.0f};
    const vector2 res = vec2_mul(a, b);
    REQUIRE(vec2_eq(res, (vector2){.x = 3.0f, .y = 8.0f}));
}

TEST_CASE(vec2_neg, "[vector2]") {
    const vector2 v = {.x = 1.0f, .y = -2.0f};
    const vector2 res = vec2_neg(v);
    REQUIRE(vec2_eq(res, (vector2){.x = -1.0f, .y = 2.0f}));
}

TEST_CASE(vec2_abs, "[vector2]") {
    const vector2 v = {.x = 1.0f, .y = -2.0f};
    const vector2 res = vec2_abs(v);
    REQUIRE(vec2_eq(res, (vector2){.x = 1.0f, .y = 2.0f}));
}

TEST_CASE(vec2_zero, "[vector2]") {
    const vector2 res = vec2_zero();
    REQUIRE(vec2_eq(res, (vector2){.x = 0.0f, .y = 0.0f}));
}

TEST_CASE(vec2_one, "[vector2]") {
    const vector2 res = vec2_one();
    REQUIRE(vec2_eq(res, (vector2){.x = 1.0f, .y = 1.0f}));
}

TEST_CASE(vec2_cross, "[vector2]") {
    const vector2 a = {.x = 1.0f, .y = 2.0f};
    const vector2 b = {.x = 3.0f, .y = 4.0f};
    const vector2 res = vec2_cross(a, b);
    REQUIRE(vec2_eq(res, (vector2){.x = -2.0f, .y = 0.0f}));
}

TEST_CASE(vec2_distance, "[vector2]") {
    const vector2 a = {.x = 0.0f, .y = 0.0f};
    const vector2 b = {.x = 3.0f, .y = 4.0f};
    const vm_float_t res = vec2_distance(a, b);
    REQUIRE(VECMAT_EQ(res, 5.0f, EPSILON));
}

TEST_CASE(vec2_angle, "[vector2]") {
    const vector2 a = {.x = 1.0f, .y = 0.0f};
    const vector2 b = {.x = 0.0f, .y = 1.0f};
    const vm_float_t res = vec2_angle(a, b);
    REQUIRE(VECMAT_EQ(res, (vm_float_t)M_PI_2, EPSILON));
}

TEST_CASE(vec2_min, "[vector2]") {
    const vector2 a = {.x = 1.0f, .y = 5.0f};
    const vector2 b = {.x = 3.0f, .y = 2.0f};
    const vector2 res = vec2_min(a, b);
    REQUIRE(vec2_eq(res, (vector2){.x = 1.0f, .y = 2.0f}));
}

TEST_CASE(vec2_max, "[vector2]") {
    const vector2 a = {.x = 1.0f, .y = 5.0f};
    const vector2 b = {.x = 3.0f, .y = 2.0f};
    const vector2 res = vec2_max(a, b);
    REQUIRE(vec2_eq(res, (vector2){.x = 3.0f, .y = 5.0f}));
}

TEST_CASE(vec2_sign, "[vector2]") {
    const vector2 v = {.x = 1.5f, .y = -2.3f};
    const vector2 res = vec2_sign(v);
    REQUIRE(vec2_eq(res, (vector2){.x = 1.0f, .y = -1.0f}));
}

TEST_CASE(vec2_floor, "[vector2]") {
    const vector2 v = {.x = 1.7f, .y = -2.3f};
    const vector2 res = vec2_floor(v);
    REQUIRE(vec2_eq(res, (vector2){.x = 1.0f, .y = -3.0f}));
}

TEST_CASE(vec2_ceil, "[vector2]") {
    const vector2 v = {.x = 1.1f, .y = -2.9f};
    const vector2 res = vec2_ceil(v);
    REQUIRE(vec2_eq(res, (vector2){.x = 2.0f, .y = -2.0f}));
}

TEST_CASE(vec2_round, "[vector2]") {
    const vector2 v = {.x = 1.5f, .y = 2.4f};
    const vector2 res = vec2_round(v);
    REQUIRE(vec2_eq(res, (vector2){.x = 2.0f, .y = 2.0f}));
}

TEST_CASE(vec2_scale, "[vector2]") {
    const vector2 v = {.x = 1.0f, .y = 2.0f};
    const vector2 res = vec2_scale(v, 3.0f);
    REQUIRE(vec2_eq(res, (vector2){.x = 3.0f, .y = 6.0f}));
}

TEST_CASE(vec2_reflect, "[vector2]") {
    const vector2 incident = {.x = 1.0f, .y = -1.0f};
    const vector2 normal = {.x = 0.0f, .y = 1.0f};
    const vector2 res = vec2_reflect(incident, normal);
    REQUIRE(vec2_eq(res, (vector2){.x = 1.0f, .y = 1.0f}));
}

TEST_CASE(vec2_project, "[vector2]") {
    const vector2 a = {.x = 3.0f, .y = 4.0f};
    const vector2 b = {.x = 1.0f, .y = 0.0f};
    const vector2 res = vec2_project(a, b);
    REQUIRE(vec2_eq(res, (vector2){.x = 3.0f, .y = 0.0f}));
}

TEST_CASE(vec2_tangent, "[vector2]") {
    const vector2 v = {.x = 3.0f, .y = 4.0f};
    const vector2 res = vec2_tangent(v);
    REQUIRE(vec2_eq(res, (vector2){.x = 4.0f, .y = -3.0f}));
}

TEST_CASE(vec2_rotate, "[vector2]") {
    const vector2 v = {.x = 1.0f, .y = 0.0f};
    const vm_float_t angle = M_PI_2;
    const vector2 res = vec2_rotate(v, angle);
    REQUIRE(vec2_eq(res, (vector2){.x = 0.0f, .y = 1.0f}));
}

TEST_CASE(vec2_slide, "[vector2]") {
    const vector2 v = {.x = 1.0f, .y = -1.0f};
    const vector2 normal = {.x = 0.0f, .y = 1.0f};
    const vector2 res = vec2_slide(v, normal);
    REQUIRE(vec2_eq(res, (vector2){.x = 1.0f, .y = 0.0f}));
}

TEST_CASE(vec2_clamp, "[vector2]") {
    const vector2 v = {.x = -5.0f, .y = 15.0f};
    const vector2 min = {.x = 0.0f, .y = 0.0f};
    const vector2 max = {.x = 10.0f, .y = 10.0f};
    const vector2 result = vec2_clamp(v, min, max);

    REQUIRE(VECMAT_EQ(result.x, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(result.y, 10.0f, EPSILON));
}

// vector3 tests
TEST_CASE(vec3_add, "[vector3]") {
    const vector3 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f};
    const vector3 b = {.x = 4.0f, .y = 5.0f, .z = 6.0f};
    const vector3 res = vec3_add(a, b);
    REQUIRE(vec3_eq(res, (vector3){.x = 5.0f, .y = 7.0f, .z = 9.0f}));
}

TEST_CASE(vec3_sub, "[vector3]") {
    const vector3 a = {.x = 5.0f, .y = 6.0f, .z = 7.0f};
    const vector3 b = {.x = 2.0f, .y = 3.0f, .z = 4.0f};
    const vector3 res = vec3_sub(a, b);
    REQUIRE(vec3_eq(res, (vector3){.x = 3.0f, .y = 3.0f, .z = 3.0f}));
}

TEST_CASE(vec3_mul_scalar, "[vector3]") {
    const vector3 v = {.x = 1.0f, .y = 2.0f, .z = 3.0f};
    const vector3 res = vec3_mul_scalar(v, 2.0f);
    REQUIRE(vec3_eq(res, (vector3){.x = 2.0f, .y = 4.0f, .z = 6.0f}));
}

TEST_CASE(vec3_dot, "[vector3]") {
    const vector3 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f};
    const vector3 b = {.x = 4.0f, .y = 5.0f, .z = 6.0f};
    const vm_float_t res = vec3_dot(a, b);
    REQUIRE(VECMAT_EQ(res, 32.0f, EPSILON));
}

TEST_CASE(vec3_cross, "[vector3]") {
    const vector3 a = {.x = 1.0f, .y = 0.0f, .z = 0.0f};
    const vector3 b = {.x = 0.0f, .y = 1.0f, .z = 0.0f};
    const vector3 res = vec3_cross(a, b);
    REQUIRE(vec3_eq(res, (vector3){.x = 0.0f, .y = 0.0f, .z = 1.0f}));
}

TEST_CASE(vec3_length, "[vector3]") {
    const vector3 v = {.x = 3.0f, .y = 4.0f, .z = 0.0f};
    const vm_float_t res = vec3_length(v);
    REQUIRE(VECMAT_EQ(res, 5.0f, EPSILON));
}

TEST_CASE(vec3_normalize, "[vector3]") {
    const vector3 v = {.x = VM_F(3.0), .y = VM_F(4.0), .z = VM_F(0.0)};
    const vector3 res = vec3_normalize(v);
    REQUIRE(VECMAT_EQ(vec3_length(res), VM_F(1.0), EPSILON));
    REQUIRE(vec3_eq(res, (vector3){
        .x = VM_F(3.0) / VM_F(5.0),
        .y = VM_F(4.0) / VM_F(5.0),
        .z = VM_F(0.0)
    }));
}

TEST_CASE(vec3_div_scalar, "[vector3]") {
    const vector3 v = {.x = 10.0f, .y = 20.0f, .z = 30.0f};
    const vector3 res = vec3_div_scalar(v, 2.0f);
    REQUIRE(vec3_eq(res, (vector3){.x = 5.0f, .y = 10.0f, .z = 15.0f}));
}

TEST_CASE(vec3_mul, "[vector3]") {
    const vector3 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f};
    const vector3 b = {.x = 4.0f, .y = 5.0f, .z = 6.0f};
    const vector3 res = vec3_mul(a, b);
    REQUIRE(vec3_eq(res, (vector3){.x = 4.0f, .y = 10.0f, .z = 18.0f}));
}

TEST_CASE(vec3_neg, "[vector3]") {
    const vector3 v = {.x = 1.0f, .y = -2.0f, .z = 3.0f};
    const vector3 res = vec3_neg(v);
    REQUIRE(vec3_eq(res, (vector3){.x = -1.0f, .y = 2.0f, .z = -3.0f}));
}

TEST_CASE(vec3_abs, "[vector3]") {
    const vector3 v = {.x = 1.0f, .y = -2.0f, .z = -3.0f};
    const vector3 res = vec3_abs(v);
    REQUIRE(vec3_eq(res, (vector3){.x = 1.0f, .y = 2.0f, .z = 3.0f}));
}

TEST_CASE(vec3_zero, "[vector3]") {
    const vector3 res = vec3_zero();
    REQUIRE(vec3_eq(res, (vector3){.x = 0.0f, .y = 0.0f, .z = 0.0f}));
}

TEST_CASE(vec3_one, "[vector3]") {
    const vector3 res = vec3_one();
    REQUIRE(vec3_eq(res, (vector3){.x = 1.0f, .y = 1.0f, .z = 1.0f}));
}

TEST_CASE(vec3_x_axis, "[vector3]") {
    const vector3 res = vec3_x_axis(5.0f);
    REQUIRE(vec3_eq(res, (vector3){.x = 5.0f, .y = 0.0f, .z = 0.0f}));
}

TEST_CASE(vec3_y_axis, "[vector3]") {
    const vector3 res = vec3_y_axis(7.0f);
    REQUIRE(vec3_eq(res, (vector3){.x = 0.0f, .y = 7.0f, .z = 0.0f}));
}

TEST_CASE(vec3_z_axis, "[vector3]") {
    const vector3 res = vec3_z_axis(9.0f);
    REQUIRE(vec3_eq(res, (vector3){.x = 0.0f, .y = 0.0f, .z = 9.0f}));
}

TEST_CASE(vec3_x_scale, "[vector3]") {
    const vector3 res = vec3_x_scale(3.0f);
    REQUIRE(vec3_eq(res, (vector3){.x = 3.0f, .y = 1.0f, .z = 1.0f}));
}

TEST_CASE(vec3_y_scale, "[vector3]") {
    const vector3 res = vec3_y_scale(4.0f);
    REQUIRE(vec3_eq(res, (vector3){.x = 1.0f, .y = 4.0f, .z = 1.0f}));
}

TEST_CASE(vec3_z_scale, "[vector3]") {
    const vector3 res = vec3_z_scale(5.0f);
    REQUIRE(vec3_eq(res, (vector3){.x = 1.0f, .y = 1.0f, .z = 5.0f}));
}

TEST_CASE(vec3_distance, "[vector3]") {
    const vector3 a = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
    const vector3 b = {.x = 3.0f, .y = 4.0f, .z = 0.0f};
    const vm_float_t res = vec3_distance(a, b);
    REQUIRE(VECMAT_EQ(res, 5.0f, EPSILON));
}

TEST_CASE(vec3_angle, "[vector3]") {
    const vector3 a = {.x = 1.0f, .y = 0.0f, .z = 0.0f};
    const vector3 b = {.x = 0.0f, .y = 1.0f, .z = 0.0f};
    const vm_float_t res = vec3_angle(a, b);
    REQUIRE(VECMAT_EQ(res, (vm_float_t)M_PI_2, EPSILON));
}

TEST_CASE(vec3_min, "[vector3]") {
    const vector3 a = {.x = 1.0f, .y = 5.0f, .z = 3.0f};
    const vector3 b = {.x = 3.0f, .y = 2.0f, .z = 4.0f};
    const vector3 res = vec3_min(a, b);
    REQUIRE(vec3_eq(res, (vector3){.x = 1.0f, .y = 2.0f, .z = 3.0f}));
}

TEST_CASE(vec3_max, "[vector3]") {
    const vector3 a = {.x = 1.0f, .y = 5.0f, .z = 3.0f};
    const vector3 b = {.x = 3.0f, .y = 2.0f, .z = 4.0f};
    const vector3 res = vec3_max(a, b);
    REQUIRE(vec3_eq(res, (vector3){.x = 3.0f, .y = 5.0f, .z = 4.0f}));
}

TEST_CASE(vec3_reflect, "[vector3]") {
    const vector3 incident = {.x = 1.0f, .y = -1.0f, .z = 0.0f};
    const vector3 normal = {.x = 0.0f, .y = 1.0f, .z = 0.0f};
    const vector3 res = vec3_reflect(incident, normal);
    REQUIRE(vec3_eq(res, (vector3){.x = 1.0f, .y = 1.0f, .z = 0.0f}));
}

TEST_CASE(vec3_refract, "[vector3]") {
    const vector3 incident = {.x = 0.0f, .y = -1.0f, .z = 0.0f};
    const vector3 normal = {.x = 0.0f, .y = 1.0f, .z = 0.0f};
    const vm_float_t eta = 1.0f;
    const vector3 res = vec3_refract(incident, normal, eta);
    REQUIRE(vec3_eq(res, (vector3){.x = 0.0f, .y = -1.0f, .z = 0.0f}));
}

TEST_CASE(vec3_lerp, "[vector3]") {
    const vector3 a = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
    const vector3 b = {.x = 10.0f, .y = 20.0f, .z = 30.0f};
    const vector3 res = vec3_lerp(a, b, 0.5f);
    REQUIRE(vec3_eq(res, (vector3){.x = 5.0f, .y = 10.0f, .z = 15.0f}));
}

TEST_CASE(vec3_clamp, "[vector3]") {
    const vector3 v = {.x = 5.0f, .y = -5.0f, .z = 15.0f};
    const vector3 min = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
    const vector3 max = {.x = 10.0f, .y = 10.0f, .z = 10.0f};
    const vector3 res = vec3_clamp(v, min, max);
    REQUIRE(vec3_eq(res, (vector3){.x = 5.0f, .y = 0.0f, .z = 10.0f}));
}

TEST_CASE(vec3_sign, "[vector3]") {
    const vector3 v = {.x = 1.5f, .y = -2.3f, .z = 0.0f};
    const vector3 res = vec3_sign(v);
    REQUIRE(vec3_eq(res, (vector3){.x = 1.0f, .y = -1.0f, .z = 0.0f}));
}

TEST_CASE(vec3_floor, "[vector3]") {
    const vector3 v = {.x = 1.7f, .y = -2.3f, .z = 3.9f};
    const vector3 res = vec3_floor(v);
    REQUIRE(vec3_eq(res, (vector3){.x = 1.0f, .y = -3.0f, .z = 3.0f}));
}

TEST_CASE(vec3_ceil, "[vector3]") {
    const vector3 v = {.x = 1.1f, .y = -2.9f, .z = 4.0f};
    const vector3 res = vec3_ceil(v);
    REQUIRE(vec3_eq(res, (vector3){.x = 2.0f, .y = -2.0f, .z = 4.0f}));
}

TEST_CASE(vec3_round, "[vector3]") {
    const vector3 v = {.x = 1.5f, .y = 2.4f, .z = -3.6f};
    const vector3 res = vec3_round(v);
    REQUIRE(vec3_eq(res, (vector3){.x = 2.0f, .y = 2.0f, .z = -4.0f}));
}

TEST_CASE(vec2_lerp, "[vector2]") {
    const vector2 a = {.x = 0.0f, .y = 0.0f};
    const vector2 b = {.x = 10.0f, .y = 20.0f};
    REQUIRE(vec2_eq(vec2_lerp(a, b, 0.5f), (vector2){.x = 5.0f, .y = 10.0f}));
}

TEST_CASE(vec2_div, "[vector2]") {
    const vector2 a = {.x = 8.0f, .y = 9.0f};
    const vector2 b = {.x = 2.0f, .y = 3.0f};
    REQUIRE(vec2_eq(vec2_div(a, b), (vector2){.x = 4.0f, .y = 3.0f}));
}

TEST_CASE(vec2_length_squared, "[vector2]") {
    REQUIRE(VECMAT_EQ(vec2_length_squared((vector2){.x = 3.0f, .y = 4.0f}), 25.0f, EPSILON));
}

TEST_CASE(vec2_cross_scalar, "[vector2]") {
    REQUIRE(VECMAT_EQ(vec2_cross_scalar((vector2){.x = 1.0f, .y = 2.0f},
        (vector2){.x = 3.0f, .y = 4.0f}), -2.0f, EPSILON));
}

TEST_CASE(vec2_from_angle, "[vector2]") {
    const vector2 res = vec2_from_angle(0.0f);
    REQUIRE(vec2_eq(res, (vector2){.x = 1.0f, .y = 0.0f}));
}

TEST_CASE(vec2_heading, "[vector2]") {
    REQUIRE(VECMAT_EQ(vec2_heading((vector2){.x = 0.0f, .y = 1.0f}), (vm_float_t)M_PI_2, EPSILON));
}

TEST_CASE(vec2_to_vec3, "[vector2]") {
    REQUIRE(vec3_eq(vec2_to_vec3((vector2){.x = 1.0f, .y = 2.0f}, 3.0f),
        (vector3){.x = 1.0f, .y = 2.0f, .z = 3.0f}));
}

TEST_CASE(vec2_saturate, "[vector2]") {
    REQUIRE(vec2_eq(vec2_saturate((vector2){.x = -1.0f, .y = 2.0f}), (vector2){.x = 0.0f, .y = 1.0f}));
}

TEST_CASE(vec2_move_toward, "[vector2]") {
    const vector2 res = vec2_move_toward((vector2){.x = 0.0f, .y = 0.0f},
        (vector2){.x = 10.0f, .y = 0.0f}, 3.0f);
    REQUIRE(vec2_eq(res, (vector2){.x = 3.0f, .y = 0.0f}));
}

TEST_CASE(vec3_project, "[vector3]") {
    const vector3 res = vec3_project((vector3){.x = 2.0f, .y = 0.0f, .z = 0.0f},
        (vector3){.x = 1.0f, .y = 0.0f, .z = 0.0f});
    REQUIRE(vec3_eq(res, (vector3){.x = 2.0f, .y = 0.0f, .z = 0.0f}));
}

TEST_CASE(vec3_slide, "[vector3]") {
    const vector3 res = vec3_slide((vector3){.x = 1.0f, .y = 1.0f, .z = 0.0f},
        (vector3){.x = 0.0f, .y = 1.0f, .z = 0.0f});
    REQUIRE(vec3_eq(res, (vector3){.x = 1.0f, .y = 0.0f, .z = 0.0f}));
}

TEST_CASE(vec3_xy, "[vector3]") {
    REQUIRE(vec2_eq(vec3_xy((vector3){.x = 1.0f, .y = 2.0f, .z = 3.0f}),
        (vector2){.x = 1.0f, .y = 2.0f}));
}

TEST_CASE(vec3_from_vec2, "[vector3]") {
    REQUIRE(vec3_eq(vec3_from_vec2((vector2){.x = 1.0f, .y = 2.0f}, 3.0f),
        (vector3){.x = 1.0f, .y = 2.0f, .z = 3.0f}));
}

TEST_CASE(vec3_ctor_assign, "[vector3]") {
    const vector3 v = vec3(1.0f, 2.0f, 3.0f);
    REQUIRE(vec3_eq(v, (vector3){.x = 1.0f, .y = 2.0f, .z = 3.0f}));
    vector3 dest;
    vec3_assign_xyz(&dest, 0.0f, 0.0f, 0.0f);
    vec3_add_assign(&dest, &v);
    REQUIRE(vec3_eq(dest, v));
}

TEST_CASE(vec3_rotate_axis, "[vector3]") {
    const vector3 res = vec3_rotate_axis((vector3){.x = 1.0f, .y = 0.0f, .z = 0.0f},
        (vector3){.x = 0.0f, .y = 0.0f, .z = 1.0f}, M_PI_2);
    REQUIRE(vec3_near(res, (vector3){.x = 0.0f, .y = 1.0f, .z = 0.0f}, EPSILON));
}

TEST_CASE(mat2_mul_vec2, "[vector2]") {
    const matrix2 m = mat2_identity();
    const vector2 v = {.x = 3.0f, .y = 4.0f};
    REQUIRE(vec2_eq(mat2_mul_vec2(m, v), v));
}

TEST_CASE(mat3_mul_vec3, "[vector3]") {
    const matrix3 m = mat3_identity();
    const vector3 v = {.x = 1.0f, .y = 2.0f, .z = 3.0f};
    REQUIRE(vec3_eq(mat3_mul_vec3(m, v), v));
}

TEST_CASE(mat4_mul_vec3_point, "[vector3]") {
    const matrix4 m = mat4_translate((vector3){.x = 1.0f, .y = 2.0f, .z = 3.0f});
    const vector3 res = mat4_mul_vec3(m, (vector3){.x = 0.0f, .y = 0.0f, .z = 0.0f}, 1.0f);
    REQUIRE(vec3_eq(res, (vector3){.x = 1.0f, .y = 2.0f, .z = 3.0f}));
}

// vector4 tests
TEST_CASE(vec4_add, "[vector4]") {
    const vector4 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f};
    const vector4 b = {.x = 5.0f, .y = 6.0f, .z = 7.0f, .w = 8.0f};
    const vector4 res = vec4_add(a, b);
    REQUIRE(vec4_eq(res, (vector4){.x = 6.0f, .y = 8.0f, .z = 10.0f, .w = 12.0f}));
}

TEST_CASE(vec4_sub, "[vector4]") {
    const vector4 a = {.x = 5.0f, .y = 6.0f, .z = 7.0f, .w = 8.0f};
    const vector4 b = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f};
    const vector4 res = vec4_sub(a, b);
    REQUIRE(vec4_eq(res, (vector4){.x = 4.0f, .y = 4.0f, .z = 4.0f, .w = 4.0f}));
}

TEST_CASE(vec4_mul_scalar, "[vector4]") {
    const vector4 v = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f};
    const vector4 res = vec4_mul_scalar(v, 2.0f);
    REQUIRE(vec4_eq(res, (vector4){.x = 2.0f, .y = 4.0f, .z = 6.0f, .w = 8.0f}));
}

TEST_CASE(vec4_dot, "[vector4]") {
    const vector4 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f};
    const vector4 b = {.x = 5.0f, .y = 6.0f, .z = 7.0f, .w = 8.0f};
    const vm_float_t res = vec4_dot(a, b);
    REQUIRE(VECMAT_EQ(res, 70.0f, EPSILON));
}

TEST_CASE(vec4_length, "[vector4]") {
    const vector4 v = {.x = 1.0f, .y = 2.0f, .z = 2.0f, .w = 4.0f};
    const vm_float_t res = vec4_length(v);
    REQUIRE(VECMAT_EQ(res, 5.0f, EPSILON));
}

TEST_CASE(vec4_normalize, "[vector4]") {
    const vector4 v = {.x = VM_F(1.0), .y = VM_F(2.0), .z = VM_F(2.0), .w = VM_F(4.0)};
    const vector4 res = vec4_normalize(v);
    REQUIRE(VECMAT_EQ(vec4_length(res), VM_F(1.0), EPSILON));
    REQUIRE(vec4_eq(res, (vector4){
        .x = VM_F(1.0) / VM_F(5.0),
        .y = VM_F(2.0) / VM_F(5.0),
        .z = VM_F(2.0) / VM_F(5.0),
        .w = VM_F(4.0) / VM_F(5.0)
    }));
}

TEST_CASE(vec4_div_scalar, "[vector4]") {
    const vector4 v = {.x = 10.0f, .y = 20.0f, .z = 30.0f, .w = 40.0f};
    const vector4 res = vec4_div_scalar(v, 2.0f);
    REQUIRE(vec4_eq(res, (vector4){.x = 5.0f, .y = 10.0f, .z = 15.0f, .w = 20.0f}));
}

TEST_CASE(vec4_mul, "[vector4]") {
    const vector4 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f};
    const vector4 b = {.x = 5.0f, .y = 6.0f, .z = 7.0f, .w = 8.0f};
    const vector4 res = vec4_mul(a, b);
    REQUIRE(vec4_eq(res, (vector4){.x = 5.0f, .y = 12.0f, .z = 21.0f, .w = 32.0f}));
}

TEST_CASE(vec4_neg, "[vector4]") {
    const vector4 v = {.x = 1.0f, .y = -2.0f, .z = 3.0f, .w = -4.0f};
    const vector4 res = vec4_neg(v);
    REQUIRE(vec4_eq(res, (vector4){.x = -1.0f, .y = 2.0f, .z = -3.0f, .w = 4.0f}));
}

TEST_CASE(vec4_abs, "[vector4]") {
    const vector4 v = {.x = 1.0f, .y = -2.0f, .z = -3.0f, .w = 4.0f};
    const vector4 res = vec4_abs(v);
    REQUIRE(vec4_eq(res, (vector4){.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f}));
}

TEST_CASE(vec4_zero, "[vector4]") {
    const vector4 res = vec4_zero();
    REQUIRE(vec4_eq(res, (vector4){.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f}));
}

TEST_CASE(vec4_one, "[vector4]") {
    const vector4 res = vec4_one();
    REQUIRE(vec4_eq(res, (vector4){.x = 1.0f, .y = 1.0f, .z = 1.0f, .w = 1.0f}));
}

TEST_CASE(vec4_x_axis, "[vector4]") {
    const vector4 res = vec4_x_axis(5.0f);
    REQUIRE(vec4_eq(res, (vector4){.x = 5.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f}));
}

TEST_CASE(vec4_y_axis, "[vector4]") {
    const vector4 res = vec4_y_axis(7.0f);
    REQUIRE(vec4_eq(res, (vector4){.x = 0.0f, .y = 7.0f, .z = 0.0f, .w = 0.0f}));
}

TEST_CASE(vec4_z_axis, "[vector4]") {
    const vector4 res = vec4_z_axis(9.0f);
    REQUIRE(vec4_eq(res, (vector4){.x = 0.0f, .y = 0.0f, .z = 9.0f, .w = 0.0f}));
}

TEST_CASE(vec4_w_axis, "[vector4]") {
    const vector4 res = vec4_w_axis(11.0f);
    REQUIRE(vec4_eq(res, (vector4){.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 11.0f}));
}

TEST_CASE(vec4_x_scale, "[vector4]") {
    const vector4 res = vec4_x_scale(3.0f);
    REQUIRE(vec4_eq(res, (vector4){.x = 3.0f, .y = 1.0f, .z = 1.0f, .w = 1.0f}));
}

TEST_CASE(vec4_y_scale, "[vector4]") {
    const vector4 res = vec4_y_scale(4.0f);
    REQUIRE(vec4_eq(res, (vector4){.x = 1.0f, .y = 4.0f, .z = 1.0f, .w = 1.0f}));
}

TEST_CASE(vec4_z_scale, "[vector4]") {
    const vector4 res = vec4_z_scale(5.0f);
    REQUIRE(vec4_eq(res, (vector4){.x = 1.0f, .y = 1.0f, .z = 5.0f, .w = 1.0f}));
}

TEST_CASE(vec4_w_scale, "[vector4]") {
    const vector4 res = vec4_w_scale(6.0f);
    REQUIRE(vec4_eq(res, (vector4){.x = 1.0f, .y = 1.0f, .z = 1.0f, .w = 6.0f}));
}

TEST_CASE(vec4_distance, "[vector4]") {
    const vector4 a = {.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f};
    const vector4 b = {.x = 1.0f, .y = 2.0f, .z = 2.0f, .w = 4.0f};
    const vm_float_t res = vec4_distance(a, b);
    REQUIRE(VECMAT_EQ(res, 5.0f, EPSILON));
}

TEST_CASE(vec4_min, "[vector4]") {
    const vector4 a = {.x = 1.0f, .y = 5.0f, .z = 3.0f, .w = 7.0f};
    const vector4 b = {.x = 3.0f, .y = 2.0f, .z = 4.0f, .w = 6.0f};
    const vector4 res = vec4_min(a, b);
    REQUIRE(vec4_eq(res, (vector4){.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 6.0f}));
}

TEST_CASE(vec4_max, "[vector4]") {
    const vector4 a = {.x = 1.0f, .y = 5.0f, .z = 3.0f, .w = 7.0f};
    const vector4 b = {.x = 3.0f, .y = 2.0f, .z = 4.0f, .w = 6.0f};
    const vector4 res = vec4_max(a, b);
    REQUIRE(vec4_eq(res, (vector4){.x = 3.0f, .y = 5.0f, .z = 4.0f, .w = 7.0f}));
}

TEST_CASE(vec4_lerp, "[vector4]") {
    const vector4 a = {.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f};
    const vector4 b = {.x = 10.0f, .y = 20.0f, .z = 30.0f, .w = 40.0f};
    const vector4 res = vec4_lerp(a, b, 0.5f);
    REQUIRE(vec4_eq(res, (vector4){.x = 5.0f, .y = 10.0f, .z = 15.0f, .w = 20.0f}));
}

TEST_CASE(vec4_clamp, "[vector4]") {
    const vector4 v = {.x = 5.0f, .y = -5.0f, .z = 15.0f, .w = 25.0f};
    const vector4 min = {.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f};
    const vector4 max = {.x = 10.0f, .y = 10.0f, .z = 10.0f, .w = 20.0f};
    const vector4 res = vec4_clamp(v, min, max);
    REQUIRE(vec4_eq(res, (vector4){.x = 5.0f, .y = 0.0f, .z = 10.0f, .w = 20.0f}));
}

TEST_CASE(vec4_homogenize, "[vector4]") {
    const vector4 v = {.x = 2.0f, .y = 4.0f, .z = 6.0f, .w = 2.0f};
    const vector4 res = vec4_homogenize(v);
    REQUIRE(vec4_eq(res, (vector4){.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 1.0f}));
}

TEST_CASE(vec4_sign, "[vector4]") {
    const vector4 v = {.x = 1.5f, .y = -2.3f, .z = 0.0f, .w = -0.5f};
    const vector4 res = vec4_sign(v);
    REQUIRE(vec4_eq(res, (vector4){.x = 1.0f, .y = -1.0f, .z = 0.0f, .w = -1.0f}));
}

TEST_CASE(vec4_floor, "[vector4]") {
    const vector4 v = {.x = 1.7f, .y = -2.3f, .z = 3.9f, .w = -0.1f};
    const vector4 res = vec4_floor(v);
    REQUIRE(vec4_eq(res, (vector4){.x = 1.0f, .y = -3.0f, .z = 3.0f, .w = -1.0f}));
}

TEST_CASE(vec4_ceil, "[vector4]") {
    const vector4 v = {.x = 1.1f, .y = -2.9f, .z = 4.0f, .w = 5.2f};
    const vector4 res = vec4_ceil(v);
    REQUIRE(vec4_eq(res, (vector4){.x = 2.0f, .y = -2.0f, .z = 4.0f, .w = 6.0f}));
}

TEST_CASE(vec4_round, "[vector4]") {
    const vector4 v = {.x = 1.5f, .y = 2.4f, .z = -3.6f, .w = 0.5f};
    const vector4 res = vec4_round(v);
    REQUIRE(vec4_eq(res, (vector4){.x = 2.0f, .y = 2.0f, .z = -4.0f, .w = 1.0f}));
}

// Conversions
TEST_CASE(vec4_to_vec3, "[vector4]") {
    const vector4 v = {.x = 2.0f, .y = 4.0f, .z = 6.0f, .w = 2.0f};
    const vector3 res = vec4_to_vec3(v);
    REQUIRE(vec3_eq(res, (vector3){.x = 2.0f, .y = 4.0f, .z = 6.0f}));
}

TEST_CASE(vec4_parity, "[vector4]") {
    const vector4 a = {.x = 2.0f, .y = 4.0f, .z = 6.0f, .w = 8.0f};
    const vector4 b = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f};
    REQUIRE(vec4_eq(vec4_div(a, b), (vector4){.x = 2.0f, .y = 2.0f, .z = 2.0f, .w = 2.0f}));
    REQUIRE(vec4_eq(vec4_add_scalar(b, 1.0f), (vector4){.x = 2.0f, .y = 3.0f, .z = 4.0f, .w = 5.0f}));
    REQUIRE(vec4_eq(vec4_sub_scalar(b, 1.0f), (vector4){.x = 0.0f, .y = 1.0f, .z = 2.0f, .w = 3.0f}));
    REQUIRE(vec4_eq(vec4_clamp_scalar((vector4){.x = -1.0f, .y = 0.5f, .z = 2.0f, .w = 0.0f}, 0.0f, 1.0f),
                    (vector4){.x = 0.0f, .y = 0.5f, .z = 1.0f, .w = 0.0f}));
    REQUIRE(vec4_eq(vec4_saturate((vector4){.x = -1.0f, .y = 0.5f, .z = 2.0f, .w = 0.0f}),
                    (vector4){.x = 0.0f, .y = 0.5f, .z = 1.0f, .w = 0.0f}));
    REQUIRE(vec4_eq(vec4_splat(3.0f), (vector4){.x = 3.0f, .y = 3.0f, .z = 3.0f, .w = 3.0f}));
    REQUIRE(VECMAT_EQ(vec4_length_squared(b), 30.0f, EPSILON));
    REQUIRE(VECMAT_EQ(vec4_distance_squared(a, b), 30.0f, EPSILON));
    REQUIRE(vec4_is_zero(vec4_zero()));
    REQUIRE(vec4_is_normalized(vec4_normalize(a)));
    REQUIRE(vec4_near(a, a, EPSILON));
    REQUIRE(vec4_eq(vec4_project(a, b), a));
    REQUIRE(vec4_eq(vec4_reject(a, b), vec4_zero()));
    REQUIRE(vec4_eq(vec4_fract((vector4){.x = 1.25f, .y = -1.25f, .z = 2.0f, .w = 0.5f}),
                    (vector4){.x = 0.25f, .y = 0.75f, .z = 0.0f, .w = 0.5f}));
    REQUIRE(vec4_eq(vec4_slide((vector4){.x = 1.0f, .y = 1.0f, .z = 0.0f, .w = 0.0f},
        (vector4){.x = 0.0f, .y = 1.0f, .z = 0.0f, .w = 0.0f}),
        (vector4){.x = 1.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f}));
}
