// Fill out your copyright notice in the Description page of Project Settings.

#include "SynOpticonActor.h"

// Sets default values
ASynOpticonActor::ASynOpticonActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//WebViewInteractionComponent = CreateDefaultSubobject<URadiantWebViewInteractionComponent>(TEXT("WebViewInteractionComponent"));
	CanRaycast = false;

	//Create the root scene component and set it
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("SynOpticonActorRootComponent")));

	//Create the camera boom
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->TargetArmLength = 55.0f;
	CameraArm->SocketOffset = FVector(0.0f, 0.f, 10.f);
	CameraArm->SetRelativeRotation(FRotator(-10.f, 0.f, 0.f));
	CameraArm->bDoCollisionTest = false;

	//Smooth the camera movement by introducing a small amount of lag
	CameraArm->bEnableCameraLag = true;
	CameraArm->CameraLagSpeed = 2;
	CameraArm->bEnableCameraRotationLag = true;
	CameraArm->CameraRotationLagSpeed = 2;

	// Create the actor camera and attach it to the camera boom
	OrientationCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActorOrientationCamera"));
	OrientationCamera->AttachToComponent(CameraArm, FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName);
	CurrentFixationGroup = nullptr;

	//Create gaze data visualizer, tmp for testing
	//GazeDataVis = CreateDefaultSubobject<UGazeDataVisualizerComponent>(TEXT("GazeDataViz"));
	GazeDataVis = nullptr;
	PreviousGazePoint.Set(-1, -1, -1);
}

// Called when the game starts or when spawned
void ASynOpticonActor::BeginPlay()
{
	Super::BeginPlay();	

	SOBlock = nullptr;
	PreviousHitFaceIndex = -1;
	GazeActorHeatMapValues.Empty();
}

// Called every frame
void ASynOpticonActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	//====================Record=====================
	if (ASynOpticonState::IsRecording())
	{
		RecordFrameData();
	}

	UpdateActorTasks();

	GazeActorHeatMapValues.Empty();
	FDateTime CurrentTime = 0;
	FDateTime StartTime = 0;
	if (ASynOpticonState::IsReplaying())
	{
		CurrentTime = ASynOpticonState::GetCurrentReplayTime();
		StartTime = ASynOpticonState::ReplayHeader.StartTimestamp;
	}
	else
	{
		CurrentTime = FDateTime::UtcNow();
		StartTime = ASynOpticonState::GetStartTime();
	}
	FTimespan HeatMapHistoryDuration;
	if (ASynOpticonState::IsHeatMapFromBeginning())
	{
		HeatMapHistoryDuration = CurrentTime - StartTime;
	}
	else
	{
		HeatMapHistoryDuration = FTimespan::FromMinutes(ASynOpticonState::GetHeatMapHistoryDuration()); 

		if (CurrentTime - HeatMapHistoryDuration < StartTime)
		{
			HeatMapHistoryDuration = CurrentTime - StartTime;
		}
	}

	USynOpticonAggLogComponent* LogComponent = FindComponentByClass<USynOpticonAggLogComponent>();
	if (LogComponent)
	{
		FDateTime HistoryThreshold = CurrentTime - HeatMapHistoryDuration;
		for (FGazePathLoggingInfoStruct GazeEvent : LogComponent->GetLoggedData(LoggingTypeEnum::LT_GazeData).GazePaths)
		{
			FDateTime EventStart = GazeEvent.TimeStamp;
			FDateTime EventEnd = EventStart + FTimespan::FromSeconds(GazeEvent.Duration);
			FTimespan Duration = FTimespan::FromSeconds(GazeEvent.Duration);

			//The EventEnd has to be bigger than the HeatMap History Threshold
			if (!(EventEnd <= HistoryThreshold || EventStart >= CurrentTime))
			{
				float HeatMapValue = 0.0f;
				FTimespan DurationDeduction = 0;

				//Reduce because event outside current time
				if (EventStart > HistoryThreshold && EventEnd > CurrentTime)
				{
					DurationDeduction += EventEnd - CurrentTime;
				}

				//Reduce because event starts before threshold but ends after
				if (EventStart < HistoryThreshold && EventEnd > HistoryThreshold)
				{
					DurationDeduction += HistoryThreshold - EventStart;

					//Reduce because event outside current time
					if (EventEnd > CurrentTime)
					{
						DurationDeduction += EventEnd - CurrentTime;
					}
				}

				HeatMapValue = Duration.GetTotalSeconds() - DurationDeduction.GetTotalSeconds();

				//Normalise to a value between 0 and 1
				HeatMapValue = HeatMapValue / HeatMapHistoryDuration.GetTotalSeconds();

				//Add the GazeActor and it's heatmap value to the map
				if (GazeActorHeatMapValues.Contains(GazeEvent.GazeActorID))
				{
					float OldValue = GazeActorHeatMapValues.FindAndRemoveChecked(GazeEvent.GazeActorID);
					HeatMapValue += OldValue;
				}

				GazeActorHeatMapValues.Add(GazeEvent.GazeActorID, HeatMapValue);
			}
			else
			{
				if (!GazeActorHeatMapValues.Contains(GazeEvent.GazeActorID))
				{
					GazeActorHeatMapValues.Add(GazeEvent.GazeActorID, 0.0f);
				}
			}
		}
	}

	if (!ASynOpticonState::IsReplaying())
	{
		//CalculateFixationsFromEyeVector(DeltaTime);
		CalculateFixationsRegardless(DeltaTime);
	}
	else
	{
		//CalculateFixations(DeltaTime);
		CalculateReplayFixation(ASynOpticonState::GetCurrentReplayTime());
	}
}

