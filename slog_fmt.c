/* This file is part of the slog library.
 *
 * Copyright (C) 2021 by Sergey Lafin
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory. */

/* I AM MAKING A VERY BOLD ASSUMPTION HERE 
 * THAT THE TM_BASE_YEAR IS ALWAYS 1900 */
#define SLOG_BASE_YEAR 1900
/* This defines the size of the primary allocation of strings for
 * slog_*fmt_get_str () functions.
 * Returned strings will be aligned to this size, so they're going 
 * to be a multiple of this value */
#define SLOG_BUFSIZ     512

#include "slog_fmt.h"
#include "slog_log.h"
#include "slog_mem.h"

#include <stdio.h>
#include <assert.h>
#include <time.h>

#include <string.h>

typedef enum slog_token {
    slog_token_none,
    slog_token_ctime,
    slog_token_level,
    slog_token_hour24,
    slog_token_hour12,
    slog_token_minutes,
    slog_token_seconds,
    slog_token_day,
    slog_token_month,
    slog_token_year2,
    slog_token_year4,
    slog_token_space,
    slog_token_literal,
    slog_token_message,
    slog_token_timestamp,
    slog_token_runtime
} slog_token;

struct slog_fmt_tok {
    slog_token token;
    struct slog_fmt_tok *next;
};
struct slog_fmt_str {
    const char *str;
    slog_fmt_str *next;
};

static slog_fmt_tok *_alloc_head () {
    slog_fmt_tok *p = slog_xalloc (sizeof (slog_fmt_tok));
    if (!p)
        return NULL;
    p->token = slog_token_none;
    p->next = NULL;
    return p;
}
static slog_fmt_str *_alloc_head_str () {
    slog_fmt_str *p = slog_xalloc (sizeof (slog_fmt_str));
    if (!p)
        return NULL;
    p->str = NULL;
    p->next = NULL;
    return p;
}

static slog_fmt_tok *_add_node (const slog_token token, slog_fmt_tok *fmt) {
    if (fmt->token != slog_token_none) {
        while (fmt->next) fmt = fmt->next;
        fmt->next = slog_xalloc (sizeof (slog_fmt_tok));
        if (!fmt)
            return NULL;
        fmt = fmt->next;
    }
    fmt->next = NULL;
    fmt->token = token;

    return fmt;
}
static slog_fmt_str *_add_node_str (const char *str, slog_fmt_str *fmt) {
    if (fmt->str != NULL) {
        while (fmt->next) fmt = fmt->next;
        fmt->next = slog_xalloc (sizeof (slog_fmt_str));
        if (!fmt)
            return NULL;
        fmt = fmt->next;
    }
    fmt->next = NULL;
    
    size_t len = strlen (str) + 1;
    fmt->str = slog_xalloc (len);
    memcpy ((char *)fmt->str, str, len);

    return fmt;
}
static void _slog_fmt_tok_clear (slog_fmt_tok *fmt) {
    assert (fmt != NULL);
    slog_fmt_tok *p = fmt;
    while (fmt) {
        p = fmt->next;
        slog_free (fmt);
        fmt = p;
    }
}
static void _slog_fmt_str_clear (slog_fmt_str *fmt_str) {
    assert (fmt_str != NULL);
    slog_fmt_str *p = fmt_str;
    while (fmt_str) {
        p = fmt_str->next;
        if (fmt_str->str)
            slog_free ((char *)fmt_str->str);
        slog_free (fmt_str);
        fmt_str = p;
    }
}

typedef enum { slog_false = 0, slog_true } slog_bool;
slog_fmt *slog_fmt_create (const char *str) {
#define addchar(x) {\
    *pb = x;\
    ++pb;\
}
#define untext()\
    if (text) {\
        text = slog_false;\
        *pb = 0x0;\
        _add_node_str (buf, fmtsp);\
        pb = buf;\
    }
