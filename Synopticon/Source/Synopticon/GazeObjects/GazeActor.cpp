// Fill out your copyright notice in the Description page of Project Settings.


#include "GazeActor.h"
#include "Async/Async.h"
#include "GazeParentActor.h"
//#include "Synopticon.h"

// Sets default values
AGazeActor::AGazeActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	
	NatNetName = "";
	Dimensions.Set(0,0,0);
	GroupID = -1;

	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = ProceduralMeshComponent;
}

// Called when the game starts or when spawned
void AGazeActor::BeginPlay()
{
	Super::BeginPlay();
	GazeBlock = nullptr;

	MicroTaskStatus = EMicroTaskStatus::NA;

	ShouldClearGazeParent = false;

	GazeParent = nullptr;
	Parented = false;

	IsGazeObject = true;

	SavedTextureSize = 0;

	EncodedFrame = new ImageDataStructures::VP8EncodedImageData();
	FirstEncodingTask = true;
	HasEncodedFrame = false;
	HasKeyframe = false;
	RecordingTexture = false;

	Material = nullptr;
	ScreenTexture = nullptr;
	FirstDecodingTask = true;

	ButtonMeshName = "";
	SplineMeshName = "";

	//TODO remove after popuptesting is finished
	/*FAOIData TestPopupData;
	TestPopupData.Name = "TestPopup";
	TestPopupData.ID = -300;
	TestPopupData.Coordinates.Add(FVector2D(0.3, 0.3));
	TestPopupData.Coordinates.Add(FVector2D(0.6, 0.3));
	TestPopupData.Coordinates.Add(FVector2D(0.6, 0.6));
	TestPopupData.Coordinates.Add(FVector2D(0.3, 0.6));
	TestPopupData.Color = FLinearColor::Red;
	TArray<FAOIData> Test;
	Test.Add(TestPopupData);
	AddAOIS(true, Test);*/

	//if (BrowserComponent)
	//{
	//	RenderComponent = BrowserComponent->WebViewRenderComponent;
	//	if (RenderComponent)
	//	{
	//		//ImageDecodedDelegate.BindUObject(this, &AGazeActor::OnImageDecoded);
	//		//DecodeTask = new FAsyncTask<FDecodeImageTask>(&ImageDecodedDelegate, &DecodeImageDelegate, EPixelFormat::PF_B8G8R8A8);//EPixelFormat::PF_B8G8R8A8); //EPixelFormat::PF_R5G6B5_UNORM

	//		ImageEncodedDelegate.BindUObject(this, &AGazeActor::OnImageEncoded);
	//		EncodeTask = new FAsyncTask<FEncodeImageTask>(&ImageEncodedDelegate, &EncodeImageDelegate, RenderComponent); //TODO should use the factory methods in the ThreadPoolTasks class
	//	}
	//}

	//TODO remove later
	//InitialVarsSet = false;

	IsPopupOpening = false;
}

void AGazeActor::PrepareForRecordAndReplay(int32 NumberOfScreens, int32 Index)
{
	RecordReplayIndex = Index;

	RecordTimer = Index * ASynOpticonState::GetScreenSaveRate() / NumberOfScreens; //Start the screens at different times to spread the saving instead of having one huge load every x seconds.
	RecordTimer += ASynOpticonState::GetScreenSaveRate(); //Add the threshold so that all the screens are saved at the first frame
	//if (!GazeWorker)
	//{
	if (ASynOpticonState::IsReplaying()) 
	{
		/*UnRegisterBrowserComponent();
		RenderComponent = NewObject<URadiantWebViewRenderComponent>(this, URadiantWebViewRenderComponent::StaticClass(), FName(*(name + "RenderComponent")));*/
		//if (RenderComponent)
		//{
		//	ImageDecodedDelegate.BindUObject(this, &AGazeActor::OnImageDecoded);
		//	DecodeTask = new FAsyncTask<FDecodeImageTask>(&ImageDecodedDelegate, &DecodeImageDelegate, EPixelFormat::PF_B8G8R8A8);//EPixelFormat::PF_B8G8R8A8); //EPixelFormat::PF_R5G6B5_UNORM
		//}
	}
	
	
	//if (RenderComponent)
	//{
	//	Material = Cast<UMaterialInstanceDynamic>(ProceduralMeshComponent->GetMaterial(0));

	//		//GazeWorker = new FGazeWorker(RenderComponent);
	//		//if (GazeWorker)
	//		//{
	//		//	UMaterialInstanceDynamic* Material = Cast<UMaterialInstanceDynamic>(ProceduralMeshComponent->GetMaterial(0));
	//		//	if (Material)
	//		//	{
	//		//		GazeWorker->SetMaterialPointer(Material);
	//		//	}
	//		//}

	//	if (ASynOpticonState::HasPlayingReplayBeenStarted())
	//	{
	//		//RenderComponent->SetReplayMode();
	//	}
	//}
	//}
}

