// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../NatNet/NatNetWAMPComponent.h"
#include "../ActorComponents/PositionAndOrientationComponent.h"
#include "../GazeObjects/GazeActor.h"

#include "GazeParentActor.generated.h"

UCLASS()
class SYNOPTICON_API AGazeParentActor : public AActor
{
	GENERATED_BODY()
	
	UPositionAndOrientationComponent* PosOriComp;
	FString NatNetName;
	FString SceneCompName;

	bool InitialLocationSaved;
	FVector InitialLocation;

	TArray<UObject*> AttachedObjects;
public:	
	// Sets default values for this actor's properties
	AGazeParentActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void SetCalibrationQuat(FQuat CalibrationQuat);

	UFUNCTION(BlueprintCallable, Category = "Gaze Actor Parenting")
	void CreatePosOriComponent(FString NatNetName);

	UFUNCTION(BlueprintCallable, Category = "Gaze Actor Parenting")
	void RegisterPosOriComponent();

	UFUNCTION(BlueprintCallable, Category = "Gaze Actor Parenting")
	FString GetNatNetName() { return NatNetName; }

	UFUNCTION(BlueprintCallable, Category = "Gaze Actor Parenting")
	void SetSceneCompName(FString SceneCompName);

	UFUNCTION(BlueprintCallable, Category = "Gaze Actor Parenting")
	void SetInitialLocation(FVector InitialLocation);

	UFUNCTION(BlueprintCallable, Category = "Gaze Actor Parenting")
	FVector GetInitialLocation() { return InitialLocation; }

	UFUNCTION(BlueprintCallable, Category = "Gaze Actor Parenting")
	FString GetSceneCompName() { return SceneCompName; }

	UFUNCTION(BlueprintCallable, Category = "Gaze Actor Parenting")
	void AddAttachedObject(UObject* AttachedObject);

	UFUNCTION(BlueprintCallable, Category = "Gaze Actor Parenting")
	void RemoveAttachedObject(UObject* AttachedObject);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
