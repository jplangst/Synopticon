// Fill out your copyright notice in the Description page of Project Settings.

#include "EyeTrackingGameMode.h"
//#include "Synopticon.h"

AEyeTrackingGameMode::AEyeTrackingGameMode(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{	
	NatNetWAMPComponent = NewObject<UNatNetWAMPComponent>(this, UNatNetWAMPComponent::StaticClass(), FName("NatNetWAMPComponent"));
	EyeTrackingWAMPComponent = NewObject<UEyeTrackingWAMPComponent>(this, UEyeTrackingWAMPComponent::StaticClass(), FName("EyeTrackingWAMPComponent"));
	RemoteTrackerWAMPComponent = NewObject<URemoteTrackerWAMPComponent>(this, URemoteTrackerWAMPComponent::StaticClass(), FName("RemoteEyeTrackingWAMPComponent"));
	ImagerWAMPComponent = NewObject<UImagerWAMPComponent>(this, UImagerWAMPComponent::StaticClass(), FName("ImagerWAMPComponent"));
	ExperimentWAMPComponent = NewObject<UExperimentWAMPComponent>(this, UExperimentWAMPComponent::StaticClass(), FName("ExperimentWAMPComponent"));
	MicroTaskWAMPComponent = NewObject<UMicroTaskWAMPComponent>(this, UMicroTaskWAMPComponent::StaticClass(), FName("MicroTaskWAMPComponent"));
	OpenFaceWAMPComponent = NewObject<UOpenFaceWAMPComponent>(this, UOpenFaceWAMPComponent::StaticClass(), FName("OpenFaceWAMPComponent"));
}

void AEyeTrackingGameMode::StartPlay()
{
	ThreadPoolManager = NewObject<UThreadPoolManager>(this, UThreadPoolManager::StaticClass());
	GlobalEventSystem = NewObject<UGlobalEventSystem>(this, UGlobalEventSystem::StaticClass());

	//------------------------------------creation manager---------------------------------------
	CreationManagerComponent = NewObject<UCreationAndDeletionManager>(this, CreationManagerComponentBPP, FName("CreationAndDeletionManager"));
	CreationManagerComponent->RegisterComponent();
	UCreationAndDeletionManager::SetInstance(CreationManagerComponent);

	//------------------------------------Record manager-----------------------------------------
	RecordManagerComponent = NewObject<URecordManager>(this, URecordManager::StaticClass(), FName("RecordManager"));
	RecordManagerComponent->RegisterComponent();
	URecordManager::SetInstance(RecordManagerComponent);

	//------------------------------------Replay manager-----------------------------------------
	_ReplayManager = NewObject<UReplayManager>(this, UReplayManager::StaticClass(), FName("ReplayManager"));
	_ReplayManager->RegisterComponent();
	UReplayManager::SetInstance(_ReplayManager);

	//------------------------------------Global event ------------------------------------------
	ASynOpticonState::InitiateState(GlobalEventSystem, ThreadPoolManager);
	ASynOpticonState::GetGlobalEventSystem()->OnSOActorChanged.AddDynamic(this, &AEyeTrackingGameMode::SynOpticonActorChanged);
	ASynOpticonState::GetGlobalEventSystem()->OnEventReceiveMicroTaskEvent.AddDynamic(this, &AEyeTrackingGameMode::OnRecieveMicroTaskEvent);
	//---------------------------------------Viewport resize event--------------------------------------
	GetWorld()->GetGameViewport()->Viewport->ViewportResizedEvent.AddUObject(this, &AEyeTrackingGameMode::ViewportResizedCallback);
	
	//------------------WAMP-----------------
	NatNetWAMPComponent->RegisterComponent();
	EyeTrackingWAMPComponent->RegisterComponent();
	ExperimentWAMPComponent->RegisterComponent();
	RemoteTrackerWAMPComponent->RegisterComponent();
	MicroTaskWAMPComponent->RegisterComponent();

	OpenFaceWAMPComponent->RegisterComponent();

	SpawnModellingTools();

	Super::StartPlay();

	//only start the WAMP worker after initializing registering all WAMP components
	//after we start the WAMP worker, adding new WAMP component will require restarting the WAMP worker
	NatNetWAMPComponent->RegisterWAMP();
	EyeTrackingWAMPComponent->RegisterWAMP();
	ExperimentWAMPComponent->RegisterWAMP();
	RemoteTrackerWAMPComponent->RegisterWAMP();

	OpenFaceWAMPComponent->RegisterWAMP();

	FWAMPWorker::JoyInit(ASynOpticonState::GetWAMPRouterAdress(), ASynOpticonState::GetWAMPRealm());
}

void AEyeTrackingGameMode::ViewportResizedCallback(FViewport * viewport, uint32 ID) {
	if (viewport) {
		ASynOpticonState::GetGlobalEventSystem()->OnEventViewportResize.Broadcast(FVector2D(viewport->GetSizeXY().X, viewport->GetSizeXY().Y));
	}
}

void AEyeTrackingGameMode::SpawnModellingTools() {
	ManualModellingTool = (APositionModellingActor*)GetWorld()->SpawnActor(APositionModellingActor::StaticClass());
	ManualModellingTool->Rename(*FString("ManualTool"));
	ManualModellingTool->Tags.Add(FName("ManualTool"));
	ManualModellingTool->SetModellingToolType(true);
	UPositionAndOrientationComponent* ManualPosOriComponent = NewObject<UPositionAndOrientationComponent>(ManualModellingTool, UPositionAndOrientationComponent::StaticClass(), FName(*(ManualModellingTool->GetName() + "PosOriComponent")));
	if (ManualPosOriComponent)
	{
		ManualPosOriComponent->RegisterComponent();
		ManualPosOriComponent->SetActorLocationOffset(ManualModellingTool->GetLocationOffset());
		ManualPosOriComponent->SetShouldApplyOffset(true);
		ManualPosOriComponent->SetOwnerRigidBodyName("ManualTool"); //Read from file / Create in UE, in modelling mode
		ManualPosOriComponent->SetMarkersConfiguration(MarkersConfigurationEnum::VE_ManualTool);
		ManualPosOriComponent->SetNatNetWAMPComponent(NatNetWAMPComponent);
		//ManualPosOriComponent->HideOrientationArrowMesh();
		ManualModellingTool->SetRootComponent(ManualPosOriComponent);
		ManualModellingTool->SetPositionAndOrientationComponent(ManualPosOriComponent);
	}

	LaserModellingTool = (APositionModellingActor*)GetWorld()->SpawnActor(APositionModellingActor::StaticClass());
	LaserModellingTool->Tags.Add(FName("LaserTool"));
	LaserModellingTool->Rename(*FString("LaserTool"));
	LaserModellingTool->SetModellingToolType(false);
	UPositionAndOrientationComponent* LaserPosOriComponent = NewObject<UPositionAndOrientationComponent>(LaserModellingTool, UPositionAndOrientationComponent::StaticClass(), FName(*(LaserModellingTool->GetName() + "PosOriComponent")));
	if (LaserPosOriComponent)
	{
		LaserPosOriComponent->RegisterComponent();
		LaserPosOriComponent->SetActorLocationOffset(LaserModellingTool->GetLocationOffset());
		LaserPosOriComponent->SetShouldApplyOffset(true);
		LaserPosOriComponent->SetOwnerRigidBodyName("LaserTool"); //Read from file / Create in UE, in modelling mode
		LaserPosOriComponent->SetMarkersConfiguration(MarkersConfigurationEnum::VE_Laser);
		LaserPosOriComponent->SetNatNetWAMPComponent(NatNetWAMPComponent);
		//LaserPosOriComponent->HideOrientationArrowMesh();
		LaserModellingTool->SetRootComponent(LaserPosOriComponent);
		LaserModellingTool->SetPositionAndOrientationComponent(LaserPosOriComponent);
	}

	CalibrationTool = (ACalibrationActor*)GetWorld()->SpawnActor(ACalibrationActor::StaticClass());
	CalibrationTool->Rename(*FString("CalibrationTool"));
	CalibrationTool->Tags.Add(FName("CalibrationTool"));
	UPositionAndOrientationComponent* CalibrationPosOriComponent = NewObject<UPositionAndOrientationComponent>(CalibrationTool, UPositionAndOrientationComponent::StaticClass(), FName(*(CalibrationTool->GetName() + "PosOriComponent")));
	if (CalibrationPosOriComponent)
	{
		CalibrationPosOriComponent->RegisterComponent();
		CalibrationPosOriComponent->SetActorLocationOffset(LaserModellingTool->GetLocationOffset());
		CalibrationPosOriComponent->SetShouldApplyOffset(false);
		CalibrationPosOriComponent->SetOwnerRigidBodyName("CalibrationTool"); 
		CalibrationPosOriComponent->SetMarkersConfiguration(MarkersConfigurationEnum::VE_CalibrationTool);
		CalibrationPosOriComponent->SetNatNetWAMPComponent(NatNetWAMPComponent);
		CalibrationPosOriComponent->HideOrientationArrowMesh();
		CalibrationTool->SetRootComponent(CalibrationPosOriComponent);
		CalibrationTool->SetPositionAndOrientationComponent(CalibrationPosOriComponent);
	}
}

void AEyeTrackingGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//Cleanup vieport delegate
	GetWorld()->GetGameViewport()->Viewport->ViewportResizedEvent.RemoveAll(this);

	//stop wamp and clean up
	FWAMPWorker::Shutdown();
	FWAMPWorker::CleanUp();
	/*GazeMaterialComponents.Empty();*/

	//Clean up
	ASynOpticonState::CleanupState();
	GlobalEventSystem = nullptr;
	ThreadPoolManager = nullptr;
}

