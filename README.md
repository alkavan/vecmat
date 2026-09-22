# Vecmat
A simple math and linear algebra library in C for 2D/3D graphics,
machine learning, physics, and science.

**Vecmat is also a heartfelt ❤ love letter to the C programming language**
— showcasing that readability and ease-of-use shouldn't be second to performance or functionality.

## Philosophy
- Elegance, simplicity, and readability matter more than squeezing every cycle.
- Performance lives under the same names; it does not own the API.

### Goals
- One **common, easy-to-read API**.
- **Usability first**, then performance. Default functions take and return
  values by copy, so call sites stay simple.
- Keep the **public API stable**. Speedups live behind the same names.
- Stay **portable C11**, easy to pull in with CMake (`FetchContent` or `find_package`).
- **Runtime SIMD dispatch** so apps do not pass ISA flags.

### What this library is not
- At the current state — not a complete replacement for GLM+OpenBLAS+PETSc stack.
- Not a graphics engine, not a physics engine, or an ocean / SPH / constraint product —
  **but is built to support such products**.
- No SSE on purpose. Pre-AVX x86 runs the scalar kernels.

## Product contract

Three layers. Anything that is not part/ready for one of these layers would use
a `vm_x_*` prefix or `VECMAT_EXPERIMENTAL` — so far, we don't have such names.

| Layer               | What it is                                                                                             |
|---------------------|--------------------------------------------------------------------------------------------------------|
| **Core**            | `vector2/3/4`, `matrix2/3/4`, `quaternion`, clip-space, transforms, easing. What called every frame.   |
| **Fast path**       | `_ptr` kernels, `vm_cpu_*`, runtime dispatch, `vm_backend_register`. Out-of-tree backends attach here. |
| **Numerics extras** | `vm_gemm`, heap `vm_mat`, LU / QR / SVD / Cholesky, CSR / KSP, integrators, grid primitives.           |

Include one header:

```c
#include <vecmat.h>
```

Or, include individual functionality, `types.h` include `config.h`, and is
included in the headers.

```c
#include <vecmat/vec.h>
#include <vecmat/mat.h>
#include <vecmat/quat.h>
```

## Core

- Default interfaces use **value types** and obvious names (`vector3`,
  `matrix4`, `quaternion`).
- Angles are **radians** on unsuffixed APIs. Write `VM_DEG(90)` or call
  the `_deg` suffix at the human/config edge; `VM_RAD(M_PI_2)` documents
  an already-radian literal.
- Layout is **column-major**. Matrix products are `AB` so `(AB)v == A(Bv)`
  (column vector on the right). Every `mat*_mul` brief states that.
- Components are **`.x/.y/.z`**, **`m11`, `m21`, …**, or a flat **`.v[]`**.
- BSD 3-Clause License.
- Tests use [`unitest.h`](test/unitest.h) and [`except.h`](test/except.h).

### Precision is chosen at build time
- Default: `float` and `int32_t`.
- Optional: `double` (`VECMAT_USE_F64`), and int width 8 / 16 / 32.

### Math types
- Float vectors: 2D, 3D, 4D (`vector2` / `vector3` / `vector4`).
- Integer vectors: same sizes (`vector2i` / `vector3i` / `vector4i`).
- Float and integer matrices: 2×2, 3×3, 4×4.
- Quaternions for rotation.
- Easing functions for animation-style interpolation.
- Clip-space presets for OpenGL (`RH_NO`), Vulkan (`RH_ZO`) and Direct3D (`LH_ZO`).

### Two ways to call everything
- By-value helpers for everyday code.
- `_ptr` kernels for hot paths and SIMD. Those are what backends implement.

## Fast path

The **real work** lives in `_ptr` functions (pointers in, pointers out).

**Selection order:**
`registered backends (by priority) → SVE2 → SVE → NEON → AVX-512F → AVX2 → AVX → scalar`

`vm_backend_register()` installs a complete `_ptr` table (`vm_backend_ops`).
Call it before the first `vm_cpu_init()`. The same pointer is idempotent; a
table with a missing slot is rejected. Public code does not `dlopen`.

```c
vm_cpu_init();
printf("compiled=%s runtime=%s selected=%s\n",
       vm_cpu_name(vm_cpu_compiled_features()),
       vm_cpu_name(vm_cpu_runtime_features()),
       vm_cpu_name(vm_cpu_selected_features()));
```

`vm_cpu_init()` is thread-safe (C11 atomics, double-checked locking) and
idempotent. Concurrent first-use of dispatched kernels is safe.