void ASynOpticonActor::SetGazeDataVisualizer(UGazeDataVisualizerComponent* _GazeDataVis) {
	GazeDataVis = _GazeDataVis;
}

void ASynOpticonActor::UpdateActorTasks() { 
	//If we are not replaying we need to update the tasks in real-time
	if (!ASynOpticonState::IsReplaying()) 
	{	
		AGazeActor* Target = nullptr;
		int32 GazeObjectID = -1;

		URaycastComponent_Ours* RaycastComponent = FindComponentByClass<URaycastComponent_Ours>();
		if (RaycastComponent)
		{
			if (RaycastComponent->GetHitResult())
			{
				FHitResult CurrentHitResult = *RaycastComponent->GetHitResult();
				GazeObjectID = RaycastComponent->GetHitTargetID();

				if(CurrentHitResult.GetActor())
				Target = Cast<AGazeActor>(CurrentHitResult.GetActor());
			}
		}
		else {
			URemoteEyeTrackerComponent* RemoteComponent = FindComponentByClass<URemoteEyeTrackerComponent>();
			if (RemoteComponent)
			{
				GazeObjectID = RemoteComponent->GetHitTargetID();
				for (AActor* Actor : ASynOpticonState::GetGazeActorList()) {
					AGazeActor* GA = Cast<AGazeActor>(Actor);
					if (GA && (GA->GetID() == GazeObjectID || GA->ContainsAOIID(false, GazeObjectID) 
						|| GA->ContainsAOIID(true, GazeObjectID))) {
						Target = GA;
					}
				}
			}
		}

		if (Target) {
			FString HitAOI = "";
			//First we extract the name of the gaze actor or area of interest that we hit
			if (GazeObjectID == Target->GetID()) {
				HitAOI = Target->GetName();
				if (Target->GetMicroTaskStatus() == EMicroTaskStatus::STARTED) {
					Target->SetMicroTaskStatus(EMicroTaskStatus::COMPLETE);
				}
			}
			else if (Target->ContainsAOIID(false, GazeObjectID)) {
				Target->MarkMicroTaskAsComplete(false, GazeObjectID);
				HitAOI = Target->GetAOIName(false, GazeObjectID);
			}
			else if (Target->ContainsAOIID(true, GazeObjectID)) {
				Target->MarkMicroTaskAsComplete(true, GazeObjectID);
				HitAOI = Target->GetAOIName(true, GazeObjectID);
			}

			//If we have a name we proceed
			if (!HitAOI.Equals("")) {
				//Iterate over all the actors tasks
				for (int i = 0; i < ActorTasks.Num(); i++) {
					if (ActorTasks[i].Complete) {
						continue;
					}

					//Iterate over all the SubTasks of the Task
					bool AllSubTasksComplete = true;
					//Check if we have a match and if all sub tasks have been completed
					for (int y = 0; y < ActorTasks[i].SubTaskList.Num(); y++) {
						//If the name matches we mark the task as complete
						if (ActorTasks[i].SubTaskList[y].AOIName.Equals(HitAOI)) {
							ActorTasks[i].SubTaskList[y].Complete = true;

							//If we are recording we need to add the subtask complete event to the micro tasks events in the replay header
							if (ASynOpticonState::IsRecording()) {
								ASynOpticonState::GetGlobalEventSystem()->OnMicroTaskEventDelegate.Broadcast(MicroTaskEventTypeEnum::SubtaskComplete, ActorTasks[i]);
							}
						}
						else if (!ActorTasks[i].SubTaskList[y].Complete) {
							AllSubTasksComplete = false;
						}
					}

					//Mark the task as complete and update the end timestamp
					if (AllSubTasksComplete) {
						ActorTasks[i].Complete = true;
						ActorTasks[i].EndTimestamp = FDateTime::UtcNow();

						//If we are recording we need to add the task complete event to the micro tasks events in the replay header
						if (ASynOpticonState::IsRecording()) {
							ASynOpticonState::GetGlobalEventSystem()->OnMicroTaskEventDelegate.Broadcast(MicroTaskEventTypeEnum::TaskComplete, ActorTasks[i]);
						}
					}
				}
			}
		}
	}
}

