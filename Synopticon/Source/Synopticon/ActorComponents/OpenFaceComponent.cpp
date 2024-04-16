// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenFaceComponent.h"

// Sets default values for this component's properties
UOpenFaceComponent::UOpenFaceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	//Load the bust object and it's materials
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BustMeshObj(TEXT("StaticMesh'/Game/Geometry/Meshes/HeadMesh/MaleBustBSCombined.MaleBustBSCombined'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> ClayMat(TEXT("Material'/Game/Geometry/Meshes/HeadMesh/Clay.Clay'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> IrisMat(TEXT("Material'/Game/Geometry/Meshes/HeadMesh/Iris.Iris'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> EyeMat(TEXT("Material'/Game/Geometry/Meshes/HeadMesh/Material_001.Material_001'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> PupilMat(TEXT("Material'/Game/Geometry/Meshes/HeadMesh/pupil.pupil'"));

	//Create the bust
	BustMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OpenFaceBustMesh"));
	BustMesh->SetStaticMesh(BustMeshObj.Object);
	BustMesh->SetMaterial(3, ClayMat.Object);
	BustMesh->SetMaterial(0, IrisMat.Object);
	BustMesh->SetMaterial(1, EyeMat.Object);
	BustMesh->SetMaterial(2, PupilMat.Object);

	//Attach to the root scene component
	BustMesh->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

	//Prepare the open face scene nodes
	LeftEyeSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("OpenFaceLeftEyeSceneComp"));
	RightEyeSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("OpenFaceRightEyeSceneComp"));
	OrientationSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("OpenFaceOrientationSceneComp"));
	CombinedVectorSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("OpenFaceCombinedVectorSceneComp"));

	//create eye vectors
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ArrowMeshObj(TEXT("StaticMesh'/Game/Models/arrow/arrow.arrow'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> BlueMaterial(TEXT("Material'/Game/Materials/SynOpticonActorMaterials/eyeVectorMaterial.eyeVectorMaterial'"));

	LeftEyeArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OpenFaceLeftArrowMesh"));
	LeftEyeArrowMesh->SetStaticMesh(ArrowMeshObj.Object);
	LeftEyeArrowMesh->SetMaterial(0, BlueMaterial.Object);
	LeftEyeArrowMesh->SetWorldScale3D(FVector(5, 1, 1));
	LeftEyeArrowMesh->AttachToComponent(LeftEyeSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

	RightEyeArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OpenFaceRightArrowMesh"));
	RightEyeArrowMesh->SetStaticMesh(ArrowMeshObj.Object);
	RightEyeArrowMesh->SetMaterial(0, BlueMaterial.Object);
	RightEyeArrowMesh->SetWorldScale3D(FVector(5, 1, 1));
	RightEyeArrowMesh->AttachToComponent(RightEyeSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//create the orientation vector
	static ConstructorHelpers::FObjectFinder<UMaterial>YellowMaterial(TEXT("Material'/Game/Materials/SynOpticonActorMaterials/OrientationVectorMaterial.OrientationVectorMaterial'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> VectorMaterial(TEXT("Material'/Game/Materials/SynOpticonActorMaterials/VectorMaterial.VectorMaterial'"));

	OrientationArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OpenFaceOrientationArrowMesh"));
	OrientationArrowMesh->SetStaticMesh(ArrowMeshObj.Object);
	OrientationArrowMesh->SetMaterial(0, UMaterialInstanceDynamic::Create(VectorMaterial.Object, NULL));
	OrientationArrowMesh->SetWorldScale3D(FVector(5, 1, 1));
	OrientationArrowMesh->AttachToComponent(OrientationSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//create the combined vector
	static ConstructorHelpers::FObjectFinder<UMaterial> GreenMaterial(TEXT("Material'/Game/Materials/SynOpticonActorMaterials/ConvergenceVectorMaterial.ConvergenceVectorMaterial'"));
	CombinedVectorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OpenFaceCombinedArrowMesh"));
	CombinedVectorMesh->SetStaticMesh(ArrowMeshObj.Object);
	CombinedVectorMesh->SetWorldScale3D(FVector(5, 1, 1));
	CombinedVectorMesh->SetMaterial(0, UMaterialInstanceDynamic::Create(VectorMaterial.Object, NULL));

	CombinedVectorMesh->AttachToComponent(CombinedVectorSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
}


// Called when the game starts
void UOpenFaceComponent::BeginPlay()
{
	Super::BeginPlay();

	BustMesh->RegisterComponent();
	LeftEyeArrowMesh->RegisterComponent();
	RightEyeArrowMesh->RegisterComponent();
	CombinedVectorMesh->RegisterComponent();
	OrientationArrowMesh->RegisterComponent();

	//Attach the eye vector scene components to the root scene component
	LeftEyeSceneComponent->AttachToComponent(BustMesh, FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("LeftEyeVectorSocket")));
	RightEyeSceneComponent->AttachToComponent(BustMesh, FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("RightEyeVectorSocket")));
	CombinedVectorSceneComponent->AttachToComponent(BustMesh, FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("OrientationVectorSocket")));
	OrientationSceneComponent->AttachToComponent(BustMesh, FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("OrientationVectorSocket")));

	ASynOpticonState::GetGlobalEventSystem()->OnEventUpdateCurrentSOActor.AddDynamic(this, &UOpenFaceComponent::OnActorChanged);
	//ASynOpticonState::GetGlobalEventSystem()->OnEventWAMPComponentDisconnected.AddDynamic(this, &UEyeVectorsComponent::OnEyeWAMPComponentDisconnected);
	HitTargetID = -1;

	OwnerIsSelected = false;

	//Pupil data
	//PreviousPupilSize = -10;
	//StartBlinking = false;
	//StartReversing = false;
	//IsEyeBlink = false;
	//BlinkClock = 0;

	ScaleDelay = 0.2f;
	ScaleDelayCounter = 0;
	StartScaleDelay = false;

	if (!OpenFaceStruct.IsTrackingEyes) {
		LeftEyeSceneComponent->SetVisibility(false, true);
		RightEyeSceneComponent->SetVisibility(false, true);
		CombinedVectorSceneComponent->SetVisibility(false, true);
	}
	if (!OpenFaceStruct.IsTrackingHead) {
		OrientationArrowMesh->SetVisibility(false, true);
	}

	OpenUDPSocket();

	if (!ASynOpticonState::IsReplaying())
	{
		AttachedScreen = nullptr;

		for (TObjectIterator<AGazeActor> Itr; Itr; ++Itr)
		{
			// Filter out objects not contained in the target world.
			if (Itr->GetWorld() != GetOwner()->GetWorld())
			{
				continue;
			}

			// Find the GazeActor representing the attached screen by comparing their names
			if (Itr->GetName().Equals(OpenFaceStruct.AttachedScreenName))
			{
				AttachedScreen = *Itr;
			}
		}

		if (AttachedScreen)
		{
			OpenFaceTransformation = AttachedScreen->GetOpenFaceTransformation();
		}
		else
		{
			OpenFaceTransformation = FMatrix::Identity;
		}
	}
	else
	{
		OpenFaceTransformation = FMatrix::Identity;
	}
}

void UOpenFaceComponent::OpenUDPSocket() {
	FIPv4Address ip;
	bool did_parse = FIPv4Address::Parse(*OpenFaceStruct.IpAddress, ip);
	FIPv4Endpoint Endpoint(ip, OpenFaceStruct.Port); // Replace 12345 with your desired port number
	//FIPv4Endpoint Endpoint(FIPv4Address::Any, 4242); // Replace 12345 with your desired port number
	UDPSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_DGram, TEXT("OpenTrackSocket"), false);
	bool Result = UDPSocket->Bind(*Endpoint.ToInternetAddr());
	if (Result) {
		UE_LOG(LogTemp, Warning, TEXT("Open Track Socket bound"));
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Open Track Socket failed to bind"));
	}
}

void UOpenFaceComponent::ReceiveData(float DeltaTime)
{
	FRigidBodyDataStruct OrientationSample;
	FWAMPEyeDataStruct GazeSample;
	bool shouldUpdate = false;
	if (!ASynOpticonState::IsReplaying()) //real-time
	{
		if (UDPSocket) {
			uint32 Size;
			while (UDPSocket->HasPendingData(Size))
			{
				int32 OpenTrackDataSize = 48;
				TArray<uint8> ReceivedData;
				ReceivedData.SetNumUninitialized(OpenTrackDataSize);
				int32 Read = 0;
				UDPSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read);

				// Process the received data as needed
				if (Read == OpenTrackDataSize) {
					FOpenTrackData OpenTrackData;

					TArray<double> FloatArray;
					FMemoryReader MemoryReader(ReceivedData, true);
					while (MemoryReader.AtEnd() == false)
					{
						double Value;
						MemoryReader << Value;
						FloatArray.Add(Value);
					}

					OpenTrackData.X = FloatArray[0];
					OpenTrackData.Y = FloatArray[1];
					OpenTrackData.Z = FloatArray[2];
					OpenTrackData.Roll = FloatArray[5];
					OpenTrackData.Pitch = FloatArray[4];
					OpenTrackData.Yaw = FloatArray[3]+180;

					OrientationSample.Orientation = FQuat::MakeFromEuler(FVector(OpenTrackData.Roll, OpenTrackData.Pitch, OpenTrackData.Yaw));
					OrientationSample.Position = FVector(OpenTrackData.Z, OpenTrackData.X, OpenTrackData.Y);
					//UE_LOG(LogTemp, Warning, TEXT("Read from UDP socket: X %2.f Y %2.f Z %2.f Roll %2.f Pitch %2.f Yaw%2.f"),
					//	OpenTrackData.X, OpenTrackData.Y, OpenTrackData.Z, OpenTrackData.Roll, OpenTrackData.Pitch, OpenTrackData.Yaw);

					shouldUpdate = true;
				}
			}
		}

		//FWAMPOpenFaceDataStruct* OpenFaceDataSample = OpenFaceWAMPComponent->GetOpenFaceDataSample(OpenFaceStruct.OpenFaceID);
		/*FWAMPOpenFaceDataStruct* OpenFaceDataSample = OpenFaceWAMPComponent->GetOpenFaceDataSample("OpenFaceStruct.OpenFaceID");
		if (OpenFaceDataSample) {
			shouldUpdate = true;
			OrientationSample = OpenFaceDataSample->Orientation;
			GazeSample = OpenFaceDataSample->GazeData;
			delete OpenFaceDataSample;
		}*/
	}
	else if (ASynOpticonState::IsReplaying() && !ASynOpticonState::IsPausingReplay())
	{
		if (SOBlock)
		{
			shouldUpdate = true;
			OrientationSample.Position = SOBlock->ActorLocation;
			OrientationSample.Orientation = SOBlock->ActorRotation.Quaternion();

			GazeSample.LeftEyeLocation = SOBlock->ETGData.LeftEyeLocation;
			GazeSample.LeftEyeDirection = SOBlock->ETGData.LeftEyeRotation.Vector();
			GazeSample.RightEyeLocation = SOBlock->ETGData.RightEyeLocation;
			GazeSample.RightEyeDirection = SOBlock->ETGData.RightEyeRotation.Vector();
			GazeSample.LeftPupilRadius = GazeSample.RightPupilRadius = SOBlock->ETGData.PupilRadius;
			GazeSample.LeftPupilConfidence = GazeSample.RightPupilConfidence = 1;
		}
	}

	if (shouldUpdate)
	{
		if (OpenFaceStruct.IsTrackingEyes) {
			UpdateGazeData(GazeSample);
		}
		if (OpenFaceStruct.IsTrackingHead) {
			UpdatePositionData(DeltaTime, OrientationSample);
		}
	}

	//raycast
	DoRaycast();
}

// Called every frame
void UOpenFaceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateVectorsVisibility();
	//UpdateOpenFaceData(DeltaTime);
	ReceiveData(DeltaTime);
}

