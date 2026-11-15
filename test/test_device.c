// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vecmat.h>
#include "unitest.h"

#ifdef VECMAT_USE_F64
#define DT VM_DTYPE_F64
#else
#define DT VM_DTYPE_F32
#endif

#define DEV_N 37

static vm_device *cpu_dev(void)
{
    return vm_device_get(0);
}

static void fill(vm_float_t *x, const int n, const int seed)
{
    for (int i = 0; i < n; ++i)
        x[i] = (vm_float_t)((i * 7 + seed * 13) % 23) / VM_F(8.0) - VM_F(1.25);
}

/* ---- Fake device: host memory, f32 ARRAY_ADD only ----------------------- */

typedef struct fake_ctx {
    const char *name;
    int submits;
    int queues;
    int fail_next;  /* next submit reports a device failure */
} fake_ctx;

static void fake_info(void *ctx, vm_device_info *out)
{
    out->kind = VM_DEVICE_ACCEL;
    out->name = ((fake_ctx *)ctx)->name;
    out->dtypes = VM_DTYPE_BIT(VM_DTYPE_F32);
    out->caps = VM_DEVICE_CAP_HOST_MEMORY;
    out->mem_bytes = 1u << 20;
}

static int fake_supports(void *ctx, vm_device_op op, vm_dtype dt)
{
    (void)ctx;
    return op == VM_DEVICE_OP_ARRAY_ADD && dt == VM_DTYPE_F32;
}

static int fake_buffer_create(void *ctx, size_t bytes, void **native)
{
    (void)ctx;
    *native = calloc(1, bytes);
    return *native ? VM_DEVICE_OK : VM_DEVICE_ERR_NOMEM;
}

static void fake_buffer_destroy(void *ctx, void *native, int wrapped)
{
    (void)ctx;
    if (!wrapped)
        free(native);
}

static int fake_queue_create(void *ctx, void **native)
{
    ++((fake_ctx *)ctx)->queues;
    *native = ctx;
    return VM_DEVICE_OK;
}

static void fake_queue_destroy(void *ctx, void *q)
{
    (void)q;
    --((fake_ctx *)ctx)->queues;
}

static int fake_write(void *ctx, void *q, void *dst, size_t offset, const void *src, size_t bytes)
{
    (void)ctx;
    (void)q;
    memcpy((unsigned char *)dst + offset, src, bytes);
    return VM_DEVICE_OK;
}

static int fake_read(void *ctx, void *q, const void *src, size_t offset, void *dst, size_t bytes)
{
    (void)ctx;
    (void)q;
    memcpy(dst, (const unsigned char *)src + offset, bytes);
    return VM_DEVICE_OK;
}

static int fake_copy(void *ctx, void *q, void *dst, size_t dst_offset,
                     const void *src, size_t src_offset, size_t bytes)
{
    (void)ctx;
    (void)q;
    memmove((unsigned char *)dst + dst_offset, (const unsigned char *)src + src_offset, bytes);
    return VM_DEVICE_OK;
}

static int fake_submit(void *ctx, void *q, const vm_device_cmd *cmd)
{
    fake_ctx *f = ctx;
    float *d = cmd->dst;
    const float *a = cmd->a, *b = cmd->b;

    (void)q;
    if (f->fail_next) {
        f->fail_next = 0;
        return VM_DEVICE_ERR_DEVICE;
    }
    ++f->submits;
    for (int i = 0; i < cmd->n; ++i)
        d[i] = a[i] + b[i];
    return VM_DEVICE_OK;
}

static int fake_finish(void *ctx, void *q)
{
    (void)ctx;
    (void)q;
    return VM_DEVICE_OK;
}

static const vm_device_ops fake_ops = {
    .struct_size = sizeof(vm_device_ops),
    .version = VM_DEVICE_OPS_VERSION,
    .info = fake_info,
    .supports = fake_supports,
    .buffer_create = fake_buffer_create,
    .buffer_wrap = NULL,
    .buffer_destroy = fake_buffer_destroy,
    .queue_create = fake_queue_create,
    .queue_destroy = fake_queue_destroy,
    .write = fake_write,
    .read = fake_read,
    .copy = fake_copy,
    .submit = fake_submit,
    .finish = fake_finish
};

