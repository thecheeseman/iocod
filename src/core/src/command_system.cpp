// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <core/case_insensitive_map.h>
#include <core/command_system.h>
#include <core/cvar_system.h>
#include <core/string_utilities.h>
#include <core/system.h>
#include <fmt/format.h>

#include <deque>
#include <vector>

namespace iocod {

namespace {
std::vector<ICommandSystem::DelayedRegisterFunction> register_callbacks{};
} // namespace

void ICommandSystem::AddRegisterCallback(DelayedRegisterFunction function) noexcept
{
    // already initialized command system
    if (g_command_system->IsSystemActive()) {
        function();
        return;
    }

    // wait until command system is initialized
    if (std::find(register_callbacks.begin(), register_callbacks.end(), function) ==
        register_callbacks.end()) {
        register_callbacks.push_back(function);
    }
}

class CommandSystemLocal final : public ICommandSystem {
public:
    void Initialize() override;
    void Shutdown() override;

    inline bool IsSystemActive() const noexcept override
    {
        return system_active;
    }

    std::size_t Argc() const noexcept override;
    String Argv(std::size_t index) const noexcept override;
    String Args() const override;
    String ArgsFrom(std::size_t index) const override;

    bool AddCommand(const String& name, std::unique_ptr<IConsoleCommand> command) override;
    bool RemoveCommand(const String& name) override;
    bool HasCommand(const String& name) const override;
    std::vector<String> GetCommandList() const override;

    void TokenizeString(const String& text) override;

    void CommandBufferInitialize() noexcept override;
    void AddCommandText(const String& text) override;
    void InsertCommandText(const String& text) override;
    void ExecuteCommandText(const String& text) override;
    void BufferCommandText(CommandExecutionType execution_type, const String& text) override;
    void ExecuteCommandBuffer() override;

    void SetWaitCounter(std::size_t count) noexcept override
    {
        wait_counter = count;
    }

private:
    bool system_active = false;
    static inline constexpr std::size_t MAX_ARGV_TOKENS = 1024;

    std::size_t wait_counter = 0;

    std::deque<String> command_buffer;
    std::vector<String> command_args;

    CaseInsensitiveMap<std::unique_ptr<IConsoleCommand>> commands;

