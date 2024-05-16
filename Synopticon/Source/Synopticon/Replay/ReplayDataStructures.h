//header file to save and load binary
#pragma once

#include "CoreMinimal.h"
#include "../Experiment/ExperimentDataStructure.h"
#include "../SystemClasses/SynOpticonCustomType.h"
#include "../GazeObjects/GazeActorDataTypes.h"
#include "../SynOpticonActors/SynOpticonDataStructures.h"

static const int32 gCompressedBufferSize = 600000;
static const int32 gEncodedSceneCameraBufferSize = 200000;

/**
 * 
 */
struct TimeIndexStruct
{
	FDateTime TimeStamp;
	uint32 PlayIndex;
	int32 FileIndex;
	uint8 HasGAKeyFrame; //0: false, 1: true
	int32 LastGAKeyFrameIndex;
	TimeIndexStruct()
	{
		PlayIndex = 0;
		FileIndex = 0;
		HasGAKeyFrame = 0;
		LastGAKeyFrameIndex = 0;
	}
};

FORCEINLINE FArchive& operator<<(FArchive &Ar, TimeIndexStruct &TimeIndex)
{
	Ar << TimeIndex.TimeStamp;
	Ar << TimeIndex.PlayIndex;
	Ar << TimeIndex.FileIndex;
	Ar << TimeIndex.HasGAKeyFrame; //0: false, 1: true
	Ar << TimeIndex.LastGAKeyFrameIndex;

	return Ar;
}

struct CompressedHeader
{
	/* The header is the data that is used to initialize the replay. It includes:
	* Model of the room:
	*	- The number of Gaze Objects
	*	- Information of each Gaze Object
	* Model of SynOpticon Actors:
	*	- The number of SynOpticon Actors
	*	- Information of each SynOpticon Actor
	* Model of Movable Objects:
	*	- The number of Movable Objects:
	*	- Information of each Movable Object
	*/
	//int32 NumOfGazeObjects;
	TArray<FGazeGroupData> GazeGroupList;
	TArray<FGazeActorData> GazeActorDataList;
	//int32 NumOfSynOpticonObjects;
	TArray<FSynOpticonActorStruct> SynOpticonActorDataList;
	//int32 NumOfMovableObjects;
	TArray<FReplayEventStruct> ReplayEvents;
	FDateTime StartTimestamp;
	TArray<FDateTime> LogFileTimestamps;
	TArray<TArray<TimeIndexStruct>> TimeIndex;
	float ReplayTimeInSeconds;
	int32 NumOfLogFiles;

	FSettingsStruct Settings;
	TArray<FAOIData> AOIHistory;
	TArray<FAOIData> PopupHistory;

	//Experiment
	TArray<TPair<FDateTime, FImageAndAOIs>> ChangeImageEvents;
	TArray<TPair<FDateTime, FPopupAndAOIs>> PopupEvents;
	TArray<TPair<MicroTaskEventTypeEnum, FActorTaskStruct>> MicroTaskEvents;
};

FORCEINLINE FArchive& operator<<(FArchive &Ar, CompressedHeader &Header)
{
	Ar << Header.GazeActorDataList;
	Ar << Header.SynOpticonActorDataList;
	Ar << Header.ReplayEvents;
	Ar << Header.StartTimestamp;
	Ar << Header.LogFileTimestamps;
	Ar << Header.TimeIndex;
	Ar << Header.ReplayTimeInSeconds;

	Ar << Header.NumOfLogFiles;

	Ar << Header.Settings;
	return Ar;
}

//A block containing the actors Eye Tracking Glasses component data
struct ETGDataBlock
{
	FVector LeftEyeLocation;
	FRotator LeftEyeRotation;
	FVector RightEyeLocation;
	FRotator RightEyeRotation;
	int32 ETGStructVersion; //the old version didn't record pupil radius
	double PupilRadius;

	ETGDataBlock()
	{
		ETGStructVersion = 1;
		PupilRadius = -2;
	}
};