static fake_ctx fake_state = {
    .name = "test-fake",
    .submits = 0,
    .queues = 0,
    .fail_next = 0
};

/* Registers on first use (idempotent), returns the fake device. */
static vm_device *fake_dev(void)
{
    if (vm_device_register(&fake_ops, &fake_state) != VM_DEVICE_OK)
        return NULL;
    for (int i = 0; i < vm_device_count(); ++i) {
        vm_device_info info;
        vm_device *d = vm_device_get(i);
        if (vm_device_info_get(d, &info) == VM_DEVICE_OK && strcmp(info.name, "test-fake") == 0)
            return d;
    }
    return NULL;
}

static int float_arrays_equal(const vm_float_t *a, const vm_float_t *b, const size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        if (a[i] != b[i])
            return 0;
    }
    return 1;
}

/* ---- CPU device -------------------------------------------------------- */

TEST_CASE(device_cpu_is_device_zero, "[device]")
{
    vm_device_info info;
    const vm_device *d = cpu_dev();

    REQUIRE(vm_device_count() >= 1);
    REQUIRE(d != NULL);
    REQUIRE(vm_device_get(-1) == NULL);
    REQUIRE(vm_device_get(vm_device_count()) == NULL);
    REQUIRE(vm_device_info_get(d, &info) == VM_DEVICE_OK);
    REQUIRE(info.kind == VM_DEVICE_CPU);
    REQUIRE(strcmp(info.name, "cpu") == 0);
    REQUIRE((info.dtypes & VM_DTYPE_BIT(DT)) != 0);
    REQUIRE((info.caps & VM_DEVICE_CAP_HOST_MEMORY) != 0);
    REQUIRE((info.caps & VM_DEVICE_CAP_ASYNC) == 0);
    REQUIRE(vm_device_info_get(NULL, &info) == VM_DEVICE_ERR_INVAL);
    REQUIRE(vm_device_info_get(d, NULL) == VM_DEVICE_ERR_INVAL);

    for (int op = 0; op < VM_DEVICE_OP_COUNT; ++op) {
        REQUIRE(vm_device_supports(d, (vm_device_op)op, DT) == 1);
        for (int dt = VM_DTYPE_F32; dt <= VM_DTYPE_F64; ++dt)
            REQUIRE(vm_device_supports(d, (vm_device_op)op, (vm_dtype)dt) ==
                    ((info.dtypes & VM_DTYPE_BIT(dt)) != 0));
    }
    REQUIRE(vm_device_supports(d, (vm_device_op)VM_DEVICE_OP_COUNT, DT) == 0);
    REQUIRE(vm_device_supports(d, VM_DEVICE_OP_GEMM, (vm_dtype)7) == 0);
    REQUIRE(vm_device_supports(NULL, VM_DEVICE_OP_GEMM, DT) == 0);
}

