#include "RemoteEyeTrackerComponent.h"

// Sets default values for this component's properties
URemoteEyeTrackerComponent::URemoteEyeTrackerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	//Prepare the remote tracker scene nodes
	RemoteTrackerRootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RETRootSceneComp"));
	LeftEyeSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RemoteTrackerLeftEyeSceneComp"));
	RightEyeSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RemoteTrackerRightEyeSceneComp"));
	
	//Create the remote tracker
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RemoteTrackerMeshObj(TEXT("StaticMesh'/Game/Models/remote_tracker/remote_tracker.remote_tracker'"));
	RemoteTrackerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RemoteTrackerMesh"));
	RemoteTrackerMesh->SetStaticMesh(RemoteTrackerMeshObj.Object);
	RemoteTrackerMesh->AttachToComponent(RemoteTrackerRootSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//Create the eye balls
	static ConstructorHelpers::FObjectFinder<UStaticMesh> EyeMeshObj(TEXT("StaticMesh'/Game/Models/remote_tracker/eye_ball.eye_ball'"));
	LeftEyeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RemoteTrackerLeftEyeMesh"));
	LeftEyeMesh->SetStaticMesh(EyeMeshObj.Object);
	LeftEyeMesh->AttachToComponent(LeftEyeSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

	RightEyeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RemoteTrackerRightEyeMesh"));
	RightEyeMesh->SetStaticMesh(EyeMeshObj.Object);
	RightEyeMesh->AttachToComponent(RightEyeSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//create eye vectors
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ArrowMeshObj(TEXT("StaticMesh'/Game/Models/arrow/arrow.arrow'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> BlueMaterial(TEXT("Material'/Game/Materials/SynOpticonActorMaterials/eyeVectorMaterial.eyeVectorMaterial'"));

	LeftEyeArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RemoteTrackerLeftArrowMesh"));
	LeftEyeArrowMesh->SetStaticMesh(ArrowMeshObj.Object);
	LeftEyeArrowMesh->SetMaterial(0, BlueMaterial.Object);
	LeftEyeArrowMesh->SetWorldScale3D(FVector(5, 1, 1));
	LeftEyeArrowMesh->AttachToComponent(LeftEyeSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

	RightEyeArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RemoteTrackerRightArrowMesh"));
	RightEyeArrowMesh->SetStaticMesh(ArrowMeshObj.Object);
	RightEyeArrowMesh->SetMaterial(0, BlueMaterial.Object);
	RightEyeArrowMesh->SetWorldScale3D(FVector(5, 1, 1));
	RightEyeArrowMesh->AttachToComponent(RightEyeSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//Attach the eye vector scene components to the root scene component
	LeftEyeSceneComponent->AttachToComponent(RemoteTrackerRootSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
	RightEyeSceneComponent->AttachToComponent(RemoteTrackerRootSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts
void URemoteEyeTrackerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	RemoteTrackerMesh->RegisterComponent();
	LeftEyeMesh->RegisterComponent();
	RightEyeMesh->RegisterComponent();
	RightEyeArrowMesh->RegisterComponent();
	LeftEyeArrowMesh->RegisterComponent();

	RemoteTrackerRootSceneComponent->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	ASynOpticonState::GetGlobalEventSystem()->OnEventUpdateCurrentSOActor.AddDynamic(this, &URemoteEyeTrackerComponent::OnActorChanged);
	//ASynOpticonState::GetGlobalEventSystem()->OnEventWAMPComponentDisconnected.AddDynamic(this, &UEyeVectorsComponent::OnEyeWAMPComponentDisconnected);
	HitTargetID = -1;

	OwnerIsSelected = false;

	//Pupil data
	PreviousPupilSize = -10;
	StartBlinking = false;
	StartReversing = false;
	IsEyeBlink = false;
	BlinkClock = 0;
}

void URemoteEyeTrackerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	RemoteTrackerMesh->DestroyComponent();
	LeftEyeMesh->DestroyComponent();
	RightEyeMesh->DestroyComponent();
	RightEyeArrowMesh->DestroyComponent();
	LeftEyeArrowMesh->DestroyComponent();
	RetWAMPComp = nullptr;
}

// Called every frame
void URemoteEyeTrackerComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	UpdateVectorsVisibility();
	UpdateEyeData(DeltaTime);
}

void URemoteEyeTrackerComponent::OnActorChanged(int ActorID, bool HasETG, bool HasSHimmerHR, bool HasShimmerGSR, FString ETEDevice, FString ShimmerDevice) {
	OwnerIsSelected = OwnerID == ActorID;
}

void URemoteEyeTrackerComponent::UpdateVectorsVisibility()
{
	LeftEyeArrowMesh->SetVisibility(ASynOpticonState::IsVectorVisible(VectorVisibilityEnum::VE_EyeVector), true);
	RightEyeArrowMesh->SetVisibility(ASynOpticonState::IsVectorVisible(VectorVisibilityEnum::VE_EyeVector), true);
}

void URemoteEyeTrackerComponent::UpdateEyeData(float DeltaTime)
{
	if (!ASynOpticonState::IsReplaying())
	{
		if (RetWAMPComp)
		{
			FWAMPRetDataStruct* RetData = RetWAMPComp->GetEyeDataSample(RemoteEyeTrackerStruct.RemoteTrackerID);
			if (RetData) {
				UpdateEyeLocations(RetData);
				UpdateGazeLocation(RetData);

				LeftEyePupilRadius = RetData->LeftPupilRadius;
				RightEyePupilRadius = RetData->RightPupilRadius;
				UpdatePupilData(GetAveragedPupilRadius(LeftEyePupilRadius, RightEyePupilRadius), DeltaTime);

				//Clean up the data
				delete RetData;
			}			
		}
	}
	else if (ASynOpticonState::IsReplaying() && !ASynOpticonState::IsPausingReplay())
	{
		if (SOBlock)
		{
			UpdateReplayEyeVectors();
			UpdateReplayGazeLocation();
			UpdatePupilData(GetAveragedPupilRadius(SOBlock->RTData.LeftEyePupilRadius, SOBlock->RTData.RightEyePupilRadius), DeltaTime);
		}
	}
}

float URemoteEyeTrackerComponent::GetAveragedPupilRadius(float LeftEye, float RightEye) {
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

void URemoteEyeTrackerComponent::SetRetWAMPComponent(URemoteTrackerWAMPComponent* _RetWAMPComponent) {
	RetWAMPComp = _RetWAMPComponent;
}

URemoteTrackerWAMPComponent* URemoteEyeTrackerComponent::GetRetWAMPComponent() {
	return RetWAMPComp;
}

void URemoteEyeTrackerComponent::UpdateEyeLocations(FWAMPRetDataStruct* RetData)
{
	//Check if the eye was tracked by summing the position coordinates. If the sum is bigger than zero the eye was tracked this frame.
	if (RetData->LeftEyeLocation.X + RetData->LeftEyeLocation.Y + RetData->LeftEyeLocation.Z > 0)
	{
		LeftEyeSceneComponent->GetChildComponent(0)->SetVisibility(true);

		//Get the eye position from the sample data
		FVector LeftEyeLocation = USynOpticonStatics::ConvertNegXYZtoXZYCoordinateSystem(RetData->LeftEyeLocation); 

		//Update the position of the eye
		FVector SmoothedLocation = UKismetMathLibrary::VLerp(LeftEyeLocation, PrevLeftEyeLocation, ASynOpticonState::GetSmoothingMomentum());
		LeftEyeSceneComponent->SetRelativeLocation(SmoothedLocation);
		PrevLeftEyeLocation = LeftEyeLocation;
	}
	else
	{
		LeftEyeSceneComponent->SetVisibility(false, true);
	}

	////Check if the eye was tracked by summing the position coordinates. If the sum is bigger than zero the eye was tracked this frame.
	if (RetData->RightEyeLocation.X + RetData->RightEyeLocation.Y + RetData->RightEyeLocation.Z > 0)
	{
		RightEyeSceneComponent->GetChildComponent(0)->SetVisibility(true);

		//Get the eye position from the sample data
		FVector RightEyeLocation = USynOpticonStatics::ConvertNegXYZtoXZYCoordinateSystem(RetData->RightEyeLocation); 

		//Update the position of the eye
		FVector SmoothedLocation = UKismetMathLibrary::VLerp(RightEyeLocation, PrevRightEyeLocation, ASynOpticonState::GetSmoothingMomentum());
		RightEyeSceneComponent->SetRelativeLocation(SmoothedLocation);
		PrevRightEyeLocation = RightEyeLocation;
	}
	else
	{
		RightEyeSceneComponent->SetVisibility(false, true);
	}
}

void URemoteEyeTrackerComponent::UpdateGazeLocation(FWAMPRetDataStruct* RetData)
{
	/* FWAMPRetDataStruct
	FVector LeftEyeLocation;
	FVector2D LeftEyeScreenCoordinate;
	float LeftPupilRadius;
	FVector RightEyeLocation;
	FVector2D RightEyeScreenCoordinate;
	float RightPupilRadius;
	*/

	//Reset the scale of the eye vectors
	LeftEyeSceneComponent->SetRelativeScale3D(FVector(1, 1, 1));
	RightEyeSceneComponent->SetRelativeScale3D(FVector(1, 1, 1));

	bool LeftEyeValid = false;
	bool RightEyeValid = false;

	FVector2D LeftEyeGazePoint = RetData->LeftEyeScreenCoordinate;
	FVector2D RightEyeGazePoint = RetData->RightEyeScreenCoordinate;
	if (LeftEyeGazePoint.X != 0 && LeftEyeGazePoint.Y != 0)
	{
		LeftEyeValid = true;
	}
	if (RightEyeGazePoint.X != 0 && RightEyeGazePoint.Y != 0)
	{
		RightEyeValid = true;
	}


	FVector2D EstimatedGazePoint;
	if (LeftEyeValid && RightEyeValid)
	{
		EstimatedGazePoint = (LeftEyeGazePoint + RightEyeGazePoint) / 2;
	}
	else if (LeftEyeValid)
	{
		EstimatedGazePoint = LeftEyeGazePoint;
	}
	else if (RightEyeValid)
	{
		EstimatedGazePoint = RightEyeGazePoint;
	}
	else
	{
		WorldPoint.Set(-42, -42, -42); //TODO figure out what this is
		return;
	}

	//Get the verticies and triangle indexes of the attached screen
	if (!AttachedScreen)
	{
		return;
	}
	TArray<FVector> ScreenVerticies = AttachedScreen->GetVerticies();
	TArray<int32> TriangleIndexes = AttachedScreen->GetTriangleIndexes();
	TArray<FVector2D> ScreenUVs = AttachedScreen->GetUVs();

	//Convert screen location to UV coordinate
	FVector2D ScreenUVCoordinate = RetData->NormalizedGazeLocation;

	bool bIsInsideScreen = false;

	//Find the triangle of the screen that contains the Screen
	for (int i = 0; i < TriangleIndexes.Num(); i += 3)
	{
		FVector2D UVA = ScreenUVs[TriangleIndexes[i]];
		FVector2D UVB = ScreenUVs[TriangleIndexes[i + 1]];
		FVector2D UVC = ScreenUVs[TriangleIndexes[i + 2]];

		//Check if the UV coordinate lies in the current triangle
		if (USynOpticonStatics::PointInTriangle2DUVs(ScreenUVCoordinate, UVA, UVB, UVC))
		{
			//Compoute the barycentric coordinate of the point
			FVector ScreenPoint;
			ScreenPoint.Set(ScreenUVCoordinate.X, ScreenUVCoordinate.Y, 0);

			FVector UVAA(UVA.X, UVA.Y, 0);
			FVector UVBB(UVB.X, UVB.Y, 0);
			FVector UVCC(UVC.X, UVC.Y, 0);

			FVector Barycentric = FMath::ComputeBaryCentric2D(ScreenPoint, UVAA, UVBB, UVCC);

			//Calculate the world coordinate of the UV coordinate using the barycentric coordinate as a factor to get the world coordinate from the triangle verticies
			FVector A = ScreenVerticies[TriangleIndexes[i]];
			FVector B = ScreenVerticies[TriangleIndexes[i + 1]];
			FVector C = ScreenVerticies[TriangleIndexes[i + 2]];

			FVector WorldCoordinate = Barycentric.X * A + Barycentric.Y * B + Barycentric.Z * C;
			WorldPoint = WorldCoordinate;

			//Rotate the eye vectors so that they face the point
			FRotator LeftEyeOrientation = UKismetMathLibrary::FindLookAtRotation(LeftEyeSceneComponent->GetComponentLocation(), WorldCoordinate);
			LeftEyeOrientation.Roll = 0;
			//LeftEyeSceneComponent->SetWorldRotation(LeftEyeOrientation);

			FRotator RightEyeOrientation = UKismetMathLibrary::FindLookAtRotation(RightEyeSceneComponent->GetComponentLocation(), WorldCoordinate);
			RightEyeOrientation.Roll = 0;
			//RightEyeSceneComponent->SetWorldRotation(RightEyeOrientation);

			//Smooth the eye data by adding a factor of the new data to the old data
			//Left
			FQuat SmoothedRotation = FQuat::FastLerp(LeftEyeOrientation.Quaternion(), PrevLeftEyeRotation, ASynOpticonState::GetSmoothingMomentum());
			SmoothedRotation.Normalize();
			LeftEyeSceneComponent->SetWorldRotation(SmoothedRotation);
			PrevLeftEyeRotation = LeftEyeOrientation.Quaternion();

			//Right		
			SmoothedRotation = FQuat::FastLerp(RightEyeOrientation.Quaternion(), PrevRightEyeRotation, ASynOpticonState::GetSmoothingMomentum());
			SmoothedRotation.Normalize();
			RightEyeSceneComponent->SetWorldRotation(SmoothedRotation);
			PrevRightEyeRotation = RightEyeOrientation.Quaternion();

			//Scale the vectors so that they do not go through the screen
			float distance = FVector::Dist(LeftEyeSceneComponent->GetComponentLocation(), WorldCoordinate);
			LeftEyeSceneComponent->SetRelativeScale3D(FVector(distance / 500, 1, 1));
			distance = FVector::Dist(RightEyeSceneComponent->GetComponentLocation(), WorldCoordinate);
			RightEyeSceneComponent->SetRelativeScale3D(FVector(distance / 500, 1, 1));

			bIsInsideScreen = true;

			break;
		}
	}

	if (bIsInsideScreen) {
		HitTargetID = AttachedScreen->GetHitID(true, ScreenUVCoordinate);
	}
	else{
		HitTargetID = -1;
	}
}

int32 URemoteEyeTrackerComponent::GetHitTargetID()
{
	return HitTargetID;
}

bool URemoteEyeTrackerComponent::UpdateReplayEyeVectors()
{
	LeftEyeSceneComponent->SetRelativeLocation(SOBlock->RTData.LeftEyeLocation);
	RightEyeSceneComponent->SetRelativeLocation(SOBlock->RTData.RightEyeLocation);
	WorldPoint = SOBlock->RTData.GazePoint;
	return true;
}

void URemoteEyeTrackerComponent::SetOwnerID(int32 _OwnerID) {
	OwnerID = _OwnerID;
}

void URemoteEyeTrackerComponent::UpdatePupilData(float AveragedPupilRadius, float DeltaTime)
{
	if (PreviousPupilSize != -10) {
		if (FGenericPlatformMath::IsNaN(AveragedPupilRadius)) {
			AveragedPupilRadius = 0;
		}

		double velocity = (FMath::Abs(AveragedPupilRadius) - FMath::Abs(PreviousPupilSize)) / DeltaTime;

		if (FGenericPlatformMath::IsFinite(velocity)) {
			if (!StartBlinking && velocity <= ASynOpticonState::ONSET_THRESHOLD) {
				StartBlinking = true;
				BlinkClock = FDateTime::UtcNow();
			}
			if (!StartReversing && StartBlinking && velocity >= ASynOpticonState::REVERSAL_THRESHOLD) {
				StartReversing = true;
				//TODO: check if the previous value is close to zero, otherwise this is not an eyeblink
				if (PreviousPupilSize <= 0) {
					TotalBlinkCount += 1;
					IsEyeBlink = true;
				}
			}
			if (StartReversing && velocity >= ASynOpticonState::OFFSET_THRESHOLD - 0.5
				&& velocity <= ASynOpticonState::OFFSET_THRESHOLD + 0.5)
			{
				if (IsEyeBlink) {
					float Duration = (FDateTime::UtcNow() - BlinkClock).GetTotalMilliseconds();
					BlinkDurations.Add(Duration);

					if (Duration >= ASynOpticonState::DOUBLE_BLINK_THRESHOLD) {
						ASynOpticonState::GetGlobalEventSystem()->OnEventGazeGesture.Broadcast(GazeGestureEnum::GG_DoubleBlink);
					}
				}

				StartReversing = false;
				StartBlinking = false;
				IsEyeBlink = false;
			}
		}
	}

	PreviousPupilSize = AveragedPupilRadius;

	if (OwnerIsSelected) {
		ASynOpticonState::GetGlobalEventSystem()->OnPupilSampleRecieved.Broadcast((AveragedPupilRadius < 0) ? 0 : AveragedPupilRadius);
	}	

	if (!StartBlinking && !StartReversing && AveragedPupilRadius > 0)
		PupilRadiusBuffer.Add(AveragedPupilRadius);
	if ((FDateTime::UtcNow() - WindowClock).GetSeconds() >= 5) //enough with one window, 
															   //TODO: handle this window instead of discarding it
	{
		if (PupilRadiusBuffer.Num() > 0) {
			double temps = 0;
			for (float s : PupilRadiusBuffer) {
				temps += s;
			}
			AveragePupilSize = temps / PupilRadiusBuffer.Num();
			PupilRadiusBuffer.Empty();
		}
		else {
			AveragePupilSize = 0;
		}

		if (BlinkDurations.Num() > 0) {
			double tempb = 0;
			for (float b : BlinkDurations) {
				tempb += b;
			}
			AverageBlinkDuration = tempb / BlinkDurations.Num();
			BlinkDurations.Empty();
		}
		else {
			AverageBlinkDuration = 0;
		}

		float min = (FDateTime::UtcNow() - StartClock).GetMinutes() <= 0 ? 1 : (FDateTime::UtcNow() - StartClock).GetMinutes();
		BlinkRate = TotalBlinkCount / min;

		WindowClock = FDateTime::UtcNow();
	}

}

float URemoteEyeTrackerComponent::GetLastPupilRadius() {
	if (PupilRadiusBuffer.Num() > 0) {
		return PupilRadiusBuffer[PupilRadiusBuffer.Num() - 1];
	}
	return 0;
}

TArray<float> URemoteEyeTrackerComponent::GetPupilRadiusBuffer() {
	return PupilRadiusBuffer;
}

void URemoteEyeTrackerComponent::UpdateReplayGazeLocation()
{
	//Reset the scale of the eye vectors
	LeftEyeArrowMesh->SetRelativeScale3D(FVector(1, 1, 1));
	RightEyeArrowMesh->SetRelativeScale3D(FVector(1, 1, 1));
	bool LeftEyeValid = false;
	bool RightEyeValid = false;

	if (SOBlock->RTData.LeftEyeLocation.X + SOBlock->RTData.LeftEyeLocation.Y + SOBlock->RTData.LeftEyeLocation.Z != 0)
	{
		LeftEyeValid = true;
	}
	else
	{
		LeftEyeSceneComponent->SetVisibility(false, true);
	}
	if (SOBlock->RTData.RightEyeLocation.X + SOBlock->RTData.RightEyeLocation.Y + SOBlock->RTData.RightEyeLocation.Z != 0)
	{
		RightEyeValid = true;
	}
	else
	{
		RightEyeSceneComponent->SetVisibility(false, true);
	}

	FVector2D EstimatedGazePoint;

	if (!LeftEyeValid && !RightEyeValid)
	{
		return;
	}

	FVector WorldCoordinate = SOBlock->RTData.GazePoint;

	//Rotate the eye vectors so that they face the point
	FRotator LeftEyeOrientation = UKismetMathLibrary::FindLookAtRotation(LeftEyeSceneComponent->GetComponentLocation(), WorldCoordinate);
	LeftEyeOrientation.Roll = 0;

	FRotator RightEyeOrientation = UKismetMathLibrary::FindLookAtRotation(RightEyeSceneComponent->GetComponentLocation(), WorldCoordinate);
	RightEyeOrientation.Roll = 0;

	//Smooth the eye data by adding a factor of the new data to the old data
	//Left
	FQuat SmoothedRotation = FQuat::FastLerp(LeftEyeOrientation.Quaternion(), PrevLeftEyeRotation, ASynOpticonState::GetSmoothingMomentum());
	SmoothedRotation.Normalize();
	LeftEyeSceneComponent->SetWorldRotation(SmoothedRotation);
	PrevLeftEyeRotation = LeftEyeOrientation.Quaternion();

	//Right		
	SmoothedRotation = FQuat::FastLerp(RightEyeOrientation.Quaternion(), PrevRightEyeRotation, ASynOpticonState::GetSmoothingMomentum());
	SmoothedRotation.Normalize();
	RightEyeSceneComponent->SetWorldRotation(SmoothedRotation);
	PrevRightEyeRotation = RightEyeOrientation.Quaternion();

	//Scale the vectors so that they do not go through the screen
	float distance = FVector::Dist(LeftEyeSceneComponent->GetComponentLocation(), WorldCoordinate);
	LeftEyeArrowMesh->SetRelativeScale3D(FVector(distance / 100, 1, 1));
	distance = FVector::Dist(RightEyeSceneComponent->GetComponentLocation(), WorldCoordinate);
	RightEyeArrowMesh->SetRelativeScale3D(FVector(distance / 100, 1, 1));
}

void URemoteEyeTrackerComponent::ToBinaryReplayData()
{
	SOBlock->hasRTData = true;
	SOBlock->RTData.LeftEyeLocation = LeftEyeSceneComponent->RelativeLocation;
	SOBlock->RTData.LeftEyeRotation = LeftEyeSceneComponent->RelativeRotation;
	SOBlock->RTData.LeftEyePupilRadius = LeftEyePupilRadius;
	SOBlock->RTData.RightEyeLocation = RightEyeSceneComponent->RelativeLocation;
	SOBlock->RTData.RightEyeRotation = RightEyeSceneComponent->RelativeRotation;
	SOBlock->RTData.RightEyePupilRadius = RightEyePupilRadius;
	SOBlock->RTData.GazePoint = WorldPoint;
}

void URemoteEyeTrackerComponent::SetRETDataStruct(FRemoteEyeTrackerStruct _RemoteEyeTrackerStruct)
{
	RemoteEyeTrackerStruct = _RemoteEyeTrackerStruct;

	TrackerPosition = (RemoteEyeTrackerStruct.RemoteTrackerPositionLeft + RemoteEyeTrackerStruct.RemoteTrackerPositionRight) / 2;
	GetOwner()->SetActorLocation(TrackerPosition);

	//Set the forward vector
	PointToLookAt = RemoteEyeTrackerStruct.PointToLookAt;

	FVector ForwardVector = TrackerPosition - PointToLookAt;

	FRotator RemoteTrackerOrientation = UKismetMathLibrary::FindLookAtRotation(TrackerPosition, PointToLookAt);
	RemoteTrackerOrientation.Roll = 0;
	RemoteTrackerOrientation.Yaw = 0;

	FVector OriginalRight = GetOwner()->GetActorRightVector();
	FVector RightVector = _RemoteEyeTrackerStruct.RemoteTrackerPositionRight - _RemoteEyeTrackerStruct.RemoteTrackerPositionLeft;

	FRotator Temp = RightVector.ToOrientationRotator().RotateVector(OriginalRight).ToOrientationRotator();
	Temp.Roll = 0;

	GetOwner()->SetActorRotation(Temp);
	GetOwner()->AddActorLocalRotation(RemoteTrackerOrientation);

	AttachedScreen = nullptr;

	for (TObjectIterator<AGazeActor> Itr; Itr; ++Itr)
	{
		// Filter out objects not contained in the target world.
		if (Itr->GetWorld() != GetOwner()->GetWorld())
		{
			continue;
		}

		// Find the GazeActor representing the attached screen by comparing their names
		if (Itr->GetName().Equals(_RemoteEyeTrackerStruct.AttachedScreenName))
		{
			AttachedScreen = *Itr;
		}
	}

	if (AttachedScreen)
	{
		TArray<FVector> ScreenVerticies = AttachedScreen->GetVerticies();

		FVector ScreenUpVector = ScreenVerticies[0] - ScreenVerticies[2];
		ScreenUpVector.Normalize();
		FVector ScreenRightVector = ScreenVerticies[0] - ScreenVerticies[1];
		ScreenRightVector.Normalize();
		FVector ScreenForwardVector = FVector::CrossProduct(ScreenRightVector, ScreenUpVector);
		ScreenForwardVector.Normalize();
	}
}

void URemoteEyeTrackerComponent::SetAttachedScreen(AGazeActor* _AttachedScreen)
{
	AttachedScreen = _AttachedScreen;
}

void URemoteEyeTrackerComponent::SetSOBlock(CompressedSOBlock* _SOBlock)
{
	SOBlock = _SOBlock;
}

FVector URemoteEyeTrackerComponent::GetWorldGazePoint()
{
	return WorldPoint;
}

AGazeActor* URemoteEyeTrackerComponent::GetAttachedScreen()
{
	return AttachedScreen;
}