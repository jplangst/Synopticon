// Fill out your copyright notice in the Description page of Project Settings.

#include "ReplayHeaderJSONExporter.h"
//#include "Synopticon.h"


UReplayHeaderJSONExporter::UReplayHeaderJSONExporter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSharedPtr<FJsonObject> UReplayHeaderJSONExporter::SaveHeaderData(CompressedHeader HeaderData)
{
	TSharedPtr<FJsonObject> HeaderJson = MakeShareable(new FJsonObject);

	TArray<TSharedPtr<FJsonValue>> GazeActorDataList;
	for (FGazeActorData GA : HeaderData.GazeActorDataList)
	{
		GazeActorDataList.Add(MakeShareable(new FJsonValueObject(UGazeActorJSONExporter::SaveGazeActorData(GA))));
	}
	HeaderJson->SetArrayField("GazeActorDataList", GazeActorDataList);

	TArray<TSharedPtr<FJsonValue>> GazeGroupList;
	for (FGazeGroupData GG : HeaderData.GazeGroupList)
	{
		GazeGroupList.Add(MakeShareable(new FJsonValueObject(UGazeActorJSONExporter::SaveGazeGroupData(GG))));
	}
	HeaderJson->SetArrayField("GazeGroupList", GazeGroupList);

	TArray<TSharedPtr<FJsonValue>> SynOpticonActorDataList;
	for (FSynOpticonActorStruct SA : HeaderData.SynOpticonActorDataList)
	{
		SynOpticonActorDataList.Add(MakeShareable(new FJsonValueObject(USynOpticonActorJSONExporter::SaveSynOpticonActorData(SA))));
	}
	HeaderJson->SetArrayField("SynOpticonActorDataList", SynOpticonActorDataList);

	TArray<TSharedPtr<FJsonValue>> ReplayEvents;
	for (FReplayEventStruct ES : HeaderData.ReplayEvents)
	{
		ReplayEvents.Add(MakeShareable(new FJsonValueObject(SaveReplayEventStruct(ES))));
	}
	HeaderJson->SetArrayField("ReplayEvents", ReplayEvents);

	HeaderJson->SetNumberField("StartTimestamp", HeaderData.StartTimestamp.ToUnixTimestamp());

	TArray<TSharedPtr<FJsonValue>> LogFileTimestamps;
	for (FDateTime T : HeaderData.LogFileTimestamps)
	{
		LogFileTimestamps.Add(MakeShareable(new FJsonValueString(T.ToString())));
	}
	HeaderJson->SetArrayField("LogFileTimestamps", LogFileTimestamps);

	TArray<TSharedPtr<FJsonValue>> TimeIndex;
	for (TArray<TimeIndexStruct> TIA : HeaderData.TimeIndex)
	{
		TArray<TSharedPtr<FJsonValue>> TimeIndexTemp;
		for (TimeIndexStruct TI : TIA)
		{
			TimeIndexTemp.Add(MakeShareable(new FJsonValueObject(SaveTimeIndexStruct(TI))));
		}
		TimeIndex.Add(MakeShareable(new FJsonValueArray(TimeIndexTemp)));
	}
	HeaderJson->SetArrayField("TimeIndex", TimeIndex);

	HeaderJson->SetNumberField("ReplayTimeInSeconds", HeaderData.ReplayTimeInSeconds);
	HeaderJson->SetNumberField("NumOfLogFiles", HeaderData.NumOfLogFiles);

	HeaderJson->SetObjectField("Settings", SaveSettingsStruct(HeaderData.Settings));

	HeaderJson->SetArrayField("ChangeImageEvents", SaveChangeImageAndAOIs(HeaderData.ChangeImageEvents));
	HeaderJson->SetArrayField("PopupEvents", SavePopupEvents(HeaderData.PopupEvents));

	TArray<TSharedPtr<FJsonValue>> AOIHistory;
	for (FAOIData aoi : HeaderData.AOIHistory)
	{
		AOIHistory.Add(MakeShareable(new FJsonValueObject(UGazeActorJSONExporter::SaveAOIData(aoi))));
	}
	HeaderJson->SetArrayField("AOIHistory", AOIHistory);

	TArray<TSharedPtr<FJsonValue>> PopupHistory;
	for (FAOIData popup : HeaderData.PopupHistory)
	{
		PopupHistory.Add(MakeShareable(new FJsonValueObject(UGazeActorJSONExporter::SaveAOIData(popup))));
	}
	HeaderJson->SetArrayField("PopupHistory", PopupHistory);

	TArray<TSharedPtr<FJsonValue>> MicroTaskEvents;
	for (TPair<MicroTaskEventTypeEnum, FActorTaskStruct> MT : HeaderData.MicroTaskEvents)
	{
		MicroTaskEvents.Add(MakeShareable(new FJsonValueObject(SaveMicroTaskEvent(MT))));
	}
	HeaderJson->SetArrayField("MicroTaskEvents", MicroTaskEvents);

	return HeaderJson;
}

