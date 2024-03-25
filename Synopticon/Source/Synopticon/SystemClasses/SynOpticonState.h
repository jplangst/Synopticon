// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runnable.h"

#include "GameFramework/GameState.h"
#include "SynOpticonCustomType.h"
#include "GlobalEventSystem.h"
#include "ThreadPoolManager.h"
#include "GazeObjects/GazeActorDataTypes.h"

#include "Utils/SynOpticonStatics.h"
//#include "WAMP/WAMPWorker.h"
#include "Replay/ReplayDataStructures.h"

#include "SynOpticonState.generated.h"


USTRUCT(BlueprintType)
struct FHeatMapConditionStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heat Map Condition Struct")
	float Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heat Map Condition Struct")
	FLinearColor Color; 

	FHeatMapConditionStruct() {

	}

	FHeatMapConditionStruct(FLinearColor NewColor, float NewValue) {
		Color = NewColor;
		Value = NewValue;
	}
};

UCLASS()
class SYNOPTICON_API ASynOpticonState : public AGameState
{
	GENERATED_BODY()
	
private:
	//===========Screen Labeling=============
	static bool bLabelingScreens;

	//===========HEAT MAP====================
	static bool bHeatMapVisible;
	//True if the heat map should use all the data avaliable since the beginning
	static bool bHeatMapFromBeginning;
	//How far back in time should the Heat Map calculate values from
	static float fHeatMapHistoryDuration;
	//A list of HeatMap conditions
	static TArray<FHeatMapConditionStruct> HeatMapConditions;

	//===========SynOpticon Actor Control====================
	static int CurrentSynOpticonActorIndex; //The index of the current actor
	static TArray<AActor*> SynOpticonActorList;
	static TArray<AActor*> GazeActorList;
	static TArray<AActor*> GazeParentActorList;
	static TMap<int32, FString> GazeGroupList;
	static TArray<UActorComponent*> GazeMaterialComponents;

	//==========Gaze Point===================
	static bool bGazePointParticlesEnabled;
	static bool bShowViewCone;

	//==========SMOOTHING====================
	static float fSmoothingMomentum;

	//==========RAY CASTING==================
	static bool bRaycastingEnabled;
	static int32 ConeDivisions; //The number of divisions to use when ray tracing a cone shape
	static float ConeAngleDegrees; //The angle of the cone (in degrees) when ray tracing a cone shape

	//==========VECTOR VISIBILITY============
	static bool bEyeVectorVisible;
	static bool bOrientationVectorVisible;
	static bool bConvergenceVectorVisible;

	//============Gaze Actor Screen===========

	static FDateTime dHeatMapStartTime;
	static int32 CreatedGazeObjects;
	//============Coordinate System===========
	static bool HorizontalCoordinateSystem;

	//==============Record State==============
	static bool bRecording;
	
	//=============Replay State===============
	static bool bReplaying;
	static bool bPausingReplay;
	static FDateTime CurrentReplayTime;

	static int32 SplitRecordingTimeThreshold;

	static UGlobalEventSystem* globalEventSystem;
	static UThreadPoolManager* threadPoolManager;

	//===============available components===============
	static TMap<ComponentTypeEnum, TArray<FString>> AvailableComponents; //the map structure is: {Glasses: {GlassesID1: SessionID1, GlassesID2: SessionID}}


	static bool BApplyOffset;
	static bool BApplyDebugLines;

	//======================SETTINGS====================
	static FSettingsStruct Settings;
	
	//====================== AOI ====================
	static bool BCreatingAOI;
	static bool bAOIVisible;
	static bool bMicroTaskOverlayVisible;

	//======================CalToolSettings=============
	static int32 CalToolForwardAxis;

	//====================EyeRadar====================
	static bool bEyeRadarVisible;
public:
	static void AddComponentToMap(ComponentTypeEnum type, FString name);
	static TMap<int32, TPair<FString, bool>> AOISessionDict;

	//======================CalToolSettings=============
	UFUNCTION(BlueprintPure, Category = "Calibration Tool Settings")
	static int32 GetCalToolForwardAxis();

