#include "ServiceDiscoveryComponent.h"

UServiceDiscoveryComponent::UServiceDiscoveryComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UServiceDiscoveryComponent::BeginPlay()
{
    Super::BeginPlay();

    /* mjansson mdns */

    // Create the socket for broadcasting mDNS queries (Broadcasting is working)
    BroadcastSocketID = mdns_socket_open_ipv4(nullptr);

    sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(5353);
	saddr.sin_addr = in4addr_any; // inet_addr("192.168.0.100");//INADDR_ANY; //Bind to interface, INADDR_ANY for all

    // Create the socket for listening to mDNS messages
    ReceiverSocketID = mdns_socket_open_ipv4(&saddr);
	ReceiverBuffer = malloc(MAX_FUZZ_SIZE);
}

static mdns_string_t
ipv4_address_to_string(char* buffer, size_t capacity, const struct sockaddr_in* addr,
	size_t addrlen) {
	char host[NI_MAXHOST] = { 0 };
	char service[NI_MAXSERV] = { 0 };
	int ret = getnameinfo((const struct sockaddr*)addr, (socklen_t)addrlen, host, NI_MAXHOST,
		service, NI_MAXSERV, NI_NUMERICSERV | NI_NUMERICHOST);
	int len = 0;
	if (ret == 0) {
		if (addr->sin_port != 0)
			len = snprintf(buffer, capacity, "%s:%s", host, service);
		else
			len = snprintf(buffer, capacity, "%s", host);
	}
	if (len >= (int)capacity)
		len = (int)capacity - 1;
	mdns_string_t str;
	str.str = buffer;
	str.length = len;
	return str;
}

static mdns_string_t
ipv6_address_to_string(char* buffer, size_t capacity, const struct sockaddr_in6* addr,
	size_t addrlen) {
	char host[NI_MAXHOST] = { 0 };
	char service[NI_MAXSERV] = { 0 };
	int ret = getnameinfo((const struct sockaddr*)addr, (socklen_t)addrlen, host, NI_MAXHOST,
		service, NI_MAXSERV, NI_NUMERICSERV | NI_NUMERICHOST);
	int len = 0;
	if (ret == 0) {
		if (addr->sin6_port != 0)
			len = snprintf(buffer, capacity, "[%s]:%s", host, service);
		else
			len = snprintf(buffer, capacity, "%s", host);
	}
	if (len >= (int)capacity)
		len = (int)capacity - 1;
	mdns_string_t str;
	str.str = buffer;
	str.length = len;
	return str;
}

static mdns_string_t
ip_address_to_string(char* buffer, size_t capacity, const struct sockaddr* addr, size_t addrlen) {
	if (addr->sa_family == AF_INET6)
		return ipv6_address_to_string(buffer, capacity, (const struct sockaddr_in6*)addr, addrlen);
	return ipv4_address_to_string(buffer, capacity, (const struct sockaddr_in*)addr, addrlen);
}

// Data for our service including the mDNS records
typedef struct {
	mdns_string_t service;
	mdns_string_t hostname;
	mdns_string_t service_instance;
	mdns_string_t hostname_qualified;
	struct sockaddr_in address_ipv4;
	struct sockaddr_in6 address_ipv6;
	int port;
	mdns_record_t record_ptr;
	mdns_record_t record_srv;
	mdns_record_t record_a;
	mdns_record_t record_aaaa;
	mdns_record_t txt_record[2];
} service_t;

static char addrbuffer[64];
static char entrybuffer[256];
static char namebuffer[256];
static char sendbuffer[1024];
static mdns_record_txt_t txtbuffer[128];

