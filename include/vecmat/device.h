// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_DEVICE_H
#define VECMAT_DEVICE_H

/**
 * @file
 * @brief Minimal device API: discovery, buffers, one in-order queue, a few ops.
 * @ingroup vecmat_device
 *
 * A device is anything that runs vecmat ops on its own memory: the CPU,
 * a GPU, an NPU, or an accelerator. Device 0 is always the built-in CPU
 * device; other devices exist only after `vm_device_register()`. The CPU
 * feature mask in `cpu.h` stays CPU-only. NVIDIA, AMD, Intel, and Arm Mali
 * GPUs are vendor modules, not kernels in this library.
 *
 * Status: every call that can fail returns `VM_DEVICE_OK` (0) or a negative
 * `VM_DEVICE_ERR_*`. Argument errors are reported before anything is
 * queued. A device may report a failed queued op from a later call or from
 * `vm_queue_finish()`; the queue then keeps that error until it is
 * destroyed.
 *
 * Counts are `int`: `n == 0` (or `m == 0` / `n == 0` for GEMM) is a no-op
 * that reads nothing; a negative count is `VM_DEVICE_ERR_INVAL`. Byte
 * ranges are checked against the buffer size without overflow; a range
 * past the end, or an element count whose byte size overflows `size_t`, is
 * `VM_DEVICE_ERR_INVAL`.
 *
 * Ownership: buffers and queues belong to the caller and are destroyed by
 * the caller. A buffer must outlive every queued op that uses it, and host
 * memory passed to `vm_queue_write()` / `vm_queue_read()` must stay valid
 * until `vm_queue_finish()` returns. `vm_buffer_wrap()` never takes
 * ownership of the host memory.
 *
 * Threads: discovery and registration are thread-safe. A queue is used by
 * one thread at a time. Buffers may be shared between queues of the same
 * device only when the caller orders them with `vm_queue_finish()`.
 */

#include <stddef.h>
#include <stdint.h>

#include "vecmat/config.h"

/** @name Status codes @{ */
enum {
    VM_DEVICE_OK = 0,
    VM_DEVICE_ERR_INVAL = -1,       /**< Bad argument, range, or handle mix. */
    VM_DEVICE_ERR_NOMEM = -2,       /**< Host or device allocation failed. */
    VM_DEVICE_ERR_UNSUPPORTED = -3, /**< Op or dtype not offered; fall back. */
    VM_DEVICE_ERR_DEVICE = -4,      /**< The device itself failed. */
    VM_DEVICE_ERR_FULL = -5,        /**< `VM_DEVICE_MAX` devices registered. */
    VM_DEVICE_ERR_CONFLICT = -6     /**< Device name already registered. */
};
/** @} */

/** Most devices, the built-in CPU device included. */
#define VM_DEVICE_MAX 8

typedef struct vm_device vm_device; /**< Registered device; never freed. */
typedef struct vm_buffer vm_buffer; /**< Device memory or wrapped host memory. */
typedef struct vm_queue vm_queue;   /**< In-order command queue. */

typedef enum vm_device_kind {
    VM_DEVICE_CPU = 0,
    VM_DEVICE_GPU = 1,
    VM_DEVICE_NPU = 2,
    VM_DEVICE_ACCEL = 3
} vm_device_kind;

/** Element types. Explicit, so device ops do not depend on the TU width. */
typedef enum vm_dtype {
    VM_DTYPE_F32 = 0,
    VM_DTYPE_F64 = 1
} vm_dtype;

/** Bit of `dt` in `vm_device_info.dtypes`. */
#define VM_DTYPE_BIT(dt) (1u << (unsigned)(dt))

typedef enum vm_device_op {
    VM_DEVICE_OP_ARRAY_ADD = 0, /**< `res[i] = a[i] + b[i]` (`vm_array_add`). */
    VM_DEVICE_OP_ARRAY_MUL = 1, /**< `res[i] = a[i] * b[i]` (`vm_array_mul`). */
    VM_DEVICE_OP_AXPY = 2,      /**< `y[i] += alpha * x[i]` (`vm_axpy`). */
    VM_DEVICE_OP_GEMM = 3       /**< Row-major packed GEMM (`vm_gemm`). */
} vm_device_op;

/** Number of `vm_device_op` values; grows when ops are appended. */
#define VM_DEVICE_OP_COUNT 4

