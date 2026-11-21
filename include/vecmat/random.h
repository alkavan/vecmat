// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_RANDOM_H
#define VECMAT_RANDOM_H

/**
 * @file
 * @brief GOE and GUE matrix generators.
 * @ingroup vecmat_extras
 *
 * `vm_rng_seed` stores a seed and advances splitmix64 once. Later draws are
 * classic Box–Muller pairs: two splitmix64 outputs, mapped to an exact
 * uniform in (0, 1), transformed in IEEE-754 double, then rounded to
 * `vm_float_t`. The same seed produces the same bits on every IEEE-754
 * platform. The caller owns the `vm_rng`.
 */

#include "vecmat/types.h"
#include "vecmat/abi.h"

/**
 * @brief Splitmix64 state for `vm_goe` and `vm_gue`.
 *
 * One counter. Call `vm_rng_seed` before drawing.
 */
typedef struct {
    uint64_t state; /**< Splitmix64 counter. */
} vm_rng;

/**
 * @brief Seed `rng` and advance splitmix64 once.
 *
 * The stored counter is `seed` plus the splitmix64 increment, so seed 0 is
 * a valid state. A null `rng` is ignored.
 *
 * @param rng Generator to seed.
 * @param seed Any value, including 0.
 */
VEC_API void vm_rng_seed(vm_rng *rng, uint64_t seed);

/**
 * @brief Overwrite `A` with a GOE matrix.
 *
 * `A` is square and `rows >= 1`. Diagonal entries are N(0, 2). Off-diagonal
 * entries are N(0, 1). Row `i` draws the diagonal first, then columns
 * `j > i`, and that one value is stored at both `(i, j)` and `(j, i)`.
 * A Box–Muller partner left over at the end of the call is discarded.
 *
 * A null `rng`, a null matrix, a null `data` pointer, a non-square, or
 * `rows < 1` returns false and does not write. `rng` is left unchanged.
 *
 * @param A Square column-major matrix. Overwritten on success.
 * @param rng Seeded generator.
 *
 * @return true on success.
 */
VEC_API bool vm_goe(const vm_mat *A, vm_rng *rng);

/**
 * @brief Overwrite `Ar` and `Ai` with the real and imaginary parts of a GUE matrix.
 *
 * Both matrices are square and have the same order, at least 1. Diagonal
 * real entries are N(0, 1). Diagonal imaginary entries are 0. Off-diagonal
 * real and imaginary entries are each N(0, 1/2). Row `i` draws the diagonal
 * real first, then for each `j > i` the real part and then the imaginary
 * part. The real value is stored at both `(i, j)` and `(j, i)`. The
 * imaginary value is stored at `(i, j)` and its negation at `(j, i)`.
 * A Box–Muller partner left over at the end of the call is discarded.
 *
 * A null `rng`, a null matrix, a null `data` pointer, a non-square,
 * `rows < 1`, or two different orders returns false and does not write.
 * `rng` is left unchanged.
 *
 * @param Ar Real part. Overwritten on success.
 * @param Ai Imaginary part. Overwritten on success.
 * @param rng Seeded generator.
 *
 * @return true on success.
 */
VEC_API bool vm_gue(const vm_mat *Ar, const vm_mat *Ai, vm_rng *rng);

#endif //VECMAT_RANDOM_H
