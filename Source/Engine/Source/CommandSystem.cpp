// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <deque>
#include <ranges>
#include <vector>
#include <Core/CaseInsensitiveMap.h>
#include <Core/CommandSystem.h>
#include <Core/CvarSystem.h>
#include <Core/StringUtilities.h>
#include <Core/System.h>
#include <fmt/format.h>
#include <regex>

namespace iocod {

std::vector<ICommandSystem::DelayedRegisterFunction> registerCallbacks{};

void ICommandSystem::AddRegisterCallback(const DelayedRegisterFunction function) noexcept
{
    // already initialized command system
    if (commandSystem->IsSystemActive()) {
        function();
        return;
    }

    // wait until command system is initialized
    if (std::ranges::find(registerCallbacks.begin(), registerCallbacks.end(), function) ==
        registerCallbacks.end()) {
        registerCallbacks.push_back(function);
    }
}

class CommandSystemLocal final : public ICommandSystem {
public:
    void Initialize() override;
    void Shutdown() override;

    [[nodiscard]] bool IsSystemActive() const noexcept override
    {
        return m_systemActive;
    }

    [[nodiscard]] std::size_t Argc() const noexcept override;
    [[nodiscard]] String Argv(std::size_t index) const noexcept override;
    [[nodiscard]] String Args() const override;
    [[nodiscard]] String ArgsFrom(std::size_t index) const override;

    bool AddCommand(const String& name, std::unique_ptr<IConsoleCommand> command) override;
    bool RemoveCommand(const String& name) override;
    [[nodiscard]] bool HasCommand(const String& name) const override;
    [[nodiscard]] std::map<String, String> GetCommandList() const override;

    bool AddAlias(const String& command, const String& alias) override;
    bool RemoveAlias(const String& alias) override;

    void TokenizeString(const String& text) override;

    void CommandBufferInitialize() noexcept override;
    void AddCommandText(const String& text) override;
    void InsertCommandText(const String& text) override;
    void ExecuteCommandText(const String& text) override;
    void BufferCommandText(CommandExecutionType type, const String& text) override;
    void ExecuteCommandBuffer() override;

    void SetWaitCounter(const std::size_t count) noexcept override
    {
        m_waitCounter = count;
    }

private:
    bool m_systemActive = false;
    static inline constexpr std::size_t kMaxArgvTokens = 1024;

    std::size_t m_waitCounter = 0;

    std::deque<String> m_commandBuffer;
    std::vector<String> m_commandArgs;

    CaseInsensitiveMap<std::unique_ptr<IConsoleCommand>> m_commands;
    CaseInsensitiveMap<String> m_aliases;

