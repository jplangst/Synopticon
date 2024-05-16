// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../Utils/SynOpticonStatics.h"
#include "../Utils/SynOpticonStatics.h"
#include "../NatNet/NatNetWAMPComponent.h"
#include "../ActorComponents/PositionAndOrientationComponent.h"

#include "GazeActor.h"
#include "GazeActorDataTypes.h"
#include "GazeObjectMaterialComponent.h"
#include "GazeParentActor.h"

#include "GeomTools.h"
#include "GazeActorFactory.generated.h"

/**
 * 
 */
UCLASS()
class SYNOPTICON_API UGazeActorFactory : public UObject
{
	GENERATED_BODY()

	//=========== AOI Unique ID ==============
	static int32 NextUniqueAOIId;

	static int32 Index; //keep track of the screen IDs
	static UGazeActorFactory* Instance;
	static bool InstanceFlag;

public:
	UGazeActorFactory();
	~UGazeActorFactory();

	static UGazeActorFactory* GetInstance();

	AGazeActor* CreateGazeActor(FGazeActorData GazeActorData, UWorld* World, TSubclassOf<AGazeActor> GazeActorBP);

	AGazeActor* CreateGazeActorWithUVs(FSlicingData GazeActorData, UWorld* World, TSubclassOf<AGazeActor> GazeActorBP);

	void UVUnwrapping(TArray<FVector> vertices, TArray<FVector2D> &UVs);

	static TArray<FVector> NormalizeBB(FVector Min, FVector Max, const TArray<FVector> &vertices);
	
	static int32 GetValidID();
	UFUNCTION(BlueprintCallable, Category = "AOIs")
	static int32 GetValidAOIID();
	UFUNCTION(BlueprintCallable, Category = "AOIs")
	static bool AOIIDIsUnique(int32 AOIID);

	static void DecrementAOIID();
};
