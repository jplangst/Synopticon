#pragma once

#include "CoreMinimal.h"
#include "Serialization/BufferArchive.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/ArchiveLoadCompressedProxy.h"

//Encode and decode images, used to free memory in this case
#include "webp/encode.h"
#include "webp/decode.h"
#include "../Utils/ReplaySyncJSONExporter.h"
#include "../SystemClasses/SynOpticonState.h"
#include "../SystemClasses/CreationAndDeletionManager.h"
//#include "WAMP/WAMPWorker.h"
#include "../GazeObjects/GazeObjectMaterialComponent.h"
#include "ReplayHeaderJSONExporter.h"
#include "DataExporter.h"

//header file to save and load binary
#include "ReplayDataStructures.h"
#include "ReplayManager.generated.h"

UCLASS()
class SYNOPTICON_API UReplayManager : public UActorComponent
{
	GENERATED_BODY()

private:
	static UReplayManager* Instance;

	//--------------files-------------
	FString ReplayFileName;
	int CurrentLogFile;
	int NextLogFileNumber;
	
	//--------------header------------
	CompressedHeader* Header;

	//--------------tick--------------
	FCompressedTickBlock TickBlock;

	//------controlling variables-----
	FTimespan PrevTickTimespan;
	float TimeUntilNextTick;
	float PlayFrameTimer;
	bool FirstSample;

	//To allow streaming during replays //Note that this is only for testing for StatCo. 
	//This should be set to false by default once the testing is done. In addition the 
	//streaming if checks should remove the isreplaying flag from their checks
	bool bStreamDuringReplay;

	//-------------handle-------------
	bool bShouldDoSeeking;
	int32 SeekingFile;
	int LastGAKeyFrameIndex;
	int SeekingIndex;
	
	//------------archives------------
	int PlayIndex;
	int ActiveArchive;
	int SecondaryArchive;

	//----------memory reader---------
	TArray<FBufferArchive> DecompressedBinaryArray;
	FMemoryReader* FromBinary;
	FCriticalSection ReadMemoryCriticalSection;

	//-------------textures-----------
	bool bTextureDataLoaded;
	FSOReplayTextureStruct ReplayTextureData;
	
	//---------synchronization--------
	float ReplaySyncStartTime;
	float ReplaySyncDuration;
	FSyncReplayStruct ReplaySyncData;
	bool bSyncDataLoaded;

	//--------------experiment-------------
	int CurrentImageAndAOIs;
	int CurrentPopup;
	int CurrentMicroTaskEvent;
	TMap<FString, TArray<TPair<FDateTime, FImageAndAOIs>>> ScreenImageHistory;
	void CreateScreenImageHistory();
	TMap<FString, TArray<TPair<FDateTime, FPopupAndAOIs>>> PopupHistory;
	void CreatePopupHistory();
	

	//----------load supporting files------
	bool LoadReplayTextureFile();
	bool LoadReplaySyncFile();

	//----------cleanup functions----------
	void CleanupTickBlock();
	void CleanupFromBinary();
	void EmptyArchives();

	//-----------load binary---------------
	void SaveLoadBlockData(FArchive& Ar, FCompressedTickBlock& Block);
	void SaveLoadHeaderData(FArchive& Ar, CompressedHeader& Header); //deprecated - do not use
public:
	UReplayManager();
	~UReplayManager();

	// Component functions
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Checks a given MicroTaskEvent pair and broadcasts events if needed
	void HandleMicroTaskEventReplay(TPair<MicroTaskEventTypeEnum, FActorTaskStruct> TaskPair);

	//getter - setter
	UFUNCTION(BlueprintPure, Category = "Replay Manager")
	static UReplayManager* GetReplayManagerInstance();
	static void SetInstance(UReplayManager* _Instance);

	//-----------------------open replay---------------------
	UFUNCTION(BlueprintCallable, Category = "Replay Manager")
	void StartReplaying(FString _FileName);

	UFUNCTION(BlueprintCallable, Category = "Replay Manager")
	void StopReplaying();

	//----------------------handle replay--------------------
	UFUNCTION(BlueprintCallable, Category = "Replay Manager")
	void SeekingTo(float Percentage);

