// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_CVAR_H
#define CORE_CVAR_H

#include <core/enum_class_flags.h>
#include <core/types.h>

namespace iocod {

inline constexpr std::size_t MAX_CVARS = 2048;
inline constexpr std::size_t FILE_HASH_SIZE = 256;

enum class CvarSource {
    Internal,
    External,
    Script
};

enum class CvarFlags {
    // clang-format off
    None        = Bit(0),
    Archive     = Bit(1),
    UserInfo    = Bit(2),
    ServerInfo  = Bit(3),
    SystemInfo  = Bit(4),
    Init        = Bit(5),
    Latch       = Bit(6),
    ReadOnly    = Bit(7),
    Cheat       = Bit(8),
    Developer   = Bit(9),
    NoRestart   = Bit(10),
    External    = Bit(11)
    // clang-format on
};
ENUM_CLASS_FLAGS(CvarFlags)

inline constexpr i32 INVALID_ENUM_INDEX = -1337;

enum class CvarType {
    Bool,
    Float,
    Vec2,
    Vec3,
    Vec4,
    Int,
    Enum,
    String,
    Color
};

union CvarLimits {
    CvarLimits() = default;
    ~CvarLimits() = default;

    explicit CvarLimits(i32 count, const char** list)
        : enumeration{count, list}
    {
    }

    explicit CvarLimits(i32 min, i32 max)
        : integer{min, max}
    {
    }

    explicit CvarLimits(f32 min, f32 max)
        : decimal{min, max}
    {
    }

    struct {
        i32 string_count;
        const char** strings;
    } enumeration;

    struct {
        i32 min;
        i32 max;
    } integer;

    struct {
        f32 min;
        f32 max;
    } decimal;
};

struct CvarValue {
    CvarValue() = default;
    ~CvarValue() = default;

    explicit CvarValue(bool b)
        : boolean{b}
    {
    }

    explicit CvarValue(i32 i)
        : integer{i}
    {
    }

    explicit CvarValue(f32 f)
        : decimal{f}
    {
    }

    explicit CvarValue(f32 x, f32 y)
        : vec2{x, y}
    {
    }

    explicit CvarValue(f32 x, f32 y, f32 z)
        : vec3{x, y, z}
    {
    }

    explicit CvarValue(f32 x, f32 y, f32 z, f32 w)
        : vec4{x, y, z, w}
    {
    }

    explicit CvarValue(const char* s)
        : string{s}
    {
    }

    explicit CvarValue(u8 r, u8 g, u8 b, u8 a)
        : color{r, g, b, a}
    {
    }

    union {
        bool boolean;
        i32 integer;
        f32 decimal;
        vec2_t vec2;
        vec3_t vec3;
        vec4_t vec4;
        const char* string;
        u8 color[4];
    };
};

struct Cvar {
    const char* name;
    CvarFlags flags;
    CvarType type;
    bool modified;

    CvarValue current;
    CvarValue latched;
    CvarValue reset;
    CvarLimits domain;

    Cvar* next;
    Cvar* hash_next;
};

} // namespace iocod

#endif // CORE_CVAR_H
