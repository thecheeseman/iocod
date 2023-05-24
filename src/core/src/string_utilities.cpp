// SPDX-FileCopyrightText: 2017 Joseph Morton
// SPDX-FileCopyrightText: 2022 Tomasz Galaj (Shot511)
// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: MIT
// SPDX-License-Identifier: MIT
// SPDX-License-Identifier: GPL-3.0-or-later

#include <core/narrow_cast.h>
#include <core/string_utilities.h>

#include <algorithm>
#include <regex>
#include <unordered_map>

namespace iocod::StringUtilities {

// --------------------------------
// StringUtilities::Capitalize
// --------------------------------
String Capitalize(const String& string)
{
    String result = string;
    if (!result.empty())
        result.front() = narrow_cast<char>(std::toupper(result.front()));
    return result;
}

// --------------------------------
// StringUtilities::Clean
// --------------------------------
String Clean(const String& string)
{
    return Replace(string, "[^ -~]", "");
}

// --------------------------------
// StringUtilities::CompareCaseInsensitive
// --------------------------------
bool CompareCaseInsensitive(const String& string1, const String& string2)
{
    return ToLower(string1) == ToLower(string2);
}

// --------------------------------
// StringUtilities::Contains
// --------------------------------
bool Contains(const String& string, const String& substring)
{
    return string.find(substring) != String::npos;
}

bool Contains(const String& string, const char character)
{
    return Contains(string, String(1, character));
}

// --------------------------------
// StringUtilities::EndsWith
// --------------------------------
bool EndsWith(const String& string, const String& substring)
{
    const auto start = string.size() - substring.size();
    const auto result = string.find(substring, start);
    return (result == start) && (result != String::npos);
}

bool EndsWith(const String& string, const char character)
{
    return !string.empty() && (string.back() == character);
}

// --------------------------------
// StringUtilities::Escape
// --------------------------------
String Escape(const String& string)
{
    std::unordered_map<char, String> escape_map = {
        {'\a', "\\a" },
        {'\b', "\\b" },
        {'\f', "\\f" },
        {'\n', "\\n" },
        {'\r', "\\r" },
        {'\t', "\\t" },
        {'\v', "\\v" },
        {'\\', "\\\\"},
        {'\'', "\\'" },
        {'\"', "\\\""},
    };

    String result;
    for (const char& c : string) {
        if (escape_map.find(c) != escape_map.end())
            result += escape_map[c];
        else
            result += c;
    }

    return result;
}

// --------------------------------
// StringUtilities::GetLines
// --------------------------------
std::vector<String> GetLines(const String& string)
{
    return Split(string, "\n\r");
}

// --------------------------------
// StringUtilities::Replace
// --------------------------------
String Replace(const String& string, const String& from, const String& to)
{
    const std::regex regex(from);
    return std::regex_replace(string, regex, to);
}

// --------------------------------
// StringUtilities::Split
// --------------------------------
std::vector<String> Split(const String& string, const String& delimiters)
{
    size_t start = 0;
    size_t end = string.find_first_of(delimiters);

    std::vector<String> result;

    while (end <= String::npos) {
        const String token = string.substr(start, end - start);
        if (!token.empty())
            result.push_back(token);

        if (end == String::npos)
            break;

        start = end + 1;
        end = string.find_first_of(delimiters, start);
    }

    return result;
}

// --------------------------------
// StringUtilities::SplitString
// --------------------------------
std::vector<String> Split(const String& string, const char delimiter)
{
    return Split(string, String(1, delimiter));
}

// --------------------------------
// StringUtilities::StartsWith
// --------------------------------
bool StartsWith(const String& string, const String& substring)
{
    return string.rfind(substring, 0) == 0;
}

bool StartsWith(const String& string, const char character)
{
    return !string.empty() && (string.front() == character);
}

// --------------------------------
// StringUtilities::ToBool
// --------------------------------
bool ToBool(const String& string)
{
    const String lower = ToLower(string);
    return lower == "true" || lower == "yes" || lower == "1";
}

// --------------------------------
// StringUtilities::ToLower
// --------------------------------
String ToLower(const String& string)
{
    String result = string;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](char c) { return narrow_cast<char>(std::tolower(c)); });
    return result;
}

// --------------------------------
// StringUtilities::ToUpper
// --------------------------------
String ToUpper(const String& string)
{
    String result = string;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](char c) { return narrow_cast<char>(std::toupper(c)); });
    return result;
}

// --------------------------------
// StringUtilities::Tokenize
// --------------------------------
std::vector<String> Tokenize(const String& string)
{
    return Split(string, " \t\r\n");
}

// --------------------------------
// StringUtilities::Trim
// --------------------------------
void Trim(String& string)
{
    TrimLeft(string);
    TrimRight(string);
}

// --------------------------------
// StringUtilities::TrimCopy
// --------------------------------
String TrimCopy(String string)
{
    Trim(string);
    return string;
}

// --------------------------------
// StringUtilities::TrimLeft
// --------------------------------
void TrimLeft(String& string)
{
    string.erase(string.begin(), std::find_if(string.begin(), string.end(),
                                              [](int ch) { return !std::isspace(ch); }));
}

// --------------------------------
// StringUtilities::TrimLeftCopy
// --------------------------------
String TrimLeftCopy(String string)
{
    TrimLeft(string);
    return string;
}

// --------------------------------
// StringUtilities::TrimRight
// --------------------------------
void TrimRight(String& string)
{
    // clang-format off
    string.erase(std::find_if(string.rbegin(), string.rend(),
                              [](int ch) { return !std::isspace(ch); }).base(),
                 string.end());
    // clang-format on
}

// --------------------------------
// StringUtilities::TrimRightCopy
// --------------------------------
String TrimRightCopy(String string)
{
    TrimRight(string);
    return string;
}

// --------------------------------
// StringUtilities::Unescape
// --------------------------------
String Unescape(const String& string)
{
    std::unordered_map<String, char> unescape_map = {
        {"\\a",  '\a'},
        {"\\b",  '\b'},
        {"\\f",  '\f'},
        {"\\n",  '\n'},
        {"\\r",  '\r'},
        {"\\t",  '\t'},
        {"\\v",  '\v'},
        {"\\\\", '\\'},
        {"\\\'", '\''},
        {"\\\"", '\"'},
    };

    String result;
    for (size_t i = 0; i < string.size(); ++i) {
        if (string[i] == '\\') {
            const String escape = string.substr(i, 2);
            if (unescape_map.find(escape) != unescape_map.end()) {
                result += unescape_map[escape];
                ++i;
                continue;
            }
        }
        result += string[i];
    }

    return result;
}

} // namespace iocod::StringUtilities
