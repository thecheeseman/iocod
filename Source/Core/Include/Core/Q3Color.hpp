// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef Q3COLOR_HPP
#define Q3COLOR_HPP

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

/// @brief Checks if a string is a Quake3 color string (e.g. "^1").
inline bool IsQ3ColorString(const char* str) noexcept
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

#endif // Q3COLOR_HPP
