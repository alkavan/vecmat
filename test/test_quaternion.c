// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "unitest.h"

// Quaternion tests
TEST_CASE(quat_identity_test, "[quaternion]") {
    const quaternion q = quat_identity();
    REQUIRE(quat_eq(q, (quaternion){.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f}));
}

TEST_CASE(quat_mul_test, "[quaternion]") {
    const quaternion a = {.x = 1.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f};
    const quaternion b = {.x = 0.0f, .y = 1.0f, .z = 0.0f, .w = 0.0f};
    const quaternion res = quat_mul(a, b);
    REQUIRE(quat_eq(res, (quaternion){.x = 0.0f, .y = 0.0f, .z = 1.0f, .w = 0.0f})); // i * j = k
}

TEST_CASE(quat_normalize_test, "[quaternion]") {
    const quaternion q = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f};
    const quaternion res = quat_normalize(q);
    const vm_float_t len = VECMAT_SQRT(1+4+9+16);
    REQUIRE(quat_eq(res, (quaternion){.x = 1/len, .y = 2/len, .z = 3/len, .w = 4/len}));
    REQUIRE(VECMAT_EQ(quat_normalize(res).w, res.w, EPSILON)); // Already normalized
}

TEST_CASE(quat_from_euler_test, "[quaternion]") {
    const vector3 euler = {.x = 0.0f, .y = 0.0f, .z = VM_RAD(M_PI_2)};
    const quaternion q = quat_from_euler(euler);
    // 90 deg around Z: cos(45), 0,0,sin(45)
    REQUIRE(VECMAT_EQ(q.x, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(q.y, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(q.z, VECMAT_SIN(M_PI_4), EPSILON));
    REQUIRE(VECMAT_EQ(q.w, VECMAT_COS(M_PI_4), EPSILON));
}

TEST_CASE(quat_to_mat4_test, "[quaternion]") {
    const quaternion q = quat_from_euler((vector3){.x = 0.0f, .y = 0.0f, .z = VM_RAD(M_PI_2)});
    const matrix4 m = quat_to_mat4(q);
    // Should match rotate Z 90
    REQUIRE(VECMAT_EQ(m.m11, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(m.m21, 1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(m.m12, -1.0f, EPSILON));
    REQUIRE(VECMAT_EQ(m.m22, 0.0f, EPSILON));
    REQUIRE(VECMAT_EQ(m.m33, 1.0f, EPSILON));
}

TEST_CASE(quat_conjugate_inverse, "[quaternion]") {
    const quaternion q = quat_from_axis_angle((vector3){.x = 0.0f, .y = 0.0f, .z = 1.0f}, VM_RAD(M_PI_2));
    const quaternion c = quat_conjugate(q);
    REQUIRE(VECMAT_EQ(c.z, -q.z, EPSILON));
    REQUIRE(VECMAT_EQ(c.w, q.w, EPSILON));
    const quaternion id = quat_mul(q, quat_inverse(q));
    REQUIRE(quat_near(id, quat_identity(), EPSILON));
}

TEST_CASE(quat_axis_angle_roundtrip, "[quaternion]") {
    const vector3 axis_in = {.x = 0.0f, .y = 0.0f, .z = 1.0f};
    const vm_float_t rad_in = VM_RAD(M_PI_2);

    const quaternion q = quat_from_axis_angle(axis_in, rad_in);

    vm_float_t radians = 0.0f;
    const vector3 axis = quat_to_axis_angle(q, &radians);
    const quaternion q2 = quat_from_axis_angle(axis, radians);

    /* same rotation (q and -q are equal orientations) */
    REQUIRE(quat_near(q2, q, EPSILON) ||
            quat_near(q2, (quaternion){.x = -q.x, .y = -q.y, .z = -q.z, .w = -q.w}, EPSILON));
}

TEST_CASE(quat_rotate_vec3_z90, "[quaternion]") {
    const quaternion q = quat_from_axis_angle((vector3){.x = 0.0f, .y = 0.0f, .z = 1.0f}, VM_RAD(M_PI_2));
    const vector3 r = quat_rotate_vec3(q, (vector3){.x = 1.0f, .y = 0.0f, .z = 0.0f});
    REQUIRE(vec3_near(r, (vector3){.x = 0.0f, .y = 1.0f, .z = 0.0f}, EPSILON));
}

TEST_CASE(quat_euler_roundtrip, "[quaternion]") {
    const vector3 e = {.x = VM_RAD(0.0), .y = VM_RAD(0.0), .z = VM_RAD(M_PI_2)};
    const quaternion q = quat_from_euler(e);
    const vector3 back = quat_to_euler(q);
    const quaternion q2 = quat_from_euler(back);

    /* same rotation: q2 ≈ q or q2 ≈ -q */
    REQUIRE(quat_near(q2, q, VECMAT_EPSILON) ||
            quat_near(q2, quat_mul(q, (quaternion){.x = -q.x, .y = -q.y, .z = -q.z, .w = -q.w}), EPSILON));
}

TEST_CASE(quat_mat_roundtrip, "[quaternion]") {
    const quaternion q = quat_from_axis_angle((vector3){.x = 0.0f, .y = 1.0f, .z = 0.0f}, VM_DEG(45));
    const quaternion q2 = quat_from_mat4(quat_to_mat4(q));
    const bool same = quat_near(q, q2, EPSILON);
    const bool flipped = quat_near(q, (quaternion){.x = -q2.x, .y = -q2.y, .z = -q2.z, .w = -q2.w}, EPSILON);
    REQUIRE(same || flipped);
}

TEST_CASE(quat_to_mat3_dot, "[quaternion]") {
    const quaternion q = quat_from_axis_angle((vector3){.x = 0.0f, .y = 0.0f, .z = 1.0f}, VM_RAD(M_PI_2));
    const matrix3 m = quat_to_mat3(q);
    REQUIRE(vec3_near(mat3_mul_vec3(m, (vector3){.x = 1.0f, .y = 0.0f, .z = 0.0f}),
        (vector3){.x = 0.0f, .y = 1.0f, .z = 0.0f}, EPSILON));
    const quaternion q2 = quat_from_mat3(m);
    const bool same = quat_near(q, q2, EPSILON);
    const bool flipped = quat_near(q, (quaternion){.x = -q2.x, .y = -q2.y, .z = -q2.z, .w = -q2.w}, EPSILON);
    REQUIRE(same || flipped);
    REQUIRE(VECMAT_EQ(quat_dot(q, q), 1.0f, EPSILON));
}

TEST_CASE(quat_slerp_nlerp, "[quaternion]") {
    const quaternion a = quat_identity();
    const quaternion b = quat_from_axis_angle((vector3){.x = 0.0f, .y = 0.0f, .z = 1.0f}, VM_RAD(M_PI_2));
    const quaternion mid = quat_slerp(a, b, 0.5f);
    REQUIRE(VECMAT_EQ(vec3_length(quat_rotate_vec3(mid, (vector3){.x = 1.0f, .y = 0.0f, .z = 0.0f})), 1.0f, EPSILON));
    const quaternion n = quat_nlerp(a, b, 0.0f);
    REQUIRE(quat_near(n, a, EPSILON));
}

TEST_CASE(quat_look_aims_local_neg_z, "[quaternion]") {
    const vector3 dir = { .x = VM_F(0.0), .y = VM_F(0.0), .z = VM_F(-1.0) };
    const vector3 up = { .x = VM_F(0.0), .y = VM_F(1.0), .z = VM_F(0.0) };
    const quaternion q = quat_look(dir, up);
    const vector3 aimed = quat_rotate_vec3(q, (vector3){ .x = 0, .y = 0, .z = VM_F(-1.0) });
    REQUIRE(vec3_near(aimed, vec3_normalize(dir), EPSILON));

    const vector3 dir2 = { .x = VM_F(1.0), .y = VM_F(0.0), .z = VM_F(0.0) };
    const quaternion q2 = quat_look(dir2, up);
    const vector3 aimed2 = quat_rotate_vec3(q2, (vector3){ .x = 0, .y = 0, .z = VM_F(-1.0) });
    REQUIRE(vec3_near(aimed2, vec3_normalize(dir2), EPSILON));
}

TEST_CASE(quat_from_to_rotates_vector, "[quaternion]") {
    const vector3 from = { .x = VM_F(1.0), .y = 0, .z = 0 };
    const vector3 to = { .x = 0, .y = VM_F(1.0), .z = 0 };
    const quaternion q = quat_from_to(from, to);
    REQUIRE(vec3_near(quat_rotate_vec3(q, from), to, EPSILON));

    const quaternion id = quat_from_to(from, from);
    REQUIRE(quat_near(id, quat_identity(), EPSILON));

    const vector3 opp = { .x = VM_F(-1.0), .y = 0, .z = 0 };
    const quaternion flip = quat_from_to(from, opp);
    REQUIRE(vec3_near(quat_rotate_vec3(flip, from), opp, EPSILON));
}
