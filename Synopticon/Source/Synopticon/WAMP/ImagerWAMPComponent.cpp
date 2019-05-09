// Fill out your copyright notice in the Description page of Project Settings.

#include "ImagerWAMPComponent.h"
//#include "Synopticon.h"


// Sets default values for this component's properties
UImagerWAMPComponent::UImagerWAMPComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UImagerWAMPComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	IsDecoding = false;
	ImageRecieved = false;
	FirstTask = true;

	EyeTrackingWAMPComponent = GetOwner()->FindComponentByClass<UEyeTrackingWAMPComponent>();

	ImageDecodedDelegate.BindUObject(this, &UImagerWAMPComponent::OnScenecameraDecoded);
	DecodeTask = new FAsyncTask<FDecodeImageTask>(&ImageDecodedDelegate, &DecodeImageDelegate, EPixelFormat::PF_R5G6B5_UNORM);//EPixelFormat::PF_R5G6B5_UNORM);
}

// Called every frame
void UImagerWAMPComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (ShouldDecodeSceneCamera) {
		if (EyeTrackingWAMPComponent) {
			ImageDataStructures::VP8EncodedImageData* EncodedImage = EyeTrackingWAMPComponent->GetEncodedSceneCameraImage(CurrentActorGlassesID);

			if (EncodedImage && EncodedImage->ImageDataBufferSize > 0) { 
				DecodeImageDelegate.Execute(EncodedImage);

				delete EncodedImage;

				if (FirstTask || DecodeTask->IsDone()) {
					DecodeTask->StartBackgroundTask(ASynOpticonState::GetThreadPoolManager()->GetWorkerPool());
				}

				FirstTask = false;
			}
		}		
	}
}

void UImagerWAMPComponent::EndPlay(const EEndPlayReason::Type EndPlayReason) 
{
	Super::EndPlay(EndPlayReason);

	if (DecodeTask) {
		DecodeTask->EnsureCompletion();
		delete DecodeTask;
	}	

	ImageDecodedDelegate.Unbind();
	DecodeImageDelegate.Unbind();
}

void UImagerWAMPComponent::SetSceneCameraStreaming(bool _ShouldDecodeSceneCamera) {
	ShouldDecodeSceneCamera = _ShouldDecodeSceneCamera;
}

void UImagerWAMPComponent::SetGlassesID(FString GlassesID) {
	CurrentActorGlassesID = GlassesID;
}

void UImagerWAMPComponent::OnScenecameraDecoded(UTexture2D* decodedSceneCamera) {
	ASynOpticonState::GetGlobalEventSystem()->OnEventNewSceneCameraImage.Broadcast(decodedSceneCamera);
}
