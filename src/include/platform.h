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
 * @file platform.h
 * @date 2022-02-04
*/

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include "build_number.h"

//
// Windows specific
//
#if defined(_WIN32)

#pragma warning(disable : 4018)     // signed/unsigned mismatch

#if defined(_MSC_VER)
#define OS_STRING "win_msvc"
#elif defined(__MINGW32__) || defined(__MINGW64__)
#define OS_STRING "win_mingw"
#else
#error Unknown or unsupported Windows OS
#endif // _MSC_VER

#if defined(_WIN64)
#define ARCH_STRING "x86_64"
#else
#define ARCH_STRING "x86"
#endif // _WIN64

// windows common
#define DLL_EXT "dll"

#define QDECL __cdecl
#define QCALL __stdcall

#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)

#define Q3_LITTLE_ENDIAN

#define IDINLINE __inline
#define PATH_SEP '\\'
// windows common

#ifndef __BASE_FILE__
#define __BASE_FILE__ __FILE__
#endif // __BASE_FILE__
#endif // _WIN32

//
// Linux
//
#if defined(__linux__) 

#include <endian.h>

#define OS_STRING "linux"

#define DLL_EXT "so"

#define IDINLINE inline
#define PATH_SEP '/'

#if defined(__i386__)
#define ARCH_STRING "i386"
#elif defined(__x86_64__)
#define ARCH_STRING "x86_64"
#endif // __i386__

#if __FLOAT_WORD_ORDER == __BIG_ENDIAN
#define Q3_BIG_ENDIAN
#else
#define Q3_LITTLE_ENDIAN
#endif

#define QDECL 
#define QCALL 

#define DLLEXPORT
#define DLLIMPORT

#endif

//
//
//
#ifndef OS_STRING
#error "Operating system not supported"
#endif

#ifndef ARCH_STRING
#error "Architecture not supported"
#endif

#ifndef IDINLINE
#error "IDINLINE not defined"
#endif

#ifndef PATH_SEP
#error "PATH_SEP not defined"
#endif

#ifndef DLL_EXT
#error "DLL_EXT not defined"
#endif

// todo endianness

#ifdef NDEBUG
#define PLATFORM_STRING OS_STRING "-" ARCH_STRING
#else
#define PLATFORM_STRING OS_STRING "-" ARCH_STRING "-debug"
#endif

#endif // __PLATFORM_H__
