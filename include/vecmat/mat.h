// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_MAT_H
#define VECMAT_MAT_H

/**
 * @file
 * @brief Fixed-size matrices.
 *
 * Products are column-major `AB` with the column vector on the right. A
 * by-value `matrix4` copies 64 bytes (f32) or 128 bytes (f64); hot loops
 * should use `_ptr`.
 *
 * @ingroup vecmat_core
 */

#include "vecmat/types.h"
#include "vecmat/abi.h"

/* ---- matrix2 ------------------------------------------------------------- */

/**
 * @brief Constructs the 2x2 identity matrix.
 *
 * @see mat2_identity_ptr
 *
 * @return The identity matrix2.
 */
VEC_API matrix2 mat2_identity(void);

/**
 * @brief Multiplies two 2x2 matrices (a * b).
 *
 * @see mat2_mul_ptr
 *
 * @param a The first matrix.
 * @param b The second matrix.
 * @return The resulting product matrix2.
 *
 * Column-major product `AB` so `(AB)v == A(Bv)` (column vector on the right).
 */
VEC_API matrix2 mat2_mul(matrix2 a, matrix2 b);

/**
 * @brief Computes the transpose of a 2x2 matrix.
 *
 * @see mat2_transpose_ptr
 *
 * @param m The input matrix.
 * @return The transposed matrix2.
 */
VEC_API matrix2 mat2_transpose(matrix2 m);

/**
  * @brief Computes the inverse of a 2x2 matrix.
  *
  * @see mat2_inverse_ptr
  *
  * @param m The input matrix.
  * @return The inverse matrix2.
  */
VEC_API matrix2 mat2_inverse(matrix2 m);

/**
 * @brief Constructs a 2x2 rotation matrix around the Z axis.
 *
 * @see mat2_rotation_z_ptr
 *
 * @param radians Rotation angle in radians.
 * @return The rotation matrix2.
 */
VEC_API matrix2 mat2_rotation_z(vm_float_t radians);

/**
 * @brief Constructs a 2x2 rotation matrix from an angle in degrees around the Z axis.
 *
 * @param degrees The rotation angle in degrees.
 * @return The rotation matrix2.
 */
VEC_API matrix2 mat2_rotation_z_deg(vm_float_t degrees);

/**
 * @brief Builds a 2x2 scaling matrix from a vector2.
 *
 * @see mat2_scale_ptr
 *
 * @param s Scale vector.
 * @return The resulting matrix2.
 */
VEC_API matrix2 mat2_scale(vector2 s);

/**
 * @brief Copies the upper-left 2x2 of a matrix3.
 *
 * @see mat2_from_mat3_ptr
 *
 * @param m Input matrix.
 * @return The resulting matrix2.
 */
VEC_API matrix2 mat2_from_mat3(matrix3 m);

/**
 * @brief Multiplies a 2x2 matrix by a vector2.
 *
 * @see mat2_mul_vec2_ptr
 *
 * @param m Input matrix.
 * @param v Input vector.
 * @return The resulting vector2.
 */
VEC_API vector2 mat2_mul_vec2(matrix2 m, vector2 v);

/**
 * @brief Calculates the determinant of the given 2x2 matrix.
 *
 * @param m The matrix.
 * @return The determinant value (m.v[0]*m.v[3] - m.v[1]*m.v[2]).
 */
VEC_API vm_float_t mat2_determinant(matrix2 m);

/* ---- matrix3 ------------------------------------------------------------- */

/**
 * @brief Constructs the 3x3 identity matrix.
 *
 * @see mat3_identity_ptr
 *
 * @return The identity matrix3.
 */
VEC_API matrix3 mat3_identity(void);

/**
 * @brief Multiplies two 3x3 matrices.
 *
 * @see mat3_mul_ptr
 *
 * @param a The first matrix.
 * @param b The second matrix.
 * @return The product matrix.
 *
 * Column-major product `AB` so `(AB)v == A(Bv)` (column vector on the right).
 */
VEC_API matrix3 mat3_mul(matrix3 a, matrix3 b);

/**
 * @brief Computes the transpose of a 3x3 matrix.
 *
 * @see mat3_transpose_ptr
 *
 * @param m The input matrix.
 * @return The transposed matrix.
 */
VEC_API matrix3 mat3_transpose(matrix3 m);

/**
 * @brief Computes the inverse of a 3x3 matrix.
 *
 * @see mat3_inverse_ptr
 *
 * @param m The input matrix.
 * @return The inverse matrix.
 */
VEC_API matrix3 mat3_inverse(matrix3 m);

/**
 * @brief Constructs a 3x3 rotation matrix around the X axis.
 *
 * @see mat3_rotation_x_ptr
 *
 * @param radians Rotation angle in radians.
 * @return The rotation matrix3.
 */
VEC_API matrix3 mat3_rotation_x(vm_float_t radians);

/**
 * @brief Constructs a 3x3 rotation matrix around the Y axis.
 *
 * @see `mat3_rotation_y_ptr`
 *
 * @param radians The rotation angle in radians.
 * @return The rotation matrix3.
 */
VEC_API matrix3 mat3_rotation_y(vm_float_t radians);

/**
 * @brief Constructs a 3x3 rotation matrix around the Z axis.
 *
 * @see mat3_rotation_z_ptr
 *
 * @param radians Rotation angle in radians.
 * @return The rotation matrix3.
 */
VEC_API matrix3 mat3_rotation_z(vm_float_t radians);

/**
 * @brief Constructs a 3x3 rotation matrix around the X axis.
 *
 * The angle is provided in degrees and converted to radians internally.
 *
 * @param degrees The rotation angle in degrees.
 * @return The rotation matrix3.
 */
VEC_API matrix3 mat3_rotation_x_deg(vm_float_t degrees);

/**
 * @brief 3x3 rotation about Y from an angle in degrees.
 *
 * @see mat3_rotation_y_ptr
 *
 * @param degrees Rotation angle in degrees.
 * @return Rotation matrix3.
 */
