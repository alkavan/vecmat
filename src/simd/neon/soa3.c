// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

/**
 * @file
 * @brief NEON SoA 3-vector geometry kernels.
 *
 * f32 is 4-wide, f64 is 2-wide. Vectorized across vectors, not components. Unaligned
 * loads and stores. Separate multiplies and adds in the scalar order, never
 * fused, so every lane matches the scalar reference bit for bit. Cross loads
 * all six components before it stores, so an exact view alias is safe.
 */

#include <arm_neon.h>
#include <vecmat.h>

#if defined(VECMAT_USE_F64)
#define LANES 2
typedef float64x2_t vec_t;
#define vload(p)     vld1q_f64((p))
#define vstore(p, v) vst1q_f64((p), (v))
#define vadd(a, b)   vaddq_f64((a), (b))
#define vsub(a, b)   vsubq_f64((a), (b))
#define vmul(a, b)   vmulq_f64((a), (b))
#else
#define LANES 4
typedef float32x4_t vec_t;
#define vload(p)     vld1q_f32((p))
#define vstore(p, v) vst1q_f32((p), (v))
#define vadd(a, b)   vaddq_f32((a), (b))
#define vsub(a, b)   vsubq_f32((a), (b))
#define vmul(a, b)   vmulq_f32((a), (b))
#endif

/* Never fuse the separate multiply and add; GCC ignores the STDC pragma. */
#if !defined(_MSC_VER)
#pragma STDC FP_CONTRACT OFF
#endif

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC optimize("fp-contract=off")
#endif

void vm_soa3_dot_neon(vm_float_t *res, const vm_csoa3 a, const vm_csoa3 b, const int n)
{
    int i = 0;
    for (; i + LANES <= n; i += LANES) {
        const vec_t xx = vmul(vload(a.x + i), vload(b.x + i));
        const vec_t yy = vmul(vload(a.y + i), vload(b.y + i));
        const vec_t zz = vmul(vload(a.z + i), vload(b.z + i));
        vstore(res + i, vadd(vadd(xx, yy), zz));
    }

    for (; i < n; ++i) {
        const vm_float_t xx = a.x[i] * b.x[i];
        const vm_float_t yy = a.y[i] * b.y[i];
        const vm_float_t zz = a.z[i] * b.z[i];
        res[i] = (xx + yy) + zz;
    }
}

void vm_soa3_cross_neon(const vm_soa3 res, const vm_csoa3 a, const vm_csoa3 b, const int n)
{
    int i = 0;
    for (; i + LANES <= n; i += LANES) {
        const vec_t ax = vload(a.x + i), ay = vload(a.y + i), az = vload(a.z + i);
        const vec_t bx = vload(b.x + i), by = vload(b.y + i), bz = vload(b.z + i);
        const vec_t cx = vsub(vmul(ay, bz), vmul(az, by));
        const vec_t cy = vsub(vmul(az, bx), vmul(ax, bz));
        const vec_t cz = vsub(vmul(ax, by), vmul(ay, bx));
        vstore(res.x + i, cx);
        vstore(res.y + i, cy);
        vstore(res.z + i, cz);
    }

    for (; i < n; ++i) {
        const vm_float_t ax = a.x[i], ay = a.y[i], az = a.z[i];
        const vm_float_t bx = b.x[i], by = b.y[i], bz = b.z[i];
        const vm_float_t cx = ay * bz - az * by;
        const vm_float_t cy = az * bx - ax * bz;
        const vm_float_t cz = ax * by - ay * bx;
        res.x[i] = cx;
        res.y[i] = cy;
        res.z[i] = cz;
    }
}

void vm_soa3_length_squared_neon(vm_float_t *res, const vm_csoa3 a, const int n)
{
    int i = 0;
    for (; i + LANES <= n; i += LANES) {
        const vec_t x = vload(a.x + i), y = vload(a.y + i), z = vload(a.z + i);
        vstore(res + i, vadd(vadd(vmul(x, x), vmul(y, y)), vmul(z, z)));
    }

    for (; i < n; ++i) {
        const vm_float_t xx = a.x[i] * a.x[i];
        const vm_float_t yy = a.y[i] * a.y[i];
        const vm_float_t zz = a.z[i] * a.z[i];
        res[i] = (xx + yy) + zz;
    }
}

void vm_soa3_distance_squared_neon(vm_float_t *res, const vm_csoa3 a, const vm_csoa3 b, const int n)
{
    int i = 0;
    for (; i + LANES <= n; i += LANES) {
        const vec_t dx = vsub(vload(a.x + i), vload(b.x + i));
        const vec_t dy = vsub(vload(a.y + i), vload(b.y + i));
        const vec_t dz = vsub(vload(a.z + i), vload(b.z + i));
        vstore(res + i, vadd(vadd(vmul(dx, dx), vmul(dy, dy)), vmul(dz, dz)));
    }

    for (; i < n; ++i) {
        const vm_float_t dx = a.x[i] - b.x[i];
        const vm_float_t dy = a.y[i] - b.y[i];
        const vm_float_t dz = a.z[i] - b.z[i];
        const vm_float_t xx = dx * dx;
        const vm_float_t yy = dy * dy;
        const vm_float_t zz = dz * dz;
        res[i] = (xx + yy) + zz;
    }
}
