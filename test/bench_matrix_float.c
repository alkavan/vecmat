// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "unitest.h"

BENCHMARK(mat2_mul, "iterations: 1000000 [mat2_inverse]")
{
    const int iterations = 1000000;
    matrix2 a = {{
        .m11 = 1.0f, .m21 = 2.0f,
        .m12 = 3.0f, .m22 = 4.0f
    }};

    for(int i = 0; i < iterations; i++) {
        const matrix2 b = {{
            .m11 = 5.0f, .m21 = 6.0f,
            .m12 = 7.0f, .m22 = 8.0f
        }};
        matrix2 c = mat2_mul(a, b);
        a = mat2_mul(c, b);
        const matrix2 inv = mat2_inverse(a);
        c = mat2_mul(a, inv);
    }
}

BENCHMARK(mat3_mul, "iterations: 1000000 [mat3_inverse]")
{
    const int iterations = 1000000;
    matrix3 a = {{
        .m11 = 1.0f, .m21 = 2.0f, .m31 = 3.0f,
        .m12 = 4.0f, .m22 = 5.0f, .m32 = 6.0f,
        .m13 = 7.0f, .m23 = 8.0f, .m33 = 9.0f
    }};

    for(int i = 0; i < iterations; i++) {
        const matrix3 b = {{
            .m11 = 9.0f, .m21 = 8.0f, .m31 = 7.0f,
            .m12 = 6.0f, .m22 = 5.0f, .m32 = 4.0f,
            .m13 = 3.0f, .m23 = 2.0f, .m33 = 1.0f
        }};
        matrix3 c = mat3_mul(a, b);
        a = mat3_mul(c, b);
        const matrix3 inv = mat3_inverse(a);
        c = mat3_mul(a, inv);
    }
}

BENCHMARK(mat4_mul, "iterations: 1000000 [mul,inverse]")
{
    const int iterations = 1000000;
    matrix4 a = {{
        .m11 = 1.0f,  .m21 = 2.0f,  .m31 = 3.0f,  .m41 = 4.0f,
        .m12 = 5.0f,  .m22 = 6.0f,  .m32 = 7.0f,  .m42 = 8.0f,
        .m13 = 9.0f,  .m23 = 10.0f, .m33 = 11.0f, .m43 = 12.0f,
        .m14 = 13.0f, .m24 = 14.0f, .m34 = 15.0f, .m44 = 16.0f
    }};

    for(int i = 0; i < iterations; i++) {
        const matrix4 b = {{
            .m11 = 16.0f, .m21 = 15.0f, .m31 = 14.0f, .m41 = 13.0f,
            .m12 = 12.0f, .m22 = 11.0f, .m32 = 10.0f, .m42 = 9.0f,
            .m13 = 8.0f,  .m23 = 7.0f,  .m33 = 6.0f,  .m43 = 5.0f,
            .m14 = 4.0f,  .m24 = 3.0f,  .m34 = 2.0f,  .m44 = 1.0f
        }};
        matrix4 c = mat4_mul(a, b);
        a = mat4_mul(c, b);
        matrix4 inv = mat4_inverse(a);
        c = mat4_mul(a, inv);
    }
}

BENCHMARK(mat2_mul_ptr, "iterations: 1000000 [mul_ptr,inverse_ptr]")
{
    const int iterations = 1000000;
    matrix2 a = {{
        .m11 = 1.0f, .m21 = 2.0f,
        .m12 = 3.0f, .m22 = 4.0f
    }};
    const matrix2 b = {{
        .m11 = 5.0f, .m21 = 6.0f,
        .m12 = 7.0f, .m22 = 8.0f
    }};
    matrix2 res;

    for(int i = 0; i < iterations; i++) {
        mat2_mul_ptr(&res, &a, &b);
        mat2_mul_ptr(&a, &res, &b);
        mat2_inverse_ptr(&res, &a);
        mat2_mul_ptr(&res, &a, &res);
    }
}

