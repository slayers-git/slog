/* This file is part of the slog library.
 *
 * Copyright (C) 2021 by Sergey Lafin
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory. */

#ifndef __SLOG_COLOR_H__
#define __SLOG_COLOR_H__

#include "slog_export.h"

typedef enum {
    slog_color_red,
    slog_color_yellow,
    slog_color_white,
    slog_color_black,
    slog_color_reset,

    slog_color_message = slog_color_white,
    slog_color_warning = slog_color_yellow,
    slog_color_error   = slog_color_red
} slog_color;

SLOG_API void slog_set_color (const slog_color color);
SLOG_API void slog_reset_color ();

#endif
