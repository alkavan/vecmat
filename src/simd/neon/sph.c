// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

/**
 * @file
 * @brief NEON neighbor loops for the SPH density sum and pressure gradient.
 *
 * Same kernels as the scalar path: cubic spline and Wendland C2, support
 * \(r < 2h\), normalized. Neighbors are processed 4-wide (f32) or 2-wide
 * (f64). No hardware gather; indices are loaded scalar then packed into a
 * vector. The scalar functions stay the fallback.
 */

#include <arm_neon.h>
#include <math.h>
#include <vecmat.h>

#if defined(VECMAT_USE_F64)
#define VM_LANES 2
typedef float64x2_t vm_vec;
typedef uint64x2_t vm_mask;
#define vm_set1        vdupq_n_f64
#define vm_setzero     vdupq_n_f64
#define vm_add         vaddq_f64
#define vm_sub         vsubq_f64
#define vm_mul         vmulq_f64
#define vm_div         vdivq_f64
#define vm_sqrt        vsqrtq_f64
#define vm_max         vmaxq_f64
#define vm_and         vandq_f64
#define vm_cmp_lt(a, b)   vcltq_f64((a), (b))
#define vm_cmp_gt(a, b)   vcgtq_f64((a), (b))
#define vm_mask_and(a, b) vandq_u64((a), (b))
#define vm_blend(a, b, m) vbslq_f64((m), (b), (a))
#define vm_load        vld1q_f64
/* any_le defined below as a static inline */
#else
#define VM_LANES 4
typedef float32x4_t vm_vec;
typedef uint32x4_t vm_mask;
#define vm_set1        vdupq_n_f32
#define vm_setzero     vdupq_n_f32
#define vm_add         vaddq_f32
#define vm_sub         vsubq_f32
#define vm_mul         vmulq_f32
#define vm_div         vdivq_f32
#define vm_sqrt        vsqrtq_f32
#define vm_max         vmaxq_f32
#define vm_and         vandq_f32
#define vm_cmp_lt(a, b)   vcltq_f32((a), (b))
#define vm_cmp_gt(a, b)   vcgtq_f32((a), (b))
#define vm_mask_and(a, b) vandq_u32((a), (b))
#define vm_blend(a, b, m) vbslq_f32((m), (b), (a))
#define vm_load        vld1q_f32
/* any_le defined below as a static inline */
#endif

static vm_float_t hsum(const vm_vec v)
{
#if defined(VECMAT_USE_F64)
    /* vaddvq_f64 is Armv8.1; base Armv8-A has no 64-bit across-lane add. */
    return vgetq_lane_f64(v, 0) + vgetq_lane_f64(v, 1);
#else
    return vaddvq_f32(v);
#endif
}

/* True if any lane of v is <= s. f64 path avoids vmaxvq_u64 (Armv8.1). */
static int any_le(const vm_vec v, const vm_float_t s)
{
#if defined(VECMAT_USE_F64)
    const uint64x2_t m = vcleq_f64(v, vdupq_n_f64(s));
    return (vgetq_lane_u64(m, 0) | vgetq_lane_u64(m, 1)) != 0;
#else
    const uint32x4_t m = vcleq_f32(v, vdupq_n_f32(s));
    return vmaxvq_u32(m) != 0;
#endif
}

/* Scalar gather into a temporary, then a vector load. NEON has no gather. */
static vm_vec gather_f(const vm_float_t *base, const int *idx)
{
    vm_float_t tmp[VM_LANES];

    for (int i = 0; i < VM_LANES; ++i) {
        tmp[i] = base[idx[i]];
    }

    return vm_load(tmp);
}

static vm_float_t sigma_of(const int dim, const int kernel)
{
    if (kernel == VM_SPH_WENDLAND) {
        if (dim == 1) return VM_F(0.75);
        if (dim == 2) return VM_F(7.0) / (VM_F(4.0) * (vm_float_t)M_PI);
        if (dim == 3) return VM_F(21.0) / (VM_F(16.0) * (vm_float_t)M_PI);

        return VM_F(0.0);
    }

    if (dim == 1) return VM_F(2.0) / VM_F(3.0);
    if (dim == 2) return VM_F(10.0) / (VM_F(7.0) * (vm_float_t)M_PI);
    if (dim == 3) return VM_F(1.0) / (vm_float_t)M_PI;

    return VM_F(0.0);
}

