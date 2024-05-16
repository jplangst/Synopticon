// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/CircularQueue.h"
#include "Components/ActorComponent.h"

#include "../SystemClasses/SynOpticonState.h"
#include "../SystemClasses/SynOpticonCustomType.h"
//#include "../WAMPWorker.h"
#include "../Replay/ReplayDataStructures.h"
#include "../ImageProcessing/ImageDataStructures.h"

#include "EyeTrackingWAMPComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYNOPTICON_API UEyeTrackingWAMPComponent : public UActorComponent
{
	GENERATED_BODY()

	TMap<FString, TCircularQueue<FWAMPEyeDataStruct*>*> EyeDataQueuesMap;
	TMap<FString, TCircularQueue<ImageDataStructures::VP8EncodedImageData*>*> EncodedSceneCameraQueuesMap;

	TCircularQueue<TPair<FString, float>>* ReplayPupilQueue;

	//Used during recording to make sure that all incoming frames are saved before they are consumed by the Imager Component
	TCircularQueue<ImageDataStructures::VP8EncodedImageData*>* CurrentActorSavedEncodedFramesQueue;

	FString CurrentActorGlassesID;

	int32 FramesSaved;
	TArray<FString> Glasses;
public:	
	// Sets default values for this component's properties
	UEyeTrackingWAMPComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FWAMPEyeDataStruct* GetEyeDataSample(FString GlassesID);
	void AddEncodedSceneCameraImage(FString GlassesID, ImageDataStructures::VP8EncodedImageData* SceneCameraImage);

	ImageDataStructures::VP8EncodedImageData* GetEncodedSceneCameraImage(FString GlassesID);

	//Should only be called from the SynOpticon actor during the RecordFrame function
	void SaveEncodedSceneCameraFrame(FString GlassesID, CompressedSOBlock* SOBlock);

	void SetCurrentActorGlassesID(FString GlassesID);

	UFUNCTION()
		void OnRecordingStatusChanged(bool Started);

	void PublishReplayData(TPair<FString, float> Sample);
	TPair<FString, float> GetReplaySample();

	void OnReceiveEyeData(const string _event);
	void OnRecieveEncodedSceneCamera(const string _event);

	void RegisterWAMP();
};