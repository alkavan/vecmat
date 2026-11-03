// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <string.h>

#include "vecmat/bf16.h"

vm_bf16_t vm_bf16_from_f32(const float x)
{
    uint32_t bits;
    memcpy(&bits, &x, sizeof bits);

    const uint32_t sign = (bits >> 16) & 0x8000u;
    const uint32_t exp = (bits >> 23) & 0xffu;
    const uint32_t mant = bits & 0x7fffffu;

    if (exp == 255u) {
        if (mant == 0u) {
            return (vm_bf16_t)(sign | 0x7f80u);
        }
        return (vm_bf16_t)0x7fc0u;
    }

    const uint32_t half = mant >> 16;
    const uint32_t lost = mant & 0xffffu;
    uint32_t rounded = half;
    if (lost > 0x8000u || (lost == 0x8000u && (half & 1u))) {
        rounded++;
    }

    uint32_t exp16 = exp;
    if (rounded == 0x80u) {
        rounded = 0;
        exp16++;
        if (exp16 >= 255u)
            return (vm_bf16_t)(sign | 0x7f80u);
    }

    return (vm_bf16_t)(sign | (exp16 << 7) | rounded);
}

float vm_f32_from_bf16(const vm_bf16_t h)
{
    const uint32_t bits = (uint32_t)h << 16;
    float out;
    memcpy(&out, &bits, sizeof out);
    return out;
}
