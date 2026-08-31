// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "unitest.h"

BENCHMARK(vec2_mul_scalar, "iterations: 1000000 [vec2_add,vec2_sub,vec2_dot,vec2_mul_scalar]")
{
    const int iterations = 1000000;
    vector2 a = {.x = 1.0f, .y = 2.0f};

    for(int i = 0; i < iterations; i++) {
        const vector2 b = {.x = 3.0f, .y = 4.0f};
        const vector2 c = vec2_add(a, b);
        a = vec2_sub(c, b);
        const vm_float_t dot = vec2_dot(a, b);
        a = vec2_mul_scalar(a, dot);
    }
}

BENCHMARK(vec2_mul_scalar_ptr, "iterations: 1000000 [vec2_add_ptr,vec2_sub_ptr,vec2_dot,vec2_mul_scalar_ptr]")
{
    const int iterations = 1000000;
    vector2 a = {.x = 1.0f, .y = 2.0f};
    vector2 res;

    for(int i = 0; i < iterations; i++) {
        const vector2 b = {.x = 3.0f, .y = 4.0f};
        vec2_add_ptr(&res, &a, &b);
        vec2_sub_ptr(&a, &res, &b);
        const vm_float_t dot = vec2_dot(a, b);
        vec2_mul_scalar_ptr(&res, &a, dot);
    }
}

BENCHMARK(vec2_normalize, "iterations: 1000000 [vec2_add,vec2_normalize,vec2_length,vec2_mul_scalar]")
{
    const int iterations = 1000000;
    vector2 a = {.x = 1.0f, .y = 2.0f};

    for(int i = 0; i < iterations; i++) {
        const vector2 b = {.x = 3.0f, .y = 4.0f};
        vector2 c = vec2_add(a, b);
        a = vec2_normalize(c);
        const vm_float_t len = vec2_length(a);
        c = vec2_mul_scalar(a, len);
    }
}

BENCHMARK(vec2_normalize_ptr, "iterations: 1000000 [vec2_add_ptr,vec2_normalize_ptr,vec2_length,vec2_mul_scalar_ptr]")
{
    const int iterations = 1000000;
    vector2 a = {.x = 1.0f, .y = 2.0f};
    const vector2 b = {.x = 3.0f, .y = 4.0f};
    vector2 res;

    for(int i = 0; i < iterations; i++) {
        vec2_add_ptr(&res, &a, &b);
        vec2_normalize_ptr(&a, &res);
        const vm_float_t len = vec2_length(a);
        vec2_mul_scalar_ptr(&res, &a, len);
    }
}

BENCHMARK(vec2_length, "iterations: 1000000 [vec2_add,vec2_sub,vec2_length,vec2_mul_scalar]")
{
    const int iterations = 1000000;
    vector2 a = {.x = 1.0f, .y = 2.0f};

    for(int i = 0; i < iterations; i++) {
        const vector2 b = {.x = 3.0f, .y = 4.0f};
        vector2 c = vec2_add(a, b);
        a = vec2_sub(c, b);
        const vm_float_t len = vec2_length(a);
        c = vec2_mul_scalar(a, len);
    }
}

BENCHMARK(vec3_cross, "iterations: 1000000 [vec3_add,vec3_sub,vec3_dot,vec3_mul_scalar,vec3_cross]")
{
    const int iterations = 1000000;
    vector3 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f};

    for(int i = 0; i < iterations; i++) {
        const vector3 b = {.x = 4.0f, .y = 5.0f, .z = 6.0f};
        vector3 c = vec3_add(a, b);
        a = vec3_sub(c, b);
        const vm_float_t dot = vec3_dot(a, b);
        c = vec3_mul_scalar(a, dot);
        vector3 cross = vec3_cross(a, b);
    }
}

BENCHMARK(vec3_cross_ptr, "iterations: 1000000 [vec3_add_ptr,vec3_sub_ptr,vec3_dot,vec3_mul_scalar_ptr,vec3_cross_ptr]")
{
    const int iterations = 1000000;
    vector3 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f};
    vector3 res;

    for(int i = 0; i < iterations; i++) {
        const vector3 b = {.x = 4.0f, .y = 5.0f, .z = 6.0f};
        vec3_add_ptr(&res, &a, &b);
        vec3_sub_ptr(&a, &res, &b);
        const vm_float_t dot = vec3_dot(a, b);
        vec3_mul_scalar_ptr(&res, &a, dot);
        vector3 cross;
        vec3_cross_ptr(&cross, &a, &b);
    }
}

