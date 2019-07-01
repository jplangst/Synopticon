// Fill out your copyright notice in the Description page of Project Settings.

#include "GazeObjectMaterialComponent.h"

// Sets default values for this component's properties
UGazeObjectMaterialComponent::UGazeObjectMaterialComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGazeObjectMaterialComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	materialAlpha = 0;
	HeatMapColour = FLinearColor(0, 0, 0, 1);
	HeatMapAlpha = 0;
	Material = nullptr;
	bForceHighlight = false;

	TextureFilepath = "";

	ProcMesh = GetOwner()->FindComponentByClass<UProceduralMeshComponent>();
}

void UGazeObjectMaterialComponent::SetupEyeRadarParameters(FVector2D ScreenSize) {
	if (ProcMesh) {
		Material = Cast<UMaterialInstanceDynamic>(ProcMesh->GetMaterial(0));
		if (Material) {
			FVector EyeRadarUVScale = FVector(1,1,1);
			FVector EyeRadarUVOffset = FVector(0, 0, 0);
			//If the screen is wider than it is high
			if (FMath::Max(ScreenSize.X, ScreenSize.Y) == ScreenSize.X) {
				//EyeRadarUVScale = FVector(ScreenSize.X / ScreenSize.Y,1,0);

			}
			else { //Height is bigger
				//EyeRadarUVScale = FVector(1, ScreenSize.Y / ScreenSize.X, 0);
				//EyeRadarUVOffset = FVector(0, FMath::Fmod(EyeRadarUVScale.Y, 1) / 2, 0);
			}
					
			Material->SetVectorParameterValue("EyeRadarUVScale", EyeRadarUVScale);
			Material->SetVectorParameterValue("EyeRadarUVOffset", EyeRadarUVOffset);
		}
	}
}

void UGazeObjectMaterialComponent::SetupVideoTexture(int32 NmbCols, int32 NmbRows, int32 VideoIndex) {
	if (ProcMesh) {
		Material = Cast<UMaterialInstanceDynamic>(ProcMesh->GetMaterial(0));
		if (Material) {
			Material->SetScalarParameterValue("VideoTexture", 1);
			Material->SetScalarParameterValue("NmbRows", NmbRows);
			Material->SetScalarParameterValue("NmbCols", NmbCols);
			Material->SetScalarParameterValue("VideoIndex", (float)VideoIndex/((float)NmbRows*(float)NmbCols));
		}
	}
}

void UGazeObjectMaterialComponent::SetupTextureBoundingBox(FVector TextureBB) {
	if (ProcMesh) {
		Material = Cast<UMaterialInstanceDynamic>(ProcMesh->GetMaterial(0));
		if (Material) {
			Material->SetVectorParameterValue("TextureBoundingBox", TextureBB);
		}
	}
}

void UGazeObjectMaterialComponent::SetupScreenTexture(FString _TextureFilepath) { //TODO might have to clean up the texture, not sure
	if (_TextureFilepath.Equals("No Texture"))
	{
		ClearScreenTexture();
		return;
	}

	bool Valid = false;
	int32 Width, Height;
	FString ImageFormat;
	FString ThrowAway;
	_TextureFilepath.Split(".", &ThrowAway, &ImageFormat, ESearchCase::Type::IgnoreCase, ESearchDir::Type::FromEnd);

	FString Filepath = USynOpticonStatics::GetTextureFolder().Append(_TextureFilepath);

	UTexture2D* ScreenTexture = USynOpticonStatics::LoadTextureFromImage(Filepath, ImageFormat, Valid, Width, Height);
	if (Valid) {
		TextureFilepath = _TextureFilepath;
		if (ProcMesh && ProcMesh->GetNumMaterials() > 0) {
			Material = Cast<UMaterialInstanceDynamic>(ProcMesh->GetMaterial(0));
			if (Material && ScreenTexture) {
				Material->SetScalarParameterValue("HasScreenTexture", 1);
				Material->SetTextureParameterValue("ScreenTexture", ScreenTexture);
			}
		}
	}
}

