// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../SystemClasses/SynOpticonCustomType.h"
#include "../Utils/JSONExporter.h"

#include "SettingsJSONExporter.generated.h"

/**
 * 
 */
UCLASS()
class SYNOPTICON_API USettingsJSONExporter : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Settings JSON Exporter")
		static void SaveSettingsStruct(FSettingsStruct Settings);

	UFUNCTION(BlueprintCallable, Category = "Settings JSON Exporter")
		static FSettingsStruct GetSettingsStruct();
};
