// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PlatformWindows.h"

#include <direct.h>
#include <shellapi.h>
#include <Platform/SharedLibrary.h>
#include <Platform/Console.h>

namespace iocod {

Platform& Platform::GetInstance()
{
    static PlatformWindows s_platform;
    return s_platform;
}

// --------------------------------
// PlatformWindows::GetLastErrorAsString
// --------------------------------
String PlatformWindows::GetLastErrorAsString()
{
    const DWORD error = GetLastError();
    if (error == ERROR_SUCCESS)
        return String{};

    LPSTR buffer = nullptr;
    const size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                                      FORMAT_MESSAGE_IGNORE_INSERTS,
                                      nullptr,
                                      error,
                                      LANG_USER_DEFAULT,
                                      reinterpret_cast<LPSTR>(&buffer),
                                      0,
                                      nullptr);

    String message{buffer, size};
    LocalFree(buffer);

    message += String{"(error code: "} + static_cast<int>(error) + ")";
    return message;
}

// --------------------------------
// PlatformWindows::DisplayFatalErrorAndExit
// --------------------------------
[[noreturn]] void PlatformWindows::DisplayFatalErrorAndExit(const String& errorMessage, const SourceLocation& loc)
{
    const String message = errorMessage + " in " + loc.FunctionName() + "() at " + loc.FileName() + ":" + loc.Line();
    MessageBox(nullptr, message.c_str(), "Fatal Error", MB_OK | MB_ICONERROR);
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
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

// --------------------------------
// PlatformWindows::Initialize
// --------------------------------
void PlatformWindows::Initialize(const String& appName)
{
    Strcpy(m_appName, appName.c_str());
    ShowMessageBox("GetACP", eastl::to_string(GetACP()));

    if (!GetModuleHandleEx(0, nullptr, &m_instance))
        DisplayFatalErrorAndExit("GetModuleHandleEx: " + GetLastErrorAsString());

    const HICON icon = LoadIcon(m_instance, IDI_APPLICATION);

    WNDCLASSEX wc{};
    wc.cbSize = sizeof(wc);
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = ProcessWindowMessage;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_instance;
    wc.hIcon = icon;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = "iocod_platform";

    if (!RegisterClassEx(&wc))
        DisplayFatalErrorAndExit("RegisterClassEx: " + GetLastErrorAsString());

    constexpr DWORD style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZE | WS_THICKFRAME;
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
void* PlatformWindows::DllOpen(const String& path)
{
    char pathBuffer[MAX_PATH]{};
    Strncpy(pathBuffer, path.c_str(), sizeof(pathBuffer) - 1);

    return LoadLibrary(pathBuffer);
}

// --------------------------------
// PlatformWindows::LibraryLoadSymbol
// --------------------------------
void* PlatformWindows::DllLoadSymbol(void* library, const String& symbolName)
{
    if (!library)
        return nullptr;

    return GetProcAddress(static_cast<HMODULE>(library), symbolName.c_str());
}

// --------------------------------
// PlatformWindows::LibraryClose
// --------------------------------
void PlatformWindows::DllClose(void* library)
{
    if (!library)
        return;

    FreeLibrary(static_cast<HMODULE>(library));
}

// --------------------------------
// PlatformWindows::GetCurrentUser
// --------------------------------
String PlatformWindows::GetCurrentUser()
{
    char buffer[256]{};
    DWORD size = sizeof(buffer);
    if (!GetUserName(buffer, &size))
        return String{"player"};

    return String{buffer};
}

// --------------------------------
// PlatformWindows::StartProcess
// --------------------------------
void PlatformWindows::StartProcess(const String& path, const bool doExit)
{
    const String newPath = CurrentDirectory() + "/" + path;

    char pathBuffer[MAX_PATH]{};
    Strncpy(pathBuffer, newPath.c_str(), sizeof(pathBuffer) - 1);

    STARTUPINFO startupInfo{};
    startupInfo.cb = sizeof(startupInfo);

    PROCESS_INFORMATION processInfo{};
    if (!CreateProcess(nullptr,
                       pathBuffer,
                       nullptr,
                       nullptr,
                       FALSE,
                       0,
                       nullptr,
                       nullptr,
                       &startupInfo,
                       &processInfo)) {
        //Log::Error("Failed to start process: %s", GetLastErrorAsString().c_str());
        return;
    }

    if (doExit)
        ExitProcess(0);
}

// --------------------------------
// PlatformWindows::OpenUrl
// --------------------------------
void PlatformWindows::OpenUrl(const String& url, const bool doExit)
{
    char urlBuffer[MAX_PATH]{};
    Strncpy(urlBuffer, url.c_str(), sizeof(urlBuffer) - 1);

    SHELLEXECUTEINFO info{};
    info.cbSize = sizeof(info);
    info.fMask = SEE_MASK_NOCLOSEPROCESS;
    info.hwnd = nullptr;
    info.lpVerb = "open";
    info.lpFile = urlBuffer;
    info.lpParameters = nullptr;
    info.lpDirectory = nullptr;
    info.nShow = SW_RESTORE;

    if (!ShellExecuteEx(&info)) {
        //Log::Error("Failed to open URL: %s", GetLastErrorAsString().c_str());
        return;
    }

    if (const HWND wnd = GetForegroundWindow(); wnd)
        ShowWindow(wnd, SW_MAXIMIZE);

    if (doExit)
        ExitProcess(0);
}

// --------------------------------
// PlatformWindows::MakeDirectory
// --------------------------------
void PlatformWindows::MakeDirectory(const String& path)
{
    char pathBuffer[MAX_PATH]{};
    Strncpy(pathBuffer, path.c_str(), sizeof(pathBuffer) - 1);
    CreateDirectory(pathBuffer, nullptr);
}

// --------------------------------
// PlatformWindows::CurrentDirectory
// --------------------------------
String PlatformWindows::CurrentDirectory()
{
    char buffer[MAX_PATH]{};
    GetCurrentDirectory(sizeof(buffer), buffer);
    return String{buffer};
}

// --------------------------------
// PlatformWindows::DefaultBasePath
// --------------------------------
String PlatformWindows::DefaultBasePath()
{
    return CurrentDirectory();
}

// --------------------------------
// PlatformWindows::GetClipboardText
// --------------------------------
String PlatformWindows::GetClipboardText()
{
    if (!OpenClipboard(nullptr))
        return String{};

    const HANDLE handle = GetClipboardData(CF_UNICODETEXT);
    if (!handle) {
        CloseClipboard();
        return String{};
    }

    const auto text = static_cast<wchar_t*>(GlobalLock(handle));
    if (!text) {
        CloseClipboard();
        return String{};
    }

    //String result = eastl::to_string(text);
    GlobalUnlock(handle);
    CloseClipboard();

    return String{};
}

// --------------------------------
// PlatformWindows::GetCpuInfo
// --------------------------------
CpuInfo PlatformWindows::GetCpuInfo()
{
    static CpuInfo s_cpuInfo;
    static bool s_initialized = false;

    if (s_initialized)
        return s_cpuInfo;

    //
    // processor cores/threads
    //

    u64 cpuCores = 0;
    u64 cpuThreads = 0;
    u64 cpuCacheL1 = 0;
    u64 cpuCacheL2 = 0;
    u64 cpuCacheL3 = 0;

    using GetLogicalProcessorInformationPtr = BOOL(WINAPI*)(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, PDWORD);
    SharedLibrary kernel32{"kernel32.dll"};
    if (const auto getLogicalProcessorInformation = kernel32.LoadSymbol<GetLogicalProcessorInformationPtr>(
        "GetLogicalProcessorInformation"); getLogicalProcessorInformation) {
        PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = nullptr;
        DWORD returnLength = 0;

        while (true) {
            if (const DWORD rc = getLogicalProcessorInformation(buffer, &returnLength); !rc) {
                if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
                    if (buffer)
                        free(buffer);

                    buffer = static_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION>(malloc(returnLength));
                    if (!buffer)
                        return {};
                } else {
                    return {};
                }
            } else {
                break;
            }
        }

        auto countSetBits = [](const ULONG_PTR bitMask) {
            constexpr DWORD leftShift = sizeof(ULONG_PTR) * 8 - 1;
            DWORD bitSetCount = 0;
            auto bitTest = static_cast<ULONG_PTR>(1) << leftShift;

            for (DWORD i = 0; i <= leftShift; ++i) {
                bitSetCount += ((bitMask & bitTest) ? 1 : 0);
                bitTest /= 2;
            }

            return bitSetCount;
        };

        if (buffer) {
            PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = buffer;
            DWORD byteOffset = 0;
            while (byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= returnLength) {
                switch (ptr->Relationship) {
                case RelationProcessorCore:
                    cpuCores++;
                    cpuThreads += countSetBits(ptr->ProcessorMask);
                    break;
                case RelationCache:
                    if (ptr->Cache.Level == 1)
                        cpuCacheL1 += ptr->Cache.Size;
                    else if (ptr->Cache.Level == 2)
                        cpuCacheL2 += ptr->Cache.Size;
                    else if (ptr->Cache.Level == 3)
                        cpuCacheL3 += ptr->Cache.Size;
                    break;
                default:
                    break;
                }

                byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
                ++ptr;
            }
        }

        free(buffer);
    }

    //
    // cpu speed
    //

    const HANDLE threadInfo = GetCurrentThread();
    const int priority = GetThreadPriority(threadInfo);
    SetThreadPriority(threadInfo, 15);

    LARGE_INTEGER count{};
    LARGE_INTEGER wait{};
    LARGE_INTEGER current{};
    QueryPerformanceCounter(&count);
    QueryPerformanceFrequency(&wait);
    wait.QuadPart >>= 5;

    const u64 start = __rdtsc();
    do {
        QueryPerformanceCounter(&current);
    } while (current.QuadPart - count.QuadPart < wait.QuadPart);

    SetThreadPriority(threadInfo, priority);
    const auto cpuMhz = static_cast<f64>((__rdtsc() - start) << 5) / 1000000.0f;

    //
    // processor name / vendor
    //

    char modelBuffer[64]{};
    char vendorBuffer[48]{};

    HKEY key;
    if (!RegOpenKey(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", &key)) {
        DWORD size;
        RegGetValue(key, nullptr, "ProcessorNameString", RRF_RT_REG_SZ, nullptr, modelBuffer, &size);
        RegGetValue(key, nullptr, "VendorIdentifier", RRF_RT_REG_SZ, nullptr, vendorBuffer, &size);
        RegCloseKey(key);
    }

    s_initialized = true;
    s_cpuInfo = {cpuCores,
                 cpuThreads,
                 cpuCacheL1,
                 cpuCacheL2,
                 cpuCacheL3,
                 cpuMhz,
                 String{modelBuffer},
                 String{vendorBuffer}};
    return s_cpuInfo;
}

// --------------------------------
// PlatformWindows::GetCpuInfoString
// --------------------------------
String PlatformWindows::GetCpuInfoString()
{
    const auto [cores, threads, cacheL1, cacheL2, cacheL3, mhz, vendor, model] = GetCpuInfo();
    return {
        String{"CpuInfo:\n"} +
        "Vendor: " + vendor + "\n" +
        "Model: " + model + "\n" +
        "Cores: " + eastl::to_string(cores) + "\n" +
        "Threads: " + eastl::to_string(threads) + "\n" +
        "Speed: " + eastl::to_string(mhz) + " MHz\n" +
        "L1 Cache: " + eastl::to_string(BytesToKB(cacheL1)) + " KB\n" +
        "L2 Cache: " + eastl::to_string(BytesToKB(cacheL2)) + " KB\n" +
        "L3 Cache:" + eastl::to_string(BytesToKB(cacheL3)) + " KB\n"};
}

// --------------------------------
// PlatformWindows::GetMemoryInfo
// --------------------------------
MemoryInfo PlatformWindows::GetMemoryInfo()
{
    MemoryInfo result;

    MEMORYSTATUSEX memoryStatus{};
    memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memoryStatus);

    result.totalPhysical = memoryStatus.ullTotalPhys;
    result.availablePhysical = memoryStatus.ullAvailPhys;
    result.totalVirtual = memoryStatus.ullTotalVirtual;
    result.availableVirtual = memoryStatus.ullAvailVirtual;
    result.totalPageFile = memoryStatus.ullTotalPageFile;
    result.availablePageFile = memoryStatus.ullAvailPageFile;

    return result;
}

// --------------------------------
// PlatformWindows::GetMemoryInfoString
// --------------------------------
String PlatformWindows::GetMemoryInfoString()
{
    const auto [totalPhys, availPhys, totalVirt, availVirt, totalPage, availPage] = GetMemoryInfo();
    return {
        String{"MemoryInfo:\n"} +
        "Physical: " + eastl::to_string(BytesToMB(availPhys)) + " MB / " + eastl::to_string(BytesToMB(totalPhys)) + " MB\n" +
        "Virtual: " + eastl::to_string(BytesToMB(availVirt)) + " MB / " + eastl::to_string(BytesToMB(totalVirt)) + " MB\n" +
        "PageFile: " + eastl::to_string(BytesToMB(availPage)) + " MB / " + eastl::to_string(BytesToMB(totalPage)) + " MB\n"
    };
}

// --------------------------------
// PlatformWindows::Print
// --------------------------------
void PlatformWindows::Print(const String& message)
{
    Console::Print(message);
}

void PlatformWindows::ShowMessageBox(const String& title, const String& message, const MessageBoxType type)
{
    DWORD flags = 0;
     
    switch (type) {
    case MessageBoxType::Ok:
        flags = MB_OK;
        break;
    case MessageBoxType::Info:
        flags = MB_OK | MB_ICONINFORMATION;
        break;
    case MessageBoxType::Warning:
        flags = MB_OK | MB_ICONWARNING;
        break;
    case MessageBoxType::Question:
        flags = MB_OK | MB_ICONQUESTION;
        break;
    case MessageBoxType::Error:
        flags = MB_OK | MB_ICONERROR;
        break;
    }

    MessageBox(nullptr, message.c_str(), title.c_str(), flags);
}

// --------------------------------
// PlatformWindows::PumpEvents
// --------------------------------
void PlatformWindows::PumpEvents()
{
    MSG msg{};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE)) {
        if (!GetMessage(&msg, nullptr, 0, 0))
            ExitProcess(1);

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

} // namespace iocod
