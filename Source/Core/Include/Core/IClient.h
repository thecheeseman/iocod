// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_ICLIENT_H
#define CORE_ICLIENT_H

namespace iocod {

// client DLL interface
class IClient {
public:
    virtual ~IClient() = default;

    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;

    virtual bool ConsoleCommand() = 0;
    virtual void DrawActiveFrame(/* TODO */) = 0;

    // TODO: sort these
    #if 0
    virtual void AddCgameCommand(char* p1) = 0;
    virtual void AddConsoleInfoChar(int p1, short p2) = 0;
    virtual void AddConsoleInfoColor(int p1, float* p2) = 0;
    virtual void AddDeathMessageText(char* p1, int p2) = 0;
    virtual void AddDebugLine(double* p1, double* p2, double* p3, int p4, int p5, int p6) = 0;
    virtual void AddDebugString(double* p1, double* p2, int p3, char* p4, int p5) = 0;
    virtual void AddReliableCommand(char* p1) = 0;
    virtual void AdjustAngles() = 0;
    virtual void AdjustTimeDelta() = 0;
    virtual void BeginDownload(char* p1, char* p2) = 0;
    virtual void CDDialog() = 0;
    virtual bool CDKeyValidate(char* p1, char* p2) = 0;
    virtual void CGameRendering(int p1, int p2) = 0;
    virtual char** CgameSystemCalls(int* p1) = 0;
    virtual void ChangeReliableCommand() = 0;
    virtual void CharEvent(int p1) = 0;
    virtual void CheckAutoUpdate() = 0;
    virtual void CheckForResend() = 0;
    virtual void CheckTimeout() = 0;
    virtual void CheckUserinfo() = 0;
    virtual void ClearKeys() = 0;
    virtual void ClearPing(int p1) = 0;
    virtual void ClearState() = 0;
    virtual void ClearStaticDownload() = 0;
    virtual void CM_LoadMap(char* p1) = 0;
    virtual void CmdButtons(int p1) = 0;
    virtual void CompareAdrSigned(int* p1, int* p2) = 0;
    virtual void ConfigstringModified() = 0;
    virtual void ConnectionlessPacket(int p1, int p2, int p3, int p4, int p5, int p6, int p7) = 0;
    virtual void ConsoleFixPosition() = 0;
    virtual int ConsolePrint_AddLine(int p1, char* p2, int p3, int p4, int p5) = 0;
    virtual void ConsolePrint(int p1, char* p2, int p3, int p4) = 0;
    virtual void CreateCmd(int* p1) = 0;
    virtual void CreateNewCommands() = 0;
    virtual void CubemapShotUsage() = 0;
    virtual void DeathMessagePrint(int p1, char* p2, float* p3, char* p4, float* p5, char* p6, int p7,
                           int p8, float* p9, int p10) = 0;
    virtual void DeltaClient(int p1, int p2, int p3, int* p4, int p5) = 0;
    virtual void DeltaEntity(int p1, int p2, int p3, void* p4, int p5) = 0;
    virtual void DemoCompleted() = 0;
    virtual void DemoFilename(int p1, char* p2) = 0;
    virtual void Disconnect(int p1) = 0;
    virtual void DisconnectPacket() = 0;
    virtual void DObjCalcAnim(int* p1, int* p2) = 0;
    virtual void DObjCalcSkel(int p1, int* p2) = 0;
    virtual int DObjCreateSkelForBone(int p1, int p2) = 0;
    virtual int DObjCreateSkelForBones(int p1, int* p2) = 0;
    virtual void DObjInvalidateSkels() = 0;
    virtual void DownloadsComplete() = 0;
    virtual void DrawLogo() = 0;
    virtual void DrawString(int p1, int p2, int p3, int p4, int p5, int p6, int p7) = 0;
    virtual int FindServerInfo(int p1, int p2, int p3, int p4, int p5) = 0;
    virtual void FinishMove(int* p1) = 0;
    virtual void FirstSnapshot() = 0;
    virtual void FlushDebugData(int p1) = 0;
    virtual void ForwardCommandToServer(char* p1) = 0;
    virtual void Frame(int p1, int p2) = 0;
    virtual int GameCommand() = 0;
    virtual void GetAutoUpdate() = 0;
    virtual int GetCurrentCmdNumber() = 0;
    virtual void GetCurrentSnapshotNumber(int* p1, int* p2) = 0;
    virtual void GetFontInfo(int p1) = 0;
    virtual int* GetFreePing() = 0;
    virtual void GetGameState(int p1) = 0;
    virtual void GetGlconfig(int p1) = 0;
    virtual void GetPing(int p1, char* p2, int p3, int* p4) = 0;
    virtual void GetPingInfo(int p1, char* p2, int p3) = 0;
    virtual int GetPingQueueCount() = 0;
    virtual int GetServerCommand(int p1) = 0;
    virtual int* GetServerIPAddress() = 0;
    virtual int* GetServerStatus() = 0;
    virtual int GetSnapshot(int p1, int* p2) = 0;
    virtual bool GetUserCmd(int p1, int* p2) = 0;
    virtual void Init() = 0;
    virtual void InitCGame() = 0;
    virtual void InitDownloads() = 0;
    virtual void InitInput() = 0;
    virtual void InitKeyCommands() = 0;
    virtual void InitRef() = 0;
    virtual void InitRenderer() = 0;
    virtual void InitServerInfo(int* p1, int p2, int p3, int p4, int p5, int p6) = 0;
    virtual void InitUI() = 0;
    virtual bool IsInMatchTimeout() = 0;
    virtual void JoystickEvent(int p1, int p2) = 0;
    virtual void JoystickMove(int p1) = 0;
    virtual void KeyEvent(char* p1, int p2) = 0;
    virtual void KeyMove(int p1) = 0;
    virtual void KeyState(int p1) = 0;
    virtual void LookupColor(char p1, double* p2) = 0;
    virtual void MakeMonkeyDoLaundry() = 0;
    virtual void MapLoading() = 0;
    virtual void MotdPacket() = 0;
    virtual void MouseEvent(int p1, int p2) = 0;
    virtual void MouseMove(int p1) = 0;
    virtual void Netchan_AddOOBProfilePacket(int p1) = 0;
    virtual void Netchan_Decode(char* p1, int p2) = 0;
    virtual void Netchan_Encode(char* p1, int p2) = 0;
    virtual void Netchan_PrintProfileStats(int p1) = 0;
    virtual void Netchan_SendOOBPacket(size_t p1, int* p2, int p3, int p4, int p5, int p6, short p7) = 0;
    virtual void Netchan_Transmit(int* p1, void* p2, size_t p3) = 0;
    virtual void Netchan_TransmitNextFragment(int* p1) = 0;
    virtual void Netchan_UpdateProfileStats() = 0;
    virtual void NextDemo() = 0;
    virtual void NextDownload() = 0;
    virtual void PacketEvent(int p1, int p2, int p3, int p4, int p5, int p6, int p7) = 0;
    virtual void ParseCommandString(int p1) = 0;
    virtual void ParseDownload(int p1) = 0;
    virtual void ParseGamestate(int p1) = 0;
    virtual void ParsePacketClients(int p1, int p2, int p3) = 0;
    virtual void ParsePacketEntities(int p1, int p2, int p3) = 0;
    virtual void ParseServerMessage(int p1) = 0;
    virtual void ParseSnapshot(int p1) = 0;
    virtual void ParseWWWDownload(int p1) = 0;
    virtual void ProfDraw(int p1, int p2) = 0;
    virtual void ReadDemoMessage() = 0;
    virtual int ReadyToSendPacket() = 0;
    virtual void RefPrintf(int p1, char* p2) = 0;
    virtual void RequestAuthorization() = 0;
    virtual void ResetPureClientAtServer() = 0;
    virtual int RestoreCgameState(int p1, int p2) = 0;
    virtual int SaveCgameState(int p1, int p2) = 0;
    virtual int ScaledMilliseconds() = 0;
    virtual void SendCmd() = 0;
    virtual void SendPbPacket(size_t p1, void* p2) = 0;
    virtual void SendPureChecksums() = 0;
    virtual void ServerInfoPacket(int p1, int p2, int p3, int p4, int p5, int p6, int p7) = 0;
    virtual void ServersResponsePacket() = 0;
    virtual int ServerStatus(char* p1, char* p2, int p3) = 0;
    virtual void ServerStatusResponse(int p1, int p2, int p3, int p4, int p5, int p6) = 0;
    virtual void SetCGameTime() = 0;
    virtual void SetClientLerpOrigin() = 0;
    virtual void SetExpectedHunkUsage(char* p1) = 0;
    virtual void SetServerInfo(int p1, char* p2, short p3) = 0;
    virtual void SetServerInfoByAddress() = 0;
    virtual void SetTempStanceStatus() = 0;
    virtual void SetupForNewServerMap(char* p1, char* p2) = 0;
    virtual void SetUserCmdAimValues(double* p1) = 0;
    virtual void SetUserCmdInShellshock(int p1) = 0;
    virtual void SetUserCmdValue(int p1) = 0;
    virtual void ShutdownAll() = 0;
    virtual void ShutdownCGame() = 0;
    virtual void ShutdownDebugData() = 0;
    virtual void ShutdownInput() = 0;
    virtual void ShutdownRef() = 0;
    virtual void ShutdownUI() = 0;
    virtual void SortGlobalServers() = 0;
    virtual void StartDemoLoop() = 0;
    virtual void StartHunkUsers() = 0;
    virtual void StopLogo() = 0;
    virtual void SubtitlePrint(char* p1, int p2, int p3) = 0;
    virtual int SurfaceTypeFromName(char* p1) = 0;
    virtual char* SurfaceTypeToName(int p1) = 0;
    virtual void SystemInfoChanged() = 0;
    virtual char* TimeDemoLogBaseName() = 0;
    virtual char* UISystemCalls(int* p1) = 0;
    virtual void UpdateColor() = 0;
    virtual void UpdateColorInternal(char* p1, float* p2) = 0;
    virtual void UpdateDebugData() = 0;
    virtual void UpdateInfoPacket() = 0;
    virtual void UpdateInGameState() = 0;
    virtual void UpdateLevelHunkUsage() = 0;
    virtual void UpdateTimeDemo() = 0;
    virtual void WriteDemoMessage(int p1, int p2) = 0;
    virtual void WritePacket() = 0;
    virtual int WWWBadChecksum(char* p1) = 0;
    virtual void WWWDownload() = 0;

