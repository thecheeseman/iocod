// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <mutex>
#include <vector>
#include <Platform/Console.h>

#include "PlatformWindows.h"

namespace iocod::Console {

constexpr const wchar_t* kDedClass = L"iocod_console";
constexpr const wchar_t* kGameName = L"iocod";
constexpr i64 kCopyId = 1;
constexpr i64 kQuitId = 2;
constexpr i64 kClearId = 3;
constexpr i64 kErrorBoxId = 10;
constexpr i64 kErrorTextId = 11;
constexpr i64 kEditBoxId = 100;
constexpr i64 kInputBoxId = 101;

HINSTANCE instance{};

HWND wnd{};
HWND editBox{};
HWND buttonClear{};
HWND buttonCopy{};
HWND buttonQuit{};
HWND errorBox{};
HWND inputBox{};

HBRUSH editBackground{};
HBRUSH errorBackground{};

WNDPROC inputLineProc{};

HFONT textFont{}; // used throughout the window
HFONT errorFont{}; // used for error messages
HFONT buttonFont{};

int windowWidth{};
int windowHeight{};
bool quitOnClose{false};
bool initialized{false};

wchar_t errorMsg[512]{};
wchar_t inputBuffer[512]{};

std::vector<String> messageBuffer;

constexpr int kConsoleBufferSize = 1 << 14;

constexpr COLORREF kBackgroundColor = RGB(0x56, 0x75, 0x64);
constexpr COLORREF kTextColor = RGB(0xff, 0xff, 0xff);
constexpr COLORREF kBackgroundError = RGB(0x80, 0x80, 0x80);
constexpr COLORREF kTextError = RGB(0xff, 0x00, 0x00);
constexpr COLORREF kTextBlack = RGB(0x00, 0x00, 0x00);

LRESULT CALLBACK ConWndProc(HWND hwnd, const UINT msg, WPARAM wparam, LPARAM lparam)
{
    // flash the error message
    static bool s_timePolarity = false;

    switch (msg) {
    case WM_ACTIVATE:
        if (LOWORD(wparam) != WA_INACTIVE)
            SetFocus(inputBox);
        break;
    case WM_CLOSE:
        if (quitOnClose)
            PostQuitMessage(0);
        else
            Show(VisLevel::Hidden, false);
        return 0;
    case WM_CTLCOLORSTATIC:
    {
        const auto handle = reinterpret_cast<HWND>(lparam);
        const auto hdc = reinterpret_cast<HDC>(wparam);

        if (handle == editBox) {
            SetBkColor(hdc, kBackgroundColor);
            SetTextColor(hdc, kTextColor);
            return reinterpret_cast<LRESULT>(editBackground);
        }
        if (handle == errorBox) {
            if (s_timePolarity) {
                SetBkColor(hdc, kBackgroundError);
                SetTextColor(hdc, kTextError);
            } else {
                SetBkColor(hdc, kBackgroundError);
                SetTextColor(hdc, kTextBlack);
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
        if (wparam == kCopyId) {
            SendMessage(editBox, EM_SETSEL, 0, -1);
            SendMessage(editBox, WM_COPY, 0, 0);
        } else if (wparam == kQuitId) {
            if (quitOnClose) {
                PostQuitMessage(0);
            } else {
                // copy_string("quit");
                // sys_queue_event(SYS_EVENT_QUIT);
            }
        } else if (wparam == kClearId) {
            Clear();
        }
        break;
    case WM_CREATE:
        editBackground = CreateSolidBrush(kBackgroundColor);
        errorBackground = CreateSolidBrush(kBackgroundError);

    // timer for flashing error messages
        SetTimer(hwnd, 1, 250, nullptr);
        break;
    case WM_TIMER:
        if (wparam == 1) {
            s_timePolarity = !s_timePolarity;
            if (errorBox)
                InvalidateRect(errorBox, nullptr, false);
        }
        break;
    default:
        break;
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

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
            GetWindowText(inputBox, inputBuffer, sizeof(inputBuffer));
            SetWindowText(inputBox, L"");

            if (Strlen(inputBuffer) > 0) {
                const String fromWide{inputBuffer};
                const String message = "]" + fromWide + "\n";
                Print(message);
            }
        }
        break;
    default:
        break;
    }

    return CallWindowProc(inputLineProc, hwnd, msg, wparam, lparam);
}

// --------------------------------
// Console::Initialize
// --------------------------------
void Initialize()
{
    if (!GetModuleHandleEx(0, nullptr, &instance))
        Platform::GetInstance().DisplayFatalErrorAndExit(Platform::GetInstance().GetLastErrorAsString());

    const HICON icon = LoadIcon(instance, IDI_APPLICATION);

    WNDCLASS wc{};
    wc.style = 0;
    wc.lpfnWndProc = reinterpret_cast<WNDPROC>(ConWndProc);
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = instance;
    wc.hIcon = icon;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH__*>(COLOR_WINDOW);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = kDedClass;

    if (!RegisterClass(&wc))
        Platform::GetInstance().DisplayFatalErrorAndExit(
            "RegisterClassA failed: " + Platform::GetInstance().GetLastErrorAsString());

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

    wnd = CreateWindowEx(0,
                         kDedClass,
                         L"iocod console",
                         rectStyle,
                         (width - 720) / 2,
                         (height - 480) / 2,
                         windowWidth,
                         windowHeight,
                         nullptr,
                         nullptr,
                         instance,
                         nullptr);

    if (wnd == nullptr)
        Platform::GetInstance().DisplayFatalErrorAndExit(
            "CreateWindowExA failed" + Platform::GetInstance().GetLastErrorAsString());

    //
    // fonts
    //
    hdc = GetDC(wnd);
    auto calculateFontSize = [hdc](const int ptSize) {
        return -MulDiv(ptSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    };

    textFont = CreateFont(calculateFontSize(10),
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
                          L"Consolas");

    errorFont = CreateFont(calculateFontSize(12),
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
                           L"Consolas Bold");

    buttonFont = CreateFont(calculateFontSize(10),
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
                            L"Consolas Bold");

    ReleaseDC(wnd, hdc);

    //
    // controls
    //
    // buffer
    editBox = CreateWindowEx(0,
                             L"edit",
                             nullptr,
                             WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | ES_LEFT |
                             ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
                             6,
                             40,
                             708,
                             377,
                             wnd,
                             reinterpret_cast<HMENU>(kEditBoxId),
                             instance,
                             nullptr);
    SendMessage(editBox, WM_SETFONT, reinterpret_cast<WPARAM>(textFont), 0);

    // inputBox
    inputBox = CreateWindowEx(0,
                              L"edit",
                              nullptr,
                              WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
                              6,
                              426,
                              708,
                              20,
                              wnd,
                              reinterpret_cast<HMENU>(kInputBoxId),
                              instance,
                              nullptr);
    inputLineProc = reinterpret_cast<WNDPROC>(
        SetWindowLongPtr(inputBox, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(InputLineWndProc)));
    SendMessage(inputBox, WM_SETFONT, reinterpret_cast<WPARAM>(textFont), 0);

    // copy
    buttonCopy = CreateWindowEx(0,
                                L"button",
                                nullptr,
                                BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                5,
                                452,
                                72,
                                24,
                                wnd,
                                reinterpret_cast<HMENU>(kCopyId),
                                instance,
                                nullptr);
    SendMessage(buttonCopy, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(L"copy"));
    SendMessage(buttonCopy, WM_SETFONT, reinterpret_cast<WPARAM>(buttonFont), 0);

    // clear
    buttonClear = CreateWindowEx(0,
                                 L"button",
                                 nullptr,
                                 BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                 82,
                                 452,
                                 72,
                                 24,
                                 wnd,
                                 reinterpret_cast<HMENU>(kClearId),
                                 instance,
                                 nullptr);
    SendMessage(buttonClear, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(L"clear"));
    SendMessage(buttonClear, WM_SETFONT, reinterpret_cast<WPARAM>(buttonFont), 0);

    // quit
    buttonQuit = CreateWindowEx(0,
                                L"button",
                                nullptr,
                                BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                642,
                                452,
                                72,
                                24,
                                wnd,
                                reinterpret_cast<HMENU>(kQuitId),
                                instance,
                                nullptr);
    SendMessage(buttonQuit, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(L"quit"));
    SendMessage(buttonQuit, WM_SETFONT, reinterpret_cast<WPARAM>(buttonFont), 0);

    //
    ShowWindow(wnd, SW_SHOWDEFAULT);
    UpdateWindow(wnd);
    SetForegroundWindow(wnd);
    SetFocus(inputBox);

    messageBuffer.reserve(256);

    initialized = true;
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
// Console::GetInput
// --------------------------------
String GetInput()
{
    static String s_returnedText;

    if (inputBuffer[0] == '\0')
        return "";

    s_returnedText = String{inputBuffer};
    inputBuffer[0] = '\0';

    return s_returnedText;
}

// --------------------------------
// Console::Clear
// --------------------------------
void Clear()
{
    SendMessage(editBox, EM_SETSEL, 0, -1);
    SendMessage(editBox, EM_REPLACESEL, 0, reinterpret_cast<LPARAM>(""));
    UpdateWindow(editBox);
}

// --------------------------------
// Console::Show
// --------------------------------
void Show(const VisLevel level, const bool exitOnClose)
{
    quitOnClose = exitOnClose;

    if (!wnd)
        return;

    switch (level) {
    case VisLevel::Hidden:
        ShowWindow(wnd, SW_HIDE);
        break;
    case VisLevel::Normal:
        ShowWindow(wnd, SW_SHOW);
        SendMessage(editBox, EM_LINESCROLL, 0, 0xffff);
        break;
    case VisLevel::Minimized:
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
    for (const auto& m : messageBuffer)
        length += m.Length();

    String message;
    message.Reserve(length);
    for (const auto& m : messageBuffer)
        message += m;

    messageBuffer.clear();

    const char* msg;
    if (message.Length() > kConsoleBufferSize - 1)
        msg = message.c_str() + message.Length() - kConsoleBufferSize + 1;
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
        SendMessage(editBox, EM_SETSEL, 0, -1);
        s_totalChars = bufferLength;
    } else {
        SendMessage(editBox, EM_SETSEL, 0xffff, 0xffff);
    }

    wchar_t wideBuffer[kConsoleBufferSize * 2]{};
    mbtowc(nullptr, nullptr, 0);
    (void) mbstowcs(wideBuffer, buffer, sizeof(wideBuffer));

    SendMessage(editBox, EM_LINESCROLL, 0, 0xffff);
    SendMessage(editBox, EM_SCROLLCARET, 0, 0);
    SendMessage(editBox, EM_REPLACESEL, 0, reinterpret_cast<LPARAM>(wideBuffer));

    messagesWaiting = 0;
}

// --------------------------------
// Console::DebugPrint
// --------------------------------
void DebugPrint(const String& message)
{
    wchar_t outputBuffer[4096]{};
    message.ToWideString(outputBuffer, sizeof(outputBuffer));
    OutputDebugString(outputBuffer);
}

// --------------------------------
// Console::DisplayError
// --------------------------------
void DisplayError(const String& message)
{
    message.ToWideString(errorMsg, sizeof(errorMsg));

    if (!errorBox) {
        errorBox = CreateWindowEx(0,
                                  L"static",
                                  nullptr,
                                  WS_CHILD | WS_VISIBLE | SS_SUNKEN,
                                  6,
                                  5,
                                  708,
                                  30,
                                  wnd,
                                  reinterpret_cast<HMENU>(kErrorBoxId),
                                  instance,
                                  nullptr);
        SendMessage(errorBox, WM_SETFONT, reinterpret_cast<WPARAM>(errorFont), true);
        SetWindowText(errorBox, errorMsg);
        DestroyWindow(inputBox);
        inputBox = nullptr;
    }
}

void WaitForQuit()
{
    MSG msg{};

    while (true) {
        if (!GetMessage(&msg, nullptr, 0, 0))
            break;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

} // namespace iocod
