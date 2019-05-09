#pragma once

#include "CoreMinimal.h"

#include "StaticRoomActorDatatypes.generated.h"

UENUM(BlueprintType)		
enum class EStaticRoomActorType : uint8
{
	SRT_Screen 			UMETA(DisplayName = "Screen"),
	SRT_CurvedScreen	UMETA(DisplayName = "Curved Screen"),
	SRT_ComplexObject 	UMETA(DisplayName = "Complex Object"),
	SRT_ActionObject 	UMETA(DisplayName = "Action Object")
};
FORCEINLINE FArchive& operator<<(FArchive &Ar, EStaticRoomActorType &AType)
{
	if (Ar.IsSaving())
	{
		uint8 Value;
		switch (AType)
		{
			case EStaticRoomActorType::SRT_CurvedScreen: Value = 1U; break;
			case EStaticRoomActorType::SRT_ComplexObject: Value = 2U; break;
			case EStaticRoomActorType::SRT_ActionObject: Value = 3U; break;
			default: Value = 0U; break;
		}

		Ar << Value; 
	}
	//uint8 AsByte = (uint8)AType;
	//Ar << AsByte;
	if (Ar.IsLoading())
	{
		uint8 Value;
		Ar << Value;
		switch (Value)
		{
			case 1U: AType = EStaticRoomActorType::SRT_CurvedScreen; break;
			case 2U: AType = EStaticRoomActorType::SRT_ComplexObject; break;
			case 3U: AType = EStaticRoomActorType::SRT_ActionObject; break;
			default: AType = EStaticRoomActorType::SRT_Screen; break;
		}

	}

	return Ar;
}

/** A Struct for storing Rigid body information */
USTRUCT(BlueprintType)
struct FStaticRoomData
{
	GENERATED_USTRUCT_BODY()

	/** Name of the Gaze Actor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Room Actor")
	FString Name;

	/** Position of the makers*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Room Actor")
	TArray<FVector> Markers;

	/** The URL to display on the screen */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Room Actor")
	FColor MeshVertexColor;

	EStaticRoomActorType RoomActorType;

	FStaticRoomData()
	{
		Name = "Not set yet";
		MeshVertexColor = FColor(125, 125, 125, 1);
	}
};
