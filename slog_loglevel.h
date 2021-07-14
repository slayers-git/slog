/* This file is part of the slog library.
 *
 * Copyright (C) 2021 by Sergey Lafin
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory. */

#ifndef __SLOG_LOGLEVEL_H__
#define __SLOG_LOGLEVEL_H__

#include "slog_export.h"

#define SLOG_SUPPRESS_NOTHING 0

typedef enum slog_loglevel {
    slog_loglevel_message = 1,
    slog_loglevel_warning = (1 << 1),
    slog_loglevel_error   = (1 << 2),
    slog_loglevel_debug   = (1 << 3),
} slog_loglevel;

SLOG_API const char *slog_loglevel_tostr (const slog_loglevel level);

#endif
