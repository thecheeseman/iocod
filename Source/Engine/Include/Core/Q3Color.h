// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef Q3COLOR_HPP
#define Q3COLOR_HPP

namespace iocod {

// Q3-compatible colors
enum class Q3Color {
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Cyan,
    Magenta,
    White,
    Purple,
    Rose,
};

// defines for string concatenation
#define S_COLOR_BLACK "^0"
#define S_COLOR_RED "^1"
#define S_COLOR_GREEN "^2"
#define S_COLOR_YELLOW "^3"
#define S_COLOR_BLUE "^4"
#define S_COLOR_CYAN "^5"
#define S_COLOR_MAGENTA "^6"
#define S_COLOR_WHITE "^7"
#define S_COLOR_PURPLE "^8"
#define S_COLOR_ROSE "^9"

/// @brief Checks if a string is a Quake3 color string (e.g. "^1").
inline constexpr bool IsQ3ColorString(const char* str) noexcept
{
    if (str == nullptr || str[0] != '^' || str[1] == '\0')
        return false;

    if (str[1] < 0 || str[1] < '0' || str[1] > '9')
        return false;

    return true;
}

/// @brief Converts a Q3Color to a virtual terminal color code.
inline constexpr const char* Q3ColorToCode(Q3Color color) noexcept
{
    const char* color_codes[] = {
        "\033[30m", // Black
        "\033[31m", // Red
        "\033[32m", // Green
        "\033[33m", // Yellow
        "\033[34m", // Blue
        "\033[36m", // Cyan
        "\033[35m", // Magenta
        "\033[37m", // White
        "\033[94m", // Purple
        "\033[91m", // Rose
    };

    return color_codes[static_cast<int>(color)];
}

} // namespace iocod

#endif // Q3COLOR_HPP
