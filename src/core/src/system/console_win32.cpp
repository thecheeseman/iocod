// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later
#define WIN32_LEAN_AND_MEAN
#include "console.h"
#include "core/string.h"
#include "core/system.h"

#include <regex>
#include <windows.h>

namespace iocod {

constexpr i64 COPY_ID = 1;
constexpr i64 QUIT_ID = 2;
constexpr i64 CLEAR_ID = 3;
constexpr i64 ERRORBOX_ID = 10;
constexpr i64 ERRORTEXT_ID = 11;
constexpr i64 EDITBOX_ID = 100;
constexpr i64 INPUTBOX_ID = 101;

namespace {

HINSTANCE hInstance;

HWND wnd;
HWND editBox;
HWND buttonClear;
HWND buttonCopy;
HWND buttonQuit;
HWND errorBox;
HWND errorText;
HWND inputBox;

HBRUSH editBackground;
HBRUSH errorBackground;

WNDPROC inputLineProc;

HFONT bufferFont;
HFONT buttonFont;

int windowWidth;
int windowHeight;
bool quitOnClose = false;
bool initialized = false;

char errorMsg[512]{};
char inputBuffer[512]{};

String GetLastErrorAsString()
{
    const DWORD error = GetLastError();
    if (error == 0)
        return String{};

    LPSTR buffer = nullptr;
    const size_t size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR) &buffer, 0, nullptr);

    String message(buffer, size);
    LocalFree(buffer);

    return message;
}

constexpr COLORREF BACKGROUND_COLOR = RGB(0x56, 0x75, 0x64);
constexpr COLORREF TEXT_COLOR = RGB(0xff, 0xff, 0xff);
constexpr COLORREF BACKGROUND_ERROR = RGB(0x80, 0x80, 0x80);
constexpr COLORREF TEXT_ERROR = RGB(0xff, 0x00, 0x00);
constexpr COLORREF TEXT_BLACK = RGB(0x00, 0x00, 0x00);

