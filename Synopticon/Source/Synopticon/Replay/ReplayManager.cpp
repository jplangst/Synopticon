// Fill out your copyright notice in the Description page of Project Settings.

#include "ReplayManager.h"

//A threshold to discard the delta time if it is too big. Meaning something caused the computer to take a long time processing this frame.
float DELTA_TIME_THRESHOLD = 0.5;

FTimespan DELTA_TRIGGERING_EVENT_THRESHOLD_UP = FTimespan::FromMilliseconds(25);
FTimespan DELTA_TRIGGERING_EVENT_THRESHOLD_BOTTOM = FTimespan::FromMilliseconds(-25);

//================================SINGLETON======================================
UReplayManager* UReplayManager::Instance = nullptr;

UReplayManager* UReplayManager::GetReplayManagerInstance()
{
	return Instance;
}

void UReplayManager::SetInstance(UReplayManager* _Instance)
{
	Instance = _Instance;
}

//================================Constructor - Destructor========================
UReplayManager::UReplayManager()
{
	PrimaryComponentTick.bCanEverTick = true;

	Header = nullptr;
	FromBinary = nullptr;
}

UReplayManager::~UReplayManager()
{
	if (Header)
	{
		delete Header;
		Header = nullptr;
	}
	CleanupFromBinary();
	EmptyArchives();
}

//==============================Component functions================================
void UReplayManager::BeginPlay()
{
	Super::BeginPlay();
	CurrentImageAndAOIs = 0;
	CurrentPopup = 0;
	CurrentMicroTaskEvent = 0;

	bStreamDuringReplay = true;
}

void UReplayManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (DeltaTime > DELTA_TIME_THRESHOLD) //if a frame takes too long, discard
	{
		return;
	}

	if (bShouldDoSeeking)
	{
		if (CurrentLogFile != SeekingFile) //the chosen time is in another file
		{
			CurrentLogFile = NextLogFileNumber = SeekingFile;
			//load new file
			FLoadWorker::Shutdown();
			LoadSubReplayFile();
			SwitchAndLoadArchive(); //this functions switch archives and load the next file 
		}
		PlayIndex = LastGAKeyFrameIndex;
		FromBinary->Seek(PlayIndex);
		
		//process to get to the chosen time
		bool bIsProcessingReplayData = true;
		while (bIsProcessingReplayData)
		{
			//We reached the end of the replay
			if (PlayIndex >= FromBinary->TotalSize() && (NextLogFileNumber > Header->NumOfLogFiles))
			{
				bShouldDoSeeking = false;
				return;
			}
			else if (PlayIndex >= FromBinary->TotalSize()) {
				SwitchAndLoadArchive(); 
				CurrentLogFile++; 
			}

			SaveLoadBlockData(*FromBinary, TickBlock);

			PlayIndex = FromBinary->Tell();
			FromBinary->Seek(PlayIndex);

			if (PlayIndex > SeekingIndex)
			{
				bIsProcessingReplayData = false;
			}
		}

		bShouldDoSeeking = false;

		TimeUntilNextTick = 0;
		PlayFrameTimer = 0;
		PrevTickTimespan = TickBlock.DateTime.GetTimeOfDay();
		ASynOpticonState::SetCurrentReplayTime(TickBlock.DateTime);

		//image
		CurrentImageAndAOIs = 0;
		if (ScreenImageHistory.Num() > 1) {
			for (auto& Elem : ScreenImageHistory)
			{
				for (int i = Elem.Value.Num() - 1; i >= 0; i--)
				{
					if (Elem.Value[i].Key < TickBlock.DateTime)
					{
						FImageAndAOIs Event = Elem.Value[i].Value;
						ASynOpticonState::GetGlobalEventSystem()->OnReceiveNewImageAndAOIsDelegate.Broadcast(Event);
						CurrentImageAndAOIs = i + 1;
						break;
					}
				}
			}
		}

		//popup
		if (PopupHistory.Num() > 1)
		{	
			CurrentPopup = 0;
			int UpdateEvent = -1;
			for (auto& Elem : PopupHistory)
			{
				for (int i = Elem.Value.Num() - 1; i >= 0; i--)
				{
					if (Elem.Value[i].Key < TickBlock.DateTime)
					{
						FPopupAndAOIs Event = Elem.Value[i].Value;
						if (!Event.Action.Equals("Update")) {
							ASynOpticonState::GetGlobalEventSystem()->OnReceivePopupAndAOIsDelegate.Broadcast(Event);
							CurrentImageAndAOIs = i + 1;
							if (UpdateEvent > -1)
							{
								ASynOpticonState::GetGlobalEventSystem()->OnReceivePopupAndAOIsDelegate.Broadcast(Elem.Value[UpdateEvent].Value);
								CurrentImageAndAOIs = UpdateEvent + 1;
							}
							break;
						}
						else {
							UpdateEvent = i;
						}
					}
				}
			}
			for (int p = 0; p < Header->PopupEvents.Num(); p++)
			{
				if ((Header->PopupEvents[p].Key - TickBlock.DateTime) >= DELTA_TRIGGERING_EVENT_THRESHOLD_BOTTOM &&
					(Header->PopupEvents[p].Key - TickBlock.DateTime) < DELTA_TRIGGERING_EVENT_THRESHOLD_UP)
				{
					ASynOpticonState::GetGlobalEventSystem()->OnReceivePopupAndAOIsDelegate.Broadcast(Header->PopupEvents[p].Value);
					CurrentPopup = p + 1;
					break;
				}
			}
		}

		//MicroTask Events
		CurrentMicroTaskEvent = 0;
		for (TPair<MicroTaskEventTypeEnum, FActorTaskStruct> TaskPair : Header->MicroTaskEvents) {
			HandleMicroTaskEventReplay(TaskPair);
		}

		ASynOpticonState::GetGlobalEventSystem()->OnSeekingEvent.Broadcast("ReplayManager");
	}

	if (ASynOpticonState::IsReplaying() && !ASynOpticonState::IsPausingReplay() && FromBinary)
	{
		//As long as the current play index is smaller than the total size of the archive
		if (PlayIndex < FromBinary->TotalSize())
		{
			if (PlayFrameTimer < TimeUntilNextTick)
			{
				PlayFrameTimer += DeltaTime;
				return;
			}
			PlayFrameTimer -= TimeUntilNextTick;
			PlayFrameTimer += DeltaTime;

			//Load the recorded data into the TickData structure
			SaveLoadBlockData(*FromBinary, TickBlock);

			//set current replay time
			ASynOpticonState::SetCurrentReplayTime(TickBlock.DateTime);

			//we calculate the time difference between the current frame and the next frame
			FTimespan NextTimespan = TickBlock.DateTime.GetTimeOfDay();
			TimeUntilNextTick = (NextTimespan - PrevTickTimespan).GetTotalSeconds();
			PrevTickTimespan = NextTimespan;

			//Calculate the next location in the archive and seekto it
			PlayIndex = FromBinary->Tell();
			FromBinary->Seek(PlayIndex);

			//Update the visual of the seeking bar
			if (Header && Header->TimeIndex.Num() > 0)
			{
				float time = ((float)(TickBlock.DateTime - Header->StartTimestamp).GetTotalMilliseconds() - ReplaySyncStartTime * 1000)
					/ (float)(ReplaySyncDuration * 1000);
				ASynOpticonState::GetGlobalEventSystem()->OnEventIsReplaying.Broadcast(time);
			}


			//image
			if (CurrentImageAndAOIs < Header->ChangeImageEvents.Num())
			{
				if ((Header->ChangeImageEvents[CurrentImageAndAOIs].Key - TickBlock.DateTime) >= DELTA_TRIGGERING_EVENT_THRESHOLD_BOTTOM &&
					(Header->ChangeImageEvents[CurrentImageAndAOIs].Key - TickBlock.DateTime) < DELTA_TRIGGERING_EVENT_THRESHOLD_UP)
				{
					FImageAndAOIs Event = Header->ChangeImageEvents[CurrentImageAndAOIs].Value;
					ASynOpticonState::GetGlobalEventSystem()->OnReceiveNewImageAndAOIsDelegate.Broadcast(Event);
					CurrentImageAndAOIs += 1;
				}
			}
			//Micro task events
			if (Header->MicroTaskEvents.Num() > CurrentMicroTaskEvent) {
				HandleMicroTaskEventReplay(Header->MicroTaskEvents[CurrentMicroTaskEvent]);
			}		

			//Check experiment event
			//image
			if (CurrentImageAndAOIs < Header->ChangeImageEvents.Num())
			{
				if ((Header->ChangeImageEvents[CurrentImageAndAOIs].Key - TickBlock.DateTime) >= DELTA_TRIGGERING_EVENT_THRESHOLD_BOTTOM &&
					(Header->ChangeImageEvents[CurrentImageAndAOIs].Key - TickBlock.DateTime) < DELTA_TRIGGERING_EVENT_THRESHOLD_UP)
				{
					FImageAndAOIs Event = Header->ChangeImageEvents[CurrentImageAndAOIs].Value;
					ASynOpticonState::GetGlobalEventSystem()->OnReceiveNewImageAndAOIsDelegate.Broadcast(Event);
					CurrentImageAndAOIs += 1;
				}
			}
			//popup
			if (CurrentPopup < Header->PopupEvents.Num())
			{
				if ((Header->PopupEvents[CurrentPopup].Key - TickBlock.DateTime) >= DELTA_TRIGGERING_EVENT_THRESHOLD_BOTTOM &&
					(Header->PopupEvents[CurrentPopup].Key - TickBlock.DateTime) < DELTA_TRIGGERING_EVENT_THRESHOLD_UP)
				{
					ASynOpticonState::GetGlobalEventSystem()->OnReceivePopupAndAOIsDelegate.Broadcast(Header->PopupEvents[CurrentPopup].Value);
					CurrentPopup += 1;
				}
			}

			return;
		}
		else //reach the end of the current file
		{
			if (NextLogFileNumber > Header->NumOfLogFiles) //if it is the last file
			{
				ASynOpticonState::SetPausingReplayStatus(true); //pause and wait
				return;
			}
			//if there are still other files
			SwitchAndLoadArchive(); //we always load one file ahead, so at this point, the secondary archive is having the next file. Switch it with the active one.
			
			CurrentLogFile++; //current log file is the next one
		}
	}
}

