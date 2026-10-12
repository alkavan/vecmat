// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <stddef.h>
#include <vecmat.h>

bool mat3_polar(matrix3 *R, matrix3 *U, const matrix3 *F)
{
    if (!R || !U || !F) {
        return false;
    }

    /* Reflection is a failure, not a silent flip. */
    const vm_float_t det_F = mat3_determinant(*F);
    if (det_F <= VM_F(0.0)) {
        return false;
    }

    /* C = Fᵀ F */
    const matrix3 Ft = mat3_transpose(*F);
    const matrix3 C = mat3_mul(Ft, *F);

    /* Newton: Y_{k+1} = 0.5 * (Y_k + C * Y_k^{-1}), start from C. */
    matrix3 Y = C;
    const int max_iter = 16;
    bool converged = false;
    for (int iter = 0; iter < max_iter; ++iter) {
        matrix3 Y_inv;
        if (!mat3_inverse_ok(&Y_inv, &Y)) {
            return false;
        }
        const matrix3 CY = mat3_mul(C, Y_inv);
        matrix3 Y_next;
        for (int i = 0; i < 9; ++i) {
            Y_next.v[i] = VM_F(0.5) * (Y.v[i] + CY.v[i]);
        }

        /* Convergence: relative change in Frobenius. */
        vm_float_t diff = VM_F(0.0);
        vm_float_t norm = VM_F(0.0);
        for (int i = 0; i < 9; ++i) {
            const vm_float_t d = Y_next.v[i] - Y.v[i];
            diff += d * d;
            norm += Y.v[i] * Y.v[i];
        }
        Y = Y_next;
        if (diff <= VECMAT_EPSILON * VECMAT_EPSILON * (VM_F(1.0) + norm)) {
            converged = true;
            break;
        }
    }
    if (!converged) {
        return false;
    }

    /* U = Y must be SPD: positive eigenvalues (via trace and det of 2x2 principals, or eigen). */
    vector3 ev = mat3_sym_eigen(Y, NULL);
    if (ev.x <= VM_F(0.0) || ev.y <= VM_F(0.0) || ev.z <= VM_F(0.0)) {
        return false;
    }

    *U = Y;

    /* R = F * U^{-1} */
    matrix3 U_inv;
    if (!mat3_inverse_ok(&U_inv, U)) {
        return false;
    }
    *R = mat3_mul(*F, U_inv);

    /* Proper rotation: det R > 0 and Rᵀ R ≈ I. */
    const vm_float_t detR = mat3_determinant(*R);
    if (detR <= VM_F(0.0)) {
        return false;
    }

    /* transpose of R times R */
    const matrix3 RtR = mat3_mul(mat3_transpose(*R), *R);
    const matrix3 I = mat3_identity();
    for (int i = 0; i < 9; ++i) {
        if (VECMAT_FABS(RtR.v[i] - I.v[i]) > VM_F(1e-4)) {
            return false;
        }
    }

    return true;
}
