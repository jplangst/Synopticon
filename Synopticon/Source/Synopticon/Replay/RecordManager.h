// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//Encode and decode images, used to free memory in this case
#include "webp/encode.h"
#include "webp/decode.h"

//get information 
#include "SystemClasses/CreationAndDeletionManager.h"

#include "BufferArchive.h"
#include "ArchiveSaveCompressedProxy.h"
//header file to save and load binary
#include "ReplayHeaderJSONExporter.h"
#include "ReplayDataStructures.h"
#include "RecordManager.generated.h"

/**
 * 
 */
UCLASS()
class SYNOPTICON_API URecordManager : public UActorComponent
{
	GENERATED_BODY()

private: 
	//singleton
	static URecordManager* Instance;
	UWorld* WorldPtr;

	//-----------files-------------
	FString FileName;
	int NextLogFileNumber;

	//-----------header------------
	CompressedHeader* Header;

	void SaveHeaderFile();
	//------------tick-------------
	FCompressedTickBlock TickBlock;
	FCriticalSection ReadMemoryCriticalSection;
	//-------- controller ---------
	bool FirstSample;
	FDateTime TimeSinceLastSave;

	//-----------buffer------------
	TArray<FBufferArchive*> ToBinaryArchives;
	int ActiveArchive;
	int SecondaryArchive;

	//-----------------private functions--------------------
	void EmptyArchives();
	void CleanupTickBlock();

	void SaveTimeIndex();
	void SaveLoadBlockData(FArchive& Ar, FCompressedTickBlock& Block);

	void SwitchAndSaveActiveArchive();

public:
	URecordManager();
	~URecordManager();

	// Component functions
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	//getter
	UFUNCTION(BlueprintPure, Category = "Record Manager")
	static URecordManager* GetRecordManagerInstance();
	static void SetInstance(URecordManager* _Instance);

	//information about file
	UFUNCTION(BlueprintPure, Category = "Record Manager")
	FString GetFileName();

	UFUNCTION(BlueprintPure, Category = "Record States")
	FDateTime GetStartTime();

	//Record
	UFUNCTION(BlueprintCallable, Category = "Record Manager")
	void StartRecording(FString _FileName);

	UFUNCTION(BlueprintCallable, Category = "Record Manager")
	void StopRecording();

	//-----------save----------------
	void SaveSubReplayFile();

	//------------------------events---------------------------
	UFUNCTION(BlueprintPure, Category = "Record Manager")
	TArray<FReplayEventStruct> GetReplayEvents();
	UFUNCTION(BlueprintCallable, Category = "Record Manager")
	void SetReplayEvents(TArray<FReplayEventStruct> ReplayEvents);

	//------------------------experiment-----------------------
	UFUNCTION()
		void OnNewImageAndAOIs(FImageAndAOIs Data);
	UFUNCTION()
		void OnPopupEvent(FPopupAndAOIs Data);

	//----------------------Micro Task------------------------------
	UFUNCTION()
		void OnMicroTaskEvent(MicroTaskEventTypeEnum EventType, FActorTaskStruct EventData);
};

//============================RECORD WORKER=============================
class SYNOPTICON_API FSaveWorker : public FRunnable
{
	/** Singleton instance **/
	static  FSaveWorker* SaveWorker;

	/** Thread to run the worker FRunnable on **/
	FRunnableThread* SaveThread;

public:
	FSaveWorker();
	virtual ~FSaveWorker();

	// Begin FRunnable interface.
	virtual uint32 Run();
	// End FRunnable interface

	/** Makes sure this thread has stopped properly */
	void EnsureCompletion();

	static FSaveWorker* JoyInit();

	/** Shuts down the thread. Static so it can easily be called from outside the thread context */
	static void Shutdown();
};
