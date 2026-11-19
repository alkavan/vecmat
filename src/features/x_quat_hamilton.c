// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include "vecmat/x_quat_hamilton.h"

void vm_x_quat_hamilton_mat(matrix4 *m, const quaternion *q)
{
    if (!m || !q) {
        return;
    }

    const vm_float_t w = q->w;
    const vm_float_t x = q->x;
    const vm_float_t y = q->y;
    const vm_float_t z = q->z;

    /* Column-major (x, y, z, w). Same matrix as the (w, x, y, z) printout. */
    m->m11 = w;  m->m21 = z;  m->m31 = -y; m->m41 = -x;
    m->m12 = -z; m->m22 = w;  m->m32 = x;  m->m42 = -y;
    m->m13 = y;  m->m23 = -x; m->m33 = w;  m->m43 = -z;
    m->m14 = x;  m->m24 = y;  m->m34 = z;  m->m44 = w;
}

/**
 * @brief Right-multiplication matrix, `M * v = v * q`.
 *
 * Columns, top to bottom `(x, y, z, w)`:
 * x `(w, -z, y, -x)`, y `(z, w, -x, -y)`, z `(-y, x, w, -z)`, w `(x, y, z, w)`.
 */
static void hamilton_mat_right(matrix4 *m, const quaternion *q)
{
    const vm_float_t w = q->w;
    const vm_float_t x = q->x;
    const vm_float_t y = q->y;
    const vm_float_t z = q->z;

    m->m11 = w;  m->m21 = -z; m->m31 = y;  m->m41 = -x;
    m->m12 = z;  m->m22 = w;  m->m32 = -x; m->m42 = -y;
    m->m13 = -y; m->m23 = x;  m->m33 = w;  m->m43 = -z;
    m->m14 = x;  m->m24 = y;  m->m34 = z;  m->m44 = w;
}

/**
 * @brief One Hamilton mat-vec. `out` may alias `in`; the input is copied first.
 */
static void hamilton_apply(quaternion *out, const matrix4 *m, const quaternion *in)
{
    const vector4 v_in = {.x = in->x, .y = in->y, .z = in->z, .w = in->w};
    vector4 v_out;
    mat4_mul_vec4_ptr(&v_out, m, &v_in);
    out->x = v_out.x;
    out->y = v_out.y;
    out->z = v_out.z;
    out->w = v_out.w;
}

static void hamilton_apply_n(quaternion *out, const matrix4 *m, const quaternion *in, const int n)
{
    for (int i = 0; i < n; ++i) {
        hamilton_apply(&out[i], m, &in[i]);
    }
}

void vm_x_quat_hamilton_mul_array(quaternion *out, const quaternion *fixed,
                                  const quaternion *in, const int n)
{
    if (n <= 0 || !out || !fixed || !in) {
        return;
    }
    matrix4 H;
    vm_x_quat_hamilton_mat(&H, fixed);
    hamilton_apply_n(out, &H, in, n);
}

void vm_x_quat_hamilton_mul_array_right(quaternion *out, const quaternion *fixed,
                                        const quaternion *in, const int n)
{
    if (n <= 0 || !out || !fixed || !in) {
        return;
    }

    matrix4 H;
    hamilton_mat_right(&H, fixed);
    hamilton_apply_n(out, &H, in, n);
}

void vm_x_quat_hamilton_mul_chain(quaternion *acc, const quaternion *qs, const int n)
{
    if (n <= 0 || !acc || !qs) {
        return;
    }

    for (int i = 0; i < n; ++i) {
        matrix4 H;
        hamilton_mat_right(&H, &qs[i]);
        hamilton_apply(acc, &H, acc);
    }
}
