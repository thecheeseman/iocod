// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_STRING_UTILITIES_H
#define CORE_STRING_UTILITIES_H

#include <core/platform.h>

#include <string>
#include <vector>

namespace iocod::StringUtilities {

/// @brief Capitalizes a string.
/// @param[in] string the string to capitalize
/// @return the capitalized string
IOCOD_EXPORT std::string Capitalize(const std::string& string);

/// @brief Cleans a string of non-printable characters.
/// @param[in] string the string to clean
/// @return the cleaned string
IOCOD_EXPORT std::string Clean(const std::string& string);

/// @brief Checks if a string contains a substring, ignoring case.
/// @param[in] string the string to check
/// @param[in] substring the substring to check for
/// @return true if the string contains the substring, false otherwise
IOCOD_EXPORT bool CompareCaseInsensitive(const std::string& string1, const std::string& string2);

/// @brief Checks if a string contains a substring.
/// @param[in] string the string to check
/// @param[in] substring the substring to check for
/// @return true if the string contains the substring, false otherwise
IOCOD_EXPORT bool Contains(const std::string& string, const std::string& substring);

/// @brief Checks if a string contains a character.
/// @param[in] string the string to check
/// @param[in] character the character to check for
/// @return true if the string contains the character, false otherwise
IOCOD_EXPORT bool Contains(const std::string& string, const char character);

/// @brief Checks if a string ends with a substring.
/// @param[in] string the string to check
/// @param[in] substring the substring to check for
/// @return true if the string ends with the substring, false otherwise
IOCOD_EXPORT bool EndsWith(const std::string& string, const std::string& substring);

/// @brief Checks if a string ends with a character.
/// @param[in] string the string to check
/// @param[in] character the character to check for
/// @return true if the string ends with the character, false otherwise
IOCOD_EXPORT bool EndsWith(const std::string& string, const char character);

/// @brief Escapes a string.
/// @param[in] string the string to escape
/// @return the escaped string
IOCOD_EXPORT std::string Escape(const std::string& string);

/// @brief Gets the lines of a string.
/// @param[in] string the string to get the lines of
/// @return a vector of strings
IOCOD_EXPORT std::vector<std::string> GetLines(const std::string& string);

/// @brief Replace all instances of a substring with another string.
/// @param[in] string the string to replace in
/// @param[in] from regex expression to replace
/// @param[in] to the string to replace with
/// @return the replaced string
IOCOD_EXPORT std::string Replace(const std::string& string, const std::string& from,
                               const std::string& to);

/// @brief Splits a string into a vector of strings using the specified delimiter(s).
/// @param[in] string the string to split
/// @param[in] delimiter the delimiter(s) to use
/// @return a vector of strings
IOCOD_EXPORT std::vector<std::string> Split(const std::string& string, const std::string& delimiter);

/// @brief Splits a string into a vector of strings using the specified delimiter.
/// @param[in] string the string to split
/// @param[in] delimiter the delimiter to use
/// @return a vector of strings
IOCOD_EXPORT std::vector<std::string> Split(const std::string& string, const char delimiter);

/// @brief Checks if a string starts with a substring.
/// @param[in] string the string to check
/// @param[in] substring the substring to check for
/// @return true if the string starts with the substring, false otherwise
IOCOD_EXPORT bool StartsWith(const std::string& string, const std::string& substring);

/// @brief Checks if a string starts with a character.
/// @param[in] string the string to check
/// @param[in] character the character to check for
/// @return true if the string starts with the character, false otherwise
IOCOD_EXPORT bool StartsWith(const std::string& string, const char character);

/// @brief Converts a string to a boolean.
/// @param[in] string the string to convert
/// @return the converted boolean
IOCOD_EXPORT bool ToBool(const std::string& string);

/// @brief Converts a string to lowercase.
/// @param[in] string the string to convert
/// @return the converted string
IOCOD_EXPORT std::string ToLower(const std::string& string);

/// @brief Converts a string to uppercase.
/// @param string the string to convert
/// @return the converted string
IOCOD_EXPORT std::string ToUpper(const std::string& string);

/// @brief Tokenizes a string into a vector of strings (splits by whitespace, newlines).
/// @param[in] string the string to tokenize
/// @return a vector of strings
IOCOD_EXPORT std::vector<std::string> Tokenize(const std::string& string);

/// @brief Trims whitespace from the beginning and end of a string.
/// @param[in] string the string to trim
IOCOD_EXPORT void Trim(std::string& string);

/// @brief Trims whitespace from the beginning and end of a string.
/// @param[in] string the string to trim
/// @return the trimmed string
IOCOD_EXPORT std::string TrimCopy(std::string string);

/// @brief Trims whitespace from the left side of a string.
/// @param[in] string the string to trim
IOCOD_EXPORT void TrimLeft(std::string& string);

/// @brief Trims whitespace from the left side of a string.
/// @param[in] string the string to trim
/// @return copy of the trimmed string
IOCOD_EXPORT std::string TrimLeftCopy(std::string string);

/// @brief Trims whitespace from the right side of a string.
/// @param[in] string the string to trim
IOCOD_EXPORT void TrimRight(std::string& string);

/// @brief Trims whitespace from the right side of a string.
/// @param[in] string the string to trim
/// @return copy of the trimmed string
IOCOD_EXPORT std::string TrimRightCopy(std::string string);

/// @brief Unescapes a string.
/// @param[in] string the string to unescape
/// @return the unescaped string
IOCOD_EXPORT std::string Unescape(const std::string& string);

} // namespace iocod::StringUtilities

#endif // CORE_STRING_UTILITIES_H
