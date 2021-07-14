/* This file is part of the slog library.
 *
 * Copyright (C) 2021 by Sergey Lafin
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory. */

#include "slog_color.h"
#include <stdio.h>

#if defined(__linux) || defined(__linux__) || defined (__unix) || defined (__unix__)
#define __slog_unix 1
const char *slog_unix_map[] = {
    "\033[31m",
    "\033[33m",
    "\033[37m",
    "\033[30m",
    "\033[0m",
};
#endif

void slog_set_color (const slog_color color) {
#if __slog_unix
    fwrite (slog_unix_map[color], 1, 5, stdout);
#else
    return;
#endif
}

/* might not work, gotta find a fix */
void slog_reset_color () {
#if __slog_unix
    fwrite (slog_unix_map[slog_color_reset], 1, 4, stdout);
#else
    return;
#endif
}
