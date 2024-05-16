// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//#include "AllowWindowsPlatformTypes.h"
//#include <autobahn.hpp>
//#include <wamp_websocketpp_websocket_transport.hpp>
//#include <websocketpp/config/asio_no_tls_client.hpp>
//#include <websocketpp/client.hpp>
//
//#include <boost/version.hpp>
//#include <iostream>
//#include <memory>
//#include <string>
//#include <tuple>
//#include <thread>
//#include "HideWindowsPlatformTypes.h"

#include "Containers/CircularQueue.h"
#include "Components/ActorComponent.h"

#include "../Replay/ReplayDataStructures.h"
#include "../SystemClasses/SynOpticonState.h"
#include "../SystemClasses/SynOpticonCustomType.h"
#include "../SynOpticonActors/SynOpticonDataStructures.h"

#include "ShimmerVRPNComponent.generated.h"

#define TIME_OUT 5


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYNOPTICON_API UShimmerVRPNComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	/** The Galvanic Skin Response (KOhms) */
	double GSRValue;

	/** The Heart Rate (Beats/min) */
	double HRValue;

	/** True if the component is streaming HR*/
	bool HREnabled;

	/** True if the component is streaming GSR*/
	bool GSREnabled;

	//Should be either 0 or 1 depending on the order it was added to the Actor
	int32 ShimmerIndex;

	FString ShimmerDeviceName;

	bool ClosedManually;

	TMap<FString, TCircularQueue<FVector2D*>*> ShimmerDataQueuesMap;

public:	
	/** Get the Galvanic Skin Response (KOhms) */
	double GetGSRValue();

	/** Get the Heart Rate (Beats/min) */
	double GetHRValue();

	// Sets default values for this component's properties
	UShimmerVRPNComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called when the game end
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;
		
	/** Whether connected to the shimmer */
	bool IsConnected();

	/** Set wether the attached shimmer device is streaming Heart Rate data*/
	void SetHREnabled(bool HREnabled);

	/** Set wether the attached shimmer device is streaming Galvanic Skin Response data*/
	void SetGSREnabled(bool GSREnabled);

	void SetShimmerLogName(FString LogName);

	void SetShimmerDeviceName(FString DeviceName);

	UFUNCTION(BlueprintPure, Category = "Shimmer Server")
		FString GetShimmerDeviceName() { return ShimmerDeviceName; };

	/** True if the connected shimmer device is streaming Heart Rate data*/
	UFUNCTION(BlueprintCallable, Category = "Shimmer Server")
	bool HasHR();

	/** True if the connected shimmer device is streaming Galvanic SKin Response data*/
	UFUNCTION(BlueprintCallable, Category = "Shimmer Server")
	bool HasGSR();

	//Used to distinguish the two possible shimmer components from eachother
	UFUNCTION(BlueprintCallable, Category = "Shimmer Server")
	int32 GetShimmerIndex();
	void SetShimmerIndex(int32 NewShimmerIndex);

	//UFUNCTION()
	//	void OnShimmerWAMPComponentDisconnected(ComponentTypeEnum ComponentType, FString ComponentID);
	//UFUNCTION()
	//	void OnShimmerWAMPComponentConnected(ComponentTypeEnum ComponentType, FString ComponentID);

	//void ReconnectToRouter();

	//void AddShimmerDataSample(FString ShimmerID, FVector2D* ShimmerData);

	void ToBinaryReplayData();

	void SetSOBlock(CompressedSOBlock* _SOBlock);
};


//typedef websocketpp::client<websocketpp::config::asio_client> client;
//
//class SYNOPTICON_API FShimmerWAMPWorker : public FRunnable
//{
//	/** Singleton instance **/
//	static  FShimmerWAMPWorker* ShimmerWAMPWorker;
//
//	/** Thread to run the worker FRunnable on **/
//	FRunnableThread* WAMPThread;
//
//	///** Use Thread Safe to stop thread **/
//	//FThreadSafeCounter StopNatNetConnect;
//
//private:
//	FString ServerIP;
//	FString Realm;
//	FString Topic;
//
//	bool ClosedManually;
//	bool IsFinished;
//	static bool ForcedStop;
//	bool Replay;
//
//	boost::asio::io_service io;
//
//	static void OnReceiveShimmerData(const autobahn::wamp_event& event);
//
//public:
//	static UShimmerVRPNComponent* ShimmerVRPNComponent;
//
//	FShimmerWAMPWorker(UShimmerVRPNComponent* _ShimmerVRPNComponent, FString _ServerIP, FString _Realm, FString _Topic, bool _Replay);
//	virtual ~FShimmerWAMPWorker();
//
//	// Begin FRunnable interface.
//	virtual uint32 Run();
//	// End FRunnable interface
//
//	/** Makes sure this thread has stopped properly */
//	void EnsureCompletion();
//
//	static FShimmerWAMPWorker* JoyInit(UShimmerVRPNComponent* _ShimmerVRPNComponent, FString _ServerIP, FString _Realm, FString _Topic, bool _Replay);
//
//	/** Shuts down the thread. Static so it can easily be called from outside the thread context */
//	static void Shutdown();
//
//	static bool IsThreadFinished();
//};
//
//class SYNOPTICON_API FReconnectShimmerWAMPWorker : public FRunnable
//{
//	/** Singleton instance **/
//	static  FReconnectShimmerWAMPWorker* ReconnectShimmerWAMPWorker;
//
//	/** Thread to run the worker FRunnable on **/
//	FRunnableThread* ReconnectShimmerWAMPThread;
//
//	///** Use Thread Safe to stop thread **/
//	//FThreadSafeCounter StopNatNetConnect;
//
//private:
//	FString ServerIP;
//	FString Realm;
//	FString Topic;
//
//public:
//	static UShimmerVRPNComponent* ShimmerVRPNComponent;
//
//	FReconnectShimmerWAMPWorker(UShimmerVRPNComponent* _ShimmerVRPNComponent, FString _ServerIP, FString _Realm, FString _Topic);
//	virtual ~FReconnectShimmerWAMPWorker();
//
//	// Begin FRunnable interface.
//	virtual uint32 Run();
//	// End FRunnable interface
//
//	/** Makes sure this thread has stopped properly */
//	void EnsureCompletion();
//
//	static FReconnectShimmerWAMPWorker* JoyInit(UShimmerVRPNComponent* _ShimmerVRPNComponent, FString _ServerIP, FString _Realm, FString _Topic);
//
//	/** Shuts down the thread. Static so it can easily be called from outside the thread context */
//	static void Shutdown();
//};