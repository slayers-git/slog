/* This file is part of the slog library.
 *
 * Copyright (C) 2021 by Sergey Lafin
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory. */

#include "slog_log.h"
#include <stdarg.h>
#include <stdio.h>

void slog_log_error (const char *fmt, ...) {
    va_list list;
    va_start (list, fmt);

    printf ("slog error: ");
    vfprintf (stdout, fmt, list);
    putc ('\n', stdout);

    va_end (list);
}
