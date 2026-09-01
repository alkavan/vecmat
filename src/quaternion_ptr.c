// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "features/cpu.h"

/**
 * @brief Sets the quaternion to the identity quaternion (x=0, y=0, z=0, w=1).
 *
 * @param res Pointer to the quaternion to initialize.
 */
void quat_identity_ptr(quaternion *res)
{
    res->x = 0.0f;
    res->y = 0.0f;
    res->z = 0.0f;
    res->w = 1.0f;
}

/**
 * @brief Multiplies two quaternions using Hamilton product (a * b) and stores
 * the result in res.
 *
 * @param res Pointer to the output quaternion.
 * @param a Pointer to the first quaternion.
 * @param b Pointer to the second quaternion.
 */
VECMAT_SCALAR_API void quat_mul_ptr_scalar(quaternion *res, const quaternion *a, const quaternion *b)
{
    res->w = a->w * b->w - a->x * b->x - a->y * b->y - a->z * b->z;
    res->x = a->w * b->x + a->x * b->w + a->y * b->z - a->z * b->y;
    res->y = a->w * b->y - a->x * b->z + a->y * b->w + a->z * b->x;
    res->z = a->w * b->z + a->x * b->y - a->y * b->x + a->z * b->w;
}

/**
 * @brief Hamilton product `a * b` (dispatched).
 *
 * @see quat_mul_ptr_scalar
 *
 * @param res Result quaternion.
 * @param a Left quaternion.
 * @param b Right quaternion.
 */
void quat_mul_ptr(quaternion *res, const quaternion *a, const quaternion *b)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    quat_mul_ptr_(res, a, b);
#else
    quat_mul_ptr_scalar(res, a, b);
#endif
}

/**
 * @brief Normalizes the input quaternion and stores the result in res.
 *
 * If the quaternion length is zero, copies the input unchanged.
 *
 * @param res Pointer to the output quaternion.
 * @param q Pointer to the input quaternion.
 */
VECMAT_SCALAR_API void quat_normalize_ptr_scalar(quaternion *res, const quaternion *q)
{
    const vm_float_t len = VECMAT_SQRT(q->x * q->x + q->y * q->y + q->z * q->z + q->w * q->w);
    if (len > 0.0f) {
        const vm_float_t inv_len = 1.0f / len;
        res->x = q->x * inv_len;
        res->y = q->y * inv_len;
        res->z = q->z * inv_len;
        res->w = q->w * inv_len;
    } else {
        res->x = q->x;
        res->y = q->y;
        res->z = q->z;
        res->w = q->w;
    }
}

/**
 * @brief Normalizes a quaternion (dispatched).
 *
 * @see quat_normalize_ptr_scalar
 *
 * @param res Result quaternion.
 * @param q Input quaternion.
 */
void quat_normalize_ptr(quaternion *res, const quaternion *q)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    quat_normalize_ptr_(res, q);
#else
    quat_normalize_ptr_scalar(res, q);
#endif
}

/**
 * @brief Converts Euler angles (in radians) to a normalized quaternion.
 *
 * Computes a quaternion from the given Euler rotation vector using the
 * XYZ (Tait-Bryan) convention. The resulting quaternion is guaranteed
 * to be normalized.
 *
 * @param res Pointer to the quaternion that will receive the result.
 * @param euler Pointer to a vector3 containing the Euler angles in radians (x, y, z).
 */
void quat_from_euler_ptr(quaternion *res, const vector3 *euler)
{
    const vm_float_t rad_x = euler->x * VM_F(0.5);
    const vm_float_t rad_y = euler->y * VM_F(0.5);
    const vm_float_t rad_z = euler->z * VM_F(0.5);

    const vm_float_t cx = VECMAT_COS(rad_x), sx = VECMAT_SIN(rad_x);
    const vm_float_t cy = VECMAT_COS(rad_y), sy = VECMAT_SIN(rad_y);
    const vm_float_t cz = VECMAT_COS(rad_z), sz = VECMAT_SIN(rad_z);

    const quaternion temp = {
        .x = sx * cy * cz - cx * sy * sz,
        .y = cx * sy * cz + sx * cy * sz,
        .z = cx * cy * sz - sx * sy * cz,
        .w = cx * cy * cz + sx * sy * sz
    };
    quat_normalize_ptr(res, &temp);
}

