// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <string.h>
#include <vecmat.h>
#include "vecmat/batch3.h"
#include "vecmat/cpu.h"

#if defined(VECMAT_ENABLE_AVX2)
bool vm_batch3_mul_avx2(vm_float_t *C, int c_stride,
                        const vm_float_t *A, int a_stride,
                        const vm_float_t *B, int b_stride, int n);
bool vm_batch3_inverse_avx2(vm_float_t *C, int c_stride,
                            const vm_float_t *A, int a_stride, int n);
bool vm_batch3_ddot_avx2(vm_float_t *d,
                         const vm_float_t *a, int a_stride,
                         const vm_float_t *b, int b_stride, int n);
#endif

#if defined(VECMAT_ENABLE_NEON)
bool vm_batch3_mul_neon(vm_float_t *C, int c_stride,
                        const vm_float_t *A, int a_stride,
                        const vm_float_t *B, int b_stride, int n);
bool vm_batch3_inverse_neon(vm_float_t *C, int c_stride,
                            const vm_float_t *A, int a_stride, int n);
bool vm_batch3_ddot_neon(vm_float_t *d,
                         const vm_float_t *a, int a_stride,
                         const vm_float_t *b, int b_stride, int n);
#endif

static void load9(matrix3 *m, const vm_float_t *p)
{
    memcpy(m->v, p, 9 * sizeof(vm_float_t));
}

static void store9(vm_float_t *p, const matrix3 *m)
{
    memcpy(p, m->v, 9 * sizeof(vm_float_t));
}

static void load6(vm_sym3 *s, const vm_float_t *p)
{
    memcpy(s->c, p, 6 * sizeof(vm_float_t));
}

bool vm_batch3_mul(vm_float_t *C, const int c_stride,
                   const vm_float_t *A, const int a_stride,
                   const vm_float_t *B, const int b_stride,
                   const int n)
{
    if (!C || !A || !B || n <= 0) {
        return false;
    }

#if defined(VECMAT_ENABLE_AVX2)
    if (vm_cpu_has(VM_LEVEL_AVX2)) {
        return vm_batch3_mul_avx2(C, c_stride, A, a_stride, B, b_stride, n);
    }
#endif

#if defined(VECMAT_ENABLE_NEON)
    if (vm_cpu_has(VM_LEVEL_NEON)) {
        return vm_batch3_mul_neon(C, c_stride, A, a_stride, B, b_stride, n);
    }
#endif

    for (int i = 0; i < n; ++i) {
        matrix3 a, b, c;
        load9(&a, A + i * a_stride);
        load9(&b, B + i * b_stride);
        c = mat3_mul(a, b);
        store9(C + i * c_stride, &c);
    }

    return true;
}

bool vm_batch3_inverse(vm_float_t *C, const int c_stride,
                       const vm_float_t *A, const int a_stride,
                       const int n)
{
    if (!C || !A || n <= 0) {
        return false;
    }

#if defined(VECMAT_ENABLE_AVX2)
    if (vm_cpu_has(VM_LEVEL_AVX2)) {
        return vm_batch3_inverse_avx2(C, c_stride, A, a_stride, n);
    }
#endif

#if defined(VECMAT_ENABLE_NEON)
    if (vm_cpu_has(VM_LEVEL_NEON)) {
        return vm_batch3_inverse_neon(C, c_stride, A, a_stride, n);
    }
#endif

    for (int i = 0; i < n; ++i) {
        matrix3 a, c;
        load9(&a, A + i * a_stride);

        if (!mat3_inverse_ok(&c, &a)) {
            return false;
        }

        store9(C + i * c_stride, &c);
    }

    return true;
}

bool vm_batch3_polar(vm_float_t *R, const int r_stride,
                     vm_float_t *U, const int u_stride,
                     const vm_float_t *F, const int f_stride,
                     const int n)
{
    if (!R || !U || !F || n <= 0) {
        return false;
    }

    for (int i = 0; i < n; ++i) {
        matrix3 f, r, u;
        load9(&f, F + i * f_stride);

        if (!mat3_polar(&r, &u, &f)) {
            return false;
        }

        store9(R + i * r_stride, &r);
        store9(U + i * u_stride, &u);
    }

    return true;
}

bool vm_batch3_ddot(vm_float_t *d,
                    const vm_float_t *a, const int a_stride,
                    const vm_float_t *b, const int b_stride,
                    const int n)
{
    if (!d || !a || !b || n <= 0) {
        return false;
    }

#if defined(VECMAT_ENABLE_AVX2)
    if (vm_cpu_has(VM_LEVEL_AVX2)) {
        return vm_batch3_ddot_avx2(d, a, a_stride, b, b_stride, n);
    }
#endif

#if defined(VECMAT_ENABLE_NEON)
    if (vm_cpu_has(VM_LEVEL_NEON)) {
        return vm_batch3_ddot_neon(d, a, a_stride, b, b_stride, n);
    }
#endif

    for (int i = 0; i < n; ++i) {
        vm_sym3 sa, sb;
        load6(&sa, a + i * a_stride);
        load6(&sb, b + i * b_stride);
        d[i] = vm_sym3_ddot(&sa, &sb);
    }

    return true;
}
