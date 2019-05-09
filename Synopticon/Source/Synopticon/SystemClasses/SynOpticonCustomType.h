#pragma once

#include "CoreMinimal.h"

#include <array>

#include "SynOpticonCustomType.generated.h"

UENUM(BlueprintType)		
enum class VectorVisibilityEnum : uint8
{
	VE_OrientationVector 	UMETA(DisplayName = "Orientation Vector"),
	VE_EyeVector 			UMETA(DisplayName = "Eye Vector"),
	VE_ConvergenceVector 	UMETA(DisplayName = "Convergence Vector")
};

UENUM(BlueprintType)		
enum class UIContextEnum : uint8
{
	DialogContext			UMETA(DisplayName = "Dialog context"),
	NoDialogContext			UMETA(DisplayName = "No Dialog context"),
	NoHotkeys 				UMETA(DisplayName = "No hotkey context"),
	Hotbar 					UMETA(DisplayName = "Hotbar context"),
	VectorGroupMenu 		UMETA(DisplayName = "Vector Group Menu context"),
	CalibrationMenu 		UMETA(DisplayName = "Calibration Menu context"),
	ModellingMenu 			UMETA(DisplayName = "Modelling Menu context"),
	SynopticonMenu 			UMETA(DisplayName = "Synopticon Menu context"),
	HeatmapMenu 			UMETA(DisplayName = "Heatmap Menu context"),
	ActorControlMenu 		UMETA(DisplayName = "ActorControl Menu context"),
	InteractionMenu 		UMETA(DisplayName = "Interaction Menu context"),
	RecordAndReplayMenu 	UMETA(DisplayName = "Record And Replay Menu context")
};

UENUM(BlueprintType)		
enum class MarkersConfigurationEnum: uint8
{
	VE_Glasses_Right 		UMETA(DisplayName = "Glasses Right"),
	VE_Glasses_Left 		UMETA(DisplayName = "Glasses Left"),
	VE_Position 			UMETA(DisplayName = "Position"),
	VE_PositionOnly 		UMETA(DisplayName = "Position Only"),
	VE_Laser				UMETA(DisplayName = "Laser Tool"),
	VE_ManualTool			UMETA(DisplayName = "Manual Tool"),
	VE_CalibrationTool		UMETA(DisplayName = "Calibration Tool"),
	VE_PositionCap			UMETA(DisplayName = "Cap")
};

UENUM(BlueprintType)		
enum class ComponentTypeEnum : uint8
{
	VE_Glasses 				UMETA(DisplayName = "Glasses Component"),
	VE_RemoteTracker 		UMETA(DisplayName = "Remote Tracker Component"),
	VE_NatNet 				UMETA(DisplayName = "NatNet Component"),
	VE_Shimmer 				UMETA(DisplayName = "Shimmer Component"),
	VE_Components 			UMETA(DisplayName = "Component"),
	VE_OpenFaceTracker		UMETA(DisplayName = "OpenFace Tracker Component")
};

FORCEINLINE bool operator==(ComponentTypeEnum e, FString s)
{
	if (e == ComponentTypeEnum::VE_Glasses && s.Equals("glasses")) return true;
	if (e == ComponentTypeEnum::VE_RemoteTracker && s.Equals("remote_tracker")) return true;
	if (e == ComponentTypeEnum::VE_NatNet && s.Equals("natnet")) return true;
	if (e == ComponentTypeEnum::VE_Shimmer && s.Equals("shimmer")) return true;
	if (e == ComponentTypeEnum::VE_Components && s.Equals("chart")) return true;
	return false;
}

FORCEINLINE ComponentTypeEnum FromFString(FString s)
{
	if (s.Equals("glasses")) return ComponentTypeEnum::VE_Glasses;
	if (s.Equals("remote_tracker")) return ComponentTypeEnum::VE_RemoteTracker;
	if (s.Equals("natnet")) return ComponentTypeEnum::VE_NatNet;
	if (s.Equals("shimmer")) return ComponentTypeEnum::VE_Shimmer;
	return ComponentTypeEnum::VE_Components;
}

//========================Micro Task Data Structures================================
UENUM(BlueprintType)
enum class MicroTaskEventTypeEnum : uint8
{
	AddTask 			UMETA(DisplayName = "Add Task"),
	RemoveTask 			UMETA(DisplayName = "Remove Task"),
	SubtaskComplete 	UMETA(DisplayName = "Subtask Complete"),
	TaskComplete 		UMETA(DisplayName = "Task Complete"),
	Unknown				UMETA(DisplayName = "Unknwn Event Type")
};