void UReplayManager::HandleMicroTaskEventReplay(TPair<MicroTaskEventTypeEnum, FActorTaskStruct> TaskPair) {
	MicroTaskEventTypeEnum EventType = TaskPair.Key;
	FActorTaskStruct EventData = TaskPair.Value;
	if (EventType == MicroTaskEventTypeEnum::TaskComplete) {
		if ((EventData.EndTimestamp - TickBlock.DateTime) >= DELTA_TRIGGERING_EVENT_THRESHOLD_BOTTOM &&
			(EventData.EndTimestamp - TickBlock.DateTime) < DELTA_TRIGGERING_EVENT_THRESHOLD_UP)
		{
			ASynOpticonState::GetGlobalEventSystem()->OnMicroTaskEventDelegate.Broadcast(EventType, EventData);
		}
	}
	else if (EventType == MicroTaskEventTypeEnum::SubtaskComplete) {
		if ((EventData.EndTimestamp - TickBlock.DateTime) >= DELTA_TRIGGERING_EVENT_THRESHOLD_BOTTOM &&
			(EventData.EndTimestamp - TickBlock.DateTime) < DELTA_TRIGGERING_EVENT_THRESHOLD_UP)
		{
			ASynOpticonState::GetGlobalEventSystem()->OnMicroTaskEventDelegate.Broadcast(EventType, EventData);
		}
	}
	else {
		if ((EventData.StartTimestamp - TickBlock.DateTime) >= DELTA_TRIGGERING_EVENT_THRESHOLD_BOTTOM &&
			(EventData.StartTimestamp - TickBlock.DateTime) < DELTA_TRIGGERING_EVENT_THRESHOLD_UP)
		{
			ASynOpticonState::GetGlobalEventSystem()->OnMicroTaskEventDelegate.Broadcast(EventType, EventData);
		}
	}
	CurrentMicroTaskEvent++;
}

//================================= replay ========================================
void UReplayManager::StartReplaying(FString _FileName)
{
	//------------keeping track of the filename------------
	ReplayFileName = _FileName;
	ReplayFileName.RemoveFromEnd(".re");

	//load header
	if (Header)
	{
		delete Header;
	}
	Header = new CompressedHeader();
	*Header = LoadHeaderFile(_FileName);

	ASynOpticonState::SetReplayingStatus(true);

	//---------- stop real-time data streaming ------------
	//we don't want the real-time data messing with our saved data
	if (!bStreamDuringReplay) {
		//FWAMPWorker::Shutdown();
	}
	
	//----load textures and synchronization information----
	ReplaySyncDuration = Header->ReplayTimeInSeconds;
	ReplaySyncStartTime = 0;
	if (!ASynOpticonState::GetScreenLabeling()) //labeling flag is set to true during synchronization. It is false when we play replay
	{
		bTextureDataLoaded = LoadReplayTextureFile();
		bSyncDataLoaded = LoadReplaySyncFile();
	}

	//----------create actors from loaded header-----------
	TArray<CompressedGazeBlock*> GazeBlocks;
	TArray<CompressedSOBlock*> SOBlocks;
	UCreationAndDeletionManager::GetCADManagerInstance()->PrepareActorsBeforeReplaying(Header->GazeActorDataList, Header->GazeGroupList, Header->SynOpticonActorDataList,
		&GazeBlocks, &SOBlocks);

	for (CompressedGazeBlock* GB : GazeBlocks)
	{
		TickBlock.GazeActorData.Add(GB);
	}
	for (CompressedSOBlock* SB : SOBlocks)
	{
		TickBlock.SOActorData.Add(SB);
	}

	ASynOpticonState::ClearGazeGroup();
	for (FGazeGroupData GG : Header->GazeGroupList)
	{
		ASynOpticonState::AddGazeGroup(GG.GroupName, GG.GroupID);
	}

	//------------reset controlling variables--------------
	NextLogFileNumber = 0;
	CurrentLogFile = 0;

	PlayIndex = 0;
	ActiveArchive = 0;
	SecondaryArchive = 1;

	FirstSample = true;
	PlayFrameTimer = 0;
	TimeUntilNextTick = 0;
	if (Header->TimeIndex.Num() > 0 && Header->TimeIndex[0].Num() > 0)
	{
		PrevTickTimespan = Header->TimeIndex[0][0].TimeStamp.GetTimeOfDay();
	}

	//-------------------set up archives-------------------
	DecompressedBinaryArray.Add(FBufferArchive()); //active archive
	DecompressedBinaryArray.Add(FBufferArchive()); //secondary archive

	//--------------load 2 first replay files--------------
	LoadSubReplayFile(); //load first file into the secondary archive
	SwitchAndLoadArchive(); //swap the active and the secondary, the first replay file is now in the active archive. Load the second replay file (if any) into the secondary archive

	//-----------------set mediate variables----------------
	ASynOpticonState::ReplayHeader = *Header;
	ASynOpticonState::ReplaySyncData = ReplaySyncData;

	if (bSyncDataLoaded) 
	{
		SeekingTo(0); //seek to the beginning of the synchronization, it also pauses the replay
	}
	else
	{
		ASynOpticonState::SetPausingReplayStatus(false); //reset flag just in case something is messed up
	}

	CurrentImageAndAOIs = 0;
	CurrentPopup = 0;
	CreateScreenImageHistory();
	CreatePopupHistory();
}

