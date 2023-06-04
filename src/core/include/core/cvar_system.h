// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_CVAR_SYSTEM_H
#define CORE_CVAR_SYSTEM_H

#include <core/cvar.h>
#include <core/types.h>

namespace iocod {

// TODO: consider std::string

/// Cvar system interface.
/// Basic usage is to locally hold a pointer to a Cvar* and then register it with the
/// cvar system. For example:
/// @code
/// extern Cvar* g_mycvar;
/// void SomeInitFunction()
/// {
///    g_mycvar = CvarSystem->RegisterInt("mycvar", 0);
/// }
/// @endcode
class ICvarSystem {
public:
    virtual ~ICvarSystem() = default;

    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;

    virtual Cvar* RegisterVariant(const char* name, CvarType type, CvarFlags flags, CvarValue value,
                                  CvarLimits domain) = 0;

    virtual void SetBool(Cvar* cvar, bool value) = 0;
    virtual void SetInt(Cvar* cvar, i32 value) = 0;
    virtual void SetFloat(Cvar* cvar, f32 value) = 0;
    virtual void SetString(Cvar* cvar, const char* value) = 0;
    virtual void SetIntByName(const char* name, i32 value) = 0;
    virtual void SetFloatByName(const char* name, f32 value) = 0;
    virtual void SetStringByName(const char* name, const char* value) = 0;

    virtual bool GetBool(const char* name) = 0;
    virtual i32 GetInt(const char* name) = 0;
    virtual f32 GetFloat(const char* name) = 0;
    virtual const char* GetString(const char* name) = 0;

    virtual Cvar* Find(const char* name) = 0;

    virtual bool IsValidName(const char* name) = 0;

    virtual bool IsSystemActive() = 0;
    virtual u64 GetCvarCount() = 0;
    virtual CvarFlags GetModifiedFlags() = 0;

    virtual bool CvarCommand() = 0;

    // TODO: consider using templates for these
    inline Cvar* RegisterBool(const char* name, bool value, CvarFlags flags = CvarFlags::None);
    inline Cvar* RegisterInt(const char* name, i32 value, i32 min = 0, i32 max = 0,
                             CvarFlags flags = CvarFlags::None);
    inline Cvar* RegisterFloat(const char* name, f32 value, f32 min = 0.0f, f32 max = 0.0f,
                               CvarFlags flags = CvarFlags::None);
    inline Cvar* RegisterString(const char* name, const char* value,
                                CvarFlags flags = CvarFlags::None);
    inline Cvar* RegisterEnum(const char* name, const char** list, i32 default_index = 0,
                              CvarFlags flags = CvarFlags::None);
    inline Cvar* RegisterVec2(const char* name, f32 x, f32 y, f32 min = 0.0f, f32 max = 0.0f,
                              CvarFlags flags = CvarFlags::None);
    inline Cvar* RegisterVec3(const char* name, f32 x, f32 y, f32 z, f32 min = 0.0f, f32 max = 0.0f,
                              CvarFlags flags = CvarFlags::None);
    inline Cvar* RegisterVec4(const char* name, f32 x, f32 y, f32 z, f32 w, f32 min = 0.0f,
                              f32 max = 0.0f, CvarFlags flags = CvarFlags::None);
    inline Cvar* RegisterColor(const char* name, f32 r, f32 g, f32 b, f32 a,
                               CvarFlags flags = CvarFlags::None);
};

//extern ICvarSystem* CvarSystem;

inline Cvar* ICvarSystem::RegisterBool(const char* name, bool value, CvarFlags flags)
{
    CvarLimits cvar_domain{i32{0}, i32{0}};
    CvarValue cvar_value{value};

    return RegisterVariant(name, CvarType::Bool, flags, cvar_value, cvar_domain);
}

inline Cvar* ICvarSystem::RegisterInt(const char* name, i32 value, i32 min, i32 max,
                                      CvarFlags flags)
{
    CvarLimits cvar_domain{min, max};
    CvarValue cvar_value{value};

    return RegisterVariant(name, CvarType::Int, flags, cvar_value, cvar_domain);
}

inline Cvar* ICvarSystem::RegisterFloat(const char* name, f32 value, f32 min, f32 max,
                                        CvarFlags flags)
{
    CvarLimits cvar_domain{min, max};
    CvarValue cvar_value{value};

    return RegisterVariant(name, CvarType::Float, flags, cvar_value, cvar_domain);
}

inline Cvar* ICvarSystem::RegisterString(const char* name, const char* value, CvarFlags flags)
{
    CvarLimits cvar_domain{0, nullptr};
    CvarValue cvar_value{value};

    return RegisterVariant(name, CvarType::String, flags, cvar_value, cvar_domain);
}

inline Cvar* ICvarSystem::RegisterEnum(const char* name, const char** list, i32 default_index,
                                       CvarFlags flags)
{
    i32 string_count = 0;
    while (list[string_count] != nullptr)
        string_count++;

    CvarLimits cvar_domain{string_count, list};
    CvarValue cvar_value{default_index};

    return RegisterVariant(name, CvarType::Enum, flags, cvar_value, cvar_domain);
}

inline Cvar* ICvarSystem::RegisterVec2(const char* name, f32 x, f32 y, f32 min, f32 max,
                                       CvarFlags flags)
{
    CvarLimits cvar_domain{min, max};
    CvarValue cvar_value{x, y};

    return RegisterVariant(name, CvarType::Vec2, flags, cvar_value, cvar_domain);
}

inline Cvar* ICvarSystem::RegisterVec3(const char* name, f32 x, f32 y, f32 z, f32 min, f32 max,
                                       CvarFlags flags)
{
    CvarLimits cvar_domain{min, max};
    CvarValue cvar_value{x, y, z};

    return RegisterVariant(name, CvarType::Vec3, flags, cvar_value, cvar_domain);
}

inline Cvar* ICvarSystem::RegisterVec4(const char* name, f32 x, f32 y, f32 z, f32 w, f32 min,
                                       f32 max, CvarFlags flags)
{
    CvarLimits cvar_domain{min, max};
    CvarValue cvar_value{x, y, z, w};

    return RegisterVariant(name, CvarType::Vec4, flags, cvar_value, cvar_domain);
}

inline Cvar* ICvarSystem::RegisterColor(const char* name, f32 r, f32 g, f32 b, f32 a,
                                        CvarFlags flags)
{
    CvarLimits cvar_domain{i32{0}, i32{0}};
    CvarValue cvar_value{static_cast<u8>(255.0f * Clamp(r, 0.0f, 1.0f)),
                         static_cast<u8>(255.0f * Clamp(g, 0.0f, 1.0f)),
                         static_cast<u8>(255.0f * Clamp(b, 0.0f, 1.0f)),
                         static_cast<u8>(255.0f * Clamp(a, 0.0f, 1.0f))};

    return RegisterVariant(name, CvarType::Color, flags, cvar_value, cvar_domain);
}

} // namespace iocod

#endif // CORE_CVAR_SYSTEM_H
