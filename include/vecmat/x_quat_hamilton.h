// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_X_QUAT_HAMILTON_H
#define VECMAT_X_QUAT_HAMILTON_H

/**
 * @file
 * @brief Experimental Hamilton-product matrix for a fixed quaternion.
 * @ingroup vecmat_experimental
 *
 * Left-multiplication by a quaternion is the Hamilton product, and that
 * product is a 4×4 matrix times the other quaternion. The matrix printed
 * in `(w, x, y, z)` order is
 *
 *     | w  -x  -y  -z |
 *     | x   w  -z   y |
 *     | y   z   w  -x |
 *     | z  -y   x   w |
 *
 * Vecmat packs a quaternion as `(x, y, z, w)` and stores a matrix
 * column-major. `vm_x_quat_hamilton_mat` writes that same matrix in this
 * layout. Columns, top to bottom `(x, y, z, w)`:
 *
 * - x: `( w,  z, -y, -x)`
 * - y: `(-z,  w,  x, -y)`
 * - z: `( y, -x,  w, -z)`
 * - w: `( x,  y,  z,  w)`
 *
 * Applying it is `mat4_mul_vec4_ptr`. No new ukernel, and no SH4 assembly.
 * A hardware matrix register that must be stored back through another
 * product does not exist here: the mat-vec result is already four
 * components, so there is no extra multiply by the identity quaternion.
 *
 * `quat_mul` is unchanged. `quat_to_mat4` is the 3×3 rotation embedding,
 * not this matrix.
 *
 * These sources are compiled only with CMake `-DVECMAT_EXPERIMENTAL=ON`,
 * which is off by default. That option builds the experimental sources and
 * defines `VECMAT_EXPERIMENTAL`, so `<vecmat.h>` declares these names.
 * This header can also be included on its own as `<vecmat/x_quat_hamilton.h>`.
 * The names may change.
 */

#include "vecmat/quat.h"
#include "vecmat/mat.h"

#define vm_x_quat_hamilton_mat VECMAT_SYMBOL(vm_x_quat_hamilton_mat)
#define vm_x_quat_hamilton_mul_array VECMAT_SYMBOL(vm_x_quat_hamilton_mul_array)
#define vm_x_quat_hamilton_mul_array_right VECMAT_SYMBOL(vm_x_quat_hamilton_mul_array_right)
#define vm_x_quat_hamilton_mul_chain VECMAT_SYMBOL(vm_x_quat_hamilton_mul_chain)

/**
 * @brief Write the left-multiplication Hamilton matrix of `q`.
 * @ingroup vecmat_experimental
 *
 * `M(q) * v = q * v`, with `v` packed `(x, y, z, w)`. A null `m` or `q`
 * writes nothing.
 *
 * @param m Destination matrix.
 * @param q Left quaternion.
 */
VEC_API void vm_x_quat_hamilton_mat(matrix4 *m, const quaternion *q);

/**
 * @brief `out[i] = fixed * in[i]`.
 * @ingroup vecmat_experimental
 *
 * Builds the left Hamilton matrix once, then calls `mat4_mul_vec4_ptr` for
 * each element. `n <= 0` is a no-op and reads nothing; null pointers are
 * then allowed. For `n > 0` every pointer must be non-null. `out` may
 * exactly alias `in`. Any other overlap is caller error.
 *
 * @param out   Results; length `n`. May exactly alias `in`.
 * @param fixed Left quaternion.
 * @param in    Right quaternions; length `n`.
 * @param n     Count. `n <= 0` is a no-op.
 */
VEC_API void vm_x_quat_hamilton_mul_array(quaternion *out, const quaternion *fixed,
                                          const quaternion *in, int n);

/**
 * @brief `out[i] = in[i] * fixed`.
 * @ingroup vecmat_experimental
 *
 * Same contract as `vm_x_quat_hamilton_mul_array`, using the
 * right-multiplication matrix of `fixed`.
 *
 * @param out   Results; length `n`. May exactly alias `in`.
 * @param fixed Right quaternion.
 * @param in    Left quaternions; length `n`.
 * @param n     Count. `n <= 0` is a no-op.
 */
VEC_API void vm_x_quat_hamilton_mul_array_right(quaternion *out, const quaternion *fixed,
                                                const quaternion *in, int n);

/**
 * @brief `acc = ((acc * qs[0]) * qs[1]) * ... * qs[n - 1]`.
 * @ingroup vecmat_experimental
 *
 * Each step is the right-multiplication matrix of `qs[i]` applied to
 * `acc`. The caller supplies the initial accumulator. `n <= 0` is a no-op
 * and reads nothing. For `n > 0`, `acc` and `qs` must be non-null. `acc`
 * must not point into `qs`.
 *
 * A chain does not reuse one matrix. `quat_mul` remains the single-product
 * function.
 *
 * @param acc Accumulator, updated in place.
 * @param qs  Factors, left to right; length `n`.
 * @param n   Count. `n <= 0` is a no-op.
 */
VEC_API void vm_x_quat_hamilton_mul_chain(quaternion *acc, const quaternion *qs, int n);

#endif //VECMAT_X_QUAT_HAMILTON_H