// Called every frame
void AEyeTrackingGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//End of testing

	UpdateRaycastComponents();
	UpdateCurrentActorHeatmap();
}

void AEyeTrackingGameMode::ResetMicroTaskStatus() {
	for (AActor* Actor : ASynOpticonState::GetGazeActorList()) {
		AGazeActor* GazeActor = Cast<AGazeActor>(Actor);

		//Reset the screen status
		GazeActor->SetMicroTaskStatus(EMicroTaskStatus::NA);

		//Reset the popup aois
		TArray<FAOIData> PopupAOIs = GazeActor->GetAOIS(true);
		for (int i = 0; i < PopupAOIs.Num(); i++) {
			PopupAOIs[i].MicroTaskStatus = EMicroTaskStatus::NA;
		}
		GazeActor->SetAOIS(true, PopupAOIs);

		//Reset the aois
		TArray<FAOIData> AOIs = GazeActor->GetAOIS(false);
		for (int i = 0; i < AOIs.Num(); i++) {
			AOIs[i].MicroTaskStatus = EMicroTaskStatus::NA;
		}
		GazeActor->SetAOIS(false, AOIs);
	}
}

void AEyeTrackingGameMode::OnRecieveMicroTaskEvent(FMicroTaskEventStruct MicroTaskEvent)
{
	//Reset the current micro task status before updating
	ResetMicroTaskStatus();

	for (FString AOIName : MicroTaskEvent.RelevantAOIs) {
		for (AActor* Actor : ASynOpticonState::GetGazeActorList()) {
			AGazeActor* GazeActor = Cast<AGazeActor>(Actor);
			//Extract the popup aois
			TArray<FAOIData> PopupAOIs = GazeActor->GetAOIS(true);
			//Extract the aois
			TArray<FAOIData> AOIs = GazeActor->GetAOIS(false);
			if (GazeActor->GetName().Equals(AOIName)) {
				GazeActor->SetMicroTaskStatus(EMicroTaskStatus::STARTED);
				break;
			}
			else {
				bool popupFound = false; //Used to skip the AOI loop if the name was a popup aoi
				for (int i = 0; i < PopupAOIs.Num(); i++) {
					if (PopupAOIs[i].Name.Equals(AOIName)) {
						PopupAOIs[i].MicroTaskStatus = EMicroTaskStatus::STARTED;
						popupFound = true;
						break;
					}
				}

				//If it was a popup aoi there is no need to check the aois, just update the popups and break
				if (popupFound) {
					//Update the popup aois
					GazeActor->SetAOIS(true, PopupAOIs);
					break;
				}

				bool aoiFound = false;
				for (int i = 0; i < AOIs.Num(); i++) {
					if (AOIs[i].Name.Equals(AOIName)) {
						AOIs[i].MicroTaskStatus = EMicroTaskStatus::STARTED;
						aoiFound = true;
						break;
					}
				}

				//Update the aois
				if (aoiFound) {
					GazeActor->SetAOIS(false, AOIs);
				}			
			}
		}
	}
}

