// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_H
#define VECMAT_H

#include <math.h>
#include <stdbool.h>
#include <stdint.h>

// Version macros
#define VECMAT_VERSION_MAJOR 0
#define VECMAT_VERSION_MINOR 2
#define VECMAT_VERSION_PATCH 2

#define VECMAT_STR_HELPER(x) #x
#define VECMAT_STR(x) VECMAT_STR_HELPER(x)
#define VECMAT_VERSION VECMAT_STR(VECMAT_VERSION_MAJOR) "." \
    VECMAT_STR(VECMAT_VERSION_MINOR) "." VECMAT_STR(VECMAT_VERSION_PATCH)

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

static const float  VM_DEG_TO_RAD_F32 = 0.017453292519943295769236907684886f;
static const float  VM_RAD_TO_DEG_F32 = 57.295779513082320876798154814105f;
static const double VM_DEG_TO_RAD_F64 = 0.017453292519943295769236907684886;
static const double VM_RAD_TO_DEG_F64 = 57.295779513082320876798154814105;

#define VECMAT_FLT_MAX __FLT_MAX__
#define VECMAT_DBL_MAX __DBL_MAX__

#define VECMAT_FLT_MIN __FLT_MIN__
#define VECMAT_DBL_MIN __DBL_MIN__

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

#if defined(VECMAT_USE_F64)
typedef double vm_float_t;
#else
typedef float vm_float_t;
#endif

#ifdef VECMAT_USE_F64
static const vm_float_t VM_DEG_TO_RAD = VM_DEG_TO_RAD_F64;
static const vm_float_t VM_RAD_TO_DEG = VM_RAD_TO_DEG_F64;
#else
static const vm_float_t VM_DEG_TO_RAD = VM_DEG_TO_RAD_F32;
static const vm_float_t VM_RAD_TO_DEG = VM_RAD_TO_DEG_F32;
#endif

#ifdef VECMAT_USE_F64
#define VM_F(x) x
#else
#define VM_F(x) x##f
#endif

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

/*******************************************************************************
 * Floating-point vector types
 ******************************************************************************/

typedef struct {
    union {
        struct {
            vm_float_t x;
            vm_float_t y;
        };
        vm_float_t v[VECMAT_VEC2_SIZE];
    };
} vector2;

typedef struct {
    union {
        struct {
            vm_float_t x;
            vm_float_t y;
            vm_float_t z;
        };
        vm_float_t v[VECMAT_VEC3_SIZE];
    };
} vector3;

typedef struct {
    union {
        struct {
            vm_float_t x;
            vm_float_t y;
            vm_float_t z;
            vm_float_t w;
        };
        vm_float_t v[VECMAT_VEC4_SIZE];
    };
} vector4;

/*******************************************************************************
 * Integer vector types
 ******************************************************************************/

typedef struct {
    union {
        struct {
            vm_int_t x;
            vm_int_t y;
        };
        vm_int_t v[VECMAT_VEC2_SIZE];
    };
} vector2i;

typedef struct {
    union {
        struct {
            vm_int_t x;
            vm_int_t y;
            vm_int_t z;
        };
        vm_int_t v[VECMAT_VEC3_SIZE];
    };
} vector3i;

typedef struct {
    union {
        struct {
            vm_int_t x;
            vm_int_t y;
            vm_int_t z;
            vm_int_t w;
        };
        vm_int_t v[VECMAT_VEC4_SIZE];
    };
} vector4i;

/*******************************************************************************
 * Floating-point matrix types
 ******************************************************************************/

/*
 * 2x2 MATRIX
 * ---------------
 * [0/m11] [2/m12]
 * [1/m21] [3/m22]
 * ---------------
 */
typedef struct {
    union {
        struct {
            vm_float_t m11;
            vm_float_t m21;
            vm_float_t m12;
            vm_float_t m22;
        };
        vm_float_t v[VECMAT_MAT2_SIZE];
    };
} matrix2;

/*
 * 3X3 MATRIX
 * -----------------------
 * [0/m11] [3/m12] [6/m13]
 * [1/m21] [4/m22] [7/m23]
 * [2/m31] [5/m32] [8/m33]
 * -----------------------
 */
typedef struct {
    union {
        struct {
            vm_float_t m11;
            vm_float_t m21;
            vm_float_t m31;
            vm_float_t m12;
            vm_float_t m22;
            vm_float_t m32;
            vm_float_t m13;
            vm_float_t m23;
            vm_float_t m33;
        };
        vm_float_t v[VECMAT_MAT3_SIZE];
    };
} matrix3;

/*
 * 4x4 MATRIX
 * -----------------------------------
 * [ 0/m11] [ 4/m12] [ 8/m13] [12/m14]
 * [ 1/m21] [ 5/m22] [ 9/m23] [13/m24]
 * [ 2/m31] [ 6/m32] [10/m33] [14/m34]
 * [ 3/m41] [ 7/m42] [11/m43] [15/m44]
 * -----------------------------------
 */
typedef struct {
    union {
        struct {
            vm_float_t m11;
            vm_float_t m21;
            vm_float_t m31;
            vm_float_t m41;
            vm_float_t m12;
            vm_float_t m22;
            vm_float_t m32;
            vm_float_t m42;
            vm_float_t m13;
            vm_float_t m23;
            vm_float_t m33;
            vm_float_t m43;
            vm_float_t m14;
            vm_float_t m24;
            vm_float_t m34;
            vm_float_t m44;
        };
        vm_float_t v[VECMAT_MAT4_SIZE];
    };
} matrix4;

/*******************************************************************************
 * Integer matrix types
 ******************************************************************************/

/*
 * 2x2 MATRIX
 * ---------------
 * [0/m11] [2/m12]
 * [1/m21] [3/m22]
 * ---------------
 */
typedef struct {
    union {
        struct {
            vm_int_t m11;
            vm_int_t m21;
            vm_int_t m12;
            vm_int_t m22;
        };
        vm_int_t v[VECMAT_MAT2_SIZE];
    };
} matrix2i;

/*
 * 3X3 MATRIX
 * -----------------------
 * [0/m11] [3/m12] [6/m13]
 * [1/m21] [4/m22] [7/m23]
 * [2/m31] [5/m32] [8/m33]
 * -----------------------
 */
typedef struct {
    union {
        struct {
            vm_int_t m11;
            vm_int_t m21;
            vm_int_t m31;
            vm_int_t m12;
            vm_int_t m22;
            vm_int_t m32;
            vm_int_t m13;
            vm_int_t m23;
            vm_int_t m33;
        };
        vm_int_t v[VECMAT_MAT3_SIZE];
    };
} matrix3i;

/*
 * 4x4 MATRIX
 * -----------------------------------
 * [ 0/m11] [ 4/m12] [ 8/m13] [12/m14]
 * [ 1/m21] [ 5/m22] [ 9/m23] [13/m24]
 * [ 2/m31] [ 6/m32] [10/m33] [14/m34]
 * [ 3/m41] [ 7/m42] [11/m43] [15/m44]
 * -----------------------------------
 */
typedef struct {
    union {
        struct {
            vm_int_t m11;
            vm_int_t m21;
            vm_int_t m31;
            vm_int_t m41;
            vm_int_t m12;
            vm_int_t m22;
            vm_int_t m32;
            vm_int_t m42;
            vm_int_t m13;
            vm_int_t m23;
            vm_int_t m33;
            vm_int_t m43;
            vm_int_t m14;
            vm_int_t m24;
            vm_int_t m34;
            vm_int_t m44;
        };
        vm_int_t v[VECMAT_MAT4_SIZE];
    };
} matrix4i;

/**
 * @struct quaternion
 * @brief
 */
typedef struct {
    union {
        struct {
            vm_float_t x;
            vm_float_t y;
            vm_float_t z;
            vm_float_t w;
        };
        vm_float_t v[VECMAT_QUAT_SIZE];
    };
} quaternion;

// Epsilon
#define VECMAT_EPS_F32 1e-6f
#define VECMAT_EPS_F64 1e-12

#ifdef VECMAT_USE_F64
#define VECMAT_EPSILON VECMAT_EPS_F64
#else
#define VECMAT_EPSILON VECMAT_EPS_F32
#endif

/**
 * @brief Checks if two floats are approximately equal within tolerance.
 *
 * @param a First value
 * @param b Second value
 * @param eps Tolerance threshold
 * @return True if values are equal within the given tolerance.
 */
#define FLOAT_EQ(a, b, eps) (fabsf((a) - (b)) < (eps))

/**
 * @brief Checks if two doubles are approximately equal within tolerance.
 *
 * @param a First value
 * @param b Second value
 * @param eps Tolerance threshold
 * @return True if values are equal within the given tolerance.
 */
#define DOUBLE_EQ(a, b, eps) (fabs((a) - (b)) < (eps))

#ifdef VECMAT_USE_F64
#define VECMAT_EQ(a, b, eps) DOUBLE_EQ((a), (b), (eps))
#else
#define VECMAT_EQ(a, b, eps) FLOAT_EQ((a), (b), (eps))
#endif

