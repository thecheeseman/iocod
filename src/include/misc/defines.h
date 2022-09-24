#pragma once

#include "iocod_config.h"

#if defined IOCOD_PLATFORM_GCC || defined IOCOD_PLATFORM_CLANG
#define IOCOD_ASSUME(expr) if (expr) {} else __builtin_unreachable()
#define IOCOD_EXPORT __attribute__((visibility("default")))
#define IOCOD_IMPORT __attribute__((visibility("default")))
#define IOCOD_LIKELY(x) __builtin_expect((x), 1)
#define IOCOD_UNLIKELY(x) __builtin_expect((x), 0)
#define IOCOD_UNREACHABLE() __builtin_unreachable()
#elif defined IOCOD_PLATFORM_MSVC
#define IOCOD_ASSUME(expr) __assume(expr)
#define IOCOD_EXPORT __declspec(dllexport)
#define IOCOD_IMPORT __declspec(dllimport)
#define IOCOD_LIKELY(x) (x)
#define IOCOD_UNLIKELY(x) (x)
#define IOCOD_UNREACHABLE() __assume(0)
#else
#define IOCOD_DEBUG_BREAK()
#define IOCOD_ASSUME(expr)
#define IOCOD_EXPORT
#define IOCOD_IMPORT
#define IOCOD_LIKELY(x) (x)
#define IOCOD_UNLIKELY(x) (x)
#define IOCOD_UNREACHABLE()
#endif

// msvc uses a different system
#ifdef IOCOD_PLATFORM_MSVC
#define IOCOD_CXX_STANDARD _MSVC_LANG
#else
#define IOCOD_CXX_STANDARD __cplusplus
#endif

#if 0
#ifndef LIKELY
#if IOCOD_CXX_STANDARD >= 201703L
#define LIKELY [[likely]]
#else
#define LIKELY
#endif
#endif

#ifndef UNLIKELY
#if IOCOD_CXX_STANDARD >= 201703L
#define UNLIKELY [[unlikely]]
#else
#define UNLIKELY 
#endif
#endif
#endif

#ifndef ALWAYS_INLINE
#if defined IOCOD_PLATFORM_GCC || defined IOCOD_PLATFORM_CLANG
#define ALWAYS_INLINE __attribute__((always_inline)) inline
#elif defined IOCOD_PLATFORM_MSVC
#define ALWAYS_INLINE __forceinline
#else
#define ALWAYS_INLINE inline
#endif
#endif

#ifndef FORCE_INLINE
#ifdef IOCOD_DEBUG
#define FORCE_INLINE inline
#else
#define FORCE_INLINE ALWAYS_INLINE
#endif
#endif

#ifndef FALLTHROUGH
#define FALLTHROUGH [[fallthrough]]
#endif

#ifndef NO_DISCARD
#define NO_DISCARD [[nodiscard]]
#endif

#ifndef DEPRECATED
#define DEPRECATED(ver, msg) [[deprecated(msg)]]
#endif
