/*
================================================================================
iocod
Copyright (C) 2021-2022 thecheeseman

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
 * @defgroup platform Platform Macros
 * @brief Macros for platform and compiler detection, features, attributes, etc.
 * @{
 */

#ifndef IC_PLATFORM_H
#define IC_PLATFORM_H

#if defined __STDC_VERSION__
/**
 * @def IC_PLATFORM_STD_C89
 * @brief This is defined if the compiler supports ANSI C.
 */
#define IC_PLATFORM_STD_C89

/**
 * @def IC_PLATFORM_STD_95
 * @brief Defined if the compiler supports `__STDC_VERSION__` as first 
 * introduced in C95.
 */
#define IC_PLATFORM_STD_C95

/**
 * @def IC_PLATFORM_C99
 * @brief Defined if the compiler supports C99 mode.
 * 
 * @note First supported in GCC ~3.0, MSVC 12.0
 */
#if __STDC_VERSION__ >= 199901L
#define IC_PLATFORM_STD_C99
#endif

/**
 * @def IC_PLATFORM_C11
 * @brief Defined if the compiler supports C11 mode.
 *
 * @note First supported in GCC 4.6, Clang 3.1, MSVC 16.8
 */
#if __STDC_VERSION__ >= 201112L
#define IC_PLATFORM_STD_C11
#endif

/**
 * @def IC_PLATFORM_STD_C17
 * @brief Defined if the compiler supports C17 mode.
 * 
 * @note First supported in GCC 8.1, Clang 7.0, MSVC 16.8
 */
#if __STDC_VERSION__ >= 201710L
#define IC_PLATFORM_STD_C17
#endif

/**
 * @def IC_PLATFORM_STD_C2X
 * @brief Defined if the compiler supports future versions of the C standard.
 *
 * @note This is currently only supported by the latest GCC versions.
 */
#if __STDC_VERSION__ >= 202000L
#define IC_PLATFORM_STD_C2X
#endif

#elif defined __STDC__
#define IC_PLATFORM_STD_C89
#endif

/**
 * @def IC_GNUC_VERSION
 * @brief Defined if we are using a GNUC compatible compiler. 
 *
 * @note This does not necessarily mean GCC, as some compilers 
 * masquerade as GNUC (e.g. clang).
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

/**
 * @def IC_HAS_ATTRIBUTE
 * @brief Checks if the current compiler has a particular attribute. 
 *
 * This will be defined as `__has_attribute` if the compiler has support for it.
 * 
 * @param attr attribute to check for
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
 * @def IC_HAS_FEATURE
 * @brief Checks if the current compiler has a particular feature. 
 *
 * This will be defined as `__has_feature` if the compiler has support for it.
 * 
 * @param feat feature to check for
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
 */

/**
 * @def IC_DIAGNOSTIC_POP
 * @brief Allows popping of the diagnostic pragma from the stack.
 */
#if defined __clang__
#define IC_DIAGNOSTIC_PUSH  _Pragma("clang diagnostic push")
#define IC_DIAGNOSTIC_POP   _Pragma("clang diagnostic pop")
#elif IC_GCC_VERSION_CHECK(4, 6, 0)
#define IC_DIAGNOSTIC_PUSH  _Pragma("GCC diagnostic push")
#define IC_DIAGNOSTIC_POP   _Pragma("GCC diagnostic pop")
#elif IC_MSVC_VERSION_CHECK(15, 0, 0)
#define IC_DIAGNOSTIC_PUSH  __pragma(warning(push))
#define IC_DIAGNOSTIC_POP   __pragma(warning(pop))
#else
#define IC_DIAGNOSTIC_PUSH
#define IC_DIAGNOSTIC_POP
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
 */
#if IC_HAS_WARNING("-Wunused-function")
#define IC_DIAGNOSTIC_DISABLE_UNUSED_FUNCTION \
    _Pragma("clang diagnostic ignored \"-Wunused-function\"")
#elif IC_GCC_VERSION_CHECK(3, 4, 0)
#define IC_DIAGNOSTIC_DISABLE_UNUSED_FUNCTION \
    _Pragma("GCC diagnostic ignored \"-Wunused-function\"")