    // commands
    //virtual void Clientinfo_f() = 0;
    //virtual void Configstrings_f() = 0;
    //virtual void Connect_f() = 0;
    //virtual void CubemapShot_f() = 0;
    //virtual void Disconnect_f() = 0;
    //virtual void ForwardToServer_f() = 0;
    //virtual void GlobalServers_f() = 0;
    //virtual void LocalServers_f() = 0;
    //virtual void OpenedPK3List_f() = 0;
    //virtual void Ping_f() = 0;
    //virtual void PlayCinematic_f() = 0;
    //virtual void PlayDemo_f() = 0;
    //virtual void PlayLogo_f() = 0;
    //virtual void Rcon_f() = 0;
    //virtual void Reconnect_f() = 0;
    //virtual void Record_f() = 0;
    //virtual void ReferencedPK3List_f() = 0;
    //virtual void ServerStatus_f() = 0;
    //virtual void Setenv_f() = 0;
    //virtual void SetRecommended_f() = 0;
    //virtual void ShowIP_f() = 0;
    //virtual void Snd_Restart_f() = 0;
    //virtual void startSingleplayer_f() = 0;
    //virtual void StopRecord_f() = 0;
    //virtual int UpdateDirtyPings_f(int p1) = 0;
    //virtual void Vid_Restart_f() = 0;
    #endif
};

extern IClient* client; // returned by the client DLL after initialization

//
// imports / exports
//
inline constexpr int kClientAPIVersion = 1;

#include "ForwardDecls.h"

struct ClientImports {
    int version{kClientAPIVersion};

    ISystem* system{nullptr};
    IFileSystem* file_system{nullptr};
    ICommandSystem* command_system{nullptr};
};

struct ClientExports {
    int version{kClientAPIVersion};

    IClient* client{nullptr};
};

using GetClientAPIPtr = ClientExports* (*)(ClientImports*);

} // namespace iocod

#endif //CORE_ICLIENT_H
