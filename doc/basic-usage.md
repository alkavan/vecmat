# Basic usage

One header covers the library. A translation unit can also include a single
module (`vecmat/vec.h`, `vecmat/mat.h`, `vecmat/quat.h`).

```c
#include <vecmat.h>
```

Values are plain C structs, passed and returned by copy. Components are named
fields (`.x` / `.y` / `.z` / `.w`, or `m11`, `m21`, …) and a flat `.v[]`.
Layout is column-major. Angles on unsuffixed APIs are radians; `VM_DEG(90)`
converts a degree literal.

```c
vector3 p = vec3(1.0f, 2.0f, 3.0f);
p.v[1] = 0.0f;          /* same as p.y */

matrix3 mat = mat3_identity();
mat.v[0] = 1.0f;        /* same as mat.m11 */
```

`vector4` and `matrix4` are the translation unit's float width. Define
`VECMAT_USE_F64` before the include to take the 64-bit symbols. Integer
vectors (`vector2i`, `vector3i`, `vector4i`) use `vm_int_t`, chosen when the
library is built.

Hot loops use the `_ptr` form. The by-value helpers call those.

```c
vector3 a = vec3(1.0f, 0.0f, 0.0f);
vector3 b = vec3(0.0f, 1.0f, 0.0f);
vector3 c = vec3_add(a, b);

vector3 out;
vec3_add_ptr(&out, &a, &b);
```

## Putting the pieces together

A renderer, a solver, and a particle step are the same types. The pages below
show each layer on its own. This is the usual split:

- Frame data stays in `vector3` / `matrix4` / `quaternion`. See
  [Vector usage](vector-usage.md) and [Matrix usage](matrix-usage.md).
- A camera is `mat4_look_at_*` times `mat4_perspective_*`. A model matrix is
  `mat4_trs`, or a 3×3 packed into a 4×4 with `mat4_from_mat3`.
- A batch of samples is `vm_gemm` or a heap `vm_mat`. See
  [Numerics extras](numerics.md).
- A particle step is `vm_euler_semi` or `vm_verlet` on length-`n` arrays.

An affine model matrix from a rotation and a translation, without writing the
16 stores by hand:

```c
const quaternion spin = quat_from_axis_angle(vec3(0.0f, 1.0f, 0.0f), VM_DEG(90.0));
const matrix4 model = mat4_trs(vec3(2.0f, 0.0f, 0.0f), spin, vec3_one());
const vector3 world = mat4_mul_vec3(model, vec3(1.0f, 0.0f, 0.0f), 1.0f);
```

The same packing, if the linear part is already a `matrix3`:

```c
void affine_matrix(matrix4 *out, const matrix3 *linear, const vector3 *translation)
{
    *out = mat4_from_mat3(*linear);
    out->m14 = translation->x;
    out->m24 = translation->y;
    out->m34 = translation->z;
}
```
