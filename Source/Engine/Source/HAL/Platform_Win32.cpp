// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <windows.h>
#include <windowsx.h>
#include <Core/Log.h>
#include <HAL/Platform.h>

namespace iocod::Platform {

//
namespace {
HINSTANCE hInstance;
HWND hwnd;
}
//

// --------------------------------
// Platform::GetLastErrorAsString
// --------------------------------
String GetLastErrorAsString()
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
// Platform::DisplayFatalErrorAndExit
// --------------------------------
[[noreturn]] void DisplayFatalErrorAndExit()
{
    const String message = GetLastErrorAsString();
    MessageBoxW(nullptr, message.ToWideString(), L"Fatal Error", MB_OK | MB_ICONERROR);
    ExitProcess(1);
}

LRESULT CALLBACK ProcessWindowMessage(HWND hwnd, const UINT msg, const WPARAM wParam, const LPARAM lParam)
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
// Platform::Initialize
// --------------------------------
void Initialize()
{
    if (!GetModuleHandleExW(0, nullptr, &hInstance))
        DisplayFatalErrorAndExit();

    const HICON icon = LoadIconA(hInstance, IDI_APPLICATION);

    WNDCLASSW wc{};
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = ProcessWindowMessage;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = icon;
    wc.hCursor = LoadCursorA(nullptr, IDC_ARROW);
    wc.hbrBackground = nullptr;
    wc.lpszClassName = L"iocod";

    if (!RegisterClassW(&wc))
        DisplayFatalErrorAndExit();

    constexpr DWORD style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZE | WS_THICKFRAME;
    constexpr DWORD exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

    RECT border{};
    AdjustWindowRectEx(&border, style, FALSE, exStyle);
}

// --------------------------------
// Platform::Shutdown
// --------------------------------
void Shutdown() { }

} // namespace iocod
