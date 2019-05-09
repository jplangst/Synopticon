// Fill out your copyright notice in the Description page of Project Settings.

#include "GazeParentActor.h"
//#include "Synopticon.h"



// Sets default values
AGazeParentActor::AGazeParentActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGazeParentActor::BeginPlay()
{
	Super::BeginPlay();
	PosOriComp = nullptr;
	InitialLocationSaved = false;
	InitialLocation = FVector(0, 0, 0);
}

// Called every frame
void AGazeParentActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AGazeParentActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	for (int i = 0; i < AttachedObjects.Num(); i++) {
		if (AttachedObjects[i]->IsValidLowLevel() && AttachedObjects[i]) {
			AGazeActor* GazeActor = Cast<AGazeActor>(AttachedObjects[i]);
			if (GazeActor) {
				GazeActor->ClearGazeParent();
			}
		}
	}

	AttachedObjects.Empty();

	PosOriComp = nullptr;
}

void AGazeParentActor::SetSceneCompName(FString _SceneCompName) {
	SceneCompName = _SceneCompName;
}

void AGazeParentActor::CreatePosOriComponent(FString _NatNetName) {
	NatNetName = _NatNetName;

	PosOriComp = NewObject<UPositionAndOrientationComponent>(this, UPositionAndOrientationComponent::StaticClass(), FName(*(GetHumanReadableName() + "PosOriComponent")));
	if (PosOriComp)
	{
		PosOriComp->SetOwnerRigidBodyName(NatNetName);
		UNatNetWAMPComponent* NatNetWAMPComponent = nullptr;
		AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this->GetWorld());
		if (GameMode) {
			TArray<UNatNetWAMPComponent*> NatNetComponents;
			GameMode->GetComponents<UNatNetWAMPComponent>(NatNetComponents);
			
			if (NatNetComponents.Num() > 0) {
				NatNetWAMPComponent = NatNetComponents[0];
			}
		}

		PosOriComp->SetNatNetWAMPComponent(NatNetWAMPComponent);
		PosOriComp->SetMarkersConfiguration(MarkersConfigurationEnum::VE_PositionOnly);
		PosOriComp->HideOrientationArrowMesh();
		PosOriComp->SetShouldApplyOffset(false);
		this->SetRootComponent(PosOriComp);		
	}
}

void AGazeParentActor::SetCalibrationQuat(FQuat CalibrationQuat) {
	UPositionAndOrientationComponent* PosOriComp = Cast<UPositionAndOrientationComponent>(GetRootComponent());
	if (PosOriComp) {
		PosOriComp->SetCalibrationToolCalibration(CalibrationQuat);
	}
}

void AGazeParentActor::RegisterPosOriComponent() {
	if (PosOriComp) {
		PosOriComp->RegisterComponent();
	}	
}

void AGazeParentActor::AddAttachedObject(UObject* AttachedObject) {
	AttachedObjects.Push(AttachedObject);

	if (!InitialLocationSaved) {
		InitialLocation = GetActorLocation();
		InitialLocationSaved = true;
	}
}

void AGazeParentActor::RemoveAttachedObject(UObject* AttachedObject) {
	AttachedObjects.Remove(AttachedObject);
}

void AGazeParentActor::SetInitialLocation(FVector _InitialLocation) {
	if (!_InitialLocation.Equals(FVector(0, 0, 0))) {
		InitialLocationSaved = true;
		SetActorLocation(_InitialLocation);
		InitialLocation = _InitialLocation;
	}	
}