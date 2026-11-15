// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

/*
 * Device registry, argument checks, and the built-in CPU device.
 *
 * Compiled once (not per float width). The CPU device calls the suffixed
 * width entry points that this library build provides.
 */

#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "vecmat/device.h"
#include "vecmat/types.h"

#ifdef VECMAT_HAVE_ABI_32
VEC_API void vm_array_add32(vm_float32_t *res, const vm_float32_t *a, const vm_float32_t *b, int n);
VEC_API void vm_array_mul32(vm_float32_t *res, const vm_float32_t *a, const vm_float32_t *b, int n);
VEC_API void vm_axpy32(vm_float32_t *y, vm_float32_t alpha, const vm_float32_t *x, int n);
VEC_API void vm_gemm32(vm_float32_t *C, int ldc, const vm_float32_t *A, int lda,
    const vm_float32_t *B, int ldb, int M, int N, int K,
    vm_float32_t alpha, vm_float32_t beta, bool transA, bool transB, vm_layout_t layout);
#endif
#ifdef VECMAT_HAVE_ABI_64
VEC_API void vm_array_add64(vm_float64_t *res, const vm_float64_t *a, const vm_float64_t *b, int n);
VEC_API void vm_array_mul64(vm_float64_t *res, const vm_float64_t *a, const vm_float64_t *b, int n);
VEC_API void vm_axpy64(vm_float64_t *y, vm_float64_t alpha, const vm_float64_t *x, int n);
VEC_API void vm_gemm64(vm_float64_t *C, int ldc, const vm_float64_t *A, int lda,
    const vm_float64_t *B, int ldb, int M, int N, int K,
    vm_float64_t alpha, vm_float64_t beta, bool transA, bool transB, vm_layout_t layout);
#endif

struct vm_device {
    const vm_device_ops *ops;
    void *ctx;
};

struct vm_buffer {
    vm_device *dev;
    void *native;
    size_t bytes;
    int wrapped;
};

struct vm_queue {
    vm_device *dev;
    void *native;
    int status;  /* first error from the device, sticky */
};

/* ---- CPU device -------------------------------------------------------- */

static void cpu_info(void *ctx, vm_device_info *out)
{
    (void)ctx;
    out->kind = VM_DEVICE_CPU;
    out->name = "cpu";
    out->dtypes = 0;
#ifdef VECMAT_HAVE_ABI_32
    out->dtypes |= VM_DTYPE_BIT(VM_DTYPE_F32);
#endif
#ifdef VECMAT_HAVE_ABI_64
    out->dtypes |= VM_DTYPE_BIT(VM_DTYPE_F64);
#endif
    out->caps = VM_DEVICE_CAP_HOST_MEMORY;
    out->mem_bytes = 0;
}

static int cpu_supports(void *ctx, vm_device_op op, vm_dtype dt)
{
    vm_device_info info;
    (void)op;  /* every op exists for every built width */
    cpu_info(ctx, &info);
    return (info.dtypes & VM_DTYPE_BIT(dt)) != 0;
}

static int cpu_buffer_create(void *ctx, size_t bytes, void **native)
{
    (void)ctx;
    *native = malloc(bytes);  /* aligned for any object type */
    return *native ? VM_DEVICE_OK : VM_DEVICE_ERR_NOMEM;
}

static int cpu_buffer_wrap(void *ctx, void *host, size_t bytes, void **native)
{
    (void)ctx;
    (void)bytes;
    *native = host;
    return VM_DEVICE_OK;
}

static void cpu_buffer_destroy(void *ctx, void *native, int wrapped)
{
    (void)ctx;
    if (!wrapped)
        free(native);
}

static int cpu_write(void *ctx, void *q, void *dst, size_t offset, const void *src, size_t bytes)
{
    (void)ctx;
    (void)q;
    memcpy((unsigned char *)dst + offset, src, bytes);
    return VM_DEVICE_OK;
}

static int cpu_read(void *ctx, void *q, const void *src, size_t offset, void *dst, size_t bytes)
{
    (void)ctx;
    (void)q;
    memcpy(dst, (const unsigned char *)src + offset, bytes);
    return VM_DEVICE_OK;
}

static int cpu_copy(void *ctx, void *q, void *dst, size_t dst_offset,
                    const void *src, size_t src_offset, size_t bytes)
{
    (void)ctx;
    (void)q;
    memmove((unsigned char *)dst + dst_offset, (const unsigned char *)src + src_offset, bytes);
    return VM_DEVICE_OK;
}

