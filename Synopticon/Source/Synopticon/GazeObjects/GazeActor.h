// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialInstanceDynamic.h"

//Worker thread pool and tasks
#include "SystemClasses/ThreadPoolManager.h"
#include "SystemClasses/ThreadPoolTasks.h"

//#include "GazeObjects/GazeParentActor.h"

#include "ActorComponents/PositionAndOrientationComponent.h"
#include "GazeObjectMaterialComponent.h"

//Image processing data structures
#include "ImageProcessing/ImageDataStructures.h"
#include "Replay/ReplayDataStructures.h"
#include "GameFramework/Actor.h"
#include "SystemClasses/SynOpticonState.h"
#include "Kismet/GameplayStatics.h"
#include "StaticRoomActorDatatypes.h"
#include "GazeActorDataTypes.h"

#include "GazeActor.generated.h"

class AGazeParentActor;

UCLASS()
class SYNOPTICON_API AGazeActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	EMicroTaskStatus MicroTaskStatus;

	int32 ID;
	int32 RecordReplayIndex;

	//The name of the gaze object
	FString name;

	int32 FontSize;
	EStaticRoomActorType GazeActorType;

	bool IsGazeObject;
	bool IsCollisionObject;

	//FLinearColor HeatMapColour;

	TArray<FVector> MarkerLocations;

	//True if the screen should move even if it is not a movable type
	bool Parented;
	int32 GroupID;

	//AOI
	TArray<FAOIData> AOIS;
	TArray<FAOIData> PopupAOIS;
	TArray<FEyeRadarPointData> EyeRadarLines;

	///////////////MOVABLE PROPERTIES////////////////
	//True if the GazeActor is of the movable type
	bool Movable;
	//The name of the object in Motive. If empty its position will not be updated
	FString NatNetName;
	//The width, length and height of the movable object
	FVector Dimensions;

	TArray<FVector> OutsideBoxVerticies;
	TArray<int32> OutsideBoxTriangleIndices;
	TArray<FVector2D> OutsideBoxUVs;

	//==========Recording and Replay=============
	/** Worker thread */
	//The runnable doing the updating and saving of the screen texture
	//FGazeWorker* GazeWorker;
	bool RecordingTexture;
	float RecordTimer;

	//Encoding delegates and the encoding task for the worker thread pool
	FImageEncodedDelegate ImageEncodedDelegate;
	FImageEncodeImageDelegate EncodeImageDelegate;
	FAsyncTask<FEncodeImageTask>* EncodeTask;

	//Decoding delegates and the decoding task for the worker thread pool
	FImageDecodedDelegate ImageDecodedDelegate;
	FImageDecodeImageDelegate DecodeImageDelegate;
	FAsyncTask<FDecodeImageTask>* DecodeTask;

	//Holds the gaze actors data that is used for recording and replaying 
	CompressedGazeBlock* GazeBlock;

	void UpdateTexture();
	bool FirstDecodingTask;
	UMaterialInstanceDynamic* Material;
	UTexture2D* ScreenTexture;
	bool HasUpdatedTexture;

	ImageDataStructures::VP8EncodedImageData* EncodedFrame;
	bool HasEncodedFrame;
	bool HasKeyframe;
	int32 SavedTextureSize;
	bool FirstEncodingTask;

	AActor* GazeParent;
	FString GazeParentName;
	FTransform OriginalTransform;

	//ProceduralMesh Properties
	TArray<FVector> Verticies;
	TArray<int32> TriangleIndices;
	TArray<FVector2D> UVs;
	int CalibrationVerticeIndex;

	//ActionObject mesh data
	FString ButtonMeshName;
	FString SplineMeshName;

	bool ShouldClearGazeParent;

	//TODO remove later
	FRotator InitialOrientation;
	FVector InitialLocation;
	bool InitialVarsSet;

	//Triangulates the AOIs for visualization . Called when adding AOIs
	void TriangulateAOIs(bool IsPopupAOI);

	//Triangulates the AOIs for visualization . Called when editing AOIs
	void TriangulateAOI(bool IsPopupAOI, int AOIIListIndex);

protected:
	FString URL;
	FVector2D WebBrowserResolution;

	//Experiment
	FString WAMPScreenID;
	bool IsPopupOpening;
