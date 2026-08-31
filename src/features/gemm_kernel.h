// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_GEMM_KERNEL_H
#define VECMAT_GEMM_KERNEL_H

/* Pack / block sizes shared by the driver and ISA micro-kernels. */
enum {
    VM_GEMM_MC = 128,
    VM_GEMM_NC = 128,
    VM_GEMM_KC = 128,
    VM_GEMM_MR = 8,
    VM_GEMM_NR = 8
};

#endif // VECMAT_GEMM_KERNEL_H
