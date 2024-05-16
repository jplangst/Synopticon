// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/UObjectIterator.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "ParticleDefinitions.h"
#include "Components/ActorComponent.h"

#include "../GazeObjects/GazeActor.h"
#include "../SystemClasses/SynOpticonState.h"
#include "../Utils/SynOpticonStatics.h"
#include "../WAMP/RemoteTrackerWAMPComponent.h"
#include "../Replay/ReplayDataStructures.h"
#include "../SynOpticonActors/SynOpticonDataStructures.h"

#include "RemoteEyeTrackerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYNOPTICON_API URemoteEyeTrackerComponent : public UActorComponent
{
	GENERATED_BODY()

	//Owner
	int32 OwnerID;
	bool OwnerIsSelected;

	AGazeActor* AttachedScreen;

	FRemoteEyeTrackerStruct RemoteEyeTrackerStruct;

	//A XY coordinate on the screen the remote tracker is attached to.
	FVector2D GazePoint;
	FVector2D ScreenSize;
	FVector WorldPoint; 

	//Smoothing 
	FQuat PrevRightEyeRotation;
	FQuat PrevLeftEyeRotation;
	FVector PrevRightEyeLocation;
	FVector PrevLeftEyeLocation;

	//Scene components
	USceneComponent* RemoteTrackerRootSceneComponent;
	USceneComponent* LeftEyeSceneComponent;
	USceneComponent* RightEyeSceneComponent;

	//Replay and recording
	bool IsReplayEnabled;
	CompressedSOBlock* SOBlock;

	//To recieve data from the remote tracker server
	URemoteTrackerWAMPComponent* RetWAMPComp;

	//Used for logging. -1 when no object
	int32 HitTargetID;

	//Current Pupil Data - For saving in Binary when recording
	float LeftEyePupilRadius;
	float RightEyePupilRadius;

	float GetAveragedPupilRadius(float LeftEye, float RightEye);

public:	
	void SetOwnerID(int32 _OwnerID);

	//The appearance of the remote tracker and the eye data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Remote Tracker | Mesh")
		UStaticMeshComponent* RemoteTrackerMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Remote Tracker | Mesh")
		UStaticMeshComponent* LeftEyeMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Remote Tracker | Mesh")
		UStaticMeshComponent* RightEyeMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Remote Tracker | Mesh")
		UStaticMeshComponent* LeftEyeArrowMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Remote Tracker | Mesh")
		UStaticMeshComponent* RightEyeArrowMesh;

	//The Remote tracker's position and angle 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Remote Tracker | Configuration")
		FVector TrackerPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Remote Tracker | Configuration")
		FVector PointToLookAt; 

	UFUNCTION(BlueprintCallable, Category = "Remote Tracker | Configuration")
		void SetRETDataStruct(FRemoteEyeTrackerStruct RemoteEyeTrackerStruct);

	UFUNCTION(BlueprintCallable, Category = "Remote Tracker | Configuration")
		void SetAttachedScreen(AGazeActor* AttachedScreen);
	UFUNCTION(BlueprintCallable, Category = "Remote Tracker | Configuration")
		AGazeActor* GetAttachedScreen();

	UFUNCTION(BlueprintCallable, Category = "Remote Tracker | Configuration")
		FRemoteEyeTrackerStruct GetRETData() { return RemoteEyeTrackerStruct; }

	FVector GetWorldGazePoint();

	USceneComponent* GetRootNode() { return RemoteTrackerRootSceneComponent; }

	// Sets default values for this component's properties
	URemoteEyeTrackerComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called when the game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	void SetRetWAMPComponent(URemoteTrackerWAMPComponent* _RetWAMPComponent);
	URemoteTrackerWAMPComponent* GetRetWAMPComponent();

	//Record and Replay 
	void SetSOBlock(CompressedSOBlock* SOBlock);
	void UpdateVectorsVisibility();
	void UpdateEyeData(float DeltaTime);
	void UpdateEyeLocations(FWAMPRetDataStruct* RetData);
	void UpdateGazeLocation(FWAMPRetDataStruct* RetData);
	void UpdatePupilData(float AveragedPupilRadius, float DeltaTime);

	//Eye data update methods
	//void UpdatePupilDataWAMP(FWAMPRetDataStruct* EyeDataSample, float DeltaTime);

	int32 GetHitTargetID();
	int32 GetHitTargetID(FString &OutSessionId, bool &OutbPopup);

	bool UpdateReplayEyeVectors();
	void UpdateReplayGazeLocation();
	void ToBinaryReplayData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pupil information")
		float AveragePupilSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pupil information")
		int TotalBlinkCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pupil information")
		float BlinkRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pupil information")
		float AverageBlinkDuration;

	//Eye data
	UFUNCTION(BlueprintCallable, Category = "Pupil Data")
		TArray<float> GetPupilRadiusBuffer();
	UFUNCTION(BlueprintCallable, Category = "Pupil Data")
		float GetLastPupilRadius();

	int MAX_BUFFER_LENGTH = 200;
	TArray<float> PupilRadiusBuffer;
	TArray<float> BlinkDurations;
	double PreviousPupilSize;
	bool StartBlinking;
	bool StartReversing;
	bool IsEyeBlink;
	FDateTime BlinkClock;
	FDateTime StartClock;
	FDateTime WindowClock;

	//Callback for when the CurrentActor is changed
	UFUNCTION()
	void OnActorChanged(int ActorID, bool HasETG, bool HasSHimmerHR, bool HasShimmerGSR, FString ETEDevice, FString ShimmerDevice);
};
