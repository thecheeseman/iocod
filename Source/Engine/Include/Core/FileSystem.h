// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_FILE_SYSTEM_H
#define CORE_FILE_SYSTEM_H

#include <Core/Expected.h>
#include <Core/Types.h>
#include <Core/String.h>

#include <filesystem>
#include <vector>

namespace iocod {

inline constexpr const char* BASEGAME = "main";

using ssize_t = std::make_signed_t<size_t>;

using FilePath = std::filesystem::path;
using FileHandle = ssize_t;

inline constexpr size_t MAX_ZPATH = 256;

enum class FileOpenMode {
    Read,
    Write,
    Append
};

class BuildOSPathException : public std::runtime_error {
public:
    BuildOSPathException(const std::string& msg)
        : std::runtime_error(msg)
    {
    }
};

class IFileSystem {
public:
    ~IFileSystem() = default;

    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;

    virtual bool Initialized() = 0;
    virtual void Restart(int checksum) = 0;

    // general operations
    // TODO: noexcept
    virtual std::vector<FilePath> ListFiles(FilePath path, String filter = "", bool subdirs = false,
                                            bool follow_symlinks = false) = 0;
    virtual bool FileExists(FilePath path) noexcept = 0;
    virtual void Rename(FilePath from, FilePath to) noexcept = 0;
    virtual void Copy(FilePath from, FilePath to) noexcept = 0;

    /// @brief Construct a path to the sandboxed game dir "base/game/...".
    /// @param[in] base the base path or home path
    /// @param[in] game the game dir, usually "main"
    /// @param[in] qpath the actual path to the file
    /// @return the full path to the file or an error message
    virtual Expected<FilePath, String> BuildOSPath(const String& base, const String& game,
                                                   const String& qpath) noexcept = 0;
    /*
    // pak file operations
    virtual bool FileIsInPak(FilePath path) = 0;

    // raw file IO operations
    virtual size_t FileRead(void* buffer, size_t size, size_t count, FileHandle file) = 0;
    virtual size_t FileWrite(void* buffer, size_t size, size_t count, FileHandle file) = 0;
    virtual FileHandle FileOpen(Path path, FileOpenMode mode) = 0;
    virtual bool FileClose(FileHandle file) = 0;
    virtual ssize_t FileSize(FileHandle file) = 0;
    virtual ssize_t FileSeek(FileHandle file, ssize_t offset, ssize_t origin) = 0;
    */
};

extern IFileSystem* file_system;

} // namespace iocod

#endif // CORE_FILE_SYSTEM_H
