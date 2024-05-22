// Fill out your copyright notice in the Description page of Project Settings.


#include "NatNetWorker.h"

int64 FNatNetWorker::ID = 0;
FNatNetWorker* FNatNetWorker::NatNetWorker = nullptr;

bool FNatNetWorker::ForcedStop = false;
char g_discoveredMulticastGroupAddr[kNatNetIpv4AddrStrLenMax] = NATNET_DEFAULT_MULTICAST_ADDRESS;

FNatNetWorker::FNatNetWorker(FString _ServerIP, FString _Port, NatNetClient* _g_pClient)
{
	ServerIP = _ServerIP;
	Port = _Port;
    g_pClient = _g_pClient;

	IsFinished = false;
	ClosedManually = false;
	ForcedStop = false;

	NatNetThread = FRunnableThread::Create(this, TEXT("NatNetThread" + FNatNetWorker::ID), 0, TPri_Normal);

	FNatNetWorker::ID++;
}

FNatNetWorker::~FNatNetWorker()
{
	if (NatNetThread)
	{
		delete NatNetThread;
		NatNetThread = nullptr;
	}
}

FNatNetWorker* FNatNetWorker::JoyInit(FString _ServerIP, FString _Port, NatNetClient* _g_pClient)
{
	if (!NatNetWorker && FPlatformProcess::SupportsMultithreading())
	{
		ForcedStop = false;
		NatNetWorker = new FNatNetWorker(_ServerIP, _Port, _g_pClient);
	}
	return NatNetWorker;
}

void FNatNetWorker::Shutdown()
{
	if (NatNetWorker)
	{
		NatNetWorker->ClosedManually = true;
		//NatNetWorker->EnsureCompletion();
		delete NatNetWorker;
		NatNetWorker = nullptr;
	}
	//FReconnectWAMPWorker::Shutdown();
}

bool FNatNetWorker::IsThreadFinished()
{
	return NatNetWorker == nullptr;
}

void FNatNetWorker::CleanUp()
{
    // Exiting - clean up
    if (NatNetWorker->g_pClient)
    {
        NatNetWorker->g_pClient->Disconnect();
        delete NatNetWorker->g_pClient;
        NatNetWorker->g_pClient = NULL;
    }

    if (NatNetWorker->g_pDataDefs)
    {
        NatNet_FreeDescriptions(NatNetWorker->g_pDataDefs);
        NatNetWorker->g_pDataDefs = NULL;
    }
}

void FNatNetWorker::Reconnect(FString _ServerIP, FString _Port)
{

}

uint32 FNatNetWorker::Run() {
    
    // Manually specify Motive server IP/connection type
    g_connectParams.connectionType = ConnectionType_Multicast;
    g_connectParams.localAddress = "127.0.0.1";
    g_connectParams.serverAddress = TCHAR_TO_ANSI(*ServerIP);
    g_connectParams.connectionType = ConnectionType_Multicast;
    g_connectParams.serverCommandPort = 1510;
    g_connectParams.serverDataPort = 1511;
    g_connectParams.multicastAddress = g_discoveredMulticastGroupAddr;

    // Connect to Motive
    int iResult = ConnectClient();
    if (iResult != ErrorCode_OK)
    {
        printf("Error initializing client. See log for details. Exiting.\n");
        return 1;
    }
    else
    {
        printf("Client initialized and ready.\n");
    }

    // Get latest asset list from Motive
    // Main thread loop
    // Data will be delivered in a separate thread to DataHandler() callback functon
    while (!ForcedStop)
    {
        // If Motive Asset list has changed, update our lookup maps
        UpdateDataDescriptions(true);

        FPlatformProcess::Sleep(0.050);
    }


	return 0;
}

int FNatNetWorker::ConnectClient()
{
    // Disconnect from any previous server (if connected)
    g_pClient->Disconnect();

    // Connect to NatNet server (e.g. Motive)
    int retCode = g_pClient->Connect(g_connectParams);
    if (retCode != ErrorCode_OK)
    {
        // Connection failed - print connection error code
        printf("[SampleClinet] Unable to connect to server.  Error code: %d. Exiting.\n", retCode);
        return ErrorCode_Internal;
    }
    else
    {
        // Connection succeeded
        void* pResult;
        int nBytes = 0;
        ErrorCode ret = ErrorCode_OK;

        // example : print server info
        memset(&g_serverDescription, 0, sizeof(g_serverDescription));
        ret = g_pClient->GetServerDescription(&g_serverDescription);
        if (ret != ErrorCode_OK || !g_serverDescription.HostPresent)
        {
            printf("[SampleClient] Unable to connect to server. Host not present. Exiting.\n");
            return 1;
        }

        // example : get mocap frame rate
        ret = g_pClient->SendMessageAndWait("FrameRate", &pResult, &nBytes);
        if (ret == ErrorCode_OK)
        {
            float fRate = *((float*)pResult);
            printf("Mocap Framerate : %3.2f\n", fRate);
        }
        else
        {
            printf("Error getting frame rate.\n");
        }

    }

    return ErrorCode_OK;
}

bool FNatNetWorker::UpdateDataDescriptions(bool printToConsole)
{
    // release memory allocated by previous in previous GetDataDescriptionList()
    if (g_pDataDefs)
    {
        NatNet_FreeDescriptions(g_pDataDefs);
    }

    // Retrieve Data Descriptions from Motive
    printf("\n\n[SampleClient] Requesting Data Descriptions...\n");
    int iResult = g_pClient->GetDataDescriptionList(&g_pDataDefs);
    if (iResult != ErrorCode_OK || g_pDataDefs == NULL)
    {
        return false;
    }

    UpdateDataToDescriptionMaps(g_pDataDefs);

    return true;
}

void FNatNetWorker::UpdateDataToDescriptionMaps(sDataDescriptions* pDataDefs)
{
    g_AssetIDtoAssetDescriptionOrder.Empty();
    g_AssetIDtoAssetName.Empty();
    int assetID = 0;
    FString assetName = "";
    int index = 0;

    if (pDataDefs == nullptr || pDataDefs->nDataDescriptions <= 0)
        return;

    for (int i = 0; i < pDataDefs->nDataDescriptions; i++)
    {
        assetID = -1;
        assetName = "";

        if (pDataDefs->arrDataDescriptions[i].type == Descriptor_RigidBody)
        {
            sRigidBodyDescription* pRB = pDataDefs->arrDataDescriptions[i].Data.RigidBodyDescription;
            assetID = pRB->ID;
            assetName = FString(pRB->szName);
        }

        if (assetID != -1)
        {
            // Add to Asset ID to Asset Name map
            g_AssetIDtoAssetName.Add(assetID, assetName);
            g_AssetIDtoAssetDescriptionOrder.Add(assetID, index++);
        }
    }

}

FString FNatNetWorker::getDataDescriptions(int _id) { 
    return NatNetWorker->g_AssetIDtoAssetName[_id];
}