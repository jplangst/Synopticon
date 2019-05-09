// Fill out your copyright notice in the Description page of Project Settings.
#include "SynOpticonAggLogComponent.h"
//#include "Synopticon.h"


double USynOpticonAggLogComponent::NOISE_THRESHOLD = 0.05;
double USynOpticonAggLogComponent::DURATION_THRESHOLD = 0.15; 

// Sets default values for this component's properties
USynOpticonAggLogComponent::USynOpticonAggLogComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	RaycastComponent = nullptr;
	LeftHandComponent = nullptr;
	RightHandComponent = nullptr;
	OwnerID = -1;
	//PreviousHitObjectID = -1;

	GazeControlStruct.Type = LoggingTypeEnum::LT_GazeData;
	LeftHandControlStruct.Type = LoggingTypeEnum::LT_LeftHandData;
	RightHandControlStruct.Type = LoggingTypeEnum::LT_RightHandData;
}


// Called when the game starts
void USynOpticonAggLogComponent::BeginPlay()
{
	Super::BeginPlay();
	GazeLoggedData.TotalTime = 0;

	ASynOpticonState::SetStartTimeToNow();
	ASynOpticonState::GetGlobalEventSystem()->OnEventStartRecord.AddDynamic(this, &USynOpticonAggLogComponent::ResetLoggedData);
	ASynOpticonState::GetGlobalEventSystem()->OnEventStartReplay.AddDynamic(this, &USynOpticonAggLogComponent::ResetLoggedData);
	ASynOpticonState::GetGlobalEventSystem()->OnRemoveGazeGroupDelegate.AddDynamic(this, &USynOpticonAggLogComponent::RemoveObjectsRelatedToID);
}


// Called every frame
void USynOpticonAggLogComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (!ASynOpticonState::IsReplaying())
	{
		if (RaycastComponent) {
			DoLogging(DeltaTime, RaycastComponent->GetHitTargetID(), GazeControlStruct, GazeLoggedData);
		}
		else if (RemoteTrackerComponent) {
			DoLogging(DeltaTime, RemoteTrackerComponent->GetHitTargetID(), GazeControlStruct, GazeLoggedData);
		}

		if (LeftHandComponent) {
			DoLogging(DeltaTime, LeftHandComponent->GetHitTargetID(), LeftHandControlStruct, LeftHandLoggedData);
		}
		if (RightHandComponent) {
			DoLogging(DeltaTime, RightHandComponent->GetHitTargetID(), RightHandControlStruct, RightHandLoggedData);
		}

	}
	else
	{
		ReplayLogging();
	}
}

// Called when the game ends
void USynOpticonAggLogComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	RaycastComponent = nullptr;
	LeftHandComponent = nullptr;
	RightHandComponent = nullptr;
	OwnerID = -1;
	//PreviousHitObjectID = -1;
	GazeLoggedData.GazePaths.Empty();
	GazeLoggedData.Objects.Empty();
	GazeLoggedData.TotalTime = 0;

	LeftHandLoggedData.GazePaths.Empty();
	LeftHandLoggedData.Objects.Empty();
	LeftHandLoggedData.TotalTime = 0;

	RightHandLoggedData.GazePaths.Empty();
	RightHandLoggedData.Objects.Empty();
	RightHandLoggedData.TotalTime = 0;
	//ASynOpticonState::GetGlobalEventSystem()->OnEventStartRecordReplay.RemoveAll(this);
	//ASynOpticonState::GetGlobalEventSystem()->OnEventStartPlayReplay.RemoveAll(this);
}

