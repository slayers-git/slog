/* This file is part of the slog library.
 *
 * Copyright (C) 2021 by Sergey Lafin
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory. */

/* puts.c - test of slog_puts () function */

#include "../slog.h"

int main (void) {
    slog_stream *stream = slog_create (NULL, slog_flags_none);
    if (!stream)
        return -1;

    slog_puts (stream, slog_loglevel_message, "My test message");
    slog_puts (stream, slog_loglevel_error, "My test error");

    slog_close (stream);

    return 0;
}
