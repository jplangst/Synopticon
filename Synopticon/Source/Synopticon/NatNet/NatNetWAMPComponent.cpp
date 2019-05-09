#pragma warning(push)
#pragma warning(disable: 4703)
#include "NatNetWAMPComponent.h"

const FString NatNetDataSample = "NatNetDataSample";
// Sets default values for this component's properties
UNatNetWAMPComponent::UNatNetWAMPComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}


// Called when the game starts
void UNatNetWAMPComponent::BeginPlay()
{
	Super::BeginPlay();

	//FNatNetWAMPWorker::JoyInit(this, ASynOpticonState::GetWAMPRouterAdress(), ASynOpticonState::GetWAMPRealm(), FString("NatNetDataSample"));

	// ...
	
	ASynOpticonState::GetGlobalEventSystem()->OnEventWAMPComponentDisconnected.AddDynamic(this, &UNatNetWAMPComponent::OnNatNetWAMPComponentDisconnected);
	ASynOpticonState::GetGlobalEventSystem()->OnEventWAMPComponentConnected.AddDynamic(this, &UNatNetWAMPComponent::OnNatNetWAMPComponentConnected);
}

void UNatNetWAMPComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	TArray<FString> NatNetDataMapKeys;
	NatNetDataQueuesMap.GetKeys(NatNetDataMapKeys);
	for (FString Key : NatNetDataMapKeys) 
	{
		TCircularQueue<FRigidBodyDataStruct*>* NatNetDataQueue = *NatNetDataQueuesMap.Find(Key);
		if (NatNetDataQueue) {
			while (!NatNetDataQueue->IsEmpty()) {
				FRigidBodyDataStruct* Sample;
				NatNetDataQueue->Dequeue(Sample);
				delete Sample;
			}
			delete NatNetDataQueue;
		}
	}
	NatNetDataQueuesMap.Empty();
}

void UNatNetWAMPComponent::RegisterWAMP()
{
	TSharedPtr<wamp_event_handler> NatNetDataSampleHandler(new wamp_event_handler());
	*NatNetDataSampleHandler = [this](const autobahn::wamp_event& _event) { OnReceiveNatNetData(_event); };
	FWAMPWorker::SubscribeToTopic(NatNetDataSample, NatNetDataSampleHandler);
}

FRigidBodyDataStruct* UNatNetWAMPComponent::GetNatNetDataSample(FString RigidBodyID) {
	FRigidBodyDataStruct* NatNetData = nullptr;

	TCircularQueue<FRigidBodyDataStruct*>** NatNetDataQueue = NatNetDataQueuesMap.Find(RigidBodyID);
	if (NatNetDataQueue)
	{
		if (!(*NatNetDataQueue)->IsEmpty()) {
			(*NatNetDataQueue)->Dequeue(NatNetData);
		}
	}

	return NatNetData;
}

void UNatNetWAMPComponent::OnNatNetWAMPComponentDisconnected(ComponentTypeEnum ComponentType, FString ComponentID)
{
	AvailableRigidBodies.Empty();
	if (ComponentType == ComponentTypeEnum::VE_NatNet)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("NatNet has disconnected from router!"));
		}
	}
}

void UNatNetWAMPComponent::OnNatNetWAMPComponentConnected(ComponentTypeEnum ComponentType, FString ComponentID)
{
	if (ComponentType == ComponentTypeEnum::VE_NatNet)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("NatNet has connected to router!"));
		}
	}
}

TArray<FString> UNatNetWAMPComponent::GetRigidBodies()
{
	//TArray<FString> RigidBodies;
	//NatNetDataQueuesMap.GenerateKeyArray(RigidBodies);
	return AvailableRigidBodies;
}

void UNatNetWAMPComponent::OnReceiveNatNetData(const autobahn::wamp_event& _event)
{
	std::list<object> receivedData = _event.arguments<std::list<object>>();
	AvailableRigidBodies.Empty();
	for (object data : receivedData)
	{
		std::array<object, 4> item = data.as < std::array<object, 4>>();
		FString RigidBodyID = FString(item[0].as<std::string>().c_str());

		FRigidBodyDataStruct* Sample = new FRigidBodyDataStruct();

		bool IsOpenFace = RigidBodyID.Equals("OpenFace");

		FVector RawPosition = FVector(item[1].as<std::array<double, 3>>()[0], item[1].as<std::array<double, 3>>()[1], item[1].as<std::array<double, 3>>()[2]);
		if (IsOpenFace) {
			Sample->Position = RawPosition;
		}
		else {
			Sample->Position = USynOpticonStatics::ConvertFromMotiveToUnrealEngineCoordinateSystem(RawPosition, ASynOpticonState::GetTrackerOffset(),
				ASynOpticonState::IsHorizontalCoordinateSystem(), ASynOpticonState::GetPosTrackingUnitToUnrealUnitFactor());
		}
		

		
		if (IsOpenFace) {
			float Pitch = FMath::RadiansToDegrees(item[2].as<std::array<double, 3>>()[0]);
			float Yaw = FMath::RadiansToDegrees(item[2].as<std::array<double, 3>>()[1]);
			float Roll = FMath::RadiansToDegrees(item[2].as<std::array<double, 3>>()[2]);
			Sample->Orientation = FQuat(FRotator(Pitch, Yaw, Roll));
		}
		else {
			FQuat RawOrientation = FQuat(item[2].as<std::array<double, 4>>()[0], item[2].as<std::array<double, 4>>()[1], item[2].as<std::array<double, 4>>()[2], item[2].as<std::array<double, 4>>()[3]);
			Sample->Orientation = USynOpticonStatics::ConvertFromMotiveToUnrealEngineCoordinateSystem(RawOrientation, ASynOpticonState::IsHorizontalCoordinateSystem());
		}
		
		std::list<object> listOfMarkers = item[3].as<std::list<object>>();

		for (object marker : listOfMarkers)
		{
			try {
				FVector RawMarker = FVector(marker.as<array<double, 3>>()[0], marker.as<array<double, 3>>()[1], marker.as<array<double, 3>>()[2]);
				Sample->Markers.Add(USynOpticonStatics::ConvertFromMotiveToUnrealEngineCoordinateSystem(RawMarker,
					ASynOpticonState::GetTrackerOffset(), ASynOpticonState::IsHorizontalCoordinateSystem(),
					ASynOpticonState::GetPosTrackingUnitToUnrealUnitFactor()));
			}
			catch (std::exception ex) {

			}
		}
		AvailableRigidBodies.Add(RigidBodyID);
		//NatNetWAMPComponent->AddNatNetDataSample(RigidBodyID, Sample);
		if (!NatNetDataQueuesMap.Contains(RigidBodyID)) {
			TCircularQueue<FRigidBodyDataStruct*>* DataQueue = new TCircularQueue<FRigidBodyDataStruct*>(10); //Allow one second worth of data to be stored at a time
			DataQueue->Enqueue(Sample);
			NatNetDataQueuesMap.Add(RigidBodyID, DataQueue);
		}
		else {
			TCircularQueue<FRigidBodyDataStruct*>** NatNetDataQueue = NatNetDataQueuesMap.Find(RigidBodyID);
			if (*NatNetDataQueue)
			{
				bool Success = (*NatNetDataQueue)->Enqueue(Sample);

				//If the queue was full we need to free the memory we used for the sample
				if (!Success) {
					delete Sample;
				}
			}
		}
	}
}

#pragma warning(pop)