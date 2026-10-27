// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <stdlib.h>
#include <vecmat.h>
#include "vecmat/project.h"

void vm_project_residual(vm_float_t *r,
                         const vm_float_t *u,
                         const vm_float_t *v,
                         const vm_float_t *w,
                         const vm_grid3 g)
{
    vm_mac_div(r, u, v, w, g);
}

static bool on_boundary(const vm_grid3 g, const int i, const int j, const int k)
{
    const int nz = (g.nz > 0) ? g.nz : 1;

    if (i == 0 || i == g.nx - 1 || j == 0 || j == g.ny - 1) {
        return true;
    }

    if (nz > 1 && (k == 0 || k == nz - 1)) {
        return true;
    }

    return false;
}

bool vm_project_laplacian(vm_spmat *A,
                          const vm_float_t *rho,
                          const vm_grid3 g,
                          const vm_bc_t bc)
{
    const int nz = (g.nz > 0) ? g.nz : 1;
    const int n = vm_grid_ncells(g);
    if (!A || n <= 0 || g.dx == VM_F(0.0) || g.dy == VM_F(0.0) ||
        (nz > 1 && g.dz == VM_F(0.0))) {
        return false;
    }

    if (rho) {
        for (int c = 0; c < n; ++c) {
            if (rho[c] <= VM_F(0.0)) {
                return false;
            }
        }
    }

    const int cap = n * 7;
    int *row = malloc((size_t)cap * sizeof(int));
    int *col = malloc((size_t)cap * sizeof(int));
    vm_float_t *val = malloc((size_t)cap * sizeof(vm_float_t));
    if (!row || !col || !val) {
        free(row);
        free(col);
        free(val);
        return false;
    }

    const vm_float_t ix = VM_F(1.0) / (g.dx * g.dx);
    const vm_float_t iy = VM_F(1.0) / (g.dy * g.dy);
    const vm_float_t iz = (nz > 1) ? (VM_F(1.0) / (g.dz * g.dz)) : VM_F(0.0);

    int nnz = 0;
    for (int k = 0; k < nz; ++k) {
        for (int j = 0; j < g.ny; ++j) {
            for (int i = 0; i < g.nx; ++i) {
                const int p = vm_grid_cell(g, i, j, k);
                if (bc == VM_BC_DIRICHLET && on_boundary(g, i, j, k)) {
                    row[nnz] = p;
                    col[nnz] = p;
                    val[nnz] = VM_F(1.0);
                    ++nnz;
                    continue;
                }

                vm_float_t diag = VM_F(0.0);
                const int nb[6][3] = {
                    { i - 1, j, k }, { i + 1, j, k },
                    { i, j - 1, k }, { i, j + 1, k },
                    { i, j, k - 1 }, { i, j, k + 1 }
                };
                const vm_float_t base[6] = { ix, ix, iy, iy, iz, iz };
                const int nnb = (nz > 1) ? 6 : 4;
                const vm_float_t rho_c = rho ? rho[p] : VM_F(1.0);
                for (int t = 0; t < nnb; ++t) {
                    const int ni = nb[t][0];
                    const int nj = nb[t][1];
                    const int nk = nb[t][2];
                    const bool inside = ni >= 0 && ni < g.nx &&
                                        nj >= 0 && nj < g.ny &&
                                        nk >= 0 && nk < nz;
                    vm_float_t rho_f = rho_c;
                    if (inside) {
                        const vm_float_t rho_n = rho ? rho[vm_grid_cell(g, ni, nj, nk)] : VM_F(1.0);
                        rho_f = VM_F(0.5) * (rho_c + rho_n);
                    }

                    const vm_float_t w = base[t] / rho_f;
                    if (!inside) {
                        if (bc != VM_BC_NEUMANN) {
                            diag += w;
                        }
                        continue;
                    }

                    if (bc == VM_BC_DIRICHLET && on_boundary(g, ni, nj, nk)) {
                        diag += w;
                        continue;
                    }

                    row[nnz] = p;
                    col[nnz] = vm_grid_cell(g, ni, nj, nk);
                    val[nnz] = -w;
                    ++nnz;
                    diag += w;
                }
                row[nnz] = p;
                col[nnz] = p;
                val[nnz] = diag;
                ++nnz;
            }
        }
    }

    const bool ok = vm_spmat_from_triplets(A, n, nnz, row, col, val);
    free(row);
    free(col);
    free(val);
    return ok;
}
