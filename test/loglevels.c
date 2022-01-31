#include "../slog.h"

/* create a variable for the new loglevel */
static slog_loglevel myloglevel;

static void myloglevel_print (slog_stream *stream, const char *, ...);

int main (void) {
    /* register the loglevel (thread unsafe btw) */
    slog_newloglevel (&myloglevel, "The prefix", slog_color_yellow, 1);
    /* the prefix is registered, create the logger and show the message */
    slog_stream *stream = slog_create (NULL, slog_flags_color);
    if (!stream) {
        return -1;
    }
    /* regular print */
    slog_printf (stream, &myloglevel, "My special message");
    /* or use a convenience function */
    myloglevel_print (stream, "Another message");
    slog_close (stream);
    return 0;
}

/* for the convenience of logging with this loglevel, we create a function */
void myloglevel_print (slog_stream *stream, const char *fmt, ...) {
    va_list list;
    va_start (list, fmt);
    slog_vprintf (stream, &myloglevel, fmt, list);
    va_end (list);
}
