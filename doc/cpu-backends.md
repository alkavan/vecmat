# Backends

A backend is an optional schedule. It is not part of the default library.
It registers a complete `_ptr` table with `vm_backend_register()`, and the
picker prefers that table over the compiled ISA.

Cortex-A53 / A55 tuning is supported. It lives in the `vendors/` submodule
at `vendors/arm/cortex-a5x`, not in the core library. The in-tree NEON row
is the generic Armv8-A schedule. `vendors/` may hold other module types
later; only backends are loaded, and only the names in `VECMAT_MODULES`.
Identity, extra HWCAP bits, and the tuned kernels stay in that backend.
There is no `vm_cpu_part()` in the base library: the backend probes, then
registers `VM_CPU_BACKEND`.

The base library keeps this surface:

- `vm_backend_register()` / `VM_CPU_BACKEND` — a registered table wins
- `vm_backend_builtin()` — copy a compiled ISA table and override slots
- `vm_cpu_set_note()` / `vm_cpu_note()` — one extra banner line, printed only if set
- `VECMAT_FORCE_ISA` — override the compiled/runtime pick when no backend is registered
- `VECMAT_MODULES` — paths under `vendors/`, e.g. `arm/cortex-a5x`
- `VECMAT_BACKEND_PATH` — extra roots if the submodule is not checked out
- `vm_backend_modules_register()` — test and benchmark hook, generated, not a library export

## Layout

A backend is a directory with `module.cmake`. In the submodule that is
`vendors/<vendor>/<name>/`. Cortex-A5x has this shape:

```
vendors/arm/cortex-a5x
├── doc
│   └── cortex-a5x.md
├── include
│   └── vecmat_cortex_a5x.h
├── module.cmake
└── src
    ├── a5x.h
    ├── gemm_a53.c
    ├── gemm_a55.c
    ├── probe.c
    ├── register.c
    └── sched.c
```

`module.cmake` is the only file the top-level build includes. It must set
`VECMAT_MODULE_KIND` to `backend`. A missing file, or any other kind, is
`Unknown Vecmat backend`. Use `CMAKE_CURRENT_LIST_DIR` for its own sources.
`.gitignore` ignores `*.cmake`; `!vendors/**/module.cmake` keeps a
not-yet-submoduled drop tracked.

## Build

Empty `VECMAT_MODULES` is the default.

```bash
cmake -S . -B build -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DVECMAT_MODULES=arm/cortex-a5x \
  -DVECMAT_RUNTIME_DISPATCH=ON
cmake --build build
```

`VECMAT_MODULES` entries are paths relative to `vendors/` (or to each
`VECMAT_BACKEND_PATH` root). Pass `arm/cortex-a5x`, not `cortex-a5x`.
`VECMAT_BACKEND_PATH` is only needed when the submodule is not checked out.

`module.cmake` appends:

- `VECMAT_MODULE_HOOKS` — called from `vecmat_add_float_objects`, once per
  float width. Add sources, include dirs, and per-file flags here.
- `VECMAT_MODULE_TEST_HOOKS` — called after the test targets exist. Add the
  public include dir. Do not define a module macro in the test mains.
- `VECMAT_MODULE_TEST_SOURCES` / `VECMAT_MODULE_BENCH_SOURCES` — extra
  files compiled into `vecmat_tests` and `vecmat_benchmarks`. They live in
  the vendor tree. The mains do not include a backend header.

```cmake
string(APPEND VECMAT_MODULE_REGISTER_INCLUDES "#include <vecmat_cortex_a5x.h>\n")
string(APPEND VECMAT_MODULE_REGISTER_CALLS "    vm_cortex_a5x_register();\n")
```

The header rename (`vm_cortex_a5x_register` to `vm_cortex_a5x_register64`
on an f64 TU) happens because the generated file includes that header and
the test target defines `VECMAT_USE_F64`.

## Register

Tests and benchmarks do not include a backend header. They call the
generated hook, then `vm_cpu_init()`:

```c
vm_backend_modules_register();
vm_cpu_init();
if (vm_cpu_note())
    printf("%s\n", vm_cpu_note());
```

With no backends selected, the generated function is empty. A backend that
does not match the CPU returns 0 and leaves the builtin table in place.
A registered backend wins over `VECMAT_FORCE_ISA`. The same pointer is
idempotent. A table with a missing slot is rejected. `VM_BACKEND_MAX` is 8.

The descriptor:

| Field      | Role                                                                     |
|------------|--------------------------------------------------------------------------|
| `name`     | Stable string. `vm_cpu_name()` returns it when this backend is selected. |
| `features` | `VM_CPU_BACKEND` if the schedule is not itself an ISA bit.               |
| `priority` | Higher wins among registered backends.                                   |
| `ops`      | Complete `vm_backend_ops`. Must outlive the process.                     |

Copy the builtin table, then replace the slots this schedule owns.
`vm_backend_builtin(VM_CPU_NEON)` is NULL when NEON was not compiled.

```c
const vm_backend_ops *builtin = vm_backend_builtin(VM_CPU_NEON);
ops = *builtin;
ops.vm_gemm_ukernel = my_ukernel;
ops.quat_mul_ptr = my_quat_mul;
backend.name = "my-backend";
backend.features = VM_CPU_BACKEND;
backend.priority = 100;
backend.ops = &ops;
vm_cpu_set_note("backend my-backend part=...");
return vm_backend_register(&backend);
```

Float symbols are width-suffixed. Internal functions that both object libs
define need the same treatment.

Benchmarks keep the selected/compiled/runtime/precision line. They print
`vm_cpu_note()` on the next line only when a backend set one.

## New backend

1. Add `vendors/<vendor>/<name>/` with `module.cmake`, `include/`, `src/`,
   and `doc/<name>.md` in the vendors submodule.
2. In `module.cmake`, set `VECMAT_MODULE_KIND` to `backend`, then append a
   sources hook, a test include hook, and the two register strings. Do not
   add an `#ifdef` to `test/test_main.c` or `test/bench_main.c`.
3. Probe in the backend, not in `src/features/cpu.c`.
4. Build the ops table from `vm_backend_builtin()` plus overrides.
5. Pass `VM_CPU_BACKEND` unless the schedule is one of the existing ISA bits.
6. Document the register call and any `VECMAT_FORCE_*` knobs in the
   backend's own doc. A55 `mat4_mul` is the interleaved column schedule;
   `mat4_mul_vec` stays on the builtin NEON kernel.
