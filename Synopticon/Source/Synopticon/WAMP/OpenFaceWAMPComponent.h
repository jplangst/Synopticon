// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Utils/CleanWampHeader.h"

#include "CoreMinimal.h"
#include "CircularQueue.h"

#include "SystemClasses/SynOpticonState.h"
#include "SystemClasses/SynOpticonCustomType.h"
#include "WAMPWorker.h"

#include "Components/ActorComponent.h"

#include "OpenFaceWAMPComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYNOPTICON_API UOpenFaceWAMPComponent : public UActorComponent
{
	GENERATED_BODY()

	TMap<FString, TCircularQueue<FWAMPOpenFaceDataStruct*>*> OpenFaceDataQueuesMap;

	FString CurrentActorSource;

	TArray<FString> OpenFaceSources;
public:	
	// Sets default values for this component's properties
	UOpenFaceWAMPComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FWAMPOpenFaceDataStruct* GetOpenFaceDataSample(FString Source);

	void SetCurrentActorSource(FString Source);

	void OnReceiveOpenFaceData(const autobahn::wamp_event& _event);

	void RegisterWAMP();
};