static bool cpu_aligned(const void *p, size_t align)
{
    return p == NULL || ((uintptr_t)p % align) == 0;
}

static int cpu_submit(void *ctx, void *q, const vm_device_cmd *c)
{
    const size_t es = c->dtype == VM_DTYPE_F64 ? sizeof(vm_float64_t) : sizeof(vm_float32_t);

    (void)ctx;
    (void)q;
    /* Wrapped host memory may be misaligned; the kernels need natural alignment. */
    if (!cpu_aligned(c->dst, es) || !cpu_aligned(c->a, es) || !cpu_aligned(c->b, es))
        return VM_DEVICE_ERR_INVAL;

    switch (c->dtype) {
#ifdef VECMAT_HAVE_ABI_32
    case VM_DTYPE_F32: {
        vm_float32_t *d = c->dst;
        const vm_float32_t *a = c->a, *b = c->b;
        switch (c->op) {
        case VM_DEVICE_OP_ARRAY_ADD: vm_array_add32(d, a, b, c->n); return VM_DEVICE_OK;
        case VM_DEVICE_OP_ARRAY_MUL: vm_array_mul32(d, a, b, c->n); return VM_DEVICE_OK;
        case VM_DEVICE_OP_AXPY: vm_axpy32(d, (vm_float32_t)c->alpha, a, c->n); return VM_DEVICE_OK;
        case VM_DEVICE_OP_GEMM:
            vm_gemm32(d, c->n, a, c->k, b, c->n, c->m, c->n, c->k,
                      (vm_float32_t)c->alpha, (vm_float32_t)c->beta,
                      false, false, VM_LAYOUT_ROW_MAJOR);
            return VM_DEVICE_OK;
        }
        break;
    }
#endif
#ifdef VECMAT_HAVE_ABI_64
    case VM_DTYPE_F64: {
        vm_float64_t *d = c->dst;
        const vm_float64_t *a = c->a, *b = c->b;
        switch (c->op) {
        case VM_DEVICE_OP_ARRAY_ADD: vm_array_add64(d, a, b, c->n); return VM_DEVICE_OK;
        case VM_DEVICE_OP_ARRAY_MUL: vm_array_mul64(d, a, b, c->n); return VM_DEVICE_OK;
        case VM_DEVICE_OP_AXPY: vm_axpy64(d, c->alpha, a, c->n); return VM_DEVICE_OK;
        case VM_DEVICE_OP_GEMM:
            vm_gemm64(d, c->n, a, c->k, b, c->n, c->m, c->n, c->k,
                      c->alpha, c->beta, false, false, VM_LAYOUT_ROW_MAJOR);
            return VM_DEVICE_OK;
        }
        break;
    }
#endif
    default:
        break;
    }
    return VM_DEVICE_ERR_UNSUPPORTED;
}

static int cpu_finish(void *ctx, void *q)
{
    (void)ctx;
    (void)q;
    return VM_DEVICE_OK;  /* every op ran inside its queue call */
}

static const vm_device_ops cpu_ops = {
    sizeof(vm_device_ops), VM_DEVICE_OPS_VERSION,
    cpu_info, cpu_supports,
    cpu_buffer_create, cpu_buffer_wrap, cpu_buffer_destroy,
    NULL, NULL,
    cpu_write, cpu_read, cpu_copy, cpu_submit, cpu_finish
};

/* ---- Registry ---------------------------------------------------------- */

/* Slots are written under the lock, then published by the release store of
 * the count, so readers need only an acquire load and no lock. */
static vm_device devices[VM_DEVICE_MAX] = { { &cpu_ops, NULL } };
static atomic_int device_count = 1;
static atomic_flag device_lock = ATOMIC_FLAG_INIT;

int vm_device_count(void)
{
    return atomic_load_explicit(&device_count, memory_order_acquire);
}

vm_device *vm_device_get(const int index)
{
    if (index < 0 || index >= vm_device_count())
        return NULL;
    return &devices[index];
}

static bool ops_complete(const vm_device_ops *o)
{
    return o->info && o->supports && o->buffer_create && o->buffer_destroy &&
           o->write && o->read && o->copy && o->submit && o->finish &&
           (o->queue_create == NULL) == (o->queue_destroy == NULL);
}