### ISA matrix

| Backend      | CMake flag              | Default       | Dispatched ops                                                                           | CI label                                                                      |
|--------------|-------------------------|---------------|------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------|
| Scalar       | (always)                | ON            | full public `_ptr` set                                                                   | run on every job (fallback)                                                   |
| AVX          | `VECMAT_ENABLE_AVX`     | ON on x86-64  | dispatched vec4 maps, mat4 mul / transpose / mul_vec, quat mul / normalize, GEMM ukernel | compiled + run on Linux / Windows x86-64 when the host has AVX                |
| AVX2 (FMA)   | `VECMAT_ENABLE_AVX2`    | ON on x86-64  | same set; lerp / mat4 / quat use FMA                                                     | compiled + typically selected on GitHub x86-64 runners                        |
| AVX-512F     | `VECMAT_ENABLE_AVX512F` | ON on x86-64  | same set                                                                                 | compiled on x86-64 jobs; **runtime only if the host has AVX-512F**            |
| NEON / ASIMD | `VECMAT_ENABLE_NEON`    | ON on AArch64 | same dispatched set (one Armv8-A ASIMD schedule, not A53/A55-tuned)                      | compiled + run on Linux aarch64 and Windows ARM64 jobs                        |
| SVE          | `VECMAT_ENABLE_SVE`     | ON on AArch64 | same dispatched set                                                                      | **compile-tested** on aarch64 jobs; selected only if `AT_HWCAP` reports SVE   |
| SVE2         | `VECMAT_ENABLE_SVE2`    | ON on AArch64 | same dispatched set                                                                      | **compile-tested** on aarch64 jobs; selected only if `AT_HWCAP2` reports SVE2 |

**Pre-AVX x86** runs scalar kernels. There is no SSE backend.

MSVC ARM64 jobs compile and run tests. There is no force-ISA job yet
(`VECMAT_FORCE_ISA` coming in 0.5.x).

Windows shared builds export with `VEC_API`; the names themselves are
unsuffixed.

## Numerics extras

Scientific work should configure `-DVECMAT_USE_F64=ON`.

### GEMM
BLAS-style dense multiply `C = alpha * op(A) * op(B) + beta * C`.
Row-major and column-major layouts.

- `vm_gemm` / `vm_gemm_ref` / `vm_gemm_ex` (optional bias and/or ReLU)
- `vm_gemm_batch` / `vm_gemm_strided_batch` (shared-`B` packs once)
- `vm_im2col` unfolds an NCHW image into a GEMM-ready panel

Packed 8×8 ukernel, TLS pack workspace `MC=NC=KC=128`, persistent thread
pool. Cap threads with `vm_gemm_set_threads(n)` or `VECMAT_GEMM_THREADS`.
fp16 / bf16 are not in this release. This is not a BLAS.

Contract: `vm_gemm` matches `vm_gemm_ref` for trans / no-trans, row / col,
odd sizes, `beta ≠ 0`, and batch shared-`B`, on a size band of about 32–512.

### Dense linear algebra
Heap `vm_mat` (M×N, column-major). LU with partial pivoting, Householder
QR, thin one-sided Jacobi SVD, Cholesky. Scale-aware cutoffs
`tol = n * VECMAT_EPSILON * max|A|`. Rank-deficient work returns `false`
(an `ok` flag), not a silent NaN. Integer `matNi_inverse` is **truncated**,
not modular inverse — the declaration says so.

### Sparse systems
- `vm_spmat` — square CSR from triplets
- `vm_spmv`, `vm_cg` (SPD), `vm_bicgstab` (nonsymmetric)
- Left preconditioners: Jacobi, SSOR (ω = 1), IC(0) (falls back to Jacobi
  on pivot breakdown)
- `vm_ksp_info` reports `iters`, `rel_res`, `ok`
- Relative residual is `||r|| / max(||b||, ε)`

### Time integration and grid primitives
- `vm_euler_semi`, `vm_verlet`, `vm_rk2` / `vm_rk4`, `vm_cfl_dt`
- `vm_rigid_step` — symplectic Euler on `(x, v, q, ω)`
- MAC operators and an assembled 5-/7-point Laplacian

### Computer graphics (core)

#### Clip-space helpers
Build projection and view matrices for different graphics APIs and depth conventions.

