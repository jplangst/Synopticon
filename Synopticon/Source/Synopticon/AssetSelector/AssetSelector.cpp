// Fill out your copyright notice in the Description page of Project Settings.

#include "AssetSelector.h"
//#include "Synopticon.h"


UAssetSelector::UAssetSelector(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}


//TArray<FAssetData> UAssetSelector::GetAssetsInPath(FString Path) {
//	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));
//	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
//
//	// Need to do this if running in the editor with -game to make sure that the assets in the following path are available
//	TArray<FString> PathsToScan;
//	PathsToScan.Add(Path);//TEXT("/Game/Assets/Meshes/")); //Note that there hould not be a / at the end of the path
//	AssetRegistry.ScanPathsSynchronous(PathsToScan);
//
//	TArray<FAssetData> AssetList;
//	AssetRegistry.GetAssetsByPath(*Path, AssetList);//FName("/Game/Assets/Meshes/"), MeshAssetList);
//
//	return AssetList;
//}

TArray<UObject*> UAssetSelector::LoadAssetsInPath(FString Path) {
	TArray<UObject*> Assets;
	EngineUtils::FindOrLoadAssetsByPath(Path, Assets, EngineUtils::ATL_Regular);
	return Assets;
}

//UObject* UAssetSelector::LoadAsset(FAssetData AssetData) {
//	UObject* Asset = nullptr;
//	//EngineUtils::FindOrLoadAssetsByPath(Path, MeshAssets, EngineUtils::ATL_Regular);
//	//EngineUtils::
//	return Asset;
//}