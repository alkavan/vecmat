// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

/**
 * @file
 * @brief AVX2 neighbor loops for density, pressure gradient, and artificial viscosity.
 *
 * Same kernels as the scalar path: cubic spline and Wendland C2, support
 * \(r < 2h\), normalized. Neighbors are gathered 8-wide (f32) or 4-wide (f64).
 * The scalar functions stay the fallback.
 */

#include <immintrin.h>
#include <math.h>
#include <vecmat.h>

#if defined(VECMAT_USE_F64)
#define VM_LANES 4
typedef __m256d vm_vec;
#define vm_set1         _mm256_set1_pd
#define vm_setzero      _mm256_setzero_pd
#define vm_add          _mm256_add_pd
#define vm_sub          _mm256_sub_pd
#define vm_mul          _mm256_mul_pd
#define vm_div          _mm256_div_pd
#define vm_sqrt         _mm256_sqrt_pd
#define vm_max          _mm256_max_pd
#define vm_and          _mm256_and_pd
#define vm_cmp_lt(a, b) _mm256_cmp_pd((a), (b), _CMP_LT_OQ)
#define vm_cmp_gt(a, b) _mm256_cmp_pd((a), (b), _CMP_GT_OQ)
#define vm_blend        _mm256_blendv_pd
#else
#define VM_LANES 8
typedef __m256 vm_vec;
#define vm_set1         _mm256_set1_ps
#define vm_setzero      _mm256_setzero_ps
#define vm_add          _mm256_add_ps
#define vm_sub          _mm256_sub_ps
#define vm_mul          _mm256_mul_ps
#define vm_div          _mm256_div_ps
#define vm_sqrt         _mm256_sqrt_ps
#define vm_max          _mm256_max_ps
#define vm_and          _mm256_and_ps
#define vm_cmp_lt(a, b) _mm256_cmp_ps((a), (b), _CMP_LT_OQ)
#define vm_cmp_gt(a, b) _mm256_cmp_ps((a), (b), _CMP_GT_OQ)
#define vm_blend        _mm256_blendv_ps
#endif

static vm_float_t hsum(const vm_vec v)
{
#if defined(VECMAT_USE_F64)
    __m128d lo = _mm256_castpd256_pd128(v);
    __m128d hi = _mm256_extractf128_pd(v, 1);
    lo = _mm_add_pd(lo, hi);
    lo = _mm_hadd_pd(lo, lo);
    return _mm_cvtsd_f64(lo);
#else
    __m128 lo = _mm256_castps256_ps128(v);
    __m128 hi = _mm256_extractf128_ps(v, 1);
    lo = _mm_add_ps(lo, hi);
    lo = _mm_hadd_ps(lo, lo);
    lo = _mm_hadd_ps(lo, lo);
    return _mm_cvtss_f32(lo);
#endif
}

