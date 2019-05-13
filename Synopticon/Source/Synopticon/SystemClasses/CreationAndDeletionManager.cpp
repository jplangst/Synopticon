// Fill out your copyright notice in the Description page of Project Settings.

#include "CreationAndDeletionManager.h"

UCreationAndDeletionManager* UCreationAndDeletionManager::Instance = nullptr;
UCreationAndDeletionManager* UCreationAndDeletionManager::GetCADManagerInstance()
{
	return Instance;
}

void UCreationAndDeletionManager::SetInstance(UCreationAndDeletionManager* _Instance)
{
	Instance = _Instance;
}

// Sets default values for this component's properties
UCreationAndDeletionManager::UCreationAndDeletionManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	ShouldDeleteSynOpticonActors = false;
	ShouldCreateSynOpticonActors = false;
	ShouldCreateGazeActors = false;
	ShouldDeleteGazeActors = false;
	ShouldDelayRegisterPosOri = false;
}

// Called when the game starts
void UCreationAndDeletionManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	LoadedModelFile = "";
}

// Called every frame
void UCreationAndDeletionManager::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
	HandleActorSpawningAndDeletion();
}

void UCreationAndDeletionManager::CreateSynOpticonActorsFromData(FString filepath)
{
	TArray<FSynOpticonActorStruct> DuplicatedItems;
	for (FSynOpticonActorStruct SynOpticonData : SynOpticonActorLoader::LoadResourceFromFile(filepath))
	{
		bool isDuplicated = false;
		for (AActor* Actor : ASynOpticonState::GetSynOpticonActorList())
		{
			ASynOpticonActor* SynOpticonActor = Cast<ASynOpticonActor>(Actor);
			if (SynOpticonActor->GetActorName().Equals(SynOpticonData.ActorName))
			{
				DuplicatedItems.Add(SynOpticonData);
				isDuplicated = true;
				break;
			}
		}
		if (!isDuplicated)
		{
			CreateSynOpticonActorFromStruct(SynOpticonData, false);
		}
	}
	if (DuplicatedItems.Num() > 0)
	{
		ReplaceOrRenameDuplicatedSynOpticonActors(DuplicatedItems);
	}
}

ASynOpticonActor* UCreationAndDeletionManager::CreateSynOpticonActorFromStruct(FSynOpticonActorStruct SynOpticonActorStruct, bool ReplayMode)
{
	TPair<FSynOpticonActorStruct, bool> NewActor;
	NewActor.Key = SynOpticonActorStruct;
	NewActor.Value = false;
	SynOpticonActorsDataToCreateFrom.Add(NewActor);
	ShouldCreateSynOpticonActors = true;

	return nullptr;
}

void UCreationAndDeletionManager::CreateAndReplaceSynOpticonActorFromStruct(FSynOpticonActorStruct SynOpticonActorStruct)
{
	RemoveSynOpticonActor(SynOpticonActorStruct.ActorName);
	CreateSynOpticonActorFromStruct(SynOpticonActorStruct, false);
}

void UCreationAndDeletionManager::HandleActorSpawningAndDeletion() {

	//Chain the parents and the gaze actors to make sure that the parents are handled before any children are created
	if (ShouldDeleteGazeParentActors) {
		DeleteGazeParentActors();
		ShouldDeleteGazeParentActors = false;
	}
	else if (ShouldCreateGazeParentActors) {
		CreateGazeParentActors();
		ShouldCreateGazeParentActors = false;
	}
	else if (ShouldDeleteGazeActors)
	{
		DeleteGazeActors();
		ShouldDeleteGazeActors = false;
	}
	else if (ShouldCreateGazeActors)
	{
		CreateGazeActors();
		ShouldCreateGazeActors = false;
	}

	if (ShouldDeleteSynOpticonActors)
	{
		DeleteSynOpticonActors();
		ShouldDeleteSynOpticonActors = false;
	}
	else if (ShouldCreateSynOpticonActors)
	{
		CreateSynOpticonActors();
		ShouldCreateSynOpticonActors = false;
	}
}

