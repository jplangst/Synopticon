// Fill out your copyright notice in the Description page of Project Settings.
//4703
#pragma warning(push)
#pragma warning(disable: 4703)
#include "OpenFaceWAMPComponent.h"
//#include "Synopticon.h"

const FString OpenFaceSample = "SynOpticon.OpenFaceSample"; //Testing only

// Sets default values for this component's properties
UOpenFaceWAMPComponent::UOpenFaceWAMPComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}

// Called when the game starts
void UOpenFaceWAMPComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UOpenFaceWAMPComponent::RegisterWAMP()
{
	TSharedPtr<wamp_event_handler> OpenFaceSampleHandler(new wamp_event_handler());
	*OpenFaceSampleHandler = [this](const autobahn::wamp_event& _event) { OnReceiveOpenFaceData(_event); };
	FWAMPWorker::SubscribeToTopic(OpenFaceSample, OpenFaceSampleHandler);
}

void UOpenFaceWAMPComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	TArray<FString> DataMapKeys;
	OpenFaceDataQueuesMap.GetKeys(DataMapKeys);
	for (FString Key : DataMapKeys) {
		TCircularQueue<FWAMPOpenFaceDataStruct*>* DataQueue = *OpenFaceDataQueuesMap.Find(Key);
		if (DataQueue) {
			while (!DataQueue->IsEmpty()) {
				FWAMPOpenFaceDataStruct* Sample;
				DataQueue->Dequeue(Sample);
				delete Sample;
			}
			delete DataQueue;
		}
	}
	OpenFaceDataQueuesMap.Empty();
}

FWAMPOpenFaceDataStruct* UOpenFaceWAMPComponent::GetOpenFaceDataSample(FString Source) {
	FWAMPOpenFaceDataStruct* OpenFaceData = nullptr;

	if (OpenFaceDataQueuesMap.Contains(Source)) {
		TCircularQueue<FWAMPOpenFaceDataStruct*>** OpenFaceDataQueue = OpenFaceDataQueuesMap.Find(Source);
		if (OpenFaceDataQueue)
		{
			if (!(*OpenFaceDataQueue)->IsEmpty()) {
				(*OpenFaceDataQueue)->Dequeue(OpenFaceData);
			}
		}
	}

	return OpenFaceData;
}

void UOpenFaceWAMPComponent::SetCurrentActorSource(FString Source) {
	CurrentActorSource = Source;
}

void UOpenFaceWAMPComponent::OnReceiveOpenFaceData(const autobahn::wamp_event& _event)
{
	/* OpenFaceDataSample: 
	FString ID
	List 2:
		EyeDataSample
		PositionSample
	*/
	FString Source = FString(_event.argument<std::string>(0).c_str());
	if (!OpenFaceSources.Contains(Source))
	{
		ASynOpticonState::AddComponentToMap(ComponentTypeEnum::VE_OpenFaceTracker, Source);
		OpenFaceSources.Add(Source);
	}



	//gaze
	FWAMPEyeDataStruct EyeData(_event.argument<std::array<double, 16>>(1));
	
	//orientation
	std::array<object, 4> item = _event.argument<std::array<object, 4>>(2);
	FString RigidBodyID = FString(item[0].as<std::string>().c_str());

	FRigidBodyDataStruct Orientation;

	FVector RawPosition = FVector(item[1].as<std::array<double, 3>>()[0], item[1].as<std::array<double, 3>>()[1], item[1].as<std::array<double, 3>>()[2]);
	Orientation.Position = RawPosition;

	float Pitch = FMath::RadiansToDegrees(item[2].as<std::array<double, 3>>()[0]);
	float Yaw = FMath::RadiansToDegrees(item[2].as<std::array<double, 3>>()[1]);
	float Roll = FMath::RadiansToDegrees(item[2].as<std::array<double, 3>>()[2]);
	Orientation.Orientation = FQuat(FRotator(Pitch, Yaw, Roll));

	std::list<object> listOfMarkers = item[3].as<std::list<object>>();

	for (object marker : listOfMarkers)
	{
		try {
			FVector RawMarker = FVector(marker.as<array<double, 3>>()[0], marker.as<array<double, 3>>()[1], marker.as<array<double, 3>>()[2]);
			Orientation.Markers.Add(USynOpticonStatics::ConvertFromMotiveToUnrealEngineCoordinateSystem(RawMarker,
				ASynOpticonState::GetTrackerOffset(), ASynOpticonState::IsHorizontalCoordinateSystem(),
				ASynOpticonState::GetPosTrackingUnitToUnrealUnitFactor()));
		}
		catch (std::exception ex) {

		}
	}

	FWAMPOpenFaceDataStruct* OpenFaceData = new FWAMPOpenFaceDataStruct(EyeData, Orientation);

	if (!OpenFaceDataQueuesMap.Contains(Source)) {
		TCircularQueue<FWAMPOpenFaceDataStruct*>* DataQueue = new TCircularQueue<FWAMPOpenFaceDataStruct*>(121); //Allow one second worth of data to be stored at a time
		DataQueue->Enqueue(OpenFaceData);
		OpenFaceDataQueuesMap.Add(Source, DataQueue);
	}
	else {
		TCircularQueue<FWAMPOpenFaceDataStruct*>** DataQueue = OpenFaceDataQueuesMap.Find(Source);
		if (*DataQueue)
		{
			bool Success = (*DataQueue)->Enqueue(OpenFaceData);

			//If the queue was full we need to free the memory we used for the sample
			if (!Success) {
				delete OpenFaceData;
			}
		}
	}
}

#pragma warning(pop)