// Fill out your copyright notice in the Description page of Project Settings.

#include "SynOpticonState.h"

FSettingsStruct ASynOpticonState::Settings;

int32 ASynOpticonState::CalToolForwardAxis = 0;

bool ASynOpticonState::BCreatingAOI = false;

bool ASynOpticonState::bEyeRadarVisible = false;

bool ASynOpticonState::bLabelingScreens = false;

bool ASynOpticonState::IsProcessingReplayData = false;

bool ASynOpticonState::BApplyOffset = true;
bool ASynOpticonState::BApplyDebugLines = false;


TArray<FPotentialSensorAddress> ASynOpticonState::DiscoveredIPAddresses;
TArray<FSensorData> ASynOpticonState::DiscoveredSensorsList;

bool ASynOpticonState::bHeatMapVisible = false;
float ASynOpticonState::HeatMapAlpha = 0.4f;
bool ASynOpticonState::bAOIVisible = false;
bool ASynOpticonState::bMicroTaskOverlayVisible = false;
float ASynOpticonState::AOIAlpha = 0.4f;
TArray<FHeatMapConditionStruct> ASynOpticonState::HeatMapConditions;
bool ASynOpticonState::bHeatMapFromBeginning = true;
float ASynOpticonState::fHeatMapHistoryDuration = 1.0f;
FDateTime ASynOpticonState::dHeatMapStartTime = FDateTime::UtcNow();
float ASynOpticonState::fSmoothingMomentum = 0.8f;

bool ASynOpticonState::bRaycastingEnabled = true;
int32 ASynOpticonState::ConeDivisions = 3;
float ASynOpticonState::ConeAngleDegrees = 0.5;

bool ASynOpticonState::bOrientationVectorVisible = true;
bool ASynOpticonState::bEyeVectorVisible = true;
bool ASynOpticonState::bConvergenceVectorVisible = true;
bool ASynOpticonState::bGazePointParticlesEnabled = false;
bool ASynOpticonState::bShowViewCone = false;

bool ASynOpticonState::bShowSceneCamera = true;
bool ASynOpticonState::HorizontalCoordinateSystem = false;
double ASynOpticonState::FIXATION_VELOCITY_THRESHOLD = 50; //deg/sec
double ASynOpticonState::SACCADE_VELOCITY_THRESHOLD = 150; //deg/sec
float ASynOpticonState::FIXATION_DISTANCE_THRESHOLD = 3.5;
float ASynOpticonState::FIXATION_DURATION_THRESHOLD = 0.2; //200ms
float ASynOpticonState::ONSET_THRESHOLD = -5;
float ASynOpticonState::REVERSAL_THRESHOLD = 5;
float ASynOpticonState::OFFSET_THRESHOLD = 0;

float ASynOpticonState::DOUBLE_BLINK_THRESHOLD = 400; //ms

int32 ASynOpticonState::SplitRecordingTimeThreshold = 120; //Minutes

TMap<int32, TPair<FString, bool>> ASynOpticonState::AOISessionDict;


void ASynOpticonState::AddSensorData(FSensorData SensorData) {
	DiscoveredSensorsList.Add(SensorData);
}
TArray<FSensorData> ASynOpticonState::GetSensorDataList() {
	return DiscoveredSensorsList;
}

void ASynOpticonState::AddDiscoveredIPAddress(FPotentialSensorAddress PotentialSensor) {
	bool SensorAddressInList = false;
	for (FPotentialSensorAddress ExistingAddresses : DiscoveredIPAddresses) {
		if (ExistingAddresses.SensorAddress.Equals(PotentialSensor.SensorAddress)) {
			SensorAddressInList = true;
			break;
		}
	}
	if (!SensorAddressInList) {
		UE_LOG(LogTemp, Warning, TEXT("Discovered IP address added: %s"), *PotentialSensor.SensorAddress)
			DiscoveredIPAddresses.Add(PotentialSensor);
	}
}

