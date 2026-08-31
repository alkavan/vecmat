// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <stdlib.h>
#include <string.h>
#include <vecmat.h>

/**
 * Returns a pointer to the element at the specified row and column
 * in the matrix.
 *
 * The matrix is stored in column-major order.
 *
 * @param m pointer to the matrix
 * @param r row index
 * @param c column index
 * @return pointer to the matrix element at (r, c)
 */
static vm_float_t *vm_mat_at(const vm_mat *m, const int r, const int c)
{
    return &m->data[r + c * m->rows];
}

/**
 * Allocates a new matrix with the specified dimensions.
 *
 * Memory is allocated using calloc to initialize all elements to zero.
 * If allocation fails or if dimensions are invalid, a matrix with
 * zero dimensions and NULL data pointer is returned.
 *
 * @param rows number of rows in the matrix
 * @param cols number of columns in the matrix
 * @return allocated matrix or zero matrix on failure
 */
vm_mat vm_mat_alloc(const int rows, const int cols)
{
    vm_mat m = { .rows = 0, .cols = 0, .data = NULL };
    if (rows <= 0 || cols <= 0) {
        return m;
    }
    m.data = (vm_float_t *)calloc((size_t)rows * (size_t)cols, sizeof(vm_float_t));
    if (!m.data) {
        return m;
    }
    m.rows = rows;
    m.cols = cols;
    return m;
}

/**
 * Frees the memory allocated for a matrix and resets its state.
 *
 * If the matrix pointer is NULL, the function returns immediately.
 * The data pointer is freed (if allocated), then set to NULL and
 * both dimensions are reset to zero.
 *
 * @param m pointer to the matrix to free
 */
void vm_mat_free(vm_mat *m)
{
    if (!m) {
        return;
    }
    free(m->data);
    m->data = NULL;
    m->rows = 0;
    m->cols = 0;
}

/**
 * Sets all elements of the matrix to zero.
 *
 * If the matrix pointer or its data pointer is NULL, the function returns
 * immediately without performing any operation.
 *
 * @param m pointer to the matrix to be zeroed
 */
void vm_mat_zero(vm_mat *m)
{
    if (!m || !m->data) {
        return;
    }
    memset(m->data, 0, (size_t)m->rows * (size_t)m->cols * sizeof(vm_float_t));
}

/**
 * Returns the value of the matrix element at the specified row and column.
 *
 * If the matrix pointer is NULL, the data pointer is NULL, or the row or
 * column indices are out of bounds, the function returns 0.0f without
 * accessing memory.
 *
 * @param m pointer to the matrix
 * @param r zero-based row index
 * @param c zero-based column index
 * @return value at (r, c) or 0.0f on invalid input
 */
vm_float_t vm_mat_get(const vm_mat *m, const int r, const int c)
{
    if (!m || !m->data || r < 0 || c < 0 || r >= m->rows || c >= m->cols) {
        return VM_F(0.0);
    }
    return *vm_mat_at(m, r, c);
}

/**
 * Sets the element at the specified row and column in the matrix to the given value.
 *
 * The function performs bounds checking and validates the matrix pointer and
 * its data buffer before performing the assignment. If any validation fails,
 * the function returns without modifying any data.
 *
 * @param m pointer to the matrix to modify
 * @param r row index
 * @param c column index
 * @param v value to set at the specified position
 */
void vm_mat_set(vm_mat *m, const int r, const int c, const vm_float_t v)
{
    if (!m || !m->data || r < 0 || c < 0 || r >= m->rows || c >= m->cols) {
        return;
    }
    *vm_mat_at(m, r, c) = v;
}

/**
 * Copies the contents of one matrix to another.
 *
 * If the destination matrix has incompatible dimensions or unallocated
 * data it is freed and reallocated to match the source dimensions.
 * The source matrix must be valid and have allocated data.
 *
 * @param dst pointer to the destination matrix
 * @param src pointer to the source matrix
 * @return true on success, false if either pointer is NULL, source data
 *         is NULL or memory allocation fails
 */
bool vm_mat_copy(vm_mat *dst, const vm_mat *src)
{
    if (!dst || !src || !src->data) {
        return false;
    }
    if (!dst->data || dst->rows != src->rows || dst->cols != src->cols) {
        vm_mat_free(dst);
        *dst = vm_mat_alloc(src->rows, src->cols);
        if (!dst->data) {
            return false;
        }
    }
    memcpy(dst->data, src->data, (size_t)src->rows * (size_t)src->cols * sizeof(vm_float_t));
    return true;
}
