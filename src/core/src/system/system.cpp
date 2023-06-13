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
    sys->Exit(1);
}

void ISystem::LogErrorNoExit(const String& message)
{
    logger->error(message);
}

class SystemLocal final : public ISystem {
public:
    void Initialize(void* handle) override;
    void Shutdown() noexcept override;

    u64 Milliseconds() noexcept override;

    String GetConsoleInput() noexcept override;
    void ClearConsole() noexcept override;

    void Print(const String& message) noexcept override;
    void DebugPrint(const String& message) noexcept override;
    void Error(const String& message) noexcept override;
    void Exit(int errorCode) noexcept override;

    SystemInfo GetSystemInfo() override;
    void PrintSystemInfo() override;

    void PumpEvents() noexcept override;

    static void AddConsoleCommands() noexcept;

private:
    using Clock = std::chrono::system_clock;
    std::chrono::time_point<Clock> start_time = Clock::now();

    SystemInfo systemInfo_{};
};

SystemLocal local;
ISystem* sys = &local;

// --------------------------------
// SystemLocal::Initialize
// --------------------------------
void SystemLocal::Initialize(void* handle)
{
    auto [result, errmsg] = Console::Initialize(handle);
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

    systemInfo_ = PopulateSystemInfo();

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

    Console::Shutdown();
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
    return Console::GetInput();
}

// --------------------------------
// SystemLocal::ClearConsole
// --------------------------------
void SystemLocal::ClearConsole() noexcept
{
    Console::Clear();
}

// --------------------------------
// SystemLocal::Print
// --------------------------------
void SystemLocal::Print(const String& message) noexcept
{
    if (Console::Initialized())
        Console::Print(message);
    else
        fprintf(stderr, "%s", message.c_str());
}

// --------------------------------
// SystemLocal::DebugPrint
// --------------------------------
void SystemLocal::DebugPrint(const String& message) noexcept
{
    if (Console::Initialized())
        Console::DebugPrint(message);
    else
        fprintf(stderr, "%s", message.c_str());
}

// --------------------------------
// SystemLocal::Error
// --------------------------------
void SystemLocal::Error(const String& message) noexcept
{
    if (Console::Initialized())
        Console::Print(message);
    else
        (void) fprintf(stderr, "%s", message.c_str());

    Exit(1);
}

// --------------------------------
// SystemLocal::Exit
// --------------------------------
void SystemLocal::Exit(const int errorCode) noexcept
{
    Console::Shutdown();
    std::exit(errorCode);
}

// --------------------------------
// SystemLocal::GetSystemInfo
// --------------------------------
SystemInfo SystemLocal::GetSystemInfo()
{
    return systemInfo_;
}

// --------------------------------
// SystemLocal::PrintSystemInfo
// --------------------------------
void SystemLocal::PrintSystemInfo()
{
    String output = "System Info:\n";

    auto human_readable = [](u64 bytes) {
        double size = static_cast<double>(bytes);

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

    if (!systemInfo_.cpuVendor.empty())
        output += "CPU Vendor: " + systemInfo_.cpuVendor + "\n";
    if (!systemInfo_.cpuModel.empty())
        output += "CPU Model: " + systemInfo_.cpuModel + "\n";
    if (systemInfo_.cpuCores != 0)
        output += "CPU Cores: " + std::to_string(systemInfo_.cpuCores) + "\n";
    if (systemInfo_.cpuThreads != 0)
        output += "CPU Threads: " + std::to_string(systemInfo_.cpuThreads) + "\n";
    if (systemInfo_.cpuMhz != 0.0f)
        output += "CPU MHz: " + std::to_string(systemInfo_.cpuMhz) + "\n";

    if (systemInfo_.memTotal != 0)
        output += "Memory Total: " + human_readable(systemInfo_.memTotal) + "\n";
    if (systemInfo_.memFree != 0)
        output += "Memory Free: " + human_readable(systemInfo_.memFree) + "\n";
    if (systemInfo_.memAvailable != 0)
        output += "Memory Available: " + human_readable(systemInfo_.memAvailable) + "\n";
    if (systemInfo_.memVirtualSize != 0)
        output += "Virtual Memory Size: " + human_readable(systemInfo_.memVirtualSize) + "\n";
    if (systemInfo_.memVirtualAvailable != 0)
        output +=
            "Virtual Memory Available: " + human_readable(systemInfo_.memVirtualAvailable) + "\n";
    if (systemInfo_.memVirtualPeak != 0)
        output += "Virtual Memory Peak: " + human_readable(systemInfo_.memVirtualPeak) + "\n";
    if (systemInfo_.memPhysicalSize != 0)
        output += "Physical Memory Size: " + human_readable(systemInfo_.memPhysicalSize) + "\n";
    if (systemInfo_.memPhysicalPeak != 0)
        output += "Physical Memory Peak: " + human_readable(systemInfo_.memPhysicalPeak) + "\n";

    Print(output);
}

// --------------------------------
// SystemLocal::PumpEvents
// --------------------------------
void SystemLocal::PumpEvents() noexcept
{
    Console::PumpEvents();
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

        sys->Print(output + "\n");
    }
};

class Command_clear final : public IConsoleCommand {
public:
    void Execute(std::vector<String> args)
    {
        sys->ClearConsole();
    }
};

class Command_quit final : public IConsoleCommand {
public:
    void Execute(std::vector<String> args)
    {
        sys->Print("See ya!\n");
        exit(0);
    }
};

class Command_sysinfo final : public IConsoleCommand {
private:
    void Execute(std::vector<String> args)
    {
        sys->PrintSystemInfo();
    }
};

// --------------------------------
// SystemLocal::AddConsoleCommands
// --------------------------------
void SystemLocal::AddConsoleCommands() noexcept
{
    command_system->AddCommand("echo", std::make_unique<Command_echo>());
    command_system->AddCommand("clear", std::make_unique<Command_clear>());
    command_system->AddCommand("quit", std::make_unique<Command_quit>());
    command_system->AddCommand("sysinfo", std::make_unique<Command_sysinfo>());
}

} // namespace iocod
