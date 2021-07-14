# This file is part of the slog library.
# Copyright (C) 2021 by Sergey Lafin
# Licensed under the LGPL v2.1, see the file LICENSE in base directory.

cmake_minimum_required (VERSION 3.12)
project (slog)

option (SLOG_EXAMPLES "build examples" OFF)

set (SOURCE
    ./slog.c
    ./slog_fmt.c
    ./slog_log.c
    ./slog_mem.c
    ./slog_color.c
    ./slog_loglevel.c)
# only these files will be included in the include directory
set (INCLUDE
    ./slog.h
    ./slog_fmt.h
    ./slog_export.h
    ./slog_loglevel.h
    ./slog_color.h)

set (EXAMPLES
    ./test/fmt.c
    ./test/logfile.c)

if (CYGWIN OR MINGW OR UNIX)
    set (CMAKE_C_FLAGS_DEBUG "-Og -g")
    set (CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG -Wall")
elseif (MSVC)
    set (CMAKE_C_FLAGS_DEBUG "/Od /Zi")
    set (CMAKE_C_FLAGS_RELEASE "/O2 /DNDEBUG /Wall")
endif ()

add_library (slog SHARED ${SOURCE})
set_target_properties (slog PROPERTIES OUTPUT_NAME "slog")

set (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

if (SLOG_EXAMPLES)
    foreach (source_file IN LISTS EXAMPLES)
        get_filename_component (target_name ${source_file} NAME_WE)
    
        # Create an executable with the above name, building the above source
        add_executable ("${target_name}" "${source_file}")
        target_link_libraries (${target_name} slog)
    endforeach ()
endif ()

install (TARGETS slog
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

install (FILES ${INCLUDE} DESTINATION include/slog)