TEST_CASE(device_buffers_and_transfers, "[device]")
{
    vm_device *d = cpu_dev();
    vm_buffer *a = NULL, *b = NULL, *w = NULL;
    vm_queue *q = NULL;
    unsigned char src[64], dst[64], host[16];

    for (int i = 0; i < 64; ++i)
        src[i] = (unsigned char)i;
    memset(host, 0, sizeof host);

    REQUIRE(vm_buffer_create(d, 64, &a) == VM_DEVICE_OK);
    REQUIRE(vm_buffer_create(d, 64, &b) == VM_DEVICE_OK);
    REQUIRE(vm_buffer_size(a) == 64);
    REQUIRE(vm_buffer_size(NULL) == 0);
    REQUIRE(vm_queue_create(d, &q) == VM_DEVICE_OK);

    REQUIRE(vm_queue_write(q, a, 0, src, 64) == VM_DEVICE_OK);
    REQUIRE(vm_queue_copy(q, b, 8, a, 0, 56) == VM_DEVICE_OK);
    REQUIRE(vm_queue_copy(q, a, 1, a, 0, 63) == VM_DEVICE_OK);  /* overlap */
    memset(dst, 0xff, sizeof dst);
    REQUIRE(vm_queue_read(q, b, 8, dst, 56) == VM_DEVICE_OK);
    REQUIRE(memcmp(dst, src, 56) == 0);
    REQUIRE(vm_queue_read(q, a, 1, dst, 63) == VM_DEVICE_OK);
    REQUIRE(memcmp(dst, src, 63) == 0);

    /* Wrapped memory is the caller's: writes land there directly. */
    REQUIRE(vm_buffer_wrap(d, host, sizeof host, &w) == VM_DEVICE_OK);
    REQUIRE(vm_queue_copy(q, w, 0, a, 1, 16) == VM_DEVICE_OK);
    REQUIRE(vm_queue_finish(q) == VM_DEVICE_OK);
    REQUIRE(memcmp(host, src, 16) == 0);
    vm_buffer_destroy(w);  /* must not free `host` (ASan checks) */
    REQUIRE(host[15] == 15);

    /* Zero bytes is a no-op, even with NULL host memory. */
    REQUIRE(vm_queue_write(q, a, 64, NULL, 0) == VM_DEVICE_OK);
    REQUIRE(vm_queue_read(q, a, 0, NULL, 0) == VM_DEVICE_OK);
    REQUIRE(vm_queue_copy(q, a, 64, b, 64, 0) == VM_DEVICE_OK);

    /* Ranges, including ones whose end overflows size_t. */
    REQUIRE(vm_queue_write(q, a, 1, src, 64) == VM_DEVICE_ERR_INVAL);
    REQUIRE(vm_queue_write(q, a, 65, src, 0) == VM_DEVICE_ERR_INVAL);
    REQUIRE(vm_queue_read(q, a, SIZE_MAX, dst, 2) == VM_DEVICE_ERR_INVAL);
    REQUIRE(vm_queue_read(q, a, 2, dst, SIZE_MAX) == VM_DEVICE_ERR_INVAL);
    REQUIRE(vm_queue_copy(q, a, 0, b, 32, 33) == VM_DEVICE_ERR_INVAL);
    REQUIRE(vm_queue_write(q, a, 0, NULL, 4) == VM_DEVICE_ERR_INVAL);
    REQUIRE(vm_queue_write(NULL, a, 0, src, 4) == VM_DEVICE_ERR_INVAL);
    REQUIRE(vm_queue_write(q, NULL, 0, src, 4) == VM_DEVICE_ERR_INVAL);

    /* Creation errors leave *out NULL. */
    w = a;
    REQUIRE(vm_buffer_create(d, 0, &w) == VM_DEVICE_ERR_INVAL && w == NULL);
    REQUIRE(vm_buffer_create(NULL, 8, &w) == VM_DEVICE_ERR_INVAL && w == NULL);
    REQUIRE(vm_buffer_create(d, 8, NULL) == VM_DEVICE_ERR_INVAL);
    REQUIRE(vm_buffer_create(d, SIZE_MAX, &w) == VM_DEVICE_ERR_INVAL && w == NULL);
    REQUIRE(vm_buffer_wrap(d, host, (size_t)PTRDIFF_MAX + 1u, &w) == VM_DEVICE_ERR_INVAL && w == NULL);
    REQUIRE(vm_buffer_wrap(d, NULL, 8, &w) == VM_DEVICE_ERR_INVAL && w == NULL);
    REQUIRE(vm_buffer_wrap(d, host, 0, &w) == VM_DEVICE_ERR_INVAL && w == NULL);
    {
        vm_queue *bad = q;
        REQUIRE(vm_queue_create(NULL, &bad) == VM_DEVICE_ERR_INVAL && bad == NULL);
    }

    vm_buffer_destroy(NULL);
    vm_queue_destroy(NULL);
    REQUIRE(vm_queue_finish(NULL) == VM_DEVICE_ERR_INVAL);
    vm_buffer_destroy(a);
    vm_buffer_destroy(b);
    vm_queue_destroy(q);
}

