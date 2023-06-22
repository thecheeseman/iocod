// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_ISERVER_H
#define CORE_ISERVER_H

namespace iocod {

// server DLL interface
class IServer {
public:
    virtual ~IServer() = default;

    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;

    virtual void RunFrame(int msec) = 0;
    virtual void PacketEvent(/* TODO */) = 0;

    virtual bool ConsoleCommand() = 0;

    // TODO: sort these / determine which are needed as part of the interface
    #if 0
    virtual void AddArchivedEntToSnapshot(int p1, int* p2) = 0;
    virtual void AddCachedEntitiesVisibleFromPoint(int p1, int p2, float* p3, int p4, int* p5,
                                                   int p6) = 0;
    virtual void AddDedicatedCommands() = 0;
    virtual void AddEntitiesVisibleFromPoint(float* p1, int p2, int* p3) = 0;
    virtual void AddEntToSnapshot(int p1, int* p2) = 0;
    virtual void AddOperatorCommands() = 0;
    virtual void AddServerCommand(int* p1, int p2, char* p3) = 0;
    virtual int AddTestClient() = 0;
    virtual void AdjustAreaPortalState(int* p1, int p2) = 0;
    virtual void ArchiveSnapshot() = 0;
    virtual void AuthorizeIpPacket() = 0;
    virtual void AuthorizeRequest(int p1) = 0;
    virtual void BadDownload(int p1, int p2) = 0;
    virtual void BanClient(int* p1) = 0;
    virtual void BanGuidBriefly(int p1) = 0;
    virtual void BotUserMove(int* p1) = 0;
    virtual void BoundMaxClients(int p1) = 0;
    virtual void BuildClientSnapshot(int* p1) = 0;
    virtual void CalcPings() = 0;
    virtual int CanReplaceServerCommand(int p1, char* p2) = 0;
    virtual void ChangeMaxClients() = 0;
    virtual int CheckFallbackURL() = 0;
    virtual int CheckPaused() = 0;
    virtual void CheckTimeouts() = 0;
    virtual void ClearServer() = 0;
    virtual int ClientCommand(int* p1, int p2) = 0;
    virtual void ClientEnterWorld(int* p1, int* p2) = 0;
    virtual void ClientThink(int* p1, int* p2) = 0;
    virtual void ClipHandleForEntity(int p1) = 0;
    virtual void ClipMoveToEntity(float* p1, int p2) = 0;
    virtual int ClipSightToEntity(float* p1, int p2) = 0;
    virtual void CloseDownload(int p1) = 0;
    virtual void ConnectionlessPacket(int p1, int p2, int p3, int p4, int p5, int p6) = 0;
    virtual void CreateBaseline() = 0;
    virtual void CullIgnorableServerCommands(int p1) = 0;
    virtual void DelayDropClient(int* p1, int p2) = 0;
    virtual void DirectConnect(int p1, int p2, int p3, int p4, int p5) = 0;
    virtual void DropClient(int* p1, char* p2) = 0;
    virtual void EmitPacketClients(int p1, int p2, int p3, int p4, int p5) = 0;
    virtual void EmitPacketEntities(int p1, int p2, int p3, int p4, int p5) = 0;
    virtual void EnableArchivedSnapshot(int p1) = 0;
    virtual int EntityContact(float* p1, float* p2, int p3, int p4) = 0;
    virtual void ExecuteClientCommand(int p1, char* p2, int p3) = 0;
    virtual void ExecuteClientMessage(int* p1, int p2) = 0;
    virtual char* ExpandNewlines(char* p1) = 0;
    virtual void FinalMessage() = 0;
    virtual int FindFreeTempBanSlot() = 0;
    virtual void FlushRedirect(char* p1) = 0;
    virtual void Frame(int p1) = 0;
    virtual void FreeArchivedSnapshot() = 0;
    virtual void FreeClient(int p1) = 0;
    virtual void FreeClients() = 0;
    virtual void FreeClientScriptId(int p1) = 0;
    virtual void FreeClientScriptPers() = 0;
    virtual int GameClientNum(int p1) = 0;
    virtual int GameCommand() = 0;
    virtual void GameDropClient(int p1, char* p2) = 0;
    virtual void GameSendServerCommand(int p1, int p2) = 0;
    virtual char** GameSystemCalls(int* p1) = 0;
    virtual void GEntityForSvEntity(int p1) = 0;
    virtual int GentityNum(int p1) = 0;
    virtual int GetArchivedClientInfo(int p1, int* p2, int* p3, int p4) = 0;
    virtual int GetCachedSnapshot(int* p1) = 0;
    virtual int GetCachedSnapshotInternal(int p1) = 0;
    virtual void GetChallenge(int p1, int p2, int p3, int p4, int p5) = 0;
    virtual void GetClientArchiveTime(int p1) = 0;
    virtual int GetClientScore(int p1) = 0;
    virtual void GetClientState(int p1) = 0;
    virtual void GetConfigstring(int p1, char* p2, int p3) = 0;
    virtual int* GetConfigstringConst(int p1) = 0;
    virtual int GetCurrentClientInfo(int p1, int p2, int p3) = 0;
    virtual void GetFollowPlayerState(int p1, int p2) = 0;
    virtual void GetMapBaseName(char* p1) = 0;
    virtual int GetMapRotationToken() = 0;
    virtual int* GetPlayerByName() = 0;
    virtual int* GetPlayerByNum() = 0;
    virtual void GetServerinfo(char* p1, int p2) = 0;
    virtual void GetUsercmd(int p1, int* p2) = 0;
    virtual void GetUserinfo(int p1, char* p2, int p3) = 0;
    virtual void Hunk_AllocAlignInternal(size_t p1, int p2) = 0;
    virtual void Hunk_AllocateTempMemoryInternal(size_t p1) = 0;
    virtual void Hunk_AllocInternal(size_t p1) = 0;
    virtual void Hunk_AllocLowAlignInternal(size_t p1, int p2) = 0;
    virtual void Hunk_AllocLowInternal(size_t p1) = 0;
    virtual void Hunk_FreeTempMemoryInternal(void* p1) = 0;
    virtual void InitArchivedSnapshot() = 0;
    virtual void InitCvar() = 0;
    virtual void InitGameProgs(int p1) = 0;
    virtual void InitGameVM(int p1, int p2) = 0;
    virtual int inPVS(float* p1, float* p2) = 0;
    virtual int inPVSIgnorePortals(float* p1, float* p2) = 0;
    virtual bool inSnapshot(float* p1, int p2) = 0;
    virtual int IsBannedGuid(int p1) = 0;
    virtual int IsFirstTokenEqual(char* p1, char* p2) = 0;
    virtual int IsTempBannedGuid(int p1) = 0;
    virtual int KickClient(int* p1, char* p2, int p3) = 0;
    virtual void LinkEntity(int* p1) = 0;
    virtual void LocateGameData(int p1, int p2, int p3, int p4, int p5) = 0;
    virtual int MapExists(char* p1) = 0;
    virtual int* MasterAddress() = 0;
    virtual void MasterGameCompleteStatus() = 0;
    virtual void MasterHeartbeat() = 0;
    virtual void MasterShutdown() = 0;
    virtual void Netchan_AddOOBProfilePacket(int p1) = 0;
    virtual void Netchan_Decode(int p1, char* p2, int p3) = 0;
    virtual void Netchan_Encode(int p1, char* p2, int p3) = 0;
    virtual void Netchan_PrintProfileStats(int p1) = 0;
    virtual void Netchan_Transmit(int p1, void* p2, size_t p3) = 0;
    virtual void Netchan_TransmitNextFragment(int* p1) = 0;
    virtual void Netchan_UpdateProfileStats() = 0;
    virtual void PacketEvent(int p1, int p2, int p3, int p4, int p5, int p6) = 0;
    virtual int PointContents(float* p1, int p2, int p3) = 0;
    virtual int PointSightTraceToEntity(float* p1, int p2) = 0;
    virtual void PointTraceToEntity(float* p1, int p2) = 0;
    virtual void PrintServerCommandsForClient(int p1) = 0;
    virtual void ProfDraw(int p1, int p2) = 0;
    virtual int RateMsec(int p1, int p2) = 0;
    virtual void RemoveDedicatedCommands() = 0;
    virtual void RemoveOperatorCommands() = 0;
    virtual void ResetEntityParsePoint() = 0;
    virtual void RestartGameProgs(int p1) = 0;
    virtual void RunFrame() = 0;
    virtual void SendClientGameState(int* p1) = 0;
    virtual void SendClientMessages() = 0;
    virtual void SendClientSnapshot(int* p1) = 0;
    virtual void SendMessageToClient(int p1, int* p2) = 0;
    virtual void SendPbPacket(size_t p1, void* p2, int p3) = 0;
    virtual void SendServerCommand(int* p1, int p2, char* p3) = 0;
    virtual void SetBrushModel(int* p1) = 0;
    virtual void SetClientArchiveTime(int p1, int p2) = 0;
    virtual void SetConfig(int p1, int p2, int p3) = 0;
    virtual void SetConfigstring(int p1, char* p2) = 0;
    virtual void SetConfigValueForKey(int p1, int p2, char* p3, char* p4) = 0;
    virtual void SetExpectedHunkUsage(char* p1) = 0;
    virtual void SetUserinfo(int p1, char* p2) = 0;
    virtual void Shutdown(char* msg) = 0;
    virtual void ShutdownGameProgs() = 0;
    virtual void SightTrace(int p1, double* p2, float* p3, float* p4, double* p5, int p6, int p7,
                            int p8, int p9) = 0;
    virtual int SightTraceToEntity(float* p1, float* p2, float* p3, float* p4, int p5, int p6,
                                   int p7) = 0;
    virtual void SpawnServer(char* p1) = 0;
    virtual void Startup() = 0;
    virtual int SvEntityForGentity(int* p1) = 0;
    virtual void Trace(int p1, double* p2, float* p3, float* p4, double* p5, int p6, int p7, int p8,
                       int p9, int p10, int p11) = 0;
    virtual void UnbanClient(char* p1) = 0;
    virtual void UnlinkEntity(int* p1) = 0;
    virtual void UpdateBots() = 0;
    virtual void UpdateServerCommandsToClient(int p1, int p2) = 0;
    virtual void UpdateServerCommandsToClient_PreventOverflow(int p1, int p2, int p3) = 0;
    virtual void UserinfoChanged(int p1) = 0;
    virtual void UserMove(int* p1, int p2, int p3) = 0;
    virtual int* WriteDownloadToClient(int p1, int p2) = 0;
    virtual void WriteSnapshotToClient(int* p1, int p2) = 0;
    virtual bool WWWRedirectClient(int p1, int p2) = 0;

