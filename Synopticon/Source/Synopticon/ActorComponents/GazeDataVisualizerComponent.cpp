// Fill out your copyright notice in the Description page of Project Settings.

#include "GazeDataVisualizerComponent.h"

// Sets default values for this component's properties
UGazeDataVisualizerComponent::UGazeDataVisualizerComponent(){
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	MaxFixationVisualisations = 10;

	ParticleColors = FVector(1, 1, 1);
}

// Called when the game starts
void UGazeDataVisualizerComponent::BeginPlay()
{
	Super::BeginPlay();
	ASynOpticonState::GetGlobalEventSystem()->OnSeekingEvent.AddDynamic(this, &UGazeDataVisualizerComponent::DoSeeking);
	ASynOpticonState::GetGlobalEventSystem()->OnEventRemoveGazeActor.AddDynamic(this, &UGazeDataVisualizerComponent::RemoveGazeActorCallback);
	ASynOpticonState::GetGlobalEventSystem()->OnEventStartRecord.AddDynamic(this, &UGazeDataVisualizerComponent::OnStartRecording);
	// ...
	GazeActorFixations.Empty();
	SpawnedSystemsID = 0;
	NmbFixationVisualisations = 0;
	MaxFixationVisualisations = 10;
	PrevFixationLocation.Set(0, 0, 0);
	FixationCount = 0;
	isFirstTime = true;
}

void UGazeDataVisualizerComponent::SetOwnerID(int32 _OwnerID) {
	OwnerID = _OwnerID;
}

void UGazeDataVisualizerComponent::AddFixationGroup(int32 GazeActorID, FVector Location, float Duration, FDateTime Timestamp, FVector Normal, FVector2D HitUV)
{
	if (GazeActorID == -999) {
		GazeActorFixations.Add(FFixationGroupStruct(-1, "", false, Location, Duration, Timestamp, Normal, HitUV));
	}
	else if (ASynOpticonState::AOISessionDict.Contains(GazeActorID))
	{
		TPair<FString, bool> Out = ASynOpticonState::AOISessionDict[GazeActorID];
		GazeActorFixations.Add(FFixationGroupStruct(GazeActorID, Out.Key, Out.Value, Location, Duration, Timestamp, Normal, HitUV));
	}
	else
	{
		GazeActorFixations.Add(FFixationGroupStruct(GazeActorID, "", false, Location, Duration, Timestamp, Normal, HitUV));
	}
	
	AddFixationData(GazeActorID, Location, Duration, Normal);
}