void UOpenFaceComponent::UpdateVectorsVisibility()
{
	if (OpenFaceStruct.IsTrackingEyes)
	{
		LeftEyeSceneComponent->SetVisibility(ASynOpticonState::IsVectorVisible(VectorVisibilityEnum::VE_EyeVector), true);
		RightEyeSceneComponent->SetVisibility(ASynOpticonState::IsVectorVisible(VectorVisibilityEnum::VE_EyeVector), true);
		CombinedVectorSceneComponent->SetVisibility(ASynOpticonState::IsVectorVisible(VectorVisibilityEnum::VE_ConvergenceVector), true);
	}
	if (OpenFaceStruct.IsTrackingHead)
	{
		OrientationSceneComponent->SetVisibility(ASynOpticonState::IsVectorVisible(VectorVisibilityEnum::VE_OrientationVector), true);
	}
}

void UOpenFaceComponent::UpdateOpenFaceData(float DeltaTime)
{
	FRigidBodyDataStruct OrientationSample;
	FWAMPEyeDataStruct GazeSample;
	bool shouldUpdate = false;
	if (!ASynOpticonState::IsReplaying()) //real-time
	{
		//FWAMPOpenFaceDataStruct* OpenFaceDataSample = OpenFaceWAMPComponent->GetOpenFaceDataSample(OpenFaceStruct.OpenFaceID);
		FWAMPOpenFaceDataStruct* OpenFaceDataSample = OpenFaceWAMPComponent->GetOpenFaceDataSample("OpenFaceStruct.OpenFaceID");
		if (OpenFaceDataSample) {
			shouldUpdate = true;
			OrientationSample = OpenFaceDataSample->Orientation;
			GazeSample = OpenFaceDataSample->GazeData;
			delete OpenFaceDataSample;
		}
	}
	else if (ASynOpticonState::IsReplaying() && !ASynOpticonState::IsPausingReplay())
	{
		if (SOBlock)
		{
			shouldUpdate = true;
			OrientationSample.Position = SOBlock->ActorLocation;
			OrientationSample.Orientation = SOBlock->ActorRotation.Quaternion();
			
			GazeSample.LeftEyeLocation = SOBlock->ETGData.LeftEyeLocation;
			GazeSample.LeftEyeDirection = SOBlock->ETGData.LeftEyeRotation.Vector();
			GazeSample.RightEyeLocation = SOBlock->ETGData.RightEyeLocation;
			GazeSample.RightEyeDirection = SOBlock->ETGData.RightEyeRotation.Vector();
			GazeSample.LeftPupilRadius = GazeSample.RightPupilRadius = SOBlock->ETGData.PupilRadius;
			GazeSample.LeftPupilConfidence = GazeSample.RightPupilConfidence = 1;
		}
	}

	if (shouldUpdate) 
	{
		if (OpenFaceStruct.IsTrackingEyes) {
			UpdateGazeData(GazeSample);
		}
		if (OpenFaceStruct.IsTrackingHead) {
			UpdatePositionData(DeltaTime, OrientationSample);
		}
	}

	//raycast
	DoRaycast();
}
void UOpenFaceComponent::UpdatePositionData(float DeltaTime, FRigidBodyDataStruct Orientation)
{
	//Transform the direction vectors so they are relative to the camera instead of the centre of the world
	FVector aAxis = OpenFaceTransformation.TransformVector(Orientation.Orientation.GetForwardVector());
	FVector bAxis = OpenFaceTransformation.TransformVector(Orientation.Orientation.GetRightVector());
	FVector cAxis = OpenFaceTransformation.TransformVector(Orientation.Orientation.GetUpVector());

	FRotator WorldRotation = UKismetMathLibrary::MakeRotationFromAxes(aAxis, bAxis, cAxis);
	FVector WorldLocation = OpenFaceTransformation.TransformPosition(Orientation.Position); //Transform the new location to make it relative to the "Camera" location instead of the centre of the world

	this->SetWorldLocation(WorldLocation);
	this->SetWorldRotation(WorldRotation);
	//this->SetWorldLocation(Orientation.Position);
	//this->SetWorldRotation(Orientation.Orientation);

	if (Calibrated)
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

		DrawDebugLine(
			GetWorld(),
			FVector(0,0,0),
			Orientation.Position + Orientation.Position * 50,
			FColor(255, 90, 0), //green
			false, 0.1, 0,
			5
		);
	}
}
void UOpenFaceComponent::UpdateGazeData(FWAMPEyeDataStruct EyeDataSample)
{
	bool LeftEyeValid = false;
	bool RightEyeValid = false;
	//We set the invalid samples to be streamed with a pupil confidence of -1
	if (EyeDataSample.LeftPupilConfidence > 0) {
		LeftEyeValid = UpdateEyeVector(EyeDataSample, LeftEyeSceneComponent, true);
	}

	if (EyeDataSample.RightPupilConfidence > 0) {
		RightEyeValid = UpdateEyeVector(EyeDataSample, RightEyeSceneComponent, false);
	}

	//UpdatePupilDataWAMP(EyeDataSample, DeltaTime);

	//TODO use left or right eye if one eye is open...
	if (LeftEyeValid && RightEyeValid) {
		FindConvergenceVector();
	}
	else {
		CombinedVectorSceneComponent->SetVisibility(false, true);
	}
}
bool UOpenFaceComponent::UpdateEyeVector(FWAMPEyeDataStruct EyeDataSample, USceneComponent* EyeVectorNode, bool Left) {
	//Get the eye position from the sample data
	FVector EyeLocation;
	if (Left) {
		EyeLocation = EyeDataSample.LeftEyeLocation;
	}
	else {
		EyeLocation = EyeDataSample.RightEyeLocation;
	}

	//Update the position of the eye
	EyeVectorNode->SetRelativeLocation(EyeLocation);

	//Get the direction vector of the eye from the sample data
	FVector SampleDirectionVector;
	if (Left) {
		SampleDirectionVector = OpenFaceTransformation.TransformVector(EyeDataSample.LeftEyeDirection);
	}
	else {
		SampleDirectionVector = OpenFaceTransformation.TransformVector(EyeDataSample.RightEyeDirection);
	}

	//Get the orientation of the eye data
	//Transform the direction vectors so they are relative to the camera instead of the centre of the world

	FQuat NewRotation = SampleDirectionVector.ToOrientationQuat();
	FRotator TestRotator = SampleDirectionVector.ToOrientationRotator();
	
	//check valid data. 
	if ((Left && (EyeDataSample.LeftPupilConfidence == -1)) ||
		(!Left && (EyeDataSample.RightPupilConfidence == -1)))//invalid left eye data
	{
		//the data is invalid but we set it anyway to make sure that we will save invalid data for replay
		//but we return 'false' and make it invisible
		NewRotation.X = NAN;

		EyeVectorNode->SetWorldRotation(NewRotation);
		//make sure we dont smooth the next valid sample
		if (Left)
		{
			PreviousLeftEyeRotator.X = NAN;
		}
		else
		{
			PreviousRightEyeRotator.X = NAN;
		}
		return false;
	}
	//only smooth if previous data is valid
	if ((Left && !PreviousLeftEyeRotator.ContainsNaN()) || (!Left && !PreviousRightEyeRotator.ContainsNaN()))
	{
		FQuat SmoothedRotation;
		if (Left)
		{
			SmoothedRotation = FQuat::FastLerp(NewRotation, PreviousLeftEyeRotator, ASynOpticonState::GetSmoothingMomentum());
			PreviousLeftEyeRotator = SmoothedRotation;
		}
		else
		{
			SmoothedRotation = FQuat::FastLerp(NewRotation, PreviousRightEyeRotator, ASynOpticonState::GetSmoothingMomentum());
			PreviousRightEyeRotator = SmoothedRotation;
		}

		SmoothedRotation.Normalize();
		EyeVectorNode->SetWorldRotation(SmoothedRotation);
	}
	else
	{
		NewRotation.Normalize();
		EyeVectorNode->SetWorldRotation(NewRotation);
	}

	if (Calibrated) {
		if (Left) {
			EyeVectorNode->AddRelativeRotation(CalibrationRotatorLeft);
		}
		else {
			EyeVectorNode->AddRelativeRotation(CalibrationRotatorRight);
		}

		HasCalibration = true;
	}

	return true;
}
void UOpenFaceComponent::FindConvergenceVector() {
	FTransform LeftEyeTransform = LeftEyeSceneComponent->GetComponentTransform();
	FTransform RightEyeTransform = RightEyeSceneComponent->GetComponentTransform();
	CombinedVectorSceneComponent->SetWorldLocation((LeftEyeTransform.GetLocation() + RightEyeTransform.GetLocation()) / 2);

	TPair<FVector, float> ConvergencePoint = USynOpticonStatics::ConvPointAndDist(LeftEyeTransform.GetRotation().GetForwardVector(), RightEyeTransform.GetRotation().GetForwardVector(), LeftEyeTransform.GetLocation(), RightEyeTransform.GetLocation());

	if (ConvergencePoint.Value != -1) {
		FVector CombinedVectorDirection = ConvergencePoint.Key - CombinedVectorSceneComponent->GetComponentLocation();
		FVector ConvergencedPoint = ConvergencePoint.Key;

		FVector PosOriForward = GetForwardVector();//OuterPosOriComp->GetForwardVector(); //GetOwner()->GetActorRotation().Vector()

		float Dotproduct = FVector::DotProduct(CombinedVectorDirection, PosOriForward);

		if (Dotproduct < 0) {
			CombinedVectorDirection = -CombinedVectorDirection;
		}

		//Smooth the convergence vector by adding a factor of the new data to the old data
		FQuat OriginalRotation = CombinedVectorSceneComponent->GetComponentTransform().GetRotation().GetForwardVector().ToOrientationQuat();
		FQuat NewRotation = CombinedVectorDirection.ToOrientationQuat();
		FQuat SmoothedRotation = FQuat::FastLerp(NewRotation, OriginalRotation, ASynOpticonState::GetSmoothingMomentum());
		SmoothedRotation.Normalize();
		CombinedVectorSceneComponent->SetWorldRotation(SmoothedRotation);
	}
}
void UOpenFaceComponent::DoRaycast()
{
	URaycastComponent_Ours* RaycastComponent = GetOwner()->FindComponentByClass<URaycastComponent_Ours>();
	if (RaycastComponent)
	{
		/*if (OpenFaceStruct.IsTrackingEyes)
		{
			RaycastComponent->SetVectorSceneComponent(CombinedVectorSceneComponent);
		}
		else if (OpenFaceStruct.IsTrackingHead)
		{
			RaycastComponent->SetVectorSceneComponent(OrientationSceneComponent);
		}
		else {
			return;
		}*/
		AGazeActor* Target = RaycastComponent->GetInteractingActor();

		double distance = RaycastComponent->GetTargetDistance();
		if (distance != -1 && distance != 0)
		{
			LeftEyeSceneComponent->SetRelativeScale3D(FVector(distance / 500, 1, 1));
			RightEyeSceneComponent->SetRelativeScale3D(FVector(distance / 500, 1, 1));
			CombinedVectorSceneComponent->SetRelativeScale3D(FVector(distance / 500, 1, 1));
			ScaleDelay = 0;
			StartScaleDelay = false;
		}
		else
		{
			if (!StartScaleDelay)
			{
				StartScaleDelay = true;
			}

			//if (StartScaleDelay && ScaleDelayCounter > ScaleDelay)
			//{
			//	LeftEyeSceneComponent->SetRelativeScale3D(FVector(1, 1, 1));
			//	RightEyeSceneComponent->SetRelativeScale3D(FVector(1, 1, 1));
			//	CombinedVectorSceneComponent->SetRelativeScale3D(FVector(1, 1, 1));
			//	ScaleViewConeUsingAngle(600, MACULAR_ANGLE);

			//	ScaleDelay = 0;
			//	StartScaleDelay = false;
			//}
		}
	}
}

