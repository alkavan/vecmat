// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_SYM3_H
#define VECMAT_SYM3_H

/**
 * @file
 * @brief Voigt-6 algebra for symmetric 3x3 tensors.
 * @ingroup vecmat_extras
 *
 * Order is (xx, yy, zz, xy, yz, zx). Shear components are the tensor
 * components (not engineering shear). Double contraction therefore
 * includes a factor of 2 on the shear products and equals the matrix
 * Frobenius inner product.
 */

#include "vecmat/types.h"
#include "vecmat/abi.h"

/**
 * @brief Symmetric 3×3 tensor in Voigt order.
 *
 * `c[0]=xx, c[1]=yy, c[2]=zz, c[3]=xy, c[4]=yz, c[5]=zx`.
 */
typedef struct {
    union {
        struct {
            vm_float_t xx;  /**< XX component. */
            vm_float_t yy;  /**< YY component. */
            vm_float_t zz;  /**< ZZ component. */
            vm_float_t xy;  /**< XY component. */
            vm_float_t yz;  /**< YZ component. */
            vm_float_t zx;  /**< ZX component. */
        };
        vm_float_t c[6];
    };
} vm_sym3;

/**
 * @brief Pack a symmetric matrix3 into Voigt-6.
 *
 * Off-diagonal tensor components are taken from the upper triangle
 * (m12, m23, m31). Non-symmetric input is accepted; only the upper
 * triangle is used for shears.
 *
 * @param m Input matrix3.
 * @return Voigt representation.
 */
VEC_API vm_sym3 vm_sym3_from_mat3(const matrix3 *m);

/**
 * @brief Unpack Voigt-6 into a symmetric matrix3.
 *
 * @param s Input Voigt tensor.
 * @return Symmetric matrix3.
 */
VEC_API matrix3 vm_sym3_to_mat3(const vm_sym3 *s);

/**
 * @brief Component-wise sum.
 */
VEC_API vm_sym3 vm_sym3_add(const vm_sym3 *a, const vm_sym3 *b);

/**
 * @brief Component-wise scale.
 */
VEC_API vm_sym3 vm_sym3_scale(const vm_sym3 *a, vm_float_t s);

/**
 * @brief Deviatoric part: s = σ − (tr σ / 3) I.
 */
VEC_API vm_sym3 vm_sym3_deviator(const vm_sym3 *a);

/**
 * @brief Hydrostatic (mean) scalar: tr(σ) / 3.
 */
VEC_API vm_float_t vm_sym3_hydrostatic(const vm_sym3 *a);

/**
 * @brief Trace: xx + yy + zz.
 */
VEC_API vm_float_t vm_sym3_trace(const vm_sym3 *a);

/**
 * @brief Principal invariants of the stress (or strain) tensor.
 *
 * I1 = tr(σ).
 * J2 = (1/2) s : s where s is the deviator (von Mises related).
 * J3 = det(s).
 * Any of the output pointers may be NULL.
 *
 * @param a Input tensor.
 * @param I1 Optional first invariant.
 * @param J2 Optional second deviatoric invariant.
 * @param J3 Optional third deviatoric invariant.
 */
VEC_API void vm_sym3_invariants(const vm_sym3 *a,
                                  vm_float_t *I1, vm_float_t *J2, vm_float_t *J3);

/**
 * @brief Push-forward σ' = F σ Fᵀ, result in Voigt.
 *
 * @param F Deformation gradient (or any matrix3).
 * @param sigma Input Cauchy (or other) stress in Voigt.
 * @return Pushed-forward tensor in Voigt.
 */
VEC_API vm_sym3 vm_sym3_push_forward(const matrix3 *F, const vm_sym3 *sigma);

/**
 * @brief Symmetric / skew split of a velocity gradient.
 *
 * D = (L + Lᵀ)/2 packed as Voigt; W = (L − Lᵀ)/2 as matrix3.
 * Either output pointer may be NULL.
 *
 * @param L Velocity gradient (matrix3).
 * @param D Optional symmetric rate-of-deformation (Voigt).
 * @param W Optional skew spin tensor (matrix3).
 */
VEC_API void vm_sym3_dw_split(const matrix3 *L, vm_sym3 *D, matrix3 *W);

/**
 * @brief Double contraction A : B.
 *
 * Equals the Frobenius inner product of the corresponding matrices.
 * For tensor-shear Voigt storage this is
 * xx_a*xx_b + yy_a*yy_b + zz_a*zz_b + 2*(xy_a*xy_b + yz_a*yz_b + zx_a*zx_b).
 *
 * @return A : B.
 */
VEC_API vm_float_t vm_sym3_ddot(const vm_sym3 *a, const vm_sym3 *b);

#endif //VECMAT_SYM3_H
