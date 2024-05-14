// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Archive.h"
//#include "ArchiveBase.h"
#include "SystemClasses/SynOpticonCustomType.h"

#include "SynOpticonDataStructures.generated.h"

/** A Struct for storing Object propertites */

USTRUCT(BlueprintType)
struct FObjectLoggingInfoStruct
{
	GENERATED_USTRUCT_BODY()

	/** Name of the Object */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Logging Info Struct")
	int32 GazeActorID;

	/** Duation the object is looked at */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Logging Info Struct")
	float Time;

	/** Number of times the object is looked at */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Logging Info Struct")
	int Count;

	/** Percentage of the duration the object is looked at */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Logging Info Struct")
	float Percentage;

	FObjectLoggingInfoStruct()
	{
		//Name = "ObjectNameNotSet";
		Time = 0;
		Count = 0;
		Percentage = 0;
		GazeActorID = -1;
	}
};
FORCEINLINE FArchive& operator<<(FArchive &Ar, FObjectLoggingInfoStruct &ObjectLoggingInfoStruct)
{
	Ar << ObjectLoggingInfoStruct.GazeActorID;
	Ar << ObjectLoggingInfoStruct.Time;
	Ar << ObjectLoggingInfoStruct.Count;
	Ar << ObjectLoggingInfoStruct.Percentage;

	return Ar;
}

/** A Struct for storing Gase Path propertites */
USTRUCT(BlueprintType)
struct FGazePathLoggingInfoStruct
{
	GENERATED_USTRUCT_BODY()

	/*Name of the Object*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gaze Path Logging Info Struct")
	int32 GazeActorID;

	/** Gaze Path start time */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gaze Path Logging Info Struct")
	FDateTime TimeStamp;

	/** Duration of the gaze */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gaze Path Logging Info Struct")
	float Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gaze Path Logging Info Struct")
	FVector2D HitUV;

	FGazePathLoggingInfoStruct()
	{
		Duration = 0;
		TimeStamp = FDateTime::Now();
		GazeActorID = -1;
		HitUV = FVector2D(-1, -1);
	}
	bool operator == (const FGazePathLoggingInfoStruct& a) const
	{
		return (GazeActorID == a.GazeActorID);
	}
};
FORCEINLINE FArchive& operator<<(FArchive &Ar, FGazePathLoggingInfoStruct &GazePathLoggingInfoStruct)
{
	Ar << GazePathLoggingInfoStruct.GazeActorID;
	Ar << GazePathLoggingInfoStruct.TimeStamp;
	Ar << GazePathLoggingInfoStruct.Duration;

	return Ar;
}

/**
* A Struct for storing SynOpticonActor propertites
*/
USTRUCT(BlueprintType)
struct FSynOpticonActorLoggingStruct
{
	GENERATED_USTRUCT_BODY()

	/** Name of the Glasses */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon Actor Logging Struct")
	int32 SynOpticonActorID;

	/** Total using time of the Glasses */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon Actor Logging Struct")
	float TotalTime;

	/** Objects were looked at by this glasses */
	TMap<int32, FObjectLoggingInfoStruct> Objects;

	/** Gaze paths of this glasses */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon Actor Logging Struct")
	TArray<FGazePathLoggingInfoStruct> GazePaths;

	FSynOpticonActorLoggingStruct()
	{
		SynOpticonActorID = -1;
		GazePaths.Empty();
		Objects.Empty();
		TotalTime = 0;
	}
};
FORCEINLINE FArchive& operator<<(FArchive &Ar, FSynOpticonActorLoggingStruct &SynOpticonActorLogging)
{
	Ar << SynOpticonActorLogging.SynOpticonActorID;
	Ar << SynOpticonActorLogging.TotalTime;
	Ar << SynOpticonActorLogging.Objects;
	Ar << SynOpticonActorLogging.GazePaths;

	return Ar;
}
//====================================================================================================
USTRUCT(BlueprintType)
struct FEyeTrackerStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon EyeTracker Struct")
		FString GlassesID; //e.g. GazeDataServer

	FEyeTrackerStruct()
	{
		GlassesID = "";
	}
};
FORCEINLINE FArchive& operator<<(FArchive &Ar, FEyeTrackerStruct &EyeTrackerStruct)
{
	Ar << EyeTrackerStruct.GlassesID; //e.g. GazeDataServer

	return Ar;
}

