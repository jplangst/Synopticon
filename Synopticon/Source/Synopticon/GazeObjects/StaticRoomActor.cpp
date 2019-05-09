// Fill out your copyright notice in the Description page of Project Settings.

#include "StaticRoomActor.h"
//#include "Synopticon.h"



// Sets default values
AStaticRoomActor::AStaticRoomActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStaticRoomActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStaticRoomActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AStaticRoomActor::CreateStaticRoomActor(FStaticRoomData RoomData) {
	StaticRoomData = RoomData;


}

void AStaticRoomActor::SetID(int32 _ID) {
	ID = _ID;
}

int32 AStaticRoomActor::GetID() {
	return ID;
}