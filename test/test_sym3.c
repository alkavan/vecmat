// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "unitest.h"

static matrix3 make_sym(const vm_float_t xx, const vm_float_t yy, const vm_float_t zz,
                        const vm_float_t xy, const vm_float_t yz, const vm_float_t zx)
{
    matrix3 m;
    for (int i = 0; i < 9; ++i) {
        m.v[i] = VM_F(0.0);
    }
    m.m11 = xx;
    m.m22 = yy;
    m.m33 = zz;
    m.m12 = m.m21 = xy;
    m.m23 = m.m32 = yz;
    m.m31 = m.m13 = zx;
    return m;
}

static vm_float_t frobenius(const matrix3 *a, const matrix3 *b)
{
    vm_float_t s = VM_F(0.0);
    for (int i = 0; i < 9; ++i) {
        s += a->v[i] * b->v[i];
    }
    return s;
}

TEST_CASE(sym3_roundtrip_symmetric, "[sym3]") {
    const matrix3 m = make_sym(VM_F(1.5), VM_F(-0.3), VM_F(2.1),
                               VM_F(0.4), VM_F(-0.7), VM_F(0.25));
    const vm_sym3 s = vm_sym3_from_mat3(&m);
    REQUIRE(VECMAT_EQ(s.xx, VM_F(1.5), EPSILON));
    REQUIRE(VECMAT_EQ(s.yy, VM_F(-0.3), EPSILON));
    REQUIRE(VECMAT_EQ(s.zz, VM_F(2.1), EPSILON));
    REQUIRE(VECMAT_EQ(s.xy, VM_F(0.4), EPSILON));
    REQUIRE(VECMAT_EQ(s.yz, VM_F(-0.7), EPSILON));
    REQUIRE(VECMAT_EQ(s.zx, VM_F(0.25), EPSILON));
    const matrix3 back = vm_sym3_to_mat3(&s);
    for (int i = 0; i < 9; ++i) {
        REQUIRE(VECMAT_EQ(back.v[i], m.v[i], EPSILON * VM_F(4.0)));
    }
}

