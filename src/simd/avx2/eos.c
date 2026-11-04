// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

/**
 * @file
 * @brief AVX2 batched ideal and stiffened gas.
 *
 * 8-wide f32 / 4-wide f64. A chunk with a bad density or negative \(c^2\)
 * falls back to the scalar call from that item, so a failure leaves the
 * same items unwritten as the scalar batch.
 */

#include <immintrin.h>
#include <vecmat.h>

#if defined(VECMAT_USE_F64)
#define VM_LANES 4
typedef __m256d vm_vec;
#define vm_set1   _mm256_set1_pd
#define vm_mul    _mm256_mul_pd
#define vm_sub    _mm256_sub_pd
#define vm_add    _mm256_add_pd
#define vm_div    _mm256_div_pd
#define vm_sqrt   _mm256_sqrt_pd
#define vm_cmp_le(a, b) _mm256_cmp_pd((a), (b), _CMP_LE_OQ)
#define vm_cmp_lt(a, b) _mm256_cmp_pd((a), (b), _CMP_LT_OQ)
#define vm_store  _mm256_storeu_pd
#else
#define VM_LANES 8
typedef __m256 vm_vec;
#define vm_set1   _mm256_set1_ps
#define vm_mul    _mm256_mul_ps
#define vm_sub    _mm256_sub_ps
#define vm_add    _mm256_add_ps
#define vm_div    _mm256_div_ps
#define vm_sqrt   _mm256_sqrt_ps
#define vm_cmp_le(a, b) _mm256_cmp_ps((a), (b), _CMP_LE_OQ)
#define vm_cmp_lt(a, b) _mm256_cmp_ps((a), (b), _CMP_LT_OQ)
#define vm_store  _mm256_storeu_ps
#endif

static vm_vec load_s(const vm_float_t *base, const int stride, const int k)
{
    int ix[VM_LANES];
    for (int i = 0; i < VM_LANES; ++i) {
        ix[i] = (k + i) * stride;
    }
#if defined(VECMAT_USE_F64)
    const __m128i idx = _mm_loadu_si128((const __m128i *)ix);
    return _mm256_i32gather_pd(base, idx, 8);
#else
    const __m256i idx = _mm256_loadu_si256((const __m256i *)ix);
    return _mm256_i32gather_ps(base, idx, 4);
#endif
}

static int any_mask(const vm_vec m)
{
#if defined(VECMAT_USE_F64)
    return _mm256_movemask_pd(m) != 0;
#else
    return _mm256_movemask_ps(m) != 0;
#endif
}

static bool ideal_tail(vm_float_t *p, vm_float_t *c, const vm_ideal_gas *eos,
                       const vm_float_t *rho, const int rho_stride,
                       const vm_float_t *e, const int e_stride,
                       const int from, const int n)
{
    for (int i = from; i < n; ++i) {
        vm_eos_state st;
        if (!vm_eos_ideal(&st, eos, rho[i * rho_stride], e[i * e_stride])) {
            return false;
        }
        p[i] = st.p;
        c[i] = st.c;
    }

    return true;
}

static bool stiff_tail(vm_float_t *p, vm_float_t *c, const vm_stiff_gas *eos,
                       const vm_float_t *rho, const int rho_stride,
                       const vm_float_t *e, const int e_stride,
                       const int from, const int n)
{
    for (int i = from; i < n; ++i) {
        vm_eos_state st;
        if (!vm_eos_stiffened(&st, eos, rho[i * rho_stride], e[i * e_stride])) {
            return false;
        }
        p[i] = st.p;
        c[i] = st.c;
    }

    return true;
}

bool vm_eos_ideal_n_avx2(vm_float_t *p, vm_float_t *c, const vm_ideal_gas *eos,
                         const vm_float_t *rho, const int rho_stride,
                         const vm_float_t *e, const int e_stride, const int n)
{
    const vm_vec gm1 = vm_set1(eos->gamma - VM_F(1.0));
    const vm_vec gamma = vm_set1(eos->gamma);
    const vm_vec zero = vm_set1(VM_F(0.0));

    int k = 0;
    for (; k + VM_LANES <= n; k += VM_LANES) {
        const vm_vec vr = load_s(rho, rho_stride, k);
        if (any_mask(vm_cmp_le(vr, zero))) {
            return ideal_tail(p, c, eos, rho, rho_stride, e, e_stride, k, n);
        }

        const vm_vec ve = load_s(e, e_stride, k);
        const vm_vec vp = vm_mul(gm1, vm_mul(vr, ve));
        const vm_vec vc2 = vm_div(vm_mul(gamma, vp), vr);

        if (any_mask(vm_cmp_lt(vc2, zero))) {
            return ideal_tail(p, c, eos, rho, rho_stride, e, e_stride, k, n);
        }

        vm_store(p + k, vp);
        vm_store(c + k, vm_sqrt(vc2));
    }

    return ideal_tail(p, c, eos, rho, rho_stride, e, e_stride, k, n);
}

bool vm_eos_stiffened_n_avx2(vm_float_t *p, vm_float_t *c, const vm_stiff_gas *eos,
                             const vm_float_t *rho, const int rho_stride,
                             const vm_float_t *e, const int e_stride, const int n)
{
    const vm_vec gm1 = vm_set1(eos->gamma - VM_F(1.0));
    const vm_vec gamma = vm_set1(eos->gamma);
    const vm_vec gpinf = vm_set1(eos->gamma * eos->p_inf);
    const vm_vec pinf = vm_set1(eos->p_inf);
    const vm_vec zero = vm_set1(VM_F(0.0));

    int k = 0;
    for (; k + VM_LANES <= n; k += VM_LANES) {
        const vm_vec vr = load_s(rho, rho_stride, k);
        if (any_mask(vm_cmp_le(vr, zero))) {
            return stiff_tail(p, c, eos, rho, rho_stride, e, e_stride, k, n);
        }

        const vm_vec ve = load_s(e, e_stride, k);
        const vm_vec vp = vm_sub(vm_mul(gm1, vm_mul(vr, ve)), gpinf);
        const vm_vec vc2 = vm_div(vm_mul(gamma, vm_add(vp, pinf)), vr);

        if (any_mask(vm_cmp_lt(vc2, zero))) {
            return stiff_tail(p, c, eos, rho, rho_stride, e, e_stride, k, n);
        }

        vm_store(p + k, vp);
        vm_store(c + k, vm_sqrt(vc2));
    }

    return stiff_tail(p, c, eos, rho, rho_stride, e, e_stride, k, n);
}