void AGazeActor::StopRecordAndReplay()
{
	//Stop the gaze worker
	//if (GazeWorker)
	//{
	//	GazeWorker->Stop();
	//	delete GazeWorker;
	//	GazeWorker = nullptr;
	//}
}

void AGazeActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//StopRecordAndReplay();

	if (DecodeTask) {
		//if (!DecodeTask->Cancel()) {
		//	DecodeTask->EnsureCompletion();			
		//}
		DecodeTask->EnsureCompletion();
		delete DecodeTask;
	}

	ImageDecodedDelegate.Unbind();
	DecodeImageDelegate.Unbind();

	if (EncodeTask) {
		//if (!EncodeTask->Cancel()) {
		//	EncodeTask->EnsureCompletion();
		//}
		EncodeTask->EnsureCompletion();
		delete EncodeTask;
	}

	ImageEncodedDelegate.Unbind();
	EncodeImageDelegate.Unbind();

	if (EncodedFrame) {
		delete EncodedFrame;
	}
}

void AGazeActor::SetGazeParentActor(AActor* NewParent, FString _GazeParentName) {
	if (NewParent) {
		GazeParent = NewParent;
		GazeParentName = _GazeParentName;
		//Store the original Transform
		OriginalTransform = GetActorTransform();

		//Setup the relationship
		this->AttachToActor(GazeParent, FAttachmentTransformRules::KeepWorldTransform);

		Parented = true;
	}
	else { //If the new parent is null, remove the old parent if it exists 
		if (GazeParent->IsValidLowLevel() && GazeParent) {
			AGazeParentActor* GPA = Cast<AGazeParentActor>(GazeParent);
			if (GPA) {
				GPA->RemoveAttachedObject(this);
			}	
		}

		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		SetActorTransform(OriginalTransform);

		Parented = false;
		GazeParent = nullptr;
		GazeParentName = "";
	}	
}

void AGazeActor::ClearGazeParent() {
	ShouldClearGazeParent = true;
}

//AOIS
void AGazeActor::SetAOIS(bool IsPopupAOI, TArray<FAOIData> _AOIS) {
	if (ASynOpticonState::GetHeatMapConditions().Num() > 0) {
		for (int i = 0; i < _AOIS.Num(); i++) {
			_AOIS[i].Color = ASynOpticonState::GetHeatMapConditions()[0].Color;
		}
	}

	if (IsPopupAOI) {
		PopupAOIS = _AOIS;
	}
	else {
		AOIS = _AOIS;
	}	
	TriangulateAOIs(IsPopupAOI);
}

void AGazeActor::AddAOIS(bool IsPopupAOI, TArray<FAOIData> _AOIS) {
	if (ASynOpticonState::GetHeatMapConditions().Num() > 0) {
		for (int i = 0; i < _AOIS.Num(); i++) {
			_AOIS[i].Color = ASynOpticonState::GetHeatMapConditions()[0].Color;
		}
	}

	if (IsPopupAOI) {
		PopupAOIS.Append(_AOIS);
	}
	else {
		AOIS.Append(_AOIS);
	}
	
	TriangulateAOIs(IsPopupAOI);
}

void AGazeActor::RemoveAOI(bool IsPopupAOI, int32 AOIId) {
	if (IsPopupAOI) {
		for (FAOIData AOIData : PopupAOIS) {
			if (AOIData.ID == AOIId) {
				PopupAOIS.Remove(AOIData);
				ASynOpticonState::GetGlobalEventSystem()->OnRemoveAOIDelegate.Broadcast(AOIId);
				break;
			}
		}
	}
	else {
		for (FAOIData AOIData : AOIS) {
			if (AOIData.ID == AOIId) {
				AOIS.Remove(AOIData);
				ASynOpticonState::GetGlobalEventSystem()->OnRemoveAOIDelegate.Broadcast(AOIId);
				break;
			}
		}
	}
}

void AGazeActor::EditAOI(bool IsPopupAOI, FAOIData NewData) {
	if (IsPopupAOI) {
		for (int i = 0; i < AOIS.Num(); i++) {
			if (PopupAOIS[i].ID == NewData.ID) {
				PopupAOIS[i] = NewData;
				TriangulateAOI(IsPopupAOI, i);
				break;
			}
		}
	}
	else {
		for (int i = 0; i < AOIS.Num(); i++) {
			if (AOIS[i].ID == NewData.ID) {
				AOIS[i] = NewData;
				TriangulateAOI(IsPopupAOI, i);
				break;
			}
		}
	}

	ASynOpticonState::GetGlobalEventSystem()->OnEditAOIDelegate.Broadcast(NewData.ID);
}

