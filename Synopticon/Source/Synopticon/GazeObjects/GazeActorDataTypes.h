#pragma once

#include "CoreMinimal.h"

//header file to save and load binary
#include "Archive.h"

#include "Utils/SynOpticonStatics.h"

//#include "ArchiveBase.h"
#include "StaticRoomActorDatatypes.h"

#include "GazeActorDataTypes.generated.h"

UENUM(BlueprintType)
enum class EMicroTaskStatus : uint8
{
	COMPLETE 	UMETA(DisplayName = "Task Completed"),
	STARTED 	UMETA(DisplayName = "Task Started"),
	NA 			UMETA(DisplayName = "Not applicable")
};

USTRUCT(BlueprintType)
struct FAOIData //TODO optimize so that the boundng box is calculated on creation and stored for use in the collision check
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOI Data")
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOI Data")
		int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOI Data")
		TArray<FVector2D> Coordinates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOI Data")
		TArray<int32> TriangleIndices;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOI Data")
		FLinearColor Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOI Data")
		EMicroTaskStatus MicroTaskStatus;

	FAOIData()
	{
		Name = "";
		ID = -1; 
		Color = FLinearColor(100.f/255.f, 191.f/255.f, 255.f/255.f, 1.f);
		MicroTaskStatus = EMicroTaskStatus::NA;
	}

	FAOIData(FString AOIName, int32 AOIID)
	{
		Name = AOIName;
		ID = AOIID;
		Color = FLinearColor(100.f / 255.f, 191.f / 255.f, 255.f / 255.f, 1.f);
		MicroTaskStatus = EMicroTaskStatus::NA;
	}

	//Check if Point is inside Polygon
	bool IsInsidePolygon(FVector2D Point) {
		return USynOpticonStatics::WindingNumberTest(Point, Coordinates);
	}

	//Simple rectangle check for now 
	bool IsInside(FVector2D Point) {
		FVector2D Min = Coordinates[0];
		FVector2D Max = Coordinates[0];
		for (FVector2D Coordinate : Coordinates) {
			if (Coordinate.X < Min.X) {
				Min.X = Coordinate.X;
			}
			if (Coordinate.Y < Min.Y) {
				Min.Y = Coordinate.Y;
			}
			if (Coordinate.X > Max.X) {
				Max.X = Coordinate.X;
			}
			if (Coordinate.Y > Max.Y) {
				Max.Y = Coordinate.Y;
			}
		}

		//If we are inside the rectangle we perform the more expensive polygon check
		if (Point.X < Max.X && Point.X > Min.X
			&& Point.Y < Max.Y && Point.Y > Min.Y) {
			return IsInsidePolygon(Point);
		}
		return false;
	}
};
FORCEINLINE bool operator==(FAOIData& Left, FAOIData Right)
{
	return Left.ID==Right.ID;
}

/** A Struct for storing Gaze Parent information */

USTRUCT(BlueprintType)
struct FGazeGroupData
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gaze Group Data")
		FString GroupName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gaze Group Data")
		int32 GroupID;

	FGazeGroupData()
	{
		GroupName = "";
		GroupID = -1;
	}

	FGazeGroupData(FString _GroupName, int32 _ID)
	{
		GroupName = _GroupName;
		GroupID = _ID;
	}
};
FORCEINLINE bool operator==(FGazeGroupData& Left, FGazeGroupData Right)
{
	return Left.GroupName.Equals(Right.GroupName);
}

USTRUCT(BlueprintType)
struct FGazeParentActorData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigid Loader Data")
		int32 Version;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gaze Parent Data")
		FString SceneCompName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gaze Parent Data")
		FString NatNetName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gaze Parent Data")
		FVector InitialLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gaze Parent Data")
		FQuat CalibrationQuat;

	FGazeParentActorData() {
		Version = 1;
		SceneCompName = "";
		NatNetName = "";
		InitialLocation = FVector(0, 0, 0);
		CalibrationQuat = FQuat::Identity;
	}

	FGazeParentActorData(FString _SceneCompName, FString _NatNetName, FVector _InitialLocation) {
		Version = 1;
		SceneCompName = _SceneCompName;
		NatNetName = _NatNetName;
		InitialLocation = _InitialLocation;
		CalibrationQuat = FQuat::Identity;
	}
};

