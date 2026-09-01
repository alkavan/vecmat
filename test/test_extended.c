// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <string.h>
#include <vecmat.h>
#include "unitest.h"

TEST_CASE(mat4_perspective_rh_no_matches_legacy, "[clip]") {
    const vm_float_t fov_deg = 90.0f;
    const vm_float_t aspect = 16.0f / 9.0f;
    const vm_float_t n = 0.1f;
    const vm_float_t f = 100.0f;
    const matrix4 legacy = mat4_perspective(deg_to_rad(fov_deg), aspect, n, f);
    const matrix4 legacy_deg = mat4_perspective_deg(fov_deg, aspect, n, f);
    const matrix4 clip = mat4_perspective_rh_no_deg(fov_deg, aspect, n, f);
    const matrix4 clip_rad = mat4_perspective_rh_no(deg_to_rad(fov_deg), aspect, n, f);
    REQUIRE(mat4_eq(legacy, clip));
    REQUIRE(mat4_eq(legacy, clip_rad));
    REQUIRE(mat4_eq(legacy, legacy_deg));
}

TEST_CASE(mat4_perspective_depth_conventions, "[clip]") {
    const vm_float_t fov = VM_RAD(M_PI_2);
    const matrix4 rh_no = mat4_perspective_rh_no(fov, 1.0f, 1.0f, 3.0f);
    const matrix4 rh_zo = mat4_perspective_rh_zo(fov, 1.0f, 1.0f, 3.0f);
    const matrix4 lh_zo = mat4_perspective_lh_zo(fov, 1.0f, 1.0f, 3.0f);
    const matrix4 lh_no = mat4_perspective_lh_no(fov, 1.0f, 1.0f, 3.0f);

    REQUIRE(VECMAT_EQ(rh_no.m43, VM_F(-1.0), EPSILON));
    REQUIRE(VECMAT_EQ(rh_zo.m43, VM_F(-1.0), EPSILON));
    REQUIRE(VECMAT_EQ(lh_zo.m43, VM_F(1.0), EPSILON));
    REQUIRE(VECMAT_EQ(lh_no.m43, VM_F(1.0), EPSILON));

    REQUIRE(VECMAT_EQ(rh_zo.m33, VM_F(-3.0) / VM_F(2.0), EPSILON));
    REQUIRE(VECMAT_EQ(lh_zo.m33, VM_F(3.0) / VM_F(2.0), EPSILON));
}

