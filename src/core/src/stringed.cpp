// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <core/stringed.h>
#include <core/string.h>
#include <stdexcept>

namespace iocod {

// --------------------------------
// StringReference::~StringReference
// --------------------------------
StringReference::~StringReference()
{
    // TEMP
    delete[] name;
    delete[] string;

    Alternate* alt = alternates;
    while (alt) {
        Alternate* next = alt->next;
        delete[] alt->string;
        delete alt;
        alt = next;
    }
}

// --------------------------------
// StringReference::SetReference
// --------------------------------
void StringReference::SetReference(const char* reference)
{
    // TEMP
    name = new char[strlen(reference) + 1];
    Strcpy(name, reference);
    name[strlen(reference)] = '\0';
}

// --------------------------------
// StringReference::SetDefaultString
// --------------------------------
void StringReference::SetDefaultString(const char* default_string)
{
    // TEMP
    string = new char[strlen(default_string) + 1];
    Strcpy(string, default_string);
    string[strlen(default_string)] = '\0';
}

// --------------------------------
// StringReference::GetString
// --------------------------------
const char* StringReference::GetString() const
{
    // TODO: get language from cvar
    return string;
}

// --------------------------------
// StringReference::GetString
// --------------------------------
const char* StringReference::GetString(StringLanguage language) const
{
    if (language == StringLanguage::English)
        return string;

    for (Alternate* alt = alternates; alt; alt = alt->next) {
        if (alt->language == language)
            return alt->string;
    }

    return string;
}

// --------------------------------
// StringReference::AddAlternate
// --------------------------------
void StringReference::AddAlternate(StringLanguage language, const char* alternate)
{
    if (language == StringLanguage::English)
        return;

    for (Alternate* alt = alternates; alt; alt = alt->next) {
        if (alt->language == language)
            throw std::runtime_error{"Trying to add duplicate language string in StringReference"};
    }

    Alternate* alt = new Alternate;
    alt->language = language;
    alt->string = new char[strlen(alternate) + 1];
    Strcpy(alt->string, alternate);
    alt->string[strlen(alternate)] = '\0';

    alt->next = alternates;
    alternates = alt;
}

} // namespace iocod
