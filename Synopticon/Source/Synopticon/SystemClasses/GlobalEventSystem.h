// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GazeEventDataStructures.h"
#include "SynOpticonCustomType.h"
#include "Components/ActorComponent.h"
#include "Experiment/ExperimentDataStructure.h"
#include "GlobalEventSystem.generated.h"

//======================GLOBAL==========================
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FErrorMessage, FString, _Message);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlaySoundEffectDelegate, ESoundEffectEnum, _SoundEffect);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSceneCameraDelegate, UTexture2D*, _SceneCameraTexture);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerAdressDelegate, FString, _ServerAdress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGazeGestureDelegate, GazeGestureEnum, _GazeGesture);

//======================Viewport Resize Events==============================================
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FViewportResized, FVector2D, _ViewportSize);

//======================Micro Task Events==============================================
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMicroTaskEventDelegate, MicroTaskEventTypeEnum, _MicrotaskEventType, FActorTaskStruct, _ActorTaskStruct);

//TODO remove later
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRightYokeDelegate, FVector, _YokeLocation, FRotator, _YokeOrientation, FVector, _LocalOffset);

//=================================Screen Labeling=================================
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScreenSelectedDelegate, FString, _ScreenName);

//=================================SynOpticon Actors===============================
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSwitchCurrentSOActorDelegate);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCreateNewSOActorDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FUpdateCurrentSOActorDelegate, int, ActorID, bool, HasETG, bool, HasSHimmerHR, bool, HasShimmerGSR, FString, ETEDevice, FString, ShimmerDevice);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCreateSOActorDelegate, int, ActorID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRemoveSOActorDelegate, int, ActorID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEditSOActorDelegate, int, ActorID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStatisticsExpandCollapseDelegate, bool, Expand);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReceiveNewPupilSampleDelegate, float, Sample);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSOActorChangedDelegate);

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateGazeActorStatisticsDelegate);

//================================Gaze Actor AOIs==================================
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRemoveAOIDelegate, int32, AOIID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEditAOIDelegate, int32, AOIID);

//================================Gaze Actor Groups==================================
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRemoveGazeGroupDelegate, int32, GroupID);
//
////==============================Gaze Actors================================
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCreateGazeActorDelegate, int32, ActorID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEditGazeActorDelegate, int32, ActorID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRemoveGazeActorDelegate, int32, ActorID);

//===========================================================================
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVectorsVisibilityDelegate, VectorVisibilityEnum, _VectorType);

//=========================delegates menu====================================
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnterMenuContextDelegate, UIContextEnum, MenuContext);

//======================delegates control replay states======================
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStartRecordDelegate, bool, _Start);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStartReplayDelegate, bool, _Start);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPauseReplayDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FIsReplayingDelegate, float, _Percentage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSeekingDelegate, FString, _CallerID);

//=============================WAMP delegates=================================
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReceiveMicroTaskEventDelegate, FMicroTaskEventStruct, _Package);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWAMPComponentDisconnected, ComponentTypeEnum, _ComponentType, FString, _ComponentID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWAMPComponentConnected, ComponentTypeEnum, _ComponentType, FString, _ComponentID);

//===============================Experiment===================================
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReceiveNewImageAndAOIsDelegate, FImageAndAOIs, _ImageAndAOIs);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReceivePopupAndAOIsDelegate, FPopupAndAOIs, _PopupAndAOIs);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartExperimentDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopExperimentDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FEventMarkerExperimentDelegate, FString, _Name, FString, _Description, FString, _SessionID);