void ASynOpticonState::SetDiscoveredIPAddressRequest(FPotentialSensorAddress PotentialSensor, bool status) {
	for (int32 Index = 0; Index < DiscoveredIPAddresses.Num(); ++Index) {
		if (DiscoveredIPAddresses[Index].SensorAddress.Equals(PotentialSensor.SensorAddress)) {
			DiscoveredIPAddresses[Index].RequestSent = true;
			break;
		}
	}
}

TArray<FPotentialSensorAddress> ASynOpticonState::GetDiscoveredIPAddress() {
	return DiscoveredIPAddresses;
}

//AOI
void ASynOpticonState::SetCreatingAOI(bool CreatingAOI) {
	BCreatingAOI = CreatingAOI;
}

//Screen labeling
void ASynOpticonState::SetScreenLabeling(bool ScreenLabeling)
{
	bLabelingScreens = ScreenLabeling;
}

bool ASynOpticonState::GetScreenLabeling()
{
	return bLabelingScreens;
}

int32 ASynOpticonState::CreatedGazeObjects = -1;
UGlobalEventSystem* ASynOpticonState::globalEventSystem = nullptr;
UThreadPoolManager* ASynOpticonState::threadPoolManager = nullptr;

TMap<ComponentTypeEnum, TArray<FString>> ASynOpticonState::AvailableComponents;

//Heatmap Colors
FLinearColor ASynOpticonState::GetHeatMapColorFromValue(float HeatMapValue) { //TODO should be moved into the statics class instead
	TArray<FHeatMapConditionStruct> HeatMapConditionss = ASynOpticonState::GetHeatMapConditions();

	FLinearColor HMColor;

	if (HeatMapConditionss.Num() > 0)
	{
		bool ColorSet = false;
		//Blend color grades together
		for (int i = 0; i < HeatMapConditionss.Num() - 1; i++)
		{
			FHeatMapConditionStruct CurrentCondition = HeatMapConditionss[i];
			FHeatMapConditionStruct NextCondition = HeatMapConditionss[i + 1];

			if (HeatMapValue >= CurrentCondition.Value && HeatMapValue <= NextCondition.Value) {
				float newS = ((HeatMapValue - CurrentCondition.Value) / (NextCondition.Value - CurrentCondition.Value));

				if (newS > 1.0f)
				{
					newS = 1.0f;
				}

				HMColor = FMath::Lerp(CurrentCondition.Color, NextCondition.Color, newS);
				ColorSet = true;
				break;
			}
		}

		if (!ColorSet)
		{
			HMColor = HeatMapConditionss.Last().Color;
		}
	}

	return HMColor;
}

bool ASynOpticonState::IsEyeRadarVisible()
{
	return bEyeRadarVisible;
}

void ASynOpticonState::SetEyeRadarVisible(bool EyeRadarVisible)
{
	bEyeRadarVisible = EyeRadarVisible;
}

//=================================== Record status =====================================
bool ASynOpticonState::bRecording = false;
bool ASynOpticonState::IsRecording()
{
	return bRecording;
}
void ASynOpticonState::SetRecordingStatus(bool _Recording)
{
	bRecording = _Recording;

	FRecordingEventStruct* RecordingEvent = new FRecordingEventStruct();
	RecordingEvent->RecordingStatus = _Recording ? "Recording" : "Stopped Recording";
	//FWAMPWorker::PublishWAMPEvent(RecordingEvent);

	globalEventSystem->OnEventStartRecord.Broadcast(_Recording);
}

//=================================== Replay status ======================================
bool ASynOpticonState::bReplaying = false;
bool ASynOpticonState::IsReplaying()
{
	return bReplaying;
}
void ASynOpticonState::SetReplayingStatus(bool _Replaying)
{
	bReplaying = _Replaying;
	globalEventSystem->OnEventStartReplay.Broadcast(_Replaying);
}

