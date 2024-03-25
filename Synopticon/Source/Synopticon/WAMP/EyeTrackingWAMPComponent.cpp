// Fill out your copyright notice in the Description page of Project Settings.
//4703
#pragma warning(push)
#pragma warning(disable: 4703)
#include "EyeTrackingWAMPComponent.h"
//#include "Synopticon.h"

const FString EyeDataSample = "EyeDataSample";
const FString ETGVPEncodedSceneCamera = "ETGVPEncodedSceneCamera";
// Sets default values for this component's properties
UEyeTrackingWAMPComponent::UEyeTrackingWAMPComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	ReplayPupilQueue = nullptr;
	// ...
}

// Called when the game starts
void UEyeTrackingWAMPComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentActorGlassesID = "";

	ReplayPupilQueue = new TCircularQueue<TPair<FString, float>>(13);

	CurrentActorSavedEncodedFramesQueue = new TCircularQueue<ImageDataStructures::VP8EncodedImageData*>(13); //Allow N compressed frames to be stored at a given time

	ASynOpticonState::GetGlobalEventSystem()->OnEventStartRecord.AddDynamic(this, &UEyeTrackingWAMPComponent::OnRecordingStatusChanged);
	
	FramesSaved = 0;
}

void UEyeTrackingWAMPComponent::RegisterWAMP()
{
	//TSharedPtr<wamp_event_handler> EyeDataSampleHandler(new wamp_event_handler());
	//*EyeDataSampleHandler = [this](const autobahn::wamp_event& _event) { OnReceiveEyeData(_event); };
	//FWAMPWorker::SubscribeToTopic(EyeDataSample, EyeDataSampleHandler);

	//TSharedPtr<wamp_event_handler> ETGVPEncodedSceneCameraHandler(new wamp_event_handler());
	//*ETGVPEncodedSceneCameraHandler = [this](const autobahn::wamp_event& _event) { OnRecieveEncodedSceneCamera(_event); };
	//FWAMPWorker::SubscribeToTopic(ETGVPEncodedSceneCamera, ETGVPEncodedSceneCameraHandler);
}

void UEyeTrackingWAMPComponent::OnRecordingStatusChanged(bool Started) {
	TArray<FString> EDKeyArray;
	EyeDataQueuesMap.GenerateKeyArray(EDKeyArray);
	for (FString Key : EDKeyArray) {
		TCircularQueue<FWAMPEyeDataStruct*>* EyeDataQueue = *EyeDataQueuesMap.Find(Key);

		while (!EyeDataQueue->IsEmpty()) {
			FWAMPEyeDataStruct* EyeData;
			EyeDataQueue->Dequeue(EyeData);
			delete EyeData;
		}
	}
	EyeDataQueuesMap.Empty();

	TArray<FString> SCKeyArray;
	EncodedSceneCameraQueuesMap.GenerateKeyArray(SCKeyArray);
	for (FString Key : SCKeyArray) {
		TCircularQueue<ImageDataStructures::VP8EncodedImageData*>* SceneCameraQueue = *EncodedSceneCameraQueuesMap.Find(Key);

		while (!SceneCameraQueue->IsEmpty()) {
			ImageDataStructures::VP8EncodedImageData* ImageData;
			SceneCameraQueue->Dequeue(ImageData);
			delete ImageData;
		}
	}

	while (!CurrentActorSavedEncodedFramesQueue->IsEmpty()) {
		ImageDataStructures::VP8EncodedImageData* ImageData;
		CurrentActorSavedEncodedFramesQueue->Dequeue(ImageData);
		delete ImageData;
	}
}

void UEyeTrackingWAMPComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	TArray<FString> EyeDataMapKeys;
	EyeDataQueuesMap.GetKeys(EyeDataMapKeys);
	for (FString Key : EyeDataMapKeys) {
		TCircularQueue<FWAMPEyeDataStruct*>* EyeDataQueue = *EyeDataQueuesMap.Find(Key);
		if (EyeDataQueue) {
			while (!EyeDataQueue->IsEmpty()) {
				FWAMPEyeDataStruct* Sample;
				EyeDataQueue->Dequeue(Sample);
				delete Sample;
			}
			delete EyeDataQueue;
		}
	}
	EyeDataQueuesMap.Empty();

	TArray<FString> SceneCameraMapKeys;
	EncodedSceneCameraQueuesMap.GetKeys(SceneCameraMapKeys);
	for (FString Key : SceneCameraMapKeys) {
		TCircularQueue<ImageDataStructures::VP8EncodedImageData*>* SceneCameraQueue = *EncodedSceneCameraQueuesMap.Find(Key);
		if (SceneCameraQueue) {
			while (!SceneCameraQueue->IsEmpty()) {
				ImageDataStructures::VP8EncodedImageData* Image;
				SceneCameraQueue->Dequeue(Image);
				delete Image;
			}
			delete SceneCameraQueue;
		}
	}
	EncodedSceneCameraQueuesMap.Empty();

	//Used during recording to make sure that all incoming frames are saved before they are consumed by the Imager Component
	while (!CurrentActorSavedEncodedFramesQueue->IsEmpty()) {
		ImageDataStructures::VP8EncodedImageData* ImageData;
		CurrentActorSavedEncodedFramesQueue->Dequeue(ImageData);
		delete ImageData;
	}
	delete CurrentActorSavedEncodedFramesQueue;

	ReplayPupilQueue->Empty();
	delete ReplayPupilQueue;
	ReplayPupilQueue = nullptr;
}

FWAMPEyeDataStruct* UEyeTrackingWAMPComponent::GetEyeDataSample(FString GlassesID) {
	FWAMPEyeDataStruct* EyeData = nullptr;

	TCircularQueue<FWAMPEyeDataStruct*>** EyeDataQueue = EyeDataQueuesMap.Find(GlassesID);
	if (EyeDataQueue)
	{
		if (!(*EyeDataQueue)->IsEmpty()) {
			(*EyeDataQueue)->Dequeue(EyeData);
		}		
	}
	
	return EyeData;
}

ImageDataStructures::VP8EncodedImageData* UEyeTrackingWAMPComponent::GetEncodedSceneCameraImage(FString GlassesID) {
	if (GlassesID.Equals("")) {
		return nullptr;
	}
	
	ImageDataStructures::VP8EncodedImageData* SceneCameraImage = nullptr;

	//If we are recording we change the queue that we pull data from
	if (ASynOpticonState::IsRecording())
	{
		if (!CurrentActorSavedEncodedFramesQueue->IsEmpty()) {
			CurrentActorSavedEncodedFramesQueue->Dequeue(SceneCameraImage);
		}
	}
	else //Otherwise we use the map as usual
	{
		TCircularQueue<ImageDataStructures::VP8EncodedImageData*>** SceneCameraQueue = EncodedSceneCameraQueuesMap.Find(GlassesID);
		if (SceneCameraQueue)
		{
			if (!(*SceneCameraQueue)->IsEmpty()) {
				(*SceneCameraQueue)->Dequeue(SceneCameraImage);
			}
		}
	}

	return SceneCameraImage;
}

void UEyeTrackingWAMPComponent::SaveEncodedSceneCameraFrame(FString GlassesID, CompressedSOBlock* SOBlock) {
	TCircularQueue<ImageDataStructures::VP8EncodedImageData*>** SceneCameraQueue = EncodedSceneCameraQueuesMap.Find(GlassesID);
	if (SceneCameraQueue)
	{	
		if (!(*SceneCameraQueue)->IsEmpty()) 
		{
			ImageDataStructures::VP8EncodedImageData* SceneCameraImage = nullptr;
			(*SceneCameraQueue)->Dequeue(SceneCameraImage);

			if (SceneCameraImage) {
				//Save the encoded frame 			
				SOBlock->ImageDataBufferSize = SceneCameraImage->ImageDataBufferSize;
				FMemory::Memcpy(SOBlock->SceneCameraImageBuffer, SceneCameraImage->ImageBuffer, SceneCameraImage->ImageDataBufferSize);
				SOBlock->IsKeyFrame = SceneCameraImage->IsKeyFrame;
				SOBlock->ImageWidth = SceneCameraImage->ImageWidth;
				SOBlock->ImageHeight = SceneCameraImage->ImageHeight;

				FramesSaved++;

				//If the image belongs to the current actor we keep it and add it to the current actor queue for decoding.
				if (!GlassesID.Equals(CurrentActorGlassesID))
				{
					delete SceneCameraImage;
				}
				else //Otherwise we free the memory used by the scene camera image
				{
					CurrentActorSavedEncodedFramesQueue->Enqueue(SceneCameraImage);
				}
			}		
		}
	}
}

