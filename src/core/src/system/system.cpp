// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <core/command_system.h>
#include <core/console_command.h>
#include <core/system.h>
#include <core/types.h>
#include <fmt/core.h>
#include <spdlog/async.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <chrono>

#include "console.h"
#include "system_info.h"

namespace iocod {

std::shared_ptr<spdlog::logger> logger;

void ISystem::LogTrace(const String& message)
{
    logger->trace(message);
}

void ISystem::LogDebug(const String& message)
{
    logger->debug(message);
}

void ISystem::LogInfo(const String& message)
{
    logger->info(message);
}

void ISystem::LogWarn(const String& message)
{
    logger->warn(message);
}

void ISystem::LogError(const String& message)
{
    logger->error(message);
    std::exit(1);
}

class SystemLocal final : public ISystem {
public:
    void Initialize() override;
    void Shutdown() noexcept override;

    u64 Milliseconds() noexcept override;

    String GetConsoleInput() noexcept override;
    void ClearConsole() noexcept override;

    void Print(const String& message) noexcept override;
    void DebugPrint(const String& message) noexcept override;
    void Warning(const String& message) noexcept override;
    void Error(const String& message) noexcept override;

    SystemInfo GetSystemInfo() override;
    void PrintSystemInfo() override;

    static void AddConsoleCommands() noexcept;

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
    auto [result, errmsg] = console.Initialize();
    if (!result)
        Error("Failed to initialize console: " + errmsg);

    // TODO: backtrace, etc
    //
    // spdlog
    //
    spdlog::init_thread_pool(8192, 1);

    // stdout_color_sink_mt is an alias for either
    // - wincolor_stdout_sink_mt (on windows)
    // - ansicolor_stdout_sink_mt (on unix)
    // but it has different set_color behavior on windows vs unix
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
#ifdef _WIN32
    console_sink->set_color(spdlog::level::trace, 5);
    console_sink->set_color(spdlog::level::info, 7);
#else
    console_sink->set_color(spdlog::level::trace, console_sink->magenta);
    console_sink->set_color(spdlog::level::info, console_sink->white);
#endif

    console_sink->set_level(spdlog::level::trace);
    console_sink->set_pattern("%^%v%$");

    spdlog::file_event_handlers handlers;
    handlers.after_open = [](spdlog::filename_t, std::FILE* fstream) {
        fputs("--------------------------------------------------------------------------------\n",
              fstream);
    };

    auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("iocod.log", 8_MB, 3,
                                                                            false, handlers);
    file_sink->set_level(spdlog::level::trace);
    file_sink->set_pattern("[%Y-%m-%d %T] [%L] %v");

    std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};
    logger = std::make_shared<spdlog::async_logger>("iocod", sinks.begin(), sinks.end(),
                                                    spdlog::thread_pool(),
                                                    spdlog::async_overflow_policy::block);

    logger->set_level(spdlog::level::trace);
    spdlog::register_logger(logger);
    spdlog::set_default_logger(logger);
    //
    // end spdlog
    //

    system_info = PopulateSystemInfo();

    // register system commands with command system
    ICommandSystem::AddRegisterCallback(AddConsoleCommands);
}