FORCEINLINE FArchive& operator<<(FArchive &Ar, FGazeParentActorData &GazeParentData)
{
	Ar << GazeParentData.Version;
	Ar << GazeParentData.SceneCompName;
	Ar << GazeParentData.NatNetName;
	Ar << GazeParentData.InitialLocation;

	return Ar;
}

/** A Struct for storing Rigid body information */
USTRUCT(BlueprintType)
struct FGazeActorData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigid Body Data")
		int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigid Loader Data")
		int32 Version;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigid Body Data")
		EStaticRoomActorType GazeActorType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigid Body Data")
		bool IsGazeObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigid Body Data")
		bool IsCollisionObject;

	/** Name of the Gaze Actor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigid Body Data")
	FString Name;

	/** Position of the makers*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigid Body Data")
		TArray<FVector> Markers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigid Body Data")
		TArray<FVector> Vertices;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigid Body Data")
		TArray<FVector2D> UVs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigid Body Data")
		TArray<int32> Indexes;

	/** AOIs */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigid Body Data")
		TArray<FAOIData> AOIS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigid Body Data")
		TArray<FAOIData> PopupAOIS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigid Body Data")
		FString WAMPScreenID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigid Body Data")
		FVector2D WebBrowserResolution;

	/** The URL to display on the screen */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigid Body Data")
	FString URL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigid Body Data")
	bool Parented;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigid Body Data")
	FString ParentSceneCompName;

	//////////////////    MOVABLE ACTOR PROPERTIES   ////////////////////////
	/** True if the GazeActor is of the Movable type*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigid Body Data")
	bool Movable;
	
	/** Name of the Actors rigid body in Motive */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigid Body Data")
	FString NatNetName;

	/** The size of the movable rigid body. X = width, Y = Length, Z = Height */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigid Body Data")
	FVector SizeDimensions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigid Body Data")
	int32 FontSize;

	//==================== Sync Data ========================
	//NB Should not be serialized
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigid Body Sync Data")
	int32 VideoIndex; //If VideoIndex == -1 it is not applicable for this actor

	//==================== Texture Data ========================
	//NB Should not be serialized
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigid Body Sync Data")
	FString TextureFilepath; //If TextureFilepath == "NotSet" it is not applicable for this actor
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigid Body Data")
	int32 Grouped;

	//=================== Action Object =================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigid Body Data")
	FString ButtonMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigid Body Data")
	FString SplineMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigid Body Data")
	int CalibrationVerticeIndex;

	FGazeActorData()
	{		
		Version = 2;
		ID = -1;
		GazeActorType = EStaticRoomActorType::SRT_Screen;
		Name = "Not set yet";
		URL = "www.google.com";
		NatNetName = "";
		Movable = false;
		Parented = false;
		ParentSceneCompName = "";
		SizeDimensions.Set(0, 0, 0);
		FontSize = 12;

		VideoIndex = -1;

		TextureFilepath = "No Texture";
		WAMPScreenID = "";

		IsGazeObject = false;
		IsCollisionObject = false;
		Grouped = -1;

		ButtonMesh = "";
		SplineMesh = "";
		CalibrationVerticeIndex = -1;
		WebBrowserResolution.Set(0, 0);
	}

	FGazeActorData(FString _Name, FString _URL, FString _NatNetName, bool _Movable, bool _Parented, 
		FString _ParentSceneCompName, TArray<FVector> _Vertices, TArray<FVector2D> _UVs, TArray<int32> _Indexes, 
		int32 _FontSize, EStaticRoomActorType Type, bool _IsGazeObject, FString _TextureFilepath)
	{
		Version = 2;
		ID = -1;
		GazeActorType = Type;
		Name = _Name;
		URL = _URL;
		NatNetName = _NatNetName;
		Movable = _Movable;
		Parented = _Parented;
		ParentSceneCompName = _ParentSceneCompName;
		SizeDimensions.Set(0, 0, 0);
		Vertices = _Vertices;
		UVs = _UVs;
		Indexes = _Indexes;
		FontSize = _FontSize;

		VideoIndex = -1;

		TextureFilepath = _TextureFilepath;

		IsGazeObject = _IsGazeObject;
		ButtonMesh = "";
		SplineMesh = "";
		Grouped = -1;

		CalibrationVerticeIndex = -1;
		WAMPScreenID = "";
	}

	FGazeActorData(FString _Name, FString _URL, FString _NatNetName, bool _Movable, bool _Parented, FString _ParentSceneCompName, TArray<FVector> _Vertices, TArray<FVector2D> _UVs, TArray<int32> _Indexes, int32 _FontSize, EStaticRoomActorType Type, bool _IsGazeObject, FString _ButtonMesh, FString _SplineMesh)
	{
		Version = 2;
		ID = -1;
		GazeActorType = Type;
		Name = _Name;
		URL = _URL;
		NatNetName = _NatNetName;
		Movable = _Movable;
		Parented = _Parented;
		ParentSceneCompName = _ParentSceneCompName;
		SizeDimensions.Set(0, 0, 0);
		Vertices = _Vertices;
		UVs = _UVs;
		Indexes = _Indexes;
		FontSize = _FontSize;

		VideoIndex = -1;

		TextureFilepath = "No Texture";

		IsGazeObject = _IsGazeObject;

		ButtonMesh = _ButtonMesh;
		SplineMesh = _SplineMesh;
		Grouped = -1;

		CalibrationVerticeIndex = -1;
		WAMPScreenID = "";
	}
};