TSharedPtr<FJsonObject> UReplayHeaderJSONExporter::SaveMicroTaskEvent(TPair<MicroTaskEventTypeEnum, FActorTaskStruct> MT) {
	FActorTaskStruct TaskStruct = MT.Value;
	TSharedPtr<FJsonObject> MicroTaskEventJson = MakeShareable(new FJsonObject);
	MicroTaskEventJson->SetStringField("Type", MicroTaskEventToFString(MT.Key));
	MicroTaskEventJson->SetNumberField("TaskId", TaskStruct.TaskID);
	MicroTaskEventJson->SetStringField("TaskName", TaskStruct.TaskName);
	MicroTaskEventJson->SetStringField("ActorName", TaskStruct.ActorName);
	MicroTaskEventJson->SetNumberField("StartTimestamp", TaskStruct.StartTimestamp.ToUnixTimestamp());
	MicroTaskEventJson->SetNumberField("EndTimestamp", TaskStruct.EndTimestamp.ToUnixTimestamp());
	MicroTaskEventJson->SetBoolField("Complete", TaskStruct.Complete);
	MicroTaskEventJson->SetArrayField("SubTasks", SaveMicroTaskSubTasks(TaskStruct.SubTaskList));
	return MicroTaskEventJson;
}

TArray<TSharedPtr<FJsonValue>> UReplayHeaderJSONExporter::SaveMicroTaskSubTasks(TArray<FSubTaskStruct> SubTaskList) {
	TArray<TSharedPtr<FJsonValue>> MicroTaskSubTasks;
	for (FSubTaskStruct SubTask : SubTaskList)
	{
		TSharedPtr<FJsonObject> SubTaskJson;
		SubTaskJson->SetStringField("AOIName", SubTask.AOIName);
		SubTaskJson->SetBoolField("Complete", SubTask.Complete);
		MicroTaskSubTasks.Add(MakeShareable(new FJsonValueObject(SubTaskJson)));
	}
	return MicroTaskSubTasks;
}

TSharedPtr<FJsonObject> UReplayHeaderJSONExporter::SaveReplayEventStruct(FReplayEventStruct ReplayEventStruct)
{
	TSharedPtr<FJsonObject> ReplayEventJson = MakeShareable(new FJsonObject);
	ReplayEventJson->SetStringField("Name", ReplayEventStruct.Name);
	ReplayEventJson->SetStringField("Description", ReplayEventStruct.Description);
	ReplayEventJson->SetStringField("SessionID", ReplayEventStruct.SessionID);
	ReplayEventJson->SetNumberField("StartTimestamp", ReplayEventStruct.StartTimestamp.GetTotalMilliseconds());
	ReplayEventJson->SetNumberField("EndTimestamp", ReplayEventStruct.EndTimestamp.GetTotalMilliseconds());

	return ReplayEventJson;
}

TSharedPtr<FJsonObject> UReplayHeaderJSONExporter::SaveTimeIndexStruct(TimeIndexStruct TimeIndexStruct)
{
	TSharedPtr<FJsonObject> TimeIndexJson = MakeShareable(new FJsonObject);

	TimeIndexJson->SetNumberField("TimeStamp", TimeIndexStruct.TimeStamp.ToUnixTimestamp());
	TimeIndexJson->SetNumberField("PlayIndex", TimeIndexStruct.PlayIndex);
	TimeIndexJson->SetNumberField("FileIndex", TimeIndexStruct.FileIndex);
	TimeIndexJson->SetNumberField("HasGAKeyFrame", TimeIndexStruct.HasGAKeyFrame);
	TimeIndexJson->SetNumberField("LastGAKeyFrameIndex", TimeIndexStruct.LastGAKeyFrameIndex);

	return TimeIndexJson;
}

