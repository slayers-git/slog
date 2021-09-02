/* This file is part of the slog library.
 *
 * Copyright (C) 2021 by Sergey Lafin
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory. */

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "slog.h"
#include "slog_fmt.h"
#include "slog_log.h"
#include "slog_mem.h"

#include "slog_color.h"

#define SLOG_DEFAULT_FORMAT "[%l] %c: %L"

struct slog_stream {
    /* path to the file */
    const char *path;
    /* file descriptor */
    FILE *file;
    /* slog_fmt is a stream of tokens containing 
     * the info about the format */
    struct slog_fmt *fmt_head;
    /* redirect to the secondary output */
    unsigned char to_stdout;
    /* should the output to stdout be colorized */
    unsigned char colorized;
    /* which loglevels should be suppressed */
    unsigned int suppress;
};

slog_stream *slog_create (const char *path) {
    slog_stream *file = malloc (sizeof (struct slog_stream));
    if (!file)
        return NULL;

    file->to_stdout = 1;
    file->colorized = 0;
    /* we only suppress debug messages by default */
    file->suppress  = slog_loglevel_debug_s.id;
    slog_format (file, SLOG_DEFAULT_FORMAT);
    if (!path) {
        file->path = NULL;
        file->file = NULL;
        return file;
    }
    file->file = fopen (path, "w");
    if (!file->file) {
        slog_log_error ("Failed to open file %s for writing", path);
        free (file);
        return NULL;
    }
    size_t len = strlen (path) + 1;
    file->path = malloc (len);
    if (!file->path) {
        free (file);
        fclose (file->file);
        return NULL;
    }

    memcpy ((char *)file->path, path, len);
    return file;
}
slog_stream *slog_desc (FILE *fd) {
    slog_stream *f = slog_create (NULL);
    f->file = fd;
    return f;
}

void slog_close (slog_stream *file) {
    assert (file != NULL);
    if (file->file)
        fclose (file->file);
    if (file->path)
        free ((char *)file->path);
    if (file->fmt_head)
        slog_fmt_clear (file->fmt_head);

    slog_free (file);
}

void slog_printf (slog_stream *stream, const slog_loglevel *level, const char *fmt, ...) {
    assert (stream != NULL);
    assert (fmt != NULL);

    va_list va;
    va_start (va, fmt);
    slog_vprintf (stream, level, fmt, va);
    va_end (va);

}
void slog_vprintf (slog_stream *stream, const slog_loglevel *level, const char *fmt, va_list list) {
#define colorize()\
    if (stream->colorized) {\
        slog_set_color (level->color);\
    }

    assert (stream != NULL);
    assert (fmt != NULL);

    /* the message should be suppressed */
    if (stream->suppress & level->id && !(level->unsuppressible))
        return;

    va_list vac;
    va_copy (vac, list);
    size_t len = vsnprintf (NULL, 0, fmt, vac) + 1;
    char *message = slog_xalloc (len);
    if (!message) {
        slog_log_error ("Failed to allocate memory for a log message");
        return;
    }

    vsnprintf (message, len, fmt, list);
    va_end (vac);

    char *end_buf = slog_fmt_get_str (level, stream->fmt_head, message);
    slog_free (message);
    if (!end_buf) {
        slog_log_error ("Failed to get a formatted string");
        return;
    }
    len = strlen (end_buf);
    if (stream->file) {
        if (stream->to_stdout) {
            colorize ();
            puts (end_buf);
        }
        end_buf[len] = '\n';
        if (fwrite (end_buf, 1, len + 1, stream->file) != len + 1)
            slog_log_error ("Log message was written *partially* into the file %s", stream->path);
    } else {
        colorize ();
        puts (end_buf);
    }
    if (stream->colorized)
        slog_reset_color ();

    slog_free (end_buf);
}  
char slog_format (slog_stream *file, const char *fmt) {
    assert (file != NULL);
    
    slog_fmt *f = slog_fmt_create (fmt);
    if (!f)
        return 1;
    if (file->fmt_head)
        slog_fmt_clear (file->fmt_head);
    file->fmt_head = f;
    return 0;
}

void slog_output_to_stdout (slog_stream *file, unsigned char flag) {
    assert (file != NULL);
    file->to_stdout = flag;
}

void slog_colorized (slog_stream *file, unsigned char flag) {
    file->colorized = flag;
}

void slog_suppress (slog_stream *file, unsigned int mask) {
    assert (file != NULL);
    file->suppress = mask;
}
unsigned int slog_get_suppressed (slog_stream *file) {
    assert (file != NULL);
    return file->suppress;
}
