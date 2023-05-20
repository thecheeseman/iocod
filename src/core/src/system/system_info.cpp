// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "system_info.h"

#include <core/utility.h>

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <core/shared_library.h>
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

#ifdef _WIN32
void GetCPUInfo(SystemInfo& info)
{

}

void GetMemoryInfo(SystemInfo& info)
{
    SharedLibrary kernel32("kernel32.dll");
    if (!kernel32.Loaded())
        return;

    using MemoryStatusPtr = int(WINAPI*)(MEMORYSTATUSEX*);
    auto MemoryStatusEx = kernel32.LoadSymbol<MemoryStatusPtr>("GlobalMemoryStatusEx");
    if (MemoryStatusEx == nullptr)
        return;

    MEMORYSTATUSEX memory_status{};
    memory_status.dwLength = sizeof(MEMORYSTATUSEX);
    if (!MemoryStatusEx(&memory_status))
        return;

    info.mem_total = Utility::KB(memory_status.ullTotalPhys);
    info.mem_available = Utility::KB(memory_status.ullAvailPhys);
    info.mem_free = Utility::KB(memory_status.ullTotalPhys - memory_status.ullAvailPhys);

    info.mem_virtual_size = Utility::KB(memory_status.ullTotalVirtual);
    info.mem_virtual_available = Utility::KB(memory_status.ullAvailVirtual);
}
#else
void GetCPUInfo(SystemInfo& info)
{
    std::ifstream cpuinfo("/proc/cpuinfo");
    if (!cpuinfo.is_open()) {
        // Warning("Failed to open /proc/cpuinfo for reading\n");
        return;
    }

    std::stringstream buffer;
    buffer << cpuinfo.rdbuf();
    const std::string data = buffer.str();

    auto get_data_on_line = [data]<typename Type>(const std::string& what, Type& out) {
        auto set_out_value = [&](const std::string& value) {
            if constexpr (std::is_same_v<Type, std::string>)
                out = value;
            else if constexpr (std::is_same_v<Type, float>)
                out = std::stof(value);
            else if constexpr (std::is_same_v<Type, int>)
                out = std::stoi(value);
        };

        const size_t location = data.find(what);
        if (location == std::string::npos) {
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

    get_data_on_line("vendor_id", info.cpu_vendor);
    get_data_on_line("model name", info.cpu_model);
    get_data_on_line("cpu cores", info.cpu_cores);
    get_data_on_line("siblings", info.cpu_threads);
    get_data_on_line("cpu MHz", info.cpu_mhz);
}

void GetMemoryInfo(SystemInfo& info)
{
    std::ifstream meminfo("/proc/meminfo");
    if (!meminfo.is_open()) {
        // Warning("Couldn't open /proc/meminfo for reading\n");
        return;
    }

    std::stringstream buffer;
    buffer << meminfo.rdbuf();
    std::string data = buffer.str();

    auto get_value = [](const std::string& data_, const std::string& what) {
        const size_t location = data_.find(what);
        if (location == std::string::npos)
            return 0;

        const size_t start = data_.find(':', location) + 1;
        const size_t end = data_.find("kB", location);
        return std::stoi(data_.substr(start, end - start));
    };

    info.mem_total = get_value(data, "MemTotal");
    info.mem_free = get_value(data, "MemFree");
    info.mem_available = get_value(data, "MemAvailable");

    std::ifstream procmem("/proc/self/status");
    if (!procmem.is_open()) {
        // Warning("Couldn't open /proc/self/status for reading\n");
        return;
    }

    buffer.clear();
    data.clear();
    buffer << procmem.rdbuf();
    data = buffer.str();

    info.mem_virtual_size = get_value(data, "VmSize");
    info.mem_virtual_peak = get_value(data, "VmPeak");
    info.mem_physical_size = get_value(data, "VmRSS");
    info.mem_physical_peak = get_value(data, "VMHWM");
}
#endif

} // namespace

SystemInfo GetSystemInfo()
{
    SystemInfo info{};

    GetCPUInfo(info);
    GetMemoryInfo(info);

    return info;
}

} // namespace iocod
