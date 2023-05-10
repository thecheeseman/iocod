// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef Q3COLOR_HPP
#define Q3COLOR_HPP

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
    Rose
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
        "30", // Black
        "31", // Red
        "32", // Green
        "33", // Yellow
        "34", // Blue
        "36", // Cyan
        "35", // Magenta
        "37", // White
        "94", // Purple
        "91", // Rose
    };

    return color_codes[static_cast<int>(color)];
}

#endif // Q3COLOR_HPP