void UReplayManager::StopReplaying()
{
	//-------------stop actors-------------
	UCreationAndDeletionManager::GetCADManagerInstance()->StopActorsFromReplaying();
	Header->SynOpticonActorDataList = UCreationAndDeletionManager::GetCADManagerInstance()->StopActorsFromRecording();
	ASynOpticonState::ClearGazeGroup();
	SaveHeaderFile(Header, ReplayFileName); //override header in case we need to save new events
	if (Header)
	{
		delete Header;
		Header = nullptr;
	}

	//----------clean up memory------------
	CleanupFromBinary();
	EmptyArchives();
	CleanupTickBlock();

	//-------------reset flags-------------
	PlayIndex = 0;
	ActiveArchive = 0;
	SecondaryArchive = 1;
	bTextureDataLoaded = false;
	bSyncDataLoaded = false;

	ASynOpticonState::SetReplayingStatus(false);
	ASynOpticonState::SetPausingReplayStatus(true);
	ReplayFileName = "";
	//---------- restart real-time data streaming ------------
	if (!bStreamDuringReplay) {
		//FWAMPWorker::JoyInit(ASynOpticonState::GetWAMPRouterAdress(), ASynOpticonState::GetWAMPRealm());
	}
}

//============================= handle replay =====================================
void UReplayManager::SeekingTo(float Percentage)
{
	//find the closest playable index (the beginning of a tick block) 
	//    start     sync      chosen
	//      |---------|----------*--------------------------|
	//                 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	//						   replay sync duration
	FDateTime chosenTimestamp = Header->StartTimestamp + FTimespan::FromSeconds(ReplaySyncStartTime + ReplaySyncDuration*Percentage);

	//find the correct file
	int FileIndex = 0;
	bool Found = false;
	for (int i = 0; i < Header->TimeIndex.Num(); i++)
	{	
		//if the beginning time of a file is bigger that the chosen time, it means that we picked a time from previous file
		if (Header->TimeIndex[i][0].TimeStamp > chosenTimestamp)
		{
			FileIndex = i - 1;
			Found = true;
			break;
		}
	}
	//if cannot find, the chosen time is in the last file
	if (!Found)
	{
		FileIndex = Header->TimeIndex.Num() - 1;
	}

	//find the playable index in the chosen file	
	uint64 chosenTime = 0;

	float GapTime = (chosenTimestamp - Header->TimeIndex[FileIndex][0].TimeStamp).GetTotalSeconds();
	float TotalFileTime = (Header->TimeIndex[FileIndex].Last(0).TimeStamp - Header->TimeIndex[FileIndex][0].TimeStamp).GetTotalSeconds();

	//if (FileIndex == 0 && bSyncDataLoaded) //if the chosen file is the first one, the starttimestamp of this file is not the first index, but the synchronization
	//{
	//	GapTime = GapTime - ReplaySyncStartTime;
	//	TotalFileTime = TotalFileTime - ReplaySyncStartTime;
	//}

	GapTime = (GapTime > 0) ? GapTime : 0; //we don't want negative gaptime
	float subPercentage = GapTime / TotalFileTime;
	if (subPercentage > 1 || subPercentage < 0) {
		return;
	}

	//TODO: this code only works with the assumption that timeindices were distributed evenly
	int closestIndex = (Header->TimeIndex[FileIndex].Num() - 1) * subPercentage;

	if (closestIndex >= 0 && closestIndex < (Header->TimeIndex[FileIndex].Num()))
	{
		SeekingIndex = Header->TimeIndex[FileIndex][closestIndex].PlayIndex;
		LastGAKeyFrameIndex = Header->TimeIndex[FileIndex][closestIndex].LastGAKeyFrameIndex;
		LastGAKeyFrameIndex = (LastGAKeyFrameIndex < 0) ? SeekingIndex : LastGAKeyFrameIndex;
		SeekingFile = FileIndex;
		bShouldDoSeeking = true;
		ASynOpticonState::SetPausingReplayStatus(true);
	}
}

FDateTime UReplayManager::GetCurrentReplayTime()
{
	return TickBlock.DateTime;
}

FDateTime UReplayManager::GetReplayStartTime()
{
	if (Header)
	{
		if (bSyncDataLoaded)
		{
			return  Header->StartTimestamp + FTimespan::FromSeconds(ReplaySyncStartTime);
		}
		else {
			return Header->StartTimestamp;
		}
	}
	return FDateTime::UtcNow();
}

float UReplayManager::GetTotalReplayTimeInSeconds()
{
	if (Header)
	{
		return Header->ReplayTimeInSeconds;
	}
	return 0;
}


//===============================supporting functions===============================
void UReplayManager::SaveLoadBlockData(FArchive& Ar, FCompressedTickBlock& Block)
{
	ReadMemoryCriticalSection.Lock();
	Ar << Block;
	ReadMemoryCriticalSection.Unlock();
}

