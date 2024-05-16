// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "WAMP/WAMPWorker.h"

#include "CoreMinimal.h"
#include "Containers/CircularQueue.h"
#include "Components/ActorComponent.h"

#include "../Utils/SynOpticonStatics.h"
#include "../SystemClasses/SynOpticonState.h"
#include "../SystemClasses/SynOpticonCustomType.h "

#include "NatNetWAMPComponent.generated.h"

// Data callback function prototype

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SYNOPTICON_API UNatNetWAMPComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	TMap<FString, TCircularQueue<FRigidBodyDataStruct*>*> NatNetDataQueuesMap;

public:

	// Sets default values for this component's properties
	UNatNetWAMPComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called when the game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called every frame
	TArray<FString> AvailableRigidBodies;

	FRigidBodyDataStruct* GetNatNetDataSample(FString RigidBodyID);

	UFUNCTION(BlueprintCallable, Category = "NatNet Information")
		TArray<FString> GetRigidBodies();

	UFUNCTION()
		void OnNatNetWAMPComponentDisconnected(ComponentTypeEnum ComponentType, FString ComponentID);
	UFUNCTION()
		void OnNatNetWAMPComponentConnected(ComponentTypeEnum ComponentType, FString ComponentID);

	void OnReceiveNatNetData(const string _event);

	void RegisterWAMP();
};