TEST_CASE(mat4_ortho_ptr_zeros_off_diagonals, "[clip]") {
    matrix4 dirty;
    for (int i = 0; i < 16; ++i) {
        dirty.v[i] = VM_F(7.0);
    }
    mat4_ortho_ptr(&dirty, -1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
    REQUIRE(VECMAT_EQ(dirty.m12, VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(dirty.m21, VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(dirty.m44, VM_F(1.0), EPSILON));
}

TEST_CASE(mat4_look_at_rh_matches_legacy, "[clip]") {
    const vector3 eye = { .x = 0.0f, .y = 0.0f, .z = 5.0f };
    const vector3 target = { .x = 0.0f, .y = 0.0f, .z = 0.0f };
    const vector3 up = { .x = 0.0f, .y = 1.0f, .z = 0.0f };
    REQUIRE(mat4_eq(mat4_look_at(eye, target, up), mat4_look_at_rh(eye, target, up)));
}

TEST_CASE(mat4_look_at_lh_looks_down_plus_z, "[clip]") {
    const vector3 eye = { .x = 0.0f, .y = 0.0f, .z = 0.0f };
    const vector3 target = { .x = 0.0f, .y = 0.0f, .z = 1.0f };
    const vector3 up = { .x = 0.0f, .y = 1.0f, .z = 0.0f };
    const matrix4 m = mat4_look_at_lh(eye, target, up);
    REQUIRE(VECMAT_EQ(m.m33, VM_F(1.0), EPSILON));
}

TEST_CASE(mat4_rotation_matches_deg, "[clip]") {
    const matrix4 a = mat4_rotation_z(VM_RAD(M_PI_2));
    const matrix4 b = mat4_rotation_z(VM_DEG(90));
    const matrix4 c = mat4_rotation_z_deg(VM_F(90.0));
    REQUIRE(mat4_eq(a, b));
    REQUIRE(mat4_eq(a, c));
}

TEST_CASE(vm_gemm_matches_mat4_mul, "[gemm]") {
    const matrix4 A = {
        .m11 = 1, .m21 = 2, .m31 = 0, .m41 = 0,
        .m12 = 3, .m22 = 4, .m32 = 0, .m42 = 0,
        .m13 = 0, .m23 = 0, .m33 = 1, .m43 = 0,
        .m14 = 0, .m24 = 0, .m34 = 0, .m44 = 1
    };
    const matrix4 B = {
        .m11 = 5, .m21 = 6, .m31 = 0, .m41 = 0,
        .m12 = 7, .m22 = 8, .m32 = 0, .m42 = 0,
        .m13 = 0, .m23 = 0, .m33 = 1, .m43 = 0,
        .m14 = 0, .m24 = 0, .m34 = 0, .m44 = 1
    };
    const matrix4 expect = mat4_mul(A, B);
    matrix4 C;
    for (int i = 0; i < 16; ++i) {
        C.v[i] = VM_F(99.0);
    }
    vm_gemm(C.v, 4, A.v, 4, B.v, 4, 4, 4, 4,
        VM_F(1.0), VM_F(0.0), false, false, VM_LAYOUT_COL_MAJOR);
    REQUIRE(mat4_eq(C, expect));
}

TEST_CASE(vm_gemm_beta_and_transpose, "[gemm]") {
    const vm_float_t A[4] = { 1, 2, 3, 4 }; /* row-major 2x2 */
    const vm_float_t B[4] = { 5, 6, 7, 8 };
    vm_float_t C[4] = { 1, 1, 1, 1 };
    /* C = 1 * A * B^T + 0 * C, row-major 2x2 */
    vm_gemm(C, 2, A, 2, B, 2, 2, 2, 2,
        VM_F(1.0), VM_F(0.0), false, true, VM_LAYOUT_ROW_MAJOR);
    /* A * B^T = [[1,2],[3,4]] * [[5,7],[6,8]] = [[17,23],[39,53]] */
    REQUIRE(VECMAT_EQ(C[0], VM_F(17.0), EPSILON));
    REQUIRE(VECMAT_EQ(C[1], VM_F(23.0), EPSILON));
    REQUIRE(VECMAT_EQ(C[2], VM_F(39.0), EPSILON));
    REQUIRE(VECMAT_EQ(C[3], VM_F(53.0), EPSILON));
}

TEST_CASE(quat_integrate_half_turn_z, "[physics]") {
    const quaternion q0 = quat_identity();
    const vector3 omega = { .x = 0.0f, .y = 0.0f, .z = M_PI };
    const quaternion q = quat_integrate(q0, omega, VM_F(1.0));
    const quaternion expect = quat_from_axis_angle((vector3){ .x = 0, .y = 0, .z = 1 }, VM_RAD(M_PI));
    REQUIRE(quat_near(q, expect, EPSILON) ||
            quat_near(q, (quaternion){ .x = -expect.x, .y = -expect.y, .z = -expect.z, .w = -expect.w }, EPSILON));

    const vector3 r = quat_rotate_vec3(q, (vector3){ .x = 1, .y = 0, .z = 0 });
    REQUIRE(vec3_near(r, (vector3){ .x = -1, .y = 0, .z = 0 }, EPSILON));
}

TEST_CASE(mat3_sym_eigen_diagonal, "[physics]") {
    const matrix3 m = {
        .m11 = 3, .m21 = 0, .m31 = 0,
        .m12 = 0, .m22 = 1, .m32 = 0,
        .m13 = 0, .m23 = 0, .m33 = 2
    };
    matrix3 axes;
    const vector3 ev = mat3_sym_eigen(m, &axes);
    REQUIRE(VECMAT_EQ(ev.x + ev.y + ev.z, VM_F(6.0), EPSILON));
    REQUIRE(VECMAT_EQ(ev.x * ev.y * ev.z, VM_F(6.0), EPSILON));
}

TEST_CASE(mat3_sym_eigen_reconstruction, "[physics]") {
    const matrix3 m = {
        .m11 = 2, .m21 = 1, .m31 = 0,
        .m12 = 1, .m22 = 2, .m32 = 1,
        .m13 = 0, .m23 = 1, .m33 = 2
    };
    matrix3 axes;
    const vector3 ev = mat3_sym_eigen(m, &axes);
    const matrix3 d = {
        .m11 = ev.x, .m21 = 0, .m31 = 0,
        .m12 = 0, .m22 = ev.y, .m32 = 0,
        .m13 = 0, .m23 = 0, .m33 = ev.z
    };
    const matrix3 recon = mat3_mul(mat3_mul(axes, d), mat3_transpose(axes));
    REQUIRE(VECMAT_EQ(recon.m11, m.m11, EPSILON));
    REQUIRE(VECMAT_EQ(recon.m22, m.m22, EPSILON));
    REQUIRE(VECMAT_EQ(recon.m33, m.m33, EPSILON));
    REQUIRE(VECMAT_EQ(recon.m12, m.m12, EPSILON));
    REQUIRE(VECMAT_EQ(recon.m23, m.m23, EPSILON));
}

TEST_CASE(vm_mat_lu_solve_2x2, "[dense]") {
    vm_mat A = vm_mat_alloc(2, 2);
    vm_mat_set(&A, 0, 0, 4);
    vm_mat_set(&A, 1, 0, 6);
    vm_mat_set(&A, 0, 1, 3);
    vm_mat_set(&A, 1, 1, 3);

    REQUIRE(VECMAT_EQ(vm_mat_det(&A), VM_F(-6.0), EPSILON));

    int pivot[2];
    REQUIRE(vm_lu_factor(&A, pivot, NULL));
    const vm_float_t b[2] = { 10, 12 };
    vm_float_t x[2];
    REQUIRE(vm_lu_solve(&A, pivot, b, x));
    /* 4x + 3y = 10, 6x + 3y = 12 => x = 1, y = 2 */
    REQUIRE(VECMAT_EQ(x[0], VM_F(1.0), EPSILON));
    REQUIRE(VECMAT_EQ(x[1], VM_F(2.0), EPSILON));
    vm_mat_free(&A);
}

TEST_CASE(vm_mat_inverse_roundtrip, "[dense]") {
    vm_mat A = vm_mat_alloc(3, 3);
    vm_mat_set(&A, 0, 0, 2); vm_mat_set(&A, 1, 0, 0); vm_mat_set(&A, 2, 0, 1);
    vm_mat_set(&A, 0, 1, 0); vm_mat_set(&A, 1, 1, 3); vm_mat_set(&A, 2, 1, 0);
    vm_mat_set(&A, 0, 2, 1); vm_mat_set(&A, 1, 2, 0); vm_mat_set(&A, 2, 2, 2);

    vm_mat inv = { 0 };
    REQUIRE(vm_mat_inverse(&inv, &A));

    vm_float_t I[9];
    vm_gemm(I, 3, A.data, 3, inv.data, 3, 3, 3, 3, VM_F(1.0), VM_F(0.0), false, false, VM_LAYOUT_COL_MAJOR);
    REQUIRE(VECMAT_EQ(I[0], VM_F(1.0), EPSILON));
    REQUIRE(VECMAT_EQ(I[4], VM_F(1.0), EPSILON));
    REQUIRE(VECMAT_EQ(I[8], VM_F(1.0), EPSILON));
    REQUIRE(VECMAT_EQ(I[1], VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(I[3], VM_F(0.0), EPSILON));

    vm_mat_free(&A);
    vm_mat_free(&inv);
}

TEST_CASE(vm_qr_reconstruct, "[solvers]") {
    vm_mat A = vm_mat_alloc(3, 2);
    vm_mat_set(&A, 0, 0, 1); vm_mat_set(&A, 1, 0, 1); vm_mat_set(&A, 2, 0, 1);
    vm_mat_set(&A, 0, 1, 1); vm_mat_set(&A, 1, 1, 2); vm_mat_set(&A, 2, 1, 3);

    vm_mat QR = { 0 };
    REQUIRE(vm_mat_copy(&QR, &A));
    vm_float_t tau[2];
    REQUIRE(vm_qr_factor(&QR, tau));

    vm_mat Q = { 0 };
    vm_mat R = { 0 };
    REQUIRE(vm_qr_unpack(&Q, &R, &QR, tau));

    vm_float_t recon[6];
    vm_gemm(recon, 3, Q.data, 3, R.data, 2, 3, 2, 2, VM_F(1.0), VM_F(0.0),
            false, false, VM_LAYOUT_COL_MAJOR);
    REQUIRE(VECMAT_EQ(recon[0], VM_F(1.0), EPSILON));
    REQUIRE(VECMAT_EQ(recon[1], VM_F(1.0), EPSILON));
    REQUIRE(VECMAT_EQ(recon[2], VM_F(1.0), EPSILON));
    REQUIRE(VECMAT_EQ(recon[3], VM_F(1.0), EPSILON));
    REQUIRE(VECMAT_EQ(recon[4], VM_F(2.0), EPSILON));
    REQUIRE(VECMAT_EQ(recon[5], VM_F(3.0), EPSILON));

    vm_mat_free(&A);
    vm_mat_free(&QR);
    vm_mat_free(&Q);
    vm_mat_free(&R);
}

TEST_CASE(vm_qr_solve_least_squares, "[solvers]") {
    vm_mat A = vm_mat_alloc(3, 2);
    vm_mat_set(&A, 0, 0, 1); vm_mat_set(&A, 1, 0, 0); vm_mat_set(&A, 2, 0, 0);
    vm_mat_set(&A, 0, 1, 0); vm_mat_set(&A, 1, 1, 1); vm_mat_set(&A, 2, 1, 0);

    vm_float_t tau[2];
    REQUIRE(vm_qr_factor(&A, tau));
    const vm_float_t b[3] = { 3, 4, 10 };
    vm_float_t x[2];
    REQUIRE(vm_qr_solve(&A, tau, b, x));
    REQUIRE(VECMAT_EQ(x[0], VM_F(3.0), EPSILON));
    REQUIRE(VECMAT_EQ(x[1], VM_F(4.0), EPSILON));
    vm_mat_free(&A);
}

TEST_CASE(vm_svd_reconstruct_tall, "[solvers]") {
    vm_mat A = vm_mat_alloc(3, 2);
    vm_mat_set(&A, 0, 0, 3); vm_mat_set(&A, 1, 0, 0); vm_mat_set(&A, 2, 0, 0);
    vm_mat_set(&A, 0, 1, 0); vm_mat_set(&A, 1, 1, 2); vm_mat_set(&A, 2, 1, 0);

    vm_mat U = { 0 };
    vm_mat V = { 0 };
    vm_float_t s[2];
    REQUIRE(vm_svd_factor(&A, &U, s, &V));
    REQUIRE(VECMAT_EQ(s[0], VM_F(3.0), EPSILON));
    REQUIRE(VECMAT_EQ(s[1], VM_F(2.0), EPSILON));

    vm_float_t US[6];
    for (int r = 0; r < 3; ++r) {
        US[r]     = vm_mat_get(&U, r, 0) * s[0];
        US[r + 3] = vm_mat_get(&U, r, 1) * s[1];
    }
    vm_float_t recon[6];
    vm_gemm(recon, 3, US, 3, V.data, 2, 3, 2, 2, VM_F(1.0), VM_F(0.0),
            false, true, VM_LAYOUT_COL_MAJOR);
    REQUIRE(VECMAT_EQ(recon[0], VM_F(3.0), EPSILON));
    REQUIRE(VECMAT_EQ(recon[4], VM_F(2.0), EPSILON));
    REQUIRE(VECMAT_EQ(recon[1], VM_F(0.0), EPSILON));

    vm_mat_free(&A);
    vm_mat_free(&U);
    vm_mat_free(&V);
}

TEST_CASE(vm_svd_wide_and_product, "[solvers]") {
    vm_mat A = vm_mat_alloc(2, 3);
    vm_mat_set(&A, 0, 0, 1); vm_mat_set(&A, 1, 0, 0);
    vm_mat_set(&A, 0, 1, 0); vm_mat_set(&A, 1, 1, 4);
    vm_mat_set(&A, 0, 2, 0); vm_mat_set(&A, 1, 2, 0);

    vm_mat U = { 0 };
    vm_mat V = { 0 };
    vm_float_t s[2];
    REQUIRE(vm_svd_factor(&A, &U, s, &V));
    REQUIRE(VECMAT_EQ(s[0], VM_F(4.0), EPSILON));
    REQUIRE(VECMAT_EQ(s[1], VM_F(1.0), EPSILON));
    REQUIRE(U.rows == 2 && U.cols == 2);
    REQUIRE(V.rows == 3 && V.cols == 2);

    vm_mat_free(&A);
    vm_mat_free(&U);
    vm_mat_free(&V);
}

TEST_CASE(vm_gemm_matches_ref_blocked, "[gemm]") {
    const int M = 17, N = 13, K = 11;
    vm_float_t A[17 * 11];
    vm_float_t B[11 * 13];
    vm_float_t C_fast[17 * 13];
    vm_float_t C_ref[17 * 13];
    for (int i = 0; i < M * K; ++i) {
        A[i] = (vm_float_t)(i % 7) - VM_F(3.0);
    }
    for (int i = 0; i < K * N; ++i) {
        B[i] = (vm_float_t)(i % 5) - VM_F(2.0);
    }
    for (int i = 0; i < M * N; ++i) {
        C_fast[i] = VM_F(0.25);
        C_ref[i] = VM_F(0.25);
    }
    vm_gemm(C_fast, M, A, M, B, K, M, N, K, VM_F(1.5), VM_F(0.5),
            false, false, VM_LAYOUT_COL_MAJOR);
    vm_gemm_ref(C_ref, M, A, M, B, K, M, N, K, VM_F(1.5), VM_F(0.5),
                false, false, VM_LAYOUT_COL_MAJOR);
    for (int i = 0; i < M * N; ++i) {
        REQUIRE(VECMAT_EQ(C_fast[i], C_ref[i], EPSILON));
    }
}

TEST_CASE(vm_gemm_matches_ref_row_major_trans, "[gemm]") {
    const vm_float_t A[6] = { 1, 2, 3, 4, 5, 6 };
    const vm_float_t B[6] = { 7, 8, 9, 10, 11, 12 };
    vm_float_t C_fast[4] = { 1, 2, 3, 4 };
    vm_float_t C_ref[4] = { 1, 2, 3, 4 };
    vm_gemm(C_fast, 2, A, 3, B, 2, 2, 2, 3, VM_F(2.0), VM_F(-1.0),
            false, true, VM_LAYOUT_ROW_MAJOR);
    vm_gemm_ref(C_ref, 2, A, 3, B, 2, 2, 2, 3, VM_F(2.0), VM_F(-1.0),
                false, true, VM_LAYOUT_ROW_MAJOR);
    for (int i = 0; i < 4; ++i) {
        REQUIRE(VECMAT_EQ(C_fast[i], C_ref[i], EPSILON));
    }
}

static void vm_gemm_fill(vm_float_t *M, const int n, const int stride, const int seed)
{
    for (int i = 0; i < n; ++i) {
        M[i] = (vm_float_t)((i * 3 + seed) % stride) - VM_F(3.0);
    }
}

static void vm_gemm_check_vs_ref(const int M, const int N, const int K,
                                 const vm_float_t alpha, const vm_float_t beta,
                                 const bool transA, const bool transB,
                                 const vm_layout_t layout)
{
    const int a_rows = transA ? K : M;
    const int a_cols = transA ? M : K;
    const int b_rows = transB ? N : K;
    const int b_cols = transB ? K : N;
    const int lda = (layout == VM_LAYOUT_ROW_MAJOR) ? a_cols : a_rows;
    const int ldb = (layout == VM_LAYOUT_ROW_MAJOR) ? b_cols : b_rows;
    const int ldc = (layout == VM_LAYOUT_ROW_MAJOR) ? N : M;
    const int a_n = a_rows * a_cols;
    const int b_n = b_rows * b_cols;
    const int c_n = M * N;

    vm_float_t *A = malloc((size_t)a_n * sizeof(vm_float_t));
    vm_float_t *B = malloc((size_t)b_n * sizeof(vm_float_t));
    vm_float_t *C_fast = malloc((size_t)c_n * sizeof(vm_float_t));
    vm_float_t *C_ref = malloc((size_t)c_n * sizeof(vm_float_t));
    REQUIRE(A && B && C_fast && C_ref);

    vm_gemm_fill(A, a_n, 7, 1);
    vm_gemm_fill(B, b_n, 5, 2);
    vm_gemm_fill(C_fast, c_n, 11, 3);
    memcpy(C_ref, C_fast, (size_t)c_n * sizeof(vm_float_t));

    vm_gemm(C_fast, ldc, A, lda, B, ldb, M, N, K, alpha, beta, transA, transB, layout);
    vm_gemm_ref(C_ref, ldc, A, lda, B, ldb, M, N, K, alpha, beta, transA, transB, layout);
    for (int i = 0; i < c_n; ++i) {
        REQUIRE(VECMAT_EQ(C_fast[i], C_ref[i], EPSILON));
    }

    free(A);
    free(B);
    free(C_fast);
    free(C_ref);
}

TEST_CASE(vm_gemm_tiny_uses_packed_kernel, "[gemm]") {
    vm_gemm_check_vs_ref(2, 2, 2, VM_F(1.0), VM_F(0.0), false, false, VM_LAYOUT_COL_MAJOR);
    vm_gemm_check_vs_ref(4, 4, 4, VM_F(1.0), VM_F(0.25), false, false, VM_LAYOUT_COL_MAJOR);
    vm_gemm_check_vs_ref(8, 7, 6, VM_F(1.5), VM_F(0.5), true, false, VM_LAYOUT_ROW_MAJOR);
}

TEST_CASE(vm_gemm_packed_shapes_and_transposes, "[gemm]") {
    /* Cache-block boundary and odd leftovers around the 8×8 micro-tile / 128 panel. */
    vm_gemm_check_vs_ref(19, 23, 29, VM_F(1.25), VM_F(0.5), false, false, VM_LAYOUT_COL_MAJOR);
    vm_gemm_check_vs_ref(65, 17, 13, VM_F(-0.5), VM_F(1.0), true, false, VM_LAYOUT_COL_MAJOR);
    vm_gemm_check_vs_ref(16, 64, 9, VM_F(2.0), VM_F(0.0), false, true, VM_LAYOUT_COL_MAJOR);
    vm_gemm_check_vs_ref(11, 10, 67, VM_F(0.75), VM_F(-1.0), true, true, VM_LAYOUT_COL_MAJOR);
    vm_gemm_check_vs_ref(18, 15, 14, VM_F(1.0), VM_F(0.25), false, false, VM_LAYOUT_ROW_MAJOR);
    vm_gemm_check_vs_ref(12, 21, 16, VM_F(1.5), VM_F(0.5), true, true, VM_LAYOUT_ROW_MAJOR);
}

TEST_CASE(vm_gemm_alpha_zero_and_k_zero, "[gemm]") {
    vm_gemm_check_vs_ref(12, 9, 10, VM_F(0.0), VM_F(0.5), false, false, VM_LAYOUT_COL_MAJOR);
    vm_gemm_check_vs_ref(12, 9, 0, VM_F(1.0), VM_F(2.0), false, false, VM_LAYOUT_COL_MAJOR);
    vm_gemm_check_vs_ref(12, 9, 0, VM_F(1.0), VM_F(0.0), false, false, VM_LAYOUT_ROW_MAJOR);
}

TEST_CASE(vm_gemm_batch_matches_ref, "[gemm]") {
    const int M = 9, N = 8, K = 11, batch = 3;
    vm_float_t A[3][9 * 11];
    vm_float_t B[3][11 * 8];
    vm_float_t C[3][9 * 8];
    vm_float_t C_ref[3][9 * 8];
    vm_float_t *Cp[3];
    const vm_float_t *Ap[3];
    const vm_float_t *Bp[3];

    for (int p = 0; p < batch; ++p) {
        vm_gemm_fill(A[p], M * K, 7, 10 + p);
        vm_gemm_fill(B[p], K * N, 5, 20 + p);
        vm_gemm_fill(C[p], M * N, 11, 30 + p);
        memcpy(C_ref[p], C[p], sizeof(C_ref[p]));
        Ap[p] = A[p];
        Bp[p] = B[p];
        Cp[p] = C[p];
    }

    vm_gemm_batch(Cp, M, Ap, M, Bp, K, M, N, K, VM_F(1.25), VM_F(0.25),
                  false, false, VM_LAYOUT_COL_MAJOR, batch);
    for (int p = 0; p < batch; ++p) {
        vm_gemm_ref(C_ref[p], M, A[p], M, B[p], K, M, N, K, VM_F(1.25), VM_F(0.25),
                    false, false, VM_LAYOUT_COL_MAJOR);
        for (int i = 0; i < M * N; ++i) {
            REQUIRE(VECMAT_EQ(C[p][i], C_ref[p][i], EPSILON));
        }
    }
}

TEST_CASE(vm_gemm_strided_batch_matches_ref, "[gemm]") {
    const int M = 7, N = 6, K = 10, batch = 4;
    const int strideA = M * K;
    const int strideB = K * N;
    const int strideC = M * N;
    vm_float_t A[4 * 7 * 10];
    vm_float_t B[4 * 10 * 6];
    vm_float_t C[4 * 7 * 6];
    vm_float_t C_ref[4 * 7 * 6];

    vm_gemm_fill(A, batch * strideA, 7, 4);
    vm_gemm_fill(B, batch * strideB, 5, 8);
    vm_gemm_fill(C, batch * strideC, 11, 12);
    memcpy(C_ref, C, sizeof(C));

    vm_gemm_strided_batch(C, M, strideC, A, M, strideA, B, K, strideB,
                          M, N, K, VM_F(0.5), VM_F(1.5),
                          false, false, VM_LAYOUT_COL_MAJOR, batch);
    for (int p = 0; p < batch; ++p) {
        vm_gemm_ref(C_ref + p * strideC, M, A + p * strideA, M, B + p * strideB, K,
                    M, N, K, VM_F(0.5), VM_F(1.5), false, false, VM_LAYOUT_COL_MAJOR);
    }
    for (int i = 0; i < batch * strideC; ++i) {
        REQUIRE(VECMAT_EQ(C[i], C_ref[i], EPSILON));
    }
}

TEST_CASE(vm_gemm_batch_shared_b, "[gemm]") {
    const int M = 10, N = 9, K = 12, batch = 4;
    vm_float_t A[4][10 * 12];
    vm_float_t B[12 * 9];
    vm_float_t C[4][10 * 9];
    vm_float_t C_ref[4][10 * 9];
    vm_float_t *Cp[4];
    const vm_float_t *Ap[4];
    const vm_float_t *Bp[4];

    vm_gemm_fill(B, K * N, 5, 2);
    for (int p = 0; p < batch; ++p) {
        vm_gemm_fill(A[p], M * K, 7, 10 + p);
        vm_gemm_fill(C[p], M * N, 11, 30 + p);
        memcpy(C_ref[p], C[p], sizeof(C_ref[p]));
        Ap[p] = A[p];
        Bp[p] = B;
        Cp[p] = C[p];
    }

    vm_gemm_batch(Cp, M, Ap, M, Bp, K, M, N, K, VM_F(1.0), VM_F(0.5),
                  false, false, VM_LAYOUT_COL_MAJOR, batch);
    for (int p = 0; p < batch; ++p) {
        vm_gemm_ref(C_ref[p], M, A[p], M, B, K, M, N, K, VM_F(1.0), VM_F(0.5),
                    false, false, VM_LAYOUT_COL_MAJOR);
        for (int i = 0; i < M * N; ++i) {
            REQUIRE(VECMAT_EQ(C[p][i], C_ref[p][i], EPSILON));
        }
    }

    vm_float_t Cs[4 * 10 * 9];
    vm_float_t As[4 * 10 * 12];
    for (int p = 0; p < batch; ++p) {
        memcpy(As + p * M * K, A[p], (size_t)M * K * sizeof(vm_float_t));
        memcpy(Cs + p * M * N, C_ref[p], (size_t)M * N * sizeof(vm_float_t));
        vm_gemm_fill(Cs + p * M * N, M * N, 11, 30 + p);
    }
    vm_gemm_strided_batch(Cs, M, M * N, As, M, M * K, B, K, 0,
                          M, N, K, VM_F(1.0), VM_F(0.5),
                          false, false, VM_LAYOUT_COL_MAJOR, batch);
    for (int p = 0; p < batch; ++p) {
        for (int i = 0; i < M * N; ++i) {
            REQUIRE(VECMAT_EQ(Cs[p * M * N + i], C_ref[p][i], EPSILON));
        }
    }
}

TEST_CASE(vm_gemm_batch_threaded_matches_ref, "[gemm]") {
    const int prev = vm_gemm_threads();
    vm_gemm_set_threads(2);
    REQUIRE(vm_gemm_threads() == 2);

    const int M = 32, N = 32, K = 32, batch = 4;
    vm_float_t A[4][32 * 32];
    vm_float_t B[32 * 32];
    vm_float_t C[4][32 * 32];
    vm_float_t C_ref[4][32 * 32];
    vm_float_t *Cp[4];
    const vm_float_t *Ap[4];
    const vm_float_t *Bp[4];

    vm_gemm_fill(B, K * N, 5, 2);
    for (int p = 0; p < batch; ++p) {
        vm_gemm_fill(A[p], M * K, 7, 10 + p);
        vm_gemm_fill(C[p], M * N, 11, 30 + p);
        memcpy(C_ref[p], C[p], sizeof(C_ref[p]));
        Ap[p] = A[p];
        Bp[p] = B;
        Cp[p] = C[p];
    }
    vm_gemm_batch(Cp, M, Ap, M, Bp, K, M, N, K, VM_F(1.0), VM_F(0.0),
                  false, false, VM_LAYOUT_COL_MAJOR, batch);
    for (int p = 0; p < batch; ++p) {
        vm_gemm_ref(C_ref[p], M, A[p], M, B, K, M, N, K, VM_F(1.0), VM_F(0.0),
                    false, false, VM_LAYOUT_COL_MAJOR);
        for (int i = 0; i < M * N; ++i) {
            REQUIRE(VECMAT_EQ(C[p][i], C_ref[p][i], EPSILON));
        }
    }

    vm_gemm_set_threads(1);
    REQUIRE(vm_gemm_threads() == 1);
    vm_gemm_set_threads(prev < 1 ? 0 : prev);
}

TEST_CASE(vm_gemm_ex_bias_relu, "[gemm]") {
    const int M = 5, N = 6, K = 7;
    vm_float_t A[5 * 7], B[7 * 6], C[5 * 6], C_ref[5 * 6], bias[6];
    vm_gemm_fill(A, M * K, 7, 1);
    vm_gemm_fill(B, K * N, 5, 2);
    vm_gemm_fill(C, M * N, 11, 3);
    memcpy(C_ref, C, sizeof(C));
    vm_gemm_fill(bias, N, 3, 4);

    vm_gemm_ex(C, M, A, M, B, K, M, N, K, VM_F(1.0), VM_F(0.25),
               false, false, VM_LAYOUT_COL_MAJOR, VM_GEMM_OP_BIAS_RELU, bias);
    vm_gemm_ref(C_ref, M, A, M, B, K, M, N, K, VM_F(1.0), VM_F(0.25),
                false, false, VM_LAYOUT_COL_MAJOR);
    for (int j = 0; j < N; ++j) {
        for (int i = 0; i < M; ++i) {
            vm_float_t v = C_ref[i + j * M] + bias[j];
            if (v < VM_F(0.0)) {
                v = VM_F(0.0);
            }
            REQUIRE(VECMAT_EQ(C[i + j * M], v, EPSILON));
        }
    }
}

TEST_CASE(vm_im2col_nchw_matches_manual, "[gemm]") {
    const int n = 1, c = 1, h = 3, w = 3, kh = 2, kw = 2, pad = 0, stride = 1;
    const int out_h = 2, out_w = 2;
    const vm_float_t img[9] = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };
    vm_float_t col[4 * 4];
    vm_im2col(col, 4, img, n, c, h, w, kh, kw, pad, pad, stride, stride,
              VM_LAYOUT_COL_MAJOR);
    /* columns are output pixels, rows are kernel taps */
    REQUIRE(VECMAT_EQ(col[0 + 0 * 4], VM_F(1.0), EPSILON));
    REQUIRE(VECMAT_EQ(col[1 + 0 * 4], VM_F(2.0), EPSILON));
    REQUIRE(VECMAT_EQ(col[2 + 0 * 4], VM_F(4.0), EPSILON));
    REQUIRE(VECMAT_EQ(col[3 + 0 * 4], VM_F(5.0), EPSILON));
    REQUIRE(VECMAT_EQ(col[0 + 3 * 4], VM_F(5.0), EPSILON));
    REQUIRE(VECMAT_EQ(col[3 + 3 * 4], VM_F(9.0), EPSILON));
    (void)out_h;
    (void)out_w;
}

TEST_CASE(vm_lu_tiny_and_large_scale, "[dense]") {
    const vm_float_t tiny = VM_F(1e-8);
    vm_mat A = vm_mat_alloc(2, 2);
    vm_mat_set(&A, 0, 0, VM_F(4.0) * tiny);
    vm_mat_set(&A, 1, 0, VM_F(6.0) * tiny);
    vm_mat_set(&A, 0, 1, VM_F(3.0) * tiny);
    vm_mat_set(&A, 1, 1, VM_F(3.0) * tiny);

    int pivot[2];
    REQUIRE(vm_lu_factor(&A, pivot, NULL));
    const vm_float_t b[2] = { VM_F(10.0) * tiny, VM_F(12.0) * tiny };
    vm_float_t x[2];
    REQUIRE(vm_lu_solve(&A, pivot, b, x));
    REQUIRE(VECMAT_EQ(x[0], VM_F(1.0), EPSILON));
    REQUIRE(VECMAT_EQ(x[1], VM_F(2.0), EPSILON));
    vm_mat_free(&A);

    const vm_float_t large = VM_F(1e5);
    A = vm_mat_alloc(2, 2);
    vm_mat_set(&A, 0, 0, VM_F(4.0) * large);
    vm_mat_set(&A, 1, 0, VM_F(6.0) * large);
    vm_mat_set(&A, 0, 1, VM_F(3.0) * large);
    vm_mat_set(&A, 1, 1, VM_F(3.0) * large);
    REQUIRE(vm_lu_factor(&A, pivot, NULL));
    const vm_float_t bl[2] = { VM_F(10.0) * large, VM_F(12.0) * large };
    REQUIRE(vm_lu_solve(&A, pivot, bl, x));
    REQUIRE(VECMAT_EQ(x[0], VM_F(1.0), EPSILON));
    REQUIRE(VECMAT_EQ(x[1], VM_F(2.0), EPSILON));
    vm_mat_free(&A);
}

TEST_CASE(vm_qr_tiny_scale, "[solvers]") {
    const vm_float_t tiny = VM_F(1e-8);
    vm_mat A = vm_mat_alloc(3, 2);
    vm_mat_set(&A, 0, 0, tiny); vm_mat_set(&A, 1, 0, VM_F(0.0)); vm_mat_set(&A, 2, 0, VM_F(0.0));
    vm_mat_set(&A, 0, 1, VM_F(0.0)); vm_mat_set(&A, 1, 1, tiny); vm_mat_set(&A, 2, 1, VM_F(0.0));
    vm_float_t tau[2];
    REQUIRE(vm_qr_factor(&A, tau));
    const vm_float_t b[3] = { VM_F(3.0) * tiny, VM_F(4.0) * tiny, VM_F(0.0) };
    vm_float_t x[2];
    REQUIRE(vm_qr_solve(&A, tau, b, x));
    REQUIRE(VECMAT_EQ(x[0], VM_F(3.0), EPSILON));
    REQUIRE(VECMAT_EQ(x[1], VM_F(4.0), EPSILON));
    vm_mat_free(&A);
}

TEST_CASE(vm_svd_tiny_scale, "[solvers]") {
    const vm_float_t tiny = VM_F(1e-8);
    vm_mat A = vm_mat_alloc(2, 2);
    vm_mat_set(&A, 0, 0, VM_F(3.0) * tiny);
    vm_mat_set(&A, 1, 0, VM_F(0.0));
    vm_mat_set(&A, 0, 1, VM_F(0.0));
    vm_mat_set(&A, 1, 1, VM_F(2.0) * tiny);
    vm_mat U = { 0 };
    vm_mat V = { 0 };
    vm_float_t s[2];
    REQUIRE(vm_svd_factor(&A, &U, s, &V));
    REQUIRE(VECMAT_EQ(s[0] / tiny, VM_F(3.0), EPSILON));
    REQUIRE(VECMAT_EQ(s[1] / tiny, VM_F(2.0), EPSILON));
    vm_mat_free(&A);
    vm_mat_free(&U);
    vm_mat_free(&V);
}

TEST_CASE(mat3_sym_eigen_tiny_scale, "[physics]") {
    const vm_float_t tiny = VM_F(1e-8);
    const matrix3 m = {
        .m11 = VM_F(3.0) * tiny, .m21 = 0, .m31 = 0,
        .m12 = 0, .m22 = VM_F(1.0) * tiny, .m32 = 0,
        .m13 = 0, .m23 = 0, .m33 = VM_F(2.0) * tiny
    };
    matrix3 axes;
    const vector3 ev = mat3_sym_eigen(m, &axes);
    REQUIRE(VECMAT_EQ((ev.x + ev.y + ev.z) / tiny, VM_F(6.0), EPSILON));
}

TEST_CASE(vm_deg_rad_literals, "[units]") {
    REQUIRE(VECMAT_EQ(VM_DEG(180), VM_RAD(M_PI), EPSILON));
    REQUIRE(VECMAT_EQ(VM_DEG(90), VM_RAD(M_PI_2), EPSILON));
    REQUIRE(mat2_eq(mat2_rotation_z(VM_RAD(M_PI_2)), mat2_rotation_z_deg(90.0f)));
    REQUIRE(mat3_eq(mat3_rotation_z(VM_RAD(M_PI_2)), mat3_rotation_z_deg(90.0f)));
    const quaternion a = quat_from_axis_angle((vector3){.x = 0, .y = 0, .z = 1}, VM_RAD(M_PI_2));
    const quaternion b = quat_from_axis_angle_deg((vector3){.x = 0, .y = 0, .z = 1}, 90.0f);
    REQUIRE(quat_near(a, b, EPSILON) ||
            quat_near(a, (quaternion){.x = -b.x, .y = -b.y, .z = -b.z, .w = -b.w}, EPSILON));
    const vector3 er = quat_to_euler(a);
    const vector3 ed = quat_to_euler_deg(a);
    REQUIRE(VECMAT_EQ(er.z, VM_RAD(M_PI_2), EPSILON));
    REQUIRE(VECMAT_EQ(ed.z, VM_F(90.0), EPSILON));
}

TEST_CASE(mat4_inverse_affine_rigid_and_scale, "[graphics]") {
    const vector3 t = { .x = VM_F(1.0), .y = VM_F(2.0), .z = VM_F(3.0) };
    const vector3 sc = { .x = VM_F(2.0), .y = VM_F(3.0), .z = VM_F(4.0) };
    const quaternion r = quat_from_axis_angle((vector3){ .x = 0, .y = 0, .z = 1 }, VM_RAD(M_PI_2));
    const matrix4 m = mat4_trs(t, r, sc);
    const matrix4 inv = mat4_inverse_affine(m);
    const matrix4 id = mat4_mul(m, inv);
    REQUIRE(mat4_eq(id, mat4_identity()) || (
        VECMAT_EQ(id.m11, VM_F(1.0), EPSILON) &&
        VECMAT_EQ(id.m22, VM_F(1.0), EPSILON) &&
        VECMAT_EQ(id.m33, VM_F(1.0), EPSILON) &&
        VECMAT_EQ(id.m44, VM_F(1.0), EPSILON) &&
        VECMAT_EQ(id.m14, VM_F(0.0), EPSILON) &&
        VECMAT_EQ(id.m24, VM_F(0.0), EPSILON) &&
        VECMAT_EQ(id.m34, VM_F(0.0), EPSILON)));

    const vector3 p = { .x = VM_F(1.0), .y = VM_F(0.0), .z = VM_F(0.0) };
    const vector3 world = mat4_mul_vec3(m, p, VM_F(1.0));
    const vector3 back = mat4_mul_vec3(inv, world, VM_F(1.0));
    REQUIRE(vec3_near(back, p, EPSILON));

    /* rigid-only: inverse_affine matches full inverse */
    const matrix4 rigid = mat4_mul(mat4_translate(t), quat_to_mat4(r));
    const matrix4 ainv = mat4_inverse_affine(rigid);
    const matrix4 finv = mat4_inverse(rigid);
    REQUIRE(mat4_eq(ainv, finv) || (
        VECMAT_EQ(ainv.m14, finv.m14, EPSILON) &&
        VECMAT_EQ(ainv.m24, finv.m24, EPSILON) &&
        VECMAT_EQ(ainv.m34, finv.m34, EPSILON)));
}

TEST_CASE(mat3_mat4_normal_nonuniform_scale, "[graphics]") {
    const matrix4 model = mat4_scale((vector3){ .x = VM_F(2.0), .y = VM_F(1.0), .z = VM_F(1.0) });
    const matrix3 n3 = mat3_normal(mat3_from_mat4(model));
    const matrix4 n4 = mat4_normal(model);
    /* inverse-transpose of diag(2,1,1) is diag(1/2, 1, 1) */
    REQUIRE(VECMAT_EQ(n3.m11, VM_F(0.5), EPSILON));
    REQUIRE(VECMAT_EQ(n3.m22, VM_F(1.0), EPSILON));
    REQUIRE(VECMAT_EQ(n3.m33, VM_F(1.0), EPSILON));
    REQUIRE(VECMAT_EQ(n4.m11, VM_F(0.5), EPSILON));
    REQUIRE(VECMAT_EQ(n4.m22, VM_F(1.0), EPSILON));
    REQUIRE(VECMAT_EQ(n4.m44, VM_F(1.0), EPSILON));

    /* a normal along +X should shrink; a tangent along +X should stretch */
    const vector3 n = mat3_mul_vec3(n3, (vector3){ .x = VM_F(1.0), .y = VM_F(0.0), .z = VM_F(0.0) });
    REQUIRE(vec3_near(n, (vector3){ .x = VM_F(0.5), .y = 0, .z = 0 }, EPSILON));
}

TEST_CASE(mat4_look_from_dir_matches_look_at, "[graphics]") {
    const vector3 eye = { .x = VM_F(1.0), .y = VM_F(2.0), .z = VM_F(3.0) };
    const vector3 dir = { .x = VM_F(0.0), .y = VM_F(0.0), .z = VM_F(-1.0) };
    const vector3 up = { .x = VM_F(0.0), .y = VM_F(1.0), .z = VM_F(0.0) };
    const vector3 target = vec3_add(eye, dir);
    const matrix4 a = mat4_look_from_dir(eye, dir, up);
    const matrix4 b = mat4_look_at(eye, target, up);
    REQUIRE(mat4_eq(a, b));
    const matrix4 lh_a = mat4_look_from_dir_lh(eye, dir, up);
    const matrix4 lh_b = mat4_look_at_lh(eye, target, up);
    REQUIRE(mat4_eq(lh_a, lh_b));
}

TEST_CASE(mat4_viewport_and_project_roundtrip, "[graphics]") {
    const matrix4 view = mat4_look_at(
        (vector3){ .x = 0, .y = 0, .z = VM_F(5.0) },
        (vector3){ .x = 0, .y = 0, .z = 0 },
        (vector3){ .x = 0, .y = VM_F(1.0), .z = 0 });
    const matrix4 proj = mat4_perspective(VM_RAD(M_PI_2), VM_F(1.0), VM_F(0.1), VM_F(100.0));
    const vector4 vp = { .x = 0, .y = 0, .z = VM_F(800.0), .w = VM_F(600.0) };
    const vector3 world = { .x = VM_F(0.0), .y = VM_F(0.0), .z = VM_F(0.0) };

    const vector3 win = vec3_world_to_window(world, view, proj, vp);
    REQUIRE(VECMAT_EQ(win.x, VM_F(400.0), VM_F(1e-3)));
    REQUIRE(VECMAT_EQ(win.y, VM_F(300.0), VM_F(1e-3)));

    const vector3 back = vec3_window_to_world(win, view, proj, vp);
    REQUIRE(vec3_near(back, world, VM_F(1e-4)));

    const matrix4 vp_m = mat4_viewport(0, 0, VM_F(800.0), VM_F(600.0));
    REQUIRE(VECMAT_EQ(vp_m.m11, VM_F(400.0), EPSILON));
    REQUIRE(VECMAT_EQ(vp_m.m14, VM_F(400.0), EPSILON));
    REQUIRE(VECMAT_EQ(vp_m.m22, VM_F(300.0), EPSILON));
    REQUIRE(VECMAT_EQ(vp_m.m24, VM_F(300.0), EPSILON));
}

TEST_CASE(mat4_infinite_reverse_z_preset, "[graphics]") {
    const vm_float_t fov = VM_RAD(M_PI_2);
    const vm_float_t n = VM_F(0.1);
    const matrix4 rz = mat4_infinite_reverse_z(fov, VM_F(1.0), n);
    REQUIRE(VECMAT_EQ(rz.m33, VM_F(0.0), EPSILON));
    REQUIRE(VECMAT_EQ(rz.m43, VM_F(-1.0), EPSILON));
    REQUIRE(VECMAT_EQ(rz.m34, n, EPSILON));

    /* RH view-space near plane is z_eye = -n: clip.z/clip.w → 1; infinity → 0 */
    const vector4 near_p = mat4_mul_vec4(rz, (vector4){ .x = 0, .y = 0, .z = -n, .w = VM_F(1.0) });
    const vm_float_t near_ndc = near_p.z / near_p.w;
    REQUIRE(VECMAT_EQ(near_ndc, VM_F(1.0), EPSILON));

    const vector4 far_p = mat4_mul_vec4(rz, (vector4){ .x = 0, .y = 0, .z = VM_F(-1.0e6), .w = VM_F(1.0) });
    const vm_float_t far_ndc = far_p.z / far_p.w;
    REQUIRE(far_ndc > VM_F(-0.01) && far_ndc < VM_F(0.01));

    const matrix4 inf_zo = mat4_perspective_infinite_clip(fov, VM_F(1.0), n, VM_CLIP_RH_ZO);
    REQUIRE(VECMAT_EQ(inf_zo.m33, VM_F(-1.0), EPSILON));
    REQUIRE(VECMAT_EQ(inf_zo.m34, -n, EPSILON));
    REQUIRE(VECMAT_EQ(inf_zo.m43, VM_F(-1.0), EPSILON));

    const matrix4 inf_no = mat4_perspective_infinite_clip(fov, VM_F(1.0), n, VM_CLIP_RH_NO);
    REQUIRE(mat4_eq(inf_no, mat4_perspective_infinite(fov, VM_F(1.0), n)));
}
