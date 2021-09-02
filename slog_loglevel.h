/* This file is part of the slog library.
 *
 * Copyright (C) 2021 by Sergey Lafin
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory. */

#ifndef __SLOG_LOGLEVEL_H__
#define __SLOG_LOGLEVEL_H__

#include "slog_export.h"
#include "slog_color.h"

#define SLOG_SUPPRESS_NOTHING 0

/* typedef enum slog_loglevel { */
/*     slog_loglevel_message = 1, */
/*     slog_loglevel_warning = (1 << 1), */
/*     slog_loglevel_error   = (1 << 2), */
/*     slog_loglevel_debug   = (1 << 3), */

/*     slog_loglevel_fatal   = (1 << 4), */
/* } slog_loglevel; */

typedef struct slog_loglevel {
    /* prefix of the message "%l" */
    const char *prefix;
    /* color of the message */
    slog_color   color;
    
    /* if the message can be suppressed */
    unsigned char unsuppressible;
    /* unique identifier of the loglevel 
     * (should be a power of 2) */
    unsigned int  id;
} slog_loglevel;

static const slog_loglevel slog_loglevel_message_s = {
    .prefix = "Message",
    .color = slog_color_message,
    .unsuppressible = 0,
    .id = (1 << 1),
};
static const slog_loglevel slog_loglevel_warning_s = {
    .prefix = "Warning",
    .color = slog_color_warning,
    .unsuppressible = 0,
    .id = (1 << 2)
};
static const slog_loglevel slog_loglevel_error_s = {
    .prefix = "Error",
    .color = slog_color_error,
    .unsuppressible = 0,
    .id = (1 << 3)
};
static const slog_loglevel slog_loglevel_debug_s = {
    .prefix = "Debug",
    .color = slog_color_warning,
    .unsuppressible = 0,
    .id = (1 << 4)
};
static const slog_loglevel slog_loglevel_fatal_s = {
    .prefix = "Fatal",
    .color = slog_color_fatal,
    .unsuppressible = 0,
    .id = (1 << 5)
};

#define slog_loglevel_message &slog_loglevel_message_s
#define slog_loglevel_warning &slog_loglevel_warning_s
#define slog_loglevel_error &slog_loglevel_error_s
#define slog_loglevel_debug &slog_loglevel_debug_s
#define slog_loglevel_fatal &slog_loglevel_fatal_s

static const slog_loglevel *slog_loglevels[] = {
    slog_loglevel_message,
    slog_loglevel_warning,
    slog_loglevel_error,
    slog_loglevel_debug,
    slog_loglevel_fatal
};

/* slog_newloglevel - create and register a new log level
 * @param level
 *   buffer, where the level information is dumped
 * @param color
 *   color of the messages with this log level
 * @param suppress
 *   can the loglevel be suppressed?
 * @note
 *   this function is thread unsafe as it reads and writes
 *   to a static variable */
SLOG_API void slog_newloglevel (slog_loglevel *level, const char *prefix, slog_color color, unsigned char suppress);

#endif
