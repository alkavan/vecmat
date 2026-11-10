// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

#if !defined(_MSC_VER)
#pragma STDC FP_CONTRACT OFF
#endif

/* GCC ignores the STDC pragma; keep the scalar reference unfused so it
 * matches the SIMD paths bit for bit (AArch64 has scalar fma). */
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC optimize("fp-contract=off")
#endif

/* The AVX2 kernels check plain VM_CPU_AVX2, not VM_LEVEL_AVX2: their TU is
 * built with -mfma but disables contraction and uses no fma intrinsics, so
 * it emits no FMA instructions. Use VM_LEVEL_AVX2 if that ever changes. */
#if defined(VECMAT_ENABLE_AVX2)
void vm_soa3_dot_avx2(vm_float_t *res, vm_csoa3 a, vm_csoa3 b, int n);
void vm_soa3_cross_avx2(vm_soa3 res, vm_csoa3 a, vm_csoa3 b, int n);
void vm_soa3_length_squared_avx2(vm_float_t *res, vm_csoa3 a, int n);
void vm_soa3_distance_squared_avx2(vm_float_t *res, vm_csoa3 a, vm_csoa3 b, int n);
#endif

#if defined(VECMAT_ENABLE_NEON)
void vm_soa3_dot_neon(vm_float_t *res, vm_csoa3 a, vm_csoa3 b, int n);
void vm_soa3_cross_neon(vm_soa3 res, vm_csoa3 a, vm_csoa3 b, int n);
void vm_soa3_length_squared_neon(vm_float_t *res, vm_csoa3 a, int n);
void vm_soa3_distance_squared_neon(vm_float_t *res, vm_csoa3 a, vm_csoa3 b, int n);
#endif

/**
 * @brief True when all three component pointers are non-null.
 */
static bool csoa3_ok(const vm_csoa3 v)
{
    return v.x && v.y && v.z;
}

void vm_soa3_dot(vm_float_t *res, const vm_csoa3 a, const vm_csoa3 b, const int n)
{
    if (n <= 0 || !res || !csoa3_ok(a) || !csoa3_ok(b)) {
        return;
    }
#if defined(VECMAT_ENABLE_AVX2)
    if (vm_cpu_has(VM_CPU_AVX2)) {
        vm_soa3_dot_avx2(res, a, b, n);
        return;
    }
#endif
#if defined(VECMAT_ENABLE_NEON)
    if (vm_cpu_has(VM_LEVEL_NEON)) {
        vm_soa3_dot_neon(res, a, b, n);
        return;
    }
#endif
    for (int i = 0; i < n; ++i) {
        const vm_float_t xx = a.x[i] * b.x[i];
        const vm_float_t yy = a.y[i] * b.y[i];
        const vm_float_t zz = a.z[i] * b.z[i];
        res[i] = (xx + yy) + zz;
    }
}

void vm_soa3_cross(const vm_soa3 res, const vm_csoa3 a, const vm_csoa3 b, const int n)
{
    if (n <= 0 || !res.x || !res.y || !res.z || !csoa3_ok(a) || !csoa3_ok(b)) {
        return;
    }
#if defined(VECMAT_ENABLE_AVX2)
    if (vm_cpu_has(VM_CPU_AVX2)) {
        vm_soa3_cross_avx2(res, a, b, n);
        return;
    }
#endif
#if defined(VECMAT_ENABLE_NEON)
    if (vm_cpu_has(VM_LEVEL_NEON)) {
        vm_soa3_cross_neon(res, a, b, n);
        return;
    }
#endif
    for (int i = 0; i < n; ++i) {
        /* load everything first: res may exactly equal a or b */
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

void vm_soa3_length_squared(vm_float_t *res, const vm_csoa3 a, const int n)
{
    if (n <= 0 || !res || !csoa3_ok(a)) {
        return;
    }
#if defined(VECMAT_ENABLE_AVX2)
    if (vm_cpu_has(VM_CPU_AVX2)) {
        vm_soa3_length_squared_avx2(res, a, n);
        return;
    }
#endif
#if defined(VECMAT_ENABLE_NEON)
    if (vm_cpu_has(VM_LEVEL_NEON)) {
        vm_soa3_length_squared_neon(res, a, n);
        return;
    }
#endif
    for (int i = 0; i < n; ++i) {
        const vm_float_t xx = a.x[i] * a.x[i];
        const vm_float_t yy = a.y[i] * a.y[i];
        const vm_float_t zz = a.z[i] * a.z[i];
        res[i] = (xx + yy) + zz;
    }
}

void vm_soa3_distance_squared(vm_float_t *res, const vm_csoa3 a, const vm_csoa3 b, const int n)
{
    if (n <= 0 || !res || !csoa3_ok(a) || !csoa3_ok(b)) {
        return;
    }
#if defined(VECMAT_ENABLE_AVX2)
    if (vm_cpu_has(VM_CPU_AVX2)) {
        vm_soa3_distance_squared_avx2(res, a, b, n);
        return;
    }
#endif
#if defined(VECMAT_ENABLE_NEON)
    if (vm_cpu_has(VM_LEVEL_NEON)) {
        vm_soa3_distance_squared_neon(res, a, b, n);
        return;
    }
#endif
    for (int i = 0; i < n; ++i) {
        const vm_float_t dx = a.x[i] - b.x[i];
        const vm_float_t dy = a.y[i] - b.y[i];
        const vm_float_t dz = a.z[i] - b.z[i];
        const vm_float_t xx = dx * dx;
        const vm_float_t yy = dy * dy;
        const vm_float_t zz = dz * dz;
        res[i] = (xx + yy) + zz;
    }
}