**Perspective projections** — camera frustum matrices (radians; `_deg` if FOV is in degrees).
The unsuffixed `mat4_perspective` / `mat4_perspective_fov` / `mat4_perspective_infinite`
helpers also take radians. Use `mat4_perspective_deg` (and friends) for degrees:
- `mat4_perspective_clip` / `mat4_perspective_clip_deg`
- `mat4_perspective_rh_no` / `mat4_perspective_rh_no_deg`
- `mat4_perspective_rh_zo` / `mat4_perspective_rh_zo_deg`
- `mat4_perspective_lh_zo` / `mat4_perspective_lh_zo_deg`
- `mat4_perspective_lh_no` / `mat4_perspective_lh_no_deg`

**Orthographic projections** — parallel projection matrices from frustum bounds:
- `mat4_ortho_clip`
- `mat4_ortho_rh_no`
- `mat4_ortho_rh_zo`
- `mat4_ortho_lh_zo`
- `mat4_ortho_lh_no`

**Look-at view matrices** — world-to-view transforms from eye, target, and up:
- `mat4_look_at_clip`
- `mat4_look_at_rh`
- `mat4_look_at_lh`

**Look-from-direction view matrices** — the same basis as look-at,
but the camera aims along a direction (FPS / fly camera, no target point):
- `mat4_look_from_dir` / `mat4_look_from_dir_clip`
- `mat4_look_from_dir_rh` / `mat4_look_from_dir_lh`
- `quat_look` / `quat_look_clip` — orientation whose local −Z (RH) or +Z (LH) aims along the direction
- `quat_from_to` — shortest rotation taking one vector onto another

**Infinite / reverse-Z projections** — infinite far plane,
optionally with reversed depth (near → 1, infinity → 0 on ZO):
- `mat4_perspective_infinite` stays historic OpenGL `RH_NO`
- `mat4_perspective_infinite_clip` — infinite and any clip convention (`*_ZO` is infinite and zero-to-one)
- `mat4_infinite_reverse_z` — modern-engine preset: infinite + RH + ZO + reversed depth
- `mat4_infinite_reverse_z_clip` — same mapping for the other clip conventions

**Viewport, world ↔ window** — NDC to a pixel box and back.
Geometric `vec3_project` (onto a direction) is unchanged:
- `mat4_viewport` / `mat4_viewport_depth`
- `vec3_world_to_window` / `vec3_window_to_world`
- `vec3_world_to_window_clip` / `vec3_window_to_world_clip`

**Affine inverse and normal matrices** — skip the 4×4 adjugate when the transform is `[A t; 0 1]`:
- `mat4_inverse_affine` — invert the 3×3 linear part and apply it to the translation
- `mat3_normal` / `mat4_normal` — inverse-transpose of the 3×3 for transforming normals

**Clip conventions** — handedness + depth range selectors used by the `*_clip` helpers:
- `VM_CLIP_RH_NO` — right-handed, clip z in `[-1, 1]` (OpenGL-style)
- `VM_CLIP_RH_ZO` — right-handed, clip z in `[0, 1]` (Vulkan-style)
- `VM_CLIP_LH_ZO` — left-handed, clip z in `[0, 1]` (Direct3D-style)
- `VM_CLIP_LH_NO` — left-handed, clip z in `[-1, 1]`

#### Rotation helpers
Build 4×4 rotation matrices from axis angles in radians
(`mat4_rotation` / `mat4_rotation_x` / `mat4_rotation_y` / `mat4_rotation_z`).
Use `*_deg` or `VM_DEG(...)` when the angle is in degrees:
- `mat4_rotation_x` / `mat4_rotation_x_deg`
- `mat4_rotation_y` / `mat4_rotation_y_deg`
- `mat4_rotation_z` / `mat4_rotation_z_deg`
- `mat4_rotation` / `mat4_rotation_deg`

## Documentation