/* Upload `n` values, return the buffer. */
static vm_buffer *upload(vm_queue *q, const vm_float_t *x, const int n)
{
    vm_buffer *buf = NULL;
    if (vm_buffer_create(vm_device_get(0), (size_t)n * sizeof(vm_float_t), &buf) != VM_DEVICE_OK)
        return NULL;
    if (vm_queue_write(q, buf, 0, x, (size_t)n * sizeof(vm_float_t)) != VM_DEVICE_OK) {
        vm_buffer_destroy(buf);
        return NULL;
    }
    return buf;
}

TEST_CASE(device_cpu_ops_match_direct_calls, "[device]")
{
    vm_queue *q = NULL;
    vm_float_t a[DEV_N], b[DEV_N], want[DEV_N], got[DEV_N];

    fill(a, DEV_N, 1);
    fill(b, DEV_N, 2);
    REQUIRE(vm_queue_create(cpu_dev(), &q) == VM_DEVICE_OK);
    
    vm_buffer *ba = upload(q, a, DEV_N);
    vm_buffer *bb = upload(q, b, DEV_N);
    vm_buffer *br = upload(q, b, DEV_N);
    REQUIRE(ba && bb && br);

    vm_array_add(want, a, b, DEV_N);
    REQUIRE(vm_queue_array_add(q, DT, br, ba, bb, DEV_N) == VM_DEVICE_OK);
    REQUIRE(vm_queue_read(q, br, 0, got, sizeof got) == VM_DEVICE_OK);
    REQUIRE(vm_queue_finish(q) == VM_DEVICE_OK);
    REQUIRE(float_arrays_equal(want, got, DEV_N));

    vm_array_mul(want, a, b, DEV_N);
    REQUIRE(vm_queue_array_mul(q, DT, br, ba, bb, DEV_N) == VM_DEVICE_OK);
    REQUIRE(vm_queue_read(q, br, 0, got, sizeof got) == VM_DEVICE_OK);
    REQUIRE(float_arrays_equal(want, got, DEV_N));

    /* Exact alias, as vm_array_add allows: res = a + a in place. */
    vm_array_add(want, a, a, DEV_N);
    REQUIRE(vm_queue_array_add(q, DT, ba, ba, ba, DEV_N) == VM_DEVICE_OK);
    REQUIRE(vm_queue_read(q, ba, 0, got, sizeof got) == VM_DEVICE_OK);
    REQUIRE(float_arrays_equal(want, got, DEV_N));

    memcpy(want, b, sizeof want);
    vm_axpy(want, VM_F(0.75), a, DEV_N);
    REQUIRE(vm_queue_write(q, bb, 0, b, sizeof b) == VM_DEVICE_OK);
    REQUIRE(vm_queue_write(q, ba, 0, a, sizeof a) == VM_DEVICE_OK);
    REQUIRE(vm_queue_axpy(q, DT, bb, 0.75, ba, DEV_N) == VM_DEVICE_OK);
    REQUIRE(vm_queue_read(q, bb, 0, got, sizeof got) == VM_DEVICE_OK);
    REQUIRE(float_arrays_equal(want, got, DEV_N));

    /* n == 0 is a no-op; n < 0 and short buffers are errors. */
    REQUIRE(vm_queue_array_add(q, DT, br, ba, bb, 0) == VM_DEVICE_OK);
    REQUIRE(vm_queue_array_add(q, DT, br, ba, bb, -1) == VM_DEVICE_ERR_INVAL);
    REQUIRE(vm_queue_array_add(q, DT, br, ba, bb, DEV_N + 1) == VM_DEVICE_ERR_INVAL);
    REQUIRE(vm_queue_array_add(q, DT, br, ba, bb, INT_MAX) == VM_DEVICE_ERR_INVAL);
    REQUIRE(vm_queue_axpy(q, DT, bb, 1.0, ba, -3) == VM_DEVICE_ERR_INVAL);
    REQUIRE(vm_queue_array_mul(q, DT, br, NULL, bb, 4) == VM_DEVICE_ERR_INVAL);
    REQUIRE(vm_queue_array_mul(NULL, DT, br, ba, bb, 4) == VM_DEVICE_ERR_INVAL);
    REQUIRE(vm_queue_array_mul(q, (vm_dtype)9, br, ba, bb, 4) == VM_DEVICE_ERR_INVAL);
    REQUIRE(vm_queue_finish(q) == VM_DEVICE_OK);  /* argument errors do not stick */

    vm_buffer_destroy(ba);
    vm_buffer_destroy(bb);
    vm_buffer_destroy(br);
    vm_queue_destroy(q);
}

