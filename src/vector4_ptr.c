// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "features/cpu.h"

/**
 * @brief Performs component-wise addition of two vector4.
 *
 * @param res Pointer to result `vector4`.
 * @param a Pointer to first `vector4`.
 * @param b Pointer to second `vector4`.
 */
VECMAT_SCALAR_API void vec4_add_ptr_scalar(vector4 *res, const vector4 *a, const vector4 *b)
{
    res->x = a->x + b->x;
    res->y = a->y + b->y;
    res->z = a->z + b->z;
    res->w = a->w + b->w;
}

/**
 * Performs component-wise subtraction of two `vector4`.
 *
 * @param res Pointer to result `vector4`.
 * @param a Pointer to first `vector4`.
 * @param b Pointer to second `vector4`.
 */
VECMAT_SCALAR_API void vec4_sub_ptr_scalar(vector4 *res, const vector4 *a, const vector4 *b)
{
    res->x = a->x - b->x;
    res->y = a->y - b->y;
    res->z = a->z - b->z;
    res->w = a->w - b->w;
}

/**
 * @brief Performs component-wise multiplication of a `vector4` by a scalar.
 *
 * @param res Pointer to result `vector4`.
 * @param v Pointer to input `vector4`.
 * @param s The scalar multiplier.
 */
VECMAT_SCALAR_API void vec4_mul_scalar_ptr_scalar(vector4 *res, const vector4 *v, const vm_float_t s)
{
    res->x = v->x * s;
    res->y = v->y * s;
    res->z = v->z * s;
    res->w = v->w * s;
}

/**
 * @brief Performs component-wise division of a `vector4` by a scalar.
 *
 * If the scalar is zero, sets the result vector to (0,0,0,0).
 *
 * @param res Pointer to result `vector4`.
 * @param v Pointer to input `vector4`.
 * @param s Scalar divisor.
 */
VECMAT_SCALAR_API void vec4_div_scalar_ptr_scalar(vector4 *res, const vector4 *v, const vm_float_t s)
{
    if (s == 0.0f) {
        res->x = res->y = res->z = res->w = 0.0f;
        return;
    }
    res->x = v->x / s;
    res->y = v->y / s;
    res->z = v->z / s;
    res->w = v->w / s;
}

/**
 * @brief Performs component-wise multiplication of two `vector4`.
 *
 * @param res Pointer to result `vector4`.
 * @param a Pointer to first `vector4`.
 * @param b Pointer to second `vector4`.
 */
VECMAT_SCALAR_API void vec4_mul_ptr_scalar(vector4 *res, const vector4 *a, const vector4 *b)
{
    res->x = a->x * b->x;
    res->y = a->y * b->y;
    res->z = a->z * b->z;
    res->w = a->w * b->w;
}

/**
 * @brief Performs component-wise negation of a `vector4`.
 *
 * @param res Pointer to result `vector4`.
 * @param v Pointer to input `vector4`.
 */
VECMAT_SCALAR_API void vec4_neg_ptr_scalar(vector4 *res, const vector4 *v)
{
    res->x = -v->x;
    res->y = -v->y;
    res->z = -v->z;
    res->w = -v->w;
}

/**
 * Performs component-wise absolute value on a `vector4`.
 *
 * @param res Pointer to result `vector4`.
 * @param v Pointer to input `vector4`.
 */
VECMAT_SCALAR_API void vec4_abs_ptr_scalar(vector4 *res, const vector4 *v)
{
    res->x = VECMAT_FABS(v->x);
    res->y = VECMAT_FABS(v->y);
    res->z = VECMAT_FABS(v->z);
    res->w = VECMAT_FABS(v->w);
}

/**
 * @brief Normalizes a `vector4` by scaling its components to unit length.
 *
 * @param res Pointer to result `vector4`.
 * @param v Pointer to input `vector4`.
 */
