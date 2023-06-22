// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "SystemInfo.h"

#include <array>
#include <Core/System.h>
#include <Core/Types.h>

#ifdef IOCOD_OS_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #include <Core/SharedLibrary.h>
    #include <intrin.h>
    #include <windows.h>
#else
    #include <cstdio>
    #include <fstream>
    #include <sstream>
    #include <string>
#endif

namespace iocod {

namespace {

#ifdef IOCOD_OS_WINDOWS
void GetCPUInfo(SystemInfo& info)
{
    SharedLibrary kernel32("kernel32.dll");
    if (!kernel32.Loaded()) {
        LogWarn("Failed to load kernel32.dll: {}", kernel32.GetLastErrorMessage());
        return;
    }

    using SystemInfoPtr = void(WINAPI*)(SYSTEM_INFO*);
    const auto getSystemInfo = kernel32.LoadSymbol<SystemInfoPtr>("GetSystemInfo");
    if (getSystemInfo == nullptr) {
        LogWarn("Failed to load GetSystemInfo: {}", kernel32.GetLastErrorMessage());
        return;
    }

    SYSTEM_INFO systemInfo{};
    getSystemInfo(&systemInfo);

    // TODO: add support for ARM

    std::array<int, 4> cpui{};
    __cpuid(cpui.data(), 0);
    int nids = cpui[0];

    std::vector<std::array<int, 4>> data;
    for (int i = 0; i <= nids; ++i) {
        __cpuidex(cpui.data(), i, 0);
        data.push_back(cpui);
    }

    // cores/threads
    __cpuid(cpui.data(), 1);
    info.cpuThreads = (cpui[1] >> 16) & 0xff;
    info.cpuCores = info.cpuThreads;

    // vendor
    char vendor[0x20]{};
    *reinterpret_cast<int*>(vendor) = data[0][1];
    *reinterpret_cast<int*>(vendor + 4) = data[0][3];
    *reinterpret_cast<int*>(vendor + 8) = data[0][2];
    info.cpuVendor = vendor;

    // core/thread count is INCORRECT ??
    if (info.cpuVendor == "GenuineIntel") {
        __cpuid(cpui.data(), 4);
        info.cpuCores = ((cpui[0] >> 26) & 0x3f) + 1;
    } else if (info.cpuVendor == "AuthenticAMD") {
        __cpuid(cpui.data(), 0x80000008);
        info.cpuCores = (cpui[2] & 0xff) + 1;
    }

    // brand info
    __cpuid(cpui.data(), 0x80000000);
    int nexids = cpui[0];

    char brand[0x40]{};

    std::vector<std::array<int, 4>> exdata;
    for (int i = 0x80000000; i <= nexids; ++i) {
        __cpuidex(cpui.data(), i, 0);
        exdata.push_back(cpui);
    }

    if (nexids >= 0x80000004) {
        memcpy(brand, exdata[2].data(), sizeof(cpui));
        memcpy(brand + 16, exdata[3].data(), sizeof(cpui));
        memcpy(brand + 32, exdata[4].data(), sizeof(cpui));
        info.cpuModel = brand;
    }

    // benchmark freq
    HANDLE thread_info = GetCurrentThread();
    int priority = GetThreadPriority(thread_info);
    SetThreadPriority(thread_info, 15);

    LARGE_INTEGER count{};
    LARGE_INTEGER wait{};
    LARGE_INTEGER current{};
    QueryPerformanceCounter(&count);
    QueryPerformanceFrequency(&wait);
    wait.QuadPart >>= 5;

    u64 start = __rdtsc();
    do {
        QueryPerformanceCounter(&current);
    } while (current.QuadPart - count.QuadPart < wait.QuadPart);

    SetThreadPriority(thread_info, priority);
    info.cpuMhz = static_cast<f32>(static_cast<f32>((__rdtsc() - start) << 5) / 1000000.0f);
}

void GetMemoryInfo(SystemInfo& info)
{
    SharedLibrary kernel32("kernel32.dll");
    if (!kernel32.Loaded()) {
        LogWarn("Failed to load kernel32.dll: {}", kernel32.GetLastErrorMessage());
        return;
    }

    using MemoryStatusPtr = int(WINAPI*)(MEMORYSTATUSEX*);
    const auto memoryStatusEx = kernel32.LoadSymbol<MemoryStatusPtr>("GlobalMemoryStatusEx");
    if (memoryStatusEx == nullptr) {
        LogWarn("Failed to load GlobalMemoryStatusEx: {}", kernel32.GetLastErrorMessage());
        return;
    }

    MEMORYSTATUSEX memoryStatus{};
    memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);
    if (!memoryStatusEx(&memoryStatus)) {
        LogWarn("Failed to call GlobalMemoryStatusEx");
        return;
    }

