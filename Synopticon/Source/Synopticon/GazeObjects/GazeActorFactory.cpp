// Fill out your copyright notice in the Description page of Project Settings.

#include "GazeActorFactory.h"
//#include "Synopticon.h"


bool UGazeActorFactory::InstanceFlag = false;
UGazeActorFactory* UGazeActorFactory::Instance = nullptr;
int32 UGazeActorFactory::Index = 0;
int32 UGazeActorFactory::NextUniqueAOIId = 500000;

UGazeActorFactory::UGazeActorFactory()
{

}

UGazeActorFactory::~UGazeActorFactory()
{
	InstanceFlag = false;
}


UGazeActorFactory* UGazeActorFactory::GetInstance()
{
	if (!InstanceFlag)
	{
		Instance = NewObject<UGazeActorFactory>();
		InstanceFlag = true;

		return Instance;
	}
	else
	{
		return Instance;
	}
}

AGazeActor* UGazeActorFactory::CreateGazeActor(FGazeActorData GazeActorData, UWorld* World, TSubclassOf<AGazeActor> GazeActorBP)
{
	AGazeActor* NewGazeActor = nullptr;

	if (GazeActorBP)
	{
		NewGazeActor = World->SpawnActor<AGazeActor>(GazeActorBP);
	}

	if (NewGazeActor)
	{
		NewGazeActor->Rename(*GazeActorData.Name);
		NewGazeActor->SetName(GazeActorData.Name);
		NewGazeActor->SetURL(GazeActorData.URL);
		NewGazeActor->SetWebBrowserResolution(GazeActorData.WebBrowserResolution);
		NewGazeActor->SetID(GetValidID());
		NewGazeActor->SetButtonMeshName(GazeActorData.ButtonMesh);
		NewGazeActor->SetSplineMeshName(GazeActorData.SplineMesh);
		int32 FontSize = GazeActorData.FontSize > 0 ? GazeActorData.FontSize : 12;
		NewGazeActor->SetFontSize(FontSize);
		NewGazeActor->SetGazeActorType(GazeActorData.GazeActorType);
		NewGazeActor->SetGrouped(GazeActorData.Grouped);
		NewGazeActor->SetWAMPScreenID(GazeActorData.WAMPScreenID);

		//Check for uniqueness //TODO this is not taking into consideration the other AOIS in the list.
		for (int i = 0; i < GazeActorData.AOIS.Num(); i++) {
			if (!UGazeActorFactory::AOIIDIsUnique(GazeActorData.AOIS[i].ID)) {
				GazeActorData.AOIS[i].ID = GetValidAOIID();
			}
		}
		NewGazeActor->SetAOIS(false, GazeActorData.AOIS);

		TArray<FVector> Vertices;
		TArray<int32> TriangleIndices;
		TArray<FVector2D> UVs;

		TArray<FVector> OutsideVertices;
		TArray<int32> OutsideTriangleIndices;
		TArray<FVector2D> OutsideUVs;

		NewGazeActor->SetParented(GazeActorData.Parented);
		if (GazeActorData.Parented && !ASynOpticonState::IsReplaying() && !ASynOpticonState::GetScreenLabeling()) {
			AGazeParentActor* GazeActorParent = nullptr;
			
			for (AActor* Parent : ASynOpticonState::GetGazeParentActorList()) {
				AGazeParentActor* GazeParent = Cast<AGazeParentActor>(Parent);
				if (GazeActorData.ParentSceneCompName.Equals(GazeParent->GetSceneCompName())) {
					GazeActorParent = GazeParent;
					GazeActorParent->AddAttachedObject(NewGazeActor);
					break;
				}
			}

			if (GazeActorParent) 
			{
				NewGazeActor->SetGazeParentActor(GazeActorParent, GazeActorParent->GetSceneCompName());
			}
		}

		if (GazeActorData.Movable)
		{
			NewGazeActor->SetMovable(true);
			NewGazeActor->SetNatNetName(GazeActorData.NatNetName);

			UNatNetWAMPComponent* NatNetWAMPComponent = nullptr;
			if (World) {
				AGameModeBase* GameMode = World->GetAuthGameMode();
				if (GameMode) {
					NatNetWAMPComponent = GameMode->FindComponentByClass<UNatNetWAMPComponent>();
				}			
			}			

			UPositionAndOrientationComponent* PosOriComponent = NewObject<UPositionAndOrientationComponent>(NewGazeActor, UPositionAndOrientationComponent::StaticClass(), FName(*(GazeActorData.Name + "PosOriComponent")));
			if (PosOriComponent)
			{
				PosOriComponent->SetOwnerRigidBodyName(GazeActorData.NatNetName);
				PosOriComponent->SetNatNetWAMPComponent(NatNetWAMPComponent);
				PosOriComponent->SetMarkersConfiguration(MarkersConfigurationEnum::VE_PositionOnly);
				PosOriComponent->HideOrientationArrowMesh();

				NewGazeActor->SetRootComponent(PosOriComponent);
				PosOriComponent->RegisterComponent();
			}

			USynOpticonStatics::CreateBoxGeometry(GazeActorData.SizeDimensions.X, GazeActorData.SizeDimensions.Y, GazeActorData.SizeDimensions.Z, Vertices, TriangleIndices, OutsideVertices, OutsideTriangleIndices, OutsideUVs);

			NewGazeActor->SetDimensions(GazeActorData.SizeDimensions);
			NewGazeActor->SetOutsideVerticies(OutsideVertices);
			NewGazeActor->SetOutsideTriangleIndexes(OutsideTriangleIndices);
			NewGazeActor->SetOutsideUVs(OutsideUVs);
		}
		else
		{
			if (GazeActorData.Markers.Num() > 0) {
				if (GazeActorData.GazeActorType == EStaticRoomActorType::SRT_Screen)
				{
					Vertices = USynOpticonStatics::CalRigidBodyWH(GazeActorData.Markers[0], GazeActorData.Markers[1], GazeActorData.Markers[2]);
					TriangleIndices.Add(2);
					TriangleIndices.Add(1);
					TriangleIndices.Add(0);
					TriangleIndices.Add(0);
					TriangleIndices.Add(3);
					TriangleIndices.Add(2);
					UVs.Add(FVector2D(0, 1));
					UVs.Add(FVector2D(0, 0));
					UVs.Add(FVector2D(1, 0));
					UVs.Add(FVector2D(1, 1));
				}
				
				else if (GazeActorData.GazeActorType == EStaticRoomActorType::SRT_CurvedScreen)
				{
					Vertices = GazeActorData.Markers;
					for (int i = 2; i < GazeActorData.Markers.Num() - 1; i += 2)
					{
						TriangleIndices.Add(i);
						TriangleIndices.Add(i - 2);
						TriangleIndices.Add(i - 1);
						TriangleIndices.Add(i - 1);
						TriangleIndices.Add(i + 1);
						TriangleIndices.Add(i);
					}

					UVs = USynOpticonStatics::CreateUVsForCurvedScreen(Vertices);
				}
			
				else if (GazeActorData.GazeActorType == EStaticRoomActorType::SRT_ComplexObject) 
				{
					Vertices = GazeActorData.Markers;
					int32 numOfTriangles = USynOpticonStatics::Triangulate(Vertices, TriangleIndices);
					if (numOfTriangles <= 0) {
						NewGazeActor->K2_DestroyActor();
						NewGazeActor = nullptr;
						return NewGazeActor;
					}

					UVUnwrapping(Vertices, UVs);
				}

				else if (GazeActorData.GazeActorType == EStaticRoomActorType::SRT_ActionObject)
				{
					Vertices = GazeActorData.Markers;
				}
			}
			else {

				Vertices = GazeActorData.Vertices;
				TriangleIndices = GazeActorData.Indexes;
				UVs = GazeActorData.UVs;

				//This will recreate the uvs of complex objects
				//if (GazeActorData.GazeActorType == EStaticRoomActorType::SRT_ComplexObject)
				//{
				//	UVs.Empty();
				//	Vertices = GazeActorData.Vertices;
				//	int32 numOfTriangles = Triangulate(Vertices, TriangleIndices);
				//	if (numOfTriangles <= 0) {
				//		NewGazeActor->K2_DestroyActor();
				//		NewGazeActor = nullptr;
				//		return NewGazeActor;
				//	}
				//	UVUnwrapping(Vertices, UVs);
				//}
			}
		}

		NewGazeActor->SetVerticies(Vertices);
		NewGazeActor->SetTriangleIndexes(TriangleIndices);
		NewGazeActor->SetUVs(UVs);
		NewGazeActor->SetIsGazeObject(GazeActorData.IsGazeObject);
		NewGazeActor->SetIsCollisionObject(GazeActorData.IsCollisionObject);

		NewGazeActor->CreateMeshInBP(GazeActorData.Movable, true);
		NewGazeActor->SetupMeshCollisionInBP();

		UGazeObjectMaterialComponent* GazeMaterialComponent = NewObject<UGazeObjectMaterialComponent>(NewGazeActor, UGazeObjectMaterialComponent::StaticClass(), FName(*(GazeActorData.Name + "GazeMaterialComponent")));
		if (GazeMaterialComponent) {
			GazeMaterialComponent->SetOwnerID(NewGazeActor->GetID());
			GazeMaterialComponent->RegisterComponent();

			if (GazeActorData.VideoIndex != -1) {
				FSyncReplayStruct ReplaySyncData = ASynOpticonState::ReplaySyncData;
				GazeMaterialComponent->SetupVideoTexture(ReplaySyncData.RecordingSyncData.NmbCols, ReplaySyncData.RecordingSyncData.NmbRows, GazeActorData.VideoIndex);
			}
			else if (!GazeActorData.TextureFilepath.Equals("No Texture")) {
				GazeMaterialComponent->SetupScreenTexture(GazeActorData.TextureFilepath);
			}
		}

		
	}

	return NewGazeActor;
}

