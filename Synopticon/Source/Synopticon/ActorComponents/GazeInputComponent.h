// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Pawn.h"
#include "Components/ActorComponent.h"

#include "../SystemClasses/GazeEventDataStructures.h"
#include "RaycastComponent_Ours.h"

#include "GazeInputComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYNOPTICON_API UGazeInputComponent : public UActorComponent
{
	GENERATED_BODY()
			
public:	
	// Sets default values for this component's properties
	UGazeInputComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void ProcessGesture(GazeGestureEnum gazeGesture);
};