VEC_API matrix3 mat3_rotation_y_deg(vm_float_t degrees);

/**
 * @brief Constructs a 3x3 rotation matrix around the Z axis.
 *
 * The angle is given in degrees and internally converted to radians.
 *
 * @param degrees The rotation angle in degrees.
 * @return The resulting rotation matrix3.
 */
VEC_API matrix3 mat3_rotation_z_deg(vm_float_t degrees);

/**
 * @brief Builds a 3x3 2D translation matrix.
 *
 * @see mat3_translate_ptr
 *
 * @param t Translation vector.
 * @return The resulting matrix3.
 */
VEC_API matrix3 mat3_translate(vector2 t);

/**
 * @brief Builds a 3x3 2D scaling matrix.
 *
 * @see mat3_scale_ptr
 *
 * @param s Scale vector.
 * @return The resulting matrix3.
 */
VEC_API matrix3 mat3_scale(vector2 s);

/**
 * @brief Inverse-transpose of a 3x3 (normal matrix).
 *
 * @see mat3_normal_ptr
 *
 * @param m Linear part of a model transform.
 * @return The normal matrix3.
 */
VEC_API matrix3 mat3_normal(matrix3 m);

/**
 * @brief Copies the upper-left 3x3 of a matrix4.
 *
 * @see mat3_from_mat4_ptr
 *
 * @param m Input matrix.
 * @return The resulting matrix3.
 */
VEC_API matrix3 mat3_from_mat4(matrix4 m);

/**
 * @brief Multiplies a 3x3 matrix by a vector3.
 *
 * @see mat3_mul_vec3_ptr
 *
 * @param m Input matrix.
 * @param v Input vector.
 * @return The resulting vector3.
 */
VEC_API vector3 mat3_mul_vec3(matrix3 m, vector3 v);

/**
 * @brief Symmetric 3x3 eigensolve (Jacobi). `m` is symmetrized as (A+A^T)/2.
 *
 * Eigenvalues are unsorted principal moments. Eigenvectors are the columns
 * of the returned rotation (`axes * diag(moments) * axes^T ~= m`).
 *
 * @see mat3_sym_eigen_ptr
 *
 * @param m Input matrix (symmetrized internally).
 * @param axes Optional; receives eigenvector columns. May be NULL.
 * @return Eigenvalues as a vector3.
 */
VEC_API vector3 mat3_sym_eigen(matrix3 m, matrix3 *axes);

/**
 * @brief Applies a 3x3 affine transform to a vector2.
 *
 * @see mat3_mul_vec2_ptr
 *
 * @param m Input matrix.
 * @param v Input vector.
 * @return The resulting vector2.
 */
VEC_API vector2 mat3_mul_vec2(matrix3 m, vector2 v);

/**
 * @brief Computes the determinant of a 3x3 matrix.
 *
 * @param m The input matrix.
 * @return The determinant value.
 */
VEC_API vm_float_t mat3_determinant(matrix3 m);

/* ---- matrix4 ------------------------------------------------------------- */

/**
 * @brief Constructs the 4x4 identity matrix.
 *
 * @see mat4_identity_ptr
 *
 * @return The identity matrix4.
 */
VEC_API matrix4 mat4_identity(void);

/**
 * @brief Multiplies two 4x4 matrices (a * b).
 *
 * @see mat4_mul_ptr
 *
 * @param a The first matrix.
 * @param b The second matrix.
 * @return The resulting product matrix4.
 *
 * Column-major product `AB` so `(AB)v == A(Bv)` (column vector on the right).
 */
VEC_API matrix4 mat4_mul(matrix4 a, matrix4 b);

/**
 * @brief Computes the transpose of a 4x4 matrix.
 *
 * @see mat4_transpose_ptr
 *
 * @param m The input matrix.
 * @return The transposed matrix4.
 */
VEC_API matrix4 mat4_transpose(matrix4 m);

/**
 * @brief Computes the inverse of a 4x4 matrix.
 *
 * @see mat4_inverse_ptr
 *
 * @param m The input matrix.
 * @return The inverse matrix4.
 */
VEC_API matrix4 mat4_inverse(matrix4 m);

/**
 * @brief Inverse of an affine matrix `[R t; 0 1]`.
 *
 * @see mat4_inverse_affine_ptr
 *
 * @param m Affine 4×4 matrix.
 * @return Inverse matrix.
 *
 * Inverts an `[A t; 0 1]` affine transform without the full 4x4 adjugate.
 */
VEC_API matrix4 mat4_inverse_affine(matrix4 m);

/**
 * @brief Constructs a translation matrix from the given vector.
 *
 * @see mat4_translate_ptr
 *
 * @param v The translation vector (x, y, z).
 * @return The translation matrix4.
 */
VEC_API matrix4 mat4_translate(vector3 v);

/**
 * @brief Constructs a scaling matrix from the given scale vector.
 *
 * @see mat4_scale_ptr
 *
 * @param v The scale vector (x, y, z factors).
 * @return The resulting scaling matrix4.
 */
VEC_API matrix4 mat4_scale(vector3 v);

/**
 * @brief Inverse-transpose of the upper 3x3, embedded in a matrix4.
 *
 * @see mat4_normal_ptr
 *
 * @param m Model matrix.
 * @return Normal matrix as a matrix4.
 */
VEC_API matrix4 mat4_normal(matrix4 m);

/**
 * @brief Constructs a 4x4 rotation matrix.
 *
 * @see mat4_rotation_ptr
 *
 * @param axis The rotation axis.
 * @param radians The rotation angle in radians.
 * @return The rotation matrix4.
 */
VEC_API matrix4 mat4_rotation(vector3 axis, vm_float_t radians);

/**
 * @brief Constructs a 4x4 rotation matrix around the X axis.
 *
 * @see mat4_rotation_x_ptr
 *
 * @param radians Rotation angle in radians.
 * @return The rotation matrix4.
 */
