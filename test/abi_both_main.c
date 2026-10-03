// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <stdio.h>

int abi_tu32_add(void);
int abi_tu64_add(void);

/* f32 and f64 TUs linked to the same BOTH library. */
int main(void)
{
    const int a = abi_tu32_add();
    const int b = abi_tu64_add();
    if (a != 3 || b != 3) {
        fprintf(stderr, "abi both: f32=%d f64=%d\n", a, b);
        return 1;
    }
    return 0;
}
