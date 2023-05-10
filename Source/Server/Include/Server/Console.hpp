// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <array>
#include <string>

#include <Core/Q3Color.hpp>

/// Console interface for the server.
class Console {
public:
    bool Initialize() noexcept;
    bool Shutdown() noexcept;

    void Print(const std::string& text);
    void DebugPrint(const std::string& text);
    void Clear();

    std::string GetInput() noexcept;

    static Console& Get() noexcept
    {
        static Console instance;
        return instance;
    }

private:
    static constexpr const char PROMPT[] = "] ";

    static inline constexpr std::size_t MAX_EDIT_LINE = 1024;
    std::array<char, MAX_EDIT_LINE> buffer{};

    bool console_on = false;

    void Back();
    void Show();
    void Hide() noexcept;
};

#endif // CONSOLE_HPP