/* Shape f(q) and df/dq, q = r/h, support q < 2. Matches src/features/sph.c. */
static vm_vec shape_f(const vm_vec q, const int kernel)
{
    const vm_vec one = vm_set1(VM_F(1.0));
    const vm_vec q2 = vm_mul(q, q);

    if (kernel == VM_SPH_WENDLAND) {
        const vm_vec u = vm_sub(one, vm_mul(vm_set1(VM_F(0.5)), q));
        const vm_vec u2 = vm_mul(u, u);
        return vm_mul(vm_mul(u2, u2), vm_add(one, vm_mul(vm_set1(VM_F(2.0)), q)));
    }

    const vm_vec q3 = vm_mul(q2, q);
    const vm_vec inner = vm_add(one, vm_add(vm_mul(vm_set1(VM_F(-1.5)), q2),
                                            vm_mul(vm_set1(VM_F(0.75)), q3)));

    const vm_vec t = vm_sub(vm_set1(VM_F(2.0)), q);
    const vm_vec outer = vm_mul(vm_set1(VM_F(0.25)), vm_mul(vm_mul(t, t), t));

    return vm_blend(outer, inner, vm_cmp_lt(q, vm_add(one, vm_set1(VM_F(1.0e-12)))));
}

static vm_vec shape_df(const vm_vec q, const int kernel)
{
    const vm_vec one = vm_set1(VM_F(1.0));

    if (kernel == VM_SPH_WENDLAND) {
        const vm_vec u = vm_sub(one, vm_mul(vm_set1(VM_F(0.5)), q));
        const vm_vec u3 = vm_mul(vm_mul(u, u), u);
        return vm_mul(vm_mul(vm_set1(VM_F(-5.0)), q), u3);
    }

    const vm_vec q2 = vm_mul(q, q);
    const vm_vec inner = vm_add(vm_mul(vm_set1(VM_F(-3.0)), q),
                                vm_mul(vm_set1(VM_F(2.25)), q2));

    const vm_vec t = vm_sub(vm_set1(VM_F(2.0)), q);
    const vm_vec outer = vm_mul(vm_set1(VM_F(-0.75)), vm_mul(t, t));

    return vm_blend(outer, inner, vm_cmp_lt(q, vm_add(one, vm_set1(VM_F(1.0e-12)))));
}

vm_float_t vm_sph_density_neon(const vm_float_t *x, const vm_float_t *y,
                               const vm_float_t *z, const vm_float_t *m,
                               const int i, const int *neighbors, const int nn,
                               const vm_float_t h, const int dim, const int kernel)
{
    const vm_float_t sig = sigma_of(dim, kernel);

    vm_float_t hd = h;
    for (int d = 1; d < dim; ++d) {
        hd *= h;
    }

    const vm_float_t inv_hd = sig / hd;
    const vm_float_t inv_h = VM_F(1.0) / h;
    const vm_float_t support2 = VM_F(4.0) * h * h;

    vm_float_t rho = m[i] * vm_sph_w(VM_F(0.0), h, dim, kernel);
    if (!neighbors || nn <= 0) {
        return rho;
    }

    const vm_vec xi = vm_set1(x[i]);
    const vm_vec yi = (dim >= 2) ? vm_set1(y[i]) : vm_setzero(0.0);
    const vm_vec zi = (dim >= 3) ? vm_set1(z[i]) : vm_setzero(0.0);
    const vm_vec vinv_h = vm_set1(inv_h);
    const vm_vec vinv_hd = vm_set1(inv_hd);
    const vm_vec vsupport = vm_set1(support2);

    vm_vec acc = vm_setzero(0.0);

    int k = 0;
    for (; k + VM_LANES <= nn; k += VM_LANES) {
        const vm_vec xj = gather_f(x, neighbors + k);
        const vm_vec dx = vm_sub(xi, xj);

        vm_vec r2 = vm_mul(dx, dx);
        if (dim >= 2) {
            const vm_vec dy = vm_sub(yi, gather_f(y, neighbors + k));
            r2 = vm_add(r2, vm_mul(dy, dy));
        }
        if (dim >= 3) {
            const vm_vec dz = vm_sub(zi, gather_f(z, neighbors + k));
            r2 = vm_add(r2, vm_mul(dz, dz));
        }

        const vm_mask inside = vm_cmp_lt(r2, vsupport);
        const vm_vec r = vm_sqrt(vm_max(r2, vm_set1(VM_F(0.0))));
        const vm_vec q = vm_mul(r, vinv_h);
        const vm_vec w = vm_mul(shape_f(q, kernel), vinv_hd);
        const vm_vec mj = gather_f(m, neighbors + k);
        const vm_vec contrib = vm_mul(mj, w);

        acc = vm_add(acc, vm_blend(vm_setzero(0.0), contrib, inside));
    }

    rho += hsum(acc);
    for (; k < nn; ++k) {
        const int j = neighbors[k];
        const vm_float_t dx = x[i] - x[j];
        const vm_float_t dy = (dim >= 2) ? (y[i] - y[j]) : VM_F(0.0);
        const vm_float_t dz = (dim >= 3) ? (z[i] - z[j]) : VM_F(0.0);
        const vm_float_t r = VECMAT_SQRT(dx * dx + dy * dy + dz * dz);
        rho += m[j] * vm_sph_w(r, h, dim, kernel);
    }

    return rho;
}

