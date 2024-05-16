// Fill out your copyright notice in the Description page of Project Settings.

#include "PositionAndOrientationComponent.h"
#include "../GazeObjects/GazeParentActor.h"
//#include "Synopticon.h"

int32 UPositionAndOrientationComponent::CompID = 0;

// Sets default values for this component's properties
UPositionAndOrientationComponent::UPositionAndOrientationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bActorHasETG = false;

	//create the orientation vector
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ArrowMeshObj(TEXT("StaticMesh'/Game/Models/arrow/arrow.arrow'"));
	ArrowMesh = ArrowMeshObj.Object;
	static ConstructorHelpers::FObjectFinder<UMaterial>YellowMaterial(TEXT("Material'/Game/Materials/SynOpticonActorMaterials/OrientationVectorMaterial.OrientationVectorMaterial'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> VectorMaterial(TEXT("Material'/Game/Materials/SynOpticonActorMaterials/VectorMaterial.VectorMaterial'"));
	ArrowMaterial = UMaterialInstanceDynamic::Create(VectorMaterial.Object, NULL);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshObj(TEXT("StaticMesh'/Game/Models/glasses/sphere.sphere'"));
	SphereMesh = SphereMeshObj.Object;
	static ConstructorHelpers::FObjectFinder<UMaterial>SphereMaterialObj(TEXT("Material'/Game/Materials/arrowMaterial.arrowMaterial'"));
	SphereMaterial = SphereMaterialObj.Object;

	FString ArrowMeshName = "OrientationArrowMesh" + FString::FromInt(UPositionAndOrientationComponent::CompID);
	ActorOrientationArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName(*ArrowMeshName));
	ActorOrientationArrowMesh->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

	ShouldShowOrientationArrowMesh = true;

	Markers.Empty();
	Calibrated = false;
	CalibrationRotator = FRotator(0, 0, 0);
	ActorLocationOffset.Set(0, 0, 0);
	ShouldApplyOffset = false;

	HasBeenCalibratedByCalibrationTool = false;
	CalibrationToolOffset = FVector(0,0,0);
	CalibrationToolRotation = FQuat(0,0,0,1);

	UPositionAndOrientationComponent::CompID++;
}

// Called when the game starts
void UPositionAndOrientationComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->IsA<AGazeParentActor>()) {
		ActorOrientationArrowMesh->SetStaticMesh(SphereMesh);
		ActorOrientationArrowMesh->SetMaterial(0, SphereMaterial);
	}
	else {
		ActorOrientationArrowMesh->SetStaticMesh(ArrowMesh);
		ActorOrientationArrowMesh->SetMaterial(0, ArrowMaterial);
		ActorOrientationArrowMesh->SetWorldScale3D(FVector(5, 1, 1));
	}

	ActorOrientationArrowMesh->RegisterComponent();
	SOBlock = nullptr;
	
	if (ShouldShowOrientationArrowMesh) 
	{
		ActorOrientationArrowMesh->SetVisibility(ASynOpticonState::IsVectorVisible(VectorVisibilityEnum::VE_OrientationVector), false);
		//ASynOpticonState::GetGlobalEventSystem()->OnEventToggleVectorsVisibility.AddDynamic(this, &UPositionAndOrientationComponent::UpdateVectorVisibility);
	}

	HasBeenCalibratedByCalibrationTool = false;
	ShouldResetToolCalibrationAndCalibrate = false;
	CalDelay = 0;
	
	// ...
	//DrawDebugLine(
	//	GetWorld(),
	//	GetOwner()->GetActorLocation(),
	//	GetOwner()->GetActorRotation().Vector() * 1000,
	//	FColor(255, 0, 0),
	//	true, -1, 0,
	//	5
	//);
}


// Called every frame
void UPositionAndOrientationComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	UpdateVectorVisibility();
	if (!ASynOpticonState::IsReplaying())
	{
		UpdateWAMPPositionData(DeltaTime);
	}
	else
	{
		UpdateReplayData();
	}
}

