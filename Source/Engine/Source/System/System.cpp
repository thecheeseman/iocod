// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <chrono>
#include <Core/CommandSystem.h>
#include <Core/ConsoleCommand.h>
#include <Core/System.h>
#include <fmt/core.h>
#include <spdlog/async.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "Console.h"
#include "SystemInfo.h"

namespace iocod {

std::shared_ptr<spdlog::logger> logger;

void ISystem::LogTrace(const String message)
{
    logger->trace(message);
}

void ISystem::LogDebug(const String message)
{
    logger->debug(message);
}

void ISystem::LogInfo(const String message)
{
    logger->info(message);
}

void ISystem::LogWarn(const String message)
{
    logger->warn(message);
}

void ISystem::LogError(const String message)
{
    logger->error(message);
    sys->Exit(1);
}

void ISystem::LogErrorNoExit(const String message)
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
    void FlushConsole() override;

    void Print(const String& message) noexcept override;
    void DebugPrint(const String& message) noexcept override;
    void Error(const String& message) noexcept override;
    void Exit(int errorCode) noexcept override;

    [[nodiscard]] const SystemInfo& GetSystemInfo() const override;
    void PrintSystemInfo() override;

    void PumpEvents() noexcept override;

    static void AddConsoleCommands() noexcept;

private:
    using Clock = std::chrono::system_clock;
    std::chrono::time_point<Clock> m_startTime = Clock::now();

    SystemInfo m_systemInfo{};
};

SystemLocal local;
ISystem* sys = &local;

// --------------------------------
// SystemLocal::Initialize
// --------------------------------
void SystemLocal::Initialize(void* handle)
{
    if (auto [result, errorString] = Console::Initialize(handle); !result)
        Error("Failed to initialize console: " + errorString);

    // TODO: backtrace, etc
    //
    // spdlog
    //
    spdlog::init_thread_pool(8192, 1);

    // stdout_color_sink_mt is an alias for either
    // - wincolor_stdout_sink_mt (on windows)
    // - ansicolor_stdout_sink_mt (on unix)
    // but it has different set_color behavior on windows vs unix
    const auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    #ifdef _WIN32
    consoleSink->set_color(spdlog::level::trace, 5);
    consoleSink->set_color(spdlog::level::info, 7);
    #else
    console_sink->set_color(spdlog::level::trace, console_sink->magenta);
    console_sink->set_color(spdlog::level::info, console_sink->white);
    #endif

    consoleSink->set_level(spdlog::level::trace);
    consoleSink->set_pattern("%^%v%$");

    spdlog::file_event_handlers handlers;
    handlers.after_open = [](spdlog::filename_t, std::FILE* fstream) {
        (void) fputs(
            "--------------------------------------------------------------------------------\n",
            fstream);
    };

    const auto fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        "iocod.log",
        8_MB,
        3,
        false,
        handlers);
    fileSink->set_level(spdlog::level::trace);
    fileSink->set_pattern("[%Y-%m-%d %T] [%L] %v");

    std::vector<spdlog::sink_ptr> sinks{consoleSink, fileSink};
    logger = std::make_shared<spdlog::async_logger>("iocod",
                                                    sinks.begin(),
                                                    sinks.end(),
                                                    spdlog::thread_pool(),
                                                    spdlog::async_overflow_policy::block);

    logger->set_level(spdlog::level::trace);
    spdlog::register_logger(logger);
    spdlog::set_default_logger(logger);
    //
    // end spdlog
    //

    m_systemInfo = PopulateSystemInfo();

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
        std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - m_startTime).count());
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
// SystemLocal::FlushConsole
// --------------------------------
void SystemLocal::FlushConsole()
{
    Console::Flush();
}

// --------------------------------
// SystemLocal::Print
// --------------------------------
void SystemLocal::Print(const String& message) noexcept
{
    if (Console::Initialized())
        Console::Print(message);
    else
        (void) fprintf(stderr, "%s", message.c_str());
}

// --------------------------------
// SystemLocal::DebugPrint
// --------------------------------
void SystemLocal::DebugPrint(const String& message) noexcept
{
    if (Console::Initialized())
        Console::DebugPrint(message);
    else
        (void) fprintf(stderr, "%s", message.c_str());
}