/*******************************************************************************
 * Precision-selected <math.h> wrappers
 ******************************************************************************/

#ifdef VECMAT_USE_F64
#define VECMAT_FABS(x)          fabs((x))
#define VECMAT_SQRT(x)          sqrt((x))
#define VECMAT_SIN(x)           sin((x))
#define VECMAT_COS(x)           cos((x))
#define VECMAT_TAN(x)           tan((x))
#define VECMAT_ASIN(x)          asin((x))
#define VECMAT_ACOS(x)          acos((x))
#define VECMAT_ATAN(x)          atan((x))
#define VECMAT_ATAN2(y, x)      atan2((y), (x))
#define VECMAT_FMIN(a, b)       fmin((a), (b))
#define VECMAT_FMAX(a, b)       fmax((a), (b))
#define VECMAT_FLOOR(x)         floor((x))
#define VECMAT_CEIL(x)          ceil((x))
#define VECMAT_ROUND(x)         round((x))
#define VECMAT_POW(x, y)        pow((x), (y))
#define VECMAT_COPYSIGN(x, y)   copysign((x), (y))
#else
#define VECMAT_FABS(x)          fabsf((x))
#define VECMAT_SQRT(x)          sqrtf((x))
#define VECMAT_SIN(x)           sinf((x))
#define VECMAT_COS(x)           cosf((x))
#define VECMAT_TAN(x)           tanf((x))
#define VECMAT_ASIN(x)          asinf((x))
#define VECMAT_ACOS(x)          acosf((x))
#define VECMAT_ATAN(x)          atanf((x))
#define VECMAT_ATAN2(y, x)      atan2f((y), (x))
#define VECMAT_FMIN(a, b)       fminf((a), (b))
#define VECMAT_FMAX(a, b)       fmaxf((a), (b))
#define VECMAT_FLOOR(x)         floorf((x))
#define VECMAT_CEIL(x)          ceilf((x))
#define VECMAT_ROUND(x)         roundf((x))
#define VECMAT_POW(x, y)        powf((x), (y))
#define VECMAT_COPYSIGN(x, y)   copysignf((x), (y))
#endif

/*******************************************************************************
 * CPU feature detection and dispatch
 ******************************************************************************/

typedef uint32_t vm_cpu_features_t;

enum {
    VM_CPU_SCALAR = 1u << 0,
    VM_CPU_AVX2   = 1u << 1,
    VM_CPU_SVE    = 1u << 2,
    VM_CPU_AVX512 = 1u << 3,
    VM_CPU_SVE2   = 1u << 4,
    VM_CPU_AVX    = 1u << 5
};

VEC_API vm_cpu_features_t vm_cpu_compiled_features(void);
VEC_API vm_cpu_features_t vm_cpu_runtime_features(void);
VEC_API vm_cpu_features_t vm_cpu_selected_features(void);
VEC_API const char *vm_cpu_name(vm_cpu_features_t features);
VEC_API void vm_cpu_init(void);

/*******************************************************************************
 * Floating-point vector functions
 ******************************************************************************/

// vector2
VEC_API vector2 vec2_add(vector2 a, vector2 b);
VEC_API vector2 vec2_sub(vector2 a, vector2 b);
VEC_API vector2 vec2_mul_scalar(vector2 v, vm_float_t s);
VEC_API vector2 vec2_div_scalar(vector2 v, vm_float_t s);
VEC_API vector2 vec2_mul(vector2 a, vector2 b);
VEC_API vector2 vec2_neg(vector2 v);
VEC_API vector2 vec2_abs(vector2 v);
VEC_API vector2 vec2_zero(void);
VEC_API vector2 vec2_one(void);
VEC_API vector2 vec2_x_axis(vm_float_t x);
VEC_API vector2 vec2_y_axis(vm_float_t y);
VEC_API vector2 vec2_x_scale(vm_float_t x);
VEC_API vector2 vec2_y_scale(vm_float_t y);
VEC_API vector2 vec2_scale(vector2 v, vm_float_t s);
VEC_API vector2 vec2_cross(vector2 a, vector2 b);
VEC_API vector2 vec2_normalize(vector2 v);
VEC_API vector2 vec2_min(vector2 a, vector2 b);
VEC_API vector2 vec2_max(vector2 a, vector2 b);
VEC_API vector2 vec2_sign(vector2 v);
VEC_API vector2 vec2_floor(vector2 v);
VEC_API vector2 vec2_ceil(vector2 v);
VEC_API vector2 vec2_round(vector2 v);
VEC_API vector2 vec2_perpendicular(vector2 v);
VEC_API vector2 vec2_reflect(vector2 v, vector2 normal);
VEC_API vector2 vec2_project(vector2 a, vector2 b);
VEC_API vector2 vec2_tangent(vector2 v);
VEC_API vector2 vec2_rotate(vector2 v, vm_float_t angle);
VEC_API vector2 vec2_slide(vector2 v, vector2 normal);
VEC_API vector2 vec2_clamp(vector2 v, vector2 min, vector2 max);
VEC_API vector2 vec2_lerp(vector2 a, vector2 b, vm_float_t t);
VEC_API vector2 vec2_div(vector2 a, vector2 b);
VEC_API vector2 vec2_add_scalar(vector2 v, vm_float_t s);
VEC_API vector2 vec2_sub_scalar(vector2 v, vm_float_t s);
VEC_API vector2 vec2_clamp_scalar(vector2 v, vm_float_t min, vm_float_t max);
VEC_API vector2 vec2_saturate(vector2 v);
VEC_API vector2 vec2_fract(vector2 v);
VEC_API vector2 vec2_refract(vector2 incident, vector2 normal, vm_float_t eta);
VEC_API vector2 vec2_reject(vector2 a, vector2 b);
VEC_API vector2 vec2_splat(vm_float_t s);
VEC_API vector2 vec2_from_angle(vm_float_t angle);
VEC_API vector2 vec2_rotate_around(vector2 v, vector2 pivot, vm_float_t angle);
VEC_API vector2 vec2_move_toward(vector2 current, vector2 target, vm_float_t max_delta);
VEC_API vector2 vec2_limit_length(vector2 v, vm_float_t max_len);

VEC_API vector3 vec2_to_vec3(vector2 v, vm_float_t z);

VEC_API vm_float_t vec2_dot(vector2 a, vector2 b);
VEC_API vm_float_t vec2_length(vector2 v);
VEC_API vm_float_t vec2_length_squared(vector2 v);
VEC_API vm_float_t vec2_length_manhattan(vector2 v);
VEC_API vm_float_t vec2_length_chebyshev(vector2 v);
VEC_API vm_float_t vec2_aspect_ratio(vector2 v);
VEC_API vm_float_t vec2_distance(vector2 a, vector2 b);
VEC_API vm_float_t vec2_distance_squared(vector2 a, vector2 b);
VEC_API vm_float_t vec2_angle(vector2 a, vector2 b);
VEC_API vm_float_t vec2_cross_scalar(vector2 a, vector2 b);
VEC_API vm_float_t vec2_heading(vector2 v);
VEC_API vm_float_t vec2_min_component(vector2 v);
VEC_API vm_float_t vec2_max_component(vector2 v);
VEC_API vm_float_t vec2_sum(vector2 v);
VEC_API bool vec2_is_zero(vector2 v);
VEC_API bool vec2_is_normalized(vector2 v);
VEC_API bool vec2_near(vector2 a, vector2 b, vm_float_t eps);

