// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

/**
 * @file
 * @brief SVE neighbor loops for the SPH density sum and pressure gradient.
 *
 * Same kernels as the scalar path. VL-agnostic: the predicate covers the
 * tail, so there is no scalar remainder. Indexed gathers replace the NEON
 * scalar gather. SVE2 hardware runs this kernel; SVE2 adds nothing it needs.
 */

#include <arm_sve.h>
#include <math.h>
#include <vecmat.h>

#if defined(VECMAT_USE_F64)
#define VM_F_SUFFIX f64
typedef svfloat64_t vm_vec;
typedef svint64_t vm_idx;
#define vm_set1        svdup_n_f64
#define vm_whilelt(a, b) svwhilelt_b64((a), (b))
#define vm_add         svadd_f64_x
#define vm_add_m       svadd_f64_m
#define vm_sub         svsub_f64_x
#define vm_mul         svmul_f64_x
#define vm_div         svdiv_f64_x
#define vm_sqrt        svsqrt_f64_x
#define vm_max         svmax_f64_x
#define vm_cmp_lt      svcmplt_f64
#define vm_cmp_gt      svcmpgt_f64
#define vm_cmp_le      svcmple_f64
#define vm_sel         svsel_f64
#define vm_hsum        svaddv_f64
#define vm_ptrue       svptrue_b64
#define vm_ld_idx(pg, p) svld1sw_s64((pg), (p))
#define vm_gather(pg, base, idx) svld1_gather_s64index_f64((pg), (base), (idx))
#else
typedef svfloat32_t vm_vec;
typedef svint32_t vm_idx;
#define vm_set1        svdup_n_f32
#define vm_whilelt(a, b) svwhilelt_b32((a), (b))
#define vm_add         svadd_f32_x
#define vm_add_m       svadd_f32_m
#define vm_sub         svsub_f32_x
#define vm_mul         svmul_f32_x
#define vm_div         svdiv_f32_x
#define vm_sqrt        svsqrt_f32_x
#define vm_max         svmax_f32_x
#define vm_cmp_lt      svcmplt_f32
#define vm_cmp_gt      svcmpgt_f32
#define vm_cmp_le      svcmple_f32
#define vm_sel         svsel_f32
#define vm_hsum        svaddv_f32
#define vm_ptrue       svptrue_b32
#define vm_ld_idx(pg, p) svld1_s32((pg), (p))
#define vm_gather(pg, base, idx) svld1_gather_s32index_f32((pg), (base), (idx))
#endif