	UFUNCTION(BlueprintCallable, Category = "Calibration Tool Settings")
	static void SetCalToolForwardAxis(int32 ForwardAxis);
	
	UFUNCTION(BlueprintPure, Category = "Recording Parameters")
	static int32 GetSplitRecordingTimeThreshold();

	UFUNCTION(BlueprintCallable, Category = "Recording Parameters")
	static void SetSplitRecordingTimeThreshold(int32 minutes);

	UFUNCTION(BlueprintCallable, Category = "Screen Labeling")
	static void SetScreenLabeling(bool ScreenLabeling);

	UFUNCTION(BlueprintPure, Category = "Screen Labeling")
	static bool GetScreenLabeling();

	//Called from the game mode begin play. Don't use elsewhere
	static void InitiateState(UGlobalEventSystem* globalEventSystem, UThreadPoolManager* _threadPoolManager);

	//Called from the game mode end play. Don't use elsewhere
	static void CleanupState();

	UFUNCTION(BlueprintCallable, Category = "Debug Offset")
	static void ApplyOffset(bool apply);

	UFUNCTION(BlueprintCallable, Category = "Debug Offset")
	static bool ShouldApplyOffset();

	UFUNCTION(BlueprintCallable, Category = "Debug Offset")
		static void ShowDebugLines(bool Show);

	UFUNCTION(BlueprintCallable, Category = "Debug Offset")
		static bool IsShowingDebugLines();

	static UThreadPoolManager* GetThreadPoolManager();

	//========== AOI ===================
	UFUNCTION(BlueprintPure, Category = "Create AOI")
		static bool IsCreatingAOI() { return BCreatingAOI; }

	UFUNCTION(BlueprintCallable, Category = "Create AOI")
		static void SetCreatingAOI(bool CreatingAOI);

	//==========Scene Camera===================
	static bool bShowSceneCamera;
	
	UFUNCTION(BlueprintPure, Category = "SceneCamera")
	static bool IsSceneCameraVisible() { return bShowSceneCamera; }

	UFUNCTION(BlueprintCallable, Category = "SceneCamera")
	static void ShowSceneCamera(bool ShowSceneCamera);

	//====================Gaze Point================
	//Toggle the visibility of the GazePoint Particles on and off
	UFUNCTION(BlueprintCallable, Category = "GazeData")
	static void ToggleGazePointParticlesVisibility();
	//Returns true when the gaze point particles are visible.
	UFUNCTION(BlueprintPure, Category = "GazeData")
	static bool IsGazePointParticlesEnabled() { return bGazePointParticlesEnabled; }
	UFUNCTION(BlueprintPure, Category = "ViewCone")
		static bool IsShowingViewCone();
	UFUNCTION(BlueprintCallable, Category = "ViewCone")
		static void ToggleShowingViewCone();

	//====================HEAT MAP=================
	//Returns true if the heat map is visible
	UFUNCTION(BlueprintPure, Category = "HeatMap")
	static bool IsHeatMapVisible();
	//How much of the alpha should consist of the heatmap
	static float HeatMapAlpha;
	//Toggle the visibility of the heat map on and off
	UFUNCTION(BlueprintCallable, Category = "HeatMap")
	static void ToggleHeatMapVisibility();
	//Get the duration the heat map uses to calculate its values
	UFUNCTION(BlueprintPure, Category = "HeatMap")
	static float GetHeatMapHistoryDuration() { return fHeatMapHistoryDuration; }
	//Check if the heat map is calculating values from the beginning of the start time. Returns true if it is and false if it is using set intervals
	UFUNCTION(BlueprintPure, Category = "HeatMap")
	static bool IsHeatMapFromBeginning() { return bHeatMapFromBeginning; }

	//====================== AOI ====================
	static float AOIAlpha;
	UFUNCTION(BlueprintCallable, Category = "AOI")
		static void ToggleAOIVisibility();
	UFUNCTION(BlueprintPure, Category = "AOI")
		static bool IsAOIVisible();