// vector3
VEC_API vector3 vec3_add(vector3 a, vector3 b);
VEC_API vector3 vec3_sub(vector3 a, vector3 b);
VEC_API vector3 vec3_mul_scalar(vector3 v, vm_float_t s);
VEC_API vector3 vec3_div_scalar(vector3 v, vm_float_t s);
VEC_API vector3 vec3_mul(vector3 a, vector3 b);
VEC_API vector3 vec3_neg(vector3 v);
VEC_API vector3 vec3_abs(vector3 v);
VEC_API vector3 vec3_zero(void);
VEC_API vector3 vec3_one(void);
VEC_API vector3 vec3_x_axis(vm_float_t x);
VEC_API vector3 vec3_y_axis(vm_float_t y);
VEC_API vector3 vec3_z_axis(vm_float_t z);
VEC_API vector3 vec3_x_scale(vm_float_t x);
VEC_API vector3 vec3_y_scale(vm_float_t y);
VEC_API vector3 vec3_z_scale(vm_float_t z);
VEC_API vector3 vec3_cross(vector3 a, vector3 b);
VEC_API vector3 vec3_normalize(vector3 v);
VEC_API vector3 vec3_min(vector3 a, vector3 b);
VEC_API vector3 vec3_max(vector3 a, vector3 b);
VEC_API vector3 vec3_sign(vector3 v);
VEC_API vector3 vec3_floor(vector3 v);
VEC_API vector3 vec3_ceil(vector3 v);
VEC_API vector3 vec3_round(vector3 v);
VEC_API vector3 vec3_reflect(vector3 incident, vector3 normal);
VEC_API vector3 vec3_refract(vector3 incident, vector3 normal, vm_float_t eta);
VEC_API vector3 vec3_lerp(vector3 a, vector3 b, vm_float_t t);
VEC_API vector3 vec3_clamp(vector3 v, vector3 min, vector3 max);
VEC_API vector3 vec3_scale(vector3 v, vm_float_t s);
VEC_API vector3 vec3_div(vector3 a, vector3 b);
VEC_API vector3 vec3_add_scalar(vector3 v, vm_float_t s);
VEC_API vector3 vec3_sub_scalar(vector3 v, vm_float_t s);
VEC_API vector3 vec3_clamp_scalar(vector3 v, vm_float_t min, vm_float_t max);
VEC_API vector3 vec3_saturate(vector3 v);
VEC_API vector3 vec3_fract(vector3 v);
VEC_API vector3 vec3_project(vector3 a, vector3 b);
VEC_API vector3 vec3_slide(vector3 v, vector3 normal);
VEC_API vector3 vec3_reject(vector3 a, vector3 b);
VEC_API vector3 vec3_rotate_axis(vector3 v, vector3 axis, vm_float_t angle);
VEC_API vector3 vec3_splat(vm_float_t s);
VEC_API vector3 vec3_from_vec2(vector2 v, vm_float_t z);
VEC_API vector3 vec3_move_toward(vector3 current, vector3 target, vm_float_t max_delta);
VEC_API vector3 vec3_limit_length(vector3 v, vm_float_t max_len);

VEC_API vector2 vec3_xy(vector3 v);
VEC_API void vec3_orthonormal_basis(vector3 n, vector3 *t, vector3 *b);

VEC_API vm_float_t vec3_dot(vector3 a, vector3 b);
VEC_API vm_float_t vec3_length(vector3 v);
VEC_API vm_float_t vec3_length_squared(vector3 v);
VEC_API vm_float_t vec3_length_manhattan(vector3 v);
VEC_API vm_float_t vec3_length_chebyshev(vector3 v);
VEC_API vm_float_t vec3_distance(vector3 a, vector3 b);
VEC_API vm_float_t vec3_distance_squared(vector3 a, vector3 b);
VEC_API vm_float_t vec3_angle(vector3 a, vector3 b);
VEC_API vm_float_t vec3_signed_angle(vector3 a, vector3 b, vector3 axis);
VEC_API vm_float_t vec3_min_component(vector3 v);
VEC_API vm_float_t vec3_max_component(vector3 v);
VEC_API vm_float_t vec3_sum(vector3 v);
VEC_API bool vec3_is_zero(vector3 v);
VEC_API bool vec3_is_normalized(vector3 v);
VEC_API bool vec3_near(vector3 a, vector3 b, vm_float_t eps);

// vector4
VEC_API vector4 vec4_add(vector4 a, vector4 b);
VEC_API vector4 vec4_sub(vector4 a, vector4 b);
VEC_API vector4 vec4_mul_scalar(vector4 v, vm_float_t s);
VEC_API vector4 vec4_div_scalar(vector4 v, vm_float_t s);
VEC_API vector4 vec4_mul(vector4 a, vector4 b);
VEC_API vector4 vec4_neg(vector4 v);
VEC_API vector4 vec4_abs(vector4 v);
VEC_API vector4 vec4_zero(void);
VEC_API vector4 vec4_one(void);
VEC_API vector4 vec4_x_axis(vm_float_t x);
VEC_API vector4 vec4_y_axis(vm_float_t y);
VEC_API vector4 vec4_z_axis(vm_float_t z);
VEC_API vector4 vec4_w_axis(vm_float_t w);
VEC_API vector4 vec4_x_scale(vm_float_t x);
VEC_API vector4 vec4_y_scale(vm_float_t y);
VEC_API vector4 vec4_z_scale(vm_float_t z);
VEC_API vector4 vec4_w_scale(vm_float_t w);
VEC_API vector4 vec4_normalize(vector4 v);
VEC_API vector4 vec4_min(vector4 a, vector4 b);
VEC_API vector4 vec4_max(vector4 a, vector4 b);
VEC_API vector4 vec4_sign(vector4 v);
VEC_API vector4 vec4_floor(vector4 v);
VEC_API vector4 vec4_ceil(vector4 v);
VEC_API vector4 vec4_round(vector4 v);
VEC_API vector4 vec4_lerp(vector4 a, vector4 b, vm_float_t t);
VEC_API vector4 vec4_clamp(vector4 v, vector4 min, vector4 max);
VEC_API vector4 vec4_homogenize(vector4 v);
VEC_API vector4 vec4_div(vector4 a, vector4 b);
VEC_API vector4 vec4_add_scalar(vector4 v, vm_float_t s);
VEC_API vector4 vec4_sub_scalar(vector4 v, vm_float_t s);
VEC_API vector4 vec4_clamp_scalar(vector4 v, vm_float_t min, vm_float_t max);
VEC_API vector4 vec4_saturate(vector4 v);
VEC_API vector4 vec4_fract(vector4 v);
VEC_API vector4 vec4_project(vector4 a, vector4 b);
VEC_API vector4 vec4_reject(vector4 a, vector4 b);
VEC_API vector4 vec4_slide(vector4 v, vector4 normal);
VEC_API vector4 vec4_splat(vm_float_t s);

VEC_API vector3 vec4_to_vec3(vector4 v);

VEC_API vm_float_t vec4_dot(vector4 a, vector4 b);
VEC_API vm_float_t vec4_length(vector4 v);
VEC_API vm_float_t vec4_length_squared(vector4 v);
VEC_API vm_float_t vec4_distance(vector4 a, vector4 b);
VEC_API vm_float_t vec4_distance_squared(vector4 a, vector4 b);
VEC_API bool vec4_is_zero(vector4 v);
VEC_API bool vec4_is_normalized(vector4 v);
VEC_API bool vec4_near(vector4 a, vector4 b, vm_float_t eps);

/*******************************************************************************
 * Floating-point vector pointer-based performance functions
 ******************************************************************************/

// vector2 pointer-based operations
VEC_API void vec2_add_ptr(vector2 *res, const vector2 *a, const vector2 *b);
VEC_API void vec2_sub_ptr(vector2 *res, const vector2 *a, const vector2 *b);
VEC_API void vec2_mul_scalar_ptr(vector2 *res, const vector2 *v, vm_float_t s);
VEC_API void vec2_div_scalar_ptr(vector2 *res, const vector2 *v, vm_float_t s);
VEC_API void vec2_mul_ptr(vector2 *res, const vector2 *a, const vector2 *b);
VEC_API void vec2_neg_ptr(vector2 *res, const vector2 *v);
VEC_API void vec2_abs_ptr(vector2 *res, const vector2 *v);
VEC_API void vec2_normalize_ptr(vector2 *res, const vector2 *v);
VEC_API void vec2_min_ptr(vector2 *res, const vector2 *a, const vector2 *b);
VEC_API void vec2_max_ptr(vector2 *res, const vector2 *a, const vector2 *b);
VEC_API void vec2_sign_ptr(vector2 *res, const vector2 *v);
VEC_API void vec2_floor_ptr(vector2 *res, const vector2 *v);
VEC_API void vec2_ceil_ptr(vector2 *res, const vector2 *v);
VEC_API void vec2_round_ptr(vector2 *res, const vector2 *v);
VEC_API void vec2_perpendicular_ptr(vector2 *res, const vector2 *v);
VEC_API void vec2_scale_ptr(vector2 *res, const vector2 *v, vm_float_t s);
VEC_API void vec2_cross_ptr(vector2 *res, const vector2 *a, const vector2 *b);
VEC_API void vec2_reflect_ptr(vector2 *res, const vector2 *v, const vector2 *normal);
VEC_API void vec2_project_ptr(vector2 *res, const vector2 *a, const vector2 *b);
VEC_API void vec2_tangent_ptr(vector2 *res, const vector2 *v);
VEC_API void vec2_rotate_ptr(vector2 *result, const vector2 *v, vm_float_t angle);
VEC_API void vec2_slide_ptr(vector2 *result, const vector2 *v, const vector2 *normal);
VEC_API void vec2_clamp_ptr(vector2 *res, const vector2 *v, const vector2 *min, const vector2 *max);
VEC_API void vec2_lerp_ptr(vector2 *res, const vector2 *a, const vector2 *b, vm_float_t t);
VEC_API void vec2_div_ptr(vector2 *res, const vector2 *a, const vector2 *b);
VEC_API void vec2_add_scalar_ptr(vector2 *res, const vector2 *v, vm_float_t s);
VEC_API void vec2_sub_scalar_ptr(vector2 *res, const vector2 *v, vm_float_t s);
VEC_API void vec2_clamp_scalar_ptr(vector2 *res, const vector2 *v, vm_float_t min, vm_float_t max);
VEC_API void vec2_saturate_ptr(vector2 *res, const vector2 *v);
VEC_API void vec2_fract_ptr(vector2 *res, const vector2 *v);
VEC_API void vec2_refract_ptr(vector2 *res, const vector2 *incident, const vector2 *normal, vm_float_t eta);
VEC_API void vec2_reject_ptr(vector2 *res, const vector2 *a, const vector2 *b);
VEC_API void vec2_rotate_around_ptr(vector2 *res, const vector2 *v, const vector2 *pivot, vm_float_t angle);
VEC_API void vec2_move_toward_ptr(vector2 *res, const vector2 *current, const vector2 *target, vm_float_t max_delta);
VEC_API void vec2_limit_length_ptr(vector2 *res, const vector2 *v, vm_float_t max_len);
VEC_API void vec2_to_vec3_ptr(vector3 *res, const vector2 *v, vm_float_t z);

