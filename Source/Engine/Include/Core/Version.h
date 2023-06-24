// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_VERSION_H
#define CORE_VERSION_H

// ***** Autogenerated by Tools/MakeVersion.py *****
// ***** Do not edit this file directly! *****

namespace iocod {

inline constexpr int kVersionMajor = 0;
inline constexpr int kVersionMinor = 1;
inline constexpr int kVersionPatch = 0;

// optional pre-release and build metadata, if any
//inline constexpr const char* kVersionPrerelease = "";
//inline constexpr const char* kVersionBuildMetadata = "";

// last git commit information, if built from git repo
inline constexpr const char* kVersionGitCommitHash = "eb491f6";
inline constexpr const char* kVersionGitBranch = "new-dev";
inline constexpr int kVersionGitCommits = 55;

// major.minor.patch
inline constexpr const char* kVersionString = "0.1.0";

// major.minor.patch(-prerelease)(+buildmetadata)
inline constexpr const char* kVersionStringFull = "0.1.0";

// build commits (branch-commithash)
inline constexpr const char* kVersionBuild = "build 55 (new-dev-eb491f6)";

// version encoding utilities
inline constexpr int kVersionMajorMultiplier = 1000000;
inline constexpr int kVersionMinorMultiplier = 1000;

constexpr int VersionEncode(const int major, const int minor, const int patch)
{
    return (major * kVersionMajorMultiplier) + (minor * kVersionMinorMultiplier) + patch;
}

constexpr int VersionDecodeMajor(const int version)
{
    return version / kVersionMajorMultiplier;
}

constexpr int VersionDecodeMinor(const int version)
{
    return (version % kVersionMajorMultiplier) / kVersionMinorMultiplier;
}

constexpr int VersionDecodePatch(const int version)
{
    return version % kVersionMinorMultiplier;
}

} // namespace iocod

// ***** Autogenerated by Tools/MakeVersion.py *****
// ***** Do not edit this file directly! *****

#endif // CORE_VERSION_H