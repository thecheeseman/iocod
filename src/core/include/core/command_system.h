// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_COMMAND_SYSTEM_H
#define CORE_COMMAND_SYSTEM_H

#include <core/console_command.h>
#include <core/platform.h>

#include <memory>

namespace iocod {

enum class CommandExecutionType {
    Immediate,
    Insert,
    Append
};

class ICommandSystem {
public:
    virtual ~ICommandSystem() = default;

    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;

    virtual bool IsSystemActive() const noexcept = 0;

    virtual std::size_t Argc() const noexcept = 0;
    virtual std::string Argv(std::size_t index) const noexcept = 0;
    virtual std::string Args() const = 0;
    virtual std::string ArgsFrom(std::size_t index) const = 0;

    virtual bool AddCommand(const std::string& name, std::unique_ptr<IConsoleCommand> command) = 0;
    virtual bool RemoveCommand(const std::string& name) = 0;
    virtual bool HasCommand(const std::string& name) const = 0;
    virtual std::vector<std::string> GetCommandList() const = 0;

    virtual void TokenizeString(const std::string& text) = 0;

    virtual void CommandBufferInitialize() noexcept = 0;
    virtual void AddCommandText(const std::string& text) = 0;
    virtual void InsertCommandText(const std::string& text) = 0;
    virtual void ExecuteCommandText(const std::string& text) = 0;
    virtual void BufferCommandText(CommandExecutionType execution_type,
                                   const std::string& text) = 0;
    virtual void ExecuteCommandBuffer() = 0;

    virtual void SetWaitCounter(std::size_t wait_counter) noexcept = 0;

    using DelayedRegisterFunction = void(*)();

    /// Delayed register functions are called after the command system is initialized.
    /// Here you can register commands before the command system is active.
    static void AddRegisterCallback(DelayedRegisterFunction function) noexcept;
};

extern ICommandSystem* g_command_system;

} // namespace iocod

#endif // CORE_COMMAND_SYSTEM_H
