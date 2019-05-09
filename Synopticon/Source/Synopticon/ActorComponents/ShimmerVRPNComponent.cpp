// Fill out your copyright notice in the Description page of Project Settings.
#include "ShimmerVRPNComponent.h"
//#include "Synopticon.h"

// Sets default values for this component's properties
UShimmerVRPNComponent::UShimmerVRPNComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}


// Called when the game starts
void UShimmerVRPNComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	GSRValue = -1;
	HRValue = -1;

	//ASynOpticonState::GetGlobalEventSystem()->OnEventWAMPComponentDisconnected.AddDynamic(this, &UShimmerVRPNComponent::OnShimmerWAMPComponentDisconnected);
	//ASynOpticonState::GetGlobalEventSystem()->OnEventWAMPComponentConnected.AddDynamic(this, &UShimmerVRPNComponent::OnShimmerWAMPComponentConnected);

}

void UShimmerVRPNComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GSRValue = -1;
	HRValue = -1;
}


// Called every frame
void UShimmerVRPNComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

bool UShimmerVRPNComponent::IsConnected()
{
	return false;
}

double UShimmerVRPNComponent::GetGSRValue()
{
	return GSRValue;
}

double UShimmerVRPNComponent::GetHRValue()
{
	return HRValue;
}

void UShimmerVRPNComponent::SetHREnabled(bool _HREnabled)
{
	HREnabled = _HREnabled;
}

void UShimmerVRPNComponent::SetGSREnabled(bool _GSREnabled)
{
	GSREnabled = _GSREnabled;
}

bool UShimmerVRPNComponent::HasHR() 
{ 
	return HREnabled; 
}

bool UShimmerVRPNComponent::HasGSR() 
{ 
	return GSREnabled; 
}

int32 UShimmerVRPNComponent::GetShimmerIndex()
{
	return ShimmerIndex;
}

void UShimmerVRPNComponent::SetShimmerIndex(int32 NewShimmerIndex)
{
	ShimmerIndex = NewShimmerIndex;
}

void UShimmerVRPNComponent::SetShimmerDeviceName(FString DeviceName)
{
	ShimmerDeviceName = DeviceName;
}

//void UShimmerVRPNComponent::OnShimmerWAMPComponentDisconnected(ComponentTypeEnum ComponentType, FString ComponentID)
//{
//	if (ComponentType == ComponentTypeEnum::VE_Shimmer)
//	{
//		if (ShimmerDeviceName == ComponentID)
//		{
//			if (GEngine)
//			{
//				FString msg("Shimmer " + ShimmerDeviceName + " has disconnected from router!");
//			
//				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, msg);
//			}
//		}
//	}
//}
//
//void UShimmerVRPNComponent::OnShimmerWAMPComponentConnected(ComponentTypeEnum ComponentType, FString ComponentID)
//{
//	if (ComponentType == ComponentTypeEnum::VE_Shimmer)
//	{
//		if (ShimmerDeviceName == ComponentID)
//		{
//			if (GEngine)
//			{
//				FString msg("Shimmer " + ShimmerDeviceName + " has connected to router!");
//
//				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, msg);
//			}
//		}
//	}
//}
//
//void UShimmerVRPNComponent::ReconnectToRouter()
//{
//	//FReconnectShimmerWAMPWorker::Shutdown();
//	//FReconnectShimmerWAMPWorker::JoyInit(this, ASynOpticonState::GetWAMPRouterAdress(), FString("realm1"), FString("ShimmerDataSample"));
//}

//void UShimmerVRPNComponent::AddShimmerDataSample(FString ShimmerID, FVector2D* ShimmerData)
//{
//	if (!ShimmerDataQueuesMap.Contains(ShimmerID)) {
//		TCircularQueue<FVector2D*>* DataQueue = new TCircularQueue<FVector2D*>(121); //Allow one second worth of data to be stored at a time
//		DataQueue->Enqueue(ShimmerData);
//		ShimmerDataQueuesMap.Add(ShimmerID, DataQueue);
//	}
//	else {
//		TCircularQueue<FVector2D*>** ShimmerDataQueue = ShimmerDataQueuesMap.Find(ShimmerID);
//		if (*ShimmerDataQueue)
//		{
//			bool Success = (*ShimmerDataQueue)->Enqueue(ShimmerData);
//
//			//If the queue was full we need to free the memory we used for the sample
//			if (!Success) {
//				delete ShimmerData;
//			}
//		}
//	}
//}

//void UShimmerVRPNComponent::ToBinaryReplayData()
//{
//	//SOB
//}
//
//void UShimmerVRPNComponent::SetSOBlock(CompressedSOBlock* _SOBlock) {
//	//SOBlock = _SOBlock;
//}

//=================WORKER============================

