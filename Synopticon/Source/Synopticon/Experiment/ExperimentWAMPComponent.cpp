// Fill out your copyright notice in the Description page of Project Settings.

#include "ExperimentWAMPComponent.h"

#include "../GazeObjects/GazeActorFactory.h"

const FString ImageAndAOIsTopic = FString("NewImageAndAOIsTopic");
const FString PopupTopic = FString("PopupTopic");
const FString StartStopTopic = FString("StartStopTopic");
const FString EventMarkerTopic = FString("EventMarkerTopic");
const FString HammlabEventMarkerTopic = FString("ife.hammlab.exp.events");

//Recording synch events
const FString RecordingSynchingTopic = FString("Synopticon.RecordingSynchEvents");

UExperimentWAMPComponent::UExperimentWAMPComponent()
{
}

void UExperimentWAMPComponent::BeginPlay()
{
	Super::BeginPlay();

	WaitingForRecordingParticipants = false;
	WaitingTimeout = 5; //Wait for 5 seconds before proceeding
	WaitingTimer = 0; 
}

void UExperimentWAMPComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (WaitingForRecordingParticipants) {
		WaitingTimer += DeltaTime;
		if (WaitingTimer > WaitingTimeout) {
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Sending request to start recording"));

			//Fire off the "Start Recording" event
			FRecordingEventStruct* RecordingEvent = new FRecordingEventStruct();
			RecordingEvent->RecordingStatus = "StartRecording";
			//FWAMPWorker::PublishWAMPEvent(RecordingEvent);

			//Reset values
			WaitingForRecordingParticipants = false;
			WaitingTimer = 0;
		}
	}
}

void UExperimentWAMPComponent::RegisterWAMP()
{
	//TSharedPtr<wamp_event_handler> ImageAndAOIsTopicHandler(new wamp_event_handler());
	//*ImageAndAOIsTopicHandler = [this](const autobahn::wamp_event& _event) { OnReceiveImageAndAOIs(_event); };
	//FWAMPWorker::SubscribeToTopic(ImageAndAOIsTopic, ImageAndAOIsTopicHandler);

	//TSharedPtr<wamp_event_handler> PopupTopicHandler(new wamp_event_handler());
	//*PopupTopicHandler = [this](const autobahn::wamp_event& _event) { OnReceivePopupEvent(_event); };
	//FWAMPWorker::SubscribeToTopic(PopupTopic, PopupTopicHandler);

	//TSharedPtr<wamp_event_handler> StartStopTopicHandler(new wamp_event_handler());
	//*StartStopTopicHandler = [this](const autobahn::wamp_event& _event) { OnReceiveStartStopEvent(_event); };
	//FWAMPWorker::SubscribeToTopic(StartStopTopic, StartStopTopicHandler);

	//TSharedPtr<wamp_event_handler> EventMarkerTopicHandler(new wamp_event_handler());
	//*EventMarkerTopicHandler = [this](const autobahn::wamp_event& _event) { OnReceiveEventMarkers(_event); };
	//FWAMPWorker::SubscribeToTopic(EventMarkerTopic, EventMarkerTopicHandler);

	//TSharedPtr<wamp_event_handler> HammlabEventMarkerTopicHandler(new wamp_event_handler());
	//*HammlabEventMarkerTopicHandler = [this](const autobahn::wamp_event& _event) { OnReceiveHammlabEventMarkers(_event); };
	//FWAMPWorker::SubscribeToTopic(HammlabEventMarkerTopic, HammlabEventMarkerTopicHandler);

	////Recording synching
	//TSharedPtr<wamp_event_handler> RecordingSynchTopicHandler(new wamp_event_handler());
	//*RecordingSynchTopicHandler = [this](const autobahn::wamp_event& _event) { OnRecieveRecordingSynchEvent(_event); };
	//FWAMPWorker::SubscribeToTopic(RecordingSynchingTopic, RecordingSynchTopicHandler);
}

void UExperimentWAMPComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UExperimentWAMPComponent::NewImageAndAOIs(FImageAndAOIs ImageAndAOIs)
{
	ScreenMap.Add(ImageAndAOIs.ScreenID, ImageAndAOIs);
	ASynOpticonState::GetGlobalEventSystem()->OnReceiveNewImageAndAOIsDelegate.Broadcast(ImageAndAOIs);
}

void UExperimentWAMPComponent::UpdatePopupAndAOIs(FPopupAndAOIs PopupAndAOIs)
{
	ASynOpticonState::GetGlobalEventSystem()->OnReceivePopupAndAOIsDelegate.Broadcast(PopupAndAOIs);
}

