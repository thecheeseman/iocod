// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_ASSERT_H
#define CORE_ASSERT_H

#include <Core/Platform.h>

#define IOCOD_ASSERTION(...)                             \
    do {                                                 \
        if (__VA_ARGS__) {                               \
        } else {                                         \
            IOCOD_REPORT_ASSERTION_FAILURE(__VA_ARGS__); \
            IOCOD_BREAK();                               \
        }                                                \
    } while (0)

/// Report an assertion failure.
#define IOCOD_REPORT_ASSERTION_FAILURE(...) \
    iocod::detail::ReportAssertionFailure(__FILE__, __LINE__, __func__, #__VA_ARGS__)

/// @def IOCOD_ASSERT
/// @brief Asserts that the given expression is true.
#ifdef IOCOD_DEBUG
#define Assert(...)       IOCOD_ASSERTION(__VA_ARGS__)
#define Ensure(...)       IOCOD_ASSERTION(__VA_ARGS__)
#else
#define Assert(...)       ((void) 0)
#deifne Ensure(...)       ((void) 0)
#endif

namespace iocod::detail {

/// @brief Report an assertion failure.
/// @param[in] filename File where the assertion failed.
/// @param[in] line Line where the assertion failed.
/// @param[in] function Function where the assertion failed.
/// @param[in] message Message to report.
IOCOD_API void ReportAssertionFailure(const char* filename, int line, const char* function,
                                      const char* message);

} // namespace iocod::detail

#endif // CORE_ASSERT_H
