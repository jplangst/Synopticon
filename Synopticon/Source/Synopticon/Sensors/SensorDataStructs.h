#pragma once

#include "CoreMinimal.h"
#include "SensorDataStructs.generated.h"

USTRUCT(BlueprintType)
struct FData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString conn_type;

    UPROPERTY(BlueprintReadWrite)
    bool connected;

    UPROPERTY(BlueprintReadWrite)
    FString ip;

    UPROPERTY(BlueprintReadWrite)
    FString params;

    UPROPERTY(BlueprintReadWrite)
    int32 port;

    UPROPERTY(BlueprintReadWrite)
    FString protocol;

    UPROPERTY(BlueprintReadWrite)
    FString sensor;
};

USTRUCT(BlueprintType)
struct FSensorData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FData data;

    UPROPERTY(BlueprintReadWrite)
    FString model;
};

USTRUCT(BlueprintType)
struct FPupilLabs
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString message;

    UPROPERTY(BlueprintReadWrite)
    TArray<FSensorData> result;
};

USTRUCT(BlueprintType)
struct FPotentialSensorAddress
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString SensorAddress = "Not set";

    UPROPERTY(BlueprintReadWrite)
    bool RequestSent = false;
};
