// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

#include "unitest.h"
#include <string.h>

// Test individual element access using v[] and named members
TEST_CASE(test_matrix_individual_element_access, "[common][matrix3][access]") {
    matrix3 mat;
    mat.v[0] = 1.0f;
    REQUIRE(VECMAT_EQ(mat.v[0], 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(mat.m11, 1.0f, EPSILON));  // Equivalent to mat.v[0]

    mat.m21 = 2.0f;
    REQUIRE(VECMAT_EQ(mat.v[1], 2.0f, EPSILON));  // Equivalent to mat.m21
    REQUIRE(VECMAT_EQ(mat.m21, 2.0f, EPSILON));
}

// Test matrix initialization methods
TEST_CASE(test_matrix_initialization, "[common][matrix3][init]") {
    // Named members
    const matrix3 mat1 = {
        .m11 = 1.0f, .m21 = 0.0f, .m31 = 0.0f,
        .m12 = 0.0f, .m22 = 1.0f, .m32 = 0.0f,
        .m13 = 0.0f, .m23 = 0.0f, .m33 = 1.0f
    };
    REQUIRE(VECMAT_EQ(mat1.m11, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(mat1.m22, 1.0f, EPSILON));

    // Zero with memset
    matrix3 mat2;
    memset(mat2.v, 0, sizeof(mat2.v));
    REQUIRE(VECMAT_EQ(mat2.v[0], 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(mat2.v[8], 0.0f, EPSILON));

    // Array initializer
    const matrix3 mat3 = { .v = {1,0,0, 0,1,0, 0,0,1} };
    REQUIRE(VECMAT_EQ(mat3.v[0], 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(mat3.v[4], 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(mat3.v[8], 1.0f, EPSILON));
}

// Test accessing matrix elements, including determinant
TEST_CASE(test_common_matrix_access_and_determinant, "[common][matrix3][access,determinant]") {
    // Simple diagonal matrix
    matrix3 mat = { .v = {2,0,0, 0,3,0, 0,0,4} };

    // Accessing by name
    REQUIRE(VECMAT_EQ(mat.m11, 2.0f, EPSILON));
    REQUIRE(VECMAT_EQ(mat.m22, 3.0f, EPSILON));
    REQUIRE(VECMAT_EQ(mat.m33, 4.0f, EPSILON));

    // Determinant function
    const vm_float_t det = mat.m11 * (mat.m22 * mat.m33 - mat.m23 * mat.m32)
              - mat.m12 * (mat.m21 * mat.m33 - mat.m23 * mat.m31)
              + mat.m13 * (mat.m21 * mat.m32 - mat.m22 * mat.m31);
    REQUIRE(VECMAT_EQ(det, VM_F(24.0), EPSILON));  // 2*3*4 = 24

    // Accessing by index and scaling
    for (int i = 0; i < 9; i++) {
        mat.v[i] *= 2.0f;
    }
    REQUIRE(VECMAT_EQ(mat.m11, 4.0f, EPSILON));
}

// Test vector operations: transform and translate
TEST_CASE(test_common_transform_and_translate, "[common][matrix3,vector3][transform,translate]") {
    const matrix3 rot = { .v = {0, 1, 0, -1, 0, 0, 0, 0, 1} };  // 90-degree rotation around Z (column-major)
    const vector3 vec = {.x = 1.0f, .y = 0.0f, .z = 0.0f};
    vector3 out;

    // Transform
    out.x = rot.m11 * vec.x + rot.m12 * vec.y + rot.m13 * vec.z;
    out.y = rot.m21 * vec.x + rot.m22 * vec.y + rot.m23 * vec.z;
    out.z = rot.m31 * vec.x + rot.m32 * vec.y + rot.m33 * vec.z;
    REQUIRE(VECMAT_EQ(out.x, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(out.y, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(out.z, 0.0f, EPSILON));

    // Translate
    const vector3 translation = {.x = 0.0f, .y = 1.0f, .z = 0.0f};
    vector3 translated;
    translated.x = vec.x + translation.x;
    translated.y = vec.y + translation.y;
    translated.z = vec.z + translation.z;
    REQUIRE(VECMAT_EQ(translated.x, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(translated.y, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(translated.z, 0.0f, EPSILON));
}

// Test matrix operations: multiply and affine_matrix
TEST_CASE(test_common_matrix_multiply_affine, "[common][matrix3,matrix4][multiply,affine]") {
    const matrix3 a = { .v = {1,2,3, 4,5,6, 7,8,9} };
    const matrix3 b = { .v = {9,8,7, 6,5,4, 3,2,1} };
    matrix3 result;

    /* Column-major GEMM: C[col*3 + row] = sum_k A[k*3 + row] * B[col*3 + k] */
    for (int c = 0; c < 3; c++) {
        for (int r = 0; r < 3; r++) {
            vm_float_t sum = 0.0f;
            for (int k = 0; k < 3; k++) {
                sum += a.v[k * 3 + r] * b.v[c * 3 + k];
            }
            result.v[c * 3 + r] = sum;
        }
    }
    REQUIRE(VECMAT_EQ(result.v[0], 90.0f, EPSILON));  /* (AB) col0 row0 */
    REQUIRE(VECMAT_EQ(result.v[4], 69.0f, EPSILON));  /* (AB) col1 row1 */

    // Affine matrix
    const vector3 trans = {.x = 10.0f, .y = 20.0f, .z = 30.0f};
    matrix4 affine;
    affine.m11 = a.m11; affine.m21 = a.m21; affine.m31 = a.m31; affine.m41 = 0.0f;
    affine.m12 = a.m12; affine.m22 = a.m22; affine.m32 = a.m32; affine.m42 = 0.0f;
    affine.m13 = a.m13; affine.m23 = a.m23; affine.m33 = a.m33; affine.m43 = 0.0f;
    affine.m14 = trans.x; affine.m24 = trans.y; affine.m34 = trans.z; affine.m44 = 1.0f;
    REQUIRE(VECMAT_EQ(affine.m14, 10.0f, EPSILON));
    REQUIRE(VECMAT_EQ(affine.m44, 1.0f, EPSILON));
}
