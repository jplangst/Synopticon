// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GlobalEventSystem.h"
#include "GameFramework/PlayerController.h"

#include "../Utils/SynOpticonStatics.h"
#include "EyeTrackingGameMode.h"
#include "SynOpticonState.h"

#include "ApplicationController.generated.h"

/**
 * 
 */
UCLASS()
class SYNOPTICON_API AApplicationController : public APlayerController
{
	GENERATED_BODY()
public:

	virtual void BeginPlay() override;
};