void AEyeTrackingGameMode::UpdateRaycastComponents() {
	//Update the raycast component of the synopticon actors who has one

	TArray<AActor*> SynOpticonActorList = ASynOpticonState::GetSynOpticonActorList();
	for (AActor* Actor : SynOpticonActorList)
	{
		ASynOpticonActor* SynOpticonActor = Cast<ASynOpticonActor>(Actor);

		URaycastComponent_Ours* RaycastComponent = SynOpticonActor->FindComponentByClass<URaycastComponent_Ours>();
		if (RaycastComponent)
		{
			int32 TargetID = RaycastComponent->GetHitTargetID();
			int32 ActorID = SynOpticonActor->GetActorID();
			//FVector2D OutUV;

			//FHitResult* HitResult = RaycastComponent->GetHitResult();
			//if (HitResult) {

			//}
			//UGameplayStatics::FindCollisionUV(*RaycastComponent->GetHitResult(), 0, OutUV);

			if (TargetID != -1)
			{
				TArray<UActorComponent*> MaterialComponentList = ASynOpticonState::GetGazeMaterialComponentList();
				for (UActorComponent* Component : MaterialComponentList) {
					UGazeObjectMaterialComponent* GazeMaterialComponent = Cast<UGazeObjectMaterialComponent>(Component);
					if (TargetID == GazeMaterialComponent->GetOwnerID()) {
						GazeMaterialComponent->NotifyCollision(true, ActorID);

						if (GazeMaterialComponent->GetOwner()->IsA(AGazeActor::StaticClass())) {
							AGazeActor* GazeActor = (AGazeActor*)GazeMaterialComponent->GetOwner();
							if (GazeActor)
							{
								/*URadiantWebBrowserComponent* BrowserComponent = GazeActor->FindComponentByClass<URadiantWebBrowserComponent>();
								if (BrowserComponent && !BrowserComponent->bSetInteractionOverlap)
								{
									BrowserComponent->BeginInteractionOverlap(RaycastComponent->GetOwner());
									BrowserComponent->bSetInteractionOverlap = true;
								}*/
							}
						}
					}
					else
					{
						GazeMaterialComponent->NotifyCollision(false, ActorID);

						if (GazeMaterialComponent->GetOwner()->IsA(AGazeActor::StaticClass())) {
							AGazeActor* GazeActor = (AGazeActor*)GazeMaterialComponent->GetOwner();
							if (GazeActor)
							{
								/*URadiantWebBrowserComponent* BrowserComponent = GazeActor->FindComponentByClass<URadiantWebBrowserComponent>();
								if (BrowserComponent && !BrowserComponent->bSetInteractionOverlap)
								{
									BrowserComponent->EndInteractionOverlap(RaycastComponent->GetOwner());
									BrowserComponent->bSetInteractionOverlap = false;
								}*/
							}
						}
					}
				}
			}
			else
			{
				TArray<UActorComponent*> MaterialComponentList = ASynOpticonState::GetGazeMaterialComponentList();
				for (UActorComponent* Component : MaterialComponentList) 
				{
					UGazeObjectMaterialComponent* GazeMaterialComponent = Cast<UGazeObjectMaterialComponent>(Component);
					GazeMaterialComponent->NotifyCollision(false, ActorID);
				}
			}
		}
	}
}

