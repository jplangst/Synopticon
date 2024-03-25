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
	//TSharedPtr<wamp_event_handler> NatNetDataSampleHandler(new wamp_event_handler());
	//*NatNetDataSampleHandler = [this](const autobahn::wamp_event& _event) { OnReceiveNatNetData(_event); };
	//FWAMPWorker::SubscribeToTopic(NatNetDataSample, NatNetDataSampleHandler);
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

void UNatNetWAMPComponent::OnReceiveNatNetData(const string _event) //Was const autobahn::wamp_event& _event
{
	////std::list<object> receiveddata = _event.arguments<std::list<object>>();
	////availablerigidbodies.empty();
	////for (object data : receiveddata)
	////{
	////	std::array<object, 4> item = data.as < std::array<object, 4>>();
	////	fstring rigidbodyid = fstring(item[0].as<std::string>().c_str());

	////	frigidbodydatastruct* sample = new frigidbodydatastruct();

	////	bool isopenface = rigidbodyid.equals("openface");

	////	fvector rawposition = fvector(item[1].as<std::array<double, 3>>()[0], item[1].as<std::array<double, 3>>()[1], item[1].as<std::array<double, 3>>()[2]);
	////	if (isopenface) {
	////		sample->position = rawposition;
	////	}
	////	else {
	////		sample->position = usynopticonstatics::convertfrommotivetounrealenginecoordinatesystem(rawposition, asynopticonstate::gettrackeroffset(),
	////			asynopticonstate::ishorizontalcoordinatesystem(), asynopticonstate::getpostrackingunittounrealunitfactor());
	////	}
	////	

	////	
	////	if (isopenface) {
	////		float pitch = fmath::radianstodegrees(item[2].as<std::array<double, 3>>()[0]);
	////		float yaw = fmath::radianstodegrees(item[2].as<std::array<double, 3>>()[1]);
	////		float roll = fmath::radianstodegrees(item[2].as<std::array<double, 3>>()[2]);
	////		sample->orientation = fquat(frotator(pitch, yaw, roll));
	////	}
	////	else {
	////		fquat raworientation = fquat(item[2].as<std::array<double, 4>>()[0], item[2].as<std::array<double, 4>>()[1], item[2].as<std::array<double, 4>>()[2], item[2].as<std::array<double, 4>>()[3]);
	////		sample->orientation = usynopticonstatics::convertfrommotivetounrealenginecoordinatesystem(raworientation, asynopticonstate::ishorizontalcoordinatesystem());
	////	}
	////	
	////	std::list<object> listofmarkers = item[3].as<std::list<object>>();

	////	for (object marker : listofmarkers)
	////	{
	////		try {
	////			fvector rawmarker = fvector(marker.as<array<double, 3>>()[0], marker.as<array<double, 3>>()[1], marker.as<array<double, 3>>()[2]);
	////			sample->markers.add(usynopticonstatics::convertfrommotivetounrealenginecoordinatesystem(rawmarker,
	////				asynopticonstate::gettrackeroffset(), asynopticonstate::ishorizontalcoordinatesystem(),
	////				asynopticonstate::getpostrackingunittounrealunitfactor()));
	////		}
	////		catch (std::exception ex) {

	////		}
	////	}
	////	availablerigidbodies.add(rigidbodyid);
	////	//natnetwampcomponent->addnatnetdatasample(rigidbodyid, sample);
	////	if (!natnetdataqueuesmap.contains(rigidbodyid)) {
	////		tcircularqueue<frigidbodydatastruct*>* dataqueue = new tcircularqueue<frigidbodydatastruct*>(10); //allow one second worth of data to be stored at a time
	////		dataqueue->enqueue(sample);
	////		natnetdataqueuesmap.add(rigidbodyid, dataqueue);
	////	}
	////	else {
	////		tcircularqueue<frigidbodydatastruct*>** natnetdataqueue = natnetdataqueuesmap.find(rigidbodyid);
	////		if (*natnetdataqueue)
	////		{
	////			bool success = (*natnetdataqueue)->enqueue(sample);

	////			//if the queue was full we need to free the memory we used for the sample
	////			if (!success) {
	////				delete sample;
	////			}
	////		}
	////	}
	////}
}

#pragma warning(pop)