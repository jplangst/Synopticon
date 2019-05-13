// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SynOpticonActors/SynOpticonDataStructures.h"
#include "WebcamComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYNOPTICON_API UWebcamComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	FVideoComponentStruct VideoComponentData;

	//The ID of the owning actor
	int32 OwnerID; 

public:	
	// Sets default values for this component's properties
	UWebcamComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	

	void SetVideoComponentStruct(FVideoComponentStruct VideoCompStruct);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SO Video Component")
	FVideoComponentStruct GetVideoComponentStruct() {
		return VideoComponentData;
	}

	void SetOwnerID(int32 OwnerID);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SO Video Component")
		void UpdateVideoFeed();
};
