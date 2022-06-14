#ifndef IOCOD_ASSERT_H
#define IOCOD_ASSERT_H

#include <assert.h>

/**
 * @defgroup assert_utilities Assert Utilities
 * @brief Assert utilities for testing code.
 * @{
 */

/**
 * @def IC_ASSERT
 * @brief Assert macro (essentially `assert()`). Defined for stylistic reasons.
 */
#define IC_ASSERT(expr)             assert(expr)

/**
 * @def IC_ASSERT_MSG
 * @brief Allows defining a message along with the assert statement.
 *
 * Strictly, there isn't much of a better way of doing this, but at least
 * you can give useful messages instead of vague asserts.
 */
#define IC_ASSERT_MSG(expr, msg)    assert(((void) msg, expr))

/**
 * @def IC_STATIC_ASSERT
 * @brief Compile time assertions.
 *
 * @note In C11, this is defined as `_Static_assert`.
 * @note On MSVC this is defined as `static_assert`.
 */
#if defined IC_PLATFORM_STD_C11 || IC_GCC_VERSION_CHECK(6, 0, 0) || \
    defined _Static_assert
#define IC_STATIC_ASSERT(expr, msg) _Static_assert(expr, msg)
#elif IC_MSVC_VERSION_CHECK(16, 0, 0)
#define IC_STATIC_ASSERT(expr, msg) static_assert(expr, msg)
#else
#define IC_STATIC_ASSERT(expr, msg)
#endif

/** @} */

#endif /* IOCOD_ASSERT_H */
