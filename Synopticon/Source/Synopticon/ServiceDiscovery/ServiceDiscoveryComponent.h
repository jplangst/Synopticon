#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Networking.h"
#include "ServiceDiscoveryComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SYNOPTICON_API UServiceDiscoveryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UServiceDiscoveryComponent();

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(EditAnywhere, Category = "Multicast Settings")
        FString MulticastAddress = "224.0.0.251";

    UPROPERTY(EditAnywhere, Category = "Multicast Settings")
        int32 MulticastPort = 5353;

private:
    FSocket* Socket = nullptr;
    FUdpSocketReceiver* SocketReceiver = nullptr;

    void HandleReceivedData(const FArrayReaderPtr& Data, const FIPv4Endpoint& Sender);
    void ParseMDNSMessage(const TArray<uint8>& MessageBytes);
    void ParseAnswersSection(const TArray<uint8>& MessageBytes, int32 AnswerSectionOffset);
};
