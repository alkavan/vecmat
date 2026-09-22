// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_TYPES_H
#define VECMAT_TYPES_H

/**
 * @file
 * @brief Public value types: vectors, matrices, quaternion, clip/layout enums, extras structs.
 * @ingroup vecmat_core
 */

#include "vecmat/config.h"

/**
 * @struct vector2
 * @brief 2-D floating-point vector (`x`, `y`).
 *
 * Also addressable as `v[2]`.
 */
typedef struct {
    union {
        struct {
            vm_float_t x;  /**< X component. */
            vm_float_t y;  /**< Y component. */
        };
        vm_float_t v[VECMAT_VEC2_SIZE];  /**< Components as an array. */
    };
} vector2;

/**
 * @struct vector3
 * @brief 3-D floating-point vector (`x`, `y`, `z`).
 *
 * Also addressable as `v[3]`.
 */
typedef struct {
    union {
        struct {
            vm_float_t x;  /**< X component. */
            vm_float_t y;  /**< Y component. */
            vm_float_t z;  /**< Z component. */
        };
        vm_float_t v[VECMAT_VEC3_SIZE];  /**< Components as an array. */
    };
} vector3;

/**
 * @struct vector4
 * @brief 4-D floating-point vector (`x`, `y`, `z`, `w`).
 *
 * Also addressable as `v[4]`.
 */
typedef struct {
    union {
        struct {
            vm_float_t x;  /**< X component. */
            vm_float_t y;  /**< Y component. */
            vm_float_t z;  /**< Z component. */
            vm_float_t w;  /**< W component. */
        };
        vm_float_t v[VECMAT_VEC4_SIZE];  /**< Components as an array. */
    };
} vector4;

/*******************************************************************************
 * Integer vector types
 ******************************************************************************/

/**
 * @struct vector2i
 * @brief 2-D integer vector (`x`, `y`).
 *
 * Also addressable as `v[2]`.
 */
typedef struct {
    union {
        struct {
            vm_int_t x;  /**< X component. */
            vm_int_t y;  /**< Y component. */
        };
        vm_int_t v[VECMAT_VEC2_SIZE];  /**< Components as an array. */
    };
} vector2i;

/**
 * @struct vector3i
 * @brief 3-D integer vector (`x`, `y`, `z`).
 *
 * Also addressable as `v[3]`.
 */
typedef struct {
    union {
        struct {
            vm_int_t x;  /**< X component. */
            vm_int_t y;  /**< Y component. */
            vm_int_t z;  /**< Z component. */
        };
        vm_int_t v[VECMAT_VEC3_SIZE];  /**< Components as an array. */
    };
} vector3i;

/**
 * @struct vector4i
 * @brief 4-D integer vector (`x`, `y`, `z`, `w`).
 *
 * Also addressable as `v[4]`.
 */
typedef struct {
    union {
        struct {
            vm_int_t x;  /**< X component. */
            vm_int_t y;  /**< Y component. */
            vm_int_t z;  /**< Z component. */
            vm_int_t w;  /**< W component. */
        };
        vm_int_t v[VECMAT_VEC4_SIZE];  /**< Components as an array. */
    };
} vector4i;

/**
 * @struct matrix2
 * @brief 2x2 column-major floating-point matrix.
 *
 * Layout:
 * [0/m11] [2/m12]
 * [1/m21] [3/m22]
 */
typedef struct {
    union {
        struct {
            vm_float_t m11;  /**< Column 0, row 0. */
            vm_float_t m21;  /**< Column 0, row 1. */
            vm_float_t m12;  /**< Column 1, row 0. */
            vm_float_t m22;  /**< Column 1, row 1. */
        };
        vm_float_t v[VECMAT_MAT2_SIZE];  /**< Elements in column-major order. */
    };
} matrix2;

/**
 * @struct matrix3
 * @brief 3x3 column-major floating-point matrix.
 *
 * Layout:
 * [0/m11] [3/m12] [6/m13]
 * [1/m21] [4/m22] [7/m23]
 * [2/m31] [5/m32] [8/m33]
 */