UCLASS( )
class SYNOPTICON_API UGlobalEventSystem : public UObject
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGlobalEventSystem();

	//Screen labeling
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Screen Labeling")
	FScreenSelectedDelegate OnScreenSelected;

	//Scene camera 
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Scene Camera")
		FSceneCameraDelegate OnEventNewSceneCameraImage;


	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Update Server Adress")
		FServerAdressDelegate OnEventUpdateServerAdress;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Gaze Gestures")
		FGazeGestureDelegate OnEventGazeGesture;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Gaze Gestures")
		FViewportResized OnEventViewportResize;

	//=============Micro Task Delegates========================
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Micro Task")
		FMicroTaskEventDelegate OnMicroTaskEventDelegate;

	//=============Actor's delegates================
	UPROPERTY(BlueprintAssignable, Category = "SynOpticon Actor")
		FUpdateCurrentSOActorDelegate OnEventUpdateCurrentSOActor;
	UPROPERTY(BlueprintAssignable, Category = "SynOpticon Actor")
		FCreateSOActorDelegate OnEventCreateSOActor;
	UPROPERTY(BlueprintAssignable, Category = "SynOpticon Actor")
		FRemoveSOActorDelegate OnEventRemoveSOActor;
	UPROPERTY(BlueprintAssignable, Category = "SynOpticon Actor")
		FEditSOActorDelegate OnEventEditSOActor;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "SynOpticon Actor")
		FSOActorChangedDelegate OnSOActorChanged;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "SynOpticon Actor")
		FStatisticsExpandCollapseDelegate OnStatisticsExpandCollapse;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "SynOpticon Actor")
		FReceiveNewPupilSampleDelegate OnPupilSampleRecieved;
	////==============================Gaze Actors================================
	UPROPERTY(BlueprintAssignable, Category = "Gaze Object")
		FCreateGazeActorDelegate OnEventCreateGazeActor;
	UPROPERTY(BlueprintAssignable, Category = "Gaze Object")
		FEditGazeActorDelegate OnEventEditGazeActor;
	UPROPERTY(BlueprintAssignable, Category = "Gaze Object")
		FRemoveGazeActorDelegate OnEventRemoveGazeActor;

	//TODO remove later
	UPROPERTY(BlueprintAssignable, Category = "Gaze Object")
	FRightYokeDelegate OnEventRightYokeUpdate;


	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Vector Visibility")
		FVectorsVisibilityDelegate OnEventToggleVectorsVisibility;

	//=======================broadcaster of replay states========================
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Replay States")
		FStartRecordDelegate OnEventStartRecord;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Replay States")
		FStartReplayDelegate OnEventStartReplay;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Replay States")
		FPauseReplayDelegate OnEventPauseReplay;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Replay States")
		FIsReplayingDelegate OnEventIsReplaying;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Replay States")
		FSeekingDelegate OnSeekingEvent;

	//===========================WAMP============================
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "WAMP")
		FReceiveMicroTaskEventDelegate OnEventReceiveMicroTaskEvent;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "WAMP")
		FWAMPComponentDisconnected OnEventWAMPComponentDisconnected;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "WAMP")
		FWAMPComponentConnected OnEventWAMPComponentConnected;

	//============================Menu===========================
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Menu Context")
		FEnterMenuContextDelegate OnEventEnterMenuContext;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "System")
		FErrorMessage OnErrorMessage;

	//==========================Sound Effects=================================
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Sound Effects")
		FPlaySoundEffectDelegate OnPlaySoundEffect; //Used to play a sound effect once

	//==========================Gaze Groups===========================
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Gaze Groups")
		FRemoveGazeGroupDelegate OnRemoveGazeGroupDelegate;

	//==========================AOIs===========================
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "AOIs")
		FRemoveAOIDelegate OnRemoveAOIDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "AOIs")
		FEditAOIDelegate OnEditAOIDelegate;

	//===============================Experiment===================================
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Experiment")
		FReceiveNewImageAndAOIsDelegate OnReceiveNewImageAndAOIsDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Experiment")
		FReceivePopupAndAOIsDelegate OnReceivePopupAndAOIsDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Experiment")
		FStartExperimentDelegate OnStartExperimentDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Experiment")
		FStopExperimentDelegate OnStopExperimentDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Experiment")
		FEventMarkerExperimentDelegate OnEventMarkerExperimentDelegate;
};