	UFUNCTION(BlueprintPure, Category = "Replay Manager")
	FDateTime GetCurrentReplayTime();

	UFUNCTION(BlueprintPure, Category = "Replay Manager")
	FDateTime GetReplayStartTime();

	//--------------------synchronization--------------------
	UFUNCTION(BlueprintPure, Category = "Replay States")
	float GetSyncedReplayTime();

	UFUNCTION(BlueprintPure, Category = "Replay Manager")
	FSyncReplayStruct GetReplaySyncData() { return ReplaySyncData; }

	UFUNCTION(BlueprintPure, Category = "Replay States")
	bool IsSyncLoaded() { return bSyncDataLoaded; }

	//Duration of replay in seconds
	UFUNCTION(BlueprintPure, Category = "Replay Manager")
	float GetTotalReplayTimeInSeconds();

	//------------------------textures-----------------------
	UFUNCTION(BlueprintCallable, Category = "Replay Manager")
	bool UpdateReplayTextureData(FSOReplayTextureStruct NewNameList);

	//------------------supporting functions-----------------
	void LoadSubReplayFile();
	void SwitchAndLoadArchive();

	//----------------header related functions---------------
	CompressedHeader LoadHeaderFile(FString _FileName);
	void SaveHeaderFile(CompressedHeader* _Header, FString _FileName);

	//------------------------events-------------------------
	UFUNCTION(BlueprintPure, Category = "Replay Manager")
	TArray<FReplayEventStruct> GetReplayEvents();
	UFUNCTION(BlueprintCallable, Category = "Replay Manager")
	void SetReplayEvents(TArray<FReplayEventStruct> ReplayEvents);

	//----------------------utilities------------------------
	UFUNCTION(BlueprintCallable, Category = "Replay Manager")
	bool ApplyTexturesFromOneFileToOther(FString From, FString To);

	UFUNCTION(BlueprintCallable, Category = "Replay Manager")
	void ExportData(bool ExportAggregated, bool ExportFixations, bool ExportGazePath, bool RelativeTime, FString _FileName);

	UFUNCTION(BlueprintCallable, Category = "Replay Manager")
	bool LoadHeaderAndCreateActors(FString _FileName);

	//---------------testing functionalities-----------------
	void ExportBinaryDataWorker(FString FileName);
	UFUNCTION(BlueprintCallable, Category = "Replay Manager")
	void ExportBinaryData(FString FileName);
};

//============================REPLAY WORKER=============================
class SYNOPTICON_API FLoadWorker : public FRunnable
{
	/** Singleton instance **/
	static  FLoadWorker* LoadWorker;

	/** Thread to run the worker FRunnable on **/
	FRunnableThread* LoadThread;

public:	
	FLoadWorker();
	virtual ~FLoadWorker();

	// Begin FRunnable interface.
	virtual uint32 Run();
	// End FRunnable interface

	/** Makes sure this thread has stopped properly */
	void EnsureCompletion();

	static FLoadWorker* JoyInit();

	/** Shuts down the thread. Static so it can easily be called from outside the thread context */
	static void Shutdown();
};


class SYNOPTICON_API FBinaryExporterWorker : public FRunnable
{
	/** Singleton instance **/
	static  FBinaryExporterWorker* BinaryExporterWorker;
	static int64 ID;

	/** Thread to run the worker FRunnable on **/
	FRunnableThread* BinaryExporterThread;

private:
	bool IsFinished;
	UReplayManager* ReplayManager;
	FString ReplayFile;
public:
	FBinaryExporterWorker(FString _ReplayFile, UReplayManager* _ReplayManager);
	virtual ~FBinaryExporterWorker();

	// Begin FRunnable interface.
	virtual uint32 Run();
	// End FRunnable interface

	/** Makes sure this thread has stopped properly */
	void EnsureCompletion();

	static FBinaryExporterWorker* JoyInit(FString _ReplayFile, UReplayManager* _ReplayManager);

	/** Shuts down the thread. Static so it can easily be called from outside the thread context */
	static void Shutdown();

	static bool IsThreadFinished();
};