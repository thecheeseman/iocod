// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_STRING_UTILITIES_H
#define CORE_STRING_UTILITIES_H

#include <vector>

namespace iocod::StringUtilities {

/// @brief Capitalizes a string.
/// @param[in] string the string to capitalize
/// @return the capitalized string
IOCOD_EXPORT String Capitalize(const String& string);

/// @brief Cleans a string of non-printable characters.
/// @param[in] string the string to clean
/// @return the cleaned string
IOCOD_EXPORT String Clean(const String& string);

/// @brief Checks if a string contains a substring, ignoring case.
/// @param[in] string1 the string to check
/// @param[in] string2 the substring to check for
/// @return true if the string contains the substring, false otherwise
IOCOD_EXPORT bool CompareCaseInsensitive(const String& string1, const String& string2);

/// @brief Checks if a string contains a substring.
/// @param[in] string the string to check
/// @param[in] substring the substring to check for
/// @return true if the string contains the substring, false otherwise
IOCOD_EXPORT bool Contains(const String& string, const String& substring);

/// @brief Checks if a string contains a character.
/// @param[in] string the string to check
/// @param[in] character the character to check for
/// @return true if the string contains the character, false otherwise
IOCOD_EXPORT bool Contains(const String& string, const char character);

/// @brief Checks if a string ends with a substring.
/// @param[in] string the string to check
/// @param[in] substring the substring to check for
/// @return true if the string ends with the substring, false otherwise
IOCOD_EXPORT bool EndsWith(const String& string, const String& substring);

/// @brief Checks if a string ends with a character.
/// @param[in] string the string to check
/// @param[in] character the character to check for
/// @return true if the string ends with the character, false otherwise
IOCOD_EXPORT bool EndsWith(const String& string, const char character);

/// @brief Escapes a string.
/// @param[in] string the string to escape
/// @return the escaped string
IOCOD_EXPORT String Escape(const String& string);

/// @brief Gets the lines of a string.
/// @param[in] string the string to get the lines of
/// @return a vector of strings
IOCOD_EXPORT std::vector<String> GetLines(const String& string);

/// @brief Replace all instances of a substring with another string.
/// @param[in] string the string to replace in
/// @param[in] from regex expression to replace
/// @param[in] to the string to replace with
/// @return the replaced string
IOCOD_EXPORT String Replace(const String& string, const String& from,
                               const String& to);

/// @brief Splits a string into a vector of strings using the specified delimiter(s).
/// @param[in] string the string to split
/// @param[in] delimiter the delimiter(s) to use
/// @return a vector of strings
IOCOD_EXPORT std::vector<String> Split(const String& string, const String& delimiter);

/// @brief Splits a string into a vector of strings using the specified delimiter.
/// @param[in] string the string to split
/// @param[in] delimiter the delimiter to use
/// @return a vector of strings
IOCOD_EXPORT std::vector<String> Split(const String& string, const char delimiter);

/// @brief Checks if a string starts with a substring.
/// @param[in] string the string to check
/// @param[in] substring the substring to check for
/// @return true if the string starts with the substring, false otherwise
IOCOD_EXPORT bool StartsWith(const String& string, const String& substring);

/// @brief Checks if a string starts with a character.
/// @param[in] string the string to check
/// @param[in] character the character to check for
/// @return true if the string starts with the character, false otherwise
IOCOD_EXPORT bool StartsWith(const String& string, const char character);

/// @brief Converts a string to a boolean.
/// @param[in] string the string to convert
/// @return the converted boolean
IOCOD_EXPORT bool ToBool(const String& string);

/// @brief Converts a string to lowercase.
/// @param[in] string the string to convert
/// @return the converted string
IOCOD_EXPORT String ToLower(const String& string);

/// @brief Converts a string to uppercase.
/// @param string the string to convert
/// @return the converted string
IOCOD_EXPORT String ToUpper(const String& string);

/// @brief Tokenizes a string into a vector of strings (splits by whitespace, newlines).
/// @param[in] string the string to tokenize
/// @return a vector of strings
IOCOD_EXPORT std::vector<String> Tokenize(const String& string);

/// @brief Trims whitespace from the beginning and end of a string.
/// @param[in] string the string to trim
IOCOD_EXPORT void Trim(String& string);

/// @brief Trims whitespace from the beginning and end of a string.
/// @param[in] string the string to trim
/// @return the trimmed string
IOCOD_EXPORT String TrimCopy(String string);

/// @brief Trims whitespace from the left side of a string.
/// @param[in] string the string to trim
IOCOD_EXPORT void TrimLeft(String& string);

/// @brief Trims whitespace from the left side of a string.
/// @param[in] string the string to trim
/// @return copy of the trimmed string
IOCOD_EXPORT String TrimLeftCopy(String string);

/// @brief Trims whitespace from the right side of a string.
/// @param[in] string the string to trim
IOCOD_EXPORT void TrimRight(String& string);

/// @brief Trims whitespace from the right side of a string.
/// @param[in] string the string to trim
/// @return copy of the trimmed string
IOCOD_EXPORT String TrimRightCopy(String string);

/// @brief Unescapes a string.
/// @param[in] string the string to unescape
/// @return the unescaped string
IOCOD_EXPORT String Unescape(const String& string);

} // namespace iocod::StringUtilities

#endif // CORE_STRING_UTILITIES_H