VECMAT_SCALAR_API void vec4_normalize_ptr_scalar(vector4 *res, const vector4 *v)
{
    const vm_float_t len = vec4_length(*v);
    if (len == 0.0f) {
        *res = *v;
        return;
    }
    const vm_float_t inv_len = 1.0f / len;
    res->x = v->x * inv_len;
    res->y = v->y * inv_len;
    res->z = v->z * inv_len;
    res->w = v->w * inv_len;
}

/**
 * @brief Performs component-wise minimum of two `vector4`.
 *
 * @param res Pointer to result `vector4`.
 * @param a Pointer to first `vector4`.
 * @param b Pointer to second `vector4`.
 */
VECMAT_SCALAR_API void vec4_min_ptr_scalar(vector4 *res, const vector4 *a, const vector4 *b)
{
    res->x = VECMAT_FMIN(a->x, b->x);
    res->y = VECMAT_FMIN(a->y, b->y);
    res->z = VECMAT_FMIN(a->z, b->z);
    res->w = VECMAT_FMIN(a->w, b->w);
}

/**
 * @brief Performs component-wise maximum of two `vector4`.
 *
 * @param res Pointer to result `vector4`.
 * @param a Pointer to first `vector4`.
 * @param b Pointer to second `vector4`.
 */
VECMAT_SCALAR_API void vec4_max_ptr_scalar(vector4 *res, const vector4 *a, const vector4 *b)
{
    res->x = VECMAT_FMAX(a->x, b->x);
    res->y = VECMAT_FMAX(a->y, b->y);
    res->z = VECMAT_FMAX(a->z, b->z);
    res->w = VECMAT_FMAX(a->w, b->w);
}

/**
 * @brief Computes the component-wise sign of a vector4.
 *
 * @param res Pointer to result `vector4`.
 * @param v Pointer to input `vector4`.
 */
VECMAT_SCALAR_API void vec4_sign_ptr_scalar(vector4 *res, const vector4 *v)
{
    res->x = v->x > 0.0f ? 1.0f : v->x < 0.0f ? -1.0f : 0.0f;
    res->y = v->y > 0.0f ? 1.0f : v->y < 0.0f ? -1.0f : 0.0f;
    res->z = v->z > 0.0f ? 1.0f : v->z < 0.0f ? -1.0f : 0.0f;
    res->w = v->w > 0.0f ? 1.0f : v->w < 0.0f ? -1.0f : 0.0f;
}

/**
 * @brief Performs component-wise floor operation on a `vector4`.
 *
 * @param res Pointer to result `vector4`.
 * @param v Pointer to input `vector4`.
 */
VECMAT_SCALAR_API void vec4_floor_ptr_scalar(vector4 *res, const vector4 *v)
{
    res->x = VECMAT_FLOOR(v->x);
    res->y = VECMAT_FLOOR(v->y);
    res->z = VECMAT_FLOOR(v->z);
    res->w = VECMAT_FLOOR(v->w);
}

/**
 * @brief Performs component-wise ceiling of a `vector4`.
 *
 * @param res Pointer to result `vector4`.
 * @param v Pointer to input `vector4`.
 */
VECMAT_SCALAR_API void vec4_ceil_ptr_scalar(vector4 *res, const vector4 *v)
{
    res->x = VECMAT_CEIL(v->x);
    res->y = VECMAT_CEIL(v->y);
    res->z = VECMAT_CEIL(v->z);
    res->w = VECMAT_CEIL(v->w);
}

/**
 * @brief Performs component-wise rounding of a `vector4` to the nearest integer.
 *
 * @param res Pointer to result `vector4`.
 * @param v Pointer to input `vector4`.
 */
VECMAT_SCALAR_API void vec4_round_ptr_scalar(vector4 *res, const vector4 *v)
{
    res->x = VECMAT_ROUND(v->x);
    res->y = VECMAT_ROUND(v->y);
    res->z = VECMAT_ROUND(v->z);
    res->w = VECMAT_ROUND(v->w);
}