/** @name Device capability bits (`vm_device_info.caps`) @{ */
/** The host can address buffer memory directly (CPU, unified memory). */
#define VM_DEVICE_CAP_HOST_MEMORY (1u << 0)
/** Queued ops may still be running when the queue call returns. */
#define VM_DEVICE_CAP_ASYNC (1u << 1)
/* Bit 2 is reserved for fixed-graph devices (capture/compile/replay). */
/** @} */

typedef struct vm_device_info {
    vm_device_kind kind;
    const char *name;  /**< Stable, unique, non-empty; owned by the device. */
    uint32_t dtypes;   /**< `VM_DTYPE_BIT()` set of supported element types. */
    uint32_t caps;     /**< `VM_DEVICE_CAP_*` bits. */
    size_t mem_bytes;  /**< Device memory size; 0 if shared or unknown. */
} vm_device_info;

/**
 * @brief Number of devices; at least 1 (the CPU device).
 */
VEC_API int vm_device_count(void);

/**
 * @brief Device `index` in `[0, vm_device_count())`, else NULL.
 */
VEC_API vm_device *vm_device_get(int index);

/**
 * @brief Fill `out` with the device description.
 */
VEC_API int vm_device_info_get(const vm_device *dev, vm_device_info *out);

/**
 * @brief 1 if `dev` runs `op` on `dt`, else 0 (also for bad arguments).
 */
VEC_API int vm_device_supports(const vm_device *dev, vm_device_op op, vm_dtype dt);

/**
 * @brief Allocate `bytes` of device memory (contents undefined).
 *
 * Aligned for any element type. `bytes` must be in `[1, PTRDIFF_MAX]`,
 * else `VM_DEVICE_ERR_INVAL`.
 */
VEC_API int vm_buffer_create(vm_device *dev, size_t bytes, vm_buffer **out);

/**
 * @brief Use caller memory as a buffer without copying or taking ownership.
 *
 * `host` must stay valid until the buffer is destroyed and be aligned to
 * the element size of every dtype used with it. `bytes` limits are as for
 * `vm_buffer_create()`. Devices that cannot use
 * host memory return `VM_DEVICE_ERR_UNSUPPORTED`.
 */
VEC_API int vm_buffer_wrap(vm_device *dev, void *host, size_t bytes, vm_buffer **out);

/**
 * @brief Release a buffer; NULL is a no-op. Finish its queues first.
 */
VEC_API void vm_buffer_destroy(vm_buffer *buf);

/**
 * @brief Size in bytes; 0 for NULL.
 */
VEC_API size_t vm_buffer_size(const vm_buffer *buf);

/**
 * @brief Create an in-order queue on `dev`.
 */
VEC_API int vm_queue_create(vm_device *dev, vm_queue **out);

/**
 * @brief Finish pending work, then release the queue; NULL is a no-op.
 */
VEC_API void vm_queue_destroy(vm_queue *q);

/**
 * @brief Wait for every queued op; returns the first error seen, if any.
 */
VEC_API int vm_queue_finish(vm_queue *q);

/**
 * @brief Copy `bytes` from host `src` into `dst` at `offset`.
 */
VEC_API int vm_queue_write(vm_queue *q, vm_buffer *dst, size_t offset,
                           const void *src, size_t bytes);

/**
 * @brief Copy `bytes` from `src` at `offset` into host `dst`.
 */
VEC_API int vm_queue_read(vm_queue *q, const vm_buffer *src, size_t offset,
                          void *dst, size_t bytes);

/**
 * @brief Copy between buffers of the queue's device; ranges may overlap.
 */
VEC_API int vm_queue_copy(vm_queue *q, vm_buffer *dst, size_t dst_offset,
                          const vm_buffer *src, size_t src_offset, size_t bytes);

/**
 * @brief `res[i] = a[i] + b[i]` for `n` elements of `dt` from offset 0.
 *
 * Buffers may exactly alias each other, as in `vm_array_add`.
 */
VEC_API int vm_queue_array_add(vm_queue *q, vm_dtype dt, vm_buffer *res,
                               const vm_buffer *a, const vm_buffer *b, int n);

/**
 * @brief `res[i] = a[i] * b[i]`; same rules as `vm_queue_array_add()`.
 */
VEC_API int vm_queue_array_mul(vm_queue *q, vm_dtype dt, vm_buffer *res,
                               const vm_buffer *a, const vm_buffer *b, int n);