// --------------------------------
// SystemLocal::Error
// --------------------------------
void SystemLocal::Error(const String& message) noexcept
{
    if (Console::Initialized()) {
        Console::Print(message);
        Console::DisplayError(message);
        Console::WaitForQuit();
    } else {
        (void) fprintf(stderr, "%s", message.c_str());
    }

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
const SystemInfo& SystemLocal::GetSystemInfo() const
{
    return m_systemInfo;
}

// --------------------------------
// SystemLocal::PrintSystemInfo
// --------------------------------
void SystemLocal::PrintSystemInfo()
{
    String output = "System Info:\n";

    auto humanReadable = [](const u64 bytes) {
        if (const auto size = static_cast<double>(bytes); size < KB)
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

    if (!m_systemInfo.cpuVendor.Empty())
        output += "CPU Vendor: " + m_systemInfo.cpuVendor + "\n";
    if (!m_systemInfo.cpuModel.Empty())
        output += "CPU Model: " + m_systemInfo.cpuModel + "\n";
    if (m_systemInfo.cpuCores != 0)
        output += "CPU Cores: " + String{m_systemInfo.cpuCores} + "\n";
    if (m_systemInfo.cpuThreads != 0)
        output += "CPU Threads: " + String{m_systemInfo.cpuThreads} + "\n";
    if (m_systemInfo.cpuMhz != 0.0f)
        output += "CPU MHz: " + String{m_systemInfo.cpuMhz} + "\n";

    if (m_systemInfo.memTotal != 0)
        output += "Memory Total: " + humanReadable(m_systemInfo.memTotal) + "\n";
    if (m_systemInfo.memFree != 0)
        output += "Memory Free: " + humanReadable(m_systemInfo.memFree) + "\n";
    if (m_systemInfo.memAvailable != 0)
        output += "Memory Available: " + humanReadable(m_systemInfo.memAvailable) + "\n";
    if (m_systemInfo.memVirtualSize != 0)
        output += "Virtual Memory Size: " + humanReadable(m_systemInfo.memVirtualSize) + "\n";
    if (m_systemInfo.memVirtualAvailable != 0)
        output +=
            "Virtual Memory Available: " + humanReadable(m_systemInfo.memVirtualAvailable) + "\n";
    if (m_systemInfo.memVirtualPeak != 0)
        output += "Virtual Memory Peak: " + humanReadable(m_systemInfo.memVirtualPeak) + "\n";
    if (m_systemInfo.memPhysicalSize != 0)
        output += "Physical Memory Size: " + humanReadable(m_systemInfo.memPhysicalSize) + "\n";
    if (m_systemInfo.memPhysicalPeak != 0)
        output += "Physical Memory Peak: " + humanReadable(m_systemInfo.memPhysicalPeak) + "\n";

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

class CommandEcho final : public IConsoleCommand {
public:
    void Execute(const std::vector<String> args) override
    {
        String output;
        for (const auto& arg : args)
            output += arg + " ";

        sys->Print(output + "\n");
    }
};

class CommandClear final : public IConsoleCommand {
public:
    void Execute(const std::vector<String> args) override
    {
        sys->ClearConsole();
    }
};

class CommandQuit final : public IConsoleCommand {
public:
    void Execute(const std::vector<String> args) override
    {
        sys->Print("See ya!\n");
        sys->Exit(0);
    }
};

class CommandSystemInfo final : public IConsoleCommand {
private:
    void Execute(const std::vector<String> args) override
    {
        sys->PrintSystemInfo();
    }
};

// --------------------------------
// SystemLocal::AddConsoleCommands
// --------------------------------
void SystemLocal::AddConsoleCommands() noexcept
{
    commandSystem->AddCommand("echo", std::make_unique<CommandEcho>());
    commandSystem->AddAlias("echo", "print");

    commandSystem->AddCommand("clear", std::make_unique<CommandClear>());

    commandSystem->AddCommand("quit", std::make_unique<CommandQuit>());
    commandSystem->AddAlias("quit", "exit");

    commandSystem->AddCommand("systeminfo", std::make_unique<CommandSystemInfo>());
    commandSystem->AddAlias("systeminfo", "sysinfo");
}

} // namespace iocod
