// Fill out your copyright notice in the Description page of Project Settings.

#include "CalibrationActor.h"
//#include "Synopticon.h"


float ACalibrationActor::CalibrationThreshold = 50;

// Sets default values
ACalibrationActor::ACalibrationActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACalibrationActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACalibrationActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ACalibrationActor::SetPositionAndOrientationComponent(UPositionAndOrientationComponent* NewPosOriComponent)
{
	PosOriComponent = NewPosOriComponent;
}

bool ACalibrationActor::CalibrateNearestObject(int32 CalibrationMethod, int32 CalibrationAxis) {
	UPositionAndOrientationComponent* NearestComponent = nullptr;

	//Check all the position and orientation components, ignore the ManualTool, LaserTool and the CalibrationTool by checking names
	TArray<AActor*> GazeActors = ASynOpticonState::GetGazeActorList();
	TArray<AActor*> SOActors = ASynOpticonState::GetSynOpticonActorList();
	TArray<AActor*> GazeParentActor = ASynOpticonState::GetGazeParentActorList();
	SOActors.Append(GazeActors);
	SOActors.Append(GazeParentActor);
	
	float NearestDistance;
	FindNearestPositionAndOrientationActorComponentInList(SOActors, NearestDistance, NearestComponent);

	if (NearestComponent && PosOriComponent) {
		NearestComponent->CalibratePosOriToCalibrationTool(PosOriComponent->GetComponentTransform(), CalibrationMethod, CalibrationAxis);
		return true;
	}

	return false;
}

void ACalibrationActor::FindNearestPositionAndOrientationActorComponentInList(TArray<AActor*> ActorList, float &OutDistance, UPositionAndOrientationComponent* &OutComponent) {
	OutDistance = CalibrationThreshold;

	for (AActor* Actor : ActorList) {
		TArray<UActorComponent*> PosComps = Actor->GetComponentsByClass(UPositionAndOrientationComponent::StaticClass());
		for (UActorComponent* ActorComp : PosComps) {
			UPositionAndOrientationComponent* CurrComp = Cast<UPositionAndOrientationComponent>(ActorComp);
			if (CurrComp) {
				float CurrDist = FVector::Dist(PosOriComponent->GetComponentLocation(), CurrComp->GetComponentLocation());
				if (CurrDist < OutDistance) {
					OutDistance = CurrDist;
					OutComponent = CurrComp;
				}
			}
		}
	}
}
