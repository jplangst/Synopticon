// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatformAffinity.h"
#include "QueuedThreadPool.h"

#include "Object.h"
#include "ThreadPoolManager.generated.h"

/**
 * 
 */
UCLASS()
class SYNOPTICON_API UThreadPoolManager : public UObject
{
	GENERATED_BODY()

	const EThreadPriority ThreadPrio = EThreadPriority::TPri_Highest;
	const int32 NumberOfThreads = 20;
	const int32 StackSize = 10 * 1024; //10 MB
	FQueuedThreadPool* WorkerPool;
	
	public:
	UThreadPoolManager();
	~UThreadPoolManager();

	FQueuedThreadPool* GetWorkerPool();
};