void UPositionAndOrientationComponent::ScaleOrientationMesh(double ScaleFactor)
{
	if (!bActorHasETG)
	{
		if (ScaleFactor != -1 && ScaleFactor != 0)
		{
			ActorOrientationArrowMesh->SetRelativeScale3D(FVector(ScaleFactor * 0.01, 1, 1));
		}
		else
		{
			ActorOrientationArrowMesh->SetRelativeScale3D(FVector(1, 1, 1));
		}
	}
}

void UPositionAndOrientationComponent::SetActorHasETG(bool ActorHasETG)
{
	bActorHasETG = ActorHasETG;
}

void UPositionAndOrientationComponent::SetNatNetWAMPComponent(UNatNetWAMPComponent* _NatNetWAMPComponent)
{
	NatNetWAMPComponent = _NatNetWAMPComponent;
}

UNatNetWAMPComponent* UPositionAndOrientationComponent::GetNatNetWAMPComponent()
{
	return NatNetWAMPComponent;
}

void UPositionAndOrientationComponent::SetSOBlock(CompressedSOBlock* _SOBlock) {
	SOBlock = _SOBlock;
}

void UPositionAndOrientationComponent::SetMarkersConfiguration(MarkersConfigurationEnum _MarkersConfiguration)
{
	MarkersConfiguration = _MarkersConfiguration;
}