VEC_API matrix4 mat4_rotation_x(vm_float_t radians);

/**
 * @brief Constructs a 4x4 rotation matrix around the Y axis.
 *
 * @see mat4_rotation_y_ptr
 *
 * @param radians The rotation angle in radians.
 * @return The rotation matrix4.
 */
VEC_API matrix4 mat4_rotation_y(vm_float_t radians);

/**
 * @brief Constructs a 4x4 rotation matrix around the Z axis.
 *
 * @see mat4_rotation_z_ptr
 *
 * @param radians Rotation angle in radians.
 * @return The rotation matrix4.
 */
VEC_API matrix4 mat4_rotation_z(vm_float_t radians);

/**
 * @brief Constructs a 4x4 rotation matrix from an axis and angle in degrees.
 *
 * @param axis The rotation axis.
 * @param degrees The rotation angle in degrees.
 *
 * @return The rotation matrix4.
 */
VEC_API matrix4 mat4_rotation_deg(vector3 axis, vm_float_t degrees);

/**
 * @brief Constructs a 4x4 rotation matrix around the X axis.
 *
 * The angle is specified in degrees and internally converted to radians.
 *
 * @param degrees The rotation angle in degrees.
 * @return The rotation matrix4.
 */
VEC_API matrix4 mat4_rotation_x_deg(vm_float_t degrees);

/**
 * @brief Constructs a 4x4 rotation matrix around the Y axis.
 *
 * @param degrees Rotation angle in degrees.
 * @return The rotation matrix4.
 */
VEC_API matrix4 mat4_rotation_y_deg(vm_float_t degrees);

/**
 * @brief Constructs a 4x4 rotation matrix around the Z axis from an angle in degrees.
 *
 * @param degrees The rotation angle in degrees.
 *
 * @return The rotation matrix4.
 */
VEC_API matrix4 mat4_rotation_z_deg(vm_float_t degrees);

/**
 * @brief Builds a 4x4 TRS matrix from translation, rotation, and scale.
 *
 * @see mat4_trs_ptr
 *
 * @param translation Translation vector.
 * @param rotation Rotation quaternion.
 * @param scale Scale vector.
 *
 * @return The resulting matrix4.
 */
VEC_API matrix4 mat4_trs(vector3 translation, quaternion rotation, vector3 scale);

/**
 * @brief Embeds a matrix3 into the upper-left of a matrix4.
 *
 * @see mat4_from_mat3_ptr
 *
 * @param m Input matrix.
 * @return The resulting matrix4.
 */
VEC_API matrix4 mat4_from_mat3(matrix3 m);

/**
 * @brief Calculates the determinant of the given 4x4 matrix.
 *
 * Uses cofactor expansion for computation.
 *
 * @param m The matrix.
 *
 * @return The determinant value as a vm_float_t.
 */
VEC_API vm_float_t mat4_determinant(matrix4 m);

/*******************************************************************************
 * Vector/Quaternion helpers for `matrix4`
 ******************************************************************************/

/**
 * @brief Extracts the translation vector from a matrix4.
 *
 * @see mat4_extract_translation_ptr
 *
 * @param m Input matrix.
 * @return The resulting vector3.
 */
VEC_API vector3 mat4_extract_translation(matrix4 m);

/**
 * @brief Extracts the scale vector from a matrix4.
 *
 * @see mat4_extract_scale_ptr
 *
 * @param m Input matrix.
 * @return The resulting vector3.
 */
VEC_API vector3 mat4_extract_scale(matrix4 m);

/**
 * @brief Transforms a vector3 by a 4x4 matrix using homogeneous w.
 *
 * @see mat4_mul_vec3_ptr
 *
 * @param m Input matrix.
 * @param v Input vector.
 * @param w Homogeneous w component.
 *
 * @return The resulting vector3.
 */
VEC_API vector3 mat4_mul_vec3(matrix4 m, vector3 v, vm_float_t w);

/**
 * @brief Multiplies a 4x4 matrix by a vector4.
 *
 * @see mat4_mul_vec4_ptr
 *
 * @param m Input matrix.
 * @param v Input vector.
 *
 * @return The resulting vector4.
 */
VEC_API vector4 mat4_mul_vec4(matrix4 m, vector4 v);

/**
 * @brief Extracts the rotation quaternion from a matrix4.
 *
 * @see mat4_extract_rotation_ptr
 *
 * @param m Input matrix.
 * @return The resulting quaternion.
 */
VEC_API quaternion mat4_extract_rotation(matrix4 m);

/*******************************************************************************
 * Floating-point matrix pointer-based performance functions
 ******************************************************************************/

/* ---- matrix2 pointer-based operations ------------------------------------ */

/**
 * @brief Initializes the 2x2 matrix to identity (diagonal 1.0, others 0.0).
 *
 * @param res Pointer to the output matrix2.
 */
VEC_API void mat2_identity_ptr(matrix2 *res);

/**
 * @brief Multiplies two 2x2 matrices (a * b) in column-major / column-vector convention.
 *
 * @param res Pointer to the output matrix2.
 * @param a Pointer to the first matrix.
 * @param b Pointer to the second matrix.
 *
 * Column-major product `AB` so `(AB)v == A(Bv)` (column vector on the right).
 */
VEC_API void mat2_mul_ptr(matrix2 *res, const matrix2 *a, const matrix2 *b);

/**
 * @brief Computes the transpose of the input 2x2 matrix and stores in res.
 *
 * @param res Pointer to the output matrix2.
 * @param m Pointer to the input matrix.
 */
VEC_API void mat2_transpose_ptr(matrix2 *res, const matrix2 *m);

/**
 * @brief Computes the inverse of the input 2x2 matrix using determinant and stores in res.
 *
 * If the determinant is zero, sets res to identity matrix.
 *
 * @param res Pointer to the output matrix2.
 * @param m Pointer to the input matrix.
 */
