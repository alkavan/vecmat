### Point Clouds, 3D Reconstruction, DNNs, LLMs (backlog)

- **12×8 / 16×14 kernels**
  Those shapes are why OpenBLAS kernels are per-μarch assembly. They need a different pack (12 rows is not 8), extra
  fringe kernels, and they break the one vm_gemm_ukernel (acc, Ap, Bp, K) dispatch row. 8×8 already fills a ymm for f32;
  12×8 is latency-hiding, maybe 1.3–1.5× on some Intel parts, not an order of magnitude. AVX-512 16×14 is another pack
  again. *Implement GPU backends first*.


- **Pack-cache hierarchy (BLIS)**
  We already have the useful half: pack B once per (jc,kc), stream A tiles, heap TLS buffers at 128. The next BLIS
  layers are L2/L3-sized A/B copies, pack reuse across jc, and tuned MC/NC/KC per CPU. That is a second product. Bumping
  128 → 192 later is a constant; a cache hierarchy is not.


- **Mixed precision**
  `fp16`/`bf16` is a type system, not a GEMM flag. We would need storage types, f32 accumulate, conversions, and ISA
  extras (F16C / BF16 / AVX-512-FP16). Vecmat's contract is still "one vm_float_t chosen at configure time."
  *We should do this when the GPU path needs the same types.*

- **Run tests on aarch64 CI**
  Linux and Windows ARM cells already compile shared/static with dispatch on. `run_tests` is still false there, so SVE /
  SVE2 kernels and the physics/GEMM suite never execute on the host they were written for. Flip those cells on when the
  runners stay cheap enough.

- **Declaration-site Doxygen in `vecmat.h`**
  Briefs and `@param` live on the `.c` definitions today. The published HTML (`docs.tekfed.org`) would be easier to
  browse if the public header carried the same short comments, or if Doxygen were told to merge declaration + definition.
