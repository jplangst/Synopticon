// Fill out your copyright notice in the Description page of Project Settings.

//#include "Synopticon.h"
#include "EyeVectorsComponent.h"
#include "SystemClasses/GazeEventDataStructures.h"


double MACULAR_ANGLE = 9.0;
double PX_TO_CM_FACTOR = (1.0 / 72.0) * 2.54; //Point to cm: 1pt = 1/72 inch, 1 inch = 2.54 cm
double STANDARD_MINUTES_OF_ARC = 15.0;

// Sets default values for this component's properties
UEyeVectorsComponent::UEyeVectorsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Calibrated = false;
	CalibrationRotatorLeft = FRotator(0, 0, 0);
	CalibrationRotatorRight = FRotator(0, 0, 0);
	// ...

	//create glasses
	GlassesMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("glassesMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GlassesMeshObj(TEXT("StaticMesh'/Game/Models/glasses/accurate_glasses.accurate_glasses'"));
	GlassesMesh->SetStaticMesh(GlassesMeshObj.Object);

	//Prepare the eye vector scene nodes
	LeftEyeSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("LeftEyeSceneComp"));
	RightEyeSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RightEyeSceneComp"));
	CombinedVectorSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CombinedVectorSceneComp"));	

	//create eye vectors
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ArrowMeshObj(TEXT("StaticMesh'/Game/Models/arrow/arrow.arrow'"));
	static ConstructorHelpers::FObjectFinder<UMaterial>YellowMaterial(TEXT("Material'/Game/Materials/SynOpticonActorMaterials/OrientationVectorMaterial.OrientationVectorMaterial'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> BlueMaterial(TEXT("Material'/Game/Materials/SynOpticonActorMaterials/eyeVectorMaterial.eyeVectorMaterial'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> GreenMaterial(TEXT("Material'/Game/Materials/SynOpticonActorMaterials/ConvergenceVectorMaterial.ConvergenceVectorMaterial'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> VectorMaterial(TEXT("Material'/Game/Materials/SynOpticonActorMaterials/VectorMaterial.VectorMaterial'"));

	LeftEyeArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftArrowMesh"));
	LeftEyeArrowMesh->SetStaticMesh(ArrowMeshObj.Object);
	LeftEyeArrowMesh->SetMaterial(0, BlueMaterial.Object);
	LeftEyeArrowMesh->SetWorldScale3D(FVector(5,1,1));
	LeftEyeArrowMesh->AttachToComponent(LeftEyeSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

	RightEyeArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightArrowMesh"));
	RightEyeArrowMesh->SetStaticMesh(ArrowMeshObj.Object);
	RightEyeArrowMesh->SetMaterial(0, BlueMaterial.Object);
	RightEyeArrowMesh->SetWorldScale3D(FVector(5, 1, 1));
	RightEyeArrowMesh->AttachToComponent(RightEyeSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

	CombinedVectorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CombinedArrowMesh"));
	CombinedVectorMesh->SetStaticMesh(ArrowMeshObj.Object);
	CombinedVectorMesh->SetWorldScale3D(FVector(5, 1, 1));
	CombinedVectorMesh->SetMaterial(0, UMaterialInstanceDynamic::Create(VectorMaterial.Object, NULL));
	CombinedVectorMesh->AttachToComponent(CombinedVectorSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//Convergenced Point
	ConvergencedPointComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CombinedPointSceneComp"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("StaticMesh'/Game/Models/glasses/ViewCone.ViewCone'"));
	static ConstructorHelpers::FObjectFinder <UMaterial>RedMaterial(TEXT("Material'/Game/Materials/GroundPlaneMaterials/xAxisMaterial.xAxisMaterial'"));
	ConvergencedPointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ConvergencedPoint"));
	ConvergencedPointMesh->SetStaticMesh(SphereVisualAsset.Object);
	ConvergencedPointMesh->SetMaterial(0, RedMaterial.Object);
	ConvergencedPointMesh->AttachToComponent(ConvergencedPointComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ConvergencedPointMesh->SetVisibility(false);

	ViewConeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("viewConeMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ViewConeMeshObj(TEXT("StaticMesh'/Game/Models/glasses/ViewCone.ViewCone'"));
	static ConstructorHelpers::FObjectFinder <UMaterial>ViewConeMaterial(TEXT("Material'/Game/Materials/SynOpticonActorMaterials/ViewConeMaterial.ViewConeMaterial'"));
	ViewConeMesh->SetStaticMesh(ViewConeMeshObj.Object);
	ViewConeMesh->SetMaterial(0, ViewConeMaterial.Object);
	ViewConeMesh->AttachToComponent(CombinedVectorSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//ViewConeMesh->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0, 90, 0)));
	//ViewConeMesh->SetRelativeLocation(FVector(50, 0, 0));
	ViewConeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PreviousPupilSize = -10;
	StartBlinking = false;
	StartReversing = false;
	IsEyeBlink = false;
	BlinkClock = 0;

	EyeTrackingWAMPComponent = nullptr;
	GlassesID = "";
}

// Called when the game starts
void UEyeVectorsComponent::BeginPlay(){
	Super::BeginPlay();

	Calibrated = false;
	CalibrationRotatorLeft = FRotator(0, 0, 0);
	CalibrationRotatorRight = FRotator(0, 0, 0);

	GlassesMesh->RegisterComponent();
	CombinedVectorMesh->RegisterComponent();
	RightEyeArrowMesh->RegisterComponent();
	LeftEyeArrowMesh->RegisterComponent();
	ConvergencedPointMesh->RegisterComponent();
	ViewConeMesh->RegisterComponent();

	GlassesMesh->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	LeftEyeSceneComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	RightEyeSceneComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	CombinedVectorSceneComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	ConvergencedPointComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

	ScaleDelay = 0.2f;
	ScaleDelayCounter = 0;
	StartScaleDelay = false;
	StartClock = FDateTime::UtcNow();
	WindowClock = FDateTime::UtcNow();

	//ScaleViewConeUsingAngle(600, FMath::DegreesToRadians(9)); //6 meters - 18 degree

	IsDoubleBlink = false;

	PreviousPupilSize = -10;
	StartBlinking = false;
	StartReversing = false;
	IsEyeBlink = false;
	BlinkClock = 0;

	ASynOpticonState::GetGlobalEventSystem()->OnEventUpdateCurrentSOActor.AddDynamic(this, &UEyeVectorsComponent::OnActorChanged);

	ASynOpticonState::GetGlobalEventSystem()->OnEventWAMPComponentDisconnected.AddDynamic(this, &UEyeVectorsComponent::OnEyeWAMPComponentDisconnected);
	ASynOpticonState::GetGlobalEventSystem()->OnEventWAMPComponentConnected.AddDynamic(this, &UEyeVectorsComponent::OnEyeWAMPComponentConnected);
}

// Called every frame
void UEyeVectorsComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ){
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
	if (StartScaleDelay){
		ScaleDelayCounter += DeltaTime;
	}

	UpdateVectorsVisibility();
	UpdateEyeData(DeltaTime);
}

void UEyeVectorsComponent::SetOwnerID(int32 _OwnerID) {
	OwnerID = _OwnerID;
}

void UEyeVectorsComponent::OnActorChanged(int ActorID, bool HasETG, bool HasSHimmerHR, bool HasShimmerGSR, FString ETEDevice, FString ShimmerDevice) {
	OwnerIsSelected = OwnerID == ActorID;
}

void UEyeVectorsComponent::UpdateEyeData(float DeltaTime){
	bool LeftEyeValid = false;
	bool RightEyeValid = false;
	if (!ASynOpticonState::IsReplaying()) //real-time
	{
		if (EyeTrackingWAMPComponent) {
			FWAMPEyeDataStruct* EyeDataSample = EyeTrackingWAMPComponent->GetEyeDataSample(GlassesID);
			if (EyeDataSample) {
				//We set the invalid samples to be streamed with a pupil confidence of -1
				if (EyeDataSample->LeftPupilConfidence > 0) {
					LeftEyeValid = UpdateEyeVectorWAMP(EyeDataSample, LeftEyeSceneComponent, true);				
				}

				if (ASynOpticonState::IsVectorVisible(VectorVisibilityEnum::VE_EyeVector)) {
					LeftEyeSceneComponent->SetVisibility(LeftEyeValid, true);
				} 
				else {
					LeftEyeSceneComponent->SetVisibility(false, true);
				}

				if (EyeDataSample->RightPupilConfidence > 0) {
					RightEyeValid = UpdateEyeVectorWAMP(EyeDataSample, RightEyeSceneComponent, false);
				}

				if (ASynOpticonState::IsVectorVisible(VectorVisibilityEnum::VE_EyeVector)) {
					RightEyeSceneComponent->SetVisibility(RightEyeValid, true);
				}
				else {
					RightEyeSceneComponent->SetVisibility(false, true);
				}

				UpdatePupilDataWAMP(EyeDataSample, DeltaTime);

				//TODO use left or right eye if one eye is open...
				if (LeftEyeValid && RightEyeValid) {
					FindConvergenceVector();
				}
				else {
					CombinedVectorSceneComponent->SetVisibility(false, true);
				}

				delete EyeDataSample;
			}
		}
	}
	else if(ASynOpticonState::IsReplaying() && !ASynOpticonState::IsPausingReplay()) //replay
	{
		if (SOBlock){
			if (EyeTrackingWAMPComponent) {			
				if (SOBlock->ImageDataBufferSize > 0) { //TODO add a check to make sure the databuffersize is not bigger than the allocated buffersize
					ImageDataStructures::VP8EncodedImageData* SceneCameraImage = new ImageDataStructures::VP8EncodedImageData();

					SceneCameraImage->ImageDataBufferSize = SOBlock->ImageDataBufferSize;
					FMemory::Memcpy(SceneCameraImage->ImageBuffer, SOBlock->SceneCameraImageBuffer, SOBlock->ImageDataBufferSize);
					SceneCameraImage->ImageWidth = SOBlock->ImageWidth;
					SceneCameraImage->ImageHeight = SOBlock->ImageHeight;
					SceneCameraImage->IsKeyFrame = SOBlock->IsKeyFrame;

					EyeTrackingWAMPComponent->AddEncodedSceneCameraImage(GlassesID, SceneCameraImage);
				}
				TPair<FString, float> Sample;

				//TODO: should only publish data from the current actor
				Sample.Key = GlassesID + "Replay";
				Sample.Value = SOBlock->ETGData.PupilRadius;
				EyeTrackingWAMPComponent->PublishReplayData(Sample);
			}

			bool ValidData = UpdateReplayEyeVectors(LeftEyeValid, RightEyeValid);
			UpdatePupilDataWAMP(SOBlock->ETGData.PupilRadius, DeltaTime);
			if (ValidData)
			{
				FindConvergenceVector();
			}
			else
			{
				CombinedVectorSceneComponent->SetVisibility(false, true);
			}
		}
	}

	URaycastComponent_Ours* RaycastComponent = GetOwner()->FindComponentByClass<URaycastComponent_Ours>();
	if (RaycastComponent)
	{
		if (LeftEyeValid && !RightEyeValid)
		{
			RaycastComponent->SetVectorSceneComponent(LeftEyeSceneComponent);
		}
		else if (RightEyeValid && !LeftEyeValid)
		{
			RaycastComponent->SetVectorSceneComponent(RightEyeSceneComponent);
		}
		else
		{
			RaycastComponent->SetVectorSceneComponent(GetConvergenceVectorSceneComponent());
		}
		AGazeActor* Target = RaycastComponent->GetInteractingActor();
		if (Target)
		{
			ScaleViewConeUsingFontSize(Target->GetFontSize());
		}
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

			if (StartScaleDelay && ScaleDelayCounter > ScaleDelay)
			{
				LeftEyeSceneComponent->SetRelativeScale3D(FVector(1, 1, 1));
				RightEyeSceneComponent->SetRelativeScale3D(FVector(1, 1, 1));
				CombinedVectorSceneComponent->SetRelativeScale3D(FVector(1, 1, 1));
				ScaleViewConeUsingAngle(600, MACULAR_ANGLE);

				ScaleDelay = 0;
				StartScaleDelay = false;
			}
		}
	}
}

bool UEyeVectorsComponent::UpdateEyeVectorWAMP(FWAMPEyeDataStruct* EyeDataSample, USceneComponent* EyeVectorNode, bool Left) {
	//Get the eye position from the sample data
	FVector EyeLocation;
	if (Left) {
		EyeLocation = EyeDataSample->LeftEyeLocation;
	}
	else {
		EyeLocation = EyeDataSample->RightEyeLocation;
	}

	//Scale the vector to the correct unit and convert it to the correct coordinate system
	FVector EyePosition = EyeLocation * ASynOpticonState::GetEyesTrackingUnitToUnrealUnitFactor();
	//Add the calculated offset from the glasses to the camera off the glasses to correct the position of the eye
	//EyePosition += SettingsManager::GetInstance()->GetGlassesOffset();
	EyePosition = USynOpticonStatics::ConvertNegXYZtoXZYCoordinateSystem(EyePosition);

	//Update the position of the eye
	EyeVectorNode->SetRelativeLocation(EyePosition);

	//Get the direction vector of the eye from the sample data
	FVector SampleDirectionVector;
	if (Left) {
		SampleDirectionVector = EyeDataSample->LeftEyeDirection;
	}
	else {
		SampleDirectionVector = EyeDataSample->RightEyeDirection;
	}

	//(Utilities::ConvertTrackerVectorToFVector(TrackerObj.quat));
	SampleDirectionVector = USynOpticonStatics::ConvertNegXYZtoXZYCoordinateSystem(SampleDirectionVector);
	//Get the orientation of the eye data
	FQuat NewRotation = SampleDirectionVector.ToOrientationQuat();
	//EyeVectorNode->GetRelativeTransform().GetRotation().GetForwardVector().ToOrientationQuat()

	//check valid data. 
	if ((Left && (EyeDataSample->LeftPupilConfidence == -1)) ||
		(!Left && (EyeDataSample->RightPupilConfidence == -1)))//invalid left eye data
	{
		//the data is invalid but we set it anyway to make sure that we will save invalid data for replay
		//but we return 'false' and make it invisible
		NewRotation.X = NAN;
		EyeVectorNode->SetRelativeRotation(NewRotation);
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
		EyeVectorNode->SetRelativeRotation(SmoothedRotation);
	}
	else
	{
		NewRotation.Normalize();
		EyeVectorNode->SetRelativeRotation(NewRotation);
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

void UEyeVectorsComponent::UpdatePupilDataWAMP(FWAMPEyeDataStruct* EyeDataSample, float DeltaTime) {
	double pupil_size = (EyeDataSample->LeftPupilConfidence >= EyeDataSample->RightPupilConfidence) ? EyeDataSample->LeftPupilRadius : EyeDataSample->RightPupilRadius;
	pupilRadius = pupil_size;

	if (PreviousPupilSize != -10) {
		if (FGenericPlatformMath::IsNaN(pupil_size)) {
			pupil_size = 0;
		}
		double velocity = (FMath::Abs(pupil_size) - FMath::Abs(PreviousPupilSize)) / DeltaTime;

		if (FGenericPlatformMath::IsFinite(velocity)) {
			if (!StartBlinking && velocity <= ASynOpticonState::ONSET_THRESHOLD) {
				StartBlinking = true;
				BlinkClock = FDateTime::UtcNow();

			}
			if (!StartReversing && StartBlinking && velocity >= ASynOpticonState::REVERSAL_THRESHOLD) {
				StartReversing = true;
				//TODO: check if the previous value is close to zero, otherwise this is not a eyeblink
				if (PreviousPupilSize <= 0) {
					TotalBlinkCount += 1;
					/*current_blink_count += 1;*/
					IsEyeBlink = true;

					/*if (IsDoubleBlink)
					{
					float timeDifference = (FDateTime::UtcNow() - DoubleBlinkClock).GetTotalMilliseconds();
					if (timeDifference <= ASynOpticonState::DOUBLE_BLINK_THRESHOLD)
					{
					ASynOpticonState::GetGlobalEventSystem()->OnEventGazeGesture.Broadcast(GazeGestureEnum::GG_DoubleBlink);
					}
					IsDoubleBlink = false;
					}
					else
					{
					IsDoubleBlink = true;
					DoubleBlinkClock = FDateTime::UtcNow();
					}*/
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

	if (OwnerIsSelected) {
		ASynOpticonState::GetGlobalEventSystem()->OnPupilSampleRecieved.Broadcast((pupilRadius < 0) ? 0 : pupilRadius);
	}

	PreviousPupilSize = pupil_size;
	if (!StartBlinking && !StartReversing && pupil_size > 0)
		PupilRadiusBuffer.Add(pupil_size);
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

void UEyeVectorsComponent::UpdatePupilDataWAMP(double pupil_size, float DeltaTime)
{
	if (PreviousPupilSize != -10) {
		if (FGenericPlatformMath::IsNaN(pupil_size)) {
			pupil_size = 0;
		}
		double velocity = (FMath::Abs(pupil_size) - FMath::Abs(PreviousPupilSize)) / DeltaTime;

		if (FGenericPlatformMath::IsFinite(velocity)) {
			if (!StartBlinking && velocity <= ASynOpticonState::ONSET_THRESHOLD) {
				StartBlinking = true;
				BlinkClock = FDateTime::UtcNow();

			}
			if (!StartReversing && StartBlinking && velocity >= ASynOpticonState::REVERSAL_THRESHOLD) {
				StartReversing = true;
				//TODO: check if the previous value is close to zero, otherwise this is not a eyeblink
				if (PreviousPupilSize <= 0) {
					TotalBlinkCount += 1;
					/*current_blink_count += 1;*/
					IsEyeBlink = true;

					/*if (IsDoubleBlink)
					{
					float timeDifference = (FDateTime::UtcNow() - DoubleBlinkClock).GetTotalMilliseconds();
					if (timeDifference <= ASynOpticonState::DOUBLE_BLINK_THRESHOLD)
					{
					ASynOpticonState::GetGlobalEventSystem()->OnEventGazeGesture.Broadcast(GazeGestureEnum::GG_DoubleBlink);
					}
					IsDoubleBlink = false;
					}
					else
					{
					IsDoubleBlink = true;
					DoubleBlinkClock = FDateTime::UtcNow();
					}*/
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

	if (OwnerIsSelected) {
		ASynOpticonState::GetGlobalEventSystem()->OnPupilSampleRecieved.Broadcast((pupil_size < 0) ? 0 : pupil_size);
	}

	PreviousPupilSize = pupil_size;
	if (!StartBlinking && !StartReversing && pupil_size > 0)
		PupilRadiusBuffer.Add(pupil_size);
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

bool UEyeVectorsComponent::UpdateReplayEyeVectors(bool& LeftEyeValid, bool& RightEyeValid) {
	if (SOBlock)
	{
		ETGDataBlock ETGData = SOBlock->ETGData;
		LeftEyeSceneComponent->SetRelativeLocation(ETGData.LeftEyeLocation);
		LeftEyeSceneComponent->SetRelativeRotation(ETGData.LeftEyeRotation);
		RightEyeSceneComponent->SetRelativeLocation(ETGData.RightEyeLocation);
		RightEyeSceneComponent->SetRelativeRotation(ETGData.RightEyeRotation);
		if (ASynOpticonState::IsVectorVisible(VectorVisibilityEnum::VE_EyeVector)) {
			LeftEyeSceneComponent->SetVisibility(!ETGData.LeftEyeRotation.ContainsNaN(), true);
			RightEyeSceneComponent->SetVisibility(!ETGData.RightEyeRotation.ContainsNaN(), true);
		}
		else {
			LeftEyeSceneComponent->SetVisibility(false, false);
			RightEyeSceneComponent->SetVisibility(false, true);
		}
		LeftEyeValid = !ETGData.LeftEyeRotation.ContainsNaN();
		RightEyeValid = !ETGData.RightEyeRotation.ContainsNaN();
		return LeftEyeValid && RightEyeValid;
	}
	return false;
}

void UEyeVectorsComponent::SetOuterPosOriComponent(USceneComponent* _OuterPosOriComp) {
	OuterPosOriComp = _OuterPosOriComp;
}

void UEyeVectorsComponent::FindConvergenceVector(){
	FTransform LeftEyeTransform = LeftEyeSceneComponent->GetComponentTransform();
	FTransform RightEyeTransform = RightEyeSceneComponent->GetComponentTransform();
	CombinedVectorSceneComponent->SetWorldLocation((LeftEyeTransform.GetLocation() + RightEyeTransform.GetLocation()) / 2);

	TPair<FVector, float> ConvergencePoint = USynOpticonStatics::ConvPointAndDist(LeftEyeTransform.GetRotation().GetForwardVector(), RightEyeTransform.GetRotation().GetForwardVector(), LeftEyeTransform.GetLocation(), RightEyeTransform.GetLocation());

	if (ConvergencePoint.Value != -1){
		FVector CombinedVectorDirection = ConvergencePoint.Key - CombinedVectorSceneComponent->GetComponentLocation();
		FVector ConvergencedPoint = ConvergencePoint.Key;

		FVector PosOriForward = GetForwardVector();//OuterPosOriComp->GetForwardVector(); //GetOwner()->GetActorRotation().Vector()

		float Dotproduct = FVector::DotProduct(CombinedVectorDirection, PosOriForward);

		if (Dotproduct < 0){
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

void UEyeVectorsComponent::UpdateVectorsVisibility()
{
	LeftEyeArrowMesh->SetVisibility(ASynOpticonState::IsVectorVisible(VectorVisibilityEnum::VE_EyeVector), false);
	RightEyeArrowMesh->SetVisibility(ASynOpticonState::IsVectorVisible(VectorVisibilityEnum::VE_EyeVector), false);
	CombinedVectorSceneComponent->SetVisibility(ASynOpticonState::IsVectorVisible(VectorVisibilityEnum::VE_ConvergenceVector), true);
	ViewConeMesh->SetVisibility(ASynOpticonState::IsShowingViewCone(), false);
}

//Calibrate the eye vectors using the selected calibration points
void UEyeVectorsComponent::CalibrateEyeVectors(FVector CalPoint, int CalNum) //Perform calibration when CalNum == 1
{
	Calibrated = false;

	//=================Convert from world coordinate to glasses coordinate================
	FVector CalibrationPoint = GetComponentTransform().InverseTransformPosition(CalPoint);

	FVector CurrentDirectionVectorLeft = LeftEyeSceneComponent->GetRelativeTransform().GetRotation().GetForwardVector();
	CurrentDirectionVectorLeft.Normalize();
	FVector CorrectDirectionVectorLeft = (CalibrationPoint - LeftEyeSceneComponent->GetRelativeTransform().GetLocation());
	CorrectDirectionVectorLeft.Normalize();
	FQuat RelativeRotationLeft = FQuat::FindBetween(CurrentDirectionVectorLeft, CorrectDirectionVectorLeft);

	FVector AnglesL = RelativeRotationLeft.Euler();
	TArray<double> DatL;
	DatL.Add(AnglesL.Y); DatL.Add(AnglesL.Z); AnglesLeft.Add(DatL);

	FVector CurrentDirectionVectorRight = RightEyeSceneComponent->GetRelativeTransform().GetRotation().GetForwardVector();
	CurrentDirectionVectorRight.Normalize();
	FVector CorrectDirectionVectorRight = (CalibrationPoint - RightEyeSceneComponent->GetRelativeTransform().GetLocation());
	CorrectDirectionVectorRight.Normalize();
	FQuat RelativeRotationRight = FQuat::FindBetween(CurrentDirectionVectorRight, CorrectDirectionVectorRight);

	FVector AnglesR = RelativeRotationRight.Euler();
	TArray<double> DatR;
	DatR.Add(AnglesR.Y); DatR.Add(AnglesR.Z); AnglesRight.Add(DatR);

	if (CalNum == 1) {
		if (AnglesLeft.Num() == 1) {
			CalibrationRotatorLeft = RelativeRotationLeft.Rotator();
			CalibrationRotatorRight = RelativeRotationRight.Rotator();
		}
		else {
			TArray<double> ResultL = USynOpticonStatics::estimator(AnglesLeft);
			FRotator ResultRotatorL = FRotator(ResultL[0], ResultL[1], 0);
			CalibrationRotatorLeft = ResultRotatorL;

			TArray<double> ResultR = USynOpticonStatics::estimator(AnglesRight);
			FRotator ResultRotatorR = FRotator(ResultR[0], ResultR[1], 0);
			CalibrationRotatorRight = ResultRotatorR;
		}

		CalibrationRotatorLeft.Normalize();
		AnglesLeft.Empty();

		CalibrationRotatorRight.Normalize();
		AnglesRight.Empty();

		if (!Calibrated)
			Calibrated = true;
	}

	//Calibrated = false;

	////=================Convert from world coordinate to glasses coordinate================
	//FVector CalibrationPoint = GetOwner()->GetActorTransform().InverseTransformPosition(CalPoint);

	//FVector CurrentDirectionVectorLeft = LeftEyeSceneComponent->GetRelativeTransform().GetRotation().GetForwardVector();
	//CurrentDirectionVectorLeft.Normalize();
	//FVector CorrectDirectionVectorLeft = (CalibrationPoint - LeftEyeSceneComponent->GetRelativeTransform().GetLocation());
	//CorrectDirectionVectorLeft.Normalize();
	//FQuat RelativeRotationLeft = FQuat::FindBetween(CurrentDirectionVectorLeft, CorrectDirectionVectorLeft);

	//FVector AnglesL = RelativeRotationLeft.Euler();
	//TArray<double> DatL;
	//DatL.Add(AnglesL.Y); DatL.Add(AnglesL.Z); AnglesLeft.Add(DatL);

	//FVector CurrentDirectionVectorRight = RightEyeSceneComponent->GetRelativeTransform().GetRotation().GetForwardVector();
	//CurrentDirectionVectorRight.Normalize();
	//FVector CorrectDirectionVectorRight = (CalibrationPoint - RightEyeSceneComponent->GetRelativeTransform().GetLocation());
	//CorrectDirectionVectorRight.Normalize();
	//FQuat RelativeRotationRight = FQuat::FindBetween(CurrentDirectionVectorRight, CorrectDirectionVectorRight);

	//FVector AnglesR = RelativeRotationRight.Euler();
	//TArray<double> DatR;
	//DatR.Add(AnglesR.Y); DatR.Add(AnglesR.Z); AnglesRight.Add(DatR);

	//if (CalNum == 1){
	//	if (AnglesLeft.Num() == 1) {
	//		CalibrationRotatorLeft = RelativeRotationLeft.Rotator();
	//		CalibrationRotatorRight = RelativeRotationRight.Rotator();
	//	}
	//	else {
	//		TArray<double> ResultL = USynOpticonStatics::estimator(AnglesLeft);
	//		FRotator ResultRotatorL = FRotator(ResultL[0], ResultL[1], 0);
	//		CalibrationRotatorLeft = ResultRotatorL;

	//		TArray<double> ResultR = USynOpticonStatics::estimator(AnglesRight);
	//		FRotator ResultRotatorR = FRotator(ResultR[0], ResultR[1], 0);
	//		CalibrationRotatorRight = ResultRotatorR;
	//	}
	//	
	//	CalibrationRotatorLeft.Normalize();
	//	AnglesLeft.Empty();
	//	
	//	CalibrationRotatorRight.Normalize();
	//	AnglesRight.Empty();

	//	if (!Calibrated)
	//		Calibrated = true;
	//}
}

TArray<float> UEyeVectorsComponent::GetPupilRadiusBuffer(){
	return PupilRadiusBuffer;
}

float UEyeVectorsComponent::GetLastPupilRadius() {
	if (PupilRadiusBuffer.Num() > 0) {
		return PupilRadiusBuffer[PupilRadiusBuffer.Num() - 1];
	}
	return 0;
}

void UEyeVectorsComponent::ResetCalibration(){
	CalibrationRotatorLeft = FRotator(0, 0, 0);
	CalibrationRotatorRight = FRotator(0, 0, 0);
	Calibrated = false;
}

void UEyeVectorsComponent::SetEyeTrackingWAMPComponent(UEyeTrackingWAMPComponent* _EyeTrackingWAMPComponent) {
	EyeTrackingWAMPComponent = _EyeTrackingWAMPComponent;
}

UEyeTrackingWAMPComponent* UEyeVectorsComponent::GetEyeTrackingWAMPComponent() {
	return EyeTrackingWAMPComponent;
}

USceneComponent* UEyeVectorsComponent::GetConvergenceVectorSceneComponent(){
	return CombinedVectorSceneComponent;
}

void UEyeVectorsComponent::ScaleViewCone(float length, float width)
{
	ViewConeMesh->SetRelativeScale3D(FVector(length/600, 1, 1));
}

void UEyeVectorsComponent::ScaleViewConeUsingAngle(float distance, float angle)
{
	float width = distance*FMath::Abs(FMath::Tan(FMath::DegreesToRadians(angle)));
	ViewConeMesh->SetWorldScale3D(FVector(distance/600, width/95, width/95));
}

void UEyeVectorsComponent::ScaleViewConeUsingFontSize(float FontSize)
{
	//first, we need to convert font size to cm
	float s = FontSize * PX_TO_CM_FACTOR;
	//calculate max distance that the font size can be read, the tan function reads radiance, remember to convert degress to radiances
	// d = s / tan(M/60), reference: http://www.extron.com/company/article.aspx?id=videowallfontsize
	double d = s / (FMath::Tan(FMath::DegreesToRadians(STANDARD_MINUTES_OF_ARC/60.0)));
	//scale view cone
	ScaleViewConeUsingAngle(d, MACULAR_ANGLE);
}

// Called when the game ends
void UEyeVectorsComponent::EndPlay(const EEndPlayReason::Type EndPlayReason){
	Super::EndPlay(EndPlayReason);
	
	if (SOBlock){
		SOBlock = nullptr;
	}

	PupilRadiusBuffer.Empty();
	Calibrated = false;

	GlassesMesh->DestroyComponent();
	CombinedVectorMesh->DestroyComponent();
	RightEyeArrowMesh->DestroyComponent();
	LeftEyeArrowMesh->DestroyComponent();
	ConvergencedPointMesh->DestroyComponent();
	EyeTrackingWAMPComponent = nullptr;
	GlassesID = "";
}

void UEyeVectorsComponent::SetETDataStruct(FEyeTrackerStruct _EyeTrackerDataStruct){
	EyeTrackerDataStruct = _EyeTrackerDataStruct;
}

void UEyeVectorsComponent::ToBinaryReplayData(){
	SOBlock->HasETGData = true;
	SOBlock->ETGData.LeftEyeLocation = LeftEyeSceneComponent->GetRelativeTransform().GetLocation();
	SOBlock->ETGData.LeftEyeRotation = LeftEyeSceneComponent->GetRelativeTransform().GetRotation().Rotator();
	SOBlock->ETGData.RightEyeLocation = RightEyeSceneComponent->GetRelativeTransform().GetLocation();
	SOBlock->ETGData.RightEyeRotation = RightEyeSceneComponent->GetRelativeTransform().GetRotation().Rotator();
	SOBlock->ETGData.PupilRadius = pupilRadius;
	SOBlock->ImageDataBufferSize = 0;

	if (EyeTrackingWAMPComponent && !GlassesID.Equals("")) 
	{
		EyeTrackingWAMPComponent->SaveEncodedSceneCameraFrame(GlassesID, SOBlock);
	}
}

void UEyeVectorsComponent::SetSOBlock(CompressedSOBlock* _SOBlock){
	SOBlock = _SOBlock;
}

void UEyeVectorsComponent::ResetPupilData(){
	AveragePupilSize = 0;
	TotalBlinkCount = 0;
	BlinkRate = 0;
	AverageBlinkDuration = 0;
}

void UEyeVectorsComponent::SetGlassesID(FString _GlassesID) {
	GlassesID = _GlassesID;
}

FString UEyeVectorsComponent::GetGlassesID() {
	return GlassesID;
}

void UEyeVectorsComponent::OnEyeWAMPComponentDisconnected(ComponentTypeEnum ComponentType, FString ComponentID)
{
	if (ComponentType == ComponentTypeEnum::VE_Glasses)
	{
		if (GlassesID == ComponentID)
		{
			if (GEngine)
			{
				FString msg("Glasses " + GlassesID + " has disconnected from router!");

				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, msg);
			}
		}
	}
}

void UEyeVectorsComponent::OnEyeWAMPComponentConnected(ComponentTypeEnum ComponentType, FString ComponentID)
{
	if (ComponentType == ComponentTypeEnum::VE_Glasses)
	{
		if (GlassesID == ComponentID)
		{
			if (GEngine)
			{
				FString msg("Glasses " + GlassesID + " has connected to router!");

				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, msg);
			}
		}
	}
}