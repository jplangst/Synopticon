// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GazeObjects/GazeActor.h"
#include "Replay/ReplayDataStructures.h"
#include "WAMP/OpenFaceWAMPComponent.h"
#include "ActorComponents/RaycastComponent_Ours.h"
#include "SystemClasses/SynOpticonCustomType.h"
#include "OpenFaceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYNOPTICON_API UOpenFaceComponent : public USceneComponent
{
	GENERATED_BODY()

private:
	//Owner
	int32 OwnerID;
	bool OwnerIsSelected;

	AGazeActor* AttachedScreen;

	//Smoothing 
	FQuat PreviousRightEyeRotator;
	FQuat PreviousLeftEyeRotator;
	FVector PrevRightEyeLocation;
	FVector PrevLeftEyeLocation;

	//Calibration
	FRotator CalibrationRotatorLeft;
	FRotator CalibrationRotatorRight;
	FRotator CalibrationRotator;
	bool Calibrated;
	bool HasCalibration;

	//Scene components
	USceneComponent* OpenFaceRootSceneComponent;
	USceneComponent* OrientationSceneComponent;
	USceneComponent* LeftEyeSceneComponent;
	USceneComponent* RightEyeSceneComponent;
	USceneComponent* CombinedVectorSceneComponent;

	float ScaleDelay;
	float ScaleDelayCounter;
	bool StartScaleDelay;

	//Replay and recording
	bool IsReplayEnabled;
	CompressedSOBlock* SOBlock;

	//Used for logging. -1 when no object
	int32 HitTargetID;

	//Current Pupil Data - For saving in Binary when recording
	float LeftEyePupilRadius;
	float RightEyePupilRadius;

	float GetAveragedPupilRadius(float LeftEye, float RightEye);

	UOpenFaceWAMPComponent* OpenFaceWAMPComponent;

	FOpenFaceStruct OpenFaceStruct;
	FMatrix OpenFaceTransformation;

public:	
	// Sets default values for this component's properties
	UOpenFaceComponent();

	//The appearance of the remote tracker and the eye data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OpenFace Tracker | Mesh")
		UStaticMeshComponent* BustMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OpenFace Tracker | Mesh")
		UStaticMeshComponent* LeftEyeMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OpenFace Tracker | Mesh")
		UStaticMeshComponent* RightEyeMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OpenFace Tracker | Mesh")
		UStaticMeshComponent* LeftEyeArrowMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OpenFace Tracker | Mesh")
		UStaticMeshComponent* RightEyeArrowMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OpenFace Tracker | Mesh")
		UStaticMeshComponent* OrientationArrowMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OpenFace Tracker | Mesh")
		UStaticMeshComponent* CombinedVectorMesh;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void UpdateVectorsVisibility();
	void UpdateOpenFaceData(float DeltaTime);
	void UpdatePositionData(float DeltaTime, FRigidBodyDataStruct Orientation);
	void UpdateGazeData(FWAMPEyeDataStruct EyeDataSample);
	bool UpdateEyeVector(FWAMPEyeDataStruct EyeDataSample, USceneComponent* EyeVectorNode, bool Left);
	void FindConvergenceVector();
	void DoRaycast();
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetRetWAMPComponent(UOpenFaceWAMPComponent* _OpenFaceWAMPComponent);
	UOpenFaceWAMPComponent* GetOpenFaceWAMPComponent();
	void SetOwnerID(int32 _OwnerID) { OwnerID = _OwnerID; }
	void SetSOBlock(CompressedSOBlock* SOBlock);
	//Replay
	void ToBinaryReplayData();

	USceneComponent* GetRaycastSceneComponent();

	UFUNCTION(BlueprintCallable, Category = "OpenFace Tracker | Configuration")
		void SetOpenFaceWAMPComponent(UOpenFaceWAMPComponent* _OpenFaceWAMPComponent) { OpenFaceWAMPComponent = _OpenFaceWAMPComponent; }

	UFUNCTION(BlueprintCallable, Category = "OpenFace Tracker | Configuration")
		void SetOpenFaceStruct(FOpenFaceStruct _OpenFaceStruct);

	UFUNCTION(BlueprintCallable, Category = "OpenFace Tracker | Configuration")
		FOpenFaceStruct GetOpenFaceStructData() { return OpenFaceStruct; }
	
	UFUNCTION()
		void OnActorChanged(int ActorID, bool HasETG, bool HasSHimmerHR, bool HasShimmerGSR, FString ETEDevice, FString ShimmerDevice);

};
