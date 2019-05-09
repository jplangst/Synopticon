// Fill out your copyright notice in the Description page of Project Settings.

#include "HandComponent.h"
//#include "Synopticon.h"

int32 UHandComponent::HandID = 0;
// Sets default values for this component's properties
UHandComponent::UHandComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	FString HandMeshName = "HandMesh" + FString::FromInt(UHandComponent::HandID);

	HandMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName(*HandMeshName));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> LeftHandMeshObj(TEXT("StaticMesh'/Game/Models/hand/lefthand.lefthand'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RightHandMeshObj(TEXT("StaticMesh'/Game/Models/hand/righthand.righthand'"));
	LeftHandMesh = LeftHandMeshObj.Object;
	RightHandMesh = RightHandMeshObj.Object;
	
	FVector Min;
	FVector Max;
	HandMesh->GetLocalBounds(Min, Max);
	DefaultHandLength = Max.X - Min.X;
	HandLength = DefaultHandLength;
	XScale = 1;
	HitTargetID = -1;

	UHandComponent::HandID++;
}

// Called when the game starts
void UHandComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (LeftHand) {
		HandMesh->SetStaticMesh(LeftHandMesh);		
	}
	else {
		HandMesh->SetStaticMesh(RightHandMesh);
	}

	//Collision Setup
	TScriptDelegate<FWeakObjectPtr> onBeginOverlapFunc;
	onBeginOverlapFunc.BindUFunction(this, "OnBeginOverlap");

	TScriptDelegate<FWeakObjectPtr> onEndOverlapFunc;
	onEndOverlapFunc.BindUFunction(this, "OnEndOverlap");

	HandMesh->OnComponentBeginOverlap.Add(onBeginOverlapFunc);
	HandMesh->OnComponentEndOverlap.Add(onEndOverlapFunc);

	HandMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
	HandMesh->SetCollisionProfileName("Custom");
	HandMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	HandMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore); //GameTraceChannel3 = HandOverlap
	HandMesh->SetGenerateOverlapEvents(true);
	HandMesh->RegisterComponent();

	HitTargetID = -1;
}


// Called every frame
void UHandComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
	if (ASynOpticonState::IsReplaying()
		&& !ASynOpticonState::IsPausingReplay()) {
		ReplayData();
	}
}

void UHandComponent::SetHandDataBlock(HandDataBlock* _HandBlock) {
	HandBlock = _HandBlock;
}

// Called when the game ends
void UHandComponent::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);

	if (HandBlock) {
		HandBlock = nullptr;
	}

	LeftHandMesh = nullptr;
	RightHandMesh = nullptr;

	HandMesh->DestroyComponent();
}

void UHandComponent::SetComponentData(FHandStruct _HandData) {
	HandData = _HandData;
}

FHandStruct UHandComponent::GetComponentData() {
	return HandData;
}

void UHandComponent::SetPosOriComponent(UPositionAndOrientationComponent* _PosOriComp) {
	PosOriComp = _PosOriComp;
	PosOriComp->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
	HandMesh->AttachToComponent(PosOriComp, FAttachmentTransformRules::KeepRelativeTransform);
}

void UHandComponent::OnBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	if (GEngine) {
		//GEngine->AddOnScreenDebugMessage(0, 5, FColor::Black, OtherComp->GetName());
		AGazeActor* GazeActor = Cast<AGazeActor>(OtherComp);
		if (GazeActor)
		{
			/*if (GazeActor->GetGazeActorType() == EStaticRoomActorType::SRT_ActionObject)
			{*/
				HitTargetID = GazeActor->GetHitID(false);
			//}

		}
	}
}

void UHandComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	HitTargetID = -1;
}

int32 UHandComponent::GetHitTargetID()
{
	return HitTargetID;;
}

void UHandComponent::ToBinaryReplayData() {
	HandBlock->HandLocation = PosOriComp->GetComponentTransform().GetLocation();
	HandBlock->HandRotation = PosOriComp->GetComponentTransform().GetRotation();
}

void UHandComponent::ReplayData() {
	PosOriComp->SetWorldLocation(HandBlock->HandLocation);
	PosOriComp->SetWorldRotation(HandBlock->HandRotation);
}