	//===================== Micro Task ===================
	UFUNCTION(BlueprintCallable, Category = "AOI")
		static void ToggleMicroTaskOverlayVisibility();
	UFUNCTION(BlueprintPure, Category = "AOI")
		static bool IsMicroTaskOverlayVisible();

	//Set the conditions that are used to decide the colors of the heat map
	UFUNCTION(BlueprintCallable, Category = "HeatMap")
	static void SetHeatMapConditions(TArray<FHeatMapConditionStruct> NewHeatMapConditions);
	//Get the current heat map conditions
	UFUNCTION(BlueprintPure, Category = "HeatMap")
	static TArray<FHeatMapConditionStruct> GetHeatMapConditions() { return HeatMapConditions; }

	//Set the history duration the heat map should use. Set it to 0 to have the heat map use all the data avaliable from the beginning of the time
	UFUNCTION(BlueprintCallable, Category = "HeatMap")
	static void SetHeatMapHistoryDuration(float HeatMapHistoryDuration);

	//Set the Heat Map to use the default values
	UFUNCTION(BlueprintCallable, Category = "HeatMap")
	static void SetDefaultHeatMap();

	//Set the start time of the heat map to be now
	UFUNCTION(BlueprintCallable, Category = "HeatMap")
	static void SetStartTimeToNow();
	//Get the start time of the heat map
	UFUNCTION(BlueprintPure, Category = "HeatMap")
	static FDateTime GetStartTime();

	//================SMOOTHING====================
	//Get the current smoothing momentum
	UFUNCTION(BlueprintPure, Category = "Smoothing Momentum")
	static float GetSmoothingMomentum() { return fSmoothingMomentum; }
	//Add or subtract to the smoothing momentum
	UFUNCTION(BlueprintCallable, Category = "Smoothing Momentum")
	static void AlterSmoothingMomentum(float value);

	//===============RAYCASTING====================
	//Toggle the raycasting on and off
	UFUNCTION(BlueprintCallable, Category = "Raycasting")
	static void ToggleRaycasting();
	//Check if raycasting is enabled. Returns true if it is.
	UFUNCTION(BlueprintPure, Category = "Raycasting")
	static bool IsRaycastingEnabled() { return bRaycastingEnabled; }

	//Get the amount of Divisions to use for cone tracing
	UFUNCTION(BlueprintPure, Category = "Raycasting")
		static int32 GetConeDivisions() { return ConeDivisions; }
	//Set the amount of Divisions to use for cone tracing
	UFUNCTION(BlueprintCallable, Category = "Raycasting")
		static void SetConeDivisions(int32 ConeDivisions);

	//Get the angle of the cone to use for cone tracing
	UFUNCTION(BlueprintPure, Category = "Raycasting")
		static float GetConeAngle() { return ConeAngleDegrees; }
	//Set the angle of the cone to use for cone tracing
	UFUNCTION(BlueprintCallable, Category = "Raycasting")
		static void SetConeAngle(float ConeAngleDegrees);

	//===============VECTOR VISIBILITY===============
	//Toggle specific vector's visibility on and off
	UFUNCTION(BlueprintCallable, Category = "Vector Visibility")
		static bool ToggleVectorsVisibility(VectorVisibilityEnum _Type);
	//Check if the orientation vector is visible. Returns true if it is
	UFUNCTION(BlueprintPure, Category = "Vector Visibility")
		static bool IsVectorVisible(VectorVisibilityEnum _Type);

	//Set the visibility of the Orientation vector
	static void SetOrientationVectorVisibility(bool Visibility);

	//Set the visibility of the eye vectors
	static void SetEyeVectorVisibility(bool Visibility);

	//Set the visibility of the Convergence vector
	static void SetConvergenceVectorVisibility(bool Visibility);

	//=================SETTINGS======================
	UFUNCTION(BlueprintPure, Category = "Setting Options")
		static float GetEyesTrackingUnitToUnrealUnitFactor() { return Settings.EyesTrackingUnitToUnrealUnitFactor; }
	UFUNCTION(BlueprintPure, Category = "Setting Options")
		static float GetPosTrackingUnitToUnrealUnitFactor() { return Settings.PosTrackingUnitToUnrealUnitFactor; }