void USynOpticonAggLogComponent::DoLogging(float DeltaTime, int32 HitTargetID, FLoggingControlStruct& ControlStruct, FSynOpticonActorLoggingStruct& LoggingStruct)
{
	FDateTime CurrentTime = FDateTime::UtcNow();
	if (HitTargetID == -1)
	{
		//Not Looking At Object Anymore
		if (ControlStruct.PreviousHitObjectID != -1)
		{
			if (ControlStruct.NotLookingStartTime != -1)
			{
				FTimespan Duration = CurrentTime - ControlStruct.NotLookingStartTime;
				if (Duration.GetTotalSeconds() > NOISE_THRESHOLD) //If the duration is above the threshold we are not looking at the object anymore
				{					
					ControlStruct.NotLookingStartTime = -1;
					ControlStruct.PreviousHitObjectID = -1;
				}
			}
			else
			{
				ControlStruct.NotLookingStartTime = CurrentTime;
			}
		}
	}
	else
	{
		if (ControlStruct.PreviousHitObjectID == -1)
		{
			ControlStruct.StartLookingTime = CurrentTime;
			
			DeleteLastGazePath(ControlStruct, LoggingStruct);

			ControlStruct.PreviousHitObjectID = HitTargetID;
			
			AddGazePath(ControlStruct.PreviousHitObjectID, 0, LoggingStruct, ControlStruct.Type);
		}
		else
		{
			//Started Looking at new object. Register the event
			if (ControlStruct.PreviousHitObjectID != HitTargetID)
			{
				
				DeleteLastGazePath(ControlStruct, LoggingStruct);

				ControlStruct.StartLookingTime = CurrentTime;
				ControlStruct.PreviousHitObjectID = HitTargetID;
				
				AddGazePath(ControlStruct.PreviousHitObjectID, 0, LoggingStruct, ControlStruct.Type);
			}
			else //Still looking at the same object, increase the duration it has been looked at
			{
				
				IncreaseLastGazeEventDuration(DeltaTime, LoggingStruct);
				
				if(LoggingStruct.Objects.Find(ControlStruct.PreviousHitObjectID))
					LoggingStruct.Objects.Find(ControlStruct.PreviousHitObjectID)->Time += DeltaTime;
			}
		}
		ControlStruct.NotLookingStartTime = -1;
	}
	
	UpdateActorsTotalTimeAndObjectPercentage(ControlStruct, LoggingStruct);
}

void USynOpticonAggLogComponent::DeleteLastGazePath(FLoggingControlStruct ControlStruct, FSynOpticonActorLoggingStruct& LoggingStruct)
{
	if (LoggingStruct.GazePaths.Num() > 0 && LoggingStruct.GazePaths.Last().Duration < DURATION_THRESHOLD)
	{
		if (LoggingStruct.Objects.Contains(ControlStruct.PreviousHitObjectID))
		{
			LoggingStruct.Objects.Find(ControlStruct.PreviousHitObjectID)->Time -= LoggingStruct.GazePaths.Last().Duration;
			LoggingStruct.Objects.Find(ControlStruct.PreviousHitObjectID)->Count -= 1;
		}

		LoggingStruct.GazePaths.RemoveAt(LoggingStruct.GazePaths.Num() - 1);
	}
}

void USynOpticonAggLogComponent::IncreaseLastGazeEventDuration(float DeltaTime, FSynOpticonActorLoggingStruct& LoggingStruct)
{
	if (LoggingStruct.GazePaths.Num() > 0)
	{
		LoggingStruct.GazePaths.Last().Duration += DeltaTime;
		LoggingStruct.TotalTime += DeltaTime;
	}
}

void USynOpticonAggLogComponent::SetRaycastComponent(URaycastComponent_Ours* _RaycastComponent)
{
	RaycastComponent = _RaycastComponent;
}

void USynOpticonAggLogComponent::SetLeftHandComponent(UHandComponent* _LeftHandComponent)
{
	LeftHandComponent = _LeftHandComponent;
}

void USynOpticonAggLogComponent::SetRightHandComponent(UHandComponent* _RightHandComponent)
{
	RightHandComponent = _RightHandComponent;
}

void USynOpticonAggLogComponent::SetRemoteTrackerComponent(URemoteEyeTrackerComponent* _RetComponent)
{
	RemoteTrackerComponent = _RetComponent;
}