void AGazeActor::MarkMicroTaskAsComplete(bool IsPopupAOI, int32 AOIID)
{
	if (IsPopupAOI) {
		for (int i = 0; i < PopupAOIS.Num(); i++) {
			if (PopupAOIS[i].ID == AOIID && PopupAOIS[i].MicroTaskStatus == EMicroTaskStatus::STARTED) {
				PopupAOIS[i].MicroTaskStatus = EMicroTaskStatus::COMPLETE;
			}
		}
	}
	else {
		for (int i = 0; i < AOIS.Num(); i++) {
			if (AOIS[i].ID == AOIID && AOIS[i].MicroTaskStatus == EMicroTaskStatus::STARTED) {
				AOIS[i].MicroTaskStatus = EMicroTaskStatus::COMPLETE;
			}
		}
	}
}

void AGazeActor::RemoveAOIS(bool IsPopupAOI) {
	if (IsPopupAOI) {
		for (FAOIData AOIData : PopupAOIS) {
			ASynOpticonState::GetGlobalEventSystem()->OnRemoveAOIDelegate.Broadcast(AOIData.ID);
		}
		PopupAOIS.Empty();
	}
	else {
		for (FAOIData AOIData : AOIS) {
			ASynOpticonState::GetGlobalEventSystem()->OnRemoveAOIDelegate.Broadcast(AOIData.ID);
		}
		AOIS.Empty();
	}
}

bool AGazeActor::ContainsAOIID(bool IsPopupAOI, int32 AoiID) {
	if (IsPopupAOI) {
		for (FAOIData AOIData : PopupAOIS) {
			if (AOIData.ID == AoiID) {
				return true;
			}
		}
	}
	else {
		for (FAOIData AOIData : AOIS) {
			if (AOIData.ID == AoiID) {
				return true;
			}
		}
	}
	
	return false;
}

FString AGazeActor::GetAOIName(bool IsPopupAOI, int32 AoiID) {
	if (IsPopupAOI) {
		for (FAOIData AOIData : PopupAOIS) {
			if (AOIData.ID == AoiID) {
				return AOIData.Name;
			}
		}
	}
	else {
		for (FAOIData AOIData : AOIS) {
			if (AOIData.ID == AoiID) {
				return AOIData.Name;
			}
		}
	}
	
	return "Not found";
}

FString AGazeActor::GetAOINameFromID(int32 AoiID)
{
	if (ID == AoiID) {
		return name;
	}
	for (FAOIData AOIData : PopupAOIS) {
		if (AOIData.ID == AoiID) {
			return AOIData.Name;
		}
	}
	for (FAOIData AOIData : AOIS) {
		if (AOIData.ID == AoiID) {
			return AOIData.Name;
		}
	}
	return "Not found";
}

TArray<FAOIData> AGazeActor::GetAOIS(bool IsPopupAOI) { 
	if (IsPopupAOI){
		return PopupAOIS;		
	}
	else {
		return AOIS;
	}
}

void AGazeActor::TriangulateAOIs(bool IsPopupAOI) {
	if (IsPopupAOI) {
		for (int i = 0; i < PopupAOIS.Num(); i++) {
			TriangulateAOI(IsPopupAOI, i);
		}
	}
	else {
		for (int i = 0; i < AOIS.Num(); i++) {
			TriangulateAOI(IsPopupAOI, i);
		}
	}
	
}

void AGazeActor::TriangulateAOI(bool IsPopupAOI, int AOIListIndex) {
	TArray<FVector> Tmp3DAOIs;

	if (IsPopupAOI) {
		for (FVector2D Coord2D : PopupAOIS[AOIListIndex].Coordinates) {
			Tmp3DAOIs.Add(FVector(Coord2D.X, Coord2D.Y, 0));
		}
		PopupAOIS[AOIListIndex].TriangleIndices.Empty();
		USynOpticonStatics::Triangulate(Tmp3DAOIs, PopupAOIS[AOIListIndex].TriangleIndices);
	}
	else {
		for (FVector2D Coord2D : AOIS[AOIListIndex].Coordinates) {
			Tmp3DAOIs.Add(FVector(Coord2D.X, Coord2D.Y, 0));
		}
		AOIS[AOIListIndex].TriangleIndices.Empty();
		USynOpticonStatics::Triangulate(Tmp3DAOIs, AOIS[AOIListIndex].TriangleIndices);
	}

}
//AOIS - END

void AGazeActor::SetIsGazeObject(bool _IsGazeObject) {
	IsGazeObject = _IsGazeObject;
	SetupMeshCollisionInBP();
}

bool AGazeActor::GetIsGazeObject() {
	return IsGazeObject;
}

