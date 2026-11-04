// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

/**
 * @file
 * @brief NEON batch3: strided 3×3 multiply, inverse, and Voigt-6 ddot.
 *
 * Items are processed 4-wide (f32) or 2-wide (f64). No gather; elements are
 * packed scalar then loaded. Polar stays on `mat3_polar`.
 */

#include <arm_neon.h>
#include <vecmat.h>

#if defined(VECMAT_USE_F64)
#define VM_LANES 2
typedef float64x2_t vm_vec;
typedef uint64x2_t vm_mask;
#define vm_set1           vdupq_n_f64
#define vm_add            vaddq_f64
#define vm_sub            vsubq_f64
#define vm_mul            vmulq_f64
#define vm_div            vdivq_f64
#define vm_max            vmaxq_f64
#define vm_abs            vabsq_f64
#define vm_cmp_le(a, b)   vcleq_f64((a), (b))
#define vm_blend(a, b, m) vbslq_f64((m), (b), (a))
#define vm_load           vld1q_f64
#define vm_store          vst1q_f64
#else
#define VM_LANES 4
typedef float32x4_t vm_vec;
typedef uint32x4_t vm_mask;
#define vm_set1           vdupq_n_f32
#define vm_add            vaddq_f32
#define vm_sub            vsubq_f32
#define vm_mul            vmulq_f32
#define vm_div            vdivq_f32
#define vm_max            vmaxq_f32
#define vm_abs            vabsq_f32
#define vm_cmp_le(a, b)   vcleq_f32((a), (b))
#define vm_blend(a, b, m) vbslq_f32((m), (b), (a))
#define vm_load           vld1q_f32
#define vm_store          vst1q_f32
#endif

static vm_vec load_e(const vm_float_t *base, const int stride, const int k, const int e)
{
    vm_float_t tmp[VM_LANES];

    for (int i = 0; i < VM_LANES; ++i) {
        tmp[i] = base[(k + i) * stride + e];
    }

    return vm_load(tmp);
}

static void store_e(vm_float_t *base, const int stride, const int k, const int e, const vm_vec v)
{
    vm_float_t tmp[VM_LANES];
    vm_store(tmp, v);

    for (int i = 0; i < VM_LANES; ++i) {
        base[(k + i) * stride + e] = tmp[i];
    }
}

static int any_le(const vm_vec v, const vm_vec s)
{
#if defined(VECMAT_USE_F64)
    const uint64x2_t m = vcleq_f64(v, s);
    return (vgetq_lane_u64(m, 0) | vgetq_lane_u64(m, 1)) != 0;
#else
    const uint32x4_t m = vcleq_f32(v, s);
    return vmaxvq_u32(m) != 0;
#endif
}

bool vm_batch3_mul_neon(vm_float_t *C, const int c_stride,
                        const vm_float_t *A, const int a_stride,
                        const vm_float_t *B, const int b_stride,
                        const int n)
{
    int k = 0;
    for (; k + VM_LANES <= n; k += VM_LANES) {
        vm_vec a[9], b[9];
        for (int e = 0; e < 9; ++e) {
            a[e] = load_e(A, a_stride, k, e);
            b[e] = load_e(B, b_stride, k, e);
        }

        const vm_vec c0 = vm_add(vm_add(vm_mul(a[0], b[0]), vm_mul(a[3], b[1])), vm_mul(a[6], b[2]));
        const vm_vec c1 = vm_add(vm_add(vm_mul(a[1], b[0]), vm_mul(a[4], b[1])), vm_mul(a[7], b[2]));
        const vm_vec c2 = vm_add(vm_add(vm_mul(a[2], b[0]), vm_mul(a[5], b[1])), vm_mul(a[8], b[2]));
        const vm_vec c3 = vm_add(vm_add(vm_mul(a[0], b[3]), vm_mul(a[3], b[4])), vm_mul(a[6], b[5]));
        const vm_vec c4 = vm_add(vm_add(vm_mul(a[1], b[3]), vm_mul(a[4], b[4])), vm_mul(a[7], b[5]));
        const vm_vec c5 = vm_add(vm_add(vm_mul(a[2], b[3]), vm_mul(a[5], b[4])), vm_mul(a[8], b[5]));
        const vm_vec c6 = vm_add(vm_add(vm_mul(a[0], b[6]), vm_mul(a[3], b[7])), vm_mul(a[6], b[8]));
        const vm_vec c7 = vm_add(vm_add(vm_mul(a[1], b[6]), vm_mul(a[4], b[7])), vm_mul(a[7], b[8]));
        const vm_vec c8 = vm_add(vm_add(vm_mul(a[2], b[6]), vm_mul(a[5], b[7])), vm_mul(a[8], b[8]));

        store_e(C, c_stride, k, 0, c0);
        store_e(C, c_stride, k, 1, c1);
        store_e(C, c_stride, k, 2, c2);
        store_e(C, c_stride, k, 3, c3);
        store_e(C, c_stride, k, 4, c4);
        store_e(C, c_stride, k, 5, c5);
        store_e(C, c_stride, k, 6, c6);
        store_e(C, c_stride, k, 7, c7);
        store_e(C, c_stride, k, 8, c8);
    }

    for (; k < n; ++k) {
        matrix3 a, b;
        for (int e = 0; e < 9; ++e) {
            a.v[e] = A[k * a_stride + e];
            b.v[e] = B[k * b_stride + e];
        }

        const matrix3 c = mat3_mul(a, b);
        for (int e = 0; e < 9; ++e) {
            C[k * c_stride + e] = c.v[e];
        }
    }

    return true;
}

