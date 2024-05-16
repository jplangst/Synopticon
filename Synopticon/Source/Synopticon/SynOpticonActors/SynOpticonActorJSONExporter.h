// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SynOpticonDataStructures.h"

#include "../Utils/JSONExporter.h"

#include "SynOpticonActorJSONExporter.generated.h"
/**
 * 
 */
UCLASS()
class SYNOPTICON_API USynOpticonActorJSONExporter : public UObject
{
	GENERATED_UCLASS_BODY()
public:
	static TSharedPtr<FJsonObject> SaveSynOpticonActorData(FSynOpticonActorStruct SynOpticonActorStruct);
	static TSharedPtr<FJsonObject> SaveEyeTrackerStructData(FEyeTrackerStruct EyeTrackerStruct);
	static TSharedPtr<FJsonObject> SaveRemoteEyeTrackerStructData(FRemoteEyeTrackerStruct RemoteEyeTrackerStruct);
	static TSharedPtr<FJsonObject> SaveShimmerComponentStructData(FShimmerComponentStruct ShimmerComponentStruct);
	static TSharedPtr<FJsonObject> SaveHandStructData(FHandStruct HandStruct);
	static TSharedPtr<FJsonObject> SaveVisionStructData(FOpenFaceStruct VisionStruct);
	static TSharedPtr<FJsonObject> SaveLoggingStructData(FSynOpticonActorLoggingStruct SynOpticonActorLoggingStruct);
	static TSharedPtr<FJsonObject> SaveFixationGroupData(FFixationGroupStruct FixationGroupStruct);

	static TSharedPtr<FJsonObject> SavePathLoggingInfoData(FGazePathLoggingInfoStruct GazePathLoggingInfoStruct);
	static TSharedPtr<FJsonObject> SaveObjectLoggingInfoData(FObjectLoggingInfoStruct ObjectLoggingInfoStruct);

	static FString SaveMarkersConfigurationAsString(MarkersConfigurationEnum MarkersConfiguration);


	static FSynOpticonActorStruct GetSynOpticonActorData(TSharedPtr<FJsonObject> SynOpticonActorStructObj);
	static FEyeTrackerStruct GetEyeTrackerStructData(TSharedPtr<FJsonObject> EyeTrackerStructObj);
	static FRemoteEyeTrackerStruct GetRemoteEyeTrackerStructData(TSharedPtr<FJsonObject> RemoteEyeTrackerStructObj);
	static FShimmerComponentStruct GetShimmerComponentStructData(TSharedPtr<FJsonObject> ShimmerComponentStructObj);
	static FHandStruct GetHandStructData(TSharedPtr<FJsonObject> HandStructObj);
	static FOpenFaceStruct GetVisionStructData(TSharedPtr<FJsonObject> VisionStructObj);
	static FSynOpticonActorLoggingStruct GetLoggingStructData(TSharedPtr<FJsonObject> SynOpticonActorLoggingStructObj);
	static FFixationGroupStruct GetFixationGroupData(TSharedPtr<FJsonObject> FixationGroupStructObj);

	static FGazePathLoggingInfoStruct GetPathLoggingInfoData(TSharedPtr<FJsonObject> GazePathLoggingInfoStructObj);
	static FObjectLoggingInfoStruct GetObjectLoggingInfoData(TSharedPtr<FJsonObject> ObjectLoggingInfoStructObj);

	static MarkersConfigurationEnum GetMarkersConfigurationFromString(FString MarkersConfigurationStr);
};
