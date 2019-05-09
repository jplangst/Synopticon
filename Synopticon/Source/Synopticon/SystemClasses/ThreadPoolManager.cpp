// Fill out your copyright notice in the Description page of Project Settings.

#include "ThreadPoolManager.h"
//#include "Synopticon.h"


UThreadPoolManager::UThreadPoolManager() {
	WorkerPool = FQueuedThreadPool::Allocate();
	WorkerPool->Create(NumberOfThreads, StackSize, ThreadPrio);
}

UThreadPoolManager::~UThreadPoolManager() {
	if (WorkerPool) {
		WorkerPool->Destroy();
	}	
}

FQueuedThreadPool* UThreadPoolManager::GetWorkerPool() {
	return WorkerPool;
}
