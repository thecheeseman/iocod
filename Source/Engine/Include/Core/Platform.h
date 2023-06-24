// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_PLATFORM_H
#define CORE_PLATFORM_H

#include <Core/Version.h>

namespace iocod {

// clang-format off

// should always be set correctly from CMake but just as a sanity check
#ifdef __cplusplus
    // MSVC doesn't set this value properly
    // __cplusplus is always 199711L unless you specify /Zc:__cplusplus
    #if defined(_MSC_VER) && _MSVC_LANG < 202002L
        #error "C++20 or later is required"
    #elif !defined(_MSC_VER) && __cplusplus < 202002L
        #error "C++20 or later is required"
    #endif
#endif

enum class PlatformArchitecture {
    X86,
    X86_64,
    Arm32,
    Arm64
};

#if defined(__i386__) || defined(_M_IX86)
    #define IOCOD_ARCH_X86
    inline constexpr PlatformArchitecture kPlatformArch = PlatformArchitecture::X86;
    inline constexpr const char* kPlatformArchString = "x86";
#elif defined(__x86_64__) || defined(_M_AMD64)
    #define IOCOD_ARCH_X86_64
    inline constexpr PlatformArchitecture kPlatformArch = PlatformArchitecture::X86_64;
    inline constexpr const char* kPlatformArchString = "x86_64";
#elif defined(__arm__)
    #if defined(__arm64__) || defined(__aarch64__)
        #define IOCOD_ARCH_ARM64
        inline constexpr PlatformArchitecture kPlatformArch = PlatformArchitecture::Arm64;
        inline constexpr const char* kPlatformArchString = "arm64";
    #else
        #define IOCOD_ARCH_ARM
        inline constexpr PlatformArchitecture kPlatformArch = PlatformArchitecture::Arm32;
        inline constexpr const char* kPlatformArchString = "arm32";
    #endif
#else
    #error "Unknown or unsupported architecture"
#endif

enum class Platform {
    Windows,
    Linux,
    Darwin
};

#ifdef _WIN32
    #define IOCOD_OS_WINDOWS
    inline constexpr Platform kPlatform = Platform::Windows;

    #ifdef IOCOD_ARCH_AMD64
        inline constexpr const char* kPlatformString = "windows64";
    #else
        inline constexpr const char* kPlatformString = "windows32";
    #endif

    inline constexpr const char* kPlatformDllExtension = ".dll";
    inline constexpr const char* kPlatformExeExtension = ".exe";
    inline constexpr const char* kPlatformPathSeparator = "\\";
    inline constexpr const char* kPlatformNewline = "\r\n";
#elif defined(__linux__)
    #define IOCOD_OS_LINUX
    inline constexpr Platform kPlatform = Platform::Linux;

    #ifdef IOCOD_ARCH_AMD64
        inline constexpr const char* kPlatformString = "linux64";
    #else
        inline constexpr const char* kPlatformString = "linux32";
    #endif

    inline constexpr const char* kPlatformDllExtension = ".so";
    inline constexpr const char* kPlatformExeExtension = "";
    inline constexpr const char* kPlatformPathSeparator = "/";
    inline constexpr const char* kPlatformNewline = "\n";
#elif defined(__APPLE__)
    #define IOCOD_OS_DARWIN
    inline constexpr Platform kPlatform = Platform::Darwin;
    inline constexpr const char* kPlatformString = "macos64";

    inline constexpr const char* kPlatformDllExtension = ".dylib";
    inline constexpr const char* kPlatformExeExtension = "";
    inline constexpr const char* kPlatformPathSeparator = "/";
    inline constexpr const char* kPlatformNewline = "\n";
#else
    #error "Unknown or unsupported operating system"
#endif

enum class PlatformCompiler {
    MSVC,
    Clang,
    GCC
};

#ifdef _MSC_VER
    #define IOCOD_COMPILER_MSVC
    inline constexpr PlatformCompiler kPlatformCompiler = PlatformCompiler::MSVC;
    inline constexpr const char* kPlatformCompilerString = "msvc";

