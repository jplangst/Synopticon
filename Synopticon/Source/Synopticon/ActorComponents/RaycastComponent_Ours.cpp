// Fill out your copyright notice in the Description page of Project Settings.
#include "RaycastComponent_Ours.h"

// Sets default values for this component's properties
URaycastComponent_Ours::URaycastComponent_Ours()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetHitTargetID(-1);
	VectorSceneComponent = nullptr;
	OwnerName = "";

	InteractingActor = nullptr;
	HitResult = nullptr;

	bSlowdownForDebug = false;
	SlowdownThreshold = 12;
	SlowdownTimer = 0;
}

// Called when the game starts
void URaycastComponent_Ours::BeginPlay() {
	Super::BeginPlay();
	PublishEventTimer = 0;
	PublishEvent = false;
}

// Called when the game ends
void URaycastComponent_Ours::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	VectorSceneComponent = nullptr;
	SetHitTargetID(-1);
	if (HitResult)
	{
		delete HitResult;
	}
	HitResult = nullptr;
	OwnerName = "";
}

// Called every frame
void URaycastComponent_Ours::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (VectorSceneComponent && ASynOpticonState::IsRaycastingEnabled())
	{
		if (bSlowdownForDebug) {
			SlowdownTimer += DeltaTime;
			if (SlowdownTimer > SlowdownThreshold) {
				PerformRaycast();
				SlowdownTimer -= SlowdownThreshold;
			}
		}
		else {
			PerformRaycast();
		}	

		PublishEventTimer += DeltaTime;
		if (PublishEventTimer > PublishEventThreshold) {
			PublishEventTimer -= PublishEventThreshold;
			PublishEvent = true;
		}
	}
}

//Set the scene component containing the direction vector that is to be used for raycasting
void URaycastComponent_Ours::SetVectorSceneComponent(USceneComponent* _VectorSceneComponent)
{
	VectorSceneComponent = _VectorSceneComponent;
}

void URaycastComponent_Ours::SetVectorColor(FVector NewVectorColor) {
	Cast<UMaterialInstanceDynamic>(Cast<UStaticMeshComponent>(VectorSceneComponent->GetChildComponent(0))->GetMaterial(0))->SetVectorParameterValue("VectorColor", NewVectorColor);
}

void URaycastComponent_Ours::PerformRaycast()
{
	//Cone line trace
	FCollisionQueryParams RV_TraceParams = PrepareTraceParams();
	TArray<FHitObjectResult> ConeHitResults = TraceConeShape(RV_TraceParams, ASynOpticonState::GetConeAngle(), ASynOpticonState::GetConeDivisions());
	ProcessHitResults(ConeHitResults);
}