bool vm_sph_pressure_grad_neon(vm_float_t *gx, vm_float_t *gy, vm_float_t *gz,
                               const vm_float_t *x, const vm_float_t *y,
                               const vm_float_t *z, const vm_float_t *m,
                               const vm_float_t *rho, const vm_float_t *p,
                               const int i, const int *neighbors, const int nn,
                               const vm_float_t h, const int dim, const int kernel)
{
    *gx = VM_F(0.0);
    *gy = VM_F(0.0);
    if (gz) {
        *gz = VM_F(0.0);
    }

    if (!neighbors || nn <= 0) {
        return true;
    }

    const vm_float_t sig = sigma_of(dim, kernel);

    vm_float_t hd = h;
    for (int d = 0; d < dim; ++d) {
        hd *= h;
    }

    const vm_float_t inv_hd = sig / hd;
    const vm_float_t inv_h = VM_F(1.0) / h;
    const vm_float_t support2 = VM_F(4.0) * h * h;
    const vm_float_t pi_term = p[i] / (rho[i] * rho[i]);

    const vm_vec xi = vm_set1(x[i]);
    const vm_vec yi = (dim >= 2) ? vm_set1(y[i]) : vm_setzero(0.0);
    const vm_vec zi = (dim >= 3) ? vm_set1(z[i]) : vm_setzero(0.0);
    const vm_vec vinv_h = vm_set1(inv_h);
    const vm_vec vinv_hd = vm_set1(inv_hd);
    const vm_vec vsupport = vm_set1(support2);
    const vm_vec vpit = vm_set1(pi_term);

    vm_vec agx = vm_setzero(0.0);
    vm_vec agy = vm_setzero(0.0);
    vm_vec agz = vm_setzero(0.0);

    int k = 0;
    for (; k + VM_LANES <= nn; k += VM_LANES) {
        const vm_vec xj = gather_f(x, neighbors + k);
        const vm_vec dx = vm_sub(xi, xj);

        vm_vec dy = vm_setzero(0.0);
        vm_vec dz = vm_setzero(0.0);
        vm_vec r2 = vm_mul(dx, dx);

        if (dim >= 2) {
            dy = vm_sub(yi, gather_f(y, neighbors + k));
            r2 = vm_add(r2, vm_mul(dy, dy));
        }
        if (dim >= 3) {
            dz = vm_sub(zi, gather_f(z, neighbors + k));
            r2 = vm_add(r2, vm_mul(dz, dz));
        }

        const vm_mask inside = vm_cmp_lt(r2, vsupport);
        const vm_mask alive = vm_cmp_gt(r2, vm_set1(VM_F(1.0e-20)));
        const vm_mask keep = vm_mask_and(inside, alive);
        const vm_vec r = vm_sqrt(vm_max(r2, vm_set1(VM_F(1.0e-20))));
        const vm_vec q = vm_mul(r, vinv_h);
        const vm_vec dw = vm_mul(shape_df(q, kernel), vinv_hd);
        const vm_vec mj = gather_f(m, neighbors + k);
        const vm_vec pj = gather_f(p, neighbors + k);
        const vm_vec rhoj = gather_f(rho, neighbors + k);

        if (any_le(rhoj, VM_F(0.0))) {
            return false;
        }

        const vm_vec coeff = vm_add(vpit, vm_div(pj, vm_mul(rhoj, rhoj)));
        const vm_vec raw = vm_mul(mj, vm_mul(coeff, vm_div(dw, r)));
        const vm_vec scale = vm_blend(vm_setzero(0.0), raw, keep);

        agx = vm_add(agx, vm_mul(scale, dx));
        if (dim >= 2) {
            agy = vm_add(agy, vm_mul(scale, dy));
        }
        if (dim >= 3) {
            agz = vm_add(agz, vm_mul(scale, dz));
        }
    }

    *gx = hsum(agx);
    *gy = hsum(agy);
    if (gz) {
        *gz = hsum(agz);
    }

    for (; k < nn; ++k) {
        const int j = neighbors[k];
        if (rho[j] <= VM_F(0.0)) {
            return false;
        }

        const vm_float_t dx = x[i] - x[j];
        const vm_float_t dy = (dim >= 2) ? (y[i] - y[j]) : VM_F(0.0);
        const vm_float_t dz = (dim >= 3) ? (z[i] - z[j]) : VM_F(0.0);
        const vm_float_t r2 = dx * dx + dy * dy + dz * dz;

        if (r2 <= VM_F(0.0) || r2 >= support2) {
            continue;
        }

        const vm_float_t r = VECMAT_SQRT(r2);
        const vm_float_t dw = vm_sph_dw(r, h, dim, kernel);
        const vm_float_t scale = m[j] * (pi_term + p[j] / (rho[j] * rho[j])) * dw / r;

        *gx += scale * dx;
        if (dim >= 2) {
            *gy += scale * dy;
        }
        if (dim >= 3 && gz) {
            *gz += scale * dz;
        }
    }

    return true;
}