void UReplayManager::LoadSubReplayFile()
{
	FString Name = (NextLogFileNumber == 0)? (ReplayFileName + ".re") : (ReplayFileName + "." + FString::FromInt(NextLogFileNumber));
	NextLogFileNumber++;

	TArray<uint8> CompressedData;
	if (!FFileHelper::LoadFileToArray(CompressedData, *Name))
	{
		return; //invalid file
	}

	FArchiveLoadCompressedProxy* Decompressor;

	//Decompressor = new FArchiveLoadCompressedProxy(CompressedData, "DXT", ECompressionFlags::COMPRESS_ZLIB);
	Decompressor = new FArchiveLoadCompressedProxy(CompressedData, NAME_Zlib);
	if (Decompressor->GetError())
	{
		return;
	}
	
	DecompressedBinaryArray[SecondaryArchive].Empty();
	*Decompressor << DecompressedBinaryArray[SecondaryArchive];

	//clean up after loading
	CompressedData.Empty();
	if (Decompressor)
	{
		Decompressor->FlushCache();
		delete Decompressor;
	}
	return;
}

void UReplayManager::SwitchAndLoadArchive()
{
	ActiveArchive = (ActiveArchive + 1) % 2;
	SecondaryArchive = (SecondaryArchive + 1) % 2;

	//----------------set up memory reader-----------------
	PlayIndex = 0; //start at the beginning of the archive
	CleanupFromBinary(); //clean up just in case
	FromBinary = new FMemoryReader(DecompressedBinaryArray[ActiveArchive], true);
	FromBinary->Seek(PlayIndex); //reset frombinary. It is important

	//use worker to load the new file in a seperated thread
	FLoadWorker::Shutdown();
	FLoadWorker::JoyInit();
}

//=========================== header related functions==============================
CompressedHeader UReplayManager::LoadHeaderFile(FString _FileName)
{
	FString HeaderFileName = _FileName;
	HeaderFileName.RemoveFromEnd(".re");
	HeaderFileName += ".header";

	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *HeaderFileName);
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	TSharedPtr<FJsonObject> HeaderData = MakeShareable(new FJsonObject);

	CompressedHeader _Header;

	if (FJsonSerializer::Deserialize(Reader, HeaderData))
	{
		_Header = UReplayHeaderJSONExporter::GetHeaderData(HeaderData);
	}
	else //it is in binary format?
	{
		//SAVE THIS PART FOR LATER WHEN WE WANT TO GO BACK TO USING BINARY
		TArray<uint8> CompressedHeaderData;
		if (!FFileHelper::LoadFileToArray(CompressedHeaderData, *HeaderFileName))
		{
			return _Header;
		}

		FArchiveLoadCompressedProxy HeaderDecompressor =
			FArchiveLoadCompressedProxy(CompressedHeaderData, "DXT", ECompressionFlags::COMPRESS_ZLIB);

		if (HeaderDecompressor.GetError())
		{
			return _Header;
		}

		FBufferArchive DecompressedHeaderBinaryArray;
		HeaderDecompressor << DecompressedHeaderBinaryArray;

		FMemoryReader FromHeaderBinary = FMemoryReader(DecompressedHeaderBinaryArray, true); //true, free data after done
		FromHeaderBinary.Seek(0);
		SaveLoadHeaderData(FromHeaderBinary, _Header);

		CompressedHeaderData.Empty();
		HeaderDecompressor.FlushCache();
		FromHeaderBinary.FlushCache();

		// Empty & Close Buffer 
		DecompressedHeaderBinaryArray.Empty();
		DecompressedHeaderBinaryArray.Close();
	}

	return _Header;
}

void UReplayManager::SaveHeaderFile(CompressedHeader* _Header, FString _FileName)
{
	if (!_Header)
	{
		return;
	}

	FString HeaderFile = _FileName;
	HeaderFile.RemoveFromEnd(".re");
	HeaderFile = HeaderFile + ".header";

	TSharedPtr<FJsonObject> HeaderJson = UReplayHeaderJSONExporter::SaveHeaderData(*_Header);
	FString OutputString;
	TSharedRef< TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(HeaderJson.ToSharedRef(), Writer);

	FFileHelper::SaveStringToFile(OutputString, *HeaderFile);
}

TArray<FReplayEventStruct> UReplayManager::GetReplayEvents()
{
	if (Header)
	{
		return Header->ReplayEvents;
	}
	return TArray<FReplayEventStruct>();
}

void UReplayManager::SetReplayEvents(TArray<FReplayEventStruct> ReplayEvents)
{
	if (Header)
	{
		Header->ReplayEvents = ReplayEvents;
	}
}

void UReplayManager::CreateScreenImageHistory()
{
	//TMap<FString, TArray<TPair<FDateTime, FImageAndAOIs>>> ScreenImageHistory;
	for (FGazeActorData GAData : Header->GazeActorDataList)
	{
		if (!ScreenImageHistory.Contains(GAData.WAMPScreenID))
		{
			ScreenImageHistory.Add(GAData.WAMPScreenID, TArray<TPair<FDateTime, FImageAndAOIs>>());
		}
		FImageAndAOIs Initialization = FImageAndAOIs(GAData.TextureFilepath, "", GAData.WAMPScreenID, GAData.AOIS);
		ScreenImageHistory[GAData.WAMPScreenID].Add(TPair<FDateTime, FImageAndAOIs>(Header->StartTimestamp, Initialization));
	}
	for (TPair<FDateTime, FImageAndAOIs> ImageAndAOIs : Header->ChangeImageEvents)
	{
		if (!ScreenImageHistory.Contains(ImageAndAOIs.Value.ScreenID))
		{
			ScreenImageHistory.Add(ImageAndAOIs.Value.ScreenID, TArray<TPair<FDateTime, FImageAndAOIs>>());
		}
		ScreenImageHistory[ImageAndAOIs.Value.ScreenID].Add(ImageAndAOIs);
	}
}

