// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

/**
 * @file
 * @brief AVX-512 neighbor loops for the SPH density sum and pressure gradient.
 *
 * Same kernels as the scalar path: cubic spline and Wendland C2, support
 * \(r < 2h\), normalized. Neighbors are gathered 16-wide (f32) or 8-wide
 * (f64). The scalar functions stay the fallback.
 */

#include <immintrin.h>
#include <math.h>
#include <vecmat.h>

#if defined(VECMAT_USE_F64)
#define VM_LANES 8
typedef __m512d vm_vec;
typedef __mmask8 vm_mask;
#define vm_set1           _mm512_set1_pd
#define vm_setzero        _mm512_setzero_pd
#define vm_add            _mm512_add_pd
#define vm_sub            _mm512_sub_pd
#define vm_mul            _mm512_mul_pd
#define vm_div            _mm512_div_pd
#define vm_sqrt           _mm512_sqrt_pd
#define vm_max            _mm512_max_pd
#define vm_cmp_lt(a, b)   _mm512_cmp_pd_mask((a), (b), _CMP_LT_OQ)
#define vm_cmp_gt(a, b)   _mm512_cmp_pd_mask((a), (b), _CMP_GT_OQ)
#define vm_cmp_le(a, b)   _mm512_cmp_pd_mask((a), (b), _CMP_LE_OQ)
#define vm_blend(a, b, m) _mm512_mask_blend_pd((m), (a), (b))
#define vm_maskz(m, v)    _mm512_maskz_mov_pd((m), (v))
#define vm_hsum           _mm512_reduce_add_pd
#else
#define VM_LANES 16
typedef __m512 vm_vec;
typedef __mmask16 vm_mask;
#define vm_set1           _mm512_set1_ps
#define vm_setzero        _mm512_setzero_ps
#define vm_add            _mm512_add_ps
#define vm_sub            _mm512_sub_ps
#define vm_mul            _mm512_mul_ps
#define vm_div            _mm512_div_ps
#define vm_sqrt           _mm512_sqrt_ps
#define vm_max            _mm512_max_ps
#define vm_cmp_lt(a, b)   _mm512_cmp_ps_mask((a), (b), _CMP_LT_OQ)
#define vm_cmp_gt(a, b)   _mm512_cmp_ps_mask((a), (b), _CMP_GT_OQ)
#define vm_cmp_le(a, b)   _mm512_cmp_ps_mask((a), (b), _CMP_LE_OQ)
#define vm_blend(a, b, m) _mm512_mask_blend_ps((m), (a), (b))
#define vm_maskz(m, v)    _mm512_maskz_mov_ps((m), (v))
#define vm_hsum           _mm512_reduce_add_ps
#endif

static vm_vec gather_f(const vm_float_t *base, const int *idx)
{
#if defined(VECMAT_USE_F64)
    const __m256i i8 = _mm256_loadu_si256((const __m256i *)idx);
    return _mm512_i32gather_pd(i8, base, 8);
#else
    const __m512i i16 = _mm512_loadu_si512(idx);
    return _mm512_i32gather_ps(i16, base, 4);
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

vm_float_t vm_sph_density_avx512(const vm_float_t *x, const vm_float_t *y,
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

        const vm_mask inside = vm_cmp_lt(r2, vsupport);
        const vm_vec r = vm_sqrt(vm_max(r2, vm_set1(VM_F(0.0))));
        const vm_vec q = vm_mul(r, vinv_h);
        const vm_vec w = vm_mul(shape_f(q, kernel), vinv_hd);
        const vm_vec mj = gather_f(m, neighbors + k);

        acc = vm_add(acc, vm_maskz(inside, vm_mul(mj, w)));
    }

    rho += vm_hsum(acc);
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

bool vm_sph_pressure_grad_avx512(vm_float_t *gx, vm_float_t *gy, vm_float_t *gz,
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

        const vm_mask inside = vm_cmp_lt(r2, vsupport);
        const vm_mask alive = vm_cmp_gt(r2, vm_set1(VM_F(1.0e-20)));
        const vm_mask keep = inside & alive;
        const vm_vec r = vm_sqrt(vm_max(r2, vm_set1(VM_F(1.0e-20))));
        const vm_vec q = vm_mul(r, vinv_h);
        const vm_vec dw = vm_mul(shape_df(q, kernel), vinv_hd);
        const vm_vec mj = gather_f(m, neighbors + k);
        const vm_vec pj = gather_f(p, neighbors + k);
        const vm_vec rhoj = gather_f(rho, neighbors + k);

        if (vm_cmp_le(rhoj, vm_set1(VM_F(0.0))) != 0) {
            return false;
        }

        const vm_vec coeff = vm_add(vpit, vm_div(pj, vm_mul(rhoj, rhoj)));
        const vm_vec scale = vm_maskz(keep, vm_mul(mj, vm_mul(coeff, vm_div(dw, r))));

        agx = vm_add(agx, vm_mul(scale, dx));

        if (dim >= 2) {
            agy = vm_add(agy, vm_mul(scale, dy));
        }
        if (dim >= 3) {
            agz = vm_add(agz, vm_mul(scale, dz));
        }
    }

    *gx = vm_hsum(agx);
    *gy = vm_hsum(agy);
    if (gz) {
        *gz = vm_hsum(agz);
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
