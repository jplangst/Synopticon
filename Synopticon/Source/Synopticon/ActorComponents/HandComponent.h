// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "UObject/ConstructorHelpers.h"

#include "../SynOpticonActors/SynOpticonDataStructures.h"
#include "../Replay/ReplayDataStructures.h"
#include "../ActorComponents/PositionAndOrientationComponent.h"
#include "../GazeObjects/GazeActor.h"
#include "../GazeObjects/StaticRoomActorDatatypes.h"

#include "HandComponent.generated.h"

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYNOPTICON_API UHandComponent : public USceneComponent
{
	GENERATED_BODY()

private:
	UPositionAndOrientationComponent* PosOriComp;

	UStaticMesh* LeftHandMesh;
	UStaticMesh* RightHandMesh;

	HandDataBlock* HandBlock;

	FHandStruct HandData;
	
	static int32 HandID;
	int32 HitTargetID;
public:	
	// Sets default values for this component's properties
	UHandComponent();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* HandMesh;
	UPROPERTY(EditAnywhere)
	UStaticMesh* HandMeshAsset;
	UPROPERTY(EditAnywhere)
	float DefaultHandLength;
	UPROPERTY(EditAnywhere)
	float XScale;
	UPROPERTY(EditAnywhere)
	float HandLength;
	UPROPERTY(EditAnywhere)
	bool LeftHand;

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called when the game end
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;		

	void SetHandDataBlock(HandDataBlock *HandBlock);

	UFUNCTION(BlueprintCallable, Category = "Hand Component")
	void SetComponentData(FHandStruct HandData);
	UFUNCTION(BlueprintPure, Category = "Hand Component")
	FHandStruct GetComponentData();

	void SetPosOriComponent(UPositionAndOrientationComponent* PosOriComp);

	UFUNCTION()
	virtual void OnBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	int32 GetHitTargetID();

	void ToBinaryReplayData();
	void ReplayData();
};