void USynOpticonAggLogComponent::SetOwnerID(int32 _OwnerID)
{
	OwnerID = _OwnerID;
	GazeLoggedData.SynOpticonActorID = _OwnerID;
	//TODO
	LeftHandLoggedData.SynOpticonActorID = _OwnerID;
	RightHandLoggedData.SynOpticonActorID = _OwnerID;
}

void USynOpticonAggLogComponent::AddGazedObject(int32 ObjectID, double Duration, FSynOpticonActorLoggingStruct& LoggingStruct, LoggingTypeEnum Type)
{
	if (LoggingStruct.Objects.Contains(ObjectID))
	{
		FObjectLoggingInfoStruct* ObjectStruct;
		ObjectStruct = LoggingStruct.Objects.Find(ObjectID);
		ObjectStruct->Time += Duration;
		ObjectStruct->Count++;
		ObjectStruct = nullptr;
	}
	else
	{
		FObjectLoggingInfoStruct ObjectStruct;
		ObjectStruct.Time += Duration;
		ObjectStruct.Count++;
		LoggingStruct.Objects.Add(ObjectID, ObjectStruct);
		OnEventAddObjectToGazePath.Broadcast(ObjectID, ObjectStruct, Type);
	}
	LoggingStruct.TotalTime += Duration;
}

void USynOpticonAggLogComponent::AddGazePath(int32 ObjectID, double Duration, FSynOpticonActorLoggingStruct& LoggingStruct, LoggingTypeEnum Type)
{
	FDateTime TimeStamp = FDateTime::UtcNow();

	if (TimeStamp > ASynOpticonState::GetStartTime())
	{
		AddGazedObject(ObjectID, Duration, LoggingStruct, Type);

		FGazePathLoggingInfoStruct Path;
		Path.TimeStamp = TimeStamp;
		Path.GazeActorID = ObjectID;
		Path.Duration = Duration;
		LoggingStruct.GazePaths.Add(Path);
	}
}

void USynOpticonAggLogComponent::UpdateActorsTotalTimeAndObjectPercentage(FLoggingControlStruct& ControlStruct, FSynOpticonActorLoggingStruct& LoggingStruct)
{
	FDateTime EndTime, StartTime;
	if (!ASynOpticonState::IsReplaying()) 
	{
		EndTime = FDateTime::UtcNow();
		StartTime = ASynOpticonState::GetStartTime();
	}
	else
	{
		EndTime = ASynOpticonState::GetCurrentReplayTime();
		StartTime = ASynOpticonState::ReplayHeader.StartTimestamp;
	}
	
	double RunTimeInSeconds = (EndTime - StartTime).GetTotalSeconds();

	//ActorLoggedData.TotalTime += RunTimeInSeconds;
	float TotalUse = 0;

	for (auto& ObjectElem : LoggingStruct.Objects)
	{
		ObjectElem.Value.Percentage = ObjectElem.Value.Time * 100.0 / RunTimeInSeconds;
		TotalUse += ObjectElem.Value.Percentage;
	}
	ControlStruct.UnspecifiedPercentage = 100 - TotalUse;
}

void USynOpticonAggLogComponent::ResetLoggedData(bool Start)
{
	ASynOpticonState::SetStartTimeToNow();
	GazeLoggedData.GazePaths.Empty();
	GazeLoggedData.Objects.Empty();
	GazeLoggedData.TotalTime = 0;
	GazeControlStruct.PreviousHitObjectID = -1;

	LeftHandLoggedData.GazePaths.Empty();
	LeftHandLoggedData.Objects.Empty();
	LeftHandLoggedData.TotalTime = 0;
	LeftHandControlStruct.PreviousHitObjectID = -1;

	RightHandLoggedData.GazePaths.Empty();
	RightHandLoggedData.Objects.Empty();
	RightHandLoggedData.TotalTime = 0;
	RightHandControlStruct.PreviousHitObjectID = -1;
	
	OnEventResetLoggedData.Broadcast();
}