void UPositionAndOrientationComponent::UpdateWAMPPositionData(float DeltaTime)
{
	if (NatNetWAMPComponent) 
	{
		FRigidBodyDataStruct* NatNetDataSample = NatNetWAMPComponent->GetNatNetDataSample(OwnerRigidBodyName);
		if (NatNetDataSample) 
		{
			if (MarkersConfiguration == MarkersConfigurationEnum::VE_PositionOnly) {
				if (HasBeenCalibratedByCalibrationTool) {
					SetWorldRotation(CalibrationToolRotation);

					this->SetWorldLocation(NatNetDataSample->Position);
					AddLocalOffset(CalibrationToolOffset);
					FTransform NewWorldTransform = FTransform(NatNetDataSample->Orientation, NatNetDataSample->Position);
					//FTransform NewRelativeTransform = UKismetMathLibrary::ConvertTransformToRelative(BeforeCalibrationToolTransform, NewWorldTransform);
					FTransform NewRelativeTransform = UKismetMathLibrary::MakeRelativeTransform(NewWorldTransform, BeforeCalibrationToolTransform);
					this->AddRelativeRotation(NewRelativeTransform.GetRotation());
						
				}
				else {
					this->SetWorldLocation(NatNetDataSample->Position);
					this->SetWorldRotation(NatNetDataSample->Orientation);
				}
			}
			else {

				TMap<float, FVector> IDToDistanceMap;

				for (FVector Marker : NatNetDataSample->Markers)
				{
					float Dist = FVector::Dist(NatNetDataSample->Position, Marker);
					IDToDistanceMap.Add(Dist, Marker);
				}

				IDToDistanceMap.KeySort([](const float& A, const float& B)
				{
					return A < B;
				});

				IDToDistanceMap.GenerateValueArray(Markers);

				if (Markers.Num() >= 3)
				{
					FVector PivotMarker = Markers[0];
					FVector CloseMarker = Markers[1];
					FVector FarMarker = Markers[2];

					if (MarkersConfiguration == MarkersConfigurationEnum::VE_Glasses_Right)
					{						
						//WARNING: DO NOT NORMALIZE OR IT WILL BE COMPLETELY WRONG
						//Have lunch before you fix any problem. That's help.
						FVector UpVector = CloseMarker - FarMarker;
						FVector RightVector = CloseMarker - PivotMarker;
						FVector ForwardVector = FVector::CrossProduct(RightVector, UpVector);
													
						//WARNING: SHOULD NOT USE MAKEFROM...
						FMatrix axis;
						axis.SetAxes(&ForwardVector, &RightVector, &UpVector, &PivotMarker);

						this->SetWorldLocationAndRotation(NatNetDataSample->Position, axis.Rotator());
					}


					else if (MarkersConfiguration == MarkersConfigurationEnum::VE_Glasses_Left)
					{
						//WARNING: DO NOT NORMALIZE OR IT WILL BE COMPLETELY WRONG
						//Have lunch before you fix any problem. That's help.
						FVector UpVector = CloseMarker - FarMarker;
						FVector RightVector = PivotMarker - CloseMarker;
						FVector ForwardVector = FVector::CrossProduct(RightVector, UpVector);

						//WARNING: SHOULD NOT USE MAKEFROM...
						FMatrix axis;
						axis.SetAxes(&ForwardVector, &RightVector, &UpVector, &PivotMarker);

						if (ASynOpticonState::IsShowingDebugLines())
						{
							DrawDebugCircle(GetWorld(), PivotMarker, 2, 24, FColor::Yellow);
							DrawDebugCircle(GetWorld(), CloseMarker, 2, 24, FColor::Green);
							DrawDebugCircle(GetWorld(), FarMarker, 2, 24, FColor::Red);

							DrawDebugLine(
								GetWorld(),
								PivotMarker,
								PivotMarker + RightVector * 100,
								FColor(0, 255, 0), //green
								false, 0.1, 0,
								1
							);
							DrawDebugLine(
								GetWorld(),
								PivotMarker,
								PivotMarker + UpVector * 100,
								FColor(0, 0, 255),
								false, 0.1, 0,
								1
							);
							DrawDebugLine(
								GetWorld(),
								PivotMarker,
								PivotMarker + ForwardVector * 100,
								FColor(255, 0, 0),
								false, 0.1, 0,
								1
							);
						}

						this->SetWorldLocationAndRotation(NatNetDataSample->Position, axis.Rotator());
					}
						
					else if (MarkersConfiguration == MarkersConfigurationEnum::VE_Position) //movable object
					{
							FVector UpVector = CloseMarker - FarMarker;
							FVector RightVector = CloseMarker - PivotMarker;
							FVector ForwardVector = FVector::CrossProduct(UpVector, RightVector);
							FVector Center = (PivotMarker + FarMarker) / 2;

							FMatrix axis;
							axis.SetAxes(&ForwardVector, &RightVector, &UpVector, &Center);

							this->SetWorldLocationAndRotation(Center, axis.Rotator());

							if (ASynOpticonState::IsShowingDebugLines())
							{
								DrawDebugCircle(GetWorld(), PivotMarker, 2, 24, FColor::Yellow);
								DrawDebugCircle(GetWorld(), CloseMarker, 2, 24, FColor::Green);
								DrawDebugCircle(GetWorld(), FarMarker, 2, 24, FColor::Red);

								DrawDebugLine(
									GetWorld(),
									Center,
									Center + RightVector * 100,
									FColor(0, 255, 0), //green
									false, 0.1, 0,
									1
								);
								DrawDebugLine(
									GetWorld(),
									Center,
									Center + UpVector * 100,
									FColor(0, 0, 255),
									false, 0.1, 0,
									1
								);
								DrawDebugLine(
									GetWorld(),
									Center,
									Center + ForwardVector * 100,
									FColor(255, 0, 0),
									false, 0.1, 0,
									1
								);
							}
					}
					else if (MarkersConfiguration == MarkersConfigurationEnum::VE_Laser)
					{
						//FVector Location = (PivotMarker + CloseMarker) / 2; //Centre of the cal tool
						//FVector RightVector;
						//FVector UpVector;
						//FVector ForwardVector;
						//RightVector = CloseMarker - PivotMarker;
						//ForwardVector = FarMarker - PivotMarker;
						//UpVector = FVector::CrossProduct(ForwardVector, RightVector);

						//RightVector.Normalize();
						//ForwardVector.Normalize();
						//UpVector.Normalize();

						//if (ASynOpticonState::IsShowingDebugLines())
						//{
						//	DrawDebugLine(
						//		GetWorld(),
						//		this->GetComponentLocation(),
						//		this->GetComponentLocation() + RightVector * 100,
						//		FColor(0, 255, 0), //green
						//		false, 0.1, 0,
						//		1
						//	);
						//	DrawDebugLine(
						//		GetWorld(),
						//		this->GetComponentLocation(),
						//		this->GetComponentLocation() + UpVector * 100,
						//		FColor(0, 0, 255),
						//		false, 0.1, 0,
						//		1
						//	);
						//	DrawDebugLine(
						//		GetWorld(),
						//		this->GetComponentLocation(),
						//		this->GetComponentLocation() + ForwardVector * 100,
						//		FColor(255, 0, 0),
						//		false, 0.1, 0,
						//		1
						//	);
						//}

						//FMatrix axis;
						//axis.SetAxes(&ForwardVector, &RightVector, &UpVector, &Location);

						//this->SetWorldLocationAndRotation(Location, axis.Rotator());
						FVector Location = (CloseMarker + FarMarker) / 2;
						FVector ForwardVector = PivotMarker - CloseMarker;
						FVector RightVector = FarMarker - CloseMarker;
						FVector UpVector = FVector::CrossProduct(ForwardVector, RightVector);
						ForwardVector.Normalize();
						RightVector.Normalize();
						UpVector.Normalize();

						FMatrix axis;
						axis.SetAxes(&ForwardVector, &RightVector, &UpVector, &Location);

						this->SetWorldLocationAndRotation(Location, axis.Rotator());
						if (ASynOpticonState::IsShowingDebugLines())
						{
							DrawDebugCircle(GetWorld(), PivotMarker, 2, 24, FColor::Yellow);
							DrawDebugCircle(GetWorld(), CloseMarker, 2, 24, FColor::Green);
							DrawDebugCircle(GetWorld(), FarMarker, 2, 24, FColor::Red);

							DrawDebugLine(
								GetWorld(),
								CloseMarker,
								CloseMarker + RightVector * 100,
								FColor(0, 255, 0), //green
								false, 0.1, 0,
								1
							);
							DrawDebugLine(
								GetWorld(),
								CloseMarker,
								CloseMarker + UpVector * 100,
								FColor(0, 0, 255),
								false, 0.1, 0,
								1
							);
							DrawDebugLine(
								GetWorld(),
								CloseMarker,
								CloseMarker + ForwardVector * 100,
								FColor(255, 0, 0),
								false, 0.1, 0,
								1
							);
						}
					}
					else if (MarkersConfiguration == MarkersConfigurationEnum::VE_CalibrationTool)
					{
						FVector Location = (CloseMarker + FarMarker) / 2; //Centre of the cal tool
						FVector RightVector;
						FVector UpVector;
						FVector ForwardVector; 

						if (ASynOpticonState::GetCalToolForwardAxis() == 0) {
							//Forward = CrossProduct(RightVector, UpVector)
							//Right   = CloseMarker - PivotMarker;
							//Up      = CloseMarker - FarMarker;

							RightVector = FarMarker - PivotMarker;
							UpVector = CloseMarker - PivotMarker;
							ForwardVector = FVector::CrossProduct(RightVector, UpVector);
						}
						else if (ASynOpticonState::GetCalToolForwardAxis() == 1) {
							//Forward = CloseMarker - PivotMarker;
							//Right   = CloseMarker - FarMarker;
							//Up      = CrossProduct(RightVector, ForwardVector)

							RightVector = CloseMarker - PivotMarker;
							ForwardVector = FarMarker - PivotMarker; 
							UpVector = FVector::CrossProduct(ForwardVector, RightVector);
						}
						else {
							//Forward = CloseMarker - FarMarker;
							//Right   = CloseMarker - PivotMarker;
							//Up      = CrossProduct(RightVector, UpVector)

							RightVector = PivotMarker - FarMarker;
							ForwardVector = CloseMarker - PivotMarker; 
							UpVector = FVector::CrossProduct(ForwardVector, RightVector);
						}			

						RightVector.Normalize();
						ForwardVector.Normalize();
						UpVector.Normalize();

						if (ASynOpticonState::IsShowingDebugLines())
						{
							DrawDebugLine(
								GetWorld(),
								this->GetComponentLocation(),
								this->GetComponentLocation() + RightVector * 100,
								FColor(0, 255, 0), //green
								false, 0.1, 0,
								1
							);
							DrawDebugLine(
								GetWorld(),
								this->GetComponentLocation(),
								this->GetComponentLocation() + UpVector * 100,
								FColor(0, 0, 255),
								false, 0.1, 0,
								1
							);
							DrawDebugLine(
								GetWorld(),
								this->GetComponentLocation(),
								this->GetComponentLocation() + ForwardVector * 100,
								FColor(255, 0, 0),
								false, 0.1, 0,
								1
							);
						}

						FMatrix axis;
						axis.SetAxes(&ForwardVector, &RightVector, &UpVector, &Location);

						this->SetWorldLocationAndRotation(Location, axis.Rotator());
					}
					else if (MarkersConfiguration == MarkersConfigurationEnum::VE_ManualTool)
					{
							FVector Location = (CloseMarker + FarMarker) / 2;
							FVector RightVector = FarMarker - CloseMarker;
							FVector ForwardVector = PivotMarker - CloseMarker;
							FVector UpVector = FVector::CrossProduct(ForwardVector, RightVector);

							FMatrix axis;
							axis.SetAxes(&ForwardVector, &RightVector, &UpVector, &Location);
							this->SetWorldLocationAndRotation(Location, axis.Rotator());

							//if (ASynOpticonState::IsShowingDebugLines())
							//{
							//	DrawDebugCircle(GetWorld(), PivotMarker, 2, 24, FColor::Yellow);
							//	DrawDebugCircle(GetWorld(), CloseMarker, 2, 24, FColor::Green);
							//	DrawDebugCircle(GetWorld(), FarMarker, 2, 24, FColor::Red);

							//	DrawDebugLine(
							//		GetWorld(),
							//		PivotMarker,
							//		PivotMarker + RightVector * 100,
							//		FColor(0, 255, 0), //green
							//		false, 0.1, 0,
							//		1
							//	);
							//	DrawDebugLine(
							//		GetWorld(),
							//		PivotMarker,
							//		PivotMarker + UpVector * 100,
							//		FColor(0, 0, 255),
							//		false, 0.1, 0,
							//		1
							//	);
							//	DrawDebugLine(
							//		GetWorld(),
							//		PivotMarker,
							//		PivotMarker + ForwardVector * 100,
							//		FColor(255, 0, 0),
							//		false, 0.1, 0,
							//		1
							//	);
							//}
					}
					else if (MarkersConfiguration == MarkersConfigurationEnum::VE_PositionCap) //TODO this one is not correct
					{
							FVector UpVector = FarMarker - CloseMarker;
							FVector RightVector = PivotMarker - CloseMarker;
							FVector ForwardVector = FVector::CrossProduct(RightVector, UpVector);

							//WARNING: SHOULD NOT USE MAKEFROM...
							FMatrix axis;
							axis.SetAxes(&ForwardVector, &RightVector, &UpVector, &PivotMarker);

							FVector Location = (PivotMarker + CloseMarker) / 2;

							this->SetWorldLocationAndRotation(Location, axis.Rotator());
					}

					if (ShouldApplyOffset && ASynOpticonState::ShouldApplyOffset())
					{
						AddLocalOffset(ActorLocationOffset);
					}
				}
			}
		}

		if (Calibrated && NatNetDataSample != nullptr)
		{
			AddLocalRotation(CalibrationRotator);
		}
	
		if (ASynOpticonState::IsShowingDebugLines())
		{
			DrawDebugLine(
				GetWorld(),
				this->GetComponentLocation(),
				this->GetComponentLocation() + this->GetRightVector() * 50,
				FColor(0, 255, 0), //green
				false, 0.1, 0,
				5
				);
			DrawDebugLine(
				GetWorld(),
				this->GetComponentLocation(),
				this->GetComponentLocation() + this->GetUpVector() * 50,
				FColor(0, 0, 255),
				false, 0.1, 0,
				5
				);
			DrawDebugLine(
				GetWorld(),
				this->GetComponentLocation(),
				this->GetComponentLocation() + this->GetForwardVector() * 50,
				FColor(255, 0, 0),
				false, 0.1, 0,
				5
				);
		}
			
		if (ShouldResetToolCalibrationAndCalibrate) {
			CalDelay++;
			if (CalDelay > 3) {
				CalibratePosOriToCalibrationTool(StoredCalibrationToolTransform, StoredCalibrationMethod, StoredCalibrationAxis);
				ShouldResetToolCalibrationAndCalibrate = false;
				CalDelay = 0;
			}			
		}

		delete NatNetDataSample;
	}
}

