// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <string.h>

#include "vecmat/fp16.h"

vm_fp16_t vm_fp16_from_f32(const float x)
{
    uint32_t bits;
    memcpy(&bits, &x, sizeof bits);

    const uint32_t sign = (bits >> 16) & 0x8000u;
    const uint32_t exp = (bits >> 23) & 0xffu;
    const uint32_t mant = bits & 0x7fffffu;

    if (exp == 255u) {
        if (mant == 0u)
            return (vm_fp16_t)(sign | 0x7c00u);
        return (vm_fp16_t)0x7e00u;
    }

    const int32_t unbiased = (int32_t)exp - 127;
    if (unbiased > 15)
        return (vm_fp16_t)(sign | 0x7c00u);

    if (unbiased < -14) {
        if (unbiased < -24)
            return (vm_fp16_t)sign;
        const uint32_t full = mant | 0x800000u;
        const int shift = -unbiased - 1;
        const uint32_t half = full >> shift;
        const uint32_t lost = full & ((1u << shift) - 1u);
        const uint32_t mid = 1u << (shift - 1);
        uint32_t rounded = half;
        if (lost > mid || (lost == mid && (half & 1u)))
            rounded++;
        return (vm_fp16_t)(sign | rounded);
    }

    const uint32_t half = mant >> 13;
    const uint32_t lost = mant & 0x1fffu;
    uint32_t rounded = half;
    if (lost > 0x1000u || (lost == 0x1000u && (half & 1u)))
        rounded++;
    return (vm_fp16_t)(sign | (((uint32_t)(unbiased + 15) << 10) + rounded));
}

float vm_f32_from_fp16(const vm_fp16_t h)
{
    const uint32_t sign = (uint32_t)(h & 0x8000u) << 16;
    uint32_t exp = (h >> 10) & 0x1fu;
    uint32_t mant = h & 0x3ffu;
    uint32_t bits;

    if (exp == 0u) {
        if (mant == 0u) {
            bits = sign;
        } else {
            exp = 127u - 14u;
            while ((mant & 0x400u) == 0u) {
                mant <<= 1;
                exp--;
            }
            mant &= 0x3ffu;
            bits = sign | (exp << 23) | (mant << 13);
        }
    } else if (exp == 31u) {
        bits = sign | 0x7f800000u | (mant ? 0x400000u : 0u);
    } else {
        bits = sign | ((exp + (127u - 15u)) << 23) | (mant << 13);
    }

    float out;
    memcpy(&out, &bits, sizeof out);
    return out;
}
