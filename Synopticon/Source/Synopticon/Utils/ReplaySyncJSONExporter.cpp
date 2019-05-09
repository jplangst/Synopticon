// Fill out your copyright notice in the Description page of Project Settings.

#include "ReplaySyncJSONExporter.h"
//#include "Synopticon.h"


UReplaySyncJSONExporter::UReplaySyncJSONExporter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

bool UReplaySyncJSONExporter::SaveReplaySyncData(FSyncReplayStruct ReplaySyncData) {	
	TSharedPtr<FJsonObject> SyncData = MakeShareable(new FJsonObject);
	FString VideoName;
	FString VideoPath;
	ReplaySyncData.RecordingSyncData.FilePath.Split("Resources/Videos/", &VideoPath, &VideoName, ESearchCase::Type::IgnoreCase, ESearchDir::Type::FromEnd);
	TSharedPtr<FJsonObject> RecordingData = MakeShareable(new FJsonObject);
	RecordingData->SetStringField("Filepath", VideoName);
	RecordingData->SetNumberField("VideoColumns", ReplaySyncData.RecordingSyncData.NmbCols);
	RecordingData->SetNumberField("VideoRows", ReplaySyncData.RecordingSyncData.NmbRows);
	RecordingData->SetNumberField("RecordingSyncTimestamp", ReplaySyncData.RecordingSyncData.RecordingSyncTime);
	RecordingData->SetNumberField("RecordingDuration", ReplaySyncData.RecordingSyncData.RecordingDuration);
	SyncData->SetObjectField("RecordingData", RecordingData);

	TSharedPtr<FJsonObject> SOReplayData = MakeShareable(new FJsonObject);
	//SOReplayData->SetStringField("Filepath", ReplaySyncData.SOReplaySyncData.Filepath);
	SOReplayData->SetNumberField("ReplaySyncTimestamp", ReplaySyncData.SOReplaySyncData.ReplaySyncTime);
	SOReplayData->SetNumberField("ReplayDuration", ReplaySyncData.SOReplaySyncData.ReplayDuration);
	SyncData->SetObjectField("SOReplayData", SOReplayData);

	
	TArray< TSharedPtr<FJsonValue> > ScreenLabels;
	for (FScreenLabelStruct ScreenLabel : ReplaySyncData.ScreenLabels) {
		TSharedPtr<FJsonObject> ScreenLabelObject = MakeShareable(new FJsonObject);
		ScreenLabelObject->SetStringField("ScreenName", ScreenLabel.ScreenName);
		ScreenLabelObject->SetNumberField("VideoIndex", ScreenLabel.VideoIndex);
		ScreenLabels.Add(MakeShareable(new FJsonValueObject(ScreenLabelObject)));
	}
	
	TArray< TSharedPtr<FJsonValue> > WebcamLabels;
	for (FScreenLabelStruct WebcamLabel : ReplaySyncData.WebcamLabels) {
		TSharedPtr<FJsonObject> WebcamLabelObject = MakeShareable(new FJsonObject);
		WebcamLabelObject->SetStringField("ScreenName", WebcamLabel.ScreenName);
		WebcamLabelObject->SetNumberField("VideoIndex", WebcamLabel.VideoIndex);
		WebcamLabels.Add(MakeShareable(new FJsonValueObject(WebcamLabelObject)));
	}

	SyncData->SetArrayField("ScreenLabels", ScreenLabels);
	SyncData->SetArrayField("WebcamLabels", WebcamLabels);

	//Serialise the JSONObject to a string
	FString OutputString;
	TSharedRef< TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(SyncData.ToSharedRef(), Writer);

	FString Left;
	FString Right;
	ReplaySyncData.SOReplaySyncData.Filepath.Split(".", &Left, &Right, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

	FString Filepath = Left.Append(".sync");

	return FFileHelper::SaveStringToFile(OutputString, *Filepath);
}

FSyncReplayStruct UReplaySyncJSONExporter::GetReplaySyncData(FString Filepath) {
	//The UE struct that will contain the deserialized json data
	FSyncReplayStruct ReplaySyncData;

	//Read the sync file content into a string
	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *Filepath);
	TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(JsonString);

	//Create the JSON object that the string will be deserialized into
	TSharedPtr<FJsonObject> SyncData = MakeShareable(new FJsonObject);

	//Attempt to deserialize the string
	if (FJsonSerializer::Deserialize(Reader, SyncData))
	{
		//If successfull we cna access the data here
		TSharedPtr<FJsonObject> RecordingData = SyncData->GetObjectField("RecordingData");
		ReplaySyncData.RecordingSyncData.FilePath = RecordingData->GetStringField("Filepath");
		ReplaySyncData.RecordingSyncData.NmbCols = RecordingData->GetNumberField("VideoColumns");
		ReplaySyncData.RecordingSyncData.NmbRows = RecordingData->GetNumberField("VideoRows");
		ReplaySyncData.RecordingSyncData.RecordingSyncTime = RecordingData->GetNumberField("RecordingSyncTimestamp");
		ReplaySyncData.RecordingSyncData.RecordingDuration = RecordingData->GetNumberField("RecordingDuration");


		TSharedPtr<FJsonObject> SOReplayData = SyncData->GetObjectField("SOReplayData");
		//ReplaySyncData.SOReplaySyncData.Filepath = SOReplayData->GetStringField("Filepath");
		ReplaySyncData.SOReplaySyncData.ReplaySyncTime = SOReplayData->GetNumberField("ReplaySyncTimestamp");
		ReplaySyncData.SOReplaySyncData.ReplayDuration = SOReplayData->GetNumberField("ReplayDuration");

		TArray< TSharedPtr<FJsonValue> > ScreenLabels = SyncData->GetArrayField("ScreenLabels");
		for (TSharedPtr<FJsonValue> ScreenLabel : ScreenLabels) {
			TSharedPtr<FJsonObject> ScreenObject = ScreenLabel->AsObject();
			FScreenLabelStruct ScreenLabelStruct;
			ScreenLabelStruct.ScreenName = ScreenObject->GetStringField("ScreenName");
			ScreenLabelStruct.VideoIndex = ScreenObject->GetNumberField("VideoIndex");
			ReplaySyncData.ScreenLabels.Add(ScreenLabelStruct);
		}


		TArray< TSharedPtr<FJsonValue> > WebcamLabels = SyncData->GetArrayField("WebcamLabels");
		for (TSharedPtr<FJsonValue> WebcamLabel : WebcamLabels) {
			TSharedPtr<FJsonObject> WebcamObject = WebcamLabel->AsObject();
			FScreenLabelStruct WebcamLabelStruct;
			WebcamLabelStruct.ScreenName = WebcamObject->GetStringField("ScreenName");
			WebcamLabelStruct.VideoIndex = WebcamObject->GetNumberField("VideoIndex");
			ReplaySyncData.WebcamLabels.Add(WebcamLabelStruct);
		}
	}

	return ReplaySyncData;
}