/**
 * @brief Converts Euler angles in degrees to a quaternion.
 *
 * @see quat_from_euler_ptr
 *
 * @param res Pointer to the quaternion to store the result.
 * @param euler_deg Pointer to the vector containing Euler angles in degrees.
 */
void quat_from_euler_deg_ptr(quaternion *res, const vector3 *euler_deg)
{
    const vector3 rad = {
        .x = deg_to_rad(euler_deg->x),
        .y = deg_to_rad(euler_deg->y),
        .z = deg_to_rad(euler_deg->z)
    };
    quat_from_euler_ptr(res, &rad);
}

/**
 * @brief Converts a unit quaternion to a 4x4 rotation matrix and stores in res.
 *
 * Starts with the identity matrix and applies rotation components.
 *
 * @param res Pointer to the output matrix4.
 * @param q Pointer to the input quaternion (should be normalized).
 */
void quat_to_mat4_ptr(matrix4 *res, const quaternion *q)
{
    mat4_identity_ptr(res);

    const vm_float_t xx = q->x * q->x;
    const vm_float_t yy = q->y * q->y;
    const vm_float_t zz = q->z * q->z;
    const vm_float_t xy = q->x * q->y;
    const vm_float_t xz = q->x * q->z;
    const vm_float_t yz = q->y * q->z;
    const vm_float_t wx = q->w * q->x;
    const vm_float_t wy = q->w * q->y;
    const vm_float_t wz = q->w * q->z;

    res->m11 = 1.0f - 2.0f * (yy + zz);
    res->m12 = 2.0f * (xy - wz);
    res->m13 = 2.0f * (xz + wy);
    res->m21 = 2.0f * (xy + wz);
    res->m22 = 1.0f - 2.0f * (xx + zz);
    res->m23 = 2.0f * (yz - wx);
    res->m31 = 2.0f * (xz - wy);
    res->m32 = 2.0f * (yz + wx);
    res->m33 = 1.0f - 2.0f * (xx + yy);
}

/**
 * @brief Writes the conjugate of a quaternion.
 *
 * @param res Output value.
 * @param q Input quaternion.
 */
void quat_conjugate_ptr(quaternion *res, const quaternion *q)
{
    res->x = -q->x;
    res->y = -q->y;
    res->z = -q->z;
    res->w = q->w;
}

/**
 * @brief Writes the inverse of a quaternion.
 *
 * @param res Output value.
 * @param q Input quaternion.
 */
void quat_inverse_ptr(quaternion *res, const quaternion *q)
{
    const vm_float_t n2 = q->x * q->x + q->y * q->y + q->z * q->z + q->w * q->w;
    if (n2 <= 0.0f) {
        *res = *q;
        return;
    }
    const vm_float_t inv = 1.0f / n2;
    res->x = -q->x * inv;
    res->y = -q->y * inv;
    res->z = -q->z * inv;
    res->w = q->w * inv;
}

/**
 * @brief Converts an axis-angle rotation to a quaternion.
 *
 * The axis vector is normalized internally. The angle is given in radians.
 *
 * @param res Pointer to the quaternion that will receive the result.
 * @param axis Pointer to the rotation axis vector.
 * @param radians Rotation angle around the axis in radians.
 */
void quat_from_axis_angle_ptr(quaternion *res, const vector3 *axis, const vm_float_t radians)
{
    const vector3 n = vec3_normalize(*axis);
    const vm_float_t half = radians * VM_F(0.5);
    const vm_float_t s = VECMAT_SIN(half);
    res->x = n.x * s;
    res->y = n.y * s;
    res->z = n.z * s;
    res->w = VECMAT_COS(half);
}

/**
 * @brief Converts an axis-angle rotation (in degrees) to a quaternion.
 *
 * @param res Pointer to the quaternion that will receive the result.
 * @param axis Pointer to the rotation axis vector.
 * @param degrees Rotation angle around the axis in degrees.
 */