FORCEINLINE MicroTaskEventTypeEnum MicroTaskEventFromFString(FString s)
{
	if (s.Equals("AddTask")) return MicroTaskEventTypeEnum::AddTask;
	if (s.Equals("RemoveTask")) return MicroTaskEventTypeEnum::RemoveTask;
	if (s.Equals("SubtaskComplete")) return MicroTaskEventTypeEnum::SubtaskComplete;
	if (s.Equals("TaskComplete")) return MicroTaskEventTypeEnum::TaskComplete;
	return MicroTaskEventTypeEnum::Unknown;
}

FORCEINLINE FString MicroTaskEventToFString(MicroTaskEventTypeEnum Type)
{
	if (Type == MicroTaskEventTypeEnum::AddTask) return "AddTask";
	if (Type == MicroTaskEventTypeEnum::RemoveTask) return "RemoveTask";
	if (Type == MicroTaskEventTypeEnum::SubtaskComplete) return "SubtaskComplete";
	if (Type == MicroTaskEventTypeEnum::TaskComplete) return "TaskComplete";
	return "Unknown";
}

USTRUCT(BlueprintType)
struct FEyeRadarPointData
{
	GENERATED_USTRUCT_BODY()

	//Normalized UV coordinate on the screen that was fixated at
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eye Radar")
		FVector2D GazePoint;

	//Normalized UV voordinate on the screen that was fixated at
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eye Radar")
		float Duration;

	FEyeRadarPointData()
	{
		Duration = -1;
		GazePoint = FVector2D(-1,-1);
	}

	FEyeRadarPointData(FVector2D _GazePoint, float _Duration)
	{
		Duration = _Duration;
		GazePoint = _GazePoint;
	}
};


USTRUCT(BlueprintType)
struct FSubTaskStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Task Struct")
		FString AOIName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Replay Event Struct")
		bool Complete;

	FSubTaskStruct()
	{
		AOIName = "Not Set";
		Complete = false;
	}
};

USTRUCT(BlueprintType)
struct FActorTaskStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Task Struct")
		FString TaskName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Task Struct")
		FString ActorName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Task Struct")
		int32 TaskID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Task Struct")
		TArray<FSubTaskStruct> SubTaskList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Task Struct")
		FDateTime StartTimestamp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Task Struct")
		FDateTime EndTimestamp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Task Struct")
		bool Complete;

	FActorTaskStruct()
	{
		TaskName = "Not Set";
		ActorName = "Not Set";
		TaskID = -1;
		StartTimestamp = FDateTime::UtcNow();
		EndTimestamp = FDateTime::UtcNow();
		Complete = false;
	}
};

//========================Replay event struct==============================
USTRUCT(BlueprintType)
struct FReplayEventStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Replay Event Struct")
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Replay Event Struct")
		FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Replay Event Struct")
		FString SessionID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Replay Event Struct")
		FTimespan StartTimestamp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Replay Event Struct")
		FTimespan EndTimestamp;

	FReplayEventStruct()
	{
		Name = "";
		Description = "";
		SessionID = "";
		StartTimestamp = FTimespan::Zero();
		EndTimestamp = FTimespan::Zero();
	}
};
FORCEINLINE FArchive& operator<<(FArchive &Ar, FReplayEventStruct &ReplayEvent)
{
	Ar << ReplayEvent.Name;
	Ar << ReplayEvent.Description;
	Ar << ReplayEvent.StartTimestamp;
	Ar << ReplayEvent.EndTimestamp;
	return Ar;
}

USTRUCT(BlueprintType)
struct FMicroTaskEventStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Micro Task Event Struct")
		int32 ClientID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Micro Task Event Struct")
		int32 QuestionID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Micro Task Event Struct")
		int32 EventCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Micro Task Event Struct")
		FString Content;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Micro Task Event Struct")
		TArray<FString> RelevantAOIs;

	FMicroTaskEventStruct() {
		ClientID = 0;
		QuestionID = 0;
		EventCode = 0;
		Content = "";
	}

	FMicroTaskEventStruct(int32 _ClientID, int32 _QuestionID, int32 _EventCode, FString _Content, TArray<FString> _RelevantAOIs) {
		ClientID = _ClientID;
		QuestionID = _QuestionID;
		EventCode = _EventCode;
		Content = _Content;
		RelevantAOIs = _RelevantAOIs;
	}
};

