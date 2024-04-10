#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#define UI UI_ST
THIRD_PARTY_INCLUDES_START

#if PLATFORM_WINDOWS
#include "WindowsHWrapper.h"
#include "AllowWindowsPlatformTypes.h"
#include "AllowWindowsPlatformAtomics.h"
#endif

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include "../zeroconf/mdns.hpp"

#if PLATFORM_WINDOWS
#include "HideWindowsPlatformAtomics.h"
#include "HideWindowsPlatformTypes.h"
#endif

THIRD_PARTY_INCLUDES_END
#undef UI

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "JsonUtilities.h"

#include "Sensors/SensorDataStructs.h"
#include "SystemClasses/SynOpticonState.h"
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

private:
    int BroadcastSocketID;
    int ReceiverSocketID;
    float TimeToBroadcast = 2;
    float TimeSinceBroadcast = 0;

    float TimeToRequestInfo = 2;
    float TimeSinceInfoRequest = 0;

    void HandleDiscoveredDevice(FString IPAddress, FString Port);

    static int OnServiceDiscovered(int sock, const struct sockaddr* from, size_t addrlen, mdns_entry_type_t entry,
        uint16_t query_id, uint16_t rtype, uint16_t rclass, uint32_t ttl, const void* data,
        size_t size, size_t name_offset, size_t name_length, size_t record_offset,
        size_t record_length, void* user_data);

    void* ReceiverBuffer;
    size_t MAX_FUZZ_SIZE = 4096;
};
