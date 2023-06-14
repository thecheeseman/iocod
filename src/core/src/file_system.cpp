// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <array>
#include <core/command_system.h>
#include <core/file_system.h>
#include <core/system.h>
#include <miniz.h>
#include <regex>
#include <variant>

namespace iocod {

namespace fs = std::filesystem;

// TODO: put somewhere else
enum class Language {
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

struct PackedFile {
    String name;

    size_t index;
    size_t offset;
    size_t size;
    size_t size_uncompressed;

    mz_zip_archive_file_stat stat;
};

// .pak, .pk3
struct PakFile {
    FilePath path;
    String filename;
    String basename;
    String gamename;

    mz_zip_archive* handle;

    i32 checksum;
    i32 pure_checksum;
    size_t num_files;
    size_t hash_size;

    bool referenced;

    std::unordered_map<String, PackedFile> files;

    PakFile() = default;

    ~PakFile() { mz_zip_reader_end(handle); }
};

struct Directory {
    FilePath path;
    String game;
};

// each search path is either a pak file or a directory
struct SearchPath {
    std::variant<PakFile, Directory> path;
    bool localized;
    Language language;

    enum {
        Pak,
        Dir
    };
};

struct FileHandleData {
    union {
        FILE* o;
        mz_zip_archive* z;
    } handle;

    size_t offset;
    size_t size;
    size_t zip_offset;
    bool zip_file;
    bool streamed;

    String name;
};

std::vector<SearchPath> search_paths;
std::array<FileHandleData, 1024> open_files;

class FileSystemLocal final : public IFileSystem {
public:
    void Initialize() override;
    void Shutdown() override;

    bool Initialized() override { return initialized; }

    void Restart(int checksum) override;

    std::vector<FilePath> ListFiles(FilePath path, String filter, bool subdirs,
                                    bool follow_symlinks) override;
    bool FileExists(FilePath path) noexcept override;
    void Rename(FilePath from, FilePath to) noexcept override;
    void Copy(FilePath from, FilePath to) noexcept override;
    Expected<FilePath, String> BuildOSPath(const String& base, const String& game,
                                           const String& qpath) noexcept override;

private:
    void Startup(const char* game);
    void AddGameDirectory(String path, String dir);
    void SetRestrictions();
    void RegisterCvars();

    void AddCommands() noexcept;
    static void AddDelayedCommands() noexcept;
    void RemoveCommands() noexcept;

    bool initialized = false;

    String last_valid_base;
    String last_valid_game;

