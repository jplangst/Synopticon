// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "Components/SceneComponent.h"

#include "Utils/SynOpticonStatics.h"
#include "DrawDebugHelpers.h"

#include "Replay/ReplayDataStructures.h"

//Global state system
#include "NatNet/NatNetWAMPComponent.h"

#include "SystemClasses/SynOpticonState.h"
#include "SystemClasses/SynOpticonCustomType.h"

#include "Kismet/KismetMathLibrary.h"

#include "PositionAndOrientationComponent.generated.h"

class AGazeParentActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYNOPTICON_API UPositionAndOrientationComponent : public USceneComponent
{
	GENERATED_BODY()

	UNatNetWAMPComponent* NatNetWAMPComponent;

	FString OwnerRigidBodyName;
	bool bActorHasETG;
	TArray<FVector> Markers;
	MarkersConfigurationEnum MarkersConfiguration;

	//Calibration
	FRotator CalibrationRotator;
	TArray<TArray<double>> CalibrationAngles;
	bool Calibrated;

	UStaticMeshComponent* ActorOrientationArrowMesh;

	FVector ActorLocationOffset;
	bool ShouldApplyOffset;

	void UpdateWAMPPositionData(float DeltaTime);
	bool ShouldShowOrientationArrowMesh;

	CompressedSOBlock* SOBlock;

	//For use with the SynOpticon Actors
	UStaticMesh* ArrowMesh;
	UMaterialInstanceDynamic* ArrowMaterial;

	//For use with the Gaze Parent Actors
	UStaticMesh* SphereMesh;
	UMaterial* SphereMaterial;

	//Used to know if we should apply the calibration information of this tool
	bool HasBeenCalibratedByCalibrationTool;
	FVector CalibrationToolOffset;
	FQuat CalibrationToolRotation;
	FTransform BeforeCalibrationToolTransform;

	bool ShouldResetToolCalibrationAndCalibrate;
	FTransform StoredCalibrationToolTransform;
	int32 StoredCalibrationMethod;
	int32 StoredCalibrationAxis;
	int32 CalDelay;

	static int32 CompID;
public:	
	// Sets default values for this component's properties
	UPositionAndOrientationComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// Called when the game end
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//Calibration tool overrides and saving
	void SetCalibrationToolCalibration(FQuat Calibration);
	FQuat GetCalibrationToolCalibration();

	UFUNCTION()
	void UpdateVectorVisibility();

	void SetActorHasETG(bool ActorHasETG);

	void SetMarkersConfiguration(MarkersConfigurationEnum _MarkersConfiguration);

	void SetOwnerRigidBodyName(FString OwnerRigidBodyName);

	/** Get the name of the rigid body which used in motive */
	UFUNCTION(BlueprintCallable, Category = "NatNet")
	FString GetRigidBodyName();

	void SetNatNetWAMPComponent(UNatNetWAMPComponent* _NatNetWAMPComponent);

	UNatNetWAMPComponent* GetNatNetWAMPComponent();

	void CalibratePositionVector(FVector CalPoint, int Num);
	
	void ResetCalibration();

	void ResetMarkerIDs();
	UFUNCTION(BlueprintCallable, Category = "Actor Offset")
	void SetActorLocationOffset(FVector Offset);
	UFUNCTION(BlueprintCallable, Category = "Actor Offset")
	void SetShouldApplyOffset(bool ApplyOffset);

	void ScaleOrientationMesh(double ScaleFactor);
	void HideOrientationArrowMesh();

	//============= Record & Replay ===================
	void SetSOBlock(CompressedSOBlock* SOBlock);

	void ToBinaryReplayData();
	void UpdateReplayData();

	//Calibrates the position and orientation component to the calibration tools location and orientation. Returns true if successfull.
	void CalibratePosOriToCalibrationTool(FTransform CalibrationToolTransform, int32 CalibrationMethod, int32 CalibrationAxis); 
};
