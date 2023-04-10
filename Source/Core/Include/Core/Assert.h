// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_ASSERT_H
#define CORE_ASSERT_H

#ifdef __cplusplus
extern "C" {
#endif

/// Never assert.
#define IOCOD_NEVER_ASSERT(...) ((void) 0)

/// Always assert.
#define IOCOD_ALWAYS_ASSERT(...)                         \
    do {                                                 \
        if (!__VA_ARGS__) {                              \
        } else {                                         \
            IOCOD_REPORT_ASSERTION_FAILURE(__VA_ARGS__); \
            IOCOD_ASSERT_TRAP();                         \
        }                                                \
    } while (0)

/// Report an assertion failure.
#define IOCOD_REPORT_ASSERTION_FAILURE(...) \
    _iocod_report_assertion_failure(__FILE__, __LINE__, __func__, #__VA_ARGS__)

/// @def IOCOD_ASSERT
/// @brief Asserts that the given expression is true.
#ifdef IOCOD_DEBUG
#define IOCOD_ASSERT(...) IOCOD_ALWAYS_ASSERT(__VA_ARGS__)
#else
#define IOCOD_ASSERT(...) IOCOD_NEVER_ASSERT(__VA_ARGS__)
#endif

#ifdef _MSC_VER
#include <intrin.h>
#define IOCOD_ASSERT_TRAP() __debugbreak()
#elif defined(__GNUC__)
#define IOCOD_ASSERT_TRAP() __builtin_trap()
#else
#include <stdlib.h>
#define IOCOD_ASSERT_TRAP() abort()
#endif

/// @brief Report an assertion failure.
/// @param[in] filename File where the assertion failed.
/// @param[in] line Line where the assertion failed.
/// @param[in] function Function where the assertion failed.
/// @param[in] message Message to report.
void _iocod_report_assertion_failure(const char* filename, int line,
                                     const char* function, const char* message);

/// @def IOCOD_STATIC_ASSERT
/// @brief Wrapper around static_assert/_Static_assert.
#ifdef __cplusplus
#define IOCOD_STATIC_ASSERT(condition, message) static_assert(condition, message)
#else
#define IOCOD_STATIC_ASSERT(condition, message) _Static_assert(condition, message)
#endif

#ifdef __cplusplus
}
#endif

#endif // CORE_ASSERT_H
