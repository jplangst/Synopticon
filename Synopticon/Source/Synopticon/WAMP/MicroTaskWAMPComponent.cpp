// Fill out your copyright notice in the Description page of Project Settings.

#include "MicroTaskWAMPComponent.h"

const FString MicroTaskDataSampleTopic = "Synopticon.WebEntry";
// Sets default values for this component's properties
UMicroTaskWAMPComponent::UMicroTaskWAMPComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMicroTaskWAMPComponent::BeginPlay()
{
	Super::BeginPlay();
	RegisterWAMP();
	// ...
	
}


// Called every frame
void UMicroTaskWAMPComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMicroTaskWAMPComponent::RegisterWAMP()
{
	TSharedPtr<wamp_event_handler> MicroTaskDataSampleHandler(new wamp_event_handler());
	*MicroTaskDataSampleHandler = [this](const autobahn::wamp_event& _event) { OnReceiveMicroTaskData(_event); };
	FWAMPWorker::SubscribeToTopic(MicroTaskDataSampleTopic, MicroTaskDataSampleHandler);
}

void UMicroTaskWAMPComponent::OnReceiveMicroTaskData(const autobahn::wamp_event& _event)
{
	/*5 parameters: 
		int32 ClientID
		int32 QuestionID
		int32 EventCode
		FString Content
		TArray<FString> Relevant AOIs
	*/

	FMicroTaskEventStruct MicroTask;
	MicroTask.ClientID = _event.argument<int>(0);
	MicroTask.QuestionID = _event.argument<int>(1);
	MicroTask.EventCode = _event.argument<int>(2);
	MicroTask.Content = FString(_event.argument<std::string>(3).c_str());
	for (std::string item : _event.argument<std::list<std::string>>(4))
	{
		MicroTask.RelevantAOIs.Add(FString(item.c_str()));
	}

	ASynOpticonState::GetGlobalEventSystem()->OnEventReceiveMicroTaskEvent.Broadcast(MicroTask);
}