// vector3 pointer-based operations
VEC_API void vec3_add_ptr(vector3 *res, const vector3 *a, const vector3 *b);
VEC_API void vec3_sub_ptr(vector3 *res, const vector3 *a, const vector3 *b);
VEC_API void vec3_mul_scalar_ptr(vector3 *res, const vector3 *v, vm_float_t s);
VEC_API void vec3_div_scalar_ptr(vector3 *res, const vector3 *v, vm_float_t s);
VEC_API void vec3_mul_ptr(vector3 *res, const vector3 *a, const vector3 *b);
VEC_API void vec3_neg_ptr(vector3 *res, const vector3 *v);
VEC_API void vec3_abs_ptr(vector3 *res, const vector3 *v);
VEC_API void vec3_normalize_ptr(vector3 *res, const vector3 *v);
VEC_API void vec3_cross_ptr(vector3 *res, const vector3 *a, const vector3 *b);
VEC_API void vec3_min_ptr(vector3 *res, const vector3 *a, const vector3 *b);
VEC_API void vec3_max_ptr(vector3 *res, const vector3 *a, const vector3 *b);
VEC_API void vec3_sign_ptr(vector3 *res, const vector3 *v);
VEC_API void vec3_floor_ptr(vector3 *res, const vector3 *v);
VEC_API void vec3_ceil_ptr(vector3 *res, const vector3 *v);
VEC_API void vec3_round_ptr(vector3 *res, const vector3 *v);
VEC_API void vec3_reflect_ptr(vector3 *res, const vector3 *incident, const vector3 *normal);
VEC_API void vec3_refract_ptr(vector3 *res, const vector3 *incident, const vector3 *normal, vm_float_t eta);
VEC_API void vec3_lerp_ptr(vector3 *res, const vector3 *a, const vector3 *b, vm_float_t t);
VEC_API void vec3_clamp_ptr(vector3 *res, const vector3 *v, const vector3 *min, const vector3 *max);
VEC_API void vec3_scale_ptr(vector3 *res, const vector3 *v, vm_float_t s);
VEC_API void vec3_div_ptr(vector3 *res, const vector3 *a, const vector3 *b);
VEC_API void vec3_add_scalar_ptr(vector3 *res, const vector3 *v, vm_float_t s);
VEC_API void vec3_sub_scalar_ptr(vector3 *res, const vector3 *v, vm_float_t s);
VEC_API void vec3_clamp_scalar_ptr(vector3 *res, const vector3 *v, vm_float_t min, vm_float_t max);
VEC_API void vec3_saturate_ptr(vector3 *res, const vector3 *v);
VEC_API void vec3_fract_ptr(vector3 *res, const vector3 *v);
VEC_API void vec3_project_ptr(vector3 *res, const vector3 *a, const vector3 *b);
VEC_API void vec3_slide_ptr(vector3 *res, const vector3 *v, const vector3 *normal);
VEC_API void vec3_reject_ptr(vector3 *res, const vector3 *a, const vector3 *b);
VEC_API void vec3_rotate_axis_ptr(vector3 *res, const vector3 *v, const vector3 *axis, vm_float_t angle);
VEC_API void vec3_from_vec2_ptr(vector3 *res, const vector2 *v, vm_float_t z);
VEC_API void vec3_xy_ptr(vector2 *res, const vector3 *v);
VEC_API void vec3_move_toward_ptr(vector3 *res, const vector3 *current, const vector3 *target, vm_float_t max_delta);
VEC_API void vec3_limit_length_ptr(vector3 *res, const vector3 *v, vm_float_t max_len);
VEC_API void vec3_orthonormal_basis_ptr(const vector3 *n, vector3 *t, vector3 *b);

// vector4 pointer-based operations  
VEC_API void vec4_add_ptr(vector4 *res, const vector4 *a, const vector4 *b);
VEC_API void vec4_sub_ptr(vector4 *res, const vector4 *a, const vector4 *b);
VEC_API void vec4_mul_scalar_ptr(vector4 *res, const vector4 *v, vm_float_t s);
VEC_API void vec4_div_scalar_ptr(vector4 *res, const vector4 *v, vm_float_t s);
VEC_API void vec4_mul_ptr(vector4 *res, const vector4 *a, const vector4 *b);
VEC_API void vec4_neg_ptr(vector4 *res, const vector4 *v);
VEC_API void vec4_abs_ptr(vector4 *res, const vector4 *v);
VEC_API void vec4_normalize_ptr(vector4 *res, const vector4 *v);
VEC_API void vec4_min_ptr(vector4 *res, const vector4 *a, const vector4 *b);
VEC_API void vec4_max_ptr(vector4 *res, const vector4 *a, const vector4 *b);
VEC_API void vec4_sign_ptr(vector4 *res, const vector4 *v);
VEC_API void vec4_floor_ptr(vector4 *res, const vector4 *v);
VEC_API void vec4_ceil_ptr(vector4 *res, const vector4 *v);
VEC_API void vec4_round_ptr(vector4 *res, const vector4 *v);
VEC_API void vec4_lerp_ptr(vector4 *res, const vector4 *a, const vector4 *b, vm_float_t t);
VEC_API void vec4_clamp_ptr(vector4 *res, const vector4 *v, const vector4 *min, const vector4 *max);
VEC_API void vec4_homogenize_ptr(vector4 *res, const vector4 *v);
VEC_API void vec4_div_ptr(vector4 *res, const vector4 *a, const vector4 *b);
VEC_API void vec4_add_scalar_ptr(vector4 *res, const vector4 *v, vm_float_t s);
VEC_API void vec4_sub_scalar_ptr(vector4 *res, const vector4 *v, vm_float_t s);
VEC_API void vec4_clamp_scalar_ptr(vector4 *res, const vector4 *v, vm_float_t min, vm_float_t max);
VEC_API void vec4_saturate_ptr(vector4 *res, const vector4 *v);
VEC_API void vec4_fract_ptr(vector4 *res, const vector4 *v);
VEC_API void vec4_project_ptr(vector4 *res, const vector4 *a, const vector4 *b);
VEC_API void vec4_reject_ptr(vector4 *res, const vector4 *a, const vector4 *b);
VEC_API void vec4_slide_ptr(vector4 *res, const vector4 *v, const vector4 *normal);
VEC_API void vec4_to_vec3_ptr(vector3 *res, const vector4 *v);

/*******************************************************************************
 * Integer vector functions
 ******************************************************************************/

// vector2i
VEC_API vector2i vec2i_add(vector2i a, vector2i b);
VEC_API vector2i vec2i_sub(vector2i a, vector2i b);
VEC_API vector2i vec2i_mul_scalar(vector2i v, vm_int_t s);
VEC_API vector2i vec2i_div_scalar(vector2i v, vm_int_t s);
VEC_API vector2i vec2i_mul(vector2i a, vector2i b);
VEC_API vector2i vec2i_neg(vector2i v);
VEC_API vector2i vec2i_abs(vector2i v);
VEC_API vector2i vec2i_zero(void);
VEC_API vector2i vec2i_one(void);
VEC_API vector2i vec2i_x_axis(vm_int_t x);
VEC_API vector2i vec2i_y_axis(vm_int_t y);
VEC_API vector2i vec2i_x_scale(vm_int_t x);
VEC_API vector2i vec2i_y_scale(vm_int_t y);
VEC_API vector2i vec2i_cross(vector2i a, vector2i b);
VEC_API vector2i vec2i_normalize(vector2i v);
VEC_API vector2i vec2i_min(vector2i a, vector2i b);
VEC_API vector2i vec2i_max(vector2i a, vector2i b);
VEC_API vector2i vec2i_sign(vector2i v);
VEC_API vector2i vec2i_perpendicular(vector2i v);
VEC_API vector2i vec2i_lerp(vector2i a, vector2i b, vm_float_t t);
VEC_API vector2i vec2i_clamp(vector2i v, vector2i min, vector2i max);
VEC_API vector2i vec2i_div(vector2i a, vector2i b);
VEC_API vector2i vec2i_add_scalar(vector2i v, vm_int_t s);
VEC_API vector2i vec2i_sub_scalar(vector2i v, vm_int_t s);
VEC_API vector2i vec2i_mod(vector2i a, vector2i b);
VEC_API vector2i vec2i_div_floor(vector2i a, vector2i b);
VEC_API vector2i vec2i_wrap(vector2i v, vector2i period);
VEC_API vector2i vec2i_splat(vm_int_t s);