LRESULT CALLBACK ConWndProc(HWND hwnd, const UINT msg, WPARAM wparam, LPARAM lparam)
{
    // flash the error message
    static bool timePolarity = false;

    switch (msg) {
    case WM_ACTIVATE:
        if (LOWORD(wparam) != WA_INACTIVE)
            SetFocus(inputBox);
        break;
    case WM_CLOSE:
        if (quitOnClose)
            PostQuitMessage(0);
        else
            Console::Show(ConsoleVisLevel::Hidden, false);
        return 0;
    case WM_CTLCOLORSTATIC: {
        const auto handle = reinterpret_cast<HWND>(lparam);
        const auto hdc = reinterpret_cast<HDC>(wparam);

        if (handle == editBox) {
            SetBkColor(hdc, BACKGROUND_COLOR);
            SetTextColor(hdc, TEXT_COLOR);
            return reinterpret_cast<LRESULT>(editBackground);
        } else if (handle == errorBox) {
            if (timePolarity) {
                SetBkColor(hdc, BACKGROUND_ERROR);
                SetTextColor(hdc, TEXT_ERROR);
            } else {
                SetBkColor(hdc, BACKGROUND_ERROR);
                SetTextColor(hdc, TEXT_BLACK);
            }
            return reinterpret_cast<LRESULT>(errorBackground);
        }
    } break;
    case WM_SYSCOMMAND:
        if (wparam == SC_CLOSE)
            PostQuitMessage(0);
        break;
    case WM_COMMAND:
        if (wparam == COPY_ID) {
            SendMessage(editBox, EM_SETSEL, 0, -1);
            SendMessage(editBox, WM_COPY, 0, 0);
        } else if (wparam == QUIT_ID) {
            if (quitOnClose) {
                PostQuitMessage(0);
            } else {
                // copy_string("quit");
                // sys_queue_event(SYS_EVENT_QUIT);
            }
        } else if (wparam == CLEAR_ID) {
            SendMessage(editBox, EM_SETSEL, 0, -1);
            SendMessage(editBox, EM_REPLACESEL, 0, reinterpret_cast<LPARAM>(""));
            UpdateWindow(editBox);
        }
        break;
    case WM_CREATE:
        editBackground = CreateSolidBrush(BACKGROUND_COLOR);
        errorBackground = CreateSolidBrush(BACKGROUND_ERROR);
        SetTimer(hwnd, 1, 1000, nullptr);
        break;
    case WM_TIMER:
        if (wparam == 1) {
            timePolarity = !timePolarity;
            if (errorBox)
                InvalidateRect(errorBox, nullptr, false);
        }
        break;
    default: break;
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

#define K_ENTER    28
#define K_KP_ENTER 0x9c

LRESULT CALLBACK InputLineWndProc(HWND hwnd, const UINT msg, WPARAM wparam, LPARAM lparam)
{
    i64 key = 0;

    switch (msg) {
    case WM_KILLFOCUS:
        if (reinterpret_cast<HWND>(wparam) == hwnd || reinterpret_cast<HWND>(wparam) == errorBox) {
            SetFocus(hwnd);
            return 0;
        }
        break;
        // TODO: handle arrow keys
    case WM_CHAR:
        key = lparam >> 16 & 0xff | (lparam >> 24 & 1) << 7;

        GetWindowText(inputBox, inputBuffer, sizeof(inputBuffer));

        if (key == K_ENTER || key == K_KP_ENTER) {
            SetWindowText(inputBox, "");

            if (strlen(inputBuffer) > 0) {
                const String message = "]" + String(inputBuffer) + "\n";
                Console::Print(message);
            }
        }
        break;
    default: break;
    }

    return CallWindowProc(inputLineProc, hwnd, msg, wparam, lparam);
}

} // namespace

namespace Console {

// --------------------------------
// Console::Initialize
// --------------------------------
std::pair<bool, String> Console::Initialize(void* handle)
{
    hInstance = static_cast<HINSTANCE>(handle);

    WNDCLASS wc{};
    wc.style = 0;
    wc.lpfnWndProc = reinterpret_cast<WNDPROC>(ConWndProc);
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    // wc.hIcon = nullptr;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<struct HBRUSH__*>(COLOR_WINDOW);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = DED_CLASS;

    if (!RegisterClass(&wc))
        return {false, "RegisterClass failed: " + GetLastErrorAsString()};

    constexpr DWORD rectStyle = WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX;
    RECT rect{};
    rect.left = 0;
    rect.right = 540;
    rect.top = 0;
    rect.bottom = 450;
    AdjustWindowRect(&rect, rectStyle, false);

    HDC hdc = GetDC(GetDesktopWindow());
    const int width = GetDeviceCaps(hdc, HORZRES);
    const int height = GetDeviceCaps(hdc, VERTRES);
    ReleaseDC(GetDesktopWindow(), hdc);

    windowWidth = rect.right - rect.left + 1;
    windowHeight = rect.bottom - rect.top + 1;

    wnd = CreateWindowEx(0, DED_CLASS, GAME_NAME, rectStyle, (width - 600) / 2, (height - 450) / 2,
                         windowWidth, windowHeight, nullptr, nullptr, hInstance, nullptr);

    if (wnd == nullptr)
        return {false, "CreateWindowEx failed" + GetLastErrorAsString()};

    // fonts
    hdc = GetDC(wnd);
    const int fontHeight = -MulDiv(8, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    bufferFont =
        CreateFont(fontHeight, 0, 0, 0, FW_LIGHT, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                   CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_MODERN | FIXED_PITCH, "Courier New");

    ReleaseDC(wnd, hdc);

    // inputBox
    constexpr DWORD inputBoxStyle = WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL;
    inputBox = CreateWindow("edit", nullptr, inputBoxStyle, 6, 400, 528, 20, wnd,
                            reinterpret_cast<HMENU>(INPUTBOX_ID), hInstance, nullptr);

    // buttons
    constexpr DWORD buttonStyle = BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON;
    buttonCopy = CreateWindow("button", nullptr, buttonStyle, 5, 425, 72, 24, wnd,
                              reinterpret_cast<HMENU>(COPY_ID), hInstance, nullptr);
    SendMessage(buttonCopy, WM_SETTEXT, 0, reinterpret_cast<LPARAM>("copy"));

    buttonClear = CreateWindow("button", nullptr, buttonStyle, 82, 425, 72, 24, wnd,
                               reinterpret_cast<HMENU>(CLEAR_ID), hInstance, nullptr);
    SendMessage(buttonClear, WM_SETTEXT, 0, reinterpret_cast<LPARAM>("clear"));

    buttonQuit = CreateWindow("button", nullptr, buttonStyle, 462, 425, 72, 24, wnd,
                              reinterpret_cast<HMENU>(QUIT_ID), hInstance, nullptr);
    SendMessage(buttonQuit, WM_SETTEXT, 0, reinterpret_cast<LPARAM>("quit"));

    // buffer
    constexpr DWORD editBoxStyle = WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | ES_LEFT |
                                   ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY;
    editBox = CreateWindow("edit", nullptr, editBoxStyle, 6, 40, 526, 354, wnd,
                           reinterpret_cast<HMENU>(EDITBOX_ID), hInstance, nullptr);
    SendMessage(editBox, WM_SETFONT, reinterpret_cast<WPARAM>(bufferFont), 0);

    inputLineProc = reinterpret_cast<WNDPROC>(
        SetWindowLongPtr(inputBox, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(InputLineWndProc)));
    SendMessage(inputBox, WM_SETFONT, reinterpret_cast<WPARAM>(bufferFont), 0);

    //
    ShowWindow(wnd, SW_SHOWDEFAULT);
    UpdateWindow(wnd);
    SetForegroundWindow(wnd);
    SetFocus(inputBox);

    initialized = true;
    return {true, ""};
}

// --------------------------------
// Console::Shutdown
// --------------------------------
void Shutdown()
{
    if (wnd) {
        ShowWindow(wnd, SW_HIDE);
        CloseWindow(wnd);
        DestroyWindow(wnd);
        wnd = nullptr;
    }
}

// --------------------------------
// Console::Initialized
// --------------------------------
bool Initialized()
{
    return initialized;
}

// --------------------------------
// Console::PumpEvents
// --------------------------------
void PumpEvents()
{
    MSG msg{};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE)) {
        if (!GetMessage(&msg, nullptr, 0, 0))
            sys->Exit(1);

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// --------------------------------
// Console::GetInput
// --------------------------------
String GetInput()
{
    static String returnedText;

    if (inputBuffer[0] == '\0')
        return "";

    returnedText = inputBuffer;
    inputBuffer[0] = '\0';

    return returnedText;
}

// --------------------------------
// Console::Clear
// --------------------------------
void Clear() {}

// --------------------------------
// Console::Show
// --------------------------------
void Show(const ConsoleVisLevel level, const bool quitOnClose_)
{
    quitOnClose = quitOnClose_;

    if (!wnd)
        return;

    switch (level) {
    case ConsoleVisLevel::Hidden: ShowWindow(wnd, SW_HIDE); break;
    case ConsoleVisLevel::Normal:
        ShowWindow(wnd, SW_SHOW);
        SendMessage(editBox, EM_LINESCROLL, 0, 0xffff);
        break;
    case ConsoleVisLevel::Minimized: ShowWindow(wnd, SW_MINIMIZE); break;
    }
}

// --------------------------------
// Console::Print
// --------------------------------
void Print(const String& message)
{
    OutputDebugString(message.c_str());

    // not sure this is the best way to do this
    const String formatted = std::regex_replace(message, std::regex("\n"), "\r\n");

    char buffer[4096]{};
    Strncpy(buffer, formatted.c_str(), 4095);

    SendMessage(editBox, EM_LINESCROLL, 0, 0xffff);
    SendMessage(editBox, EM_SCROLLCARET, 0, 0);
    SendMessage(editBox, EM_REPLACESEL, 0, reinterpret_cast<LPARAM>(buffer));
}

// --------------------------------
// Console::DebugPrint
// --------------------------------
void DebugPrint(const String& message)
{
    OutputDebugString(message.c_str());
}

// --------------------------------
// Console::DisplayError
// --------------------------------
void DisplayError(const String& message)
{
    Strncpy(errorMsg, message.c_str(), sizeof(errorMsg));

    if (!errorBox) {
        // clang-format off
        errorBox = CreateWindow("static", nullptr,
                                 WS_CHILD | WS_VISIBLE | SS_SUNKEN,
                                 6, 5, 526, 30,
                                 wnd,
                                 reinterpret_cast<HMENU>(ERRORBOX_ID),
                                 hInstance, nullptr);
        SendMessage(errorBox, WM_SETFONT, reinterpret_cast<WPARAM>(bufferFont), 0);
        SetWindowText(errorBox, errorMsg);
        DestroyWindow(inputBox);
        inputBox = nullptr;
        // clang-format on
    }
}

} // namespace Console

} // namespace iocod