typedef struct {
    union {
        struct {
            vm_float_t m11;  /**< Column 0, row 0. */
            vm_float_t m21;  /**< Column 0, row 1. */
            vm_float_t m31;  /**< Column 0, row 2. */
            vm_float_t m12;  /**< Column 1, row 0. */
            vm_float_t m22;  /**< Column 1, row 1. */
            vm_float_t m32;  /**< Column 1, row 2. */
            vm_float_t m13;  /**< Column 2, row 0. */
            vm_float_t m23;  /**< Column 2, row 1. */
            vm_float_t m33;  /**< Column 2, row 2. */
        };
        vm_float_t v[VECMAT_MAT3_SIZE];  /**< Elements in column-major order. */
    };
} matrix3;

/**
 * @struct matrix4
 * @brief 4x4 column-major floating-point matrix.
 *
 * Layout:
 * [ 0/m11] [ 4/m12] [ 8/m13] [12/m14]
 * [ 1/m21] [ 5/m22] [ 9/m23] [13/m24]
 * [ 2/m31] [ 6/m32] [10/m33] [14/m34]
 * [ 3/m41] [ 7/m42] [11/m43] [15/m44]
 */
typedef struct {
    union {
        struct {
            vm_float_t m11;  /**< Column 0, row 0. */
            vm_float_t m21;  /**< Column 0, row 1. */
            vm_float_t m31;  /**< Column 0, row 2. */
            vm_float_t m41;  /**< Column 0, row 3. */
            vm_float_t m12;  /**< Column 1, row 0. */
            vm_float_t m22;  /**< Column 1, row 1. */
            vm_float_t m32;  /**< Column 1, row 2. */
            vm_float_t m42;  /**< Column 1, row 3. */
            vm_float_t m13;  /**< Column 2, row 0. */
            vm_float_t m23;  /**< Column 2, row 1. */
            vm_float_t m33;  /**< Column 2, row 2. */
            vm_float_t m43;  /**< Column 2, row 3. */
            vm_float_t m14;  /**< Column 3, row 0. */
            vm_float_t m24;  /**< Column 3, row 1. */
            vm_float_t m34;  /**< Column 3, row 2. */
            vm_float_t m44;  /**< Column 3, row 3. */
        };
        vm_float_t v[VECMAT_MAT4_SIZE];  /**< Elements in column-major order. */
    };
} matrix4;

/*******************************************************************************
 * Integer matrix types
 ******************************************************************************/

/**
 * @struct matrix2i
 * @brief 2x2 column-major integer matrix.
 *
 * Layout:
 * [0/m11] [2/m12]
 * [1/m21] [3/m22]
 */
typedef struct {
    union {
        struct {
            vm_int_t m11;  /**< Column 0, row 0. */
            vm_int_t m21;  /**< Column 0, row 1. */
            vm_int_t m12;  /**< Column 1, row 0. */
            vm_int_t m22;  /**< Column 1, row 1. */
        };
        vm_int_t v[VECMAT_MAT2_SIZE];  /**< Elements in column-major order. */
    };
} matrix2i;

/**
 * @struct matrix3i
 * @brief 3x3 column-major integer matrix.
 *
 * Layout:
 * [0/m11] [3/m12] [6/m13]
 * [1/m21] [4/m22] [7/m23]
 * [2/m31] [5/m32] [8/m33]
 */
typedef struct {
    union {
        struct {
            vm_int_t m11;  /**< Column 0, row 0. */
            vm_int_t m21;  /**< Column 0, row 1. */
            vm_int_t m31;  /**< Column 0, row 2. */
            vm_int_t m12;  /**< Column 1, row 0. */
            vm_int_t m22;  /**< Column 1, row 1. */
            vm_int_t m32;  /**< Column 1, row 2. */
            vm_int_t m13;  /**< Column 2, row 0. */
            vm_int_t m23;  /**< Column 2, row 1. */
            vm_int_t m33;  /**< Column 2, row 2. */
        };
        vm_int_t v[VECMAT_MAT3_SIZE];  /**< Elements in column-major order. */
    };
} matrix3i;

