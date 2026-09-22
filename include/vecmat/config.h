// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_CONFIG_H
#define VECMAT_CONFIG_H

/**
 * @file
 * @brief Build-time precision, sizes, angle helpers, and the shared-library export macro.
 * @ingroup vecmat_core
 */

#include <math.h>
#if defined(_MSC_VER)
#include <float.h>
#endif
#include <stdbool.h>
#include <stdint.h>

// Export macro for a shared library on Windows
#ifdef _WIN32
    #ifdef VEC_BUILD_SHARED
        #define VEC_API __declspec(dllexport)
    #else
        #define VEC_API
    #endif
#else
    #define VEC_API
#endif

/*******************************************************************************
 * Mathematical constants
 ******************************************************************************/

#ifndef M_PI
#define M_E        2.71828182845904523536   // e
#define M_LOG2E    1.44269504088896340736   // log2(e)
#define M_LOG10E   0.434294481903251827651  // log10(e)
#define M_LN2      0.693147180559945309417  // ln(2)
#define M_LN10     2.30258509299404568402   // ln(10)
#define M_PI       3.14159265358979323846   // pi
#define M_PI_2     1.57079632679489661923   // pi/2
#define M_PI_4     0.785398163397448309616  // pi/4
#define M_1_PI     0.318309886183790671538  // 1/pi
#define M_2_PI     0.636619772367581343076  // 2/pi
#define M_2_SQRTPI 1.12837916709551257390   // 2/sqrt(pi)
#define M_SQRT2    1.41421356237309504880   // sqrt(2)
#define M_SQRT1_2  0.707106781186547524401  // 1/sqrt(2)
#endif

// Radians/Degrees conversions
static const float  VM_DEG_TO_RAD_F32 = 0.017453292519943295769236907684886f;
static const float  VM_RAD_TO_DEG_F32 = 57.295779513082320876798154814105f;
static const double VM_DEG_TO_RAD_F64 = 0.017453292519943295769236907684886;
static const double VM_RAD_TO_DEG_F64 = 57.295779513082320876798154814105;

// Epsilon conversions
#define VECMAT_EPS_F32 1e-6f
#define VECMAT_EPS_F64 1e-12

// Max/Min conversions
#if defined(_MSC_VER)
#define VECMAT_FLT_MAX FLT_MAX
#define VECMAT_DBL_MAX DBL_MAX
#define VECMAT_FLT_MIN FLT_MIN
#define VECMAT_DBL_MIN DBL_MIN
#else
#define VECMAT_FLT_MAX __FLT_MAX__
#define VECMAT_DBL_MAX __DBL_MAX__
#define VECMAT_FLT_MIN __FLT_MIN__
#define VECMAT_DBL_MIN __DBL_MIN__
#endif

// Vector/Matrix common size Conversions
#define VECMAT_VEC2_SIZE 2
#define VECMAT_VEC3_SIZE 3
#define VECMAT_VEC4_SIZE 4

#define VECMAT_MAT2_SIZE 4
#define VECMAT_MAT3_SIZE 9
#define VECMAT_MAT4_SIZE 16

#define VECMAT_QUAT_SIZE 4

/*******************************************************************************
 * Floating-point type
 ******************************************************************************/

#ifdef VECMAT_USE_F64
typedef double vm_float_t;
#else
typedef float vm_float_t;
#endif

#ifdef VECMAT_USE_F64
/* MSVC C does not treat a `static const double` as a constant expression. */
#if defined(_MSC_VER)
static const vm_float_t VM_DEG_TO_RAD = 0.017453292519943295769236907684886;
static const vm_float_t VM_RAD_TO_DEG = 57.295779513082320876798154814105;
#else
static const vm_float_t VM_DEG_TO_RAD = VM_DEG_TO_RAD_F64;
static const vm_float_t VM_RAD_TO_DEG = VM_RAD_TO_DEG_F64;
#endif
#else
#if defined(_MSC_VER)
static const vm_float_t VM_DEG_TO_RAD = 0.017453292519943295769236907684886f;
static const vm_float_t VM_RAD_TO_DEG = 57.295779513082320876798154814105f;
#else
static const vm_float_t VM_DEG_TO_RAD = VM_DEG_TO_RAD_F32;
static const vm_float_t VM_RAD_TO_DEG = VM_RAD_TO_DEG_F32;
#endif
#endif

#ifdef VECMAT_USE_F64
#define VM_F(x) x
#else
#define VM_F(x) x##f
#endif

/**
 * Angle literals in the library's native unit (radians).
 *
 * `VM_DEG(90)` writes a human degree constant and yields radians.
 * `VM_RAD(M_PI_2)` documents that the value is already radians and
 * casts it to `vm_float_t`. Unsuffixed angle APIs take and return radians;
 * use the `_deg` suffix at the human/config boundary.
 */
#define VM_DEG(d) ((vm_float_t)(d) * VM_DEG_TO_RAD)
#define VM_RAD(r) ((vm_float_t)(r))

/*******************************************************************************
 * Integer type
 ******************************************************************************/

#if defined(VECMAT_USE_INT8)
typedef int8_t vm_int_t;
#define VEC_INT_MAX INT8_MAX
#define VEC_INT_MIN INT8_MIN
#elif defined(VECMAT_USE_INT16)
typedef int16_t vm_int_t;
#define VEC_INT_MAX INT16_MAX
#define VEC_INT_MIN INT16_MIN
#elif defined(VECMAT_USE_INT32)
typedef int32_t vm_int_t;
#define VEC_INT_MAX INT32_MAX
#define VEC_INT_MIN INT32_MIN
#else
typedef int32_t vm_int_t;
#define VEC_INT_MAX INT32_MAX
#define VEC_INT_MIN INT32_MIN
#endif

#endif //VECMAT_CONFIG_H