/**
 * @brief Performs linear interpolation between two `vector4` with interpolation
 * factor t.
 *
 * @param res Pointer to result `vector4`.
 * @param a Pointer to first `vector4`.
 * @param b Pointer to second `vector4`.
 * @param t Interpolation factor.
 */
VECMAT_SCALAR_API void vec4_lerp_ptr_scalar(vector4 *res, const vector4 *a, const vector4 *b, const vm_float_t t)
{
    res->x = a->x + t * (b->x - a->x);
    res->y = a->y + t * (b->y - a->y);
    res->z = a->z + t * (b->z - a->z);
    res->w = a->w + t * (b->w - a->w);
}

/**
 * @brief Performs component-wise clamping of a `vector4` to the specified min
 * and max bounds.
 *
 * @param res Pointer to result `vector4`.
 * @param v Pointer to input `vector4`.
 * @param min Pointer to minimum bounds `vector4`.
 * @param max Pointer to maximum bounds `vector4`.
 */
VECMAT_SCALAR_API void vec4_clamp_ptr_scalar(vector4 *res, const vector4 *v, const vector4 *min, const vector4 *max)
{
    res->x = VECMAT_FMAX(min->x, VECMAT_FMIN(max->x, v->x));
    res->y = VECMAT_FMAX(min->y, VECMAT_FMIN(max->y, v->y));
    res->z = VECMAT_FMAX(min->z, VECMAT_FMIN(max->z, v->z));
    res->w = VECMAT_FMAX(min->w, VECMAT_FMIN(max->w, v->w));
}

/**
 * @brief Homogenizes a `vector4` by dividing its x, y, z components by w.
 *
 * If |w| > `VECMAT_EPSILON` and setting w to 1.0f.
 * Otherwise, sets all components of the result to 0.0f.
 *
 * @param res Pointer to result `vector4`.
 * @param v Pointer to input `vector4`.
 */
VECMAT_SCALAR_API void vec4_homogenize_ptr_scalar(vector4 *res, const vector4 *v)
{
    if (VECMAT_FABS(v->w) > VECMAT_EPSILON) {
        const vm_float_t inv_w = 1.0f / v->w;
        res->x = v->x * inv_w;
        res->y = v->y * inv_w;
        res->z = v->z * inv_w;
        res->w = 1.0f;
    } else {
        res->x = res->y = res->z = res->w = 0.0f;
    }
}

/**
 * @brief Copies the x, y, z components from a `vector4` to a `vector3`.
 *
 * @param res Pointer to result `vector3`.
 * @param v Pointer to input `vector4`.
 */
void vec4_to_vec3_ptr(vector3 *res, const vector4 *v)
{
    res->x = v->x;
    res->y = v->y;
    res->z = v->z;
}

/**
 * @brief Divides two vectors component-wise.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
VECMAT_SCALAR_API void vec4_div_ptr_scalar(vector4 *res, const vector4 *a, const vector4 *b)
{
    res->x = (b->x == 0.0f) ? 0.0f : a->x / b->x;
    res->y = (b->y == 0.0f) ? 0.0f : a->y / b->y;
    res->z = (b->z == 0.0f) ? 0.0f : a->z / b->z;
    res->w = (b->w == 0.0f) ? 0.0f : a->w / b->w;
}

/**
 * @brief Adds a scalar to each component.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param s Scalar value.
 */
VECMAT_SCALAR_API void vec4_add_scalar_ptr_scalar(vector4 *res, const vector4 *v, const vm_float_t s)
{
    res->x = v->x + s;
    res->y = v->y + s;
    res->z = v->z + s;
    res->w = v->w + s;
}

/**
 * @brief Subtracts a scalar from each component.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param s Scalar value.
 */
VECMAT_SCALAR_API void vec4_sub_scalar_ptr_scalar(vector4 *res, const vector4 *v, const vm_float_t s)
{
    res->x = v->x - s;
    res->y = v->y - s;
    res->z = v->z - s;
    res->w = v->w - s;
}

