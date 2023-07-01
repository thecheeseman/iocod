// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ENGINE_PLATFORM_H
#define ENGINE_PLATFORM_H

#include <Core/String.h>

namespace iocod {

enum class PlatformArchitecture {
    x86,
    x86_64,
    Arm32,
    Arm64
};

enum class PlatformCompiler {
    MSVC,
    Clang,
    GCC
};

enum class PlatformOS {
    Windows,
    Linux,
    Darwin
};

class IOCOD_API Platform {
public:
    CLASS_ABSTRACT(Platform)

    virtual void Initialize(const String& appName) = 0;
    virtual void Shutdown() = 0;

    virtual String GetLastErrorAsString() = 0;
    virtual void DisplayFatalErrorAndExit(const String& errorMessage) = 0;

    virtual void* LibraryOpen(const String& path) = 0;
    virtual void* LibraryLoadSymbol(void* library, const String& symbolName) = 0;
    virtual void LibraryClose(void* library) = 0;

    //
    // static methods
    //

    static constexpr PlatformArchitecture GetArchitecture();
    static constexpr const char* GetArchitectureString();
    static constexpr PlatformCompiler GetCompiler();
    static constexpr int GetCompilerVersion();
    static constexpr const char* GetCompilerString();
    static constexpr PlatformOS GetOS();
    static constexpr const char* GetOSString();
    static constexpr const char* GetDllExtension();
    static constexpr const char* GetExeExtension();
    static constexpr const char* GetPathSeparator();
    static constexpr const char* GetNewLineChar();

    static String GetPlatformInfoString();

    static Platform& GetInstance();
};

} // namespace iocod

#endif //ENGINE_PLATFORM_H