bool vm_batch3_inverse_neon(vm_float_t *C, const int c_stride,
                            const vm_float_t *A, const int a_stride,
                            const int n)
{
    int k = 0;
    for (; k + VM_LANES <= n; k += VM_LANES) {
        vm_vec m[9];
        for (int e = 0; e < 9; ++e) {
            m[e] = load_e(A, a_stride, k, e);
        }

        vm_vec scale = vm_abs(m[0]);
        for (int e = 1; e < 9; ++e) {
            scale = vm_max(scale, vm_abs(m[e]));
        }

        const vm_vec det = vm_add(vm_sub(vm_mul(m[0], vm_sub(vm_mul(m[4], m[8]), vm_mul(m[5], m[7]))),
                                         vm_mul(m[1], vm_sub(vm_mul(m[3], m[8]), vm_mul(m[5], m[6])))),
                                  vm_mul(m[2], vm_sub(vm_mul(m[3], m[7]), vm_mul(m[4], m[6]))));

        const vm_vec tol = vm_mul(vm_set1(VECMAT_EPSILON * VM_F(3.0)),
                                  vm_mul(vm_mul(scale, scale), scale));

        const vm_vec tol_use = vm_blend(tol, vm_set1(VECMAT_EPSILON),
                                        vm_cmp_le(scale, vm_set1(VM_F(0.0))));

        if (any_le(vm_abs(det), tol_use)) {
            return false;
        }

        const vm_vec inv_det = vm_div(vm_set1(VM_F(1.0)), det);
        const vm_vec c0 = vm_mul(vm_sub(vm_mul(m[4], m[8]), vm_mul(m[5], m[7])), inv_det);
        const vm_vec c1 = vm_mul(vm_sub(vm_mul(m[2], m[7]), vm_mul(m[1], m[8])), inv_det);
        const vm_vec c2 = vm_mul(vm_sub(vm_mul(m[1], m[5]), vm_mul(m[2], m[4])), inv_det);
        const vm_vec c3 = vm_mul(vm_sub(vm_mul(m[5], m[6]), vm_mul(m[3], m[8])), inv_det);
        const vm_vec c4 = vm_mul(vm_sub(vm_mul(m[0], m[8]), vm_mul(m[2], m[6])), inv_det);
        const vm_vec c5 = vm_mul(vm_sub(vm_mul(m[2], m[3]), vm_mul(m[0], m[5])), inv_det);
        const vm_vec c6 = vm_mul(vm_sub(vm_mul(m[3], m[7]), vm_mul(m[4], m[6])), inv_det);
        const vm_vec c7 = vm_mul(vm_sub(vm_mul(m[1], m[6]), vm_mul(m[0], m[7])), inv_det);
        const vm_vec c8 = vm_mul(vm_sub(vm_mul(m[0], m[4]), vm_mul(m[1], m[3])), inv_det);

        store_e(C, c_stride, k, 0, c0);
        store_e(C, c_stride, k, 1, c1);
        store_e(C, c_stride, k, 2, c2);
        store_e(C, c_stride, k, 3, c3);
        store_e(C, c_stride, k, 4, c4);
        store_e(C, c_stride, k, 5, c5);
        store_e(C, c_stride, k, 6, c6);
        store_e(C, c_stride, k, 7, c7);
        store_e(C, c_stride, k, 8, c8);
    }

    for (; k < n; ++k) {
        matrix3 a, c;
        for (int e = 0; e < 9; ++e) {
            a.v[e] = A[k * a_stride + e];
        }

        if (!mat3_inverse_ok(&c, &a)) {
            return false;
        }

        for (int e = 0; e < 9; ++e) {
            C[k * c_stride + e] = c.v[e];
        }
    }

    return true;
}

bool vm_batch3_ddot_neon(vm_float_t *d,
                         const vm_float_t *a, const int a_stride,
                         const vm_float_t *b, const int b_stride,
                         const int n)
{
    int k = 0;
    for (; k + VM_LANES <= n; k += VM_LANES) {
        vm_vec acc = vm_set1(VM_F(0.0));
        for (int e = 0; e < 3; ++e) {
            acc = vm_add(acc, vm_mul(load_e(a, a_stride, k, e), load_e(b, b_stride, k, e)));
        }

        vm_vec shear = vm_set1(VM_F(0.0));
        for (int e = 3; e < 6; ++e) {
            shear = vm_add(shear, vm_mul(load_e(a, a_stride, k, e), load_e(b, b_stride, k, e)));
        }

        acc = vm_add(acc, vm_mul(vm_set1(VM_F(2.0)), shear));
        vm_float_t tmp[VM_LANES];
        vm_store(tmp, acc);

        for (int i = 0; i < VM_LANES; ++i) {
            d[k + i] = tmp[i];
        }
    }

    for (; k < n; ++k) {
        vm_sym3 sa, sb;
        for (int e = 0; e < 6; ++e) {
            sa.c[e] = a[k * a_stride + e];
            sb.c[e] = b[k * b_stride + e];
        }
        d[k] = vm_sym3_ddot(&sa, &sb);
    }

    return true;
}
