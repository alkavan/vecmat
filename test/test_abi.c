// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "unitest.h"

TEST_CASE(abi_float_bits_contain_this_tu, "[abi]") {
    const unsigned bits = vm_compiled_float_bits();
    REQUIRE((bits & (unsigned)VECMAT_FLOAT_BITS) == (unsigned)VECMAT_FLOAT_BITS);
    REQUIRE(vm_abi_mismatch() == 0);
#if defined(VECMAT_HAVE_ABI_32)
    REQUIRE((bits & 32u) == 32u);
#endif
#if defined(VECMAT_HAVE_ABI_64)
    REQUIRE((bits & 64u) == 64u);
#endif
}

static int vecmat_symbol_probe32(void) { return 32; }
static int vecmat_symbol_probe64(void) { return 64; }

typedef char vecmat_symbol_width32[32];
typedef char vecmat_symbol_width64[64];

#define vecmat_symbol_probe VECMAT_SYMBOL(vecmat_symbol_probe)
#define vecmat_symbol_width VECMAT_SYMBOL(vecmat_symbol_width)

_Static_assert(sizeof(vecmat_symbol_width) == VECMAT_ABI_SUFFIX,
               "VECMAT_SYMBOL pasted the wrong suffix");

TEST_CASE(abi_symbol_macro_pastes_suffix, "[abi]") {
    REQUIRE(vecmat_symbol_probe() == VECMAT_ABI_SUFFIX);
}

TEST_CASE(abi_suffixed_add_matches_unsuffixed, "[abi]") {
    const vector2 a = {.x = VM_F(1.0), .y = VM_F(2.0)};
    const vector2 b = {.x = VM_F(3.0), .y = VM_F(4.0)};
    const vector2 r = vec2_add(a, b);
#if VECMAT_FLOAT_BITS == 64
    const vector2 s = vec2_add64(a, b);
#else
    const vector2 s = vec2_add32(a, b);
#endif
    REQUIRE(VECMAT_EQ(r.x, s.x, EPSILON));
    REQUIRE(VECMAT_EQ(r.y, s.y, EPSILON));
    REQUIRE(VECMAT_EQ(r.x, VM_F(4.0), EPSILON));
}
