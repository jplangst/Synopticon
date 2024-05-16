// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/ActorComponent.h"

//Utility
#include "../SystemClasses/SynOpticonState.h"
#include "../SystemClasses/SynOpticonCustomType.h"
#include "../SynOpticonActors/SynOpticonDataStructures.h"

//Actor components
#include "../WAMP/EyeTrackingWAMPComponent.h"
#include "../ActorComponents/RaycastComponent_Ours.h"

#include "EyeVectorsComponent.generated.h"

/**
	The component 
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYNOPTICON_API UEyeVectorsComponent : public USceneComponent
{
	GENERATED_BODY()

	//Owner
	int32 OwnerID;
	bool OwnerIsSelected;

	//Actor components
	UEyeTrackingWAMPComponent* EyeTrackingWAMPComponent;

	//Static mesh components
	UStaticMeshComponent* ViewConeMesh;

	UStaticMeshComponent* GlassesMesh;
	UStaticMeshComponent* LeftEyeArrowMesh;
	UStaticMeshComponent* RightEyeArrowMesh;
	UStaticMeshComponent* CombinedVectorMesh;
	//TODO: remove this after finishing testing
	UStaticMeshComponent* ConvergencedPointMesh;
	USceneComponent* ConvergencedPointComponent;

	//Scene components
	USceneComponent* CombinedVectorSceneComponent;
	USceneComponent* LeftEyeSceneComponent;
	USceneComponent* RightEyeSceneComponent;
	float ScaleDelay;
	float ScaleDelayCounter;
	bool StartScaleDelay;

	//Calibration
	FRotator CalibrationRotatorLeft;
	FRotator CalibrationRotatorRight;
	bool Calibrated;
	bool HasCalibration;

	//Eye data
	FString GlassesID;
	int MAX_BUFFER_LENGTH = 200;
	TArray<float> PupilRadiusBuffer;
	
	//Calibration
	TArray<TArray<double>> AnglesLeft, AnglesRight;
	FQuat PreviousLeftEyeRotator;
	FQuat PreviousRightEyeRotator;

	//Eye data update methods
	bool UpdateEyeVectorWAMP(FWAMPEyeDataStruct* EyeDataSample, USceneComponent* EyeVectorNode, bool Left);
	void UpdatePupilDataWAMP(FWAMPEyeDataStruct* EyeDataSample, float DeltaTime);
	void UpdatePupilDataWAMP(double PupilSample, float DeltaTime);

	bool UpdateReplayEyeVectors(bool& LeftEyeValid, bool& RightEyeValid);
	void UpdateEyeData(float DeltaTime);
	void FindConvergenceVector();

	FEyeTrackerStruct EyeTrackerDataStruct;

	//Replay and recording
	CompressedSOBlock* SOBlock;

	double pupilRadius;
	bool IsDoubleBlink;
	FDateTime DoubleBlinkClock;

	//The position and orientation component that this is attached to
	USceneComponent* OuterPosOriComp;

	//static void ETConnectionChanged(bool IsConnected, void* UserData);
public:	
	void SetOwnerID(int32 _OwnerID);

	void ScaleViewCone(float length, float width);
	void ScaleViewConeUsingAngle(float distance, float angle);
	void ScaleViewConeUsingFontSize(float FontSize);

	void SetOuterPosOriComponent(USceneComponent* OuterPosOriComp);

	UFUNCTION(BlueprintCallable, Category = "EyeTracking Server")
	void SetETDataStruct(FEyeTrackerStruct EyeTrackerDataStruct);

	UFUNCTION(BlueprintCallable, Category = "EyeTracking Server")
	FEyeTrackerStruct GetETData() { return EyeTrackerDataStruct; }

	// Sets default values for this component's properties
	UEyeVectorsComponent();

	void SetSOBlock(CompressedSOBlock* SOBlock);

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// Called when the game end
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void CalibrateEyeVectors(FVector CalPoint, int Num);
	
	void SetEyeTrackingWAMPComponent(UEyeTrackingWAMPComponent* EyeTrackingWAMPComponent);

	UEyeTrackingWAMPComponent* GetEyeTrackingWAMPComponent();

	void UpdateVectorsVisibility();

	UFUNCTION(BlueprintCallable, Category = "Pupil Data")
	TArray<float> GetPupilRadiusBuffer();

	UFUNCTION(BlueprintCallable, Category = "Pupil Data")
	float GetLastPupilRadius();

	void ResetCalibration();

	void SetGlassesID(FString GlassesID);
	FString GetGlassesID();

	USceneComponent* GetConvergenceVectorSceneComponent();

	//Replay
	void ToBinaryReplayData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pupil information")
		float AveragePupilSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pupil information")
		int TotalBlinkCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pupil information")
		float BlinkRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pupil information")
		float AverageBlinkDuration;

	TArray<float> BlinkDurations;
	double PreviousPupilSize;
	bool StartBlinking;
	bool StartReversing;
	bool IsEyeBlink;
	FDateTime BlinkClock;
	FDateTime StartClock;
	FDateTime WindowClock;

	UFUNCTION(BlueprintCallable, Category = "Pupil Data")
		void ResetPupilData();

	UFUNCTION()
		void OnEyeWAMPComponentDisconnected(ComponentTypeEnum ComponentType, FString ComponentID);
	UFUNCTION()
		void OnEyeWAMPComponentConnected(ComponentTypeEnum ComponentType, FString ComponentID);

	//Callback for when the CurrentActor is changed
	UFUNCTION()
	void OnActorChanged(int ActorID, bool HasETG, bool HasSHimmerHR, bool HasShimmerGSR, FString ETEDevice, FString ShimmerDevice);
};