/**
 * @brief Clamps each component to the scalar range [min, max].
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param min Lower bound.
 * @param max Upper bound.
 */
VECMAT_SCALAR_API void vec4_clamp_scalar_ptr_scalar(vector4 *res, const vector4 *v, const vm_float_t min, const vm_float_t max)
{
    res->x = VECMAT_FMIN(VECMAT_FMAX(v->x, min), max);
    res->y = VECMAT_FMIN(VECMAT_FMAX(v->y, min), max);
    res->z = VECMAT_FMIN(VECMAT_FMAX(v->z, min), max);
    res->w = VECMAT_FMIN(VECMAT_FMAX(v->w, min), max);
}

/**
 * @brief Clamps each component to the range [0, 1].
 *
 * @param res Output vector.
 * @param v Input vector.
 */
VECMAT_SCALAR_API void vec4_saturate_ptr_scalar(vector4 *res, const vector4 *v)
{
    vec4_clamp_scalar_ptr_scalar(res, v, 0.0f, 1.0f);
}

/**
 * @brief Returns the fractional part of each component.
 *
 * @param res Output vector.
 * @param v Input vector.
 */
VECMAT_SCALAR_API void vec4_fract_ptr_scalar(vector4 *res, const vector4 *v)
{
    res->x = v->x - VECMAT_FLOOR(v->x);
    res->y = v->y - VECMAT_FLOOR(v->y);
    res->z = v->z - VECMAT_FLOOR(v->z);
    res->w = v->w - VECMAT_FLOOR(v->w);
}

/**
 * @brief Projects a onto b.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
void vec4_project_ptr(vector4 *res, const vector4 *a, const vector4 *b)
{
    const vm_float_t denom = vec4_dot(*b, *b);
    if (denom == 0.0f) {
        res->x = res->y = res->z = res->w = 0.0f;
        return;
    }
    const vm_float_t scale = vec4_dot(*a, *b) / denom;
    res->x = b->x * scale;
    res->y = b->y * scale;
    res->z = b->z * scale;
    res->w = b->w * scale;
}

/**
 * @brief Removes the component of v along normal.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param normal Surface normal.
 */
void vec4_slide_ptr(vector4 *res, const vector4 *v, const vector4 *normal)
{
    const vm_float_t d = vec4_dot(*v, *normal);
    res->x = v->x - normal->x * d;
    res->y = v->y - normal->y * d;
    res->z = v->z - normal->z * d;
    res->w = v->w - normal->w * d;
}

/**
 * @brief Returns the component of a orthogonal to b.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
void vec4_reject_ptr(vector4 *res, const vector4 *a, const vector4 *b)
{
    vector4 projected;
    vec4_project_ptr(&projected, a, b);
    res->x = a->x - projected.x;
    res->y = a->y - projected.y;
    res->z = a->z - projected.z;
    res->w = a->w - projected.w;
}

/**
 * @brief Component-wise addition (dispatched).
 *
 * @see vec4_add_ptr_scalar
 *
 * @param res Result vector.
 * @param a First vector.
 * @param b Second vector.
 */
void vec4_add_ptr(vector4 *res, const vector4 *a, const vector4 *b)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    vec4_add_ptr_(res, a, b);
#else
    vec4_add_ptr_scalar(res, a, b);
#endif
}

/**
 * @brief Component-wise subtraction (dispatched).
 *
 * @see vec4_sub_ptr_scalar
 *
 * @param res Result vector.
 * @param a First vector.
 * @param b Second vector.
 */
void vec4_sub_ptr(vector4 *res, const vector4 *a, const vector4 *b)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    vec4_sub_ptr_(res, a, b);
#else
    vec4_sub_ptr_scalar(res, a, b);
#endif
}

