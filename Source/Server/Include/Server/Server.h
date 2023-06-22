// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include <Core/IServer.h>

namespace iocod {

class Server final : public IServer {
public:
    void Initialize() override;
    void Shutdown() override;

    void RunFrame(int msec) override;
    void PacketEvent(/* TODO */) override;

    bool ConsoleCommand() override;

    // TODO: sort these / determine which are needed as part of the interface
    #if 0
    void AddArchivedEntToSnapshot(int p1, int* p2) override {}

    void AddCachedEntitiesVisibleFromPoint(int p1, int p2, float* p3, int p4, int* p5,
                                           int p6) override
    {
    }

    void AddDedicatedCommands() override {}

    void AddEntitiesVisibleFromPoint(float* p1, int p2, int* p3) override {}

    void AddEntToSnapshot(int p1, int* p2) override {}

    void AddOperatorCommands() override {}

    void AddServerCommand(int* p1, int p2, char* p3) override {}

    int AddTestClient() override
    {
        return 0;
    }

    void AdjustAreaPortalState(int* p1, int p2) override {}

    void ArchiveSnapshot() override {}

    void AuthorizeIpPacket() override {}

    void AuthorizeRequest(int p1) override {}

    void BadDownload(int p1, int p2) override {}

    void BanClient(int* p1) override {}

    void BanGuidBriefly(int p1) override {}

    void BotUserMove(int* p1) override {}

    void BoundMaxClients(int p1) override {}

    void BuildClientSnapshot(int* p1) override {}

    void CalcPings() override {}

    int CanReplaceServerCommand(int p1, char* p2) override
    {
        return 0;
    }

    void ChangeMaxClients() override {}

    int CheckFallbackURL() override
    {
        return 0;
    }

    int CheckPaused() override
    {
        return 0;
    }

    void CheckTimeouts() override {}

    void ClearServer() override {}

    int ClientCommand(int* p1, int p2) override
    {
        return 0;
    }

    void ClientEnterWorld(int* p1, int* p2) override {}

    void ClientThink(int* p1, int* p2) override {}

    void ClipHandleForEntity(int p1) override {}

    void ClipMoveToEntity(float* p1, int p2) override {}

    int ClipSightToEntity(float* p1, int p2) override
    {
        return 0;
    }

    void CloseDownload(int p1) override {}

    void ConnectionlessPacket(int p1, int p2, int p3, int p4, int p5, int p6) override {}

    void CreateBaseline() override {}

    void CullIgnorableServerCommands(int p1) override {}

    void DelayDropClient(int* p1, int p2) override {}

    void DirectConnect(int p1, int p2, int p3, int p4, int p5) override {}

    void DropClient(int* p1, char* p2) override {}

    void EmitPacketClients(int p1, int p2, int p3, int p4, int p5) override {}

    void EmitPacketEntities(int p1, int p2, int p3, int p4, int p5) override {}

    void EnableArchivedSnapshot(int p1) override {}

    int EntityContact(float* p1, float* p2, int p3, int p4) override
    {
        return 0;
    }

    void ExecuteClientCommand(int p1, char* p2, int p3) override {}

    void ExecuteClientMessage(int* p1, int p2) override {}

    char* ExpandNewlines(char* p1) override
    {
        return nullptr;
    }

    void FinalMessage() override {}

    int FindFreeTempBanSlot() override
    {
        return 0;
    }

    void FlushRedirect(char* p1) override {}

    void Frame(int p1) override {}

    void FreeArchivedSnapshot() override {}

    void FreeClient(int p1) override {}

    void FreeClients() override {}

    void FreeClientScriptId(int p1) override {}

    void FreeClientScriptPers() override {}

    int GameClientNum(int p1) override
    {
        return 0;
    }

    int GameCommand() override
    {
        return 0;
    }

    void GameDropClient(int p1, char* p2) override {}

    void GameSendServerCommand(int p1, int p2) override {}

    char** GameSystemCalls(int* p1) override
    {
        return nullptr;
    }

    void GEntityForSvEntity(int p1) override {}

    int GentityNum(int p1) override
    {
        return 0;
    }

    int GetArchivedClientInfo(int p1, int* p2, int* p3, int p4) override
    {
        return 0;
    }

    int GetCachedSnapshot(int* p1) override
    {
        return 0;
    }

    int GetCachedSnapshotInternal(int p1) override
    {
        return 0;
    }

    void GetChallenge(int p1, int p2, int p3, int p4, int p5) override {}

    void GetClientArchiveTime(int p1) override {}

