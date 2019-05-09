// Fill out your copyright notice in the Description page of Project Settings.

#include "SynOpticonPawn.h"
//#include "Synopticon.h"

#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/PlayerInput.h"

FName ASynOpticonPawn::MovementComponentName(TEXT("MovementComponent0"));

// Sets default values
ASynOpticonPawn::ASynOpticonPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	BaseEyeHeight = 0.0f;
	bCollideWhenPlacing = false;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	MovementComponent = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>("SynOpticonMovement"); //ADefaultPawn::MovementComponentName

	// This is the default pawn class, we want to have it be able to move out of the box.
	bAddDefaultMovementBindings = false;

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	MovementSpeedFactor = 0.5f;

	AllowMovement = true;
}

// Called when the game starts or when spawned
void ASynOpticonPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASynOpticonPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void ASynOpticonPawn::SetupPlayerInputComponent(class UInputComponent* InInputComponent)
{
	Super::SetupPlayerInputComponent(InInputComponent);

	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_MoveForward2", EKeys::W, 1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_MoveForward2", EKeys::S, -1.f));	
	//UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_MoveForward", EKeys::Up, 1.f));
	//UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_MoveForward", EKeys::Down, -1.f));
	//UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_MoveForward", EKeys::Gamepad_LeftY, 1.f));

	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_MoveRight2", EKeys::A, -1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_MoveRight2", EKeys::D, 1.f));
	//UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_MoveRight", EKeys::Gamepad_LeftX, 1.f));

	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_MoveUp2", EKeys::Q, -1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_MoveUP2", EKeys::E, 1.f));

	//UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_TurnRate", EKeys::Gamepad_RightX, 1.f));
	//UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_TurnRate", EKeys::Left, -1.f));
	//UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_TurnRate", EKeys::Right, 1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_Turn2", EKeys::MouseX, 1.f));

	//UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_LookUpRate", EKeys::Gamepad_RightY, 1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("DefaultPawn_LookUp2", EKeys::MouseY, -1.f));

	InInputComponent->BindAxis("DefaultPawn_MoveForward2", this, &ASynOpticonPawn::MoveForward);
	InInputComponent->BindAxis("DefaultPawn_MoveRight2", this, &ASynOpticonPawn::MoveRight);
	InInputComponent->BindAxis("DefaultPawn_MoveUp2", this, &ASynOpticonPawn::MoveUp_World);
	InInputComponent->BindAxis("DefaultPawn_Turn2", this, &ASynOpticonPawn::AddControllerYawInput);
	//InInputComponent->BindAxis("DefaultPawn_TurnRate", this, &ASynOpticonPawn::TurnAtRate);
	InInputComponent->BindAxis("DefaultPawn_LookUp2", this, &ASynOpticonPawn::AddControllerPitchInput);
	//InInputComponent->BindAxis("DefaultPawn_LookUpRate", this, &ASynOpticonPawn::LookUpAtRate);
}

void ASynOpticonPawn::MoveRight(float Val)
{
	if (!AllowMovement) {
		return;
	}

	if (Val != 0.f)
	{
		if (Controller)
		{
			FRotator const ControlSpaceRot = Controller->GetControlRotation();

			// transform to world space and add it
			AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::Y), Val*MovementSpeedFactor);
		}
	}
}

void ASynOpticonPawn::MoveForward(float Val)
{
	if (!AllowMovement) {
		return;
	}

	if (Val != 0.f)
	{
		if (Controller)
		{
			FRotator const ControlSpaceRot = Controller->GetControlRotation();

			// transform to world space and add it
			AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::X), Val*MovementSpeedFactor);
		}
	}
}

void ASynOpticonPawn::MoveUp_World(float Val)
{
	if (!AllowMovement) {
		return;
	}

	if (Val != 0.f)
	{
		AddMovementInput(FVector::UpVector, Val*MovementSpeedFactor);
	}
}

void ASynOpticonPawn::AddMovementRate(float Rate) {
	MovementSpeedFactor += Rate;
	
	if (MovementSpeedFactor < 0.1) {
		MovementSpeedFactor = 0.1;
	}
	else if (MovementSpeedFactor > 4) {
		MovementSpeedFactor = 4;
	}
}

void ASynOpticonPawn::TurnAtRate(float Rate)
{
	if (!AllowMovement) {
		return;
	}

	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
}

void ASynOpticonPawn::LookUpAtRate(float Rate)
{
	if (!AllowMovement) {
		return;
	}

	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
}

void ASynOpticonPawn::LookUp(float Val)
{
	if (!AllowMovement) {
		return;
	}

	FRotator NewRotation = GetActorRotation();
	NewRotation.Pitch += Val;
	SetActorRotation(NewRotation);
}

void ASynOpticonPawn::Turn(float Val)
{
	if (!AllowMovement) {
		return;
	}

	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += Val;
	SetActorRotation(NewRotation);
}

UPawnMovementComponent* ASynOpticonPawn::GetMovementComponent() const
{
	return MovementComponent;
}

void ASynOpticonPawn::SetAllowMovement(bool _AllowMovement) {
	AllowMovement = _AllowMovement;
}