bool ASynOpticonState::bPausingReplay = false;
bool ASynOpticonState::IsPausingReplay()
{
	return bPausingReplay;
}

void ASynOpticonState::SetPausingReplayStatus(bool _Pausing)
{
	bPausingReplay = _Pausing;
	globalEventSystem->OnEventPauseReplay.Broadcast();
}

//==============================Replay states==================================

/*-------------------*/
CompressedHeader ASynOpticonState::ReplayHeader;
FSyncReplayStruct ASynOpticonState::ReplaySyncData;
FDateTime ASynOpticonState::CurrentReplayTime = FDateTime::UtcNow();
FDateTime ASynOpticonState::GetCurrentReplayTime()
{
	if (IsReplaying())
	{
		return CurrentReplayTime;
	}
	else
	{
		return FDateTime::UtcNow();
	}
}
void ASynOpticonState::SetCurrentReplayTime(FDateTime _Time)
{
	CurrentReplayTime = _Time;
}

//SynOpticonActor Controls
int ASynOpticonState::CurrentSynOpticonActorIndex = 0;
TArray<AActor*> ASynOpticonState::SynOpticonActorList;
TArray<AActor*> ASynOpticonState::GazeActorList;
TArray<AActor*> ASynOpticonState::GazeParentActorList;
TArray<UActorComponent*> ASynOpticonState::GazeMaterialComponents;
TMap<int32, FString> ASynOpticonState::GazeGroupList;
const FString ComponentMetaDataTopic = "ComponentMetaData";
void ASynOpticonState::InitiateState(UGlobalEventSystem* _globalEventSystem, UThreadPoolManager* _threadPoolManager)
{
	globalEventSystem = _globalEventSystem;
	threadPoolManager = _threadPoolManager;
	SetDefaultHeatMap();
	SetScreenSaveRate(1, false);
	bLabelingScreens = false;

	//TSharedPtr<wamp_event_handler> ComponentMetaDataHandler(new wamp_event_handler());
	//*ComponentMetaDataHandler = [](const autobahn::wamp_event& _event) { ASynOpticonState::OnReceiveChangeOfComponent(_event); };
	//FWAMPWorker::SubscribeToTopic(ComponentMetaDataTopic, ComponentMetaDataHandler);
	
	ASynOpticonState::CalToolForwardAxis = 0;

	ASynOpticonState::BCreatingAOI = false;
	ASynOpticonState::bLabelingScreens = false;
	ASynOpticonState::IsProcessingReplayData = false;
	ASynOpticonState::BApplyOffset = true;
	ASynOpticonState::BApplyDebugLines = false;

	ASynOpticonState::bHeatMapVisible = false;
	ASynOpticonState::HeatMapAlpha = 0.4f;
	ASynOpticonState::bAOIVisible = false;
	ASynOpticonState::AOIAlpha = 0.4f;
	ASynOpticonState::bHeatMapFromBeginning = true;
	ASynOpticonState::fHeatMapHistoryDuration = 1.0f;
	ASynOpticonState::dHeatMapStartTime = FDateTime::UtcNow();
	ASynOpticonState::fSmoothingMomentum = 0.8f;

	ASynOpticonState::bRaycastingEnabled = true;
	ASynOpticonState::ConeDivisions = 3;
	ASynOpticonState::ConeAngleDegrees = 0.5;

	ASynOpticonState::bOrientationVectorVisible = true;
	ASynOpticonState::bEyeVectorVisible = true;
	ASynOpticonState::bConvergenceVectorVisible = true;
	ASynOpticonState::bGazePointParticlesEnabled = false;
	ASynOpticonState::bShowViewCone = false;

	ASynOpticonState::bShowSceneCamera = true;
	ASynOpticonState::HorizontalCoordinateSystem = false;
	ASynOpticonState::FIXATION_VELOCITY_THRESHOLD = 50; //deg/sec
	ASynOpticonState::SACCADE_VELOCITY_THRESHOLD = 150; //deg/sec
	ASynOpticonState::FIXATION_DISTANCE_THRESHOLD = 3.5;
	ASynOpticonState::FIXATION_DURATION_THRESHOLD = 0.2; //200ms
	ASynOpticonState::ONSET_THRESHOLD = -5;
	ASynOpticonState::REVERSAL_THRESHOLD = 5;
	ASynOpticonState::OFFSET_THRESHOLD = 0;

	ASynOpticonState::DOUBLE_BLINK_THRESHOLD = 400; //ms

	ASynOpticonState::SplitRecordingTimeThreshold = 120; //Minutes

	//GetGlobalEventSystem()->OnEventWAMPComponentConnected.BindDynamic(*void, &ASynOpticonState::ReconnectToRouter);
}