	UFUNCTION(BlueprintPure, Category = "Setting Options")
		static bool IsHorizontalCoordinateSystem() { return HorizontalCoordinateSystem; }

	UFUNCTION(BlueprintCallable, Category = "Setting Options")
		static void SetHorizontalCoordinateSystem(bool _Horizontal);

	//=========================GROUNDPOINT===============================
	UFUNCTION(BlueprintPure, Category = "Setting Options")
		static FVector GetGroundPointLocation();
	UFUNCTION(BlueprintPure, Category = "Setting Options")
		static FRotator GetGroundPointRotation();

	UFUNCTION(BlueprintPure, Category = "Setting Options")
		static FVector GetTrackerOffset();

	UFUNCTION(BlueprintPure, Category = "Setting Options")
		static FVector GetGlassesOffset();

	UFUNCTION(BlueprintCallable, Category = "Setting Options")
		static void SetTrackerOffset(FVector _Offset);

	UFUNCTION(BlueprintCallable, Category = "Setting Options")
		static void SetGlassesOffset(FVector _Offset);

	UFUNCTION(BlueprintPure, Category = "Setting Options")
		static float GetScreenSaveRate();

	UFUNCTION(BlueprintCallable, Category = "Setting Options")
		static void SetScreenSaveRate(int32 FPS, bool Seconds);

	UFUNCTION(BlueprintPure, Category = "Setting Options")
		static int32 GetScreenSaveRateFPS();

	UFUNCTION(BlueprintPure, Category = "Setting Options")
		static bool IsScreenSaveRatePerSeconds();

	UFUNCTION(BlueprintPure, Category = "Setting Options")
		static FString GetWAMPRouterAdress();

	UFUNCTION(BlueprintCallable, Category = "Setting Options")
		static void SetWAMPRouterAdress(FString adress);

	UFUNCTION(BlueprintPure, Category = "Setting Options")
		static FString GetWAMPRealm();

	UFUNCTION(BlueprintCallable, Category = "Setting Options")
		static void SetWAMPRealm(FString realm);

	UFUNCTION(BlueprintPure, Category = "Setting Options")
		static FSettingsStruct GetSettings();

	UFUNCTION(BlueprintCallable, Category = "Setting Options")
		static void ApplySettings(FSettingsStruct Settings);
	//==================Algorithm Thresholds==================

	static double FIXATION_VELOCITY_THRESHOLD;
	static double SACCADE_VELOCITY_THRESHOLD;
	static float ONSET_THRESHOLD;
	static float REVERSAL_THRESHOLD;
	static float OFFSET_THRESHOLD;
	static float FIXATION_DISTANCE_THRESHOLD;
	static float FIXATION_DURATION_THRESHOLD;
	static float DOUBLE_BLINK_THRESHOLD;
	UFUNCTION(BlueprintCallable, Category = "Modelling Menu")
		static int32 IncrementCreatedGazeObjects();

	//====================================Replay states====================================
	//---------Record----------
	UFUNCTION(BlueprintPure, Category = "Record Status")
	static bool IsRecording();

	UFUNCTION(BlueprintCallable, Category = "Record Status")
	static void SetRecordingStatus(bool _Recording);

	//---------Replay------------
	static CompressedHeader ReplayHeader;
	UFUNCTION(BlueprintPure, Category = "Replay Status")
	static bool IsReplaying();
	UFUNCTION(BlueprintCallable, Category = "Replay Status")
	static void SetReplayingStatus(bool _Replaying);

	UFUNCTION(BlueprintPure, Category = "Replay Status")
	static bool IsPausingReplay();

	UFUNCTION(BlueprintCallable, Category = "Replay Status")
	static void SetPausingReplayStatus(bool _Pausing);

	UFUNCTION(BlueprintPure, Category = "Replay Status")
	static FDateTime GetCurrentReplayTime();
	static void SetCurrentReplayTime(FDateTime _Time);
	static FSyncReplayStruct ReplaySyncData;

	/*
	* Used to let the different parts of the system know that we are seeking in the replay file and that
	* we should skip all expensive operations that are uneeded to speed up the processing
	*/
	static bool IsProcessingReplayData;

