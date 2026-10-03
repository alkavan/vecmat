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

### Precision
- One library exports both float widths. Float symbols are `name32` and `name64`
  (`vec4_add32`, `vec4_add64`).
- `-DVECMAT_FLOAT_ABI=32` or `64` omits the other set. Integer names and
  `vm_cpu_*` have no width suffix.
- Call sites keep `vec4_add` and `vector4`. Those names are the 32-bit symbols
  unless `VECMAT_USE_F64` is defined before the `#include`. `vector4` is that
  translation unit's width.
- Integer width is configure-time (`vm_int_t`). It is separate from the float width.
- `vm_compiled_float_bits()` reports which float widths are in the linked 
  library. `vm_abi_mismatch()` is non-zero when this translation unit's width is
  not one of them.

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
| SVE          | `VECMAT_ENABLE_SVE`     | ON on AArch64 | same dispatched set; f64 walks `svcntd()` chunks                                         | **compile-tested** on aarch64 jobs; selected only if `AT_HWCAP` reports SVE   |
| SVE2         | `VECMAT_ENABLE_SVE2`    | ON on AArch64 | same dispatched set; f64 walks `svcntd()` chunks                                         | **compile-tested** on aarch64 jobs; selected only if `AT_HWCAP2` reports SVE2 |

**Pre-AVX x86** runs scalar kernels. There is no SSE backend.
SVE and SVE2 f64 `vec4` / `mat4` kernels cover four doubles in `svcntd()` chunks,
so a 128-bit core takes two passes and a 256-bit core takes one. f64 `quat_mul`
uses the scalar formula: `svtbl` needs all four lanes in one register. MSVC has
no SVE intrinsics, so those kernels are off on MSVC for both widths.

MSVC ARM64 jobs compile and run tests. There is no force-ISA job yet
(`VECMAT_FORCE_ISA` coming in 0.5.x).

Windows shared builds export with `VEC_API`. Float exports are `name32` /
`name64`. Integer names and `vm_cpu_*` stay unsuffixed.

## Numerics extras

Heap matrices, GEMM, sparse solvers, integrators, and the clip-space helpers
are in [Numerics extras](doc/numerics.md).

## Documentation

Guides, also built as pages on the docs site:

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

Integer width is selected when configuring the library and published on
`vecmat::vecmat`. Float width is per translation unit: a `BOTH` library
holds both symbol sets, and a TU picks the 64-bit aliases with
`VECMAT_USE_F64`. `-DVECMAT_USE_F64=ON` applies that to this build's tests
and benchmarks. It does not omit `name32`; pass `-DVECMAT_FLOAT_ABI=64` for that.

**Defaults** (no flags): unsuffixed float names are the 32-bit symbols,
`vm_int_t` is `int32_t`.

| CMake flag                          | Header macro               | Effect                                                           |
|-------------------------------------|----------------------------|------------------------------------------------------------------|
| `-DVECMAT_FLOAT_ABI=BOTH`/`32`/`64` | `VECMAT_HAVE_ABI_32`/`_64` | Which float symbol sets are in the `.a` / `.so` (default `BOTH`) |
| `-DVECMAT_USE_F64=ON`               | `VECMAT_USE_F64`           | This TU's unsuffixed float names alias `name64`                  |
| `-DVECMAT_USE_INT8=ON`              | `VECMAT_USE_INT8`          | `vm_int_t` is `int8_t`                                           |
| `-DVECMAT_USE_INT16=ON`             | `VECMAT_USE_INT16`         | `vm_int_t` is `int16_t`                                          |
| `-DVECMAT_USE_INT32=ON`             | `VECMAT_USE_INT32`         | `vm_int_t` is `int32_t`                                          |

The integer flags are mutually exclusive. CMake will error if more than one is `ON`.
Integer width must match the library. Float width does not: a `BOTH` lib accepts both.

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

Integer macros must match the linked library. `VECMAT_USE_F64` is per TU.

## Contributing

We don't have any complicated rules for contributing (for now); we only expect
people to comply with the project _Philosophy_ and the contract
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

* [Basic usage](doc/basic-usage.md) — include, values, and an affine model matrix.
* [Vector usage](doc/vector-usage.md) — constructors, slide / reflect, a particle step.
* [Matrix usage](doc/matrix-usage.md) — column-major layout, `mat4_trs`, a camera.
