/* slog.h - fast and customizable logging library for C/C++
 * Copyright (C) 2021 Sergey Lafin
 *
 * This library is free software; you can redistribute it and/or
 * odify it under the terms of the GNU Lesser General Public
 * icense as published by the Free Software Foundation; either
 * ersion 2.1 of the License, or (at your option) any later version.
 * 
 * his library is distributed in the hope that it will be useful,
 * ut WITHOUT ANY WARRANTY; without even the implied warranty of
 * ERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * esser General Public License for more details.
 * 
 * ou should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA */

#ifndef __SLOG_H__ 
#define __SLOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#if __STDC_VERSION__ >= 199903L || defined (__cplusplus)
#   define __inline inline
#else
#   define __inline
#endif

#define SLOG_VERSION 100
#include "slog_export.h"
#include "slog_loglevel.h"
#include <stdio.h>

typedef struct slog_stream slog_stream;

/* slog_create - initialize an slog_stream
 * @param path
 *   path to the stream, where the output will be written, can be NULL
 * @return
 *   valid pointer to slog_stream on success, otherwise NULL */
SLOG_API slog_stream *slog_create (const char *path);
/* slog_desc - create an slog_stream structure from an existing FILE
 * @param fd
 *   pointer to the FILE structure
 * @return
 *   valid pointer to slog_stream on success, otherwise NULL */
SLOG_API slog_stream *slog_desc (FILE *fd);
/* slog_close - close an slog_stream structure
 * @param stream
 *   stream to be closed */
SLOG_API void      slog_close (slog_stream  *stream);

/* slog_printf - print a formated message
 * @param stream
 *   pointer to the slog_stream structure
 * @param level
 *   log level of the message
 * @param fmt
 *   message or formated string (like in printf ())
 * @param ...
 *   variadic arguments for the format string */
SLOG_API void slog_printf (slog_stream *stream, slog_loglevel level, const char *fmt, ...);
/* slog_vprintf - print a formated message (va_list)
 * @param stream
 *   pointer to the slog_stream structure
 * @param level
 *   log level of the message
 * @param fmt
 *   message or formated string (like in printf ())
 * @param list
 *   variadic arguments for the format string */
SLOG_API void slog_vprintf (slog_stream *stream, slog_loglevel level, const char *fmt, va_list list);

/* slog_format - set the format string for the slog_stream
 * @param stream
 *   pointer to the string slog_stream structure
 * @param fmt
 *   format string
 * #return
 *   0 on success, non-zero otherwise */
SLOG_API char slog_format (slog_stream *stream, const char *fmt);

/* slog_output_to_stdout - set the to_stdout flag
 * @param stream
 *   pointer to the slog_stream structure
 * @param state
 *   state (0, 1) */
SLOG_API void slog_output_to_stdout (slog_stream *stream, char);

/* slog_colorized - set the colorized flag
 * @param stream
 *   pointer to the slog_stream structure
 * @param state
 *   state (0, 1) */
SLOG_API void slog_colorized (slog_stream *stream, char state);

/* slog_suppress - suppress certain loglevels
 * @param stream
 *   pointer to the slog_stream structure
 * @param mask
 *   selected loglevels to suppress */
SLOG_API void slog_suppress (slog_stream *stream, short mask);
/* slog_get_suppressed - get suppressed loglevels
 * @param stream
 *   pointer to the slog_stream structure
 * @return
 *   suppressed levels */
SLOG_API short slog_get_suppressed (slog_stream *stream);

#ifdef __cplusplus
}
#endif

/****************************************************************/
/* Made as inline funcs to allow dropping of variadic arguments */
/****************************************************************/

#include <stdarg.h>

static __inline void slog_message (slog_stream *stream, const char *fmt, ...) {
    va_list list;
    va_start (list, fmt);
    slog_vprintf (stream, slog_loglevel_message, fmt, list);
    va_end (list);
}
static __inline void slog_warning (slog_stream *stream, const char *fmt, ...) {
    va_list list;
    va_start (list, fmt);
    slog_vprintf (stream, slog_loglevel_warning, fmt, list);
    va_end (list);
}
static __inline void slog_error (slog_stream *stream, const char *fmt, ...) {
    va_list list;
    va_start (list, fmt);
    slog_vprintf (stream, slog_loglevel_error, fmt, list);
    va_end (list);
}
static __inline void slog_debug (slog_stream *stream, const char *fmt, ...) {
    va_list list;
    va_start (list, fmt);
    slog_vprintf (stream, slog_loglevel_debug, fmt, list);
    va_end (list);
}
    

#undef __inline
#endif 
