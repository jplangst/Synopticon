// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"

#include "Components/ActorComponent.h"

//Eye data wamp
#include "WAMP/EyeTrackingWAMPComponent.h"

//NatNet
#include "NatNet/NatNetWAMPComponent.h"

#include "Utils/ReplaySyncJSONExporter.h"
#include "Replay/ReplayDataStructures.h"

//SynOpticon Actors
#include "SynOpticonActors/SynOpticonActorLoader.h"
#include "SynOpticonActors/SynOpticonFactory.h"
#include "SynOpticonActors/SynOpticonActor.h"

//Gaze Actors
#include "GazeObjects/GazeActorDataTypes.h"
#include "GazeObjects/GazeActorLoader.h"
#include "GazeObjects/GazeActorFactory.h"
#include "GazeObjects/GazeActor.h"
#include "GazeObjects/GazeParentActor.h"

#include "CreationAndDeletionManager.generated.h"

/*
 *	Used to manage the creation and the deletion of SynOpticon actors and Gaze actors
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYNOPTICON_API UCreationAndDeletionManager : public UActorComponent
{
	GENERATED_BODY()

	static UCreationAndDeletionManager* Instance;

	//For the creation and removal of synopticon actors
	bool ShouldDeleteSynOpticonActors;
	TArray<FString> SynOpticonActorsToDelete;
	bool ShouldCreateSynOpticonActors;
	TArray<TPair<FSynOpticonActorStruct, bool>> SynOpticonActorsDataToCreateFrom;
	void DeleteSynOpticonActors();
	void CreateSynOpticonActors();

	//For the creation and removal of gaze actors
	bool ShouldDeleteGazeActors;
	TArray<AGazeActor*> GazeActorsToDelete;
	bool ShouldCreateGazeActors;
	TArray<FGazeActorData> GazeActorsDataToCreateFrom;
	void DeleteGazeActors();
	void CreateGazeActors();

	//For the creation and removal of gaze parent actors
	bool ShouldDeleteGazeParentActors;
	TArray<AGazeParentActor*> GazeParentActorsToDelete;
	bool ShouldCreateGazeParentActors;
	TArray<FGazeParentActorData> GazeParentActorsDataToCreateFrom;
	void DeleteGazeParentActors();
	void CreateGazeParentActors();
	bool ShouldDelayRegisterPosOri;

	//Spawns and removes actors depending on set flags
	void HandleActorSpawningAndDeletion();

	FString LoadedModelFile;

public:	
	// Sets default values for this component's properties
	UCreationAndDeletionManager();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GazeActorBP")
		TSubclassOf<AGazeActor> GazeActorBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GazeVisualizationBP")
		TSubclassOf<UGazeDataVisualizerComponent> GazeVizBP;

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;
		
	UFUNCTION(BlueprintCallable, Category = "SynOpticon Actors")
		bool RemoveSynOpticonActor(FString actorName);

	UFUNCTION(BlueprintCallable, Category = "SynOpticon Actors")
		void RemoveAllSynOpticonActors();

	UFUNCTION(BlueprintCallable, Category = "SynOpticon Actors")
		ASynOpticonActor* CreateSynOpticonActorFromStruct(FSynOpticonActorStruct SynOpticonActorStruct, bool ReplayMode);

	UFUNCTION(BlueprintCallable, Category = "SynOpticon Actors")
		void CreateAndReplaceSynOpticonActorFromStruct(FSynOpticonActorStruct SynOpticonActorStruct);

	UFUNCTION(BlueprintCallable, Category = "SynOpticon Actors")
		void EditSynOpticonActor(ASynOpticonActor* Actor, FSynOpticonActorStruct SynOpticonActorStruct);

	UFUNCTION(BlueprintCallable, Category = "SynOpticon Objects")
		void CreateRigidBody(FGazeActorData GazeActorData);

	UFUNCTION(BlueprintCallable, Category = "Gaze Parent Objects")
		void CreateGazeParent(FGazeParentActorData GazeParentData);

	UFUNCTION(BlueprintCallable, Category = "Gaze Parent Objects")
		void RemoveGazeParentActor(AGazeParentActor* GazeParent);

	UFUNCTION(BlueprintCallable, Category = "Gaze Parent Objects")
		void RemoveAllGazeParentActors();

	UFUNCTION(BlueprintCallable, Category = "Gaze Actors")
		void RemoveGazeActor(AGazeActor* gazeActor);

	UFUNCTION(BlueprintCallable, Category = "Gaze Actors")
		bool EditGazeActor(AGazeActor* actor, FGazeActorData newInfo);

	UFUNCTION(BlueprintCallable, Category = "Gaze Actors")
		void CreateAndReplaceRigidBody(FGazeActorData item);

	UFUNCTION(BlueprintCallable, Category = "Gaze Actors")
		void RemoveAllGazeActors();

	UFUNCTION(BlueprintImplementableEvent)
		void ReplaceOrRenameDuplicatedSynOpticonActors(const TArray<FSynOpticonActorStruct>& items);

	UFUNCTION(BlueprintImplementableEvent)
		void ReplaceOrRenameDuplicatedGazeActors(const TArray<FGazeActorData>& items);

	//Returns the loaded models filepath. Returns empty string if nothing is loaded
	UFUNCTION(BlueprintCallable, Category = "Loaded Model")
		FString GetLoadedModelFile() { return LoadedModelFile; }

	void LoadRigidBodies(FString FileName);

	//Creates SynOpticonActors from the loaded data of the SynOpticonActorLoader class
	void CreateSynOpticonActorsFromData(FString filepath);

	//===================================singleton=======================================
	static UCreationAndDeletionManager* GetCADManagerInstance();
	static void SetInstance(UCreationAndDeletionManager* _Instance);

	//=======================supporting functions for recording==========================
	//before
	TArray<FGazeActorData> PrepareActorsBeforeRecording(TArray<CompressedGazeBlock*> &GazeBlocks, TArray<CompressedSOBlock*> &SOBlocks);

	//after
	TArray<FSynOpticonActorStruct> StopActorsFromRecording();

	//=======================supporting functions for replaying==========================
	void PrepareActorsBeforeReplaying(TArray<FGazeActorData> GAList, TArray<FGazeGroupData> GGList, TArray<FSynOpticonActorStruct> SOList,
		TArray<CompressedGazeBlock*>* GazeBlocks, TArray<CompressedSOBlock*>* SOBlocks);
	void StopActorsFromReplaying();
};