void UPositionAndOrientationComponent::SetOwnerRigidBodyName(FString _OwnerRigidBodyName)
{
	OwnerRigidBodyName = _OwnerRigidBodyName;
}

FString UPositionAndOrientationComponent::GetRigidBodyName()
{
	return OwnerRigidBodyName;
}

void UPositionAndOrientationComponent::UpdateVectorVisibility()
{
	//if (ShouldShowOrientationArrowMesh)
	//{
	if (!ShouldShowOrientationArrowMesh) {
		ActorOrientationArrowMesh->SetVisibility(false, false);
	}
	else{
		ActorOrientationArrowMesh->SetVisibility(ASynOpticonState::IsVectorVisible(VectorVisibilityEnum::VE_OrientationVector), false);
	}
}

// Called when the game ends
void UPositionAndOrientationComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	Markers.Empty();
	Calibrated = false;
	CalibrationRotator = FRotator(0, 0, 0);

	ActorOrientationArrowMesh->DestroyComponent();
}

//Calibrate the eye vectors using the selected calibration points
void UPositionAndOrientationComponent::CalibratePositionVector(FVector CalPoint, int CalNum) //Perform calibration when CalNum == 1
{
	//AActor* Owner = GetOwner();
	FQuat OriginalRotation = GetComponentTransform().GetRotation();
	if(Calibrated)
	{		
		OriginalRotation = OriginalRotation * CalibrationRotator.Quaternion().Inverse();
		SetRelativeRotation(OriginalRotation);
		//Owner->SetActorRelativeRotation(OriginalRotation);
	}

	//=================Convert from world coordinate to actors local coordinates================
	FVector CalibrationPoint = GetComponentTransform().InverseTransformPosition(CalPoint);
	FVector CurrentDirectionVector = GetComponentTransform().InverseTransformVector(OriginalRotation.Vector());
	CurrentDirectionVector.Normalize();
	FVector CorrectDirectionVector = (CalibrationPoint - GetComponentTransform().InverseTransformPosition(GetComponentLocation()));
	CorrectDirectionVector.Normalize();
	FQuat RelativeRotationLocal = FQuat::FindBetween(CurrentDirectionVector, CorrectDirectionVector);

	FVector Angles = RelativeRotationLocal.Euler();
	TArray<double> Dat;
	Dat.Add(Angles.Y); Dat.Add(Angles.Z); 
	CalibrationAngles.Add(Dat);

	if (CalNum == 1)
	{
		if (CalibrationAngles.Num() == 1) {
			CalibrationRotator = RelativeRotationLocal.Rotator();
		}
		else {
			TArray<double> Result = USynOpticonStatics::estimator(CalibrationAngles);
			FRotator ResultRotator = FRotator(Result[0], Result[1], 0);
			CalibrationRotator = ResultRotator;						
		}

		CalibrationRotator.Normalize();
		CalibrationAngles.Empty();
		if (!Calibrated)
			Calibrated = true;
	}
}

