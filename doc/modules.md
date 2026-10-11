# Modules

Optional modules are not part of the default library. They are loaded only
when named in `VECMAT_MODULES`. Public modules live in the `vecmat-vendors`
repository. The recommended layout is a `vendors/` checkout under this tree.
That checkout may be a git submodule; it is optional. If the repository is
checked out elsewhere, pass its path in `VECMAT_MODULES_PATH`. Private
modules are loaded the same way from their own tree.

The loader accepts two kinds today: `backend` and `width`. A third kind,
`feature`, will be added later. A missing `module.cmake`, or any other kind,
is `Unknown Vecmat module`.

## Kinds

| Kind      | What it does                                                                   | Lists it appends                                      |
|-----------|--------------------------------------------------------------------------------|-------------------------------------------------------|
| `backend` | Registers a complete `_ptr` table with `vm_backend_register()`. A schedule.    | `VECMAT_MODULE_HOOKS`, register includes and calls.   |
| `width`   | Overrides a slot on a core width such as fp16. Does not fill `vm_backend_ops`. | That width's lists (`VECMAT_FP16_MODULE_HOOKS`, ...). |
| `feature` | Not accepted yet. Domain helpers that do not replace a schedule or a width.    | —                                                     |

A backend owns identity, extra HWCAP bits, and the tuned kernels. There is
no `vm_cpu_part()` in the base library: the backend probes, then registers
`VM_CPU_BACKEND`. A width module does not register a backend. The core owns
the storage type; the module only overrides a slot. See `doc/fp16.md`.

## Layout

A module is a directory with `module.cmake`. Under the recommended layout
that is `vendors/<vendor>/<name>/`. `module.cmake` is the only file the
top-level build includes. It must set `VECMAT_MODULE_KIND`. Use
`CMAKE_CURRENT_LIST_DIR` for its own sources.

A `backend` module:

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

A `width` module:

```
vendors/arm/fp16
├── doc
│   └── fp16.md
├── include
│   └── vecmat_arm_fp16.h
├── module.cmake
└── src
    ├── arm_fp16.h
    ├── neon.c
    ├── probe.c
    └── register.c
```

## Build

Empty `VECMAT_MODULES` is the default. With `vendors/` checked out under
this tree:

```bash
cmake -S . -B build -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DVECMAT_MODULES=arm/fp16 \
  -DVECMAT_RUNTIME_DISPATCH=ON
cmake --build build
```

If the modules are elsewhere, add `-DVECMAT_MODULES_PATH=../vecmat-vendors`.
`VECMAT_MODULES` entries are paths relative to `vendors/` (or to each
`VECMAT_MODULES_PATH` root). A private vendors tree is the same.

`module.cmake` appends, depending on kind:

- `backend`: `VECMAT_MODULE_HOOKS` (called from `vecmat_add_float_objects`,
  once per float width), `VECMAT_MODULE_REGISTER_INCLUDES`,
  `VECMAT_MODULE_REGISTER_CALLS`, and optionally
  `VECMAT_MODULE_TEST_HOOKS`, `VECMAT_MODULE_TEST_SOURCES`,
  `VECMAT_MODULE_BENCH_SOURCES`.
- `width`: the lists for the width it implements
  (`VECMAT_FP16_MODULE_HOOKS`, `VECMAT_FP16_MODULE_REGISTER_CALLS`).
  Those lists are consumed only by that width's object lib. The module's
  own `module.cmake` errors if that width's switch is off. A width module
  must not append to the backend lists; the loader rejects that.

Test and bench sources live in the vendor tree. The mains do not include a
module header. `VECMAT_MODULE_TEST_HOOKS` runs after the test targets exist
and may add the public include dir. Do not define a module macro in the
test mains.

## Backend registration

The base library keeps this surface for backends:

- `vm_backend_register()` / `VM_CPU_BACKEND` — a registered table wins
- `vm_backend_builtin()` — copy a compiled ISA table and override slots.
  NEON, SVE, and SVE2 are provided. Returns NULL if that ISA was not compiled.
- `vm_cpu_set_note()` / `vm_cpu_note()` — one extra banner line, printed only if set
- `vm_backend_modules_register()` — test and benchmark hook, generated, not a library export
- `VECMAT_FORCE_ISA` — override the compiled/runtime pick when no backend is registered

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

### The `cortex-a5x` module

```cmake
string(APPEND VECMAT_MODULE_REGISTER_INCLUDES "#include <vecmat_cortex_a5x.h>\n")
string(APPEND VECMAT_MODULE_REGISTER_CALLS "    vm_cortex_a5x_register();\n")
```

The header rename (`vm_cortex_a5x_register` to `vm_cortex_a5x_register64`
on an f64 TU) happens because the generated file includes that header and
the test target defines `VECMAT_USE_F64`.

## Adding a module

1. Add `vendors/<vendor>/<name>/` with `module.cmake`, `include/`, `src/`,
   and `doc/<name>.md`.
2. In `module.cmake`, set `VECMAT_MODULE_KIND` to `backend` or `width`.
3. Append the lists for that kind. Do not add an `#ifdef` to
   `test/test_main.c` or `test/bench_main.c`.
4. For a backend: probe in the module, not in `src/features/cpu.c`. Build
   the ops table from `vm_backend_builtin()` plus overrides. Pass
   `VM_CPU_BACKEND` unless the schedule is one of the existing ISA bits.
5. Document the register call and any `VECMAT_FORCE_*` knobs in the
   module's own doc.