    info.memTotal = memoryStatus.ullTotalPhys;
    info.memAvailable = memoryStatus.ullAvailPhys;
    info.memFree = memoryStatus.ullTotalPhys - memoryStatus.ullAvailPhys;

    info.memVirtualSize = memoryStatus.ullTotalVirtual;
    info.memVirtualAvailable = memoryStatus.ullAvailVirtual;
}
#else
void GetCPUInfo(SystemInfo& info)
{
    std::ifstream cpuinfo("/proc/cpuinfo");
    if (!cpuinfo.is_open()) {
        // Warning("Failed to open /proc/cpuinfo for reading\n");
        return;
    }

    Stringstream buffer;
    buffer << cpuinfo.rdbuf();
    const String data = buffer.str();

    auto get_data_on_line = [data]<typename Type>(const String& what, Type& out) {
        auto set_out_value = [&](const String& value) {
            if constexpr (std::is_same_v<Type, String>)
                out = value;
            else if constexpr (std::is_same_v<Type, float>)
                out = std::stof(value);
            else if constexpr (std::is_same_v<Type, int>)
                out = std::stoi(value);
        };

        const size_t location = data.find(what);
        if (location == String::npos) {
            if constexpr (std::is_same_v<Type, float> || std::is_same_v<Type, int>)
                set_out_value("0");
            else
                set_out_value("");
            return;
        }

        const size_t start = data.find(':', location) + 2;
        const size_t end = data.find('\n', location);
        set_out_value(data.substr(start, end - start));
    };

    get_data_on_line("vendor_id", info.cpuVendor);
    get_data_on_line("model name", info.cpuModel);
    get_data_on_line("cpu cores", info.cpuCores);
    get_data_on_line("siblings", info.cpuThreads);
    get_data_on_line("cpu MHz", info.cpuMhz);
}

void GetMemoryInfo(SystemInfo& info)
{
    std::ifstream meminfo("/proc/meminfo");
    if (!meminfo.is_open()) {
        // Warning("Couldn't open /proc/meminfo for reading\n");
        return;
    }

    Stringstream buffer;
    buffer << meminfo.rdbuf();
    String data = buffer.str();

    auto get_value = [](const String& data_, const String& what) {
        const size_t location = data_.find(what);
        if (location == String::npos)
            return 0;

        const size_t start = data_.find(':', location) + 1;
        const size_t end = data_.find("kB", location);
        return std::stoi(data_.substr(start, end - start));
    };

    info.memTotal = get_value(data, "MemTotal");
    info.memFree = get_value(data, "MemFree");
    info.memAvailable = get_value(data, "MemAvailable");

    std::ifstream procmem("/proc/self/status");
    if (!procmem.is_open()) {
        // Warning("Couldn't open /proc/self/status for reading\n");
        return;
    }

    buffer.clear();
    data.clear();
    buffer << procmem.rdbuf();
    data = buffer.str();

    info.memVirtualSize = get_value(data, "VmSize");
    info.memVirtualPeak = get_value(data, "VmPeak");
    info.memPhysicalSize = get_value(data, "VmRSS");
    info.memPhysicalPeak = get_value(data, "VMHWM");
}
#endif

} // namespace

SystemInfo PopulateSystemInfo()
{
    SystemInfo info{};

    GetCPUInfo(info);
    GetMemoryInfo(info);

    return info;
}

} // namespace iocod
