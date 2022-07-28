/*
================================================================================
iocod
Copyright (C) 2022 thecheeseman

This file is part of the iocod GPL source code.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
================================================================================
*/

/**
 * @defgroup platform Platform
 * @brief Utilities for platform/compiler detection, features, attributes, etc.
 * 
 * @{
 */

#ifndef IC_PLATFORM_H
#define IC_PLATFORM_H

/* always include config.h */
#if defined HAVE_CONFIG_H
#include "config.h"
#endif

#include "iocod/version.h"

#ifdef __cplusplus
#define IC_BEGIN_C_DECLS extern "C" {
#define IC_END_C_DECLS }
#define IC_C_DECL extern "C"
#else
#define IC_BEGIN_C_DECLS
#define IC_END_C_DECLS
#define IC_C_DECL
#endif

/**
 * @def IC_PLATFORM_COMPILER
 * @brief A string containing the name of the compiler used. This is defined
 * by CMake, but if it is not for some reason, we will manually check.
 * @since 0.110.0
 */
/**
 * @def IC_PLATFORM_MSVC
 * @brief Defined if we are compiling with MSVC.
 * @since 0.110.0
 */
/**
 * @def IC_PLATFORM_MINGW
 * @brief Defined if we are compiling with MinGW.
 * @since 0.110.0
 */
/**
 * @def IC_PLATFORM_GCC
 * @brief Defined if we are compiling with GCC.
 * @since 0.110.0
 */
/**
 * @def IC_PLATFORM_CLANG
 * @brief Defined if we are compiling with Clang.
 * @since 0.110.0
 */
#ifndef IC_PLATFORM_COMPILER
/* MSVC */
#if defined _MSC_VER 
#define IC_PLATFORM_MSVC
#define IC_PLATFORM_COMPILER "msvc"
/* MinGW */
#elif defined __MINGW32__ || defined __MINGW64__
#define IC_PLATFORM_MINGW
#define IC_PLATFORM_COMPILER "mingw"
/* Clang */
#elif defined __clang__
#define IC_PLATFORM_CLANG
#define IC_PLATFORM_COMPILER "clang"
/* GCC */
#elif (defined __GNUC__ && !defined __clang__)
#define IC_PLATFORM_GCC
#define IC_PLATFORM_COMPILER "gcc"
/* Doxygen */
#elif defined __DOXYGEN__
#define IC_PLATFORM_MSVC
#define IC_PLATFORM_MINGW
#define IC_PLATFORM_GCC
#define IC_PLATFORM_CLANG
#define IC_PLATFORM_COMPILER "doxygen"
#define IC_PLATFORM_DOXYGEN
#else
#define IC_PLATFORM_UNKNOWN
#define IC_PLATFORM_COMPILER "unknown"
#endif /* _MSC_VER */
#endif

// include for sal stuff
#ifdef IC_PLATFORM_MSVC
#include <sal.h>
#endif

/**
 * @def IC_GNUC_VERSION
 * @brief Defined if we are using a GNUC compatible compiler. 
 *
 * @note This does not necessarily mean GCC, as some compilers 
 * masquerade as GNUC (e.g. clang).
 * 
 * @since 0.110.0
 */