VEC_API vector3i vec2i_to_vec3i(vector2i v, vm_int_t z);
VEC_API vector2 vec2i_normalize_to_vec2(vector2i v);

VEC_API vm_int_t vec2i_dot(vector2i a, vector2i b);
VEC_API vm_int_t vec2i_cross_scalar(vector2i a, vector2i b);
VEC_API vm_int_t vec2i_length_squared(vector2i v);
VEC_API vm_int_t vec2i_distance_squared(vector2i a, vector2i b);
VEC_API vm_int_t vec2i_length_manhattan(vector2i v);
VEC_API vm_int_t vec2i_length_chebyshev(vector2i v);
VEC_API vm_int_t vec2i_min_component(vector2i v);
VEC_API vm_int_t vec2i_max_component(vector2i v);
VEC_API vm_int_t vec2i_sum(vector2i v);
VEC_API vm_float_t vec2i_length(vector2i v);
VEC_API vm_float_t vec2i_distance(vector2i a, vector2i b);
VEC_API vm_float_t vec2i_angle(vector2i a, vector2i b);
VEC_API vm_float_t vec2i_aspect_ratio(vector2i v);
VEC_API bool vec2i_is_zero(vector2i v);

// vector3i
VEC_API vector3i vec3i_add(vector3i a, vector3i b);
VEC_API vector3i vec3i_sub(vector3i a, vector3i b);
VEC_API vector3i vec3i_mul_scalar(vector3i v, vm_int_t s);
VEC_API vector3i vec3i_div_scalar(vector3i v, vm_int_t s);
VEC_API vector3i vec3i_mul(vector3i a, vector3i b);
VEC_API vector3i vec3i_neg(vector3i v);
VEC_API vector3i vec3i_abs(vector3i v);
VEC_API vector3i vec3i_zero(void);
VEC_API vector3i vec3i_one(void);
VEC_API vector3i vec3i_x_axis(vm_int_t x);
VEC_API vector3i vec3i_y_axis(vm_int_t y);
VEC_API vector3i vec3i_z_axis(vm_int_t z);
VEC_API vector3i vec3i_x_scale(vm_int_t x);
VEC_API vector3i vec3i_y_scale(vm_int_t y);
VEC_API vector3i vec3i_z_scale(vm_int_t z);
VEC_API vector3i vec3i_cross(vector3i a, vector3i b);
VEC_API vector3i vec3i_normalize(vector3i v);
VEC_API vector3i vec3i_min(vector3i a, vector3i b);
VEC_API vector3i vec3i_max(vector3i a, vector3i b);
VEC_API vector3i vec3i_sign(vector3i v);
VEC_API vector3i vec3i_lerp(vector3i a, vector3i b, vm_float_t t);
VEC_API vector3i vec3i_clamp(vector3i v, vector3i min, vector3i max);
VEC_API vector3i vec3i_div(vector3i a, vector3i b);
VEC_API vector3i vec3i_add_scalar(vector3i v, vm_int_t s);
VEC_API vector3i vec3i_sub_scalar(vector3i v, vm_int_t s);
VEC_API vector3i vec3i_mod(vector3i a, vector3i b);
VEC_API vector3i vec3i_div_floor(vector3i a, vector3i b);
VEC_API vector3i vec3i_wrap(vector3i v, vector3i period);
VEC_API vector3i vec3i_splat(vm_int_t s);
VEC_API vector3i vec3i_from_vec2i(vector2i v, vm_int_t z);

VEC_API vector2i vec3i_xy(vector3i v);
VEC_API vector3 vec3i_normalize_to_vec3(vector3i v);

VEC_API vm_int_t vec3i_dot(vector3i a, vector3i b);
VEC_API vm_int_t vec3i_length_squared(vector3i v);
VEC_API vm_int_t vec3i_distance_squared(vector3i a, vector3i b);
VEC_API vm_int_t vec3i_length_manhattan(vector3i v);
VEC_API vm_int_t vec3i_length_chebyshev(vector3i v);
VEC_API vm_int_t vec3i_min_component(vector3i v);
VEC_API vm_int_t vec3i_max_component(vector3i v);
VEC_API vm_int_t vec3i_sum(vector3i v);
VEC_API vm_float_t vec3i_length(vector3i v);
VEC_API vm_float_t vec3i_distance(vector3i a, vector3i b);
VEC_API vm_float_t vec3i_angle(vector3i a, vector3i b);
VEC_API bool vec3i_is_zero(vector3i v);

// vector4i
VEC_API vector4i vec4i_add(vector4i a, vector4i b);
VEC_API vector4i vec4i_sub(vector4i a, vector4i b);
VEC_API vector4i vec4i_mul_scalar(vector4i v, vm_int_t s);
VEC_API vector4i vec4i_div_scalar(vector4i v, vm_int_t s);
VEC_API vector4i vec4i_mul(vector4i a, vector4i b);
VEC_API vector4i vec4i_neg(vector4i v);
VEC_API vector4i vec4i_abs(vector4i v);

VEC_API vector4i vec4i_zero(void);
VEC_API vector4i vec4i_one(void);
VEC_API vector4i vec4i_x_axis(vm_int_t x);
VEC_API vector4i vec4i_y_axis(vm_int_t y);
VEC_API vector4i vec4i_z_axis(vm_int_t z);
VEC_API vector4i vec4i_w_axis(vm_int_t w);
VEC_API vector4i vec4i_x_scale(vm_int_t x);
VEC_API vector4i vec4i_y_scale(vm_int_t y);
VEC_API vector4i vec4i_z_scale(vm_int_t z);
VEC_API vector4i vec4i_w_scale(vm_int_t w);
VEC_API vector4i vec4i_normalize(vector4i v);
VEC_API vector4i vec4i_min(vector4i a, vector4i b);
VEC_API vector4i vec4i_max(vector4i a, vector4i b);
VEC_API vector4i vec4i_sign(vector4i v);
VEC_API vector4i vec4i_lerp(vector4i a, vector4i b, vm_float_t t);
VEC_API vector4i vec4i_clamp(vector4i v, vector4i min, vector4i max);
VEC_API vector4i vec4i_div(vector4i a, vector4i b);
VEC_API vector4i vec4i_add_scalar(vector4i v, vm_int_t s);
VEC_API vector4i vec4i_sub_scalar(vector4i v, vm_int_t s);
VEC_API vector4i vec4i_mod(vector4i a, vector4i b);
VEC_API vector4i vec4i_div_floor(vector4i a, vector4i b);
VEC_API vector4i vec4i_wrap(vector4i v, vector4i period);
VEC_API vector4i vec4i_splat(vm_int_t s);

VEC_API vector3i vec4i_to_vec3i(vector4i v);

VEC_API vm_int_t vec4i_dot(vector4i a, vector4i b);
VEC_API vm_int_t vec4i_length_squared(vector4i v);
VEC_API vm_int_t vec4i_distance_squared(vector4i a, vector4i b);
VEC_API vm_int_t vec4i_length_manhattan(vector4i v);
VEC_API vm_int_t vec4i_length_chebyshev(vector4i v);
VEC_API vm_int_t vec4i_min_component(vector4i v);
VEC_API vm_int_t vec4i_max_component(vector4i v);
VEC_API vm_int_t vec4i_sum(vector4i v);
VEC_API vm_float_t vec4i_length(vector4i v);
VEC_API vm_float_t vec4i_distance(vector4i a, vector4i b);
VEC_API bool vec4i_is_zero(vector4i v);

/*******************************************************************************
 * Integer vector (pointer-based) performance functions
 ******************************************************************************/