void UGazeObjectMaterialComponent::ClearScreenTexture() {
	TextureFilepath = "";
	if (ProcMesh) {
		Material = Cast<UMaterialInstanceDynamic>(ProcMesh->GetMaterial(0));
		if (Material) {
			Material->SetScalarParameterValue("HasScreenTexture", 0);
			Material->SetTextureParameterValue("ScreenTexture", nullptr);
		}
	}
}

FString UGazeObjectMaterialComponent::GetTextureFilepath() {
	return TextureFilepath;
}

// Called every frame
void UGazeObjectMaterialComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
	if (ProcMesh && ProcMesh->GetNumMaterials() > 0) {
		Material = Cast<UMaterialInstanceDynamic>(ProcMesh->GetMaterial(0));

		if (Material)
		{
			bool isLookedAt = false;
			for (auto& Elem : IsLookedAtMap)
			{
				if (Elem.Value)
				{
					isLookedAt = true;
					//StencilValue = Elem.Key;
					break;
				}
			}

			if (bForceHighlight) {
				ProcMesh->CustomDepthStencilValue = StencilValue;
				ProcMesh->SetRenderCustomDepth(true);			
			}
			else {
				//Change the outline of the GazeActor so it glows when it is looked at
				if (isLookedAt)
				{
					ProcMesh->CustomDepthStencilValue = StencilValue;
					ProcMesh->SetRenderCustomDepth(true);		
					MarkRenderStateDirty();
				}
				else
				{
					if (!bBeingEdited) {
						ProcMesh->SetRenderCustomDepth(false);
					}
				}
			}
			
			HeatMapAlpha = ASynOpticonState::IsHeatMapVisible() ? ASynOpticonState::HeatMapAlpha : 0.0f;
			float AOIAlpha = ASynOpticonState::IsAOIVisible() ? ASynOpticonState::AOIAlpha : 0.0f;
			float EyeRadarVisible = ASynOpticonState::IsEyeRadarVisible() ? 1.0f : 0.0f;
			float MicroTaskOverlayVisible = ASynOpticonState::IsMicroTaskOverlayVisible() ? 1.0f : 0.0f;
			Material->SetScalarParameterValue("AlphaDriver", materialAlpha);
			Material->SetScalarParameterValue("HeatMapVisible", HeatMapAlpha);
			Material->SetScalarParameterValue("AOIVisible", AOIAlpha);
			Material->SetVectorParameterValue("HeatMapColor", HeatMapColour);
			Material->SetScalarParameterValue("EyeRadarVisible", EyeRadarVisible);
			Material->SetScalarParameterValue("MicroTaskOverlayVisible", EyeRadarVisible);
		}
	}
}

void UGazeObjectMaterialComponent::SetForceHighlight(bool ForceHighlight, int32 _StencilValue){
	bForceHighlight = ForceHighlight;
	StencilValue = _StencilValue;
}

// Add default functionality here for any IGazeObjectInterface functions that are not pure virtual.
void UGazeObjectMaterialComponent::SetHeatMapValue(float HeatMapValue)
{
	HeatMapColour = ASynOpticonState::GetHeatMapColorFromValue(HeatMapValue);
}

void UGazeObjectMaterialComponent::NotifyCollision(bool colliding, int32 SynOpticonActorID)
{
	if (colliding) {
		StencilValue = SynOpticonActorID + 1; //2 for two actors. Was 1
	}
	
	if (IsLookedAtMap.Contains(SynOpticonActorID))
	{
		bool* value = IsLookedAtMap.Find(SynOpticonActorID);
		*value = colliding;
	}
	else
	{
		IsLookedAtMap.Add(SynOpticonActorID, colliding);
	}
}

void UGazeObjectMaterialComponent::SetGazeObjectMaterial(UMaterialInstanceDynamic* _Material) {
	Material = _Material;
}

void UGazeObjectMaterialComponent::SetOwnerID(int32 _OwnerID) {
	OwnerID = _OwnerID;
}

int32 UGazeObjectMaterialComponent::GetOwnerID() {
	return OwnerID;
}

void UGazeObjectMaterialComponent::SetBeingEdited(bool Edited)
{
	bBeingEdited = Edited;
	if (ProcMesh) {
		ProcMesh->CustomDepthStencilValue = Edited ? 1 : 0;
		ProcMesh->SetRenderCustomDepth(bBeingEdited);		
	}
}