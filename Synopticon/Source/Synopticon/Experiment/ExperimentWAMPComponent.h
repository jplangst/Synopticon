// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "WAMP/WAMPWorker.h"
#include "CoreMinimal.h"

#include "ExperimentDataStructure.h"
#include "SystemClasses/SynOpticonState.h"
#include "RunnableThread.h"
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
public:
	UExperimentWAMPComponent();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void NewImageAndAOIs(FImageAndAOIs ImageAndAOIs);
	void UpdatePopupAndAOIs(FPopupAndAOIs PopupAndAOIs);
	void StartStopExperiment(bool Start);
	void EventMarkerExperiment(FString Name, FString Description, FString SessionID);

	UFUNCTION(BlueprintCallable, Category = "Experiment Information")
	TArray<FString> GetListOfScreens();

	UFUNCTION(BlueprintCallable, Category = "Experiment Information")
	FImageAndAOIs GetCurrentImageAndAOIs(FString Screen);

	void OnReceiveImageAndAOIs(const autobahn::wamp_event& _event);
	void OnReceivePopupEvent(const autobahn::wamp_event& _event);
	void OnReceiveStartStopEvent(const autobahn::wamp_event& _event);
	void OnReceiveEventMarkers(const autobahn::wamp_event& _event);

	void OnReceiveHammlabEventMarkers(const autobahn::wamp_event& _event);

	void RegisterWAMP();
};