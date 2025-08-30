# Vecmat
A simple math and linear algebra library in C for 2d/3d graphics,
machine learning, physics, and science.

**This project is a heartfelt ❤ love letter ️to the C programming language** —
with emphasis the elegance, simplicity and readability of the language, even for
scenarios where other languages might seem more suited. Performance is important
but second to usability and elegance.

## Philosophy
Elegance, simplicity, and readability matter more than squeezing every cycle.

## Goals
- One **common, easy-to-read API** that is self-explanatory.
- Put **usability first**, then performance. Default functions take and return values by copy so call sites stay simple.
- Keep the **public API stable**. Speedups live behind the same names.
- Work well in **graphics engines, simulations, and games**, not only tiny demos.
- Stay portable C11, easy to pull in with CMake (`FetchContent` or `find_package`).
- Grow SIMD amd MMA without forcing apps to pass ISA flags.

## Features
- Default interfaces use **value types** and obvious names (`vector3`, `matrix4`, `quaternion`).
- The **real work** lives in `_ptr` functions (pointers in, pointers out). Those are what SIMD/MMA backends implement.
- You can access components as **`.x/.y/.z`** or as **`m11`, `m21`, ...** or as a flat **`.v[]` array**.
- Performance is not ignored; it is layered *under* a stable, comfortable API.
- BSD 3-Clause License — great for individuals, organizations, and companies.
- Includes a unit testing and benchmarking framework [`unitest.h`](test/unitest.h)
- Exceptions in tests are handled using a custom handler [`except.h`](test/except.h) and you can use it for whatever
  it's only 24 lines of code.

### Precision chosen at build time
- Default: `float` and `int32_t`.
- Optional: `double` (`VECMAT_USE_F64`), and int width 8 / 16 / 32.

### Math types
- Float vectors: 2D, 3D, 4D (`vector2` / `vector3` / `vector4`).
- Integer vectors: same sizes (`vector2i` / `vector3i` / `vector4i`).
- Float and integer matrices: 2x2, 3x3, 4x4.
- Quaternions for rotation.
- Easing functions for animation-style interpolation.

### Features to Avoid
- No SSE and no NEON on purpose. The library jumps to AVX / AVX2 / AVX-512 and ARM SVE / SVE2.

### Two ways to call everything
- By-value helpers for everyday code.
- `_ptr` kernels for hot paths and SIMD.

### Precision chosen at build time
- Default: `float` and `int32_t`.
- Optional: `double` (`VECMAT_USE_F64`), and int width 8 / 16 / 32.

### History
This library started as a quick replacement to the `mathc` library by Felipe
Ferreira da Silva but evolved into somewhat larger scope. While Felipe's library
worked well for some simple stuff, the `mathc` API wasn't ideal for larger like
graphics engines, simulations, and games. I found the math API was somewhat
incomplete, and not straightforward as expected.

I decided to build my own library with the main goal of common API, easy to use,
and self-explanatory interfaces. While `mathc` put performance first making the
API uncomfortable, `vecmat` puts usage and API first; that means that all
default interfaces are copy and type names are expected.


## SIMD and MMA

### CPU Feature Support
* AVX <small style="color: #34d399;">supported</small>
* AVX2 (FMA3) <small style="color: #34d399;">supported</small>
* AVX-512F (AVX-512 FMA) <small style="color: #34d399;">supported</small>
* AVX10 (FMA3) <small style="color: #e0a04e;">work in progress</small>
* AVX10.1 (Xeon 6) <small style="color: #a78bfa;">coming in 2027</small>
* AVX10.2 (Xeon 7) <small style="color: #22d3ee;">tbd</small>
* SVE (ARMv8.2-A+) <small style="color: #34d399;">supported</small>
* SVE2 (ARMv9) <small style="color: #34d399;">supported</small>

### MMA Support
* WMMA / MMA (NVIDIA/CUDA) <small style="color: #e0a04e;">work in progress</small>
* MFMA / WMMA (AMD/ROCm) <small style="color: #e0a04e;">work in progress</small>
* AMX (4th-7th generation Intel Xeon) <small style="color: #a78bfa;">coming in 2027</small>
* SME / SME2 (ARMv9.2-A+) <small style="color: #22d3ee;">tbd</small>

*At this moment we have no plans to support NEON.*

### Relevant Resources
* [Convenient CPU feature detection and dispatch](https://blog.magnum.graphics/backstage/cpu-feature-detection-dispatch/) by [Vladimír Vondruš](https://github.com/mosra)
* [LAPACK: Linear Algebra PACKage](https://www.netlib.org/lapack/explore-html/d3/dcc/md__r_e_a_d_m_e.html)

## CMake Integration

### Source using `FetchContent`

```cmake
if(NOT TARGET vecmat::vecmat)
    include(FetchContent)
    FetchContent_Declare(vecmat
        GIT_REPOSITORY https://github.com/alkavan/vecmat.git
        GIT_TAG v0.1.0
    )
    FetchContent_MakeAvailable(vecmat)
endif()

target_link_libraries(my_app PRIVATE vecmat::vecmat)
```

### Installed Package
```cmake
find_package(vecmat 0.1 CONFIG REQUIRED)
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

We don't have any complicated rules for contributing (for now), we only expect
people to comply with the project [Philosophy](#Philosophy) and [Goals](#Goals).

### Artificial Intelligence Guidelines and Transparency

1. **AI use:** Use of AI is neither prohibited nor encouraged. You may use AI only if you follow all the guidelines in
   this section.


2. **Disclosure:** If you add AI-generated material to a contribution or derivative work, say so clearly — for example
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
vector3 p = vec3(1.0f, 2.0f, 3.0f);
vector2 q = vec2(4.0f, 5.0f);
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
        mat->m11 * (mat->m22 * mat->m33 - mat->m23 * mat->m32)   // First term
      - mat->m12 * (mat->m21 * mat->m33 - mat->m23 * mat->m31)   // Second term (negative)
      + mat->m13 * (mat->m21 * mat->m32 - mat->m22 * mat->m31);  // Third term
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
    for (int i = 0; i < 3; i++) {  // Rows of result
        for (int j = 0; j < 3; j++) {  // Columns of result
            float sum = 0.0f;
            for (int k = 0; k < 3; k++) {
                sum += a.v[i * 3 + k] * b.v[k * 3 + j];  // Row-major indexing
            }
            result->v[i + j*3] = sum;
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