float UOpenFaceComponent::GetAveragedPupilRadius(float LeftEye, float RightEye) {
	float AveragedPupilRadius = -1;
	if (LeftEye > 0 && RightEye > 0) {
		AveragedPupilRadius = (LeftEye + RightEye) / 2;
	}
	else if (LeftEye > 0) {
		AveragedPupilRadius = LeftEye;
	}
	else if (RightEye) {
		AveragedPupilRadius = RightEye;
	}
	return AveragedPupilRadius;
}

void UOpenFaceComponent::SetRetWAMPComponent(UOpenFaceWAMPComponent* _OpenFaceWAMPComponent) {
	OpenFaceWAMPComponent = _OpenFaceWAMPComponent;
}

UOpenFaceWAMPComponent* UOpenFaceComponent::GetOpenFaceWAMPComponent() {
	return OpenFaceWAMPComponent;
}

void UOpenFaceComponent::OnActorChanged(int ActorID, bool HasETG, bool HasSHimmerHR, bool HasShimmerGSR, FString ETEDevice, FString ShimmerDevice)
{
	OwnerIsSelected = OwnerID == ActorID;
}

void UOpenFaceComponent::SetOpenFaceStruct(FOpenFaceStruct _OpenFaceStruct)
{
	OpenFaceStruct = _OpenFaceStruct;
}

