// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ENGINE_TYPES_COLOR_H
#define ENGINE_TYPES_COLOR_H

namespace iocod {

struct Color {
    static constexpr float kEpsilon = 0.0001f;

    constexpr /*implicit*/ Color(const float rgba) :
        r(Clamp(rgba, 0.0f, 1.0f)),
        g(Clamp(rgba, 0.0f, 1.0f)),
        b(Clamp(rgba, 0.0f, 1.0f)),
        a(Clamp(rgba, 0.0f, 1.0f)) {}

    constexpr Color(const float r, const float g, const float b, const float a = 1.0f) :
        r(Clamp(r, 0.0f, 1.0f)),
        g(Clamp(g, 0.0f, 1.0f)),
        b(Clamp(b, 0.0f, 1.0f)),
        a(Clamp(a, 0.0f, 1.0f)) {}

    [[nodiscard]] constexpr u32 ToRGBA() const
    {
        return (static_cast<u32>(r * 255.0f) << 24) |
            (static_cast<u32>(g * 255.0f) << 16) |
            (static_cast<u32>(b * 255.0f) << 8) |
            (static_cast<u32>(a * 255.0f));
    }

    constexpr explicit operator u32() const
    {
        return ToRGBA();
    }

    [[nodiscard]] constexpr bool operator==(const Color& rhs) const
    {
        return (Abs(r - rhs.r) < kEpsilon) &&
            (Abs(g - rhs.g) < kEpsilon) &&
            (Abs(b - rhs.b) < kEpsilon) &&
            (Abs(a - rhs.a) < kEpsilon);
    }

    [[nodiscard]] constexpr bool operator!=(const Color& rhs) const
    {
        return !(*this == rhs);
    }

    float r;
    float g;
    float b;
    float a;
};

inline constexpr Color kColorBlack{0.0f};
inline constexpr Color kColorRed{1.0f, 0.0f, 0.0f};
inline constexpr Color kColorGreen{0.0f, 1.0f, 0.0f};
inline constexpr Color kColorBlue{0.0f, 0.0f, 1.0f};
inline constexpr Color kColorYellow{1.0f, 1.0f, 0.0f};
inline constexpr Color kColorMagenta{1.0f, 0.0f, 1.0f};
inline constexpr Color kColorCyan{0.0f, 1.0f, 1.0f};
inline constexpr Color kColorWhite{1.0f};
inline constexpr Color kColorLightGrey{0.75f};
inline constexpr Color kColorMediumGrey{0.5f};
inline constexpr Color kColorDarkGrey{0.25f};

} // namespace iocod

#endif // ENGINE_TYPES_COLOR_H
