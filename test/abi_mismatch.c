// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <stdio.h>
#include <vecmat.h>

/*
 * Compiled as an f32 TU and linked with cpu.c built HAVE_ABI_64 only.
 * That is the slim-64 tripwire: symbols of the other width are absent,
 * and vm_abi_mismatch() is non-zero.
 */
int main(void)
{
    const unsigned bits = vm_compiled_float_bits();
    const int mismatch = vm_abi_mismatch();
    if (bits != 64u || mismatch == 0 || VECMAT_FLOAT_BITS != 32) {
        fprintf(stderr, "abi mismatch: bits=%u mismatch=%d tu=%d\n",
                bits, mismatch, VECMAT_FLOAT_BITS);
        return 1;
    }
    return 0;
}