VEC_API void mat2_inverse_ptr(matrix2 *res, const matrix2 *m);

/**
 * @brief Sets the 2x2 matrix to a Z-axis (counter-clockwise) rotation.
 *
 * @param res Pointer to the output matrix2.
 * @param radians Rotation angle in radians.
 */
VEC_API void mat2_rotation_z_ptr(matrix2 *res, vm_float_t radians);

/**
 * @brief Initializes the 2x2 matrix to a rotation around Z axis (counter-clockwise).
 *
 * @param res Pointer to the output matrix2.
 * @param degrees Rotation angle in degrees.
 */
VEC_API void mat2_rotation_z_deg_ptr(matrix2 *res, vm_float_t degrees);

/**
 * @brief Builds a 2x2 scaling matrix from a vector2.
 *
 * @param res Output value.
 * @param s Scale vector.
 */
VEC_API void mat2_scale_ptr(matrix2 *res, const vector2 *s);

/**
 * @brief Copies the upper-left 2x2 of a matrix3.
 *
 * @param res Output value.
 * @param m Input matrix.
 */
VEC_API void mat2_from_mat3_ptr(matrix2 *res, const matrix3 *m);

/**
 * @brief Multiplies a 2x2 matrix by a vector2.
 *
 * @param res Output value.
 * @param m Input matrix.
 * @param v Input vector.
 */
VEC_API void mat2_mul_vec2_ptr(vector2 *res, const matrix2 *m, const vector2 *v);

/* ---- matrix3 pointer-based operations ------------------------------------ */

/**
 * @brief Initializes the 3x3 matrix to identity (diagonal 1.0, others 0.0).
 *
 * @param res Pointer to the output matrix3.
 */
VEC_API void mat3_identity_ptr(matrix3 *res);

/**
 * @brief Multiplies two 3x3 matrices (a * b) in column-major / column-vector convention.
 *
 * Accumulates into a temporary matrix.
 *
 * @param res Pointer to the output matrix3.
 * @param a Pointer to the first matrix.
 * @param b Pointer to the second matrix.
 *
 * Column-major product `AB` so `(AB)v == A(Bv)` (column vector on the right).
 */
VEC_API void mat3_mul_ptr(matrix3 *res, const matrix3 *a, const matrix3 *b);

/**
 * @brief Computes the transpose of the input 3x3 matrix and stores in res.
 *
 * @param res Pointer to the output matrix3.
 * @param m Pointer to the input matrix.
 */
VEC_API void mat3_transpose_ptr(matrix3 *res, const matrix3 *m);

/**
 * @brief Computes the inverse of the input 3x3 matrix using the adjugate method
 * and stores in res.
 *
 * If the determinant is zero, sets res to identity.
 *
 * @param res Pointer to the output matrix3.
 * @param m Pointer to the input matrix.
 */
VEC_API void mat3_inverse_ptr(matrix3 *res, const matrix3 *m);

/**
 * @brief Sets the 3x3 matrix to a rotation around the X axis.
 *
 * @param res Pointer to the output matrix3.
 * @param radians Rotation angle in radians.
 */
VEC_API void mat3_rotation_x_ptr(matrix3 *res, vm_float_t radians);

/**
 * @brief Sets the 3x3 matrix to a rotation around the Y axis.
 *
 * @param res Pointer to the output matrix3.
 * @param radians Rotation angle in radians.
 */
VEC_API void mat3_rotation_y_ptr(matrix3 *res, vm_float_t radians);

/**
 * @brief Sets the 3x3 matrix to a Z-axis rotation by the given angle in radians.
 *
 * @param res Pointer to the output matrix3.
 * @param radians Rotation angle in radians.
 */
VEC_API void mat3_rotation_z_ptr(matrix3 *res, vm_float_t radians);

/**
 * @brief Initializes the 3x3 matrix to a rotation around the X axis.
 *
 * @see mat3_rotation_x_ptr
 *
 * @param res Pointer to the output matrix3.
 * @param degrees Rotation angle in degrees.
 */
VEC_API void mat3_rotation_x_deg_ptr(matrix3 *res, vm_float_t degrees);

/**
 * @brief Initializes the 3x3 matrix to a rotation around the Y axis.
 *
 * @see mat3_rotation_y_ptr
 *
 * @param res Pointer to the output matrix3.
 * @param degrees Rotation angle in degrees.
 */
VEC_API void mat3_rotation_y_deg_ptr(matrix3 *res, vm_float_t degrees);

/**
 * @brief Initializes the 3x3 matrix to a rotation around the Z axis.
 *
 * @see mat3_rotation_z_ptr
 *
 * @param res Pointer to the output matrix3.
 * @param degrees Rotation angle in degrees.
 */
VEC_API void mat3_rotation_z_deg_ptr(matrix3 *res, vm_float_t degrees);

/**
 * @brief Builds a 3x3 2D translation matrix.
 *
 * @param res Output value.
 * @param t Translation vector.
 */
VEC_API void mat3_translate_ptr(matrix3 *res, const vector2 *t);

/**
 * @brief Builds a 3x3 2D scaling matrix.
 *
 * @param res Output value.
 * @param s Scale vector.
 */
VEC_API void mat3_scale_ptr(matrix3 *res, const vector2 *s);

/**
 * @brief Copies the upper-left 3x3 of a matrix4.
 *
 * @param res Output value.
 * @param m Input matrix.
 */
VEC_API void mat3_from_mat4_ptr(matrix3 *res, const matrix4 *m);

/**
 * @brief Inverse-transpose of a 3x3, for transforming normals.
 *
 * If @p m is singular the result is identity.
 *
 * @param res Normal matrix.
 * @param m Linear part of a model transform.
 */
VEC_API void mat3_normal_ptr(matrix3 *res, const matrix3 *m);

