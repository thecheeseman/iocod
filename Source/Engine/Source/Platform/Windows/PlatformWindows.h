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
    void DisplayFatalErrorAndExit(const String& errorMessage) override;

    void* LibraryOpen(const String& path) override;
    void* LibraryLoadSymbol(void* library, const String& symbolName) override;
    void LibraryClose(void* library) override;

private:
    HINSTANCE m_instance{};
    HWND m_hwnd{};

    wchar_t m_appName[256]{};
};

} // namespace iocod

#endif // ENGINE_PLATFORM_WINDOWS_PLATFORM_WINDOWS_H
