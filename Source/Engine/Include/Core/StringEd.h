// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_STRINGED_H
#define CORE_STRINGED_H

namespace iocod {

// StringEd files consist of a sequence of records, each of which contains
// at a minimum:
//
// REFERENCE        REF_NAME
// LANG_ENGLISH     "STRING"
//
// with an optional number of additional LANG_XXX strings. Language strings
// may contain "#same" which indicates that the default LANG_ENGLISH string
// should be used.
//
// Files begin with a header:
//
// VERSION "1"
// CONFIG "F:\projects\mk\bin\StringEd\StringEd.cfg"
// FILENOTES ""
//
// Header fields can be effectively ignored. All files MUST end with:
//
// ENDMARKER

enum class StringLanguage {
    English,
    French,
    German,
    Italian,
    Spanish,
    British,
    Russian,
    Polish,
    Korean,
    Taiwanese,
    Japanese,
    Chinese,
    Thai,
    Leet
};

inline constexpr int max_languages = 14;

class IOCOD_API StringReference {
public:
    constexpr StringReference() = default;

    ~StringReference();
    StringReference(const StringReference&) = delete;
    StringReference& operator=(const StringReference&) = delete;

    /// Construct a StringReference from a reference name and default English string.
    explicit StringReference(const char* reference, const char* default_string)
    {
        SetReference(reference);
        SetDefaultString(default_string);
    }

    /// Construct a StringReference from a reference name, default English string,
    /// with an alternate string for a given language.
    explicit StringReference(const char* reference, const char* default_string,
                             StringLanguage language, const char* alternate)
    {
        SetReference(reference);
        SetDefaultString(default_string);
        AddAlternate(language, alternate);
    }

    /// Set the reference string.
    void SetReference(const char* reference);

    constexpr const char* Reference() const { return name; }

    /// Set the default English string.
    void SetDefaultString(const char* default_string);

    /// Get the default English string.
    constexpr const char* DefaultString() const { return string; }

    /// Attempt to get the string based on cvar cl_language. If the string
    /// doesn't exist, or the cvar is invalid, return the default English string.
    const char* GetString() const;

    /// Attempt to get the string for a given language. If the string doesn't
    /// exist, return the default English string.
    const char* GetString(StringLanguage language) const;

    /// Add an different language string with the given language.
    void AddAlternate(StringLanguage language, const char* alternate);

private:
    char* name{nullptr};
    char* string{nullptr}; // default english string

    struct Alternate {
        StringLanguage language;
        char* string{nullptr};
        Alternate* next{nullptr};
    };

    Alternate* alternates{nullptr};
};

} // namespace iocod

#endif // CORE_STRINGED_H
