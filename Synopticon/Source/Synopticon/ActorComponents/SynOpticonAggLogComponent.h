// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "ActorComponents/RemoteEyeTrackerComponent.h"
#include "RaycastComponent_Ours.h"
#include "HandComponent.h"
#include "SynOpticonActors/SynOpticonDataStructures.h"
#include "SynOpticonAggLogComponent.generated.h"

UENUM(BlueprintType)
enum class LoggingTypeEnum : uint8
{
	LT_GazeData 			UMETA(DisplayName = "Gaze Data"),
	LT_LeftHandData 		UMETA(DisplayName = "Left Hand Data"),
	LT_RightHandData 		UMETA(DisplayName = "Right Hand Data")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDelegateAddObjectToGazePath, int32, ID, FObjectLoggingInfoStruct, Info, LoggingTypeEnum, _Type);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateResetLoggedData);

struct FLoggingControlStruct
{
	int32 PreviousHitObjectID;
	FDateTime StartLookingTime;
	FDateTime NotLookingStartTime;
	float UnspecifiedPercentage;
	LoggingTypeEnum Type;

	FLoggingControlStruct()
	{
		PreviousHitObjectID = -1;
		UnspecifiedPercentage = 0;
		StartLookingTime = FDateTime::UtcNow();
		NotLookingStartTime = FDateTime::UtcNow();
		Type = LoggingTypeEnum::LT_GazeData;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYNOPTICON_API USynOpticonAggLogComponent : public UActorComponent
{
	GENERATED_BODY()

	FSynOpticonActorLoggingStruct GazeLoggedData;
	FSynOpticonActorLoggingStruct LeftHandLoggedData;
	FSynOpticonActorLoggingStruct RightHandLoggedData;

	URaycastComponent_Ours* RaycastComponent;
	UHandComponent* LeftHandComponent;
	UHandComponent* RightHandComponent;
	URemoteEyeTrackerComponent* RemoteTrackerComponent;

	int32 OwnerID;

	FLoggingControlStruct GazeControlStruct;
	FLoggingControlStruct LeftHandControlStruct;
	FLoggingControlStruct RightHandControlStruct;


	void DoLogging(float DeltaTime, int32 HitTargetID, FLoggingControlStruct& ControlStruct, FSynOpticonActorLoggingStruct& LoggingStruct);

	static double NOISE_THRESHOLD;
	static double DURATION_THRESHOLD;

	void AddGazePath(int32 ObjectName, double Duration, FSynOpticonActorLoggingStruct& LoggingStruct, LoggingTypeEnum Type);

	void AddGazedObject(int32 ObjectID, double Duration, FSynOpticonActorLoggingStruct& LoggingStruct, LoggingTypeEnum Type);

	void IncreaseLastGazeEventDuration(float DeltaTime, FSynOpticonActorLoggingStruct& LoggingStruct);

	void DeleteLastGazePath(FLoggingControlStruct ControlStruct, FSynOpticonActorLoggingStruct& LoggingStruct);

	TArray<int32> ReplayGazeObjectList;
	TArray<int32> ReplayLeftHandObjectList;
	TArray<int32> ReplayRightHandObjectList;

	void RemoveObjectsRelatedToIDHelper(int32 GazeID, FSynOpticonActorLoggingStruct& LoggingStruct);
	//void DoLogging();
public:	
	// Sets default values for this component's properties
	USynOpticonAggLogComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// Called when the game end
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//The raycasting component that will contain the actors hit target. Must be set in order for logging to start.
	void SetRaycastComponent(URaycastComponent_Ours* _RaycastComponent);

	void SetLeftHandComponent(UHandComponent* _LeftHandComponent);

	void SetRightHandComponent(UHandComponent* _RightHandComponent);

	void SetRemoteTrackerComponent(URemoteEyeTrackerComponent* _RetComponent);

	//The name of the owning actor. Must be set in order for logging to start.
	void SetOwnerID(int32 _OwnerID);

	UFUNCTION(BlueprintCallable, Category = "Logged Object data")
		FSynOpticonActorLoggingStruct GetLoggedData(LoggingTypeEnum _Type);
	UFUNCTION(BlueprintCallable, Category = "Logged Object data")
		TArray<FObjectLoggingInfoStruct> GetLoggedObjectDataArray(LoggingTypeEnum _Type);
	UFUNCTION(BlueprintCallable, Category = "Logged Object data")
		TArray<int32> GetLoggedObjectArray(LoggingTypeEnum _Type);

	UFUNCTION(BlueprintCallable, Category = "Logged Object data")
		FObjectLoggingInfoStruct GetLoggedObjectData(int32 ID, LoggingTypeEnum _Type);

	void UpdateActorsTotalTimeAndObjectPercentage(FLoggingControlStruct& ControlStruct, FSynOpticonActorLoggingStruct& LoggingStruct);

	UFUNCTION()
		void ResetLoggedData(bool Start);

	UPROPERTY(BlueprintAssignable, Category = "Logged Object data")
		FDelegateAddObjectToGazePath OnEventAddObjectToGazePath;

	UPROPERTY(BlueprintAssignable, Category = "Logged Object data")
		FDelegateResetLoggedData OnEventResetLoggedData;

	UFUNCTION(BlueprintPure, Category = "Logged Object data")
		float GetUnspecifiedPercentage(LoggingTypeEnum _Type); 

	UFUNCTION(BlueprintPure, Category = "Logged Component")
		bool HasLoggedComponent(LoggingTypeEnum _Type);

	UFUNCTION(Category = "Logged Component")
	void RemoveObjectsRelatedToID(int32 GazeID);

	void SetReplayLogComponentData(int32 _OwnerID, FSynOpticonActorLoggingStruct LogComponentData, LoggingTypeEnum _Type);

	void ReplayLogging();
};