TEST_CASE(sym3_deviator_trace_zero, "[sym3]") {
    const matrix3 m = make_sym(VM_F(3.0), VM_F(1.0), VM_F(2.0),
                               VM_F(0.5), VM_F(0.1), VM_F(-0.2));
    const vm_sym3 s = vm_sym3_from_mat3(&m);
    const vm_sym3 d = vm_sym3_deviator(&s);
    REQUIRE(VECMAT_EQ(vm_sym3_trace(&s), VM_F(6.0), EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(vm_sym3_hydrostatic(&s), VM_F(2.0), EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(vm_sym3_trace(&d), VM_F(0.0), EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(vm_sym3_hydrostatic(&d), VM_F(0.0), EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(d.xx, VM_F(1.0), EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(d.yy, VM_F(-1.0), EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(d.zz, VM_F(0.0), EPSILON * VM_F(8.0)));
    REQUIRE(VECMAT_EQ(d.xy, VM_F(0.5), EPSILON));
    REQUIRE(VECMAT_EQ(d.yz, VM_F(0.1), EPSILON));
    REQUIRE(VECMAT_EQ(d.zx, VM_F(-0.2), EPSILON));
}

TEST_CASE(sym3_ddot_matches_frobenius, "[sym3]") {
    const matrix3 a = make_sym(VM_F(1.0), VM_F(2.0), VM_F(3.0),
                               VM_F(0.5), VM_F(0.25), VM_F(-0.1));
    const matrix3 b = make_sym(VM_F(0.2), VM_F(-1.0), VM_F(1.5),
                               VM_F(0.3), VM_F(-0.4), VM_F(0.6));

    const vm_sym3 sa = vm_sym3_from_mat3(&a);
    const vm_sym3 sb = vm_sym3_from_mat3(&b);
    const vm_float_t vd = vm_sym3_ddot(&sa, &sb);
    const vm_float_t fd = frobenius(&a, &b);
    REQUIRE(VECMAT_EQ(vd, fd, EPSILON * VM_F(8.0)));
}

TEST_CASE(sym3_add_scale, "[sym3]") {
    const vm_sym3 a = {
        {
            .xx = VM_F(1.0), .yy = VM_F(2.0), .zz = VM_F(3.0),
            .xy = VM_F(0.1), .yz = VM_F(0.2), .zx = VM_F(0.3)
        }
    };
    const vm_sym3 b = {
        {
            .xx = VM_F(4.0), .yy = VM_F(5.0), .zz = VM_F(6.0),
            .xy = VM_F(0.4), .yz = VM_F(0.5), .zx = VM_F(0.6)
        }
    };

    const vm_sym3 sum = vm_sym3_add(&a, &b);
    REQUIRE(VECMAT_EQ(sum.xx, VM_F(5.0), EPSILON));
    REQUIRE(VECMAT_EQ(sum.yy, VM_F(7.0), EPSILON));
    REQUIRE(VECMAT_EQ(sum.zz, VM_F(9.0), EPSILON));
    REQUIRE(VECMAT_EQ(sum.xy, VM_F(0.5), EPSILON));
    REQUIRE(VECMAT_EQ(sum.yz, VM_F(0.7), EPSILON));
    REQUIRE(VECMAT_EQ(sum.zx, VM_F(0.9), EPSILON));

    const vm_sym3 sc = vm_sym3_scale(&a, VM_F(2.0));
    REQUIRE(VECMAT_EQ(sc.xx, VM_F(2.0), EPSILON));
    REQUIRE(VECMAT_EQ(sc.yy, VM_F(4.0), EPSILON));
    REQUIRE(VECMAT_EQ(sc.zz, VM_F(6.0), EPSILON));
    REQUIRE(VECMAT_EQ(sc.xy, VM_F(0.2), EPSILON));
    REQUIRE(VECMAT_EQ(sc.yz, VM_F(0.4), EPSILON));
    REQUIRE(VECMAT_EQ(sc.zx, VM_F(0.6), EPSILON));
}

TEST_CASE(sym3_invariants_known, "[sym3]") {
    /* Diagonal stress: eigenvalues 1,2,3. I1=6, deviator mean 0,
       s = diag(-1,0,1), s:s = 1+0+1 = 2, J2 = 1, J3 = det(s) = 0. */
    const vm_sym3 s = {
        {
            .xx = VM_F(1.0), .yy = VM_F(2.0), .zz = VM_F(3.0),
            .xy = VM_F(0.0), .yz = VM_F(0.0), .zx = VM_F(0.0)
        }
    };
    vm_float_t I1, J2, J3;
    vm_sym3_invariants(&s, &I1, &J2, &J3);
    REQUIRE(VECMAT_EQ(I1, VM_F(6.0), EPSILON));
    REQUIRE(VECMAT_EQ(J2, VM_F(1.0), EPSILON * VM_F(4.0)));
    REQUIRE(VECMAT_EQ(J3, VM_F(0.0), EPSILON * VM_F(4.0)));
}

TEST_CASE(sym3_push_forward_identity, "[sym3]") {
    const matrix3 F = mat3_identity();
    const vm_sym3 sigma = {
        {
            .xx = VM_F(1.0), .yy = VM_F(2.0), .zz = VM_F(3.0),
            .xy = VM_F(0.1), .yz = VM_F(0.2), .zx = VM_F(0.3)
        }
    };
    const vm_sym3 out = vm_sym3_push_forward(&F, &sigma);
    REQUIRE(VECMAT_EQ(out.xx, sigma.xx, EPSILON * VM_F(4.0)));
    REQUIRE(VECMAT_EQ(out.yy, sigma.yy, EPSILON * VM_F(4.0)));
    REQUIRE(VECMAT_EQ(out.zz, sigma.zz, EPSILON * VM_F(4.0)));
    REQUIRE(VECMAT_EQ(out.xy, sigma.xy, EPSILON * VM_F(4.0)));
    REQUIRE(VECMAT_EQ(out.yz, sigma.yz, EPSILON * VM_F(4.0)));
    REQUIRE(VECMAT_EQ(out.zx, sigma.zx, EPSILON * VM_F(4.0)));
}

TEST_CASE(sym3_push_forward_uniform_stretch, "[sym3]") {
    matrix3 F = mat3_identity();
    F.m11 = F.m22 = F.m33 = VM_F(2.0);
    const vm_sym3 sigma = {
        {
            .xx = VM_F(1.0), .yy = VM_F(2.0), .zz = VM_F(3.0),
            .xy = VM_F(0.1), .yz = VM_F(0.2), .zx = VM_F(0.3)
        }
    };
    const vm_sym3 out = vm_sym3_push_forward(&F, &sigma);
    REQUIRE(VECMAT_EQ(out.xx, VM_F(4.0), EPSILON * VM_F(4.0)));
    REQUIRE(VECMAT_EQ(out.yy, VM_F(8.0), EPSILON * VM_F(4.0)));
    REQUIRE(VECMAT_EQ(out.zz, VM_F(12.0), EPSILON * VM_F(4.0)));
    REQUIRE(VECMAT_EQ(out.xy, VM_F(0.4), EPSILON * VM_F(4.0)));
    REQUIRE(VECMAT_EQ(out.yz, VM_F(0.8), EPSILON * VM_F(4.0)));
    REQUIRE(VECMAT_EQ(out.zx, VM_F(1.2), EPSILON * VM_F(4.0)));
}

TEST_CASE(sym3_dw_split_pure_spin, "[sym3]") {
    /* Pure rotation rate: L = [[0, -w, 0], [w, 0, 0], [0, 0, 0]] */
    matrix3 L;
    for (int i = 0; i < 9; ++i) {
        L.v[i] = VM_F(0.0);
    }

    const vm_float_t w = VM_F(1.5);
    L.m12 = -w; /* m12 is row0 col1 */
    L.m21 = w;
    vm_sym3 D;
    matrix3 W;
    vm_sym3_dw_split(&L, &D, &W);

    REQUIRE(VECMAT_EQ(D.xx, VM_F(0.0), EPSILON * VM_F(4.0)));
    REQUIRE(VECMAT_EQ(D.yy, VM_F(0.0), EPSILON * VM_F(4.0)));
    REQUIRE(VECMAT_EQ(D.zz, VM_F(0.0), EPSILON * VM_F(4.0)));
    REQUIRE(VECMAT_EQ(D.xy, VM_F(0.0), EPSILON * VM_F(4.0)));
    REQUIRE(VECMAT_EQ(D.yz, VM_F(0.0), EPSILON * VM_F(4.0)));
    REQUIRE(VECMAT_EQ(D.zx, VM_F(0.0), EPSILON * VM_F(4.0)));

    REQUIRE(VECMAT_EQ(W.m12, -w, EPSILON));
    REQUIRE(VECMAT_EQ(W.m21, w, EPSILON));
    REQUIRE(VECMAT_EQ(W.m11, VM_F(0.0), EPSILON));
}

TEST_CASE(sym3_dw_split_pure_stretch, "[sym3]") {
    const matrix3 L = make_sym(VM_F(1.0), VM_F(2.0), VM_F(3.0),
                               VM_F(0.4), VM_F(-0.5), VM_F(0.6));
    vm_sym3 D;
    matrix3 W;
    vm_sym3_dw_split(&L, &D, &W);

    REQUIRE(VECMAT_EQ(D.xx, VM_F(1.0), EPSILON * VM_F(4.0)));
    REQUIRE(VECMAT_EQ(D.yy, VM_F(2.0), EPSILON * VM_F(4.0)));
    REQUIRE(VECMAT_EQ(D.zz, VM_F(3.0), EPSILON * VM_F(4.0)));
    REQUIRE(VECMAT_EQ(D.xy, VM_F(0.4), EPSILON * VM_F(4.0)));
    REQUIRE(VECMAT_EQ(D.yz, VM_F(-0.5), EPSILON * VM_F(4.0)));
    REQUIRE(VECMAT_EQ(D.zx, VM_F(0.6), EPSILON * VM_F(4.0)));
    for (int i = 0; i < 9; ++i) {
        REQUIRE(VECMAT_EQ(W.v[i], VM_F(0.0), EPSILON * VM_F(4.0)));
    }
}

TEST_CASE(sym3_null_inputs, "[sym3]") {
    const vm_sym3 z = vm_sym3_from_mat3(NULL);
    REQUIRE(VECMAT_EQ(z.xx, VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(z.yy, VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(z.zz, VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(z.xy, VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(z.yz, VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(z.zx, VM_F(0.0), EPSILON));

    const matrix3 zm = vm_sym3_to_mat3(NULL);
    for (int i = 0; i < 9; ++i) {
        REQUIRE(VECMAT_EQ(zm.v[i], VM_F(0.0), EPSILON));
    }

    const vm_sym3 addz = vm_sym3_add(NULL, &z);
    REQUIRE(VECMAT_EQ(addz.xx, VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(vm_sym3_scale(NULL, VM_F(3.0)).yy, VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(vm_sym3_trace(NULL), VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(vm_sym3_hydrostatic(NULL), VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(vm_sym3_deviator(NULL).zz, VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(vm_sym3_ddot(NULL, &z), VM_F(0.0), EPSILON));

    vm_float_t I1 = VM_F(1.0), J2 = VM_F(1.0), J3 = VM_F(1.0);
    vm_sym3_invariants(NULL, &I1, &J2, &J3);
    REQUIRE(VECMAT_EQ(I1, VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(J2, VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(J3, VM_F(0.0), EPSILON));
    vm_sym3_invariants(NULL, NULL, NULL, NULL);

    const vm_sym3 pf = vm_sym3_push_forward(NULL, &z);
    REQUIRE(VECMAT_EQ(pf.xx, VM_F(0.0), EPSILON));

    vm_sym3 D;
    matrix3 W;
    vm_sym3_dw_split(NULL, &D, &W);
    REQUIRE(VECMAT_EQ(D.xx, VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(D.xy, VM_F(0.0), EPSILON));
    for (int i = 0; i < 9; ++i) {
        REQUIRE(VECMAT_EQ(W.v[i], VM_F(0.0), EPSILON));
    }
    vm_sym3_dw_split(NULL, NULL, NULL);
}
