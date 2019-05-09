// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "WAMP/WAMPWorker.h"

#include "CoreMinimal.h"
#include "CircularQueue.h"

#include "SystemClasses/SynOpticonState.h"
#include "SystemClasses/SynOpticonCustomType.h"

#include "Replay/ReplayDataStructures.h"

#include "Components/ActorComponent.h"
#include "RemoteTrackerWAMPComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYNOPTICON_API URemoteTrackerWAMPComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	FString CurrentActorRetID;
	TMap<FString, TCircularQueue<FWAMPRetDataStruct*>*> EyeDataQueuesMap;
	TCircularQueue<TPair<FString, float>>* ReplayPupilQueue;

public:	
	// Sets default values for this component's properties
	URemoteTrackerWAMPComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//void AddEyeDataSample(FString GlassesID, FWAMPRetDataStruct* EyeData);
	FWAMPRetDataStruct* GetEyeDataSample(FString GlassesID);

	void SetCurrentActorRetID(FString RetID);

	UFUNCTION()
		void OnRecordingStatusChanged(bool Started);

	void PublishReplayData(TPair<FString, float> Sample);
	TPair<FString, float> GetReplaySample();
	TArray<FString> RemoteTrackers;
	void OnReceiveEyeData(const autobahn::wamp_event& _event);

	void RegisterWAMP();
};