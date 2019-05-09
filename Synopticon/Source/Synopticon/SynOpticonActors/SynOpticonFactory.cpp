// Fill out your copyright notice in the Description page of Project Settings.

#include "SynOpticonFactory.h"
//#include "Synopticon.h"


int32 USynOpticonFactory::Index = 0;
bool USynOpticonFactory::ReplayMode = false;

ASynOpticonActor* USynOpticonFactory::CreateSynOpticonActor(FSynOpticonActorStruct SynOpticonActorStruct, UWorld* World, 
	UNatNetWAMPComponent* NatNetWAMPComponent, 
	UEyeTrackingWAMPComponent* EyeTrackingWAMPComponent, 
	URemoteTrackerWAMPComponent* RetWAMPComp, 
	UOpenFaceWAMPComponent* OpenFaceWAMPComponent,
	bool _ReplayMode, TSubclassOf<UGazeDataVisualizerComponent> GazeVizBP)
{
	USynOpticonFactory::ReplayMode = _ReplayMode;

	ASynOpticonActor* NewSynOpticonActor = (ASynOpticonActor*)World->SpawnActor(ASynOpticonActor::StaticClass());
	NewSynOpticonActor->Rename(*SynOpticonActorStruct.ActorName);
	NewSynOpticonActor->SetActorName(SynOpticonActorStruct.ActorName);
	NewSynOpticonActor->SetActorID(Index);
	Index++;

	bool hasPositionComponent = false;
	bool hasEyeTrackingComponent = false;
	bool hasHandComponent = false;
	bool hasRemoteTrackerComponent = false;
	bool hasVisionComponent = false;
	if (SynOpticonActorStruct.HandComponents.Num() > 0)
	{
		hasHandComponent = true;
	}

	//If the NatNetName is not empty we set the NatNetName of the NewSynOpticonActor
	if (SynOpticonActorStruct.NatNetName != "")
	{
		CreatePosOriComponent(NewSynOpticonActor, SynOpticonActorStruct, NatNetWAMPComponent);
		hasPositionComponent = true;
	}

	//If the remote tracker server address is not empty we create a remote tracker component
	if (SynOpticonActorStruct.RemoteEyeTrackerStruct.RemoteTrackerID != "")
	{
		CreateRemoteTrackerComponent(NewSynOpticonActor, SynOpticonActorStruct.RemoteEyeTrackerStruct, RetWAMPComp);
		hasRemoteTrackerComponent = true;
	}

	//If the value of the eye tracker adress is not empty we create a EyeTrackingVRPNComponent and make the NewSynOpticonActor the owner of the component
	if (SynOpticonActorStruct.EyeTrackerStruct.GlassesID != "")
	{
		CreateEyeVectorsComponent(NewSynOpticonActor, SynOpticonActorStruct.EyeTrackerStruct, EyeTrackingWAMPComponent);
		hasEyeTrackingComponent = true;
	}

	if (!SynOpticonActorStruct.VisualComponentData.OpenFaceID.Equals(""))
	{
		CreateVisualComponent(NewSynOpticonActor, SynOpticonActorStruct.VisualComponentData, OpenFaceWAMPComponent);
		hasVisionComponent = true;
	}

	//If the actor has a position tracking or eye tracking component we add a raycast component and an aggregated logging component
	if (hasPositionComponent || hasEyeTrackingComponent || hasHandComponent || hasRemoteTrackerComponent || hasVisionComponent)
	{
		if (hasEyeTrackingComponent || hasPositionComponent || hasVisionComponent) {
			CreateRaycastComponent(NewSynOpticonActor);
		}	
		CreateLogComponent(NewSynOpticonActor);
	}

	if (SynOpticonActorStruct.HasMyo)
	{
		CreateMyoInputComponent(NewSynOpticonActor);
	}

	int ShimmerIndex = 0;
	for (FShimmerComponentStruct ShimmerCompStruct : SynOpticonActorStruct.ShimmerComponents)
	{
		//If the value of the shimmer adress is not empty we create a UShimmerVRPNComponent and make the NewSynOpticonActor the owner of the component
		if (ShimmerCompStruct.DeviceName != "")
		{
			CreateShimmerComponent(NewSynOpticonActor, ShimmerCompStruct, ShimmerIndex);
			ShimmerIndex++;
		}
	}

	for (FHandStruct HandCompStruct : SynOpticonActorStruct.HandComponents)
	{
		UHandComponent* HandComponent = CreateHandComponent(NewSynOpticonActor, HandCompStruct, NatNetWAMPComponent);
		USynOpticonAggLogComponent* AggLogComponent = NewSynOpticonActor->FindComponentByClass<USynOpticonAggLogComponent>();

		if (HandComponent && AggLogComponent) {
			if (HandCompStruct.LeftHand) {
				AggLogComponent->SetLeftHandComponent(HandComponent);
			}
			else {
				AggLogComponent->SetRightHandComponent(HandComponent);
			}
		}
	}

	FString VisCompName = NewSynOpticonActor->GetActorName() + "GazeVizComponent";
	UGazeDataVisualizerComponent* VisComponent = NewObject<UGazeDataVisualizerComponent>(NewSynOpticonActor, GazeVizBP, FName(*VisCompName));
	if (VisComponent) {
		NewSynOpticonActor->SetGazeDataVisualizer(VisComponent);
		VisComponent->SetOwnerID(NewSynOpticonActor->GetActorID());
		VisComponent->RegisterComponent();
	}

	if (ASynOpticonState::IsReplaying())
	{
		USynOpticonAggLogComponent* LogComponent = NewSynOpticonActor->FindComponentByClass<USynOpticonAggLogComponent>();
		if (LogComponent)
		{
			for (FHandStruct HandStruct : SynOpticonActorStruct.HandComponents)
			{
				if (HandStruct.LeftHand)
					LogComponent->SetReplayLogComponentData(NewSynOpticonActor->GetActorID(), SynOpticonActorStruct.LeftHandLogComponentData, LoggingTypeEnum::LT_LeftHandData);
				else
					LogComponent->SetReplayLogComponentData(NewSynOpticonActor->GetActorID(), SynOpticonActorStruct.RightHandLogComponentData, LoggingTypeEnum::LT_RightHandData);
			}
			LogComponent->SetReplayLogComponentData(NewSynOpticonActor->GetActorID(), SynOpticonActorStruct.GazeLogComponentData, LoggingTypeEnum::LT_GazeData);
		}

		UGazeDataVisualizerComponent* GazeComponent = NewSynOpticonActor->FindComponentByClass<UGazeDataVisualizerComponent>();
		if (GazeComponent)
		{
			GazeComponent->SetReplayGazeData(SynOpticonActorStruct.GazeData);
		}
	}
	
	return NewSynOpticonActor;
}