    int GetClientScore(int p1) override
    {
        return 0;
    }

    void GetClientState(int p1) override {}

    void GetConfigstring(int p1, char* p2, int p3) override {}

    int* GetConfigstringConst(int p1) override
    {
        return nullptr;
    }

    int GetCurrentClientInfo(int p1, int p2, int p3) override
    {
        return 0;
    }

    void GetFollowPlayerState(int p1, int p2) override {}

    void GetMapBaseName(char* p1) override {}

    int GetMapRotationToken() override
    {
        return 0;
    }

    int* GetPlayerByName() override
    {
        return nullptr;
    }

    int* GetPlayerByNum() override
    {
        return nullptr;
    }

    void GetServerinfo(char* p1, int p2) override {}

    void GetUsercmd(int p1, int* p2) override {}

    void GetUserinfo(int p1, char* p2, int p3) override {}

    void Hunk_AllocAlignInternal(size_t p1, int p2) override {}

    void Hunk_AllocateTempMemoryInternal(size_t p1) override {}

    void Hunk_AllocInternal(size_t p1) override {}

    void Hunk_AllocLowAlignInternal(size_t p1, int p2) override {}

    void Hunk_AllocLowInternal(size_t p1) override {}

    void Hunk_FreeTempMemoryInternal(void* p1) override {}

    void InitArchivedSnapshot() override {}

    void InitCvar() override {}

    void InitGameProgs(int p1) override {}

    void InitGameVM(int p1, int p2) override {}

    int inPVS(float* p1, float* p2) override
    {
        return 0;
    }

    int inPVSIgnorePortals(float* p1, float* p2) override
    {
        return 0;
    }

    bool inSnapshot(float* p1, int p2) override
    {
        return false;
    }

    int IsBannedGuid(int p1) override
    {
        return 0;
    }

    int IsFirstTokenEqual(char* p1, char* p2) override
    {
        return 0;
    }

    int IsTempBannedGuid(int p1) override
    {
        return 0;
    }

    int KickClient(int* p1, char* p2, int p3) override
    {
        return 0;
    }

    void LinkEntity(int* p1) override {}

    void LocateGameData(int p1, int p2, int p3, int p4, int p5) override {}

    int MapExists(char* p1) override
    {
        return 0;
    }

    int* MasterAddress() override
    {
        return nullptr;
    }

    void MasterGameCompleteStatus() override {}

    void MasterHeartbeat() override {}

    void MasterShutdown() override {}

    void Netchan_AddOOBProfilePacket(int p1) override {}

    void Netchan_Decode(int p1, char* p2, int p3) override {}

    void Netchan_Encode(int p1, char* p2, int p3) override {}

    void Netchan_PrintProfileStats(int p1) override {}

    void Netchan_Transmit(int p1, void* p2, size_t p3) override {}

    void Netchan_TransmitNextFragment(int* p1) override {}

    void Netchan_UpdateProfileStats() override {}

    void PacketEvent(int p1, int p2, int p3, int p4, int p5, int p6) override {}

    int PointContents(float* p1, int p2, int p3) override
    {
        return 0;
    }

    int PointSightTraceToEntity(float* p1, int p2) override
    {
        return 0;
    }

    void PointTraceToEntity(float* p1, int p2) override {}

    void PrintServerCommandsForClient(int p1) override {}

    void ProfDraw(int p1, int p2) override {}

    int RateMsec(int p1, int p2) override
    {
        return 0;
    }

    void RemoveDedicatedCommands() override {}

    void RemoveOperatorCommands() override {}

    void ResetEntityParsePoint() override {}

    void RestartGameProgs(int p1) override {}

    void RunFrame() override {}

    void SendClientGameState(int* p1) override {}

    void SendClientMessages() override {}

    void SendClientSnapshot(int* p1) override {}

    void SendMessageToClient(int p1, int* p2) override {}

    void SendPbPacket(size_t p1, void* p2, int p3) override {}

    void SendServerCommand(int* p1, int p2, char* p3) override {}

    void SetBrushModel(int* p1) override {}

    void SetClientArchiveTime(int p1, int p2) override {}

    void SetConfig(int p1, int p2, int p3) override {}

    void SetConfigstring(int p1, char* p2) override {}

    void SetConfigValueForKey(int p1, int p2, char* p3, char* p4) override {}

    void SetExpectedHunkUsage(char* p1) override {}

    void SetUserinfo(int p1, char* p2) override {}

    void Shutdown(char* msg) override {}

    void ShutdownGameProgs() override {}

