// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_VERSION_H
#define CORE_VERSION_H

// ***** Autogenerated by tools/make_version.py *****
// ***** Do not edit this file directly! *****

namespace iocod {

inline constexpr int version_major = 0;
inline constexpr int version_minor = 1;
inline constexpr int version_patch = 0;

// optional pre-release and build metadata, if any
//inline constexpr const char* version_prerelease = "";
//inline constexpr const char* version_build_metadata = "";

// last git commit information, if built from git repo
inline constexpr const char* version_git_commit_hash = "1c9413d";
inline constexpr const char* version_git_branch = "new-dev";
inline constexpr int version_git_commits = 51;

// major.minor.patch
inline constexpr const char* version_string = "0.1.0";

// major.minor.patch(-prerelease)(+buildmetadata)
inline constexpr const char* version_string_full = "0.1.0";

// build commits (branch-commithash)
inline constexpr const char* version_build = "build 51 (new-dev-1c9413d)";

// version encoding utilities
inline constexpr int version_major_multiplier = 1000000;
inline constexpr int version_minor_multiplier = 1000;

inline constexpr int VersionEncode(int major, int minor, int patch)
{
    return (major * version_major_multiplier) + (minor * version_minor_multiplier) + patch;
}

inline constexpr int VersionDecodeMajor(int version)
{
    return version / version_major_multiplier;
}

inline constexpr int VersionDecodeMinor(int version)
{
    return (version % version_major_multiplier) / version_minor_multiplier;
}

inline constexpr int VersionDecodePatch(int version)
{
    return version % version_minor_multiplier;
}

} // namespace iocod

// ***** Autogenerated by tools/make_version.py *****
// ***** Do not edit this file directly! *****

#endif // CORE_VERSION_H