	//====================================Handle states====================================
	UFUNCTION(BlueprintPure, Category = "Global Event System")
	static UGlobalEventSystem* GetGlobalEventSystem();

	//============================Component meta data==============================
	static void OnReceiveChangeOfComponent(const string event);

	static void InitializeAvailableComponents(string result);

	UFUNCTION(BlueprintPure, Category = "Available Components")
		static TArray<FString> GetAvailableComponents(ComponentTypeEnum Type);

	//========================SynOpticonActor Control====================================
		static int GetCurrentActorIndex() { return CurrentSynOpticonActorIndex; } //Returns the index of the actor in the SO ArrayList. It is not the ID of the CurrentActor.

	UFUNCTION(BlueprintCallable, Category = "Change glasses") //---
		static void NextSynOpticonActor();

	UFUNCTION(BlueprintCallable, Category = "Change glasses") //---
		static void PreviousSynOpticonActor();

	UFUNCTION(BlueprintPure, Category = "Control Panel") //---
		static AActor* GetCurrentActor();

	UFUNCTION(BlueprintCallable, Category = "Main Menu") //---
		static int GetNumberOfSOActors();

	UFUNCTION(BlueprintPure, Category = "SynOpticon Actors") //---
		static const TArray<AActor*> GetSynOpticonActorList() { return SynOpticonActorList; }

	UFUNCTION(BlueprintPure, Category = "Gaze Actors") //---
		static const TArray<AActor*> GetGazeActorList() { return GazeActorList; }

	UFUNCTION(BlueprintPure, Category = "Gaze Parent Actors") //---
		static const TArray<AActor*> GetGazeParentActorList() { return GazeParentActorList; }

	UFUNCTION(BlueprintPure, Category = "Gaze Groups") //---
		static TArray<FGazeGroupData> GetGazeGroupData();

	UFUNCTION(BlueprintPure, Category = "Gaze Groups") //---
		static TArray<FString> GetGazeGroupList();

	UFUNCTION(BlueprintPure, Category = "Gaze Groups") //---
		static FString GetGazeGroup(int32 ID);

	UFUNCTION(BlueprintPure, Category = "Gaze Groups") //---
		static int32 GetGazeGroupID(FString GroupName);

	UFUNCTION(BlueprintPure, Category = "GazeMaterial Components") //---
		static const TArray<UActorComponent*> GetGazeMaterialComponentList() { return GazeMaterialComponents; }

	static void AddSOActor(AActor* SOActor);
	static void RemoveSOActor(AActor* SOActor);
	static void ClearSOActorList();
	static void AddGazeActor(AActor* GazeActor);
	static void RemoveGazeActor(AActor* GazeActor);
	static void ClearGazeActorList();
	static void AddGazeParentActor(AActor* GazeParentActor);
	static void RemoveGazeParentActor(AActor* GazeParentActor);
	static void ClearGazeParentActorList();

	UFUNCTION(BlueprintCallable, Category = "Gaze Groups") //---
	static void AddGazeGroup(FString GazeGroup, int32 ID);
	UFUNCTION(BlueprintCallable, Category = "Gaze Groups") //---
	static void RemoveGazeGroup(FString GazeGroup);
	UFUNCTION(BlueprintCallable, Category = "Gaze Groups") //---
	static void EditGazeGroup(FString NewGazeGroupName, FString OldGazeGroupName);
	UFUNCTION(BlueprintCallable, Category = "Gaze Groups") //---
	static void ClearGazeGroup();

	static void AddMaterialComponent(UActorComponent* MaterialComponent);
	static void RemoveMaterialComponent(int32 Index);
	static void ClearMaterialComponentList();
	
	UFUNCTION(BlueprintPure, Category = "Heat Map")
	static FLinearColor GetHeatMapColorFromValue(float HeatMapValue);

	UFUNCTION(BlueprintPure, Category = "EyeRadar")
		static bool IsEyeRadarVisible();
	UFUNCTION(BlueprintCallable, Category = "EyeRadar")
		static void SetEyeRadarVisible(bool EyeRadarVisible);
};