void UServiceDiscoveryComponent::HandleDiscoveredDevice(FString IPAddress, FString Port) {
	// Create a new HTTP request object
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();

	// TODO remove the port parameter 
	//We don't actually want to use the port that was the target when we send the REST request, we want to send to port 8080. 
	Port = "8080";

	// TODO this also needs to be generelized.
	// Set the URL of the request
	FString URL = "http://" + IPAddress + ":" + Port + "/api/status";
	HttpRequest->SetURL(*URL);
	// Set the verb (GET, POST, etc.)
	HttpRequest->SetVerb(TEXT("GET"));
	// Set the request completion callback
	HttpRequest->OnProcessRequestComplete().BindLambda([](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess) {
		if (bSuccess && Response.IsValid())
		{
			// Process the response data
			int32 ResponseCode = Response->GetResponseCode();
			FString ResponseBody = Response->GetContentAsString();

			if (ResponseBody.Contains("Neon Companion")) {
				FPupilLabs PupilLabsStructure;
				if (FJsonObjectConverter::JsonObjectStringToUStruct(ResponseBody, &PupilLabsStructure, 0, 0))
				{
					// Successfully parsed JSON string into struct
					// Now you can access the data
					FString Message = PupilLabsStructure.message;
					TArray<FSensorData> SensorDataList = PupilLabsStructure.result;

					// Access each sensor
					for (const FSensorData& NewSensorData : SensorDataList)
					{
						if (NewSensorData.model.Equals("Sensor") && NewSensorData.data.conn_type.Equals("WEBSOCKET")) {
							// Check if the sensor is already in the discovered list, if nto add it.
							TArray<FSensorData> ExistingSensorDataList = ASynOpticonState::GetSensorDataList();
							bool sensorInList = false;
							for (FSensorData ExistingSensorData : ExistingSensorDataList) {
								if (ExistingSensorData.data.ip == NewSensorData.data.ip && ExistingSensorData.data.port == NewSensorData.data.port) {
									sensorInList = true;
									break;
								}
							}
							if (!sensorInList) {
								ASynOpticonState::AddSensorData(NewSensorData);
							}
						}
					}
				}
				else
				{
					// Failed to parse JSON string
					UE_LOG(LogTemp, Error, TEXT("Failed to parse pupil labs JSON string"));
				}
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("Unknown device: %s"), *ResponseBody);
			}
			
			
		}
		else
		{
			// Handle request failure...
			int32 ResponseCode = -1;
		}
		});

	// Send the request
	HttpRequest->ProcessRequest();
}

// Callback handling parsing answers to queries sent
int UServiceDiscoveryComponent::OnServiceDiscovered(int sock, const struct sockaddr* from, size_t addrlen, mdns_entry_type_t entry,
	uint16_t query_id, uint16_t rtype, uint16_t rclass, uint32_t ttl, const void* data,
	size_t size, size_t name_offset, size_t name_length, size_t record_offset,
	size_t record_length, void* user_data)
{
	(void)sizeof(sock);
	(void)sizeof(query_id);
	(void)sizeof(name_length);
	(void)sizeof(user_data);

	mdns_string_t fromaddrstr = ip_address_to_string(addrbuffer, sizeof(addrbuffer), from, addrlen);
	const char* entrytype = (entry == MDNS_ENTRYTYPE_ANSWER) ?
		"answer" :
		((entry == MDNS_ENTRYTYPE_AUTHORITY) ? "authority" : "additional");
	const FString EntryType = (entry == MDNS_ENTRYTYPE_ANSWER) ?
		"answer" :
		((entry == MDNS_ENTRYTYPE_AUTHORITY) ? "authority" : "additional");

	mdns_string_t entrystr =
		mdns_string_extract(data, size, &name_offset, entrybuffer, sizeof(entrybuffer));
	if (rtype == MDNS_RECORDTYPE_PTR) {
		mdns_string_t namestr = mdns_record_parse_ptr(data, size, record_offset, record_length,
			namebuffer, sizeof(namebuffer));

		FString FromAddr = fromaddrstr.str;
		FString EntryStr = entrystr.str;
		FString NameStr = namestr.str;

		printf("%.*s : %s %.*s PTR %.*s rclass 0x%x ttl %u length %d\n",
			MDNS_STRING_FORMAT(fromaddrstr), entrytype, MDNS_STRING_FORMAT(entrystr),
			MDNS_STRING_FORMAT(namestr), rclass, ttl, (int)record_length);

		UE_LOG(LogTemp, Warning, TEXT("Message from: %s, Entry type: %s, Entry string: %s, Name: %s, rclass: %d, ttl: %d, length: %d"),
			*FromAddr, *EntryType, *EntryStr, *NameStr, rclass, ttl, (int)record_length);

		FPotentialSensorAddress PotentialSensorAddress;
		PotentialSensorAddress.SensorAddress = FromAddr;
		ASynOpticonState::AddDiscoveredIPAddress(PotentialSensorAddress);
	}
	else if (rtype == MDNS_RECORDTYPE_SRV) {
		UE_LOG(LogTemp, Warning, TEXT("MDNS Recordtype SRV"));
		mdns_record_srv_t srv = mdns_record_parse_srv(data, size, record_offset, record_length,
			namebuffer, sizeof(namebuffer));
		printf("%.*s : %s %.*s SRV %.*s priority %d weight %d port %d\n",
			MDNS_STRING_FORMAT(fromaddrstr), entrytype, MDNS_STRING_FORMAT(entrystr),
			MDNS_STRING_FORMAT(srv.name), srv.priority, srv.weight, srv.port);
	}
	else if (rtype == MDNS_RECORDTYPE_A) {
		UE_LOG(LogTemp, Warning, TEXT("MDNS Recordtype A"));
		struct sockaddr_in addr;
		mdns_record_parse_a(data, size, record_offset, record_length, &addr);
		mdns_string_t addrstr =
			ipv4_address_to_string(namebuffer, sizeof(namebuffer), &addr, sizeof(addr));
		printf("%.*s : %s %.*s A %.*s\n", MDNS_STRING_FORMAT(fromaddrstr), entrytype,
			MDNS_STRING_FORMAT(entrystr), MDNS_STRING_FORMAT(addrstr));
	}
	else if (rtype == MDNS_RECORDTYPE_AAAA) {
		UE_LOG(LogTemp, Warning, TEXT("MDNS Recordtype AAAA"));
		struct sockaddr_in6 addr;
		mdns_record_parse_aaaa(data, size, record_offset, record_length, &addr);
		mdns_string_t addrstr =
			ipv6_address_to_string(namebuffer, sizeof(namebuffer), &addr, sizeof(addr));
		printf("%.*s : %s %.*s AAAA %.*s\n", MDNS_STRING_FORMAT(fromaddrstr), entrytype,
			MDNS_STRING_FORMAT(entrystr), MDNS_STRING_FORMAT(addrstr));
	}
	else if (rtype == MDNS_RECORDTYPE_TXT) {
		UE_LOG(LogTemp, Warning, TEXT("MDNS Recordtype TXT"));
		size_t parsed = mdns_record_parse_txt(data, size, record_offset, record_length, txtbuffer,
			sizeof(txtbuffer) / sizeof(mdns_record_txt_t));
		for (size_t itxt = 0; itxt < parsed; ++itxt) {
			if (txtbuffer[itxt].value.length) {
				printf("%.*s : %s %.*s TXT %.*s = %.*s\n", MDNS_STRING_FORMAT(fromaddrstr),
					entrytype, MDNS_STRING_FORMAT(entrystr),
					MDNS_STRING_FORMAT(txtbuffer[itxt].key),
					MDNS_STRING_FORMAT(txtbuffer[itxt].value));
			}
			else {
				printf("%.*s : %s %.*s TXT %.*s\n", MDNS_STRING_FORMAT(fromaddrstr), entrytype,
					MDNS_STRING_FORMAT(entrystr), MDNS_STRING_FORMAT(txtbuffer[itxt].key));
			}
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("MDNS Recordtype Other"));
		printf("%.*s : %s %.*s type %u rclass 0x%x ttl %u length %d\n",
			MDNS_STRING_FORMAT(fromaddrstr), entrytype, MDNS_STRING_FORMAT(entrystr), rtype,
			rclass, ttl, (int)record_length);
	}
	return 0;
}

void UServiceDiscoveryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Broadcast MDNS discovery query
	if (TimeSinceBroadcast > TimeToBroadcast) {
		TimeSinceBroadcast -= TimeToBroadcast;

		int QueryId = mdns_discovery_send(BroadcastSocketID);
		UE_LOG(LogTemp, Warning, TEXT("Broadcasting discovery"));
		if (QueryId < 0) {
			//Error
			UE_LOG(LogTemp, Error, TEXT("Error sending discovery"));
		}

		//TODO This is only tmp to see the contents of the stored sensor list
		TArray<FSensorData> SensorDataList = ASynOpticonState::GetSensorDataList();
		UE_LOG(LogTemp, Warning, TEXT("Discovered Sensors length: %d"), SensorDataList.Num());
	}
	TimeSinceBroadcast += DeltaTime;

	// Listen for MDNS messages
	size_t NmbQueriesParsed = mdns_socket_listen(ReceiverSocketID, ReceiverBuffer, MAX_FUZZ_SIZE, &UServiceDiscoveryComponent::OnServiceDiscovered, 0);

	// Iterate over the discovered IP Addresses and attempt to send a HTTP Rest request
	if (TimeSinceInfoRequest > TimeToRequestInfo) {
		TimeSinceInfoRequest -= TimeToRequestInfo;

		TArray<FPotentialSensorAddress> DiscoveredIpAddresses = ASynOpticonState::GetDiscoveredIPAddress();
		for (FPotentialSensorAddress PotentialSensorAddress : DiscoveredIpAddresses) {
			if (!PotentialSensorAddress.RequestSent) {
				FString Address;
				FString Port;
				PotentialSensorAddress.SensorAddress.Split(TEXT(":"), &Address, &Port);
				HandleDiscoveredDevice(Address, Port);

				ASynOpticonState::SetDiscoveredIPAddressRequest(PotentialSensorAddress, true);
			}	
		}
	}
	TimeSinceInfoRequest += DeltaTime;


}

void UServiceDiscoveryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    // Cleanup the sockets
    mdns_socket_close(BroadcastSocketID);
    mdns_socket_close(ReceiverSocketID);
	// Cleanup the data buffer
	free(ReceiverBuffer);
}