BENCHMARK(mat3_mul_ptr, "iterations: 1000000)")
{
    const int iterations = 1000000;
    matrix3 a = {{
        .m11 = 1.0f, .m21 = 2.0f, .m31 = 3.0f,
        .m12 = 4.0f, .m22 = 5.0f, .m32 = 6.0f,
        .m13 = 7.0f, .m23 = 8.0f, .m33 = 9.0f
    }};
    const matrix3 b = {{
        .m11 = 9.0f, .m21 = 8.0f, .m31 = 7.0f,
        .m12 = 6.0f, .m22 = 5.0f, .m32 = 4.0f,
        .m13 = 3.0f, .m23 = 2.0f, .m33 = 1.0f
    }};
    matrix3 res;

    for(int i = 0; i < iterations; i++) {
        mat3_mul_ptr(&res, &a, &b);
        mat3_mul_ptr(&a, &res, &b);
        mat3_inverse_ptr(&res, &a);
        mat3_mul_ptr(&res, &a, &res);
    }
}

BENCHMARK(mat4_mul_ptr, "iterations: 1000000")
{
    const int iterations = 1000000;
    matrix4 a = {{
        .m11 = 1.0f,  .m21 = 2.0f,  .m31 = 3.0f,  .m41 = 4.0f,
        .m12 = 5.0f,  .m22 = 6.0f,  .m32 = 7.0f,  .m42 = 8.0f,
        .m13 = 9.0f, .m23 = 10.0f,  .m33 = 11.0f, .m43 = 12.0f,
        .m14 = 13.0f, .m24 = 14.0f, .m34 = 15.0f, .m44 = 16.0f
    }};
    const matrix4 b = {{
        .m11 = 16.0f, .m21 = 15.0f, .m31 = 14.0f, .m41 = 13.0f,
        .m12 = 12.0f, .m22 = 11.0f, .m32 = 10.0f, .m42 = 9.0f,
        .m13 = 8.0f,  .m23 = 7.0f,  .m33 = 6.0f,  .m43 = 5.0f,
        .m14 = 4.0f,  .m24 = 3.0f,  .m34 = 2.0f,  .m44 = 1.0f
    }};
    matrix4 res;

    for(int i = 0; i < iterations; i++) {
        mat4_mul_ptr(&res, &a, &b);
        mat4_mul_ptr(&a, &res, &b);
        mat4_inverse_ptr(&res, &a);
        mat4_mul_ptr(&res, &a, &res);
    }
}

BENCHMARK(mat4_transform_chain, "iterations: 1000000")
{
    const int iterations = 1000000;
    matrix4 model = mat4_identity();
    const matrix4 view  = mat4_look_at(
        (vector3){.x = 0.0f, .y = 0.0f, .z = 5.0f},
        (vector3){.x = 0.0f, .y = 0.0f, .z = 0.0f},
        (vector3){.x = 0.0f, .y = 1.0f, .z = 0.0f}
    );
    const matrix4 proj  = mat4_perspective(45.0f, 16.0f/9.0f, 0.1f, 100.0f);

    for(int i = 0; i < iterations; i++) {
        matrix4 mvp = mat4_mul(proj, mat4_mul(view, model));
        model = mat4_mul(model, mat4_rotation((vector3){.x = 0.0f, .y = 1.0f, .z = 0.0f}, 1.0f));
    }
}

BENCHMARK(mat4_transform_chain_ptr, "iterations: 1000000")
{
    const int iterations = 1000000;
    matrix4 model; mat4_identity_ptr(&model);
    matrix4 view;  mat4_look_at_ptr(&view,
        &(vector3){.x = 0.0f, .y = 0.0f, .z = 5.0f},
        &(vector3){.x = 0.0f, .y = 0.0f, .z = 0.0f},
        &(vector3){.x = 0.0f, .y = 1.0f, .z = 0.0f}
    );
    matrix4 proj;  mat4_perspective_ptr(&proj, 45.0f, 16.0f/9.0f, 0.1f, 100.0f);
    matrix4 temp, mvp;

    for(int i = 0; i < iterations; i++) {
        mat4_mul_ptr(&temp, &view, &model);
        mat4_mul_ptr(&mvp,  &proj, &temp);
        mat4_rotation_ptr(&temp, &(vector3){.x = 0.0f, .y = 1.0f, .z = 0.0f}, 1.0f);
        mat4_mul_ptr(&model, &model, &temp);
    }
}
