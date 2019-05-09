// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "ActorComponents/PositionAndOrientationComponent.h"
#include "SystemClasses/SynOpticonState.h"

#include "CalibrationActor.generated.h"

UCLASS()
class SYNOPTICON_API ACalibrationActor : public AActor
{
	GENERATED_BODY()
	UPositionAndOrientationComponent* PosOriComponent;

	static float CalibrationThreshold;

	//Search all the actors in the list and find the one with the closest UPositionAndOrientationComponent. 
	//Returns the distance and the component of the closest actor. 
	//Returns false if no component found or no component was within the distance threshold
	void FindNearestPositionAndOrientationActorComponentInList(TArray<AActor*> ActorList, float &OutDistance, UPositionAndOrientationComponent* &OutComponent);

public:	
	// Sets default values for this actor's properties
	ACalibrationActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void SetPositionAndOrientationComponent(UPositionAndOrientationComponent* NewPosOriComponent);
	
	UFUNCTION(BlueprintCallable, Category="CalibrationTool")
	bool CalibrateNearestObject(int32 CalibrationMethod, int32 CalibrationAxis);
};