void ASynOpticonState::AlterSmoothingMomentum(float Value)
{
	fSmoothingMomentum += Value;

	if (fSmoothingMomentum > 0.99f)
	{
		fSmoothingMomentum = 0.99f;
	}
	else if (fSmoothingMomentum < 0.0f)
	{
		fSmoothingMomentum = 0.0f;
	}
}

void ASynOpticonState::ApplyOffset(bool ApplyOffset) {
	BApplyOffset = ApplyOffset;

	FString message = ApplyOffset ? "Applying Offset" : "Stopped Applying Offset";

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, message);
	}
}

bool ASynOpticonState::ShouldApplyOffset() {
	return BApplyOffset;
}

void ASynOpticonState::ShowDebugLines(bool ShowDebugLines) {
	BApplyDebugLines = ShowDebugLines;

	FString message = BApplyDebugLines ? "Showing debug lines" : "Stopped Showing debug lines";

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, message);
	}
}

bool ASynOpticonState::IsShowingDebugLines() {
	return BApplyDebugLines;
}

bool ASynOpticonState::IsHeatMapVisible()
{
	return bHeatMapVisible;
}

void ASynOpticonState::ToggleHeatMapVisibility()
{
	bHeatMapVisible = !bHeatMapVisible;
}

bool ASynOpticonState::IsAOIVisible()
{
	return bAOIVisible;
}

void ASynOpticonState::ToggleAOIVisibility()
{
	bAOIVisible = !bAOIVisible;
}

bool ASynOpticonState::IsMicroTaskOverlayVisible()
{
	return bMicroTaskOverlayVisible;
}

void ASynOpticonState::ToggleMicroTaskOverlayVisibility()
{
	bMicroTaskOverlayVisible = !bMicroTaskOverlayVisible;
}

void ASynOpticonState::ToggleRaycasting()
{
	bRaycastingEnabled = !bRaycastingEnabled;
}

void ASynOpticonState::SetConvergenceVectorVisibility(bool Visibility)
{
	bConvergenceVectorVisible = Visibility;
}

void ASynOpticonState::SetOrientationVectorVisibility(bool Visibility)
{
	bOrientationVectorVisible = Visibility;
}

void ASynOpticonState::SetEyeVectorVisibility(bool Visibility)
{
	bEyeVectorVisible = Visibility;
}

void ASynOpticonState::ToggleGazePointParticlesVisibility()
{
	bGazePointParticlesEnabled = !bGazePointParticlesEnabled;
}

bool ASynOpticonState::IsShowingViewCone()
{
	return bShowViewCone;
}

void ASynOpticonState::ToggleShowingViewCone()
{
	bShowViewCone = !bShowViewCone;
}

void ASynOpticonState::SetHeatMapConditions(TArray<FHeatMapConditionStruct> NewHeatMapConditions)
{
	HeatMapConditions.Empty();
	HeatMapConditions = NewHeatMapConditions;
}