FORCEINLINE FArchive& operator<<(FArchive &Ar, FGazeActorData &GazeData)
{
	Ar << GazeData.Version;

	Ar << GazeData.ID;
	Ar << GazeData.GazeActorType;
	Ar << GazeData.Name;
	Ar << GazeData.Vertices;
	Ar << GazeData.UVs;
	Ar << GazeData.Indexes;
	Ar << GazeData.URL;
	Ar << GazeData.Movable;

	Ar << GazeData.Parented;
	if (GazeData.Parented) {
		Ar << GazeData.ParentSceneCompName;
	}

	Ar << GazeData.NatNetName;
	Ar << GazeData.SizeDimensions;
	Ar << GazeData.FontSize;

	if (GazeData.Version == 1) {
		GazeData.IsGazeObject = true;
	}
	else {
		Ar << GazeData.IsGazeObject;
	}

	return Ar;
}

/** A Struct for storing all the rigid body data */
USTRUCT(BlueprintType)
struct FGazeObjectAndParentData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigid Loader Data")
		int32 Version;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gaze Parent Data")
		TArray<FGazeParentActorData> GazeParentActorsData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gaze Parent Data")
		TArray<FGazeActorData> GazeActorsData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gaze Group Data")
		TArray<FGazeGroupData> GazeGroupData;

	FGazeObjectAndParentData() {
		Version = 1;
	}

	FGazeObjectAndParentData(TArray<FGazeParentActorData> _GazeParentActorsData, TArray<FGazeActorData> _GazeActorsData,
		TArray<FGazeGroupData> _GroupData) 
	{
		Version = 1;
		GazeParentActorsData = _GazeParentActorsData;
		GazeActorsData = _GazeActorsData;
		GazeGroupData = _GroupData;
	}
};

FORCEINLINE FArchive& operator<<(FArchive &Ar, FGazeObjectAndParentData &GazeObjectAndParentData)
{
	Ar << GazeObjectAndParentData.Version;
	Ar << GazeObjectAndParentData.GazeParentActorsData;
	Ar << GazeObjectAndParentData.GazeActorsData;

	return Ar;
}

USTRUCT(BlueprintType)
struct FSlicingData
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slicing Data")
		TArray<FVector> Vertices;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slicing Data")
		TArray<FVector2D> UVs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slicing Data")
		TArray<int32> Indexes;

	FSlicingData(TArray<FVector> FV, TArray<FVector2D> FUv, TArray<int32> FI)
	{
		Vertices = FV;
		UVs = FUv;
		Indexes = FI;
	}

	FSlicingData() {

	}
};

