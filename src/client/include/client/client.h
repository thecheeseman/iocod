// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include <core/iclient.h>

namespace iocod {

class Client final : public IClient {
public:
    void Initialize() override;
    void Shutdown() override;

    bool ConsoleCommand() override;
    void DrawActiveFrame(/* TODO */) override;

    // TODO: sort these
    #if 0
    void AddCgameCommand(char* p1) override {}

    void AddConsoleInfoChar(int p1, short p2) override {}

    void AddConsoleInfoColor(int p1, float* p2) override {}

    void AddDeathMessageText(char* p1, int p2) override {}

    void AddDebugLine(double* p1, double* p2, double* p3, int p4, int p5, int p6) override {}

    void AddDebugString(double* p1, double* p2, int p3, char* p4, int p5) override {}

    void AddReliableCommand(char* p1) override {}

    void AdjustAngles() override {}

    void AdjustTimeDelta() override {}

    void BeginDownload(char* p1, char* p2) override {}

    void CDDialog() override {}

    bool CDKeyValidate(char* p1, char* p2) override
    {
        return false;
    }

    void CGameRendering(int p1, int p2) override {}

    char** CgameSystemCalls(int* p1) override
    {
        return nullptr;
    }

    void ChangeReliableCommand() override {}

    void CharEvent(int p1) override {}

    void CheckAutoUpdate() override {}

    void CheckForResend() override {}

    void CheckTimeout() override {}

    void CheckUserinfo() override {}

    void ClearKeys() override {}

    void ClearPing(int p1) override {}

    void ClearState() override {}

    void ClearStaticDownload() override {}

    void CM_LoadMap(char* p1) override {}

    void CmdButtons(int p1) override {}

    void CompareAdrSigned(int* p1, int* p2) override {}

    void ConfigstringModified() override {}

    void ConnectionlessPacket(int p1, int p2, int p3, int p4, int p5, int p6, int p7) override {}

    void ConsoleFixPosition() override {}

    int ConsolePrint_AddLine(int p1, char* p2, int p3, int p4, int p5) override
    {
        return 0;
    }

    void ConsolePrint(int p1, char* p2, int p3, int p4) override {}

    void CreateCmd(int* p1) override {}

    void CreateNewCommands() override {}

    void CubemapShotUsage() override {}

    void DeathMessagePrint(int p1, char* p2, float* p3, char* p4, float* p5, char* p6, int p7,
                           int p8, float* p9, int p10) override
    {
    }

    void DeltaClient(int p1, int p2, int p3, int* p4, int p5) override {}

    void DeltaEntity(int p1, int p2, int p3, void* p4, int p5) override {}

    void DemoCompleted() override {}

    void DemoFilename(int p1, char* p2) override {}

    void Disconnect(int p1) override {}

    void DisconnectPacket() override {}

    void DObjCalcAnim(int* p1, int* p2) override {}

    void DObjCalcSkel(int p1, int* p2) override {}

    int DObjCreateSkelForBone(int p1, int p2) override
    {
        return 0;
    }

    int DObjCreateSkelForBones(int p1, int* p2) override
    {
        return 0;
    }

    void DObjInvalidateSkels() override {}

    void DownloadsComplete() override {}

    void DrawLogo() override {}

    void DrawString(int p1, int p2, int p3, int p4, int p5, int p6, int p7) override {}

    int FindServerInfo(int p1, int p2, int p3, int p4, int p5) override
    {
        return 0;
    }

    void FinishMove(int* p1) override {}

    void FirstSnapshot() override {}

    void FlushDebugData(int p1) override {}

    void ForwardCommandToServer(char* p1) override {}

    void Frame(int p1, int p2) override {}

    int GameCommand() override
    {
        return 0;
    }

    void GetAutoUpdate() override {}

    int GetCurrentCmdNumber() override
    {
        return 0;
    }

    void GetCurrentSnapshotNumber(int* p1, int* p2) override {}

    void GetFontInfo(int p1) override {}

    int* GetFreePing() override
    {
        return nullptr;
    }

    void GetGameState(int p1) override {}

    void GetGlconfig(int p1) override {}

    void GetPing(int p1, char* p2, int p3, int* p4) override {}

    void GetPingInfo(int p1, char* p2, int p3) override {}

    int GetPingQueueCount() override
    {
        return 0;
    }

    int GetServerCommand(int p1) override
    {
        return 0;
    }

    int* GetServerIPAddress() override
    {
        return nullptr;
    }

    int* GetServerStatus() override
    {
        return nullptr;
    }

    int GetSnapshot(int p1, int* p2) override
    {
        return 0;
    }

    bool GetUserCmd(int p1, int* p2) override
    {
        return false;
    }

    void Init() override {}

    void InitCGame() override {}

    void InitDownloads() override {}

    void InitInput() override {}

    void InitKeyCommands() override {}

    void InitRef() override {}

    void InitRenderer() override {}

    void InitServerInfo(int* p1, int p2, int p3, int p4, int p5, int p6) override {}

    void InitUI() override {}

    bool IsInMatchTimeout() override
    {
        return false;
    }

    void JoystickEvent(int p1, int p2) override {}

    void JoystickMove(int p1) override {}

    void KeyEvent(char* p1, int p2) override {}

    void KeyMove(int p1) override {}

    void KeyState(int p1) override {}

    void LookupColor(char p1, double* p2) override {}

    void MakeMonkeyDoLaundry() override {}

    void MapLoading() override {}

    void MotdPacket() override {}

