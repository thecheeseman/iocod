// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_COMMAND_SYSTEM_H
#define CORE_COMMAND_SYSTEM_H

#include <core/console_command.h>
#include <core/platform.h>
#include <core/types.h>

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
    virtual String Argv(std::size_t index) const noexcept = 0;
    virtual String Args() const = 0;
    virtual String ArgsFrom(std::size_t index) const = 0;

    virtual bool AddCommand(const String& name, std::unique_ptr<IConsoleCommand> command) = 0;
    virtual bool RemoveCommand(const String& name) = 0;
    virtual bool HasCommand(const String& name) const = 0;
    virtual std::vector<String> GetCommandList() const = 0;

    virtual void TokenizeString(const String& text) = 0;

    virtual void CommandBufferInitialize() noexcept = 0;
    virtual void AddCommandText(const String& text) = 0;
    virtual void InsertCommandText(const String& text) = 0;
    virtual void ExecuteCommandText(const String& text) = 0;
    virtual void BufferCommandText(CommandExecutionType execution_type,
                                   const String& text) = 0;
    virtual void ExecuteCommandBuffer() = 0;

    virtual void SetWaitCounter(std::size_t wait_counter) noexcept = 0;

    using DelayedRegisterFunction = void (*)();

    /// Delayed register functions are called after the command system is initialized.
    /// Here you can register commands before the command system is active.
    static void AddRegisterCallback(DelayedRegisterFunction function) noexcept;
};

extern ICommandSystem* g_command_system;

} // namespace iocod

#endif // CORE_COMMAND_SYSTEM_H