void ASynOpticonState::SetDefaultHeatMap()
{
	HeatMapConditions.Empty();
	FHeatMapConditionStruct Condition1(FLinearColor(150.0 / 255.0, 150.0 / 255.0, 150.0 / 255.0, 1), 0);
	FHeatMapConditionStruct Condition2(FLinearColor(255.0 / 255.0, 255.0 / 255.0, 50.0 / 255.0, 1), 0.1);
	FHeatMapConditionStruct Condition3(FLinearColor(255.0 / 255.0, 165.0 / 255.0, 0.0 / 255.0, 1), 0.2);
	FHeatMapConditionStruct Condition4(FLinearColor(255.0 / 255.0, 10.0 / 255.0, 10.0 / 255.0, 1),0.3);
	HeatMapConditions.Add(Condition1);
	HeatMapConditions.Add(Condition2);
	HeatMapConditions.Add(Condition3);
	HeatMapConditions.Add(Condition4);
}

void ASynOpticonState::SetHeatMapHistoryDuration(float HeatMapHistoryDuration)
{
	if (HeatMapHistoryDuration == 0.0f)
	{
		bHeatMapFromBeginning = true;
	}
	else
	{
		bHeatMapFromBeginning = false;
	}

	fHeatMapHistoryDuration = HeatMapHistoryDuration;
}

void ASynOpticonState::SetStartTimeToNow()
{
	dHeatMapStartTime = FDateTime::UtcNow();
}

void ASynOpticonState::ShowSceneCamera(bool ShowSceneCamera)
{
	bShowSceneCamera = ShowSceneCamera;
}

void ASynOpticonState::SetHorizontalCoordinateSystem(bool _Horizontal)
{
	HorizontalCoordinateSystem = _Horizontal;
}

FVector ASynOpticonState::GetGroundPointLocation()
{
	return USynOpticonStatics::ConvertFromMotiveToUnrealEngineCoordinateSystem(FVector(0, 0, 0),
		ASynOpticonState::GetTrackerOffset(), ASynOpticonState::IsHorizontalCoordinateSystem(),
		ASynOpticonState::GetPosTrackingUnitToUnrealUnitFactor());
}

FRotator ASynOpticonState::GetGroundPointRotation()
{
	if (ASynOpticonState::IsHorizontalCoordinateSystem())
	{
		return FRotator::MakeFromEuler(FVector(0, 0, 0));
	}
	else
	{
		return FRotator::MakeFromEuler(FVector(0, 0, 90));//FRotator::MakeFromEuler(USynOpticonStatics::ConvertFromMotiveToUnrealEngineCoordinateSystem(FVector(-180, -90, 0), SettingsManager::GetInstance()->GetTrackerOffset(), false,1));
	}
}

void ASynOpticonState::SetTrackerOffset(FVector _Offset)
{
	Settings.MotiveOffset = _Offset;
}

void ASynOpticonState::SetGlassesOffset(FVector _Offset)
{
	Settings.ETOffset = _Offset;
}

FVector ASynOpticonState::GetTrackerOffset()
{
	return Settings.MotiveOffset;
}

FVector ASynOpticonState::GetGlassesOffset()
{
	return Settings.ETOffset;
}

int32 ASynOpticonState::GetScreenSaveRateFPS()
{
	return Settings.SaveScreenRate;
}

float ASynOpticonState::GetScreenSaveRate()
{
	return Settings.SaveRate;
}

void ASynOpticonState::SetScreenSaveRate(int32 FPS, bool Seconds)
{
	if (FPS == 0)
	{
		Settings.SaveScreenRate = 1;
		Settings.SaveRate = 1;

		if (!Seconds)
		{
			Settings.SaveRate *= 60;
		}
	}
	else
	{
		Settings.SaveScreenRate = FPS;
		Settings.SaveRate = 1.0f / (float)Settings.SaveScreenRate;

		if (!Seconds)
		{
			Settings.SaveRate *= 60;
		}
	}
	Settings.bSaveRatePerMinute = !Seconds;
}

