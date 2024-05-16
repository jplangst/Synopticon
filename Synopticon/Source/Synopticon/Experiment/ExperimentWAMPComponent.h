// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/RunnableThread.h"

#include "ExperimentDataStructure.h"
#include "../SystemClasses/SynOpticonState.h"
//#include "../WAMP/WAMPWorker.h"

#include "ExperimentWAMPComponent.generated.h"
/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SYNOPTICON_API UExperimentWAMPComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	TMap<FString, FImageAndAOIs> ScreenMap;
	TMap<FString, bool> RecordingSynchStatus;

	//Recording participation announcment
	bool WaitingForRecordingParticipants;
	float WaitingTimeout;
	float WaitingTimer;

public:
	UExperimentWAMPComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void NewImageAndAOIs(FImageAndAOIs ImageAndAOIs);
	void UpdatePopupAndAOIs(FPopupAndAOIs PopupAndAOIs);
	void StartStopExperiment(bool Start);
	void EventMarkerExperiment(FString Name, FString Description, FString SessionID);

	UFUNCTION(BlueprintCallable, Category = "Experiment Information")
	TArray<FString> GetListOfScreens();

	UFUNCTION(BlueprintCallable, Category = "Experiment Information")
	FImageAndAOIs GetCurrentImageAndAOIs(FString Screen);



	void OnReceiveImageAndAOIs(const string _event);
	void OnReceivePopupEvent(const string _event);
	void OnReceiveStartStopEvent(const string _event);
	void OnReceiveEventMarkers(const string _event);

	void OnReceiveHammlabEventMarkers(const string _event);

	void RegisterWAMP();


	//Recording synching
	void OnRecieveRecordingSynchEvent(const string _event);
	//Starts the process of synchronizing recordings
	void StartSynchingRecording();
};