// vector2i pointer-based operations
VEC_API void vec2i_add_ptr(vector2i *res, const vector2i *a, const vector2i *b);
VEC_API void vec2i_sub_ptr(vector2i *res, const vector2i *a, const vector2i *b);
VEC_API void vec2i_mul_scalar_ptr(vector2i *res, const vector2i *v, vm_int_t s);
VEC_API void vec2i_div_scalar_ptr(vector2i *res, const vector2i *v, vm_int_t s);
VEC_API void vec2i_mul_ptr(vector2i *res, const vector2i *a, const vector2i *b);
VEC_API void vec2i_neg_ptr(vector2i *res, const vector2i *v);
VEC_API void vec2i_abs_ptr(vector2i *res, const vector2i *v);
VEC_API void vec2i_normalize_ptr(vector2i *res, const vector2i *v);
VEC_API void vec2i_min_ptr(vector2i *res, const vector2i *a, const vector2i *b);
VEC_API void vec2i_max_ptr(vector2i *res, const vector2i *a, const vector2i *b);
VEC_API void vec2i_sign_ptr(vector2i *res, const vector2i *v);
VEC_API void vec2i_perpendicular_ptr(vector2i *res, const vector2i *v);
VEC_API void vec2i_cross_ptr(vector2i *res, const vector2i *a, const vector2i *b);
VEC_API void vec2i_lerp_ptr(vector2i *res, const vector2i *a, const vector2i *b, vm_float_t t);
VEC_API void vec2i_clamp_ptr(vector2i *res, const vector2i *v, const vector2i *min, const vector2i *max);
VEC_API void vec2i_div_ptr(vector2i *res, const vector2i *a, const vector2i *b);
VEC_API void vec2i_add_scalar_ptr(vector2i *res, const vector2i *v, vm_int_t s);
VEC_API void vec2i_sub_scalar_ptr(vector2i *res, const vector2i *v, vm_int_t s);
VEC_API void vec2i_mod_ptr(vector2i *res, const vector2i *a, const vector2i *b);
VEC_API void vec2i_div_floor_ptr(vector2i *res, const vector2i *a, const vector2i *b);
VEC_API void vec2i_wrap_ptr(vector2i *res, const vector2i *v, const vector2i *period);
VEC_API void vec2i_to_vec3i_ptr(vector3i *res, const vector2i *v, vm_int_t z);
VEC_API void vec2i_normalize_to_vec2_ptr(vector2 *res, const vector2i *v);

// vector3i pointer-based operations
VEC_API void vec3i_add_ptr(vector3i *res, const vector3i *a, const vector3i *b);
VEC_API void vec3i_sub_ptr(vector3i *res, const vector3i *a, const vector3i *b);
VEC_API void vec3i_mul_scalar_ptr(vector3i *res, const vector3i *v, vm_int_t s);
VEC_API void vec3i_div_scalar_ptr(vector3i *res, const vector3i *v, vm_int_t s);
VEC_API void vec3i_mul_ptr(vector3i *res, const vector3i *a, const vector3i *b);
VEC_API void vec3i_neg_ptr(vector3i *res, const vector3i *v);
VEC_API void vec3i_abs_ptr(vector3i *res, const vector3i *v);
VEC_API void vec3i_normalize_ptr(vector3i *res, const vector3i *v);
VEC_API void vec3i_cross_ptr(vector3i *res, const vector3i *a, const vector3i *b);
VEC_API void vec3i_min_ptr(vector3i *res, const vector3i *a, const vector3i *b);
VEC_API void vec3i_max_ptr(vector3i *res, const vector3i *a, const vector3i *b);
VEC_API void vec3i_sign_ptr(vector3i *res, const vector3i *v);
VEC_API void vec3i_lerp_ptr(vector3i *res, const vector3i *a, const vector3i *b, vm_float_t t);
VEC_API void vec3i_clamp_ptr(vector3i *res, const vector3i *v, const vector3i *min, const vector3i *max);
VEC_API void vec3i_div_ptr(vector3i *res, const vector3i *a, const vector3i *b);
VEC_API void vec3i_add_scalar_ptr(vector3i *res, const vector3i *v, vm_int_t s);
VEC_API void vec3i_sub_scalar_ptr(vector3i *res, const vector3i *v, vm_int_t s);
VEC_API void vec3i_mod_ptr(vector3i *res, const vector3i *a, const vector3i *b);
VEC_API void vec3i_div_floor_ptr(vector3i *res, const vector3i *a, const vector3i *b);
VEC_API void vec3i_wrap_ptr(vector3i *res, const vector3i *v, const vector3i *period);
VEC_API void vec3i_from_vec2i_ptr(vector3i *res, const vector2i *v, vm_int_t z);
VEC_API void vec3i_xy_ptr(vector2i *res, const vector3i *v);
VEC_API void vec3i_normalize_to_vec3_ptr(vector3 *res, const vector3i *v);

// vector4i pointer-based operations
VEC_API void vec4i_add_ptr(vector4i *res, const vector4i *a, const vector4i *b);
VEC_API void vec4i_sub_ptr(vector4i *res, const vector4i *a, const vector4i *b);
VEC_API void vec4i_mul_scalar_ptr(vector4i *res, const vector4i *v, vm_int_t s);
VEC_API void vec4i_div_scalar_ptr(vector4i *res, const vector4i *v, vm_int_t s);
VEC_API void vec4i_mul_ptr(vector4i *res, const vector4i *a, const vector4i *b);
VEC_API void vec4i_neg_ptr(vector4i *res, const vector4i *v);
VEC_API void vec4i_abs_ptr(vector4i *res, const vector4i *v);
VEC_API void vec4i_normalize_ptr(vector4i *res, const vector4i *v);
VEC_API void vec4i_min_ptr(vector4i *res, const vector4i *a, const vector4i *b);
VEC_API void vec4i_max_ptr(vector4i *res, const vector4i *a, const vector4i *b);
VEC_API void vec4i_sign_ptr(vector4i *res, const vector4i *v);
VEC_API void vec4i_lerp_ptr(vector4i *res, const vector4i *a, const vector4i *b, vm_float_t t);
VEC_API void vec4i_clamp_ptr(vector4i *res, const vector4i *v, const vector4i *min, const vector4i *max);
VEC_API void vec4i_div_ptr(vector4i *res, const vector4i *a, const vector4i *b);
VEC_API void vec4i_add_scalar_ptr(vector4i *res, const vector4i *v, vm_int_t s);
VEC_API void vec4i_sub_scalar_ptr(vector4i *res, const vector4i *v, vm_int_t s);
VEC_API void vec4i_mod_ptr(vector4i *res, const vector4i *a, const vector4i *b);
VEC_API void vec4i_div_floor_ptr(vector4i *res, const vector4i *a, const vector4i *b);
VEC_API void vec4i_wrap_ptr(vector4i *res, const vector4i *v, const vector4i *period);
VEC_API void vec4i_to_vec3i_ptr(vector3i *res, const vector4i *v);

/*******************************************************************************
 * Floating-point matrix functions
 ******************************************************************************/

// matrix2
VEC_API matrix2 mat2_identity(void);
VEC_API matrix2 mat2_mul(matrix2 a, matrix2 b);
VEC_API matrix2 mat2_transpose(matrix2 m);
VEC_API matrix2 mat2_inverse(matrix2 m);
VEC_API matrix2 mat2_rotation_z(vm_float_t degrees);
VEC_API matrix2 mat2_scale(vector2 s);
VEC_API matrix2 mat2_from_mat3(matrix3 m);

VEC_API vector2 mat2_mul_vec2(matrix2 m, vector2 v);

VEC_API vm_float_t mat2_determinant(matrix2 m);

// matrix3
VEC_API matrix3 mat3_identity(void);
VEC_API matrix3 mat3_mul(matrix3 a, matrix3 b);
VEC_API matrix3 mat3_transpose(matrix3 m);
VEC_API matrix3 mat3_inverse(matrix3 m);
VEC_API matrix3 mat3_rotation_x(vm_float_t degrees);
VEC_API matrix3 mat3_rotation_y(vm_float_t degrees);
VEC_API matrix3 mat3_rotation_z(vm_float_t degrees);
VEC_API matrix3 mat3_translate(vector2 t);
VEC_API matrix3 mat3_scale(vector2 s);
VEC_API matrix3 mat3_from_mat4(matrix4 m);

VEC_API vector3 mat3_mul_vec3(matrix3 m, vector3 v);
VEC_API vector2 mat3_mul_vec2(matrix3 m, vector2 v);

VEC_API vm_float_t mat3_determinant(matrix3 m);

// matrix4
VEC_API matrix4 mat4_identity(void);
VEC_API matrix4 mat4_mul(matrix4 a, matrix4 b);
VEC_API matrix4 mat4_transpose(matrix4 m);
VEC_API matrix4 mat4_inverse(matrix4 m);
VEC_API matrix4 mat4_translate(vector3 v);
VEC_API matrix4 mat4_scale(vector3 v);
VEC_API matrix4 mat4_rotation(vector3 axis, vm_float_t angle);
VEC_API matrix4 mat4_rotation_x(vm_float_t degrees);
VEC_API matrix4 mat4_rotation_y(vm_float_t degrees);
VEC_API matrix4 mat4_rotation_z(vm_float_t degrees);
VEC_API matrix4 mat4_trs(vector3 translation, quaternion rotation, vector3 scale);
VEC_API matrix4 mat4_from_mat3(matrix3 m);
VEC_API matrix4 mat4_perspective(vm_float_t fov, vm_float_t aspect, vm_float_t near, vm_float_t far);
VEC_API matrix4 mat4_ortho(vm_float_t left, vm_float_t right, vm_float_t bottom, vm_float_t top, vm_float_t near, vm_float_t far);
VEC_API matrix4 mat4_look_at(vector3 position, vector3 target, vector3 up);
VEC_API matrix4 mat4_perspective_fov(vm_float_t fov, vm_float_t w, vm_float_t h, vm_float_t n, vm_float_t f);
VEC_API matrix4 mat4_perspective_infinite(vm_float_t fov_y, vm_float_t aspect, vm_float_t n);

