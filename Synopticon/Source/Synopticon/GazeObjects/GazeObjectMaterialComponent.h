// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SystemClasses/SynOpticonState.h"

#include "KismetProceduralMeshLibrary.h"

#include "Components/ActorComponent.h"
#include "GazeObjectMaterialComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYNOPTICON_API UGazeObjectMaterialComponent : public UActorComponent
{
	GENERATED_BODY()

	UMaterialInstanceDynamic* Material;

	FLinearColor HeatMapColour;
	TMap<int32, bool> IsLookedAtMap;
	float materialAlpha;

	//The alpha of the heat map, used to determine how visible the heat map is
	float HeatMapAlpha;

	int32 OwnerID;

	bool bBeingEdited;
	bool bForceHighlight;
	int32 StencilValue; //Determines the color of the outline. Can be between 1-4

	UProceduralMeshComponent* ProcMesh;

	FString TextureFilepath;
public:	
	// Sets default values for this component's properties
	UGazeObjectMaterialComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "EyeRadar")
		void SetupEyeRadarParameters(FVector2D ScreenSize);

	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;
		
	void SetHeatMapValue(float HeatMapValue);
	void NotifyCollision(bool colliding, int32 SynOpticonActorID);

	void SetGazeObjectMaterial(UMaterialInstanceDynamic* Material);
	void SetOwnerID(int32 OwnerID);
	int32 GetOwnerID();

	//The stencil value determines the coor of the outline. Can currently be between 0-3
	UFUNCTION(BlueprintCallable, Category = "Outline")
		void SetForceHighlight(bool ForceHighlight, int32 StencilValue);

	UFUNCTION(BlueprintCallable, Category = "Outline")
		void SetBeingEdited(bool Edited);

	void SetupVideoTexture(int32 NmbCols, int32 NmbRows, int32 VideoIndex);

	UFUNCTION(BlueprintCallable, Category = "Screen Texture")
	void SetupScreenTexture(FString TextureFilepath);

	UFUNCTION(BlueprintCallable, Category = "Screen Texture")
	void ClearScreenTexture();

	UFUNCTION(BlueprintPure, Category = "Screen Texture")
	FString GetTextureFilepath();

	void SetupTextureBoundingBox(FVector TextureBB);
};