void ASynOpticonActor::CalculateFixations(float DeltaTime)
{
	//TODO: smooth hit points before calculate fixations and saccades to remove micro saccades and blink
	FHitResult CurrentHitResult;
	FVector CurrentGazePoint;
	FVector TempPrevPoint;
	int32 GazeObjectID = -1;
	bool isRaycastValid = false;
	bool isRemoteValid = false;
	URaycastComponent_Ours* RaycastComponent = FindComponentByClass<URaycastComponent_Ours>();
	if (RaycastComponent)
	{
		if (RaycastComponent->GetHitResult())
		{
			CurrentHitResult = *RaycastComponent->GetHitResult();
			CurrentGazePoint = CurrentHitResult.Location;
			GazeObjectID = RaycastComponent->GetHitTargetID();
			TempPrevPoint = PreviousHitResult.Location;
			isRaycastValid = true;
			PreviousHitFaceIndex = CurrentHitResult.FaceIndex;
			PreviousHitResult = CurrentHitResult;		
		}
	}

	if (!isRaycastValid)
	{
		URemoteEyeTrackerComponent* RemoteComponent = FindComponentByClass<URemoteEyeTrackerComponent>();
		if (RemoteComponent)
		{
			GazeObjectID = RemoteComponent->GetHitTargetID();
			CurrentGazePoint = RemoteComponent->GetWorldGazePoint();
			if (CurrentGazePoint != FVector(-42, -42, -42))
			{
				TempPrevPoint = PreviousGazePoint;
				PreviousGazePoint = CurrentGazePoint;
				isRemoteValid = true;
			}
		}
	}

	if ((isRaycastValid || isRemoteValid) && GazeObjectID != -1)
	{
		double velocity = FVector::Dist(CurrentGazePoint, TempPrevPoint) / DeltaTime;
		if (velocity < ASynOpticonState::FIXATION_VELOCITY_THRESHOLD) //fixation
		{
			if (!CurrentFixationGroup)
			{
				CurrentFixationGroup = new UFixationGroup(GazeObjectID);
				//CurrentFixationGroup->TimeOfFirstPoint = FDateTime::UtcNow();
			}
			FFixationPoint p;
			p.Location = CurrentGazePoint;
			p.Duration = DeltaTime;
			if (FVector::Dist(p.Location, CurrentFixationGroup->GetCentroid()) > ASynOpticonState::FIXATION_DISTANCE_THRESHOLD)
			{
				if (CurrentFixationGroup->GetDuration() >= ASynOpticonState::FIXATION_DURATION_THRESHOLD && GazeDataVis)
				{
					FVector2D HitUV(-1, -1);
					for (AActor* Actor : ASynOpticonState::GetGazeActorList()) {
						AGazeActor * GazeActor = Cast<AGazeActor>(Actor);
						if (GazeActor && GazeActor->GetID() == CurrentFixationGroup->GazeActorId) {
							HitUV = HandleEyeRadarData(GazeActor, CurrentFixationGroup->GetCentroid(), CurrentHitResult.FaceIndex, CurrentFixationGroup->GetDuration());
						}
						else if (GazeActor->ContainsAOIID(true, CurrentFixationGroup->GazeActorId) || GazeActor->ContainsAOIID(false, CurrentFixationGroup->GazeActorId)) {
							HitUV = GazeActor->FindUVFromMeshBP(CurrentFixationGroup->GetCentroid(), CurrentHitResult.FaceIndex);
						}
					}

					GazeDataVis->AddFixationGroup(CurrentFixationGroup->GazeActorId, CurrentFixationGroup->GetCentroid(), CurrentFixationGroup->GetDuration(), CurrentFixationGroup->TimeOfFirstPoint, CurrentHitResult.ImpactNormal, HitUV);
				}
				else
				{
					delete CurrentFixationGroup;
				}
				CurrentFixationGroup = new UFixationGroup(GazeObjectID);
			}
			CurrentFixationGroup->Points.Add(p);
		}
		else if (velocity > ASynOpticonState::SACCADE_VELOCITY_THRESHOLD) //saccade
		{
			if (CurrentFixationGroup)
			{
				if (CurrentFixationGroup->GetDuration() >= ASynOpticonState::FIXATION_DURATION_THRESHOLD && GazeDataVis) //average fixation is 100-600ms, discard noisy fixations
				{
					FVector2D HitUV(-1, -1);
					for (AActor* Actor : ASynOpticonState::GetGazeActorList()) {
						AGazeActor * GazeActor = Cast<AGazeActor>(Actor);
						if (GazeActor && GazeActor->GetID() == CurrentFixationGroup->GazeActorId) {
							HitUV = HandleEyeRadarData(GazeActor, CurrentFixationGroup->GetCentroid(), CurrentHitResult.FaceIndex, CurrentFixationGroup->GetDuration());
						}
						else if (GazeActor->ContainsAOIID(true, CurrentFixationGroup->GazeActorId) || GazeActor->ContainsAOIID(false, CurrentFixationGroup->GazeActorId)) {
							HitUV = GazeActor->FindUVFromMeshBP(CurrentFixationGroup->GetCentroid(), CurrentHitResult.FaceIndex);
						}
					}

					GazeDataVis->AddFixationGroup(CurrentFixationGroup->GazeActorId, CurrentFixationGroup->GetCentroid(), CurrentFixationGroup->GetDuration(), CurrentFixationGroup->TimeOfFirstPoint, CurrentHitResult.ImpactNormal, HitUV);
				}
				else
				{
					delete CurrentFixationGroup;
				}
				CurrentFixationGroup = nullptr;
			}
		}
	}	
}