/**
 * @brief `y[i] += alpha * x[i]`; `alpha` is rounded to `dt`.
 */
VEC_API int vm_queue_axpy(vm_queue *q, vm_dtype dt, vm_buffer *y, double alpha,
                          const vm_buffer *x, int n);

/**
 * @brief `C = alpha * A * B + beta * C`, row-major, packed, no transpose.
 *
 * `A` is `m×k`, `B` is `k×n`, `C` is `m×n`. `k == 0` scales C by `beta`
 * without reading A or B. `C` must be a different buffer from `A` and `B`.
 * `alpha` and `beta` are rounded to `dt`.
 */
VEC_API int vm_queue_gemm(vm_queue *q, vm_dtype dt, int m, int n, int k,
                          double alpha, const vm_buffer *A, const vm_buffer *B,
                          double beta, vm_buffer *C);

/**
 * @name Backend interface
 * For device modules (for example in vecmat-vendors). Applications only
 * call `vm_device_register()` with the table a module gives them.
 * @{
 */

/** Current `vm_device_ops.version`. */
#define VM_DEVICE_OPS_VERSION 1

/**
 * @brief One op, already validated by the core.
 *
 * Buffer fields are the backend's own native handles. `alpha` and `beta`
 * are unused where the op has none. Counts are positive, except GEMM `k`,
 * which may be 0 (then `a` and `b` must not be read).
 */
typedef struct vm_device_cmd {
    vm_device_op op;
    vm_dtype dtype;
    int m, n, k;     /**< Element ops use only `n`. */
    double alpha, beta;
    void *dst;       /**< `res`, `y`, or `C`. */
    const void *a;   /**< `a`, `x`, or `A`; NULL if unused. */
    const void *b;   /**< `b` or `B`; NULL if unused. */
} vm_device_cmd;

/**
 * @brief Backend function table. Every slot is required unless marked.
 *
 * `ctx` is the pointer given to `vm_device_register()`. `q` is the native
 * queue (NULL when `queue_create` is NULL). Ranges and counts are checked
 * before a slot is called, and only supported (op, dtype) pairs reach
 * `submit`. Slots return `VM_DEVICE_OK` or a negative code.
 */
typedef struct vm_device_ops {
    size_t struct_size;  /**< `sizeof(vm_device_ops)` the module was built with. */
    uint32_t version;    /**< `VM_DEVICE_OPS_VERSION`. */
    void (*info)(void *ctx, vm_device_info *out);
    int (*supports)(void *ctx, vm_device_op op, vm_dtype dt);
    int (*buffer_create)(void *ctx, size_t bytes, void **native);
    /** Optional; NULL makes `vm_buffer_wrap()` unsupported. */
    int (*buffer_wrap)(void *ctx, void *host, size_t bytes, void **native);
    /** `wrapped` is nonzero for `buffer_wrap` buffers; never free their host memory. */
    void (*buffer_destroy)(void *ctx, void *native, int wrapped);
    /** Optional pair; NULL means the device needs no queue state. */
    int (*queue_create)(void *ctx, void **native);
    void (*queue_destroy)(void *ctx, void *q);
    int (*write)(void *ctx, void *q, void *dst, size_t offset, const void *src, size_t bytes);
    int (*read)(void *ctx, void *q, const void *src, size_t offset, void *dst, size_t bytes);
    int (*copy)(void *ctx, void *q, void *dst, size_t dst_offset,
                const void *src, size_t src_offset, size_t bytes);
    int (*submit)(void *ctx, void *q, const vm_device_cmd *cmd);
    int (*finish)(void *ctx, void *q);
} vm_device_ops;

/**
 * @brief Add a device after the module has probed its hardware.
 *
 * Idempotent for the same (`ops`, `ctx`) pair. Rejects NULL `ops`, a
 * `struct_size` smaller than this header's table, a missing required slot,
 * or an empty name (`VM_DEVICE_ERR_INVAL`); an unknown `version`
 * (`VM_DEVICE_ERR_UNSUPPORTED`); a name already registered
 * (`VM_DEVICE_ERR_CONFLICT`); a full table (`VM_DEVICE_ERR_FULL`).
 * `ops` and `ctx` must outlive the process. Devices are never removed, so
 * an index stays valid; new devices are appended.
 */
VEC_API int vm_device_register(const vm_device_ops *ops, void *ctx);

/** @} */

#endif //VECMAT_DEVICE_H
