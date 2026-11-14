# Numerics extras

Scientific work defines `VECMAT_USE_F64` before the include. `-DVECMAT_USE_F64=ON`
does that for this build's tests and benchmarks; it does not drop `name32`.

## GEMM

BLAS-style dense multiply `C = alpha * op(A) * op(B) + beta * C`.
Row-major and column-major layouts.

- `vm_gemm` / `vm_gemm_ref` / `vm_gemm_ex` (optional bias and/or ReLU)
- `vm_gemm_batch` / `vm_gemm_strided_batch` (shared-`B` packs once)
- `vm_im2col` unfolds an NCHW image into a GEMM-ready panel

Packed 8×8 ukernel, TLS pack workspace `MC=NC=KC=128`, persistent thread
pool. Cap threads with `vm_gemm_set_threads(n)` or `VECMAT_GEMM_THREADS`.
fp16 and bf16 are optional widths, off by default. See [`doc/fp16.md`](fp16.md)
and [`doc/bf16.md`](bf16.md). This is not a BLAS.

Contract: `vm_gemm` matches `vm_gemm_ref` for trans / no-trans, row / col,
odd sizes, `beta ≠ 0`, and batch shared-`B`, on a size band of about 32–512.

Column-major `C = A * B` for square panels:

```c
vm_gemm(C, n, A, n, B, n, n, n, n,
        1.0f, 0.0f, false, false, VM_LAYOUT_COL_MAJOR);
```

## Contiguous arrays

Scalar loops on length-`n` buffers. `n <= 0` does not read. An elementwise
result may exactly alias an input. `vm_array_add`, `vm_array_sub`, `vm_array_mul`, `vm_array_div`,
`vm_array_fma`, and `vm_axpy` use AVX2 or NEON when the runtime CPU has it;
the scalar path remains the fallback. `vm_array_fma` is one rounding through
`VECMAT_FMA` (`fma` / `fmaf`). `vm_axpy` is `y += alpha * x`, a multiply
then an add (not fused). Sums and dots use Neumaier compensation. `vm_sum_f32f64` and
`vm_dot_f32f64` are compiled once and are not width-remapped; the dot widens
each factor before the multiply.

- `vm_array_add`, `vm_array_sub`, `vm_array_mul`, `vm_array_div`, `vm_array_fma`, `vm_axpy`
- `vm_rsqrt_array` returns `1 / sqrt(x)` for each element using a real square root then a divide (not a cheap estimate). Ordinary positive numbers are accurate to within 2 ULP; `+0` → `+inf`, `−0` → `−inf`, `+inf` → `+0`, and any negative number → `NaN`.
- `vm_sum_compensated`, `vm_dot_compensated`, `vm_sum_f32f64`, `vm_dot_f32f64`
- `vm_array_minmax` ignores NaNs. The minimum of `±0` is `−0` and the maximum is `+0`
- `vm_periodic_delta_ortho` is the orthorhombic minimum image of `a − b` on `vector3` arrays, ties-to-even, in `[-L/2, L/2]`
- `vm_wrap_ortho` folds a `vector3` array into `[0, L)`

`VECMAT_REMAINDER` is `remainder` / `remainderf`. These are not a force
field. They do not change `vm_verlet` or `vm_gemm`.

## SoA geometry

`include/vecmat/soa3.h` batches independent 3-vectors held as a
structure of arrays. `vm_soa3` / `vm_csoa3` are borrowed views of three
unit-stride component pointers; they carry no size, ownership, padding, or
SIMD width, and the component arrays must not overlap. `vm_soa3_dot`,
`vm_soa3_cross`, `vm_soa3_length_squared`, and `vm_soa3_distance_squared`
take `n` vectors; `n <= 0` is a no-op. Products and sums are separate
roundings in the scalar order, never fused, so the AVX2 and NEON paths match
the scalar loop bit for bit. Cross loads every component before it stores,
so its output view may exactly equal an input view. Accuracy is an absolute
bound, 16 eps × the sum of the absolute terms, not a ULP claim. This does
not change `vector3` normalization.

## Dense linear algebra

Heap `vm_mat` (M×N, column-major). LU with partial pivoting, Householder
QR, thin one-sided Jacobi SVD, Cholesky. Scale-aware cutoffs
`tol = n * VECMAT_EPSILON * max|A|`. Rank-deficient work returns `false`
(an `ok` flag), not a silent NaN. Integer `matNi_inverse` is **truncated**,
not modular inverse — the declaration says so.

```c
vm_mat A = vm_mat_alloc(3, 3);
vm_mat_set(&A, 0, 0, 2.0);
vm_mat_set(&A, 1, 1, 2.0);
vm_mat_set(&A, 2, 2, 2.0);
vm_mat inv;
bool ok = vm_mat_inverse(&inv, &A);
vm_mat_free(&A);
vm_mat_free(&inv);
```

