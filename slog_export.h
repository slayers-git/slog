/* This file is part of the slog library.
 *
 * Copyright (C) 2021 by Sergey Lafin
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory. */

#ifndef __SLOG_EXPORT_H__
#define __SLOG_EXPORT_H__

#if defined(_WIN32) || defined(__WIN32__)
#	if defined(slog_EXPORTS) /* should be added by cmake */
#		define  SLOG_API __declspec(dllexport)
#	else
#		define  SLOG_API __declspec(dllimport)
#	endif
#else
#	define SLOG_API
#endif

#endif
