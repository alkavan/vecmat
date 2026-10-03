# Matrix usage

Matrices are column-major. `m11` is column 0, row 0. `.v[]` is that same
order. Products are `AB`, so `(AB)v == A(Bv)` with the column vector on the
right.

```c
matrix3 ident = {
    .m11 = 1.0f, .m21 = 0.0f, .m31 = 0.0f,
    .m12 = 0.0f, .m22 = 1.0f, .m32 = 0.0f,
    .m13 = 0.0f, .m23 = 0.0f, .m33 = 1.0f
};

matrix3 also = { .v = {1, 0, 0,  0, 1, 0,  0, 0, 1} };

for (int i = 0; i < 9; i++)
    also.v[i] *= 2.0f;
```

The named form of a 3×3 determinant matches the column-major fields:

```c
float determinant(const matrix3 *mat)
{
    return mat->m11 * (mat->m22 * mat->m33 - mat->m23 * mat->m32)
         - mat->m12 * (mat->m21 * mat->m33 - mat->m23 * mat->m31)
         + mat->m13 * (mat->m21 * mat->m32 - mat->m22 * mat->m31);
}
```

`mat3_mul` is that product. The loop is the same layout, written out:

```c
void multiply(matrix3 *result, const matrix3 *a, const matrix3 *b)
{
    for (int c = 0; c < 3; c++) {      /* columns of result / of B */
        for (int r = 0; r < 3; r++) {  /* rows of result / of A */
            float sum = 0.0f;
            for (int k = 0; k < 3; k++)
                sum += a->v[k * 3 + r] * b->v[c * 3 + k];
            result->v[c * 3 + r] = sum;
        }
    }
}
```

## Affine model matrix

A 4×4 that applies a 3×3 and then a translation is `[A t; 0 1]`.
`mat4_from_mat3` copies `A` and leaves the translation at zero. The fourth
column is `t`.

```c
void affine_matrix(matrix4 *out, const matrix3 *linear, const vector3 *translation)
{
    *out = mat4_from_mat3(*linear);
    out->m14 = translation->x;
    out->m24 = translation->y;
    out->m34 = translation->z;
    out->m44 = 1.0f;
}
```

The same matrix from translation, rotation, and scale, which is the usual
model matrix:

```c
const quaternion rot = quat_from_euler_deg(vec3(0.0f, 90.0f, 0.0f));
const matrix4 model = mat4_trs(vec3(1.0f, 0.0f, 2.0f), rot, vec3(2.0f, 2.0f, 2.0f));
const matrix4 inverse = mat4_inverse_affine(model);
const matrix4 normal = mat4_normal(model);
```

`mat4_inverse_affine` is the inverse of an `[A t; 0 1]` transform.
`mat4_normal` is the inverse-transpose of the 3×3, for normals.

## Camera

View, then projection. `mat4_mul(proj, view)` applies the view first.
Clip helpers select handedness and depth range; the `*_rh` / `*_zo` names
are the fixed ones.

```c
const matrix4 view = mat4_look_at_rh(vec3(0.0f, 2.0f, 6.0f),
                                     vec3_zero(),
                                     vec3(0.0f, 1.0f, 0.0f));
const matrix4 proj = mat4_perspective_rh_no(VM_DEG(70.0), 1.777f, 0.05f, 500.0f);
const matrix4 clip_from_world = mat4_mul(proj, view);
const vector4 clip = mat4_mul_vec4(clip_from_world, vec4(0.0f, 1.0f, 0.0f, 1.0f));
```

A fly camera has a direction instead of a target:

```c
const vector3 forward = vec3_normalize(vec3(0.0f, 0.0f, -1.0f));
const matrix4 fly = mat4_look_from_dir_rh(vec3(0.0f, 1.6f, 0.0f),
                                          forward,
                                          vec3(0.0f, 1.0f, 0.0f));
```
