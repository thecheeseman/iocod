#pragma once
#ifndef DEBUG_H
#define DEBUG_H

#include "iocod_config.h"
#include "defines.h"

#ifdef IOCOD_DEBUG
#if defined IOCOD_PLATFORM_GCC || defined IOCOD_PLATFORM_CLANG
#if defined IOCOD_PLATFORM_X86 || defined IOCOD_PLATFORM_AMD64
#define IOCOD_DEBUG_BREAK() do { __asm__ volatile ("int $3"); } while (0)
#elif defined IOCOD_PLATFORM_ARM32
#if defined(__thumb__)
#define IOCOD_DEBUG_BREAK() do { __asm__ volatile (".inst 0xde01"); } while(0)
#else
#define IOCOD_DEBUG_BREAK() do { __asm__ volatile (".inst 0xe7f001f0"); } while(0)
#endif
#elif defined(IOCOD_PLATFORM_ARM64)
#define IOCOD_DEBUG_BREAK() do { __asm__ volatile (".inst 0xd4200000"); } while(0)
#elif __has_include(<signal.h>)
#include <signal.h>
#if defined(SIGTRAP)
#define IOCOD_DEBUG_BREAK() raise(SIGTRAP) // SIGTRAP is preferred
#else
#define IOCOD_DEBUG_BREAK() __builtin_trap() // raises SIGILL
#endif
#else
#define IOCOD_DEBUG_BREAK() __builtin_trap() // raises SIGILL
#endif
#define IOCOD_ASSERT(cond, msg, ...)
#elif defined IOCOD_PLATFORM_MSVC
#include <intrin.h>
#define IOCOD_DEBUG_BREAK() __debugbreak()
#define IOCOD_ASSERT(cond, msg, ...)
#endif
#else
#define IOCOD_DEBUG_BREAK()
#define IOCOD_ASSERT(cond, msg, ...)
#endif

#endif /* DEBUG_H */