void quat_from_axis_angle_deg_ptr(quaternion *res, const vector3 *axis, const vm_float_t degrees)
{
    quat_from_axis_angle_ptr(res, axis, deg_to_rad(degrees));
}

/**
 * @brief Builds a quaternion from a 3x3 rotation matrix.
 *
 * @param res Output value.
 * @param m Input matrix.
 */
void quat_from_mat3_ptr(quaternion *res, const matrix3 *m)
{
    const vm_float_t trace = m->m11 + m->m22 + m->m33;
    if (trace > 0.0f) {
        const vm_float_t s = VECMAT_SQRT(trace + 1.0f) * 2.0f;
        res->w = 0.25f * s;
        res->x = (m->m32 - m->m23) / s;
        res->y = (m->m13 - m->m31) / s;
        res->z = (m->m21 - m->m12) / s;
    } else if (m->m11 > m->m22 && m->m11 > m->m33) {
        const vm_float_t s = VECMAT_SQRT(1.0f + m->m11 - m->m22 - m->m33) * 2.0f;
        res->w = (m->m32 - m->m23) / s;
        res->x = 0.25f * s;
        res->y = (m->m12 + m->m21) / s;
        res->z = (m->m13 + m->m31) / s;
    } else if (m->m22 > m->m33) {
        const vm_float_t s = VECMAT_SQRT(1.0f + m->m22 - m->m11 - m->m33) * 2.0f;
        res->w = (m->m13 - m->m31) / s;
        res->x = (m->m12 + m->m21) / s;
        res->y = 0.25f * s;
        res->z = (m->m23 + m->m32) / s;
    } else {
        const vm_float_t s = VECMAT_SQRT(1.0f + m->m33 - m->m11 - m->m22) * 2.0f;
        res->w = (m->m21 - m->m12) / s;
        res->x = (m->m13 + m->m31) / s;
        res->y = (m->m23 + m->m32) / s;
        res->z = 0.25f * s;
    }
    quat_normalize_ptr(res, res);
}

/**
 * @brief Builds a quaternion from the rotation of a 4x4 matrix.
 *
 * @param res Output value.
 * @param m Input matrix.
 */
void quat_from_mat4_ptr(quaternion *res, const matrix4 *m)
{
    const matrix3 m3 = {
        .m11 = m->m11, .m21 = m->m21, .m31 = m->m31,
        .m12 = m->m12, .m22 = m->m22, .m32 = m->m32,
        .m13 = m->m13, .m23 = m->m23, .m33 = m->m33
    };
    quat_from_mat3_ptr(res, &m3);
}

/**
 * @brief Normalized-linearly interpolates from a to b by t.
 *
 * @param res Output value.
 * @param a First input quaternion.
 * @param b Second input quaternion.
 * @param t Interpolation factor.
 */
void quat_nlerp_ptr(quaternion *res, const quaternion *a, const quaternion *b, const vm_float_t t)
{
    quaternion bb = *b;
    if (quat_dot(*a, *b) < 0.0f) {
        bb.x = -bb.x;
        bb.y = -bb.y;
        bb.z = -bb.z;
        bb.w = -bb.w;
    }
    const quaternion tmp = {
        .x = a->x + t * (bb.x - a->x),
        .y = a->y + t * (bb.y - a->y),
        .z = a->z + t * (bb.z - a->z),
        .w = a->w + t * (bb.w - a->w)
    };
    quat_normalize_ptr(res, &tmp);
}

/**
 * @brief Spherical-linearly interpolates from a to b by t.
 *
 * @param res Output value.
 * @param a First input quaternion.
 * @param b Second input quaternion.
 * @param t Interpolation factor.
 */
