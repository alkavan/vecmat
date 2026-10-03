# Vector usage

Vectors are value types. Constructors, named fields, and `.v[]` are the same
object. The integer twins (`vector2i`, `vector3i`, `vector4i`) follow the same
shape.

```c
vector3 p     = vec3(1.0f, 2.0f, 3.0f);
vector2 q     = vec2(4.0f, 5.0f);
vector3i grid = vec3i(8, 16, 24);

vector3 origin = vec3_zero();
vector3 ones   = vec3_one();
vector3 fill   = vec3_splat(0.5f);

vector3 named = { .x = 1.0f, .y = 0.0f, .z = 0.0f };
vector4 homog = { .v = {1.0f, 2.0f, 3.0f, 1.0f} };

vec3_assign_xyz(&p, 0.0f, 1.0f, 0.0f);
vector3 lifted = vec3_from_vec2(q, 0.0f);
```

`vec2` / `vec4` and the integer types have the same `zero` / `one` / `splat`
helpers.

## Graphics

A slide along a surface drops the part of the motion that faces the normal.
`vec3_reflect` is the mirror. Both expect a unit normal.

```c
const vector3 wish = vec3(1.0f, -0.2f, 0.0f);
const vector3 n = vec3_normalize(vec3(0.0f, 1.0f, 0.0f));
const vector3 along = vec3_slide(wish, n);
const vector3 bounce = vec3_reflect(wish, n);
```

A heading in the ground plane is an angle, not a matrix:

```c
const vector2 dir = vec2_from_angle(VM_DEG(45.0));
const vm_float_t heading = vec2_heading(dir);
```

## Simulation

A velocity step is a scaled add. The library step functions take the same
layout as a length-`n` array of components.

```c
vector3 x = vec3(0.0f, 1.0f, 0.0f);
vector3 v = vec3(1.0f, 0.0f, 0.0f);
const vector3 a = vec3(0.0f, -9.81f, 0.0f);
const float dt = 1.0f / 60.0f;

v = vec3_add(v, vec3_mul_scalar(a, dt));
x = vec3_add(x, vec3_mul_scalar(v, dt));
```

The same update through the integrator, for a solver that already stores
components in arrays:

```c
vm_float_t xs[3] = {x.x, x.y, x.z};
vm_float_t vs[3] = {v.x, v.y, v.z};
const vm_float_t as[3] = {a.x, a.y, a.z};
vm_euler_semi(xs, vs, as, 3, dt);
```

## Integer grids

`vector2i` / `vector3i` are the cell coordinates. `wrap` folds a neighbor
back into the grid.

```c
const vector2i cell = vec2i(3, 5);
const vector2i step = vec2i(1, 0);
const vector2i next = vec2i_add(cell, step);
const vector2i wrapped = vec2i_wrap(next, vec2i(8, 8));
```

## What the hand-written form is doing

A linear map is a matrix-vector product. Prefer `mat3_mul_vec3` unless the
point of the code is to show the layout.

```c
void transform(vector3 *out, const matrix3 *mat, const vector3 *vec)
{
    out->x = mat->m11 * vec->x + mat->m12 * vec->y + mat->m13 * vec->z;
    out->y = mat->m21 * vec->x + mat->m22 * vec->y + mat->m23 * vec->z;
    out->z = mat->m31 * vec->x + mat->m32 * vec->y + mat->m33 * vec->z;
}
```

A translation is an add:

```c
void translate(vector3 *out, const vector3 *vec, const vector3 *translation)
{
    out->x = vec->x + translation->x;
    out->y = vec->y + translation->y;
    out->z = vec->z + translation->z;
}
```
