// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <core/command_system.h>
#include <core/console_command.h>
#include <core/system.h>

#include <chrono>

#include "console.h"
#include "system_info.h"

namespace iocod {

class SystemLocal final : public ISystem {
public:
    void Initialize() override;
    void Shutdown() noexcept override;

    u64 Milliseconds() noexcept override;

    std::string GetConsoleInput() noexcept override;
    void ClearConsole() noexcept override;

    void Print(const std::string& message) noexcept override;
    void DebugPrint(const std::string& message) noexcept override;
    void Warning(const std::string& message) noexcept override;
    void Error(const std::string& message) noexcept override;

    SystemInfo GetSystemInfo() override;
    void PrintSystemInfo() override;

    void AddConsoleCommands() noexcept override;

private:
    using Clock = std::chrono::system_clock;
    std::chrono::time_point<Clock> start_time = Clock::now();

    SystemInfo system_info;
    Console console;
};

SystemLocal local;
ISystem* g_system = &local;

// --------------------------------
// SystemLocal::Initialize
// --------------------------------
void SystemLocal::Initialize()
{
    // TODO: backtrace, etc

    auto [result, errmsg] = console.Initialize();
    if (!result)
        Error("Failed to initialize console: " + errmsg);

    system_info = GetSystemInfo();
}

// --------------------------------
// SystemLocal::Shutdown
// --------------------------------
void SystemLocal::Shutdown() noexcept
{
    console.Shutdown();
}

// --------------------------------
// SystemLocal::Milliseconds
// --------------------------------
u64 SystemLocal::Milliseconds() noexcept
{
    return static_cast<u64>(
        std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start_time).count());
}

// --------------------------------
// SystemLocal::GetConsoleInput
// --------------------------------
std::string SystemLocal::GetConsoleInput() noexcept
{
    return console.GetInput();
}

// --------------------------------
// SystemLocal::ClearConsole
// --------------------------------
void SystemLocal::ClearConsole() noexcept
{
    console.Clear();
}

// --------------------------------
// SystemLocal::Print
// --------------------------------
void SystemLocal::Print(const std::string& message) noexcept
{
    console.Print(message);
}

// --------------------------------
// SystemLocal::DebugPrint
// --------------------------------
void SystemLocal::DebugPrint(const std::string& message) noexcept
{
    console.DebugPrint(message);
}

// --------------------------------
// SystemLocal::Warning
// --------------------------------
void SystemLocal::Warning(const std::string& message) noexcept
{
    console.WarningPrint(message);
}

// --------------------------------
// SystemLocal::Error
// --------------------------------
void SystemLocal::Error(const std::string& message) noexcept
{
    console.ErrorPrint(message);
    // TODO: exit
}

// --------------------------------
// SystemLocal::GetSystemInfo
// --------------------------------
SystemInfo SystemLocal::GetSystemInfo()
{
    return system_info;
}

// --------------------------------
// SystemLocal::PrintSystemInfo
// --------------------------------
void SystemLocal::PrintSystemInfo()
{
    std::string output = "System Info:\n";

    if (!system_info.cpu_vendor.empty())
        output += "CPU Vendor: " + std::string(system_info.cpu_vendor) + "\n";
    if (!system_info.cpu_model.empty())
        output += "CPU Model: " + std::string(system_info.cpu_model) + "\n";
    if (system_info.cpu_cores != 0)
        output += "CPU Cores: " + std::to_string(system_info.cpu_cores) + "\n";
    if (system_info.cpu_threads != 0)
        output += "CPU Threads: " + std::to_string(system_info.cpu_threads) + "\n";
    if (system_info.cpu_mhz != 0.0f)
        output += "CPU MHz: " + std::to_string(system_info.cpu_mhz) + "\n";

    if (system_info.mem_total != 0)
        output += "Memory Total: " + std::to_string(system_info.mem_total) + " kB\n";
    if (system_info.mem_free != 0)
        output += "Memory Free: " + std::to_string(system_info.mem_free) + " kB\n";
    if (system_info.mem_available != 0)
        output += "Memory Available: " + std::to_string(system_info.mem_available) + " kB\n";
    if (system_info.mem_virtual_size != 0)
        output += "Virtual Memory Size: " + std::to_string(system_info.mem_virtual_size) + " kB\n";
    if (system_info.mem_virtual_available != 0)
        output += "Virtual Memory Available: " + std::to_string(system_info.mem_virtual_available) +
                  " kB\n";
    if (system_info.mem_virtual_peak != 0)
        output += "Virtual Memory Peak: " + std::to_string(system_info.mem_virtual_peak) + " kB\n";
    if (system_info.mem_physical_size != 0)
        output +=
            "Physical Memory Size: " + std::to_string(system_info.mem_physical_size) + " kB\n";
    if (system_info.mem_physical_peak != 0)
        output +=
            "Physical Memory Peak: " + std::to_string(system_info.mem_physical_peak) + " kB\n";

    Print(output);
}

// ================================
// commands
// ================================

class Command_echo final : public IConsoleCommand {
public:
    void Execute(std::vector<std::string> args)
    {
        std::string output;
        for (const auto& arg : args)
            output += arg + " ";

        g_system->Print(output + "\n");
    }
};

class Command_clear final : public IConsoleCommand {
public:
    void Execute(std::vector<std::string> args)
    {
        g_system->ClearConsole();
    }
};

class Command_quit final : public IConsoleCommand {
public:
    void Execute(std::vector<std::string> args)
    {
        g_system->Print("See ya!\n");
        exit(0);
    }
};

class Command_sysinfo final : public IConsoleCommand {
private:
    void Execute(std::vector<std::string> args)
    {
        g_system->PrintSystemInfo();
    }
};

// --------------------------------
// SystemLocal::AddConsoleCommands
// --------------------------------
void SystemLocal::AddConsoleCommands() noexcept
{
    g_commandSystem->AddCommand("echo", std::make_unique<Command_echo>());
    g_commandSystem->AddCommand("clear", std::make_unique<Command_clear>());
    g_commandSystem->AddCommand("quit", std::make_unique<Command_quit>());
    g_commandSystem->AddCommand("sysinfo", std::make_unique<Command_sysinfo>());
}

} // namespace iocod