void UGazeDataVisualizerComponent::AddFixationData(int32 GazeActorID, FVector Location, float Duration, FVector Normal)
{
	CheckIfMaxFixations();

	UParticleSystemComponent* FixationParticleComponent = nullptr;
	if (FixationTemplate) {
		FixationParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(GetOwner()->GetWorld(), FixationTemplate, Location, FRotator::ZeroRotator, false);

		if (FixationParticleComponent) {
			SetupDynamicMaterial(FixationParticleComponent, true, MaxFixationVisualisations, NmbFixationVisualisations);

			FixationParticleComponent->bAutoActivate = false;
			FixationParticleComponent->Deactivate();
			FixationParticleComponent->SetWorldLocation(Location);
			FixationParticleComponent->SetTranslucentSortPriority(2);
			FixationParticleSystems.Enqueue(FixationParticleComponent);
		}
	}

	FVector ForwardVector = Normal;

	//Find the gaze actor corresponding to the id
	AGazeActor* AttachedScreen = nullptr;
	for (AActor* ItrActor : ASynOpticonState::GetGazeActorList())
	//for (TObjectIterator<UObject> Itr; Itr; ++Itr)
	{
		//if (Itr->IsA(AGazeActor::StaticClass())) {
		AGazeActor* GazeActor = Cast<AGazeActor>(ItrActor);
		if (GazeActor)
		{
			if (GazeActor->GetID() == GazeActorID || GazeActor->ContainsAOIID(false, GazeActorID) || GazeActor->ContainsAOIID(true, GazeActorID)) {
				AttachedScreen = GazeActor;
				break;
			}
		}
	}

	//If the gaze actor was found we orient the particle system so it lies on the surface of the screen
	if (AttachedScreen && AttachedScreen->GetGazeActorType() != EStaticRoomActorType::SRT_ActionObject){
		if (FixationParticleComponent){
			UProceduralMeshComponent* ProcMesh = AttachedScreen->GetProceduralMeshComponent();
			if (ProcMesh && ProcMesh->GetNumSections() > 0 ) {
				ForwardVector = ProcMesh->GetProcMeshSection(0)->ProcVertexBuffer[0].Normal;
				FixationParticleComponent->SetWorldLocation(Location + ForwardVector * 0.1);
				FRotator Rot = FRotationMatrix::MakeFromX(ForwardVector).Rotator();
				FixationParticleComponent->SetWorldRotation(Rot);
			}
		}
	}

	//Setup the particle systems parameters
	if (FixationParticleComponent) {
		FixationParticleComponent->SetColorParameter("Color", ParticleColors);

		float MaxParticleSize = 3;
		if (Duration == 0) {
			Duration = 0.01;
		}
		float size = FMath::Clamp((MaxParticleSize * (Duration*0.5f)), 0.5f, MaxParticleSize);
		FixationParticleComponent->SetVectorParameter("InitialSize", FVector(size, size, size));
		FixationParticleComponent->ActivateSystem(true);
		FixationParticleComponent->SetVisibility(ASynOpticonState::IsGazePointParticlesEnabled());
	}

	//Create and set the saccade particle systems parameters
	if (NmbFixationVisualisations > 1){ //If we have 2 or more fixations we visualize the saccades with a saccade particle system
		UParticleSystemComponent* SaccadeParticleComponent = nullptr;
		if (SaccadeTemplate) {
			SaccadeParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(GetOwner()->GetWorld(), SaccadeTemplate, PrevFixationLocation, FRotator::ZeroRotator, false);

			if (SaccadeParticleComponent) {

				SetupDynamicMaterial(SaccadeParticleComponent, false, MaxFixationVisualisations, NmbFixationVisualisations);

				SaccadeParticleComponent->SetBeamSourcePoint(0, PrevFixationLocation, 0);
				Location = Location + ForwardVector * 0.8;
				SaccadeParticleComponent->SetBeamTargetPoint(0, Location, 0);
				SaccadeParticleSystems.Enqueue(SaccadeParticleComponent);
				SaccadeParticleComponent->SetColorParameter("SaccadeColor", ParticleColors);
				FixationParticleComponent->ActivateSystem(true);
				SaccadeParticleComponent->SetVisibility(ASynOpticonState::IsGazePointParticlesEnabled());
			}
		}
	}

	UpdateParticleSystemNumbers();
	PrevFixationLocation = Location;
}

void UGazeDataVisualizerComponent::CheckIfMaxFixations() {
	NmbFixationVisualisations++;

	//If we have more fixations to visualise than the max limit we remove the oldest fixation before adding the new one
	if (NmbFixationVisualisations > MaxFixationVisualisations) {
		UParticleSystemComponent* FixationParticleSystem = nullptr;
		bool result = FixationParticleSystems.Dequeue(FixationParticleSystem);
		if (FixationParticleSystem) {
			FixationParticleSystem->Deactivate();
			FixationParticleSystem->DestroyComponent();
		}

		UParticleSystemComponent* SaccadeParticleSystem = nullptr;
		result = SaccadeParticleSystems.Dequeue(SaccadeParticleSystem);
		if (SaccadeParticleSystem) {
			SaccadeParticleSystem->Deactivate();
			SaccadeParticleSystem->DestroyComponent();
		}

		NmbFixationVisualisations--;
	}
}

void UGazeDataVisualizerComponent::ClearParticleSystems(){
	while (!FixationParticleSystems.IsEmpty()){
		UParticleSystemComponent* FixationParticleSystem = nullptr;
		bool result = FixationParticleSystems.Dequeue(FixationParticleSystem);
		if (FixationParticleSystem){
			FixationParticleSystem->Deactivate();
			FixationParticleSystem->DestroyComponent();
		}
	}

	while (!SaccadeParticleSystems.IsEmpty()){
		UParticleSystemComponent* SaccadeParticleSystem = nullptr;
		bool result = SaccadeParticleSystems.Dequeue(SaccadeParticleSystem);
		if (SaccadeParticleSystem){
			SaccadeParticleSystem->Deactivate();
			SaccadeParticleSystem->DestroyComponent();
		}
	}

	NmbFixationVisualisations = 0;
	SpawnedSystemsID = 0;
	FixationCount = 0;
}

