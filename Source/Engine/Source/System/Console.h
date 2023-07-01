// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_CONSOLE_CONSOLE_H
#define CORE_CONSOLE_CONSOLE_H

#include <core/q3color.h>

#include <array>

namespace iocod {

#if 0
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
#endif

enum class ConsoleVisLevel {
    Hidden,
    Normal,
    Minimized
};

namespace Console {

constexpr const char* DED_CLASS = "iocod_console";
constexpr const char* GAME_NAME = "iocod";

std::pair<bool, String> Initialize(void* handle = nullptr);
bool Initialized();
void Shutdown();

void Print(const String& message);
void DebugPrint(const String& message);
void Flush();
void Clear();
void Show(ConsoleVisLevel level, bool quitOnClose);
void DisplayError(const String& message);

String GetInput();

void PumpEvents();

void WaitForQuit();

} // namespace Console

#if 0
class Console {
public:
    Console() = default;
    ~Console() = default;

    Console(const Console&) = delete;
    Console(Console&&) = delete;
    Console& operator=(const Console&) = delete;
    Console& operator=(Console&&) = delete;

    std::pair<bool, String> Initialize(void* handle = nullptr) noexcept;
    bool Initialized() const noexcept { return initialized_; }
    bool Shutdown() noexcept;

    void Print(const String& text);
    void Clear();

    String GetInput() noexcept;

    void PumpEvents();

    void Show(const ConsoleVisLevel level, const bool quitOnClose);
    void DisplayError(const String& message);

    void SetTextColor(VTColor foreground, VTColor background = VTColor::Black);
    void SetTitle(const String& title);

private:
    static constexpr const char PROMPT[] = "] ";

    static inline constexpr std::size_t MAX_EDIT_LINE = 1024;
    std::array<char, MAX_EDIT_LINE> buffer_{};

    bool initialized_ = false;
    bool console_on_ = false;
    std::size_t cursor_ = 0;

    VTColor foreground_ = VTColor::White;
    VTColor background_ = VTColor::Black;

    void ColorPrint(const String& text);
    #if 0
    void Back();
    void Show();
    void Hide();
    #endif
};
#endif

} // namespace iocod

#endif // CORE_CONSOLE_CONSOLE_H
