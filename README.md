# slog

slog is a fast and customizable logging library written in C89.

## Features

- Custom formats for the output
- Optionally colored output
- Certain log levels can be suppressed

## Example

```c
#include <slog/slog.h>

int main (void) {
    slog_stream *logger = slog_create ("mylog.txt", slog_flags_none);
    if (!logger)
        return -1;
    if (slog_format (logger, "[%l] %d/%M/%Y: %L") != 0) {
        return -2;
    }
    slog_message (logger, "My message");
    const char *message = "message";
    slog_printf (logger, slog_loglevel_message, "My formatted %s", message);

    slog_colorized (logger, 1);
    slog_error (logger, "This message now will be red (if the OS supports it)");
    slog_debug (logger, "You won't see this message unless...");
    slog_suppress (logger, SLOG_SUPPRESS_NOTHING);
    slog_debug (logger, "Now this message is visible!");

    slog_close (logger);
    return 0;
}
```

More informative examples can be found in `test/` directory.

## Format

You can use the following symbols to describe your log format:

- %c - standart time representation (see: acstime ())
- %h - hours (12h)
- %H - hours (24h)
- %m - minutes
- %s - seconds
- %d - days
- %M - months
- %y - years [2 digit]
- %Y - years [4 digit]
- %l - log level
- %L - message
- %p - seconds since the start of the program
- %P - seconds since 01/01/1970

## Building

To build and install slog library on a \*nix system, in your shell type:
```bash
mkdir build && cd build\
cmake ../ && make && sudo make install
```

You can also compile the contents of the `test/` directory by appending `-DSLOG_EXAMPLES=1` to the `cmake` command.
