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

#if __STDC_VERSION__ >= 199903L
#   define __slog_inline inline
#   if __STDC_VERSION__ >= 201112L
#       define __slog_noreturn _Noreturn
#   endif
#elif defined (__cplusplus)
#   define __slog_inline inline
#   define __slog_noreturn [[ noreturn ]]
#else
#   define __slog_inline
#endif

/* Check if the formatting is correct */
#if defined (__GNUC__) || defined (__MINGW32__) || defined (__MINGW64__)
#   define __slog_fmt_check(x, y) __attribute__((format(printf, x, y)))
#   ifndef __slog_noreturn
#       define __slog_noreturn __attribute__((noreturn))
#   endif
#else
#   define __slog_fmt_check(x, y)
#   ifndef __slog_noreturn
#       define __slog_noreturn
#   endif
#endif

#define SLOG_VERSION 103
#include "slog_export.h"
#include "slog_loglevel.h"
#include <stdio.h>

typedef struct slog_stream slog_stream;

typedef enum slog_flags {
    /* defaults */
    slog_flags_none = 0,
    /* rewrite the file instead of appending logs to it */
    slog_flags_rewrite = (1 << 1),
    /* disable logging to stdout */
    slog_flags_nostdout = (1 << 2),
    /* colorize the output (if supported) */
    slog_flags_color = (1 << 3)
} slog_flags;

/* slog_create - initialize an slog_stream
 * @param path
 *   path to the stream, where the output will be written, can be NULL
 * @param flags
 *   stream flags
 * @return
 *   valid pointer to slog_stream on success, otherwise NULL */
SLOG_API slog_stream *slog_create (const char *path, unsigned int flags);
/* slog_desc - create an slog_stream structure from an existing FILE
 * @param fd
 *   pointer to the FILE structure
 * @return
 *   valid pointer to slog_stream on success, otherwise NULL */
SLOG_API slog_stream *slog_desc (FILE *fd);
/* slog_close - close an slog_stream structure
 * @param stream
 *   stream to be closed */
SLOG_API void slog_close (slog_stream *stream);

/* slog_puts - print a string 
 * @param stream 
 *   pointer to the slog_stream structure 
 * @param level 
 *   log level of the message 
 * @param message 
 *   message to print */
SLOG_API void slog_puts (slog_stream *stream, const slog_loglevel *level, const char *message);

/* slog_printf - print a formated message
 * @param stream
 *   pointer to the slog_stream structure
 * @param level
 *   log level of the message
 * @param fmt
 *   message or a formated string (like in printf ())
 * @param ...
 *   variadic arguments for the format string */
SLOG_API void slog_printf (slog_stream *stream, const slog_loglevel *level, const char *fmt, ...) __slog_fmt_check(3, 4);
/* slog_vprintf - print a formated message (va_list)
 * @param stream
 *   pointer to the slog_stream structure
 * @param level
 *   log level of the message
 * @param fmt
 *   message or formated string (like in printf ())
 * @param list
 *   variadic arguments for the format string */
SLOG_API void slog_vprintf (slog_stream *stream, const slog_loglevel *level, const char *fmt, va_list list);

/* slog_format - set the format string for the slog_stream
 * @param stream
 *   pointer to the string slog_stream structure
 * @param fmt
 *   format string
 * @return
 *   0 on success, non-zero otherwise */
SLOG_API char slog_format (slog_stream *stream, const char *fmt);

/* slog_output_to_stdout - set the to_stdout flag
 * @param stream
 *   pointer to the slog_stream structure
 * @param state
 *   state (0, 1) */
SLOG_API void slog_output_to_stdout (slog_stream *stream, unsigned char state);

/* slog_colorized - set the colorized flag
 * @param stream
 *   pointer to the slog_stream structure
 * @param state
 *   state (0, 1) */
SLOG_API void slog_colorized (slog_stream *stream, unsigned char state);

/* slog_suppress - suppress certain loglevels
 * @param stream
 *   pointer to the slog_stream structure
 * @param mask
 *   selected loglevels to be suppressed
 * @note
 *   slog_loglevel_fatal cannot be suppressed */
SLOG_API void slog_suppress (slog_stream *stream, unsigned int mask);
/* slog_get_suppressed - get suppressed loglevels
 * @param stream
 *   pointer to the slog_stream structure
 * @return
 *   suppressed levels */
SLOG_API unsigned int slog_get_suppressed (slog_stream *stream);

/***********************************************************/
/* Made as inline funcs to allow for GCC's format checking */
/***********************************************************/

#include <stdarg.h>
#include <stdlib.h>

static __slog_inline __slog_fmt_check(2, 3) void slog_message (slog_stream *stream, const char *fmt, ...) {
    va_list list;
    va_start (list, fmt);
    slog_vprintf (stream, slog_loglevel_message, fmt, list);
    va_end (list);
}
static __slog_inline __slog_fmt_check(2, 3) void slog_warning (slog_stream *stream, const char *fmt, ...) {
    va_list list;
    va_start (list, fmt);
    slog_vprintf (stream, slog_loglevel_warning, fmt, list);
    va_end (list);
}
static __slog_inline __slog_fmt_check(2, 3) void slog_error (slog_stream *stream, const char *fmt, ...) {
    va_list list;
    va_start (list, fmt);
    slog_vprintf (stream, slog_loglevel_error, fmt, list);
    va_end (list);
}
static __slog_inline __slog_fmt_check(2, 3) void slog_debug (slog_stream *stream, const char *fmt, ...) {
    va_list list;
    va_start (list, fmt);
    slog_vprintf (stream, slog_loglevel_debug, fmt, list);
    va_end (list);
}

/* slog_fatal - print a fatal error message and exit the program with "status"
 * @param stream
 *   pointer to slog_stream structure
 * @param status
 *   exit status
 * @param fmt
 *   format string
 * @param ...
 *   variadic arguments for the format string */
static __slog_noreturn __slog_inline __slog_fmt_check(3, 4) void slog_fatal (slog_stream *stream, int status, const char *fmt, ...) {
    va_list list;
    va_start (list, fmt);
    slog_vprintf (stream, slog_loglevel_fatal, fmt, list);
    va_end (list);
    exit (status);
}

#ifdef __cplusplus
}
#endif

#undef __slog_inline
#undef __slog_fmt_check
#undef __slog_noreturn
#endif 
