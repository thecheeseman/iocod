// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_VERSIONINFO_H
#define CORE_VERSIONINFO_H

#include <Core/Version.h>

#define IOCOD_VERSION_MAJOR 0
#define IOCOD_VERSION_MINOR 1
#define IOCOD_VERSION_PATCH 0
#define IOCOD_VERSION                                              \
    IOCOD_VERSION_ENCODE(IOCOD_VERSION_MAJOR, IOCOD_VERSION_MINOR, \
                         IOCOD_VERSION_PATCH)

// clang-format off
#define IOCOD_VERSION_STRING \
    IOCOD_VERSION_DECODE_MAJOR(IOCOD_VERSION) "." \
    IOCOD_VERSION_DECODE_MINOR(IOCOD_VERSION) "." \
    IOCOD_VERSION_DECODE_PATCH(IOCOD_VERSION)
// clang-format on

#define IOCOD_VERSION_DATE __DATE__

#ifdef __has_include
    #if __has_include(<iocod_config.h>)
        #include <iocod_config.h>
    #endif
#endif

#ifndef IOCOD_GIT_BRANCH
    #define IOCOD_GIT_BRANCH ""
#endif

#ifndef IOCOD_GIT_COMMIT_HASH
    #define IOCOD_GIT_COMMIT_HASH ""
#endif

#ifndef IOCOD_GIT_COMMIT_COUNT
    #define IOCOD_GIT_COMMIT_COUNT 0
#endif

#endif // CORE_VERSIONINFO_H