void AEyeTrackingGameMode::UpdateCurrentActorHeatmap() {
	ASynOpticonActor* CurrentActor = Cast<ASynOpticonActor>(ASynOpticonState::GetCurrentActor());
	if (CurrentActor)
	{
		TMap<int32, float> ActorHeatMap = CurrentActor->GetHeatValues();

		if (ActorHeatMap.Num() > 0)
		{
			TArray<int32> GazeActorIDs;
			ActorHeatMap.GenerateKeyArray(GazeActorIDs);

			//Update all the screens overall heatmap
			TArray<UActorComponent*> MaterialComponentList = ASynOpticonState::GetGazeMaterialComponentList();
			for (UActorComponent* Component : MaterialComponentList)
			{
				UGazeObjectMaterialComponent* GazeMaterial = Cast<UGazeObjectMaterialComponent>(Component);		
				bool MatchFound = false;
				for (int32 GazeActorID : GazeActorIDs)
				{
					if (GazeActorID == GazeMaterial->GetOwnerID()) //Looking at the screen itself
					{
						if (*ActorHeatMap.Find(GazeActorID) > 1 || *ActorHeatMap.Find(GazeActorID) < 0)
						{
							float i = *ActorHeatMap.Find(GazeActorID);
							int j = 0;
						}

						GazeMaterial->SetHeatMapValue(*ActorHeatMap.Find(GazeActorID));
						MatchFound = true;
						break;
					}					
				}
				if (!MatchFound)
				{
					GazeMaterial->SetHeatMapValue(0);
				}
			}

			TArray<AActor*> GazeActorList = ASynOpticonState::GetGazeActorList();

			//Update all the AOIs heatmap			
			for (AActor* Actor : GazeActorList) {
				AGazeActor* GazeActor = Cast<AGazeActor>(Actor);

				//Update the normal AOIs
				TArray<FAOIData> AOIS = GazeActor->GetAOIS(false);
				for (int i = 0; i < AOIS.Num(); i++){
					bool MatchFound = false;
					for (int32 GazeActorID : GazeActorIDs)
					{
						if (AOIS[i].ID == GazeActorID) {
							AOIS[i].Color = ASynOpticonState::GetHeatMapColorFromValue(*ActorHeatMap.Find(GazeActorID));
							GazeActor->EditAOI(false, AOIS[i]);
							MatchFound = true;
							break;
						}						
					}

					if (!MatchFound)
					{
						AOIS[i].Color = ASynOpticonState::GetHeatMapColorFromValue(0);
						GazeActor->EditAOI(false, AOIS[i]);
					}				
				}	

				//Update the PopupAOIs
				TArray<FAOIData> PopupAOIS = GazeActor->GetAOIS(true);
				for (int i = 0; i < PopupAOIS.Num(); i++) {
					bool MatchFound = false;
					for (int32 GazeActorID : GazeActorIDs)
					{
						if (PopupAOIS[i].ID == GazeActorID) {
							PopupAOIS[i].Color = ASynOpticonState::GetHeatMapColorFromValue(*ActorHeatMap.Find(GazeActorID));
							GazeActor->EditAOI(true, PopupAOIS[i]);
							MatchFound = true;
							break;
						}
					}

					if (!MatchFound)
					{
						PopupAOIS[i].Color = ASynOpticonState::GetHeatMapColorFromValue(0);
						GazeActor->EditAOI(true, PopupAOIS[i]);
					}
				}
			}			
		}
		else
		{
			TArray<UActorComponent*> MaterialComponentList = ASynOpticonState::GetGazeMaterialComponentList();
			for (UActorComponent* Component : MaterialComponentList)
			{
				UGazeObjectMaterialComponent* GazeMaterial = Cast<UGazeObjectMaterialComponent>(Component);
				GazeMaterial->SetHeatMapValue(0);
			}
		}
	}
}