void ASynOpticonActor::HandleEyeRadarDataFromUV(AGazeActor* GazeActor, FVector2D UV, float Duration) {
	//Map the square UV coordinate to a unit circle
	FVector2D EyeRadarUV = USynOpticonStatics::MapFromUnitSquareToUnitCircle(UV);

	//Add the coordinate to the EyeRadar lines container in the hit GazeActor
	GazeActor->AddEyeRadarLine(EyeRadarUV, Duration);
}

FVector2D ASynOpticonActor::HandleEyeRadarData(AGazeActor* GazeActor, FVector WorldLocation, int32 FaceIndex, float Duration) {
	//Find the UV cordinate of the hit
	FVector2D EyeRadarUV = GazeActor->FindUVFromMeshBP(WorldLocation, FaceIndex);

	HandleEyeRadarDataFromUV(GazeActor, EyeRadarUV, Duration);

	return EyeRadarUV;
}

void ASynOpticonActor::CalculateFixationsRegardless(float DeltaTime)
{
	FHitResult CurrentHitResult;
	FVector CurrentGazePoint;
	FVector TempPrevPoint;
	int32 GazeObjectID = -1;
	
	bool isRaycastValid = false;
	bool isRemoteValid = false;
	URaycastComponent_Ours* RaycastComponent = FindComponentByClass<URaycastComponent_Ours>();
	if (RaycastComponent)
	{
		if (RaycastComponent->GetHitResult())
		{
			CurrentHitResult = *RaycastComponent->GetHitResult();
			CurrentGazePoint = CurrentHitResult.Location;
			GazeObjectID = RaycastComponent->GetHitTargetID();
			TempPrevPoint = PreviousHitResult.Location;
			isRaycastValid = true;
			PreviousHitFaceIndex = CurrentHitResult.FaceIndex;
			PreviousHitResult = CurrentHitResult;
		}
	}

	if (!isRaycastValid)
	{
		URemoteEyeTrackerComponent* RemoteComponent = FindComponentByClass<URemoteEyeTrackerComponent>();
		if (RemoteComponent)
		{
			GazeObjectID = RemoteComponent->GetHitTargetID();
			CurrentGazePoint = RemoteComponent->GetWorldGazePoint();
			if (CurrentGazePoint != FVector(-42, -42, -42))
			{
				TempPrevPoint = PreviousGazePoint;
				PreviousGazePoint = CurrentGazePoint;
				isRemoteValid = true;
			}
		}
	}

	if ((isRaycastValid || isRemoteValid) && GazeObjectID != -1)
	{
		double velocity = FVector::Dist(CurrentGazePoint, TempPrevPoint) / DeltaTime;
		if (velocity < ASynOpticonState::FIXATION_VELOCITY_THRESHOLD) //fixation
		{
			if (!CurrentFixationGroup)
			{
				CurrentFixationGroup = new UFixationGroup(GazeObjectID);
				//CurrentFixationGroup->TimeOfFirstPoint = FDateTime::UtcNow();
			}
			FFixationPoint p;
			p.Location = CurrentGazePoint;
			p.Duration = DeltaTime;
			if (FVector::Dist(p.Location, CurrentFixationGroup->GetCentroid()) > ASynOpticonState::FIXATION_DISTANCE_THRESHOLD)
			{
				if (CurrentFixationGroup->GetDuration() >= ASynOpticonState::FIXATION_DURATION_THRESHOLD && GazeDataVis)
				{
					FVector2D HitUV(-1, -1);
					for (AActor* Actor : ASynOpticonState::GetGazeActorList()) {
						AGazeActor * GazeActor = Cast<AGazeActor>(Actor);
						if (GazeActor && GazeActor->GetID() == CurrentFixationGroup->GazeActorId) {
							HitUV = HandleEyeRadarData(GazeActor, CurrentFixationGroup->GetCentroid(), CurrentHitResult.FaceIndex, CurrentFixationGroup->GetDuration());
						}
						else if (GazeActor->ContainsAOIID(true, CurrentFixationGroup->GazeActorId) || GazeActor->ContainsAOIID(false, CurrentFixationGroup->GazeActorId)) {
							HitUV = GazeActor->FindUVFromMeshBP(CurrentFixationGroup->GetCentroid(), CurrentHitResult.FaceIndex);
						}
					}
					GazeDataVis->AddFixationGroup(CurrentFixationGroup->GazeActorId, CurrentFixationGroup->GetCentroid(), CurrentFixationGroup->GetDuration(), CurrentFixationGroup->TimeOfFirstPoint, CurrentHitResult.ImpactNormal, HitUV);
				}
				else
				{
					delete CurrentFixationGroup;
				}
				CurrentFixationGroup = new UFixationGroup(GazeObjectID);
			}
			CurrentFixationGroup->Points.Add(p);
		}
		else if (velocity > ASynOpticonState::SACCADE_VELOCITY_THRESHOLD) //saccade
		{
			if (CurrentFixationGroup)
			{
				if (CurrentFixationGroup->GetDuration() >= ASynOpticonState::FIXATION_DURATION_THRESHOLD && GazeDataVis) //average fixation is 100-600ms, discard noisy fixations
				{
					FVector2D HitUV(-1, -1);
					for (AActor* Actor : ASynOpticonState::GetGazeActorList()) {
						AGazeActor * GazeActor = Cast<AGazeActor>(Actor);
						if (GazeActor && GazeActor->GetID() == CurrentFixationGroup->GazeActorId) {
							HitUV = HandleEyeRadarData(GazeActor, CurrentFixationGroup->GetCentroid(), CurrentHitResult.FaceIndex, CurrentFixationGroup->GetDuration());
						}
						else if (GazeActor->ContainsAOIID(true, CurrentFixationGroup->GazeActorId) || GazeActor->ContainsAOIID(false, CurrentFixationGroup->GazeActorId)) {
							HitUV = GazeActor->FindUVFromMeshBP(CurrentFixationGroup->GetCentroid(), CurrentHitResult.FaceIndex);
						}
					}
					GazeDataVis->AddFixationGroup(CurrentFixationGroup->GazeActorId, CurrentFixationGroup->GetCentroid(), CurrentFixationGroup->GetDuration(), CurrentFixationGroup->TimeOfFirstPoint, CurrentHitResult.ImpactNormal, HitUV);

				}
				else
				{
					delete CurrentFixationGroup;
				}
				CurrentFixationGroup = nullptr;
			}
		}
	}
	else {
		CalculateFixationsFromEyeVector(DeltaTime);
	}
}

