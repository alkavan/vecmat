// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_H
#define VECMAT_H

/**
 * @file vecmat.h
 * @brief An umbrella header.
 *
 * Layered modules under `vecmat/` may also be included directly!
 */

/**
 * @defgroup vecmat_core Core
 * @brief Value types and helpers: vec/mat/quat, clip-space, easing.
 */

/**
 * @defgroup vecmat_fastpath Fast path
 * @brief `_ptr` kernels, `vm_cpu_*`, runtime dispatch, backends.
 */

/**
 * @defgroup vecmat_extras Numerics extras
 * @brief GEMM, heap `vm_mat`, factorizations, CSR/KSP, integrators.
 */

// Version
#define VECMAT_VERSION_MAJOR 0
#define VECMAT_VERSION_MINOR 3
#define VECMAT_VERSION_PATCH 2

// Version macros
#define VECMAT_STR_HELPER(x) #x
#define VECMAT_STR(x) VECMAT_STR_HELPER(x)
#define VECMAT_VERSION VECMAT_STR(VECMAT_VERSION_MAJOR) "." \
    VECMAT_STR(VECMAT_VERSION_MINOR) "." VECMAT_STR(VECMAT_VERSION_PATCH)

#include "vecmat/config.h"
#include "vecmat/abi.h"
#include "vecmat/types.h"
#include "vecmat/cpu.h"
#include "vecmat/backend.h"
#include "vecmat/vec.h"
#include "vecmat/mat.h"
#include "vecmat/quat.h"
#include "vecmat/easing.h"
#include "vecmat/clip.h"
#include "vecmat/gemm.h"
#include "vecmat/dense.h"
#include "vecmat/sparse.h"
#include "vecmat/ode.h"

#endif //VECMAT_H
