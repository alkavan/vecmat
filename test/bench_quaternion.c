// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "unitest.h"

BENCHMARK(quat_normalize_ptr_simd, "iterations: 1000000 [simd,normalize_ptr,mul_ptr]")
{
    const int iterations = 1000000;
    quaternion a = {.x = 0.1f, .y = 0.2f, .z = 0.3f, .w = 0.9f};
    quaternion res;

    for (int i = 0; i < iterations; i++) {
        quat_normalize_ptr(&res, &a);
        quat_mul_ptr(&a, &res, &res);
    }
}

BENCHMARK(quat_mul, "iterations: 1000000 [quat_mul]")
{
    const int iterations = 1000000;
    quaternion a = {.x = 0.0f, .y = 0.70710678f, .z = 0.0f, .w = 0.70710678f};
    const quaternion b = {.x = 0.0f, .y = 0.0f, .z = 0.70710678f, .w = 0.70710678f};

    for (int i = 0; i < iterations; i++) {
        a = quat_mul(a, b);
    }
}

BENCHMARK(quat_slerp, "iterations: 1000000 [quat_slerp,quat_mul]")
{
    const int iterations = 1000000;
    const quaternion a = {.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f};
    const quaternion b = {.x = 0.0f, .y = 0.70710678f, .z = 0.0f, .w = 0.70710678f};
    quaternion res = a;

    for (int i = 0; i < iterations; i++) {
        res = quat_slerp(a, b, 0.35f);
        res = quat_mul(res, a);
    }
}

BENCHMARK(quat_nlerp, "iterations: 1000000 [quat_nlerp,quat_mul]")
{
    const int iterations = 1000000;
    const quaternion a = {.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f};
    const quaternion b = {.x = 0.0f, .y = 0.70710678f, .z = 0.0f, .w = 0.70710678f};
    quaternion res = a;

    for (int i = 0; i < iterations; i++) {
        res = quat_nlerp(a, b, 0.35f);
        res = quat_mul(res, a);
    }
}

BENCHMARK(quat_rotate_vec3, "iterations: 1000000 [quat_rotate_vec3]")
{
    const int iterations = 1000000;
    const quaternion q = {.x = 0.0f, .y = 0.70710678f, .z = 0.0f, .w = 0.70710678f};
    vector3 v = {.x = 1.0f, .y = 0.0f, .z = 0.0f};

    for (int i = 0; i < iterations; i++) {
        v = quat_rotate_vec3(q, v);
    }
}

BENCHMARK(quat_mul_ptr_simd, "iterations: 1000000 [simd,mul_ptr]")
{
    const int iterations = 1000000;
    quaternion a = {.x = 0.0f, .y = 0.70710678f, .z = 0.0f, .w = 0.70710678f};
    const quaternion b = {.x = 0.0f, .y = 0.0f, .z = 0.70710678f, .w = 0.70710678f};
    quaternion res;

    for (int i = 0; i < iterations; i++) {
        quat_mul_ptr(&res, &a, &b);
        a = res;
    }
}

