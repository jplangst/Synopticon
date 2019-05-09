// Fill out your copyright notice in the Description page of Project Settings.

#include "RecordManager.h"

//================================SINGLETON======================================
URecordManager* URecordManager::Instance = nullptr;

URecordManager* URecordManager::GetRecordManagerInstance()
{
	return Instance;
}

void URecordManager::SetInstance(URecordManager* _Instance)
{
	Instance = _Instance;
}

//================================Constructor - Destructor========================
URecordManager::URecordManager()
{
	PrimaryComponentTick.bCanEverTick = true;
	Header = nullptr;

	FirstSample = true;
	ActiveArchive = 0;
	SecondaryArchive = 1;
	NextLogFileNumber = 0;
}

URecordManager::~URecordManager()
{
	if (Header)
	{
		delete Header;
		Header = nullptr;
	}
	EmptyArchives();
	CleanupTickBlock();
}

//==============================Component functions================================
void URecordManager::BeginPlay()
{
	Super::BeginPlay();
	ASynOpticonState::GetGlobalEventSystem()->OnReceiveNewImageAndAOIsDelegate.AddDynamic(this, &URecordManager::OnNewImageAndAOIs);
	ASynOpticonState::GetGlobalEventSystem()->OnReceivePopupAndAOIsDelegate.AddDynamic(this, &URecordManager::OnPopupEvent);
	ASynOpticonState::GetGlobalEventSystem()->OnMicroTaskEventDelegate.AddDynamic(this, &URecordManager::OnMicroTaskEvent);
}

void URecordManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ASynOpticonState::IsRecording())
	{
		TickBlock.DateTime = FDateTime::UtcNow();
		if (FirstSample) //initialize info in header
		{
			TimeSinceLastSave = FDateTime::UtcNow();
			
			Header->TimeIndex.Add(TArray<TimeIndexStruct>());
			SaveTimeIndex();

			FirstSample = false;
		}

		//Update the timestamp of the tick block
		//if (GazeActorHasKeyFrame)
		//{
		//	HasGAKeyFrame = 1;
		//	LastKeyFrameIndex = GetActiveArchive()->TotalSize();
		//	LastKeyFrameTimestamp = FDateTime::UtcNow();
		//}

		if ((Header->TimeIndex.Last(0).Num() <= 0) || ((TickBlock.DateTime - Header->TimeIndex.Last(0).Last(0).TimeStamp).GetSeconds() >= 1))
		{
			SaveTimeIndex();
		}

		SaveLoadBlockData(*ToBinaryArchives[ActiveArchive], TickBlock);


		if ((FDateTime::UtcNow() - TimeSinceLastSave).GetTotalMinutes() >= ASynOpticonState::GetSplitRecordingTimeThreshold())
		{
			SwitchAndSaveActiveArchive();

			//add a new time index array for the new file
			TArray<TimeIndexStruct> TimeIndexForNewFile;
			Header->TimeIndex.Add(TimeIndexForNewFile);
			TimeSinceLastSave = FDateTime::UtcNow();
		}
	}
}

//===============================Information about file ===========================
FString URecordManager::GetFileName() {
	return FileName;
}

FDateTime URecordManager::GetStartTime()
{
	if (Header)
	{
		return Header->StartTimestamp;
	}
	return FDateTime::UtcNow();
}