void URaycastComponent_Ours::ProcessHitResults(TArray<FHitObjectResult> &HitResults) {
	int32 BiggestHitAmount = 0;
	FHitObjectResult BestObjectResult;
	for (FHitObjectResult ObjectHitResult : HitResults) {
		if (ObjectHitResult.HitLocations.Num() > BiggestHitAmount) {
			BiggestHitAmount = ObjectHitResult.HitLocations.Num();
			BestObjectResult = ObjectHitResult;			
		}
	}

	//Update the HitResult if we have a valid hit
	if (BiggestHitAmount > 0 && BestObjectResult.FaceIndex >= 0) {
		FVector AveragedHitLocation = FVector(0,0,0);		
		for (FVector HitLocation : BestObjectResult.HitLocations) {
			AveragedHitLocation += HitLocation;
		}
		AveragedHitLocation = AveragedHitLocation / BestObjectResult.HitLocations.Num();

		double AveragedTargetDistance = 0;
		for (double TargetDistance : BestObjectResult.TargetDistances) {
			AveragedTargetDistance += TargetDistance;
		}
		AveragedTargetDistance = AveragedTargetDistance / BestObjectResult.TargetDistances.Num();

		FVector AveragedImpactNormal = FVector(0,0,0);
		for (FVector ImpactNormal : BestObjectResult.ImpactNormals) {
			AveragedImpactNormal += ImpactNormal;
		}
		AveragedImpactNormal = AveragedImpactNormal / BestObjectResult.ImpactNormals.Num();

		if (!HitResult) {
			//HitResult = new FRaycastHitResult();
			HitResult = new FHitResult();
		}
		HitResult->Actor = BestObjectResult.HitActor;
		HitResult->Location = AveragedHitLocation;
		HitResult->Distance = AveragedTargetDistance;
		HitResult->ImpactNormal = AveragedImpactNormal;
		

		AGazeActor* GazeActor = Cast<AGazeActor>(BestObjectResult.HitActor);
		TArray<FVector> TriangleVerticies;
		TArray<FVector2D> TriangleUVs;
		GazeActor->GetTriangleInformation(BestObjectResult.FaceIndex, TriangleVerticies, TriangleUVs);
		FVector2D HitUV = USynOpticonStatics::FindUVInTriangle(TriangleVerticies, TriangleUVs, AveragedHitLocation);

		
		int32 HitAOIID = BestObjectResult.HitActor->GetHitID(true, HitUV);
		SetHitTargetID(HitAOIID);
		SetInteractingActor(BestObjectResult.HitActor);

		//Create an eye event for streaming. Stream at all times
		//if (ASynOpticonState::IsReplaying() || ASynOpticonState::IsRecording()) {
			if (PublishEvent) {
				FEyeEventStruct* EyeEvent = new FEyeEventStruct();
				EyeEvent->ActorName = OwnerName;
				EyeEvent->LocationOnAOI = HitUV;
				EyeEvent->TargetAOIName = GazeActor->GetAOINameFromID(HitAOIID);
				EyeEvent->Timestamp = FDateTime::UtcNow().GetTimeOfDay().GetTotalMilliseconds();
				FWAMPWorker::PublishWAMPEvent(EyeEvent);
				PublishEvent = false;
			}			
		//}
		
		UPositionAndOrientationComponent* PosOriComponent = GetOwner()->FindComponentByClass<UPositionAndOrientationComponent>();
		if (PosOriComponent)
		{
			PosOriComponent->ScaleOrientationMesh(GetTargetDistance());
		}
	}
	else {
		delete HitResult;
		HitResult = nullptr;
		SetHitTargetID(-1);
		SetInteractingActor(nullptr);
	}
}

FCollisionQueryParams URaycastComponent_Ours::PrepareTraceParams() {
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this->GetOwner());
	RV_TraceParams.bTraceComplex = false; //Was true, but complex does not work with splines
	RV_TraceParams.bReturnPhysicalMaterial = false;
	RV_TraceParams.AddIgnoredActor(this->GetOwner()); //Don't hit the actor
												  //Find all actors that are supposed to be ignored: ground plane, laser tool, calibration points
	TArray<AActor*> GroundPlane;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("GroundPlane"), GroundPlane);
	for (AActor* actor : GroundPlane)
	{
		RV_TraceParams.AddIgnoredActor(actor);
	}

	TArray<AActor*> LaserTool;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("LaserTool"), LaserTool);
	for (AActor* actor : LaserTool)
	{
		RV_TraceParams.AddIgnoredActor(actor);
	}

	TArray<AActor*> CalibrationTool;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("CalibrationTool"), CalibrationTool);
	for (AActor* actor : CalibrationTool)
	{
		RV_TraceParams.AddIgnoredActor(actor);
	}

	TArray<AActor*> ManualTool;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("ManualTool"), ManualTool);
	for (AActor* actor : ManualTool)
	{
		RV_TraceParams.AddIgnoredActor(actor);
	}

	TArray<AActor*> CalPoints;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("CalPoints"), CalPoints);
	for (AActor* actor : CalPoints)
	{
		RV_TraceParams.AddIgnoredActor(actor);
	}

	RV_TraceParams.bReturnFaceIndex = true;

	return RV_TraceParams;
}

FHitResult* URaycastComponent_Ours::TraceLine(FCollisionQueryParams RV_TraceParams, FVector StartOfTrace,
	FVector EndOfTrace, ECollisionChannel CollisionChannel) {
	FHitResult* HitResultNew = new FHitResult(ForceInit);
	GetWorld()->LineTraceSingleByChannel(
		*HitResultNew,
		StartOfTrace,
		EndOfTrace,
		CollisionChannel,
		RV_TraceParams
	);

	//DEBUG
	if (bSlowdownForDebug) {
		DrawDebugLine(
			GetWorld(),
			StartOfTrace,
			EndOfTrace,
			FColor(255, 0, 0),
			false, 10, 0,
			SlowdownThreshold-1
		);
	}

	return HitResultNew;
}

