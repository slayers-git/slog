/* This file is part of the slog library.
 *
 * Copyright (C) 2021 by Sergey Lafin
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory. */

#include "slog_loglevel.h"

const char *slog_loglevel_tostr (const slog_loglevel level) {
    switch (level) {
        case slog_loglevel_message:
            return "Message";
        case slog_loglevel_warning:
            return "Warning";
        case slog_loglevel_error:
            return "Error";
        case slog_loglevel_debug:
            return "Debug";
        case slog_loglevel_fatal:
            return "Fatal";
        default:
            return "";
    }
}
