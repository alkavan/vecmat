# Explicit mechanics

Continuum primitives for a single cell. No mesh, no solver.

## Symmetric tensor

`vm_sym3` is a 3×3 symmetric tensor in Voigt order
`(xx, yy, zz, xy, yz, zx)`. Shear components are tensor shear, not engineering
shear, so a double contraction is a dot.

- `vm_sym3_from_mat3` / `vm_sym3_to_mat3` pack and unpack with `matrix3`
- `vm_sym3_add`, `vm_sym3_scale`
- `vm_sym3_deviator`, `vm_sym3_hydrostatic`, `vm_sym3_trace`
- `vm_sym3_invariants` returns `I1`, `J2`, `J3`
- `vm_sym3_push` is the push-forward `σ' = F σ Fᵀ`, written back as Voigt
- `vm_sym3_dw_split` splits a velocity gradient into `D` and `W`
- `vm_sym3_ddot` is the double contraction

## Strain

From a deformation gradient `F`:

- `vm_strain_cauchy_green` is `C = Fᵀ F`
- `vm_strain_green_lagrange` is `E = ½(C − I)`
- `vm_strain_almansi` is `e = ½(I − b⁻¹)`
- `vm_strain_logarithmic` is `ln U` from the stretch
- `vm_strain_logarithmic_from_grad` runs polar first
- `vm_strain_rotation_to_quat` converts a rotation; improper matrices are rejected

A pure rotation has zero Green–Lagrange strain. `ln(λ I) = ln(λ) I`.

## Geometry

Closest point and gap predicates. Degenerate input returns `ok = false`.

- `vm_geom3_barycentric` and `vm_geom3_closest_triangle`
- `vm_geom3_point_triangle_gap` is signed
- `vm_geom3_edge_edge` includes the parallel case
- `vm_geom3_aabb_triangle` and `vm_geom3_aabb_overlap`
- `vm_geom3_cell_index` maps a point into an existing grid

## Batch

`batch3.h` is a strided scalar reference. A batch of one matches the scalar
call.

- `vm_batch3_mul`, `vm_batch3_inverse`, `vm_batch3_polar`
- `vm_batch3_ddot` is the Voigt-6 double contraction

Strides are in elements, so packed and padded layouts both work. SIMD backends
register through the existing mechanism and must match this result.

## Structural scale

Folded into the physics feature. No new module.

- `vm_lumped_accel` is `a = f / m` over interleaved force, per-node mass.
  Non-positive mass is rejected.
- `vm_rayleigh` is `f -= α m v + β f_int`, component-wise.
