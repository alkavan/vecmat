# Radiation

Scalar primitives. No grid, no solver. The caller owns the quadrature and the
transport step.

## Planck

- `vm_planck_b` is the integrated blackbody `σ T⁴ / π`
- `vm_planck_bnu` is the spectral `B_ν`
- `vm_planck_group` integrates a frequency group. `nu_hi <= 0` returns the
  analytic full integral.
- `vm_planck_db_dt` is the temperature derivative

## Angular

Level-symmetric `S_N` tables for `S2`, `S4`, `S6`, and `S8`. Weights sum to
`4π`.

- `vm_angular_count` and `vm_angular_sn` fill nodes and weights
- `vm_angular_isotropic` is the constant phase
- `vm_angular_hg` is the Henyey–Greenstein phase. `g = 0` matches isotropic.
- `vm_angular_scatter` is the scatter source

## Ray

- `vm_ray_tau` is `κ s`
- `vm_ray_tau_sum` sums a segment
- `vm_ray_transmit` is `exp(−τ)`. A cutoff of 80 returns 0.
- `vm_ray_emit` is `(1 − exp(−τ)) B`

## Rosseland diffusion

- `vm_raddiff_d` is `D = 1 / (3 κ_R)`
- `vm_raddiff_limiter` is the Levermore–Pomraning limiter. `λ(0) = 1/3`, and
  `λ` stays in `(0, 1/3]`.
- `vm_raddiff_ratio` is `R = |∇E| / (κ E)`
- `vm_raddiff_residual` is the gray emission–absorption residual
  `κ_a (E − a T⁴)`
- `vm_raddiff_laplacian` fills a caller-owned CSR with `−∇·(D ∇)`. Constant
  `D = 1` matches the assembled grid Laplacian.

The limited coefficient is `D λ(R)`. The solve stays `vm_cg`.
