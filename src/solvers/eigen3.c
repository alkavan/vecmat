// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

/**
 * @brief Scale-aware singularity cutoff for a 3×3.
 */
static vm_float_t mat3_sing_tol(const matrix3 *m)
{
    vm_float_t scale = VECMAT_FABS(m->m11);
    scale = VECMAT_FMAX(scale, VECMAT_FABS(m->m12));
    scale = VECMAT_FMAX(scale, VECMAT_FABS(m->m13));
    scale = VECMAT_FMAX(scale, VECMAT_FABS(m->m21));
    scale = VECMAT_FMAX(scale, VECMAT_FABS(m->m22));
    scale = VECMAT_FMAX(scale, VECMAT_FABS(m->m23));
    scale = VECMAT_FMAX(scale, VECMAT_FABS(m->m31));
    scale = VECMAT_FMAX(scale, VECMAT_FABS(m->m32));
    scale = VECMAT_FMAX(scale, VECMAT_FABS(m->m33));
    if (scale <= VM_F(0.0)) {
        return VECMAT_EPSILON;
    }
    /* det scales as scale^3 */
    return VECMAT_EPSILON * VM_F(3.0) * scale * scale * scale;
}

bool mat3_inverse_ok(matrix3 *res, const matrix3 *m)
{
    if (!res || !m) {
        return false;
    }

    /* Same cofactor expansion as mat3_inverse_ptr (column-major). */
    const vm_float_t det = m->v[0] * (m->v[4] * m->v[8] - m->v[5] * m->v[7]) -
                           m->v[1] * (m->v[3] * m->v[8] - m->v[5] * m->v[6]) +
                           m->v[2] * (m->v[3] * m->v[7] - m->v[4] * m->v[6]);

    if (VECMAT_FABS(det) <= mat3_sing_tol(m)) {
        return false;
    }

    const vm_float_t inv_det = VM_F(1.0) / det;
    matrix3 inv;
    inv.v[0] = (m->v[4] * m->v[8] - m->v[5] * m->v[7]) * inv_det;
    inv.v[1] = (m->v[2] * m->v[7] - m->v[1] * m->v[8]) * inv_det;
    inv.v[2] = (m->v[1] * m->v[5] - m->v[2] * m->v[4]) * inv_det;
    inv.v[3] = (m->v[5] * m->v[6] - m->v[3] * m->v[8]) * inv_det;
    inv.v[4] = (m->v[0] * m->v[8] - m->v[2] * m->v[6]) * inv_det;
    inv.v[5] = (m->v[2] * m->v[3] - m->v[0] * m->v[5]) * inv_det;
    inv.v[6] = (m->v[3] * m->v[7] - m->v[4] * m->v[6]) * inv_det;
    inv.v[7] = (m->v[1] * m->v[6] - m->v[0] * m->v[7]) * inv_det;
    inv.v[8] = (m->v[0] * m->v[4] - m->v[1] * m->v[3]) * inv_det;
    *res = inv;
    return true;
}

static void sort3_asc(vm_float_t *a, vm_float_t *b, vm_float_t *c)
{
    if (*a > *b) {
        const vm_float_t t = *a; *a = *b; *b = t;
    }
    if (*b > *c) {
        const vm_float_t t = *b; *b = *c; *c = t;
    }
    if (*a > *b) {
        const vm_float_t t = *a; *a = *b; *b = t;
    }
}

/**
 * @brief Null-space vector of a near-singular 3×3 via largest cross product
 * of two columns. Writes a unit vector into `v` (column-major layout).
 */
static void null_vec(vector3 *v, const matrix3 *s)
{
    const vector3 c0 = { .x = s->m11, .y = s->m21, .z = s->m31 };
    const vector3 c1 = { .x = s->m12, .y = s->m22, .z = s->m32 };
    const vector3 c2 = { .x = s->m13, .y = s->m23, .z = s->m33 };
    const vector3 n0 = vec3_cross(c1, c2);
    const vector3 n1 = vec3_cross(c2, c0);
    const vector3 n2 = vec3_cross(c0, c1);

    const vm_float_t l0 = vec3_length_squared(n0);
    const vm_float_t l1 = vec3_length_squared(n1);
    const vm_float_t l2 = vec3_length_squared(n2);

    if (l0 >= l1 && l0 >= l2) {
        *v = n0;
    } else if (l1 >= l2) {
        *v = n1;
    } else {
        *v = n2;
    }

    const vm_float_t len = vec3_length(*v);
    if (len > VM_F(0.0)) {
        *v = vec3_div_scalar(*v, len);
    } else {
        *v = (vector3){ .x = VM_F(1.0), .y = VM_F(0.0), .z = VM_F(0.0) };
    }
}

