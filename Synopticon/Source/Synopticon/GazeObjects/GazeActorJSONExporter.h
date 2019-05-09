// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GazeActorDataTypes.h"
#include "StaticRoomActorDatatypes.h"

#include "Utils/JSONExporter.h"

#include "GazeActorJSONExporter.generated.h"
/**
 * 
 */
UCLASS()
class SYNOPTICON_API UGazeActorJSONExporter : public UObject
{
	GENERATED_UCLASS_BODY()
public:
	static TSharedPtr<FJsonObject> SaveGazeActorData(FGazeActorData GazeActorData);
	static FGazeActorData GetGazeActorData(TSharedPtr<FJsonObject> GazeActorObj);

	static FString SaveGazeActorTypeAsString(EStaticRoomActorType GazeActorType);
	static EStaticRoomActorType GetGazeActorTypeFromString(FString GazeActorTypeString);

	static TSharedPtr<FJsonObject> SaveGazeParentActorData(FGazeParentActorData GazeParentActorData);
	static FGazeParentActorData GetGazeParentActorData(TSharedPtr<FJsonObject> GazeParentActorDataObj);

	static TSharedPtr<FJsonObject> SaveGazeObjectAndParentData(FGazeObjectAndParentData GazeObjectAndParentData);
	static FGazeObjectAndParentData GetGazeObjectAndParentData(TSharedPtr<FJsonObject> GazeObjectAndParentDataObj);

	static TSharedPtr<FJsonObject> SaveGazeGroupData(FGazeGroupData GazeGroupData);
	static FGazeGroupData GetGazeGroupData(TSharedPtr<FJsonObject> GazeGroupDataObj);

	static TSharedPtr<FJsonObject> SaveAOIData(FAOIData AOIData);
	static FAOIData GetAOIData(TSharedPtr<FJsonObject> AOIData);
};