void AGazeActor::SetIsCollisionObject(bool _IsCollisionObject) {
	IsCollisionObject = _IsCollisionObject;
}

bool AGazeActor::GetIsCollisionObject() {
	return IsCollisionObject;
}

void AGazeActor::SetButtonMeshName(FString _ButtonMeshName) {
	ButtonMeshName = _ButtonMeshName;
}

void AGazeActor::SetSplineMeshName(FString _SplineMeshName) {
	SplineMeshName = _SplineMeshName;
}

FString AGazeActor::GetButtonMeshName()
{ 
	return ButtonMeshName; 
}

FString AGazeActor::GetSplineMeshName()
{ 
	return SplineMeshName; 
}

UProceduralMeshComponent* AGazeActor::GetProceduralMeshComponent()
{
	return ProceduralMeshComponent;
}

void AGazeActor::SetMicroTaskStatus(EMicroTaskStatus Status)
{
	MicroTaskStatus = Status;
}

void AGazeActor::SetParented(bool _Parented) {
	Parented = _Parented;
}

void AGazeActor::SetDimensions(FVector _Dimensions)
{
	Dimensions = _Dimensions;
}

// Called every frame
void AGazeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ShouldClearGazeParent) {
		SetGazeParentActor(nullptr, "");
		ShouldClearGazeParent = false;
	}
	//======================Record========================
	if (ASynOpticonState::IsRecording())
	{
		RecordFrameData(DeltaTime);
	}

	//=============Replay====================
	//Playing back
	if (ASynOpticonState::IsReplaying()
		&& !ASynOpticonState::IsPausingReplay())
	{
		UpdateTexture();



		if (Movable || Parented)
		{
			SetActorLocation(GazeBlock->ActorLocation);
			SetActorRotation(GazeBlock->ActorRotation);

			//TODO remove later
			/*if (!InitialVarsSet  && GetActorLocation().X > -1000) {
				if (name.Equals("YokeRight2")){ 
					InitialLocation = GazeBlock->ActorLocation;
					InitialOrientation = GazeBlock->ActorRotation;
				}
				InitialVarsSet = true;
			}*/

			if (name.Equals("YokeRight2") && GetActorLocation().X > -1000) {
				FVector WorldLocation;
				for (FVector Vertex : Verticies) {
					WorldLocation += Vertex;
				}
				WorldLocation = WorldLocation / Verticies.Num();
				//WorldLocation += GazeBlock->ActorLocation;

				//FVector LocalOffset = GazeBlock->ActorLocation - InitialLocation;

				FRotator LocalRotation = GazeBlock->ActorRotation - InitialOrientation;

				ASynOpticonState::GetGlobalEventSystem()->OnEventRightYokeUpdate.Broadcast(InitialLocation, GazeBlock->ActorRotation, GazeBlock->ActorLocation);
			}
		}
	}
	//================END REPLAY==================
}

void AGazeActor::SetEyeRadarLines(TArray<FEyeRadarPointData> _EyeRadarLines)
{
	EyeRadarLines = _EyeRadarLines;
}

void AGazeActor::AddEyeRadarLine(FVector2D EyeRadarLine, float Duration)
{
	AddEyeRadarLineStruct(FEyeRadarPointData(EyeRadarLine, Duration));
}

void AGazeActor::AddEyeRadarLineStruct(FEyeRadarPointData EyeRadarData) {
	if (EyeRadarLines.Num() > 20) { //Hardcoded threshold looool. TODO put into statics at some point
		EyeRadarLines.RemoveAt(0, 1, true);
	}
	EyeRadarLines.Add(EyeRadarData);
}


TArray<FEyeRadarPointData> AGazeActor::GetEyeRadarLines()
{
	return EyeRadarLines;
}

void AGazeActor::SetMovable(bool _Movable)
{
	Movable = _Movable;
}

//TODO remove later
void AGazeActor::EventUpdateTransform(FVector ActualLocation, FRotator ActualRotation, FVector LocationOffset) {
	

	//ProceduralMeshComponent->UpdateMeshSection(LocalVerticies);
	//
	//FVector Test = FVector(-InitialLocation.X, -InitialLocation.Y, -InitialLocation.Z);
	//Test += FVector(0, 0, -8);
	//SetActorLocation(Test); //- InitialLocation + LocationOffset);
	SetActorRotation(ActualRotation);
	SetActorLocation(ActualLocation);
	AddActorWorldOffset(LocationOffset);
	//AddLocalActorRotation(ActualRotation);
	//SetActorRotation(ActualRotation);
	//SetActorLocation(ActualLocation - InitialLocation + LocationOffset);
	//SetActorLocation(LocationOffset);
}

void AGazeActor::SetName(FString _name)
{
	name = _name;
}