void UEyeTrackingWAMPComponent::AddEncodedSceneCameraImage(FString GlassesID, ImageDataStructures::VP8EncodedImageData* SceneCameraImage) {
	if (!EncodedSceneCameraQueuesMap.Contains(GlassesID))
	{
		TCircularQueue<ImageDataStructures::VP8EncodedImageData*>* SceneCameraQueue = new TCircularQueue<ImageDataStructures::VP8EncodedImageData*>(13); //Allow N compressed frames to be stored at a given time
		SceneCameraQueue->Enqueue(SceneCameraImage);
		EncodedSceneCameraQueuesMap.Add(GlassesID, SceneCameraQueue);
	}
	else {
		TCircularQueue<ImageDataStructures::VP8EncodedImageData*>** SceneCameraQueue = EncodedSceneCameraQueuesMap.Find(GlassesID);
		if (SceneCameraQueue)
		{
			bool Success = (*SceneCameraQueue)->Enqueue(SceneCameraImage);

			//If the queue was full we need to free the memory we used for the sample
			if (!Success) {
				delete SceneCameraImage;
			}
		}
	}
}


void UEyeTrackingWAMPComponent::SetCurrentActorGlassesID(FString GlassesID) {
	CurrentActorGlassesID = GlassesID;
}

void UEyeTrackingWAMPComponent::PublishReplayData(TPair<FString, float> Sample)
{
	ReplayPupilQueue->Enqueue(Sample);
}

TPair<FString, float> UEyeTrackingWAMPComponent::GetReplaySample()
{
	TPair<FString, float> Sample;
	Sample.Key = "";
	if (ASynOpticonState::IsReplaying() && ReplayPupilQueue && !ReplayPupilQueue->IsEmpty()) {
		ReplayPupilQueue->Dequeue(Sample);
	}
	return Sample;
}

void UEyeTrackingWAMPComponent::OnReceiveEyeData(const string _event)
{
	if (ASynOpticonState::IsReplaying()) {
		return;
	}

	FString GlassesID = "Not Set"; //FString(_event.argument<std::string>(0).c_str());
	if (!Glasses.Contains(GlassesID))
	{
		ASynOpticonState::AddComponentToMap(ComponentTypeEnum::VE_Glasses, GlassesID);
		Glasses.Add(GlassesID);
	}

	FWAMPEyeDataStruct* EyeData = new FWAMPEyeDataStruct();//new FWAMPEyeDataStruct(_event.argument<std::array<double, 16>>(1));

	if (!EyeDataQueuesMap.Contains(GlassesID)) {
		TCircularQueue<FWAMPEyeDataStruct*>* DataQueue = new TCircularQueue<FWAMPEyeDataStruct*>(121); //Allow one second worth of data to be stored at a time
		DataQueue->Enqueue(EyeData);
		EyeDataQueuesMap.Add(GlassesID, DataQueue);
	}
	else {
		TCircularQueue<FWAMPEyeDataStruct*>** EyeDataQueue = EyeDataQueuesMap.Find(GlassesID);
		if (*EyeDataQueue)
		{
			bool Success = (*EyeDataQueue)->Enqueue(EyeData);

			//If the queue was full we need to free the memory we used for the sample
			if (!Success) {
				delete EyeData;
			}
		}
	}
}

void UEyeTrackingWAMPComponent::OnRecieveEncodedSceneCamera(const string _event)
{
	if (ASynOpticonState::IsReplaying()) {
		return;
	}

	FString GlassesID = "Not Set"; // FString(_event.argument<std::string>(0).c_str());
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Red, GlassesID);
	//}

	//std::vector<uint8_t> ImageData = _event.argument<std::vector<uint8_t>>(1);
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Black, FString::FromInt(ImageData.size()));
	//}

	//int32 IsKeyFrame = _event.argument<int32>(2);
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, FString::FromInt(IsKeyFrame));
	//}

	//int32 ImageWidth = _event.argument<int32>(3);
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Yellow, FString::FromInt(ImageWidth));
	//}

	//int32 ImageHeight = _event.argument<int32>(4);
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Cyan, FString::FromInt(ImageHeight));
	//	GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Black, "");
	//}

	//ImageDataStructures::VP8EncodedImageData* EncodedCameraImage = new ImageDataStructures::VP8EncodedImageData();
	//EncodedCameraImage->ImageDataBufferSize = ImageData.size();
	//memcpy(EncodedCameraImage->ImageBuffer, ImageData.data(), ImageData.size());
	//EncodedCameraImage->IsKeyFrame = IsKeyFrame;
	//EncodedCameraImage->ImageWidth = ImageWidth;
	//EncodedCameraImage->ImageHeight = ImageHeight;

	//AddEncodedSceneCameraImage(GlassesID, EncodedCameraImage);
}

#pragma warning(pop)