void UCreationAndDeletionManager::EditSynOpticonActor(ASynOpticonActor* Actor, FSynOpticonActorStruct SynOpticonActorStruct)
{
	UNatNetWAMPComponent* NatNetWAMPComponent = GetOwner()->FindComponentByClass<UNatNetWAMPComponent>();
	UEyeTrackingWAMPComponent* EyeTrackingWAMPComponent = GetOwner()->FindComponentByClass<UEyeTrackingWAMPComponent>();
	URemoteTrackerWAMPComponent* RetWAMPComponent = GetOwner()->FindComponentByClass<URemoteTrackerWAMPComponent>();

	USynOpticonFactory::EditSynOpticonActor(Actor, SynOpticonActorStruct, NatNetWAMPComponent, EyeTrackingWAMPComponent, RetWAMPComponent, WebcamCompBP);

	ASynOpticonActor* CurrActor = Cast<ASynOpticonActor>(ASynOpticonState::GetCurrentActor());

	if (CurrActor && Actor->GetActorID() == CurrActor->GetActorID()) {
		ASynOpticonState::GetGlobalEventSystem()->OnSOActorChanged.Broadcast();
	}

	ASynOpticonState::GetGlobalEventSystem()->OnEventEditSOActor.Broadcast(Actor->GetActorID());
}

void UCreationAndDeletionManager::DeleteGazeParentActors()
{
	for (AGazeParentActor* GazeParentActor : GazeParentActorsToDelete)
	{
		ASynOpticonState::RemoveGazeParentActor(GazeParentActor);

		GazeParentActor->K2_DestroyActor();
		GazeParentActor = NULL;
	}
	GEngine->ForceGarbageCollection(true);
	GazeParentActorsToDelete.Empty();
}

void UCreationAndDeletionManager::CreateGazeParentActors()
{
	for (FGazeParentActorData GazeParentActorData : GazeParentActorsDataToCreateFrom)
	{
		AGazeParentActor* NewParentActor = GetWorld()->SpawnActor<AGazeParentActor>();
		if (NewParentActor) {
			NewParentActor->CreatePosOriComponent(GazeParentActorData.NatNetName);

			if (!ShouldDelayRegisterPosOri) {
				NewParentActor->RegisterPosOriComponent();
			}

			NewParentActor->SetSceneCompName(GazeParentActorData.SceneCompName);
			NewParentActor->SetInitialLocation(GazeParentActorData.InitialLocation);

			//NewParentActor->SetCalibrationQuat(GazeParentActorData.CalibrationQuat);

			ASynOpticonState::AddGazeParentActor(NewParentActor);
		}
	}

	GazeParentActorsDataToCreateFrom.Empty();
}

void UCreationAndDeletionManager::DeleteGazeActors()
{
	for (AGazeActor* GazeActor : GazeActorsToDelete)
	{
		//Remove from GazeActorList. Take in note that it does not remove the screen in the world. Destroy its blueprint to remove it from the world.
		ASynOpticonState::RemoveGazeActor(GazeActor);

		//Clear any gaze parents
		if (GazeActor->IsParented()) {
			GazeActor->SetGazeParentActor(nullptr, "");
		}

		//Find references to this actor from raycast component and remove it.
		for (AActor* Actor : ASynOpticonState::GetSynOpticonActorList())
		{
			ASynOpticonActor* SynOpticonActor = Cast<ASynOpticonActor>(Actor);

			URaycastComponent_Ours* RaycastComponent = SynOpticonActor->FindComponentByClass<URaycastComponent_Ours>();
			if (RaycastComponent)
			{
				if (RaycastComponent->GetHitTargetID() == GazeActor->GetID())
					RaycastComponent->SetHitTargetID(-1);
			}

			USynOpticonAggLogComponent* LogComponent = SynOpticonActor->FindComponentByClass<USynOpticonAggLogComponent>();
			if (LogComponent)
			{
				LogComponent->RemoveObjectsRelatedToID(GazeActor->GetID());
			}

			URemoteEyeTrackerComponent* RemoteComponent = SynOpticonActor->FindComponentByClass<URemoteEyeTrackerComponent>();
			if (RemoteComponent)
			{
				if (GazeActor == RemoteComponent->GetAttachedScreen())
				{
					RemoteComponent->SetAttachedScreen(nullptr);
					FRemoteEyeTrackerStruct retData = RemoteComponent->GetRETData();
					retData.AttachedScreenName = "";
					RemoteComponent->SetRETDataStruct(retData);
				}
			}			
		}

		//Remove the material component from the array list
		UGazeObjectMaterialComponent* MaterialComponent = GazeActor->FindComponentByClass<UGazeObjectMaterialComponent>();
		if (MaterialComponent) {
			int Index = -1;
			for (int i = 0; i < ASynOpticonState::GetGazeMaterialComponentList().Num(); i++) {
				UGazeObjectMaterialComponent* CurrentMaterialComponent = Cast<UGazeObjectMaterialComponent>(ASynOpticonState::GetGazeMaterialComponentList()[i]);
				if (MaterialComponent->GetOwnerID() == CurrentMaterialComponent->GetOwnerID()) {
					Index = i;
					break;
				}
			}

			if (Index != -1) {
				ASynOpticonState::RemoveMaterialComponent(Index);
			}
		}
		int32 ActorID = GazeActor->GetID();

		GazeActor->K2_DestroyActor();
		GazeActor = NULL;
		ASynOpticonState::GetGlobalEventSystem()->OnEventRemoveGazeActor.Broadcast(ActorID);
	}
	GEngine->ForceGarbageCollection(true);
	GazeActorsToDelete.Empty();
}