TSharedPtr<FJsonObject> UReplayHeaderJSONExporter::SaveSettingsStruct(FSettingsStruct SettingsStruct)
{
	TSharedPtr<FJsonObject> SettingsJson = MakeShareable(new FJsonObject);

	SettingsJson->SetObjectField("MotiveOffset", UJSONExporter::SaveFVectorAsJSON(SettingsStruct.MotiveOffset));
	SettingsJson->SetObjectField("ETOffset", UJSONExporter::SaveFVectorAsJSON(SettingsStruct.ETOffset));
	SettingsJson->SetNumberField("PosTrackingUnitToUnrealUnitFactor", SettingsStruct.PosTrackingUnitToUnrealUnitFactor);
	SettingsJson->SetNumberField("EyesTrackingUnitToUnrealUnitFactor", SettingsStruct.EyesTrackingUnitToUnrealUnitFactor);
	SettingsJson->SetNumberField("SaveRate", SettingsStruct.SaveRate);
	SettingsJson->SetNumberField("SaveScreenRate", SettingsStruct.SaveScreenRate);
	SettingsJson->SetBoolField("bSaveRatePerMinute", SettingsStruct.bSaveRatePerMinute);
	SettingsJson->SetStringField("CrossbarAdress", SettingsStruct.CrossbarAdress);

	return SettingsJson;
}

TArray<TSharedPtr<FJsonValue>> UReplayHeaderJSONExporter::SaveChangeImageAndAOIs(TArray<TPair<FDateTime, FImageAndAOIs>> ChangeImageEvents)
{
	TArray<TSharedPtr<FJsonValue>> ChangeImageEventsObj;
	for (TPair<FDateTime, FImageAndAOIs> ImageAndAOIsPair : ChangeImageEvents)
	{
		TSharedPtr<FJsonObject> ImageAndAOIsPairObj = MakeShareable(new FJsonObject);
		ImageAndAOIsPairObj->SetNumberField("Timestamp", ImageAndAOIsPair.Key.ToUnixTimestamp());
		ImageAndAOIsPairObj->SetObjectField("ImageAndAOIsEvent", UExperimentDataJSONExporter::SaveImageAndAOIs(ImageAndAOIsPair.Value));

		ChangeImageEventsObj.Add(MakeShareable(new FJsonValueObject(ImageAndAOIsPairObj)));
	}
	return ChangeImageEventsObj;
}

TArray<TSharedPtr<FJsonValue>> UReplayHeaderJSONExporter::SavePopupEvents(TArray<TPair<FDateTime, FPopupAndAOIs>> PopupEvents)
{
	TArray<TSharedPtr<FJsonValue>> PopupEventsObj;
	for (TPair<FDateTime, FPopupAndAOIs> PopupAndAOIsPair : PopupEvents)
	{
		TSharedPtr<FJsonObject> PopupAndAOIsPairObj = MakeShareable(new FJsonObject);
		PopupAndAOIsPairObj->SetNumberField("Timestamp", PopupAndAOIsPair.Key.ToUnixTimestamp());
		PopupAndAOIsPairObj->SetObjectField("PopupEvent", UExperimentDataJSONExporter::SavePopupAndAOIs(PopupAndAOIsPair.Value));

		PopupEventsObj.Add(MakeShareable(new FJsonValueObject(PopupAndAOIsPairObj)));
	}
	return PopupEventsObj;
}

//----------------------------------------------GETTERS------------------------------------------------

