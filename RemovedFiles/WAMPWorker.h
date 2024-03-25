// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Utils/CleanWampHeader.h"
#include "SystemClasses/SynOpticonCustomType.h"
#include "SystemClasses/SynOpticonState.h"

#include "CoreMinimal.h"

/**
 * Currently: 
	1. EyeTrackingWAMPComponent 
	2. RemoteTrackerWAMPComponent
	3. NatNetWAMPComponent
	4. ExperimentWAMPComponent
	5. ASynOpticonState
	(6). Shimmer
 */
typedef websocketpp::client<websocketpp::config::asio_client> client;
typedef msgpack::v1::object object;
class SYNOPTICON_API FWAMPWorker : public FRunnable
{

private:
	/** Singleton instance **/
	static  FWAMPWorker* WAMPWorker;
	static int64 ID;
	//static TMap<FString, const wamp_event_handler> WAMPTopics;
	static TArray<TPair<FString, TSharedPtr<wamp_event_handler>>> WAMPTopics;

	/** Thread to run the worker FRunnable on **/
	FRunnableThread* WAMPThread;

	FString ServerIP;
	FString Realm;

	bool ClosedManually;
	bool IsFinished;
	static bool ForcedStop;

	//For publishing events
	static TQueue<FPublishWAMPEventStruct*> WAMPEventsToPublish;
	void PublishEvents(std::shared_ptr<autobahn::wamp_session> session);

	boost::asio::io_service io;

	static void Reconnect(FString _ServerIP, FString _Realm);
public:
	static void SubscribeToTopic(FString Topic, TSharedPtr<wamp_event_handler> Handler);
	static FWAMPWorker* JoyInit(FString _ServerIP, FString _Realm);
	static void Shutdown();
	static void CleanUp();
	static bool IsThreadFinished();

	//For publishing events
	static void PublishWAMPEvent(FPublishWAMPEventStruct* WAMPEvent);

	FWAMPWorker(FString _ServerIP, FString _Realm);
	virtual ~FWAMPWorker();
	virtual uint32 Run();
		
	void EnsureCompletion();
};

class SYNOPTICON_API FReconnectWAMPWorker : public FRunnable
{
private:
	static  FReconnectWAMPWorker* ReconnectWAMPWorker;
	static int64 ID;

	FRunnableThread* ReconnectWAMPThread;

	FString ServerIP;
	FString Realm;

public:
	FReconnectWAMPWorker(FString _ServerIP, FString _Realm);
	virtual ~FReconnectWAMPWorker();

	virtual uint32 Run();

	void EnsureCompletion();

	static FReconnectWAMPWorker* JoyInit(FString _ServerIP, FString _Realm);

	/** Shuts down the thread. Static so it can easily be called from outside the thread context */
	static void Shutdown();
};