bool ASynOpticonState::IsScreenSaveRatePerSeconds()
{
	return !Settings.bSaveRatePerMinute;
}

FString ASynOpticonState::GetWAMPRouterAdress()
{
	return Settings.CrossbarAdress;
}

void ASynOpticonState::SetWAMPRouterAdress(FString adress)
{
	Settings.CrossbarAdress = adress;
}

FString ASynOpticonState::GetWAMPRealm()
{
	return Settings.CrossbarRealm;
}

void ASynOpticonState::SetWAMPRealm(FString realm)
{
	Settings.CrossbarRealm = realm;
}


FSettingsStruct ASynOpticonState::GetSettings()
{
	return Settings;
}

void ASynOpticonState::ApplySettings(FSettingsStruct _Settings)
{
	Settings = _Settings;
}

int32 ASynOpticonState::IncrementCreatedGazeObjects() {
	CreatedGazeObjects++;
	return CreatedGazeObjects;
}

void ASynOpticonState::CleanupState() {
	/*GlobalEventSystem->OnEventUpdateServerAdress.Clear();
	GlobalEventSystem->OnEventGazeGesture.Clear();
	ASynOpticonState::GlobalEventSystem->ConditionalBeginDestroy();*/
	globalEventSystem = nullptr;
	threadPoolManager = nullptr;

	SynOpticonActorList.Empty();
	GazeActorList.Empty();
	GazeParentActorList.Empty();
	GazeMaterialComponents.Empty();

	DiscoveredIPAddresses.Empty();
	DiscoveredSensorsList.Empty();
}

UGlobalEventSystem* ASynOpticonState::GetGlobalEventSystem()
{
	return globalEventSystem;
}

UThreadPoolManager* ASynOpticonState::GetThreadPoolManager()
{
	return threadPoolManager;
}

bool ASynOpticonState::ToggleVectorsVisibility(VectorVisibilityEnum _Type)
{
	switch (_Type)
	{
	case VectorVisibilityEnum::VE_OrientationVector: bOrientationVectorVisible = !bOrientationVectorVisible; 
		globalEventSystem->OnEventToggleVectorsVisibility.Broadcast(_Type); return bOrientationVectorVisible;
	case VectorVisibilityEnum::VE_EyeVector: bEyeVectorVisible = !bEyeVectorVisible;  return bEyeVectorVisible;
	case VectorVisibilityEnum::VE_ConvergenceVector: bConvergenceVectorVisible = !bConvergenceVectorVisible; return bConvergenceVectorVisible;
	}
	return false;
}

bool ASynOpticonState::IsVectorVisible(VectorVisibilityEnum _Type)
{
	switch (_Type)
	{
	case VectorVisibilityEnum::VE_OrientationVector: return bOrientationVectorVisible;
	case VectorVisibilityEnum::VE_EyeVector: return bEyeVectorVisible;
	case VectorVisibilityEnum::VE_ConvergenceVector: return bConvergenceVectorVisible;
	}
	return false;
}


//playing states
FDateTime ASynOpticonState::GetStartTime() { 
	return dHeatMapStartTime;
}

//==============================Available Components===============================

TArray<FString> ASynOpticonState::GetAvailableComponents(ComponentTypeEnum Type)
{
	TArray<FString>* ComponentArray = AvailableComponents.Find(Type);
	if (ComponentArray)
	{
		return *ComponentArray;
	}
	return TArray<FString>();
}

void ASynOpticonState::AddComponentToMap(ComponentTypeEnum Type, FString name)
{
	if (AvailableComponents.Contains(Type))
	{
		TArray<FString>* list = AvailableComponents.Find(Type);
		if (!list->Contains(name))
		{
			list->Add(name);
		}
	}
	else
	{
		TArray<FString> list;
		list.Add(name);
		AvailableComponents.Add(Type, list);
	}
}