CompressedHeader UReplayHeaderJSONExporter::GetHeaderData(TSharedPtr<FJsonObject> HeaderJsonObj)
{
	CompressedHeader HeaderData;
	TArray< TSharedPtr<FJsonValue> > GazeActorDataList = HeaderJsonObj->GetArrayField(TEXT("GazeActorDataList"));
	for (TSharedPtr<FJsonValue> GazeActorData : GazeActorDataList)
	{
		TSharedPtr<FJsonObject> GazeActorDataObj = GazeActorData->AsObject();
		HeaderData.GazeActorDataList.Add(UGazeActorJSONExporter::GetGazeActorData(GazeActorDataObj));
	}

	TArray< TSharedPtr<FJsonValue> > GazeGroupList = HeaderJsonObj->GetArrayField(TEXT("GazeGroupList"));
	for (TSharedPtr<FJsonValue> GazeGroupData : GazeGroupList)
	{
		TSharedPtr<FJsonObject> GazeGroupDataObj = GazeGroupData->AsObject();
		HeaderData.GazeGroupList.Add(UGazeActorJSONExporter::GetGazeGroupData(GazeGroupDataObj));
	}

	TArray< TSharedPtr<FJsonValue> > SynOpticonActorDataList = HeaderJsonObj->GetArrayField(TEXT("SynOpticonActorDataList"));
	for (TSharedPtr<FJsonValue> SynOpticonActorData : SynOpticonActorDataList)
	{
		TSharedPtr<FJsonObject> SynOpticonActorDataObj = SynOpticonActorData->AsObject();
		HeaderData.SynOpticonActorDataList.Add(USynOpticonActorJSONExporter::GetSynOpticonActorData(SynOpticonActorDataObj));
	}

	TArray< TSharedPtr<FJsonValue> > ReplayEvents = HeaderJsonObj->GetArrayField(TEXT("ReplayEvents"));
	for (TSharedPtr<FJsonValue> ReplayEvent : ReplayEvents)
	{
		TSharedPtr<FJsonObject> ReplayEventObj = ReplayEvent->AsObject();
		HeaderData.ReplayEvents.Add(GetReplayEventStruct(ReplayEventObj));
	}

	double StartTimestampUnix;
	if(HeaderJsonObj->TryGetNumberField("StartTimestamp", StartTimestampUnix))
	{ 
		HeaderData.StartTimestamp = FDateTime::FromUnixTimestamp((int)StartTimestampUnix);
	}
	else
	{
		FString StartTimestamp = HeaderJsonObj->GetStringField(TEXT("StartTimestamp"));
		FDateTime::Parse(StartTimestamp, HeaderData.StartTimestamp);
	}
	
	TArray< TSharedPtr<FJsonValue> > LogFileTimestamps = HeaderJsonObj->GetArrayField(TEXT("LogFileTimestamps"));
	for (TSharedPtr<FJsonValue> LogFileTimestamp : LogFileTimestamps)
	{
		FDateTime Timestamp;
		FDateTime::Parse(LogFileTimestamp->AsString(), Timestamp);
		HeaderData.LogFileTimestamps.Add(Timestamp);
	}
	
	TArray< TSharedPtr<FJsonValue> > TimeIndex = HeaderJsonObj->GetArrayField(TEXT("TimeIndex"));
	for (TSharedPtr<FJsonValue> TI : TimeIndex)
	{
		TArray<TimeIndexStruct> TempStruct;
		for (TSharedPtr<FJsonValue> TITemp : TI->AsArray())
		{
			TSharedPtr<FJsonObject> TIObj = TITemp->AsObject();
			TempStruct.Add(GetTimeIndexStruct(TIObj));
		}
		HeaderData.TimeIndex.Add(TempStruct);
	}
	
	HeaderData.ReplayTimeInSeconds = HeaderJsonObj->GetIntegerField(TEXT("ReplayTimeInSeconds"));
	HeaderData.NumOfLogFiles = HeaderJsonObj->GetIntegerField(TEXT("NumOfLogFiles"));
	HeaderData.Settings = GetSettingsStruct(HeaderJsonObj->GetObjectField("Settings"));

	HeaderData.ChangeImageEvents = GetChangeImageEvents(HeaderJsonObj->GetArrayField(TEXT("ChangeImageEvents")));
	HeaderData.PopupEvents = GetPopupEvents(HeaderJsonObj->GetArrayField(TEXT("PopupEvents")));

	HeaderData.MicroTaskEvents = GetMicroTaskEvents(HeaderJsonObj->GetArrayField(TEXT("MicroTaskEvents")));

	TArray< TSharedPtr<FJsonValue> > AOIHistory = HeaderJsonObj->GetArrayField(TEXT("AOIHistory"));
	for (TSharedPtr<FJsonValue> aoi : AOIHistory)
	{
		TSharedPtr<FJsonObject> aoiObj = aoi->AsObject();
		HeaderData.AOIHistory.Add(UGazeActorJSONExporter::GetAOIData(aoiObj));
	}

	TArray< TSharedPtr<FJsonValue> > PopupHistory = HeaderJsonObj->GetArrayField(TEXT("PopupHistory"));
	for (TSharedPtr<FJsonValue> popup : PopupHistory)
	{
		TSharedPtr<FJsonObject> popupObj = popup->AsObject();
		HeaderData.PopupHistory.Add(UGazeActorJSONExporter::GetAOIData(popupObj));
	}

	return HeaderData;
}

