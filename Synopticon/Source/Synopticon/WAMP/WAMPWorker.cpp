// Fill out your copyright notice in the Description page of Project Settings.

#include "WAMPWorker.h"

int64 FWAMPWorker::ID = 0;
FWAMPWorker* FWAMPWorker::WAMPWorker = nullptr;
//TMap<FString, const wamp_event_handler> FWAMPWorker::WAMPTopics;
TArray<TPair<FString, TSharedPtr<wamp_event_handler>>> FWAMPWorker::WAMPTopics;
bool FWAMPWorker::ForcedStop = false;

TQueue<FPublishWAMPEventStruct*> FWAMPWorker::WAMPEventsToPublish;

FWAMPWorker::FWAMPWorker(FString _ServerIP, FString _Realm)
{
	ServerIP = _ServerIP;
	Realm = _Realm;
	IsFinished = false;
	ClosedManually = false;
	ForcedStop = false;

	WAMPThread = FRunnableThread::Create(this, TEXT("FWAMPWorker" + FWAMPWorker::ID), 0, TPri_Normal);

	FWAMPWorker::ID++;
}

FWAMPWorker::~FWAMPWorker()
{
	if (WAMPThread)
	{
		delete WAMPThread;
		WAMPThread = nullptr;
	}

	FPublishWAMPEventStruct* EventToPublish;
	while (WAMPEventsToPublish.Dequeue(EventToPublish)) { //While we have events to publish we keep publishing
		if (EventToPublish) 
			delete EventToPublish;
	}
}

void FWAMPWorker::SubscribeToTopic(FString Topic, TSharedPtr<wamp_event_handler> Handler)
{
	//if (!WAMPTopics.Contains(Topic))
	//{
	//	WAMPTopics.Add(Topic, Handler);
	//}
	auto WAMPTopic = TPair<FString, TSharedPtr<wamp_event_handler>>(Topic, Handler);
	WAMPTopics.Add(WAMPTopic);
}

uint32 FWAMPWorker::Run()
{
	FPlatformProcess::Sleep(0.03);
	try {
		bool debug = false;

		client ws_clinet;
		ws_clinet.init_asio(&io);
		std::string websocket_endpoint(TCHAR_TO_UTF8(*ServerIP));
		auto transport = std::make_shared < autobahn::wamp_websocketpp_websocket_transport<websocketpp::config::asio_client> >(
			ws_clinet, websocket_endpoint, debug);

		auto session = std::make_shared<autobahn::wamp_session>(io, debug);

		transport->attach(std::static_pointer_cast<autobahn::wamp_transport_handler>(session));

		// Make sure the continuation futures we use do not run out of scope prematurely.
		// Since we are only using one thread here this can cause the io service to block
		// as a future generated by a continuation will block waiting for its promise to be
		// fulfilled when it goes out of scope. This would prevent the session from receiving
		// responses from the router.
		boost::future<void> connect_future;
		boost::future<void> start_future;
		boost::future<void> join_future;
		boost::future<void> subscribe_future_eye;
		boost::future<void> subscribe_future_scene_camera;
		boost::future<void> leave_future;
		boost::future<void> stop_future;

		connect_future = transport->connect().then([&](boost::future<void> connected) {
			try {
				connected.get();
			}
			catch (const std::exception& e) {
				std::cerr << e.what() << std::endl;
				io.stop();
				return;
			}


			start_future = session->start().then([&](boost::future<void> started) {
				try {
					started.get();
				}
				catch (const std::exception& e) {
					std::cerr << e.what() << std::endl;
					io.stop();
					return;
				}
				std::string realm(TCHAR_TO_UTF8(*Realm));
				join_future = session->join(realm).then([&](boost::future<uint64_t> joined) {
					try {
						joined.get();
					}
					catch (const std::exception& e) {
						e.what();
						io.stop();
						return;
					}

					//std::string topic(TCHAR_TO_UTF8(*));
					for (auto& WAMPTopic : WAMPTopics)
					{
						std::string Topic(TCHAR_TO_UTF8(*WAMPTopic.Key));
						auto TopicHandler = *WAMPTopic.Value;

						//const wamp_event_handler TopicHandler = WAMPTopic.Value;
						subscribe_future_eye = session->subscribe(Topic, TopicHandler).then([&](boost::future<autobahn::wamp_subscription> subscribed)
						{
							try {
								subscribed.get();
							}
							catch (const std::exception& e) {
								e.what();
								io.stop();
								return;
							}

						});
					}

					//Try to stream at a rate of 60fps
					while (!ForcedStop){ //&& !ClosedManually) {
						PublishEvents(session);
				
						FPlatformProcess::Sleep(0.017);
					}

					//subscribe_future_scene_camera = session->subscribe("ETGSceneCamera", &FWAMPWorker::OnReceiveSceneCamera).then([&](boost::future<autobahn::wamp_subscription> subscribed)
					//{
					//	try {
					//		subscribed.get();
					//	}
					//	catch (const std::exception& e) {
					//		e.what();
					//		io.stop();
					//		return;
					//	}

					//});

				});
			});
		});

		io.run();
		try {
			FPlatformProcess::Sleep(0.050);
			if (!ClosedManually) {
				io.stop();

			}

			leave_future = session->leave().then([&](boost::future<std::string> reason) {
				try {
					std::cerr << "left session (" << reason.get() << ")" << std::endl;
				}
				catch (const std::exception& e) {
					std::cerr << "failed to leave session: " << e.what() << std::endl;
					io.stop();
					return;
				}
			});

			stop_future = session->stop().then([&](boost::future<void> stopped) {
				std::cerr << "stopped session" << std::endl;
			});

			//SleepEx(100 + 200 * reconnection_attempts, false);
			//reconnection_attempts++;

			if (!ClosedManually && !ForcedStop) {
				std::cerr << "Reconnecting WAMP" << std::endl;
				Reconnect(ServerIP, Realm);
			}
		}
		catch (const std::exception& e) {
			e;
			return 0;
		}

	}
	catch (const std::exception& e) {
		e;
		return 0;
	}
	return 1;
}