BENCHMARK(vec3_normalize, "iterations: 1000000 [vec3_add,vec3_normalize,vec3_length,vec3_mul_scalar]")
{
    const int iterations = 1000000;
    vector3 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f};

    for(int i = 0; i < iterations; i++) {
        const vector3 b = {.x = 4.0f, .y = 5.0f, .z = 6.0f};
        vector3 c = vec3_add(a, b);
        a = vec3_normalize(c);
        const vm_float_t len = vec3_length(a);
        c = vec3_mul_scalar(a, len);
    }
}

BENCHMARK(vec3_normalize_ptr, "iterations: 1000000 [vec3_add_ptr,vec3_normalize_ptr,vec3_length,vec3_mul_scalar_ptr]")
{
    const int iterations = 1000000;
    vector3 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f};
    vector3 res;

    for(int i = 0; i < iterations; i++) {
        const vector3 b = {.x = 4.0f, .y = 5.0f, .z = 6.0f};
        vec3_add_ptr(&res, &a, &b);
        vec3_normalize_ptr(&a, &res);
        const vm_float_t len = vec3_length(a);
        vec3_mul_scalar_ptr(&res, &a, len);
    }
}

BENCHMARK(vec3_reflect, "iterations: 1000000 [vec3_reflect,vec3_dot,vec3_mul_scalar]")
{
    const int iterations = 1000000;
    vector3 incident = {.x = 1.0f, .y = 2.0f, .z = 3.0f};

    for(int i = 0; i < iterations; i++) {
        const vector3 normal = {.x = 0.0f, .y = 1.0f, .z = 0.0f};
        const vector3 reflected = vec3_reflect(incident, normal);
        const vm_float_t dot = vec3_dot(reflected, normal);
        incident = vec3_mul_scalar(reflected, dot);
    }
}

BENCHMARK(vec3_reflect_ptr, "iterations: 1000000 [vec3_reflect_ptr,vec3_dot,vec3_mul_scalar_ptr]")
{
    const int iterations = 1000000;
    vector3 incident = {.x = 1.0f, .y = 2.0f, .z = 3.0f};
    const vector3 normal = {.x = 0.0f, .y = 1.0f, .z = 0.0f};
    vector3 res;

    for(int i = 0; i < iterations; i++) {
        vec3_reflect_ptr(&res, &incident, &normal);
        const vm_float_t dot = vec3_dot(res, normal);
        vec3_mul_scalar_ptr(&incident, &res, dot);
    }
}

BENCHMARK(vec3_lerp, "iterations: 1000000 [vec3_lerp,vec3_add,vec3_length,vec3_mul_scalar]")
{
    const int iterations = 1000000;
    vector3 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f};

    for(int i = 0; i < iterations; i++) {
        const vector3 b = {.x = 4.0f, .y = 5.0f, .z = 6.0f};
        const vector3 c = vec3_lerp(a, b, 0.5f);
        a = vec3_add(c, a);
        const vm_float_t len = vec3_length(c);
        a = vec3_mul_scalar(a, len);
    }
}

BENCHMARK(vec3_lerp_ptr, "iterations: 1000000 [vec3_lerp_ptr,vec3_add_ptr,vec3_length,vec3_mul_scalar_ptr]")
{
    const int iterations = 1000000;
    vector3 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f};
    const vector3 b = {.x = 4.0f, .y = 5.0f, .z = 6.0f};
    vector3 res;

    for(int i = 0; i < iterations; i++) {
        vec3_lerp_ptr(&res, &a, &b, 0.5f);
        vec3_add_ptr(&a, &res, &a);
        const vm_float_t len = vec3_length(res);
        vec3_mul_scalar_ptr(&a, &a, len);
    }
}

BENCHMARK(vec4_mul_scalar, "iterations: 1000000 [vec4_add,vec4_sub,vec4_dot,vec4_mul_scalar]")
{
    const int iterations = 1000000;
    vector4 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f};

    for(int i = 0; i < iterations; i++) {
        const vector4 b = {.x = 5.0f, .y = 6.0f, .z = 7.0f, .w = 8.0f};
        vector4 c = vec4_add(a, b);
        a = vec4_sub(c, b);
        const vm_float_t dot = vec4_dot(a, b);
        c = vec4_mul_scalar(a, dot);
    }
}