    void SightTrace(int p1, double* p2, float* p3, float* p4, double* p5, int p6, int p7, int p8,
                    int p9) override
    {
    }

    int SightTraceToEntity(float* p1, float* p2, float* p3, float* p4, int p5, int p6,
                           int p7) override
    {
        return 0;
    }

    void SpawnServer(char* p1) override {}

    void Startup() override {}

    int SvEntityForGentity(int* p1) override
    {
        return 0;
    }

    void Trace(int p1, double* p2, float* p3, float* p4, double* p5, int p6, int p7, int p8, int p9,
               int p10, int p11) override
    {
    }

    void UnbanClient(char* p1) override {}

    void UnlinkEntity(int* p1) override {}

    void UpdateBots() override {}

    void UpdateServerCommandsToClient(int p1, int p2) override {}

    void UpdateServerCommandsToClient_PreventOverflow(int p1, int p2, int p3) override {}

    void UserinfoChanged(int p1) override {}

    void UserMove(int* p1, int p2, int p3) override {}

    int* WriteDownloadToClient(int p1, int p2) override
    {
        return nullptr;
    }

    void WriteSnapshotToClient(int* p1, int p2) override {}

    bool WWWRedirectClient(int p1, int p2) override
    {
        return false;
    }

    // dobj/xanim/xmodel
    void DObjCalcAnim(int p1, int* p2) override {}

    void DObjCalcSkel(int p1, int* p2) override {}

    int DObjCreateSkelForBone(int p1, int p2) override
    {
        return 0;
    }

    int DObjCreateSkelForBones(int p1, int* p2) override
    {
        return 0;
    }

    void DObjDisplayAnim(int p1) override {}

    void DObjDumpInfo(int p1) override {}

    int DObjGetBoneIndex(int p1, char* p2) override
    {
        return 0;
    }

    void DObjGetHierarchyBits(int p1, int p2, int* p3) override {}

    void DObjGetMatrixArray(int p1) override {}

    void DObjGetRotTransArray(int p1) override {}

    void DObjGetTree(int p1) override {}

    void DObjInitServerTime(int p1) override {}

    void DObjNumBones(int p1) override {}

    void DObjSetControlRotTransIndex(int p1, int p2, int p3) override {}

    void DObjSetRotTransIndex(int p1, int p2, int p3) override {}

    int DObjUpdateServerTime(int p1, int p2, int p3) override
    {
        return 0;
    }

    void XModelDebugBoxes(int p1) override {}

    void XModelGet(char* p1) override {}

    // commands
    // virtual void Ban_f() = 0;
    // virtual void BanNum_f() = 0;
    // virtual void BeginDownload_f(int p1) = 0;
    // virtual void ConSay_f() = 0;
    // virtual void ConTell_f() = 0;
    // virtual void Disconnect_f(int* p1) = 0;
    // virtual void DoneDownload_f(int* p1) = 0;
    // virtual void Drop_f() = 0;
    // virtual void DropNum_f() = 0;
    // virtual void DumpUser_f() = 0;
    // virtual void GameCompleteStatus_f() = 0;
    // virtual void Heartbeat_f() = 0;
    // virtual int KickClient_f(char* p1, int p2) = 0;
    // virtual int KickUser_f(char* p1, int p2) = 0;
    // virtual void KillServer_f() = 0;
    // virtual void Map_f() = 0;
    // virtual void MapRestart_f() = 0;
    // virtual void MapRotate_f() = 0;
    // virtual void NextDownload_f(int* p1) = 0;
    // virtual void ResetPureClient_f(int p1) = 0;
    // virtual void RetransmitDownload_f(int p1) = 0;
    // virtual void ScriptUsage_f() = 0;
    // virtual void Serverinfo_f() = 0;
    // virtual void SetDrawFriend_f() = 0;
    // virtual void SetFriendlyFire_f() = 0;
    // virtual void SetKillcam_f() = 0;
    // virtual void Status_f() = 0;
    // virtual void StopDownload_f(int p1) = 0;
    // virtual void StringUsage_f() = 0;
    // virtual void Systeminfo_f() = 0;
    // virtual void TempBan_f() = 0;
    // virtual void TempBanNum_f() = 0;
    // virtual void Unban_f() = 0;
    // virtual void UpdateUserinfo_f(int p1) = 0;
    // virtual void VerifyPaks_f(int p1) = 0;
    // virtual void WWWDownload_f(int* p1) = 0;
    #endif

private:
    // other impl details
};

} // namespace iocod

#endif // SERVER_SERVER_H