#elif IC_MSVC_VERSION_CHECK(1, 0, 0)
#define IC_DIAGNOSTIC_DISABLE_UNUSED_FUNCTION \
    __pragma(warning(disable:4505))
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
 * @def IC_WARN_UNUSED_RESULT
 * @brief This attribute allows compatible compilers to generate a warning 
 * if the returned result of the function is ignored.
 * 
 * @note On GNUC this is defined as `__attribute__((__warn_unused_result__))`.
 * @note On MSVC this is defined as the SAL annotation `_Check_return_`.
 */
#if IC_HAS_ATTRIBUTE(warn_unused_result) || IC_GCC_VERSION_CHECK(3, 4, 0)
#define IC_WARN_UNUSED_RESULT __attribute__((__warn_unused_result__))
#elif defined _Check_return_
#define IC_WARN_UNUSED_RESULT _Check_return_
#else
#define IC_WARN_UNUSED_RESULT
#endif

/**
 * @def IC_NO_RETURN
 * @brief This attribute tells compatible compilers that the given function will 
 * never return (i.e. _must_ exit) and allows them to optimise based on that.
 * 
 * @note On GNUC this is defined as `__attribute__((__noreturn__))`.
 * @note On MSVC 13.10+ this is defined as `_declspec(noreturn)`.
 */
#if defined IC_PLATFORM_STD_C11
#define IC_NO_RETURN _Noreturn
#elif IC_HAS_ATTRIBUTE(noreturn) || IC_GCC_VERSION_CHECK(3, 2, 0)
#define IC_NO_RETURN __attribute__((__noreturn__))
#elif IC_MSVC_VERSION_CHECK(13, 10, 0)
#define IC_NO_RETURN __declspec(noreturn)
#else
#define IC_NO_RETURN
#endif

/**
 * @def IC_UNREACHABLE
 * @brief This attribute tells compilers that the code following will never 
 * be reached, allowing them to generate optimised code (or simply 
 * ignore warnings).
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
 */
IC_DIAGNOSTIC_PUSH
#if IC_HAS_WARNING("-Wpedantic")
#pragma clang diagnostic ignored "-Wpedantic"
#endif

#if IC_GCC_HAS_WARNING("-Wvariadic-macros", 4, 0, 0)
#if defined __clang__
#pragma clang diagnostic ignored "-Wvariadic-macros"
#elif defined IC_GCC_VERSION
#pragma GCC diagnostic ignored "-Wvariadic-macros"
#endif
#endif

#if IC_HAS_ATTRIBUTE(nonnull) || IC_GCC_VERSION_CHECK(3, 3, 0)
#define IC_NON_NULL(...) __attribute__((__nonnull__(__VA_ARGS__)))
#else
#define IC_NON_NULL(...) 
#define IC_NON_NULL_RET 
#endif
IC_DIAGNOSTIC_POP

/**
 * @def IC_PRINTF_FORMAT
 * @brief This attribute tells the compiler to check the arguments for 
 consistency with `printf`-style formatted strings. 
 * 
 * @param idx starting index of the format specifier
 * @param first starting index of the varargs
 * 
 * For example:
 * @code
 *  IC_PRINTF_FORMAT(2, 3)
 *  void print_error(int code, const char *fmt, ...);
 * @endcode
 * 
 * @note This feature is only available on compatible GNUC compilers, where it
 * is defined as `__attribute__((format(printf, idx, first)))`.
 */
#if defined __MINGW32__ && IC_GCC_HAS_ATTRIBUTE(format, 4, 4, 0) && \
    !defined __USE_MINGW_ASNI_STDIO
#define IC_PRINTF_FORMAT(idx, first) __attribute__((format(ms_printf, idx, first)))
#elif defined __MINGW32__ && IC_GCC_HAS_ATTRIBUTE(format, 4, 4, 0) && \
    defined __USE_MINGW_ASNI_STDIO
#define IC_PRINTF_FORMAT(idx, first) __attribute__((format(gnu_printf, idx, first)))
#elif IC_HAS_ATTRIBUTE(format) || IC_GCC_VERSION_CHECK(3, 1, 0)
#define IC_PRINTF_FORMAT(idx, first) __attribute__((format(printf, idx, first)))
#else
#define IC_PRINTF_FORMAT(idx, first)
#endif