    void AddConsoleCommands();
};

CommandSystemLocal local;
ICommandSystem* g_command_system = &local;

// --------------------------------
// CommandSystemLocal::Init
// --------------------------------
void CommandSystemLocal::Initialize()
{
    AddConsoleCommands();
    system_active = true;

    // Call any delayed register callbacks,
    while (!register_callbacks.empty()) {
        register_callbacks.back()();
        register_callbacks.pop_back();
    }
}

// --------------------------------
// CommandSystemLocal::Shutdown
// --------------------------------
void CommandSystemLocal::Shutdown()
{
    system_active = false;
}

// --------------------------------
// CommandSystemLocal::Argc
// --------------------------------
std::size_t CommandSystemLocal::Argc() const noexcept
{
    return command_args.size();
}

// --------------------------------
// CommandSystemLocal::Argv
// --------------------------------
String CommandSystemLocal::Argv(std::size_t index) const noexcept
{
    if (index >= command_args.size())
        return String("");

    return command_args[index];
}

// --------------------------------
// CommandSystemLocal::Args
// --------------------------------
String CommandSystemLocal::Args() const
{
    return ArgsFrom(1);
}

// --------------------------------
// CommandSystemLocal::ArgsFrom
// --------------------------------
String CommandSystemLocal::ArgsFrom(std::size_t index) const
{
    String args;
    for (size_t i = index; i < command_args.size(); i++) {
        args += command_args[i];
        if (i != command_args.size() - 1)
            args += " ";
    }
    return args;
}

// --------------------------------
// CommandSystemLocal::AddCommand
// --------------------------------
bool CommandSystemLocal::AddCommand(const String& name,
                                    std::unique_ptr<IConsoleCommand> command)
{
    if (HasCommand(name)) {
        LogWarn("[CommandSystem] Command '{}' already exists", name);
        return false;
    }

    LogTrace("[CommandSystem] Added command '{}'", name);
    commands[name] = std::move(command);
    return true;
}

// --------------------------------
// CommandSystemLocal::RemoveCommand
// --------------------------------
bool CommandSystemLocal::RemoveCommand(const String& name)
{
    if (!HasCommand(name))
        return false;

    LogTrace("[CommandSystem] Removed command '{}'", name);
    return commands.erase(name) > 0;
}

// --------------------------------
// CommandSystemLocal::HasCommand
// --------------------------------
bool CommandSystemLocal::HasCommand(const String& name) const
{
    return commands.find(name) != commands.end();
}

// --------------------------------
// CommandSystemLocal::GetCommandList
// --------------------------------
std::vector<String> CommandSystemLocal::GetCommandList() const
{
    std::vector<String> list{};

    list.reserve(commands.size());
    for (const auto& [name, command] : commands)
        list.push_back(name);

    return list;
}

// --------------------------------
// CommandSystemLocal::TokenizeString
// --------------------------------
void CommandSystemLocal::TokenizeString(const String& text)
{
    command_args.clear();

    String trimmed = StringUtilities::TrimCopy(text);
    if (trimmed.empty() || (trimmed.size() > 1 && trimmed[0] == '/' && trimmed[1] == '/'))
        return;

    while (true) {
        if (trimmed[0] == '"') {
            std::size_t end = trimmed.find('"', 1);
            command_args.push_back(trimmed.substr(1, end - 1));
            trimmed = trimmed.substr(end + 1);
        } else {
            if (trimmed[0] == '/' && trimmed[1] == '/')
                break;

            std::size_t end = trimmed.find(' ');
            if (end == String::npos) {
                command_args.push_back(trimmed);
                break;
            }

            command_args.push_back(trimmed.substr(0, end));
            trimmed = trimmed.substr(end + 1);
        }

        StringUtilities::Trim(trimmed);
        if (trimmed.empty())
            break;
    }
}

// --------------------------------
// CommandSystemLocal::CommandBufferInit
// --------------------------------
void CommandSystemLocal::CommandBufferInitialize() noexcept
{
    command_buffer.clear();
}

// --------------------------------
// CommandSystemLocal::AddCommandText
// --------------------------------
void CommandSystemLocal::AddCommandText(const String& text)
{
    const std::vector<String> lines = StringUtilities::Split(text, ";\n\r");
    for (const auto& line : lines) {
        LogTrace("[CommandSystem] AddCommandText(\"{}\")", StringUtilities::Escape(line));
        command_buffer.push_front(line);
    }
}

// --------------------------------
// CommandSystemLocal::InsertCommandText
// --------------------------------
void CommandSystemLocal::InsertCommandText(const String& text)
{
    const std::vector<String> lines = StringUtilities::Split(text, ";\n\r");
    for (const auto& line : lines) {
        LogTrace("[CommandSystem] InsertCommandText(\"{}\")", StringUtilities::Escape(line));
        command_buffer.push_back(line);
    }
}

// --------------------------------
// CommandSystemLocal::ExecuteCommandText
// --------------------------------
void CommandSystemLocal::ExecuteCommandText(const String& text)
{
    if (text.empty())
        return;

    TokenizeString(text);
    if (Argc() == 0)
        return;

    String cmd = Argv(0);
    if (HasCommand(cmd)) {
        // TEMP: do better
        std::vector<String> args;
        args.reserve(Argc() - 1);
        for (int i = 1; i < Argc(); i++)
            args.push_back(Argv(i));

        commands[cmd]->Execute(args);
        return;
    }

    //if (CvarSystem->IsSystemActive() && CvarSystem->CvarCommand())
    //    return;

    g_system->Print(fmt::format("Unknown command '{}'\n", cmd));
}

// --------------------------------
// CommandSystemLocal::BufferCommandText
// --------------------------------
void CommandSystemLocal::BufferCommandText(CommandExecutionType type, const String& text)
{
    switch (type) {
    case CommandExecutionType::Immediate:
        if (!text.empty())
            ExecuteCommandText(text);
        else
            ExecuteCommandBuffer();
        break;
    case CommandExecutionType::Insert:
        InsertCommandText(text);
        break;
    case CommandExecutionType::Append:
        AddCommandText(text);
        break;
    default:
        // ERORR
        break;
    }
}

// --------------------------------
// CommandSystemLocal::ExecuteCommandBuffer
// --------------------------------
void CommandSystemLocal::ExecuteCommandBuffer()
{
    if (command_buffer.empty())
        return;

    if (wait_counter > 0) {
        wait_counter--;
        return;
    }

    ExecuteCommandText(command_buffer.front());
    command_buffer.pop_front();
}

// ================================
// commands
// ================================

class Command_cmdlist final : public IConsoleCommand {
public:
    void Execute(std::vector<String> args) {}
};

class Command_exec final : public IConsoleCommand {
public:
    void Execute(std::vector<String> args) {}
};

class Command_vstr final : public IConsoleCommand {
public:
    void Execute(std::vector<String> args) {}
};

class Command_wait final : public IConsoleCommand {
public:
    void Execute(std::vector<String> args)
    {
        if (args.size() != 1)
            return;

        g_command_system->SetWaitCounter(std::stol(args[0]));
    }
};

// --------------------------------
// CommandSystemLocal::AddConsoleCommands
// --------------------------------
void CommandSystemLocal::AddConsoleCommands()
{
    AddCommand("cmdlist", std::make_unique<Command_cmdlist>());
    AddCommand("exec", std::make_unique<Command_exec>());
    AddCommand("vstr", std::make_unique<Command_vstr>());
    AddCommand("wait", std::make_unique<Command_wait>());
}

} // namespace iocod