//===================================WAMP======================================
//Data structure for the eye data of the eye tracking glasses
struct FWAMPEyeDataStruct
{
	FVector LeftEyeLocation;
	FVector LeftEyeDirection;
	float LeftPupilRadius;
	float LeftPupilConfidence;

	FVector RightEyeLocation;
	FVector RightEyeDirection;
	float RightPupilRadius;
	float RightPupilConfidence;

	FWAMPEyeDataStruct() {

	}

	FWAMPEyeDataStruct(std::array<double, 16> EyeData) {
		LeftPupilConfidence = EyeData[0];
		LeftPupilRadius = EyeData[1];
		LeftEyeLocation.Set(EyeData[2], EyeData[3], EyeData[4]);
		LeftEyeDirection.Set(EyeData[5], EyeData[6], EyeData[7]);

		RightPupilConfidence = EyeData[8];
		RightPupilRadius = EyeData[9];
		RightEyeLocation.Set(EyeData[10], EyeData[11], EyeData[12]);
		RightEyeDirection.Set(EyeData[13], EyeData[14], EyeData[15]);
	}

	FWAMPEyeDataStruct(std::array<std::array<double, 3>, 2> EyeData) {
		LeftPupilConfidence = 1;
		LeftPupilRadius = 4;
		LeftEyeLocation.Set(10, 0, 0);
		LeftEyeDirection.Set(EyeData[0][0], EyeData[0][1], EyeData[0][2]);

		RightPupilConfidence = 1;
		RightPupilRadius = 4;
		RightEyeLocation.Set(-10, 0, 0);
		RightEyeDirection.Set(EyeData[1][0], EyeData[1][1], EyeData[1][2]);
	}
};

//Data structure for the eye data of the remote tracker
struct FWAMPRetDataStruct
{
	FVector LeftEyeLocation;
	FVector2D LeftEyeScreenCoordinate;
	float LeftPupilRadius;

	FVector RightEyeLocation;
	FVector2D RightEyeScreenCoordinate;
	float RightPupilRadius;

	FVector2D NormalizedGazeLocation;

	FWAMPRetDataStruct() {

	}

	FWAMPRetDataStruct(std::array<double, 14> RetData) {
		LeftPupilRadius = RetData[0]/2.0f; //It is in diameter so convert to radius for consistency
		LeftEyeLocation.Set(RetData[6]/10.0f, RetData[7]/10.0f, RetData[8]/10.0f); //It is in mm, but UE needs it in cm.
		LeftEyeScreenCoordinate.Set(RetData[1], RetData[2]); //In pixels

		RightPupilRadius = RetData[3]/2.0f; //It is in diameter so convert to radius for consistency
		RightEyeLocation.Set(RetData[9]/10.0f, RetData[10]/10.0f, RetData[11]/10.0f); //It is in mm, but UE needs it in cm.
		RightEyeScreenCoordinate.Set(RetData[4], RetData[5]); //In pixels

		NormalizedGazeLocation.Set(RetData[12],RetData[13]);
	}
};

struct FRigidBodyDataStruct
{
	/** Whether the data is valid */
	bool IsValid;

	/** Position of the rigid body */
	FVector Position;

	/** Orientation of the rigid body */
	FQuat Orientation;

	/** Position of the markers*/
	TArray<FVector> Markers;
};

struct FWAMPOpenFaceDataStruct
{
	FWAMPEyeDataStruct GazeData;
	FRigidBodyDataStruct Orientation;

	FWAMPOpenFaceDataStruct(FWAMPEyeDataStruct _GazeData, FRigidBodyDataStruct _Orientation)
	{
		GazeData = _GazeData;
		Orientation = _Orientation;
	}
};

