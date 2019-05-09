#pragma once
#include "CoreMinimal.h"

#include "GazeEventDataStructures.generated.h"

UENUM(BlueprintType)		
enum class GazeGestureEnum : uint8
{
	GG_DoubleBlink 	UMETA(DisplayName = "Double Blink")
};