TArray<TPair<MicroTaskEventTypeEnum, FActorTaskStruct>> UReplayHeaderJSONExporter::GetMicroTaskEvents(TArray<TSharedPtr<FJsonValue>> MicroTasksObject) {
	TArray<TPair<MicroTaskEventTypeEnum, FActorTaskStruct>> MicroTaskEvents;

	for (TSharedPtr<FJsonValue> MicroTaskValue : MicroTasksObject) {
		TSharedPtr<FJsonObject> MicroTaskObject = MicroTaskValue->AsObject();
		MicroTaskEvents.Add(GetMicroTaskEvent(MicroTaskObject));
	}

	return MicroTaskEvents;
}

TPair<MicroTaskEventTypeEnum, FActorTaskStruct> UReplayHeaderJSONExporter::GetMicroTaskEvent(TSharedPtr<FJsonObject> MicroTaskEventObject) {
	MicroTaskEventTypeEnum TaskType;
	TaskType = MicroTaskEventFromFString(MicroTaskEventObject->GetStringField(TEXT("Type")));

	FActorTaskStruct TaskData;
	TaskData.TaskID = MicroTaskEventObject->GetNumberField(TEXT("TaskId"));
	TaskData.TaskName = MicroTaskEventObject->GetStringField(TEXT("TaskName"));
	TaskData.ActorName = MicroTaskEventObject->GetStringField(TEXT("ActorName"));
	TaskData.Complete = MicroTaskEventObject->GetBoolField(TEXT("Complete"));
	TaskData.StartTimestamp = FDateTime::FromUnixTimestamp(MicroTaskEventObject->GetNumberField(TEXT("StartTimestamp")));
	TaskData.EndTimestamp = FDateTime::FromUnixTimestamp(MicroTaskEventObject->GetNumberField(TEXT("EndTimestamp")));
	TaskData.SubTaskList = GetMicroTaskSubTasks(MicroTaskEventObject->GetArrayField(TEXT("SubTasks")));

	return TPair<MicroTaskEventTypeEnum, FActorTaskStruct>(TaskType, TaskData);
}

TArray<FSubTaskStruct> UReplayHeaderJSONExporter::GetMicroTaskSubTasks(TArray<TSharedPtr<FJsonValue>> SubTaskListObj) {
	TArray<FSubTaskStruct> MicroTaskSubTasks;
	for (TSharedPtr<FJsonValue> SubTaskValue : SubTaskListObj)
	{
		TSharedPtr<FJsonObject> SubTaskObj = SubTaskValue->AsObject();
		FSubTaskStruct SubTask;
		SubTask.AOIName = SubTaskObj->GetStringField(TEXT("AOIName"));
		SubTask.Complete = SubTaskObj->GetBoolField(TEXT("Complete"));
		MicroTaskSubTasks.Add(SubTask);
	}
	return MicroTaskSubTasks;
}

FReplayEventStruct UReplayHeaderJSONExporter::GetReplayEventStruct(TSharedPtr<FJsonObject> ReplayEventStructObj)
{
	FReplayEventStruct ReplayEventStruct;
	ReplayEventStruct.Name = ReplayEventStructObj->GetStringField(TEXT("Name"));
	ReplayEventStruct.Description = ReplayEventStructObj->GetStringField(TEXT("Description"));
	ReplayEventStruct.SessionID = ReplayEventStructObj->GetStringField(TEXT("SessionID"));
	double StartTimestampUnix;
	if (ReplayEventStructObj->TryGetNumberField("StartTimestamp", StartTimestampUnix))
	{
		ReplayEventStruct.StartTimestamp = FTimespan::FromMilliseconds(StartTimestampUnix);
	}
	else
	{
		FTimespan::Parse(ReplayEventStructObj->GetStringField(TEXT("StartTimestamp")), ReplayEventStruct.StartTimestamp);
	}
	
	double EndTimestampUnix;
	if (ReplayEventStructObj->TryGetNumberField(TEXT("EndTimestamp"), EndTimestampUnix))
	{
		ReplayEventStruct.EndTimestamp = FTimespan::FromMilliseconds(EndTimestampUnix);
	}
	else
	{
		FTimespan::Parse(ReplayEventStructObj->GetStringField(TEXT("EndTimestamp")), ReplayEventStruct.EndTimestamp);
	}

	return ReplayEventStruct;
}

