/* This file is part of the slog library.
 *
 * Copyright (C) 2021 by Sergey Lafin
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory. */

/* I AM MAKING A VERY BOLD ASSUMPTION HERE 
 * THAT THE TM_BASE_YEAR IS ALWAYS 1900 */
#define SLOG_BASE_YEAR 1900
#define SLOG_BUFSIZ      32

#include "slog_fmt.h"
#include "slog_log.h"
#include "slog_mem.h"
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
static char *slog_itoa_pad (unsigned int n, unsigned char padding) {
    assert (padding <= 12);
#define pad(p)\
    while ((signed char)padding - p > 0) {\
        *pb = '0';\
        ++pb;\
        ++n_size;\
        --padding;\
    }
    char buf[12],
         *pb = buf;
    int digit;
    unsigned char n_size = 0;
    if (n == 0) {
        *pb = '0';
        ++n_size;
        ++pb;
        pad (1);
        /* gotos are sometimes useful, right? */
        goto buf_to_res;
    }
    while (n > 0) {
        digit = n % 10;
        *pb = digit + 48;
        ++n_size;
        n /= 10;
        ++pb;
    }
    char p = n_size;
    pad (p);
    --n_size;
    char temp;
    unsigned char i;
    for (i = 0; i < (n_size + 1) / 2; ++i) {
        temp = buf[i];
        buf[i] = buf[n_size - i];
        buf[n_size - i] = temp;
    }
buf_to_res:
    buf[++n_size] = 0x0;
    char *ret = slog_xalloc (n_size);
    if (!ret)
        return NULL;
    strcpy (ret, buf);

    return ret;
}
static char *_m_strcat (char *str, const char *what) {
    size_t size = strlen (str) + strlen (what) + 1;
    char *res = slog_realloc (str, size);
    if (!res)
        return NULL;
    strcat (res, what);
    return res;
}
char *slog_fmt_get_str (const slog_loglevel *level, slog_fmt *fmt, const char *message) {
    char *res = slog_xalloc (1),
         *buf;
    *res = 0x0;
    slog_fmt_tok *tok = fmt->fmt_tok_head;
    slog_fmt_str *str = fmt->fmt_str_head;
    time_t ep;
    time (&ep);
    struct tm *c_time = localtime (&ep);
    slog_bool aloc = slog_true;

    while (tok) {
        switch (tok->token) {
            case slog_token_none:
                break;
            case slog_token_level:
                buf = (char *)level->prefix;
                aloc = slog_false;
                break;
            case slog_token_hour12: {
                char _b;
                buf = slog_itoa_pad ((_b = c_time->tm_hour % 12) != 0 ? _b : 1, 2);
                break;
            }
            case slog_token_hour24:
                buf = slog_itoa_pad (c_time->tm_hour, 2);
                break;
            case slog_token_minutes:
                buf = slog_itoa_pad (c_time->tm_min, 2);
                break;
            case slog_token_seconds:
                buf = slog_itoa_pad (c_time->tm_sec, 2);
                break;
            case slog_token_literal:
                buf = (char *)str->str;
                str = str->next;
                aloc = slog_false;
                break;
            case slog_token_message:
                buf = (char *)message;
                aloc = slog_false;
                break;
            case slog_token_day:
                buf = slog_itoa_pad (c_time->tm_mday, 2);
                break;
            case slog_token_month:
                buf = slog_itoa_pad (c_time->tm_mon, 2);
                break;
            case slog_token_year2:
                buf = slog_itoa_pad ((SLOG_BASE_YEAR + c_time->tm_year) % 100, 2);
                break;
            case slog_token_year4:
                buf = slog_itoa_pad (SLOG_BASE_YEAR + c_time->tm_year, 0);
                break;
            case slog_token_space:
                buf = " ";
                break;
            case slog_token_runtime:
                buf = slog_itoa_pad ((long long)(clock () / CLOCKS_PER_SEC), 0);
                break;
            case slog_token_timestamp:
                /* eventually, this will overflow. Fortunately enough it will only
                 * happen in 2038. */
                buf = slog_itoa_pad ((int)ep, 0);
                break;
            case slog_token_ctime:
                buf = asctime (c_time);
                {
                    /* AFAIK windows manages strings in mysterious ways */
                    char *nlp = strchr (buf, '\r');
                    if (!nlp)
                        nlp = strchr (buf, '\n');
                    if (nlp)
                        *nlp = 0x0;
                }
                aloc = slog_false;
                break;
            default:
                break;
        }
        res = _m_strcat (res, buf);
        if (aloc) {
            slog_free (buf);
        }
        aloc = slog_true;
        tok = tok->next;
    }
    return res;
}