## 3×3 factorizations

`mat3_inverse_ok` returns false on a singular matrix (scale-aware cutoff).
`mat3_sym_eigen` is the analytical symmetric 3×3 eigensolve, eigenvalues
sorted ascending, eigenvectors on request. `mat3_polar` is the polar
decomposition `F = R U` (Newton on `C = Fᵀ F`). A reflection is a failure.

## Dense spectral primitives

`include/vecmat/spectral.h`:

- `vm_matvec`, `vm_symv` — dense matvec (symmetric reads the lower triangle)
- `vm_syev` — Jacobi symmetric eigensolve, eigenvalues descending, `A` overwritten
- `vm_laplacian` — unnormalized graph Laplacian `L = D − A`
- `vm_syev_topk` — thin wrapper for the leading k eigenpairs

## Sparse systems

- `vm_spmat` — square CSR from triplets
- `vm_spmv`, `vm_cg` (SPD), `vm_bicgstab` (nonsymmetric)
- Left preconditioners: Jacobi, SSOR (ω = 1), IC(0) (falls back to Jacobi
  on pivot breakdown)
- `vm_ksp_info` reports `iters`, `rel_res`, `ok`
- Relative residual is `||r|| / max(||b||, ε)`

## Time integration and grid primitives

- `vm_euler_semi`, `vm_verlet`, `vm_rk2` / `vm_rk4`, `vm_cfl_dt`
- `vm_rigid_step` — symplectic Euler on `(x, v, q, ω)`
- MAC operators and an assembled 5-/7-point Laplacian

Semi-implicit Euler on one particle. `x`, `v`, and `a` are length `n`.

```c
vm_float_t x[3] = {0.0f, 1.0f, 0.0f};
vm_float_t v[3] = {1.0f, 0.0f, 0.0f};
const vm_float_t a[3] = {0.0f, -9.81f, 0.0f};
vm_euler_semi(x, v, a, 3, 1.0f / 60.0f);
```

## Computer graphics (core)

### Clip-space helpers

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

**Frustum planes** — six planes of an existing `projection * view` matrix.
Inside each plane is `ax + by + cz + d >= 0`. Order is left, right, bottom,
top, near, far. Pass the same `vm_clip_t` the projection was built with
(`*_NO` keeps near at `z = -w`, `*_ZO` keeps near at `z = 0`). `normalize`
makes `(a, b, c)` a unit normal.

- `mat4_frustum_planes` / `mat4_frustum_planes_ptr`
- `vm_frustum_contains` — point test, boundary counts as inside
- `vm_frustum_aabb` — `VM_FRUSTUM_INSIDE`, `VM_FRUSTUM_CROSSING`, or `VM_FRUSTUM_OUTSIDE`

**Affine inverse and normal matrices** — skip the 4×4 adjugate when the
transform is `[A t; 0 1]`:

- `mat4_inverse_affine` — invert the 3×3 linear part and apply it to the translation
- `mat3_normal` / `mat4_normal` — inverse-transpose of the 3×3 for transforming normals

**Clip conventions** — handedness + depth range selectors used by the `*_clip` helpers:

- `VM_CLIP_RH_NO` — right-handed, clip z in `[-1, 1]` (OpenGL-style)
- `VM_CLIP_RH_ZO` — right-handed, clip z in `[0, 1]` (Vulkan-style)
- `VM_CLIP_LH_ZO` — left-handed, clip z in `[0, 1]` (Direct3D-style)
- `VM_CLIP_LH_NO` — left-handed, clip z in `[-1, 1]`

A Vulkan-style camera is a projection times a view. Angles are radians;
`VM_DEG` converts a degree literal.

```c
const vector3 eye = vec3(0.0f, 1.6f, 4.0f);
const vector3 at  = vec3(0.0f, 1.0f, 0.0f);
const vector3 up  = vec3(0.0f, 1.0f, 0.0f);
const matrix4 view = mat4_look_at_rh(eye, at, up);
const matrix4 proj = mat4_perspective_rh_zo(VM_DEG(60.0), 16.0f / 9.0f, 0.1f, 200.0f);
const matrix4 view_proj = mat4_mul(proj, view);
```

### Rotation helpers

Build 4×4 rotation matrices from axis angles in radians
(`mat4_rotation` / `mat4_rotation_x` / `mat4_rotation_y` / `mat4_rotation_z`).
Use `*_deg` or `VM_DEG(...)` when the angle is in degrees:

- `mat4_rotation_x` / `mat4_rotation_x_deg`
- `mat4_rotation_y` / `mat4_rotation_y_deg`
- `mat4_rotation_z` / `mat4_rotation_z_deg`
- `mat4_rotation` / `mat4_rotation_deg`
