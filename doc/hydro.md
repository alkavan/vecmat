# Hydrodynamics

One-cell primitives. The caller owns the mesh and the time step.

## Equations of state

Four closures. Energy is per reference volume.

- `vm_eos_ideal` is ideal gas
- `vm_eos_stiffened` is stiffened gas
- `vm_eos_mie` is Mie–Grüneisen
- `vm_eos_jwl` is JWL

Each fills a `vm_eos_state` with pressure, sound speed, and internal energy.
A non-positive density returns false.

## Cell primitives

- `vm_hydro_viscous` is the Newtonian stress `σ = 2 μ D + λ (tr D) I`
- `vm_hydro_q` is the von Neumann–Richtmyer artificial viscosity
- `vm_hydro_rusanov` is the face flux from left and right states
- `vm_hydro_cfl` is the cell time-step limit

Artificial viscosity is zero in expansion. Pure spin gives zero stress.
Symmetric states give zero mass flux.

## Projection

Incompressible step. Reuses the existing Laplacian and `vm_cg`.

- `vm_project_residual` is the MAC divergence
- `vm_project_laplacian` fills a caller-owned CSR with the variable-density
  operator. Constant density 1 matches the assembled grid Laplacian.

## SPH

Cubic spline and Wendland C2, both supported on `r < 2 h` and normalized so
the integral is 1.

- `vm_sph_w` and `vm_sph_dw` in 1, 2, or 3-D
- `vm_sph_density` sums over a caller-supplied neighbor list. The self term is
  included. An empty list returns the self term only.
- `vm_sph_pressure_grad` is the symmetric pressure gradient

There is no search structure. A caller bins with `vm_geom3_cell_index` and an
AABB.