int vm_device_register(const vm_device_ops *ops, void *ctx)
{
    vm_device_info info;
    int n, status = VM_DEVICE_OK;

    if (!ops || ops->struct_size < sizeof(vm_device_ops))
        return VM_DEVICE_ERR_INVAL;
    if (ops->version != VM_DEVICE_OPS_VERSION)
        return VM_DEVICE_ERR_UNSUPPORTED;
    if (!ops_complete(ops))
        return VM_DEVICE_ERR_INVAL;
    memset(&info, 0, sizeof info);
    ops->info(ctx, &info);
    if (!info.name || info.name[0] == '\0')
        return VM_DEVICE_ERR_INVAL;

    while (atomic_flag_test_and_set_explicit(&device_lock, memory_order_acquire)) {
        /* spin; registration is rare and short */
    }
    n = atomic_load_explicit(&device_count, memory_order_relaxed);
    for (int i = 0; i < n; ++i) {
        vm_device_info other;
        if (devices[i].ops == ops && devices[i].ctx == ctx)
            goto out;
        devices[i].ops->info(devices[i].ctx, &other);
        if (strcmp(other.name, info.name) == 0) {
            status = VM_DEVICE_ERR_CONFLICT;
            goto out;
        }
    }
    if (n >= VM_DEVICE_MAX) {
        status = VM_DEVICE_ERR_FULL;
        goto out;
    }
    devices[n].ops = ops;
    devices[n].ctx = ctx;
    atomic_store_explicit(&device_count, n + 1, memory_order_release);
out:
    atomic_flag_clear_explicit(&device_lock, memory_order_release);
    return status;
}

int vm_device_info_get(const vm_device *dev, vm_device_info *out)
{
    if (!dev || !out)
        return VM_DEVICE_ERR_INVAL;
    dev->ops->info(dev->ctx, out);
    return VM_DEVICE_OK;
}

static bool dtype_valid(const vm_dtype dt)
{
    return dt == VM_DTYPE_F32 || dt == VM_DTYPE_F64;
}

int vm_device_supports(const vm_device *dev, const vm_device_op op, const vm_dtype dt)
{
    vm_device_info info;

    if (!dev || (unsigned)op >= VM_DEVICE_OP_COUNT || !dtype_valid(dt))
        return 0;
    dev->ops->info(dev->ctx, &info);
    if ((info.dtypes & VM_DTYPE_BIT(dt)) == 0)
        return 0;
    return dev->ops->supports(dev->ctx, op, dt) != 0;
}

/* ---- Buffers and queues ------------------------------------------------ */

static int buffer_new(vm_device *dev, void *host, const size_t bytes, vm_buffer **out)
{
    vm_buffer *buf;
    int status;

    if (!out)
        return VM_DEVICE_ERR_INVAL;
    *out = NULL;
    if (!dev || bytes == 0 || bytes > PTRDIFF_MAX)
        return VM_DEVICE_ERR_INVAL;
    buf = malloc(sizeof *buf);
    if (!buf)
        return VM_DEVICE_ERR_NOMEM;
    buf->dev = dev;
    buf->bytes = bytes;
    buf->native = NULL;
    buf->wrapped = host != NULL;
    status = host ? dev->ops->buffer_wrap(dev->ctx, host, bytes, &buf->native)
                  : dev->ops->buffer_create(dev->ctx, bytes, &buf->native);
    if (status != VM_DEVICE_OK) {
        free(buf);
        return status < 0 ? status : VM_DEVICE_ERR_DEVICE;
    }
    *out = buf;
    return VM_DEVICE_OK;
}

int vm_buffer_create(vm_device *dev, const size_t bytes, vm_buffer **out)
{
    return buffer_new(dev, NULL, bytes, out);
}

int vm_buffer_wrap(vm_device *dev, void *host, const size_t bytes, vm_buffer **out)
{
    if (out)
        *out = NULL;
    if (!host)
        return VM_DEVICE_ERR_INVAL;
    if (dev && !dev->ops->buffer_wrap)
        return VM_DEVICE_ERR_UNSUPPORTED;
    return buffer_new(dev, host, bytes, out);
}

void vm_buffer_destroy(vm_buffer *buf)
{
    if (!buf)
        return;
    buf->dev->ops->buffer_destroy(buf->dev->ctx, buf->native, buf->wrapped);
    free(buf);
}

size_t vm_buffer_size(const vm_buffer *buf)
{
    return buf ? buf->bytes : 0;
}