VEC_API vector3 mat4_extract_translation(matrix4 m);
VEC_API vector3 mat4_extract_scale(matrix4 m);
VEC_API vector3 mat4_mul_vec3(matrix4 m, vector3 v, vm_float_t w);
VEC_API vector4 mat4_mul_vec4(matrix4 m, vector4 v);
VEC_API quaternion mat4_extract_rotation(matrix4 m);

VEC_API vm_float_t mat4_determinant(matrix4 m);

/*******************************************************************************
 * Floating-point matrix pointer-based performance functions
 ******************************************************************************/

// matrix2 pointer-based operations
VEC_API void mat2_identity_ptr(matrix2 *res);
VEC_API void mat2_mul_ptr(matrix2 *res, const matrix2 *a, const matrix2 *b);
VEC_API void mat2_transpose_ptr(matrix2 *res, const matrix2 *m);
VEC_API void mat2_inverse_ptr(matrix2 *res, const matrix2 *m);
VEC_API void mat2_rotation_z_ptr(matrix2 *res, vm_float_t degrees);
VEC_API void mat2_scale_ptr(matrix2 *res, const vector2 *s);
VEC_API void mat2_from_mat3_ptr(matrix2 *res, const matrix3 *m);
VEC_API void mat2_mul_vec2_ptr(vector2 *res, const matrix2 *m, const vector2 *v);

// matrix3 pointer-based operations
VEC_API void mat3_identity_ptr(matrix3 *res);
VEC_API void mat3_mul_ptr(matrix3 *res, const matrix3 *a, const matrix3 *b);
VEC_API void mat3_transpose_ptr(matrix3 *res, const matrix3 *m);
VEC_API void mat3_inverse_ptr(matrix3 *res, const matrix3 *m);
VEC_API void mat3_rotation_x_ptr(matrix3 *res, vm_float_t degrees);
VEC_API void mat3_rotation_y_ptr(matrix3 *res, vm_float_t degrees);
VEC_API void mat3_rotation_z_ptr(matrix3 *res, vm_float_t degrees);
VEC_API void mat3_translate_ptr(matrix3 *res, const vector2 *t);
VEC_API void mat3_scale_ptr(matrix3 *res, const vector2 *s);
VEC_API void mat3_from_mat4_ptr(matrix3 *res, const matrix4 *m);
VEC_API void mat3_mul_vec3_ptr(vector3 *res, const matrix3 *m, const vector3 *v);
VEC_API void mat3_mul_vec2_ptr(vector2 *res, const matrix3 *m, const vector2 *v);

// matrix4 pointer-based operations
VEC_API void mat4_identity_ptr(matrix4 *res);
VEC_API void mat4_mul_ptr(matrix4 *res, const matrix4 *a, const matrix4 *b);
VEC_API void mat4_transpose_ptr(matrix4 *res, const matrix4 *m);
VEC_API void mat4_inverse_ptr(matrix4 *res, const matrix4 *m);
VEC_API void mat4_translate_ptr(matrix4 *res, const vector3 *v);
VEC_API void mat4_scale_ptr(matrix4 *res, const vector3 *v);
VEC_API void mat4_rotation_ptr(matrix4 *res, const vector3 *axis, vm_float_t angle);
VEC_API void mat4_rotation_x_ptr(matrix4 *res, vm_float_t degrees);
VEC_API void mat4_rotation_y_ptr(matrix4 *res, vm_float_t degrees);
VEC_API void mat4_rotation_z_ptr(matrix4 *res, vm_float_t degrees);
VEC_API void mat4_trs_ptr(matrix4 *res, const vector3 *translation, const quaternion *rotation, const vector3 *scale);
VEC_API void mat4_from_mat3_ptr(matrix4 *res, const matrix3 *m);
VEC_API void mat4_extract_translation_ptr(vector3 *res, const matrix4 *m);
VEC_API void mat4_extract_scale_ptr(vector3 *res, const matrix4 *m);
VEC_API void mat4_extract_rotation_ptr(quaternion *res, const matrix4 *m);
VEC_API void mat4_perspective_ptr(matrix4 *res, vm_float_t fov, vm_float_t aspect, vm_float_t near, vm_float_t far);
VEC_API void mat4_ortho_ptr(matrix4 *res, vm_float_t left, vm_float_t right, vm_float_t bottom, vm_float_t top, vm_float_t near, vm_float_t far);
VEC_API void mat4_look_at_ptr(matrix4 *res, const vector3 *position, const vector3 *target, const vector3 *up);
VEC_API void mat4_perspective_fov_ptr(matrix4 *res, vm_float_t fov, vm_float_t w, vm_float_t h, vm_float_t n, vm_float_t f);
VEC_API void mat4_perspective_infinite_ptr(matrix4 *res, vm_float_t fov_y, vm_float_t aspect, vm_float_t n);
VEC_API void mat4_mul_vec4_ptr(vector4 *res, const matrix4 *m, const vector4 *v);
VEC_API void mat4_mul_vec3_ptr(vector3 *res, const matrix4 *m, const vector3 *v, vm_float_t w);

/*******************************************************************************
 * Integer matrix functions
 ******************************************************************************/

// matrix2i
VEC_API matrix2i mat2i_identity(void);
VEC_API matrix2i mat2i_mul(matrix2i a, matrix2i b);
VEC_API matrix2i mat2i_transpose(matrix2i m);
VEC_API matrix2i mat2i_inverse(matrix2i m);

VEC_API vector2i mat2i_mul_vec2i(matrix2i m, vector2i v);

VEC_API vm_int_t mat2i_determinant(matrix2i m);

// matrix3i
VEC_API matrix3i mat3i_identity(void);
VEC_API matrix3i mat3i_mul(matrix3i a, matrix3i b);
VEC_API matrix3i mat3i_transpose(matrix3i m);
VEC_API matrix3i mat3i_inverse(matrix3i m);

VEC_API vector3i mat3i_mul_vec3i(matrix3i m, vector3i v);
VEC_API vector2i mat3i_mul_vec2i(matrix3i m, vector2i v);

VEC_API vm_int_t mat3i_determinant(matrix3i m);

// matrix4i
VEC_API matrix4i mat4i_identity(void);
VEC_API matrix4i mat4i_mul(matrix4i a, matrix4i b);
VEC_API matrix4i mat4i_transpose(matrix4i m);
VEC_API matrix4i mat4i_inverse(matrix4i m);

VEC_API vector4i mat4i_mul_vec4i(matrix4i m, vector4i v);
VEC_API vector3i mat4i_mul_vec3i(matrix4i m, vector3i v, vm_int_t w);

VEC_API vm_int_t mat4i_determinant(matrix4i m);

/*******************************************************************************
 * Integer matrix pointer-based performance functions
 ******************************************************************************/

// matrix2i pointer-based operations
VEC_API void mat2i_identity_ptr(matrix2i *res);
VEC_API void mat2i_mul_ptr(matrix2i *res, const matrix2i *a, const matrix2i *b);
VEC_API void mat2i_transpose_ptr(matrix2i *res, const matrix2i *m);
VEC_API void mat2i_inverse_ptr(matrix2i *res, const matrix2i *m);
VEC_API void mat2i_mul_vec2i_ptr(vector2i *res, const matrix2i *m, const vector2i *v);

// matrix3i pointer-based operations
VEC_API void mat3i_identity_ptr(matrix3i *res);
VEC_API void mat3i_mul_ptr(matrix3i *res, const matrix3i *a, const matrix3i *b);
VEC_API void mat3i_transpose_ptr(matrix3i *res, const matrix3i *m);
VEC_API void mat3i_inverse_ptr(matrix3i *res, const matrix3i *m);
VEC_API void mat3i_mul_vec3i_ptr(vector3i *res, const matrix3i *m, const vector3i *v);
VEC_API void mat3i_mul_vec2i_ptr(vector2i *res, const matrix3i *m, const vector2i *v);

// matrix4i pointer-based operations
VEC_API void mat4i_identity_ptr(matrix4i *res);
VEC_API void mat4i_mul_ptr(matrix4i *res, const matrix4i *a, const matrix4i *b);
VEC_API void mat4i_transpose_ptr(matrix4i *res, const matrix4i *m);
VEC_API void mat4i_inverse_ptr(matrix4i *res, const matrix4i *m);
VEC_API void mat4i_mul_vec4i_ptr(vector4i *res, const matrix4i *m, const vector4i *v);
VEC_API void mat4i_mul_vec3i_ptr(vector3i *res, const matrix4i *m, const vector3i *v, vm_int_t w);

/*******************************************************************************
 * Quaternion functions
 ******************************************************************************/