// --------------------------------
// SystemLocal::Shutdown
// --------------------------------
void SystemLocal::Shutdown() noexcept
{
    logger->flush();
    spdlog::drop_all();

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
String SystemLocal::GetConsoleInput() noexcept
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
void SystemLocal::Print(const String& message) noexcept
{
    console.Print(message);
}

// --------------------------------
// SystemLocal::DebugPrint
// --------------------------------
void SystemLocal::DebugPrint(const String& message) noexcept
{
    console.DebugPrint(message);
}

// --------------------------------
// SystemLocal::Warning
// --------------------------------
void SystemLocal::Warning(const String& message) noexcept
{
    console.WarningPrint(message);
}

// --------------------------------
// SystemLocal::Error
// --------------------------------
void SystemLocal::Error(const String& message) noexcept
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
    String output = "System Info:\n";

    auto human_readable = [](u64 bytes) {
        double size = static_cast<double>(bytes);
        constexpr u64 KB = 1024;
        constexpr u64 MB = KB * 1024;
        constexpr u64 GB = MB * 1024;
        constexpr u64 TB = GB * 1024;

        if (size < KB)
            return fmt::format("{} B", size);
        else if (size < MB)
            return fmt::format("{:g} KB", static_cast<double>(size / KB));
        else if (size < GB)
            return fmt::format("{:g} MB", static_cast<double>(size / MB));
        else if (size < TB)
            return fmt::format("{:g} GB", static_cast<double>(size / GB));
        else
            return fmt::format("{:g} TB", static_cast<double>(size / TB));
    };

    if (!system_info.cpu_vendor.empty())
        output += "CPU Vendor: " + system_info.cpu_vendor + "\n";
    if (!system_info.cpu_model.empty())
        output += "CPU Model: " + system_info.cpu_model + "\n";
    if (system_info.cpu_cores != 0)
        output += "CPU Cores: " + std::to_string(system_info.cpu_cores) + "\n";
    if (system_info.cpu_threads != 0)
        output += "CPU Threads: " + std::to_string(system_info.cpu_threads) + "\n";
    if (system_info.cpu_mhz != 0.0f)
        output += "CPU MHz: " + std::to_string(system_info.cpu_mhz) + "\n";

    if (system_info.mem_total != 0)
        output += "Memory Total: " + human_readable(system_info.mem_total) + "\n";
    if (system_info.mem_free != 0)
        output += "Memory Free: " + human_readable(system_info.mem_free) + "\n";
    if (system_info.mem_available != 0)
        output += "Memory Available: " + human_readable(system_info.mem_available) + "\n";
    if (system_info.mem_virtual_size != 0)
        output += "Virtual Memory Size: " + human_readable(system_info.mem_virtual_size) + "\n";
    if (system_info.mem_virtual_available != 0)
        output +=
            "Virtual Memory Available: " + human_readable(system_info.mem_virtual_available) + "\n";
    if (system_info.mem_virtual_peak != 0)
        output += "Virtual Memory Peak: " + human_readable(system_info.mem_virtual_peak) + "\n";
    if (system_info.mem_physical_size != 0)
        output += "Physical Memory Size: " + human_readable(system_info.mem_physical_size) + "\n";
    if (system_info.mem_physical_peak != 0)
        output += "Physical Memory Peak: " + human_readable(system_info.mem_physical_peak) + "\n";

    Print(output);
}

// ================================
// commands
// ================================

class Command_echo final : public IConsoleCommand {
public:
    void Execute(std::vector<String> args)
    {
        String output;
        for (const auto& arg : args)
            output += arg + " ";

        g_system->Print(output + "\n");
    }
};

class Command_clear final : public IConsoleCommand {
public:
    void Execute(std::vector<String> args)
    {
        g_system->ClearConsole();
    }
};

class Command_quit final : public IConsoleCommand {
public:
    void Execute(std::vector<String> args)
    {
        g_system->Print("See ya!\n");
        exit(0);
    }
};

class Command_sysinfo final : public IConsoleCommand {
private:
    void Execute(std::vector<String> args)
    {
        g_system->PrintSystemInfo();
    }
};

// --------------------------------
// SystemLocal::AddConsoleCommands
// --------------------------------
void SystemLocal::AddConsoleCommands() noexcept
{
    g_command_system->AddCommand("echo", std::make_unique<Command_echo>());
    g_command_system->AddCommand("clear", std::make_unique<Command_clear>());
    g_command_system->AddCommand("quit", std::make_unique<Command_quit>());
    g_command_system->AddCommand("sysinfo", std::make_unique<Command_sysinfo>());
}

} // namespace iocod