* [Online Documentation](https://docs.tekfed.org/vecmat/latest/)

API pages use the [m.css Doxygen theme](https://mcss.mosra.cz/documentation/doxygen/)
with a custom **Dark Fire** palette (`doc/m-theme-dark-fire.css`).
`doc/conf.py` and `doc/Doxyfile-mcss` drive that pipeline. The published
HTML is built from the **public headers** (`include/vecmat.h` and
`include/vecmat/*.h`); briefs and `@param` / `@return` live on the
declarations, docs on implementations only in special cases that required or 
static functions.

### Generate local docs with m.css
```bash
python3 -m venv .venv && source .venv/bin/activate
python3 -m pip install jinja2 Pygments
git clone --depth 1 https://github.com/mosra/m.css /tmp/m.css
python3 /tmp/m.css/documentation/doxygen.py doc/conf.py
```
HTML lands in `doc/html/`. Doxygen writes XML to `doc/xml/` first; both
directories are git-ignored.

### Stock Doxygen HTML
```bash
cd doc && doxygen Doxyfile
```

### Relevant resources
* [Convenient CPU feature detection and dispatch](https://blog.magnum.graphics/backstage/cpu-feature-detection-dispatch/) by [Vladimír Vondruš](https://github.com/mosra)
* [Eigen 5.0.1 Documentation](https://libeigen.gitlab.io/eigen/docs-5.0.1/)
* [LAPACK: Linear Algebra PACKage](https://www.netlib.org/lapack/explore-html/d3/dcc/md__r_e_a_d_m_e.html)
* [BiCGSTAB](https://www.ctcms.nist.gov/~langer/oof2man/RegisteredClass-StabilizedBiConjugateGradient.html)

## CMake Integration

### Source using `FetchContent`

```cmake
if(NOT TARGET vecmat::vecmat)
    include(FetchContent)
    FetchContent_Declare(vecmat
        GIT_REPOSITORY https://github.com/alkavan/vecmat.git
        GIT_TAG v0.3.2
    )
    FetchContent_MakeAvailable(vecmat)
endif()

target_link_libraries(my_app PRIVATE vecmat::vecmat)
```

### Installed Package
```cmake
find_package(vecmat 0.3 CONFIG REQUIRED)
target_link_libraries(my_app PRIVATE vecmat::vecmat)
```

## System integration / Out-of-source build and installation
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug \
  -DVECMAT_BUILD_TESTS=ON \
  -DCMAKE_INSTALL_PREFIX="$HOME/.local"
cmake --build build -j
cmake --install build
```

**Note:** Use `-DVECMAT_INSTALL=ON` only when install rules were turned off or `vecmat`
isn't top-level — and you still want `cmake --install` to install it.

### Scalar precision flags

`vm_float_t` and `vm_int_t` are selected at compile time. Pass the matching CMake
options when configuring Vecmat. The options become **public** compile definitions
on `vecmat::vecmat` and `vecmat::vecmat_static`, so anything that links the library
sees the same typedefs.

**Defaults** (no flags): `vm_float_t` is `float`, `vm_int_t` is `int32_t`.

| CMake flag               | Header macro         | Effect                   |
|--------------------------|----------------------|--------------------------|
| `-DVECMAT_USE_F64=ON`    | `VECMAT_USE_F64`     | `vm_float_t` is `double` |
| `-DVECMAT_USE_INT8=ON`   | `VECMAT_USE_INT8`    | `vm_int_t` is `int8_t`   |
| `-DVECMAT_USE_INT16=ON`  | `VECMAT_USE_INT16`   | `vm_int_t` is `int16_t`  |
| `-DVECMAT_USE_INT32=ON`  | `VECMAT_USE_INT32`   | `vm_int_t` is `int32_t`  |

The integer flags are mutually exclusive. CMake will error if more than one is `ON`.
`VECMAT_USE_F64` can be combined with any one integer flag.

Configure from the command line:

```bash
cmake -S . -B build \
  -DVECMAT_USE_F64=ON \
  -DVECMAT_USE_INT16=ON \
  -DVECMAT_BUILD_TESTS=ON
```

With FetchContent, set the cache variables **before** `FetchContent_MakeAvailable`:

```cmake
set(VECMAT_USE_F64 ON CACHE BOOL "" FORCE)
set(VECMAT_USE_INT16 ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(vecmat)
```

Without CMake, define the same macros yourself (compiler flag or before the library include):

```bash
cc -DVECMAT_USE_F64 -DVECMAT_USE_INT16 ...
```

```c
#define VECMAT_USE_F64
#define VECMAT_USE_INT16
#include <vecmat.h>
```

The library and every translation unit that includes `vecmat.h` must use the same
set of macros, or the types will not match at link time.

## Contributing

We don't have any complicated rules for contributing (for now); we only expect
people to comply with the project [Philosophy](#Philosophy) and the contract
above.

### Artificial Intelligence Guidelines and Transparency

1. **AI use:** Use of AI is neither prohibited nor encouraged. You may use AI only if you follow all the guidelines in
   this section.

2. **Disclosure:** If you add AI-generated material to a contribution or derivative work, say so clearly — for example,
   in the pull request, commit message, or nearby comments. Note which parts were AI-generated or heavily AI-assisted.
   Everyday autocomplete or small wording help does not need a notice.

3. **Responsibility:** When you contribute or share a derivative, you take responsibility that the work has enough
   original human authorship, and that any AI-generated parts don't violate someone else's terms or the project
   [LICENSE](LICENSE).

4. **AI training:** If you train an AI system on this code, it is recommended to give it the whole project, including
   in-code comments and any generated documentation that exists.

## Usage and Examples

Vectors and matrices are plain C structs. Components are available as named
fields (`.x` / `.y` / `.z` / `.w`, or `m11`, `m21`, …) and as a flat `.v[]`
array. Prefer the value constructors for everyday code.

### Individual element access

```c
vector3 p;
p.x = 1.0f;               // same as p.v[0]
p.v[1] = 2.0f;            // same as p.y
printf("%f\n", p.z);
```

```c
matrix3 mat;
mat.v[0] = 1.0f;          // same as mat.m11 (column-major)
printf("%f\n", mat.m21);  // same as mat.v[1]
```

### Initializing a vector

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

The same pattern exists for `vector2` / `vector4` and the integer types
(`vecN_zero`, `vecN_one`, `vecN_splat`, plus `vec2i` / `vec3i`).

### Initializing a matrix

```c
matrix3 ident = {
    .m11 = 1.0f, .m21 = 0.0f, .m31 = 0.0f,
    .m12 = 0.0f, .m22 = 1.0f, .m32 = 0.0f,
    .m13 = 0.0f, .m23 = 0.0f, .m33 = 1.0f
};

matrix3 also = { .v = {1,0,0,  0,1,0,  0,0,1} };
```

### Accessing matrix elements

#### Accessing elements by name
```c
float determinant(const matrix3 *mat) {
    float det =
        mat->m11 * (mat->m22 * mat->m33 - mat->m23 * mat->m32)
      - mat->m12 * (mat->m21 * mat->m33 - mat->m23 * mat->m31)
      + mat->m13 * (mat->m21 * mat->m32 - mat->m22 * mat->m31);
    return det;
}
```

#### Accessing elements by index
```c
matrix3 mat;
for (int i = 0; i < 9; i++) {
    mat.v[i] *= 2.0f;  // Scale all elements by 2
}
```

---

## Implementing Common Vector And Matrix Operations

### Vector Operations Examples

A function for general linear transformation to the vector:
```c
void transform(vector3 *out, const matrix3 *mat, const vector3 *vec) {
    out->x = mat->m11 * vec->x + mat->m12 * vec->y + mat->m13 * vec->z;
    out->y = mat->m21 * vec->x + mat->m22 * vec->y + mat->m23 * vec->z;
    out->z = mat->m31 * vec->x + mat->m32 * vec->y + mat->m33 * vec->z;
}
```

A function to translate a vector by adding a translation offset:
```c
void translate(vector3 *out, const vector3 *vec, const vector3 *translation) {
    out->x = vec->x + translation->x;
    out->y = vec->y + translation->y;
    out->z = vec->z + translation->z;
}
```

### Matrix Operations Examples

You can write a function to multiply two matrix3 instances.
Using the array access makes it easier to implement with nested loops:
```c
void multiply(matrix3 *result, const matrix3 *a, const matrix3 *b) {
    for (int c = 0; c < 3; c++) {      /* columns of result / of B */
        for (int r = 0; r < 3; r++) {  /* rows of result / of A */
            float sum = 0.0f;
            for (int k = 0; k < 3; k++) {
                sum += a->v[k * 3 + r] * b->v[c * 3 + k];  /* column-major */
            }
            result->v[c * 3 + r] = sum;
        }
    }
}
```

This creates a matrix4 that can apply rotation/scaling (from matrix3) followed by translation:
```c
void affine_matrix(matrix4 *out, const matrix3 *linear, const vector3 *translation) {
    // Copy the 3x3 linear part (columns 1-3)
    out->m11 = linear->m11; out->m21 = linear->m21; out->m31 = linear->m31; out->m41 = 0.0f;
    out->m12 = linear->m12; out->m22 = linear->m22; out->m32 = linear->m32; out->m42 = 0.0f;
    out->m13 = linear->m13; out->m23 = linear->m23; out->m33 = linear->m33; out->m43 = 0.0f;

    // Set translation in the fourth column
    out->m14 = translation->x;
    out->m24 = translation->y;
    out->m34 = translation->z;
    out->m44 = 1.0f;
}
```