void UGazeDataVisualizerComponent::SetGazePointParticlesVisibility(bool Visible)
{
	//Tmp queue to hold the dequeued particle systems
	TQueue<UParticleSystemComponent*> TmpQueue;

	while (!FixationParticleSystems.IsEmpty()){
		UParticleSystemComponent* FixationParticleSystem = nullptr;
		bool result = FixationParticleSystems.Dequeue(FixationParticleSystem);
		
		if (FixationParticleSystem)
		{
			if (Visible){
				FixationParticleSystem->SetVisibility(true, true);
			}
			else{
				FixationParticleSystem->SetVisibility(false, true);
			}

			TmpQueue.Enqueue(FixationParticleSystem);
		}
	}

	while (!TmpQueue.IsEmpty()){
		UParticleSystemComponent* FixationParticleSystem = nullptr;
		bool result = TmpQueue.Dequeue(FixationParticleSystem);

		if (FixationParticleSystem){
			FixationParticleSystems.Enqueue(FixationParticleSystem);
		}
	}

	while (!SaccadeParticleSystems.IsEmpty()){
		UParticleSystemComponent* SaccadeParticleSystem = nullptr;
		bool result = SaccadeParticleSystems.Dequeue(SaccadeParticleSystem);
		if (SaccadeParticleSystem){
			if (Visible){
				SaccadeParticleSystem->SetVisibility(true, true);
			}
			else{
				SaccadeParticleSystem->SetVisibility(false, true);
			}

			TmpQueue.Enqueue(SaccadeParticleSystem);
		}
	}

	while (!TmpQueue.IsEmpty()){
		UParticleSystemComponent* SaccadeParticleSystem = nullptr;
		bool result = TmpQueue.Dequeue(SaccadeParticleSystem);
		if (SaccadeParticleSystem){
			SaccadeParticleSystems.Enqueue(SaccadeParticleSystem);
		}
	}
}

void UGazeDataVisualizerComponent::UpdateParticleSystemNumbers()
{
	//Tmp queue to hold the dequeued particle systems
	TQueue<UParticleSystemComponent*> TmpQueue;

	int fixations = 0;
	while (!FixationParticleSystems.IsEmpty()) {
		UParticleSystemComponent* FixationParticleSystem = nullptr;
		bool result = FixationParticleSystems.Dequeue(FixationParticleSystem);

		if (FixationParticleSystem)
		{			
			TmpQueue.Enqueue(FixationParticleSystem);
			fixations++;
		}
	}

	int NumberOfFixations = fixations;
	while (!TmpQueue.IsEmpty()) {
		UParticleSystemComponent* FixationParticleSystem = nullptr;
		bool result = TmpQueue.Dequeue(FixationParticleSystem);

		if (FixationParticleSystem) {
			UpdateDynamicMaterial(FixationParticleSystem, fixations-1, NumberOfFixations-1);
			FixationParticleSystems.Enqueue(FixationParticleSystem);
			fixations--;
		}
	}
	
	int saccades = 0;
	while (!SaccadeParticleSystems.IsEmpty()) {
		UParticleSystemComponent* SaccadeParticleSystem = nullptr;
		bool result = SaccadeParticleSystems.Dequeue(SaccadeParticleSystem);
		if (SaccadeParticleSystem) {							
			TmpQueue.Enqueue(SaccadeParticleSystem);
			saccades++;
		}
	}

	int NumberOfSaccades = saccades;
	while (!TmpQueue.IsEmpty()) {
		UParticleSystemComponent* SaccadeParticleSystem = nullptr;
		bool result = TmpQueue.Dequeue(SaccadeParticleSystem);
		if (SaccadeParticleSystem) {
			UpdateDynamicMaterial(SaccadeParticleSystem, saccades-1, NumberOfSaccades-1);
			SaccadeParticleSystems.Enqueue(SaccadeParticleSystem);
			saccades--;
		}
	}
}

void UGazeDataVisualizerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason){
	Super::EndPlay(EndPlayReason);

	ClearParticleSystems();
	GazeActorFixations.Empty();
}

