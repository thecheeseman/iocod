// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_PLATFORM_H
#define CORE_PLATFORM_H

#include <core/version.h>

/// @def IOCOD_ARCH
/// @brief Architecture string.

/// @def IOCOD_ARCH_I386
/// @brief Defined if the architecture is i386.

/// @def IOCOD_ARCH_AMD64
/// @brief Defined if the architecture is amd64.

#ifndef IOCOD_ARCH
    #if defined(__i386__) || defined(_M_IX86)
        #define IOCOD_ARCH "i386"
        #define IOCOD_ARCH_I386
    #elif defined(__x86_64__) || defined(_M_AMD64)
        #define IOCOD_ARCH "amd64"
        #define IOCOD_ARCH_AMD64
    #elif defined(__arm__)
        #if defined(__arm64__) || defined(__aarch64__)
            #define IOCOD_ARCH "arm64"
            #define IOCOD_ARCH_ARM64
        #else
            #define IOCOD_ARCH "arm32"
            #define IOCOD_ARCH_ARM32
        #endif
    #endif
#endif

/// @def IOCOD_OS
/// @brief Operating system string.

/// @def IOCOD_DLL_EXTENSION
/// @brief File extension for dynamic libraries.

/// @def IOCOD_EXE_EXTENSION
/// @brief File extension for executables.

/// @def IOCOD_PATH_SEPARATOR
/// @brief Path separator.

/// @def IOCOD_NEWLINE
/// @brief Newline string.

#ifdef _WIN32
    /// @def IOCOD_OS_WINDOWS
    /// @brief Defined if the operating system is Windows.
    #define IOCOD_OS_WINDOWS

    #ifdef IOCOD_ARCH_AMD64
        #define IOCOD_OS "win64"
    #else
        #define IOCOD_OS "win32"
    #endif

    #define IOCOD_DLL_EXTENSION  ".dll"
    #define IOCOD_EXE_EXTENSION  ".exe"
    #define IOCOD_PATH_SEPARATOR "\\"
    #define IOCOD_NEWLINE        "\r\n"
#elif defined(__linux__)
    /// @def IOCOD_OS_LINUX
    /// @brief Defined if the operating system is Linux.
    #define IOCOD_OS_LINUX

    #ifdef IOCOD_ARCH_AMD64
        #define IOCOD_OS "linux64"
    #else
        #define IOCOD_OS "linux32"
    #endif

    #define IOCOD_DLL_EXTENSION  ".so"
    #define IOCOD_EXE_EXTENSION  ""
    #define IOCOD_PATH_SEPARATOR "/"
    #define IOCOD_NEWLINE        "\n"
#elif defined(__APPLE__)
    /// @def IOCOD_OS_DARWIN
    /// @brief Defined if the operating system is MacOS.
    #define IOCOD_OS_DARWIN
    #define IOCOD_OS             "darwin64"

    #define IOCOD_DLL_EXTENSION  ".dylib"
    #define IOCOD_EXE_EXTENSION  ""
    #define IOCOD_PATH_SEPARATOR "/"
    #define IOCOD_NEWLINE        "\n"
#else
    #error "Unknown or unsupported operating system"
#endif

/// @def IOCOD_COMPILER
/// String name of the compiler.

/// @def IOCOD_MSVC
/// Defined if compiling under MSVC.

/// @def IOCOD_CLANG
/// Defined if compiling under Clang.

/// @def IOCOD_GCC
/// Defined if compiling under GCC.

/// @def IOCOD_COMPILER_VERSION
/// Current version number of the compiler.

#ifndef IOCOD_COMPILER
    #ifdef _MSC_VER
        #define IOCOD_COMPILER_MSVC
        #define IOCOD_COMPILER "msvc"

        #if defined(_MSC_FULL_VER) && (_MSC_FULL_VER >= 140000000)
            #define IOCOD_COMPILER_VERSION                                \
                IOCOD_VERSION_ENCODE(_MSC_FULL_VER / 10000000,            \
                                     (_MSC_FULL_VER % 10000000) / 100000, \
                                     (_MSC_FULL_VER % 100000) / 100)
        #elif defined(_MSC_FULL_VER)
            #define IOCOD_COMPILER_VERSION                              \
                IOCOD_VERSION_ENCODE(_MSC_FULL_VER / 1000000,           \
                                     (_MSC_FULL_VER % 1000000) / 10000, \
                                     (_MSC_FULL_VER % 10000) / 10)
        #elif defined(_MSC_VER)
            #define IOCOD_COMPILER_VERSION \
                IOCOD_VERSION_ENCODE(_MSC_VER / 100, _MSC_VER % 100, 0)
        #endif

        #define IOCOD_UNREACHABLE() __assume(0)
    #elif defined(__GNUC__)
        #ifdef __clang__
            #define IOCOD_COMPILER_CLANG
            #define IOCOD_COMPILER "clang"
        #else
            #define IOCOD_COMPILER_GCC
            #define IOCOD_COMPILER "gcc"
        #endif

        #ifdef __GNUC_PATCHLEVEL__
            #define IOCOD_COMPILER_VERSION                     \
                IOCOD_VERSION_ENCODE(__GNUC__, __GNUC_MINOR__, \
                                     __GNUC_PATCHLEVEL__)
        #elif defined(__GNUC_)
            #define IOCOD_COMPILER_VERSION \
                IOCOD_VERSION_ENCODE(__GNUC__, __GNUC_MINOR__, 0)
        #endif

        #define IOCOD_UNREACHABLE() __builtin_unreachable()
    #else
        #error "Unknown or unsupported compiler."
    #endif