void UCreationAndDeletionManager::CreateGazeActors()
{
	for (FGazeActorData GazeActorData : GazeActorsDataToCreateFrom)
	{
		AGazeActor* GazeActor = UGazeActorFactory::GetInstance()->CreateGazeActor(GazeActorData, GetWorld(), GazeActorBP);
		if (!GazeActor) {
			FString Message = "Something's wrong, can not create this Gaze Actor";
			ASynOpticonState::GetGlobalEventSystem()->OnErrorMessage.Broadcast(Message);
			continue;
		}
		if (GazeActorData.ID != -1)
		{
			GazeActor->SetID(GazeActorData.ID);
		}

		GazeActor->SetButtonMeshName(GazeActorData.ButtonMesh);
		GazeActor->SetSplineMeshName(GazeActorData.SplineMesh);

		UGazeObjectMaterialComponent* MaterialComponent = GazeActor->FindComponentByClass<UGazeObjectMaterialComponent>();
		if (MaterialComponent) {
			ASynOpticonState::AddMaterialComponent(MaterialComponent);
			MaterialComponent->SetOwnerID(GazeActor->GetID());
		}

		ASynOpticonState::AddGazeActor(GazeActor);
	}

	if (ShouldDelayRegisterPosOri) {
		for (AActor* Parent : ASynOpticonState::GetGazeParentActorList()) {
			AGazeParentActor* GazeParent = Cast<AGazeParentActor>(Parent);
			GazeParent->RegisterPosOriComponent();
		}
		ShouldDelayRegisterPosOri = false;
	}

	GazeActorsDataToCreateFrom.Empty();
}

void UCreationAndDeletionManager::RemoveGazeActor(AGazeActor* gazeActor)
{
	GazeActorsToDelete.Add(gazeActor);
	ShouldDeleteGazeActors = true;
}

void UCreationAndDeletionManager::RemoveAllGazeActors()
{
	for (int i = ASynOpticonState::GetGazeActorList().Num() - 1; i >= 0; i--)
	{
		AGazeActor* ActorToRemove = Cast<AGazeActor>(ASynOpticonState::GetGazeActorList()[i]);
		RemoveGazeActor(ActorToRemove);
	}

	ASynOpticonState::ClearGazeActorList();

	LoadedModelFile = "";
}

void UCreationAndDeletionManager::RemoveAllGazeParentActors()
{
	for (int i = ASynOpticonState::GetGazeParentActorList().Num() - 1; i >= 0; i--)
	{
		AGazeParentActor* Parent = Cast<AGazeParentActor>(ASynOpticonState::GetGazeParentActorList()[i]);
		RemoveGazeParentActor(Parent);
	}

	ASynOpticonState::ClearGazeParentActorList();
}