BENCHMARK(vec4_mul_scalar_ptr, "iterations: 1000000 [vec4_add_ptr,vec4_sub_ptr,vec4_dot,vec4_mul_scalar_ptr]")
{
    const int iterations = 1000000;
    vector4 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f};
    vector4 res;

    for(int i = 0; i < iterations; i++) {
        const vector4 b = {.x = 5.0f, .y = 6.0f, .z = 7.0f, .w = 8.0f};
        vec4_add_ptr(&res, &a, &b);
        vec4_sub_ptr(&a, &res, &b);
        const vm_float_t dot = vec4_dot(a, b);
        vec4_mul_scalar_ptr(&res, &a, dot);
    }
}

BENCHMARK(vec4_normalize, "iterations: 1000000 [vec4_add,vec4_normalize,vec4_length,vec4_mul_scalar]")
{
    const int iterations = 1000000;
    vector4 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f};

    for(int i = 0; i < iterations; i++) {
        const vector4 b = {.x = 5.0f, .y = 6.0f, .z = 7.0f, .w = 8.0f};
        vector4 c = vec4_add(a, b);
        a = vec4_normalize(c);
        const vm_float_t len = vec4_length(a);
        c = vec4_mul_scalar(a, len);
    }
}

BENCHMARK(vec4_normalize_ptr, "iterations: 1000000 [vec4_add_ptr,vec4_normalize_ptr,vec4_length,vec4_mul_scalar_ptr]")
{
    const int iterations = 1000000;
    vector4 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f};
    vector4 res;

    for(int i = 0; i < iterations; i++) {
        const vector4 b = {.x = 5.0f, .y = 6.0f, .z = 7.0f, .w = 8.0f};
        vec4_add_ptr(&res, &a, &b);
        vec4_normalize_ptr(&a, &res);
        const vm_float_t len = vec4_length(a);
        vec4_mul_scalar_ptr(&res, &a, len);
    }
}

BENCHMARK(vec4_lerp, "iterations: 1000000 [vec4_lerp,vec4_add,vec4_length,vec4_mul_scalar]")
{
    const int iterations = 1000000;
    vector4 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f};

    for(int i = 0; i < iterations; i++) {
        const vector4 b = {.x = 5.0f, .y = 6.0f, .z = 7.0f, .w = 8.0f};
        const vector4 c = vec4_lerp(a, b, 0.5f);
        a = vec4_add(c, a);
        const vm_float_t len = vec4_length(c);
        a = vec4_mul_scalar(a, len);
    }
}

BENCHMARK(vec4_lerp_ptr, "iterations: 1000000 [vec4_lerp_ptr,vec4_add_ptr,vec4_length,vec4_mul_scalar_ptr]")
{
    const int iterations = 1000000;
    vector4 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f};
    const vector4 b = {.x = 5.0f, .y = 6.0f, .z = 7.0f, .w = 8.0f};
    vector4 res;

    for(int i = 0; i < iterations; i++) {
        vec4_lerp_ptr(&res, &a, &b, 0.5f);
        vec4_add_ptr(&a, &res, &a);
        const vm_float_t len = vec4_length(res);
        vec4_mul_scalar_ptr(&a, &a, len);
    }
}

/* Isolated SIMD kernels. One op per bench so dispatch wins are visible. */

BENCHMARK(vec4_add_ptr_simd, "iterations: 1000000 [simd,vec4_add_ptr]")
{
    const int iterations = 1000000;
    vector4 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f};
    const vector4 b = {.x = 0.5f, .y = -0.25f, .z = 1.5f, .w = 0.125f};
    vector4 res;

    for (int i = 0; i < iterations; i++) {
        vec4_add_ptr(&res, &a, &b);
        a = res;
    }
}

BENCHMARK(vec4_mul_ptr_simd, "iterations: 1000000 [simd,vec4_mul_ptr]")
{
    const int iterations = 1000000;
    vector4 a = {.x = 1.1f, .y = 0.9f, .z = 1.05f, .w = 0.95f};
    /* Unit-magnitude factors so the product stays finite across 1e6 iters. */
    const vector4 b = {.x = 1.0f, .y = -1.0f, .z = 1.0f, .w = -1.0f};
    vector4 res;

    for (int i = 0; i < iterations; i++) {
        vec4_mul_ptr(&res, &a, &b);
        a = res;
    }
}

