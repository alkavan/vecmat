// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

/**
 * @file
 * @brief NEON batched ideal and stiffened gas.
 *
 * 4-wide f32 / 2-wide f64. No gather; values are packed scalar. A bad chunk
 * falls back to the scalar call from that item.
 */

#include <arm_neon.h>
#include <vecmat.h>

#if defined(VECMAT_USE_F64)
#define VM_LANES 2
typedef float64x2_t vm_vec;
#define vm_set1   vdupq_n_f64
#define vm_mul    vmulq_f64
#define vm_sub    vsubq_f64
#define vm_add    vaddq_f64
#define vm_div    vdivq_f64
#define vm_sqrt   vsqrtq_f64
#define vm_load   vld1q_f64
#define vm_store  vst1q_f64
#else
#define VM_LANES 4
typedef float32x4_t vm_vec;
#define vm_set1   vdupq_n_f32
#define vm_mul    vmulq_f32
#define vm_sub    vsubq_f32
#define vm_add    vaddq_f32
#define vm_div    vdivq_f32
#define vm_sqrt   vsqrtq_f32
#define vm_load   vld1q_f32
#define vm_store  vst1q_f32
#endif

static vm_vec load_s(const vm_float_t *base, const int stride, const int k)
{
    vm_float_t tmp[VM_LANES];
    for (int i = 0; i < VM_LANES; ++i) {
        tmp[i] = base[(k + i) * stride];
    }

    return vm_load(tmp);
}

static int any_le_zero(const vm_vec v)
{
#if defined(VECMAT_USE_F64)
    const uint64x2_t m = vcleq_f64(v, vdupq_n_f64(0.0));
    return (vgetq_lane_u64(m, 0) | vgetq_lane_u64(m, 1)) != 0;
#else
    const uint32x4_t m = vcleq_f32(v, vdupq_n_f32(0.0f));
    return vmaxvq_u32(m) != 0;
#endif
}

static int any_lt_zero(const vm_vec v)
{
#if defined(VECMAT_USE_F64)
    const uint64x2_t m = vcltq_f64(v, vdupq_n_f64(0.0));
    return (vgetq_lane_u64(m, 0) | vgetq_lane_u64(m, 1)) != 0;
#else
    const uint32x4_t m = vcltq_f32(v, vdupq_n_f32(0.0f));
    return vmaxvq_u32(m) != 0;
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

bool vm_eos_ideal_n_neon(vm_float_t *p, vm_float_t *c, const vm_ideal_gas *eos,
                         const vm_float_t *rho, const int rho_stride,
                         const vm_float_t *e, const int e_stride, const int n)
{
    const vm_vec gm1 = vm_set1(eos->gamma - VM_F(1.0));
    const vm_vec gamma = vm_set1(eos->gamma);

    int k = 0;
    for (; k + VM_LANES <= n; k += VM_LANES) {
        const vm_vec vr = load_s(rho, rho_stride, k);
        if (any_le_zero(vr)) {
            return ideal_tail(p, c, eos, rho, rho_stride, e, e_stride, k, n);
        }

        const vm_vec ve = load_s(e, e_stride, k);
        const vm_vec vp = vm_mul(gm1, vm_mul(vr, ve));
        const vm_vec vc2 = vm_div(vm_mul(gamma, vp), vr);

        if (any_lt_zero(vc2)) {
            return ideal_tail(p, c, eos, rho, rho_stride, e, e_stride, k, n);
        }

        vm_store(p + k, vp);
        vm_store(c + k, vm_sqrt(vc2));
    }

    return ideal_tail(p, c, eos, rho, rho_stride, e, e_stride, k, n);
}

bool vm_eos_stiffened_n_neon(vm_float_t *p, vm_float_t *c, const vm_stiff_gas *eos,
                             const vm_float_t *rho, const int rho_stride,
                             const vm_float_t *e, const int e_stride, const int n)
{
    const vm_vec gm1 = vm_set1(eos->gamma - VM_F(1.0));
    const vm_vec gamma = vm_set1(eos->gamma);
    const vm_vec gpinf = vm_set1(eos->gamma * eos->p_inf);
    const vm_vec pinf = vm_set1(eos->p_inf);

    int k = 0;
    for (; k + VM_LANES <= n; k += VM_LANES) {
        const vm_vec vr = load_s(rho, rho_stride, k);
        if (any_le_zero(vr)) {
            return stiff_tail(p, c, eos, rho, rho_stride, e, e_stride, k, n);
        }

        const vm_vec ve = load_s(e, e_stride, k);
        const vm_vec vp = vm_sub(vm_mul(gm1, vm_mul(vr, ve)), gpinf);
        const vm_vec vc2 = vm_div(vm_mul(gamma, vm_add(vp, pinf)), vr);

        if (any_lt_zero(vc2)) {
            return stiff_tail(p, c, eos, rho, rho_stride, e, e_stride, k, n);
        }

        vm_store(p + k, vp);
        vm_store(c + k, vm_sqrt(vc2));
    }

    return stiff_tail(p, c, eos, rho, rho_stride, e, e_stride, k, n);
}