void USynOpticonFactory::EditSynOpticonActor(ASynOpticonActor* Actor, FSynOpticonActorStruct SynOpticonActorStruct, UNatNetWAMPComponent* NatNetWAMPComponent, UEyeTrackingWAMPComponent* EyeTrackingWAMPComponent, URemoteTrackerWAMPComponent* RetWAMPComp)
{
	bool hasPosOriComponent = false;
	bool hasETComponent = false;

	//If the actor name was changed
	if (!SynOpticonActorStruct.ActorName.Equals(Actor->GetActorName()))
	{
		//Rename the Actor to the new name
		Actor->Rename(*SynOpticonActorStruct.ActorName);
		Actor->SetActorName(SynOpticonActorStruct.ActorName);
	}

	//Edit the remote eye tracking component
	EditRemoteTrackerComponent(Actor, SynOpticonActorStruct.RemoteEyeTrackerStruct, RetWAMPComp);
	//Edit the position and orientation component
	hasPosOriComponent = EditPosOriComponent(Actor, SynOpticonActorStruct, NatNetWAMPComponent);
	//Edit the eye tracking components
	hasETComponent = EditEyeVectorsComponent(Actor, SynOpticonActorStruct.EyeTrackerStruct, EyeTrackingWAMPComponent);
	//Edit the Myo Component
	EditMyoInputComponent(Actor, SynOpticonActorStruct);
	//Edit the Shimmer Component(s)
	EditShimmerComponents(Actor, SynOpticonActorStruct);
	//Edit the Raycast and Logging components
	EditRaycastAndLogComponent(Actor, hasETComponent, hasPosOriComponent);

	//if (hasPosOriComponent)
	//{
	//	Actor->SetupCameraAttachement();
	//}
}

