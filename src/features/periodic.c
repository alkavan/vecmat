// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <math.h>
#include <vecmat.h>

#if !defined(_MSC_VER)
#pragma STDC FP_CONTRACT OFF
#endif

/**
 * @brief True when every axis length is finite and positive.
 */
static bool box_positive(const vector3 box)
{
    return isfinite(box.x) && box.x > VM_F(0.0)
        && isfinite(box.y) && box.y > VM_F(0.0)
        && isfinite(box.z) && box.z > VM_F(0.0);
}

/**
 * @brief One axis of `a - b`, in `[-L/2, L/2]`.
 */
static vm_float_t delta_axis(const vm_float_t a, const vm_float_t b, const vm_float_t length)
{
    return VECMAT_REMAINDER(a - b, length);
}

/**
 * @brief Fold one coordinate into `[0, L)`, with `±0` and `L` stored as `+0`.
 */
static vm_float_t wrap_axis(const vm_float_t v, const vm_float_t length)
{
    const vm_float_t r0 = VECMAT_REMAINDER(v, length);
    if (isnan(r0)) {
        return r0;
    }

    vm_float_t r = r0;
    if (r < VM_F(0.0)) {
        r += length;
    }

    if (!(r > VM_F(0.0)) || !(r < length)) {
        return VM_F(0.0);
    }

    return r;
}

bool vm_periodic_delta_ortho(vector3 *res, const vector3 *a, const vector3 *b,
                             const vector3 box, const int n)
{
    if (n < 0 || !box_positive(box)) {
        return false;
    }

    if (n == 0) {
        return true;
    }

    if (!res || !a || !b) {
        return false;
    }

    for (int i = 0; i < n; ++i) {
        const vector3 ai = a[i];
        const vector3 bi = b[i];
        vector3 d;
        d.x = delta_axis(ai.x, bi.x, box.x);
        d.y = delta_axis(ai.y, bi.y, box.y);
        d.z = delta_axis(ai.z, bi.z, box.z);
        res[i] = d;
    }

    return true;
}

bool vm_wrap_ortho(vector3 *x, const vector3 box, const int n)
{
    if (n < 0 || !box_positive(box)) {
        return false;
    }

    if (n == 0) {
        return true;
    }

    if (!x) {
        return false;
    }

    for (int i = 0; i < n; ++i) {
        const vector3 p = x[i];
        vector3 w;
        w.x = wrap_axis(p.x, box.x);
        w.y = wrap_axis(p.y, box.y);
        w.z = wrap_axis(p.z, box.z);
        x[i] = w;
    }

    return true;
}
