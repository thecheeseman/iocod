// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <core/case_insensitive_map.h>
#include <core/command_system.h>
#include <core/string_utilities.h>

#include <deque>

namespace iocod {

class CommandSystemLocal final : public ICommandSystem {
public:
    void Initialize() override;
    void Shutdown() override;

    std::size_t Argc() const noexcept override;
    std::string Argv(std::size_t index) const noexcept override;
    std::string Args() const override;
    std::string ArgsFrom(std::size_t index) const override;

    bool AddCommand(const std::string& name, std::unique_ptr<IConsoleCommand> command) override;
    bool RemoveCommand(const std::string& name) override;
    bool HasCommand(const std::string& name) const override;
    std::vector<std::string> GetCommandList() const override;

    void TokenizeString(const std::string& text) override;

    void CommandBufferInitialize() noexcept override;
    void AddCommandText(const std::string& text) override;
    void InsertCommandText(const std::string& text) override;
    void ExecuteCommandText(const std::string& text) override;
    void BufferCommandText(CommandExecutionType execution_type, const std::string& text) override;
    void ExecuteCommandBuffer() override;

    void SetWaitCounter(std::size_t count) noexcept override
    {
        wait_counter = count;
    }

private:
    static inline constexpr std::size_t MAX_ARGV_TOKENS = 1024;

    std::size_t wait_counter = 0;

    std::deque<std::string> command_buffer;
    std::vector<std::string> command_args;

    case_insensitive_map<std::unique_ptr<IConsoleCommand>> commands;

    void AddConsoleCommands();
};

CommandSystemLocal local;
ICommandSystem* g_commandSystem = &local;

// --------------------------------
// CommandSystemLocal::Init
// --------------------------------
void CommandSystemLocal::Initialize()
{
    AddConsoleCommands();
}

// --------------------------------
// CommandSystemLocal::Shutdown
// --------------------------------
void CommandSystemLocal::Shutdown() {}

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
std::string CommandSystemLocal::Argv(std::size_t index) const noexcept
{
    if (index >= command_args.size())
        return "";

    return command_args[index];
}

// --------------------------------
// CommandSystemLocal::Args
// --------------------------------
std::string CommandSystemLocal::Args() const
{
    return ArgsFrom(1);
}

// --------------------------------
// CommandSystemLocal::ArgsFrom
// --------------------------------
std::string CommandSystemLocal::ArgsFrom(std::size_t index) const
{
    std::string args;
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
bool CommandSystemLocal::AddCommand(const std::string& name,
                                    std::unique_ptr<IConsoleCommand> command)
{
    if (HasCommand(name)) {
        // ctx.log->Warn("[CommandSystem] Command '{}' already exists", name);
        return false;
    }

    // ctx.log->Trace("[CommandSystem] Added command '{}'", name);
    commands[name] = std::move(command);
    return true;
}

// --------------------------------
// CommandSystemLocal::RemoveCommand
// --------------------------------
bool CommandSystemLocal::RemoveCommand(const std::string& name)
{
    if (!HasCommand(name))
        return false;

    // ctx.log->Trace("[CommandSystem] Removed command '{}'", name);
    return commands.erase(name) > 0;
}

// --------------------------------
// CommandSystemLocal::HasCommand
// --------------------------------
bool CommandSystemLocal::HasCommand(const std::string& name) const
{
    return commands.find(name) != commands.end();
}

// --------------------------------
// CommandSystemLocal::GetCommandList
// --------------------------------
std::vector<std::string> CommandSystemLocal::GetCommandList() const
{
    std::vector<std::string> list{};

    list.reserve(commands.size());
    for (const auto& [name, command] : commands)
        list.push_back(name);

    return list;
}

// --------------------------------
// CommandSystemLocal::TokenizeString
// --------------------------------
void CommandSystemLocal::TokenizeString(const std::string& text)
{
    command_args.clear();

    std::string trimmed = StringUtilities::TrimCopy(text);
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
            if (end == std::string::npos) {
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
void CommandSystemLocal::AddCommandText(const std::string& text)
{
    const std::vector<std::string> lines = StringUtilities::Split(text, ";\n\r");
    for (const auto& line : lines) {
        // ctx.log->Trace("[CommandSystem] AddCommandText(\"{}\")", StringUtilities::Escape(line));
        command_buffer.push_front(line);
    }
}

// --------------------------------
// CommandSystemLocal::InsertCommandText
// --------------------------------
void CommandSystemLocal::InsertCommandText(const std::string& text)
{
    const std::vector<std::string> lines = StringUtilities::Split(text, ";\n\r");
    for (const auto& line : lines) {
        // ctx.log->Trace("[CommandSystem] InsertCommandText(\"{}\")",
        // StringUtilities::Escape(line));
        command_buffer.push_back(line);
    }
}

// --------------------------------
// CommandSystemLocal::ExecuteCommandText
// --------------------------------
void CommandSystemLocal::ExecuteCommandText(const std::string& text)
{
    if (text.empty())
        return;

    TokenizeString(text);
    if (Argc() == 0)
        return;

    std::string cmd = Argv(0);
    if (HasCommand(cmd)) {
        // TEMP: do better
        std::vector<std::string> args;
        args.reserve(Argc() - 1);
        for (int i = 1; i < Argc(); i++)
            args.push_back(Argv(i));

        commands[cmd]->Execute(args);
        return;
    }

    // if (ctx.cvar_system->Command())
    //     return;

    // ctx.os->Print(fmt::format("Unknown command '{}'\n", cmd));
}

// --------------------------------
// CommandSystemLocal::BufferCommandText
// --------------------------------
void CommandSystemLocal::BufferCommandText(CommandExecutionType type, const std::string& text)
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
    void Execute(std::vector<std::string> args) {}
};

class Command_exec final : public IConsoleCommand {
public:
    void Execute(std::vector<std::string> args) {}
};

class Command_vstr final : public IConsoleCommand {
public:
    void Execute(std::vector<std::string> args) {}
};

class Command_wait final : public IConsoleCommand {
public:
    void Execute(std::vector<std::string> args)
    {
        if (args.size() != 1)
            return;

        g_commandSystem->SetWaitCounter(std::stol(args[0]));
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
