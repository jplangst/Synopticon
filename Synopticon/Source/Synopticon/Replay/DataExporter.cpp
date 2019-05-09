// Fill out your copyright notice in the Description page of Project Settings.

#include "DataExporter.h"
//#include "Synopticon.h"


FString DataExporter::DataDirectory = FPaths::ProjectDir() + "Resources/Data";
TMap<int32, TPair<FString, bool>> DataExporter::AOISessionDict;
DataExporter::DataExporter()
{
}

DataExporter::~DataExporter()
{
}

bool DataExporter::ExportData(FString FileName, CompressedHeader* Header, bool ExportAggregated, bool ExportFixations, bool RelativeTimestamp)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (PlatformFile.CreateDirectoryTree(*DataExporter::DataDirectory))
	{
		FString SavedPath = DataExporter::DataDirectory + "/" + FileName;
		if (PlatformFile.CreateDirectoryTree(*SavedPath)) {
			AOISessionDict.Empty();
			for (TPair<FDateTime, FImageAndAOIs> ImageEvent : Header->ChangeImageEvents)
			{
				for (FAOIData aoi : ImageEvent.Value.AOIs)
				{
					AOISessionDict.Add(aoi.ID, TPair<FString, bool>(ImageEvent.Value.SpecifiedID, false));
				}
			}
			for (TPair<FDateTime, FPopupAndAOIs> PopupEvent : Header->PopupEvents)
			{
				for (FAOIData aoi : PopupEvent.Value.AOIs)
				{
					AOISessionDict.Add(aoi.ID, TPair<FString, bool>(PopupEvent.Value.SpecifiedID, true));
				}
			}

			FDateTime BeginTimestamp = Header->StartTimestamp;

			TMap<int32, FString> GANames;
			for (FAOIData aoi : Header->AOIHistory)
			{
				GANames.Add(aoi.ID, aoi.Name);
			}
			for (FAOIData popup : Header->PopupHistory)
			{
				GANames.Add(popup.ID, popup.Name);
			}
			for (FGazeActorData GA : Header->GazeActorDataList)
			{
				GANames.Add(GA.ID, GA.Name);
			}
			for (FGazeGroupData GG : Header->GazeGroupList)
			{
				GANames.Add(GG.GroupID, GG.GroupName);
			}
			for (FSynOpticonActorStruct SOData : Header->SynOpticonActorDataList)
			{
				//SaveLogComponentTable(FString FileName, FString ParticipantID, double ReplayTime,
				//TMap<int32, FString> GANames, FSynOpticonActorLoggingStruct LogComponentData, TArray<FFixationGroupStruct> FixationData)
				bool GazeLogSave = false;
				FString _FileName = FileName + "_" + SOData.ActorName + "_" + (RelativeTimestamp ? "ReplayTime" : "UTC");
				if (ExportFixations)
				{
					GazeLogSave = SaveGazeWithFixationGroupedLogComponentTable(SavedPath + "/" + _FileName, SOData.ActorName, Header->ReplayTimeInSeconds, GANames, SOData.GazeLogComponentData, SOData.GazeData, RelativeTimestamp);
				}
				if (ExportAggregated)
				{
					GazeLogSave = SaveLogComponentTable(SavedPath + "/" + _FileName + "_eye_data", SOData.ActorName, Header->ReplayTimeInSeconds, GANames, SOData.GazeLogComponentData, RelativeTimestamp);
				}

				bool FixationSave = SaveFixations(SavedPath + "/" + _FileName, SOData.ActorName, GANames, SOData.GazeData, BeginTimestamp, RelativeTimestamp);

				bool HandLogSave = true;
				for (FHandStruct Hand : SOData.HandComponents)
				{
					if (Hand.LeftHand)
					{
						HandLogSave &= SaveLogComponentTable(SavedPath + "/" + _FileName + "_left_hand", SOData.ActorName, Header->ReplayTimeInSeconds, GANames, SOData.LeftHandLogComponentData, RelativeTimestamp);
					}
					else
					{
						HandLogSave &= SaveLogComponentTable(SavedPath + "/" + _FileName + "right_hand", SOData.ActorName, Header->ReplayTimeInSeconds, GANames, SOData.RightHandLogComponentData, RelativeTimestamp);
					}
				}

				if (!(GazeLogSave && FixationSave && HandLogSave))
					return false;
			}

			return SaveEvents(SavedPath + "/" + FileName, Header, RelativeTimestamp);
		}
		
	}
	return false;
}