USceneComponent* UOpenFaceComponent::GetRaycastSceneComponent() 
{
	if (OpenFaceStruct.IsTrackingEyes)
	{
		return CombinedVectorSceneComponent;
	}
	else if (OpenFaceStruct.IsTrackingHead)
	{
		return OrientationSceneComponent;
	}
	else {
		return nullptr;
	}
}

void UOpenFaceComponent::SetSOBlock(CompressedSOBlock* _SOBlock)
{
	this->SOBlock = _SOBlock;
}

void UOpenFaceComponent::ToBinaryReplayData() {
	SOBlock->HasETGData = true;
	SOBlock->ETGData.LeftEyeLocation = LeftEyeSceneComponent->GetRelativeTransform().GetLocation();
	SOBlock->ETGData.LeftEyeRotation = LeftEyeSceneComponent->GetComponentRotation();
	SOBlock->ETGData.RightEyeLocation = RightEyeSceneComponent->GetRelativeTransform().GetLocation();
	SOBlock->ETGData.RightEyeRotation = RightEyeSceneComponent->GetComponentRotation();
	SOBlock->ETGData.PupilRadius = LeftEyePupilRadius;
	SOBlock->ImageDataBufferSize = 0;

	SOBlock->ActorLocation = GetComponentLocation();
	SOBlock->ActorRotation = GetComponentRotation();

	//if (EyeTrackingWAMPComponent && !GlassesID.Equals(""))
	//{
	//	EyeTrackingWAMPComponent->SaveEncodedSceneCameraFrame(GlassesID, SOBlock);
	//}
}

void UOpenFaceComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	//Clean up the UDP socket
	if (UDPSocket)
	{
		UDPSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(UDPSocket);
		UDPSocket = nullptr;
	}
}