VEC_API quaternion quat_identity(void);
VEC_API quaternion quat_mul(quaternion a, quaternion b);
VEC_API quaternion quat_normalize(quaternion q);
VEC_API quaternion quat_conjugate(quaternion q);
VEC_API quaternion quat_inverse(quaternion q);
VEC_API quaternion quat_from_euler(vector3 euler);
VEC_API quaternion quat_from_axis_angle(vector3 axis, vm_float_t degrees);
VEC_API quaternion quat_from_mat3(matrix3 m);
VEC_API quaternion quat_from_mat4(matrix4 m);
VEC_API quaternion quat_slerp(quaternion a, quaternion b, vm_float_t t);
VEC_API quaternion quat_nlerp(quaternion a, quaternion b, vm_float_t t);

VEC_API vector3 quat_rotate_vec3(quaternion q, vector3 v);
VEC_API vector3 quat_to_euler(quaternion q);
VEC_API vector3 quat_to_axis_angle(quaternion q, vm_float_t *degrees);

VEC_API matrix4 quat_to_mat4(quaternion q);
VEC_API matrix3 quat_to_mat3(quaternion q);

VEC_API vm_float_t quat_dot(quaternion a, quaternion b);
VEC_API bool quat_near(quaternion a, quaternion b, vm_float_t eps);

/*******************************************************************************
 * Quaternion pointer-based performance functions
 ******************************************************************************/

VEC_API void quat_identity_ptr(quaternion *res);
VEC_API void quat_mul_ptr(quaternion *res, const quaternion *a, const quaternion *b);
VEC_API void quat_normalize_ptr(quaternion *res, const quaternion *q);
VEC_API void quat_conjugate_ptr(quaternion *res, const quaternion *q);
VEC_API void quat_inverse_ptr(quaternion *res, const quaternion *q);
VEC_API void quat_from_euler_ptr(quaternion *res, const vector3 *euler);
VEC_API void quat_from_axis_angle_ptr(quaternion *res, const vector3 *axis, vm_float_t degrees);
VEC_API void quat_from_mat3_ptr(quaternion *res, const matrix3 *m);
VEC_API void quat_from_mat4_ptr(quaternion *res, const matrix4 *m);
VEC_API void quat_slerp_ptr(quaternion *res, const quaternion *a, const quaternion *b, vm_float_t t);
VEC_API void quat_nlerp_ptr(quaternion *res, const quaternion *a, const quaternion *b, vm_float_t t);
VEC_API void quat_rotate_vec3_ptr(vector3 *res, const quaternion *q, const vector3 *v);
VEC_API void quat_to_euler_ptr(vector3 *res, const quaternion *q);
VEC_API void quat_to_axis_angle_ptr(vector3 *axis, vm_float_t *degrees, const quaternion *q);
VEC_API void quat_to_mat4_ptr(matrix4 *res, const quaternion *q);
VEC_API void quat_to_mat3_ptr(matrix3 *res, const quaternion *q);

/*******************************************************************************
 * Conversion functions
 ******************************************************************************/

VEC_API vector2 vec2_from(const vector2i *v);
VEC_API vector2i vec2i_from(const vector2 *v);
VEC_API vector2i vec2i_from_floored(const vector2 *v);
VEC_API vector2i vec2i_from_rounded(const vector2 *v);
VEC_API vector3 vec3_from(const vector3i *v);
VEC_API vector3i vec3i_from(const vector3 *v);
VEC_API vector3i vec3i_from_floored(const vector3 *v);
VEC_API vector3i vec3i_from_rounded(const vector3 *v);

VEC_API vm_float_t deg_to_rad(vm_float_t degrees);
VEC_API vm_float_t rad_to_deg(vm_float_t radians);

/*******************************************************************************
 * Assigment functions
 ******************************************************************************/

VEC_API vector2 vec2(vm_float_t x, vm_float_t y);
VEC_API void vec2_assign(vector2 *dest, const vector2 *src);
VEC_API void vec2_assign_xy(vector2 *dest, vm_float_t x, vm_float_t y);
VEC_API void vec2_add_assign(vector2 *dest, const vector2 *src);

VEC_API vector3 vec3(vm_float_t x, vm_float_t y, vm_float_t z);
VEC_API void vec3_assign(vector3 *dest, const vector3 *src);
VEC_API void vec3_assign_xyz(vector3 *dest, vm_float_t x, vm_float_t y, vm_float_t z);
VEC_API void vec3_add_assign(vector3 *dest, const vector3 *src);

VEC_API vector2i vec2i(vm_int_t x, vm_int_t y);
VEC_API void vec2i_assign(vector2i *dest, const vector2i *src);
VEC_API void vec2i_assign_xy(vector2i *dest, vm_int_t x, vm_int_t y);
VEC_API void vec2i_add_assign(vector2i *dest, const vector2i *src);

VEC_API vector3i vec3i(vm_int_t x, vm_int_t y, vm_int_t z);
VEC_API void vec3i_assign(vector3i *dest, const vector3i *src);
VEC_API void vec3i_assign_xyz(vector3i *dest, vm_int_t x, vm_int_t y, vm_int_t z);
VEC_API void vec3i_add_assign(vector3i *dest, const vector3i *src);

/*******************************************************************************
 * Compare functions
 ******************************************************************************/

VEC_API bool vec2_eq(vector2 a, vector2 b);
VEC_API bool vec3_eq(vector3 a, vector3 b);
VEC_API bool vec4_eq(vector4 a, vector4 b);

VEC_API bool vec2i_eq(vector2i a, vector2i b);
VEC_API bool vec3i_eq(vector3i a, vector3i b);
VEC_API bool vec4i_eq(vector4i a, vector4i b);

VEC_API bool mat2_eq(matrix2 a, matrix2 b);
VEC_API bool mat3_eq(matrix3 a, matrix3 b);
VEC_API bool mat4_eq(matrix4 a, matrix4 b);

VEC_API bool mat2i_eq(matrix2i a, matrix2i b);
VEC_API bool mat3i_eq(matrix3i a, matrix3i b);
VEC_API bool mat4i_eq(matrix4i a, matrix4i b);

VEC_API bool quat_eq(quaternion a, quaternion b);

/*******************************************************************************
 * Easing functions
 ******************************************************************************/

// Easing helper functions
VEC_API vm_float_t elastic_oscillation(vm_float_t f, vm_float_t exp_mult, vm_float_t sin_mult, vm_float_t sin_offset, vm_float_t period);
VEC_API vm_float_t poly_ease_in(vm_float_t f, vm_float_t n);
VEC_API vm_float_t poly_ease_out(vm_float_t f, vm_float_t n);
VEC_API vm_float_t poly_ease_in_out(vm_float_t f, vm_float_t n);

// Quadratic easing
VEC_API vm_float_t quadratic_ease_out(vm_float_t f);
VEC_API vm_float_t quadratic_ease_in(vm_float_t f);
VEC_API vm_float_t quadratic_ease_in_out(vm_float_t f);

// Cubic easing
VEC_API vm_float_t cubic_ease_out(vm_float_t f);
VEC_API vm_float_t cubic_ease_in(vm_float_t f);
VEC_API vm_float_t cubic_ease_in_out(vm_float_t f);

// Quartic easing
VEC_API vm_float_t quartic_ease_out(vm_float_t f);
VEC_API vm_float_t quartic_ease_in(vm_float_t f);
VEC_API vm_float_t quartic_ease_in_out(vm_float_t f);

// Quintic easing
VEC_API vm_float_t quintic_ease_out(vm_float_t f);
VEC_API vm_float_t quintic_ease_in(vm_float_t f);
VEC_API vm_float_t quintic_ease_in_out(vm_float_t f);

// Sine easing
VEC_API vm_float_t sine_ease_out(vm_float_t f);
VEC_API vm_float_t sine_ease_in(vm_float_t f);
VEC_API vm_float_t sine_ease_in_out(vm_float_t f);

// Circular easing
VEC_API vm_float_t circular_ease_out(vm_float_t f);
VEC_API vm_float_t circular_ease_in(vm_float_t f);
VEC_API vm_float_t circular_ease_in_out(vm_float_t f);

// Exponential easing
VEC_API vm_float_t exponential_ease_out(vm_float_t f);
VEC_API vm_float_t exponential_ease_in(vm_float_t f);
VEC_API vm_float_t exponential_ease_in_out(vm_float_t f);

// Elastic easing
VEC_API vm_float_t elastic_ease_out(vm_float_t f);
VEC_API vm_float_t elastic_ease_in(vm_float_t f);
VEC_API vm_float_t elastic_ease_in_out(vm_float_t f);

// Back easing
VEC_API vm_float_t back_ease_out(vm_float_t f);
VEC_API vm_float_t back_ease_in(vm_float_t f);
VEC_API vm_float_t back_ease_in_out(vm_float_t f);

// Bounce easing
VEC_API vm_float_t bounce_ease_out(vm_float_t f);
VEC_API vm_float_t bounce_ease_in(vm_float_t f);
VEC_API vm_float_t bounce_ease_in_out(vm_float_t f);

#endif // VECMAT_H
