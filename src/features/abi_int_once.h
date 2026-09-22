// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_ABI_INT_ONCE_H
#define VECMAT_ABI_INT_ONCE_H

/*
 * Integer-only bodies are emitted once.
 * Prefer the f32 object lib when it is built; slim-64 emits them here.
 * Integer APIs that take or return vm_float_t are compiled for every
 * width present in the library.
 */
#if !defined(VECMAT_ABI_BITS) || (VECMAT_ABI_BITS == 32) || !defined(VECMAT_HAVE_ABI_32)
#define VECMAT_INT_ONCE 1
#else
#define VECMAT_INT_ONCE 0
#endif

#endif //VECMAT_ABI_INT_ONCE_H