#define check_await()\
    
    assert (str != NULL);

    /* fmt head node */
    slog_fmt_tok *fmt_tok_head = _alloc_head (),
    /* fmt last node */
                 *fmtp = fmt_tok_head;

    /* fmt_str head node */
    slog_fmt_str *fmt_str_head = _alloc_head_str (),
                 *fmtsp = fmt_str_head;

    if (!fmt_str_head || !fmt_tok_head) {
        slog_log_error ("Could not allocate memory for a format string");
        return NULL;
    }

    char buf[SLOG_BUFSIZ],
         *pb = buf;
    const char *p = str;

    size_t lit_size = 0;

    /* is it a regular literal */
    slog_bool text  = slog_false;
    while (*p) {
        if (*p == '%') {
            ++p;
            if (!(*p))
                break;
            switch (*p) {
                case '%':
                    addchar (*p);
                    break;
                case 'c':
                    fmtp = _add_node (slog_token_ctime, fmtp);
                    untext ();
                    break;
                case 'h':
                    fmtp = _add_node (slog_token_hour12, fmtp);
                    untext ();
                    break;
                case 'H':
                    fmtp = _add_node (slog_token_hour24, fmtp);
                    untext ();
                    break;
                case 'm':
                    fmtp = _add_node (slog_token_minutes, fmtp);
                    untext ();
                    break;
                case 's':
                    fmtp = _add_node (slog_token_seconds, fmtp);
                    untext ();
                    break;
                case 'd':
                    fmtp = _add_node (slog_token_day, fmtp);
                    untext ();
                    break;
                case 'M':
                    fmtp = _add_node (slog_token_month, fmtp);
                    untext ();
                    break;
                case 'y':
                    fmtp = _add_node (slog_token_year2, fmtp);
                    untext ();
                    break;
                case 'Y':
                    fmtp = _add_node (slog_token_year4, fmtp);
                    untext ();
                    break;
                case 'p':
                    fmtp = _add_node (slog_token_runtime, fmtp);
                    untext ();
                    break;
                case 'P':
                    fmtp = _add_node (slog_token_timestamp, fmtp);
                    untext ();
                    break;
                case ' ':
                    if (text) {
                        addchar (*p);
                        break;
                    }
                    fmtp = _add_node (slog_token_space, fmtp);
                    break;
                case 'l':
                    fmtp = _add_node (slog_token_level, fmtp);
                    untext ();
                    break;
                case 'L':
                    fmtp = _add_node (slog_token_message, fmtp);
                    untext ();
                    break;
                default:
                    slog_log_error ("Invalid format syntax");
                    _slog_fmt_tok_clear (fmt_tok_head);
                    _slog_fmt_str_clear (fmt_str_head);

                    return NULL;
            }
        } else {
                if (!text) {
                    fmtp = _add_node (slog_token_literal, fmtp);
                    text = slog_true;
                    lit_size = 0;
                }
                ++lit_size;
                if (lit_size >= SLOG_BUFSIZ) {
                    slog_log_error ("Size of the string literal exceeds the size of the buffer!");
                    _slog_fmt_tok_clear (fmt_tok_head);
                    _slog_fmt_str_clear (fmt_str_head);

                    return NULL;
                }
                addchar (*p);
        }
        ++p;

    }

    slog_fmt *res = slog_xalloc (sizeof (slog_fmt));
    if (!res) {
        _slog_fmt_tok_clear (fmt_tok_head);
        _slog_fmt_str_clear (fmt_str_head);
        return NULL;
    }
    res->fmt_tok_head = fmt_tok_head;
    res->fmt_str_head = fmt_str_head;
    
    return res;
#undef addchar 
#undef untext
}
void slog_fmt_clear (slog_fmt *p) {
    assert (p != NULL);
    _slog_fmt_tok_clear (p->fmt_tok_head);
    _slog_fmt_str_clear (p->fmt_str_head);
    slog_free (p);
}

/* unsigned itoa with an optional padding */
static char *slog_itoa_pad (char *buf, unsigned n, unsigned pad) {
    unsigned result, nlen;
    char *p = buf;
    unsigned i;

    slog_bool needs_pad = pad ? slog_true : slog_false;

    /* convert the number to ASCII */
    if (!n) {
        *p++ = '0';
        nlen = 1;
        needs_pad = ((signed)--pad > 0);
    }
    else {
        for (i = 0; n; ++i) {
            *p++ = n % 10 + 48;
            n /= 10;

            if (needs_pad && (signed)(--pad) < 0) {
                needs_pad = slog_false;
                break;
            }
        }
        nlen = i;
    }

    /* add padding */
    if (needs_pad) {
        for (i = 0; i < pad; ++i) {
            buf[nlen++] = '0';
        }
    }
    
    /* invert the order */
    for (i = 0; i < nlen / 2; ++i) {
        char temp = buf[i];
        buf[i] = buf[nlen - i - 1];
        buf[nlen - i - 1] =   temp;
    }
    buf[nlen] = 0x0;

    return buf;
}