AGazeActor* UGazeActorFactory::CreateGazeActorWithUVs(FSlicingData GazeActorData, UWorld* World, TSubclassOf<AGazeActor> GazeActorBP)
{
	AGazeActor* NewGazeActor = nullptr;

	if (GazeActorBP)
	{
		NewGazeActor = World->SpawnActor<AGazeActor>(GazeActorBP);
	}

	if (NewGazeActor)
	{
		NewGazeActor->SetID(GetValidID());
		NewGazeActor->SetFontSize(12);
		NewGazeActor->SetVerticies(GazeActorData.Vertices);
		NewGazeActor->SetTriangleIndexes(GazeActorData.Indexes);
		NewGazeActor->SetUVs(GazeActorData.UVs);
		NewGazeActor->CreateMeshInBP(false, true);

		UGazeObjectMaterialComponent* GazeMaterialComponent = NewObject<UGazeObjectMaterialComponent>(NewGazeActor, UGazeObjectMaterialComponent::StaticClass(), FName(*FString("GazeMaterialComponent")));
		if (GazeMaterialComponent) {
			GazeMaterialComponent->SetOwnerID(NewGazeActor->GetID());
			GazeMaterialComponent->RegisterComponent();
		}
	}

	return NewGazeActor;
}

void UGazeActorFactory::UVUnwrapping(TArray<FVector> vertices, TArray<FVector2D> &OutUVs)
{
	FVector Min;
	FVector Max;
	USynOpticonStatics::CalculateBoundingBox(vertices, Min, Max);
	TArray<FVector> UVWVertices = NormalizeBB(Min,Max,vertices);

	FPoly polygon;
	for (int i = 0; i < UVWVertices.Num(); i++)
	{
		FVector v = UVWVertices[i];
		polygon.InsertVertex(i, FVector(v.X, v.Y, v.Z));
	}

	//Choose unwrapping axis based on the dominant axis of the objects boundingbox
	FVector BoundsRanges = Max - Min;
	if (BoundsRanges.X > BoundsRanges.Z && BoundsRanges.Y > BoundsRanges.Z) {
		for (FVector v : UVWVertices)
		{
			OutUVs.Add(FVector2D(v.X, v.Y));
		}
	}
	else if (BoundsRanges.Y > BoundsRanges.X && BoundsRanges.Z > BoundsRanges.X) {
		for (FVector v : UVWVertices)
		{
			OutUVs.Add(FVector2D(v.Y, v.Z));
		}
	}
	else { //(BoundsRanges.X > BoundsRanges.Y && BoundsRanges.Z > BoundsRanges.Y)
		for (FVector v : UVWVertices)
		{
			OutUVs.Add(FVector2D(v.X, v.Z));
		}
	}
	
	//Choose unwrapping axis based on the polygon's normal
	//polygon.CalcNormal();
	//FVector n = polygon.Normal;
	//float xAngle = FMath::Acos(FVector::DotProduct(n, FVector::ForwardVector));
	//float yAngle = FMath::Acos(FVector::DotProduct(n, FVector::RightVector));
	//float zAngle = FMath::Acos(FVector::DotProduct(n, FVector::UpVector));
	//
	//if (zAngle <= xAngle && zAngle <= yAngle)
	//{
	//	for (FVector v : UVWVertices)
	//	{
	//		OutUVs.Add(FVector2D(v.X, v.Y));
	//	}
	//}
	//
	//else if (yAngle <= xAngle && yAngle <= zAngle)
	//{
	//	for (FVector v : UVWVertices)
	//	{
	//		OutUVs.Add(FVector2D(v.Y, v.Z));
	//	}
	//}
	//
	//else //if (xRange <= yRange && xRange <= zRange)
	//{
	//	for (FVector v : UVWVertices)
	//	{
	//		OutUVs.Add(FVector2D(v.X, v.Z));
	//	}
	//}
}

