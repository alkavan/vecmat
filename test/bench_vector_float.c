// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "unitest.h"

BENCHMARK(vec2_mul_scalar, "iterations: 1000000")
{
    const int iterations = 1000000;
    vector2 a = {.x = 1.0f, .y = 2.0f};

    for(int i = 0; i < iterations; i++) {
        const vector2 b = {.x = 3.0f, .y = 4.0f};
        const vector2 c = vec2_add(a, b);
        a = vec2_sub(c, b);
        const float dot = vec2_dot(a, b);
        a = vec2_mul_scalar(a, dot);
    }
}

BENCHMARK(vec2_mul_scalar_ptr, "iterations: 1000000")
{
    const int iterations = 1000000;
    vector2 a = {.x = 1.0f, .y = 2.0f};
    vector2 res;

    for(int i = 0; i < iterations; i++) {
        const vector2 b = {.x = 3.0f, .y = 4.0f};
        vec2_add_ptr(&res, &a, &b);
        vec2_sub_ptr(&a, &res, &b);
        const float dot = vec2_dot(a, b);
        vec2_mul_scalar_ptr(&res, &a, dot);
    }
}

BENCHMARK(vec2_normalize, "iterations: 1000000")
{
    const int iterations = 1000000;
    vector2 a = {.x = 1.0f, .y = 2.0f};

    for(int i = 0; i < iterations; i++) {
        const vector2 b = {.x = 3.0f, .y = 4.0f};
        vector2 c = vec2_add(a, b);
        a = vec2_normalize(c);
        const float len = vec2_length(a);
        c = vec2_mul_scalar(a, len);
    }
}

BENCHMARK(vec2_normalize_ptr, "iterations: 1000000")
{
    const int iterations = 1000000;
    vector2 a = {.x = 1.0f, .y = 2.0f};
    const vector2 b = {.x = 3.0f, .y = 4.0f};
    vector2 res;

    for(int i = 0; i < iterations; i++) {
        vec2_add_ptr(&res, &a, &b);
        vec2_normalize_ptr(&a, &res);
        const float len = vec2_length(a);
        vec2_mul_scalar_ptr(&res, &a, len);
    }
}

BENCHMARK(vec2_length, "iterations: 1000000")
{
    const int iterations = 1000000;
    vector2 a = {.x = 1.0f, .y = 2.0f};

    for(int i = 0; i < iterations; i++) {
        const vector2 b = {.x = 3.0f, .y = 4.0f};
        vector2 c = vec2_add(a, b);
        a = vec2_sub(c, b);
        const float len = vec2_length(a);
        c = vec2_mul_scalar(a, len);
    }
}

BENCHMARK(vec3_cross, "iterations: 1000000")
{
    const int iterations = 1000000;
    vector3 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f};

    for(int i = 0; i < iterations; i++) {
        const vector3 b = {.x = 4.0f, .y = 5.0f, .z = 6.0f};
        vector3 c = vec3_add(a, b);
        a = vec3_sub(c, b);
        const float dot = vec3_dot(a, b);
        c = vec3_mul_scalar(a, dot);
        vector3 cross = vec3_cross(a, b);
    }
}

BENCHMARK(vec3_cross_ptr, "iterations: 1000000")
{
    const int iterations = 1000000;
    vector3 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f};
    vector3 res;

    for(int i = 0; i < iterations; i++) {
        const vector3 b = {.x = 4.0f, .y = 5.0f, .z = 6.0f};
        vec3_add_ptr(&res, &a, &b);
        vec3_sub_ptr(&a, &res, &b);
        const float dot = vec3_dot(a, b);
        vec3_mul_scalar_ptr(&res, &a, dot);
        vector3 cross;
        vec3_cross_ptr(&cross, &a, &b);
    }
}

BENCHMARK(vec3_normalize, "iterations: 1000000")
{
    const int iterations = 1000000;
    vector3 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f};

    for(int i = 0; i < iterations; i++) {
        const vector3 b = {.x = 4.0f, .y = 5.0f, .z = 6.0f};
        vector3 c = vec3_add(a, b);
        a = vec3_normalize(c);
        const float len = vec3_length(a);
        c = vec3_mul_scalar(a, len);
    }
}

BENCHMARK(vec3_normalize_ptr, "iterations: 1000000")
{
    const int iterations = 1000000;
    vector3 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f};
    vector3 res;

    for(int i = 0; i < iterations; i++) {
        const vector3 b = {.x = 4.0f, .y = 5.0f, .z = 6.0f};
        vec3_add_ptr(&res, &a, &b);
        vec3_normalize_ptr(&a, &res);
        const float len = vec3_length(a);
        vec3_mul_scalar_ptr(&res, &a, len);
    }
}

