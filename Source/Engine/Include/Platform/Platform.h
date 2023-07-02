// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ENGINE_PLATFORM_H
#define ENGINE_PLATFORM_H

#include <Core/String.h>

namespace iocod {

enum class PlatformArchitecture {
    X86_64,
    Arm64
};

enum class PlatformCompiler {
    MSVC,
    Clang,
    GCC
};

enum class PlatformOs {
    Windows,
    Linux,
    MacOs
};

struct CpuInfo {
    u64 cpuCores{};
    u64 cpuThreads{};
    u64 cpuCacheL1{};
    u64 cpuCacheL2{};
    u64 cpuCacheL3{};
    f64 cpuMhz{};
    String cpuVendor{};
    String cpuModel{};
};

struct MemoryInfo {
    u64 totalPhysical{};
    u64 availablePhysical{};
    u64 totalPageFile{};
    u64 availablePageFile{};
    u64 totalVirtual{};
    u64 availableVirtual{};
};

class IOCOD_API Platform {
public:
    CLASS_ABSTRACT(Platform)

    virtual void Initialize(const String& appName) = 0;
    virtual void Shutdown() = 0;

    virtual String GetLastErrorAsString() = 0;
    virtual void DisplayFatalErrorAndExit(const String& errorMessage) = 0;

    virtual void* DllOpen(const String& path) = 0;
    virtual void* DllLoadSymbol(void* library, const String& symbolName) = 0;
    virtual void DllClose(void* library) = 0;

    virtual String GetCurrentUser() = 0;
    virtual void StartProcess(const String& path, bool doExit) = 0;
    virtual void OpenUrl(const String& url, bool doExit) = 0;

    virtual void MakeDirectory(const String& path) = 0;
    virtual String CurrentDirectory() = 0;
    virtual String DefaultBasePath() = 0;

    virtual String GetClipboardText() = 0;

    virtual CpuInfo GetCpuInfo() = 0;
    virtual String GetCpuInfoString() = 0;
    virtual MemoryInfo GetMemoryInfo() = 0;
    virtual String GetMemoryInfoString() = 0;

    virtual void Print(const String& message) = 0;

    virtual void PumpEvents() = 0;

    //
    // static members
    //

    #if defined(IOCOD_X86_64)
    static constexpr PlatformArchitecture kPlatformArch = PlatformArchitecture::X86_64;
    static constexpr const char* kPlatformArchString = "x86_64";
    #elif defined(IOCOD_ARM64)
    static constexpr PlatformArchitecture kPlatformArch = PlatformArchitecture::Arm64;
    static constexpr const char* kPlatformArchString = "arm64";
    #endif

    #ifdef IOCOD_MSVC
    static constexpr PlatformCompiler kPlatformCompiler = PlatformCompiler::MSVC;
    static constexpr const char* kPlatformCompilerString = "msvc";

    static constexpr int kPlatformCompilerVersion =
        VersionEncode((_MSC_FULL_VER / 10000000),
                      (_MSC_FULL_VER % 10000000) / 100000,
                      (_MSC_FULL_VER % 100000) / 100);
    #elif defined(IOCOD_GCC) || defined(IOCOD_CLANG)
    #ifdef IOCOD_CLANG
    static constexpr PlatformCompiler kPlatformCompiler = PlatformCompiler::Clang;
    static constexpr const char* kPlatformCompilerString = "clang";
    #else
    static constexpr PlatformCompiler kPlatformCompiler = PlatformCompiler::GCC;
    static constexpr const char* kPlatformCompilerString = "gcc";
    #endif

    static constexpr int kPlatformCompilerVersion =
        VersionEncode(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
    #endif

    #ifdef IOCOD_WINDOWS
    static constexpr PlatformOs kPlatform = PlatformOs::Windows;
    static constexpr const char* kPlatformString = "windows64";
    static constexpr const char* kPlatformDllExtension = ".dll";
    static constexpr const char* kPlatformExeExtension = ".exe";
    static constexpr const char* kPlatformPathSeparator = "\\";
    static constexpr const char* kPlatformNewline = "\r\n";
    #elif defined(IOCOD_LINUX)
    static constexpr Platform kPlatform = Platform::Linux;
    static constexpr const char* kPlatformString = "linux64";
    static constexpr const char* kPlatformDllExtension = ".so";
    static constexpr const char* kPlatformExeExtension = "";
    static constexpr const char* kPlatformPathSeparator = "/";
    static constexpr const char* kPlatformNewline = "\n";
    #elif defined(IOCOD_MACOS)
    static constexpr Platform kPlatform = Platform::Darwin;
    static constexpr const char* kPlatformString = "macos64";

    static constexpr const char* kPlatformDllExtension = ".dylib";
    static constexpr const char* kPlatformExeExtension = "";
    static constexpr const char* kPlatformPathSeparator = "/";
    static constexpr const char* kPlatformNewline = "\n";
    #endif

    static String GetPlatformInfoString()
    {
        return String{kPlatformString} + ' ' + kPlatformArchString + " [" + kPlatformCompilerString + ' '
        + kPlatformCompilerVersion + ']';
    }

    static Platform& GetInstance();

    static void Log(const String& message);
};

} // namespace iocod

#endif // ENGINE_PLATFORM_H
