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

#ifndef IC_PLATFORM_H
#define IC_PLATFORM_H

/*
================================================================================
C standards

If you need to check for a feature that is only present in a specific 
C standard, you can check do an ifdef check, e.g.
    #if defined IC_PLATFORM_STD_CXX 
    do this
    #else
    do that
    #endif
================================================================================
 */
#if defined __STDC_VERSION__
#define IC_PLATFORM_STD_C89
#define IC_PLATFORM_STD_C95 /* __STDC_VERSION__ first defined here */

/* GCC ~3.0, MSVC 12.0 */
#if __STDC_VERSION__ >= 199901L
#define IC_PLATFORM_STD_C99
#endif /* __STDC_VERSION__ >= 199901L */

/* GCC 4.6, Clang 3.1, MSVC 16.8 */
#if __STDC_VERSION__ >= 201112L
#define IC_PLATFORM_STD_C11
#endif /* __STDC_VERSION__ >= 201112L */

/* GCC 8.1, Clang 7.0, MSVC 16.8 */
#if __STDC_VERSION__ >= 201710L
#define IC_PLATFORM_STD_C17
#endif /* __STDC_VERSION__ >= 201710L */

/* future versions (GCC only) */
#if __STDC_VERSION__ >= 202000L
#define IC_PLATFORM_STD_C2X
#endif /* __STDC_VERSION__ >= 202000L */
#elif defined __STDC__
#define IC_PLATFORM_STD_C89
#endif /* __STDC_VERSION__ */

