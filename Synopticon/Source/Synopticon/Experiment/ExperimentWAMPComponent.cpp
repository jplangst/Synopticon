// Fill out your copyright notice in the Description page of Project Settings.

#include "ExperimentWAMPComponent.h"

#include "GazeObjects/GazeActorFactory.h"

const FString ImageAndAOIsTopic = FString("NewImageAndAOIsTopic");
const FString PopupTopic = FString("PopupTopic");
const FString StartStopTopic = FString("StartStopTopic");
const FString EventMarkerTopic = FString("EventMarkerTopic");
const FString HammlabEventMarkerTopic = FString("ife.hammlab.exp.events");
UExperimentWAMPComponent::UExperimentWAMPComponent()
{
}

void UExperimentWAMPComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UExperimentWAMPComponent::RegisterWAMP()
{
	TSharedPtr<wamp_event_handler> ImageAndAOIsTopicHandler(new wamp_event_handler());
	*ImageAndAOIsTopicHandler = [this](const autobahn::wamp_event& _event) { OnReceiveImageAndAOIs(_event); };
	FWAMPWorker::SubscribeToTopic(ImageAndAOIsTopic, ImageAndAOIsTopicHandler);

	TSharedPtr<wamp_event_handler> PopupTopicHandler(new wamp_event_handler());
	*PopupTopicHandler = [this](const autobahn::wamp_event& _event) { OnReceivePopupEvent(_event); };
	FWAMPWorker::SubscribeToTopic(PopupTopic, PopupTopicHandler);

	TSharedPtr<wamp_event_handler> StartStopTopicHandler(new wamp_event_handler());
	*StartStopTopicHandler = [this](const autobahn::wamp_event& _event) { OnReceiveStartStopEvent(_event); };
	FWAMPWorker::SubscribeToTopic(StartStopTopic, StartStopTopicHandler);

	TSharedPtr<wamp_event_handler> EventMarkerTopicHandler(new wamp_event_handler());
	*EventMarkerTopicHandler = [this](const autobahn::wamp_event& _event) { OnReceiveEventMarkers(_event); };
	FWAMPWorker::SubscribeToTopic(EventMarkerTopic, EventMarkerTopicHandler);

	TSharedPtr<wamp_event_handler> HammlabEventMarkerTopicHandler(new wamp_event_handler());
	*HammlabEventMarkerTopicHandler = [this](const autobahn::wamp_event& _event) { OnReceiveHammlabEventMarkers(_event); };
	FWAMPWorker::SubscribeToTopic(HammlabEventMarkerTopic, HammlabEventMarkerTopicHandler);
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

void UExperimentWAMPComponent::OnReceiveImageAndAOIs(const autobahn::wamp_event& _event)
{
	std::array<object, 4> receivedData = _event.arguments<std::array<object, 4>>();

	//ImageName: string
	FString ImageName = FString(receivedData[0].as<std::string>().c_str());
	//SpecifiedID: string
	FString SpecifiedID = FString(receivedData[1].as<std::string>().c_str());
	//ScreenID: string
	FString ScreenID = FString(receivedData[2].as<std::string>().c_str());
	//AOIs: list of array (list for unknown size, array for known size)
	std::list<object> aois = receivedData[3].as<std::list<object>>();

	TArray<FAOIData> AOIs;
	//only way to retrieve items from list is using iterative loop
	for (object aoi : aois)
	{
		//aoi: Name: string, boundingBox: list of array(x, y)
		std::array<object, 2> aoiObj = aoi.as<std::array<object, 2>>();
		FString AOIName = FString(aoiObj[0].as<std::string>().c_str());
		std::list<std::array<float, 2>> coordinates = aoiObj[1].as<std::list<std::array<float, 2>>>();
		FAOIData AOI;
		AOI.ID = UGazeActorFactory::GetValidAOIID();
		UGazeActorFactory::DecrementAOIID();
		AOI.Name = AOIName;
		for (std::array<float, 2> point : coordinates)
		{
			AOI.Coordinates.Add(FVector2D(point[0], point[1]));
		}
		AOIs.Add(AOI);
	}

	FImageAndAOIs ImageAndAOIs(ImageName, SpecifiedID, ScreenID, AOIs);
	ScreenMap.Add(ScreenID, ImageAndAOIs);
	ASynOpticonState::GetGlobalEventSystem()->OnReceiveNewImageAndAOIsDelegate.Broadcast(ImageAndAOIs);
}

void UExperimentWAMPComponent::OnReceivePopupEvent(const autobahn::wamp_event& _event)
{
	std::array<object, 5> receivedData = _event.arguments<std::array<object, 5>>();

	//SpecifiedID: string
	FString SpecifiedID = FString(receivedData[0].as<std::string>().c_str());
	//ScreenID: string
	FString ScreenID = FString(receivedData[1].as<std::string>().c_str());
	//Action: string: Open, Update, Close
	FString Action = FString(receivedData[2].as<std::string>().c_str());
	//AOIs: list of array (list for unknown size, array for known size)
	std::list<object> aois = receivedData[3].as<std::list<object>>();

	TArray<FAOIData> AOIs;
	//only way to retrieve items from list is using iterative loop
	for (object aoi : aois)
	{
		//aoi: Name: string, boundingBox: list of array(x, y)
		std::array<object, 2> aoiObj = aoi.as<std::array<object, 2>>();
		FString AOIName = FString(aoiObj[0].as<std::string>().c_str());
		std::list<std::array<float, 2>> coordinates = aoiObj[1].as<std::list<std::array<float, 2>>>();
		FAOIData AOI;
		if (Action.Equals("Open"))
		{
			AOI.ID = UGazeActorFactory::GetValidAOIID();
			UGazeActorFactory::DecrementAOIID();
		}
		AOI.Name = AOIName;
		for (std::array<float, 2> point : coordinates)
		{
			AOI.Coordinates.Add(FVector2D(point[0], point[1]));
		}
		AOIs.Add(AOI);
	}

	ASynOpticonState::GetGlobalEventSystem()->OnReceivePopupAndAOIsDelegate.Broadcast(FPopupAndAOIs(SpecifiedID, ScreenID, Action, AOIs));
}

void UExperimentWAMPComponent::OnReceiveStartStopEvent(const autobahn::wamp_event& _event)
{
	std::array<object, 1> receivedData = _event.arguments<std::array<object, 1>>();

	//Start: boolean
	bool Start = receivedData[0].as<bool>();
	
	if (Start)
	{
		ASynOpticonState::GetGlobalEventSystem()->OnStartExperimentDelegate.Broadcast();
	}
	else
	{
		ASynOpticonState::GetGlobalEventSystem()->OnStopExperimentDelegate.Broadcast();
	}
}

void UExperimentWAMPComponent::OnReceiveEventMarkers(const autobahn::wamp_event& _event)
{
	std::array<object, 3> receivedData = _event.arguments<std::array<object, 3>>();

	//Name: string
	FString Name = FString(receivedData[0].as<std::string>().c_str());

	//Description: string
	FString Description = FString(receivedData[1].as<std::string>().c_str());

	//SessionID: string
	FString SessionID = FString(receivedData[2].as<std::string>().c_str());

	ASynOpticonState::GetGlobalEventSystem()->OnEventMarkerExperimentDelegate.Broadcast(Name, Description, SessionID);
}

void UExperimentWAMPComponent::OnReceiveHammlabEventMarkers(const autobahn::wamp_event& _event)
{
	std::array<object, 2> receivedData = _event.arguments<std::array<object, 2>>();

	//Name: string
	FString Name = FString(receivedData[0].as<std::string>().c_str());

	//Description: string
	FString Description = FString(receivedData[1].as<std::string>().c_str());

	//SessionID: string
	FString SessionID = "Hammlab";

	ASynOpticonState::GetGlobalEventSystem()->OnEventMarkerExperimentDelegate.Broadcast(Name, Description, SessionID);
}