// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Platform/Platform.h>

namespace iocod {

#ifdef IOCOD_ARCH_X86
constexpr PlatformArchitecture kPlatformArch = PlatformArchitecture::x86;
constexpr const char* kPlatformArchString = "x86";
#elif defined(IOCOD_ARCH_X86_64)
constexpr PlatformArchitecture kPlatformArch = PlatformArchitecture::x86_64;
constexpr const char* kPlatformArchString = "x86_64";
#elif defined(IOCOD_ARCH_ARM)
constexpr PlatformArchitecture kPlatformArch = PlatformArchitecture::Arm32;
constexpr const char* kPlatformArchString = "arm32";
#elif defined(IOCOD_ARCH_ARM64)
constexpr PlatformArchitecture kPlatformArch = PlatformArchitecture::Arm64;
constexpr const char* kPlatformArchString = "arm64";
#endif

constexpr PlatformArchitecture Platform::GetArchitecture()
{
    return kPlatformArch;
}

constexpr const char* Platform::GetArchitectureString()
{
    return kPlatformArchString;
}

#ifdef IOCOD_COMPILER_MSVC
constexpr PlatformCompiler kPlatformCompiler = PlatformCompiler::MSVC;
constexpr const char* kPlatformCompilerString = "msvc";

constexpr int kPlatformCompilerVersion =
    VersionEncode((_MSC_FULL_VER / 10000000),
                  (_MSC_FULL_VER % 10000000) / 100000,
                  (_MSC_FULL_VER % 100000) / 100);
#elif defined(IOCOD_COMPILER_GCC) || defined(IOCOD_COMPILER_CLANG)
#ifdef IOCOD_COMPILER_CLANG
constexpr PlatformCompiler kPlatformCompiler = PlatformCompiler::Clang;
constexpr const char* kPlatformCompilerString = "clang";
#else
constexpr PlatformCompiler kPlatformCompiler = PlatformCompiler::GCC;
constexpr const char* kPlatformCompilerString = "gcc";
#endif

constexpr int kPlatformCompilerVersion =
    VersionEncode(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#endif

constexpr PlatformCompiler Platform::GetCompiler()
{
    return kPlatformCompiler;
}

constexpr int Platform::GetCompilerVersion()
{
    return kPlatformCompilerVersion;
}

constexpr const char* Platform::GetCompilerString()
{
    return kPlatformCompilerString;
}

#ifdef IOCOD_OS_WINDOWS
constexpr PlatformOS kPlatform = PlatformOS::Windows;

#ifdef IOCOD_ARCH_X86_64
constexpr const char* kPlatformString = "windows64";
#else
constexpr const char* kPlatformString = "windows32";
#endif

constexpr const char* kPlatformDllExtension = ".dll";
constexpr const char* kPlatformExeExtension = ".exe";
constexpr const char* kPlatformPathSeparator = "\\";
constexpr const char* kPlatformNewline = "\r\n";
#elif defined(IOCOD_OS_LINUX)
constexpr Platform kPlatform = Platform::Linux;

#ifdef IOCOD_ARCH_X86_64
constexpr const char* kPlatformString = "linux64";
#else
constexpr const char* kPlatformString = "linux32";
#endif

constexpr const char* kPlatformDllExtension = ".so";
constexpr const char* kPlatformExeExtension = "";
constexpr const char* kPlatformPathSeparator = "/";
constexpr const char* kPlatformNewline = "\n";
#elif defined(IOCOD_OS_DARWIN)
constexpr Platform kPlatform = Platform::Darwin;
constexpr const char* kPlatformString = "macos64";

constexpr const char* kPlatformDllExtension = ".dylib";
constexpr const char* kPlatformExeExtension = "";
constexpr const char* kPlatformPathSeparator = "/";
constexpr const char* kPlatformNewline = "\n";
#endif

constexpr PlatformOS Platform::GetOS()
{
    return kPlatform;
}

constexpr const char* Platform::GetOSString()
{
    return kPlatformString;
}

constexpr const char* Platform::GetDllExtension()
{
    return kPlatformDllExtension;
}

constexpr const char* Platform::GetExeExtension()
{
    return kPlatformExeExtension;
}

constexpr const char* Platform::GetPathSeparator()
{
    return kPlatformPathSeparator;
}

constexpr const char* Platform::GetNewLineChar()
{
    return kPlatformNewline;
}

String Platform::GetPlatformInfoString()
{
    return String{GetOSString()} + ' ' + GetArchitectureString() + " [" + GetCompilerString() + ' '
        + GetCompilerVersion() + ']';
}

} // namespace iocod
