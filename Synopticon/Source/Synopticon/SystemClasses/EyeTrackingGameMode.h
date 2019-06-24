// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WAMP/WAMPWorker.h"

#include "GlobalEventSystem.h"

#include <Viewport.h>

//Utilities
#include "Utils/SynOpticonStatics.h"

#include "CsvParser.h"

//Actor Components
#include "WAMP/OpenFaceWAMPComponent.h"
#include "WAMP/EyeTrackingWAMPComponent.h"
#include "WAMP/MicroTaskWAMPComponent.h"
#include "WAMP/ImagerWAMPComponent.h"
#include "WAMP/WAMPWorker.h"
#include "CreationAndDeletionManager.h"

#include "GazeObjects/GazeObjectMaterialComponent.h"

//Replay
#include "Replay/ReplayDataStructures.h"
#include "Replay/RecordManager.h"
#include "Replay/ReplayManager.h"

//NatNet
#include "NatNet/NatNetWAMPComponent.h"

//Gaze Actors
#include "GazeObjects/GazeActorLoader.h"
#include "GazeObjects/GazeActor.h"
#include "GazeObjects/GazeActorFactory.h"
#include "GazeObjects/GazeParentActor.h"

//PositionModelling Actor
#include "PositionModelling/PositionModellingActor.h"
#include "PositionModelling/CalibrationActor.h"

//SynOpticon Actors
#include "SynOpticonActors/SynOpticonActorLoader.h"
#include "SynOpticonActors/SynOpticonFactory.h"
#include "SynOpticonActors/SynOpticonActor.h"

#include "Containers/TripleBuffer.h"

//System classes
#include "SystemClasses/SynOpticonState.h"

//Engine
#include "Runtime/Online/HTTP/Public/Http.h"

//Experiment
#include "Experiment/ExperimentWAMPComponent.h"

//Radiant UI
#include "GameFramework/HUD.h"

//Game Mode
#include "GameFramework/GameMode.h"
#include "EyeTrackingGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegatePlayingReplayFinished);

/**
 * 
 */
UCLASS()
class SYNOPTICON_API AEyeTrackingGameMode : public AGameMode
{
	GENERATED_BODY()
	
private:
	AEyeTrackingGameMode(const FObjectInitializer& ObjectInitializer);

	virtual void StartPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//Scene Camera
	UImagerWAMPComponent* ImagerWAMPComponent;

	//Used to prepare the replay when the actors are finished loading
	bool ShouldSetupReplay;

	//Spawns the modelling tool actors used for the modelling of objects
	void SpawnModellingTools();

	//Update the raycast components of all the SynOpticon actors that have one
	void UpdateRaycastComponents();

	//Update the heatmap of the current actor
	void UpdateCurrentActorHeatmap();

	//Called from the global event system, don't call manually
	UFUNCTION()
		void SynOpticonActorChanged();

	//----------------------Position Modelling Actor----------------------
	APositionModellingActor* ManualModellingTool;
	APositionModellingActor* LaserModellingTool;
	ACalibrationActor* CalibrationTool; //Prolly don't need the reference

	//----------------------Nat Net----------------------
	UNatNetWAMPComponent* NatNetWAMPComponent;
	UEyeTrackingWAMPComponent* EyeTrackingWAMPComponent;

	UOpenFaceWAMPComponent* OpenFaceWAMPComponent;

	//--------------------Experiment---------------------
	UExperimentWAMPComponent* ExperimentWAMPComponent;

	UMicroTaskWAMPComponent* MicroTaskWAMPComponent;

	//----------------------Remote Eye Tracker------------------
	URemoteTrackerWAMPComponent* RemoteTrackerWAMPComponent;

	TArray<FVector> CalibrationPoints;

	UPROPERTY()
	UGlobalEventSystem* GlobalEventSystem;
	UPROPERTY()
	UThreadPoolManager* ThreadPoolManager;
	UPROPERTY()
	UReplayManager* _ReplayManager;
public:	
	//=========================Creation Manager=============================
	//This is a value set in the EyeTrackingGameMode BP, used to create the BP version of the component
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Creation And Deletion Manager")
	TSubclassOf<UCreationAndDeletionManager> CreationManagerComponentBPP; 

	//The component that was created from the BP class
	UCreationAndDeletionManager* CreationManagerComponent;

	//=========================Record Manager===============================
	URecordManager* RecordManagerComponent;
	//=========================Scene Camera=================================
	void UpdateActorsSceneCameraStreaming(bool Stream); //---

	//=========================POSITION MODELLING===========================
	UFUNCTION(BlueprintCallable, Category = "Position Modelling") //---
		FVector CalculatePoint(float distance);

	//=========================CALIBRATION==================================
	UFUNCTION(BlueprintCallable, Category = "Calibration")
		void CalibrateVectorToPoint(int CalibrationPointNumber);

	UFUNCTION(BlueprintCallable, Category = "Calibration")
		void AddCalibrationPoint(FVector NewCalibrationPoint);

	UFUNCTION(BlueprintCallable, Category = "Calibration")
		void ClearCalibrationPoints();

	UFUNCTION(BlueprintCallable, Category = "Calibration") //---
		void ResetCurrentActorsCalibration();

	UFUNCTION(BlueprintCallable, Category = "Calibration") //---
		void SetGazeActorGroundplaneZOffset(float Offset);

	//=========================SAVE AND LOAD FILES==========================
	//Callback from the UI. Don't call manually
	UFUNCTION(BlueprintCallable, Category = "File Manager")
		void OpenOrSaveXMLFile(EFileDialogTypeEnum type, FString fileName);

	//==========================WAMP WORKER==========================
	UFUNCTION(BlueprintCallable, Category = "WAMP Worker")
		void RestartWAMP();

	//==========================Viewport Resize Callback======================
	void ViewportResizedCallback(FViewport * viewport, uint32 ID);

	//=========================LOGGING===============================
	//reset the logged data of the SynOpticonActors
	UFUNCTION(BlueprintCallable, Category = "Logging")
		void ResetLoggedData();

	//=========================UNREAL OVERRIDEN FUNCTIONS===============================
	virtual void Tick(float DeltaSeconds) override;

	void ResetMicroTaskStatus();
	UFUNCTION(BlueprintCallable, Category = "WAMP Worker")
	void OnRecieveMicroTaskEvent(FMicroTaskEventStruct MicroTaskEvent);

	//UFUNCTION(BlueprintImplementableEvent)
	//	void UpdateChartHUDVisibility();
	//End of testing
};