/**
 * @brief Multiplies a 3x3 matrix by a vector3.
 *
 * @param res Output value.
 * @param m Input matrix.
 * @param v Input vector.
 */
VEC_API void mat3_mul_vec3_ptr(vector3 *res, const matrix3 *m, const vector3 *v);

/**
 * @brief Applies a 3x3 affine transform to a vector2.
 *
 * @param res Output value.
 * @param m Input matrix.
 * @param v Input vector.
 */
VEC_API void mat3_mul_vec2_ptr(vector2 *res, const matrix3 *m, const vector2 *v);

/**
 * @brief Jacobi eigensolve of a symmetric 3x3 matrix.
 *
 * `m` is first replaced by `(m + m^T) / 2`. Eigenvalues are the diagonal of
 * the rotated matrix; eigenvector `i` is column `i` of `axes`.
 *
 * @param eigenvalues Output eigenvalues.
 * @param axes Output eigenvector columns.
 * @param m Input matrix (copied and symmetrized).
 */
VEC_API void mat3_sym_eigen_ptr(vector3 *eigenvalues, matrix3 *axes, const matrix3 *m);

/* ---- matrix4 pointer-based operations ------------------------------------ */

/**
 * @brief Sets the matrix to the identity matrix.
 *
 * The identity matrix is a 4x4 matrix with 1s on the main diagonal and 0s elsewhere.
 *
 * @param res Pointer to the matrix4 to set to identity.
 */
VEC_API void mat4_identity_ptr(matrix4 *res);

/**
 * @brief Multiplies two 4x4 matrices (dispatched).
 *
 * @see mat4_mul_ptr_scalar
 *
 * @param res Result matrix.
 * @param a Left matrix.
 * @param b Right matrix.
 *
 * Column-major product `AB` so `(AB)v == A(Bv)` (column vector on the right).
 */
VEC_API void mat4_mul_ptr(matrix4 *res, const matrix4 *a, const matrix4 *b);

/**
 * @brief Transposes a 4x4 matrix (dispatched).
 *
 * @see mat4_transpose_ptr_scalar
 *
 * @param res Result matrix.
 * @param m Input matrix.
 */
VEC_API void mat4_transpose_ptr(matrix4 *res, const matrix4 *m);

/**
 * @brief Computes the inverse of a 4x4 matrix.
 *
 * This function calculates the inverse of the given 4x4 matrix using the
 * adjugate matrix and determinant. If the matrix is singular (determinant is
 * zero), the result is set to the identity matrix.
 *
 * @param res Pointer to the matrix4 where the inverse will be stored.
 * @param m Pointer to the matrix4 to invert.
 */
VEC_API void mat4_inverse_ptr(matrix4 *res, const matrix4 *m);

/**
 * @brief Inverse of an affine matrix `[R t; 0 1]`.
 *
 * Inverts the upper-left 3×3 and corrects the translation. Last row is fixed
 * as `[0 0 0 1]`. For a full 4×4 inverse, use `mat4_inverse`. Singular 3×3
 * yields identity.
 *
 * @param res Output inverse matrix.
 * @param m   Affine 4×4 matrix.
 */
VEC_API void mat4_inverse_affine_ptr(matrix4 *res, const matrix4 *m);

/**
 * @brief Sets the matrix to a translation matrix.
 *
 * This function initializes a 4x4 matrix to represent a translation transformation
 * based on the provided vector, with the translation components placed in the
 * last column and the rest forming an identity matrix.
 *
 * @param res Pointer to the matrix4 to set.
 * @param v Pointer to the vector3 containing the translation values.
 */
VEC_API void mat4_translate_ptr(matrix4 *res, const vector3 *v);

/**
 * @brief Sets the matrix to a scaling matrix using the provided scale vector.
 *
 * This function constructs a 4x4 scaling matrix where the diagonal elements
 * correspond to the x, y, and z scale factors from the input vector. The
 * bottom-right element is set to 1.0f for homogeneous coordinates. All other
 * elements are implicitly zero (not set, assuming the matrix is initialized).
 *
 * @param res Pointer to the matrix4 to set as a scaling matrix.
 * @param v Pointer to the vector3 containing the scale factors (x, y, z).
 */
VEC_API void mat4_scale_ptr(matrix4 *res, const vector3 *v);

/**
 * @brief Inverse-transpose of the upper-left 3x3, embedded in a matrix4.
 *
 * Used to transform surface normals under a (possibly non-uniformly scaled)
 * model matrix. Last row/column stay identity.
 *
 * @param res Normal matrix as a matrix4.
 * @param m Model matrix.
 */
VEC_API void mat4_normal_ptr(matrix4 *res, const matrix4 *m);

/**
 * @brief Sets the matrix to a rotation matrix around the given axis.
 *
 * Constructs a 4x4 rotation matrix that rotates by the specified angle
 * (in radians) around the provided axis. The axis vector is normalized
 * internally.
 *
 * @param res Pointer to the matrix4 that will receive the rotation matrix.
 * @param axis Pointer to the vector3 representing the rotation axis.
 * @param radians The rotation angle in radians.
 */
VEC_API void mat4_rotation_ptr(matrix4 *res, const vector3 *axis, vm_float_t radians);

/**
 * @brief Builds a 4x4 rotation matrix around the X axis (radians).
 *
 * @param res Result matrix.
 * @param radians Rotation angle in radians.
 */
VEC_API void mat4_rotation_x_ptr(matrix4 *res, vm_float_t radians);

/**
 * @brief Sets the matrix to a rotation matrix around the Y axis.
 *
 * @param res Pointer to the matrix4 to store the result.
 * @param radians Rotation angle in radians.
 */
VEC_API void mat4_rotation_y_ptr(matrix4 *res, vm_float_t radians);

/**
 * @brief Sets the matrix to a rotation around the Z axis.
 *
 * @param res Pointer to the matrix4 to store the result.
 * @param radians Rotation angle in radians.
 */
