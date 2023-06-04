// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <core/case_insensitive_map.h>
#include <core/cvar.h>
#include <core/cvar_system.h>
#include <core/system.h>

#include <cctype>
#include <cstring>

// hack
#ifdef _WIN32
    #define strcasecmp  _stricmp
    #define strncasecmp _strnicmp
#endif

namespace iocod {

class CvarSystemLocal final : public ICvarSystem {
public:
    void Initialize() override;
    void Shutdown() override;

    Cvar* RegisterVariant(const char* name, CvarType type, CvarFlags flags, CvarValue value,
                          CvarLimits domain) override
    {
        return nullptr;
    }

    void SetBool(Cvar* cvar, bool value) override {}

    void SetInt(Cvar* cvar, i32 value) override {}

    void SetFloat(Cvar* cvar, f32 value) override {}

    void SetString(Cvar* cvar, const char* value) override {}

    void SetIntByName(const char* name, i32 value) override {}

    void SetFloatByName(const char* name, f32 value) override {}

    void SetStringByName(const char* name, const char* value) override {}

    bool GetBool(const char* name) override
    {
        return false;
    }

    i32 GetInt(const char* name) override
    {
        return 0;
    }

    f32 GetFloat(const char* name) override
    {
        return 0.0;
    }

    const char* GetString(const char* name) override
    {
        return nullptr;
    }

    Cvar* Find(const char* name) override;

    bool IsValidName(const char* name) override
    {
        return false;
    }

    bool IsSystemActive() override
    {
        return system_active;
    }

    u64 GetCvarCount() override
    {
        return cvar_map.size();
    }

    CvarFlags GetModifiedFlags() override
    {
        return modified_flags;
    }

    bool CvarCommand() override
    {
        return false;
    }

private:
    bool system_active = false;

    CvarFlags modified_flags = CvarFlags::None;
    Cvar* cvar_cheats = nullptr;

    CaseInsensitiveMap<Cvar> cvar_map;

    // helper functions
    Cvar* RegisterNew(const char* name, CvarType type, CvarFlags flags, CvarValue value,
                      CvarLimits domain);
    Cvar* Reregister(Cvar* cvar, const char* name, CvarType type, CvarFlags flags, CvarValue value,
                     CvarLimits domain);
    Cvar* Reinterpret(Cvar* cvar, const char* name, CvarType type, CvarFlags flags, CvarValue value,
                      CvarLimits domain);
    Cvar* MakeExplicitType(Cvar* cvar, const char* name, CvarType type, CvarFlags flags,
                           CvarValue value, CvarLimits domain);

    void PerformUnregistration(Cvar* cvar);

    void SetVariant(Cvar* cvar, CvarValue value, CvarSource source);
    void SetLatchedValue(Cvar* cvar, CvarValue value);

    CvarValue StringToValue(CvarType type, CvarLimits domain, const char* string);
    bool StringToBool(const char* string);
    i32 StringToInt(const char* string);
    f32 StringToFloat(const char* string);
    i32 StringToEnum(const char* string, CvarLimits* domain);

    CvarValue ClampValueToDomain(CvarType type, CvarValue value, CvarValue reset,
                                 CvarLimits domain);

    void UpdateValue(Cvar* cvar, CvarValue value);
    void UpdateResetValue(Cvar* cvar, CvarValue value);

    inline void MakeLatchedValueCurrent(Cvar* cvar)
    {
        SetVariant(cvar, cvar->latched, CvarSource::Internal);
    }

    u64 GenerateHashValue(const char* name);

    inline void FreeString(CvarValue* value)
    {
        delete value->string;
        value->string = nullptr;
    }

    inline bool ShouldFreeCurrentString(Cvar* cvar)
    {
        return cvar->current.string && cvar->current.string != cvar->latched.string &&
               cvar->current.string != cvar->reset.string;
    }

    inline bool ShouldFreeResetString(Cvar* cvar)
    {
        return cvar->reset.string && cvar->reset.string != cvar->current.string &&
               cvar->reset.string != cvar->latched.string;
    }

