// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <array>
#include <string>

#include <Core/Q3Color.hpp>

enum class VTColor {
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,
    BrightBlack,
    BrightRed,
    BrightGreen,
    BrightYellow,
    BrightBlue,
    BrightMagenta,
    BrightCyan,
    BrightWhite
};

enum class VTAttribute {
    None,
    Bold,
    Underline
};

inline constexpr const char* VTForeColorCode(VTColor color) noexcept
{
    const char* color_codes[] = {
        "\033[30m", // Black
        "\033[31m", // Red
        "\033[32m", // Green
        "\033[33m", // Yellow
        "\033[34m", // Blue
        "\033[35m", // Magenta
        "\033[36m", // Cyan
        "\033[37m", // White
        "\033[90m", // BrightBlack
        "\033[91m", // BrightRed
        "\033[92m", // BrightGreen
        "\033[93m", // BrightYellow
        "\033[94m", // BrightBlue
        "\033[95m", // BrightMagenta
        "\033[96m", // BrightCyan
        "\033[97m", // BrightWhite
    };
    return color_codes[static_cast<int>(color)];
}

inline constexpr const char* VTBackColorCode(VTColor color) noexcept
{
    const char* color_codes[] = {
        "\033[40m",  // Black
        "\033[41m",  // Red
        "\033[42m",  // Green
        "\033[43m",  // Yellow
        "\033[44m",  // Blue
        "\033[45m",  // Magenta
        "\033[46m",  // Cyan
        "\033[47m",  // White
        "\033[100m", // BrightBlack
        "\033[101m", // BrightRed
        "\033[102m", // BrightGreen
        "\033[103m", // BrightYellow
        "\033[104m", // BrightBlue
        "\033[105m", // BrightMagenta
        "\033[106m", // BrightCyan
        "\033[107m", // BrightWhite
    };
    return color_codes[static_cast<int>(color)];
}

inline constexpr VTColor Q3ColorToVTColor(Q3Color color) noexcept
{
    const VTColor color_codes[] = {
        VTColor::Black,         VTColor::Red,       VTColor::Green,   VTColor::Yellow,
        VTColor::Blue,          VTColor::Cyan,      VTColor::Magenta, VTColor::White,
        VTColor::BrightMagenta, VTColor::BrightRed,
    };
    return color_codes[static_cast<int>(color)];
}

/// Console interface for the server.
class Console {
public:
    Console() = default;

    ~Console() {}

    Console(const Console&) = delete;
    Console(Console&&) = delete;
    Console& operator=(const Console&) = delete;
    Console& operator=(Console&&) = delete;

    bool Initialize() noexcept;
    bool Shutdown() noexcept;

    void Print(const std::string& text, bool manual_color = false);
    void DebugPrint(const std::string& text);
    void Warn(const std::string& text);
    void Error(const std::string& text);

    void Clear();

    std::string GetInput() noexcept;

    void SetTextColor(VTColor foreground, VTColor background = VTColor::Black);
    void SetTitle(const std::string& title);

    static Console& GetInstance() noexcept
    {
        static Console instance;
        return instance;
    }

private:
    static constexpr const char PROMPT[] = "] ";

    static inline constexpr std::size_t MAX_EDIT_LINE = 1024;
    std::array<char, MAX_EDIT_LINE> buffer_{};

    bool console_on_ = false;
    std::size_t cursor_ = 0;

    VTColor foreground_ = VTColor::White;
    VTColor background_ = VTColor::Black;

    void ColorPrint(const std::string& text);

    void Back();
    void Show();
    void Hide();
};

#endif // CONSOLE_HPP
