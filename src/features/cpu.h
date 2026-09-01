// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_CPU_H
#define VECMAT_CPU_H

#include <vecmat.h>
#include "dispatch_unmangle.h"
#include "dispatch_list.h"

/*
 * Scalar kernel linkage. When dispatch is off the _scalar body is static and
 * inlined into the public function in the same TU. When dispatch is on it has
 * external linkage so dispatch.c can take its address.
 */
#ifdef VECMAT_RUNTIME_DISPATCH
#define VECMAT_SCALAR_API
#else
#define VECMAT_SCALAR_API static
#endif

#ifdef VECMAT_RUNTIME_DISPATCH

#define VECMAT_FN_TYPE(name, params, args) typedef void (*name##_fn) params;
VECMAT_DISPATCH_LIST(VECMAT_FN_TYPE)
#undef VECMAT_FN_TYPE

#define VECMAT_FN_PTR_DECL(name, params, args) extern name##_fn name##_;
VECMAT_DISPATCH_LIST(VECMAT_FN_PTR_DECL)
#undef VECMAT_FN_PTR_DECL

#define VECMAT_DECL_SCALAR(name, params, args) void name##_scalar params;
#define VECMAT_DECL_AVX(name, params, args)    void name##_avx params;
#define VECMAT_DECL_AVX2(name, params, args)   void name##_avx2 params;
#define VECMAT_DECL_AVX512(name, params, args) void name##_avx512 params;
#define VECMAT_DECL_SVE(name, params, args)    void name##_sve params;
#define VECMAT_DECL_SVE2(name, params, args)   void name##_sve2 params;

VECMAT_DISPATCH_LIST(VECMAT_DECL_SCALAR)

#if defined(VECMAT_ENABLE_AVX)
VECMAT_DISPATCH_LIST(VECMAT_DECL_AVX)
#endif

#if defined(VECMAT_ENABLE_AVX2)
VECMAT_DISPATCH_LIST(VECMAT_DECL_AVX2)
#endif

#if defined(VECMAT_ENABLE_AVX512)
VECMAT_DISPATCH_LIST(VECMAT_DECL_AVX512)
#endif

#if defined(VECMAT_ENABLE_SVE)
VECMAT_DISPATCH_LIST(VECMAT_DECL_SVE)
#endif

#if defined(VECMAT_ENABLE_SVE2)
VECMAT_DISPATCH_LIST(VECMAT_DECL_SVE2)
#endif

#undef VECMAT_DECL_SCALAR
#undef VECMAT_DECL_AVX
#undef VECMAT_DECL_AVX2
#undef VECMAT_DECL_AVX512
#undef VECMAT_DECL_SVE
#undef VECMAT_DECL_SVE2

#endif /* VECMAT_RUNTIME_DISPATCH */

#endif // VECMAT_CPU_H
