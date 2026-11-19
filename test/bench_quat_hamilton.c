// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

/**
 * @file
 * @brief Experimental Hamilton product against `quat_mul`.
 *
 * Built only with `-DVECMAT_EXPERIMENTAL=ON`. Each pair starts from the same
 * inputs and does the same iteration count. The single-product pair builds
 * the left matrix and applies it once per iteration. The batch pair builds
 * that matrix once per call and applies it to eight quaternions.
 */

#include <vecmat.h>
#include "unitest.h"

enum { HAMILTON_ITERS = 1000000, HAMILTON_BATCH = 8 };

static volatile vm_float_t hamilton_sink;

static void consume_quat(const quaternion q)
{
    hamilton_sink = q.x + q.w;
}

static void load_pair(quaternion *q, quaternion *p)
{
    *q = (quaternion){
        .x = 0.0f, .y = 0.70710678f, .z = 0.0f, .w = 0.70710678f
    };
    *p = (quaternion){
        .x = 0.0f, .y = 0.0f, .z = 0.70710678f, .w = 0.70710678f
    };
}

static void load_batch(quaternion *fixed, quaternion *in)
{
    *fixed = (quaternion){
        .x = 0.0f, .y = 0.70710678f, .z = 0.0f, .w = 0.70710678f
    };
    for (int j = 0; j < HAMILTON_BATCH; ++j) {
        in[j].x = 0.1f * (float)(j + 1);
        in[j].y = 0.05f * (float)((j % 3) - 1);
        in[j].z = 0.02f * (float)j;
        in[j].w = 0.9f;
    }
}

static void advance_batch(quaternion *fixed, quaternion *in, const quaternion *out)
{
    *fixed = out[0];
    in[0] = out[HAMILTON_BATCH - 1];
}

BENCHMARK(x_quat_hamilton_mat_apply, "iterations: 1000000 [experimental,hamilton,mat,apply]")
{
    quaternion q;
    quaternion p;
    load_pair(&q, &p);

    for (int i = 0; i < HAMILTON_ITERS; i++) {
        matrix4 H;
        vector4 v_out;
        const vector4 v_in = {.x = p.x, .y = p.y, .z = p.z, .w = p.w};

        vm_x_quat_hamilton_mat(&H, &q);
        mat4_mul_vec4_ptr(&v_out, &H, &v_in);
        q.x = v_out.x;
        q.y = v_out.y;
        q.z = v_out.z;
        q.w = v_out.w;
    }
    consume_quat(q);
}

BENCHMARK(quat_mul_hamilton_pair, "iterations: 1000000 [experimental,hamilton,quat_mul]")
{
    quaternion q;
    quaternion p;
    load_pair(&q, &p);

    for (int i = 0; i < HAMILTON_ITERS; i++) {
        q = quat_mul(q, p);
    }
    consume_quat(q);
}

BENCHMARK(x_quat_hamilton_mul_array_8, "iterations: 1000000 [experimental,hamilton,batch,n=8]")
{
    quaternion fixed;
    quaternion in[HAMILTON_BATCH];
    quaternion out[HAMILTON_BATCH];
    load_batch(&fixed, in);

    for (int i = 0; i < HAMILTON_ITERS; i++) {
        vm_x_quat_hamilton_mul_array(out, &fixed, in, HAMILTON_BATCH);
        advance_batch(&fixed, in, out);
    }
    consume_quat(out[0]);
    consume_quat(out[HAMILTON_BATCH - 1]);
}

BENCHMARK(quat_mul_hamilton_batch8, "iterations: 1000000 [experimental,hamilton,quat_mul,batch,n=8]")
{
    quaternion fixed;
    quaternion in[HAMILTON_BATCH];
    quaternion out[HAMILTON_BATCH];
    load_batch(&fixed, in);

    for (int i = 0; i < HAMILTON_ITERS; i++) {
        for (int j = 0; j < HAMILTON_BATCH; ++j) {
            out[j] = quat_mul(fixed, in[j]);
        }
        advance_batch(&fixed, in, out);
    }
    consume_quat(out[0]);
    consume_quat(out[HAMILTON_BATCH - 1]);
}