/*
================================================================================
GNUC versioning

IC_GNUC_VERSION is defined if we have any compiler that supports GNU C,
but that also means compilers which are _not_ GCC, e.g. clang
================================================================================
*/
#if defined __GNUC__ && defined __GNUC_PATCHLEVEL__
#define IC_GNUC_VERSION \
    IC_VERSION_ENCODE(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#elif defined __GNUC__
#define IC_GNUC_VERSION IC_VERSION_ENCODE(__GNUC__, __GNUC_MINOR__, 0)
#endif /* defined __GNUC__ && defined __GNUC_PATCHLEVEL__ */

#if defined IC_GNUC_VERSION 
#define IC_GNUC_VERSION_CHECK(maj, min, patch) \
    (IC_GNUC_VERSION >= IC_VERSION_ENCODE(maj, min, patch))
#else
#define IC_GNUC_VERSION_CHECK(maj, min, patch) (0)
#endif /* defined IC_GNUC_VERSION  */

/*
================================================================================
GCC versioning

IC_GCC_VERSION is only defined if we are _really_ using GCC 
================================================================================
*/
#if defined IC_GNUC_VERSION && !defined __clang__
#define IC_GCC_VERSION IC_GNUC_VERSION
#endif

#if defined IC_GCC_VERSION
#define IC_GCC_VERSION_CHECK(maj, min, patch) \
    (IC_GCC_VERSION >= IC_VERSION_ENCODE(maj, min, patch))
#else
#define IC_GCC_VERSION_CHECK(maj, min, patch) (0)
#endif /* defined IC_GCC_VERSION */

/*
================================================================================
MSVC versioning
================================================================================
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
#endif /* defined _MSC_FULL_VER && (_MSC_FULL_VER >= 140000000) */

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
#endif /* !defined IC_MSVC_VERSION */

/*
================================================================================
Attributes and feature checking
================================================================================
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

/*
================================================================================
Pragmas
================================================================================
*/
#if defined IC_PLATFORM_STD_C99 || defined __clang__ || \
    IC_GCC_VERSION_CHECK(3, 0, 0)
#define IC_PRAGMA(val) _Pragma(#val)
#elif IC_MSVC_VERSION_CHECK(15, 0, 0)
#define IC_PRAGMA(val) __pragma(val)
#else
#define IC_PRAGMA(val)
#endif

/*
================================================================================
Diagnostics
================================================================================
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

/*
================================================================================
Deprecation

You can define a deprecated function in the following way:
    IC_DEPRECATED("1.0")
    void old_function(char *msg);

If you deprecated a function in favour of another one, you can use
    IC_DEPRECATED_FOR("1.0", "new_function")
    void old_function(char *msg);
================================================================================
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

/*
================================================================================
Unused result

For a function defined as
    IC_WARN_UNUSED_RESULT
    void *some_function(void);

On compatible compilers, the follow usage should warn that the result of the 
function is unused:
    some_function();
================================================================================
*/
#if IC_HAS_ATTRIBUTE(warn_unused_result) || IC_GCC_VERSION_CHECK(3, 4, 0)
#define IC_WARN_UNUSED_RESULT __attribute__((__warn_unused_result__))
#elif defined _Check_return_
#define IC_WARN_UNUSED_RESULT _Check_return_
#else
#define IC_WARN_UNUSED_RESULT
#endif

/*
================================================================================
No return

On compatible compilers, tells the compiler a specific function will never
return
================================================================================
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

/*
================================================================================
Unreachable code

Tells compatible compilers that the code following will never be reached
================================================================================
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

/*
================================================================================
Non-null for varargs (GNU/GCC only)

Allows you to tell GNUC compilers that certain parameters should never be null,
for example:
    IC_NON_NULL(1, 3)
    void do_something(char *str, size_t size, char *out);
================================================================================
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
#endif
IC_DIAGNOSTIC_POP

/*
================================================================================
Printf format (GNU/GCC only)

Allows you to tell GNUC compilers about vararg style format parameters in 
a function, e.g.:
    IC_PRINTF_FORMAT(2, 3)
    void print_error(int code, const char *fmt, ...);
================================================================================
*/
#if defined __MINGW32__ && IC_GCC_HAS_ATTRIBUTE(format, 4, 4, 0) && \
    !defined __USE_MINGW_ASNI_STDIO
#define IC_PRINTF_FORMAT(idx, first) __attribute__((__format(ms_printf, idx, first)))
#elif defined __MINGW32__ &&IC_GCC_HAS_ATTRIBUTE(format, 4, 4, 0) && \
    defined __USE_MINGW_ASNI_STDIO
#define IC_PRINTF_FORMAT(idx, first) __attribute__((__format(gnu_printf, idx, first)))
#elif IC_HAS_ATTRIBUTE(format) || IC_GCC_VERSION_CHECK(3, 1, 0)
#define IC_PRINTF_FORMAT(idx, first) __attribute__((__format__(__printf__, idx, first)))
#else
#define IC_PRINTF_FORMAT(idx, first)
#endif

/*
================================================================================
Malloc

From GCC manual:
    The malloc attribute is used to tell the compiler that a function may be 
    treated as if any non-NULL pointer it returns cannot alias any other pointer 
    valid when the function returns and that the memory has undefined content
================================================================================
*/
#if IC_HAS_ATTRIBUTE(malloc) || IC_GCC_VERSION_CHECK(3, 1, 0)
#define IC_MALLOC __attribute__((__malloc__))
#elif IC_MSVC_VERSION_CHECK(14, 0, 0)
#define IC_MALLOC __declspec(restrict)
#endif

/*
================================================================================
Inline

This shouldn't be strictly necessary since we will always be compiling for C99+
================================================================================
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

/*
================================================================================
Nothrow

Tells compilers that a function should not throw an exception
================================================================================
*/
#if IC_HAS_ATTRIBUTE(nothrow) || IC_GCC_VERSION_CHECK(3, 3, 0)
#define IC_NO_THROW __attribute__((__nothrow__))
#elif IC_MSVC_VERSION_CHECK(13, 1, 0)
#define IC_NO_THROW __declspec(nothrow)
#else
#define IC_NO_THROW
#endif

/*
================================================================================
Fallthrough for switch statements

Tells the compiler that we explicitly intend to have a switch case fallthrough
================================================================================
*/
#if IC_HAS_ATTRIBUTE(fallthrough) || IC_GCC_VERSION_CHECK(7, 0, 0)
#define IC_FALL_THROUGH __attribute__((__fallthrough__))
#elif defined __fallthrough
#define IC_FALL_THROUGH __fallthrough
#else
#define IC_FALL_THROUGH
#endif

/*
================================================================================
Non null returns 

Allows compilers to optimise knowing that the function never returns NULL
================================================================================
*/
#if IC_HAS_ATTRIBUTE(returns_nonnull) || IC_GCC_VERSION_CHECK(4, 9, 0)  
#define IC_RETURNS_NON_NULL __attribute__((__returns_nonnull__))
#elif defined _Ret_notnull_
#define IC_RETURNS_NON_NULL _Ret_notnull_
#else
#define IC_RETURNS_NON_NULL
#endif

/*
================================================================================
Message / warning
================================================================================
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

/*
================================================================================
Unused attributes
================================================================================
*/
#if IC_HAS_ATTRIBUTE(unused) || IC_GCC_VERSION_CHECK(4, 7, 2)
#define IC_UNUSED __attribute__((unused))
#else
#define IC_UNUSED
#endif

/*
================================================================================
Determine which compiler we are using

Currently we only support MSVC + MinGW on Windows, GCC/clang everywhere else
================================================================================
*/
#ifndef IC_PLATFORM_COMPILER
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
#endif
#endif /* IC_PLATFORM_COMPILER */

/*
================================================================================
Determine which architecture we are on

Currently these are defined to match the game dlls from the original game
for testing compatibility purposes, e.g.:
    game.mp.i386.so
    game_mp_x86.dll

In the future these should just be:
    iocodgame-x64.so
    iocodgame-x64.dll
================================================================================
*/
#if defined __i386__ 
#define IC_PLATFORM_ARCH "i386" /* 32-bit *nix */
#elif defined _M_IX86
#define IC_PLATFORM_ARCH "x86"  /* 32-bit windows */
#elif defined __x86_64__ || defined _M_AMD64
#define IC_PLATFORM_ARCH "x86_64"
#elif defined __arm__
#if defined __arm64__ || defined __aarch64__
#define IC_PLATFORM_ARCH "arm64"
#else
#define IC_PLATFORM_ARCH "arm32"
#endif /* __arm64__ || defined __aarch64__ */
#endif /* __i386__ */

/*
================================================================================
Windows platform defines
================================================================================
*/
#if defined _WIN32
#define IC_PLATFORM_WINDOWS 1
#define IC_PLATFORM_OS      "win"
#define IC_PLATFORM_DLL     "dll"

#define PATH_SEP    '\\'

#define IC_PRIVATE  
#define IC_PUBLIC   __declspec(dllexport)
#define IC_IMPORT   __declspec(dllimport)

#ifndef __BASE_FILE__
#define __BASE_FILE__ __FILE__
#endif

/*
================================================================================
Non-Windows platform defines that are common to *nix/macos
================================================================================
*/
#else

#define PATH_SEP    '/'
#define DECL 
#define CALL 

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
#define IC_PLATFORM_LINUX   1
#define IC_PLATFORM_OS      "linux"
#define IC_PLATFORM_DLL     "so"
#elif defined __APPLE__
#define IC_PLATFORM_MACOS   1
#define IC_PLATFORM_OS      "macos"
#define IC_PLATFORM_DLL     "dylib"
#endif

#endif

/*
================================================================================
Unsupported architectures / OS
Still should try to compile but problems might occur
================================================================================
*/
#ifndef IC_PLATFORM_ARCH
#define IC_PLATFORM_ARCH "unsupported"
#endif

#ifndef IC_PLATFORM_OS
#define IC_PLATFORM_OS "unsupported"
#endif

#ifndef IC_PLATFORM_DLL
#define IC_PLATFORM_DLL "lib"
#endif

/*
================================================================================
Platform string for versioning
================================================================================
*/
#ifndef IC_DEBUG
#define IC_PLATFORM_STRING IC_PLATFORM_OS "-" IC_PLATFORM_ARCH
#else
#define IC_PLATFORM_STRING IC_PLATFORM_OS "-" IC_PLATFORM_ARCH "-debug"
#endif

#endif /* IC_PLATFORM_H */