    inline constexpr int kPlatformCompilerVersion =
        VersionEncode((_MSC_FULL_VER / 10000000), (_MSC_FULL_VER % 10000000) / 100000,
                      (_MSC_FULL_VER % 100000) / 100);

    #define IOCOD_LOCAL
    #define IOCOD_EXPORT             __declspec(dllexport)
    #define IOCOD_IMPORT             __declspec(dllimport)
    #define IOCOD_CONSTRUCTOR
    #define IOCOD_DESTRUCTOR
    #define IOCOD_CODE_SECTION(name) __declspec(code_seg(name))
    #define IOCOD_ALWAYS_INLINE      __forceinline
    #define IOCOD_NO_INLINE          __declspec(noinline)
    #define IOCOD_NO_RETURN          __declspec(noreturn)
    #define IOCOD_BREAK()            __debugbreak()
    #define IOCOD_RETURN_ADDRESS()   _ReturnAddress()
    #define IOCOD_ASSUME(expr)       __analysis_assume(!!(expr))
    #define IOCOD_UNREACHABLE()      __assume(0)
#elif defined(__GNUC__)
    #ifdef __clang__
        #define IOCOD_COMPILER_CLANG
        inline constexpr PlatformCompiler kPlatformCompiler = PlatformCompiler::Clang;
        inline constexpr const char* kPlatformCompilerString = "clang";
    #else
        #define IOCOD_COMPILER_GCC
        inline constexpr PlatformCompiler kPlatformCompiler = PlatformCompiler::GCC;
        inline constexpr const char* kPlatformCompilerString = "gcc";
    #endif

    inline constexpr int kPlatformCompilerVersion =
        VersionEncode(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);

    #define IOCOD_LOCAL              __attribute__((visibility("hidden")))
    #define IOCOD_EXPORT             __attribute__((visibility("default")))
    #define IOCOD_IMPORT
    #define IOCOD_CONSTRUCTOR        __attribute__((constructor))
    #define IOCOD_DESTRUCTOR         __attribute__((destructor))
    #define IOCOD_CODE_SECTION(name) __attribute__((section(name)))
    #define IOCOD_ALWAYS_INLINE      __attribute__((always_inline)) inline
    #define IOCOD_NO_INLINE          __attribute__((noinline))
    #define IOCOD_NO_RETURN          __attribute__((noreturn))
    #define IOCOD_BREAK()            __builtin_trap()
    #define IOCOD_RETURN_ADDRESS()   __builtin_return_address(0)
    #define IOCOD_ASSUME(expr)       (__builtin_expect(!(expr), 0) ? __builtin_unreachable() : (void) 0)
    #define IOCOD_UNREACHABLE()      __builtin_unreachable()
#else
    #error "Unknown or unsupported compiler"
#endif

#ifdef IOCOD_BUILD_ENGINE
#define IOCOD_API IOCOD_EXPORT
#else
#define IOCOD_API IOCOD_IMPORT
#endif

#ifdef IOCOD_DEBUG
    inline constexpr bool kDebugBuild = true;
#else
    inline constexpr bool kDebugBuild = false;
#endif

// clang-format on

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

#ifdef IOCOD_COMPILER_MSVC
    #define IOCOD_DISABLE_ALL_VC_WARNINGS()          \
        __pragma(warning(push, 0)) __pragma(warning( \
            disable : 4244 4265 4267 4350 4472 4509 4548 4623 4710 4985 6320 4755 4625 4626 4702))
    #define IOCOD_RESTORE_ALL_VC_WARNINGS() __pragma(warning(pop))
#else
    #define IOCOD_DISABLE_ALL_VC_WARNINGS()
    #define IOCOD_RESTORE_ALL_VC_WARNINGS()
#endif

} // namespace iocod

#endif // CORE_PLATFORM_H