/**
 * @struct matrix4i
 * @brief 4x4 column-major integer matrix.
 *
 * Layout:
 * [ 0/m11] [ 4/m12] [ 8/m13] [12/m14]
 * [ 1/m21] [ 5/m22] [ 9/m23] [13/m24]
 * [ 2/m31] [ 6/m32] [10/m33] [14/m34]
 * [ 3/m41] [ 7/m42] [11/m43] [15/m44]
 */
typedef struct {
    union {
        struct {
            vm_int_t m11;  /**< Column 0, row 0. */
            vm_int_t m21;  /**< Column 0, row 1. */
            vm_int_t m31;  /**< Column 0, row 2. */
            vm_int_t m41;  /**< Column 0, row 3. */
            vm_int_t m12;  /**< Column 1, row 0. */
            vm_int_t m22;  /**< Column 1, row 1. */
            vm_int_t m32;  /**< Column 1, row 2. */
            vm_int_t m42;  /**< Column 1, row 3. */
            vm_int_t m13;  /**< Column 2, row 0. */
            vm_int_t m23;  /**< Column 2, row 1. */
            vm_int_t m33;  /**< Column 2, row 2. */
            vm_int_t m43;  /**< Column 2, row 3. */
            vm_int_t m14;  /**< Column 3, row 0. */
            vm_int_t m24;  /**< Column 3, row 1. */
            vm_int_t m34;  /**< Column 3, row 2. */
            vm_int_t m44;  /**< Column 3, row 3. */
        };
        vm_int_t v[VECMAT_MAT4_SIZE];  /**< Elements in column-major order. */
    };
} matrix4i;

/**
 * @struct quaternion
 * @brief Rotation quaternion (vector part x,y,z, scalar part w).
 *
 * Also addressable as v[4].
 */
typedef struct {
    union {
        struct {
            vm_float_t x;  /**< Vector part X. */
            vm_float_t y;  /**< Vector part Y. */
            vm_float_t z;  /**< Vector part Z. */
            vm_float_t w;  /**< Scalar part. */
        };
        vm_float_t v[VECMAT_QUAT_SIZE];  /**< Components as an array. */
    };
} quaternion;

/**
 * @brief Clip-space handedness and depth range for projection / look-at matrices.
 *
 * Historic `mat4_perspective` / `mat4_look_at` / `mat4_ortho` stay OpenGL
 * right-handed with z in [-1, 1] (`VM_CLIP_RH_NO`). Perspective FOV and
 * rotation angles are in radians; use the `_deg` suffix for degrees.
 */
typedef enum {
    VM_CLIP_RH_NO = 0,  /**< OpenGL: right-handed, clip z in [-1, 1] */
    VM_CLIP_RH_ZO,      /**< Vulkan: right-handed, clip z in [0, 1] */
    VM_CLIP_LH_ZO,      /**< Direct3D: left-handed, clip z in [0, 1] */
    VM_CLIP_LH_NO       /**< Left-handed, clip z in [-1, 1] */
} vm_clip_t;

#define VM_CLIP_OPENGL  VM_CLIP_RH_NO
#define VM_CLIP_VULKAN  VM_CLIP_RH_ZO
#define VM_CLIP_DIRECTX VM_CLIP_LH_ZO

/**
 * @brief Memory layout for dense panels passed to `vm_gemm` and `vm_mat`.
 */
typedef enum {
    VM_LAYOUT_COL_MAJOR = 0,  /**< Element (r, c) at data[r + c * ld] */
    VM_LAYOUT_ROW_MAJOR = 1   /**< Element (r, c) at data[c + r * ld] */
} vm_layout_t;

/**
 * @brief Optional GEMM epilogue. Flags may be OR-ed.
 *
 * Bias is length `N` and broadcasts over rows: `C(i, j) += bias[j]`.
 * ReLU is `max(C, 0)` after alpha/beta/bias.
 */
