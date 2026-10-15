// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include "vecmat/sym3.h"
#include "vecmat/mat.h"

vm_sym3 vm_sym3_from_mat3(const matrix3 *m)
{
    vm_sym3 s;
    if (!m) {
        s.xx = s.yy = s.zz = s.xy = s.yz = s.zx = VM_F(0.0);
        return s;
    }
    s.xx = m->m11;
    s.yy = m->m22;
    s.zz = m->m33;
    s.xy = m->m12;
    s.yz = m->m23;
    s.zx = m->m31;
    return s;
}

matrix3 vm_sym3_to_mat3(const vm_sym3 *s)
{
    matrix3 m;
    for (int i = 0; i < 9; ++i) {
        m.v[i] = VM_F(0.0);
    }
    if (!s) {
        return m;
    }
    m.m11 = s->xx;
    m.m22 = s->yy;
    m.m33 = s->zz;
    m.m12 = m.m21 = s->xy;
    m.m23 = m.m32 = s->yz;
    m.m31 = m.m13 = s->zx;
    return m;
}

vm_sym3 vm_sym3_add(const vm_sym3 *a, const vm_sym3 *b)
{
    vm_sym3 r;
    if (!a || !b) {
        r.xx = r.yy = r.zz = r.xy = r.yz = r.zx = VM_F(0.0);
        return r;
    }
    r.xx = a->xx + b->xx;
    r.yy = a->yy + b->yy;
    r.zz = a->zz + b->zz;
    r.xy = a->xy + b->xy;
    r.yz = a->yz + b->yz;
    r.zx = a->zx + b->zx;
    return r;
}

vm_sym3 vm_sym3_scale(const vm_sym3 *a, const vm_float_t s)
{
    vm_sym3 r;
    if (!a) {
        r.xx = r.yy = r.zz = r.xy = r.yz = r.zx = VM_F(0.0);
        return r;
    }
    r.xx = a->xx * s;
    r.yy = a->yy * s;
    r.zz = a->zz * s;
    r.xy = a->xy * s;
    r.yz = a->yz * s;
    r.zx = a->zx * s;
    return r;
}

vm_float_t vm_sym3_trace(const vm_sym3 *a)
{
    if (!a) {
        return VM_F(0.0);
    }
    return a->xx + a->yy + a->zz;
}

vm_float_t vm_sym3_hydrostatic(const vm_sym3 *a)
{
    return vm_sym3_trace(a) / VM_F(3.0);
}

vm_sym3 vm_sym3_deviator(const vm_sym3 *a)
{
    vm_sym3 d = vm_sym3_scale(a, VM_F(1.0));
    if (!a) {
        return d;
    }
    const vm_float_t p = vm_sym3_hydrostatic(a);
    d.xx -= p;
    d.yy -= p;
    d.zz -= p;
    /* shears unchanged */
    return d;
}

vm_float_t vm_sym3_ddot(const vm_sym3 *a, const vm_sym3 *b)
{
    if (!a || !b) {
        return VM_F(0.0);
    }
    vm_float_t s = a->xx * b->xx + a->yy * b->yy + a->zz * b->zz;
    s += VM_F(2.0) * (a->xy * b->xy + a->yz * b->yz + a->zx * b->zx);
    return s;
}

void vm_sym3_invariants(const vm_sym3 *a,
                          vm_float_t *I1, vm_float_t *J2, vm_float_t *J3)
{
    if (I1) {
        *I1 = vm_sym3_trace(a);
    }
    if (!a) {
        if (J2) *J2 = VM_F(0.0);
        if (J3) *J3 = VM_F(0.0);
        return;
    }
    const vm_sym3 dev = vm_sym3_deviator(a);
    if (J2) {
        *J2 = VM_F(0.5) * vm_sym3_ddot(&dev, &dev);
    }
    if (J3) {
        const matrix3 dm = vm_sym3_to_mat3(&dev);
        *J3 = mat3_determinant(dm);
    }
}

vm_sym3 vm_sym3_push_forward(const matrix3 *F, const vm_sym3 *sigma)
{
    if (!F || !sigma) {
        vm_sym3 z;
        z.xx = z.yy = z.zz = z.xy = z.yz = z.zx = VM_F(0.0);
        return z;
    }
    const matrix3 S = vm_sym3_to_mat3(sigma);
    const matrix3 FS = mat3_mul(*F, S);
    const matrix3 Ft = mat3_transpose(*F);
    const matrix3 Sp = mat3_mul(FS, Ft);
    return vm_sym3_from_mat3(&Sp);
}

void vm_sym3_dw_split(const matrix3 *L, vm_sym3 *D, matrix3 *W)
{
    if (!L) {
        if (D) {
            D->xx = D->yy = D->zz = D->xy = D->yz = D->zx = VM_F(0.0);
        }
        if (W) {
            for (int i = 0; i < 9; ++i) {
                W->v[i] = VM_F(0.0);
            }
        }
        return;
    }

    const matrix3 Lt = mat3_transpose(*L);

    if (D) {
        matrix3 D_mat;
        for (int i = 0; i < 9; ++i) {
            D_mat.v[i] = VM_F(0.5) * (L->v[i] + Lt.v[i]);
        }
        *D = vm_sym3_from_mat3(&D_mat);
    }

    if (W) {
        for (int i = 0; i < 9; ++i) {
            W->v[i] = VM_F(0.5) * (L->v[i] - Lt.v[i]);
        }
    }
}