    void AddConsoleCommands();
};

CommandSystemLocal local;
ICommandSystem* commandSystem = &local;

// --------------------------------
// CommandSystemLocal::Init
// --------------------------------
void CommandSystemLocal::Initialize()
{
    AddConsoleCommands();
    m_systemActive = true;

    // Call any delayed register callbacks,
    while (!registerCallbacks.empty()) {
        registerCallbacks.back()();
        registerCallbacks.pop_back();
    }
}

// --------------------------------
// CommandSystemLocal::Shutdown
// --------------------------------
void CommandSystemLocal::Shutdown()
{
    m_systemActive = false;
}

// --------------------------------
// CommandSystemLocal::Argc
// --------------------------------
std::size_t CommandSystemLocal::Argc() const noexcept
{
    return m_commandArgs.size();
}

// --------------------------------
// CommandSystemLocal::Argv
// --------------------------------
String CommandSystemLocal::Argv(const std::size_t index) const noexcept
{
    if (index >= m_commandArgs.size())
        return {""};

    return m_commandArgs[index];
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
String CommandSystemLocal::ArgsFrom(const std::size_t index) const
{
    String args;
    for (size_t i = index; i < m_commandArgs.size(); i++) {
        args += m_commandArgs[i];
        if (i != m_commandArgs.size() - 1)
            args += " ";
    }
    return args;
}

// --------------------------------
// CommandSystemLocal::AddCommand
// --------------------------------
bool CommandSystemLocal::AddCommand(const String& name, std::unique_ptr<IConsoleCommand> command)
{
    if (HasCommand(name)) {
        LogWarn("[CommandSystem] Command '{}' already exists", name);
        return false;
    }

    LogTrace("[CommandSystem] Added command '{}'", name);
    m_commands[name] = std::move(command);
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
    return m_commands.erase(name) > 0;
}

// --------------------------------
// CommandSystemLocal::HasCommand
// --------------------------------
bool CommandSystemLocal::HasCommand(const String& name) const
{
    return m_commands.contains(name);
}

// --------------------------------
// CommandSystemLocal::GetCommandList
// --------------------------------
std::map<String, String> CommandSystemLocal::GetCommandList() const
{
    std::map<String, String> list{};

    for (const auto& name : std::views::keys(m_commands)) {
        String aliases;
        bool hasAliases = false;

        for (const auto& [alias, command] : m_aliases) {
            if (command == name) {
                if (!hasAliases) {
                    hasAliases = true;
                    aliases += alias;
                } else {
                    aliases += ", " + alias;
                }
            }
        }

        list[name] = aliases;
    }

    return list;
}

// --------------------------------
// CommandSystemLocal::AddAlias
// --------------------------------
bool CommandSystemLocal::AddAlias(const String& command, const String& alias)
{
    if (m_aliases.contains(alias)) {
        LogWarn("[CommandSystem] Alias '{}' already exists", alias);
        return false;
    }

    if (m_aliases.contains(command)) {
        LogWarn("[CommandSystem] Cannot alias '{}' to '{}', already exists", alias, command);
        return false;
    }

    if (!m_commands.contains(command)) {
        LogWarn("[CommandSystem] Cannot alias non-existent command '{}'", command);
        return false;
    }

    m_aliases[alias] = command;
    return true;
}

// --------------------------------
// CommandSystemLocal::RemoveAlias
// --------------------------------
bool CommandSystemLocal::RemoveAlias(const String& alias)
{
    if (m_aliases.contains(alias))
        m_aliases.erase(alias);

    return true;
}

// --------------------------------
// CommandSystemLocal::TokenizeString
// --------------------------------
void CommandSystemLocal::TokenizeString(const String& text)
{
    m_commandArgs.clear();

    String trimmed = StringUtilities::TrimCopy(text);
    if (trimmed.Empty() || (trimmed.Length() > 1 && trimmed[0] == '/' && trimmed[1] == '/'))
        return;

    while (true) {
        if (trimmed[0] == '"') {
            const std::size_t end = trimmed.Find('"', 1);
            m_commandArgs.push_back(trimmed.Substring(1, end - 1));
            trimmed = trimmed.Substring(end + 1);
        } else {
            if (trimmed[0] == '/' && trimmed[1] == '/')
                break;

            const std::size_t end = trimmed.Find(' ');
            if (end == String::npos) {
                m_commandArgs.push_back(trimmed);
                break;
            }

            m_commandArgs.push_back(trimmed.Substring(0, end));
            trimmed = trimmed.Substring(end + 1);
        }

        StringUtilities::Trim(trimmed);
        if (trimmed.Empty())
            break;
    }
}

// --------------------------------
// CommandSystemLocal::CommandBufferInit
// --------------------------------
void CommandSystemLocal::CommandBufferInitialize() noexcept
{
    m_commandBuffer.clear();
}

// --------------------------------
// CommandSystemLocal::AddCommandText
// --------------------------------
void CommandSystemLocal::AddCommandText(const String& text)
{
    for (const auto lines = StringUtilities::Split(text, ";\n\r"); const auto& line : lines) {
        LogTrace("[CommandSystem] AddCommandText(\"{}\")", StringUtilities::Escape(line));
        m_commandBuffer.push_back(line);
    }
}

// --------------------------------
// CommandSystemLocal::InsertCommandText
// --------------------------------
void CommandSystemLocal::InsertCommandText(const String& text)
{
    for (const auto lines = StringUtilities::Split(text, ";\n\r"); const auto& line : lines) {
        LogTrace("[CommandSystem] InsertCommandText(\"{}\")", StringUtilities::Escape(line));
        m_commandBuffer.push_front(line);
    }
}

// --------------------------------
// CommandSystemLocal::ExecuteCommandText
// --------------------------------
void CommandSystemLocal::ExecuteCommandText(const String& text)
{
    if (text.Empty())
        return;

    TokenizeString(text);
    if (Argc() == 0)
        return;

    String cmd = Argv(0);

    if (m_aliases.contains(cmd))
        cmd = m_aliases[cmd];

    if (HasCommand(cmd)) {
        // TEMP: do better
        std::vector<String> args;
        args.reserve(Argc() - 1);
        for (size_t i = 1; i < Argc(); i++)
            args.push_back(Argv(i));

        m_commands[cmd]->Execute(args);
        return;
    }

    // if (CvarSystem->IsSystemActive() && CvarSystem->CvarCommand())
    //     return;

    sys->Print(fmt::format("Unknown command '{}'\n", cmd));
}

// --------------------------------
// CommandSystemLocal::BufferCommandText
// --------------------------------
void CommandSystemLocal::BufferCommandText(const CommandExecutionType type, const String& text)
{
    switch (type) {
    case CommandExecutionType::Immediate:
        if (!text.Empty())
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
    }
}

// --------------------------------
// CommandSystemLocal::ExecuteCommandBuffer
// --------------------------------
void CommandSystemLocal::ExecuteCommandBuffer()
{
    if (m_commandBuffer.empty())
        return;

    if (m_waitCounter > 0) {
        m_waitCounter--;
        return;
    }

    ExecuteCommandText(m_commandBuffer.front());
    m_commandBuffer.pop_front();
}

// ================================
// m_commands
// ================================

class CommandCommandList final : public IConsoleCommand {
public:
    void Execute(const std::vector<String> args) override
    {
        std::regex match(".*");
        if (!args.empty())
            match = std::regex(args[0], std::regex_constants::icase);

        size_t count = 0;
        size_t matched = 0;
        sys->Print("command                          aliases (if any)\n");
        sys->Print("-------------------------------- --------------------------------\n");
        for (const auto& [cmd, aliases] : commandSystem->GetCommandList()) {
            count++;

            if (!std::regex_match(cmd, match))
                continue;

            matched++;

            sys->Print(fmt::format("{:<32} {}\n", cmd, aliases.Empty() ? "" : aliases));
        }
        sys->Print("-----------------------------------------------------------------\n");
        sys->Print(fmt::format("{} total commands", count));

        if (!args.empty())
            sys->Print(fmt::format(", {} matched '{}'\n", matched, args[0]));

        sys->Print("\n");
    }
};

class CommandExec final : public IConsoleCommand {
public:
    void Execute(std::vector<String> args) override
    {}
};

class CommandVstr final : public IConsoleCommand {
public:
    void Execute(std::vector<String> args) override
    {}
};

class CommandWait final : public IConsoleCommand {
public:
    void Execute(const std::vector<String> args) override
    {
        if (args.size() != 1)
            return;

        commandSystem->SetWaitCounter(std::stol(args[0]));
    }
};

// --------------------------------
// CommandSystemLocal::AddConsoleCommands
// --------------------------------
void CommandSystemLocal::AddConsoleCommands()
{
    AddCommand("commandlist", std::make_unique<CommandCommandList>());
    AddAlias("commandlist", "cmdlist");
    AddCommand("exec", std::make_unique<CommandExec>());
    AddCommand("vstr", std::make_unique<CommandVstr>());
    AddCommand("wait", std::make_unique<CommandWait>());
}

} // namespace iocod