void UExperimentWAMPComponent::StartStopExperiment(bool Start)
{
	if(Start)
		ASynOpticonState::GetGlobalEventSystem()->OnStartExperimentDelegate.Broadcast();
	else
		ASynOpticonState::GetGlobalEventSystem()->OnStopExperimentDelegate.Broadcast();
}

void UExperimentWAMPComponent::EventMarkerExperiment(FString Name, FString Description, FString SessionID)
{
	ASynOpticonState::GetGlobalEventSystem()->OnEventMarkerExperimentDelegate.Broadcast(Name, Description, SessionID);
}

TArray<FString> UExperimentWAMPComponent::GetListOfScreens()
{
	TArray<FString> Screens;
	ScreenMap.GenerateKeyArray(Screens);
	return Screens;
}

FImageAndAOIs UExperimentWAMPComponent::GetCurrentImageAndAOIs(FString Screen)
{
	FImageAndAOIs* ImageAndAOIs = ScreenMap.Find(Screen);
	if (ImageAndAOIs)
	{
		return *ImageAndAOIs;
	}
	return FImageAndAOIs();
}

void UExperimentWAMPComponent::OnReceiveImageAndAOIs(const string _event)
{
	//std::array<object, 4> receivedData = _event.arguments<std::array<object, 4>>();

	////ImageName: string
	//FString ImageName = FString(receivedData[0].as<std::string>().c_str());
	////SpecifiedID: string
	//FString SpecifiedID = FString(receivedData[1].as<std::string>().c_str());
	////ScreenID: string
	//FString ScreenID = FString(receivedData[2].as<std::string>().c_str());
	////AOIs: list of array (list for unknown size, array for known size)
	//std::list<object> aois = receivedData[3].as<std::list<object>>();

	//TArray<FAOIData> AOIs;
	////only way to retrieve items from list is using iterative loop
	//for (object aoi : aois)
	//{
	//	//aoi: Name: string, boundingBox: list of array(x, y)
	//	std::array<object, 2> aoiObj = aoi.as<std::array<object, 2>>();
	//	FString AOIName = FString(aoiObj[0].as<std::string>().c_str());
	//	std::list<std::array<float, 2>> coordinates = aoiObj[1].as<std::list<std::array<float, 2>>>();
	//	FAOIData AOI;
	//	AOI.ID = UGazeActorFactory::GetValidAOIID();
	//	UGazeActorFactory::DecrementAOIID();
	//	AOI.Name = AOIName;
	//	for (std::array<float, 2> point : coordinates)
	//	{
	//		AOI.Coordinates.Add(FVector2D(point[0], point[1]));
	//	}
	//	AOIs.Add(AOI);
	//}

	//FImageAndAOIs ImageAndAOIs(ImageName, SpecifiedID, ScreenID, AOIs);
	//ScreenMap.Add(ScreenID, ImageAndAOIs);
	//ASynOpticonState::GetGlobalEventSystem()->OnReceiveNewImageAndAOIsDelegate.Broadcast(ImageAndAOIs);
}

void UExperimentWAMPComponent::OnReceivePopupEvent(const string _event)
{
	//std::array<object, 5> receivedData = _event.arguments<std::array<object, 5>>();

	////SpecifiedID: string
	//FString SpecifiedID = FString(receivedData[0].as<std::string>().c_str());
	////ScreenID: string
	//FString ScreenID = FString(receivedData[1].as<std::string>().c_str());
	////Action: string: Open, Update, Close
	//FString Action = FString(receivedData[2].as<std::string>().c_str());
	////AOIs: list of array (list for unknown size, array for known size)
	//std::list<object> aois = receivedData[3].as<std::list<object>>();

	//TArray<FAOIData> AOIs;
	////only way to retrieve items from list is using iterative loop
	//for (object aoi : aois)
	//{
	//	//aoi: Name: string, boundingBox: list of array(x, y)
	//	std::array<object, 2> aoiObj = aoi.as<std::array<object, 2>>();
	//	FString AOIName = FString(aoiObj[0].as<std::string>().c_str());
	//	std::list<std::array<float, 2>> coordinates = aoiObj[1].as<std::list<std::array<float, 2>>>();
	//	FAOIData AOI;
	//	if (Action.Equals("Open"))
	//	{
	//		AOI.ID = UGazeActorFactory::GetValidAOIID();
	//		UGazeActorFactory::DecrementAOIID();
	//	}
	//	AOI.Name = AOIName;
	//	for (std::array<float, 2> point : coordinates)
	//	{
	//		AOI.Coordinates.Add(FVector2D(point[0], point[1]));
	//	}
	//	AOIs.Add(AOI);
	//}

	//ASynOpticonState::GetGlobalEventSystem()->OnReceivePopupAndAOIsDelegate.Broadcast(FPopupAndAOIs(SpecifiedID, ScreenID, Action, AOIs));
}