TArray<FVector> UGazeActorFactory::NormalizeBB(FVector Min, FVector Max, const TArray<FVector> &vertices) {
	Min.X = Max.X - Min.X == 0 ? 0 : Min.X;
	Min.Y = Max.Y - Min.Y == 0 ? 0 : Min.Y;
	Min.Z = Max.Z - Min.Z == 0 ? 0 : Min.Z;

	TArray<FVector> UVWVertices;
	for (FVector v : vertices)
	{

		UVWVertices.Add((v - Min) / (Max - Min));
	}

	return UVWVertices;
}

int32 UGazeActorFactory::GetValidID()
{
	for (AActor* Actor : ASynOpticonState::GetGazeActorList())
	{
		AGazeActor* GazeActor = Cast<AGazeActor>(Actor);
		if (GazeActor)
		{
			if (Index == GazeActor->GetID())
			{
				Index++;
				return GetValidID();
			}
		}
	}
	return Index;
}


//TODO the ID of an AOI only has to be unique within the actors AOIS
int32 UGazeActorFactory::GetValidAOIID()
{
	if (!AOIIDIsUnique(NextUniqueAOIId)) {
		NextUniqueAOIId--;
		return GetValidAOIID();
	}

	return NextUniqueAOIId;
}

void UGazeActorFactory::DecrementAOIID()
{
	NextUniqueAOIId--;
	if (!AOIIDIsUnique(NextUniqueAOIId)) {
		NextUniqueAOIId--;
	}
}


bool UGazeActorFactory::AOIIDIsUnique(int32 AOIID) {
	for (AActor* Actor : ASynOpticonState::GetGazeActorList())
	{
		AGazeActor* GazeActor = Cast<AGazeActor>(Actor);
		if (GazeActor)
		{
			for (FAOIData AOIData : GazeActor->GetAOIS(false)) {
				if (AOIID == AOIData.ID)
				{
					return false;
				}
			}
		}
	}


	return true;
}