bool UCreationAndDeletionManager::EditGazeActor(AGazeActor* gazeActor, FGazeActorData newInfo)
{
	//Update information of this gaze actor
	gazeActor->Rename(*newInfo.Name);
	gazeActor->SetName(newInfo.Name);
	gazeActor->SetIsGazeObject(newInfo.IsGazeObject);
	gazeActor->SetIsCollisionObject(newInfo.IsCollisionObject);
	gazeActor->SetFontSize(newInfo.FontSize);
	if (!gazeActor->GetURL().Equals(newInfo.URL))
	{
		gazeActor->SetURL(newInfo.URL);
		if (!newInfo.URL.Equals(""))
		{
			/*URadiantWebViewRenderComponent* RenderComponent = gazeActor->FindComponentByClass<URadiantWebViewRenderComponent>();
			if (RenderComponent)
			{
				RenderComponent->Activate();
			}
			URadiantWebBrowserComponent* BrowserComponent = gazeActor->FindComponentByClass<URadiantWebBrowserComponent>();
			if (BrowserComponent)
			{
				BrowserComponent->SetCanInteract(true);
				BrowserComponent->StartRefresh();
				BrowserComponent->LoadURL(newInfo.URL);
			}*/
		}
		else
		{
			/*URadiantWebViewRenderComponent* RenderComponent = gazeActor->FindComponentByClass<URadiantWebViewRenderComponent>();
			if (RenderComponent)
			{
				RenderComponent->Deactivate();
			}
			URadiantWebBrowserComponent* BrowserComponent = gazeActor->FindComponentByClass<URadiantWebBrowserComponent>();
			if (BrowserComponent)
			{
				BrowserComponent->SetCanInteract(false);
			}*/
		}
	}
	if (gazeActor->IsMovable())
	{
		gazeActor->SetNatNetName(newInfo.NatNetName);
		gazeActor->SetDimensions(newInfo.SizeDimensions);
	}

	//gazeActor->AddAOIS(newInfo.AOIS);

	ASynOpticonState::GetGlobalEventSystem()->OnEventEditGazeActor.Broadcast(gazeActor->GetID());
	return true;
}

void UCreationAndDeletionManager::CreateSynOpticonActors()
{
	UNatNetWAMPComponent* NatNetWAMPComponent = GetOwner()->FindComponentByClass<UNatNetWAMPComponent>();
	UEyeTrackingWAMPComponent* EyeTrackingWAMPComponent = GetOwner()->FindComponentByClass<UEyeTrackingWAMPComponent>();
	URemoteTrackerWAMPComponent* RetWAMPComponent = GetOwner()->FindComponentByClass<URemoteTrackerWAMPComponent>();
	UOpenFaceWAMPComponent* OpenFaceWAMPComponent = GetOwner()->FindComponentByClass<UOpenFaceWAMPComponent>();

	for (TPair<FSynOpticonActorStruct, bool> ActorData : SynOpticonActorsDataToCreateFrom)
	{
		ASynOpticonActor* NewActor = USynOpticonFactory::CreateSynOpticonActor(ActorData.Key, GetWorld(), NatNetWAMPComponent, EyeTrackingWAMPComponent, RetWAMPComponent, OpenFaceWAMPComponent, ActorData.Value, GazeVizBP, WebcamCompBP);

		ASynOpticonState::AddSOActor(NewActor);

		//TODO same as edit actor above
		//If this was the first actor created we let the HUD know so it can update 
		if (ASynOpticonState::GetSynOpticonActorList().Num() == 1) {
			ASynOpticonState::NextSynOpticonActor();
			ASynOpticonState::GetGlobalEventSystem()->OnSOActorChanged.Broadcast();		
		}
		ASynOpticonState::GetGlobalEventSystem()->OnEventCreateSOActor.Broadcast(NewActor->GetActorID());
	}

	SynOpticonActorsDataToCreateFrom.Empty();
}

void UCreationAndDeletionManager::DeleteSynOpticonActors()
{
	for (FString ActorName : SynOpticonActorsToDelete)
	{
		bool ActorRemoved = false;
		for (AActor* Actor : ASynOpticonState::GetSynOpticonActorList())
		{
			ASynOpticonActor* SynOpticonActor = Cast<ASynOpticonActor>(Actor);
			if (SynOpticonActor->GetActorName().Equals(ActorName))
			{
				bool CurrentActorWasDeleted = false;

				int32 index = ASynOpticonState::GetSynOpticonActorList().Find(SynOpticonActor);
				if (index == ASynOpticonState::GetCurrentActorIndex())
				{
					CurrentActorWasDeleted = true;
				}

				int32 ActorID = SynOpticonActor->GetActorID();
				ASynOpticonState::RemoveSOActor(SynOpticonActor);
				//SynOpticonActor->PlayerController->UnPossess();
				SynOpticonActor->K2_DestroyActor();
				ActorRemoved = true;
				SynOpticonActor = NULL;

				if (CurrentActorWasDeleted) {
					ASynOpticonState::NextSynOpticonActor();
				}

				ASynOpticonState::GetGlobalEventSystem()->OnEventRemoveSOActor.Broadcast(ActorID);

				break;
			}
		}
	}
	GEngine->ForceGarbageCollection(true);
	SynOpticonActorsToDelete.Empty();
}