/**
 * @def IC_MALLOC
 * @brief The malloc attribute is used to tell the compiler that a function may 
 * be treated as if any non-NULL pointer it returns cannot alias any other 
 * pointer valid when the function returns and that the memory has undefined 
 * content.
 * 
 * @note On GNUC, this is defined as `__attribute__((__malloc__))`.
 * @note On MSVC 14.0+, this is defined as `__declspec(restrict)`.
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
 */
#if IC_HAS_ATTRIBUTE(nothrow) || IC_GCC_VERSION_CHECK(3, 3, 0)
#define IC_NO_THROW __attribute__((__nothrow__))
#elif IC_MSVC_VERSION_CHECK(13, 1, 0)
#define IC_NO_THROW __declspec(nothrow)
#else
#define IC_NO_THROW
#endif

/**
 * @def IC_FALL_THROUGH
 * @brief This attribute tells the compiler that we explictly intend to have
 * a fall through switch case.
 * 
 * @note On GNUC this is defined as `__attribute__((__fallthrough__))`.
 * @note On MSVC this is defined as `__fallthrough`.
 */
#if IC_HAS_ATTRIBUTE(fallthrough) || IC_GCC_VERSION_CHECK(7, 0, 0)
#define IC_FALL_THROUGH __attribute__((__fallthrough__))
#elif defined __fallthrough
#define IC_FALL_THROUGH __fallthrough
#else
#define IC_FALL_THROUGH
#endif

/**
 * @def IC_RETURNS_NON_NULL
 * @brief This attribute allows compilers to better optimise functions knowing
 * that they will never return NULL.
 * 
 * @note On GNUC this is defined as `__attribute__((__returns_nonnull__)`.
 * @note On MSVC this is defined with the SAL annotation `_Ret_notnull_`.
 */
#if IC_HAS_ATTRIBUTE(returns_nonnull) || IC_GCC_VERSION_CHECK(4, 9, 0)  
#define IC_RETURNS_NON_NULL __attribute__((__returns_nonnull__))
#elif defined _Ret_notnull_
#define IC_RETURNS_NON_NULL _Ret_notnull_
#else
#define IC_RETURNS_NON_NULL
#endif

/**
 * @def IC_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS
 * @brief Disable unknown pragma warnings.
 */
#if IC_HAS_WARNING("-Wunknown-pragmas")
#define IC_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS _Pragma("clang diagnostic ignored \"-Wunknown-pragmas\"")
#elif IC_GCC_VERSION_CHECK(4, 3, 0)
#define IC_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS _Pragma("GCC diagnostic ignored \"-Wunknown-pragmas\"")
#elif IC_MSVC_VERSION_CHECK(15, 0, 0)
#define IC_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS __pragma(warning(disable:4068))
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
 */

/**
 * @def IC_WARNING
 * @brief Push a warning message to the compiler at compile time.
 * 
 * @param msg warning message
 *
 * @note On GCC 4.8+ this is defined as `IC_PRAGMA(GCC warning msg)`.
 * @note On MSVC 15+ this is defined as `IC_PRAGMA(message msg)`.
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
 */
#if IC_HAS_ATTRIBUTE(unused) || IC_GCC_VERSION_CHECK(4, 7, 2)
#define IC_UNUSED __attribute__((unused))
#else
#define IC_UNUSED
#endif

/**
 * @def IC_PLATFORM_COMPILER
 * @brief A string containing the name of the compiler used. Valid options are
 * `msvc`, `mingw`, `gcc`, `clang` or `unknown`.
 */

/**
 * @def IC_PLATFORM_MSVC
 * @brief Defined if we are compiling with MSVC.
 */

/**
 * @def IC_PLATFORM_MINGW
 * @brief Defined if we are compiling with MinGW.
 */

/**
 * @def IC_PLATFORM_GCC
 * @brief Defined if we are compiling with GCC.
 */

/**
 * @def IC_PLATFORM_CLANG
 * @brief Defined if we are compiling with Clang.
 */
#ifndef IC_PLATFORM_COMPILER