BENCHMARK(vec3_reflect, "iterations: 1000000")
{
    const int iterations = 1000000;
    vector3 incident = {.x = 1.0f, .y = 2.0f, .z = 3.0f};

    for(int i = 0; i < iterations; i++) {
        const vector3 normal = {.x = 0.0f, .y = 1.0f, .z = 0.0f};
        const vector3 reflected = vec3_reflect(incident, normal);
        const float dot = vec3_dot(reflected, normal);
        incident = vec3_mul_scalar(reflected, dot);
    }
}

BENCHMARK(vec3_reflect_ptr, "iterations: 1000000")
{
    const int iterations = 1000000;
    vector3 incident = {.x = 1.0f, .y = 2.0f, .z = 3.0f};
    const vector3 normal = {.x = 0.0f, .y = 1.0f, .z = 0.0f};
    vector3 res;

    for(int i = 0; i < iterations; i++) {
        vec3_reflect_ptr(&res, &incident, &normal);
        const float dot = vec3_dot(res, normal);
        vec3_mul_scalar_ptr(&incident, &res, dot);
    }
}

BENCHMARK(vec3_lerp, "iterations: 1000000")
{
    const int iterations = 1000000;
    vector3 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f};

    for(int i = 0; i < iterations; i++) {
        const vector3 b = {.x = 4.0f, .y = 5.0f, .z = 6.0f};
        const vector3 c = vec3_lerp(a, b, 0.5f);
        a = vec3_add(c, a);
        const float len = vec3_length(c);
        a = vec3_mul_scalar(a, len);
    }
}

BENCHMARK(vec3_lerp_ptr, "iterations: 1000000")
{
    const int iterations = 1000000;
    vector3 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f};
    const vector3 b = {.x = 4.0f, .y = 5.0f, .z = 6.0f};
    vector3 res;

    for(int i = 0; i < iterations; i++) {
        vec3_lerp_ptr(&res, &a, &b, 0.5f);
        vec3_add_ptr(&a, &res, &a);
        const float len = vec3_length(res);
        vec3_mul_scalar_ptr(&a, &a, len);
    }
}

BENCHMARK(vec4_mul_scalar, "iterations: 1000000")
{
    const int iterations = 1000000;
    vector4 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f};

    for(int i = 0; i < iterations; i++) {
        const vector4 b = {.x = 5.0f, .y = 6.0f, .z = 7.0f, .w = 8.0f};
        vector4 c = vec4_add(a, b);
        a = vec4_sub(c, b);
        const float dot = vec4_dot(a, b);
        c = vec4_mul_scalar(a, dot);
    }
}

BENCHMARK(vec4_mul_scalar_ptr, "iterations: 1000000")
{
    const int iterations = 1000000;
    vector4 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f};
    vector4 res;

    for(int i = 0; i < iterations; i++) {
        const vector4 b = {.x = 5.0f, .y = 6.0f, .z = 7.0f, .w = 8.0f};
        vec4_add_ptr(&res, &a, &b);
        vec4_sub_ptr(&a, &res, &b);
        const float dot = vec4_dot(a, b);
        vec4_mul_scalar_ptr(&res, &a, dot);
    }
}

BENCHMARK(vec4_normalize, "iterations: 1000000")
{
    const int iterations = 1000000;
    vector4 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f};

    for(int i = 0; i < iterations; i++) {
        const vector4 b = {.x = 5.0f, .y = 6.0f, .z = 7.0f, .w = 8.0f};
        vector4 c = vec4_add(a, b);
        a = vec4_normalize(c);
        const float len = vec4_length(a);
        c = vec4_mul_scalar(a, len);
    }
}

BENCHMARK(vec4_normalize_ptr, "iterations: 1000000")
{
    const int iterations = 1000000;
    vector4 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f};
    vector4 res;

    for(int i = 0; i < iterations; i++) {
        const vector4 b = {.x = 5.0f, .y = 6.0f, .z = 7.0f, .w = 8.0f};
        vec4_add_ptr(&res, &a, &b);
        vec4_normalize_ptr(&a, &res);
        const float len = vec4_length(a);
        vec4_mul_scalar_ptr(&res, &a, len);
    }
}

BENCHMARK(vec4_lerp, "iterations: 1000000")
{
    const int iterations = 1000000;
    vector4 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f};

    for(int i = 0; i < iterations; i++) {
        const vector4 b = {.x = 5.0f, .y = 6.0f, .z = 7.0f, .w = 8.0f};
        const vector4 c = vec4_lerp(a, b, 0.5f);
        a = vec4_add(c, a);
        const float len = vec4_length(c);
        a = vec4_mul_scalar(a, len);
    }
}

BENCHMARK(vec4_lerp_ptr, "iterations: 1000000")
{
    const int iterations = 1000000;
    vector4 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f};
    const vector4 b = {.x = 5.0f, .y = 6.0f, .z = 7.0f, .w = 8.0f};
    vector4 res;

    for(int i = 0; i < iterations; i++) {
        vec4_lerp_ptr(&res, &a, &b, 0.5f);
        vec4_add_ptr(&a, &res, &a);
        const float len = vec4_length(res);
        vec4_mul_scalar_ptr(&a, &a, len);
    }
}
