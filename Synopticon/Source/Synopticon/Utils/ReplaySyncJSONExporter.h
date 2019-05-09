// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SystemClasses/SynOpticonCustomType.h"

#include "ReplaySyncJSONExporter.generated.h"
/**
 * 
 */

UCLASS()
class UReplaySyncJSONExporter : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Replay Sync Data")
	static bool SaveReplaySyncData(FSyncReplayStruct ReplaySyncData);

	UFUNCTION(BlueprintCallable, Category = "Replay Sync Data")
	static FSyncReplayStruct GetReplaySyncData(FString Filepath);

	UFUNCTION(BlueprintCallable, Category = "Replay Texture Data")
	static bool SaveReplayTextureData(FSOReplayTextureStruct ReplayTextureData);

	UFUNCTION(BlueprintCallable, Category = "Replay Texture Data")
	static FSOReplayTextureStruct GetReplayTextureData(FString Filepath);

	UFUNCTION(BlueprintCallable, Category = "Replay Screen Sync")
	static bool UpdateSyncWithNewScreenNames(FString Filepath, TArray<FRenameScreenData> NewNameList);
};
