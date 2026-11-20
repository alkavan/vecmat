# Devices

`vecmat/device.h` runs a few vecmat ops on a device: the CPU, a GPU, an NPU,
or an accelerator. The CPU feature mask in `cpu.h` stays CPU-only; a GPU is
never a CPU feature bit or a `vm_backend_ops` table.

## Model

**Device 0 is always the built-in CPU device.** It needs no setup and runs
the same kernels as direct calls, so its results match `vm_array_add`,
`vm_array_mul`, `vm_axpy`, and row-major `vm_gemm` bit for bit. Other devices
exist only after a module registers them.

**Buffers** are device memory (`vm_buffer_create`) or caller memory used in
place (`vm_buffer_wrap`, if the device can). The host moves data with
`vm_queue_write` / `vm_queue_read`; `vm_queue_copy` moves it inside one device.
There is no cross-device copy: read to the host, then write.

**A queue is in order.** Each op starts after the previous one. On an async
device (`VM_DEVICE_CAP_ASYNC`) a call may return before its op ran;
`vm_queue_finish` waits for all of them. The CPU device runs each op inside
its call.

**Ops take an explicit `vm_dtype`**, not `vm_float_t`, so one device call
does not depend on the TU's width. The CPU device offers the widths the
library was built with (`VECMAT_FLOAT_ABI`). `vm_device_supports(dev, op, dt)`
answers before you queue.

| Op | Call | Meaning |
|----|------|---------|
| `VM_DEVICE_OP_ARRAY_ADD` | `vm_queue_array_add` | `res[i] = a[i] + b[i]` |
| `VM_DEVICE_OP_ARRAY_MUL` | `vm_queue_array_mul` | `res[i] = a[i] * b[i]` |
| `VM_DEVICE_OP_AXPY`      | `vm_queue_axpy`      | `y[i] += alpha * x[i]` |
| `VM_DEVICE_OP_GEMM`      | `vm_queue_gemm`      | `C = alpha A B + beta C`, row-major, packed |

## Errors

Every fallible call returns `VM_DEVICE_OK` (0) or a negative code:

| Code | When |
|------|------|
| `VM_DEVICE_ERR_INVAL` | NULL handle, negative count, range past a buffer, size overflow, buffers of another device, GEMM `C` equal to `A` or `B`, a misaligned wrapped pointer on the CPU device |
| `VM_DEVICE_ERR_NOMEM` | allocation failed |
| `VM_DEVICE_ERR_UNSUPPORTED` | op or dtype not offered, or `vm_buffer_wrap` on a device without it; nothing was queued, so fall back to the host call |
| `VM_DEVICE_ERR_DEVICE` | the device failed |
| `VM_DEVICE_ERR_FULL` / `_CONFLICT` | registration only |

Core argument errors are found before anything is queued and leave no state.
An error returned from the device, including the CPU device rejecting a
misaligned wrapped pointer, is returned by that call and stays on the queue:
every later `vm_queue_finish` returns the first one. Make a new queue to
continue.

A count of 0 is a no-op that reads nothing (GEMM: `m == 0` or `n == 0`;
`k == 0` scales `C` by `beta`). A transfer of 0 bytes is a no-op and the host
pointer may be NULL. Buffer sizes must be in `[1, PTRDIFF_MAX]`. Every byte
range and every `count × element size` is checked without overflow.

```c
if (vm_queue_gemm(q, VM_DTYPE_F32, m, n, k, 1.0, A, B, 0.0, C) == VM_DEVICE_ERR_UNSUPPORTED) {
    /* run vm_gemm on host copies instead */
}
```

## Ownership and threads

* The caller creates and destroys buffers and queues. Devices are never
  removed, so a `vm_device *` and its index stay valid.
* `vm_buffer_wrap` never takes ownership. The memory must outlive the buffer
  and be aligned to the element size of each dtype used on it.
* A buffer must outlive the queued ops that use it. Host memory passed to
  `vm_queue_write` / `vm_queue_read` must stay valid until `vm_queue_finish`.
  `vm_queue_destroy` finishes first.
* Discovery (`vm_device_count`, `vm_device_get`, info, supports) and
  `vm_device_register` are thread-safe. Use a queue from one thread at a
  time. Two queues may share a buffer only when you order them with
  `vm_queue_finish`.
* `vm_buffer_create` memory is aligned for any element type.

## Device modules

The core has no threads, no `dlopen`, and no vendor headers. A module (in
`vecmat-vendors` or the app) probes its hardware and, only if that works,
calls `vm_device_register(&ops, ctx)`. A failed probe registers nothing.

`vm_device_ops` starts with `struct_size` and `version`
(`VM_DEVICE_OPS_VERSION`, currently 1). The core rejects a table smaller
than its own (`INVAL`) or a different `version` (`UNSUPPORTED`). A larger
table with this version still loads; new slots are appended. All slots are
required except `buffer_wrap` and the `queue_create` / `queue_destroy` pair
(both present, or both absent). Registration is idempotent for the same
(`ops`, `ctx`); a second device with the same name is a conflict. At most
`VM_DEVICE_MAX` (8) devices, CPU included.

The core checks every argument, range, and (op, dtype) pair first. A backend
gets native handles and one `vm_device_cmd` per op in `submit`, so a new op
adds an enum value, not a table slot. There is no priority: the app picks a
device from `vm_device_info` (kind, dtypes, caps, memory).

How each kind fits:

* **CPU** (`VM_DEVICE_CPU`): device 0. It sets `VM_DEVICE_CAP_HOST_MEMORY`,
  runs each op inside the call, and keeps no queue state.
* **GPU** (`VM_DEVICE_GPU`): native buffers, and a native stream per queue
  when the device has queue state. Set `VM_DEVICE_CAP_ASYNC` when a call can
  return before the op finishes. Host-visible memory also sets
  `VM_DEVICE_CAP_HOST_MEMORY` and may offer `buffer_wrap`.
* **NPU** (`VM_DEVICE_NPU`) and **accelerator** (`VM_DEVICE_ACCEL`): offer
  the ops they run. Anything else returns `UNSUPPORTED` and the caller
  falls back.

Bit 2 of `caps` is reserved. The core does not ship a GPU, NPU, or
accelerator kernel. A device module probes its hardware and calls
`vm_device_register` with its kind. That is not a bit in `cpu.h`, and it
does not fill `vm_backend_ops`. `nvidia/ga10x` is the layout example in
[Modules](modules.md). A module offers the ops it can run. Anything else
returns `UNSUPPORTED` and the caller uses the CPU device.