void ASynOpticonActor::CalculateFixationsFromEyeVector(float DeltaTime)
{
	const float ANGULAR_FIXATION_VELOCITY_THRESHOLD = 1.74533; //100 degrees or 1.74533 radians
	const float ANGULAR_SACCADE_VELOCITY_THRESHOLD = 5.23599; //300 degrees or 5.23599 radians
	//const int ANGULAR_DISTANCE_THRESHOLD = 
	FVector CurrentGazeVector;
	FVector TempPrevVector;
	int32 GazeObjectID = -1;
	bool isRaycastValid = false;
	bool isRemoteValid = false;
	UEyeVectorsComponent* EyeVectorsComponent = FindComponentByClass<UEyeVectorsComponent>();
	if (EyeVectorsComponent)
	{
		USceneComponent* ConvergenceVector = EyeVectorsComponent->GetConvergenceVectorSceneComponent();
		CurrentGazeVector = ConvergenceVector->GetForwardVector();
		TempPrevVector = PreviousGazePoint;
		PreviousGazePoint = CurrentGazeVector;
		isRaycastValid = true;
	}

	if (isRaycastValid)
	{
		CurrentGazeVector.Normalize();
		TempPrevVector.Normalize();
		
		double velocity = FGenericPlatformMath::Acos(FVector::DotProduct(CurrentGazeVector, TempPrevVector)) / DeltaTime;
		if (velocity < ANGULAR_FIXATION_VELOCITY_THRESHOLD) //fixation
		{
			if (!CurrentFixationGroup)
			{
				CurrentFixationGroup = new UFixationGroup(GazeObjectID);
				//CurrentFixationGroup->TimeOfFirstPoint = FDateTime::UtcNow();
			}
			FFixationPoint p;
			p.Location = CurrentGazeVector*100;
			p.Duration = DeltaTime;
			//if (FVector::Dist(p.Location, CurrentFixationGroup->GetCentroid()) > ASynOpticonState::FIXATION_DISTANCE_THRESHOLD)
			//{
			//	if (CurrentFixationGroup->GetDuration() >= ASynOpticonState::FIXATION_DURATION_THRESHOLD && GazeDataVis)
			//	{
			//		GazeDataVis->AddFixationGroup(CurrentFixationGroup->GazeActorId, CurrentFixationGroup->GetCentroid(), CurrentFixationGroup->GetDuration(), CurrentFixationGroup->TimeOfFirstPoint, CurrentGazeVector);
			//	}
			//	else
			//	{
			//		delete CurrentFixationGroup;
			//	}
			//	CurrentFixationGroup = new UFixationGroup(GazeObjectID);
			//	//CurrentFixationGroup->TimeOfFirstPoint = ASynOpticonState::GetCurrentReplayTime();
			//}
			CurrentFixationGroup->Points.Add(p);

		}
		else if (velocity > ANGULAR_SACCADE_VELOCITY_THRESHOLD) //saccade
		{
			if (CurrentFixationGroup)
			{
				UWorld* World = GetWorld();

				if (CurrentFixationGroup->GetDuration() >= ASynOpticonState::FIXATION_DURATION_THRESHOLD && GazeDataVis) //average fixation is 100-600ms, discard noisy fixations
				{
					GazeDataVis->AddFixationGroup(CurrentFixationGroup->GazeActorId, CurrentFixationGroup->GetCentroid(), CurrentFixationGroup->GetDuration(), CurrentFixationGroup->TimeOfFirstPoint, CurrentGazeVector, FVector2D(-1, -1));
				}
				else
				{
					delete CurrentFixationGroup;
				}

				CurrentFixationGroup = nullptr;
			}
		}
	}
}