USTRUCT(BlueprintType)
struct FSettingsStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings Struct")
		FVector MotiveOffset;

	/** Glasses origin offset */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings Struct")
		FVector ETOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings Struct")
		FVector LaserToolOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings Struct")
		float PosTrackingUnitToUnrealUnitFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings Struct")
		float EyesTrackingUnitToUnrealUnitFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings Struct")
		bool IsHorizontalCoordinateSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings Struct")
		float SaveRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings Struct")
		int32 SaveScreenRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings Struct")
		bool bSaveRatePerMinute; //or minute

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings Struct")
		FString CrossbarAdress; //or minute

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings Struct")
		FString CrossbarRealm;

	FSettingsStruct() {
		MotiveOffset = FVector(0, 0, 0);
		ETOffset = FVector(0, 0, -2);
		LaserToolOffset = FVector(0, 0, 0);
		PosTrackingUnitToUnrealUnitFactor = 100;
		EyesTrackingUnitToUnrealUnitFactor = 0.1;
		IsHorizontalCoordinateSystem = true;
		SaveScreenRate = 1;
		bSaveRatePerMinute = true; //or minute
		CrossbarAdress = "ws://127.0.0.1:8080/ws";//"ws://127.0.0.1:8080/ws"; //"ws://lupin.ad.ife.no:9080/ws";//"ws://127.0.0.1:8080/ws";
		CrossbarRealm = "realm1";
	}
};
FORCEINLINE FArchive& operator<<(FArchive &Ar, FSettingsStruct &Settings)
{
	Ar << Settings.MotiveOffset;
	Ar << Settings.ETOffset;
	Ar << Settings.LaserToolOffset;
	Ar << Settings.PosTrackingUnitToUnrealUnitFactor;
	Ar << Settings.EyesTrackingUnitToUnrealUnitFactor;
	Ar << Settings.IsHorizontalCoordinateSystem;
	Ar << Settings.SaveRate;
	Ar << Settings.SaveScreenRate;
	Ar << Settings.bSaveRatePerMinute; //or minute
	return Ar;
}

//=========================Replay Sync Struct===================================
USTRUCT(BlueprintType)
struct FRecordingSyncStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recording Sync Struct")
		FString FilePath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recording Sync Struct")
		int32 NmbCols;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recording Sync Struct")
		int32 NmbRows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recording Sync Struct")
		float RecordingSyncTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recording Sync Struct")
		float RecordingDuration;

	FRecordingSyncStruct()
	{
		FilePath = "NotSet";
		NmbCols = -1;
		NmbRows = -1;
		RecordingSyncTime = -1;
		RecordingDuration = -1;
	}
};

USTRUCT(BlueprintType)
struct FSOReplaySyncStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SO Replay Sync Struct")
		FString Filepath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recording Sync Struct")
		float ReplaySyncTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recording Sync Struct")
		float ReplayDuration;


	FSOReplaySyncStruct()
	{
		Filepath = "NotSet";
		ReplaySyncTime = -1;
		ReplayDuration = -1;
	}
};

USTRUCT(BlueprintType)
struct FScreenLabelStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sync Replay Struct")
		FString ScreenName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sync Replay Struct")
		int32 VideoIndex;

	FScreenLabelStruct()
	{
		ScreenName = "NotSet";
		VideoIndex = -1;
	}
};

USTRUCT(BlueprintType)
struct FSyncReplayStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sync Replay Struct")
		FRecordingSyncStruct RecordingSyncData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sync Replay Struct")
		FSOReplaySyncStruct SOReplaySyncData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sync Replay Struct")
		TArray<FScreenLabelStruct> ScreenLabels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sync Replay Struct")
		TArray<FScreenLabelStruct> WebcamLabels;

	FSyncReplayStruct()
	{
	}
};

USTRUCT(BlueprintType)
struct FRenameScreenData
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rename Screen Data Struct")
		FString OldName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rename Screen Data Struct")
		FString NewName;
};

USTRUCT(BlueprintType)
struct FScreenTextureStruct
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture Replay Struct")
		FString ScreenName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture Replay Struct")
		FString ScreenTexture;

	FScreenTextureStruct()
	{
		ScreenName = "NotSet";
		ScreenTexture = "No Texture";
	}
};

USTRUCT(BlueprintType)
struct FSOReplayTextureStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recording Sync Struct")
		FString ReplayFilepath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recording Sync Struct")
		TArray<FScreenTextureStruct> ScreenTextureData;

	FSOReplayTextureStruct()
	{
		ReplayFilepath = "NotSet";
	}
};

USTRUCT(BlueprintType)
struct FSequenceObjectData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence Object Struct")
		FLinearColor LineColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence Object Struct")
		FString ObjectName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence Object Struct")
		TArray<FVector2D> SequenceLines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence Object Struct")
		int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence Object Struct")
		float YPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence Object Struct")
		float ValidDuration;

	FSequenceObjectData() {
		ValidDuration = 0;
	}
};

USTRUCT(BlueprintType)
struct FEyeEventStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence Object Struct")
		FString ActorName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence Object Struct")
		FString TargetAOIName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence Object Struct")
		FVector2D LocationOnAOI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence Object Struct")
		int32 Timestamp;

	FEyeEventStruct() {
		ActorName = "not set";
		TargetAOIName = "not set";
		LocationOnAOI = FVector2D(-1, -1);
		Timestamp = -1;
	}
};