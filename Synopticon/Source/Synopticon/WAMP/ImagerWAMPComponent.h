// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "../SystemClasses/SynOpticonCustomType.h"
#include "../WAMP/EyeTrackingWAMPComponent.h"
//Synopticon State
#include "../SystemClasses/SynOpticonState.h"
//Worker thread pool and tasks
#include "../SystemClasses/ThreadPoolManager.h"
#include "../SystemClasses/ThreadPoolTasks.h"

#include "ImagerWAMPComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYNOPTICON_API UImagerWAMPComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	//Used to get the latest encoded scene camer image
	UEyeTrackingWAMPComponent* EyeTrackingWAMPComponent;

	//Delegate used to recieve callbacks when new images are ready
	FImageDecodedDelegate ImageDecodedDelegate;

	FImageDecodeImageDelegate DecodeImageDelegate;

	//Worker Task for the worker thread pool
	FAsyncTask<FDecodeImageTask>* DecodeTask;

	//When true we decode and display the scene camera. Otherwise it is hidden and we don't waste resources processing the images.
	bool ShouldDecodeSceneCamera;

	bool FirstTask;
	bool ImageRecieved;
	bool IsDecoding;

	//The glasses id of the current actor. Used to get the scene camera image from the correct actor.
	FString CurrentActorGlassesID;
public:	
	// Sets default values for this component's properties
	UImagerWAMPComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void SetSceneCameraStreaming(bool ShouldDecodeSceneCamera);
	
	//Set the ID of the glasses we want to show the scene camera of.
	void SetGlassesID(FString GlassesID);

	//Do not call directly, used as a callback from the image decoding tasks.
	void OnScenecameraDecoded(UTexture2D* decodedSceneCamera);
};
