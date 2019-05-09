// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Utils/JSONExporter.h"
#include "GazeObjects/GazeActorJSONExporter.h"
#include "ExperimentDataStructure.h"
#include "CoreMinimal.h"

#include "ExperimentDataJSONExporter.generated.h"

/**
 * 
 */
UCLASS()
class SYNOPTICON_API UExperimentDataJSONExporter : public UObject
{
	GENERATED_UCLASS_BODY()
public:
	static TSharedPtr<FJsonObject> SaveImageAndAOIs(FImageAndAOIs Data);
	static FImageAndAOIs GetImageAndAOIs(TSharedPtr<FJsonObject> DataObj);

	static TSharedPtr<FJsonObject> SavePopupAndAOIs(FPopupAndAOIs Data);
	static FPopupAndAOIs GetPopupAndAOIs(TSharedPtr<FJsonObject> DataObj);
};
