// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef EXCEPT_H
#define EXCEPT_H

#include <setjmp.h>

// Global jump buffer for exception handling.
// Define in the global scope: jmp_buf _except_buf;
extern jmp_buf _except_buf;

#define TRY do { \
    if (setjmp(_except_buf) == 0) {

#define CATCH } else {

#define END_TRY } } while (0)

#define THROW longjmp(_except_buf, 1)

#endif //EXCEPT_H