void quat_slerp_ptr(quaternion *res, const quaternion *a, const quaternion *b, const vm_float_t t)
{
    vm_float_t d = quat_dot(*a, *b);
    quaternion bb = *b;
    if (d < 0.0f) {
        d = -d;
        bb.x = -bb.x;
        bb.y = -bb.y;
        bb.z = -bb.z;
        bb.w = -bb.w;
    }
    if (d > 0.9995f) {
        quat_nlerp_ptr(res, a, &bb, t);
        return;
    }
    const vm_float_t theta = VECMAT_ACOS(d);
    const vm_float_t s = VECMAT_SIN(theta);
    const vm_float_t wa = VECMAT_SIN((1.0f - t) * theta) / s;
    const vm_float_t wb = VECMAT_SIN(t * theta) / s;
    res->x = a->x * wa + bb.x * wb;
    res->y = a->y * wa + bb.y * wb;
    res->z = a->z * wa + bb.z * wb;
    res->w = a->w * wa + bb.w * wb;
}

/**
 * @brief Rotates a vector3 by a quaternion.
 *
 * @param res Output value.
 * @param q Input quaternion.
 * @param v Input vector.
 */
void quat_rotate_vec3_ptr(vector3 *res, const quaternion *q, const vector3 *v)
{
    const quaternion p = {.x = v->x, .y = v->y, .z = v->z, .w = 0.0f};
    quaternion qi;
    quat_inverse_ptr(&qi, q);
    quaternion qp;
    quat_mul_ptr(&qp, q, &p);
    quaternion out;
    quat_mul_ptr(&out, &qp, &qi);
    res->x = out.x;
    res->y = out.y;
    res->z = out.z;
}

/**
 * @brief Converts a quaternion to Euler angles in degrees (XYZ).
 *
 * @param res Output value.
 * @param q Input quaternion.
 */
void quat_to_euler_ptr(vector3 *res, const quaternion *q)
{
    const vm_float_t x = q->x;
    const vm_float_t y = q->y;
    const vm_float_t z = q->z;
    const vm_float_t w = q->w;

    const vm_float_t sinr_cosp = VM_F(2.0) * (w * x + y * z);
    const vm_float_t cosr_cosp = VM_F(1.0) - VM_F(2.0) * (x * x + y * y);
    res->x = VECMAT_ATAN2(sinr_cosp, cosr_cosp);

    const vm_float_t sinp = VM_F(2.0) * (w * y - z * x);
    if (VECMAT_FABS(sinp) >= VM_F(1.0)) {
        res->y = VECMAT_COPYSIGN(M_PI_2, sinp);
    } else {
        res->y = VECMAT_ASIN(sinp);
    }

    const vm_float_t siny_cosp = VM_F(2.0) * (w * z + x * y);
    const vm_float_t cosy_cosp = VM_F(1.0) - VM_F(2.0) * (y * y + z * z);
    res->z = VECMAT_ATAN2(siny_cosp, cosy_cosp);
}

/**
 * @brief Converts a quaternion to XYZ Euler angles in degrees.
 *
 * @see quat_to_euler_ptr
 *
 * @param res Euler angles in degrees (x, y, z).
 * @param q Input quaternion.
 */
void quat_to_euler_deg_ptr(vector3 *res, const quaternion *q)
{
    quat_to_euler_ptr(res, q);
    res->x = rad_to_deg(res->x);
    res->y = rad_to_deg(res->y);
    res->z = rad_to_deg(res->z);
}

/**
 * @brief Converts a quaternion to an axis-angle representation.
 *
 * The quaternion is first normalized. The axis is stored in the provided
 * vector3 pointer. The rotation angle in radians is optionally written to
 * the radians pointer if it is not NULL. When the axis cannot be uniquely
 * determined (near zero rotation) the axis is set to (1, 0, 0).
 *
 * @param axis Pointer to the vector3 that will receive the rotation axis.
 * @param radians Pointer to a float that will receive the rotation angle in radians, or NULL.
 * @param q Pointer to the input quaternion.
 */
void quat_to_axis_angle_ptr(vector3 *axis, vm_float_t *radians, const quaternion *q)
{
    quaternion n;
    quat_normalize_ptr(&n, q);
    const vm_float_t w = (n.w > 1.0f) ? 1.0f : (n.w < -1.0f) ? -1.0f : n.w;
    const vm_float_t angle = VM_F(2.0) * VECMAT_ACOS(w);
    const vm_float_t s = VECMAT_SQRT(1.0f - w * w);
    if (s < VECMAT_EPSILON) {
        axis->x = 1.0f;
        axis->y = 0.0f;
        axis->z = 0.0f;
    } else {
        axis->x = n.x / s;
        axis->y = n.y / s;
        axis->z = n.z / s;
    }
    if (radians) {
        *radians = angle;
    }
}