void UReplayManager::CreatePopupHistory()
{
	//TMap<FString, TArray<TPair<FDateTime, FPopupAndAOIs>>> PopupHistory;
	for (TPair<FDateTime, FPopupAndAOIs> PopupAndAOIs : Header->PopupEvents)
	{
		if (!PopupHistory.Contains(PopupAndAOIs.Value.ScreenID))
		{
			PopupHistory.Add(PopupAndAOIs.Value.ScreenID, TArray<TPair<FDateTime, FPopupAndAOIs>>());
		}
		PopupHistory[PopupAndAOIs.Value.ScreenID].Add(PopupAndAOIs);
	}
}

//deprecated - do not use
void UReplayManager::SaveLoadHeaderData(FArchive& Ar, CompressedHeader& _Header)
{
	Ar << _Header;
}

//=============================== screen's textures ===============================
bool UReplayManager::LoadReplayTextureFile() {

	FString TextureFileName = ReplayFileName + ".tex";

	ReplayTextureData = UReplaySyncJSONExporter::GetReplayTextureData(TextureFileName); //TODO make the struct a second param. Return bool to indicate successfull load instead

	if (!ReplayTextureData.ReplayFilepath.Equals("No Texture")) {
		for (int i = 0; i < Header->GazeActorDataList.Num(); i++) {
			for (int z = 0; z < ReplayTextureData.ScreenTextureData.Num(); z++) {
				if (Header->GazeActorDataList[i].Name.Equals(ReplayTextureData.ScreenTextureData[z].ScreenName)) {
					Header->GazeActorDataList[i].TextureFilepath = ReplayTextureData.ScreenTextureData[z].ScreenTexture;
				}
			}
		}
		return true;
	}
	else {
		return false;
	}
}

//================================ synchronization ================================
bool UReplayManager::LoadReplaySyncFile() {
	ReplaySyncStartTime = 0;
	ReplaySyncDuration = Header->ReplayTimeInSeconds;

	FString SyncFileName = ReplayFileName + ".sync";

	ReplaySyncData = UReplaySyncJSONExporter::GetReplaySyncData(SyncFileName); //TODO make the struct a second param. Return bool to indicate successfull load instead

	if (ReplaySyncData.SOReplaySyncData.ReplaySyncTime != -1 && ReplaySyncData.RecordingSyncData.RecordingSyncTime != -1) {
		ReplaySyncStartTime = ReplaySyncData.SOReplaySyncData.ReplaySyncTime;
		float ReplaySyncedDuration = ReplaySyncData.SOReplaySyncData.ReplayDuration - ReplaySyncStartTime;
		float RecordingSyncedDuration = ReplaySyncData.RecordingSyncData.RecordingDuration - ReplaySyncData.RecordingSyncData.RecordingSyncTime;
		ReplaySyncDuration = FMath::Min(ReplaySyncedDuration, RecordingSyncedDuration); //Use the shortest duration as the common duration

		for (int i = 0; i < Header->GazeActorDataList.Num(); i++) {
			for (int z = 0; z < ReplaySyncData.ScreenLabels.Num(); z++) {
				if (Header->GazeActorDataList[i].Name.Equals(ReplaySyncData.ScreenLabels[z].ScreenName)) {
					Header->GazeActorDataList[i].VideoIndex = ReplaySyncData.ScreenLabels[z].VideoIndex;
				}
			}
		}
		return true;
	}
	else {
		return false;
	}
}

float UReplayManager::GetSyncedReplayTime()
{
	if (bSyncDataLoaded)
	{
		return ReplaySyncDuration;
	}
	else if (Header) {
		return Header->ReplayTimeInSeconds;
	}
	return 0;
}

//====================================textures=====================================
bool UReplayManager::UpdateReplayTextureData(FSOReplayTextureStruct NewNameList)
{
	if (Header)
	{
		TMap<FString, FString> NewScreenTexture;
		for (FScreenTextureStruct NewName : NewNameList.ScreenTextureData)
		{
			NewScreenTexture.Add(NewName.ScreenName, NewName.ScreenTexture);
		}

		for (FGazeActorData& GA : Header->GazeActorDataList)
		{
			GA.TextureFilepath = NewScreenTexture[GA.Name];
		}
		SaveHeaderFile(Header, ReplayFileName);
		return true;

	}
	else
	{
		return false;
	}
}

//===============================clean up functions================================
void UReplayManager::CleanupFromBinary()
{
	if (FromBinary)
	{
		bShouldDoSeeking = false;
		FromBinary->FlushCache();
		FromBinary->Close();
		delete FromBinary;
	}
	FromBinary = nullptr;
}

void UReplayManager::EmptyArchives()
{
	for (int i = 0; i < DecompressedBinaryArray.Num(); i++)
	{
		DecompressedBinaryArray[i].Empty();
		DecompressedBinaryArray[i].Close();
	}
	DecompressedBinaryArray.Empty();
}

void UReplayManager::CleanupTickBlock()
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

