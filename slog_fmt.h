/* This file is part of the slog library.
 *
 * Copyright (C) 2021 by Sergey Lafin
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory. */

#ifndef __SLOG_FMT_H__
#define __SLOG_FMT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdarg.h>

#include "slog_export.h"
#include "slog_loglevel.h"

typedef struct slog_fmt {
    struct slog_fmt_tok *fmt_tok_head;
    struct slog_fmt_str *fmt_str_head;
} slog_fmt;

/* a structure which contains a stream of tokens */
typedef struct slog_fmt_tok slog_fmt_tok;
/* a structure which is tied to slog_fmt, contains 
 * a stream of literals, to which tokens may refer */
typedef struct slog_fmt_str slog_fmt_str;

/* slog_fmt_create - create an slog_fmt structure
 * @param str
 *   string for the format 
 * @return
 *   a valid pointer to the slog_fmt structure, NULL otherwise */
SLOG_API slog_fmt *slog_fmt_create  (const char *str);
/* slog_fmt_get_str - get the string, formed with a slog_fmt
 * @param level
 *   loglevel of a log entry
 * @param fmt
 *   format to be used on the string
 * @param message
 *   message of the log entry
 * @return
 *   valid pointer to a string, NULL otherwise */
SLOG_API char *slog_fmt_get_str (const slog_loglevel *level, slog_fmt *fmt, const char *message);
/* slog_vfmt_get_str - get the string, formed with a slog_fmt to a dynamic buffer
 * @param level
 *   loglevel of a log entry
 * @param fmt
 *   format to be used on the string
 * @param message
 *   message of the log entry
 * @param mfmt
 *   message format (as in token %L)
 * @param list 
 *   vararg list pointer for mfmt
 * @return
 *   valid pointer to a string, NULL otherwise */
SLOG_API char *slog_vfmt_get_str (const slog_loglevel *level, slog_fmt *fmt, const char *mfmt, va_list *list);

SLOG_API void slog_fmt_clear (slog_fmt *p);

#ifdef __cplusplus
}
#endif

#endif
