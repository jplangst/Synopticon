// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ReplayDataStructures.h"
#include "Experiment/ExperimentDataStructure.h"
#include "GazeObjects/GazeActorJSONExporter.h"
#include "SynOpticonActors/SynOpticonActorJSONExporter.h"
#include "Experiment/ExperimentDataJSONExporter.h"

#include "ReplayHeaderJSONExporter.generated.h"

/**
 * 
 */
UCLASS()
class SYNOPTICON_API UReplayHeaderJSONExporter : public UObject
{
	GENERATED_UCLASS_BODY()
public:
	static TSharedPtr<FJsonObject> SaveHeaderData(CompressedHeader HeaderData);
	static TSharedPtr<FJsonObject> SaveReplayEventStruct(FReplayEventStruct ReplayEventStruct);
	static TSharedPtr<FJsonObject> SaveTimeIndexStruct(TimeIndexStruct TimeIndexStruct);
	static TSharedPtr<FJsonObject> SaveSettingsStruct(FSettingsStruct SettingsStruct);
	static TSharedPtr<FJsonObject> SaveMicroTaskEvent(TPair<MicroTaskEventTypeEnum, FActorTaskStruct> MT);
	static TArray<TSharedPtr<FJsonValue>> SaveMicroTaskSubTasks(TArray<FSubTaskStruct> SubTaskList);
	static TArray<TSharedPtr<FJsonValue>> SaveChangeImageAndAOIs(TArray<TPair<FDateTime, FImageAndAOIs>> ChangeImageEvents);
	static TArray<TSharedPtr<FJsonValue>> SavePopupEvents(TArray<TPair<FDateTime, FPopupAndAOIs>> PopupEvents);

	static CompressedHeader GetHeaderData(TSharedPtr<FJsonObject> HeaderJsonObj);
	static TArray<TPair<MicroTaskEventTypeEnum, FActorTaskStruct>> GetMicroTaskEvents(TArray<TSharedPtr<FJsonValue>> MicroTasksObject);
	static TPair<MicroTaskEventTypeEnum, FActorTaskStruct> GetMicroTaskEvent(TSharedPtr<FJsonObject> MicroTaskEventObject);
	static TArray<FSubTaskStruct> GetMicroTaskSubTasks(TArray<TSharedPtr<FJsonValue>> SubTaskListObj);
	static FReplayEventStruct GetReplayEventStruct(TSharedPtr<FJsonObject> ReplayEventStructObj);
	static TimeIndexStruct GetTimeIndexStruct(TSharedPtr<FJsonObject> TimeIndexStructObj);
	static FSettingsStruct GetSettingsStruct(TSharedPtr<FJsonObject> SettingsStructObj);
	static TArray<TPair<FDateTime, FImageAndAOIs>> GetChangeImageEvents(TArray< TSharedPtr<FJsonValue> > ChangeImageEventsObj);
	static TArray<TPair<FDateTime, FPopupAndAOIs>> GetPopupEvents(TArray< TSharedPtr<FJsonValue> > PopupEventsObj);
};