enum {
    VM_GEMM_OP_NONE      = 0,  /**< No epilogue */
    VM_GEMM_OP_BIAS      = 1,  /**< Add per-column bias */
    VM_GEMM_OP_RELU      = 2,  /**< Apply ReLU */
    VM_GEMM_OP_BIAS_RELU = 3   /**< Bias then ReLU (`BIAS | RELU`) */
};

/**
 * @brief Heap dense matrix used by LU / QR / SVD / Cholesky / det / inverse.
 *
 * `data` is column-major: element (r, c) lives at `data[r + c * rows]`.
 */
typedef struct {
    int rows;          /**< Number of rows */
    int cols;          /**< Number of columns */
    vm_float_t *data;  /**< Column-major storage, length `rows * cols` */
} vm_mat;

/**
 * @brief Heap sparse matrix in compressed sparse row (CSR) form.
 *
 * Square `n × n`. Row `i` owns `col[k]`, `val[k]` for
 * `row_ptr[i] <= k < row_ptr[i + 1]`.
 */
typedef struct {
    int n;            /**< Matrix order (`n × n`) */
    int nnz;          /**< Number of stored non-zeros */
    int *row_ptr;     /**< Row starts; length `n + 1`, `row_ptr[n] == nnz` */
    int *col;         /**< Column indices; length `nnz` */
    vm_float_t *val;  /**< Nonzero values; length `nnz` */
} vm_spmat;

/**
 * @brief Iteration stats from `vm_cg` / `vm_bicgstab`.
 */
typedef struct {
    int iters;           /**< Iterations performed */
    vm_float_t rel_res;  /**< ||r|| / max(||b||, ε) at exit */
    bool ok;             /**< Converged within `tol` */
} vm_ksp_info;

/**
 * @brief Left preconditioner for the Krylov solvers.
 *
 * `IC0` is incomplete Cholesky with no fill. On breakdown the solver
 * falls back to Jacobi for that call.
 */
typedef enum {
    VM_KSP_PREC_NONE   = 0,  /**< No preconditioning */
    VM_KSP_PREC_JACOBI = 1,  /**< Diagonal (Jacobi) scaling */
    VM_KSP_PREC_SSOR   = 2,  /**< Symmetric successive over-relaxation */
    VM_KSP_PREC_IC0    = 3   /**< Incomplete Cholesky, zero fill */
} vm_ksp_prec_t;

/**
 * @brief Boundary condition for assembled grid operators.
 *
 * Dirichlet rows become the identity (rhs holds the boundary value).
 * Homogeneous Neumann drops the missing neighbour; the 7-point
 * Neumann Laplacian is singular (constant nullspace).
 */
typedef enum {
    VM_BC_DIRICHLET = 0,  /**< Fixed-value boundary (identity rows) */
    VM_BC_NEUMANN   = 1   /**< Homogeneous Neumann (drop missing neighbour) */
} vm_bc_t;

/**
 * @brief Uniform Cartesian grid. `nz == 1` is a 2-D problem.
 */
typedef struct {
    int nx;         /**< Cells in x */
    int ny;         /**< Cells in y */
    int nz;         /**< Cells in z (`1` for 2-D) */
    vm_float_t dx;  /**< Cell size in x */
    vm_float_t dy;  /**< Cell size in y */
    vm_float_t dz;  /**< Cell size in z */
} vm_grid3;

/**
 * @brief First-order ODE right-hand side `ydot = f(y)`.
 */
typedef void (*vm_ode_fn)(const vm_float_t *y, vm_float_t *ydot, void *ctx);

/**
 * @brief Acceleration callback `a = acc(x)` for velocity Verlet.
 */
typedef void (*vm_acc_fn)(const vm_float_t *x, vm_float_t *a, void *ctx);

/*******************************************************************************
 * Helpers
 ******************************************************************************/

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

/*******************************************************************************
 * Type Alignment
 ******************************************************************************/

#ifdef VECMAT_USE_F64
#define VECMAT_EPSILON VECMAT_EPS_F64
#else
#define VECMAT_EPSILON VECMAT_EPS_F32
#endif

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

#endif //VECMAT_TYPES_H
