// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "SynOpticonPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SYNOPTICON_API ASynOpticonPlayerController : public APlayerController
{
	GENERATED_BODY()

private: 
	AActor* Owner;

public: 
	void SetOwner(AActor* a) 
	{ 
		Owner = a; 
	}

	UWorld* GetWorld() const 
	{
		if(Owner)
			return Owner->GetWorld();
		return nullptr;
	}
};
