// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_COMMON_H
#define CORE_COMMON_H

namespace iocod {

enum class ErrorCode {
    Fatal,
    Drop,
    ServerDisconnect,
    NeedCD
};

enum class SystemEventType {
    None,
    Key,
    Char,
    Mouse,
    JoystickAxis,
    Console,
    Packet
};

struct SystemEvent {
    int time;
    SystemEventType type;
    int value;
    int value2;
    int ptr_length;
    void* ptr;
};

enum class PrintType {
    All,
    Developer,
    Warning,
    Error
};

class ICommon {
public:
    virtual ~ICommon() = default;

    virtual void Initialize(int argc, char** argv, const char* cmdline) = 0;
    virtual void Shutdown(const char* msg) = 0;

    // TODO:
    // - move into private implementation (not all of these are public)
    #if 0
    virtual void AddStartupCommands() = 0;
    virtual void AppendCDKey(const char* filename) = 0;
    virtual void BeginRedirect(char* buffer, int buffersize, void (*flush)(char*)) = 0;
    virtual u32 BlockChecksum(const void* buffer, int length) = 0;
    virtual u32 BlockChecksumKey(void* buffer, int length, int key) = 0;
    virtual int ClampMsec(int msec) = 0;
    virtual void ClearTempMemory() = 0;
    virtual void Close() = 0;
    virtual int Compress(char* data_p) = 0;
    virtual int ConfigureChecksum(char* p1, u32 p2) = 0;
    virtual void ConfigureFileChanged() = 0;
    virtual void CvarDump(int p1) = 0;
    virtual void DefaultExtension(char* path, int max_size, const char* extension) = 0;
    virtual void DPrintf(const char* fmt, ...) = 0;
    virtual void EndRedirect() = 0;
    virtual void Error(ErrorCode code, const char* fmt, ...) = 0;
    virtual void ErrorCleanup() = 0;
    virtual int EventLoop() = 0;
    virtual int Filter(char* filter, char* name, bool case_sensitive) = 0;
    virtual int FilterPath(char* filter, char* name, bool case_sensitive) = 0;
    virtual void Frame() = 0;
    virtual SystemEvent GetEvent() = 0; // GetEvent(__out SystemEvent* event)
    virtual SystemEvent GetRealEvent() = 0; // GetRealEvent(__out SystemEvent* event)
    virtual int HashKey(char* string, int maxlen) = 0;
    virtual void InitHunkMemory() = 0;
    virtual void InitJournaling() = 0;
    virtual void InitPushEvent() = 0;
    // virtual void InitZoneMemory() = 0; // unused
    virtual bool LoadCvarsFromBuffer(char** p1, size_t p2, int p3, char* p4) = 0;
    // virtual void Memcpy(void* dest, void* src, size_t count) = 0;
    // virtual void Memset(void* dest, void* src, size_t count) = 0;
    virtual int Milliseconds() = 0;
    virtual int ModifyMsec(int msec) = 0;
    virtual void Printf(const char* fmt, ...) = 0;
    virtual void PrintMessage(PrintType type, const char* msg) = 0;
    virtual void PumpMessageLoop() = 0;
    virtual void PushEvent(SystemEvent* event) = 0;
    virtual void ReadCDKey(const char* filename) = 0;
    virtual time_t RealTime(void* qtime) = 0;
    virtual void Restart() = 0;
    virtual void RunAndTimeServerPacket(void* netadr_t_evFrom, void* msg_t_buf) = 0;
    virtual bool SafeMode() = 0;
    virtual bool SaveCvarsToBuffer(char** p1, int p2, char* p3, size_t p4) = 0;
    // virtual void Sprintf(char* dest, int size, const char* fmt, ...) = 0;
    virtual void StartupVariable(const char* match) = 0;
    virtual char* StringContains(char* str1, char* str2, bool case_sensitive) = 0;
    virtual void StripExtension(const char* in, char* out) = 0;
    virtual void StripFilename(char* in, char* out) = 0;
    virtual int SurfaceTypeFromName(char* p1) = 0;
    virtual char* SurfaceTypeToName(int p1) = 0;
    virtual void TouchMemory() = 0;
    virtual void VPrintf(const char* fmt, va_list argptr) = 0;
    virtual void WriteCDKey(const char* gamedir, const char* filename, const char* key) = 0;
    virtual void WriteConfigToFile(const char* filename) = 0;
    virtual void WriteConfiguration() = 0;
    virtual void WriteDefaultsToFile(const char* filename) = 0;