void AGazeActor::SetNatNetName(FString _NatNetName)
{
	NatNetName = _NatNetName;
}

void AGazeActor::SetVerticies(TArray<FVector> _verticies) 
{
	Verticies = _verticies;

	if (name.Contains("YokeLeft")) {
		UGlobalEventSystem* EventSystem = ASynOpticonState::GetGlobalEventSystem();
		EventSystem->OnEventRightYokeUpdate.AddDynamic(this, &AGazeActor::EventUpdateTransform);
	}

	if (name.Contains("YokeRight2")) {
		float X = 80.005622863769531 + 73.612648010253906 + 75.467254638671875;
		X /= 3;
		float Y = -43.405616760253906 - 39.925071716308594 - 29.232049942016602;
		Y /= 3;
		float Z = 44.298797607421875 + 28.754640579223633 + 28.611961364746094;
		Z /= 3;

		FVector LeftYokeInit = FVector(X, Y, Z);

		X = 79.270301818847656 + 73.126136779785156 + 74.585807800292969;
		X /= 3;
		Y = 59.105258941650391 + 63.5777587890625 + 73.782081604003906;
		Y /= 3;
		Z = 43.125724792480469 + 28.350154876708984 + 28.459213256835938;
		Z /= 3;

		FVector RightYokeInit = FVector(X, Y, Z);

		InitialLocation = LeftYokeInit - RightYokeInit;
	}
}

void AGazeActor::SetTriangleIndexes(TArray<int32> _TriangleIndices)
{
	TriangleIndices = _TriangleIndices;
}

void AGazeActor::SetUVs(TArray<FVector2D> _uv)
{
	UVs = _uv;
}

TArray<FVector2D> AGazeActor::GetUVs()
{
	//Used to be used for the interaction?
	/*if (ProceduralMeshComponent && ProceduralMeshComponent->GetNumSections() > 0) {
		TArray<FVector2D> _UVs;
		for (int i = 0; i < ProceduralMeshComponent->GetProcMeshSection(0)->ProcVertexBuffer.Num(); i++) {
			_UVs.Add(ProceduralMeshComponent->GetProcMeshSection(0)->ProcVertexBuffer[i].UV0);
		}
		return _UVs;
	}*/

	return UVs;
}

void AGazeActor::SetOutsideVerticies(TArray<FVector> _verticies)
{
	OutsideBoxVerticies = _verticies;
}

void AGazeActor::SetOutsideTriangleIndexes(TArray<int32> _triangleIndexes)
{
	OutsideBoxTriangleIndices = _triangleIndexes;
}

void AGazeActor::SetOutsideUVs(TArray<FVector2D> _UVs)
{
	OutsideBoxUVs = _UVs;
}

void AGazeActor::SetURL(FString _url)
{
	URL = _url;
}

/** Get the name of the rigid body which used in motive */
FString AGazeActor::GetRigidBodyName()
{
	return NatNetName;
}

int32 AGazeActor::GetID()
{
	return ID;
}

int32 AGazeActor::GetHitID(bool GazeInteraction)
{
	if ((GazeInteraction && IsGazeObject) || (!GazeInteraction && IsCollisionObject)) {
		if (GroupID < -1)
		{
			return GroupID;
		}
		else {
			return ID;
		}
	}
	else {
		return -1;
	}
}

void AGazeActor::GetTriangleInformation(int32 TriangleIndex, TArray<FVector> &OutVerticies, TArray<FVector2D> &OutUVs) {
	if (TriangleIndex >= (TriangleIndices.Num()/3)) {
		return;
	}
	
	int32 P1Ind = TriangleIndices[TriangleIndex*3];
	int32 P2Ind = TriangleIndices[(TriangleIndex*3)+1];
	int32 P3Ind = TriangleIndices[(TriangleIndex*3)+2];
	
	OutVerticies.Add(Verticies[P1Ind]);
	OutVerticies.Add(Verticies[P2Ind]);
	OutVerticies.Add(Verticies[P3Ind]);

	OutUVs.Add(UVs[P1Ind]);
	OutUVs.Add(UVs[P2Ind]);
	OutUVs.Add(UVs[P3Ind]);
}

int32 AGazeActor::GetHitID(bool GazeInteraction, FVector2D HitUV)
{
	if (PopupAOIS.Num() > 0)
	{
		for (FAOIData AOIData : PopupAOIS) {
			if (AOIData.IsInside(HitUV)) {
				return AOIData.ID;
			}
		}
	}
	if (AOIS.Num() > 0) {
		for (FAOIData AOIData : AOIS) {
			if (AOIData.IsInside(HitUV)) {
				return AOIData.ID;
			}
		}	
	}

	return GetHitID(GazeInteraction);
}