bool DataExporter::SaveGazeWithFixationGroupedLogComponentTable(FString FileName, FString ParticipantID, double ReplayTime,
	TMap<int32, FString> GANames, FSynOpticonActorLoggingStruct LogComponentData, TArray<FFixationGroupStruct> FixationData, bool RelativeTimestamp)
{
	FString AbsoluteFilePath = FileName + "_fixation_aggregated_table.csv";
	
	FString ColumnHeaders = FString("Participant,SessionID,AOI,Popup,Time of fixations,Number of fixations,Percentage\n");
	FFileHelper::SaveStringToFile(ColumnHeaders, *AbsoluteFilePath);

	TMap<int32, int32> FixationCount;
	TMap<int32, float> FixationDurations;
	double TotalTime = 0;
	FString Row = "";

	for (FFixationGroupStruct Fixation : FixationData)
	{
		int32 value = 1;
		float duration = 0;
		if (FixationCount.Contains(Fixation.GazeActorId)) {
			value = FixationCount.FindAndRemoveChecked(Fixation.GazeActorId);
			value++;
		}
		if (FixationDurations.Contains(Fixation.GazeActorId)) {
			duration = FixationDurations.FindAndRemoveChecked(Fixation.GazeActorId);
			duration += Fixation.Duration;
		}
		else {
			duration = Fixation.Duration;
		}
		FixationCount.Add(Fixation.GazeActorId, value);
		FixationDurations.Add(Fixation.GazeActorId, duration);
	}

	for (TPair<int32, float> Obj : FixationDurations)
	{			
		TotalTime += Obj.Value;
		if (AOISessionDict.Contains(Obj.Key))
		{
			TPair<FString, bool> Out = AOISessionDict[Obj.Key];
			FString IsPopup = Out.Value ? "Yes" : "No";
			Row = ParticipantID + "," + Out.Key + "," + GANames.FindRef(Obj.Key) + "," + IsPopup + "," + GetFloatAsStringWithPrecision(Obj.Value, 2) + "," + FString::FromInt(FixationCount.FindRef(Obj.Key)) + "," + GetFloatAsStringWithPrecision(Obj.Value*100/ReplayTime, 2) + "\n";
		}
		else
		{
			Row = ParticipantID + ",," + GANames.FindRef(Obj.Key) + ",," + GetFloatAsStringWithPrecision(Obj.Value, 2) + "," + FString::FromInt(FixationCount.FindRef(Obj.Key)) + "," + GetFloatAsStringWithPrecision(Obj.Value*100/ReplayTime, 2) + "\n";
		}
				
		FFileHelper::SaveStringToFile(Row, *AbsoluteFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
	}

	double UnspecifiedTime = ReplayTime - TotalTime;
	double UnspecifiedPercentage = UnspecifiedTime * 100 / ReplayTime;
	Row = ParticipantID + ",,Unspecified,," + GetFloatAsStringWithPrecision(UnspecifiedTime, 2) + ",," + GetFloatAsStringWithPrecision(UnspecifiedPercentage, 2) + "\n";

	FFileHelper::SaveStringToFile(Row, *AbsoluteFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);

	return true;
}

bool DataExporter::SaveLogComponentTable(FString FileName, FString ParticipantID, double ReplayTime,
	TMap<int32, FString> GANames, FSynOpticonActorLoggingStruct LogComponentData, bool RelativeTimestamp)
{
	FString AbsoluteFilePath = FileName + "_aggregated_table.csv";

	// Allow overwriting or file doesn't already exist

	FString ColumnHeaders = FString("Participant,SessionID,AOI,Popup,Time,Count,Percentage\n");
	FFileHelper::SaveStringToFile(ColumnHeaders, *AbsoluteFilePath);

	double TotalPercentages = 0;
	double KnownTime = 0;
	for (TPair<int32, FObjectLoggingInfoStruct> Obj : LogComponentData.Objects)
	{
		FString Row = "";
		KnownTime += Obj.Value.Time;
		TotalPercentages += Obj.Value.Percentage;
		if (AOISessionDict.Contains(Obj.Key))
		{
			TPair<FString, bool> Out = AOISessionDict[Obj.Key];
			FString IsPopup = Out.Value ? "Yes" : "No";
			Row = ParticipantID + "," + Out.Key + "," + GANames.FindRef(Obj.Key) + "," + IsPopup + "," + GetFloatAsStringWithPrecision(Obj.Value.Time, 2) + "," + FString::FromInt(Obj.Value.Count) + "," + GetFloatAsStringWithPrecision(Obj.Value.Percentage, 2) + "\n";
		}
		else
		{
			Row = ParticipantID + ",," + GANames.FindRef(Obj.Key) + ",," + GetFloatAsStringWithPrecision(Obj.Value.Time, 2) + "," + FString::FromInt(Obj.Value.Count) + "," + GetFloatAsStringWithPrecision(Obj.Value.Percentage, 2) + "\n";
		}

		FFileHelper::SaveStringToFile(Row, *AbsoluteFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
	}

	
	//double UnspecifiedPercentage = UnspecifiedTime * 100 / ReplayTime;
	double UnspecifiedPercentage = 100 - TotalPercentages;
	double UnspecifiedTime = KnownTime * UnspecifiedPercentage / TotalPercentages;

	FString Row = ParticipantID + ",,Unspecified,," + GetFloatAsStringWithPrecision(UnspecifiedTime, 2) + ",," + GetFloatAsStringWithPrecision(UnspecifiedPercentage, 2) + "\n";

	FFileHelper::SaveStringToFile(Row, *AbsoluteFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);

	return true;
}

bool DataExporter::SaveFixations(FString FileName, FString ParticipantID, TMap<int32, FString> GANames, 
	TArray<FFixationGroupStruct> Fixations, FDateTime Begin, bool RelativeTimestamp)
{
	FString FixationFilePath = FileName + "_fixations_list.csv";
	FString ColumnHeaders = FString("Participant,Start,End,Duration,x,y,SessionID,AOI,Popup\n");
	FFileHelper::SaveStringToFile(ColumnHeaders, *FixationFilePath);
	for (FFixationGroupStruct Fixation : Fixations) 
	{
		FString StartTimestamp;
		FString EndTimestamp;
		if (RelativeTimestamp) 
		{
			StartTimestamp = FromTimespanToString(Fixation.Timestamp - Begin);
			EndTimestamp = FromTimespanToString(Fixation.Timestamp + FTimespan::FromSeconds(Fixation.Duration) - Begin);
		}
		else //Utc time
		{	
			StartTimestamp = FromDateTimeToString(Fixation.Timestamp);
			EndTimestamp = FromDateTimeToString(Fixation.Timestamp + FTimespan::FromSeconds(Fixation.Duration));
		}

		FString Row = ParticipantID + "," + StartTimestamp + "," +
			EndTimestamp + "," +
			GetFloatAsStringWithPrecision(Fixation.Duration, 2) + "," +
			GetFloatAsStringWithPrecision(Fixation.HitUV.X, 2) + "," +
			GetFloatAsStringWithPrecision(Fixation.HitUV.Y, 2) + "," +
			Fixation.SessionId + "," +
			GANames.FindRef(Fixation.GazeActorId) + "," + 
			(Fixation.bPopup ? "Yes" : "No") + "\n";
		FFileHelper::SaveStringToFile(Row, *FixationFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
	}
	return true;
}

bool DataExporter::SaveEvents(FString FileName, CompressedHeader* Header, bool RelativeTimestamp)
{
	FDateTime BeginTimestamp = Header->StartTimestamp;
	FDateTime EndReplayTimestamp = BeginTimestamp + FTimespan::FromSeconds(Header->ReplayTimeInSeconds);

	FString FixationFilePath = FileName + (RelativeTimestamp?"_ReplayTime":"_UTC") + "_events.csv";
	FString ColumnHeaders = FString("Start,End,Duration,SessionID,Name,Description\n");
	FFileHelper::SaveStringToFile(ColumnHeaders, *FixationFilePath);
	FString Row;
	if (RelativeTimestamp) {
		Row = FromTimespanToString(FTimespan::FromSeconds(0)) + ",,,,Start, Start recording\n";
	}
	else {
		Row = FromDateTimeToString(BeginTimestamp) + ",,,,Start, Start recording\n";
	}
	FFileHelper::SaveStringToFile(Row, *FixationFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
	for (FReplayEventStruct Event : Header->ReplayEvents)
	{
		FString StartTimestamp;
		FString EndTimestamp = "";
		FString Duration = "";
		if (RelativeTimestamp)
		{
			StartTimestamp = FromTimespanToString(Event.StartTimestamp);
			EndTimestamp = FromTimespanToString(Event.EndTimestamp);
		}
		else //Utc time
		{
			StartTimestamp = FromDateTimeToString(BeginTimestamp + Event.StartTimestamp);
			EndTimestamp = FromDateTimeToString(BeginTimestamp + Event.EndTimestamp);
		}
		if (Event.EndTimestamp == FTimespan::Zero())
		{
			EndTimestamp = "";
		}
		else 
		{
			Duration = GetFloatAsStringWithPrecision((Event.EndTimestamp - Event.StartTimestamp).GetTotalSeconds(), 2);
		}

		FString Replace = "\"";
		FString ReplaceTo = "\"\"";
		FString Tmp = Event.Description.Replace(*Replace, *ReplaceTo);
		FString Description = "\"" + Tmp + "\"";

		Row = StartTimestamp + "," +
			EndTimestamp + "," +
			Duration + "," +
			Event.SessionID + "," +
			Event.Name + "," + 
			Description + "\n";
		FFileHelper::SaveStringToFile(Row, *FixationFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
	}

	for (TPair<FDateTime, FImageAndAOIs> ImageWAMPEvent : Header->ChangeImageEvents)
	{
		FString StartTimestamp;

		if (RelativeTimestamp)
		{
			StartTimestamp = FromTimespanToString(ImageWAMPEvent.Key - BeginTimestamp);
		}
		else //Utc time
		{
			StartTimestamp = FromDateTimeToString(ImageWAMPEvent.Key);
		}
		//"Start,End,Duration,SessionID,Name,Description\n"
		Row = StartTimestamp + ",,," + ImageWAMPEvent.Value.SpecifiedID + ",Set Image And AOIs,\n";
		FFileHelper::SaveStringToFile(Row, *FixationFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
	}

	for (TPair<FDateTime, FPopupAndAOIs> PopupWAMPEvent : Header->PopupEvents)
	{
		FString StartTimestamp;

		if (RelativeTimestamp)
		{
			StartTimestamp = FromTimespanToString(PopupWAMPEvent.Key - BeginTimestamp);
		}
		else //Utc time
		{
			StartTimestamp = FromDateTimeToString(PopupWAMPEvent.Key);
		}
		//"Start,End,Duration,SessionID,Name,Description\n"
		Row = StartTimestamp + ",,," + PopupWAMPEvent.Value.SpecifiedID + "," + PopupWAMPEvent.Value.Action + " Popup, SessionID: " + PopupWAMPEvent.Value.SpecifiedID + "\n";
		FFileHelper::SaveStringToFile(Row, *FixationFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
	}
	if (RelativeTimestamp) {
		Row = FromTimespanToString(EndReplayTimestamp - BeginTimestamp) + ",,,,Stop, Stop recording\n";
	}
	else {
		Row = FromDateTimeToString(EndReplayTimestamp) + ",,,,Stop, Stop recording\n";
	}
	
	FFileHelper::SaveStringToFile(Row, *FixationFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
	return true;
}

FString DataExporter::ExportBinaryData(FString FileName, CompressedHeader* Header)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString _File;
	FString _Path;
	FileName.Split("/", &_Path, &_File, ESearchCase::IgnoreCase, ESearchDir::Type::FromEnd);
	FString File = DataExporter::DataDirectory + "/" + _File + "/" + _File + "_binary_data.csv";
	if (PlatformFile.CreateDirectoryTree(*DataExporter::DataDirectory))
	{
		if (PlatformFile.CreateDirectoryTree(*(DataExporter::DataDirectory + "/" + _File))) {
			
			FString ColumnHeaders = FString("Timestamp");
			for (FSynOpticonActorStruct SO : Header->SynOpticonActorDataList)
			{
				/*FString ColumnHeaders = FString("Timestamp, Location-X, Location-Y, Location-Z, Rotation-Pitch, Rotation-Roll, Rotation-Yaw,")
					+ FString("LeftEyeLocation-X, LeftEyeLocation-Y, LeftEyeLocation-Z, LeftEyeRotation-Pitch, LeftEyeRotation-Roll, LeftEyeRotation-Yaw,")
					+ FString("RightEyeLocation-X, RightEyeLocation-Y, RightEyeLocation-Z, RightEyeRotation-Pitch, RightEyeRotation-Roll, RightEyeRotation-Yaw,")
					+ FString("PupilRadius,")
					+ FString("LeftHandLocation-X, LeftHandLocation-Y, LeftHandLocation-Z, LeftHandRotation-Pitch, LeftHandRotation-Roll, LeftHandRotation-Yaw,")
					+ FString("RightHandLocation-X, RightHandLocation-Y, RightHandLocation-Z, RightHandRotation-Pitch, RightHandRotation-Roll, RightHandRotation-Yaw");*/

				ColumnHeaders += ", PupilRadius-" + SO.ActorName
					+ ", LeftEyeLocationX-" + SO.ActorName + ", LeftEyeLocationY-" + SO.ActorName + ", LeftEyeLocationZ-" + SO.ActorName
					+ ", LeftEyeRotationPitch-" + SO.ActorName + ", LeftEyeRotationYaw-" + SO.ActorName + ", LeftEyeRotationRoll-" + SO.ActorName
					+ ", RightEyeLocationX-" + SO.ActorName + ", RightEyeLocationY-" + SO.ActorName + ", RightEyeLocationZ-" + SO.ActorName
					+ ", RightEyeRotationPitch-" + SO.ActorName + ", RightEyeRotationYaw-" + SO.ActorName + ", RightEyeRotationRoll-" + SO.ActorName;
			}
			ColumnHeaders += "\n";
			FFileHelper::SaveStringToFile(ColumnHeaders, *File);
		}
		
	}
	return File;
}

void DataExporter::ExportBinaryDataForSOActor(FString FileName)
{

}


void DataExporter::SaveSOBinaryDataBlock(FString FileName, FString Data)
{
	FFileHelper::SaveStringToFile(Data, *FileName, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
}

FString DataExporter::GetSOBinaryDataRow(CompressedHeader* Header, FCompressedTickBlock DataBlock)
{
	int i = 0;
	FString Rows = FromDateTimeToString(DataBlock.DateTime);
	for (CompressedSOBlock* SOData : DataBlock.SOActorData)
	{
		if (SOData->HasETGData) {
			ETGDataBlock ETGBlock = SOData->ETGData;
			Rows += "," + FString::SanitizeFloat(SOData->ETGData.PupilRadius) +
				"," + FString::SanitizeFloat(ETGBlock.LeftEyeLocation.X) + //left
				"," + FString::SanitizeFloat(ETGBlock.LeftEyeLocation.Y) +
				"," + FString::SanitizeFloat(ETGBlock.LeftEyeLocation.Z) +
				"," + FString::SanitizeFloat(ETGBlock.LeftEyeRotation.Pitch) +
				"," + FString::SanitizeFloat(ETGBlock.LeftEyeRotation.Yaw) +
				"," + FString::SanitizeFloat(ETGBlock.LeftEyeRotation.Roll) +
				"," + FString::SanitizeFloat(ETGBlock.RightEyeLocation.X) + //right
				"," + FString::SanitizeFloat(ETGBlock.RightEyeLocation.Y) +
				"," + FString::SanitizeFloat(ETGBlock.RightEyeLocation.Z) +
				"," + FString::SanitizeFloat(ETGBlock.RightEyeRotation.Pitch) +
				"," + FString::SanitizeFloat(ETGBlock.RightEyeRotation.Yaw) +
				"," + FString::SanitizeFloat(ETGBlock.RightEyeRotation.Roll);
		}
		else if (SOData->hasRTData) {
			RTDataBlock RTBlock = SOData->RTData;
			Rows += "," + FString::SanitizeFloat((RTBlock.RightEyePupilRadius + RTBlock.LeftEyePupilRadius)/2) +
				"," + FString::SanitizeFloat(RTBlock.LeftEyeLocation.X) + //left
				"," + FString::SanitizeFloat(RTBlock.LeftEyeLocation.Y) +
				"," + FString::SanitizeFloat(RTBlock.LeftEyeLocation.Z) +
				"," + FString::SanitizeFloat(RTBlock.LeftEyeRotation.Pitch) +
				"," + FString::SanitizeFloat(RTBlock.LeftEyeRotation.Yaw) +
				"," + FString::SanitizeFloat(RTBlock.LeftEyeRotation.Roll) +
				"," + FString::SanitizeFloat(RTBlock.RightEyeLocation.X) + //right
				"," + FString::SanitizeFloat(RTBlock.RightEyeLocation.Y) +
				"," + FString::SanitizeFloat(RTBlock.RightEyeLocation.Z) +
				"," + FString::SanitizeFloat(RTBlock.RightEyeRotation.Pitch) +
				"," + FString::SanitizeFloat(RTBlock.RightEyeRotation.Yaw) +
				"," + FString::SanitizeFloat(RTBlock.RightEyeRotation.Roll);
		}
		else {
			Rows += ",,,,,,,,,,,,,";
		}
		//Row = DataBlock.DateTime.ToString() + "," + FString::SanitizeFloat(SOData->ActorLocation.X) + "," + FString::SanitizeFloat(SOData->ActorLocation.Y) + "," + FString::SanitizeFloat(SOData->ActorLocation.Z) + 
		//				"," + FString::SanitizeFloat(SOData->ActorRotation.Pitch) + "," + FString::SanitizeFloat(SOData->ActorRotation.Roll) + "," + FString::SanitizeFloat(SOData->ActorRotation.Yaw);
		//if (SOData->HasETGData)
		//{
		//	//export etgdata
		//	ETGDataBlock ETGBlock = SOData->ETGData;
		//	FString PupilRadius = "";
		//	if (ETGBlock.ETGStructVersion == 1)
		//	{
		//		PupilRadius = FString::SanitizeFloat(ETGBlock.PupilRadius);
		//	}
		//	/*
		//	"LeftEyeLocation", "LeftEyeRotation", "RightEyeLocation", "RightEyeRotation",
		//	"PupilRadius"
		//	*/
		//	Row += ("," + FString::SanitizeFloat(ETGBlock.LeftEyeLocation.X) + "," + FString::SanitizeFloat(ETGBlock.LeftEyeLocation.Y) + "," + FString::SanitizeFloat(ETGBlock.LeftEyeLocation.Z) +
		//		    "," + FString::SanitizeFloat(ETGBlock.LeftEyeRotation.Pitch) + "," + FString::SanitizeFloat(ETGBlock.LeftEyeRotation.Roll) + "," + FString::SanitizeFloat(ETGBlock.LeftEyeRotation.Yaw) + 
		//			"," + FString::SanitizeFloat(ETGBlock.RightEyeLocation.X) + "," + FString::SanitizeFloat(ETGBlock.RightEyeLocation.Y) + "," + FString::SanitizeFloat(ETGBlock.RightEyeLocation.Z) + 
		//			"," + FString::SanitizeFloat(ETGBlock.RightEyeRotation.Pitch) + "," + FString::SanitizeFloat(ETGBlock.RightEyeRotation.Roll) + "," + FString::SanitizeFloat(ETGBlock.RightEyeRotation.Yaw) +
		//			"," + PupilRadius);
		//}

		//int j = 0;
		//FSynOpticonActorStruct SO = Header->SynOpticonActorDataList[i];
		//for (HandDataBlock* HandData : SOData->HandDataBlocks)
		//{
		//	FRotator Rotation = HandData->HandRotation.Rotator();
		//	if (SO.HandComponents[j].LeftHand)
		//	{
		//		Row += ("," + FString::SanitizeFloat(HandData->HandLocation.X) + "," + FString::SanitizeFloat(HandData->HandLocation.Y) + "," + FString::SanitizeFloat(HandData->HandLocation.Z) +
		//				"," + FString::SanitizeFloat(Rotation.Pitch) + "," + FString::SanitizeFloat(Rotation.Roll) + "," + FString::SanitizeFloat(Rotation.Yaw) + 
		//				",,\n");
		//	}
		//	else
		//	{
		//		Row += (",,," + FString::SanitizeFloat(HandData->HandLocation.X) + "," + FString::SanitizeFloat(HandData->HandLocation.Y) + "," + FString::SanitizeFloat(HandData->HandLocation.Z) +
		//				"," + FString::SanitizeFloat(Rotation.Pitch) + "," + FString::SanitizeFloat(Rotation.Roll) + "," + FString::SanitizeFloat(Rotation.Yaw) +
		//				"\n");
		//	}
		//	j++;
		//}

		
		i++;
	}
	Rows += "\n";
	return Rows;
}


FString DataExporter::FromDateTimeToString(FDateTime Timestamp)
{
	FString TimestampInString = FromIntToTimeFormat(Timestamp.GetDay()) + "/" + FromIntToTimeFormat(Timestamp.GetMonth()) + "/" + FromIntToTimeFormat(Timestamp.GetYear()) +
		" " + FromIntToTimeFormat(Timestamp.GetHour()) + ":" + FromIntToTimeFormat(Timestamp.GetMinute()) + ":" + FromIntToTimeFormat(Timestamp.GetSecond()) + "." + FString::FromInt(Timestamp.GetMillisecond());
	return TimestampInString;
}

FString DataExporter::FromTimespanToString(FTimespan Timeduration)
{
	FString TimedurationInString = FromIntToTimeFormat(Timeduration.GetHours()) + ":" + FromIntToTimeFormat(Timeduration.GetMinutes()) + ":" + FromIntToTimeFormat(Timeduration.GetSeconds()) + "." + FString::FromInt(Timeduration.GetFractionMilli());
	return TimedurationInString;
}

FString DataExporter::FromIntToTimeFormat(int Number) 
{
	if (Number < 10) {
		return "0" + FString::FromInt(Number);
	}
	else {
		return FString::FromInt(Number);
	}
}