char *slog_vfmt_get_str (const slog_loglevel *level, slog_fmt *fmt, const char *mfmt, va_list *va) {
    char buf[48],
         *ptr;

    char *_str = slog_xalloc (SLOG_BUFSIZ);
#   define _realloc_to_fill(need) {           \
        bufsiz = bufsiz + need / SLOG_BUFSIZ; \
        _str = slog_realloc (_str, bufsiz);   \
    }

    /* if message is required twice or more times for some reason */
    const char *msg_ptr = va ? NULL : mfmt;
    size_t msg_size     = 0;
    
    size_t written = 0,
           bufsiz  = SLOG_BUFSIZ;

    slog_fmt_tok *tok = fmt->fmt_tok_head;
    slog_fmt_str *str = fmt->fmt_str_head;

    time_t ep;
    time (&ep);

    struct tm *c_time;
    c_time = localtime (&ep);

    while (tok) {
        switch (tok->token) {
            case slog_token_none:
                break;
            case slog_token_level:
                ptr = (char *)level->prefix;
                break;
            case slog_token_hour12: {
                char _b;
                ptr = slog_itoa_pad (buf, (_b = c_time->tm_hour % 12) != 0 ? _b : 1, 2);
                break;
            }
            case slog_token_hour24:
                ptr = slog_itoa_pad (buf, c_time->tm_hour, 2);
                break;
            case slog_token_minutes:
                ptr = slog_itoa_pad (buf, c_time->tm_min, 2);
                break;
            case slog_token_seconds:
                ptr = slog_itoa_pad (buf, c_time->tm_sec, 2);
                break;
            case slog_token_literal:
                ptr = (char *)str->str;
                str = str->next;
                break;
            case slog_token_message:
                if (!msg_ptr) {
                    va_list vac;
                    va_copy (vac, *va);
                    msg_size = vsnprintf (&_str[written], 0, mfmt, vac);
                    va_end (vac);

                    if (msg_size + 1 >= bufsiz - written) {
                        _realloc_to_fill (msg_size + 1);
                    }

                    (void)vsnprintf (&_str[written], msg_size + 1, mfmt, *va);
                    msg_ptr = &_str[written];
                    written += msg_size;

                    ptr = NULL;
                } else {
                    ptr = (char *)msg_ptr;
                }
                break;
            case slog_token_day:
                ptr = slog_itoa_pad (buf, c_time->tm_mday, 2);
                break;
            case slog_token_month:
                ptr = slog_itoa_pad (buf, c_time->tm_mon + 1, 2);
                break;
            case slog_token_year2:
                ptr = slog_itoa_pad (buf, (SLOG_BASE_YEAR + c_time->tm_year) % 100, 2);
                break;
            case slog_token_year4:
                ptr = slog_itoa_pad (buf, SLOG_BASE_YEAR + c_time->tm_year, 0);
                break;
            case slog_token_space:
                ptr = " ";
                break;
            case slog_token_runtime:
                ptr = slog_itoa_pad (buf, (long long)(clock () / CLOCKS_PER_SEC), 0);
                break;
            case slog_token_timestamp:
                /* eventually, this will overflow. Fortunately enough it will only
                 * happen in 2038. */
                ptr = slog_itoa_pad (buf, (int)ep, 0);
                break;
            case slog_token_ctime:
                strncpy (buf, asctime (c_time), 48);
                {
                    /* AFAIK windows manages strings in mysterious ways */
                    char *nlp = strchr (buf, '\r');
                    if (!nlp)
                        nlp = strchr (buf, '\n');
                    if (nlp)
                        *nlp = 0x0;
                }
                ptr = buf;
                break;
            default:
                break;
        }

        if (ptr) {
            size_t tsiz = strlen (ptr);
            if (written + tsiz >= bufsiz) {
                _realloc_to_fill (written + tsiz);
            }
            memcpy (&_str[written], ptr, tsiz);
            written += tsiz;
        }

        tok = tok->next;
    }

    _str[written] = 0x0;

    return _str;
}

/* compatibility with older versions */
char *slog_fmt_get_str (const slog_loglevel *level, slog_fmt *fmt, const char *message) {
    return slog_vfmt_get_str (level, fmt, message, NULL);
}