//=================================CALIBRATION=========================================
void AEyeTrackingGameMode::CalibrateVectorToPoint(int CalibrationPointNumber) 
{
	ASynOpticonActor* CurrentSOActor = Cast<ASynOpticonActor>(ASynOpticonState::GetCurrentActor());
	if (CurrentSOActor) {
		int CalNum = CalibrationPointNumber == CalibrationPoints.Num() ? 1 : 0;
		CurrentSOActor->CalibrateToPoint(CalibrationPoints[CalibrationPointNumber-1], CalNum);
	}
}

void AEyeTrackingGameMode::AddCalibrationPoint(FVector NewCalibrationPoint)
{
	CalibrationPoints.Add(NewCalibrationPoint);
}

void AEyeTrackingGameMode::ClearCalibrationPoints()
{
	CalibrationPoints.Empty();
}

//TODO Do this directly from wherever it is called
void AEyeTrackingGameMode::ResetCurrentActorsCalibration()
{
	ASynOpticonActor* CurrActor = Cast<ASynOpticonActor>(ASynOpticonState::GetCurrentActor());
	if (CurrActor)
	{
		CurrActor->ResetCalibration();
	}	
}

//Called from the Global event system, do not call this function directly!
void AEyeTrackingGameMode::SynOpticonActorChanged() {
	int32 CurrentActorID = -1;
	bool HasETG = false;
	bool HasShimmerHR = false;
	bool HasShimmerGSR = false;

	//get the current actor
	ASynOpticonActor* CurrentActor = Cast<ASynOpticonActor>(ASynOpticonState::GetCurrentActor());
	FString ShimmerDeviceName = "";
	FString ETGDevice = "";
	if (CurrentActor && !CurrentActor->IsPendingKill()) {
		//Get the ID of the current actor
		CurrentActorID = CurrentActor->GetActorID();

		//Check if the current actor has an eye vectors component
		HasETG = false;
		UEyeVectorsComponent* EyeVectorsComponent = CurrentActor->FindComponentByClass<UEyeVectorsComponent>();
		if (EyeVectorsComponent) {
			HasETG = true;
			ETGDevice = EyeVectorsComponent->GetGlassesID();
			if (ImagerWAMPComponent) {
				EyeTrackingWAMPComponent->SetCurrentActorGlassesID(EyeVectorsComponent->GetGlassesID());
				ImagerWAMPComponent->SetGlassesID(EyeVectorsComponent->GetGlassesID());
			}

			UpdateActorsSceneCameraStreaming(true);
		}
		else {
			UpdateActorsSceneCameraStreaming(false);
		}

		//Check if the current actor has a remote tracker component
		URemoteEyeTrackerComponent* RetComp = CurrentActor->FindComponentByClass<URemoteEyeTrackerComponent>();
		if (RetComp) {
			RemoteTrackerWAMPComponent->SetCurrentActorRetID(RetComp->GetRETData().RemoteTrackerID);
			HasETG = true; //TODO make sure this doesn't break anything. If it does we have to add another field to the event broadcast
		}

		//Check if the actor has a shimmer component and which sensors are activated
		HasShimmerHR = false;
		HasShimmerGSR = false;

		//Get the shimmer components attached to the Actor
		TArray<UShimmerVRPNComponent*> ShimmerComponents;
		CurrentActor->GetComponents(ShimmerComponents);

		for (UShimmerVRPNComponent* ShimmerComponent : ShimmerComponents) {
			if (ShimmerComponent->HasHR()) {
				ShimmerDeviceName = ShimmerComponent->GetShimmerDeviceName();
				HasShimmerHR = true;
			}
			if (ShimmerComponent->HasGSR()) {
				ShimmerDeviceName = ShimmerComponent->GetShimmerDeviceName();
				HasShimmerGSR = true;
			}
		}
	}

	ASynOpticonState::GetGlobalEventSystem()->OnEventUpdateCurrentSOActor.Broadcast(CurrentActorID, HasETG, HasShimmerHR, HasShimmerGSR, ETGDevice, ShimmerDeviceName);
}