/**
 * @brief Converts a quaternion to an axis-angle representation, with the angle in degrees.
 *
 * Extracts the rotation axis and rotation angle (in degrees) equivalent to the
 * given quaternion. The quaternion is first normalized internally. If the
 * quaternion represents no rotation, the axis is set to (1, 0, 0).
 *
 * @param axis Pointer to the vector3 that will receive the normalized rotation axis.
 * @param degrees Pointer to a float that will receive the rotation angle in degrees (may be NULL).
 * @param q Pointer to the source quaternion.
 */
void quat_to_axis_angle_deg_ptr(vector3 *axis, vm_float_t *degrees, const quaternion *q)
{
    vm_float_t radians = VM_F(0.0);
    quat_to_axis_angle_ptr(axis, &radians, q);
    if (degrees) {
        *degrees = rad_to_deg(radians);
    }
}

/**
 * @brief Converts a quaternion to a 3x3 rotation matrix.
 *
 * @param res Output value.
 * @param q Input quaternion.
 */
void quat_to_mat3_ptr(matrix3 *res, const quaternion *q)
{
    matrix4 m4;
    quat_to_mat4_ptr(&m4, q);
    res->m11 = m4.m11; res->m21 = m4.m21; res->m31 = m4.m31;
    res->m12 = m4.m12; res->m22 = m4.m22; res->m32 = m4.m32;
    res->m13 = m4.m13; res->m23 = m4.m23; res->m33 = m4.m33;
}

/**
 * @brief Integrates angular velocity over a time step and applies the
 * resulting rotation to the input quaternion.
 *
 * Computes a delta quaternion from the angular velocity and timestep,
 * multiplies it with the source quaternion and normalizes the result.
 *
 * @param res Pointer to the quaternion where the integrated result is stored.
 * @param q Pointer to the source quaternion.
 * @param omega Pointer to the angular velocity vector (in radians per second).
 * @param dt Time step (in seconds).
 */
void quat_integrate_ptr(quaternion *res, const quaternion *q, const vector3 *omega, const vm_float_t dt)
{
    const vm_float_t omega_len = vec3_length(*omega);
    const vm_float_t angle = omega_len * dt;
    quaternion dq;

    if (angle < VM_F(1e-8)) {
        dq.x = omega->x * dt * VM_F(0.5);
        dq.y = omega->y * dt * VM_F(0.5);
        dq.z = omega->z * dt * VM_F(0.5);
        dq.w = VM_F(1.0);
        quat_normalize_ptr(&dq, &dq);
    } else {
        const vm_float_t half = angle * VM_F(0.5);
        const vm_float_t s = VECMAT_SIN(half) / omega_len;
        dq.x = omega->x * s;
        dq.y = omega->y * s;
        dq.z = omega->z * s;
        dq.w = VECMAT_COS(half);
    }

    quat_mul_ptr(res, q, &dq);
    quat_normalize_ptr(res, res);
}

/**
 * @brief Orthonormal basis that aims along `direction`.
 *
 * @param r Output rotation (columns: right, up, +/-forward).
 * @param direction Look direction.
 * @param up World up hint.
 * @param left_handed Non-zero for a left-handed basis.
 */
