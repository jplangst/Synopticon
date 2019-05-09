// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "GameFramework/Pawn.h"
#include "GazeObjects/GazeActor.h"

#include "SynOpticonPlayerController.h"
#include "SynOpticonDataStructures.h"

//Actor components
#include "ActorComponents/PositionAndOrientationComponent.h"
#include "ActorComponents/EyeVectorsComponent.h"
#include "ActorComponents/RemoteEyeTrackerComponent.h"
#include "ActorComponents/SynOpticonAggLogComponent.h"
#include "ActorComponents/RaycastComponent_Ours.h"
#include "ActorComponents/GazeDataVisualizerComponent.h"
#include "ActorComponents/ShimmerVRPNComponent.h"
#include "ActorComponents/HandComponent.h"
#include "ActorComponents/OpenFaceComponent.h"

//NatNet

#include "SynOpticonActor.generated.h"

class UFixationGroup
{
public:
	int32 GazeActorId;
	TArray<FFixationPoint> Points;
		
	FVector GetCentroid();

	FDateTime TimeOfFirstPoint;

	float GetDuration();

	UFixationGroup(int32 ID)
	{
		GazeActorId = ID;
		//TimeOfFirstPoint = ASynOpticonState::GetCurrentReplayTime();
		TimeOfFirstPoint = FDateTime::UtcNow();
	}

	UFixationGroup(int32 ID, FDateTime Timestamp)
	{
		GazeActorId = ID;
		TimeOfFirstPoint = Timestamp;
	}
};

UCLASS()
class SYNOPTICON_API ASynOpticonActor : public APawn
{
	GENERATED_BODY()

	FString ActorName;
	bool CanRaycast;
	int32 ID;

	TMap<int32, float> GazeActorHeatMapValues;

	FHitResult PreviousHitResult;
	FVector PreviousGazePoint;
	UFixationGroup* CurrentFixationGroup;
	int32 PreviousHitFaceIndex;

	//Record and replay
	CompressedSOBlock* SOBlock;

	//Gaze data visualizer, tmp for testing
	UGazeDataVisualizerComponent* GazeDataVis;

	//Actor camera
	UCameraComponent* OrientationCamera;

	TArray<FActorTaskStruct> ActorTasks;

public:	
	// Sets default values for this actor's properties
	ASynOpticonActor();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		USpringArmComponent* CameraArm;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called when the game end
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void SetGazeDataVisualizer(UGazeDataVisualizerComponent* GazeDataVis);

	//Set the name of the SynOpticonActor
	void SetActorName(FString ActorName);
	//Get the name of the SynOpticonActor
	UFUNCTION(BlueprintCallable, Category = "SynOpticon Actor Properties")
	FString GetActorName(){ return ActorName; }

	//Used to update the current state of the tasks assigned to the actor
	void UpdateActorTasks();

	//Get the name of the SynOpticonActor
	UFUNCTION(BlueprintCallable, Category = "SynOpticon Actor Properties")
		int32 GetActorID() { return ID; }

	//Get the task list of the current actor
	UFUNCTION(BlueprintCallable, Category = "SynOpticon Actor Tasks")
		TArray<FActorTaskStruct> GetActorTaskList() { return ActorTasks; }

	//Set the name of the SynOpticonActor
	void SetActorID(int32 _ID) { ID = _ID; }

	UFUNCTION(BlueprintCallable, Category = "SynOpticon Calibration")
		void ResetCalibration();

	UFUNCTION(BlueprintCallable, Category = "SynOpticon Calibration")
		void ResetPositionAndOrientation();

	void CalibrateToPoint(FVector CalPoint, int CalNum);

	ASynOpticonPlayerController* PlayerController;

	void SetCanRaycast(bool _CanRaycast);

	TMap<int32, float> GetHeatValues() { return GazeActorHeatMapValues; }

	//============= Record & Replay ===================
	void SetCompressedSOBlock(CompressedSOBlock* SOBlock);

	void RecordFrameData();

	//Sets the camera arm to a child of the provided USceneComponent, if it is nullptr the root component of the actor is used instead
	void SetupCameraAttachement(USceneComponent* CameraParent);

	void CalculateFixations(float DeltaTime);
	void HandleEyeRadarDataFromUV(AGazeActor* GazeActor, FVector2D UV, float Duration);
	FVector2D HandleEyeRadarData(AGazeActor* GazeActor, FVector WorldLocation, int32 FaceIndex, float Duration);
	void CalculateFixationsFromEyeVector(float DeltaTime);
	void CalculateFixationsRegardless(float DeltaTime);
	void CalculateReplayFixation(FDateTime CurrentTime);

	FSynOpticonActorStruct GetDataStruct();
};
