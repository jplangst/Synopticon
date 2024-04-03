#include "ServiceDiscoveryComponent.h"

UServiceDiscoveryComponent::UServiceDiscoveryComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UServiceDiscoveryComponent::BeginPlay()
{
    Super::BeginPlay();

    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    if (!SocketSubsystem)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get socket subsystem"));
        return;
    }

    TSharedRef<FInternetAddr> MulticastAddr = SocketSubsystem->CreateInternetAddr();
    bool bIsValid = false;
    MulticastAddr->SetIp(*MulticastAddress, bIsValid);
    if (!bIsValid) {
        UE_LOG(LogTemp, Error, TEXT("Multicast address not valid"));
    }

    Socket = SocketSubsystem->CreateSocket(NAME_DGram, TEXT("MulticastSocket"), true);
    Socket->SetNonBlocking(true);
    Socket->SetReuseAddr(true);
    Socket->SetMulticastLoopback(true);
    Socket->SetMulticastTtl(32);

    TArray<TSharedPtr<FInternetAddr>> LocalAdapterAddresses;
    SocketSubsystem->GetLocalAdapterAddresses(LocalAdapterAddresses);
    TSharedPtr<FInternetAddr> LocalAdapterAddress = LocalAdapterAddresses[0];

    TSharedRef<FInternetAddr> AnyAddr = SocketSubsystem->CreateInternetAddr();
    AnyAddr->SetAnyAddress();
    AnyAddr->SetPort(MulticastPort);

    //NBNB Should bind the socket before joining groups
    //NBNB cannot bind to multicast address on windows, so bind to all interfaces, but only listen to multicast port
    if (!Socket->Bind(*AnyAddr)) {
        UE_LOG(LogTemp, Error, TEXT("Failed to bind socket"));
    }

    if (!Socket->SetMulticastInterface(*LocalAdapterAddress)) {
        UE_LOG(LogTemp, Error, TEXT("Failed to set multicast interface"));
    }

    if (!Socket->JoinMulticastGroup(*MulticastAddr, *LocalAdapterAddress)) {
        UE_LOG(LogTemp, Error, TEXT("Failed to join multicast group"));
    }

    // Create FUdpSocketReceiver
    SocketReceiver = new FUdpSocketReceiver(Socket, FTimespan::FromMilliseconds(100), TEXT("ServiceDiscoveryThread"));
    SocketReceiver->OnDataReceived().BindUObject(this, &UServiceDiscoveryComponent::HandleReceivedData);
    SocketReceiver->Start();
}

void UServiceDiscoveryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

#pragma pack(push, 1) // Ensure byte alignment
struct FMDNSHeader
{
    uint16 Id;           // ID field
    uint16 Flags;        // Flags field
    uint16 NumQuestions; // Number of questions field
    uint16 NumAnswers;   // Number of answers field
    uint16 NumAuthority; // Number of authority records field
    uint16 NumAdditional;// Number of additional records field
};

struct FMDNSQuestion
{
    uint16 Type;       // 2 bytes
    uint16 Class;      // 2 bytes
    // Variable size domain name (using TArray<uint8> as a placeholder)
    TArray<uint8> DomainName;
};

struct FMDNSAnswer
{
    uint16 Type;        // 2 bytes
    uint16 Class;       // 2 bytes
    uint32 TTL;         // 4 bytes
    uint16 RDLength;    // 2 bytes
    // Variable size resource data (using TArray<uint8> as a placeholder)
    TArray<uint8> ResourceData;
};

struct FMDNSAuthority
{
    uint16 Type;        // 2 bytes
    uint16 Class;       // 2 bytes
    uint32 TTL;         // 4 bytes
    uint16 RDLength;    // 2 bytes
    // Variable size resource data (using TArray<uint8> as a placeholder)
    TArray<uint8> ResourceData;
};

struct FMDNSAdditional
{
    uint16 Type;        // 2 bytes
    uint16 Class;       // 2 bytes
    uint32 TTL;         // 4 bytes
    uint16 RDLength;    // 2 bytes
    // Variable size resource data (using TArray<uint8> as a placeholder)
    TArray<uint8> ResourceData;
};
#pragma pack(pop) // Restore original byte alignment

FArchive& operator<<(FArchive& Ar, FMDNSHeader& Header)
{
    Ar << Header.Id;
    Ar << Header.Flags;
    Ar << Header.NumQuestions;
    Ar << Header.NumAnswers;
    Ar << Header.NumAuthority;
    Ar << Header.NumAdditional;
    return Ar;
}

const uint16 MDNS_RECORD_TYPE_A = 1;

