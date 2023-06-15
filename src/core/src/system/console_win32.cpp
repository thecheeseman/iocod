// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later
#define WIN32_LEAN_AND_MEAN
#include <mutex>
#include <regex>
#include <windows.h>

#include "console.h"
#include "core/string.h"
#include "core/system.h"

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
HWND inputBox;

HBRUSH editBackground;
HBRUSH errorBackground;

WNDPROC inputLineProc;

HFONT textFont; // used throughout the window
HFONT errorFont; // used for error messages
HFONT buttonFont;

int windowWidth;
int windowHeight;
bool quitOnClose = false;
bool initialized = false;

char errorMsg[512]{};
char inputBuffer[512]{};

std::vector<String> messageBuffer;

constexpr int kConsoleBufferSize = 1 << 14;

String GetLastErrorAsString()
{
    const DWORD error = GetLastError();
    if (error == 0)
        return String{};

    LPSTR buffer = nullptr;
    const size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                                       FORMAT_MESSAGE_IGNORE_INSERTS,
                                       nullptr,
                                       error,
                                       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                       (LPSTR) &buffer,
                                       0,
                                       nullptr);

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
    case WM_CTLCOLORSTATIC:
    {
        const auto handle = reinterpret_cast<HWND>(lparam);
        const auto hdc = reinterpret_cast<HDC>(wparam);

        if (handle == editBox) {
            SetBkColor(hdc, BACKGROUND_COLOR);
            SetTextColor(hdc, TEXT_COLOR);
            return reinterpret_cast<LRESULT>(editBackground);
        }
        if (handle == errorBox) {
            if (timePolarity) {
                SetBkColor(hdc, BACKGROUND_ERROR);
                SetTextColor(hdc, TEXT_ERROR);
            } else {
                SetBkColor(hdc, BACKGROUND_ERROR);
                SetTextColor(hdc, TEXT_BLACK);
            }
            return reinterpret_cast<LRESULT>(errorBackground);
        }
    }
    break;
    case WM_SYSCOMMAND:
        if (wparam == SC_CLOSE)
            PostQuitMessage(0);
        break;
    case WM_COMMAND:
        if (wparam == COPY_ID) {
            SendMessageA(editBox, EM_SETSEL, 0, -1);
            SendMessageA(editBox, WM_COPY, 0, 0);
        } else if (wparam == QUIT_ID) {
            if (quitOnClose) {
                PostQuitMessage(0);
            } else {
                // copy_string("quit");
                // sys_queue_event(SYS_EVENT_QUIT);
            }
        } else if (wparam == CLEAR_ID) {
            Console::Clear();
        }
        break;
    case WM_CREATE:
        editBackground = CreateSolidBrush(BACKGROUND_COLOR);
        errorBackground = CreateSolidBrush(BACKGROUND_ERROR);

    // timer for flashing error messages
        SetTimer(hwnd, 1, 250, nullptr);
        break;
    case WM_TIMER:
        if (wparam == 1) {
            timePolarity = !timePolarity;
            if (errorBox)
                InvalidateRect(errorBox, nullptr, false);
        }
        break;
    default:
        break;
    }

    return DefWindowProcA(hwnd, msg, wparam, lparam);
}

