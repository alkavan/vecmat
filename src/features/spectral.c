// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <stdlib.h>
#include <vecmat.h>

void vm_matvec(vm_float_t *y, const vm_mat *A, const vm_float_t *x)
{
    if (!y || !A || !A->data || !x || A->rows <= 0 || A->cols <= 0) {
        return;
    }
    const int m = A->rows;
    const int n = A->cols;
    for (int i = 0; i < m; ++i) {
        vm_float_t s = VM_F(0.0);
        for (int j = 0; j < n; ++j) {
            s += vm_mat_get(A, i, j) * x[j];
        }
        y[i] = s;
    }
}

void vm_symv(vm_float_t *y, const vm_mat *A, const vm_float_t *x)
{
    if (!y || !A || !A->data || !x || A->rows <= 0 || A->rows != A->cols) {
        return;
    }
    const int n = A->rows;
    for (int i = 0; i < n; ++i) {
        y[i] = VM_F(0.0);
    }
    /* Lower triangle only. */
    for (int j = 0; j < n; ++j) {
        const vm_float_t xj = x[j];
        for (int i = j; i < n; ++i) {
            const vm_float_t aij = vm_mat_get(A, i, j);
            y[i] += aij * xj;

            if (i != j) {
                y[j] += aij * x[i];
            }
        }
    }
}

/**
 * @brief One Jacobi rotation for dense syev. Rotates columns p,q of A and of V.
 */
static void syev_jacobi_rotate(vm_mat *A, vm_mat *V, const int p, const int q, const vm_float_t tol)
{
    const int n = A->rows;
    const vm_float_t app = vm_mat_get(A, p, p);
    const vm_float_t aqq = vm_mat_get(A, q, q);
    const vm_float_t apq = vm_mat_get(A, p, q);
    const vm_float_t pair = VECMAT_FABS(app) + VECMAT_FABS(aqq);
    if (VECMAT_FABS(apq) <= tol * (VM_F(1.0) + pair)) {
        return;
    }

    const vm_float_t tau = (aqq - app) / (VM_F(2.0) * apq);
    const vm_float_t t = (tau >= VM_F(0.0))
        ? VM_F(1.0) / (tau + VECMAT_SQRT(VM_F(1.0) + tau * tau))
        : VM_F(-1.0) / (-tau + VECMAT_SQRT(VM_F(1.0) + tau * tau));
    const vm_float_t c = VM_F(1.0) / VECMAT_SQRT(VM_F(1.0) + t * t);
    const vm_float_t s = t * c;

    for (int i = 0; i < n; ++i) {
        if (i == p || i == q) {
            continue;
        }
        const vm_float_t aip = vm_mat_get(A, i, p);
        const vm_float_t aiq = vm_mat_get(A, i, q);
        const vm_float_t np = c * aip - s * aiq;
        const vm_float_t nq = s * aip + c * aiq;
        vm_mat_set(A, i, p, np);
        vm_mat_set(A, p, i, np);
        vm_mat_set(A, i, q, nq);
        vm_mat_set(A, q, i, nq);
    }
    vm_mat_set(A, p, p, c * c * app - VM_F(2.0) * s * c * apq + s * s * aqq);
    vm_mat_set(A, q, q, s * s * app + VM_F(2.0) * s * c * apq + c * c * aqq);
    vm_mat_set(A, p, q, VM_F(0.0));
    vm_mat_set(A, q, p, VM_F(0.0));

    if (V) {
        for (int i = 0; i < n; ++i) {
            const vm_float_t vip = vm_mat_get(V, i, p);
            const vm_float_t viq = vm_mat_get(V, i, q);
            vm_mat_set(V, i, p, c * vip - s * viq);
            vm_mat_set(V, i, q, s * vip + c * viq);
        }
    }
}

