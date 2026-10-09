// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_MODULE_REGISTER_H
#define VECMAT_MODULE_REGISTER_H

/**
 * Defined in the generated vecmat_module_register.c. Empty unless a backend
 * listed in VECMAT_MODULES appended a register call. Not a library export.
 */
void vm_backend_modules_register(void);

#endif