static uint64_t lanes(void)
{
#if defined(VECMAT_USE_F64)
    return svcntd();
#else
    return svcntw();
#endif
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
static vm_vec shape_f(const svbool_t pg, const vm_vec q, const int kernel)
{
    const vm_vec one = vm_set1(VM_F(1.0));
    const vm_vec q2 = vm_mul(pg, q, q);

    if (kernel == VM_SPH_WENDLAND) {
        const vm_vec u = vm_sub(pg, one, vm_mul(pg, vm_set1(VM_F(0.5)), q));
        const vm_vec u2 = vm_mul(pg, u, u);
        return vm_mul(pg, vm_mul(pg, u2, u2),
                      vm_add(pg, one, vm_mul(pg, vm_set1(VM_F(2.0)), q)));
    }

    const vm_vec q3 = vm_mul(pg, q2, q);
    const vm_vec inner = vm_add(pg, one,
                                vm_add(pg, vm_mul(pg, vm_set1(VM_F(-1.5)), q2),
                                       vm_mul(pg, vm_set1(VM_F(0.75)), q3)));

    const vm_vec t = vm_sub(pg, vm_set1(VM_F(2.0)), q);
    const vm_vec outer = vm_mul(pg, vm_set1(VM_F(0.25)), vm_mul(pg, vm_mul(pg, t, t), t));
    const svbool_t inner_pg = vm_cmp_lt(pg, q, vm_set1(VM_F(1.0) + VM_F(1.0e-12)));

    return vm_sel(inner_pg, inner, outer);
}

static vm_vec shape_df(const svbool_t pg, const vm_vec q, const int kernel)
{
    const vm_vec one = vm_set1(VM_F(1.0));

    if (kernel == VM_SPH_WENDLAND) {
        const vm_vec u = vm_sub(pg, one, vm_mul(pg, vm_set1(VM_F(0.5)), q));
        const vm_vec u3 = vm_mul(pg, vm_mul(pg, u, u), u);
        return vm_mul(pg, vm_mul(pg, vm_set1(VM_F(-5.0)), q), u3);
    }

    const vm_vec q2 = vm_mul(pg, q, q);
    const vm_vec inner = vm_add(pg, vm_mul(pg, vm_set1(VM_F(-3.0)), q),
                                vm_mul(pg, vm_set1(VM_F(2.25)), q2));

    const vm_vec t = vm_sub(pg, vm_set1(VM_F(2.0)), q);
    const vm_vec outer = vm_mul(pg, vm_set1(VM_F(-0.75)), vm_mul(pg, t, t));
    const svbool_t inner_pg = vm_cmp_lt(pg, q, vm_set1(VM_F(1.0) + VM_F(1.0e-12)));

    return vm_sel(inner_pg, inner, outer);
}

vm_float_t vm_sph_density_sve(const vm_float_t *x, const vm_float_t *y,
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
    const vm_vec yi = (dim >= 2) ? vm_set1(y[i]) : vm_set1(VM_F(0.0));
    const vm_vec zi = (dim >= 3) ? vm_set1(z[i]) : vm_set1(VM_F(0.0));

    const vm_vec vinv_h = vm_set1(inv_h);
    const vm_vec vinv_hd = vm_set1(inv_hd);
    const vm_vec vsupport = vm_set1(support2);

    const uint64_t step = lanes();
    vm_vec acc = vm_set1(VM_F(0.0));

    for (uint64_t k = 0; k < (uint64_t)nn; k += step) {
        const svbool_t pg = vm_whilelt(k, (uint64_t)nn);
        const vm_idx idx = vm_ld_idx(pg, neighbors + k);
        const vm_vec dx = vm_sub(pg, xi, vm_gather(pg, x, idx));

        vm_vec r2 = vm_mul(pg, dx, dx);
        if (dim >= 2) {
            const vm_vec dy = vm_sub(pg, yi, vm_gather(pg, y, idx));
            r2 = vm_add(pg, r2, vm_mul(pg, dy, dy));
        }
        if (dim >= 3) {
            const vm_vec dz = vm_sub(pg, zi, vm_gather(pg, z, idx));
            r2 = vm_add(pg, r2, vm_mul(pg, dz, dz));
        }

        const svbool_t inside = vm_cmp_lt(pg, r2, vsupport);
        const vm_vec r = vm_sqrt(pg, vm_max(pg, r2, vm_set1(VM_F(0.0))));
        const vm_vec q = vm_mul(pg, r, vinv_h);
        const vm_vec w = vm_mul(pg, shape_f(pg, q, kernel), vinv_hd);
        const vm_vec mj = vm_gather(pg, m, idx);
        acc = svsel(inside, vm_add_m(pg, acc, vm_mul(pg, mj, w)), acc);
    }

    rho += vm_hsum(vm_ptrue(), acc);
    return rho;
}

bool vm_sph_pressure_grad_sve(vm_float_t *gx, vm_float_t *gy, vm_float_t *gz,
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
    const vm_vec yi = (dim >= 2) ? vm_set1(y[i]) : vm_set1(VM_F(0.0));
    const vm_vec zi = (dim >= 3) ? vm_set1(z[i]) : vm_set1(VM_F(0.0));

    const vm_vec vinv_h = vm_set1(inv_h);
    const vm_vec vinv_hd = vm_set1(inv_hd);
    const vm_vec vsupport = vm_set1(support2);
    const vm_vec vpit = vm_set1(pi_term);

    const uint64_t step = lanes();
    vm_vec agx = vm_set1(VM_F(0.0));
    vm_vec agy = vm_set1(VM_F(0.0));
    vm_vec agz = vm_set1(VM_F(0.0));

    for (uint64_t k = 0; k < (uint64_t)nn; k += step) {
        const svbool_t pg = vm_whilelt(k, (uint64_t)nn);
        const vm_idx idx = vm_ld_idx(pg, neighbors + k);
        const vm_vec dx = vm_sub(pg, xi, vm_gather(pg, x, idx));

        vm_vec dy = vm_set1(VM_F(0.0));
        vm_vec dz = vm_set1(VM_F(0.0));
        vm_vec r2 = vm_mul(pg, dx, dx);

        if (dim >= 2) {
            dy = vm_sub(pg, yi, vm_gather(pg, y, idx));
            r2 = vm_add(pg, r2, vm_mul(pg, dy, dy));
        }
        if (dim >= 3) {
            dz = vm_sub(pg, zi, vm_gather(pg, z, idx));
            r2 = vm_add(pg, r2, vm_mul(pg, dz, dz));
        }

        const svbool_t inside = vm_cmp_lt(pg, r2, vsupport);
        const svbool_t alive = vm_cmp_gt(pg, r2, vm_set1(VM_F(1.0e-20)));
        const svbool_t keep = svand_z(pg, inside, alive);

        const vm_vec r = vm_sqrt(pg, vm_max(pg, r2, vm_set1(VM_F(1.0e-20))));
        const vm_vec q = vm_mul(pg, r, vinv_h);
        const vm_vec dw = vm_mul(pg, shape_df(pg, q, kernel), vinv_hd);
        const vm_vec mj = vm_gather(pg, m, idx);
        const vm_vec pj = vm_gather(pg, p, idx);
        const vm_vec rhoj = vm_gather(pg, rho, idx);

        if (svptest_any(pg, vm_cmp_le(pg, rhoj, vm_set1(VM_F(0.0))))) {
            return false;
        }

        const vm_vec coeff = vm_add(pg, vpit, vm_div(pg, pj, vm_mul(pg, rhoj, rhoj)));
        const vm_vec scale = svsel(keep, vm_mul(pg, mj, vm_mul(pg, coeff, vm_div(pg, dw, r))),
                                   vm_set1(VM_F(0.0)));

        agx = vm_add_m(pg, agx, vm_mul(pg, scale, dx));
        if (dim >= 2) {
            agy = vm_add_m(pg, agy, vm_mul(pg, scale, dy));
        }
        if (dim >= 3) {
            agz = vm_add_m(pg, agz, vm_mul(pg, scale, dz));
        }
    }

    const svbool_t all = vm_ptrue();

    *gx = vm_hsum(all, agx);
    *gy = vm_hsum(all, agy);
    if (gz) {
        *gz = vm_hsum(all, agz);
    }

    return true;
}
