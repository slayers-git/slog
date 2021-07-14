/* This file is part of the slog library.
 *
 * Copyright (C) 2021 by Sergey Lafin
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory. */

/* fmt.c - example of creating a log string
 * To just print it on the screen standart slog.h functions should be used 
 * (see: sloh_ptintf()) */

/* for slog_fmt_create () and slog_fmt_get_str () */
#include "../slog_fmt.h"
/* puts () */
#include <stdio.h>
/* malloc () */
#include <stdlib.h>

int main (void) {
    /* we need an slog_fmt struct to which the format will be written */
    slog_fmt *fmt;
    /* now we create the fmt object from the const char * format */
    if (!(fmt = slog_fmt_create ("[%l] %H:%m:%s, %d/%M/%y (%P) <%c>: %L"))) {
        puts ("failed to parse format string");
        return -1;
    }
    /* slog_fmt_get_str() dynamically allocates the string of a necessary size */
    char *mbuf = slog_fmt_get_str (slog_loglevel_message, fmt, "My test message");
    puts (mbuf);
    /* don't forget to free the string */
    free (mbuf);
    /* free up the fmt structure */
    slog_fmt_clear (fmt);
    return 0;
}
