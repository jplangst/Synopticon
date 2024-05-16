// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "SynOpticonDataStructures.h"
#include "SynOpticonActor.h"
#include "../SystemClasses/SynOpticonState.h"
#include "../NatNet/NatNetWAMPComponent.h"
//WAMP components
#include "../WAMP/EyeTrackingWAMPComponent.h"
#include "../WAMP/OpenFaceWAMPComponent.h"
//Actor components
#include "../ActorComponents/RemoteEyeTrackerComponent.h"
#include "../ActorComponents/EyeVectorsComponent.h"
#include "../ActorComponents/PositionAndOrientationComponent.h"
#include "../ActorComponents/RaycastComponent_Ours.h"
#include "../ActorComponents/SynOpticonAggLogComponent.h"
#include "../ActorComponents/WebcamComponent.h"
//#include "../ActorComponents/MyoInputComponent.h"
#include "../ActorComponents/OpenFaceComponent.h"
#include "../ActorComponents/ShimmerVRPNComponent.h"
#include "../ActorComponents/GazeInputComponent.h"
#include "../ActorComponents/HandComponent.h"

#include "SynOpticonFactory.generated.h"



/**
 * 
 */
UCLASS()
class SYNOPTICON_API USynOpticonFactory : public UObject
{
	GENERATED_BODY()

	static int32 Index; //keep track of the SynOpticon ID
	static bool ReplayMode;

	static TArray<FLinearColor> ActorColors;
	static void FillActorColors();
	//Create Components
	static void CreateRemoteTrackerComponent(ASynOpticonActor* NewSynOpticonActor, FRemoteEyeTrackerStruct RemoteEyeTrackerStruct, URemoteTrackerWAMPComponent* RetWAMPComp);

	static void CreateEyeVectorsComponent(ASynOpticonActor* NewSynOpticonActor, FEyeTrackerStruct EyeTrackerStruct, UEyeTrackingWAMPComponent* EyeTrackingWAMPComponent);

	static void CreatePosOriComponent(ASynOpticonActor* NewSynOpticonActor, FSynOpticonActorStruct SynOpticonActorStruct, UNatNetWAMPComponent* NatNetWAMPComponent);

	static void CreateMyoInputComponent(ASynOpticonActor* NewSynOpticonActor);

	static void CreateVisualComponent(ASynOpticonActor* NewSynOpticonActor, FOpenFaceStruct VisualComponentStruct, UOpenFaceWAMPComponent* OpenFaceWAMPComponent);

	static void CreateShimmerComponent(ASynOpticonActor* NewSynOpticonActor, FShimmerComponentStruct ShimmerCompStruct, int ShimmerIndex);

	static void CreateLogComponent(ASynOpticonActor* NewSynOpticonActor);

	static void CreateRaycastComponent(ASynOpticonActor* NewSynOpticonActor);

	static UHandComponent* CreateHandComponent(ASynOpticonActor* NewSynOpticonActor, FHandStruct HandCompStruct, UNatNetWAMPComponent* NatNetComponent);

	static void CreateVideoComponent(ASynOpticonActor* NewSynOpticonActor, FVideoComponentStruct VideoCompStruct, TSubclassOf<UWebcamComponent> VideoCompBP);

	//Edit Components
	static void EditVideoComponents(ASynOpticonActor* NewSynOpticonActor, TArray<FVideoComponentStruct> VideoCompStructs, TSubclassOf<UWebcamComponent> VideoCompBP);

	static bool EditRemoteTrackerComponent(ASynOpticonActor* NewSynOpticonActor, FRemoteEyeTrackerStruct RemoteEyeTrackerStruct, URemoteTrackerWAMPComponent* RetWAMPComp);

	static bool EditEyeVectorsComponent(ASynOpticonActor* NewSynOpticonActor, FEyeTrackerStruct EyeTrackerStruct, UEyeTrackingWAMPComponent* EyeTrackingWAMPComponent);

	static bool EditPosOriComponent(ASynOpticonActor* NewSynOpticonActor, FSynOpticonActorStruct SynOpticonActorStruct, UNatNetWAMPComponent* NatNetComponent);

	static void EditMyoInputComponent(ASynOpticonActor* Actor, FSynOpticonActorStruct SynOpticonActorStruct);

	static void EditShimmerComponents(ASynOpticonActor* NewSynOpticonActor, FSynOpticonActorStruct SynOpticonActorStruct);

	static void EditRaycastAndLogComponent(ASynOpticonActor* NewSynOpticonActor, bool hasEyeTrackingComponent, bool hasPosComponent);

public:
	static ASynOpticonActor* CreateSynOpticonActor(FSynOpticonActorStruct SynOpticonActorStruct, UWorld* World, 
		UNatNetWAMPComponent* NatNetComponent, 
		UEyeTrackingWAMPComponent* EyeTrackingWAMPComponent, 
		URemoteTrackerWAMPComponent* RetWAMPComp, 
		UOpenFaceWAMPComponent* OpenFaceWAMPComponent,
		bool ReplayMode, 
		TSubclassOf<UGazeDataVisualizerComponent> GazeVizBP,
		TSubclassOf<UWebcamComponent> VideoComponentBP);
	static void EditSynOpticonActor(ASynOpticonActor* Actor, FSynOpticonActorStruct SynOpticonActorStruct, UNatNetWAMPComponent* NatNetComponent, UEyeTrackingWAMPComponent* EyeTrackingWAMPComponent, URemoteTrackerWAMPComponent* RetWAMPComp, TSubclassOf<UWebcamComponent> VideoComponentBP);
};