void UExperimentWAMPComponent::OnReceiveStartStopEvent(const string _event)
{
	//std::array<object, 1> receivedData = _event.arguments<std::array<object, 1>>();

	////Start: boolean
	//bool Start = receivedData[0].as<bool>();
	//
	//if (Start)
	//{
	//	ASynOpticonState::GetGlobalEventSystem()->OnStartExperimentDelegate.Broadcast();
	//}
	//else
	//{
	//	ASynOpticonState::GetGlobalEventSystem()->OnStopExperimentDelegate.Broadcast();
	//}
}

void UExperimentWAMPComponent::OnReceiveEventMarkers(const string _event)
{
	//std::array<object, 3> receivedData = _event.arguments<std::array<object, 3>>();

	////Name: string
	//FString Name = FString(receivedData[0].as<std::string>().c_str());

	////Description: string
	//FString Description = FString(receivedData[1].as<std::string>().c_str());

	////SessionID: string
	//FString SessionID = FString(receivedData[2].as<std::string>().c_str());

	//ASynOpticonState::GetGlobalEventSystem()->OnEventMarkerExperimentDelegate.Broadcast(Name, Description, SessionID);
}

void UExperimentWAMPComponent::OnReceiveHammlabEventMarkers(const string _event)
{
	//std::array<object, 2> receivedData = _event.arguments<std::array<object, 2>>();

	////Name: string
	//FString Name = FString(receivedData[0].as<std::string>().c_str());

	////Description: string
	//FString Description = FString(receivedData[1].as<std::string>().c_str());

	////SessionID: string
	//FString SessionID = "Hammlab";

	//ASynOpticonState::GetGlobalEventSystem()->OnEventMarkerExperimentDelegate.Broadcast(Name, Description, SessionID);
}

//Recording Synch Events
void UExperimentWAMPComponent::StartSynchingRecording()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Preparing recording synch, asking for participants"));

	// First we request whoever is listening and want to be part of the recording to announce themselves
	FRecordingEventStruct* RecordingEvent = new FRecordingEventStruct();
	RecordingEvent->RecordingStatus = "AnnounceRecordingParticipation";
	//FWAMPWorker::PublishWAMPEvent(RecordingEvent);

	//Next we set a flag that is used by the TickComponent function to trigger the next step of the synching
	WaitingForRecordingParticipants = true;
}

void UExperimentWAMPComponent::OnRecieveRecordingSynchEvent(const string _event)
{
	//std::array<object, 2> receivedData = _event.arguments<std::array<object, 2>>();

	////ImageName: string
	//FString EventType = FString(receivedData[0].as<std::string>().c_str());
	////SpecifiedID: string
	//FString SenderName = FString(receivedData[1].as<std::string>().c_str());

	//if (EventType.Equals("ParticipantName")) {
	//	RecordingSynchStatus.Add(SenderName, false);

	//	if (GEngine)
	//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Participant responded %s"), *SenderName));
	//}
	//else if (EventType.Equals("ReadyToRecord")) {
	//	RecordingSynchStatus.Add(SenderName, true);

	//	if (GEngine)
	//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Participant ready to record %s"), *SenderName));

	//	bool AllReady = true;
	//	//Check if all the participating streams are ready to record
	//	for (auto& Elem : RecordingSynchStatus)
	//	{
	//		bool Ready = Elem.Value;
	//		if (!Ready)
	//		{
	//			AllReady = false;
	//			break;
	//		}
	//	}

	//	//All participants are ready, send the Set Start Timestamp event
	//	if (AllReady) { 
	//		if (GEngine)
	//			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("All participants ready, sending synch message"));

	//		FRecordingEventStruct* RecordingEvent = new FRecordingEventStruct();
	//		RecordingEvent->RecordingStatus = "SetStartTimestamp";
	//		FWAMPWorker::PublishWAMPEvent(RecordingEvent);

	//		//TODO set internal timestamp for our binary recording here
	//	}
	//}
}