void ASynOpticonActor::CalculateReplayFixation(FDateTime CurrentTime)
{
	if (GazeDataVis)
	{
		AGazeActor* GazeActor = nullptr;
		FEyeRadarPointData EyeRadarData = GazeDataVis->Replay(CurrentTime, GazeActor);
		if (EyeRadarData.GazePoint.X != -1 && EyeRadarData.GazePoint.Y != -1 && GazeActor) {
			GazeActor->AddEyeRadarLineStruct(EyeRadarData);
		}	
	}
}

// Called when the game ends
void ASynOpticonActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	SOBlock = nullptr;

	GazeActorHeatMapValues.Empty();

	//PlayerController->UnPossess();
	Super::EndPlay(EndPlayReason);
}

void ASynOpticonActor::SetCanRaycast(bool _CanRaycast)
{
	CanRaycast = _CanRaycast;
}

void ASynOpticonActor::SetActorName(FString _ActorName)
{
	ActorName = _ActorName;
}

void ASynOpticonActor::ResetCalibration()
{
	UEyeVectorsComponent* EyeVectorsComponent = FindComponentByClass<UEyeVectorsComponent>();
	UPositionAndOrientationComponent* PosComponent = FindComponentByClass<UPositionAndOrientationComponent>();
	if (EyeVectorsComponent)
	{
		EyeVectorsComponent->ResetCalibration();
	}
	else if (PosComponent)
	{
		PosComponent->ResetCalibration();
	}
}

void ASynOpticonActor::ResetPositionAndOrientation()
{
	UPositionAndOrientationComponent* PosComponent = FindComponentByClass<UPositionAndOrientationComponent>();
	if (PosComponent)
	{
		PosComponent->ResetMarkerIDs();
	}	
}

void ASynOpticonActor::CalibrateToPoint(FVector CalPoint,int CalNum)
{
	UEyeVectorsComponent* EyeVectorsComponent = FindComponentByClass<UEyeVectorsComponent>();
	UPositionAndOrientationComponent* PosComponent = FindComponentByClass<UPositionAndOrientationComponent>();
	if (EyeVectorsComponent)
	{
		EyeVectorsComponent->CalibrateEyeVectors(CalPoint, CalNum);
	}
	else if (PosComponent)
	{
		PosComponent->CalibratePositionVector(CalPoint, CalNum);
	}
}