UHandComponent* USynOpticonFactory::CreateHandComponent(ASynOpticonActor* NewSynOpticonActor, FHandStruct HandCompStruct, UNatNetWAMPComponent* NatNetWAMPComponent) {
	FString leftHandName = HandCompStruct.LeftHand ? "Left" : "Right";
	FString HandCompName = NewSynOpticonActor->GetActorName() + leftHandName + "HandComponent";
	
	UHandComponent* HandComponent = NewObject<UHandComponent>(NewSynOpticonActor, UHandComponent::StaticClass(), FName(*HandCompName));
	if (!HandComponent) {
		return nullptr;
	}

	HandComponent->LeftHand = HandCompStruct.LeftHand;
	HandComponent->SetComponentData(HandCompStruct);
	HandComponent->AttachToComponent(NewSynOpticonActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	UPositionAndOrientationComponent* PosOriComponent = NewObject<UPositionAndOrientationComponent>(NewSynOpticonActor, UPositionAndOrientationComponent::StaticClass(), FName(*(HandCompName + "PosOriComponent")));
	if (PosOriComponent)
	{
		PosOriComponent->SetOwnerRigidBodyName(HandCompStruct.NatNetName);
		PosOriComponent->SetNatNetWAMPComponent(NatNetWAMPComponent);
		PosOriComponent->SetMarkersConfiguration(HandCompStruct.MarkersConfiguration);
		PosOriComponent->HideOrientationArrowMesh();
		PosOriComponent->RegisterComponent();
		HandComponent->SetPosOriComponent(PosOriComponent);
	}

	HandComponent->RegisterComponent();
	return HandComponent;
}

void USynOpticonFactory::CreateRemoteTrackerComponent(ASynOpticonActor* NewSynOpticonActor, FRemoteEyeTrackerStruct RemoteEyeTrackerStruct, URemoteTrackerWAMPComponent* RetWAMPComp)
{
	URemoteEyeTrackerComponent* RemoteTrackerComponent = NewObject<URemoteEyeTrackerComponent>(NewSynOpticonActor, URemoteEyeTrackerComponent::StaticClass(), FName(*(NewSynOpticonActor->GetActorName() + "RemoteTrackerComponent")));
	if (RemoteTrackerComponent)
	{
		RemoteTrackerComponent->SetRETDataStruct(RemoteEyeTrackerStruct);
		RemoteTrackerComponent->SetRetWAMPComponent(RetWAMPComp);
		RemoteTrackerComponent->SetOwnerID(NewSynOpticonActor->GetActorID());
		RemoteTrackerComponent->RegisterComponent();
		
		//NewSynOpticonActor->SetupCameraAttachement(RemoteTrackerComponent->GetRootNode());
		NewSynOpticonActor->SetupCameraAttachement(nullptr);
	}
}

void USynOpticonFactory::CreateEyeVectorsComponent(ASynOpticonActor* NewSynOpticonActor, FEyeTrackerStruct EyeTrackerStruct, UEyeTrackingWAMPComponent* EyeTrackingWAMPComponent)
{
	UEyeVectorsComponent* EyeVectorsComponent = NewObject<UEyeVectorsComponent>(NewSynOpticonActor, UEyeVectorsComponent::StaticClass(), FName(*(NewSynOpticonActor->GetActorName() + "EyeVectorsComponent")));
	if (EyeVectorsComponent)
	{
		if (EyeTrackingWAMPComponent) {
			EyeVectorsComponent->SetGlassesID(EyeTrackerStruct.GlassesID);
		}

		UGazeInputComponent* GazeInputComponent = NewObject<UGazeInputComponent>(EyeVectorsComponent, UGazeInputComponent::StaticClass(), FName(*(NewSynOpticonActor->GetActorName() + "GazeInputComponent")));
		if (GazeInputComponent)
		{
			GazeInputComponent->RegisterComponent();
		}

		EyeVectorsComponent->SetETDataStruct(EyeTrackerStruct);
		EyeVectorsComponent->SetEyeTrackingWAMPComponent(EyeTrackingWAMPComponent);
		EyeVectorsComponent->SetOwnerID(NewSynOpticonActor->GetActorID());
		EyeVectorsComponent->RegisterComponent();	
		NewSynOpticonActor->SetupCameraAttachement(EyeVectorsComponent);
	}

	//If the NewSynOpticonActor has a position and orientation component, notify it that the actor also has an ETG component so 
	//it can use the correct way to set the orientation of the actor
	UPositionAndOrientationComponent* PosOriComponent = NewSynOpticonActor->FindComponentByClass<UPositionAndOrientationComponent>();
	if (PosOriComponent)
	{
		PosOriComponent->SetActorHasETG(true);
		PosOriComponent->SetActorLocationOffset(ASynOpticonState::GetGlassesOffset());
		PosOriComponent->SetShouldApplyOffset(true);

		EyeVectorsComponent->AttachToComponent(PosOriComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		EyeVectorsComponent->SetOuterPosOriComponent(PosOriComponent);
	}
}

void USynOpticonFactory::CreatePosOriComponent(ASynOpticonActor* NewSynOpticonActor, FSynOpticonActorStruct SynOpticonActorStruct, UNatNetWAMPComponent* NatNetWAMPComponent)
{
	UPositionAndOrientationComponent* PosOriComponent = NewObject<UPositionAndOrientationComponent>(NewSynOpticonActor, UPositionAndOrientationComponent::StaticClass(), FName(*(NewSynOpticonActor->GetActorName() + "PosOriComponent")));
	if (PosOriComponent)
	{
		PosOriComponent->SetOwnerRigidBodyName(SynOpticonActorStruct.NatNetName);
		PosOriComponent->SetNatNetWAMPComponent(NatNetWAMPComponent);
		PosOriComponent->SetMarkersConfiguration(SynOpticonActorStruct.MarkersConfiguration);
		PosOriComponent->AttachToComponent(NewSynOpticonActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		PosOriComponent->RegisterComponent();

		NewSynOpticonActor->SetupCameraAttachement(PosOriComponent);
	}
}

void USynOpticonFactory::CreateMyoInputComponent(ASynOpticonActor* NewSynOpticonActor)
{
	//UMyoInputComponent* MyoInputComponent = NewObject<UMyoInputComponent>(NewSynOpticonActor, UMyoInputComponent::StaticClass(), FName(*(NewSynOpticonActor->GetActorName() + "MyoInputComponent")));
	//if (MyoInputComponent)
	//{
	//	MyoInputComponent->RegisterComponent();
	//}
}

void USynOpticonFactory::CreateVisualComponent(ASynOpticonActor* NewSynOpticonActor, FOpenFaceStruct VisualComponentStruct, UOpenFaceWAMPComponent* OpenFaceWAMPComponent)
{
	UOpenFaceComponent* OpenFaceComponent = NewObject<UOpenFaceComponent>(NewSynOpticonActor, UOpenFaceComponent::StaticClass(), FName(*(NewSynOpticonActor->GetActorName() + "OpenFaceComponent")));
	if (OpenFaceComponent)
	{
		OpenFaceComponent->SetOpenFaceStruct(VisualComponentStruct);
		OpenFaceComponent->SetOpenFaceWAMPComponent(OpenFaceWAMPComponent);
		OpenFaceComponent->SetOwnerID(NewSynOpticonActor->GetActorID());
		OpenFaceComponent->RegisterComponent();

		NewSynOpticonActor->SetupCameraAttachement(OpenFaceComponent);
	}
}

void USynOpticonFactory::CreateShimmerComponent(ASynOpticonActor* NewSynOpticonActor, FShimmerComponentStruct ShimmerCompStruct, int ShimmerIndex)
{
	//Create a UShimmerVRPNComponent and make the NewSynOpticonActor the owner of the component
	UShimmerVRPNComponent* ShimmerComponent = NewObject<UShimmerVRPNComponent>(NewSynOpticonActor, UShimmerVRPNComponent::StaticClass(), FName(*(NewSynOpticonActor->GetActorName().Append("ShimmerVRPNComponent" + ShimmerIndex))));
	//If the creation of the component was succsessful we register it with the Unreal Engine
	if (ShimmerComponent)
	{
		ShimmerComponent->SetHREnabled(ShimmerCompStruct.HasHeartRate);
		ShimmerComponent->SetGSREnabled(ShimmerCompStruct.HasGalvanicSkinResponse);
		ShimmerComponent->SetShimmerIndex(ShimmerIndex);
		ShimmerComponent->SetShimmerDeviceName(ShimmerCompStruct.DeviceName);

		//if (!USynOpticonFactory::ReplayMode)
		//{
			//ShimmerComponent->SetUpVRPNClient();
		//}

		ShimmerComponent->RegisterComponent();
	}
}

void USynOpticonFactory::CreateRaycastComponent(ASynOpticonActor* NewSynOpticonActor)
{
	bool CanRaycast = false;

	URaycastComponent_Ours* RaycastComponent = NewObject<URaycastComponent_Ours>(NewSynOpticonActor, URaycastComponent_Ours::StaticClass(), FName(*(NewSynOpticonActor->GetActorName() + "RaycastComponent")));
	if (RaycastComponent)
	{
		RaycastComponent->SetOwnerName(NewSynOpticonActor->GetActorName());

		UEyeVectorsComponent* EyeComp = NewSynOpticonActor->FindComponentByClass<UEyeVectorsComponent>();
		if (EyeComp) {
			RaycastComponent->SetVectorSceneComponent(EyeComp->GetConvergenceVectorSceneComponent());
			CanRaycast = true;
		}
		else {
			UOpenFaceComponent* VisionComp = NewSynOpticonActor->FindComponentByClass<UOpenFaceComponent>();
			if (VisionComp) {
				RaycastComponent->SetVectorSceneComponent(VisionComp->GetRaycastSceneComponent());
				CanRaycast = true;
			}
			else {
				TArray<UActorComponent*> PosOriComps = NewSynOpticonActor->GetComponentsByClass(UPositionAndOrientationComponent::StaticClass());
				for (UActorComponent* PosComp : PosOriComps) {
					if (!PosComp->GetName().Contains("Hand")) {
						UPositionAndOrientationComponent* RaySceneComp = Cast<UPositionAndOrientationComponent>(PosComp);
						RaycastComponent->SetVectorSceneComponent(RaySceneComp);
						CanRaycast = true;
						break;
					}
				}
			}
		}

		RaycastComponent->RegisterComponent();
	}

	NewSynOpticonActor->SetCanRaycast(CanRaycast);
}

void USynOpticonFactory::CreateLogComponent(ASynOpticonActor* NewSynOpticonActor) {
	USynOpticonAggLogComponent* AggLogComponent = NewObject<USynOpticonAggLogComponent>(NewSynOpticonActor, USynOpticonAggLogComponent::StaticClass(), FName(*(NewSynOpticonActor->GetActorName() + "AggLogComponent")));
	if (AggLogComponent)
	{
		AggLogComponent->SetOwnerID(NewSynOpticonActor->GetActorID());

		URaycastComponent_Ours* RaycastComponent = NewSynOpticonActor->FindComponentByClass<URaycastComponent_Ours>();
		if (RaycastComponent) {
			AggLogComponent->SetRaycastComponent(RaycastComponent);
		}		

		URemoteEyeTrackerComponent* RemoteTrackerComponent = NewSynOpticonActor->FindComponentByClass<URemoteEyeTrackerComponent>();
		if (RemoteTrackerComponent) {
			AggLogComponent->SetRemoteTrackerComponent(RemoteTrackerComponent);
		}

		AggLogComponent->RegisterComponent();
	}
}

bool USynOpticonFactory::EditRemoteTrackerComponent(ASynOpticonActor* Actor, FRemoteEyeTrackerStruct RemoteEyeTrackerStruct, URemoteTrackerWAMPComponent* RetWAMPComp)
{
	bool hasETComponent = false;
	//Check to see if the Actor already has a remote tracker component
	//URemoteEyeTrackerComponent* RETComponent = Actor->FindComponentByClass<URemoteEyeTrackerComponent>();
	//if (RETComponent) //If it does, check the struct to see if it is the same or if it has been removed
	//{
	//	if (RemoteEyeTrackerStruct.RemoteTrackerID.Equals("")) //If the server ip address is empty in the struct the component has been removed
	//	{
	//		RETComponent->DestroyComponent(true);
	//	}
	//	else //Otherwise we check if the server ip address is the same as before, if it is we don't do anything
	//	{
	//		/*FRemoteEyeTrackerStruct OriginalETData = RETComponent->GetRETData();

	//		bool IpAddressChanged = !OriginalETData.ServerIPAddress.Equals(OriginalETData.ServerIPAddress);
	//		bool GazeDataChanged = !OriginalETData.GetGazeDataAddress().Equals(OriginalETData.GetGazeDataAddress());

	//		if (IpAddressChanged || GazeDataChanged)
	//		{
	//			RETComponent->SetRETDataStruct(RemoteEyeTrackerStruct);

	//			UEyeTrackingVRPNComponent* ETVRPNComponent = RETComponent->GetEyeTrackingVRPNComponent();
	//			if (ETVRPNComponent)
	//			{
	//				ETVRPNComponent->EstablishNewConnection(RemoteEyeTrackerStruct.GetGazeDataAddress());
	//			}
	//		}
	//		else if(!RETComponent->GetRETData().AttachedScreenName.Equals(RemoteEyeTrackerStruct.AttachedScreenName))
	//		{
	//			RETComponent->SetRETDataStruct(RemoteEyeTrackerStruct);
	//		}*/

	//		hasETComponent = true;
	//	}		
	//}
	//else //Otherwise check if the component should be created
	//{
	//	if (!RemoteEyeTrackerStruct.RemoteTrackerID.Equals(""))
	//	{
	//		CreateRemoteTrackerComponent(Actor, RemoteEyeTrackerStruct);
	//		hasETComponent = true;
	//	}
	//}

	return hasETComponent;
}

bool USynOpticonFactory::EditEyeVectorsComponent(ASynOpticonActor* Actor, FEyeTrackerStruct EyeTrackerStruct, UEyeTrackingWAMPComponent* EyeTrackingWAMPComponent)
{
	bool hasETComponent = false;
	//Check to see if the Actor already has a Position nad Orientation component
	UEyeVectorsComponent* ETComponent = Actor->FindComponentByClass<UEyeVectorsComponent>();
	if (ETComponent) //If it does, check the struct to see if it is the same or if it has been removed
	{
		if (EyeTrackerStruct.GlassesID.Equals("")) //If the server ip address is empty in the struct the component has been removed
		{
			ETComponent->DestroyComponent(true);  //TODO check if the glasses are correct if the actor still has an et component after the removal of the pos component
		}
		else //Otherwise we check if the server ip address is the same as before, if it is we don't do anything
		{
			ETComponent->SetGlassesID(EyeTrackerStruct.GlassesID);

			hasETComponent = true;
		}
	}
	else //Otherwise check if the component should be created
	{
		if (!EyeTrackerStruct.GlassesID.Equals(""))
		{
			CreateEyeVectorsComponent(Actor, EyeTrackerStruct, EyeTrackingWAMPComponent);
			hasETComponent = true;
		}
	}

	return hasETComponent;
}

bool USynOpticonFactory::EditPosOriComponent(ASynOpticonActor* Actor, FSynOpticonActorStruct SynOpticonActorStruct, UNatNetWAMPComponent* NatNetWAMPComponent)
{
	bool hasPosOriComponent = false;
	//Check to see if the Actor already has a Position nad Orientation component
	UPositionAndOrientationComponent* PosOriComponent = Actor->FindComponentByClass<UPositionAndOrientationComponent>();
	//USynOpticonAggLogComponent* AggLogComponent = Actor->FindComponentByClass<USynOpticonAggLogComponent>();
	if (PosOriComponent) //If it does, check the struct to see if it is the same or if it has been removed
	{
		if (SynOpticonActorStruct.NatNetName.Equals("")) //If the name is empty in the struct the component has been removed
		{
			PosOriComponent->DestroyComponent(true);  //TODO check if the glasses are correct if the actor still has an et component after the removal of the pos component. Due to change of root component
		}
		else //Otherwise we check if the name is the same as before, if it is we don't do anything
		{
			//If the name is different we have to rename the rigid body and update the natnet component with the new name
			if (!PosOriComponent->GetRigidBodyName().Equals(SynOpticonActorStruct.NatNetName))
			{
				PosOriComponent->SetOwnerRigidBodyName(SynOpticonActorStruct.NatNetName);
			}

			hasPosOriComponent = true;
		}
	}
	else //Otherwise check if the component should be created
	{
		if (!SynOpticonActorStruct.NatNetName.Equals(""))
		{
			CreatePosOriComponent(Actor, SynOpticonActorStruct, NatNetWAMPComponent);
			hasPosOriComponent = true;
		}
	}

	return hasPosOriComponent;
}

void USynOpticonFactory::EditMyoInputComponent(ASynOpticonActor* Actor, FSynOpticonActorStruct SynOpticonActorStruct)
{
	//UMyoInputComponent* MyoInputComponent = Actor->FindComponentByClass<UMyoInputComponent>();

	//if (MyoInputComponent) //If it does, check the struct to see if it is the same or if it has been removed
	//{
	//	if (!SynOpticonActorStruct.HasMyo) //If it is false then the component was removed
	//	{
	//		MyoInputComponent->DestroyComponent(true); 
	//	}
	//	else //Otherwise we check if the name is the same as before, if it is we don't do anything
	//	{
	//		//If the name is different we have to update the name
	//		//TODO currently MyoComponent has no name or identifier for different actors
	//	}
	//}
	//else //Otherwise check if the component should be created
	//{
	//	if (SynOpticonActorStruct.HasMyo)
	//	{
	//		CreateMyoInputComponent(Actor);
	//	}
	//}
}

void USynOpticonFactory::EditShimmerComponents(ASynOpticonActor* Actor, FSynOpticonActorStruct SynOpticonActorStruct)
{
	//Get the shimmer components attached to the Actor
	TArray<UShimmerVRPNComponent*> ShimmerComponents;
	Actor->GetComponents(ShimmerComponents);

	bool HasHR = false;
	bool HasGSR = false;
	
	bool HRCreated = false;
	bool GSRCreated = false;

	//We can only have a maximum of 2 shimmers per actor
	for (int32 CurrentIndex = 0; CurrentIndex < 2; CurrentIndex++)
	{
		UShimmerVRPNComponent* ShimmerComponentMatch = nullptr;
		FShimmerComponentStruct ShimmerStructMatch;

		//Find the shimmer component that has the current index
		for (UShimmerVRPNComponent* ShimmerComponent : ShimmerComponents)
		{
			if (ShimmerComponent->GetShimmerIndex() == CurrentIndex)
			{
				ShimmerComponentMatch = ShimmerComponent;
			}
		}

		//Find the shimmer struct that has the current index
		for (FShimmerComponentStruct ShimmerComponentStruct : SynOpticonActorStruct.ShimmerComponents)
		{
			if (ShimmerComponentStruct.ShimmerIndex == CurrentIndex)
			{
				ShimmerStructMatch = ShimmerComponentStruct;
			}
		}

		//If both lists contains the same index we need to check if the data has changed
		if (ShimmerComponentMatch && ShimmerStructMatch.ShimmerIndex != -1)
		{
			//The address was changed
			ShimmerComponentMatch->SetShimmerDeviceName(ShimmerStructMatch.DeviceName);

			if (!HasHR && ShimmerStructMatch.HasHeartRate)
			{
				HasHR = true;
			}

			if (!HasGSR && ShimmerStructMatch.HasGalvanicSkinResponse)
			{
				HasGSR = true;
			}

			//Set the HeartRate flag to match the struct
			ShimmerComponentMatch->SetHREnabled(ShimmerStructMatch.HasHeartRate);

			//Set the GSR flag to match the struct
			ShimmerComponentMatch->SetGSREnabled(ShimmerStructMatch.HasGalvanicSkinResponse);
		}
		//If only the shimmer component was a match the component was removed
		else if (ShimmerComponentMatch)
		{			
			ShimmerComponentMatch->DestroyComponent();		
		}
		//If only the shimmer struct was a match a new component should be created
		else if (ShimmerStructMatch.ShimmerIndex != -1)
		{
			CreateShimmerComponent(Actor, ShimmerStructMatch, CurrentIndex);
		}
	}
}

void USynOpticonFactory::EditRaycastAndLogComponent(ASynOpticonActor* Actor, bool hasEyeTrackingComponent, bool hasPosComponent)
{
	URaycastComponent_Ours* RaycastComponent = Actor->FindComponentByClass<URaycastComponent_Ours>();
	UHandComponent* HandComponent = Actor->FindComponentByClass<UHandComponent>();

	//If the actor already has a raycast component. If it has a raycast component it already has a log component so we don't need to do anything with the log component
	if (RaycastComponent)
	{

		//If the actor has et or pos component we just update the raycast vector in case one of the components was removed or another was added
		if (hasEyeTrackingComponent || hasPosComponent)
		{
			//Update so it uses the correct vector for raycasting
			if (hasEyeTrackingComponent)
			{
				RaycastComponent->SetVectorSceneComponent(Actor->FindComponentByClass<UEyeVectorsComponent>()->GetConvergenceVectorSceneComponent());
			}
			else
			{
				RaycastComponent->SetVectorSceneComponent(Actor->GetRootComponent());
			}
		}
		//If the actor has neither we remove the raycast component as well as the log component
		else
		{
			USynOpticonAggLogComponent* LogComponent = Actor->FindComponentByClass<USynOpticonAggLogComponent>();
			LogComponent->DestroyComponent();
			RaycastComponent->DestroyComponent();
			Actor->SetCanRaycast(false);
		}
	}
	//Otherwise we create one if it should have one
	else
	{
		if (hasEyeTrackingComponent || hasPosComponent || HandComponent)
		{
			if (hasEyeTrackingComponent || hasPosComponent) {
				CreateRaycastComponent(Actor);
			}

			CreateLogComponent(Actor);
		}	
	}
}