TArray<FObjectLoggingInfoStruct> USynOpticonAggLogComponent::GetLoggedObjectDataArray(LoggingTypeEnum _Type)
{
	TArray<FObjectLoggingInfoStruct> ValueArray;
	switch (_Type) {
	case LoggingTypeEnum::LT_LeftHandData: LeftHandLoggedData.Objects.GenerateValueArray(ValueArray); break;
	case LoggingTypeEnum::LT_RightHandData: RightHandLoggedData.Objects.GenerateValueArray(ValueArray); break;
	default:
		GazeLoggedData.Objects.GenerateValueArray(ValueArray); break;
	}
	
	return ValueArray;
}

TArray<int32> USynOpticonAggLogComponent::GetLoggedObjectArray(LoggingTypeEnum _Type)
{
	TArray<int32> KeyArray;
	switch (_Type) {
	case LoggingTypeEnum::LT_LeftHandData: 
		if (ASynOpticonState::IsReplaying()) {
			return ReplayLeftHandObjectList;
		}

		LeftHandLoggedData.Objects.GenerateKeyArray(KeyArray);
		break;
	case LoggingTypeEnum::LT_RightHandData:
		if (ASynOpticonState::IsReplaying()) {
			return ReplayRightHandObjectList;
		}

		RightHandLoggedData.Objects.GenerateKeyArray(KeyArray);
		break;
	default: 
		if (ASynOpticonState::IsReplaying()) {
			return ReplayGazeObjectList;
		}

		GazeLoggedData.Objects.GenerateKeyArray(KeyArray);
		break;
	}

	return KeyArray;
}

FObjectLoggingInfoStruct USynOpticonAggLogComponent::GetLoggedObjectData(int32 ID, LoggingTypeEnum _Type)
{
	FObjectLoggingInfoStruct* result = nullptr;
	switch (_Type) {
	case LoggingTypeEnum::LT_LeftHandData:
		result = LeftHandLoggedData.Objects.Find(ID);
		break;
	case LoggingTypeEnum::LT_RightHandData:
		result = RightHandLoggedData.Objects.Find(ID);
		break;
	default:
		result = GazeLoggedData.Objects.Find(ID);
		break;
	}
	if (result)
		return *result;
	return FObjectLoggingInfoStruct();
}

void USynOpticonAggLogComponent::RemoveObjectsRelatedToIDHelper(int32 ID, FSynOpticonActorLoggingStruct& LoggingStruct)
{
	TArray<FGazePathLoggingInfoStruct> ToDeleteList;
	for (FGazePathLoggingInfoStruct GazeEvent : LoggingStruct.GazePaths)
	{
		if (GazeEvent.GazeActorID == ID)
		{
			ToDeleteList.Add(GazeEvent);
		}
	}

	for (FGazePathLoggingInfoStruct GE : ToDeleteList)
	{
		LoggingStruct.GazePaths.Remove(GE);
	}

	if (LoggingStruct.Objects.Contains(ID))
	{
		LoggingStruct.Objects.Remove(ID);
		LoggingStruct.Objects.Shrink();
	}
}

void USynOpticonAggLogComponent::RemoveObjectsRelatedToID(int32 ID)
{
	RemoveObjectsRelatedToIDHelper(ID, GazeLoggedData);
	RemoveObjectsRelatedToIDHelper(ID, LeftHandLoggedData);
	RemoveObjectsRelatedToIDHelper(ID, RightHandLoggedData);
}