//TODO prolly dont need to go through the gamemode for this, just access it directly from wherever this is called
void AEyeTrackingGameMode::UpdateActorsSceneCameraStreaming(bool Stream)
{
	ASynOpticonActor* CurrentActor = Cast<ASynOpticonActor>(ASynOpticonState::GetCurrentActor());
	if (CurrentActor)
	{
		UEyeVectorsComponent* EyeVectorsComponent = CurrentActor->FindComponentByClass<UEyeVectorsComponent>();
		if (EyeVectorsComponent)
		{
			if (ImagerWAMPComponent)
			{
				ImagerWAMPComponent->SetSceneCameraStreaming(Stream);
			}			
		}
	}
}

//Callback from the UI. Don't call manually
void AEyeTrackingGameMode::OpenOrSaveXMLFile(EFileDialogTypeEnum type, FString fileName)
{
	switch (type)
	{
		case EFileDialogTypeEnum::FT_Open_SOObjects: CreationManagerComponent->LoadRigidBodies(fileName); break;
		case EFileDialogTypeEnum::FT_Save_SOObjects: GazeActorLoader::SaveFile(fileName); break;
		case EFileDialogTypeEnum::FT_Open_SOActors:	CreationManagerComponent->CreateSynOpticonActorsFromData(fileName); break;
		case EFileDialogTypeEnum::FT_Save_SOActors:	SynOpticonActorLoader::SaveFile(fileName); break;
		default:
			break;
	}
}

FVector AEyeTrackingGameMode::CalculatePoint(float distance) //TODO prolly dont need to go through the gamemode for this. Just get the actor directly
{
	if (distance > 0)
	{
		return LaserModellingTool->CalculatePoint(distance);
	}
	else
	{
		return ManualModellingTool->CalculatePoint(distance);
	}
}

void AEyeTrackingGameMode::ResetLoggedData() { //TODO prolly dont need to go through the gamemode for this. Just get the actor components directly
	for (AActor* SynOpticonActor : ASynOpticonState::GetSynOpticonActorList())
	{
		USynOpticonAggLogComponent* LogComp = SynOpticonActor->FindComponentByClass<USynOpticonAggLogComponent>();

		if (LogComp)
		{
			LogComp->ResetLoggedData(true);
		}
	}
}

//=============================WAMP WORKER==============================
void AEyeTrackingGameMode::RestartWAMP()
{
	FWAMPWorker::Shutdown();
	FReconnectWAMPWorker::Shutdown();
	FWAMPWorker::JoyInit(ASynOpticonState::GetWAMPRouterAdress(), ASynOpticonState::GetWAMPRealm());
}