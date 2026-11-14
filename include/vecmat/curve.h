// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_CURVE_H
#define VECMAT_CURVE_H

/**
 * @file
 * @brief One-segment quadratic and cubic curves. Not a path or an arc-length.
 * @ingroup vecmat_extras
 *
 * `t` is not clamped. `0` and `1` are the segment ends. Easing stays the
 * 1D remap; these functions evaluate a position.
 */

#include "vecmat/types.h"
#include "vecmat/abi.h"

/**
 * @brief Quadratic Bézier. `t = 0` is `p0`, `t = 1` is `p2`.
 */
VEC_API vm_float_t vm_bezier2(vm_float_t p0, vm_float_t p1, vm_float_t p2, vm_float_t t);

/**
 * @brief Cubic Bézier. `t = 0` is `p0`, `t = 1` is `p3`.
 */
VEC_API vm_float_t vm_bezier3(vm_float_t p0, vm_float_t p1, vm_float_t p2, vm_float_t p3,
                              vm_float_t t);

/**
 * @brief Cubic Hermite from endpoints `p0`, `p1` and tangents `m0`, `m1`.
 *
 * `t = 0` returns `p0` and `t = 1` returns `p1`.
 */
VEC_API vm_float_t vm_hermite(vm_float_t p0, vm_float_t p1, vm_float_t m0, vm_float_t m1,
                              vm_float_t t);

/**
 * @brief Uniform Catmull-Rom segment from `p1` to `p2`.
 *
 * `p0` and `p3` are the neighboring samples. `t = 0` returns `p1` and
 * `t = 1` returns `p2`.
 */
VEC_API vm_float_t vm_catmull_rom(vm_float_t p0, vm_float_t p1, vm_float_t p2, vm_float_t p3,
                                  vm_float_t t);

/** @brief Quadratic Bézier on `vector2`. Component-wise, same polynomial as `vm_bezier2`. */
VEC_API vector2 vec2_bezier2(vector2 p0, vector2 p1, vector2 p2, vm_float_t t);

/** @brief Cubic Bézier on `vector2`. */
VEC_API vector2 vec2_bezier3(vector2 p0, vector2 p1, vector2 p2, vector2 p3, vm_float_t t);

/** @brief Cubic Hermite on `vector2`. */
VEC_API vector2 vec2_hermite(vector2 p0, vector2 p1, vector2 m0, vector2 m1, vm_float_t t);

/** @brief Uniform Catmull-Rom on `vector2`. */
VEC_API vector2 vec2_catmull_rom(vector2 p0, vector2 p1, vector2 p2, vector2 p3, vm_float_t t);

/** @brief Quadratic Bézier on `vector3`. */
VEC_API vector3 vec3_bezier2(vector3 p0, vector3 p1, vector3 p2, vm_float_t t);

/** @brief Cubic Bézier on `vector3`. */
VEC_API vector3 vec3_bezier3(vector3 p0, vector3 p1, vector3 p2, vector3 p3, vm_float_t t);

/** @brief Cubic Hermite on `vector3`. */
VEC_API vector3 vec3_hermite(vector3 p0, vector3 p1, vector3 m0, vector3 m1, vm_float_t t);

/** @brief Uniform Catmull-Rom on `vector3`. */
VEC_API vector3 vec3_catmull_rom(vector3 p0, vector3 p1, vector3 p2, vector3 p3, vm_float_t t);

#endif //VECMAT_CURVE_H