TArray<FFixationGroupStruct> UGazeDataVisualizerComponent::GetGazeData()
{
	return GazeActorFixations;
}

void UGazeDataVisualizerComponent::SetReplayGazeData(TArray<FFixationGroupStruct> Data)
{
	GazeActorFixations = Data;
	isFirstTime = true;
}

void UGazeDataVisualizerComponent::DoSeeking(FString _CallerID)
{
	FDateTime CurrentTime = ASynOpticonState::GetCurrentReplayTime();
	int i;
	ClearParticleSystems();
	for (i = 0; i < GazeActorFixations.Num() - 1; i++)
	{
		FDateTime TmpA = GazeActorFixations[i].Timestamp;
		FDateTime TmpB = GazeActorFixations[i + 1].Timestamp;

		double TmAA = (TmpA - CurrentTime).GetTotalSeconds();
		double TmBB = (TmpB - CurrentTime).GetTotalSeconds();
		if (TmpA <= CurrentTime
			&& TmpB > CurrentTime)
		{
			break;
		}
	}
	for (int j = i - MaxFixationVisualisations + 1; j <= i; j++)
	{
		if (j >= 0 && j < GazeActorFixations.Num()) {
			AddFixationData(GazeActorFixations[j].GazeActorId, GazeActorFixations[j].Location, GazeActorFixations[j].Duration, GazeActorFixations[j].Normal);
			FixationCount = j + 1;
		}
	}

	PrevTime = CurrentTime;
}

FEyeRadarPointData UGazeDataVisualizerComponent::Replay(FDateTime CurrentTime, AGazeActor*& OutGazeActor)
{	
	if (isFirstTime)
	{
		int i;
		ClearParticleSystems();
		for (i = 0; i < GazeActorFixations.Num() - 1; i++)
		{
			FDateTime TmpA = GazeActorFixations[i].Timestamp;
			FDateTime TmpB = GazeActorFixations[i + 1].Timestamp;

			double TmAA = (TmpA - CurrentTime).GetTotalSeconds();
			double TmBB = (TmpB - CurrentTime).GetTotalSeconds();

			double TimeDifference = (TmpB - TmpA).GetTotalSeconds();

			if (TmpA <= CurrentTime
				&& TmpB > CurrentTime)
			{
				FixationCount = i;
				isFirstTime = false;
				return FEyeRadarPointData();
			}
		}
	}
	else
	{
		if (FixationCount < 0 || FixationCount >= GazeActorFixations.Num())
		{
			return FEyeRadarPointData();
		}
		if (GazeActorFixations[FixationCount].Timestamp + FTimespan::FromSeconds(GazeActorFixations[FixationCount].Duration) < CurrentTime)
		{
			AddFixationData(GazeActorFixations[FixationCount].GazeActorId, 
				GazeActorFixations[FixationCount].Location, 
				GazeActorFixations[FixationCount].Duration, 
				GazeActorFixations[FixationCount].Normal);

			FEyeRadarPointData EyeRadarData;
			for (AActor* GActor : ASynOpticonState::GetGazeActorList()) {
				AGazeActor* GazeActor = Cast<AGazeActor>(GActor);
				if (GazeActor->GetID() == GazeActorFixations[FixationCount].GazeActorId 
					|| GazeActor->ContainsAOIID(false, GazeActorFixations[FixationCount].GazeActorId)
					|| GazeActor->ContainsAOIID(true, GazeActorFixations[FixationCount].GazeActorId)) {
					OutGazeActor = GazeActor;
					EyeRadarData.GazePoint = USynOpticonStatics::FindUVInTriangle(GazeActor->GetVerticies(), GazeActor->GetUVs(), GazeActorFixations[FixationCount].Location);
					EyeRadarData.Duration = GazeActorFixations[FixationCount].Duration;
					break;
				}
			}

			FixationCount++;
			return EyeRadarData;
		}
	}

	return FEyeRadarPointData();
}

void UGazeDataVisualizerComponent::SetParticleColor(FVector NewColor)
{
	ParticleColors = NewColor;
}

void UGazeDataVisualizerComponent::RemoveGazeActorCallback(int32 GazeActorID)
{

}

void UGazeDataVisualizerComponent::OnStartRecording(bool Start)
{
	if (Start) {
		GazeActorFixations.Empty();
	}
	ClearParticleSystems();
}