//FShimmerWAMPWorker* FShimmerWAMPWorker::ShimmerWAMPWorker = nullptr;
//UShimmerVRPNComponent* FShimmerWAMPWorker::ShimmerVRPNComponent = nullptr;
//bool FShimmerWAMPWorker::ForcedStop = false;
//FShimmerWAMPWorker::FShimmerWAMPWorker(UShimmerVRPNComponent* _ShimmerVRPNComponent, FString _ServerIP, FString _Realm, FString _Topic, bool _Replay)
//{
//	ShimmerVRPNComponent = _ShimmerVRPNComponent;
//	ServerIP = _ServerIP;
//	Topic = _Topic;
//	WAMPThread = FRunnableThread::Create(this, TEXT("FShimmerWAMPWorker"), 0, TPri_Normal);
//	Realm = _Realm;
//	IsFinished = false;
//	ClosedManually = false;
//	Replay = _Replay;
//}
//
//FShimmerWAMPWorker::~FShimmerWAMPWorker()
//{
//	if (WAMPThread)
//	{
//		delete WAMPThread;
//		WAMPThread = nullptr;
//	}
//}
//
//uint32 FShimmerWAMPWorker::Run()
//{
//	FPlatformProcess::Sleep(0.03);
//	try {
//		bool debug = false;
//
//		client ws_clinet;
//		ws_clinet.init_asio(&io);
//		std::string websocket_endpoint(TCHAR_TO_UTF8(*ServerIP));
//		auto transport = std::make_shared < autobahn::wamp_websocketpp_websocket_transport<websocketpp::config::asio_client> >(
//			ws_clinet, websocket_endpoint, debug);
//
//		auto session = std::make_shared<autobahn::wamp_session>(io, debug);
//
//		transport->attach(std::static_pointer_cast<autobahn::wamp_transport_handler>(session));
//
//		// Make sure the continuation futures we use do not run out of scope prematurely.
//		// Since we are only using one thread here this can cause the io service to block
//		// as a future generated by a continuation will block waiting for its promise to be
//		// fulfilled when it goes out of scope. This would prevent the session from receiving
//		// responses from the router.
//		boost::future<void> connect_future;
//		boost::future<void> start_future;
//		boost::future<void> join_future;
//		boost::future<void> subscribe_future_eye;
//		boost::future<void> subscribe_future_scene_camera;
//		boost::future<void> leave_future;
//		boost::future<void> stop_future;
//
//		connect_future = transport->connect().then([&](boost::future<void> connected) {
//			try {
//				connected.get();
//			}
//			catch (const std::exception& e) {
//				std::cerr << e.what() << std::endl;
//				io.stop();
//				return;
//			}
//
//
//			start_future = session->start().then([&](boost::future<void> started) {
//				try {
//					started.get();
//				}
//				catch (const std::exception& e) {
//					std::cerr << e.what() << std::endl;
//					io.stop();
//					return;
//				}
//				std::string realm(TCHAR_TO_UTF8(*Realm));
//				join_future = session->join(realm).then([&](boost::future<uint64_t> joined) {
//					try {
//						joined.get();
//					}
//					catch (const std::exception& e) {
//						e.what();
//						io.stop();
//						return;
//					}
//
//					//std::string topic(TCHAR_TO_UTF8(*));
//					if (!Replay) {
//						subscribe_future_eye = session->subscribe("ShimmerDataSample", &FShimmerWAMPWorker::OnReceiveShimmerData).then([&](boost::future<autobahn::wamp_subscription> subscribed)
//						{
//							try {
//								subscribed.get();
//							}
//							catch (const std::exception& e) {
//								e.what();
//								io.stop();
//								return;
//							}
//
//						});
//					}
//					else {
//						//session->publish();
//					}
//				});
//			});
//		});
//
//		io.run();
//		try {
//			FPlatformProcess::Sleep(0.050);
//			if (!ClosedManually) {
//				io.stop();
//
//			}
//
//			leave_future = session->leave().then([&](boost::future<std::string> reason) {
//				try {
//					std::cerr << "left session (" << reason.get() << ")" << std::endl;
//				}
//				catch (const std::exception& e) {
//					std::cerr << "failed to leave session: " << e.what() << std::endl;
//					io.stop();
//					return;
//				}
//			});
//
//			stop_future = session->stop().then([&](boost::future<void> stopped) {
//				std::cerr << "stopped session" << std::endl;
//			});
//
//			//SleepEx(100 + 200 * reconnection_attempts, false);
//			//reconnection_attempts++;
//
//			if (!ClosedManually) {
//				std::cerr << "Reconnecting WAMP" << std::endl;
//				ShimmerVRPNComponent->ReconnectToRouter();
//				//WebsocketStreamer(crossbarAddress, _glassesID);
//			}
//		}
//		catch (const std::exception& e) {
//			e;
//			return 0;
//		}
//
//	}
//	catch (const std::exception& e) {
//		e;
//		return 0;
//	}
//	return 1;
//}
//
//void FShimmerWAMPWorker::EnsureCompletion()
//{
//	io.stop();
//	while (!io.stopped())
//	{
//		//wait until it stops
//	}
//	Stop();
//	ForcedStop = true;
//	WAMPThread->WaitForCompletion();
//}
//
//FShimmerWAMPWorker* FShimmerWAMPWorker::JoyInit(UShimmerVRPNComponent* _ShimmerVRPNComponent, FString _ServerIP, FString _Realm, FString _Topic, bool _Replay)
//{
//	if (!ShimmerWAMPWorker && FPlatformProcess::SupportsMultithreading())
//	{
//		ForcedStop = false;
//		ShimmerWAMPWorker = new FShimmerWAMPWorker(_ShimmerVRPNComponent, _ServerIP, _Realm, _Topic, _Replay);
//	}
//	return ShimmerWAMPWorker;
//}
//
//void FShimmerWAMPWorker::Shutdown()
//{
//	if (ShimmerWAMPWorker)
//	{
//		ShimmerWAMPWorker->ClosedManually = true;
//		ShimmerWAMPWorker->EnsureCompletion();
//		delete ShimmerWAMPWorker;
//		ShimmerWAMPWorker = nullptr;
//	}
//}
//
//bool FShimmerWAMPWorker::IsThreadFinished()
//{
//	return ShimmerWAMPWorker == nullptr;
//}
//
//void FShimmerWAMPWorker::OnReceiveShimmerData(const autobahn::wamp_event& event)
//{
//	//if (ASynOpticonState::HasPlayingReplayBeenStarted()) {
//	//	return;
//	//}
//
//	FString ShimmerID = FString(event.argument<std::string>(0).c_str());
//	std::array<double, 2> data = event.argument<std::array<double, 2>>(1);
//	FVector2D* Sample = new FVector2D(data.at(0), data.at(1));
//	ShimmerVRPNComponent->AddShimmerDataSample(ShimmerID, Sample);
//}
//
//FReconnectShimmerWAMPWorker* FReconnectShimmerWAMPWorker::ReconnectShimmerWAMPWorker = nullptr;
//UShimmerVRPNComponent* FReconnectShimmerWAMPWorker::ShimmerVRPNComponent = nullptr;
//FReconnectShimmerWAMPWorker::FReconnectShimmerWAMPWorker(UShimmerVRPNComponent* _ShimmerVRPNComponent, FString _ServerIP, FString _Realm, FString _Topic)
//{
//	ShimmerVRPNComponent = _ShimmerVRPNComponent;
//	ServerIP = _ServerIP;
//	Topic = _Topic;
//	ReconnectShimmerWAMPThread = FRunnableThread::Create(this, TEXT("FReconnectShimmerWAMPWorker"), 0, TPri_BelowNormal);
//	Realm = _Realm;
//}
//
//FReconnectShimmerWAMPWorker::~FReconnectShimmerWAMPWorker()
//{
//	if (ReconnectShimmerWAMPThread)
//	{
//		delete ReconnectShimmerWAMPThread;
//		ReconnectShimmerWAMPThread = nullptr;
//	}
//}
//
//uint32 FReconnectShimmerWAMPWorker::Run()
//{
//	FPlatformProcess::Sleep(2);
//	while (!FShimmerWAMPWorker::IsThreadFinished())
//	{
//		FShimmerWAMPWorker::Shutdown();
//		FPlatformProcess::Sleep(0.5);
//	}
//
//	FShimmerWAMPWorker::JoyInit(ShimmerVRPNComponent, ServerIP, Realm, Topic, false);
//	return 1;
//}
//
//void FReconnectShimmerWAMPWorker::EnsureCompletion()
//{
//	Stop();
//	ReconnectShimmerWAMPThread->WaitForCompletion();
//}
//
//FReconnectShimmerWAMPWorker* FReconnectShimmerWAMPWorker::JoyInit(UShimmerVRPNComponent* _ShimmerVRPNComponent, FString _ServerIP, FString _Realm, FString _Topic)
//{
//	if (!ReconnectShimmerWAMPWorker && FPlatformProcess::SupportsMultithreading())
//	{
//		ReconnectShimmerWAMPWorker = new FReconnectShimmerWAMPWorker(_ShimmerVRPNComponent, _ServerIP, _Realm, _Topic);
//	}
//	return ReconnectShimmerWAMPWorker;
//}
//
//void FReconnectShimmerWAMPWorker::Shutdown()
//{
//	if (ReconnectShimmerWAMPWorker)
//	{
//		ReconnectShimmerWAMPWorker->EnsureCompletion();
//		delete ReconnectShimmerWAMPWorker;
//		ReconnectShimmerWAMPWorker = nullptr;
//	}
//}