void USynOpticonAggLogComponent::SetReplayLogComponentData(int32 _OwnerID, FSynOpticonActorLoggingStruct LogComponentData, LoggingTypeEnum _Type)
{
	switch (_Type) {
	case LoggingTypeEnum::LT_LeftHandData: 
		LeftHandLoggedData = LogComponentData;
		SetOwnerID(_OwnerID);
		LeftHandLoggedData.Objects.GenerateKeyArray(ReplayLeftHandObjectList);
		break;
	case LoggingTypeEnum::LT_RightHandData:
		RightHandLoggedData = LogComponentData;
		SetOwnerID(_OwnerID);
		RightHandLoggedData.Objects.GenerateKeyArray(ReplayRightHandObjectList);
		break;
	default:
		GazeLoggedData = LogComponentData;
		SetOwnerID(_OwnerID);
		GazeLoggedData.Objects.GenerateKeyArray(ReplayGazeObjectList);
		break;
	}

	//ActorLoggedData.TotalTime = 0;
	//PrimaryComponentTick.bCanEverTick = false;
}

void USynOpticonAggLogComponent::ReplayLogging()
{
	FDateTime CurrentTime = ASynOpticonState::GetCurrentReplayTime();
	GazeLoggedData.Objects.Empty();
	for (FGazePathLoggingInfoStruct Path : GazeLoggedData.GazePaths)
	{
		if (Path.TimeStamp > CurrentTime)
		{
			break;
		}
		double duration = (CurrentTime - Path.TimeStamp).GetTotalSeconds();
		AddGazedObject(Path.GazeActorID, duration <= Path.Duration ? duration : Path.Duration, GazeLoggedData, LoggingTypeEnum::LT_GazeData);
	}
	UpdateActorsTotalTimeAndObjectPercentage(GazeControlStruct, GazeLoggedData);

	LeftHandLoggedData.Objects.Empty();
	for (FGazePathLoggingInfoStruct Path : LeftHandLoggedData.GazePaths)
	{
		if (Path.TimeStamp > CurrentTime)
		{
			break;
		}
		double duration = (CurrentTime - Path.TimeStamp).GetTotalSeconds();
		AddGazedObject(Path.GazeActorID, duration <= Path.Duration ? duration : Path.Duration, LeftHandLoggedData, LoggingTypeEnum::LT_LeftHandData);
	}
	UpdateActorsTotalTimeAndObjectPercentage(LeftHandControlStruct, LeftHandLoggedData);

	RightHandLoggedData.Objects.Empty();
	for (FGazePathLoggingInfoStruct Path : RightHandLoggedData.GazePaths)
	{
		if (Path.TimeStamp > CurrentTime)
		{
			break;
		}
		double duration = (CurrentTime - Path.TimeStamp).GetTotalSeconds();
		AddGazedObject(Path.GazeActorID, duration <= Path.Duration ? duration : Path.Duration, RightHandLoggedData, LoggingTypeEnum::LT_RightHandData);
	}
	UpdateActorsTotalTimeAndObjectPercentage(RightHandControlStruct, RightHandLoggedData);
}

FSynOpticonActorLoggingStruct USynOpticonAggLogComponent::GetLoggedData(LoggingTypeEnum _Type)
{
	switch (_Type) {
	case LoggingTypeEnum::LT_LeftHandData: return LeftHandLoggedData;
	case LoggingTypeEnum::LT_RightHandData: return RightHandLoggedData;
	default:
		return GazeLoggedData;
	}
}

float USynOpticonAggLogComponent::GetUnspecifiedPercentage(LoggingTypeEnum _Type) 
{ 
	switch (_Type) {
	case LoggingTypeEnum::LT_LeftHandData: return LeftHandControlStruct.UnspecifiedPercentage;
	case LoggingTypeEnum::LT_RightHandData: return RightHandControlStruct.UnspecifiedPercentage;
	default: return GazeControlStruct.UnspecifiedPercentage;
	}
}

bool USynOpticonAggLogComponent::HasLoggedComponent(LoggingTypeEnum _Type)
{
	switch (_Type) {
	case LoggingTypeEnum::LT_LeftHandData: return LeftHandComponent != nullptr;
	case LoggingTypeEnum::LT_RightHandData: return RightHandComponent != nullptr;
	default: return (RaycastComponent != nullptr || RemoteTrackerComponent != nullptr);
	}
}