bool UCreationAndDeletionManager::RemoveSynOpticonActor(FString actorName)
{
	SynOpticonActorsToDelete.Add(actorName);
	ShouldDeleteSynOpticonActors = true;
	return true;
}

void UCreationAndDeletionManager::RemoveAllSynOpticonActors()
{
	for (int i = ASynOpticonState::GetSynOpticonActorList().Num() - 1; i >= 0; i--)
	{
		ASynOpticonActor* Actor = Cast<ASynOpticonActor>(ASynOpticonState::GetSynOpticonActorList()[i]);
		RemoveSynOpticonActor(Actor->GetActorName());
	}
}

void UCreationAndDeletionManager::LoadRigidBodies(FString FileName)
{
	LoadedModelFile = FileName;

	FGazeObjectAndParentData AllGazeObjectAndParentData = GazeActorLoader::LoadResourceFromFile(FileName);

	FString Left;
	FString Right;
	FileName.Split(".", &Left, &Right, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	Left.Append(".tex");
	FSOReplayTextureStruct TextureDataStruct = UReplaySyncJSONExporter::GetReplayTextureData(Left); 

	for (FGazeParentActorData GazeParentData : AllGazeObjectAndParentData.GazeParentActorsData) {
		CreateGazeParent(GazeParentData);
	}

	for (FGazeGroupData GroupData : AllGazeObjectAndParentData.GazeGroupData)
	{
		ASynOpticonState::AddGazeGroup(GroupData.GroupName, GroupData.GroupID);
	}

	TArray<FGazeActorData> GazeActorsData = AllGazeObjectAndParentData.GazeActorsData;
	TArray<FGazeActorData> DuplicatedItems;
	for (FGazeActorData GazeActorData : GazeActorsData)
	{	
		GazeActorData.ID = -1;
		bool isDuplicated = false;
		for (AActor* Actor : ASynOpticonState::GetGazeActorList())
		{
			AGazeActor* GazeActor = Cast<AGazeActor>(Actor);
			if (GazeActor->GetName().Equals(GazeActorData.Name))
			{
				DuplicatedItems.Add(GazeActorData);
				isDuplicated = true;
				break;
			}
		}
		if (!isDuplicated)
		{
			for (FScreenTextureStruct ScreenTextureStruct : TextureDataStruct.ScreenTextureData) {
				if (GazeActorData.Name.Equals(ScreenTextureStruct.ScreenName)) {
					GazeActorData.TextureFilepath = ScreenTextureStruct.ScreenTexture;
					break;
				}
			}
			CreateRigidBody(GazeActorData);
		}
	}
	if (DuplicatedItems.Num() > 0)
	{
		ReplaceOrRenameDuplicatedGazeActors(DuplicatedItems);
	}

	ShouldDelayRegisterPosOri = true;


}

void UCreationAndDeletionManager::CreateAndReplaceRigidBody(FGazeActorData GazeActorData)
{
	for (AActor* Actor : ASynOpticonState::GetGazeActorList())
	{
		AGazeActor* GazeActor = Cast<AGazeActor>(Actor);
		if (GazeActor->GetName().Equals(GazeActorData.Name))
		{
			RemoveGazeActor(GazeActor);
			CreateRigidBody(GazeActorData);
			return;
		}
	}
}

void UCreationAndDeletionManager::CreateGazeParent(FGazeParentActorData GazeParentData)
{
	GazeParentActorsDataToCreateFrom.Add(GazeParentData);
	ShouldCreateGazeParentActors = true;
}

void UCreationAndDeletionManager::RemoveGazeParentActor(AGazeParentActor* GazeParent)
{
	GazeParentActorsToDelete.Add(GazeParent);
	ShouldDeleteGazeParentActors = true;
}

void UCreationAndDeletionManager::CreateRigidBody(FGazeActorData GazeActorData)
{
	GazeActorsDataToCreateFrom.Add(GazeActorData);
	ShouldCreateGazeActors = true;
}


//=============================================supporting functions for recording================================================
TArray<FGazeActorData> UCreationAndDeletionManager::PrepareActorsBeforeRecording(TArray<CompressedGazeBlock*> &GazeBlocks, TArray<CompressedSOBlock*> &SOBlocks)
{
	TArray<FGazeActorData> GazeActorDataStructs;
	int ScreenIndex = 0;
	for (AActor* Actor : ASynOpticonState::GetGazeActorList())
	{
		AGazeActor* GazeActor = Cast<AGazeActor>(Actor);
		GazeActorDataStructs.Add(GazeActor->GetDataStruct());

		GazeActor->FirstSave = false;

		CompressedGazeBlock* GazeBlock = new CompressedGazeBlock();
		GazeActor->SetCompressedGazeBlock(GazeBlock);
		GazeBlocks.Add(GazeBlock);

		GazeActor->PrepareForRecordAndReplay(ASynOpticonState::GetGazeActorList().Num(), ScreenIndex);
		ScreenIndex++;
	}

	for (AActor* Actor : ASynOpticonState::GetSynOpticonActorList())
	{
		ASynOpticonActor* SynOpticonActor = Cast<ASynOpticonActor>(Actor);

		CompressedSOBlock* SOBlock = new CompressedSOBlock();
		SynOpticonActor->SetCompressedSOBlock(SOBlock);
		SOBlocks.Add(SOBlock);
	}

	return GazeActorDataStructs;
}

TArray<FSynOpticonActorStruct> UCreationAndDeletionManager::StopActorsFromRecording()
{
	TArray<FSynOpticonActorStruct> SynOpticonActorDataStructs;
	for (AActor* Actor : ASynOpticonState::GetGazeActorList())
	{
		AGazeActor* GazeActor = Cast<AGazeActor>(Actor);
		GazeActor->StopRecordAndReplay();
	}
	for (AActor* Actor : ASynOpticonState::GetSynOpticonActorList())
	{
		ASynOpticonActor* SynOpticonActor = Cast<ASynOpticonActor>(Actor);
		SynOpticonActorDataStructs.Add(SynOpticonActor->GetDataStruct());
	}
	return SynOpticonActorDataStructs;
}

void UCreationAndDeletionManager::PrepareActorsBeforeReplaying(TArray<FGazeActorData> GAList, TArray<FGazeGroupData> GGList, TArray<FSynOpticonActorStruct> SOList, 
	TArray<CompressedGazeBlock*>* GazeBlocks, TArray<CompressedSOBlock*>* SOBlocks)
{
	//GazeActorsDataToCreateFrom.Empty();
	for (FGazeActorData GA : GAList)
	{
		GazeActorsDataToCreateFrom.Add(GA);
	}
	CreateGazeActors();
	if (GazeBlocks) 
	{
		for (AActor* Actor : ASynOpticonState::GetGazeActorList())
		{
			AGazeActor* GazeActor = Cast<AGazeActor>(Actor);
			CompressedGazeBlock* GazeBlock = new CompressedGazeBlock();
			GazeBlock->movable = GazeActor->IsMovable();
			GazeBlock->parented = GazeActor->IsParented();
			GazeActor->SetCompressedGazeBlock(GazeBlock);
			GazeActor->PrepareForRecordAndReplay(1, 1);
			GazeBlocks->Add(GazeBlock);
		}
	}

	for (FGazeGroupData GG : GGList)
	{
		ASynOpticonState::AddGazeGroup(GG.GroupName, GG.GroupID);
	}
	if (SOBlocks)
	{
		for (FSynOpticonActorStruct SO : SOList)
		{
			TPair<FSynOpticonActorStruct, bool> NewActor;
			NewActor.Key = SO;
			NewActor.Value = false;
			SynOpticonActorsDataToCreateFrom.Add(NewActor);
		}
		CreateSynOpticonActors();

		for (AActor* Actor : ASynOpticonState::GetSynOpticonActorList())
		{
			ASynOpticonActor* SynOpticonActor = Cast<ASynOpticonActor>(Actor);
			CompressedSOBlock* SOBlock = new CompressedSOBlock();
			SynOpticonActor->SetCompressedSOBlock(SOBlock);
			SOBlocks->Add(SOBlock);
		}
	}
}

void UCreationAndDeletionManager::StopActorsFromReplaying()
{
	for (AActor* Actor : ASynOpticonState::GetGazeActorList())
	{
		AGazeActor* GazeActor = Cast<AGazeActor>(Actor);
		GazeActor->StopRecordAndReplay();
	}
}