#if defined __GNUC__ && defined __GNUC_PATCHLEVEL__
#define IC_GNUC_VERSION \
    IC_VERSION_ENCODE(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#elif defined __GNUC__
#define IC_GNUC_VERSION IC_VERSION_ENCODE(__GNUC__, __GNUC_MINOR__, 0)
#endif

/**
 * @def IC_GNUC_VERSION_CHECK
 * @brief Checks if the current compiler is at least this version. 
 * 
 * @param maj major version
 * @param min minor version
 * @param patch patch version
 * 
 * @note This only has any meaning on GNUC compatible compilers.
 * 
 * @since 0.110.0
 */
#if defined IC_GNUC_VERSION 
#define IC_GNUC_VERSION_CHECK(maj, min, patch) \
    (IC_GNUC_VERSION >= IC_VERSION_ENCODE(maj, min, patch))
#else
#define IC_GNUC_VERSION_CHECK(maj, min, patch) (0)
#endif

/**
 * @def IC_GCC_VERSION
 * @brief Defined if we are _really_ using GCC.
 * 
 * @since 0.110.0
 */
#if defined IC_GNUC_VERSION && !defined __clang__
#define IC_GCC_VERSION IC_GNUC_VERSION
#endif

/**
 * @def IC_GCC_VERSION_CHECK
 * @brief Checks if GCC is at least this version. 
 * 
 * @param maj major version
 * @param min minor version
 * @param patch patch version
 * 
 * @note This only has any meaning if the current compiler is GCC.
 * 
 * @since 0.110.0
 */
#if defined IC_GCC_VERSION
#define IC_GCC_VERSION_CHECK(maj, min, patch) \
    (IC_GCC_VERSION >= IC_VERSION_ENCODE(maj, min, patch))
#else
#define IC_GCC_VERSION_CHECK(maj, min, patch) (0)
#endif 

/**
 * @def IC_MSVC_VERSION
 * @brief Defined if we are using MSVC.
 * 
 * @since 0.110.0
 */
#if defined _MSC_FULL_VER && (_MSC_FULL_VER >= 140000000)
#define IC_MSVC_VERSION \
    IC_VERSION_ENCODE(_MSC_FULL_VER / 10000000, \
        (_MSC_FULL_VER % 10000000) / 100000, (_MSC_FULL_VER % 100000) / 100)
#elif defined(_MSC_FULL_VER)
#define IC_MSVC_VERSION \
    IC_VERSION_ENCODE(_MSC_FULL_VER / 1000000, \
        (_MSC_FULL_VER % 1000000) / 10000, (_MSC_FULL_VER % 10000) / 10)
#elif defined(_MSC_VER)
#define IC_MSVC_VERSION IC_VERSION_ENCODE(_MSC_VER / 100, _MSC_VER % 100, 0)
#endif

/**
 * @def IC_MSVC_VERSION_CHECK
 * @brief Check if the version of MSVC is at least this version. 
 * 
 * @param maj major version
 * @param min minor version
 * @param patch patch version
 * 
 * @note This only has any meaning if the compiler is MSVC.
 * 
 * @since 0.110.0
 */
#if !defined IC_MSVC_VERSION
#define IC_MSVC_VERSION_CHECK(maj, min, patch) (0)
#elif defined _MSC_VER && (_MSC_VER >= 1400)
#define IC_MSVC_VERSION_CHECK(maj, min, patch) \
    (_MSC_FULL_VER >= ((maj * 10000000) + (min * 100000) + (patch)))
#elif defined _MSC_VER && (_MSC_VER >= 1200)
#define IC_MSVC_VERSION_CHECK(maj, min, patch) \
    (_MSC_FULL_VER >= ((maj * 1000000) + (min * 10000) + (patch)))
#else
#define IC_MSVC_VERSION_CHECK(maj, min, patch) \
    (_MSC_VER >= ((maj * 100) + (min)))
#endif 

/*
 * Minimum required C standard is C11. Preferred if we can use some of the
 * newer features in C23.
 */
#if defined __STDC_VERSION__
/**
 * @def IC_PLATFORM_STD_C99
 * @brief Defined if the compiler supports the C99 standard.
 * @note Supported by MSVC 12.0
 * @since 0.186.0
 */
#if __STDC_VERSION__ >= 199901L
#define IC_PLATFORM_STD_C99
#endif

/**
 * @def IC_PLATFORM_STD_C11
 * @brief Defined if the compiler supports the C11 standard.
 * @note Supported by MSVC 19.28+ (Visual Studio 16.8)
 * @since 0.186.0
 */
#if __STDC_VERSION__ >= 201112L
#define IC_PLATFORM_STD_C11
#endif

/**
 * @def IC_PLATFORM_STD_C17
 * @brief Defined if the compiler supports the C17 standard.
 * @note Supported by MSVC 19.28+ (Visual Studio 16.8)
 * @since 0.186.0
 */
#if __STDC_VERSION__ >= 201710L
#define IC_PLATFORM_STD_C17
#endif

/**
 * @def IC_PLATFORM_STD_C2X
 * @brief Defined if the compiler supports the (upcoming) C23 standard.
 * @since 0.186.0
 */
#if __STDC_VERSION__ >= 202000L
#define IC_PLATFORM_STD_C2X
#endif
#endif

/**
 * MSVC 19.28 (Visual Studio 2019 16.8) is the minimum required version 
 * that actually supports C11/17. 
 *
 * MSVC does not always report the correct C standard with the 
 * `__STDC_VERSION__` macro. For example, on a few test systems, 
 * it reports as `199409L`, even though the compiler really supports C11/17.
 */
#ifdef IC_PLATFORM_MSVC
#if IC_MSVC_VERSION_CHECK(19, 28, 0)
#define IC_PLATFORM_STD_C17
#define IC_PLATFORM_STD_C11
#define IC_PLATFORM_STD_C99
#endif
#endif

#ifndef IC_PLATFORM_STD_C11
//#error "A compatible C11 compiler is required"
#endif

/**
 * @def IC_HAS_ATTRIBUTE
 * @brief Checks if the current compiler has a particular attribute. 
 *
 * This will be defined as `__has_attribute` if the compiler has support for it.
 * 
 * @param attr attribute to check for
 * 
 * @since 0.110.0
 */
#if defined __has_attribute
#define IC_HAS_ATTRIBUTE(attr)                          __has_attribute(attr)
#define IC_GNUC_HAS_ATTRIBUTE(attr, maj, min, patch)    __has_attribute(attr)
#define IC_GCC_HAS_ATTRIBUTE(attr, maj, min, patch)     __has_attribute(attr)
#else
#define IC_HAS_ATTRIBUTE(attr) (0)
#define IC_GNUC_HAS_ATTRIBUTE(attr, maj, min, patch) \
    IC_GNUC_VERSION_CHECK(maj, min, patch)
#define IC_GCC_HAS_ATTRIBUTE(attr, maj, min, patch) \
    IC_GCC_VERSION_CHECK(maj, min, patch)
#endif

/**
 * @def IC_HAS_BUILTIN
 * @brief Checks if the current compiler has a particular builtin. 
 *
 * This will be defined as `__has_builtin` if the compiler has support for it.
 * 
 * @param bt builtin option to check for
 * 
 * @since 0.110.0
 */
#if defined __has_builtin
#define IC_HAS_BUILTIN(bt)                          __has_builtin(bt)
#define IC_GNUC_HAS_BUILTIN(bt, maj, min, patch)    __has_builtin(bt)
#define IC_GCC_HAS_BUILTIN(bt, maj, min, patch)     __has_builtin(bt)
#else
#define IC_HAS_BUILTIN(bt) (0)
#define IC_GNUC_HAS_BUILTIN(bt, maj, min, patch) \
    IC_GNUC_VERSION_CHECK(maj, min, patch)
#define IC_GCC_HAS_BUILTIN(bt, maj, min, patch) \
    IC_GCC_VERSION_CHECK(maj, min, patch)
#endif

/**
 * @def IC_LIKELY
 * @brief Hints to the compiler that a condition is likely to be true, so it
 * can perform optimizations (if it has this feature).
 * 
 * @since 0.185.0
 */

/**
 * @def IC_UNLIKELY
 * @brief Hints to the compiler that a condition is likely to be false, so it
 * can perform optimizations (if it has this feature).
 * 
 * @since 0.185.0
 */
#if IC_GNUC_VERSION_CHECK(2, 0, 0) || IC_HAS_BUILTIN(__builtin_expect)
#define IC_LIKELY(x)    __builtin_expect(!!(x), 1)
#define IC_UNLIKELY(x)  __builtin_expect(!!(x), 0)
#else
#define IC_LIKELY(x)
#define IC_UNLIKELY(x)
#endif

/**
 * @def IC_HAS_FEATURE
 * @brief Checks if the current compiler has a particular feature. 
 *
 * This will be defined as `__has_feature` if the compiler has support for it.
 * 
 * @param feat feature to check for
 * 
 * @since 0.110.0
 */
#if defined __has_feature
#define IC_HAS_FEATURE(feat)                        __has_feature(feat)
#define IC_GNUC_HAS_FEATURE(feat, maj, min, patch)  __has_feature(feat)
#define IC_GCC_HAS_FEATURE(feat, maj, min, patch)   __has_feature(feat)
#else
#define IC_HAS_FEATURE(feat) (0)
#define IC_GNUC_HAS_FEATURE(feat, maj, min, patch) \
    IC_GNUC_VERSION_CHECK(maj, min, patch)
#define IC_GCC_HAS_FEATURE(feat, maj, min, patch) \
    IC_GCC_VERSION_CHECK(maj, min, patch)
#endif

/**
 * @def IC_HAS_EXTENSION
 * @brief Checks if the current compiler has a particular extension. 
 *
 * This will be defined as `__has_extension` if the compiler has support for it.
 * 
 * @param ext extension to check for
 * 
 * @since 0.110.0
 */
#if defined __has_extension
#define IC_HAS_EXTENSION(ext)                       __has_extension(ext)
#define IC_GNUC_HAS_EXTENSION(ext, maj, min, patch) __has_extension(ext)
#define IC_GCC_HAS_EXTENSION(ext, maj, min, patch)  __has_extension(ext)
#else
#define IC_HAS_EXTENSION(ext) (0)
#define IC_GNUC_HAS_EXTENSION(ext, maj, min, patch) \
    IC_GNUC_VERSION_CHECK(maj, min, patch)
#define IC_GCC_HAS_EXTENSION(ext, maj, min, patch) \
    IC_GCC_VERSION_CHECK(maj, min, patch)
#endif

/**
 * @def IC_HAS_DECLSPEC_ATTRIBUTE
 * @brief Checks if the current compiler has a particular attribute. 
 *
 * This will be defined as `__has_declspec_attribute` if the compiler 
 * has support for it.
 * 
 * @param attr declspec attribute to check for
 * 
 * @since 0.110.0
 */
#if defined __has_declspec_attribute
#define IC_HAS_DECLSPEC_ATTRIBUTE(attr) __has_declspec_attribute(attr)
#define IC_GNUC_HAS_DECLSPEC_ATTRIBUTE(attr, maj, min, patch) __has_declspec_attribute(attr)
#define IC_GCC_HAS_DECLSPEC_ATTRIBUTE(attr, maj, min, patch) __has_declspec_attribute(attr)
#else
#define IC_HAS_DECLSPEC_ATTRIBUTE(attr) (0)
#define IC_GNUC_HAS_DECLSPEC_ATTRIBUTE(attr, maj, min, patch) \
    IC_GNUC_VERSION_CHECK(maj, min, patch)
#define IC_GCC_HAS_DECLSPEC_ATTRIBUTE(attr, maj, min, patch) \
    IC_GCC_VERSION_CHECK(maj, min, patch)
#endif

/**
 * @def IC_HAS_WARNING
 * @brief Checks if the current compiler has a particular warning. 
 *
 * This will be defined as `__has_warning` if the compiler has support for it.
 * 
 * @param warn warning to check for
 * 
 * @since 0.110.0
 */
#if defined __has_warning
#define IC_HAS_WARNING(warn)                            __has_warning(warn)
#define IC_GNUC_HAS_WARNING(warn, maj, min, patch)      __has_warning(warn)
#define IC_GCC_HAS_WARNING(warn, maj, min, patch)       __has_warning(warn)
#else
#define IC_HAS_WARNING(warn) (0)
#define IC_GNUC_HAS_WARNING(warn, maj, min, patch) \
    IC_GNUC_VERSION_CHECK(maj, min, patch)
#define IC_GCC_HAS_WARNING(warn, maj, min, patch) \
    IC_GCC_VERSION_CHECK(maj, min, patch)
#endif

/**
 * @def IC_PRAGMA
 * @brief Wrapper for the various methods of using pragmas.
 * 
 * @param val pragma value
 *
 * @note On all C99 platforms, this is defined as `_Pragma(#val)`.
 * @note On non-C99 MSVC, this is defined as `__pragma(val)`.
 * 
 * @since 0.110.0
 */
#if defined IC_PLATFORM_STD_C99 || defined __clang__ || \
    IC_GCC_VERSION_CHECK(3, 0, 0)
#define IC_PRAGMA(val) _Pragma(#val)
#elif IC_MSVC_VERSION_CHECK(15, 0, 0)
#define IC_PRAGMA(val) __pragma(val)
#else
#define IC_PRAGMA(val)
#endif

/**
 * @def IC_DIAGNOSTIC_PUSH
 * @brief Allows pushing a new diagnostic pragma to the diagnostic stack.
 * 
 * @since 0.110.0
 */

/**
 * @def IC_DIAGNOSTIC_POP
 * @brief Allows popping of the diagnostic pragma from the stack.
 * 
 * @since 0.110.0
 */
#if defined IC_PLATFORM_CLANG
#define IC_DIAGNOSTIC_PUSH  IC_PRAGMA(clang diagnostic push)
#define IC_DIAGNOSTIC_POP   IC_PRAGMA(clang diagnostic pop)
#elif IC_GCC_VERSION_CHECK(4, 6, 0)
#define IC_DIAGNOSTIC_PUSH  IC_PRAGMA(GCC diagnostic push)
#define IC_DIAGNOSTIC_POP   IC_PRAGMA(GCC diagnostic pop)
#elif IC_MSVC_VERSION_CHECK(15, 0, 0)
#define IC_DIAGNOSTIC_PUSH  IC_PRAGMA(warning(push))
#define IC_DIAGNOSTIC_POP   IC_PRAGMA(warning(pop))
#else
#define IC_DIAGNOSTIC_PUSH
#define IC_DIAGNOSTIC_POP
#endif

/**
 * @def IC_SILENCE_WARNING
 * @brief Silences the given warning. GCC/Clang can use the same syntax,
 * but you should do an `#if` check for MSVC as warnings are numerical and not
 * flags.
 * 
 * Example:
 * @code
 * IC_DIAGNOSTIC_PUSH
 * 
 * #ifdef IC_PLATFORM_MSVC
 * IC_SILENCE_WARNING(4505)
 * #elif defined IC_PLATFORM_GCC || defined IC_PLATFORM_CLANG
 * IC_SILENCE_WARNING(-Wunused-function)
 * #endif
 * 
 * IC_DIAGNOSTIC_POP
 * @endcode
 * 
 * @note Must be called between an @ref IC_DIAGNOSTIC_PUSH and 
 * @ref IC_DIAGNOSTIC_POP.
 * 
 * @since 0.169.0
 */
#if defined __clang__
#define IC_SILENCE_WARNING_EX(w) IC_PRAGMA(clang diagnostic ignored w)
#define IC_SILENCE_WARNING(w) IC_SILENCE_WARNING_EX(#w)
#elif IC_GCC_VERSION_CHECK(4, 6, 0)
#define IC_SILENCE_WARNING_EX(w) IC_PRAGMA(GCC diagnostic ignored w)
#define IC_SILENCE_WARNING(w) IC_SILENCE_WARNING_EX(#w)
#elif IC_MSVC_VERSION_CHECK(15, 0, 0)
#define IC_SILENCE_WARNING_EX(w) IC_PRAGMA(warning(disable:w))
#define IC_SILENCE_WARNING(w) IC_SILENCE_WARNING_EX(w)
#else
#define IC_SILENCE_WARNING(w)
#endif

/**
 * @def IC_DIAGNOSTIC_DISABLE_UNUSED_FUNCTION
 * @brief Will silence "unused function" warnings when used on a given function.
 * 
 * For example:
 * @code
 * IC_DIAGNOSTIC_DISABLE_UNUSED_FUNCTION
 * void old_function(void);
 * @endcode
 * 
 * @since 0.110.0
 */
#if IC_HAS_WARNING("-Wunused-function")
#define IC_DIAGNOSTIC_DISABLE_UNUSED_FUNCTION \
    IC_SILENCE_WARNING(-Wunused-function)
#elif IC_GCC_VERSION_CHECK(3, 4, 0)
#define IC_DIAGNOSTIC_DISABLE_UNUSED_FUNCTION \
    IC_SILENCE_WARNING(-Wunused-function)
#elif IC_MSVC_VERSION_CHECK(1, 0, 0)
#define IC_DIAGNOSTIC_DISABLE_UNUSED_FUNCTION \
    IC_SILENCE_WARNING(4505)
#else
#define IC_DIAGNOSTIC_DISABLE_UNUSED_FUNCTION
#endif

/**
 * @def IC_DEPRECATED
 * @brief This attribute tells the compiler that a particular function has 
 * become deprecated and will cause compatible compilers to generate warnings.
 * 
 * @param since the version when this function first became deprecated
 * 
 * Example:
 * @code
 * IC_DEPRECATED("1.7")
 * void old_function(char *msg);
 * @endcode
 * 
 * @note On GNUC this is defined as `__attribute__((__deprecated__))`.
 * @note On MSVC 13.10+ this is defined as `__declspec(deprecated)`.
 * 
 * @since 0.110.0
 */

/**
 * @def IC_DEPRECATED_FOR
 * @brief This attribute is the same as `IC_DEPRECATED`, but allows you 
 * to specify a replacement function.
 * 
 * @param since the version when this function first became deprecated
 * @param replacement the function that should be used instead
 * 
 * Example:
 * @code
 * IC_DEPRECATED_FOR("1.7", "new_function")
 * void old_function(char *msg);
 * @endcode
 * 
 * @note On GNUC this is defined as `__attribute__((__deprecated__))`.
 * @note On MSVC 13.10+ this is defined as `__declspec(deprecated)`.
 * 
 * @since 0.110.0
 */
#if IC_MSVC_VERSION_CHECK(14, 0, 0)
#define IC_DEPRECATED(since) \
    __declspec(deprecated("Since " # since))
#define IC_DEPRECATED_FOR(since, replacement) \
    __declspec(deprecated("Since " # since "; use " # replacement))
#elif IC_HAS_EXTENSION(attribute_deprecated_with_message) || \
    IC_GCC_VERSION_CHECK(4, 5, 0)
#define IC_DEPRECATED(since) __attribute__((__deprecated__("Since " # since)))
#define IC_DEPRECATED_FOR(since, replacement) \
    __attribute__((__deprecated__("Since " # since "; use " # replacement)))
#elif IC_HAS_ATTRIBUTE(deprecated) || IC_GCC_VERSION_CHECK(3, 1, 0)
#define IC_DEPRECATED(since) __attribute__((__deprecated__))
#define IC_DEPRECATED_FOR(since, replacement) \
    __attribute__((__deprecated__))
#elif IC_MSVC_VERSION_CHECK(13, 10, 0)
#define IC_DEPRECATED(since) \
    __declspec(deprecated)
#define IC_DEPRECATED_FOR(since, replacement) \
    __declspec(deprecated)
#else
#define IC_DEPRECATED
#define IC_DEPRECATED_FOR
#endif

/**
 * @def IC_NO_RETURN
 * @brief This attribute tells compatible compilers that the given function will 
 * never return (i.e. _must_ exit) and allows them to optimise based on that.
 * 
 * @note On GNUC this is defined as `__attribute__((__noreturn__))`.
 * @note On MSVC 13.10+ this is defined as `_declspec(noreturn)`.
 * 
 * @since 0.110.0
 */
#if IC_MSVC_VERSION_CHECK(13, 10, 0)
#define IC_NO_RETURN __declspec(noreturn)
#elif defined IC_PLATFORM_STD_C11
#define IC_NO_RETURN _Noreturn
#elif IC_HAS_ATTRIBUTE(noreturn) || IC_GCC_VERSION_CHECK(3, 2, 0)
#define IC_NO_RETURN __attribute__((__noreturn__))
#else
#define IC_NO_RETURN
#endif

/**
 * @def IC_UNREACHABLE
 * @brief This attribute tells compilers that the code following will never 
 * be reached, allowing them to generate optimised code (or simply 
 * ignore warnings).
 * 
 * @since 0.110.0
 */
#define IC_STATIC_CAST(T, expr) ((T) expr)

#if IC_MSVC_VERSION_CHECK(13, 10, 0)
#define IC_ASSUME(expr) __assume(expr)
#elif IC_HAS_BUILTIN(__builtin_assume)
#define IC_ASSUME(expr) __builtin_assume(expr)
#endif

#if IC_HAS_BUILTIN(__builtin_unreachable) || IC_GCC_VERSION_CHECK(4, 5, 0)
#define IC_UNREACHABLE() __builtin_unreachable()
#elif defined IC_ASSUME
#define IC_UNREACHABLE() IC_ASSUME(0)
#endif

#if !defined IC_ASSUME
#if defined IC_UNREACHABLE
#define IC_ASSUME(expr) IC_STATIC_CAST(void, ((expr) ? 1 : (IC_UNREACHABLE(), 1)))
#else
#define IC_ASSUME(expr) IC_STATIC_CAST(void, expr)
#endif
#endif

#if defined IC_UNREACHABLE
#define IC_UNREACHABLE_RETURN(val) IC_UNREACHABLE()
#else
#define IC_UNREACHABLE_RETURN(VAL) return (value)
#define IC_UNREACHABLE() IC_ASSUME(0)
#endif

/**
 * @def IC_NON_NULL
 * @brief This attribute tells compilers that certain parameters should 
 * never be NULL, and will generate compile warnings if that is the case.
 * 
 * Example usage:
 * @code
 * IC_NON_NULL(1, 3)
 * void do_something(char *str, size_t size, char *out);
 * @endcode
 * 
 * @note This only has effect on GNUC compilers.
 * 
 * @since 0.110.0
 */
IC_DIAGNOSTIC_PUSH
#if IC_HAS_WARNING("-Wpedantic")
IC_SILENCE_WARNING(-Wpedantic)
#endif

#if IC_GCC_HAS_WARNING("-Wvariadic-macros", 4, 0, 0)
IC_SILENCE_WARNING(-Wvariadic-macros)
#endif

#if IC_HAS_ATTRIBUTE(nonnull) || IC_GCC_VERSION_CHECK(3, 3, 0)
#define IC_NON_NULL(...) __attribute__((__nonnull__(__VA_ARGS__)))
#else
#define IC_NON_NULL(...) 
#define IC_NON_NULL_RET 
#endif
IC_DIAGNOSTIC_POP

/**
 * @def IC_MALLOC
 * @brief The malloc attribute is used to tell the compiler that a function may 
 * be treated as if any non-NULL pointer it returns cannot alias any other 
 * pointer valid when the function returns and that the memory has undefined 
 * content.
 * 
 * @note On GNUC, this is defined as `__attribute__((__malloc__))`.
 * @note On MSVC 14.0+, this is defined as `__declspec(restrict)`.
 * 
 * @since 0.110.0
 */
#if IC_HAS_ATTRIBUTE(malloc) || IC_GCC_VERSION_CHECK(3, 1, 0)
#define IC_MALLOC __attribute__((__malloc__))
#elif IC_MSVC_VERSION_CHECK(14, 0, 0)
#define IC_MALLOC __declspec(restrict)
#else
#define IC_MALLOC
#endif

/**
 * @def IC_INLINE
 * 
 * @note On C99+ platforms this is defined simply as the builtin `inline`.
 * @note On GCC platforms (non-C99) this is defined as `__inline__`.
 * @note On MSVC 12.0 and lower this is defined as `__inline`.
 * 
 * @since 0.110.0
 */
#if defined IC_PLATFORM_STD_C99
#define IC_INLINE inline
#elif defined IC_GCC_VERSION
#define IC_INLINE __inline__
#elif IC_MSVC_VERSION_CHECK(12, 0, 0)
#define IC_INLINE __inline
#else
#define IC_INLINE
#endif

/**
 * @def IC_NO_THROW
 * @brief The nothrow attribute is used to tell the compiler that this 
 * function should never throw an exception.
 * 
 * @note On GNUC this is defined as `__attribute__((__nothrow__))`.
 * @note On MSVC 13.1+ this is defined as `__declspec(nothrow)`.
 * 
 * @since 0.110.0
 */
#if IC_HAS_ATTRIBUTE(nothrow) || IC_GCC_VERSION_CHECK(3, 3, 0)
#define IC_NO_THROW __attribute__((__nothrow__))
#elif IC_MSVC_VERSION_CHECK(13, 1, 0)
#define IC_NO_THROW __declspec(nothrow)
#else
#define IC_NO_THROW
#endif

/**
 * @def IC_FALLTHROUGH
 * @brief This attribute tells the compiler that we explictly intend to have
 * a fall through switch case.
 * 
 * @note On GNUC this is defined as `__attribute__((__fallthrough__))`.
 * @note On MSVC this is defined as `__fallthrough`.
 * 
 * @since 0.110.0
 */
#if IC_HAS_ATTRIBUTE(fallthrough) || IC_GCC_VERSION_CHECK(7, 0, 0)
#define IC_FALLTHROUGH __attribute__((__fallthrough__))
#elif defined __fallthrough
#define IC_FALLTHROUGH __fallthrough
#else
#define IC_FALLTHROUGH do {} while (0)
#endif
#define fallthrough IC_FALLTHROUGH

/**
 * @def IC_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS
 * @brief Disable unknown pragma warnings.
 * 
 * @since 0.110.0
 */
#if IC_HAS_WARNING("-Wunknown-pragmas") || IC_GCC_VERSION_CHECK(4, 3, 0)
IC_SILENCE_WARNING(-Wunknown-pragmas)
#elif IC_MSVC_VERSION_CHECK(15, 0, 0)
IC_SILENCE_WARNING(4068)
#else
#define IC_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS
#endif

/**
 * @def IC_MESSAGE
 * @brief Push a message to the compiler at compile time.
 * 
 * @param msg message
 *
 * @note This is only supported on GNUC compilers.
 * 
 * @since 0.110.0
 */

/**
 * @def IC_WARNING
 * @brief Push a warning message to the compiler at compile time.
 * 
 * @param msg warning message
 *
 * @note On GCC 4.8+ this is defined as `IC_PRAGMA(GCC warning msg)`.
 * @note On MSVC 15+ this is defined as `IC_PRAGMA(message msg)`.
 * 
 * @since 0.110.0
 */
#if IC_HAS_WARNING("-Wunknown-pragmas")
#define IC_MESSAGE(msg) \
    IC_DIAGNOSTIC_PUSH \
    IC_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS \
    IC_PRAGMA(message msg) \
    IC_DIAGNOSTIC_POP
#define IC_WARNING(msg) \
    IC_DIAGNOSTIC_PUSH \
    IC_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS \
    IC_PRAGMA(clang warning msg) \
    IC_DIAGNOSTIC_POP
#elif defined IC_GCC_VERSION
#if IC_GCC_VERSION_CHECK(4, 4, 0)
#define IC_MESSAGE(msg) IC_PRAGMA(message msg)
#else
#define IC_MESSAGE(msg)
#endif

#if IC_GCC_VERSION_CHECK(4, 8, 0)
#define IC_WARNING(msg) IC_PRAGMA(GCC warning msg)
#else
#define IC_WARNING(msg) IC_MESSAGE(msg)
#endif
#elif IC_MSVC_VERSION_CHECK(15, 0, 0)
#define IC_MESSAGE(msg)
#define IC_WARNING(msg) IC_PRAGMA(message(msg))
#else
#define IC_MESSAGE(msg)
#define IC_WARNING(msg) IC_MESSAGE(msg)
#endif

/**
 * @def IC_UNUSED
 * @brief Mark a particular function as unused to avoid compiler warnings.
 * 
 * @note This only has effect on GNUC compilers.
 * 
 * @since 0.110.0
 */
#if IC_HAS_ATTRIBUTE(unused) || IC_GCC_VERSION_CHECK(4, 7, 2)
#define IC_UNUSED __attribute__((unused))
#else
#define IC_UNUSED
#endif

/**
 * @def IC_PURE
 * @brief Tell GCC that this function is pure, i.e. it has no side effects.
 */
/**
 * @def IC_CONST
 * @brief Tell GCC that this function is pure, i.e. it has no side effects.
 * 
 * Const functions cannot examine pointer data.
 */
#ifdef IC_PLATFORM_GCC
#define IC_PURE __attribute__((pure))
#define IC_CONST __attribute__((const))
#else
#define IC_PURE
#define IC_CONST
#endif

/**
 * @def IC_PLATFORM_ARCH
 * @brief A string containing the name of the architecture being compiled on. 
 *
 * Valid options are `x86`, `amd64`, `arm32`, and `arm64`.
 * 
 * @since 0.110.0
 */
#ifndef IC_PLATFORM_ARCH
#if defined __i386__ || defined _M_IX86
#define IC_PLATFORM_ARCH "x86"
#elif defined __x86_64__ || defined _M_AMD64
#define IC_PLATFORM_ARCH "amd64"
#elif defined __arm__
#if defined __arm64__ || defined __aarch64__
#define IC_PLATFORM_ARCH "arm64"
#else
#define IC_PLATFORM_ARCH "arm32"
#endif /* __arm64__ || defined __aarch64__ */
#endif /* __i386__ */
#endif /* IC_PLATFORM_ARCH */

/**
 * @def IC_PLATFORM_OS
 * @brief String containing the name of the operating system we were 
 * compiled on.
 * 
 * Valid options are `windows`, `linux`, `macos`, or `unsupported`.
 * 
 * @since 0.110.0
 */

/**
 * @def IC_PLATFORM_DLL
 * @brief String containing the extension of the shared library objects 
 * on the system.
 * 
 * Valid options are `dll` for Windows, `so` for Linux, and `dylib` for MacOS.
 * 
 * @since 0.110.0
 */

/**
 * @def IC_PLATFORM_EXE
 * @brief String containing the extension of the executables on the system.
 *
 * Valid options are `exe` for Windows and blank for every othersystem.
 * 
 * @since 0.110.0
 */

/**
 * @def PATH_SEP
 * @brief The character of the path separator on the system. 
 *
 * For Windows this is `\\`, other systems are `/`.
 * 
 * @since 0.110.0
 */

/**
 * @def IC_PLATFORM_WINDOWS
 * @brief Defined if we are on a Windows system.
 * @since 0.110.0
 */

/**
 * @def IC_PLATFORM_LINUX
 * @brief Defined if we are on a Linux system.
 * @since 0.110.0
 */

/**
 * @def IC_PLATFORM_MACOS
 * @brief Defined if we are compiling on a MacOS system.
 * @since 0.110.0
 */

/**
 * @def IC_PRIVATE
 * @brief This attribute can be used for private functions which should not be
 * visible. 
 
 * @note On GNUC this is defined as `__attribute__((visibility("hidden")))`. 
 * @note On MSVC this has no effect.
 * @since 0.110.0
 */

/**
 * @def IC_PUBLIC
 * @brief This attribute can be used to declare functions for library exports.
 * 
 * @note On GNUC this is defined as `__attribute__((visibility("default")))`.
 * @note On MSVC this is defined as `__declspec(dllexport)`.
 * @since 0.110.0
 */

/**
 * @def IC_IMPORT
 * @brief This attribute can be used to declare library import functions.
 * 
 * @note On GNUC this has no effect.
 * @note On MSVC this is defined as `__declspec(dllimport)`.
 * @since 0.110.0
 */

/**
 * @def IC_PLATFORM_NEWLINE
 * @brief Newline character string for the given OS. 
 
 * @note On Windows, this is `"\r\n"` for backwards compatibility with older 
 * Windows versions.
 * @note On MacOS / Linux this is just `"\n"`
 * @since 0.169.0
 */
#if defined IC_PLATFORM_WINDOWS

#ifndef IC_PLATFORM_OS
#define IC_PLATFORM_OS      "windows"
#endif

#ifndef IC_PLATFORM_DLL
#define IC_PLATFORM_DLL     "dll"
#endif

#define PATH_SEP    '\\'

#define IC_PRIVATE  
#define IC_PUBLIC   __declspec(dllexport)
#define IC_IMPORT   __declspec(dllimport)

#define IC_PLATFORM_NEWLINE "\r\n"

#ifndef __BASE_FILE__
#define __BASE_FILE__ __FILE__
#endif
#elif defined IC_PLATFORM_LINUX || defined IC_PLATFORM_MACOS
#define PATH_SEP    '/'

#if IC_HAS_ATTRIBUTE(visibility) || \
    IC_GCC_VERSION_CHECK(3, 3, 0)
#define IC_PRIVATE __attribute__((visibility("hidden")))
#define IC_PUBLIC  __attribute__((visibility("default")))
#else
#define IC_PRIVATE
#define IC_PUBLIC
#endif

#define IC_PLATFORM_NEWLINE "\n"
#define IC_IMPORT extern

#if defined __linux__ 
#ifndef IC_PLATFORM_OS
#define IC_PLATFORM_OS      "linux"
#endif

#ifndef IC_PLATFORM_DLL
#define IC_PLATFORM_DLL     "so"
#endif
#elif defined __APPLE__
#ifndef IC_PLATFORM_OS
#define IC_PLATFORM_OS      "macos"
#endif

#ifndef IC_PLATFORM_DLL
#define IC_PLATFORM_DLL     "dylib"
#endif
#endif
#else /* other systems */
#define PATH_SEP '/'
#define IC_PRIVATE
#define IC_PUBLIC
#define IC_IMPORT
#define IC_PLATFORM_NEWLINE "\n"
#define IC_PLATFORM_EXE ""
#define IC_PLATFORM_DLL ""

#if defined __DOXYGEN__
#define IC_PLATFORM_WINDOWS
#define IC_PLATFORM_LINUX
#define IC_PLATFORM_MACOS

#ifdef IC_PLATFORM_ARCH
#undef IC_PLATFORM_ARCH
#endif
#define IC_PLATFORM_ARCH "doxygen"

#ifdef IC_PLATFORM_OS
#undef IC_PLATFORM_OS
#endif
#define IC_PLATFORM_OS "doxygen"

#ifdef IC_PLATFORM_EXE
#undef IC_PLATFORM_EXE
#endif
#define IC_PLATFORM_EXE ".doxygen"

#ifdef IC_PLATFORM_DLL
#undef IC_PLATFORM_DLL
#endif
#define IC_PLATFORM_DLL ".doxygen"

#define IC_GNUC_VERSION 0
#define IC_GCC_VERSION  0
#define IC_MSVC_VERSION 0

#define IC_PLATFORM_LITTLE_ENDIAN   // doxygen is little endian obviously
#define IC_PLATFORM_BIG_ENDIAN      // doxygen is big endian obviously

#else
#ifndef IC_PLATFORM_ARCH
#define IC_PLATFORM_ARCH "unsupported"
#endif
#ifndef IC_PLATFORM_OS
#define IC_PLATFORM_OS "unsupported"
#endif
#endif
#endif

/**
 * @def IC_PLATFORM_STRING
 * @brief A string containing the platform information and optionally 
 * a debug flag.
 * @since 0.110.0
 */
#ifdef IC_DEBUG
#define IC_PLATFORM_STRING IC_PLATFORM_OS "-" IC_PLATFORM_ARCH "-debug"
#else
#define IC_PLATFORM_STRING IC_PLATFORM_OS "-" IC_PLATFORM_ARCH
#endif

/** @} */

#endif /* IC_PLATFORM_H */