void ASynOpticonActor::SetCompressedSOBlock(CompressedSOBlock* _SOBlock)
{
	SOBlock = _SOBlock;

	UPositionAndOrientationComponent* PosOriComponent = FindComponentByClass<UPositionAndOrientationComponent>();
	if (PosOriComponent)
	{
		PosOriComponent->SetSOBlock(SOBlock);
	}

	UEyeVectorsComponent* EyeComponent = FindComponentByClass<UEyeVectorsComponent>();
	if (EyeComponent)
	{
		SOBlock->HasETGData = true;
		EyeComponent->SetSOBlock(SOBlock);
	}

	URemoteEyeTrackerComponent* RETComponent = FindComponentByClass<URemoteEyeTrackerComponent>();
	if (RETComponent)
	{
		SOBlock->hasRTData = true;
		RETComponent->SetSOBlock(SOBlock);
	}

	UOpenFaceComponent* VisionComponent = FindComponentByClass<UOpenFaceComponent>();
	if (VisionComponent)
	{
		SOBlock->HasETGData = true;
		VisionComponent->SetSOBlock(SOBlock);
	}

	TArray<UHandComponent*> Components;
	this->GetComponents(Components);
	//TArray<UActorComponent*> Components = this->GetComponentsByClass(UHandComponent::StaticClass());
	for (UActorComponent* ActorComp : Components) {
		HandDataBlock* HandData = new HandDataBlock();
		SOBlock->HandDataBlocks.Add(HandData);
		UHandComponent* HandComp = Cast<UHandComponent>(ActorComp);
		HandComp->SetHandDataBlock(HandData);
	}
	SOBlock->NmbHandComponents = Components.Num();
}

void ASynOpticonActor::RecordFrameData()
{
	if (SOBlock)
	{
		TArray<UPositionAndOrientationComponent*> PosComps;
		this->GetComponents(PosComps);
		//TArray<UActorComponent*> PosComps = this->GetComponentsByClass(UPositionAndOrientationComponent::StaticClass());
		for (UActorComponent* ActorComp : PosComps) {
			UPositionAndOrientationComponent* PosOriComponent = Cast<UPositionAndOrientationComponent>(ActorComp);
			if (PosOriComponent && !PosOriComponent->GetName().Contains("hand")) {
				PosOriComponent->ToBinaryReplayData();
				break;
			}
		}

		UEyeVectorsComponent* EyeVectorsComponent = FindComponentByClass<UEyeVectorsComponent>();
		if (EyeVectorsComponent)
		{
			EyeVectorsComponent->ToBinaryReplayData();
		}

		URemoteEyeTrackerComponent* RETComponent = FindComponentByClass<URemoteEyeTrackerComponent>();
		if (RETComponent)
		{
			RETComponent->ToBinaryReplayData();
		}

		UOpenFaceComponent* VisionComponent = FindComponentByClass<UOpenFaceComponent>();
		if (VisionComponent)
		{
			VisionComponent->ToBinaryReplayData();
		}

		//FString MyoGesture;
		//UMyoInputComponent* MyoInputComponent = FindComponentByClass<UMyoInputComponent>();
		//if (MyoInputComponent)
		//{
		//	//MyoGesture = FString(MyoInputComponent->GetCurrentPose().toString().c_str());
		//}

		TArray<UHandComponent*> HandComps;
		this->GetComponents(HandComps);
		//TArray<UActorComponent*> HandComps = this->GetComponentsByClass(UHandComponent::StaticClass());
		for (UActorComponent* ActorComp : HandComps) {
			UHandComponent* HandComponent = Cast<UHandComponent>(ActorComp);
			HandComponent->ToBinaryReplayData();
		}

		SOBlock->MyoGesture = -1;
	}
}

void ASynOpticonActor::SetupCameraAttachement(USceneComponent* CameraParent)
{
	if (CameraParent) { //Glasses and Position Only
		CameraArm->AttachToComponent(CameraParent, FAttachmentTransformRules::KeepRelativeTransform);
	}
	else{ //Currently only the Remote Tracker
		CameraArm->TargetArmLength = 100.0f;
		CameraArm->SetRelativeLocation(FVector(0.0f, 0.f, 20.f));
		CameraArm->SetRelativeRotation(FRotator(45.f, 180.f, 0.f));
		CameraArm->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);	
	}
}