void ASynOpticonState::OnReceiveChangeOfComponent(const string evt)
{
	//FString evt_type = FString(evt.argument<std::string>(0).c_str());
	//std::array<object, 2> componentInfo = evt.argument<std::array<object, 2>>(1);
	//FString name = FString(componentInfo[0].as<std::string>().c_str());
	//FString type = FString(componentInfo[1].as<std::string>().c_str());
	//
	//ComponentTypeEnum Type = FromFString(type);
	//
	//if (evt_type.Equals("add"))
	//{
	//	AddComponentToMap(Type, name);
	//	GetGlobalEventSystem()->OnEventWAMPComponentConnected.Broadcast(Type, name);
	//}
	//else //remove
	//{
	//	if (AvailableComponents.Contains(Type))
	//	{
	//		TArray<FString>* list = AvailableComponents.Find(Type);
	//		if (list->Contains(name))
	//		{
	//			list->Remove(name);
	//		}
	//	}
	//	GetGlobalEventSystem()->OnEventWAMPComponentDisconnected.Broadcast(Type, name);
	//}
}

void ASynOpticonState::InitializeAvailableComponents(string result)
{
	//std::list<object> receivedData = result.argument<std::list<object>>(0);
	//for (object component : receivedData)
	//{
	//	std::array<object, 2> details = component.as<std::array<object, 2>>();

	//	std::array<object, 2> componentInfo = details[0].as<std::array<object, 2>>();
	//	FString name = FString(componentInfo[0].as<std::string>().c_str());
	//	FString type = FString(componentInfo[1].as<std::string>().c_str());
	//	ComponentTypeEnum Type = FromFString(type);

	//	AddComponentToMap(Type, name);
	//}
}


//================== SynOpticon Actor Control ================================
void ASynOpticonState::NextSynOpticonActor()
{
	CurrentSynOpticonActorIndex++;
	if (CurrentSynOpticonActorIndex >= SynOpticonActorList.Num())
	{
		CurrentSynOpticonActorIndex = 0;
	}

	globalEventSystem->OnSOActorChanged.Broadcast();
}

void ASynOpticonState::PreviousSynOpticonActor()
{
	CurrentSynOpticonActorIndex--;
	if (SynOpticonActorList.Num() > 1)
	{
		if (CurrentSynOpticonActorIndex < 0)
		{
			CurrentSynOpticonActorIndex = SynOpticonActorList.Num() - 1;
		}
	}
	else
	{
		CurrentSynOpticonActorIndex = 0;
	}

	globalEventSystem->OnSOActorChanged.Broadcast();
}

AActor* ASynOpticonState::GetCurrentActor()
{
	if (SynOpticonActorList.Num() > 0 && CurrentSynOpticonActorIndex < SynOpticonActorList.Num())
	{
		return SynOpticonActorList[CurrentSynOpticonActorIndex];
	}
	else
	{
		return nullptr;
	}
}

int ASynOpticonState::GetNumberOfSOActors()
{
	return SynOpticonActorList.Num();
}

void ASynOpticonState::AddSOActor(AActor* SOActor) {
	SynOpticonActorList.Add(SOActor);
}

void ASynOpticonState::RemoveSOActor(AActor* SOActor) {
	SynOpticonActorList.Remove(SOActor);
}

void ASynOpticonState::ClearSOActorList() {
	SynOpticonActorList.Empty();
}

void ASynOpticonState::AddGazeActor(AActor* GazeActor) {
	GazeActorList.Add(GazeActor);
}

void ASynOpticonState::RemoveGazeActor(AActor* GazeActor) {
	GazeActorList.Remove(GazeActor);
}

void ASynOpticonState::ClearGazeActorList() {
	GazeActorList.Empty();
}

void ASynOpticonState::AddGazeParentActor(AActor* GazeParentActor) {
	GazeParentActorList.Add(GazeParentActor);
}

