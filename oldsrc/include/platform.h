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

#ifndef PLATFORM_H
#define PLATFORM_H

/* C standard */
#if defined __STDC__
#define PLATFORM_STD_C89

#if defined __STDC_VERSION__
#if __STDC_VERSION__ >= 201710L
#define PLATFORM_STD_C18
#endif /* __STDC_VERSION__ >= 201710L */

#if __STDC_VERSION__ >= 201112L
#define PLATFORM_STD_C11
#endif /* __STDC_VERSION__ >= 201112L */

#if __STDC_VERSION__ >= 199901L
#define PLATFORM_STD_C99
#endif /* __STDC_VERSION__ >= 199901L */
#endif /* __STDC_VERSION__ */
#endif /* __STDC__ */

/* compiler */
#ifndef PLATFORM_COMPILER
#if defined _MSC_VER 
#define PLATFORM_MSVC
#define PLATFORM_COMPILER "msvc"
#elif defined __MINGW32__ || defined __MINGW64__
#define PLATFORM_MINGW
#define PLATFORM_COMPILER "mingw"
#elif defined __GNUC__
#define PLATFORM_GCC
#define PLATFORM_COMPILER "gcc"
#elif defined __clang__
#define PLATFORM_CLANG
#define PLATFORM_COMPILER "clang"
#elif defined __llvm__
#define PLATFORM_LLVM
#define PLATFORM_COMPILER "llvm"
#endif
#endif /* PLATFORM_COMPILER */

/* architecture */
#if defined __i386__ || defined _M_IX86
#define PLATFORM_ARCH "x86"
#elif defined __x86_64__ || defined _M_AMD64
#define PLATFORM_ARCH "amd64"
#elif defined __arm__
#if defined __arm64__ || defined __aarch64__
#define PLATFORM_ARCH "arm64"
#else
#define PLATFORM_ARCH "arm32"
#endif /* __arm64__ || defined __aarch64__ */
#endif /* __i386__ */

/* OS: Windows */
#if defined _WIN32
#define PLATFORM_WINDOWS 1
#define PLATFORM_OS "win"
#define PLATFORM_DLL "dll"

#define INLINE __inline
#define PATH_SEP '\\'
#define DECL __cdecl
#define CALL __stdcall
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#define LOCAL 

#ifndef __BASE_FILE__
#define __BASE_FILE__ __FILE__
#endif /* __BASE_FILE__ */

/* OS: non-windows */
#else

#define INLINE inline
#define PATH_SEP '/'
#define DECL 
#define CALL 

#if defined __GNUC__ && __GNUC__ >= 4
#define EXPORT __attribute__((visibility("default")))
#define IMPORT
#define LOCAL __attribute__((visibility("hidden")))
#else
#define EXPORT
#define IMPORT
#define LOCAL
#endif /* __GNUC__ && __GNUC__ >= 4*/

/* linux */
#if defined __linux__ 
#define PLATFORM_LINUX 1
#define PLATFORM_OS "linux"
#define PLATFORM_DLL "so"
/* macos */
#elif defined __APPLE__
#define PLATFORM_MACOS 1
#define PLATFORM_OS "macos"
#define PLATFORM_DLL "dylib"
#endif
#endif /* PLATFORM_WINDOWS */

#ifndef PLATFORM_ARCH
#define PLATFORM_ARCH "unsupported"
#endif /* PLATFORM_ARCH */

#ifndef PLATFORM_OS
#define PLATFORM_OS "unsupported"
#endif /* PLATFORM_OS */

#ifndef PLATFORM_DLL
#define PLATFORM_DLL "lib"
#endif /* PLATFORM_DLL */

#ifdef NDEBUG
#define PLATFORM_STRING PLATFORM_OS "-" PLATFORM_ARCH
#else
#define PLATFORM_STRING PLATFORM_OS "-" PLATFORM_ARCH "-debug"
#endif

#endif /* PLATFORM_H */