FSynOpticonActorStruct ASynOpticonActor::GetDataStruct()
{
	FString NatNetName = "";

	TArray<UPositionAndOrientationComponent*> PosComps;
	this->GetComponents(PosComps);
	//TArray<UActorComponent*> PosComps = this->GetComponentsByClass(UPositionAndOrientationComponent::StaticClass());
	for (UActorComponent* ActorComp : PosComps) {
		UPositionAndOrientationComponent* PosOriComponent = Cast<UPositionAndOrientationComponent>(ActorComp);
		if (PosOriComponent && !PosOriComponent->GetName().Contains("hand")) {
			NatNetName = PosOriComponent->GetRigidBodyName();
			break;
		}
	}

	//Create the EyeTracking Component Node
	FEyeTrackerStruct EyeTrackerData;
	UEyeVectorsComponent* EyeVectorsComponent = this->FindComponentByClass<UEyeVectorsComponent>();
	if (EyeVectorsComponent)
	{
		EyeTrackerData = EyeVectorsComponent->GetETData();
	}

	//Create the EyeTracking Component Node
	FRemoteEyeTrackerStruct RemoteEyeTrackerData;
	URemoteEyeTrackerComponent* RETComponent = this->FindComponentByClass<URemoteEyeTrackerComponent>();
	if (RETComponent)
	{
		RemoteEyeTrackerData = RETComponent->GetRETData();
	}

	FOpenFaceStruct VisionTrackerData;
	UOpenFaceComponent* VisionComponent = this->FindComponentByClass<UOpenFaceComponent>();
	if (VisionComponent)
	{
		VisionTrackerData = VisionComponent->GetOpenFaceStructData();
	}

	//Create the MyoInput Component Node
	//UMyoInputComponent* MyoComponent = this->FindComponentByClass<UMyoInputComponent>();
	bool HasMyo = false;

	//Create the Shimmer Components Group Node
	TArray<UShimmerVRPNComponent*> ShimmerComponents;
	this->GetComponents(ShimmerComponents);
	TArray<FShimmerComponentStruct> ShimmerList;
	if (ShimmerComponents.Num() > 0)
	{
		for (UShimmerVRPNComponent* ShimmerComponent : ShimmerComponents)
		{
			FShimmerComponentStruct ShimmerComponentStruct;

			ShimmerComponentStruct.DeviceName = ShimmerComponentStruct.DeviceName;
			ShimmerComponentStruct.HasHeartRate = ShimmerComponent->HasHR();
			ShimmerComponentStruct.HasGalvanicSkinResponse = ShimmerComponent->HasGSR();
			ShimmerComponentStruct.ShimmerIndex = ShimmerComponent->GetShimmerIndex();
			ShimmerList.Add(ShimmerComponentStruct);
		}
	}

	TArray<FHandStruct> HandComponentsData;
	TArray<UHandComponent*> HandComps;
	this->GetComponents(HandComps);
	//TArray<UActorComponent*> HandComps = this->GetComponentsByClass(UHandComponent::StaticClass());
	for (UActorComponent* ActorComp : HandComps) {
		UHandComponent* HandComponent = Cast<UHandComponent>(ActorComp);
		FHandStruct HandData = HandComponent->GetComponentData();
		HandComponentsData.Add(HandData);
	}

	FSynOpticonActorStruct SynOpticonData(GetActorName(), NatNetName, EyeTrackerData,
		RemoteEyeTrackerData, ShimmerList, HandComponentsData, VisionTrackerData);

	USynOpticonAggLogComponent* LogComponent = this->FindComponentByClass<USynOpticonAggLogComponent>();
	if (LogComponent)
	{
		SynOpticonData.GazeLogComponentData = LogComponent->GetLoggedData(LoggingTypeEnum::LT_GazeData);
		SynOpticonData.LeftHandLogComponentData = LogComponent->GetLoggedData(LoggingTypeEnum::LT_LeftHandData);
		SynOpticonData.RightHandLogComponentData = LogComponent->GetLoggedData(LoggingTypeEnum::LT_RightHandData);
	}
	UGazeDataVisualizerComponent* GazeDataComponent = this->FindComponentByClass<UGazeDataVisualizerComponent>();
	if (GazeDataComponent)
	{
		SynOpticonData.GazeData = GazeDataComponent->GetGazeData();
	}

	return SynOpticonData;
}

FVector UFixationGroup::GetCentroid()
{
	FVector centroid(0,0,0); //calculate centre of mass using barycentric coordinate system
	for (FFixationPoint p : Points)
	{
		//centroid += p.Location*p.Duration;
		centroid += p.Location;
	}
	return centroid/(Points.Num());
}

float UFixationGroup::GetDuration()
{
	float Duration = 0;
	for (FFixationPoint p : Points)
	{
		Duration += p.Duration;
	}
	return Duration;
}