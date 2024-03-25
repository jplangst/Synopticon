// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ConstructorHelpers.h"

#include "Particles/ParticleSystemComponent.h"
#include "ParticleDefinitions.h"

#include "SynOpticonActors/SynOpticonDataStructures.h"
#include "GazeObjects/GazeActor.h"

#include "Components/ActorComponent.h"
#include "GazeDataVisualizerComponent.generated.h"

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SYNOPTICON_API UGazeDataVisualizerComponent : public UActorComponent
{
	GENERATED_BODY()

	TQueue<UParticleSystemComponent*> FixationParticleSystems;
	TQueue<UParticleSystemComponent*> SaccadeParticleSystems;
	TArray<FFixationGroupStruct> GazeActorFixations;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GazeVisualizationBP")
		//TSubclassOf<UParticleSystem> GazeVizBP;

	//Used to connect saccades without having to iterate through the fixation list
	FVector PrevFixationLocation;
	
	//Used during the replay mode
	int32 FixationCount;
	FDateTime PrevTime;

	int32 OwnerID;

	FLinearColor ParticleColors;

	int32 SpawnedSystemsID;
	int32 MaxFixationVisualisations;
	int32 NmbFixationVisualisations;
	bool isFirstTime;
	void AddFixationData(int32 GazeActorID, FVector Location, float Duration, FVector Normal);

public:	
	// Sets default values for this component's properties
	UGazeDataVisualizerComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Particle System Template")
		UParticleSystem* FixationTemplate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Particle System Template")
		UParticleSystem* SaccadeTemplate;

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called when the game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//Add a new fixation point data to the visualizer
	void AddFixationGroup(int32 GazeActorID, FVector Location, float Duration, FDateTime Timestamp, FVector Normal, FVector2D HitUV);

	void CheckIfMaxFixations();

	void SetOwnerID(int32 OwnerID);

	//Deactivate and remove all the current particle systems
	UFUNCTION(BlueprintCallable, Category = "Gaze Data")
	void ClearParticleSystems();

	//Set the visibility of the Gaze Data Particle System
	UFUNCTION(BlueprintCallable, Category = "Gaze Data")
	void SetGazePointParticlesVisibility(bool Visible);

	void UpdateParticleSystemNumbers();

	TArray<FFixationGroupStruct> GetGazeData();
	UFUNCTION()
	void DoSeeking(FString _CallerID);

	UFUNCTION()
	void RemoveGazeActorCallback(int32 GazeActorID);

	void SetReplayGazeData(TArray<FFixationGroupStruct> Data);
	FEyeRadarPointData Replay(FDateTime CurrentTime, AGazeActor*& OutGazeActor);

	UFUNCTION(BlueprintCallable, Category="GazeParticles")
	void SetParticleColor(FLinearColor NewColor);

	UFUNCTION(BlueprintImplementableEvent, Category = "Gaze Data")
	void SetupDynamicMaterial(UParticleSystemComponent* ParticleSystem, bool Fixation, int32 MaxSystems, int32 SystemNumber);
	UFUNCTION(BlueprintImplementableEvent, Category = "Gaze Data")
	void UpdateDynamicMaterial(UParticleSystemComponent* ParticleSystem, int32 SystemNumber, int32 MaxSystems);

	UFUNCTION()
		void OnStartRecording(bool Start);
};
