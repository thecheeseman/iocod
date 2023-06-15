// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_COMMAND_SYSTEM_H
#define CORE_COMMAND_SYSTEM_H

#include <core/class_non_copyable.h>
#include <core/console_command.h>
#include <core/platform.h>
#include <map>
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
    ICommandSystem() = default;
    virtual ~ICommandSystem() = default;
    CLASS_NON_COPYABLE(ICommandSystem)

    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;

    [[nodiscard]] virtual bool IsSystemActive() const noexcept = 0;

    [[nodiscard]] virtual std::size_t Argc() const noexcept = 0;
    [[nodiscard]] virtual String Argv(std::size_t index) const noexcept = 0;
    [[nodiscard]] virtual String Args() const = 0;
    [[nodiscard]] virtual String ArgsFrom(std::size_t index) const = 0;

    virtual bool AddCommand(const String& name, std::unique_ptr<IConsoleCommand> command) = 0;
    virtual bool RemoveCommand(const String& name) = 0;
    [[nodiscard]] virtual bool HasCommand(const String& name) const = 0;
    [[nodiscard]] virtual std::map<String, String> GetCommandList() const = 0;

    virtual bool AddAlias(const String& command, const String& alias) = 0;
    virtual bool RemoveAlias(const String& alias) = 0;

    virtual void TokenizeString(const String& text) = 0;

    virtual void CommandBufferInitialize() noexcept = 0;
    virtual void AddCommandText(const String& text) = 0;
    virtual void InsertCommandText(const String& text) = 0;
    virtual void ExecuteCommandText(const String& text) = 0;
    virtual void BufferCommandText(CommandExecutionType executionType, const String& text) = 0;
    virtual void ExecuteCommandBuffer() = 0;

    virtual void SetWaitCounter(std::size_t waitCounter) noexcept = 0;

    using DelayedRegisterFunction = void (*)();

    /// Delayed register functions are called after the command system is initialized.
    /// Here you can register commands before the command system is active.
    static void AddRegisterCallback(DelayedRegisterFunction function) noexcept;
};

extern ICommandSystem* commandSystem;

} // namespace iocod

#endif // CORE_COMMAND_SYSTEM_H