void UServiceDiscoveryComponent::ParseMDNSMessage(const TArray<uint8>& MessageBytes)
{
    if (MessageBytes.Num() < sizeof(FMDNSHeader))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid mDNS message"));
        return;
    }

    // Create a memory reader to read from the message bytes
    FMemoryReader Reader(MessageBytes, true); // true indicates reading in little-endian mode

    // Deserialize the mDNS header from the archive
    FMDNSHeader Header;
    Reader << Header; // This will handle byte order swapping automatically

    // Extract information from the header
    uint16 Id = Header.Id;
    uint16 Flags = Header.Flags;
    uint16 NumQuestions = Header.NumQuestions;
    uint16 NumAnswers = Header.NumAnswers;
    uint16 NumAuthority = Header.NumAuthority;
    uint16 NumAdditional = Header.NumAdditional;

    // Log the extracted information (replace with your actual handling logic)
    UE_LOG(LogTemp, Display, TEXT("mDNS Message: ID=%d, Flags=%d, Questions=%d, Answers=%d, Authority=%d, Additional=%d"),
        Id, Flags, NumQuestions, NumAnswers, NumAuthority, NumAdditional);

    // Calculate the offset to the start of the answers section
    /*int32 AnswerSectionOffset = sizeof(FMDNSHeader) +
        NumQuestions * sizeof(FMDNSQuestion) +
        NumAnswers * sizeof(FMDNSAnswer) +
        NumAuthority * sizeof(FMDNSAuthority) +
        NumAdditional * sizeof(FMDNSAdditional);*/

    int32 AnswerSectionOffset = sizeof(FMDNSHeader);

    for (int32 i = 0; i < NumQuestions; ++i)
    {
        // Skip over each question section
        // In this example, we assume each question section is 4 bytes long (adjust as needed)
        AnswerSectionOffset += 4; // Adjust as needed based on the actual structure of question sections
    }

    // Ensure alignment if necessary (align to 4-byte boundary)
    AnswerSectionOffset = AnswerSectionOffset + ((4 - (AnswerSectionOffset % 4)) % 4);


    // Parse the answers section
    ParseAnswersSection(MessageBytes, AnswerSectionOffset);
}

void UServiceDiscoveryComponent::ParseAnswersSection(const TArray<uint8>& MessageBytes, int32 AnswerSectionOffset)
{
    // Create a memory reader to read from the message bytes
    FMemoryReader Reader(MessageBytes, true);

    // Seek to the start of the answers section
    Reader.Seek(AnswerSectionOffset);

    Reader.SetByteSwapping(false);

    // Iterate through the answers section
    while (!Reader.AtEnd())
    {
        // Parse answer record header
        FMDNSAnswer Answer;
        Reader << Answer.Type;
        Reader << Answer.Class;
        Reader << Answer.TTL;
        Reader << Answer.RDLength;

        // Extract information from the answer record
        uint16 Type = Answer.Type;
        uint16 Class = Answer.Class;
        uint32 TTL = Answer.TTL;
        uint16 RDLength = Answer.RDLength;
        //uint32 TTL = FMemory::BigEndianToSystem(Answer.TTL);
        //uint16 RDLength = FMemory::BigEndianToSystem(Answer.RDLength);

        // Handle A (IPv4 address) records
        if (Type == MDNS_RECORD_TYPE_A){
            if (RDLength == sizeof(FIPv4Address))
            {
                FIPv4Address IPAddress;
                Reader.Serialize(&IPAddress, sizeof(FIPv4Address));

                // Process IPv4 address (e.g., store or log)
                FString IPAddressString = IPAddress.ToString();
                UE_LOG(LogTemp, Display, TEXT("IPv4 Address: %s"), *IPAddressString);
            }
        }
        else if(Type == 1024) {
            // Deserialize the SRV record data
            FString Name, Target;
            uint16 Priority, Weight, Port;
            Reader << Name; // Name field
            Reader << Priority;
            Reader << Weight;
            Reader << Port;
            Reader << Target;

            // Process the SRV record data (e.g., store or log)
            UE_LOG(LogTemp, Display, TEXT("SRV Record: Name=%s, Priority=%d, Weight=%d, Port=%d, Target=%s"), *Name, Priority, Weight, Port, *Target);
        }
        else
        {
            // Handle other types of records as needed
            // You can add additional logic here to parse other types of records
            Reader.Seek(Reader.Tell() + RDLength);
        }
    }
}

void UServiceDiscoveryComponent::HandleReceivedData(const FArrayReaderPtr& Data, const FIPv4Endpoint& Sender)
{
    uint8* DataBuffer = Data->GetData();
    int32 BufferSize = Data->Num();
    // Convert uint8* buffer to TArray<uint8>
    
    if (DataBuffer && BufferSize > 0)
    {
        TArray<uint8> MessageBytes;
        MessageBytes.Append(DataBuffer, BufferSize);
        ParseMDNSMessage(MessageBytes);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid mDNS message buffer"));
        return;
    }

    // Process the received data
    //UE_LOG(LogTemp, Warning, TEXT("Received message: %s"), *Message);
}


void UServiceDiscoveryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    if (SocketReceiver)
    {
        SocketReceiver->Stop();
        delete SocketReceiver;
        SocketReceiver = nullptr;
    }

    if (Socket)
    {
        Socket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
        Socket = nullptr;
    }
}