    // all paths must be relative to base_path
    FilePath base_path;
};

FileSystemLocal local;
IFileSystem* file_system = &local;

// --------------------------------
// FileSystemLocal::Initialize
// --------------------------------
void FileSystemLocal::Initialize()
{
    base_path = fs::current_path();

    // StartupVariable() calls

    // SEH_InitLanguage

    Startup(BASEGAME);
    SetRestrictions();

    // read cfg

    // last_valid_base = fs_basepath->current.string;
    // last_valid_game = fs_gamedir->current.string;
}

// --------------------------------
// FileSystemLocal::Shutdown
// --------------------------------
void FileSystemLocal::Shutdown()
{
    for (auto& f : open_files) {
        // if (f.file_size)
        // close it
    }

    search_paths.clear();

    RemoveCommands();
}

// --------------------------------
// FileSystemLocal::Restart
// --------------------------------
void FileSystemLocal::Restart(int checksum)
{
    Shutdown();

    // checksumfeed

    Startup(BASEGAME);
    SetRestrictions();

    // read cfg

    // last_valid_base = fs_basepath->current.string;
    // last_valid_game = fs_gamedir->current.string;
}

std::vector<FilePath> FileSystemLocal::ListFiles(FilePath path, String filter, bool subdirs,
                                                 bool follow_symlinks)
{
    std::regex match{filter};
    auto matches_extension = [&](const FilePath& p) {
        if (filter.empty())
            return true;

        return std::regex_match(p.extension().string(), match);
    };

    auto options = fs::directory_options::skip_permission_denied;
    if (follow_symlinks)
        options |= fs::directory_options::follow_directory_symlink;

    std::vector<FilePath> files;
    try {
        if (subdirs) {
            std::ranges::for_each(
                fs::recursive_directory_iterator{path, options}, [&](const auto& entry) {
                    if (entry.is_regular_file() && matches_extension(entry.path()))
                        files.push_back(entry.path());
                });
        } else {
            std::ranges::for_each(fs::directory_iterator{path, options}, [&](const auto& entry) {
                if (entry.is_regular_file() && matches_extension(entry.path()))
                    files.push_back(entry.path());
            });
        }
    } catch (fs::filesystem_error& ex) {
        LogWarn("[FileSystem] ListFiles: {}\n", ex.what());
    }

    return files;
}

bool FileSystemLocal::FileExists(FilePath path) noexcept
{
    std::error_code ec;
    bool exists = fs::exists(path, ec);

    if (ec) {
        LogWarn("[FileSystem] FileExists: {}\n", ec.message());
        return false;
    }

    return exists;
}

void FileSystemLocal::Rename(FilePath from, FilePath to) noexcept
{
    std::error_code ec;
    fs::rename(from, to, ec);

    if (ec)
        LogWarn("[FileSystem] Rename: {}\n", ec.message());
}

void FileSystemLocal::Copy(FilePath from, FilePath to) noexcept
{
    std::error_code ec;
    fs::copy(from, to, ec);

    if (ec)
        LogWarn("[FileSystem] Copy: {}\n", ec.message());
}

Expected<FilePath, String> FileSystemLocal::BuildOSPath(const String& base, const String& game,
                                                        const String& qpath) noexcept
{
    FilePath path = qpath;

    if (path.is_absolute())
        return MakeUnexpected<String>("BuildOSPath: qpath must be relative");

    if (path.has_parent_path() && path.parent_path().compare("..") == 0)
        return MakeUnexpected<String>("BuildOSPath: qpath must not contain '..'");

    return {FilePath(base) / game / path.make_preferred()};
}

// ============================================================================
//
// local
//
// ============================================================================

// --------------------------------
// FileSystemLocal::Startup
// --------------------------------
void FileSystemLocal::Startup(const char* game)
{
    LogInfo("----- FileSystem::Startup({}) -----\n", game);

    RegisterCvars();

    // cdpath
    // basepath
    // homepath
    // AddGameDirectory(path, game);

    // TEMP
    AddGameDirectory("", game);

    AddCommands();
    // DisplayPath(true);
    //  CvarSystem->ClearModified(fs_gamedir);

    LogInfo("------------------------------\n");
}

// --------------------------------
// FileSystemLocal::AddGameDirectory
// --------------------------------
void FileSystemLocal::AddGameDirectory(String path, String dir)
{
    LogInfo(base_path.string());
}

// --------------------------------
// FileSystemLocal::SetRestrictions
// --------------------------------
void FileSystemLocal::SetRestrictions() {}

// --------------------------------
// FileSystemLocal::RegisterCvars
// --------------------------------
void FileSystemLocal::RegisterCvars() {}

// ============================================================================

class Command_path final : public IConsoleCommand {
public:
    void Execute(std::vector<String> args) override
    {
        // DisplayPath(true);
    }
};

class Command_fullpath final : public IConsoleCommand {
public:
    void Execute(std::vector<String> args) override
    {
        // DisplayPath(false);
    }
};

class Command_dir final : public IConsoleCommand {
public:
    void Execute(std::vector<String> args) override {}
};

class Command_fdir final : public IConsoleCommand {
public:
    void Execute(std::vector<String> args) override {}
};

class Command_touchfile final : public IConsoleCommand {
public:
    void Execute(std::vector<String> args) override {}
};

void FileSystemLocal::AddDelayedCommands() noexcept
{
    commandSystem->AddCommand("path", std::make_unique<Command_path>());
    commandSystem->AddCommand("fullpath", std::make_unique<Command_fullpath>());
    commandSystem->AddCommand("dir", std::make_unique<Command_dir>());
    commandSystem->AddCommand("fdir", std::make_unique<Command_fdir>());
    commandSystem->AddCommand("touchfile", std::make_unique<Command_touchfile>());
}

// --------------------------------
// FileSystemLocal::AddCommands
// --------------------------------
void FileSystemLocal::AddCommands() noexcept
{
    commandSystem->AddRegisterCallback(AddDelayedCommands);
}

// --------------------------------
// FileSystemLocal::RemoveCommands
// --------------------------------
void FileSystemLocal::RemoveCommands() noexcept
{
    commandSystem->RemoveCommand("path");
    commandSystem->RemoveCommand("fullpath");
    commandSystem->RemoveCommand("dir");
    commandSystem->RemoveCommand("fdir");
    commandSystem->RemoveCommand("touchfile");
}

// ============================================================================

} // namespace iocod
