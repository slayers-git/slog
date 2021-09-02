/* This file is part of the slog library.
 *
 * Copyright (C) 2021 by Sergey Lafin
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory. */

#include "slog_loglevel.h"
#include <limits.h>

static unsigned int slog_lastuuid_ = (1 << 5);

void slog_newloglevel (slog_loglevel *level, const char *prefix, slog_color color, unsigned char suppress) {
    level->prefix = prefix;
    level->color  = color;
    level->unsuppressible = !suppress;

    level->id = slog_lastuuid_;
    slog_lastuuid_ <<= 1;
}