BENCHMARK(vec4_div_ptr_simd, "iterations: 1000000 [simd,vec4_div_ptr,vec4_add_ptr]")
{
    const int iterations = 1000000;
    vector4 a = {.x = 8.0f, .y = 4.0f, .z = 2.0f, .w = 1.0f};
    const vector4 b = {.x = 1.1f, .y = 1.2f, .z = 1.3f, .w = 1.4f};
    vector4 res;

    for (int i = 0; i < iterations; i++) {
        vec4_div_ptr(&res, &a, &b);
        vec4_add_ptr(&a, &res, &b);
    }
}

BENCHMARK(vec4_mul_scalar_ptr_simd, "iterations: 1000000 [simd,vec4_mul_scalar_ptr]")
{
    const int iterations = 1000000;
    vector4 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f};
    vector4 res;

    for (int i = 0; i < iterations; i++) {
        vec4_mul_scalar_ptr(&res, &a, 1.000001f);
        a = res;
    }
}

BENCHMARK(vec4_normalize_ptr_simd, "iterations: 1000000 [simd,vec4_normalize_ptr,vec4_add_ptr]")
{
    const int iterations = 1000000;
    vector4 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f};
    vector4 res;

    for (int i = 0; i < iterations; i++) {
        vec4_normalize_ptr(&res, &a);
        vec4_add_ptr(&a, &res, &a);
    }
}

BENCHMARK(vec4_lerp_ptr_simd, "iterations: 1000000 [simd,vec4_lerp_ptr]")
{
    const int iterations = 1000000;
    vector4 a = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f};
    const vector4 b = {.x = 5.0f, .y = 6.0f, .z = 7.0f, .w = 8.0f};
    vector4 res;

    for (int i = 0; i < iterations; i++) {
        vec4_lerp_ptr(&res, &a, &b, 0.5f);
        a = res;
    }
}

BENCHMARK(vec4_min_max_ptr_simd, "iterations: 1000000 [simd,vec4_min_ptr,vec4_max_ptr]")
{
    const int iterations = 1000000;
    vector4 a = {.x = 1.0f, .y = -2.0f, .z = 3.0f, .w = -4.0f};
    const vector4 lo = {.x = -1.0f, .y = -1.0f, .z = -1.0f, .w = -1.0f};
    const vector4 hi = {.x = 1.0f, .y = 1.0f, .z = 1.0f, .w = 1.0f};
    vector4 res;

    for (int i = 0; i < iterations; i++) {
        vec4_min_ptr(&res, &a, &hi);
        vec4_max_ptr(&a, &res, &lo);
    }
}

BENCHMARK(vec4_clamp_ptr_simd, "iterations: 1000000 [simd,vec4_clamp_ptr,vec4_add_scalar_ptr]")
{
    const int iterations = 1000000;
    vector4 a = {.x = 2.0f, .y = -2.0f, .z = 0.5f, .w = 0.0f};
    const vector4 lo = {.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f};
    const vector4 hi = {.x = 1.0f, .y = 1.0f, .z = 1.0f, .w = 1.0f};
    vector4 res;

    for (int i = 0; i < iterations; i++) {
        vec4_clamp_ptr(&res, &a, &lo, &hi);
        vec4_add_scalar_ptr(&a, &res, 0.1f);
    }
}

BENCHMARK(vec4_round_family_ptr_simd, "iterations: 1000000 [simd,vec4_*]")
{
    const int iterations = 1000000;
    vector4 a = {.x = 1.25f, .y = -2.75f, .z = 3.5f, .w = -0.5f};
    vector4 res;

    for (int i = 0; i < iterations; i++) {
        vec4_floor_ptr(&res, &a);
        vec4_ceil_ptr(&a, &res);
        vec4_round_ptr(&res, &a);
        vec4_fract_ptr(&a, &res);
        vec4_add_scalar_ptr(&a, &a, 0.01f);
    }
}

BENCHMARK(vec4_homogenize_ptr_simd, "iterations: 1000000 [simd,vec4_homogenize_ptr,vec4_add_scalar_ptr]")
{
    const int iterations = 1000000;
    vector4 a = {.x = 2.0f, .y = 4.0f, .z = 6.0f, .w = 2.0f};
    vector4 res;

    for (int i = 0; i < iterations; i++) {
        vec4_homogenize_ptr(&res, &a);
        vec4_add_scalar_ptr(&a, &res, 0.001f);
        a.w = 2.0f;
    }
}