void FWAMPWorker::PublishEvents(std::shared_ptr<autobahn::wamp_session> session) {
	FPublishWAMPEventStruct* EventToPublish;
	while (WAMPEventsToPublish.Dequeue(EventToPublish)) { //While we have events to publish we keep publishing
		if (EventToPublish) {
			switch (EventToPublish->EventType)
			{
			case PublishWAMPEventType::EyeEvent:
			{
				FEyeEventStruct* EyeEvent = (FEyeEventStruct*)EventToPublish;
				session->publish(EyeEvent->GetWAMPTopic(), EyeEvent->GetWAMPArguments());
				break;
			}
			case PublishWAMPEventType::RecordingEvent:
			{
				FRecordingEventStruct* RecordingEvent = (FRecordingEventStruct*)EventToPublish;
				session->publish(RecordingEvent->GetWAMPTopic(), RecordingEvent->GetWAMPArguments());
				break;
			}
			case PublishWAMPEventType::NotSet:
			{
				break;
			}
			}

			delete EventToPublish;
		}		
	}
}

void FWAMPWorker::EnsureCompletion()
{
	io.stop();
	while (!io.stopped())
	{
		//wait until it stops
	}
	Stop();
	ForcedStop = true;
	if (WAMPThread) {
		WAMPThread->WaitForCompletion();
	}
}

FWAMPWorker* FWAMPWorker::JoyInit(FString _ServerIP, FString _Realm)
{
	if (!WAMPWorker && FPlatformProcess::SupportsMultithreading())
	{
		ForcedStop = false;
		WAMPWorker = new FWAMPWorker(_ServerIP, _Realm);
	}
	return WAMPWorker;
}

void FWAMPWorker::Shutdown()
{
	if (WAMPWorker)
	{
		WAMPWorker->ClosedManually = true;
		WAMPWorker->EnsureCompletion();
		delete WAMPWorker;
		WAMPWorker = nullptr;
	}
	//FReconnectWAMPWorker::Shutdown();
}

bool FWAMPWorker::IsThreadFinished()
{
	return WAMPWorker == nullptr;
}

void FWAMPWorker::PublishWAMPEvent(FPublishWAMPEventStruct* WAMPEvent)
{
	WAMPEventsToPublish.Enqueue(WAMPEvent);
}

void FWAMPWorker::CleanUp()
{
	//TArray<TPair<FString, TSharedPtr<wamp_event_handler>>> WAMPTopics
	for (TPair<FString, TSharedPtr<wamp_event_handler>> Topic : WAMPTopics)
	{
		Topic.Value.Reset();
		Topic.Value = nullptr;
	}
	WAMPTopics.Empty();
}

void FWAMPWorker::Reconnect(FString _ServerIP, FString _Realm)
{
	FReconnectWAMPWorker::Shutdown();
	FReconnectWAMPWorker::JoyInit(_ServerIP, _Realm);
}

//================= Reconnect WORKER================
FReconnectWAMPWorker* FReconnectWAMPWorker::ReconnectWAMPWorker = nullptr;
int64 FReconnectWAMPWorker::ID = 0;
//int32 FReconnectWAMPWorker::ReconnectAttempts = 0;
FReconnectWAMPWorker::FReconnectWAMPWorker(FString _ServerIP, FString _Realm)
{
	ServerIP = _ServerIP;
	Realm = _Realm;
	ReconnectWAMPThread = FRunnableThread::Create(this, TEXT("FReconnectWAMPWorker" + FReconnectWAMPWorker::ID), 0, TPri_BelowNormal);
	
	FReconnectWAMPWorker::ID++;
}

FReconnectWAMPWorker::~FReconnectWAMPWorker()
{
	if (ReconnectWAMPThread)
	{
		delete ReconnectWAMPThread;
		ReconnectWAMPThread = nullptr;
	}
}

uint32 FReconnectWAMPWorker::Run()
{
	
	FPlatformProcess::Sleep(2);
	while (!FWAMPWorker::IsThreadFinished())
	{
		FWAMPWorker::Shutdown();
		FPlatformProcess::Sleep(1);

	}

	FWAMPWorker::JoyInit(ServerIP, Realm);
	return 1;
}

void FReconnectWAMPWorker::EnsureCompletion()
{
	Stop();
	ReconnectWAMPThread->WaitForCompletion();
}

FReconnectWAMPWorker* FReconnectWAMPWorker::JoyInit(FString _ServerIP, FString _Realm)
{
	if (!ReconnectWAMPWorker && FPlatformProcess::SupportsMultithreading())
	{
		ReconnectWAMPWorker = new FReconnectWAMPWorker(_ServerIP, _Realm);
	}
	return ReconnectWAMPWorker;
}

void FReconnectWAMPWorker::Shutdown()
{
	if (ReconnectWAMPWorker)
	{
		ReconnectWAMPWorker->EnsureCompletion();
		delete ReconnectWAMPWorker;
		ReconnectWAMPWorker = nullptr;
	}
}