VEC_API void mat4_rotation_z_ptr(matrix4 *res, vm_float_t radians);

/**
 * @brief Sets the matrix to a rotation matrix using an axis and angle in degrees.
 *
 * @see mat4_rotation_ptr
 *
 * @param res Pointer to the matrix4 that will receive the rotation matrix.
 * @param axis Pointer to the vector3 defining the rotation axis.
 * @param degrees The rotation angle in degrees.
 */
VEC_API void mat4_rotation_deg_ptr(matrix4 *res, const vector3 *axis, vm_float_t degrees);

/**
 * @brief Builds a 4x4 rotation matrix around the X axis (degrees).
 *
 * @param res Output value.
 * @param degrees Rotation angle in degrees.
 */
VEC_API void mat4_rotation_x_deg_ptr(matrix4 *res, vm_float_t degrees);

/**
 * @brief Builds a 4x4 rotation matrix around the Y axis (degrees).
 *
 * @param res Output value.
 * @param degrees Rotation angle in degrees.
 */
VEC_API void mat4_rotation_y_deg_ptr(matrix4 *res, vm_float_t degrees);

/**
 * @brief Builds a 4x4 rotation matrix around the Z axis (degrees).
 *
 * @param res Output value.
 * @param degrees Rotation angle in degrees.
 */
VEC_API void mat4_rotation_z_deg_ptr(matrix4 *res, vm_float_t degrees);

/**
 * @brief Builds a 4x4 TRS matrix from translation, rotation, and scale.
 *
 * @param res Output value.
 * @param translation Translation vector.
 * @param rotation Rotation quaternion.
 * @param scale Scale vector.
 */
VEC_API void mat4_trs_ptr(matrix4 *res, const vector3 *translation, const quaternion *rotation, const vector3 *scale);

/**
 * @brief Embeds a matrix3 into the upper-left of a matrix4.
 *
 * @param res Output value.
 * @param m Input matrix.
 */
VEC_API void mat4_from_mat3_ptr(matrix4 *res, const matrix3 *m);

/**
 * @brief Extracts the translation vector from a matrix4.
 *
 * @param res Output value.
 * @param m Input matrix.
 */
VEC_API void mat4_extract_translation_ptr(vector3 *res, const matrix4 *m);

/**
 * @brief Extracts the scale vector from a matrix4.
 *
 * @param res Output value.
 * @param m Input matrix.
 */
VEC_API void mat4_extract_scale_ptr(vector3 *res, const matrix4 *m);

/**
 * @brief Extracts the rotation quaternion from a matrix4.
 *
 * @param res Output value.
 * @param m Input matrix.
 */
VEC_API void mat4_extract_rotation_ptr(quaternion *res, const matrix4 *m);

/**
 * @brief Transforms a vector4 by a 4x4 matrix (dispatched).
 *
 * @see mat4_mul_vec4_ptr_scalar
 *
 * @param res Result vector.
 * @param m Input matrix.
 * @param v Input vector.
 */
VEC_API void mat4_mul_vec4_ptr(vector4 *res, const matrix4 *m, const vector4 *v);

/**
 * @brief Transforms a vector3 by a 4x4 matrix using homogeneous w (dispatched).
 *
 * @see mat4_mul_vec3_ptr_scalar
 *
 * @param res Result vector.
 * @param m Input matrix.
 * @param v Input vector.
 * @param w Homogeneous w component.
 */
VEC_API void mat4_mul_vec3_ptr(vector3 *res, const matrix4 *m, const vector3 *v, vm_float_t w);

/* ---- matrix2i ------------------------------------------------------------ */

/**
 * @brief Constructs the 2x2 integer identity matrix.
 *
 * @see mat2i_identity_ptr
 *
 * @return The identity matrix2i.
 */
VEC_API matrix2i mat2i_identity(void);

/**
 * @brief Multiplies two 2x2 integer matrices (a * b).
 *
 * @see mat2i_mul_ptr
 *
 * @param a The first matrix.
 * @param b The second matrix.
 * @return The resulting product matrix2i.
 *
 * Column-major product `AB` so `(AB)v == A(Bv)` (column vector on the right).
 */
VEC_API matrix2i mat2i_mul(matrix2i a, matrix2i b);

/**
 * @brief Computes the transpose of a 2x2 integer matrix.
 *
 * @see mat2i_transpose_ptr
 *
 * @param m The input matrix.
 * @return The transposed matrix2i.
 */
VEC_API matrix2i mat2i_transpose(matrix2i m);

/**
 * @brief Computes the inverse of a 2x2 integer matrix.
 *
 * @see mat2i_inverse_ptr
 *
 * @param m The input matrix.
 * @return The inverse matrix2i.
 *
 * Truncated integer inverse (not a modular inverse).
 */
VEC_API matrix2i mat2i_inverse(matrix2i m);

/**
 * @brief Multiplies a 2x2 integer matrix by a vector2i.
 *
 * @see mat2i_mul_vec2i_ptr
 *
 * @param m Input matrix.
 * @param v Input vector.
 * @return The resulting vector2i.
 */
VEC_API vector2i mat2i_mul_vec2i(matrix2i m, vector2i v);

/**
 * @brief Calculates the determinant of the given 2x2 integer matrix.
 *
 * @param m The matrix.
 * @return The determinant value (m.v[0]*m.v[3] - m.v[1]*m.v[2]).
 */
VEC_API vm_int_t mat2i_determinant(matrix2i m);

/* ---- matrix3i ------------------------------------------------------------ */

/**
 * @brief Constructs the 3x3 identity matrix.
 *
 * @see mat3i_identity_ptr
 *
 * @return The identity matrix3i.
 */
VEC_API matrix3i mat3i_identity(void);

/**
 * @brief Multiplies two 3x3 integer matrices.
 *
 * @see mat3i_mul_ptr
 *
 * @param a The first matrix.
 * @param b The second matrix.
 * @return The product matrix.
 *
 * Column-major product `AB` so `(AB)v == A(Bv)` (column vector on the right).
 */
