// Fill out your copyright notice in the Description page of Project Settings.

#include "SettingsJSONExporter.h"

const FString SettingsFile = FPaths::ProjectDir() + "Resources/Settings.json";

USettingsJSONExporter::USettingsJSONExporter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void USettingsJSONExporter::SaveSettingsStruct(FSettingsStruct Settings)
{
	TSharedPtr<FJsonObject> SettingsObject = MakeShareable(new FJsonObject);
	
	SettingsObject->SetObjectField("MotiveOffset", UJSONExporter::SaveFVectorAsJSON(Settings.MotiveOffset));
	SettingsObject->SetObjectField("ETOffset", UJSONExporter::SaveFVectorAsJSON(Settings.ETOffset));
	SettingsObject->SetObjectField("LaserToolOffset", UJSONExporter::SaveFVectorAsJSON(Settings.LaserToolOffset));
	SettingsObject->SetNumberField("PosTrackingUnitToUnrealUnitFactor", Settings.PosTrackingUnitToUnrealUnitFactor);
	SettingsObject->SetNumberField("EyesTrackingUnitToUnrealUnitFactor", Settings.EyesTrackingUnitToUnrealUnitFactor);
	SettingsObject->SetBoolField("IsHorizontalCoordinateSystem", Settings.IsHorizontalCoordinateSystem);
	SettingsObject->SetNumberField("SaveRate", Settings.SaveRate);
	SettingsObject->SetNumberField("SaveScreenRate", Settings.SaveScreenRate);
	SettingsObject->SetBoolField("bSaveRatePerMinute", Settings.bSaveRatePerMinute);
	SettingsObject->SetStringField("CrossbarAdress", Settings.CrossbarAdress);
	SettingsObject->SetStringField("CrossbarRealm", Settings.CrossbarRealm);

	FString OutputString;
	TSharedRef< TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(SettingsObject.ToSharedRef(), Writer);

	FFileHelper::SaveStringToFile(OutputString, *SettingsFile);
}

FSettingsStruct USettingsJSONExporter::GetSettingsStruct()
{
	if (FPaths::FileExists(SettingsFile))
	{
		FString SettingsString;
		FFileHelper::LoadFileToString(SettingsString, *SettingsFile);
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(SettingsString);
		TSharedPtr<FJsonObject> SettingsObject = MakeShareable(new FJsonObject);

		if (FJsonSerializer::Deserialize(Reader, SettingsObject))
		{
			FSettingsStruct SettingsStruct;
			SettingsStruct.MotiveOffset = UJSONExporter::GetFVectorFromJSON(SettingsObject->GetObjectField("MotiveOffset"));
			SettingsStruct.ETOffset = UJSONExporter::GetFVectorFromJSON(SettingsObject->GetObjectField("ETOffset"));
			SettingsStruct.LaserToolOffset = UJSONExporter::GetFVectorFromJSON(SettingsObject->GetObjectField("LaserToolOffset"));
			SettingsStruct.PosTrackingUnitToUnrealUnitFactor = SettingsObject->GetNumberField("PosTrackingUnitToUnrealUnitFactor");
			SettingsStruct.EyesTrackingUnitToUnrealUnitFactor = SettingsObject->GetNumberField("EyesTrackingUnitToUnrealUnitFactor");
			SettingsStruct.IsHorizontalCoordinateSystem = SettingsObject->GetBoolField("IsHorizontalCoordinateSystem");
			SettingsStruct.SaveRate = SettingsObject->GetNumberField("SaveRate");
			SettingsStruct.SaveScreenRate = SettingsObject->GetNumberField("SaveScreenRate");
			SettingsStruct.bSaveRatePerMinute = SettingsObject->GetBoolField("bSaveRatePerMinute");
			SettingsStruct.CrossbarAdress = SettingsObject->GetStringField("CrossbarAdress");
			SettingsStruct.CrossbarRealm = SettingsObject->GetStringField("CrossbarRealm");

			return SettingsStruct;
		}
	}
	
	return FSettingsStruct();
}