public:

	//TODO remove later
	UFUNCTION()
	void EventUpdateTransform(FVector Location, FRotator Rotation, FVector Offset);

	void ClearGazeParent();

	bool FirstSave;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		UProceduralMeshComponent* ProceduralMeshComponent;

	// Sets default values for this actor's properties
	AGazeActor(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "EyeRadar")
		void SetEyeRadarLines(TArray<FEyeRadarPointData> EyeRadarLines);

	UFUNCTION(BlueprintCallable, Category = "EyeRadar")
		void AddEyeRadarLine(FVector2D EyeRadarLine, float Duration);

	UFUNCTION(BlueprintCallable, Category = "EyeRadar")
		void AddEyeRadarLineStruct(FEyeRadarPointData EyeRadarData);

	UFUNCTION(BlueprintPure, Category = "EyeRadar")
		TArray<FEyeRadarPointData> GetEyeRadarLines();

	UFUNCTION(BlueprintCallable, Category = "Gaze Parent")
	void SetGazeParentActor(AActor* NewParent, FString GazeParentName);

	UFUNCTION(BlueprintPure, Category = "Gaze Parent")
	AActor* GetGazeParentActor(){return GazeParent; }

	//Get the verticies of this gaze actor
	UFUNCTION(BlueprintPure, Category = "ProceduralMesh")
		TArray<int32> GetFlippedTriangles();

	//Get the verticies of this gaze actor
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ProceduralMesh")
		FVector2D FindUVFromMeshBP(FVector HitLocation, int32 FaceIndex);

	//Get the verticies of this gaze actor
	UFUNCTION(BlueprintPure, Category = "ProceduralMesh")
		TArray<FVector> GetVerticies() { return Verticies; }
	//Get the triangle index of this gaze actor
	UFUNCTION(BlueprintPure, Category = "ProceduralMesh")
		TArray<int> GetTriangleIndexes() { return TriangleIndices; }
	//Get the uvs of the gaze actors mesh
	UFUNCTION(BlueprintPure, Category = "ProceduralMesh")
		TArray<FVector2D> GetUVs();

	//Get the verticies of this gaze actor
	UFUNCTION(BlueprintPure, Category = "ProceduralMesh")
		TArray<FVector> GetOutsideVerticies() { return OutsideBoxVerticies; }
	//Get the triangle index of this gaze actor
	UFUNCTION(BlueprintPure, Category = "ProceduralMesh")
		TArray<int> GetOutsideTriangleIndexes() { return OutsideBoxTriangleIndices; }
	//Get the verticies of this gaze actor
	UFUNCTION(BlueprintPure, Category = "ProceduralMesh")
		TArray<FVector2D> GetOutsideUVs() { return OutsideBoxUVs; }

	//AOIS
	UFUNCTION(BlueprintPure, Category = "AOIS")
		TArray<FAOIData> GetAOIS(bool IsPopupAOI);
	UFUNCTION(BlueprintCallable, Category = "AOIS")
		void SetAOIS(bool IsPopupAOI, TArray<FAOIData> AOIS);
	UFUNCTION(BlueprintCallable, Category = "AOIS")
		void AddAOIS(bool IsPopupAOI, TArray<FAOIData> AOIS);
	UFUNCTION(BlueprintCallable, Category = "AOIS")
		void RemoveAOI(bool IsPopupAOI, int32 AOIId);
	UFUNCTION(BlueprintCallable, Category = "AOIS")
		void RemoveAOIS(bool IsPopupAOI);
	UFUNCTION(BlueprintCallable, Category = "AOIS")
		bool ContainsAOIID(bool IsPopupAOI, int32 ID);
	UFUNCTION(BlueprintCallable, Category = "AOIS")
		FString GetAOINameFromID(int32 AoiID);
	UFUNCTION(BlueprintCallable, Category = "AOIS")
		FString GetAOIName(bool IsPopupAOI, int32 ID);
	UFUNCTION(BlueprintCallable, Category = "AOIS")
		void EditAOI(bool IsPopupAOI, FAOIData NewData);
	UFUNCTION(BlueprintCallable, Category = "AOIS")
		void MarkMicroTaskAsComplete(bool IsPopupAOI, int32 AOIID);


	UFUNCTION(BlueprintCallable, Category = "AOIS")
		void SetWAMPScreenID(FString _WAMPScreenID) { WAMPScreenID = _WAMPScreenID; }

	UFUNCTION(BlueprintPure, Category = "AOIS")
		FString GetWAMPScreenID() { return WAMPScreenID; }

	//Set the name of the gaze object
	UFUNCTION(BlueprintPure, Category = "Gaze Actor Properties")
		FString GetName() {return name;}

	//Set the name of the gaze object
	UFUNCTION(BlueprintCallable, Category = "Gaze Actor Properties")
		void SetName(FString name);

	//Set the name of the gaze object
	UFUNCTION(BlueprintPure, Category = "Gaze Actor Properties")
		int32 GetFontSize() { return FontSize; }

	//Set the name of the gaze object
	UFUNCTION(BlueprintCallable, Category = "Gaze Actor Properties")
		void SetFontSize(int32 _FontSize) { FontSize = _FontSize; }

	UFUNCTION(BlueprintPure, Category = "Gaze Actor Properties")
		EStaticRoomActorType GetGazeActorType() { return GazeActorType; }
	UFUNCTION(BlueprintCallable, Category = "Gaze Actor Properties")
		void SetGazeActorType(EStaticRoomActorType Type) { GazeActorType = Type; }

	//Set the name of the gaze object
	UFUNCTION(BlueprintCallable, Category = "Gaze Actor Properties")
		void SetNatNetName(FString NatNetName);

	//Set wether the object can be interacted with gaze
	UFUNCTION(BlueprintCallable, Category = "Gaze Actor Properties")
		void SetIsGazeObject(bool IsGazeObject);
	UFUNCTION(BlueprintPure, Category = "Gaze Actor Properties")
		bool GetIsGazeObject();

	//Set wether the object can be collided with
	UFUNCTION(BlueprintCallable, Category = "Gaze Actor Properties")
		void SetIsCollisionObject(bool IsCollisionObject);
	UFUNCTION(BlueprintPure, Category = "Gaze Actor Properties")
		bool GetIsCollisionObject();

	//Set the verticies of the gaze object
	UFUNCTION(BlueprintCallable, Category = "Gaze Actor Properties")
		void SetVerticies(TArray<FVector> verticies);
	//Set the triangle indices of the gaze object
	UFUNCTION(BlueprintCallable, Category = "Gaze Actor Properties")
		void SetTriangleIndexes(TArray<int32> triangleIndexes);
	UFUNCTION(BlueprintCallable, Category = "Gaze Actor Properties")
		void SetUVs(TArray<FVector2D> uv);

	//Set the verticies of the outside box of the gaze object
	UFUNCTION(BlueprintCallable, Category = "Gaze Actor Properties")
		void SetOutsideVerticies(TArray<FVector> verticies);
	//Set the triangle indeces of the outside box of the gaze object
	UFUNCTION(BlueprintCallable, Category = "Gaze Actor Properties")
		void SetOutsideTriangleIndexes(TArray<int32> triangleIndexes);
	//Set the verticies of the outside box of the gaze object
	UFUNCTION(BlueprintCallable, Category = "Gaze Actor Properties")
		void SetOutsideUVs(TArray<FVector2D> UVs);

	//Web Browser
	UFUNCTION(BlueprintCallable, Category = "Gaze Actor Properties")
		void SetURL(FString url);

	UFUNCTION(BlueprintPure, Category = "Gaze Actor Properties")
		FString GetURL() { return URL; }

	UFUNCTION(BlueprintCallable, Category = "Gaze Actor Properties")
		void SetWebBrowserResolution(FVector2D _Res) { WebBrowserResolution = _Res; }

	UFUNCTION(BlueprintPure, Category = "Gaze Actor Properties")
		FVector2D GetWebBrowserResolution() { return WebBrowserResolution; }

	UFUNCTION(BlueprintPure, Category = "Gaze Actor Properties")
		int32 GetID(); 

	UFUNCTION(BlueprintCallable, Category = "Gaze Actor Properties")
		void SetID(int32 id);
	UFUNCTION(BlueprintPure, Category = "Gaze Actor Movable")
		bool IsMovable() { return Movable; }

	UFUNCTION(BlueprintPure, Category = "Micro Task")
		EMicroTaskStatus GetMicroTaskStatus() { return MicroTaskStatus; }
	UFUNCTION(BlueprintCallable, Category = "Gaze Actor Properties")
		void SetMicroTaskStatus(EMicroTaskStatus Status);

	UFUNCTION(BlueprintPure, Category = "Gaze Actor Properties")
		bool IsParented() { return Parented; }

	UFUNCTION(BlueprintCallable, Category = "Gaze Actor Properties")
		void SetParented(bool Parented);

	UFUNCTION(BlueprintPure, Category = "Gaze Actor Properties")
		int32 GetGrouped() { return GroupID; }

	UFUNCTION(BlueprintPure, Category = "Gaze Actor Properties")
		FString GetGroupName();

	UFUNCTION(BlueprintPure, Category = "Gaze Actor Properties")
		int32 GetHitID(bool IsGazeInteraction);

	int32 GetHitID(bool IsGazeInteraction, FVector2D HitUV);

	UFUNCTION(BlueprintCallable, Category = "Gaze Actor Properties")
		void SetGrouped(int32 _Grouped);

	UFUNCTION(BlueprintCallable, Category = "Gaze Actor Movable")
		void SetMovable(bool Movable);

	UFUNCTION(BlueprintCallable, Category = "Gaze Actor Movable")
		void SetDimensions(FVector Dimensions);
	UFUNCTION(BlueprintPure, Category = "Gaze Actor Movable")
		FVector GetDimensions() { return Dimensions; }
	
	UFUNCTION(BlueprintCallable, Category = "Gaze Actor Movable")
		FString GetNatNetName() { return NatNetName; }

	UFUNCTION(BlueprintPure, Category = "Gaze Actor")
		UMaterialInstanceDynamic* GetMaterial();

	UFUNCTION(BlueprintCallable, Category = "Gaze Actor")
		void RotateUVs(int32 Angle);

	UFUNCTION(BlueprintCallable, Category = "Gaze Actor")
		TArray<FGazeActorData> SliceGazeActor(UProceduralMeshComponent* First, UProceduralMeshComponent* Second);

	UFUNCTION(BlueprintPure, Category = "Gaze Actor")
		FString GetButtonMeshName();

	UFUNCTION(BlueprintPure, Category = "Gaze Actor")
		FString GetSplineMeshName();

	UFUNCTION(BlueprintCallable, Category = "Gaze Actor")
		void SetButtonMeshName(FString ButtonMeshName);

	UFUNCTION(BlueprintCallable, Category = "Triangle Information")
		void GetTriangleInformation(int32 TriangleIndex, TArray<FVector> &OutVerticies, TArray<FVector2D> &OutUVs);

	UFUNCTION(BlueprintCallable, Category = "Gaze Actor")
		void SetSplineMeshName(FString SplineMeshName);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Get the name of the rigid body which used in motive */
	FString GetRigidBodyName();

	//Blueprint event to call the create mesh blueprint function from c++
	UFUNCTION(BlueprintImplementableEvent, Category = "Gaze Actor")
	void CreateMeshInBP(bool _Movable, bool hasUVs);

	UFUNCTION(BlueprintImplementableEvent, Category = "Gaze Actor")
	void SetupMeshCollisionInBP();

	UProceduralMeshComponent* GetProceduralMeshComponent();

	//===============Replay & Recording==========================

	//Set the CompressedGazeBlock that this actor will update during recording
	void SetCompressedGazeBlock(CompressedGazeBlock* GazeBlock);

	//Update the gaze blocks replay data (might just do it in tick instead. e.g. make this private)
	//Returns true if the gazeblock contains a keyframe
	bool RecordFrameData(float DeltaTime);

	//Prepare the resources used during replay or recording
	UFUNCTION(BlueprintCallable, Category = "Gaze Actor")
	void PrepareForRecordAndReplay(int32 NumberOfScreens, int32 Index);

	//Release the resources used during replay or recording
	void StopRecordAndReplay();

	//Callback from the decoding task once an image has been decoded
	void OnImageDecoded(UTexture2D* DecodedImage);

	//Callback from the encoding task once an image has been encoded
	void OnImageEncoded(ImageDataStructures::VP8EncodedImageData* EncodedImage);

	FGazeActorData GetDataStruct();

	UFUNCTION(BlueprintCallable, Category = "Gaze Actor")
	void CalibrateHeight(float DeltaHeight);

	UFUNCTION(BlueprintPure, Category = "Gaze Actor")
	int GetCalibrationVerticeIndex();

	void SetCalibrationVerticeIndex(int _Index);

	UFUNCTION(BlueprintCallable, Category = "Gaze Actor")
		void LoadImageAndAOIsAsync(FImageAndAOIs Data);

	UFUNCTION(BlueprintCallable, Category = "Gaze Actor")
		void UpdatePopupAndAOIsAsync(FPopupAndAOIs Data);

	//TODO: we will work on this later
	FMatrix GetOpenFaceTransformation();
};
