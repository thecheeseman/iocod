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
std::string Capitalize(const std::string& string)
{
    std::string result = string;
    if (!result.empty())
        result.front() = narrow_cast<char>(std::toupper(result.front()));
    return result;
}

// --------------------------------
// StringUtilities::Clean
// --------------------------------
std::string Clean(const std::string& string)
{
    return Replace(string, "[^ -~]", "");
}

// --------------------------------
// StringUtilities::CompareCaseInsensitive
// --------------------------------
bool CompareCaseInsensitive(const std::string& string1, const std::string& string2)
{
    return ToLower(string1) == ToLower(string2);
}

// --------------------------------
// StringUtilities::Contains
// --------------------------------
bool Contains(const std::string& string, const std::string& substring)
{
    return string.find(substring) != std::string::npos;
}

bool Contains(const std::string& string, const char character)
{
    return Contains(string, std::string(1, character));
}

// --------------------------------
// StringUtilities::EndsWith
// --------------------------------
bool EndsWith(const std::string& string, const std::string& substring)
{
    const auto start = string.size() - substring.size();
    const auto result = string.find(substring, start);
    return (result == start) && (result != std::string::npos);
}

bool EndsWith(const std::string& string, const char character)
{
    return !string.empty() && (string.back() == character);
}

// --------------------------------
// StringUtilities::Escape
// --------------------------------
std::string Escape(const std::string& string)
{
    std::unordered_map<char, std::string> escape_map = {
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

    std::string result;
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
std::vector<std::string> GetLines(const std::string& string)
{
    return Split(string, "\n\r");
}

// --------------------------------
// StringUtilities::Replace
// --------------------------------
std::string Replace(const std::string& string, const std::string& from, const std::string& to)
{
    const std::regex regex(from);
    return std::regex_replace(string, regex, to);
}

// --------------------------------
// StringUtilities::Split
// --------------------------------
std::vector<std::string> Split(const std::string& string, const std::string& delimiters)
{
    size_t start = 0;
    size_t end = string.find_first_of(delimiters);

    std::vector<std::string> result;

    while (end <= std::string::npos) {
        const std::string token = string.substr(start, end - start);
        if (!token.empty())
            result.push_back(token);

        if (end == std::string::npos)
            break;

        start = end + 1;
        end = string.find_first_of(delimiters, start);
    }

    return result;
}

// --------------------------------
// StringUtilities::SplitString
// --------------------------------
std::vector<std::string> Split(const std::string& string, const char delimiter)
{
    return Split(string, std::string(1, delimiter));
}

// --------------------------------
// StringUtilities::StartsWith
// --------------------------------
bool StartsWith(const std::string& string, const std::string& substring)
{
    return string.rfind(substring, 0) == 0;
}

bool StartsWith(const std::string& string, const char character)
{
    return !string.empty() && (string.front() == character);
}

// --------------------------------
// StringUtilities::ToBool
// --------------------------------
bool ToBool(const std::string& string)
{
    const std::string lower = ToLower(string);
    return lower == "true" || lower == "yes" || lower == "1";
}

// --------------------------------
// StringUtilities::ToLower
// --------------------------------
std::string ToLower(const std::string& string)
{
    std::string result = string;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

// --------------------------------
// StringUtilities::ToUpper
// --------------------------------
std::string ToUpper(const std::string& string)
{
    std::string result = string;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

// --------------------------------
// StringUtilities::Tokenize
// --------------------------------
std::vector<std::string> Tokenize(const std::string& string)
{
    return Split(string, " \t\r\n");
}

// --------------------------------
// StringUtilities::Trim
// --------------------------------
void Trim(std::string& string)
{
    TrimLeft(string);
    TrimRight(string);
}

// --------------------------------
// StringUtilities::TrimCopy
// --------------------------------
std::string TrimCopy(std::string string)
{
    Trim(string);
    return string;
}

// --------------------------------
// StringUtilities::TrimLeft
// --------------------------------
void TrimLeft(std::string& string)
{
    string.erase(string.begin(), std::find_if(string.begin(), string.end(),
                                              [](int ch) { return !std::isspace(ch); }));
}

// --------------------------------
// StringUtilities::TrimLeftCopy
// --------------------------------
std::string TrimLeftCopy(std::string string)
{
    TrimLeft(string);
    return string;
}

// --------------------------------
// StringUtilities::TrimRight
// --------------------------------
void TrimRight(std::string& string)
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
std::string TrimRightCopy(std::string string)
{
    TrimRight(string);
    return string;
}

// --------------------------------
// StringUtilities::Unescape
// --------------------------------
std::string Unescape(const std::string& string)
{
    std::unordered_map<std::string, char> unescape_map = {
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

    std::string result;
    for (size_t i = 0; i < string.size(); ++i) {
        if (string[i] == '\\') {
            const std::string escape = string.substr(i, 2);
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