TEST_CASE(device_cpu_gemm_matches_vm_gemm, "[device]")
{
    enum { M = 5, N = 7, K = 3 };
    vm_queue *q = NULL;
    vm_float_t A[M * K], B[K * N], C0[M * N], want[M * N], got[M * N];

    fill(A, M * K, 3);
    fill(B, K * N, 4);
    fill(C0, M * N, 5);
    REQUIRE(vm_queue_create(cpu_dev(), &q) == VM_DEVICE_OK);

    vm_buffer *bA = upload(q, A, M * K);
    vm_buffer *bB = upload(q, B, K * N);
    vm_buffer *bC = upload(q, C0, M * N);
    REQUIRE(bA && bB && bC);

    memcpy(want, C0, sizeof want);
    vm_gemm(want, N, A, K, B, N, M, N, K, VM_F(1.5), VM_F(0.5), false, false, VM_LAYOUT_ROW_MAJOR);
    REQUIRE(vm_queue_gemm(q, DT, M, N, K, 1.5, bA, bB, 0.5, bC) == VM_DEVICE_OK);
    REQUIRE(vm_queue_read(q, bC, 0, got, sizeof got) == VM_DEVICE_OK);
    REQUIRE(vm_queue_finish(q) == VM_DEVICE_OK);
    REQUIRE(float_arrays_equal(want, got, (size_t)M * (size_t)N));

    /* k == 0 only scales C. */
    vm_gemm(want, N, A, K, B, N, M, N, 0, VM_F(1.0), VM_F(2.0), false, false, VM_LAYOUT_ROW_MAJOR);
    REQUIRE(vm_queue_gemm(q, DT, M, N, 0, 1.0, bA, bB, 2.0, bC) == VM_DEVICE_OK);
    REQUIRE(vm_queue_read(q, bC, 0, got, sizeof got) == VM_DEVICE_OK);
    REQUIRE(float_arrays_equal(want, got, (size_t)M * (size_t)N));

    REQUIRE(vm_queue_gemm(q, DT, 0, N, K, 1.0, bA, bB, 0.0, bC) == VM_DEVICE_OK);
    REQUIRE(vm_queue_gemm(q, DT, M, 0, K, 1.0, bA, bB, 0.0, bC) == VM_DEVICE_OK);
    REQUIRE(vm_queue_gemm(q, DT, -1, N, K, 1.0, bA, bB, 0.0, bC) == VM_DEVICE_ERR_INVAL);
    REQUIRE(vm_queue_gemm(q, DT, M, N, K + 1, 1.0, bA, bB, 0.0, bC) == VM_DEVICE_ERR_INVAL);
    REQUIRE(vm_queue_gemm(q, DT, M, N, K, 1.0, bA, bB, 0.0, bA) == VM_DEVICE_ERR_INVAL);
    /* m * n * sizeof overflows on 32-bit hosts and exceeds C on 64-bit ones. */
    REQUIRE(vm_queue_gemm(q, DT, INT_MAX, INT_MAX, 1, 1.0, bA, bB, 0.0, bC) == VM_DEVICE_ERR_INVAL);
    REQUIRE(vm_queue_gemm(q, DT, 1, 1, INT_MAX, 1.0, bA, bB, 0.0, bC) == VM_DEVICE_ERR_INVAL);
    REQUIRE(vm_queue_read(q, bC, 0, got, sizeof got) == VM_DEVICE_OK);
    REQUIRE(float_arrays_equal(want, got, (size_t)M * (size_t)N));  /* rejected calls wrote nothing */

    vm_buffer_destroy(bA);
    vm_buffer_destroy(bB);
    vm_buffer_destroy(bC);
    vm_queue_destroy(q);
}