    void MouseEvent(int p1, int p2) override {}

    void MouseMove(int p1) override {}

    void Netchan_AddOOBProfilePacket(int p1) override {}

    void Netchan_Decode(char* p1, int p2) override {}

    void Netchan_Encode(char* p1, int p2) override {}

    void Netchan_PrintProfileStats(int p1) override {}

    void Netchan_SendOOBPacket(size_t p1, int* p2, int p3, int p4, int p5, int p6,
                               short p7) override
    {
    }

    void Netchan_Transmit(int* p1, void* p2, size_t p3) override {}

    void Netchan_TransmitNextFragment(int* p1) override {}

    void Netchan_UpdateProfileStats() override {}

    void NextDemo() override {}

    void NextDownload() override {}

    void PacketEvent(int p1, int p2, int p3, int p4, int p5, int p6, int p7) override {}

    void ParseCommandString(int p1) override {}

    void ParseDownload(int p1) override {}

    void ParseGamestate(int p1) override {}

    void ParsePacketClients(int p1, int p2, int p3) override {}

    void ParsePacketEntities(int p1, int p2, int p3) override {}

    void ParseServerMessage(int p1) override {}

    void ParseSnapshot(int p1) override {}

    void ParseWWWDownload(int p1) override {}

    void ProfDraw(int p1, int p2) override {}

    void ReadDemoMessage() override {}

    int ReadyToSendPacket() override
    {
        return 0;
    }

    void RefPrintf(int p1, char* p2) override {}

    void RequestAuthorization() override {}

    void ResetPureClientAtServer() override {}

    int RestoreCgameState(int p1, int p2) override
    {
        return 0;
    }

    int SaveCgameState(int p1, int p2) override
    {
        return 0;
    }

    int ScaledMilliseconds() override
    {
        return 0;
    }

    void SendCmd() override {}

    void SendPbPacket(size_t p1, void* p2) override {}

    void SendPureChecksums() override {}

    void ServerInfoPacket(int p1, int p2, int p3, int p4, int p5, int p6, int p7) override {}

    void ServersResponsePacket() override {}

    int ServerStatus(char* p1, char* p2, int p3) override
    {
        return 0;
    }

    void ServerStatusResponse(int p1, int p2, int p3, int p4, int p5, int p6) override {}

    void SetCGameTime() override {}

    void SetClientLerpOrigin() override {}

    void SetExpectedHunkUsage(char* p1) override {}

    void SetServerInfo(int p1, char* p2, short p3) override {}

    void SetServerInfoByAddress() override {}

    void SetTempStanceStatus() override {}

    void SetupForNewServerMap(char* p1, char* p2) override {}

    void SetUserCmdAimValues(double* p1) override {}

    void SetUserCmdInShellshock(int p1) override {}

    void SetUserCmdValue(int p1) override {}

    void ShutdownAll() override {}

    void ShutdownCGame() override {}

    void ShutdownDebugData() override {}

    void ShutdownInput() override {}

    void ShutdownRef() override {}

    void ShutdownUI() override {}

    void SortGlobalServers() override {}

    void StartDemoLoop() override {}

    void StartHunkUsers() override {}

    void StopLogo() override {}

    void SubtitlePrint(char* p1, int p2, int p3) override {}

    int SurfaceTypeFromName(char* p1) override
    {
        return 0;
    }

    char* SurfaceTypeToName(int p1) override
    {
        return nullptr;
    }

    void SystemInfoChanged() override {}

    char* TimeDemoLogBaseName() override
    {
        return nullptr;
    }

    char* UISystemCalls(int* p1) override
    {
        return nullptr;
    }

    void UpdateColor() override {}

    void UpdateColorInternal(char* p1, float* p2) override {}

    void UpdateDebugData() override {}

    void UpdateInfoPacket() override {}

    void UpdateInGameState() override {}

    void UpdateLevelHunkUsage() override {}

    void UpdateTimeDemo() override {}

    void WriteDemoMessage(int p1, int p2) override {}

    void WritePacket() override {}

    int WWWBadChecksum(char* p1) override
    {
        return 0;
    }

    void WWWDownload() override {}

    // commands
    // virtual void Clientinfo_f() = 0;
    // virtual void Configstrings_f() = 0;
    // virtual void Connect_f() = 0;
    // virtual void CubemapShot_f() = 0;
    // virtual void Disconnect_f() = 0;
    // virtual void ForwardToServer_f() = 0;
    // virtual void GlobalServers_f() = 0;
    // virtual void LocalServers_f() = 0;
    // virtual void OpenedPK3List_f() = 0;
    // virtual void Ping_f() = 0;
    // virtual void PlayCinematic_f() = 0;
    // virtual void PlayDemo_f() = 0;
    // virtual void PlayLogo_f() = 0;
    // virtual void Rcon_f() = 0;
    // virtual void Reconnect_f() = 0;
    // virtual void Record_f() = 0;
    // virtual void ReferencedPK3List_f() = 0;
    // virtual void ServerStatus_f() = 0;
    // virtual void Setenv_f() = 0;
    // virtual void SetRecommended_f() = 0;
    // virtual void ShowIP_f() = 0;
    // virtual void Snd_Restart_f() = 0;
    // virtual void startSingleplayer_f() = 0;
    // virtual void StopRecord_f() = 0;
    // virtual int UpdateDirtyPings_f(int p1) = 0;
    // virtual void Vid_Restart_f() = 0;
    #endif

private:
    // other impl details
};

} // namespace iocod

#endif // CLIENT_CLIENT_H