#define K_ENTER 28
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
        if (wparam == 13) {
            GetWindowTextA(inputBox, inputBuffer, sizeof(inputBuffer));
            SetWindowTextA(inputBox, "");

            if (strlen(inputBuffer) > 0) {
                const String message = "]" + String(inputBuffer) + "\n";
                Console::Print(message);
            }
        }
        break;
    default:
        break;
    }

    return CallWindowProcA(inputLineProc, hwnd, msg, wparam, lparam);
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
    wc.hCursor = LoadCursorA(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<struct HBRUSH__*>(COLOR_WINDOW);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = DED_CLASS;

    if (!RegisterClassA(&wc))
        return std::make_pair(false, "RegisterClassA failed: " + GetLastErrorAsString());

    constexpr DWORD rectStyle = WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX;
    RECT rect{};
    rect.left = 0;
    rect.right = 720;
    rect.top = 0;
    rect.bottom = 480;
    AdjustWindowRect(&rect, rectStyle, false);

    HDC hdc = GetDC(GetDesktopWindow());
    const int width = GetDeviceCaps(hdc, HORZRES);
    const int height = GetDeviceCaps(hdc, VERTRES);
    ReleaseDC(GetDesktopWindow(), hdc);

    windowWidth = rect.right - rect.left + 1;
    windowHeight = rect.bottom - rect.top + 1;

    wnd = CreateWindowExA(0,
                          DED_CLASS,
                          "iocod console",
                          rectStyle,
                          (width - 720) / 2,
                          (height - 480) / 2,
                          windowWidth,
                          windowHeight,
                          nullptr,
                          nullptr,
                          hInstance,
                          nullptr);

    if (wnd == nullptr)
        return std::make_pair(false, "CreateWindowExA failed" + GetLastErrorAsString());

    //
    // fonts
    //
    hdc = GetDC(wnd);
    auto calculateFontSize = [hdc](const int ptSize) {
        return -MulDiv(ptSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    };

    textFont = CreateFontA(calculateFontSize(10),
                           0,
                           0,
                           0,
                           FW_LIGHT,
                           0,
                           0,
                           0,
                           DEFAULT_CHARSET,
                           OUT_DEFAULT_PRECIS,
                           CLIP_DEFAULT_PRECIS,
                           CLEARTYPE_QUALITY,
                           FF_MODERN | FIXED_PITCH,
                           "Consolas");

    errorFont = CreateFontA(calculateFontSize(12),
                            0,
                            0,
                            0,
                            FW_BOLD,
                            0,
                            0,
                            0,
                            DEFAULT_CHARSET,
                            OUT_DEFAULT_PRECIS,
                            CLIP_DEFAULT_PRECIS,
                            CLEARTYPE_QUALITY,
                            FF_MODERN | FIXED_PITCH,
                            "Consolas Bold");

    buttonFont = CreateFontA(calculateFontSize(10),
                             0,
                             0,
                             0,
                             FW_BOLD,
                             0,
                             0,
                             0,
                             DEFAULT_CHARSET,
                             OUT_DEFAULT_PRECIS,
                             CLIP_DEFAULT_PRECIS,
                             CLEARTYPE_QUALITY,
                             FF_MODERN | FIXED_PITCH,
                             "Consolas Bold");

    ReleaseDC(wnd, hdc);

    //
    // controls
    //
    // buffer
    editBox = CreateWindowExA(0,
                              "edit",
                              nullptr,
                              WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | ES_LEFT |
                              ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
                              6,
                              40,
                              708,
                              377,
                              wnd,
                              reinterpret_cast<HMENU>(EDITBOX_ID),
                              hInstance,
                              nullptr);
    SendMessageA(editBox, WM_SETFONT, reinterpret_cast<WPARAM>(textFont), 0);

    // inputBox
    inputBox = CreateWindowExA(0,
                               "edit",
                               nullptr,
                               WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
                               6,
                               426,
                               708,
                               20,
                               wnd,
                               reinterpret_cast<HMENU>(INPUTBOX_ID),
                               hInstance,
                               nullptr);
    inputLineProc = reinterpret_cast<WNDPROC>(
        SetWindowLongPtrA(inputBox, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(InputLineWndProc)));
    SendMessageA(inputBox, WM_SETFONT, reinterpret_cast<WPARAM>(textFont), 0);

    // copy
    buttonCopy = CreateWindowExA(0,
                                 "button",
                                 nullptr,
                                 BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                 5,
                                 452,
                                 72,
                                 24,
                                 wnd,
                                 reinterpret_cast<HMENU>(COPY_ID),
                                 hInstance,
                                 nullptr);
    SendMessageA(buttonCopy, WM_SETTEXT, 0, reinterpret_cast<LPARAM>("copy"));
    SendMessageA(buttonCopy, WM_SETFONT, reinterpret_cast<WPARAM>(buttonFont), 0);

    // clear
    buttonClear = CreateWindowExA(0,
                                  "button",
                                  nullptr,
                                  BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                  82,
                                  452,
                                  72,
                                  24,
                                  wnd,
                                  reinterpret_cast<HMENU>(CLEAR_ID),
                                  hInstance,
                                  nullptr);
    SendMessageA(buttonClear, WM_SETTEXT, 0, reinterpret_cast<LPARAM>("clear"));
    SendMessageA(buttonClear, WM_SETFONT, reinterpret_cast<WPARAM>(buttonFont), 0);

    // quit
    buttonQuit = CreateWindowExA(0,
                                 "button",
                                 nullptr,
                                 BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                 642,
                                 452,
                                 72,
                                 24,
                                 wnd,
                                 reinterpret_cast<HMENU>(QUIT_ID),
                                 hInstance,
                                 nullptr);
    SendMessageA(buttonQuit, WM_SETTEXT, 0, reinterpret_cast<LPARAM>("quit"));
    SendMessageA(buttonQuit, WM_SETFONT, reinterpret_cast<WPARAM>(buttonFont), 0);

    //
    ShowWindow(wnd, SW_SHOWDEFAULT);
    UpdateWindow(wnd);
    SetForegroundWindow(wnd);
    SetFocus(inputBox);

    messageBuffer.reserve(256);

    initialized = true;
    return std::make_pair(true, "");
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
    while (PeekMessageA(&msg, nullptr, 0, 0, PM_NOREMOVE)) {
        if (!GetMessageA(&msg, nullptr, 0, 0))
            sys->Exit(1);

        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

// --------------------------------
// Console::GetInput
// --------------------------------
String GetInput()
{
    static String s_returnedText;

    if (inputBuffer[0] == '\0')
        return "";

    s_returnedText = inputBuffer;
    inputBuffer[0] = '\0';

    return s_returnedText;
}

// --------------------------------
// Console::Clear
// --------------------------------
void Clear()
{
    SendMessageA(editBox, EM_SETSEL, 0, -1);
    SendMessageA(editBox, EM_REPLACESEL, 0, reinterpret_cast<LPARAM>(""));
    UpdateWindow(editBox);
}

// --------------------------------
// Console::Show
// --------------------------------
void Show(const ConsoleVisLevel level, const bool quitOnClose_)
{
    quitOnClose = quitOnClose_;

    if (!wnd)
        return;

    switch (level) {
    case ConsoleVisLevel::Hidden:
        ShowWindow(wnd, SW_HIDE);
        break;
    case ConsoleVisLevel::Normal:
        ShowWindow(wnd, SW_SHOW);
        SendMessageA(editBox, EM_LINESCROLL, 0, 0xffff);
        break;
    case ConsoleVisLevel::Minimized:
        ShowWindow(wnd, SW_MINIMIZE);
        break;
    }
}

std::mutex consoleMutex;
std::atomic_size_t messagesWaiting = 0;

// --------------------------------
// Console::Print
// --------------------------------
void Print(const String& message)
{
    std::scoped_lock lock(consoleMutex);

    messageBuffer.push_back(message);
    ++messagesWaiting;
}

void Flush()
{
    std::scoped_lock lock(consoleMutex);

    if (!messagesWaiting.load())
        return;

    size_t length = 0;
    for (const auto& m : messageBuffer) {
        length += m.length();
    }

    String message;
    message.reserve(length);
    for (const auto& m : messageBuffer) {
        message += m;
    }
    messageBuffer.clear();
    OutputDebugStringA(message.c_str());

    const char* msg;
    if (message.length() > kConsoleBufferSize - 1)
        msg = message.c_str() + message.length() - kConsoleBufferSize + 1;
    else
        msg = message.c_str();

    static u64 s_totalChars = 0;
    char buffer[kConsoleBufferSize * 2]{};
    char* b = buffer;
    size_t i = 0;
    while (msg[i] && ((b - buffer) < sizeof(buffer) - 1)) {
        if ((msg[i] == '\n' && msg[i + 1] == '\r')) {
            b[0] = '\r';
            b[1] = '\n';
            b += 2;
            i++;
        } else if (msg[i] == '\r' || msg[i] == '\n') {
            b[0] = '\r';
            b[1] = '\n';
            b += 2;
        } else {
            *b = msg[i];
            b++;
        }

        i++;
    }
    *b = '\0';
    const size_t bufferLength = b - buffer;
    s_totalChars += bufferLength;

    if (s_totalChars > sizeof(buffer)) {
        SendMessageA(editBox, EM_SETSEL, 0, -1);
        s_totalChars = bufferLength;
    } else {
        SendMessageA(editBox, EM_SETSEL, 0xffff, 0xffff);
    }

    SendMessageA(editBox, EM_LINESCROLL, 0, 0xffff);
    SendMessageA(editBox, EM_SCROLLCARET, 0, 0);
    SendMessageA(editBox, EM_REPLACESEL, 0, reinterpret_cast<LPARAM>(buffer));

    messagesWaiting = 0;
    // const String formatted = std::regex_replace(message, std::regex("\n"), "\r\n");
}

// --------------------------------
// Console::DebugPrint
// --------------------------------
void DebugPrint(const String& message)
{
    OutputDebugStringA(message.c_str());
}

// --------------------------------
// Console::DisplayError
// --------------------------------
void DisplayError(const String& message)
{
    Strncpy(errorMsg, message.c_str(), sizeof(errorMsg));

    if (!errorBox) {
        errorBox = CreateWindowExA(0,
                                   "static",
                                   nullptr,
                                   WS_CHILD | WS_VISIBLE | SS_SUNKEN,
                                   6,
                                   5,
                                   708,
                                   30,
                                   wnd,
                                   reinterpret_cast<HMENU>(ERRORBOX_ID),
                                   hInstance,
                                   nullptr);
        SendMessageA(errorBox, WM_SETFONT, reinterpret_cast<WPARAM>(errorFont), true);
        SetWindowTextA(errorBox, errorMsg);
        DestroyWindow(inputBox);
        inputBox = nullptr;
    }
}

void WaitForQuit()
{
    MSG msg{};

    while (true) {
        if (!GetMessageA(&msg, nullptr, 0, 0))
            break;

        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}
} // namespace Console
} // namespace iocod