    inline bool ShouldFreeLatchedString(Cvar* cvar)
    {
        return cvar->latched.string && cvar->latched.string != cvar->current.string &&
               cvar->latched.string != cvar->reset.string;
    }
};

CvarSystemLocal local;
ICvarSystem* CvarSystem = &local;

// --------------------------------
// CvarSystemLocal::Initialize
// --------------------------------
void CvarSystemLocal::Initialize()
{
    system_active = true;
    CvarSystem->RegisterBool("sv_cheats", false, CvarFlags::Init);
}

// --------------------------------
// CvarSystemLocal::Shutdown
// --------------------------------
void CvarSystemLocal::Shutdown()
{
    system_active = false;

    for (auto& [name, cvar] : cvar_map) {
        if (cvar.type == CvarType::String) {
            if (ShouldFreeCurrentString(&cvar))
                FreeString(&cvar.current);
            if (ShouldFreeResetString(&cvar))
                FreeString(&cvar.reset);
            if (ShouldFreeLatchedString(&cvar))
                FreeString(&cvar.latched);
        }

        if (HasEnumFlag(cvar.flags, CvarFlags::External))
            delete[] cvar.name;
    }

    cvar_map.clear();
    cvar_cheats = nullptr;
    modified_flags = CvarFlags::None;
}

// --------------------------------
// CvarSystemLocal::RegisterVariant
// --------------------------------
Cvar* CvarSystemLocal::RegisterVariant(const char* name, CvarType type, CvarFlags flags,
                                       CvarValue value, CvarLimits domain)
{
    Cvar* cvar = Find(name);
    if (!cvar)
        return RegisterNew(name, type, flags, value, domain);

    Reregister(cvar, name, type, flags, value, domain);
    return cvar;
}

// --------------------------------
// CvarSystemLocal::Find
// --------------------------------
Cvar* CvarSystemLocal::Find(const char* name)
{
    if (cvar_map.contains(name))
        return &cvar_map[name];

    return nullptr;
}

// ================================================================================================
// local helper functions
// ================================================================================================

// --------------------------------
// CvarSystemLocal::RegisterNew
// --------------------------------
Cvar* CvarSystemLocal::RegisterNew(const char* name, CvarType type, CvarFlags flags,
                                   CvarValue value, CvarLimits domain)
{
    Cvar& cvar = cvar_map[name];

    if (HasEnumFlag(flags, CvarFlags::External))
        cvar.name = strdup(name);
    else
        cvar.name = name;

    cvar.type = type;
    cvar.flags = flags;

    switch (type) {
    // TODO: vectors
    case CvarType::String:
        cvar.current.string = strdup(value.string);
        cvar.reset.string = cvar.current.string;
        cvar.latched.string = cvar.current.string;
        break;
    default:
        cvar.current = value;
        cvar.reset = value;
        cvar.latched = value;
        break;
    }

    cvar.domain = domain;
    cvar.modified = false;

    return &cvar;
}

// --------------------------------
// CvarSystemLocal::Reregister
// --------------------------------
Cvar* CvarSystemLocal::Reregister(Cvar* cvar, const char* name, CvarType type, CvarFlags flags,
                                  CvarValue value, CvarLimits domain)
{
    if (HasEnumFlag(cvar->flags ^ flags, CvarFlags::External))
        Reinterpret(cvar, name, type, flags, value, domain);

    if (HasEnumFlag(cvar->flags, CvarFlags::External) && cvar->type != type)
        MakeExplicitType(cvar, name, type, flags, value, domain);

    cvar->flags |= flags;

    if (HasEnumFlag(cvar->flags, CvarFlags::Cheat) && cvar_cheats &&
        !cvar_cheats->current.boolean) {
        SetVariant(cvar, cvar->reset, CvarSource::Internal);
        SetLatchedValue(cvar, cvar->reset);
    }

    if (HasEnumFlag(cvar->flags, CvarFlags::Latch))
        MakeLatchedValueCurrent(cvar);
}

// --------------------------------
// CvarSystemLocal::Reinterpret
// --------------------------------
Cvar* CvarSystemLocal::Reinterpret(Cvar* cvar, const char* name, CvarType type, CvarFlags flags,
                                   CvarValue value, CvarLimits domain)
{
    if (HasEnumFlag(cvar->flags, CvarFlags::External) && !HasEnumFlag(flags, CvarFlags::External)) {
        CvarValue reset = value;
        PerformUnregistration(cvar);

        delete[] cvar->name;
        cvar->name = name;
        cvar->flags &= ~CvarFlags::External;

        MakeExplicitType(cvar, name, type, flags, reset, domain);
    }
}

// --------------------------------
// CvarSystemLocal::MakeExplicitType
// --------------------------------
Cvar* CvarSystemLocal::MakeExplicitType(Cvar* cvar, const char* name, CvarType type,
                                        CvarFlags flags, CvarValue value, CvarLimits domain)
{
    cvar->type = type;
    cvar->domain = domain;

    CvarValue cast{};

    const bool read_only = HasEnumFlag(flags, CvarFlags::ReadOnly);
    const bool cheat_disabled =
        HasEnumFlag(flags, CvarFlags::Cheat) && cvar_cheats && !cvar_cheats->current.boolean;
    if (read_only || cheat_disabled) {
        cast = value;
    } else {
        cast = StringToValue(cvar->type, cvar->domain, cvar->current.string);
        cast = ClampValueToDomain(type, cast, value, domain);
    }

    bool was_string = cvar->type == CvarType::String && cast.integer;
    if (was_string)
        FreeString(&cvar->current);

    if (cvar->type != CvarType::String && ShouldFreeCurrentString(cvar))
        FreeString(&cvar->current);
    cvar->current.string = nullptr;

    if (ShouldFreeLatchedString(cvar))
        FreeString(&cvar->latched);
    cvar->latched.string = nullptr;

    if (ShouldFreeResetString(cvar))
        FreeString(&cvar->reset);
    cvar->reset.string = nullptr;

    UpdateResetValue(cvar, value);
    UpdateValue(cvar, cast);

    modified_flags |= flags;

    if (was_string)
        FreeString(&cast);
}

void CvarSystemLocal::PerformUnregistration(Cvar* cvar)
{
    if (!HasEnumFlag(cvar->flags, CvarFlags::External)) {
        cvar->flags |= CvarFlags::External;
        cvar->name = strdup(cvar->name);
    }

    if (cvar->type != CvarType::String) {
        cvar->current.string = strdup(ValueToString(cvar, cvar->latched));
        if (ShouldFreeLatchedString(cvar))
            FreeString(&cvar->latched);
        cvar->latched.string = cvar->current.string;

        if (ShouldFreeResetString(cvar))
            FreeString(&cvar->reset);

        CvarValue reset_string{};
        AssignResetStringValue(cvar, &reset_string, ValueToString(cvar, cvar->reset));
        cvar->reset = reset_string;
        cvar->type = CvarType::String;
    }
}

void CvarSystemLocal::SetVariant(Cvar* cvar, CvarValue value, CvarSource source)
{
    if (!cvar || !cvar->name || !cvar->name[0])
        return;

    if (!ValueInDomain(cvar->type, value, cvar->domain)) {
        LogWarn("'{}' is not a valid value for cvar '{}'", ValueToString(cvar, value), cvar->name);
        PrintDomain(cvar->type, cvar->domain);

        if (cvar->type == CvarType::Enum)
            SetVariant(cvar, cvar->reset, source);

        return;
    }

    if (!CanChangeValue(cvar, value, source))
        return;

    if (HasEnumFlag(cvar->flags, CvarFlags::Latch)) {
        SetLatchedValue(cvar, value);

        if (!ValuesEqual(cvar->type, cvar->latched, cvar->current))
            LogInfo("{} will be changed upon restarting", cvar->name);

        return;
    }

    if (ValuesEqual(cvar->type, cvar->current, value)) {
        SetLatchedValue(cvar, cvar->current);
    } else {
        modified_flags |= cvar->flags;

        bool should_free = false;
        CvarValue old_string{};
        CvarValue current_string{};

        switch (cvar->type) {
        case CvarType::String:
            should_free = ShouldFreeCurrentString(cvar);
            if (should_free)
                old_string.string = cvar->current.string;

            AssignCurrentStringValue(cvar, &current_string, value.string);
            cvar->current.string = current_string.string;

            if (ShouldFreeLatchedString(cvar))
                FreeString(&cvar->latched);
            cvar->latched.string = cvar->current.string;

            if (should_free)
                FreeString(&old_string);
            break;
        default:
            cvar->current = value;
            cvar->latched = value;
            break;
        }

        cvar->modified = true;
    }
}
void CvarSystemLocal::SetLatchedValue(Cvar* cvar, CvarValue value);

// --------------------------------
// CvarSystemLocal::StringToValue
// --------------------------------
CvarValue CvarSystemLocal::StringToValue(CvarType type, CvarLimits domain, const char* string)
{
    CvarValue value{};

    switch (type) {
    case CvarType::Bool:
        value.boolean = StringToBool(string);
        break;
    case CvarType::Float:
        value.decimal = StringToFloat(string);
        break;
    // TODO: vector & color
    case CvarType::Int:
        value.integer = StringToInt(string);
        break;
    case CvarType::Enum:
        value.integer = StringToEnum(string, &domain);
        break;
    case CvarType::String:
        value.string = string;
        break;
    default:
        break;
    }

    return value;
}

bool CvarSystemLocal::StringToBool(const char* string)
{
    // TODO: support for "on", "off", "yes", "no", "true", "false"
    return std::atoi(string) != 0;
}

i32 CvarSystemLocal::StringToInt(const char* string)
{
    return std::atoi(string);
}

f32 CvarSystemLocal::StringToFloat(const char* string)
{
    return std::atof(string);
}

// TODO: consider std::string instead
i32 CvarSystemLocal::StringToEnum(const char* string, CvarLimits* domain)
{
    i32 index = INVALID_ENUM_INDEX;

    for (index = 0; index < domain->enumeration.string_count; ++index) {
        if (strcasecmp(string, domain->enumeration.strings[index]) == 0)
            return index;
    }

    index = 0;
    for (const char* digit = string; *digit; ++digit) {
        if (*digit < '0' || *digit > '9')
            return INVALID_ENUM_INDEX;

        index = index * 10 + (*digit - '0');
    }

    if (index >= 0 && index < domain->enumeration.string_count)
        return index;

    for (index = 0; index < domain->enumeration.string_count; ++index) {
        if (strncasecmp(string, domain->enumeration.strings[index], strlen(string)) == 0)
            return index;
    }

    return INVALID_ENUM_INDEX;
}

CvarValue CvarSystemLocal::ClampValueToDomain(CvarType type, CvarValue value, CvarValue reset,
                                              CvarLimits domain)
{
    switch (type) {
    case CvarType::Bool:
        value.boolean = value.boolean != 0;
        break;
    case CvarType::Float:
        if (domain.decimal.min <= value.decimal) {
            if (value.decimal > domain.decimal.max)
                value.decimal = domain.decimal.max;
        } else {
            value.decimal = domain.decimal.min;
        }
        break;
    // TODO: vectors & color
    case CvarType::Int:
        if (domain.integer.min <= value.integer) {
            if (value.integer > domain.integer.max)
                value.integer = domain.integer.max;
        } else {
            value.integer = domain.integer.min;
        }
        break;
    case CvarType::Enum:
        if (value.integer < 0 || value.integer >= domain.enumeration.string_count)
            value.integer = reset.integer;
        break;
    default:
        break;
    }

    return value;
}

void CvarSystemLocal::UpdateValue(Cvar* cvar, CvarValue value)
{
    switch (cvar->type) {
        // TODO: vectors
    case CvarType::String:
        if (value.string != cvar->current.string) {
            bool should_free = ShouldFreeCurrentString(cvar);
            CvarValue old_string{};
            if (should_free)
                old_string.string = cvar->current.string;

            CvarValue current_string{};
            AssignCurrentStringValue(cvar, &current_string, value.string);
            cvar->current = current_string;

            if (ShouldFreeLatchedString(cvar))
                FreeString(&cvar->latched);
            cvar->latched.string = cvar->current.string;

            if (should_free)
                FreeString(&old_string);
        }
    default:
        cvar->current = value;
        cvar->latched = value;
        break;
    }
}

void CvarSystemLocal::UpdateResetValue(Cvar* cvar, CvarValue value)
{
    switch (cvar->type) {
        // TODO: vectors
    case CvarType::String:
        if (cvar->reset.string != value.string) {
            bool should_free = ShouldFreeResetString(cvar);
            CvarValue old_string{};
            if (should_free)
                old_string.string = cvar->reset.string;

            CvarValue reset_string{};
            AssignResetStringValue(cvar, &reset_string, value.string);
            cvar->reset = reset_string;

            if (should_free)
                FreeString(&old_string);
        }
        break;
    default:
        cvar->reset = value;
        break;
    }
}

u64 CvarSystemLocal::GenerateHashValue(const char* fname)
{
    if (!fname) // TODO: error out
        return 0;

    u64 hash = 0;
    std::size_t i = 0;
    while (fname[i] != '\0') {
        int letter = std::tolower(fname[i]);
        hash += static_cast<u64>(letter) * (i + 119);
        ++i;
    }

    hash &= (FILE_HASH_SIZE - 1);
    return hash;
}

} // namespace iocod