VEC_API matrix3i mat3i_mul(matrix3i a, matrix3i b);

/**
 * @brief Computes the transpose of a 3x3 integer matrix.
 *
 * @see mat3i_transpose_ptr
 *
 * @param m The input matrix.
 * @return The transposed matrix.
 */
VEC_API matrix3i mat3i_transpose(matrix3i m);

/**
 * @brief Computes the inverse of a 3x3 integer matrix.
 *
 * @see mat3i_inverse_ptr
 *
 * @param m The input matrix.
 * @return The inverse matrix.
 *
 * Truncated integer inverse (not a modular inverse).
 */
VEC_API matrix3i mat3i_inverse(matrix3i m);

/**
 * @brief Multiplies a 3x3 integer matrix by a vector3i.
 *
 * @see mat3i_mul_vec3i_ptr
 *
 * @param m Input matrix.
 * @param v Input vector.
 * @return The resulting vector3i.
 */
VEC_API vector3i mat3i_mul_vec3i(matrix3i m, vector3i v);

/**
 * @brief Applies a 3x3 integer affine transform to a vector2i.
 *
 * @see mat3i_mul_vec2i_ptr
 *
 * @param m Input matrix.
 * @param v Input vector.
 * @return The resulting vector2i.
 */
VEC_API vector2i mat3i_mul_vec2i(matrix3i m, vector2i v);

/**
 * @brief Computes the determinant of a 3x3 integer matrix.
 *
 * @param m The input matrix.
 * @return The determinant value.
 */
VEC_API vm_int_t mat3i_determinant(matrix3i m);

/* ---- matrix4i ------------------------------------------------------------ */

/**
 * @brief Constructs the 4x4 identity matrix.
 *
 * @see mat4i_identity_ptr
 *
 * @return The identity matrix4i.
 */
VEC_API matrix4i mat4i_identity(void);

/**
 * @brief Multiplies two 4x4 integer matrices.
 *
 * @see mat4i_mul_ptr
 *
 * @param a The first matrix.
 * @param b The second matrix.
 * @return The product matrix.
 *
 * Column-major product `AB` so `(AB)v == A(Bv)` (column vector on the right).
 */
VEC_API matrix4i mat4i_mul(matrix4i a, matrix4i b);

/**
 * @brief Computes the transpose of a 4x4 integer matrix.
 *
 * @see mat4i_transpose_ptr
 *
 * @param m The input matrix.
 * @return The transposed matrix.
 */
VEC_API matrix4i mat4i_transpose(matrix4i m);

/**
 * @brief Computes the inverse of a 4x4 integer matrix.
 *
 * @see mat4i_inverse_ptr
 *
 * @param m The input matrix.
 * @return The inverse matrix.
 *
 * Truncated integer inverse (not a modular inverse).
 */
VEC_API matrix4i mat4i_inverse(matrix4i m);

/**
 * @brief Multiplies a 4x4 integer matrix by a vector4i.
 *
 * @see mat4i_mul_vec4i_ptr
 *
 * @param m Input matrix.
 * @param v Input vector.
 * @return The resulting vector4i.
 */
VEC_API vector4i mat4i_mul_vec4i(matrix4i m, vector4i v);

/**
 * @brief Transforms a vector3i by a 4x4 integer matrix using homogeneous w.
 *
 * @see mat4i_mul_vec3i_ptr
 *
 * @param m Input matrix.
 * @param v Input vector.
 * @param w Homogeneous w component.
 * @return The resulting vector3i.
 */
VEC_API vector3i mat4i_mul_vec3i(matrix4i m, vector3i v, vm_int_t w);

/**
 * @brief  Computes the determinant of a 4x4 integer matrix (Laplace expansion along first row).
 *
 * @param m The input matrix.
 * @return The determinant value.
 */
VEC_API vm_int_t mat4i_determinant(matrix4i m);

/*******************************************************************************
 * Integer matrix pointer-based performance functions
 ******************************************************************************/

/* ---- matrix2i pointer-based operations ----------------------------------- */

/**
 * @brief Initializes the integer 2x2 matrix to identity (diagonal 1, others 0).
 *
 * @param res Pointer to the output matrix2i.
 */
VEC_API void mat2i_identity_ptr(matrix2i *res);

/**
 * @brief Multiplies two integer 2x2 matrices (a * b) using explicit loops and
 * stores the result in res.
 *
 * @param res Pointer to the output matrix2i.
 * @param a Pointer to the first matrix.
 * @param b Pointer to the second matrix.
 *
 * Column-major product `AB` so `(AB)v == A(Bv)` (column vector on the right).
 */
VEC_API void mat2i_mul_ptr(matrix2i *res, const matrix2i *a, const matrix2i *b);

/**
 * @brief Computes the transpose of the input integer 2x2 matrix and stores in res.
 *
 * @param res Pointer to the output matrix2i.
 * @param m Pointer to the input matrix.
 */
VEC_API void mat2i_transpose_ptr(matrix2i *res, const matrix2i *m);

/**
 * @brief Computes the inverse of the input integer 2x2 matrix and stores in res.
 *
 * Uses double precision for inversion, truncates to int; sets to identity if det=0.
 *
 * @param res Pointer to the output matrix2i.
 * @param m Pointer to the input matrix.
 *
 * Truncated integer inverse (not a modular inverse).
 */
VEC_API void mat2i_inverse_ptr(matrix2i *res, const matrix2i *m);

/**
 * @brief Multiplies a 2x2 integer matrix by a vector2i.
 *
 * @param res Output value.
 * @param m Input matrix.
 * @param v Input vector.
 */
VEC_API void mat2i_mul_vec2i_ptr(vector2i *res, const matrix2i *m, const vector2i *v);

/* ---- matrix3i pointer-based operations ----------------------------------- */