//====================================================================================================
USTRUCT(BlueprintType)
struct FRemoteEyeTrackerStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon RemoteEyeTracker Struct")
		FString RemoteTrackerID; //e.g. GazeDataServer

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon RemoteEyeTracker Struct")
		FVector PointToLookAt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon RemoteEyeTracker Struct")
		FVector RemoteTrackerPositionRight; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon RemoteEyeTracker Struct")
		FVector RemoteTrackerPositionLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon RemoteEyeTracker Struct")
		FString AttachedScreenName;

	FRemoteEyeTrackerStruct()
	{
		RemoteTrackerID = "";
		PointToLookAt.Set(0, 0, 0);
		RemoteTrackerPositionLeft.Set(0, 0, 0);
		RemoteTrackerPositionRight.Set(0, 0, 0);
		AttachedScreenName = "";
	}
};
FORCEINLINE FArchive& operator<<(FArchive &Ar, FRemoteEyeTrackerStruct &RemoteEyeTrackerStruct)
{
	Ar << RemoteEyeTrackerStruct.RemoteTrackerID; 
	Ar << RemoteEyeTrackerStruct.PointToLookAt;
	Ar << RemoteEyeTrackerStruct.RemoteTrackerPositionLeft;
	Ar << RemoteEyeTrackerStruct.RemoteTrackerPositionRight;
	Ar << RemoteEyeTrackerStruct.AttachedScreenName;

	return Ar;
}

//====================================================================================================
USTRUCT(BlueprintType)
struct FOpenFaceStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon OpenFaceTracker Struct")
		FString IpAddress; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon OpenFaceTracker Struct")
		int32 Port; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon OpenFaceTracker Struct")
		FVector CameraTrackerPositionRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon OpenFaceTracker Struct")
		FVector CameraTrackerPositionLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon OpenFaceTracker Struct")
		FString AttachedScreenName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon OpenFaceTracker Struct")
		bool IsTrackingHead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon OpenFaceTracker Struct")
		bool IsTrackingEyes;

	FOpenFaceStruct()
	{
		IpAddress = "";
		Port = -1;
		CameraTrackerPositionLeft.Set(0, 0, 0);
		CameraTrackerPositionRight.Set(0, 0, 0);
		AttachedScreenName = "";
		IsTrackingHead = true;
		IsTrackingEyes = true;
	}
};
FORCEINLINE FArchive& operator<<(FArchive &Ar, FOpenFaceStruct &OpenFaceStruct)
{
	Ar << OpenFaceStruct.IpAddress;
	Ar << OpenFaceStruct.Port;
	Ar << OpenFaceStruct.CameraTrackerPositionLeft;
	Ar << OpenFaceStruct.CameraTrackerPositionRight;
	Ar << OpenFaceStruct.AttachedScreenName;

	return Ar;
}

USTRUCT(BlueprintType)
struct FVideoComponentStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon VideoComponent Struct")
		FString VideoFeedName; //Friendly name


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon VideoComponent Struct")
		FString VideoURL; //Used in real-time/recording

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon VideoComponent Struct")
		FString VideoRecordingFilepath; //Used when replaying

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon VideoComponent Struct")
		int32 SelectedVideoTrack; //Which video track is selected

	FVideoComponentStruct()
	{
		VideoFeedName = "";
		VideoURL = "";
		SelectedVideoTrack = 0;
		VideoRecordingFilepath = "";
	}

	bool operator == (const FVideoComponentStruct& a) const
	{
		return (VideoFeedName.Equals(a.VideoFeedName) && VideoURL.Equals(a.VideoURL) && SelectedVideoTrack == a.SelectedVideoTrack 
			&& VideoRecordingFilepath.Equals(a.VideoRecordingFilepath));
	}
};
FORCEINLINE FArchive& operator<<(FArchive &Ar, FVideoComponentStruct &VideoCompStruct)
{
	Ar << VideoCompStruct.VideoFeedName;
	Ar << VideoCompStruct.VideoURL;
	Ar << VideoCompStruct.SelectedVideoTrack;
	Ar << VideoCompStruct.VideoRecordingFilepath;
	
	return Ar;
}