//===============================Header related functions===========================
void URecordManager::SaveHeaderFile()
{
	if (!Header)
	{
		return;
	}

	FString HeaderFile = FileName + ".header";

	TSharedPtr<FJsonObject> HeaderJson = UReplayHeaderJSONExporter::SaveHeaderData(*Header);
	FString OutputString;
	TSharedRef< TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(HeaderJson.ToSharedRef(), Writer);

	FFileHelper::SaveStringToFile(OutputString, *HeaderFile);

	/*// SAVE THIS PART FOR LATER WHEN WE WANT TO CONVERT BACK TO SAVING IN BINARY
	FBufferArchive ToHeaderBinary;
	SaveLoadHeaderData(ToHeaderBinary, *ReplayHeader);

	TArray<uint8> CompressedHeaderData;
	FArchiveSaveCompressedProxy HeaderCompressor =
	FArchiveSaveCompressedProxy(CompressedHeaderData, ECompressionFlags::COMPRESS_ZLIB);

	HeaderCompressor << ToHeaderBinary;
	HeaderCompressor.Flush();

	//vibes to file, return successful or not
	bool _IsHeaderSaved = FFileHelper::SaveArrayToFile(CompressedHeaderData, *HeaderFile);

	//Update the .tex file for the recording
	if (_IsHeaderSaved) {
	FSOReplayTextureStruct ReplayTextureDataL;
	ReplayTextureDataL.ReplayFilepath = ReplayFileName;
	TArray<FGazeActorData> GazeObjectsData;
	for (AActor* GA : ASynOpticonState::GetGazeActorList())
	{
	//Add the screens texture data
	FScreenTextureStruct ScreenTextureStruct;
	ScreenTextureStruct.ScreenName = GA->GetName();
	UGazeObjectMaterialComponent* GazeMaterialComp = Cast<UGazeObjectMaterialComponent>(GA->GetComponentByClass(UGazeObjectMaterialComponent::StaticClass()));
	if (GazeMaterialComp && !GazeMaterialComp->GetTextureFilepath().Equals("")) {
	ScreenTextureStruct.ScreenTexture = GazeMaterialComp->GetTextureFilepath();
	}
	ReplayTextureDataL.ScreenTextureData.Add(ScreenTextureStruct);
	}

	//Save the texture data if the header was saved correctly
	UReplaySyncJSONExporter::SaveReplayTextureData(ReplayTextureDataL);
	}

	HeaderCompressor.FlushCache();
	CompressedHeaderData.Empty();

	ToHeaderBinary.FlushCache();
	ToHeaderBinary.Empty();

	ToHeaderBinary.Close();

	return _IsHeaderSaved;
	*/
}

void URecordManager::SetReplayEvents(TArray<FReplayEventStruct> ReplayEvents)
{
	if (Header)
	{
		Header->ReplayEvents = ReplayEvents;
	}
}

TArray<FReplayEventStruct> URecordManager::GetReplayEvents()
{
	if (Header)
	{
		return Header->ReplayEvents;
	}
	return TArray<FReplayEventStruct>();
}

void URecordManager::OnNewImageAndAOIs(FImageAndAOIs Data)
{
	if (Header)
	{
		Header->ChangeImageEvents.Add(TPair<FDateTime, FImageAndAOIs>(FDateTime::UtcNow(), Data));
		for (FAOIData aoi : Data.AOIs)
		{
			Header->AOIHistory.Add(aoi);
		}
	}
}

void URecordManager::OnPopupEvent(FPopupAndAOIs Data)
{
	if (Header)
	{
		Header->PopupEvents.Add(TPair<FDateTime, FPopupAndAOIs>(FDateTime::UtcNow(), Data));
		for (FAOIData aoi : Data.AOIs)
		{
			Header->PopupHistory.Add(aoi);
		}
	}
}

//====================================Micro Task====================================
void URecordManager::OnMicroTaskEvent(MicroTaskEventTypeEnum EventType, FActorTaskStruct EventData) {
	if (Header && ASynOpticonState::IsRecording())
	{
		Header->MicroTaskEvents.Add(TPair<MicroTaskEventTypeEnum, FActorTaskStruct>(EventType, EventData));
	}
}

