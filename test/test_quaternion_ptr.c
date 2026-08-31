// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "unitest.h"

TEST_CASE(quat_identity_ptr_test, "[quaternion_ptr]") {
    quaternion q;
    quat_identity_ptr(&q);
    REQUIRE(quat_eq(q, (quaternion){.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f}));
}

TEST_CASE(quat_mul_ptr_test, "[quaternion_ptr]") {
    const quaternion a = {.x = 1.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f};
    const quaternion b = {.x = 0.0f, .y = 1.0f, .z = 0.0f, .w = 0.0f};
    quaternion res;
    quat_mul_ptr(&res, &a, &b);
    REQUIRE(quat_eq(res, (quaternion){.x = 0.0f, .y = 0.0f, .z = 1.0f, .w = 0.0f})); // i * j = k
}

TEST_CASE(quat_normalize_ptr_test, "[quaternion_ptr]") {
    const quaternion q = {
        .x = VM_F(1.0), .y = VM_F(2.0), .z = VM_F(3.0), .w = VM_F(4.0)
    };
    quaternion res;
    quat_normalize_ptr(&res, &q);

    const vm_float_t len = VECMAT_SQRT(
        VM_F(1.0) + VM_F(4.0) + VM_F(9.0) + VM_F(16.0));

    REQUIRE(quat_eq(res, (quaternion){
        .x = VM_F(1.0) / len,
        .y = VM_F(2.0) / len,
        .z = VM_F(3.0) / len,
        .w = VM_F(4.0) / len
    }));
    REQUIRE(VECMAT_EQ(quat_normalize(res).w, res.w, EPSILON)); // Already normalized
}

TEST_CASE(quat_from_euler_ptr_test, "[quaternion_ptr]") {
    const vector3 euler = {.x = 0.0f, .y = 0.0f, .z = VM_RAD(M_PI_2)};
    quaternion q;
    quat_from_euler_ptr(&q, &euler);
    // 90 deg around Z: cos(45), 0,0,sin(45)
    REQUIRE(VECMAT_EQ(q.x, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(q.y, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(q.z, VECMAT_SIN(M_PI_4), EPSILON));
    REQUIRE(VECMAT_EQ(q.w, VECMAT_COS(M_PI_4), EPSILON));
}

TEST_CASE(quat_to_mat4_ptr_test, "[quaternion_ptr]") {
    quaternion q;
    quat_from_euler_ptr(&q, &(vector3){.x = 0.0f, .y = 0.0f, .z = VM_RAD(M_PI_2)});
    matrix4 m;
    quat_to_mat4_ptr(&m, &q);
    // Should match rotate Z 90
    REQUIRE(VECMAT_EQ(m.m11, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(m.m21, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(m.m12, -1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(m.m22, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(m.m33, 1.0f, EPSILON));
}

TEST_CASE(quat_rotate_and_slerp_ptr_test, "[quaternion_ptr]") {
    quaternion q;
    quat_from_axis_angle_ptr(&q, &(vector3){.x = 0.0f, .y = 0.0f, .z = 1.0f}, VM_RAD(M_PI_2));
    vector3 r;
    quat_rotate_vec3_ptr(&r, &q, &(vector3){.x = 1.0f, .y = 0.0f, .z = 0.0f});
    REQUIRE(vec3_near(r, (vector3){.x = 0.0f, .y = 1.0f, .z = 0.0f}, EPSILON));

    quaternion c, inv, id;
    quat_conjugate_ptr(&c, &q);
    quat_inverse_ptr(&inv, &q);
    quat_mul_ptr(&id, &q, &inv);
    REQUIRE(quat_near(id, quat_identity(), EPSILON));

    quaternion mid;
    const quaternion a = quat_identity();
    quat_slerp_ptr(&mid, &a, &q, 0.0f);
    REQUIRE(quat_near(mid, a, EPSILON));

    vector3 euler;
    quat_to_euler_ptr(&euler, &q);
    REQUIRE(VECMAT_EQ(euler.z, VM_RAD(M_PI_2), EPSILON));
}