TEST_CASE(device_cpu_rejects_misaligned_wrap, "[device]")
{
    vm_float_t store[DEV_N + 1];
    vm_buffer *w = NULL;
    vm_queue *q = NULL;

    fill(store, DEV_N + 1, 6);
    REQUIRE(vm_queue_create(cpu_dev(), &q) == VM_DEVICE_OK);
    REQUIRE(vm_buffer_wrap(cpu_dev(), (unsigned char *)store + 1, sizeof store - 1, &w) == VM_DEVICE_OK);
    REQUIRE(vm_queue_array_add(q, DT, w, w, w, 4) == VM_DEVICE_ERR_INVAL);
    vm_buffer_destroy(w);
    vm_queue_destroy(q);
}

/* ---- Registration and the fake device ---------------------------------- */

TEST_CASE(device_register_validates_table, "[device]")
{
    vm_device_ops ops = fake_ops;
    fake_ctx other = { .name = "test-other", .submits = 0, .queues = 0, .fail_next = 0 };
    fake_ctx empty = { .name = "", .submits = 0, .queues = 0, .fail_next = 0 };
    fake_ctx cpu_name = { .name = "cpu", .submits = 0, .queues = 0, .fail_next = 0 };
    const vm_device *f = fake_dev();
    const int count = vm_device_count();

    REQUIRE(f != NULL);
    REQUIRE(f != vm_device_get(0));
    REQUIRE(vm_device_register(&fake_ops, &fake_state) == VM_DEVICE_OK);  /* idempotent */
    REQUIRE(vm_device_register(NULL, &other) == VM_DEVICE_ERR_INVAL);

    ops.struct_size = sizeof(vm_device_ops) - 1;
    REQUIRE(vm_device_register(&ops, &other) == VM_DEVICE_ERR_INVAL);
    ops = fake_ops;
    ops.version = VM_DEVICE_OPS_VERSION + 1;
    REQUIRE(vm_device_register(&ops, &other) == VM_DEVICE_ERR_UNSUPPORTED);
    ops = fake_ops;
    ops.submit = NULL;
    REQUIRE(vm_device_register(&ops, &other) == VM_DEVICE_ERR_INVAL);
    ops = fake_ops;
    ops.queue_destroy = NULL;  /* half a pair */
    REQUIRE(vm_device_register(&ops, &other) == VM_DEVICE_ERR_INVAL);
    REQUIRE(vm_device_register(&fake_ops, &empty) == VM_DEVICE_ERR_INVAL);
    REQUIRE(vm_device_register(&fake_ops, &cpu_name) == VM_DEVICE_ERR_CONFLICT);
    REQUIRE(vm_device_count() == count);
    REQUIRE(vm_device_get(0) != NULL);

    /* Fill the table; indices stay valid and the next one is FULL. */
    {
        static char names[VM_DEVICE_MAX][16];
        static fake_ctx fill_ctx[VM_DEVICE_MAX];
        for (int i = 0; i < VM_DEVICE_MAX; ++i) {
            names[i][0] = 'f';
            names[i][1] = (char)('a' + i);
            names[i][2] = '\0';
            fill_ctx[i].name = names[i];
            const int rc = vm_device_register(&fake_ops, &fill_ctx[i]);
            REQUIRE(rc == VM_DEVICE_OK || rc == VM_DEVICE_ERR_FULL);
            REQUIRE(rc == VM_DEVICE_OK || vm_device_count() == VM_DEVICE_MAX);
        }
        REQUIRE(vm_device_count() == VM_DEVICE_MAX);
        REQUIRE(vm_device_get(VM_DEVICE_MAX - 1) != NULL);
        REQUIRE(fake_dev() == f);  /* still found, still idempotent */
    }
}

