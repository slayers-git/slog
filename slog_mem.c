/* This file is part of the slog library.
 *
 * Copyright (C) 2021 by Sergey Lafin
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory. */

#include <stdlib.h>
#include "slog_mem.h"
#include "slog_log.h"
#include <assert.h>

void *slog_xalloc (size_t nr) {
    assert (nr > 0);

    void *blk = malloc (nr);
    if (!blk) {
        slog_log_error ("Could not allocate memory");
        return NULL;
    }
    return blk;
}
void *slog_xalloc_die (size_t nr) {
    void *blk = slog_xalloc (nr);
    if (!blk) {
        slog_log_error ("Could not allocate memory");
        abort ();
    }
    return blk;
}
void *slog_realloc (void *blk, size_t nr) {
    assert (blk != NULL);
    void *p = realloc (blk, nr);
    if (!p) {
        slog_log_error ("Could not reallocate memory");
        return NULL;
    }
    return p;
}
void slog_free (void *blk) {
    assert (blk != NULL);
    free (blk);
}