//====================================================================================================
USTRUCT(BlueprintType)
struct FHandStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon Hand Struct")
	FString NatNetName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon Hand Struct")
	MarkersConfigurationEnum MarkersConfiguration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon Hand Struct")
	bool LeftHand;

	FHandStruct()
	{
		NatNetName = "";
		MarkersConfiguration = MarkersConfigurationEnum::VE_Position;
		LeftHand = true;
	}

	FHandStruct(FString _NatNetName, MarkersConfigurationEnum _MarkersConfiguration, bool _LeftHand)
	{
		NatNetName = _NatNetName;
		MarkersConfiguration = _MarkersConfiguration;
		LeftHand = _LeftHand;
	}
};
FORCEINLINE FArchive& operator<<(FArchive &Ar, FHandStruct &HandStruct)
{
	Ar << HandStruct.NatNetName;
	Ar << HandStruct.MarkersConfiguration;
	Ar << HandStruct.LeftHand;

	return Ar;
}
//====================================================================================================
USTRUCT(BlueprintType)
struct FShimmerComponentStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon Shimmer Struct")
		FString DeviceName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon Shimmer Struct")
		bool HasHeartRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon Shimmer Struct")
		bool HasGalvanicSkinResponse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon Shimmer Struct")
		int32 ShimmerIndex;

	FShimmerComponentStruct()
	{
		DeviceName = "";
		HasHeartRate = false;
		HasGalvanicSkinResponse = false;
		ShimmerIndex = -1;
	}
};
FORCEINLINE FArchive& operator<<(FArchive &Ar, FShimmerComponentStruct &ShimmerComponentStruct)
{
	Ar << ShimmerComponentStruct.DeviceName; //(e.g., Shimmer3-9F24@localhost:6789)
	Ar << ShimmerComponentStruct.HasHeartRate;
	Ar << ShimmerComponentStruct.HasGalvanicSkinResponse;
	Ar << ShimmerComponentStruct.ShimmerIndex;
	
	return Ar;
}

//===================================================
USTRUCT(BlueprintType)
struct FFixationGroupStruct
{
	GENERATED_USTRUCT_BODY()