void ASynOpticonState::RemoveGazeParentActor(AActor* GazeParentActor) {
	GazeParentActorList.Remove(GazeParentActor);
}

void ASynOpticonState::ClearGazeParentActorList() {
	GazeParentActorList.Empty();
}

void ASynOpticonState::AddMaterialComponent(UActorComponent* MaterialComponent) {
	GazeMaterialComponents.Add(MaterialComponent);
}

void ASynOpticonState::RemoveMaterialComponent(int32 Index) {
	GazeMaterialComponents.RemoveAtSwap(Index);
}

void ASynOpticonState::ClearMaterialComponentList() {
	GazeMaterialComponents.Empty();
}

void ASynOpticonState::SetSplitRecordingTimeThreshold(int32 minutes) {
	SplitRecordingTimeThreshold = minutes;
}

int32 ASynOpticonState::GetSplitRecordingTimeThreshold() {
	return SplitRecordingTimeThreshold;
}

int32 ASynOpticonState::GetCalToolForwardAxis() {
	return CalToolForwardAxis;
}

void ASynOpticonState::SetCalToolForwardAxis(int32 ForwardAxis) {
	CalToolForwardAxis = ForwardAxis;
}

FString ASynOpticonState::GetGazeGroup(int32 ID)
{
	FString* GroupName = GazeGroupList.Find(ID);
	if(GroupName)
	{
		return *GroupName;
	}
	return "None";
}

int32 ASynOpticonState::GetGazeGroupID(FString GroupName)
{
	const int32* ID = GazeGroupList.FindKey(GroupName);
	if (ID)
		return *ID;
	return -1;
}

TArray<FGazeGroupData> ASynOpticonState::GetGazeGroupData()
{
	TArray<FGazeGroupData> Result;
	for (TPair<int32, FString> Pair : GazeGroupList)
	{
		Result.Add(FGazeGroupData(Pair.Value, Pair.Key));
	}
	return Result;
}

TArray<FString> ASynOpticonState::GetGazeGroupList()
{
	TArray<FString> Groups;
	GazeGroupList.GenerateValueArray(Groups);
	return Groups;
}

void ASynOpticonState::AddGazeGroup(FString GazeGroup, int32 ID)
{
	if (ID < -1)
	{
		GazeGroupList.Add(ID, GazeGroup);
	}
	else
	{
		int32 Index = -2;
		while (GazeGroupList.Contains(Index))
		{
			Index--;
		}
		GazeGroupList.Add(Index, GazeGroup);
	}
}

void ASynOpticonState::RemoveGazeGroup(FString GazeGroup)
{
	for (TPair<int32, FString>& Pair: GazeGroupList)
	{
		if (Pair.Value.Equals(GazeGroup)) 
		{
			GetGlobalEventSystem()->OnRemoveGazeGroupDelegate.Broadcast(Pair.Key);
			GazeGroupList.Remove(Pair.Key);
			return;
		}
	}
}

void ASynOpticonState::EditGazeGroup(FString NewGazeGroupName, FString OldGazeGroupName)
{
	for (TPair<int32, FString>& Pair : GazeGroupList)
	{
		if (Pair.Value.Equals(OldGazeGroupName))
		{
			GazeGroupList[Pair.Key] = NewGazeGroupName;
			return;
		}
	}
}

void ASynOpticonState::ClearGazeGroup()
{
	for (TPair<int32, FString>& Pair : GazeGroupList)
	{
		GetGlobalEventSystem()->OnRemoveGazeGroupDelegate.Broadcast(Pair.Key);
	}
	GazeGroupList.Empty();
}

void ASynOpticonState::SetConeDivisions(int32 _ConeDivisions) {
	ConeDivisions = _ConeDivisions;
}

void ASynOpticonState::SetConeAngle(float _ConeAngleDegrees) {
	ConeAngleDegrees = _ConeAngleDegrees;
}