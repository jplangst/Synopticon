#pragma once

#include "CoreMinimal.h"

#include "RaycastDatatypes.generated.h"

/** A Struct for storing raycast hit information */
USTRUCT(BlueprintType)
struct FRaycastHitResult_Ours
{
	GENERATED_USTRUCT_BODY()

	AActor* HitActor;
	FVector HitLocation;
	double TargetDistance;
	FVector ImpactNormal;

	FRaycastHitResult_Ours()
	{
		HitActor = nullptr;
	}
};