FORCEINLINE FArchive& operator<<(FArchive &Ar, ETGDataBlock &Block)
{
	Ar << Block.LeftEyeLocation;
	Ar << Block.LeftEyeRotation;
	Ar << Block.RightEyeLocation;
	Ar << Block.RightEyeRotation;


	uint32 currArchiveSize = Ar.Tell();
	Ar << Block.ETGStructVersion;
	if (Block.ETGStructVersion == 1)
	{
		Ar << Block.PupilRadius;
	}
	else
	{
		//	//Rewind archive
		Ar.Seek(currArchiveSize);
	}
	////Check if the read data is invalid
	//if (!(Block.PupilRadius > -1 && Block.PupilRadius < 8)) {

	//	//Init normal
	//	Block.PupilRadius = -2;

	//	//Rewind archive
	//	Ar.Seek(currArchiveSize);
	//}
	return Ar;
}

//A block containing the actors Remote Tracker component data
struct RTDataBlock
{
	int32 RETStructVersion; //New
	FVector LeftEyeLocation;
	FRotator LeftEyeRotation;
	FVector RightEyeLocation;
	FRotator RightEyeRotation;
	FVector GazePoint;
	float RightEyePupilRadius; //New
	float LeftEyePupilRadius; //New

	RTDataBlock()
	{
		RETStructVersion = 1;
		RightEyePupilRadius = -10;
		LeftEyePupilRadius = -10;
	}
};

FORCEINLINE FArchive& operator<<(FArchive &Ar, RTDataBlock &Block)
{
	Ar << Block.LeftEyeLocation;
	Ar << Block.LeftEyeRotation;
	Ar << Block.RightEyeLocation;
	Ar << Block.RightEyeRotation;
	Ar << Block.GazePoint;

	uint32 ArLocation = Ar.Tell();
	Ar << Block.RETStructVersion;
	if (Block.RETStructVersion == 1) {
		Ar << Block.LeftEyePupilRadius;
		Ar << Block.RightEyePupilRadius;
	}
	else {
		Ar.Seek(ArLocation);
	}
	
	return Ar;
}

//A block containing the actors Remote Tracker component data
struct HandDataBlock
{
	FVector HandLocation;
	FQuat HandRotation;
};

FORCEINLINE FArchive& operator<<(FArchive &Ar, HandDataBlock &Block)
{
	Ar << Block.HandLocation;
	Ar << Block.HandRotation;
	return Ar;
}

//A block contain data for a SynOpticon Actor
struct CompressedSOBlock
{
	int32 ActorStructVersion; //Note that this should be set after the SOStruct was loaded

	FVector ActorLocation;
	FRotator ActorRotation;

	bool HasETGData;
	ETGDataBlock ETGData;

	//Scene Camera Data
	int32 BufferSize;
	int32 ImageDataBufferSize;
	uint8* SceneCameraImageBuffer;
	uint8 IsKeyFrame;
	int32 ImageWidth;
	int32 ImageHeight;
	
	bool hasRTData;
	RTDataBlock RTData;
	int32 MyoGesture;

	int32 NmbHandComponents;
	TArray<HandDataBlock*> HandDataBlocks;

	CompressedSOBlock()
	{
		ActorStructVersion = 1;

		HasETGData = false;

		BufferSize = gEncodedSceneCameraBufferSize;
		ImageDataBufferSize = 0;
		SceneCameraImageBuffer = (uint8*)FMemory::Malloc(BufferSize);

		hasRTData = false;

		//NmbHandComponents = 0;
	}
};

