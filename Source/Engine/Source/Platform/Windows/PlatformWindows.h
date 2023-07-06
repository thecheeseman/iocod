// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ENGINE_PLATFORM_WINDOWS_PLATFORM_WINDOWS_H
#define ENGINE_PLATFORM_WINDOWS_PLATFORM_WINDOWS_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <Platform/Platform.h>

namespace iocod {

class PlatformWindows final : public Platform {
public:
    void Initialize(const String& appName) override;
    void Shutdown() override;

    String GetLastErrorAsString() override;
    void DisplayFatalErrorAndExit(const String& errorMessage, const SourceLocation& loc = SourceLocation::Current()) override;

    void* DllOpen(const String& path) override;
    void* DllLoadSymbol(void* library, const String& symbolName) override;
    void DllClose(void* library) override;

    String GetCurrentUser() override;
    void StartProcess(const String& path, bool doExit) override;
    void OpenUrl(const String& url, bool doExit) override;
    void MakeDirectory(const String& path) override;
    String CurrentDirectory() override;
    String DefaultBasePath() override;
    String GetClipboardText() override;

    CpuInfo GetCpuInfo() override;
    String GetCpuInfoString() override;
    MemoryInfo GetMemoryInfo() override;
    String GetMemoryInfoString() override;

    void Print(const String& message) override;

    void ShowMessageBox(const String& title, const String& message, MessageBoxType type = MessageBoxType::Ok) override;

    void PumpEvents() override;

private:
    HINSTANCE m_instance{};
    HWND m_hwnd{};

    char m_appName[256]{};
};

} // namespace iocod

#endif // ENGINE_PLATFORM_WINDOWS_PLATFORM_WINDOWS_H