/* if these aren't already defined by CMake, we'll check ourselves */
#if !defined IC_PLATFORM_MSVC && !defined IC_PLATFORM_GCC && !defined IC_PLATFORM_CLANG
#if defined _MSC_VER 
#define IC_PLATFORM_MSVC
#define IC_PLATFORM_COMPILER "msvc"
#elif defined __MINGW32__ || defined __MINGW64__
#define IC_PLATFORM_MINGW
#define IC_PLATFORM_COMPILER "mingw"
#elif (defined __GNUC__ && !defined __clang__)
#define IC_PLATFORM_GCC
#define IC_PLATFORM_COMPILER "gcc"
#elif defined __clang__
#define IC_PLATFORM_CLANG
#define IC_PLATFORM_COMPILER "clang"
#elif defined __DOXYGEN__
/* define these so they show up in the documentation */
#define IC_PLATFORM_MSVC
#define IC_PLATFORM_MINGW
#define IC_PLATFORM_GCC
#define IC_PLATFORM_CLANG
#define IC_PLATFORM_COMPILER "doxygen"
#else
#define IC_PLATFORM_UNKNOWN
#define IC_PLATFORM_COMPILER "unknown"
#endif /* _MSC_VER */
#endif
#endif

/**
 * @def IC_PLATFORM_ARCH
 * @brief A string containing the name of the architecture being compiled on. 
 *
 * Valid options are `x86`, `amd64`, `aarch32`, and `aarch64`.
 */
#ifndef IC_PLATFORM_ARCH
#if defined __i386__ || defined _M_IX86
#define IC_PLATFORM_ARCH "x86"
#elif defined __x86_64__ || defined _M_AMD64
#define IC_PLATFORM_ARCH "amd64"
#elif defined __arm__
#if defined __arm64__ || defined __aarch64__
#define IC_PLATFORM_ARCH "aarch64"
#else
#define IC_PLATFORM_ARCH "aarch32"
#endif /* __arm64__ || defined __aarch64__ */
#endif /* __i386__ */
#endif /* IC_PLATFORM_ARCH */

/**
 * @def IC_PLATFORM_OS
 * @brief String containing the name of the operating system we were 
 * compiled on.
 * 
 * Valid options are `windows`, `linux`, `macos`, or `unsupported`.
 */

/**
 * @def IC_PLATFORM_DLL
 * @brief String containing the extension of the shared library objects 
 * on the system.
 * 
 * Valid options are `dll` for Windows, `so` for Linux, and `dylib` for MacOS.
 */

/**
 * @def IC_PLATFORM_EXE
 * @brief String containing the extension of the executables on the system.
 *
 * Valid options are `exe` for Windows and blank for every othersystem.
 */

/**
 * @def PATH_SEP
 * @brief The character of the path separator on the system. 
 *
 * For Windows this is `\\`, other systems are `/`.
 */

/**
 * @def IC_PLATFORM_WINDOWS
 * @brief Defined if we are on a Windows system.
 */

/**
 * @def IC_PLATFORM_LINUX
 * @brief Defined if we are on a Linux system.
 */

/**
 * @def IC_PLATFORM_MACOS
 * @brief Defined if we are compiling on a MacOS system.
 */

/**
 * @def IC_PRIVATE
 * @brief This attribute can be used for private functions which should not be
 * visible. 
 
 * @note On GNUC this is defined as `__attribute__((visibility("hidden")))`. 
 * @note On MSVC this has no effect.
 */

/**
 * @def IC_PUBLIC
 * @brief This attribute can be used to declare functions for library exports.
 * 
 * @note On GNUC this is defined as `__attribute__((visibility("default")))`.
 * @note On MSVC this is defined as `__declspec(dllexport)`.
 */

/**
 * @def IC_IMPORT
 * @brief This attribute can be used to declare library import functions.
 * 
 * @note On GNUC this has no effect.
 * @note On MSVC this is defined as `__declspec(dllimport)`.
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
 */
#ifndef IC_DEBUG
#define IC_PLATFORM_STRING IC_PLATFORM_OS "-" IC_PLATFORM_ARCH
#else
#define IC_PLATFORM_STRING IC_PLATFORM_OS "-" IC_PLATFORM_ARCH "-debug"
#endif

/** @} */

#endif /* IC_PLATFORM_H */
