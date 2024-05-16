#pragma once

#include "../GazeObjects/GazeActorDataTypes.h"
#include "ExperimentDataStructure.generated.h"

USTRUCT(BlueprintType)
struct FImageAndAOIs
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ImageAndAOIs Data")
		FString Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ImageAndAOIs Data")
		FString SpecifiedID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ImageAndAOIs Data")
		FString ScreenID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ImageAndAOIs Data")
		TArray<FAOIData> AOIs;

	FImageAndAOIs()
	{

	}

	FImageAndAOIs(FString _Image, FString _SpecifiedID, FString _ScreenID, TArray<FAOIData> _AOIs)
	{
		Image = _Image;
		SpecifiedID = _SpecifiedID;
		ScreenID = _ScreenID;
		AOIs = _AOIs;
	}
};

USTRUCT(BlueprintType)
struct FPopupAndAOIs
{
	GENERATED_USTRUCT_BODY()	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PopupAndAOIs Data")
		FString SpecifiedID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PopupAndAOIs Data")
		FString ScreenID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PopupAndAOIs Data")
		FString Action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PopupAndAOIs Data")
		TArray<FAOIData> AOIs;

	FPopupAndAOIs()
	{

	}

	FPopupAndAOIs(FString _SpecifiedID, FString _ScreenID, FString _Action, TArray<FAOIData> _AOIs)
	{
		SpecifiedID = _SpecifiedID;
		ScreenID = _ScreenID;
		Action = _Action;
		AOIs = _AOIs;
	}
};