void mat3_sym_eigen_ptr(vector3 *eigenvalues, matrix3 *axes, const matrix3 *m)
{
    if (!eigenvalues || !m) {
        return;
    }

    /* Symmetrize. */
    const vm_float_t a = m->m11;
    const vm_float_t b = m->m22;
    const vm_float_t c = m->m33;
    const vm_float_t d = VM_F(0.5) * (m->m12 + m->m21);
    const vm_float_t e = VM_F(0.5) * (m->m13 + m->m31);
    const vm_float_t f = VM_F(0.5) * (m->m23 + m->m32);

    const vm_float_t p1 = d * d + e * e + f * f;
    vm_float_t scale = VECMAT_FABS(a);
    scale = VECMAT_FMAX(scale, VECMAT_FABS(b));
    scale = VECMAT_FMAX(scale, VECMAT_FABS(c));
    scale = VECMAT_FMAX(scale, VECMAT_FABS(d));
    scale = VECMAT_FMAX(scale, VECMAT_FABS(e));
    scale = VECMAT_FMAX(scale, VECMAT_FABS(f));
    const vm_float_t tol = (scale > VM_F(0.0))
        ? VECMAT_EPSILON * VM_F(3.0) * scale * scale
        : VECMAT_EPSILON;

    vm_float_t l0, l1, l2;

    if (p1 <= tol) {
        l0 = a;
        l1 = b;
        l2 = c;
    } else {
        const vm_float_t q = (a + b + c) / VM_F(3.0);
        const vm_float_t p2 =
            (a - q) * (a - q) + (b - q) * (b - q) + (c - q) * (c - q) +
            VM_F(2.0) * p1;
        const vm_float_t p = VECMAT_SQRT(p2 / VM_F(6.0));
        if (p <= tol) {
            l0 = l1 = l2 = q;
        } else {
            const vm_float_t ba = (a - q) / p;
            const vm_float_t bb = (b - q) / p;
            const vm_float_t bc = (c - q) / p;
            const vm_float_t bd = d / p;
            const vm_float_t be = e / p;
            const vm_float_t bf = f / p;
            const vm_float_t detB =
                ba * (bb * bc - bf * bf) -
                bd * (bd * bc - bf * be) +
                be * (bd * bf - bb * be);
            vm_float_t r = detB / VM_F(2.0);

            if (r < VM_F(-1.0)) {
                r = VM_F(-1.0);
            } else if (r > VM_F(1.0)) {
                r = VM_F(1.0);
            }

            const vm_float_t phi = VECMAT_ACOS(r) / VM_F(3.0);
            const vm_float_t two_p = VM_F(2.0) * p;
            l0 = q + two_p * VECMAT_COS(phi);
            l1 = q + two_p * VECMAT_COS(phi + VM_F(2.0) * M_PI / VM_F(3.0));
            l2 = q + two_p * VECMAT_COS(phi + VM_F(4.0) * M_PI / VM_F(3.0));
        }
    }

    sort3_asc(&l0, &l1, &l2);
    eigenvalues->x = l0;
    eigenvalues->y = l1;
    eigenvalues->z = l2;

    if (!axes) {
        return;
    }

    /* Eigenvectors via null space of (A - λI). */
    matrix3 S;
    vector3 v0, v1, v2;
    S.m11 = a - l0; S.m21 = d;     S.m31 = e;
    S.m12 = d;      S.m22 = b - l0; S.m32 = f;
    S.m13 = e;      S.m23 = f;      S.m33 = c - l0;
    null_vec(&v0, &S);

    S.m11 = a - l1; S.m22 = b - l1; S.m33 = c - l1;
    null_vec(&v1, &S);

    S.m11 = a - l2; S.m22 = b - l2; S.m33 = c - l2;
    null_vec(&v2, &S);

    /* Orthogonalize against previous (Gram-Schmidt) for near-multiplicity. */
    v1 = vec3_sub(v1, vec3_mul_scalar(v0, vec3_dot(v1, v0)));
    if (vec3_length_squared(v1) > tol) {
        v1 = vec3_normalize(v1);
    } else {
        /* Pick a perpendicular. */
        vector3 ax = { .x = VM_F(1.0), .y = VM_F(0.0), .z = VM_F(0.0) };
        if (VECMAT_FABS(vec3_dot(v0, ax)) > VM_F(0.9)) {
            ax = (vector3){ .x = VM_F(0.0), .y = VM_F(1.0), .z = VM_F(0.0) };
        }
        v1 = vec3_normalize(vec3_cross(v0, ax));
    }
    v2 = vec3_cross(v0, v1);
    if (vec3_length_squared(v2) > tol) {
        v2 = vec3_normalize(v2);
    }

    /* Columns. */
    axes->m11 = v0.x; axes->m21 = v0.y; axes->m31 = v0.z;
    axes->m12 = v1.x; axes->m22 = v1.y; axes->m32 = v1.z;
    axes->m13 = v2.x; axes->m23 = v2.y; axes->m33 = v2.z;
}