    // parser
    virtual int AddToString(char* p1, int p2, int p3, int p4, int p5) = 0;
    virtual void BeginParseSession(char* name) = 0;
    virtual void EndParseSession() = 0;
    virtual int GetLastTokenPos() = 0;
    virtual char* Parse(char** data_p) = 0;
    virtual void ParseCommandLine(char* command_line) = 0;
    virtual void* ParseCSV(char** p1, int p2) = 0;
    virtual char* ParseExt(char** data_p, bool allow_line_break) = 0;
    virtual char* ParseOnLine(char** data_p) = 0;
    virtual void ParseReturnToMark(int* p1, int* p2) = 0;
    virtual void ParseSetMark(int* p1, int* p2) = 0;
    virtual void ResetParseSessions() = 0;
    virtual void ScriptError(const char* msg, ...) = 0;
    virtual void SetCSV(int p1) = 0;
    virtual void SetErrorMessage(char* p1) = 0;
    virtual void SetParseNegativeNumbers(bool value) = 0;
    virtual void SetRecommended() = 0;
    virtual void SetSpaceDelimited(bool value) = 0;
    virtual bool SkipBracedSection(char** data_p, int p2) = 0;
    virtual char* SkipPath(char* p1) = 0;
    virtual void SkipRestOfLine(char** data_p) = 0;
    virtual void UngetToken() = 0;

    // stringed / localized stuff
    virtual void* GetSubtitleStringEdReference(char* p1) = 0;
    virtual void SetStringEdReference(int p1, char* p2) = 0;
    virtual bool StringEdReferenceExists(char* p1) = 0;
    virtual void WriteFinalStringEdFile(char* p1, char* p2) = 0;
    virtual void WriteStringEdReferenceToFile(const char* ref, const char* string, int file) = 0;

    // soundalias stuff that should get moved to its own class
    virtual void AddBuildSoundAlias(int p1) = 0;
    virtual void AddSoundAlias(int p1, char** p2, char* p3, char* p4, char* p5, int p6) = 0;
    virtual char** FindSoundAlias(char* p1, int p2) = 0;
    virtual void FinishBuildingSoundAlias(int p1) = 0;
    virtual int GetSoundAlias(int p1, int p2) = 0;
    virtual u32 HashAliasName(char* p1) = 0;
    virtual bool IsValidAliasName(char* p1) = 0;
    virtual void LoadedSoundList(int p1) = 0;
    virtual void LoadedSoundAliasDefaults(char* p1, u32 p2) = 0;
    virtual void LoadedSoundAliases(char* p1, u32 p2) = 0;
    virtual void LoadedSoundAliasField(char* p1, double* p2, int p3, int p4, int p5) = 0;
    virtual int LoadSoundAliasFile(char* p1, char* p2, int p3, u32 p4) = 0;
    virtual void LoadSoundAliasSounds(int p1) = 0;
    virtual void MakeSoundAliasesPermanent(int p1, int p2) = 0;
    virtual void NoiseGet4f() = 0;
    virtual void NoiseInit() = 0;
    virtual char** PickSoundAlias(char* p1, int p2) = 0;
    virtual void ProcessSoundAliasFile(char* p1) = 0;
    virtual char* SortTempSoundAliases_r(int p1, int* p2) = 0;
    virtual int SoundAliasChannelForName(char* p1) = 0;
    virtual int SoundAliasChecksum(int p1) = 0;
    virtual int SoundAliasIndex(int p1, int p2) = 0;
    virtual bool SoundAliasLoadSpec(char* p1, char* p2) = 0;
    virtual bool SoundAliasLoop(char* p1) = 0;
    virtual void SoundAliasMasterSlave(double* p1, int p2) = 0;
    virtual char* SoundAliasString(char* p1, int p2) = 0;
    virtual int SoundAliasTypeForName(char* p1) = 0;
    virtual void StreamedSoundList(int p1) = 0;
    virtual void UnloadSoundAliases(u32 p1) = 0;
    virtual void UnloadSoundAliasSounds(int p1) = 0;
    virtual void WriteLocalizedSoundAliasFiles() = 0;

    // dobj/xmodel/xanim
    virtual void CleanupSkeletons() = 0;
    virtual void ClientDObjCreate(int** p1, int p2, int* p3, int p4, short p5) = 0;
    virtual int GetClientDObj(int p1) = 0;
    virtual u32 GetFreeDObjIndex() = 0;
    virtual int GetServerDObj(int p1) = 0;
    virtual void InitDObj() = 0;
    virtual void ResetSkeletonCache() = 0;
    virtual void SafeClientDObjFree(int p1, int p2) = 0;
    virtual void SafeServerDObjFree(int p1, int p2) = 0;
    virtual void ServerDObjCreate(int** p1, int p2, int* p3, int p4, short p5) = 0;
    virtual void ShutdownDObj() = 0;
    virtual void XAnimCreateSmallTree(int p1) = 0;
    virtual void XAnimCreateTree(int p1) = 0;
    virtual void XAnimFreeSmallTree() = 0;

    // weaponinfo memory - should be abstracted away
    virtual void FreeWeaponInfoMemory(int p1, int p2) = 0;
    virtual void* GetWeaponInfoMemory(size_t p1, int* p2, int p3) = 0;

    // common commands
    // Com_Crash_f
    // Com_Error_f
    // Com_Freeze_f
    // Com_Meminfo_f
    // Com_Quit_f
    // Com_SoundList_f
    // Com_WriteConfig_f
    // Com_WriteDefaults_f
    #endif
};

extern ICommon* common;

} // namespace iocod

#endif // CORE_COMMON_H