TArray<FHitObjectResult> URaycastComponent_Ours::TraceConeShape(FCollisionQueryParams RV_TraceParams, float AngleInDegrees, int32 Divisions) {
	TArray<FHitObjectResult> HitResults;

	//Centre trace, we always trace the centre
	int32 TraceLength = 1500;
	FTransform orientationVectorTransform = VectorSceneComponent->GetComponentTransform();
	FVector StartOfTrace = orientationVectorTransform.GetLocation();
	FVector EndOfTrace = (orientationVectorTransform.GetRotation().GetForwardVector() * TraceLength) + StartOfTrace;
	FHitResult* CentreHitResult = TraceLine(RV_TraceParams, StartOfTrace, EndOfTrace, ECollisionChannel::ECC_GameTraceChannel4); 

	ProcessHitResult(HitResults, CentreHitResult);
	delete CentreHitResult;
	CentreHitResult = nullptr;

	//World 
	FVector PerimiterVector = EndOfTrace - StartOfTrace;
	PerimiterVector = PerimiterVector.GetSafeNormal();
	PerimiterVector = PerimiterVector.RotateAngleAxis(AngleInDegrees, VectorSceneComponent->GetUpVector());

	//Cone perimiter traces. Amount of traces is decided by the Divisions parameter
	if (Divisions > 0) {
		float AngleOfRotation = 360 / (Divisions * 4); //Find the angle amount per rotation based on the number of divisions
		FVector NormalizedForward = orientationVectorTransform.GetRotation().GetForwardVector().GetSafeNormal();
		for (int i = 0; i < Divisions * 4; i++) {
			FVector PerimiterDirection = PerimiterVector.RotateAngleAxis(AngleOfRotation*i, NormalizedForward);
			FVector PerimiterEndOfTrace = (PerimiterDirection * TraceLength) + StartOfTrace;
			FHitResult* PerimiterHitResult = TraceLine(RV_TraceParams, StartOfTrace, PerimiterEndOfTrace, ECollisionChannel::ECC_GameTraceChannel4);

			ProcessHitResult(HitResults, PerimiterHitResult);
			delete PerimiterHitResult;
			CentreHitResult = nullptr;
		}
	}

	return HitResults;
}

void URaycastComponent_Ours::ProcessHitResult(TArray<FHitObjectResult> &OutHitResults, FHitResult* HitResultToProcess) {
	if (HitResultToProcess->bBlockingHit)
	{
		if (HitResultToProcess->GetActor()->IsA(AGazeActor::StaticClass())) {
			AGazeActor* GazeActor = Cast<AGazeActor>(HitResultToProcess->GetActor());

			//Check if the list already contains this actor
			bool Contains = false;
			for (FHitObjectResult &ConeHitResult : OutHitResults) {
				//If it does we update the struct with the new information 
				if (GazeActor == ConeHitResult.HitActor) {
					ConeHitResult.HitLocations.Add(HitResultToProcess->ImpactPoint);
					ConeHitResult.TargetDistances.Add(HitResultToProcess->Distance);
					ConeHitResult.ImpactNormals.Add(HitResultToProcess->ImpactNormal);
					Contains = true;
				}
			}
			//Otherwise we add a new struct with the information
			if (!Contains) {
				FHitObjectResult NewHitResult;
				NewHitResult.HitActor = GazeActor;
				NewHitResult.HitLocations.Add(HitResultToProcess->ImpactPoint);
				NewHitResult.TargetDistances.Add(HitResultToProcess->Distance);
				NewHitResult.ImpactNormals.Add(HitResultToProcess->ImpactNormal);
				NewHitResult.FaceIndex = HitResultToProcess->FaceIndex;
				OutHitResults.Add(NewHitResult);
			}
		}
	}
}

double URaycastComponent_Ours::GetTargetDistance()
{
	if (HitResult) {
		return HitResult->Distance;
	}
	return -1;
}

int32 URaycastComponent_Ours::GetHitTargetID()
{
	return HitTargetID;
}

void URaycastComponent_Ours::SetHitTargetID(int32 TargetID)
{
	HitTargetID = TargetID;
}

FString URaycastComponent_Ours::GetOwnerName()
{
	return OwnerName;
}

void URaycastComponent_Ours::SetOwnerName(FString _OwnerName)
{
	OwnerName = _OwnerName;
}

void URaycastComponent_Ours::SetInteractingActor(AGazeActor* gazeActor)
{
	InteractingActor = gazeActor;
}

AGazeActor* URaycastComponent_Ours::GetInteractingActor()
{
	return InteractingActor;
}

FHitResult* URaycastComponent_Ours::GetHitResult()
{
	if (GetHitTargetID() != -1) {
		return HitResult;
	}
	return nullptr;
}