void UPositionAndOrientationComponent::ResetCalibration()
{
	//MarkersID.Empty();
	CalibrationAngles.Empty();
	CalibrationRotator = FRotator(0, 0, 0);
	Calibrated = false;

	HasBeenCalibratedByCalibrationTool = false;
	CalibrationToolOffset = FVector(0,0,0);
	CalibrationToolRotation = FQuat(0,0,0,1);
}

void  UPositionAndOrientationComponent::ResetMarkerIDs()
{
	Markers.Empty();
}

void UPositionAndOrientationComponent::SetActorLocationOffset(FVector Offset)
{
	ActorLocationOffset = Offset;
}

void UPositionAndOrientationComponent::SetShouldApplyOffset(bool ApplyOffset)
{
	ShouldApplyOffset = ApplyOffset;
}

void UPositionAndOrientationComponent::HideOrientationArrowMesh()
{
	ShouldShowOrientationArrowMesh = false;
}

void UPositionAndOrientationComponent::UpdateReplayData()
{
	if (SOBlock && !GetName().Contains("hand")) //TODO Hotfix, find better solution later
	{
		SetWorldLocation(SOBlock->ActorLocation);
		SetWorldRotation(SOBlock->ActorRotation);
	}
}

void UPositionAndOrientationComponent::ToBinaryReplayData()
{
	SOBlock->ActorLocation = GetComponentLocation();
	SOBlock->ActorRotation = GetComponentRotation();
}

