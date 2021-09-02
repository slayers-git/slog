/* This file is part of the slog library.
 *
 * Copyright (C) 2021 by Sergey Lafin
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory. */

#include "../slog.h"
#include <stdio.h>

/* the file, to which the log will be written */
#define LOGFILE "mylog.txt"
/* define the log format:
 * [LEVEL] DAY/MONTH/YEAR (SECONDS_SINCE_THE_PROGRAM_STARTs): MESSAGE */
#define LOGFORMAT "[%l] %d/%M/%y (%ps): %L"

int main (void) {
    /* firstly, we create a logger
     * set the color flag (messages will be colored) */
    slog_stream *logger = slog_create (LOGFILE, slog_flags_color);
    /* check for proper initialization */
    if (!logger) {
        puts ("failed to open file " LOGFILE);
        return -1;
    }
    /* set the format of the log entries */
    slog_format (logger, LOGFORMAT);

    const char *towhat = "The World";
    /* just print a message */
    slog_printf (logger, slog_loglevel_message, "My message to %s", towhat);
    /* macros can be used to save some time: */
    slog_error (logger, "Some nasty error");
    slog_printf (logger, slog_loglevel_warning, "A warning");
    /* this won't be shown as the slog_stream suppresses slog_debug 
     * by default. */
    slog_printf (logger, slog_loglevel_debug, "A debug message");
    /* set the suppressed flag to nothing, so all entries will be shown */
    slog_suppress (logger, SLOG_SUPPRESS_NOTHING);
    /* now this message WILL be shown */
    slog_printf (logger, slog_loglevel_debug, "A debug message");
    /* don't forget to free up the used resource */
    slog_close (logger);
    return 0;
}
