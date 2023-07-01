// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ENGINE_ENGINE_DEFINES_H
#define ENGINE_ENGINE_DEFINES_H

//
// global defines
//

#if defined(__i386__) || defined(_M_IX86)
    #define IOCOD_ARCH_X86
#elif defined(__x86_64__) || defined(_M_AMD64)
    #define IOCOD_ARCH_X86_64
#elif defined(__arm__)
    #if defined(__arm64__) || defined(__aarch64__)
        #define IOCOD_ARCH_ARM64
    #else
        #define IOCOD_ARCH_ARM
    #endif
#else
    #error "Unknown or unsupported architecture"
#endif

#ifdef _WIN32
    #define IOCOD_OS_WINDOWS
#elif defined(__linux__)
    #define IOCOD_OS_LINUX
#elif defined(__APPLE__)
    #define IOCOD_OS_DARWIN
#else
    #error "Unknown or unsupported operating system"
#endif

#ifdef _MSC_VER
    #define IOCOD_COMPILER_MSVC
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
    #else
        #define IOCOD_COMPILER_GCC
    #endif

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

//
// useful defines that should be available everywhere
//

#define ENABLE_ENUM_CLASS_FLAGS(Type)         \
    template <>                               \
    struct EnumClassFlag<Type> {              \
        static constexpr bool enabled = true; \
    };

#ifndef CLASS_ABSTRACT
#define CLASS_ABSTRACT(Class) \
    Class() = default; \
    Class(const Class&) = delete; \
    Class(Class&&) = delete; \
    Class& operator=(const Class&) = delete; \
    Class& operator=(Class&&) = delete; \
    virtual ~Class() = default;
#endif

#ifndef CLASS_NON_COPYABLE
#define CLASS_NON_COPYABLE(Class) \
    Class(const Class&) = delete; \
    Class(Class&&) = delete; \
    Class& operator=(const Class&) = delete; \
    Class& operator=(Class&&) = delete;
#endif

#ifdef __has_attribute
    #define IOCOD_HAS_ATTRIBUTE(attribute) __has_attribute(attribute)
#else
    #define IOCOD_HAS_ATTRIBUTE(attribute) (0)
#endif

#if defined(__cplusplus) && defined(__has_cpp_attribute)
    #define IOCOD_HAS_CPP_ATTRIBUTE(attribute) __has_cpp_attribute(attribute)
#else
    #define IOCOD_HAS_CPP_ATTRIBUTE(attribute) (0)
#endif

#ifdef __has_builtin
    #define IOCOD_HAS_BUILTIN(builtin) __has_builtin(builtin)
#else
    #define IOCOD_HAS_BUILTIN(builtin) (0)
#endif

#ifdef __has_extension
    #define IOCOD_HAS_EXTENSION(extension) __has_extension(extension)
#else
    #define IOCOD_HAS_EXTENSION(extension) (0)
#endif

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

#endif // ENGINE_ENGINE_DEFINES_H
