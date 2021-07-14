/* This file is part of the slog library.
 *
 * Copyright (C) 2021 by Sergey Lafin
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory. */

#ifndef __SLOC_MEM_H__
#define __SLOC_MEM_H__

#include "slog_export.h"
#include <stddef.h>

/* just inform the user if allocation was failed */
SLOG_API void *slog_xalloc (size_t nr);
/* abort the execution if malloc failed */
SLOG_API void *slog_xalloc_die (size_t nr);
SLOG_API void *slog_realloc (void *, size_t);
SLOG_API void  slog_free (void *);

#endif
