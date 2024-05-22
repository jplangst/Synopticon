// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <NatNetTypes.h>
#include <NatNetCAPI.h>
#include <NatNetClient.h>

/**
 * 
 */
//char g_discoveredMulticastGroupAddr[kNatNetIpv4AddrStrLenMax] = NATNET_DEFAULT_MULTICAST_ADDRESS;

class SYNOPTICON_API FNatNetWorker : public FRunnable
{
private:
	/** Singleton instance **/
	static  FNatNetWorker* NatNetWorker;
	static int64 ID;

	/** Thread to run the worker FRunnable on **/
	FRunnableThread* NatNetThread;

	FString ServerIP;
	FString Port;
	NatNetClient* g_pClient;

	sDataDescriptions* g_pDataDefs = NULL;
	sNatNetClientConnectParams g_connectParams;
	sServerDescription g_serverDescription;

	TMap<int32, int32> g_AssetIDtoAssetDescriptionOrder;
	TMap<int32, FString> g_AssetIDtoAssetName;

	bool ClosedManually;
	bool IsFinished;
	static bool ForcedStop;

	static void Reconnect(FString _ServerIP, FString _Port);

	int ConnectClient();
	bool UpdateDataDescriptions(bool printToConsole);
	void UpdateDataToDescriptionMaps(sDataDescriptions* pDataDefs);
public:
	static FNatNetWorker* JoyInit(FString _ServerIP, FString _Port, NatNetClient* _g_pClient);
	static void Shutdown();
	static void CleanUp();
	static bool IsThreadFinished();

	static FString getDataDescriptions(int _id);


	FNatNetWorker(FString _ServerIP, FString _Port, NatNetClient* _g_pClient);
	virtual ~FNatNetWorker();
	virtual uint32 Run();
};
