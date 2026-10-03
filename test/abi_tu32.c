// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

int abi_tu32_add(void)
{
    const vector4 a = vec4_splat(1.0f);
    const vector4 b = vec4_splat(2.0f);
    const vector4 c = vec4_add(a, b);
    if (vm_abi_mismatch() != 0)
        return -1;
    if ((vm_compiled_float_bits() & 32u) == 0u)
        return -2;
    return (int)c.x;
}
