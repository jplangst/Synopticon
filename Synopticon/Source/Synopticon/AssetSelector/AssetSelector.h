// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "EngineUtils.h"

//#include "AssetRegistryModule.h"
//#include "AssetData.h"
#include "EngineUtils.h"

#include "AssetSelector.generated.h"
/**
 * 
 */
UCLASS()
class SYNOPTICON_API UAssetSelector : public UObject
{
	GENERATED_UCLASS_BODY()
public:
	//UFUNCTION(BlueprintCallable, Category="Asset Loading")
	//static TArray<FAssetData> GetAssetsInPath(FString Path);
	UFUNCTION(BlueprintCallable, Category = "Asset Loading")
	static TArray<UObject*> LoadAssetsInPath(FString Path);
	//UFUNCTION(BlueprintCallable, Category = "Asset Loading")
	//static UObject* LoadAsset(FAssetData AssetData);
};