bool UReplaySyncJSONExporter::SaveReplayTextureData(FSOReplayTextureStruct ReplayTextureData) {
	TSharedPtr<FJsonObject> TextureData = MakeShareable(new FJsonObject);

	TextureData->SetStringField("ReplayFilepath", ReplayTextureData.ReplayFilepath);

	TArray< TSharedPtr<FJsonValue> > ScreenTextures;
	for (FScreenTextureStruct ScreenTexture : ReplayTextureData.ScreenTextureData) {
		TSharedPtr<FJsonObject> ScreenLabelObject = MakeShareable(new FJsonObject);
		ScreenLabelObject->SetStringField("ScreenName", ScreenTexture.ScreenName);
		ScreenLabelObject->SetStringField("TextureFilepath", ScreenTexture.ScreenTexture);
		ScreenTextures.Add(MakeShareable(new FJsonValueObject(ScreenLabelObject)));
	}

	TextureData->SetArrayField("ScreenTextures", ScreenTextures);

	//Serialise the JSONObject to a string
	FString OutputString;
	TSharedRef< TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(TextureData.ToSharedRef(), Writer);

	FString Left;
	FString Right;
	ReplayTextureData.ReplayFilepath.Split(".", &Left, &Right, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

	FString Filepath = Left.Append(".tex");

	return FFileHelper::SaveStringToFile(OutputString, *Filepath);
}

FSOReplayTextureStruct UReplaySyncJSONExporter::GetReplayTextureData(FString Filepath) {
	//The UE struct that will contain the deserialized json data
	FSOReplayTextureStruct ReplayTextures;

	//Read the sync file content into a string
	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *Filepath);
	TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(JsonString);

	//Create the JSON object that the string will be deserialized into
	TSharedPtr<FJsonObject> TextureData = MakeShareable(new FJsonObject);

	//Attempt to deserialize the string
	if (FJsonSerializer::Deserialize(Reader, TextureData))
	{
		//If successfull we cna access the data here
		ReplayTextures.ReplayFilepath = TextureData->GetStringField("ReplayFilepath");

		TArray< TSharedPtr<FJsonValue> > ScreenLabels = TextureData->GetArrayField("ScreenTextures");
		for (TSharedPtr<FJsonValue> ScreenLabel : ScreenLabels) {
			TSharedPtr<FJsonObject> ScreenObject = ScreenLabel->AsObject();
			FScreenTextureStruct ScreenTextureStruct;
			ScreenTextureStruct.ScreenName = ScreenObject->GetStringField("ScreenName");
			ScreenTextureStruct.ScreenTexture = ScreenObject->GetStringField("TextureFilepath");
			ReplayTextures.ScreenTextureData.Add(ScreenTextureStruct);
		}
	}

	return ReplayTextures;
}

bool UReplaySyncJSONExporter::UpdateSyncWithNewScreenNames(FString Filepath, TArray<FRenameScreenData> NewNameList)
{
	FString Left;
	FString Right;
	Filepath.Split(".", &Left, &Right, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

	FString SyncFile = Left.Append(".sync");
	if (FPaths::FileExists(SyncFile))
	{
		FSyncReplayStruct SyncStruct = GetReplaySyncData(SyncFile);
		TMap<FString, FString> nameDict;
		for (FRenameScreenData data : NewNameList)
		{
			nameDict.Add(data.OldName, data.NewName);
		}
		for (FScreenLabelStruct& screenlabel : SyncStruct.ScreenLabels)
		{
			FString* PtrName = nameDict.Find(screenlabel.ScreenName);
			if (PtrName) {
				screenlabel.ScreenName = *PtrName;
			}
		}

		SaveReplaySyncData(SyncStruct);
		return true;
	}
	return false;
}