//===================================utilities=====================================
bool UReplayManager::ApplyTexturesFromOneFileToOther(FString From, FString To)
{
	CompressedHeader* FromHeader = new CompressedHeader();
	*FromHeader = LoadHeaderFile(From);

	CompressedHeader* ToHeader = new CompressedHeader();
	*ToHeader = LoadHeaderFile(To);

	bool Result = false;
	if (FromHeader->GazeActorDataList.Num() == ToHeader->GazeActorDataList.Num())
	{
		for (int i = 0; i < FromHeader->GazeActorDataList.Num(); i++)
		{
			ToHeader->GazeActorDataList[i].TextureFilepath = FromHeader->GazeActorDataList[i].TextureFilepath;
			ToHeader->GazeActorDataList[i].Vertices = FromHeader->GazeActorDataList[i].Vertices;
			ToHeader->GazeActorDataList[i].UVs = FromHeader->GazeActorDataList[i].UVs;
			ToHeader->GazeActorDataList[i].Name = FromHeader->GazeActorDataList[i].Name;
		}
		SaveHeaderFile(ToHeader, To);
		Result = true;
	}

	delete FromHeader; FromHeader = nullptr;
	delete ToHeader; ToHeader = nullptr;

	return Result;
}

void UReplayManager::ExportData(bool ExportAggregated, bool ExportFixations, bool RelativeTimestamp, FString _FileName)
{
	CompressedHeader _Header;
	if (!Header) {
		_Header = LoadHeaderFile(_FileName);
	}
	else {
		_Header = *Header;
	}

	FString Path, _Name, _Ext;
	if (_FileName.Equals("")) {
		ReplayFileName.Split("/", &Path, &_Name, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	}
	else {
		_FileName.Split("/", &Path, &_Name, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		_Name.Split(".", &_Name, &_Ext, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	}
	DataExporter::ExportData(_Name, &_Header, ExportAggregated, ExportFixations, RelativeTimestamp);
}

bool UReplayManager::LoadHeaderAndCreateActors(FString _FileName)
{
	if (Header)
	{
		delete Header;
	}
	Header = new CompressedHeader();
	*Header = LoadHeaderFile(_FileName);

	UCreationAndDeletionManager::GetCADManagerInstance()->PrepareActorsBeforeReplaying(Header->GazeActorDataList, Header->GazeGroupList, Header->SynOpticonActorDataList,
		nullptr, nullptr);

	return true;
}

//=======================TESTING FUNCTIONALITIES========================
void UReplayManager::ExportBinaryData(FString _ReplayFile)
{
	FBinaryExporterWorker::Shutdown();
	if (!_ReplayFile.Equals("")) {
		FBinaryExporterWorker::JoyInit(_ReplayFile, this);
	}
	else if (!ReplayFileName.Equals("")) {
		FBinaryExporterWorker::JoyInit(ReplayFileName + ".re", this);
	}
}
void UReplayManager::ExportBinaryDataWorker(FString FileName)
{
	try {
		TArray<uint8> _CompressedData;
		FArchiveLoadCompressedProxy* _Decompressor = nullptr;
		FBufferArchive _DecompressedBinaryBuffer;
		FMemoryReader* _BinaryBuffer = nullptr;
		FCompressedTickBlock _DataBlock;

		FString Path, FullName, Name, Ext;
		FileName.Split(".re", &FullName, &Ext, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		FullName.Split("/", &Path, &Name, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		//Read meta data from header file
		CompressedHeader* _Header = new CompressedHeader();
		*_Header = LoadHeaderFile(FileName);

		//Setup DataBlock

		//setup for gaze actors
		for (FGazeActorData GA : _Header->GazeActorDataList)
		{
			CompressedGazeBlock* NewGazeBlock = new CompressedGazeBlock();
			NewGazeBlock->movable = GA.Movable;
			NewGazeBlock->parented = GA.Parented;
			_DataBlock.GazeActorData.Add(NewGazeBlock);
		}
		//setup for so actors
		for (FSynOpticonActorStruct SO : _Header->SynOpticonActorDataList)
		{
			CompressedSOBlock* NewSOBlock = new CompressedSOBlock();
			if (SO.EyeTrackerStruct.GlassesID != "")
			{
				NewSOBlock->HasETGData = true;
			}

			if (SO.RemoteEyeTrackerStruct.RemoteTrackerID != "")
			{
				NewSOBlock->hasRTData = true;
			}

			for (FHandStruct HandCompStruct : SO.HandComponents) {
				HandDataBlock* HandData = new HandDataBlock();
				NewSOBlock->HandDataBlocks.Add(HandData);
			}
			NewSOBlock->NmbHandComponents = SO.HandComponents.Num();

			_DataBlock.SOActorData.Add(NewSOBlock);
		}
		FString ExportedFile = DataExporter::ExportBinaryData(FullName, _Header);
		//ExportBinary
		int FileIndex = 0;
		FString ReadFile = FileName;

		int _PlayIndex = 0;

		do {
			_CompressedData.Empty();
			if (!FFileHelper::LoadFileToArray(_CompressedData, *ReadFile))
			{
				//return; //invalid file
			}
			FileIndex += 1;
			if (_Decompressor)
			{
				_Decompressor->FlushCache();
				delete _Decompressor;
			}
			_Decompressor = new FArchiveLoadCompressedProxy(_CompressedData, "DXT", ECompressionFlags::COMPRESS_ZLIB);
			if (_Decompressor->GetError())
			{
				//return;
			}

			_DecompressedBinaryBuffer.Empty();
			*_Decompressor << _DecompressedBinaryBuffer;

			if (_BinaryBuffer)
			{
				delete _BinaryBuffer;
			}

			_BinaryBuffer = new FMemoryReader(_DecompressedBinaryBuffer, true);
			_BinaryBuffer->Seek(0);
			_PlayIndex = 0;
			int Print = 0;
			FString Rows = "";

			while (_PlayIndex < _BinaryBuffer->TotalSize())
			{
				SaveLoadBlockData(*_BinaryBuffer, _DataBlock);

				_PlayIndex = _BinaryBuffer->Tell();
				_BinaryBuffer->Seek(_PlayIndex);

				//Export to csv
				Rows += DataExporter::GetSOBinaryDataRow(_Header, _DataBlock);
				Print++;
				if (Print >= 500)
				{
					DataExporter::SaveSOBinaryDataBlock(ExportedFile, Rows);
					Print = 0;
					Rows = "";
				}
			}

			ReadFile = FullName + FString::FromInt(FileIndex);
		} while (FileIndex < _Header->NumOfLogFiles);

		if (_Decompressor)
		{
			delete _Decompressor;
			_Decompressor = nullptr;
		}
		if (_BinaryBuffer)
		{
			delete _BinaryBuffer;
			_BinaryBuffer = nullptr;
		}
		//TODO: clean _dataBlock;
		for (CompressedGazeBlock* GazeBlock : _DataBlock.GazeActorData)
		{
			if (GazeBlock->Texture)
			{
				FMemory::Free(GazeBlock->Texture);
				GazeBlock->Texture = nullptr;
			}
		}

		for (CompressedSOBlock* SOBlock : _DataBlock.SOActorData)
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

		_DataBlock.GazeActorData.Empty();
		_DataBlock.SOActorData.Empty();

		if (_Header)
		{
			delete _Header;
			_Header = nullptr;
		}
		AsyncTask(ENamedThreads::GameThread, []() {
			ASynOpticonState::GetGlobalEventSystem()->OnErrorMessage.Broadcast("Data Exportation Done!");
		});
	}
	catch (int e)
	{
		AsyncTask(ENamedThreads::GameThread, [e]() {
			ASynOpticonState::GetGlobalEventSystem()->OnErrorMessage.Broadcast("Data Exportation Failed!" + FString::FromInt(e));
		});
	}
}

//============================REPLAY WORKER=============================
FLoadWorker* FLoadWorker::LoadWorker = nullptr;

FLoadWorker::FLoadWorker()
{
	LoadThread = FRunnableThread::Create(this, TEXT("FLoadWorker"), 0, TPri_BelowNormal);
}

FLoadWorker::~FLoadWorker()
{
	if (LoadThread)
	{
		delete LoadThread;
		LoadThread = nullptr;
	}
}

uint32 FLoadWorker::Run()
{
	UReplayManager::GetReplayManagerInstance()->LoadSubReplayFile();
	return 1;
}

void FLoadWorker::EnsureCompletion()
{
	Stop();
	LoadThread->WaitForCompletion();
}

FLoadWorker* FLoadWorker::JoyInit()
{
	if (!LoadWorker && FPlatformProcess::SupportsMultithreading())
	{
		LoadWorker = new FLoadWorker();
	}
	return LoadWorker;
}

void FLoadWorker::Shutdown()
{
	if (LoadWorker)
	{
		LoadWorker->EnsureCompletion();
		delete LoadWorker;
		LoadWorker = nullptr;
	}
}

FBinaryExporterWorker* FBinaryExporterWorker::BinaryExporterWorker = nullptr;
int64 FBinaryExporterWorker::ID = 0;
FBinaryExporterWorker::FBinaryExporterWorker(FString _ReplayFile, UReplayManager* _ReplayManager)
{
	ReplayFile = _ReplayFile;
	ReplayManager = _ReplayManager;
	BinaryExporterThread = FRunnableThread::Create(this, TEXT("FBinaryExporterWorker" + ID), 0, TPri_BelowNormal);
	ID++;
}

FBinaryExporterWorker::~FBinaryExporterWorker()
{
	if (BinaryExporterThread)
	{
		delete BinaryExporterThread;
		BinaryExporterThread = nullptr;
		IsFinished = true;
	}
}

uint32 FBinaryExporterWorker::Run()
{
	ReplayManager->ExportBinaryDataWorker(ReplayFile);
	return 1;
}

void FBinaryExporterWorker::EnsureCompletion()
{
	if (BinaryExporterThread)
	{
		BinaryExporterThread->WaitForCompletion();
	}
}

FBinaryExporterWorker* FBinaryExporterWorker::JoyInit(FString _ReplayFile, UReplayManager* _ReplayManager)
{
	if (!BinaryExporterWorker && FPlatformProcess::SupportsMultithreading())
	{
		BinaryExporterWorker = new FBinaryExporterWorker(_ReplayFile, _ReplayManager);
	}
	return BinaryExporterWorker;
}

void FBinaryExporterWorker::Shutdown()
{
	if (BinaryExporterWorker)
	{
		BinaryExporterWorker->EnsureCompletion();
		delete BinaryExporterWorker;
		BinaryExporterWorker = nullptr;
	}
}

bool FBinaryExporterWorker::IsThreadFinished()
{
	return false;
}