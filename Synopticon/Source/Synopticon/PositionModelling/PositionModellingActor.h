// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Utils/SynOpticonStatics.h"

#include "ActorComponents/PositionAndOrientationComponent.h"

#include "PositionModellingActor.generated.h"

UCLASS()
class SYNOPTICON_API APositionModellingActor : public AActor
{
	GENERATED_BODY()

		UPositionAndOrientationComponent* PosOriComponent;

	bool ToolVisible;
	bool ManualType;

	FVector ManualToolOffset;
	FVector LaserToolOffset;
public:	
	// Sets default values for this actor's properties
	APositionModellingActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetPositionAndOrientationComponent(UPositionAndOrientationComponent* NewPosOriComponent);
	
	UFUNCTION(BlueprintCallable, Category = "ModellingTool")
	FVector CalculatePoint(float distance);
	UFUNCTION(BlueprintCallable, Category = "ModellingTool")
	void SetToolVisibility(bool on);

	void SetModellingToolType(bool _Manual);

	UFUNCTION(BlueprintCallable, Category = "ModellingTool")
	bool IsManualModellingTool();

	UFUNCTION(BlueprintCallable, Category = "ModellingTool")
	void SetManualModellingToolOffset(float dist);

	UFUNCTION(BlueprintPure, Category = "ModellingTool")
	FVector GetLocationOffset();

	UFUNCTION(BlueprintCallable, Category = "ModellingTool")
	void SetShouldApplyOffset(bool ApplyOffset);

	UFUNCTION(BlueprintCallable, Category = "ModellingTool")
	void ResetPositionAndOrientation();

	UFUNCTION(BlueprintCallable, Category = "ModellingTool")
	void SetLaserToolOffset(FVector _Offset);

	//Creates half ellipsis mesh
	UFUNCTION(BlueprintPure, Category = "ModellingTool")
	static TArray<FVector> CreateEllipseShape(TArray<FVector> Points, int Subdivisions);

	static float EllipseFormula(float longR, float shortR, FVector Center, float x);

	//Creates arc
	UFUNCTION(BlueprintPure, Category = "ModellingTool")
	static TArray<FVector> CreateArcShape(TArray<FVector> Points, int Subdivisions);

	static float CircularFormula(float a, float b, float r, float x);
	static void CalculateCircle(FVector P1, FVector P2, FVector P3, float& a, float& b, float& r);
};