    // dobj/xanim/xmodel
    virtual void DObjCalcAnim(int p1, int* p2) = 0;
    virtual void DObjCalcSkel(int p1, int* p2) = 0;
    virtual int DObjCreateSkelForBone(int p1, int p2) = 0;
    virtual int DObjCreateSkelForBones(int p1, int* p2) = 0;
    virtual void DObjDisplayAnim(int p1) = 0;
    virtual void DObjDumpInfo(int p1) = 0;
    virtual int DObjGetBoneIndex(int p1, char* p2) = 0;
    virtual void DObjGetHierarchyBits(int p1, int p2, int* p3) = 0;
    virtual void DObjGetMatrixArray(int p1) = 0;
    virtual void DObjGetRotTransArray(int p1) = 0;
    virtual void DObjGetTree(int p1) = 0;
    virtual void DObjInitServerTime(int p1) = 0;
    virtual void DObjNumBones(int p1) = 0;
    virtual void DObjSetControlRotTransIndex(int p1, int p2, int p3) = 0;
    virtual void DObjSetRotTransIndex(int p1, int p2, int p3) = 0;
    virtual int DObjUpdateServerTime(int p1, int p2, int p3) = 0;
    virtual void XModelDebugBoxes(int p1) = 0;
    virtual void XModelGet(char* p1) = 0;

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
};

extern IServer* server; // returned by the server DLL after initialization

//
// imports / exports
//
inline constexpr int kServerAPIVersion = 1;

#include "ForwardDecls.h"

struct ServerImports {
    int version{kServerAPIVersion};

    ISystem* system{nullptr};
    IFileSystem* file_system{nullptr};
    ICommandSystem* command_system{nullptr};
};

struct ServerExports {
    int version{kServerAPIVersion};

    IServer* server{nullptr};
};

using GetServerAPIPtr = ServerExports* (*) (ServerImports*);

} // namespace iocod

#endif // CORE_ISERVER_H