void AGazeActor::SetID(int32 id)
{
	ID = id;	
}

bool AGazeActor::RecordFrameData(float DeltaTime)
{	
	bool ContainsKeyframe = false;
	if (!GazeBlock)
	{
		return ContainsKeyframe;
	}

	//Save the data that should be saved every tick
	GazeBlock->DataSize = 0;
	GazeBlock->movable = Movable;
	GazeBlock->parented = Parented;
	ContainsKeyframe = false;
	if (Movable || Parented)
	{
		GazeBlock->ActorLocation = GetActorLocation();
		GazeBlock->ActorRotation = GetActorRotation();
	}

	//Check if the texture should be saved
	//RecordTimer += DeltaTime;
	//if (RecordTimer > ASynOpticonState::GetScreenSaveRate()) 
	//{
	//	if (HasEncodedFrame) {
	//		GazeBlock->DataSize = SavedTextureSize;
	//		RecordTimer -= ASynOpticonState::GetScreenSaveRate();
	//		ContainsKeyframe = HasKeyframe;
	//		SavedTextureSize = 0;
	//		HasEncodedFrame = false;
	//	}
	//	else if(FirstEncodingTask || EncodeTask->IsDone())
	//	{			
	//		FirstEncodingTask = false;

	//		/*if (RenderComponent)
	//		{
	//			uint8* WebViewTextureData = (uint8*)RenderComponent->GetFirstReplayData();
	//			if (WebViewTextureData) {
	//				EncodeImageDelegate.Execute(WebViewTextureData);
	//				EncodeTask->StartBackgroundTask(ASynOpticonState::GetThreadPoolManager()->GetWorkerPool());
	//			}
	//		}*/
	//	}
	//}

	return ContainsKeyframe;
}

void AGazeActor::SetCompressedGazeBlock(CompressedGazeBlock* _GazeBlock)
{
	GazeBlock = _GazeBlock;
}

void AGazeActor::UpdateTexture()
{
	if (!GazeBlock || !GazeBlock->Texture)
	{
		return;
	}

	//if (GazeBlock->DataSize > 0){ //&& !HasUpdatedTexture && (FirstDecodingTask || DecodeTask->IsDone())) {		
	//	if (GazeBlock->DataSize > GazeBlock->BufferSize) {
	//		return;
	//	}
	//	EncodedFrame->ImageDataBufferSize = GazeBlock->DataSize;
	//	memcpy(EncodedFrame->ImageBuffer, GazeBlock->Texture, GazeBlock->DataSize);
	//	EncodedFrame->ImageWidth = GazeBlock->ImageWidth;
	//	EncodedFrame->ImageHeight = GazeBlock->ImageHeight;
	//	EncodedFrame->IsKeyFrame = GazeBlock->IsKeyframe;
	//
	//	DecodeImageDelegate.Execute(EncodedFrame);

	//	if (FirstDecodingTask || DecodeTask->IsDone()) {
	//		DecodeTask->StartBackgroundTask(ASynOpticonState::GetThreadPoolManager()->GetWorkerPool());
	//	}	

	//	FirstDecodingTask = false;
	//}
	//else if (HasUpdatedTexture) {
	//	if (Material)
	//	{
	//		Material->SetTextureParameterValue("WebViewTexture", ScreenTexture);
	//	}
	//	ScreenTexture = nullptr;
	//	HasUpdatedTexture = false;
	//}
}

void AGazeActor::OnImageDecoded(UTexture2D* DecodedImage){
	ScreenTexture = DecodedImage;
	HasUpdatedTexture = true;
}

void AGazeActor::OnImageEncoded(ImageDataStructures::VP8EncodedImageData* _EncodedImage){
	//This should be save to do outside the game thread since we don't touch the texture data unless the datasize is more than 0. 
	//The datasize is not set until the texture data is copied into the gaze block, and it is set in the game thread.
	if (GazeBlock && GazeBlock->Texture) {
		memcpy(GazeBlock->Texture, _EncodedImage->ImageBuffer, _EncodedImage->ImageDataBufferSize);
		
		GazeBlock->ImageWidth = _EncodedImage->ImageWidth;
		GazeBlock->ImageHeight = _EncodedImage->ImageHeight;
		GazeBlock->IsKeyframe = _EncodedImage->IsKeyFrame;

		HasEncodedFrame = true;

		HasKeyframe = ((RecordReplayIndex == 0) && _EncodedImage->IsKeyFrame);

		SavedTextureSize = _EncodedImage->ImageDataBufferSize;
	}
}

TArray<int32> AGazeActor::GetFlippedTriangles()
{
	TArray<int32> FlippedTriangles;

	for (int i = 0; i < TriangleIndices.Num(); i += 3)
	{
		FlippedTriangles.Add(TriangleIndices[i + 2]);
		FlippedTriangles.Add(TriangleIndices[i + 1]);
		FlippedTriangles.Add(TriangleIndices[i]);
	}

	return FlippedTriangles;
}

