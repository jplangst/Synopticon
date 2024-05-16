// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RaycastDatatypes.h"
#include "Components/ActorComponent.h"
#include "ParticleDefinitions.h"

#include "../GazeObjects/GazeActor.h"
#include "../GazeObjects/StaticRoomActor.h"
#include "../ActorComponents/PositionAndOrientationComponent.h"
#include "../SystemClasses/SynOpticonState.h"

#include "RaycastComponent_Ours.generated.h"

//Struct that contains the combined trace results on a gaze object
USTRUCT()
struct FHitObjectResult {
	GENERATED_USTRUCT_BODY()

	AGazeActor* HitActor;
	TArray<FVector> HitLocations;
	TArray<double> TargetDistances;
	TArray<FVector> ImpactNormals;
	int32 FaceIndex;

	FHitObjectResult()
	{
		HitActor = nullptr;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYNOPTICON_API URaycastComponent_Ours : public UActorComponent
{
	GENERATED_BODY()
	int32 HitTargetID;
	FHitResult* HitResult;
	FString OwnerName;
	USceneComponent* VectorSceneComponent;

	//Used to control the frequency at which we publish eye events in real-time
	float PublishEventThreshold = (1000/60)/1000; //60frequency/1000ms
	float PublishEventTimer;
	bool PublishEvent;

	//Called to perform raycast using the assigned VectorSceneComponent
	void PerformRaycast();

	AGazeActor* InteractingActor;

	//Prepares the parameters used for the ray tracing
	FCollisionQueryParams PrepareTraceParams();

	//Fires a ray trace with the specified parameters and returns the result of the trace
	FHitResult* TraceLine(FCollisionQueryParams RV_TraceParams, FVector StartOfTrace,
		FVector EndOfTrace, ECollisionChannel CollisionChannel);

	/* Fires several ray traces in a cone shape and returns the actors that were hit with their hit information.
	 * AngleInDegrees = The angle away from the centre to use as the cone perimiter
	 * Divisions = The number of ray traces that will be fired around the perimiter of the cone
	 *	0 = only centre, 1 = centre + 4 traces in the cardinal directions, 2 = centre + 8 traces, 3 = centre + 16 traces, and so forth
	 */
	TArray<FHitObjectResult> TraceConeShape(FCollisionQueryParams RV_TraceParams, float AngleInDegrees, int32 Divisions);

	//Processes the hit results and updates HitResult with the best fitting result 
	void ProcessHitResults(TArray<FHitObjectResult> &HitResults);

	//Process a ray trace hit result and update the provided list
	void ProcessHitResult(TArray<FHitObjectResult> &OutHitResults, FHitResult* HitResult);

	//TODO REMOVE
	bool bSlowdownForDebug;
	float SlowdownThreshold;
	float SlowdownTimer;
	

public:	
	// Sets default values for this component's properties
	URaycastComponent_Ours();
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// Called when the game end
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called when the game starts
	virtual void BeginPlay() override;

	//Set the scene component containing the direction vector that is to be used for raycasting
	void SetVectorSceneComponent(USceneComponent* VectorSceneComponent);

	//Set the color of the vector used for raycasting 
	UFUNCTION(BlueprintCallable, Category="RaycastingVector")
	void SetVectorColor(FLinearColor NewVectorColor);

	//Get the currently hit gaze actor target, returns nullptr if there is no target
	int32 GetHitTargetID();
	void SetHitTargetID(int32 TargetID);

	FHitResult* GetHitResult();

	FString GetOwnerName();
	void SetOwnerName(FString OwnerName);

	void SetInteractingActor(AGazeActor* gazeActor);
	AGazeActor* GetInteractingActor();

	double GetTargetDistance();
};