//====================================record file===================================
void URecordManager::StartRecording(FString _FileName)
{
	//keep a record of filename
	FileName = _FileName;
	FileName.RemoveFromEnd(".re");

	//-------------------------set up header-------------------------------
	if (Header)
	{
		delete Header;
	}
	Header = new CompressedHeader();

	//get gaze actor info & so actor infor from creationManager and save them into header
	TArray<CompressedGazeBlock*> GazeBlocks;
	TArray<CompressedSOBlock*> SOBlocks;
	Header->GazeActorDataList = UCreationAndDeletionManager::GetCADManagerInstance()->PrepareActorsBeforeRecording(GazeBlocks, SOBlocks);

	for (FGazeActorData GA : Header->GazeActorDataList)
	{
		for (FAOIData aoi : GA.AOIS)
		{
			Header->AOIHistory.Add(aoi);
		}
		for (FAOIData popup : GA.PopupAOIS)
		{
			Header->PopupHistory.Add(popup);
		}
	}

	//save gaze groups info
	Header->GazeGroupList = ASynOpticonState::GetGazeGroupData();
	//save settings
	Header->Settings = ASynOpticonState::GetSettings();

	//------------------------set up tick block-----------------------------
	for (CompressedGazeBlock* GazeBlock : GazeBlocks)
	{
		TickBlock.GazeActorData.Add(GazeBlock);
	}
	for (CompressedSOBlock* SOBlock : SOBlocks)
	{
		TickBlock.SOActorData.Add(SOBlock);
	}
	
	//----------------------------set up archives----------------------------
	EmptyArchives(); //clean up archives just in case
	ToBinaryArchives.Add(new FBufferArchive()); //active archive
	ToBinaryArchives.Add(new FBufferArchive()); //secondary archive

	//-------------------------set up controlling data-----------------------
	NextLogFileNumber = 0;
	FirstSample = true;

	//---------------after finishing setting up, start recording------------
	ASynOpticonState::SetRecordingStatus(true);

	Header->StartTimestamp = FDateTime::UtcNow();
	ASynOpticonState::SetCurrentReplayTime(FDateTime::UtcNow());
}

void URecordManager::StopRecording()
{
	//whatever it is doing, stop recording and save last data
	FSaveWorker::Shutdown();
	ActiveArchive = (ActiveArchive + 1) % 2;
	SecondaryArchive = (SecondaryArchive + 1) % 2;

	SaveSubReplayFile();
	
	//save header
	Header->SynOpticonActorDataList = UCreationAndDeletionManager::GetCADManagerInstance()->StopActorsFromRecording();
	Header->NumOfLogFiles = NextLogFileNumber;
	Header->ReplayTimeInSeconds = (FDateTime::UtcNow() - Header->StartTimestamp).GetTotalSeconds();
	SaveHeaderFile();
	if (Header)
	{
		delete Header;
		Header = nullptr;
	}

	//clean up archives
	EmptyArchives();
	CleanupTickBlock();
	ASynOpticonState::SetRecordingStatus(false);
}

//============================clean up functions====================================
void URecordManager::EmptyArchives()
{
	for (int i = 0; i < ToBinaryArchives.Num(); i++)
	{
		if (ToBinaryArchives[i])
		{
			ToBinaryArchives[i]->FlushCache();
			ToBinaryArchives[i]->Empty();
			delete ToBinaryArchives[i];
			ToBinaryArchives[i] = nullptr;
		}
	}
	ToBinaryArchives.Empty();
	ActiveArchive = 0;
	SecondaryArchive = 1;
}

void URecordManager::CleanupTickBlock()
{
	FirstSample = true;

	for (CompressedGazeBlock* GazeBlock : TickBlock.GazeActorData)
	{
		if (GazeBlock->Texture)
		{
			FMemory::Free(GazeBlock->Texture);
			GazeBlock->Texture = nullptr;
		}
	}

	for (CompressedSOBlock* SOBlock : TickBlock.SOActorData)
	{
		if (SOBlock->SceneCameraImageBuffer)
		{
			FMemory::Free(SOBlock->SceneCameraImageBuffer);
			SOBlock->SceneCameraImageBuffer = nullptr;
		}

		for (HandDataBlock* HandData : SOBlock->HandDataBlocks) {
			delete (HandData);
		}
	}

	TickBlock.GazeActorData.Empty();
	TickBlock.SOActorData.Empty();
}