TArray<FGazeActorData> AGazeActor::SliceGazeActor(UProceduralMeshComponent* First, UProceduralMeshComponent* Second)
{
	TArray<FGazeActorData> Result;
	/*if (First && Second) {
		TArray<FVector> FirstVertices;
		TArray<FVector2D> FirstUVs;
		for (int i = 0; i < First->GetProcMeshSection(0)->ProcVertexBuffer.Num(); i++) {
			FirstVertices.Add(First->GetProcMeshSection(0)->ProcVertexBuffer[i].Position);
			FirstUVs.Add(First->GetProcMeshSection(0)->ProcVertexBuffer[i].UV0);
		}
		TArray<int32> FirstIndexes = First->GetProcMeshSection(0)->ProcIndexBuffer;
		FGazeActorData GA1 = GetDataStruct();
		GA1.Name = name + "_1";
		GA1.Vertices = FirstVertices;
		GA1.UVs = FirstUVs;
		GA1.Indexes = FirstIndexes;
		Result.Add(GA1);

		TArray<FVector> SecondVertices;
		TArray<FVector2D> SecondUVs;
		for (int i = 0; i < Second->GetProcMeshSection(0)->ProcVertexBuffer.Num(); i++) {
			SecondVertices.Add(Second->GetProcMeshSection(0)->ProcVertexBuffer[i].Position);
			SecondUVs.Add(Second->GetProcMeshSection(0)->ProcVertexBuffer[i].UV0);
		}
		TArray<int32> SecondIndexes = Second->GetProcMeshSection(0)->ProcIndexBuffer;
		FGazeActorData GA2 = GetDataStruct();
		GA2.Name = name + "_2";
		GA2.Vertices = SecondVertices;
		GA2.UVs = SecondUVs;
		GA2.Indexes = SecondIndexes;
		Result.Add(GA2);
	}*/
	return Result;
}

UMaterialInstanceDynamic* AGazeActor::GetMaterial()
{
	return Cast<UMaterialInstanceDynamic>(ProceduralMeshComponent->GetMaterial(0));
}

void AGazeActor::RotateUVs(int32 Angle) {
	float rotationRadians = FMath::DegreesToRadians(Angle);

	float rotMatrix00 = FMath::Cos(rotationRadians);
	float rotMatrix01 = -FMath::Sin(rotationRadians);
	float rotMatrix10 = FMath::Sin(rotationRadians);
	float rotMatrix11 = FMath::Cos(rotationRadians);

	FVector2D halfVector = FVector2D(0.5f, 0.5f);

	for (int j = 0; j < UVs.Num(); j++)
	{
		// Switch coordinates to be relative to center of the plane
		UVs[j] = UVs[j] - halfVector;
		// Apply the rotation matrix
		float u = rotMatrix00 * UVs[j].X + rotMatrix01 * UVs[j].Y;
		float v = rotMatrix10 * UVs[j].X + rotMatrix11 * UVs[j].Y;
		UVs[j].X = u;
		UVs[j].Y = v;
		// Switch back coordinates to be relative to edge
		UVs[j] = UVs[j] + halfVector;
	}
}

FGazeActorData AGazeActor::GetDataStruct()
{
	FGazeActorData GazeActorData;
	GazeActorData.ID = ID;
	GazeActorData.GazeActorType = GazeActorType;
	GazeActorData.IsGazeObject = IsGazeObject;
	GazeActorData.IsCollisionObject = IsCollisionObject;
	GazeActorData.Name = name;
	GazeActorData.AOIS = AOIS;
	GazeActorData.Vertices = Verticies;
	GazeActorData.UVs = UVs;
	GazeActorData.Indexes = TriangleIndices;
	GazeActorData.URL = URL;
	GazeActorData.Parented = Parented;
	GazeActorData.Movable = Movable;
	GazeActorData.NatNetName = NatNetName;
	GazeActorData.SizeDimensions = Dimensions;
	GazeActorData.FontSize = FontSize;
	GazeActorData.Grouped = GroupID;
	GazeActorData.WAMPScreenID = WAMPScreenID;
	//GazeActorData.VideoIndex = VideoIndex; 
	
	UGazeObjectMaterialComponent* GazeMaterialComp = Cast<UGazeObjectMaterialComponent>(GetComponentByClass(UGazeObjectMaterialComponent::StaticClass()));
	if (GazeMaterialComp && !GazeMaterialComp->GetTextureFilepath().Equals("")) {
		GazeActorData.TextureFilepath = GazeMaterialComp->GetTextureFilepath();
	}

	FString ParentCompName = "";
	if (IsParented()) {
		AGazeParentActor* _GazeParent = Cast<AGazeParentActor>(GetGazeParentActor());
		GazeActorData.ParentSceneCompName = _GazeParent->GetSceneCompName();
	}
	GazeActorData.ButtonMesh = ButtonMeshName;
	GazeActorData.SplineMesh = SplineMeshName;

	return GazeActorData;
}

