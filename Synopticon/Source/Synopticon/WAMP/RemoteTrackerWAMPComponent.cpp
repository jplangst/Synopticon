// Fill out your copyright notice in the Description page of Project Settings.
#pragma warning(push)
#pragma warning(disable: 4703)
#include "RemoteTrackerWAMPComponent.h"

const FString RETDataSample = "RETDataSample";
// Sets default values for this component's properties

URemoteTrackerWAMPComponent::URemoteTrackerWAMPComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	ReplayPupilQueue = nullptr;
}

// Called when the game starts
void URemoteTrackerWAMPComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CurrentActorRetID = "";

	ReplayPupilQueue = new TCircularQueue<TPair<FString, float>>(13);

	ASynOpticonState::GetGlobalEventSystem()->OnEventStartRecord.AddDynamic(this, &URemoteTrackerWAMPComponent::OnRecordingStatusChanged);
}

void URemoteTrackerWAMPComponent::RegisterWAMP()
{
	//TSharedPtr<wamp_event_handler> RETDataSampleHandler(new wamp_event_handler());
	//*RETDataSampleHandler = [this](const autobahn::wamp_event& _event) { OnReceiveEyeData(_event); };
	//FWAMPWorker::SubscribeToTopic(RETDataSample, RETDataSampleHandler);
}

// Called every frame
void URemoteTrackerWAMPComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URemoteTrackerWAMPComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//Clean up the stored eye data
	TArray<FString> EyeDataMapKeys;
	EyeDataQueuesMap.GetKeys(EyeDataMapKeys);
	for (FString Key : EyeDataMapKeys) {
		TCircularQueue<FWAMPRetDataStruct*>* EyeDataQueue = *EyeDataQueuesMap.Find(Key);
		if (EyeDataQueue) {
			while (!EyeDataQueue->IsEmpty()) {
				FWAMPRetDataStruct* Sample;
				EyeDataQueue->Dequeue(Sample);
				delete Sample;
			}
			delete EyeDataQueue;
		}
	}
	EyeDataQueuesMap.Empty();

	//Clean up the pupil data
	ReplayPupilQueue->Empty();
	delete ReplayPupilQueue;
	ReplayPupilQueue = nullptr;
}

//Clear out the stored eye data whenever our recording status changes
void URemoteTrackerWAMPComponent::OnRecordingStatusChanged(bool Started) {
	TArray<FString> EDKeyArray;
	EyeDataQueuesMap.GenerateKeyArray(EDKeyArray);
	for (FString Key : EDKeyArray) {
		TCircularQueue<FWAMPRetDataStruct*>* EyeDataQueue = *EyeDataQueuesMap.Find(Key);

		while (!EyeDataQueue->IsEmpty()) {
			FWAMPRetDataStruct* EyeData;
			EyeDataQueue->Dequeue(EyeData);
			delete EyeData;
		}
	}
	EyeDataQueuesMap.Empty();
}

//Add a new data sample to the specified RemoteTrackerID
//void URemoteTrackerWAMPComponent::AddEyeDataSample(FString RetID, FWAMPRetDataStruct* EyeData) {
//	if (!EyeDataQueuesMap.Contains(RetID)) {
//		TCircularQueue<FWAMPRetDataStruct*>* DataQueue = new TCircularQueue<FWAMPRetDataStruct*>(121); //Allow one second worth of data to be stored at a time
//		DataQueue->Enqueue(EyeData);
//		EyeDataQueuesMap.Add(RetID, DataQueue);
//	}
//	else {
//		TCircularQueue<FWAMPRetDataStruct*>** EyeDataQueue = EyeDataQueuesMap.Find(RetID);
//		if (*EyeDataQueue)
//		{
//			bool Success = (*EyeDataQueue)->Enqueue(EyeData);
//
//			//If the queue was full we need to free the memory we used for the sample
//			if (!Success) {
//				delete EyeData;
//			}
//		}
//	}
//}

//Get the next data sample from the RetID queue
FWAMPRetDataStruct* URemoteTrackerWAMPComponent::GetEyeDataSample(FString RetID) {
	FWAMPRetDataStruct* EyeData = nullptr;

	TCircularQueue<FWAMPRetDataStruct*>** EyeDataQueue = EyeDataQueuesMap.Find(RetID);
	if (EyeDataQueue)
	{
		if (!(*EyeDataQueue)->IsEmpty()) {
			(*EyeDataQueue)->Dequeue(EyeData);
		}
	}

	return EyeData;
}

void URemoteTrackerWAMPComponent::SetCurrentActorRetID(FString RetID) {
	CurrentActorRetID = RetID;
}

void URemoteTrackerWAMPComponent::PublishReplayData(TPair<FString, float> Sample)
{
	ReplayPupilQueue->Enqueue(Sample);
}

//Get the next pupil data sample for replay
TPair<FString, float> URemoteTrackerWAMPComponent::GetReplaySample()
{
	TPair<FString, float> Sample;
	Sample.Key = "";
	if (ASynOpticonState::IsReplaying() && ReplayPupilQueue && !ReplayPupilQueue->IsEmpty()) {
		ReplayPupilQueue->Dequeue(Sample);
	}
	return Sample;
}

void URemoteTrackerWAMPComponent::OnReceiveEyeData(const string _event)
{
	FString RetID = "";//FString(_event.argument<std::string>(0).c_str());
	if (!RemoteTrackers.Contains(RetID))
	{
		ASynOpticonState::AddComponentToMap(ComponentTypeEnum::VE_RemoteTracker, RetID);
		RemoteTrackers.Add(RetID);
	}
	FWAMPRetDataStruct* Sample = new FWAMPRetDataStruct();//new FWAMPRetDataStruct(_event.argument<std::array<double, 14>>(1));

	if (!EyeDataQueuesMap.Contains(RetID)) {
		TCircularQueue<FWAMPRetDataStruct*>* DataQueue = new TCircularQueue<FWAMPRetDataStruct*>(121); //Allow one second worth of data to be stored at a time
		DataQueue->Enqueue(Sample);
		EyeDataQueuesMap.Add(RetID, DataQueue);
	}
	else {
		TCircularQueue<FWAMPRetDataStruct*>** EyeDataQueue = EyeDataQueuesMap.Find(RetID);
		if (*EyeDataQueue)
		{
			bool Success = (*EyeDataQueue)->Enqueue(Sample);

			//If the queue was full we need to free the memory we used for the sample
			if (!Success) {
				delete Sample;
			}
		}
	}
}

#pragma warning(pop)