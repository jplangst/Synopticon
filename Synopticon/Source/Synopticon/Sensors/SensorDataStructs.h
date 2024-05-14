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

    FData() {
        conn_type = "Not set";
        connected = false;
        ip = "Not set";
        params = "Not set";
        port = -1;
        protocol = "Not set";
        sensor = "Not set";
    }
};

USTRUCT(BlueprintType)
struct FSensorData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FData data;

    UPROPERTY(BlueprintReadWrite)
    FString model;

    FSensorData() {
        data = FData();
        model = "Not set";
    }
};

USTRUCT(BlueprintType)
struct FPupilLabs
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString message;

    UPROPERTY(BlueprintReadWrite)
    TArray<FSensorData> result;

    FPupilLabs() {
        message = "Not set";
        result = TArray<FSensorData>();
    }
};

USTRUCT(BlueprintType)
struct FPotentialSensorAddress
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString SensorAddress;

    UPROPERTY(BlueprintReadWrite)
    bool RequestSent;

    FPotentialSensorAddress() {
        SensorAddress = "Not set";
        RequestSent = false;
    }
};