void AGazeActor::SetGrouped(int32 Grouped)
{
	GroupID = Grouped;
}

FString AGazeActor::GetGroupName()
{
	if (GroupID < -1) {
		return ASynOpticonState::GetGazeGroup(GroupID);
	}
	return name;
}

void AGazeActor::CalibrateHeight(float DeltaHeight)
{
	for (int i = 0; i < Verticies.Num(); i++)
	{
		Verticies[i].Z += DeltaHeight;
	}

	//AddActorWorldOffset(FVector(0, 0, DeltaHeight));
}

int AGazeActor::GetCalibrationVerticeIndex()
{
	return CalibrationVerticeIndex;
}

void AGazeActor::SetCalibrationVerticeIndex(int _Index)
{
	CalibrationVerticeIndex = _Index;
}

void AGazeActor::LoadImageAndAOIsAsync(FImageAndAOIs Dat)
{
	if (Dat.ScreenID.Equals(WAMPScreenID))
	{
		SetAOIS(false, Dat.AOIs);
		UGazeObjectMaterialComponent* MaterialComponent = FindComponentByClass<UGazeObjectMaterialComponent>();
		if (MaterialComponent)
		{
			AsyncTask(ENamedThreads::GameThread, [MaterialComponent, Dat]() {
				MaterialComponent->SetupScreenTexture(Dat.Image);
			});
		}

		ASynOpticonState::AOISessionDict.Add(ID, TPair<FString, bool>(Dat.SpecifiedID, false));
		for (FAOIData aoi : Dat.AOIs)
		{
			ASynOpticonState::AOISessionDict.Add(aoi.ID, TPair<FString, bool>(Dat.SpecifiedID, false));
		}
	}
}

void AGazeActor::UpdatePopupAndAOIsAsync(FPopupAndAOIs Data)
{
	if (Data.ScreenID.Equals(WAMPScreenID))
	{
		if (Data.Action.Equals("Open"))
		{
			if (IsPopupOpening)
			{
				RemoveAOIS(true);
			}

			AddAOIS(true, Data.AOIs);
			IsPopupOpening = true;
		}
		else if (Data.Action.Equals("Update"))
		{
			for (FAOIData aoi : Data.AOIs)
			{
				EditAOI(true, aoi);
			}
		}
		else if (Data.Action.Equals("Close"))
		{
			for (FAOIData aoi : Data.AOIs)
			{
				for (int i = 0; i < PopupAOIS.Num(); i++)
				{
					if (aoi.Name.Equals(PopupAOIS[i].Name))
					{
						RemoveAOI(true, PopupAOIS[i].ID);
						break;
					}
				}
			}
			IsPopupOpening = false;
		}
		ASynOpticonState::AOISessionDict.Add(ID, TPair<FString, bool>(Data.SpecifiedID, false));
		for (FAOIData aoi : Data.AOIs)
		{
			ASynOpticonState::AOISessionDict.Add(aoi.ID, TPair<FString, bool>(Data.SpecifiedID, true));
		}
	}
}

FMatrix AGazeActor::GetOpenFaceTransformation()
{
	FVector Center = (Verticies[1] + Verticies[2]) / 2;
	//FVector ForwardVector = ProceduralMeshComponent->GetProcMeshSection(0)->ProcVertexBuffer[0].Normal;
	FVector RightVector = Verticies[1] - Verticies[2];
	FVector UpVector = Verticies[1] - Verticies[0];
	RightVector.Normalize();
	UpVector.Normalize();
	FVector ForwardVector =	FVector::CrossProduct(RightVector, UpVector);
	ForwardVector.Normalize();
	FMatrix axis;
	axis.SetAxes(&ForwardVector, &RightVector, &UpVector, &Center);

	if (ASynOpticonState::IsShowingDebugLines())
	{
		DrawDebugLine(
			GetWorld(),
			Center,
			Center + RightVector * 50,
			FColor(0, 255, 0), //green
			false, 500, 0,
			5
		);
		DrawDebugLine(
			GetWorld(),
			Center,
			Center + UpVector * 50,
			FColor(0, 0, 255),
			false, 500, 0,
			5
		);
		DrawDebugLine(
			GetWorld(),
			Center,
			Center + ForwardVector * 50,
			FColor(255, 0, 0),
			false, 0.1, 0,
			5
		);
	}

	return axis;
}