/**
 * @brief Fills the given 3x3 integer matrix with the identity matrix.
 *
 * @param res Pointer to the result matrix.
 */
VEC_API void mat3i_identity_ptr(matrix3i *res);

/**
 * @brief Multiplies two 3x3 integer matrices (standard matrix multiplication).
 *
 * @param res Pointer to the result matrix.
 * @param a   Pointer to the first input matrix.
 * @param b   Pointer to the second input matrix.
 *
 * Column-major product `AB` so `(AB)v == A(Bv)` (column vector on the right).
 */
VEC_API void mat3i_mul_ptr(matrix3i *res, const matrix3i *a, const matrix3i *b);

/**
 * @brief Computes the transpose of a 3x3 integer matrix.
 *
 * @param res Pointer to the result matrix.
 * @param m   Pointer to the input matrix.
 */
VEC_API void mat3i_transpose_ptr(matrix3i *res, const matrix3i *m);

/**
 * @brief Computes the inverse of a 3x3 integer matrix using adjugate over determinant.
 *
 * If the determinant is zero (singular), sets the result to the identity matrix.
 *
 * @param res Pointer to the result matrix.
 * @param m   Pointer to the input matrix.
 *
 * Truncated integer inverse (not a modular inverse).
 */
VEC_API void mat3i_inverse_ptr(matrix3i *res, const matrix3i *m);

/**
 * @brief Multiplies a 3x3 integer matrix by a vector3i.
 *
 * @param res Output value.
 * @param m Input matrix.
 * @param v Input vector.
 */
VEC_API void mat3i_mul_vec3i_ptr(vector3i *res, const matrix3i *m, const vector3i *v);

/**
 * @brief Applies a 3x3 integer affine transform to a vector2i.
 *
 * @param res Output value.
 * @param m Input matrix.
 * @param v Input vector.
 */
VEC_API void mat3i_mul_vec2i_ptr(vector2i *res, const matrix3i *m, const vector2i *v);

/* ---- matrix4i pointer-based operations ----------------------------------- */

/**
 * @brief Sets the given 4x4 integer matrix to the identity matrix.
 *
 * @param res Pointer to the matrix structure to set to identity.
 */
VEC_API void mat4i_identity_ptr(matrix4i *res);

/**
 * @brief Performs matrix multiplication of two 4x4 matrices and stores the result.
 *
 * @param res Pointer to the matrix structure to store the result of the multiplication.
 * @param a Pointer to the first matrix operand.
 * @param b Pointer to the second matrix operand.
 *
 * Column-major product `AB` so `(AB)v == A(Bv)` (column vector on the right).
 */
VEC_API void mat4i_mul_ptr(matrix4i *res, const matrix4i *a, const matrix4i *b);

/**
 * @brief Transposes the given 4x4 integer matrix and stores the result in the
 * provided matrix structure.
 *
 * @param res Pointer to the matrix structure to store the transposed matrix.
 * @param m Pointer to the input matrix to be transposed.
 */
VEC_API void mat4i_transpose_ptr(matrix4i *res, const matrix4i *m);

/**
 * @brief Computes the inverse of the given 4x4 integer matrix and stores the
 * result in the specified matrix.
 *
 * If the matrix is singular (determinant is zero), the result is set to the identity matrix.
 *
 * @param res Pointer to the matrix structure to store the inverse matrix.
 * @param m Pointer to the constant matrix to be inverted.
 *
 * Truncated integer inverse (not a modular inverse).
 */
VEC_API void mat4i_inverse_ptr(matrix4i *res, const matrix4i *m);

/**
 * @brief Multiplies a 4x4 integer matrix by a vector4i.
 *
 * @param res Output value.
 * @param m Input matrix.
 * @param v Input vector.
 */
VEC_API void mat4i_mul_vec4i_ptr(vector4i *res, const matrix4i *m, const vector4i *v);

/**
 * @brief Transforms a vector3i by a 4x4 integer matrix using homogeneous w.
 *
 * @param res Output value.
 * @param m Input matrix.
 * @param v Input vector.
 * @param w Homogeneous w component.
 */
VEC_API void mat4i_mul_vec3i_ptr(vector3i *res, const matrix4i *m, const vector3i *v, vm_int_t w);

/*******************************************************************************
 * Matrix equality
 ******************************************************************************/

/**
 * @brief Returns true if two matrices are equal within `VECMAT_EPSILON`.
 *
 * @param a First matrix.
 * @param b Second matrix.
 * @return True if a and b are approximately equal.
 */
VEC_API bool mat2_eq(matrix2 a, matrix2 b);

/**
 * @brief Returns true if two matrices are equal within `VECMAT_EPSILON`.
 *
 * @param a First matrix.
 * @param b Second matrix.
 * @return True if a and b are approximately equal.
 */
VEC_API bool mat3_eq(matrix3 a, matrix3 b);

/**
 * @brief Returns true if two matrices are equal within `VECMAT_EPSILON`.
 *
 * @param a First matrix.
 * @param b Second matrix.
 * @return True if a and b are approximately equal.
 */
VEC_API bool mat4_eq(matrix4 a, matrix4 b);

/**
 * @brief Returns true if two matrices are exactly equal.
 *
 * @param a First matrix.
 * @param b Second matrix.
 * @return True if a and b are equal.
 */
VEC_API bool mat2i_eq(matrix2i a, matrix2i b);

/**
 * @brief Returns true if two matrices are exactly equal.
 *
 * @param a First matrix.
 * @param b Second matrix.
 * @return True if a and b are equal.
 */
VEC_API bool mat3i_eq(matrix3i a, matrix3i b);

/**
 * @brief Returns true if two matrices are exactly equal.
 *
 * @param a First matrix.
 * @param b Second matrix.
 * @return True if a and b are equal.
 */
VEC_API bool mat4i_eq(matrix4i a, matrix4i b);

#endif //VECMAT_MAT_H