	int32 GazeActorId;
	FString SessionId;
	bool bPopup;
	FVector Location;
	float Duration;
	FDateTime Timestamp;
	FVector Normal;
	FVector2D HitUV;
	FFixationGroupStruct()
	{
		GazeActorId = -1;
	}
	/*FFixationGroupStruct(int32 _ID, FString _SessionId, bool _bPopup, FVector _Location, float _Duration, FDateTime _Timestamp)
	{
		GazeActorId = _ID;
		SessionId = _SessionId;
		bPopup = _bPopup;
		Location = _Location;
		Duration = _Duration;
		Timestamp = _Timestamp;
		Normal = FVector(0, 0, 0);
		HitUV = FVector2D(-1, -1);
	}*/
	FFixationGroupStruct(int32 _ID, FString _SessionId, bool _bPopup, FVector _Location, float _Duration, FDateTime _Timestamp, FVector _Normal, FVector2D _HitUV)
	{
		GazeActorId = _ID;
		SessionId = _SessionId;
		bPopup = _bPopup;
		Location = _Location;
		Duration = _Duration;
		Timestamp = _Timestamp;
		Normal = _Normal;
		HitUV = _HitUV;
	}
};
FORCEINLINE FArchive& operator<<(FArchive &Ar, FFixationGroupStruct &FixationGroupStruct)
{
	Ar << FixationGroupStruct.GazeActorId;
	Ar << FixationGroupStruct.GazeActorId;
	Ar << FixationGroupStruct.Location;
	Ar << FixationGroupStruct.Duration;
	Ar << FixationGroupStruct.Timestamp;

	//Init normal
	FixationGroupStruct.Normal = FVector(0, 0, 0);

	return Ar;
}
//====================================================================================================
USTRUCT(BlueprintType)
struct FSynOpticonActorStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon Actor Struct")
		int32 HeaderVersion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon Actor Struct")
		FString ActorName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon Actor Struct")
		FString NatNetName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon Actor Struct")
		FEyeTrackerStruct EyeTrackerStruct;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon Actor Struct")
		FRemoteEyeTrackerStruct RemoteEyeTrackerStruct;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon Actor Struct")
		TArray<FShimmerComponentStruct> ShimmerComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon Actor Struct")
		TArray<FHandStruct> HandComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon Actor Struct")
		TArray<FVideoComponentStruct> VideoComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon Actor Struct")
		FSynOpticonActorLoggingStruct GazeLogComponentData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon Actor Struct")
		FOpenFaceStruct VisualComponentData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon Actor Struct")
		FSynOpticonActorLoggingStruct LeftHandLogComponentData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon Actor Struct")
		FSynOpticonActorLoggingStruct RightHandLogComponentData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon Actor Struct")
		TArray<FFixationGroupStruct> GazeData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon Actor Struct")
		bool HasMyo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SynOpticon Actor Struct")
		MarkersConfigurationEnum MarkersConfiguration;

	FSynOpticonActorStruct()
	{
		HeaderVersion = 0;
		ActorName = "";
		NatNetName = "";
		HasMyo = false;
		VideoComponents = TArray<FVideoComponentStruct>();
		MarkersConfiguration = MarkersConfigurationEnum::VE_Position;
	}
	FSynOpticonActorStruct(FString _ActorName, FString _NatNetName, 
		FEyeTrackerStruct _EyeTrackerStruct, FRemoteEyeTrackerStruct _RemoteEyeTrackerStruct,
		TArray<FShimmerComponentStruct> _ShimmerComponents, bool _HasMyo)
	{
		HeaderVersion = 0;
		ActorName = _ActorName;
		NatNetName = _NatNetName;
		EyeTrackerStruct = _EyeTrackerStruct;
		RemoteEyeTrackerStruct = _RemoteEyeTrackerStruct;
		ShimmerComponents = _ShimmerComponents;
		HasMyo = _HasMyo;
		MarkersConfiguration = MarkersConfigurationEnum::VE_Position;
		VideoComponents = TArray<FVideoComponentStruct>();
	}
	FSynOpticonActorStruct(FString _ActorName, FString _NatNetName,
		FEyeTrackerStruct _EyeTrackerStruct, FRemoteEyeTrackerStruct _RemoteEyeTrackerStruct,
		TArray<FShimmerComponentStruct> _ShimmerComponents, TArray<FHandStruct> _HandComponents, bool _HasMyo)
	{
		HeaderVersion = 2;
		ActorName = _ActorName;
		NatNetName = _NatNetName;
		EyeTrackerStruct = _EyeTrackerStruct;
		RemoteEyeTrackerStruct = _RemoteEyeTrackerStruct;
		ShimmerComponents = _ShimmerComponents;
		HasMyo = _HasMyo;
		MarkersConfiguration = MarkersConfigurationEnum::VE_Position;
		HandComponents = _HandComponents;
		VideoComponents = TArray<FVideoComponentStruct>();
	}
	FSynOpticonActorStruct(FString _ActorName, FString _NatNetName,
		FEyeTrackerStruct _EyeTrackerStruct, FRemoteEyeTrackerStruct _RemoteEyeTrackerStruct,
		TArray<FShimmerComponentStruct> _ShimmerComponents, TArray<FHandStruct> _HandComponents, FOpenFaceStruct _VisualComponentData)
	{
		HeaderVersion = 2;
		ActorName = _ActorName;
		NatNetName = _NatNetName;
		EyeTrackerStruct = _EyeTrackerStruct;
		RemoteEyeTrackerStruct = _RemoteEyeTrackerStruct;
		ShimmerComponents = _ShimmerComponents;
		HasMyo = false;
		MarkersConfiguration = MarkersConfigurationEnum::VE_Position;
		HandComponents = _HandComponents;
		VisualComponentData = _VisualComponentData;
		VideoComponents = TArray<FVideoComponentStruct>();
	}
	FSynOpticonActorStruct(FString _ActorName, FString _NatNetName,
		FEyeTrackerStruct _EyeTrackerStruct, FRemoteEyeTrackerStruct _RemoteEyeTrackerStruct,
		TArray<FShimmerComponentStruct> _ShimmerComponents, TArray<FHandStruct> _HandComponents, FOpenFaceStruct _VisualComponentData,
		TArray<FVideoComponentStruct> _VideoComponents)
	{
		HeaderVersion = 3;
		ActorName = _ActorName;
		NatNetName = _NatNetName;
		EyeTrackerStruct = _EyeTrackerStruct;
		RemoteEyeTrackerStruct = _RemoteEyeTrackerStruct;
		ShimmerComponents = _ShimmerComponents;
		HasMyo = false;
		MarkersConfiguration = MarkersConfigurationEnum::VE_Position;
		HandComponents = _HandComponents;
		VisualComponentData = _VisualComponentData;
		VideoComponents = _VideoComponents;
	}
};
FORCEINLINE FArchive& operator<<(FArchive &Ar, FSynOpticonActorStruct &SynOpticonActorStruct)
{
	uint32 ArLocation = Ar.Tell();
	Ar << SynOpticonActorStruct.HeaderVersion;

	if (SynOpticonActorStruct.HeaderVersion == 3) {
		Ar << SynOpticonActorStruct.ActorName;
		Ar << SynOpticonActorStruct.NatNetName;
		Ar << SynOpticonActorStruct.EyeTrackerStruct;
		Ar << SynOpticonActorStruct.RemoteEyeTrackerStruct;
		Ar << SynOpticonActorStruct.ShimmerComponents;
		Ar << SynOpticonActorStruct.GazeLogComponentData;
		Ar << SynOpticonActorStruct.LeftHandLogComponentData;
		Ar << SynOpticonActorStruct.RightHandLogComponentData;
		Ar << SynOpticonActorStruct.GazeData;
		Ar << SynOpticonActorStruct.HasMyo;
		Ar << SynOpticonActorStruct.HandComponents;
		Ar << SynOpticonActorStruct.VideoComponents;
	}
	else if (SynOpticonActorStruct.HeaderVersion == 2) {
		Ar << SynOpticonActorStruct.ActorName;
		Ar << SynOpticonActorStruct.NatNetName;
		Ar << SynOpticonActorStruct.EyeTrackerStruct;
		Ar << SynOpticonActorStruct.RemoteEyeTrackerStruct;
		Ar << SynOpticonActorStruct.ShimmerComponents;
		Ar << SynOpticonActorStruct.GazeLogComponentData;
		Ar << SynOpticonActorStruct.LeftHandLogComponentData;
		Ar << SynOpticonActorStruct.RightHandLogComponentData;
		Ar << SynOpticonActorStruct.GazeData;
		Ar << SynOpticonActorStruct.HasMyo;
		Ar << SynOpticonActorStruct.HandComponents;
	}
	else if (SynOpticonActorStruct.HeaderVersion == 1) {
		Ar << SynOpticonActorStruct.ActorName;
		Ar << SynOpticonActorStruct.NatNetName;
		Ar << SynOpticonActorStruct.EyeTrackerStruct;
		Ar << SynOpticonActorStruct.RemoteEyeTrackerStruct;
		Ar << SynOpticonActorStruct.ShimmerComponents;
		Ar << SynOpticonActorStruct.GazeLogComponentData;
		Ar << SynOpticonActorStruct.GazeData;
		Ar << SynOpticonActorStruct.HasMyo;
		Ar << SynOpticonActorStruct.HandComponents;
	}
	else {
		Ar.Seek(ArLocation);
		Ar << SynOpticonActorStruct.ActorName;
		Ar << SynOpticonActorStruct.NatNetName;
		Ar << SynOpticonActorStruct.EyeTrackerStruct;
		Ar << SynOpticonActorStruct.RemoteEyeTrackerStruct;
		Ar << SynOpticonActorStruct.ShimmerComponents;
		Ar << SynOpticonActorStruct.GazeLogComponentData;
		Ar << SynOpticonActorStruct.GazeData;
		Ar << SynOpticonActorStruct.HasMyo;
	}

	return Ar;
}

struct FFixationPoint
{
	FVector Location;
	float Duration;
};
FORCEINLINE FArchive& operator<<(FArchive &Ar, FFixationPoint &FixationPoint)
{
	Ar << FixationPoint.Location;
	Ar << FixationPoint.Duration;

	return Ar;
}