/**
 * @brief Component-wise multiplication (dispatched).
 *
 * @see vec4_mul_ptr_scalar
 *
 * @param res Result vector.
 * @param a First vector.
 * @param b Second vector.
 */
void vec4_mul_ptr(vector4 *res, const vector4 *a, const vector4 *b)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    vec4_mul_ptr_(res, a, b);
#else
    vec4_mul_ptr_scalar(res, a, b);
#endif
}

/**
 * @brief Multiplies a vector by a scalar (dispatched).
 *
 * @see vec4_mul_scalar_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 * @param s Scalar.
 */
void vec4_mul_scalar_ptr(vector4 *res, const vector4 *v, const vm_float_t s)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    vec4_mul_scalar_ptr_(res, v, s);
#else
    vec4_mul_scalar_ptr_scalar(res, v, s);
#endif
}

/**
 * @brief Divides a vector by a scalar (dispatched).
 *
 * @see vec4_div_scalar_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 * @param s Scalar.
 */
void vec4_div_scalar_ptr(vector4 *res, const vector4 *v, const vm_float_t s)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    vec4_div_scalar_ptr_(res, v, s);
#else
    vec4_div_scalar_ptr_scalar(res, v, s);
#endif
}

/**
 * @brief Negates a vector (dispatched).
 *
 * @see vec4_neg_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 */
void vec4_neg_ptr(vector4 *res, const vector4 *v)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    vec4_neg_ptr_(res, v);
#else
    vec4_neg_ptr_scalar(res, v);
#endif
}

/**
 * @brief Component-wise absolute value (dispatched).
 *
 * @see vec4_abs_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 */
void vec4_abs_ptr(vector4 *res, const vector4 *v)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    vec4_abs_ptr_(res, v);
#else
    vec4_abs_ptr_scalar(res, v);
#endif
}

/**
 * @brief Normalizes a vector (dispatched).
 *
 * @see vec4_normalize_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 */
void vec4_normalize_ptr(vector4 *res, const vector4 *v)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    vec4_normalize_ptr_(res, v);
#else
    vec4_normalize_ptr_scalar(res, v);
#endif
}

/**
 * @brief Component-wise minimum (dispatched).
 *
 * @see vec4_min_ptr_scalar
 *
 * @param res Result vector.
 * @param a First vector.
 * @param b Second vector.
 */
void vec4_min_ptr(vector4 *res, const vector4 *a, const vector4 *b)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    vec4_min_ptr_(res, a, b);
#else
    vec4_min_ptr_scalar(res, a, b);
#endif
}

/**
 * @brief Component-wise maximum (dispatched).
 *
 * @see vec4_max_ptr_scalar
 *
 * @param res Result vector.
 * @param a First vector.
 * @param b Second vector.
 */
void vec4_max_ptr(vector4 *res, const vector4 *a, const vector4 *b)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    vec4_max_ptr_(res, a, b);
#else
    vec4_max_ptr_scalar(res, a, b);
#endif
}

/**
 * @brief Linear interpolation (dispatched).
 *
 * @see vec4_lerp_ptr_scalar
 *
 * @param res Result vector.
 * @param a First vector.
 * @param b Second vector.
 * @param t Interpolation factor.
 */
void vec4_lerp_ptr(vector4 *res, const vector4 *a, const vector4 *b, const vm_float_t t)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    vec4_lerp_ptr_(res, a, b, t);
#else
    vec4_lerp_ptr_scalar(res, a, b, t);
#endif
}

/**
 * @brief Component-wise clamp (dispatched).
 *
 * @see vec4_clamp_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 * @param min Lower bound.
 * @param max Upper bound.
 */
void vec4_clamp_ptr(vector4 *res, const vector4 *v, const vector4 *min, const vector4 *max)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    vec4_clamp_ptr_(res, v, min, max);
#else
    vec4_clamp_ptr_scalar(res, v, min, max);
#endif
}

