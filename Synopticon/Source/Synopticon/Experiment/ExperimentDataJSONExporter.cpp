// Fill out your copyright notice in the Description page of Project Settings.

#include "ExperimentDataJSONExporter.h"

UExperimentDataJSONExporter::UExperimentDataJSONExporter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSharedPtr<FJsonObject> UExperimentDataJSONExporter::SaveImageAndAOIs(FImageAndAOIs Data)
{
	TSharedPtr<FJsonObject> DataJson = MakeShareable(new FJsonObject);

	DataJson->SetStringField("Image", Data.Image);
	DataJson->SetStringField("SpecifiedID", Data.SpecifiedID);
	DataJson->SetStringField("ScreenID", Data.ScreenID);
	TArray<TSharedPtr<FJsonValue>> AOIs;
	for (FAOIData aoi : Data.AOIs)
	{
		AOIs.Add(MakeShareable(new FJsonValueObject(UGazeActorJSONExporter::SaveAOIData(aoi))));
	}
	DataJson->SetArrayField("AOIs", AOIs);
	return DataJson;
}

FImageAndAOIs UExperimentDataJSONExporter::GetImageAndAOIs(TSharedPtr<FJsonObject> DataObj)
{
	FImageAndAOIs ImageAndAOIs;

	DataObj->TryGetStringField("Image", ImageAndAOIs.Image);
	DataObj->TryGetStringField("SpecifiedID", ImageAndAOIs.SpecifiedID);
	DataObj->TryGetStringField("ScreenID", ImageAndAOIs.ScreenID);
	TArray< TSharedPtr<FJsonValue> > AOIs = DataObj->GetArrayField("AOIs");
	for (TSharedPtr<FJsonValue> aoi : AOIs) {
		TSharedPtr<FJsonObject> aoiObj = aoi->AsObject();
		ImageAndAOIs.AOIs.Add(UGazeActorJSONExporter::GetAOIData(aoiObj));
	}
	return ImageAndAOIs;
}

TSharedPtr<FJsonObject> UExperimentDataJSONExporter::SavePopupAndAOIs(FPopupAndAOIs Data)
{
	TSharedPtr<FJsonObject> DataJson = MakeShareable(new FJsonObject);

	DataJson->SetStringField("SpecifiedID", Data.SpecifiedID);
	DataJson->SetStringField("ScreenID", Data.ScreenID);
	DataJson->SetStringField("Action", Data.Action);
	TArray<TSharedPtr<FJsonValue>> AOIs;
	for (FAOIData aoi : Data.AOIs)
	{
		AOIs.Add(MakeShareable(new FJsonValueObject(UGazeActorJSONExporter::SaveAOIData(aoi))));
	}
	DataJson->SetArrayField("AOIs", AOIs);
	return DataJson;
}

FPopupAndAOIs UExperimentDataJSONExporter::GetPopupAndAOIs(TSharedPtr<FJsonObject> DataObj)
{
	FPopupAndAOIs PopupAndAOIs;

	DataObj->TryGetStringField("SpecifiedID", PopupAndAOIs.SpecifiedID);
	DataObj->TryGetStringField("ScreenID", PopupAndAOIs.ScreenID);
	DataObj->TryGetStringField("Action", PopupAndAOIs.Action);
	TArray< TSharedPtr<FJsonValue> > AOIs = DataObj->GetArrayField("AOIs");
	for (TSharedPtr<FJsonValue> aoi : AOIs) {
		TSharedPtr<FJsonObject> aoiObj = aoi->AsObject();
		PopupAndAOIs.AOIs.Add(UGazeActorJSONExporter::GetAOIData(aoiObj));
	}
	return PopupAndAOIs;
}