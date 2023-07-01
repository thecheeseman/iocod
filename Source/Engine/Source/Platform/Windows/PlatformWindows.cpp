// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PlatformWindows.h"

#include <Core/Log.h>

namespace iocod {

Platform& Platform::GetInstance()
{
    static constinit PlatformWindows s_platform;
    return s_platform;
}

// --------------------------------
// PlatformWindows::GetLastErrorAsString
// --------------------------------
String PlatformWindows::GetLastErrorAsString()
{
    const DWORD error = GetLastError();
    if (error == 0)
        return String{};

    LPWSTR buffer = nullptr;
    const size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                                       FORMAT_MESSAGE_IGNORE_INSERTS,
                                       nullptr,
                                       error,
                                       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                       reinterpret_cast<LPWSTR>(&buffer),
                                       0,
                                       nullptr);

    String message{buffer, size};
    LocalFree(buffer);

    return message;
}

// --------------------------------
// PlatformWindows::DisplayFatalErrorAndExit
// --------------------------------
[[noreturn]] void PlatformWindows::DisplayFatalErrorAndExit(const String& errorMessage)
{
    MessageBoxW(nullptr, errorMessage.ToWideString(), L"Fatal Error", MB_OK | MB_ICONERROR);
    ExitProcess(1);
}

LRESULT CALLBACK ProcessWindowMessage(HWND hwnd, const UINT msg, const WPARAM wParam,
                                      const LPARAM lParam)
{
    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }

    return 0;
}

// --------------------------------
// PlatformWindows::Initialize
// --------------------------------
void PlatformWindows::Initialize(const String& appName)
{
    appName.ToWideString(m_appName, sizeof(m_appName));

    if (!GetModuleHandleExW(0, nullptr, &m_instance))
        DisplayFatalErrorAndExit(GetLastErrorAsString());

    const HICON icon = LoadIconA(m_instance, IDI_APPLICATION);

    WNDCLASSW wc{};
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = ProcessWindowMessage;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_instance;
    wc.hIcon = icon;
    wc.hCursor = LoadCursorA(nullptr, IDC_ARROW);
    wc.hbrBackground = nullptr;
    wc.lpszClassName = m_appName;

    if (!RegisterClassW(&wc))
        DisplayFatalErrorAndExit(GetLastErrorAsString());

    constexpr DWORD style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZE |
        WS_THICKFRAME;
    constexpr DWORD exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

    RECT border{};
    AdjustWindowRectEx(&border, style, FALSE, exStyle);
}

// --------------------------------
// PlatformWindows::Shutdown
// --------------------------------
void PlatformWindows::Shutdown() { }

// --------------------------------
// PlatformWindows::LibraryOpen
// --------------------------------
void* PlatformWindows::LibraryOpen(const String& path)
{
    return LoadLibraryW(path.ToWideString());
}

// --------------------------------
// PlatformWindows::LibraryLoadSymbol
// --------------------------------
void* PlatformWindows::LibraryLoadSymbol(void* library, const String& symbolName)
{
    return GetProcAddress(static_cast<HMODULE>(library), symbolName.c_str());
}

// --------------------------------
// PlatformWindows::LibraryClose
// --------------------------------
void PlatformWindows::LibraryClose(void* library)
{
    FreeLibrary(static_cast<HMODULE>(library));
}

} // namespace iocod