TimeIndexStruct UReplayHeaderJSONExporter::GetTimeIndexStruct(TSharedPtr<FJsonObject> TimeIndexStructObj)
{
	TimeIndexStruct TimeIndex;
	int64 TimeStampUnix;
	if (TimeIndexStructObj->TryGetNumberField("TimeStamp", TimeStampUnix))
	{
		TimeIndex.TimeStamp = FDateTime::FromUnixTimestamp(TimeStampUnix);
	}
	else
	{
		FDateTime::Parse(TimeIndexStructObj->GetStringField(TEXT("TimeStamp")), TimeIndex.TimeStamp);
	}
	
	TimeIndex.PlayIndex = TimeIndexStructObj->GetNumberField(TEXT("PlayIndex"));
	TimeIndex.FileIndex = TimeIndexStructObj->GetNumberField(TEXT("FileIndex"));
	TimeIndex.HasGAKeyFrame = TimeIndexStructObj->GetNumberField(TEXT("HasGAKeyFrame"));
	TimeIndex.LastGAKeyFrameIndex = TimeIndexStructObj->GetNumberField(TEXT("LastGAKeyFrameIndex"));

	return TimeIndex;
}

FSettingsStruct UReplayHeaderJSONExporter::GetSettingsStruct(TSharedPtr<FJsonObject> SettingsStructObj)
{
	FSettingsStruct SettingsStruct;
	TSharedPtr<FJsonObject> MotiveOffsetObj = SettingsStructObj->GetObjectField(TEXT("MotiveOffset"));
	SettingsStruct.MotiveOffset = UJSONExporter::GetFVectorFromJSON(MotiveOffsetObj);
	TSharedPtr<FJsonObject> ETOffsetObj = SettingsStructObj->GetObjectField(TEXT("ETOffset"));
	SettingsStruct.ETOffset = UJSONExporter::GetFVectorFromJSON(ETOffsetObj);
	SettingsStruct.PosTrackingUnitToUnrealUnitFactor = SettingsStructObj->GetNumberField(TEXT("PosTrackingUnitToUnrealUnitFactor"));
	SettingsStruct.EyesTrackingUnitToUnrealUnitFactor = SettingsStructObj->GetNumberField(TEXT("EyesTrackingUnitToUnrealUnitFactor"));
	SettingsStruct.SaveRate = SettingsStructObj->GetNumberField(TEXT("SaveRate"));
	SettingsStruct.SaveScreenRate = SettingsStructObj->GetNumberField(TEXT("SaveScreenRate"));
	SettingsStruct.bSaveRatePerMinute = SettingsStructObj->GetBoolField(TEXT("bSaveRatePerMinute"));
	SettingsStruct.CrossbarAdress = SettingsStructObj->GetStringField(TEXT("CrossbarAdress"));

	return SettingsStruct;
}

TArray<TPair<FDateTime, FImageAndAOIs>> UReplayHeaderJSONExporter::GetChangeImageEvents(TArray< TSharedPtr<FJsonValue> > ChangeImageEventsObj)
{
	TArray<TPair<FDateTime, FImageAndAOIs>> ChangeImageEvents;
	
	for (TSharedPtr<FJsonValue> ChangeImageEventObj : ChangeImageEventsObj)
	{
		TPair<FDateTime, FImageAndAOIs > ChangeImageEvent;
		TSharedPtr<FJsonObject> DataObj = ChangeImageEventObj->AsObject();
		ChangeImageEvent.Key = FDateTime::FromUnixTimestamp(DataObj->GetNumberField(TEXT("Timestamp")));
		ChangeImageEvent.Value = UExperimentDataJSONExporter::GetImageAndAOIs(DataObj->GetObjectField(TEXT("ImageAndAOIsEvent")));
		ChangeImageEvents.Add(ChangeImageEvent);
	}
	return ChangeImageEvents;
}

TArray<TPair<FDateTime, FPopupAndAOIs>> UReplayHeaderJSONExporter::GetPopupEvents(TArray< TSharedPtr<FJsonValue> > PopupEventsObj)
{
	TArray<TPair<FDateTime, FPopupAndAOIs>> PopupEvents;
	
	for (TSharedPtr<FJsonValue> PopupEventObj : PopupEventsObj)
	{
		TPair<FDateTime, FPopupAndAOIs > PopupEvent;
		TSharedPtr<FJsonObject> DataObj = PopupEventObj->AsObject();
		PopupEvent.Key = FDateTime::FromUnixTimestamp(DataObj->GetNumberField(TEXT("Timestamp")));
		PopupEvent.Value = UExperimentDataJSONExporter::GetPopupAndAOIs(DataObj->GetObjectField(TEXT("PopupEvent")));
		PopupEvents.Add(PopupEvent);
	}
	return PopupEvents;
}