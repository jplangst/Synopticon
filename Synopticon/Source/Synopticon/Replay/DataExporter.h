// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"

#include "HAL/PlatformFilemanager.h"
#include "HAL/PlatformFile.h"

#include "../GazeObjects/GazeActorDataTypes.h"
#include "../SynOpticonActors/SynOpticonDataStructures.h"
#include "../Replay/ReplayDataStructures.h"
#include "../Utils/SynOpticonStatics.h"

class SYNOPTICON_API DataExporter
{
	static TMap<int32, TPair<FString, bool>> AOISessionDict;
	static bool SaveGazeWithFixationGroupedLogComponentTable(FString FileName, FString ParticipantID, double ReplayTime,
		TMap<int32, FString> GANames,FSynOpticonActorLoggingStruct LogComponentData, 
		TArray<FFixationGroupStruct> Data, bool RelativeTimestamp);
	static bool SaveLogComponentTable(FString FileName, FString ParticipantID, double ReplayTime,
		TMap<int32, FString> GANames, FSynOpticonActorLoggingStruct LogComponentData, bool RelativeTimestamp);
	static bool SaveFixations(FString FileName, FString ParticipantID, TMap<int32, FString> GANames,
		TArray<FFixationGroupStruct> Fixations, FDateTime Begin, bool RelativeTimestamp);
	static bool SaveEvents(FString FileName, CompressedHeader* Header, bool RelativeTimestamp);

	static FString FromDateTimeToString(FDateTime Timestamp);
	static FString FromTimespanToString(FTimespan Timeduration);
	static FString FromIntToTimeFormat(int Number);
public:

	static FString DataDirectory;
	DataExporter();
	~DataExporter();

	static bool ExportData(FString FileName, CompressedHeader* Header, bool ExportAggregated, bool ExportFixations, bool RelativeTimestamp);
	//export binary
	static FString ExportBinaryData(FString FileName, CompressedHeader* Header);
	static void SaveSOBinaryDataBlock(FString FileName, FString Data);
	static FString GetSOBinaryDataRow(CompressedHeader* Header, FCompressedTickBlock DataBlock);



	static void ExportBinaryDataForSOActor(FString FileName);
};