TEST_CASE(device_fake_unsupported_falls_back, "[device]")
{
    vm_device *f = fake_dev();
    vm_queue *q = NULL;
    vm_buffer *a = NULL, *b = NULL, *r = NULL, *c = NULL;
    float x[8], y[8], got[8];

    REQUIRE(f != NULL);
    for (int i = 0; i < 8; ++i) {
        x[i] = (float)i;
        y[i] = 0.5f * (float)i;
    }
    REQUIRE(vm_device_supports(f, VM_DEVICE_OP_ARRAY_ADD, VM_DTYPE_F32) == 1);
    REQUIRE(vm_device_supports(f, VM_DEVICE_OP_ARRAY_MUL, VM_DTYPE_F32) == 0);
    REQUIRE(vm_device_supports(f, VM_DEVICE_OP_ARRAY_ADD, VM_DTYPE_F64) == 0);

    REQUIRE(vm_queue_create(f, &q) == VM_DEVICE_OK);
    REQUIRE(fake_state.queues >= 1);
    REQUIRE(vm_buffer_create(f, sizeof x, &a) == VM_DEVICE_OK);
    REQUIRE(vm_buffer_create(f, sizeof y, &b) == VM_DEVICE_OK);
    REQUIRE(vm_buffer_create(f, sizeof got, &r) == VM_DEVICE_OK);
    REQUIRE(vm_buffer_wrap(f, got, sizeof got, &c) == VM_DEVICE_ERR_UNSUPPORTED && c == NULL);
    REQUIRE(vm_queue_write(q, a, 0, x, sizeof x) == VM_DEVICE_OK);
    REQUIRE(vm_queue_write(q, b, 0, y, sizeof y) == VM_DEVICE_OK);

    const int submits = fake_state.submits;
    REQUIRE(vm_queue_array_add(q, VM_DTYPE_F32, r, a, b, 8) == VM_DEVICE_OK);
    REQUIRE(fake_state.submits == submits + 1);
    REQUIRE(vm_queue_read(q, r, 0, got, sizeof got) == VM_DEVICE_OK);
    REQUIRE(vm_queue_finish(q) == VM_DEVICE_OK);
    for (int i = 0; i < 8; ++i)
        REQUIRE(got[i] == x[i] + y[i]);

    /* Unsupported ops never reach the backend; the caller falls back. */
    REQUIRE(vm_queue_array_mul(q, VM_DTYPE_F32, r, a, b, 8) == VM_DEVICE_ERR_UNSUPPORTED);
    REQUIRE(vm_queue_array_add(q, VM_DTYPE_F64, r, a, b, 1) == VM_DEVICE_ERR_UNSUPPORTED);
    REQUIRE(vm_queue_gemm(q, VM_DTYPE_F32, 1, 1, 1, 1.0, a, b, 0.0, r) == VM_DEVICE_ERR_UNSUPPORTED);
    REQUIRE(fake_state.submits == submits + 1);
    REQUIRE(vm_queue_read(q, a, 0, got, sizeof got) == VM_DEVICE_OK);
    for (int i = 0; i < 8; ++i)
        got[i] *= y[i];  /* the caller's host fallback */
    for (int i = 0; i < 8; ++i)
        REQUIRE(got[i] == x[i] * y[i]);

    /* Buffers cannot cross devices. */
    {
        vm_queue *cq = NULL;
        REQUIRE(vm_queue_create(cpu_dev(), &cq) == VM_DEVICE_OK);
        REQUIRE(vm_queue_read(cq, a, 0, got, 4) == VM_DEVICE_ERR_INVAL);
        REQUIRE(vm_queue_copy(q, r, 0, a, 0, 4) == VM_DEVICE_OK);
        vm_queue_destroy(cq);
    }

    /* A device failure is returned and sticks to the queue. */
    fake_state.fail_next = 1;
    REQUIRE(vm_queue_array_add(q, VM_DTYPE_F32, r, a, b, 8) == VM_DEVICE_ERR_DEVICE);
    REQUIRE(vm_queue_finish(q) == VM_DEVICE_ERR_DEVICE);
    REQUIRE(vm_queue_finish(q) == VM_DEVICE_ERR_DEVICE);

    vm_buffer_destroy(a);
    vm_buffer_destroy(b);
    vm_buffer_destroy(r);
    vm_queue_destroy(q);
    REQUIRE(fake_state.queues == 0);
}