static vm_vec gather_f(const vm_float_t *base, const int *idx)
{
#if defined(VECMAT_USE_F64)
    const __m128i i4 = _mm_loadu_si128((const __m128i *)idx);
    return _mm256_i32gather_pd(base, i4, 8);
#else
    const __m256i i8 = _mm256_loadu_si256((const __m256i *)idx);
    return _mm256_i32gather_ps(base, i8, 4);
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

vm_float_t vm_sph_density_avx2(const vm_float_t *x, const vm_float_t *y,
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
    const vm_vec yi = (dim >= 2) ? vm_set1(y[i]) : vm_setzero();
    const vm_vec zi = (dim >= 3) ? vm_set1(z[i]) : vm_setzero();
    const vm_vec vinv_h = vm_set1(inv_h);
    const vm_vec vinv_hd = vm_set1(inv_hd);
    const vm_vec vsupport = vm_set1(support2);
    vm_vec acc = vm_setzero();

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

        const vm_vec inside = vm_cmp_lt(r2, vsupport);
        const vm_vec r = vm_sqrt(vm_max(r2, vm_set1(VM_F(0.0))));
        const vm_vec q = vm_mul(r, vinv_h);
        const vm_vec w = vm_mul(shape_f(q, kernel), vinv_hd);
        const vm_vec mj = gather_f(m, neighbors + k);

        acc = vm_add(acc, vm_and(vm_mul(mj, w), inside));
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

bool vm_sph_pressure_grad_avx2(vm_float_t *gx, vm_float_t *gy, vm_float_t *gz,
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
    const vm_vec yi = (dim >= 2) ? vm_set1(y[i]) : vm_setzero();
    const vm_vec zi = (dim >= 3) ? vm_set1(z[i]) : vm_setzero();
    const vm_vec vinv_h = vm_set1(inv_h);
    const vm_vec vinv_hd = vm_set1(inv_hd);
    const vm_vec vsupport = vm_set1(support2);
    const vm_vec vpit = vm_set1(pi_term);

    vm_vec agx = vm_setzero();
    vm_vec agy = vm_setzero();
    vm_vec agz = vm_setzero();

    int k = 0;
    for (; k + VM_LANES <= nn; k += VM_LANES) {
        const vm_vec xj = gather_f(x, neighbors + k);
        const vm_vec dx = vm_sub(xi, xj);

        vm_vec dy = vm_setzero();
        vm_vec dz = vm_setzero();
        vm_vec r2 = vm_mul(dx, dx);

        if (dim >= 2) {
            dy = vm_sub(yi, gather_f(y, neighbors + k));
            r2 = vm_add(r2, vm_mul(dy, dy));
        }
        if (dim >= 3) {
            dz = vm_sub(zi, gather_f(z, neighbors + k));
            r2 = vm_add(r2, vm_mul(dz, dz));
        }

        const vm_vec inside = vm_cmp_lt(r2, vsupport);
        const vm_vec alive = vm_cmp_gt(r2, vm_set1(VM_F(1.0e-20)));
        const vm_vec keep = vm_and(inside, alive);
        const vm_vec r = vm_sqrt(vm_max(r2, vm_set1(VM_F(1.0e-20))));
        const vm_vec q = vm_mul(r, vinv_h);
        const vm_vec dw = vm_mul(shape_df(q, kernel), vinv_hd);
        const vm_vec mj = gather_f(m, neighbors + k);
        const vm_vec pj = gather_f(p, neighbors + k);
        const vm_vec rhoj = gather_f(rho, neighbors + k);

#if defined(VECMAT_USE_F64)
        if (_mm256_movemask_pd(_mm256_cmp_pd(rhoj, vm_set1(VM_F(0.0)), _CMP_LE_OQ)) != 0) {
            return false;
        }
#else
        if (_mm256_movemask_ps(_mm256_cmp_ps(rhoj, vm_set1(VM_F(0.0)), _CMP_LE_OQ)) != 0) {
            return false;
        }
#endif

        const vm_vec coeff = vm_add(vpit, vm_div(pj, vm_mul(rhoj, rhoj)));
        const vm_vec scale = vm_and(vm_mul(mj, vm_mul(coeff, vm_div(dw, r))), keep);

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

bool vm_sph_forces_avx2(vm_float_t *gx, vm_float_t *gy, vm_float_t *gz,
                        vm_float_t *ax, vm_float_t *ay, vm_float_t *az,
                        const vm_float_t *x, const vm_float_t *y,
                        const vm_float_t *z,
                        const vm_float_t *vx, const vm_float_t *vy,
                        const vm_float_t *vz,
                        const vm_float_t *m, const vm_float_t *rho,
                        const vm_float_t *p, const vm_float_t *c,
                        const int i, const int *neighbors, const int nn,
                        const vm_float_t h, const vm_float_t alpha,
                        const vm_float_t beta, const vm_float_t eta,
                        const int dim, const int kernel)
{
    if (!gx || !gy || !ax || !ay || !x || !vx || !m || !rho || !p || !c
        || (dim >= 2 && (!y || !vy)) || (dim >= 3 && (!z || !vz || !gz || !az))
        || h <= VM_F(0.0) || dim < 1 || dim > 3 || rho[i] <= VM_F(0.0)) {
        return false;
    }

    *gx = VM_F(0.0);
    *gy = VM_F(0.0);
    *ax = VM_F(0.0);
    *ay = VM_F(0.0);

    if (gz) {
        *gz = VM_F(0.0);
    }

    if (az) {
        *az = VM_F(0.0);
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
    const vm_float_t eta_h2 = (eta * h) * (eta * h);
    const vm_float_t pi_term = p[i] / (rho[i] * rho[i]);

    const vm_vec xi = vm_set1(x[i]);
    const vm_vec yi = (dim >= 2) ? vm_set1(y[i]) : vm_setzero();
    const vm_vec zi = (dim >= 3) ? vm_set1(z[i]) : vm_setzero();
    const vm_vec vxi = vm_set1(vx[i]);
    const vm_vec vyi = (dim >= 2) ? vm_set1(vy[i]) : vm_setzero();
    const vm_vec vzi = (dim >= 3) ? vm_set1(vz[i]) : vm_setzero();
    const vm_vec vinv_h = vm_set1(inv_h);
    const vm_vec vinv_hd = vm_set1(inv_hd);
    const vm_vec vsupport = vm_set1(support2);
    const vm_vec vpit = vm_set1(pi_term);
    const vm_vec vh = vm_set1(h);
    const vm_vec veta = vm_set1(eta_h2);
    const vm_vec valpha = vm_set1(alpha);
    const vm_vec vbeta = vm_set1(beta);
    const vm_vec vrho_i = vm_set1(rho[i]);
    const vm_vec vc_i = vm_set1(c[i]);
    const vm_vec half = vm_set1(VM_F(0.5));
    vm_vec ggx = vm_setzero(), ggy = vm_setzero(), ggz = vm_setzero();
    vm_vec aax = vm_setzero(), aay = vm_setzero(), aaz = vm_setzero();

    int k = 0;
    for (; k + VM_LANES <= nn; k += VM_LANES) {
        const vm_vec xj = gather_f(x, neighbors + k);
        const vm_vec dx = vm_sub(xi, xj);
        vm_vec dy = vm_setzero();
        vm_vec dz = vm_setzero();
        vm_vec r2 = vm_mul(dx, dx);

        if (dim >= 2) {
            dy = vm_sub(yi, gather_f(y, neighbors + k));
            r2 = vm_add(r2, vm_mul(dy, dy));
        }
        if (dim >= 3) {
            dz = vm_sub(zi, gather_f(z, neighbors + k));
            r2 = vm_add(r2, vm_mul(dz, dz));
        }

        const vm_vec inside = vm_cmp_lt(r2, vsupport);
        const vm_vec alive = vm_cmp_gt(r2, vm_set1(VM_F(1.0e-20)));
        const vm_vec keep = vm_and(inside, alive);
        const vm_vec r = vm_sqrt(vm_max(r2, vm_set1(VM_F(1.0e-20))));
        const vm_vec q = vm_mul(r, vinv_h);
        const vm_vec dw = vm_mul(shape_df(q, kernel), vinv_hd);
        const vm_vec dw_over_r = vm_div(dw, r);
        const vm_vec mj = gather_f(m, neighbors + k);
        const vm_vec pj = gather_f(p, neighbors + k);
        const vm_vec rhoj = gather_f(rho, neighbors + k);

#if defined(VECMAT_USE_F64)
        if (_mm256_movemask_pd(_mm256_cmp_pd(rhoj, vm_set1(VM_F(0.0)), _CMP_LE_OQ)) != 0) {
            return false;
        }
#else
        if (_mm256_movemask_ps(_mm256_cmp_ps(rhoj, vm_set1(VM_F(0.0)), _CMP_LE_OQ)) != 0) {
            return false;
        }
#endif

        const vm_vec coeff = vm_add(vpit, vm_div(pj, vm_mul(rhoj, rhoj)));
        const vm_vec gscale = vm_and(vm_mul(mj, vm_mul(coeff, dw_over_r)), keep);
        ggx = vm_add(ggx, vm_mul(gscale, dx));

        if (dim >= 2) {
            ggy = vm_add(ggy, vm_mul(gscale, dy));
        }
        if (dim >= 3) {
            ggz = vm_add(ggz, vm_mul(gscale, dz));
        }

        const vm_vec vxj = gather_f(vx, neighbors + k);
        vm_vec vdot = vm_mul(vm_sub(vxi, vxj), dx);

        if (dim >= 2) {
            vdot = vm_add(vdot, vm_mul(vm_sub(vyi, gather_f(vy, neighbors + k)), dy));
        }
        if (dim >= 3) {
            vdot = vm_add(vdot, vm_mul(vm_sub(vzi, gather_f(vz, neighbors + k)), dz));
        }

        const vm_vec approaching = vm_cmp_lt(vdot, vm_setzero());
        const vm_vec akeep = vm_and(keep, approaching);
        const vm_vec mu = vm_div(vm_mul(vh, vdot), vm_add(r2, veta));
        const vm_vec cj = gather_f(c, neighbors + k);
        const vm_vec c_bar = vm_mul(half, vm_add(vc_i, cj));
        const vm_vec rho_bar = vm_mul(half, vm_add(vrho_i, rhoj));
        const vm_vec pi = vm_div(vm_add(vm_mul(vm_mul(vm_set1(VM_F(-1.0)), valpha), vm_mul(c_bar, mu)),
                                        vm_mul(vbeta, vm_mul(mu, mu))),
                                 rho_bar);
        const vm_vec ascale = vm_and(vm_mul(vm_mul(vm_set1(VM_F(-1.0)), mj), vm_mul(pi, dw_over_r)), akeep);
        aax = vm_add(aax, vm_mul(ascale, dx));

        if (dim >= 2) {
            aay = vm_add(aay, vm_mul(ascale, dy));
        }
        if (dim >= 3) {
            aaz = vm_add(aaz, vm_mul(ascale, dz));
        }
    }

    *gx = hsum(ggx);
    *gy = hsum(ggy);
    *ax = hsum(aax);
    *ay = hsum(aay);

    if (gz) {
        *gz = hsum(ggz);
    }
    if (az) {
        *az = hsum(aaz);
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
        const vm_float_t gscale = m[j] * (pi_term + p[j] / (rho[j] * rho[j])) * dw / r;
        *gx += gscale * dx;

        if (dim >= 2) {
            *gy += gscale * dy;
        }
        if (dim >= 3 && gz) {
            *gz += gscale * dz;
        }

        const vm_float_t vdot = (vx[i] - vx[j]) * dx
            + ((dim >= 2) ? (vy[i] - vy[j]) * dy : VM_F(0.0))
            + ((dim >= 3) ? (vz[i] - vz[j]) * dz : VM_F(0.0));

        const vm_float_t pi = vm_sph_pi(r, vdot, rho[i], rho[j], c[i], c[j],
                                        h, alpha, beta, eta);

        const vm_float_t ascale = -m[j] * pi * dw / r;
        *ax += ascale * dx;

        if (dim >= 2) {
            *ay += ascale * dy;
        }
        if (dim >= 3 && az) {
            *az += ascale * dz;
        }
    }

    return true;
}

bool vm_sph_artvisc_avx2(vm_float_t *ax, vm_float_t *ay, vm_float_t *az,
                         const vm_float_t *x, const vm_float_t *y,
                         const vm_float_t *z,
                         const vm_float_t *vx, const vm_float_t *vy,
                         const vm_float_t *vz,
                         const vm_float_t *m, const vm_float_t *rho,
                         const vm_float_t *c,
                         const int i, const int *neighbors, const int nn,
                         const vm_float_t h, const vm_float_t alpha,
                         const vm_float_t beta, const vm_float_t eta,
                         const int dim, const int kernel)
{
    if (!ax || !ay || !x || !vx || !m || !rho || !c
        || (dim >= 2 && (!y || !vy)) || (dim >= 3 && (!z || !vz || !az))
        || h <= VM_F(0.0) || dim < 1 || dim > 3 || rho[i] <= VM_F(0.0)) {
        return false;
    }

    *ax = VM_F(0.0);
    *ay = VM_F(0.0);
    if (az) {
        *az = VM_F(0.0);
    }

    if (!neighbors || nn <= 0) {
        return true;
    }

    const vm_float_t sig = sigma_of(dim, kernel);

    /* dW/dr scales as h^(dim+1), same as vm_sph_dw and the pressure kernel. */
    vm_float_t hd = h;
    for (int d = 0; d < dim; ++d) {
        hd *= h;
    }

    const vm_float_t inv_hd = sig / hd;
    const vm_float_t inv_h = VM_F(1.0) / h;
    const vm_float_t support2 = VM_F(4.0) * h * h;
    const vm_float_t eta_h2 = (eta * h) * (eta * h);

    const vm_vec xi = vm_set1(x[i]);
    const vm_vec yi = (dim >= 2) ? vm_set1(y[i]) : vm_setzero();
    const vm_vec zi = (dim >= 3) ? vm_set1(z[i]) : vm_setzero();
    const vm_vec vxi = vm_set1(vx[i]);
    const vm_vec vyi = (dim >= 2) ? vm_set1(vy[i]) : vm_setzero();
    const vm_vec vzi = (dim >= 3) ? vm_set1(vz[i]) : vm_setzero();
    const vm_vec vinv_h = vm_set1(inv_h);
    const vm_vec vinv_hd = vm_set1(inv_hd);
    const vm_vec vsupport = vm_set1(support2);
    const vm_vec vh = vm_set1(h);
    const vm_vec veta = vm_set1(eta_h2);
    const vm_vec valpha = vm_set1(alpha);
    const vm_vec vbeta = vm_set1(beta);
    const vm_vec vrho_i = vm_set1(rho[i]);
    const vm_vec vc_i = vm_set1(c[i]);
    const vm_vec half = vm_set1(VM_F(0.5));

    vm_vec aax = vm_setzero();
    vm_vec aay = vm_setzero();
    vm_vec aaz = vm_setzero();

    int k = 0;
    for (; k + VM_LANES <= nn; k += VM_LANES) {
        const vm_vec xj = gather_f(x, neighbors + k);
        const vm_vec dx = vm_sub(xi, xj);

        vm_vec dy = vm_setzero();
        vm_vec dz = vm_setzero();
        vm_vec r2 = vm_mul(dx, dx);

        if (dim >= 2) {
            dy = vm_sub(yi, gather_f(y, neighbors + k));
            r2 = vm_add(r2, vm_mul(dy, dy));
        }

        if (dim >= 3) {
            dz = vm_sub(zi, gather_f(z, neighbors + k));
            r2 = vm_add(r2, vm_mul(dz, dz));
        }

        const vm_vec inside = vm_cmp_lt(r2, vsupport);
        const vm_vec alive = vm_cmp_gt(r2, vm_set1(VM_F(1.0e-20)));
        const vm_vec rhoj = gather_f(rho, neighbors + k);

#if defined(VECMAT_USE_F64)
        if (_mm256_movemask_pd(_mm256_cmp_pd(rhoj, vm_set1(VM_F(0.0)), _CMP_LE_OQ)) != 0) {
            return false;
        }
#else
        if (_mm256_movemask_ps(_mm256_cmp_ps(rhoj, vm_set1(VM_F(0.0)), _CMP_LE_OQ)) != 0) {
            return false;
        }
#endif

        const vm_vec vxj = gather_f(vx, neighbors + k);
        const vm_vec dvx = vm_sub(vxi, vxj);
        vm_vec vdot = vm_mul(dvx, dx);

        if (dim >= 2) {
            const vm_vec vyj = gather_f(vy, neighbors + k);
            vdot = vm_add(vdot, vm_mul(vm_sub(vyi, vyj), dy));
        }
        if (dim >= 3) {
            const vm_vec vzj = gather_f(vz, neighbors + k);
            vdot = vm_add(vdot, vm_mul(vm_sub(vzi, vzj), dz));
        }

        const vm_vec approaching = vm_cmp_lt(vdot, vm_setzero());
        const vm_vec keep = vm_and(vm_and(inside, alive), approaching);
        const vm_vec r = vm_sqrt(vm_max(r2, vm_set1(VM_F(1.0e-20))));
        const vm_vec q = vm_mul(r, vinv_h);
        const vm_vec dw = vm_mul(shape_df(q, kernel), vinv_hd);
        const vm_vec mu = vm_div(vm_mul(vh, vdot), vm_add(r2, veta));
        const vm_vec cj = gather_f(c, neighbors + k);
        const vm_vec c_bar = vm_mul(half, vm_add(vc_i, cj));
        const vm_vec rho_bar = vm_mul(half, vm_add(vrho_i, rhoj));
        const vm_vec pi = vm_div(vm_add(vm_mul(vm_mul(vm_set1(VM_F(-1.0)), valpha), vm_mul(c_bar, mu)),
                                        vm_mul(vbeta, vm_mul(mu, mu))),
                                 rho_bar);
        const vm_vec mj = gather_f(m, neighbors + k);
        const vm_vec scale = vm_and(vm_mul(vm_mul(vm_set1(VM_F(-1.0)), mj), vm_mul(pi, vm_div(dw, r))), keep);
        aax = vm_add(aax, vm_mul(scale, dx));

        if (dim >= 2) {
            aay = vm_add(aay, vm_mul(scale, dy));
        }
        if (dim >= 3) {
            aaz = vm_add(aaz, vm_mul(scale, dz));
        }
    }

    *ax = hsum(aax);
    *ay = hsum(aay);

    if (az) {
        *az = hsum(aaz);
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
        const vm_float_t vdot = (vx[i] - vx[j]) * dx
            + ((dim >= 2) ? (vy[i] - vy[j]) * dy : VM_F(0.0))
            + ((dim >= 3) ? (vz[i] - vz[j]) * dz : VM_F(0.0));
        const vm_float_t pi = vm_sph_pi(r, vdot, rho[i], rho[j], c[i], c[j],
                                        h, alpha, beta, eta);
        const vm_float_t dw = vm_sph_dw(r, h, dim, kernel);
        const vm_float_t scale = -m[j] * pi * dw / r;
        *ax += scale * dx;

        if (dim >= 2) {
            *ay += scale * dy;
        }
        if (dim >= 3 && az) {
            *az += scale * dz;
        }
    }

    return true;
}