/**
 * @brief Component-wise division (dispatched).
 *
 * @see vec4_div_ptr_scalar
 *
 * @param res Result vector.
 * @param a First vector.
 * @param b Second vector.
 */
void vec4_div_ptr(vector4 *res, const vector4 *a, const vector4 *b)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    vec4_div_ptr_(res, a, b);
#else
    vec4_div_ptr_scalar(res, a, b);
#endif
}

/**
 * @brief Adds a scalar to each component (dispatched).
 *
 * @see vec4_add_scalar_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 * @param s Scalar.
 */
void vec4_add_scalar_ptr(vector4 *res, const vector4 *v, const vm_float_t s)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    vec4_add_scalar_ptr_(res, v, s);
#else
    vec4_add_scalar_ptr_scalar(res, v, s);
#endif
}

/**
 * @brief Subtracts a scalar from each component (dispatched).
 *
 * @see vec4_sub_scalar_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 * @param s Scalar.
 */
void vec4_sub_scalar_ptr(vector4 *res, const vector4 *v, const vm_float_t s)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    vec4_sub_scalar_ptr_(res, v, s);
#else
    vec4_sub_scalar_ptr_scalar(res, v, s);
#endif
}

/**
 * @brief Clamps each component to a scalar range (dispatched).
 *
 * @see vec4_clamp_scalar_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 * @param min Lower bound.
 * @param max Upper bound.
 */
void vec4_clamp_scalar_ptr(vector4 *res, const vector4 *v, const vm_float_t min, const vm_float_t max)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    vec4_clamp_scalar_ptr_(res, v, min, max);
#else
    vec4_clamp_scalar_ptr_scalar(res, v, min, max);
#endif
}

/**
 * @brief Clamps each component to [0, 1] (dispatched).
 *
 * @see vec4_saturate_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 */
void vec4_saturate_ptr(vector4 *res, const vector4 *v)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    vec4_saturate_ptr_(res, v);
#else
    vec4_saturate_ptr_scalar(res, v);
#endif
}

/**
 * @brief Component-wise sign (dispatched).
 *
 * @see vec4_sign_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 */
void vec4_sign_ptr(vector4 *res, const vector4 *v)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    vec4_sign_ptr_(res, v);
#else
    vec4_sign_ptr_scalar(res, v);
#endif
}

/**
 * @brief Component-wise floor (dispatched).
 *
 * @see vec4_floor_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 */
void vec4_floor_ptr(vector4 *res, const vector4 *v)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    vec4_floor_ptr_(res, v);
#else
    vec4_floor_ptr_scalar(res, v);
#endif
}

/**
 * @brief Component-wise ceil (dispatched).
 *
 * @see vec4_ceil_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 */
void vec4_ceil_ptr(vector4 *res, const vector4 *v)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    vec4_ceil_ptr_(res, v);
#else
    vec4_ceil_ptr_scalar(res, v);
#endif
}

/**
 * @brief Component-wise round (dispatched).
 *
 * @see vec4_round_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 */
void vec4_round_ptr(vector4 *res, const vector4 *v)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    vec4_round_ptr_(res, v);
#else
    vec4_round_ptr_scalar(res, v);
#endif
}

/**
 * @brief Component-wise fractional part (dispatched).
 *
 * @see vec4_fract_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 */
void vec4_fract_ptr(vector4 *res, const vector4 *v)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    vec4_fract_ptr_(res, v);
#else
    vec4_fract_ptr_scalar(res, v);
#endif
}

/**
 * @brief Divides xyz by w (dispatched).
 *
 * @see vec4_homogenize_ptr_scalar
 *
 * @param res Result vector.
 * @param v Input vector.
 */
void vec4_homogenize_ptr(vector4 *res, const vector4 *v)
{
#ifdef VECMAT_RUNTIME_DISPATCH
    vm_cpu_init();
    vec4_homogenize_ptr_(res, v);
#else
    vec4_homogenize_ptr_scalar(res, v);
#endif
}