int vm_queue_create(vm_device *dev, vm_queue **out)
{
    vm_queue *q;
    int status = VM_DEVICE_OK;

    if (!out)
        return VM_DEVICE_ERR_INVAL;
    *out = NULL;
    if (!dev)
        return VM_DEVICE_ERR_INVAL;
    q = malloc(sizeof *q);
    if (!q)
        return VM_DEVICE_ERR_NOMEM;
    q->dev = dev;
    q->native = NULL;
    q->status = VM_DEVICE_OK;
    if (dev->ops->queue_create)
        status = dev->ops->queue_create(dev->ctx, &q->native);
    if (status != VM_DEVICE_OK) {
        free(q);
        return status < 0 ? status : VM_DEVICE_ERR_DEVICE;
    }
    *out = q;
    return VM_DEVICE_OK;
}

/* Record a device result; the first error sticks to the queue. */
static int queue_note(vm_queue *q, int status)
{
    if (status > 0)
        status = VM_DEVICE_ERR_DEVICE;
    if (status != VM_DEVICE_OK && q->status == VM_DEVICE_OK)
        q->status = status;
    return status;
}

int vm_queue_finish(vm_queue *q)
{
    if (!q)
        return VM_DEVICE_ERR_INVAL;
    queue_note(q, q->dev->ops->finish(q->dev->ctx, q->native));
    return q->status;
}

void vm_queue_destroy(vm_queue *q)
{
    if (!q)
        return;
    (void)vm_queue_finish(q);
    if (q->dev->ops->queue_destroy)
        q->dev->ops->queue_destroy(q->dev->ctx, q->native);
    free(q);
}

/* `offset + bytes <= buf->bytes` without overflow. */
static bool range_ok(const vm_buffer *buf, const size_t offset, const size_t bytes)
{
    return offset <= buf->bytes && bytes <= buf->bytes - offset;
}

/* Queue usable and the buffer belongs to its device. */
static bool same_dev(const vm_queue *q, const vm_buffer *buf)
{
    return buf && buf->dev == q->dev;
}

int vm_queue_write(vm_queue *q, vm_buffer *dst, const size_t offset,
                   const void *src, const size_t bytes)
{
    if (!q || !same_dev(q, dst) || !range_ok(dst, offset, bytes))
        return VM_DEVICE_ERR_INVAL;
    if (bytes == 0)
        return VM_DEVICE_OK;
    if (!src)
        return VM_DEVICE_ERR_INVAL;
    return queue_note(q, q->dev->ops->write(q->dev->ctx, q->native, dst->native, offset, src, bytes));
}

int vm_queue_read(vm_queue *q, const vm_buffer *src, const size_t offset,
                  void *dst, const size_t bytes)
{
    if (!q || !same_dev(q, src) || !range_ok(src, offset, bytes))
        return VM_DEVICE_ERR_INVAL;
    if (bytes == 0)
        return VM_DEVICE_OK;
    if (!dst)
        return VM_DEVICE_ERR_INVAL;
    return queue_note(q, q->dev->ops->read(q->dev->ctx, q->native, src->native, offset, dst, bytes));
}

int vm_queue_copy(vm_queue *q, vm_buffer *dst, const size_t dst_offset,
                  const vm_buffer *src, const size_t src_offset, const size_t bytes)
{
    if (!q || !same_dev(q, dst) || !same_dev(q, src) ||
        !range_ok(dst, dst_offset, bytes) || !range_ok(src, src_offset, bytes))
        return VM_DEVICE_ERR_INVAL;
    if (bytes == 0)
        return VM_DEVICE_OK;
    return queue_note(q, q->dev->ops->copy(q->dev->ctx, q->native, dst->native, dst_offset,
                                           src->native, src_offset, bytes));
}

static size_t dtype_size(const vm_dtype dt)
{
    return dt == VM_DTYPE_F64 ? 8u : 4u;
}

/* Buffer holds `rows * cols` elements of `dt`, checked for overflow. */
static bool fits(const vm_buffer *buf, const size_t rows, const size_t cols, const vm_dtype dt)
{
    const size_t es = dtype_size(dt);
    if (cols != 0 && rows > SIZE_MAX / cols)
        return false;
    if (rows * cols > SIZE_MAX / es)
        return false;
    return rows * cols * es <= buf->bytes;
}

/* Checks shared by every op, before the op-specific ones. */
static int op_begin(const vm_queue *q, const vm_device_op op, const vm_dtype dt)
{
    if (!q || !dtype_valid(dt))
        return VM_DEVICE_ERR_INVAL;
    if (!vm_device_supports(q->dev, op, dt))
        return VM_DEVICE_ERR_UNSUPPORTED;
    return VM_DEVICE_OK;
}

