// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StaticRoomActorDatatypes.h"

#include "GameFramework/Actor.h"
#include "StaticRoomActor.generated.h"

UCLASS()
class SYNOPTICON_API AStaticRoomActor : public AActor
{
	GENERATED_BODY()

	FStaticRoomData StaticRoomData;
	int32 ID;
public:	
	// Sets default values for this actor's properties
	AStaticRoomActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = "GazeActor Factory")
	void CreateStaticRoomActor(FStaticRoomData RoomData);

	void SetID(int32 ID);
	int32 GetID();
};