FORCEINLINE FArchive& operator<<(FArchive &Ar, CompressedSOBlock &Block)
{
	uint32 ArLocation = Ar.Tell();

	if (Block.ActorStructVersion == 1) {
		Ar << Block.ActorLocation;
		Ar << Block.ActorRotation;

		//This flag is set for the gaze block when we load the header data. If the actor has ETG we save the data every frame.
		if (Block.HasETGData)
		{
			Ar << Block.ETGData;
		}

		Ar << Block.ImageDataBufferSize;
		if (Block.ImageDataBufferSize > 0)
		{
			Ar.Serialize((uint8*)Block.SceneCameraImageBuffer, Block.ImageDataBufferSize);
			Ar << Block.ImageWidth;
			Ar << Block.ImageHeight;
			Ar << Block.IsKeyFrame;
		}

		//This flag is set for the gaze block when we load the header data. If the actor has RET we save the data every frame.
		if (Block.hasRTData)
		{
			Ar << Block.RTData;
		}

		Ar << Block.MyoGesture;

		//Ar << Block.NmbHandComponents;
		for (HandDataBlock* HandData : Block.HandDataBlocks)
		{
			Ar << *HandData;
		}

		//if (Block.NmbHandComponents > 0) {
			//for (HandDataBlock HandData : Block.HandDataBlocks) {
			//	Ar << HandData; //TODO will this work?
			//}		
		//	Ar << Block.HandDataBlocks;
		//}
	}
	else {	
		Ar.Seek(ArLocation);

		Ar << Block.ActorLocation;
		Ar << Block.ActorRotation;
	
		//This flag is set for the gaze block when we load the header data. If the actor has ETG we save the data every frame.
		if (Block.HasETGData)
		{
			Ar << Block.ETGData;
		}

		Ar << Block.ImageDataBufferSize;
		if (Block.ImageDataBufferSize > 0) 
		{
			Ar.Serialize((uint8*)Block.SceneCameraImageBuffer, Block.ImageDataBufferSize);
			Ar << Block.ImageWidth;
			Ar << Block.ImageHeight;
			Ar << Block.IsKeyFrame;
		}

		//This flag is set for the gaze block when we load the header data. If the actor has RET we save the data every frame.
		if (Block.hasRTData)
		{
			Ar << Block.RTData;
		}

		Ar << Block.MyoGesture;
	}
	return Ar;
}

struct CompressedGazeBlock
{
	int32 BufferSize;
	int32 DataSize;
	uint8* Texture;
	int32 ImageWidth;
	int32 ImageHeight;
	uint8 IsKeyframe;
	bool movable;
	bool parented;
	FVector ActorLocation;
	FRotator ActorRotation;

	CompressedGazeBlock()
	{
		BufferSize = gCompressedBufferSize;
		DataSize = 0;
		Texture = (uint8*)FMemory::Malloc(BufferSize);
		ImageWidth = 0;
		ImageHeight = 0;
		movable = false;
		parented = false;
	}
};

FORCEINLINE FArchive& operator<<(FArchive &Ar, CompressedGazeBlock &Block)
{
	Ar << Block.DataSize;

	if (Block.DataSize > 0)
	{
		Ar.Serialize((uint8*)Block.Texture, Block.DataSize);
		Ar << Block.ImageWidth;
		Ar << Block.ImageHeight;
		Ar << Block.IsKeyframe;
	}

	if (Block.movable || Block.parented)
	{
		Ar << Block.ActorLocation;
		Ar << Block.ActorRotation;
	}

	return Ar;
}

struct FCompressedTickBlock 
{
	FDateTime DateTime;
	TArray<CompressedSOBlock*> SOActorData;
	TArray<CompressedGazeBlock*> GazeActorData;

	FCompressedTickBlock()
	{
		DateTime = FDateTime::UtcNow();
	}
};

FORCEINLINE FArchive& operator<<(FArchive &Ar, FCompressedTickBlock &Block)
{
	Block.DateTime.Serialize(Ar);

	for (CompressedSOBlock* SOBlock : Block.SOActorData)
	{
		Ar << *SOBlock;
	}

	for (CompressedGazeBlock* GazeBlock : Block.GazeActorData)
	{
		Ar << *GazeBlock;
	}

	return Ar;
}