//===========================supporting functions===================================
void URecordManager::SaveTimeIndex()
{
	TimeIndexStruct TimeIndex;
	TimeIndex.TimeStamp = TickBlock.DateTime;
	TimeIndex.PlayIndex = ToBinaryArchives[ActiveArchive]->TotalSize();
	TimeIndex.LastGAKeyFrameIndex = -1; //TODO: check the has key frame again, it is used for saving screen's content
	TimeIndex.HasGAKeyFrame = false; //TODO: check the has key frame again, it is used for saving screen's content
	//HasGAKeyFrame = 0; //TODO: check the has key frame again, it is used for saving screen's content

	Header->TimeIndex.Last(0).Add(TimeIndex);
}

void URecordManager::SaveLoadBlockData(FArchive& Ar, FCompressedTickBlock& Block)
{
	ReadMemoryCriticalSection.Lock();
	Ar << Block;
	ReadMemoryCriticalSection.Unlock();
}

void URecordManager::SaveSubReplayFile()
{
	TArray<uint8> CompressedData;
	FArchiveSaveCompressedProxy* Compressor;
	Compressor = new FArchiveSaveCompressedProxy(CompressedData, ECompressionFlags::COMPRESS_ZLIB);
	*Compressor << *ToBinaryArchives[SecondaryArchive];
	Compressor->Flush();

	//create file name
	FString Name = (NextLogFileNumber == 0) ? (FileName + ".re") : (FileName + "." + FString::FromInt(NextLogFileNumber));

	FFileHelper::SaveArrayToFile(CompressedData, *Name);

	NextLogFileNumber++;

	Compressor->FlushCache();
	CompressedData.Empty();
	delete Compressor;
	Compressor = nullptr;

	if (!ToBinaryArchives[SecondaryArchive]) {
		ToBinaryArchives[SecondaryArchive]->Flush();
		ToBinaryArchives[SecondaryArchive]->Empty();
		delete ToBinaryArchives[SecondaryArchive];
	}
	ToBinaryArchives[SecondaryArchive] = new FBufferArchive();
	ToBinaryArchives[SecondaryArchive]->Seek(0); //this is important, otherwise it would keep writing at the previous index, even though we have deleted its content.
}

void URecordManager::SwitchAndSaveActiveArchive()
{
	ActiveArchive = (ActiveArchive + 1) % 2;
	SecondaryArchive = (SecondaryArchive + 1) % 2;

	FSaveWorker::Shutdown();
	FSaveWorker::JoyInit();
}


//============================RECORD WORKER=============================
FSaveWorker* FSaveWorker::SaveWorker = nullptr;

FSaveWorker::FSaveWorker()
{
	SaveThread = FRunnableThread::Create(this, TEXT("FSaveWorker"), 0, TPri_BelowNormal);
}

FSaveWorker::~FSaveWorker()
{
	if (SaveThread)
	{
		delete SaveThread;
		SaveThread = nullptr;
	}
}

uint32 FSaveWorker::Run()
{
	URecordManager::GetRecordManagerInstance()->SaveSubReplayFile();
	
	return 1;
}

void FSaveWorker::EnsureCompletion()
{
	Stop();
	SaveThread->WaitForCompletion();
}

FSaveWorker* FSaveWorker::JoyInit()
{
	if (!SaveWorker && FPlatformProcess::SupportsMultithreading())
	{
		SaveWorker = new FSaveWorker();
	}
	return SaveWorker;
}

void FSaveWorker::Shutdown()
{
	if (SaveWorker)
	{
		SaveWorker->EnsureCompletion();
		delete SaveWorker;
		SaveWorker = nullptr;
	}
}