void UPositionAndOrientationComponent::CalibratePosOriToCalibrationTool(FTransform CalibrationToolTransform, int32 CalibrationMethod, int32 CalibrationAxis) {

	if (HasBeenCalibratedByCalibrationTool) {
		StoredCalibrationToolTransform = CalibrationToolTransform;	
		StoredCalibrationMethod = CalibrationMethod;
		StoredCalibrationAxis = CalibrationAxis;
		HasBeenCalibratedByCalibrationTool = false;
		ShouldResetToolCalibrationAndCalibrate = true;
		return;
	}

	BeforeCalibrationToolTransform = this->GetComponentTransform();

	//Calibrate location
	if (CalibrationMethod == 0 || CalibrationMethod == 1) {
		CalibrationToolOffset = CalibrationToolTransform.GetLocation() - BeforeCalibrationToolTransform.GetLocation();
	}
	else {
		CalibrationToolOffset = FVector(0, 0, 0);
	}

	//Calibrate orientation
	if (CalibrationMethod != 1) {
		CalibrationToolRotation = CalibrationToolTransform.GetRotation();
	}
	else {
		CalibrationToolRotation = FQuat::Identity;
	}

	HasBeenCalibratedByCalibrationTool = true;
}

void UPositionAndOrientationComponent::SetCalibrationToolCalibration(FQuat Calibration) {	

	if (!(Calibration.X == 0 && Calibration.Y == 0 && Calibration.Z == 0 && Calibration.W == 1)) {
		BeforeCalibrationToolTransform = this->GetComponentTransform();

		CalibrationToolRotation = Calibration;
		HasBeenCalibratedByCalibrationTool = true;

		//TODO might have to reset an existing calibration :O Unless it is only used while loading
	}
}

FQuat UPositionAndOrientationComponent::GetCalibrationToolCalibration() {
	return CalibrationToolRotation;
}