#endif // IOCOD_COMPILER

#ifdef IOCOD_COMPILER_MSVC
    #define IOCOD_LOCAL
    #define IOCOD_EXPORT __declspec(dllexport)
    #define IOCOD_IMPORT __declspec(dllimport)
    #define IOCOD_CONSTRUCTOR
    #define IOCOD_DESTRUCTOR
#elif defined(IOCOD_COMPILER_GCC) || defined(IOCOD_COMPILER_CLANG)
    #define IOCOD_LOCAL  __attribute__((visibility("hidden")))
    #define IOCOD_EXPORT __attribute__((visibility("default")))
    #define IOCOD_IMPORT
    #define IOCOD_CONSTRUCTOR __attribute__((constructor))
    #define IOCOD_DESTRUCTOR  __attribute__((destructor))
#else
    #define IOCOD_LOCAL
    #define IOCOD_EXPORT
    #define IOCOD_IMPORT
    #define IOCOD_CONSTRUCTOR
    #define IOCOD_DESTRUCTOR
#endif

/// @def IOCOD_VERSION_CHECK
/// Check if the given compiler version matches or is newer.
#define IOCOD_VERSION_CHECK(major, minor, patch) \
    (IOCOD_COMPILER_VERSION >= IOCOD_VERSION_ENCODE(major, minor, patch))

/// @def IOCOD_HAS_ATTRIBUTE
/// Check if a given attribute exists.
#ifdef __has_attribute
    #define IOCOD_HAS_ATTRIBUTE(attribute) __has_attribute(attribute)
#else
    #define IOCOD_HAS_ATTRIBUTE(attribute) (0)
#endif

/// @def IOCOD_HAS_ATTRIBUTE
/// Check if a given attribute exists in C++.
#if defined(__cplusplus) && defined(__has_cpp_attribute)
    #define IOCOD_HAS_CPP_ATTRIBUTE(attribute) __has_cpp_attribute(attribute)
#else
    #define IOCOD_HAS_CPP_ATTRIBUTE(attribute) (0)
#endif

/// @def IOCOD_HAS_BUILTIN
/// Check if a given builtin exists.
#ifdef __has_builtin
    #define IOCOD_HAS_BUILTIN(builtin) __has_builtin(builtin)
#else
    #define IOCOD_HAS_BUILTIN(builtin) (0)
#endif

/// @def IOCOD_HAS_EXTENSION
/// Check if the compiler supports a given extension.
#ifdef __has_extension
    #define IOCOD_HAS_EXTENSION(extension) __has_extension(extension)
#else
    #define IOCOD_HAS_EXTENSION(extension) (0)
#endif

/// @def IOCOD_HAS_FEATURE
/// Check if the compiler supports a given feature.
#ifdef __has_feature
    #define IOCOD_HAS_FEATURE(feature) __has_feature(feature)
#else
    #define IOCOD_HAS_FEATURE(feature) (0)
#endif

/// @def IOCOD_NODISCARD
/// Mark a function as not discarding its return value.
#if IOCOD_HAS_CPP_ATTRIBUTE(nodiscard)
    #define IOCOD_NODISCARD [[nodiscard]]
#elif IOCOD_HAS_ATTRIBUTE(warn_unused_result)
    #define IOCOD_NODISCARD __attribute__((warn_unused_result))
#else
    #define IOCOD_NODISCARD
#endif

/// @def IOCOD_DEPRECATED
/// Mark a function as deprecated.
#if IOCOD_HAS_CPP_ATTRIBUTE(deprecated)
    #define IOCOD_DEPRECATED [[deprecated]]
#elif IOCOD_HAS_ATTRIBUTE(deprecated)
    #define IOCOD_DEPRECATED __attribute__((deprecated))
#else
    #define IOCOD_DEPRECATED
#endif

/// @def IOCOD_DEPRECATED_MSG
/// Mark a function as deprecated with a message.
#if IOCOD_HAS_CPP_ATTRIBUTE(deprecated)
    #define IOCOD_DEPRECATED_MSG(msg) [[deprecated(msg)]]
#elif IOCOD_HAS_ATTRIBUTE(deprecated)
    #define IOCOD_DEPRECATED_MSG(msg) __attribute__((deprecated(msg)))
#else
    #define IOCOD_DEPRECATED_MSG(msg)
#endif

/// @def IOCOD_NORETURN
/// Mark a function as not returning.
#if IOCOD_HAS_CPP_ATTRIBUTE(noreturn)
    #define IOCOD_NORETURN [[noreturn]]
#elif IOCOD_HAS_ATTRIBUTE(noreturn)
    #define IOCOD_NORETURN __attribute__((noreturn))
#else
    #define IOCOD_NORETURN
#endif

/// @def IOCOD_ALWAYS_INLINE
/// Mark a function as always inline.
#if IOCOD_HAS_ATTRIBUTE(always_inline)
    #define IOCOD_ALWAYS_INLINE __attribute__((always_inline)) inline
#elif defined(IOCOD_COMPILER_MSVC)
    #define IOCOD_ALWAYS_INLINE __forceinline
#else
    #define IOCOD_ALWAYS_INLINE inline
#endif

/// @def IOCOD_NOINLINE
/// Mark a function as not inlinable.
#if IOCOD_HAS_ATTRIBUTE(noinline)
    #define IOCOD_NOINLINE __attribute__((noinline))
#elif defined(IOCOD_COMPILER_MSVC)
    #define IOCOD_NOINLINE __declspec(noinline)
#else
    #define IOCOD_NOINLINE
#endif

#endif // CORE_PLATFORM_H