static void quat_look_basis(matrix3 *r, const vector3 *direction, const vector3 *up,
                            const int left_handed)
{
    vector3 forward = *direction;
    if (vec3_length_squared(forward) <= VECMAT_EPSILON * VECMAT_EPSILON) {
        forward.x = VM_F(0.0);
        forward.y = VM_F(0.0);
        forward.z = left_handed ? VM_F(1.0) : VM_F(-1.0);
    }
    forward = vec3_normalize(forward);

    vector3 world_up = *up;
    if (vec3_length_squared(world_up) <= VECMAT_EPSILON * VECMAT_EPSILON) {
        world_up.x = VM_F(0.0);
        world_up.y = VM_F(1.0);
        world_up.z = VM_F(0.0);
    }

    vector3 right = vec3_cross(forward, world_up);
    if (vec3_length_squared(right) <= VECMAT_EPSILON * VECMAT_EPSILON) {
        const vector3 alt = { VM_F(1.0), VM_F(0.0), VM_F(0.0) };
        right = vec3_cross(forward, alt);
        if (vec3_length_squared(right) <= VECMAT_EPSILON * VECMAT_EPSILON) {
            const vector3 alt2 = { VM_F(0.0), VM_F(0.0), VM_F(1.0) };
            right = vec3_cross(forward, alt2);
        }
    }
    right = vec3_normalize(right);
    const vector3 true_up = vec3_cross(right, forward);

    /* Columns: local X→right, Y→true_up, Z→±forward so local forward aims along dir.
     * RH cameras look down local -Z, so column Z is -forward.
     * LH cameras look down local +Z, so column Z is +forward. */
    const vector3 col_z = left_handed ? forward : vec3_neg(forward);
    r->m11 = right.x;   r->m21 = right.y;   r->m31 = right.z;
    r->m12 = true_up.x; r->m22 = true_up.y; r->m32 = true_up.z;
    r->m13 = col_z.x;   r->m23 = col_z.y;   r->m33 = col_z.z;
}

/**
 * @brief Orientation that aims local -Z along @p direction (RH / FPS camera).
 *
 * Matches the rotation part of `mat4_look_from_dir` inverted (view-to-world).
 *
 * @param res Result quaternion.
 * @param direction Look direction.
 * @param up World up hint.
 */
void quat_look_ptr(quaternion *res, const vector3 *direction, const vector3 *up)
{
    quat_look_clip_ptr(res, direction, up, VM_CLIP_RH_NO);
}

/**
 * @brief Orientation that aims along @p direction.
 *
 * RH: local -Z maps to @p direction. LH: local +Z maps to @p direction.
 *
 * @param res Result quaternion.
 * @param direction Look direction.
 * @param up World up hint.
 * @param clip Clip-space convention selecting handedness.
 */
void quat_look_clip_ptr(quaternion *res, const vector3 *direction, const vector3 *up,
                        const vm_clip_t clip)
{
    matrix3 r;
    const int left_handed = (clip == VM_CLIP_LH_ZO || clip == VM_CLIP_LH_NO);
    quat_look_basis(&r, direction, up, left_handed);
    quat_from_mat3_ptr(res, &r);
}

/**
 * @brief Shortest rotation taking @p from onto @p to.
 *
 * Opposite vectors pick a stable orthogonal axis (180 deg). Near-parallel
 * vectors return identity.
 *
 * @param res Result quaternion.
 * @param from Source direction.
 * @param to Target direction.
 */
void quat_from_to_ptr(quaternion *res, const vector3 *from, const vector3 *to)
{
    vector3 a = *from;
    vector3 b = *to;
    if (vec3_length_squared(a) <= VECMAT_EPSILON * VECMAT_EPSILON ||
        vec3_length_squared(b) <= VECMAT_EPSILON * VECMAT_EPSILON) {
        quat_identity_ptr(res);
        return;
    }
    a = vec3_normalize(a);
    b = vec3_normalize(b);
    const vm_float_t d = vec3_dot(a, b);

    if (d >= VM_F(1.0) - VECMAT_EPSILON) {
        quat_identity_ptr(res);
        return;
    }

    if (d <= VM_F(-1.0) + VECMAT_EPSILON) {
        vector3 axis = vec3_cross((vector3){ VM_F(1.0), VM_F(0.0), VM_F(0.0) }, a);
        if (vec3_length_squared(axis) <= VECMAT_EPSILON * VECMAT_EPSILON) {
            axis = vec3_cross((vector3){ VM_F(0.0), VM_F(1.0), VM_F(0.0) }, a);
        }
        quat_from_axis_angle_ptr(res, &axis, (vm_float_t)M_PI);
        return;
    }

    const vector3 c = vec3_cross(a, b);
    res->x = c.x;
    res->y = c.y;
    res->z = c.z;
    res->w = VM_F(1.0) + d;
    quat_normalize_ptr(res, res);
}