bool vm_syev(vm_mat *A, vm_float_t *eigenvalues, vm_mat *V)
{
    if (!A || !A->data || !eigenvalues || A->rows <= 0 || A->rows != A->cols) {
        return false;
    }

    const int n = A->rows;
    if (V && (V->rows != n || V->cols != n || !V->data)) {
        return false;
    }

    /* Symmetrize lower into upper so Jacobi sees a full symmetric matrix. */
    for (int j = 0; j < n; ++j) {
        for (int i = j + 1; i < n; ++i) {
            const vm_float_t a = vm_mat_get(A, i, j);
            vm_mat_set(A, j, i, a);
        }
    }

    if (V) {
        for (int j = 0; j < n; ++j) {
            for (int i = 0; i < n; ++i) {
                vm_mat_set(V, i, j, (i == j) ? VM_F(1.0) : VM_F(0.0));
            }
        }
    }

    vm_float_t scale = VM_F(0.0);
    for (int i = 0; i < n * n; ++i) {
        const vm_float_t a = VECMAT_FABS(A->data[i]);
        if (a > scale) {
            scale = a;
        }
    }

    const vm_float_t tol = (scale > VM_F(0.0))
        ? VECMAT_EPSILON * (vm_float_t)n * scale
        : VECMAT_EPSILON;

    const int max_sweeps = 32;
    for (int sweep = 0; sweep < max_sweeps; ++sweep) {
        vm_float_t off = VM_F(0.0);
        for (int j = 0; j < n; ++j) {
            for (int i = j + 1; i < n; ++i) {
                off += VECMAT_FABS(vm_mat_get(A, i, j));
            }
        }

        if (off <= tol) {
            break;
        }

        for (int p = 0; p < n - 1; ++p) {
            for (int q = p + 1; q < n; ++q) {
                syev_jacobi_rotate(A, V, p, q, tol);
            }
        }
    }

    /* Diagonal is the eigenvalues; sort descending with column swaps. */
    for (int i = 0; i < n; ++i) {
        eigenvalues[i] = vm_mat_get(A, i, i);
    }

    for (int i = 0; i < n - 1; ++i) {
        int max_j = i;
        for (int j = i + 1; j < n; ++j) {
            if (eigenvalues[j] > eigenvalues[max_j]) {
                max_j = j;
            }
        }

        if (max_j != i) {
            const vm_float_t tmp = eigenvalues[i];
            eigenvalues[i] = eigenvalues[max_j];
            eigenvalues[max_j] = tmp;
            /* Swap columns i and max_j in A (optional) and V. */
            for (int r = 0; r < n; ++r) {
                const vm_float_t ta = vm_mat_get(A, r, i);
                vm_mat_set(A, r, i, vm_mat_get(A, r, max_j));
                vm_mat_set(A, r, max_j, ta);

                if (V) {
                    const vm_float_t tv = vm_mat_get(V, r, i);
                    vm_mat_set(V, r, i, vm_mat_get(V, r, max_j));
                    vm_mat_set(V, r, max_j, tv);
                }
            }
        }
    }
    return true;
}

bool vm_laplacian(vm_mat *L, const vm_mat *A)
{
    if (!L || !A || !A->data || A->rows <= 0 || A->rows != A->cols) {
        return false;
    }

    const int n = A->rows;
    if (!L->data || L->rows != n || L->cols != n) {
        vm_mat_free(L);
        *L = vm_mat_alloc(n, n);
        if (!L->data) {
            return false;
        }
    }

    for (int i = 0; i < n; ++i) {
        vm_float_t d = VM_F(0.0);
        for (int j = 0; j < n; ++j) {
            const vm_float_t a = vm_mat_get(A, i, j);
            d += a;

            if (i != j) {
                vm_mat_set(L, i, j, -a);
            }
        }
        vm_mat_set(L, i, i, d);
    }

    return true;
}

bool vm_syev_topk(const vm_mat *A, int k,
                  vm_float_t *eigenvalues, vm_mat *eigenvectors)
{
    if (!A || !A->data || !eigenvalues || !eigenvectors ||
        A->rows <= 0 || A->rows != A->cols || k <= 0 || k > A->rows) {
        return false;
    }
    const int n = A->rows;
    if (eigenvectors->rows != n || eigenvectors->cols != k || !eigenvectors->data) {
        return false;
    }

    vm_mat Ac = { 0 };
    if (!vm_mat_copy(&Ac, A)) {
        return false;
    }

    vm_mat V = vm_mat_alloc(n, n);
    if (!V.data) {
        vm_mat_free(&Ac);
        return false;
    }

    vm_float_t *ev = (vm_float_t *)malloc((size_t)n * sizeof(vm_float_t));
    if (!ev) {
        vm_mat_free(&Ac);
        vm_mat_free(&V);
        return false;
    }

    const bool ok = vm_syev(&Ac, ev, &V);
    if (ok) {
        for (int i = 0; i < k; ++i) {
            eigenvalues[i] = ev[i];
            for (int r = 0; r < n; ++r) {
                vm_mat_set(eigenvectors, r, i, vm_mat_get(&V, r, i));
            }
        }
    }

    free(ev);
    vm_mat_free(&Ac);
    vm_mat_free(&V);
    return ok;
}