static int submit(vm_queue *q, const vm_device_cmd *cmd)
{
    return queue_note(q, q->dev->ops->submit(q->dev->ctx, q->native, cmd));
}

static int queue_binary(vm_queue *q, const vm_device_op op, const vm_dtype dt, vm_buffer *res,
                        const vm_buffer *a, const vm_buffer *b, const int n)
{
    vm_device_cmd cmd;
    int status = op_begin(q, op, dt);

    if (status != VM_DEVICE_OK)
        return status;
    if (n < 0 || !same_dev(q, res) || !same_dev(q, a) || !same_dev(q, b))
        return VM_DEVICE_ERR_INVAL;
    if (n == 0)
        return VM_DEVICE_OK;
    if (!fits(res, 1, (size_t)n, dt) || !fits(a, 1, (size_t)n, dt) || !fits(b, 1, (size_t)n, dt))
        return VM_DEVICE_ERR_INVAL;
    memset(&cmd, 0, sizeof cmd);
    cmd.op = op;
    cmd.dtype = dt;
    cmd.n = n;
    cmd.dst = res->native;
    cmd.a = a->native;
    cmd.b = b->native;
    return submit(q, &cmd);
}

int vm_queue_array_add(vm_queue *q, const vm_dtype dt, vm_buffer *res,
                       const vm_buffer *a, const vm_buffer *b, const int n)
{
    return queue_binary(q, VM_DEVICE_OP_ARRAY_ADD, dt, res, a, b, n);
}

int vm_queue_array_mul(vm_queue *q, const vm_dtype dt, vm_buffer *res,
                       const vm_buffer *a, const vm_buffer *b, const int n)
{
    return queue_binary(q, VM_DEVICE_OP_ARRAY_MUL, dt, res, a, b, n);
}

int vm_queue_axpy(vm_queue *q, const vm_dtype dt, vm_buffer *y, const double alpha,
                  const vm_buffer *x, const int n)
{
    vm_device_cmd cmd;
    int status = op_begin(q, VM_DEVICE_OP_AXPY, dt);

    if (status != VM_DEVICE_OK)
        return status;
    if (n < 0 || !same_dev(q, y) || !same_dev(q, x))
        return VM_DEVICE_ERR_INVAL;
    if (n == 0)
        return VM_DEVICE_OK;
    if (!fits(y, 1, (size_t)n, dt) || !fits(x, 1, (size_t)n, dt))
        return VM_DEVICE_ERR_INVAL;
    memset(&cmd, 0, sizeof cmd);
    cmd.op = VM_DEVICE_OP_AXPY;
    cmd.dtype = dt;
    cmd.n = n;
    cmd.alpha = alpha;
    cmd.dst = y->native;
    cmd.a = x->native;
    return submit(q, &cmd);
}

int vm_queue_gemm(vm_queue *q, const vm_dtype dt, const int m, const int n, const int k,
                  const double alpha, const vm_buffer *A, const vm_buffer *B,
                  const double beta, vm_buffer *C)
{
    vm_device_cmd cmd;
    int status = op_begin(q, VM_DEVICE_OP_GEMM, dt);

    if (status != VM_DEVICE_OK)
        return status;
    if (m < 0 || n < 0 || k < 0 || !same_dev(q, A) || !same_dev(q, B) || !same_dev(q, C))
        return VM_DEVICE_ERR_INVAL;
    if (C == A || C == B)
        return VM_DEVICE_ERR_INVAL;
    if (m == 0 || n == 0)
        return VM_DEVICE_OK;
    if (!fits(C, (size_t)m, (size_t)n, dt) ||
        (k > 0 && (!fits(A, (size_t)m, (size_t)k, dt) || !fits(B, (size_t)k, (size_t)n, dt))))
        return VM_DEVICE_ERR_INVAL;
    memset(&cmd, 0, sizeof cmd);
    cmd.op = VM_DEVICE_OP_GEMM;
    cmd.dtype = dt;
    cmd.m = m;
    cmd.n = n;
    cmd.k = k;
    cmd.alpha = alpha;
    cmd.beta = beta;
    cmd.dst = C->native;
    cmd.a = k > 0 ? A->native : NULL;
    cmd.b = k > 0 ? B->native : NULL;
    return submit(q, &cmd);
}
