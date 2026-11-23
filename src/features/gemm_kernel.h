// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_GEMM_KERNEL_H
#define VECMAT_GEMM_KERNEL_H

/* Pack / block sizes shared by the driver and ISA micro-kernels.
 * MR/NR are the default 8×8 tile. A selected tile may be larger, up to the max. */
enum {
    VM_GEMM_MC = 128,
    VM_GEMM_NC = 128,
    VM_GEMM_KC = 128,
    VM_GEMM_MR = 8,
    VM_GEMM_NR = 8,
    VM_GEMM_MR_MAX = 16,
    VM_GEMM_NR_MAX = 16,
    /* One KC slice of packed A, reused across NC panels, when it fits.
     * A fixed byte ceiling, not a per-CPU cache probe. */
    VM_GEMM_PACK_CACHE = 4 * 1024 * 1024
};

#endif /* VECMAT_GEMM_KERNEL_H */
