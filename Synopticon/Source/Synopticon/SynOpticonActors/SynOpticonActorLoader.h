// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XmlFile.h"
#include "XmlNode.h"
#include "Serialization/ArchiveLoadCompressedProxy.h"

#include "../SystemClasses/SynOpticonState.h"
//Actor Components
#include "../ActorComponents/PositionAndOrientationComponent.h"
#include "../ActorComponents/EyeVectorsComponent.h"
#include "../ActorComponents/ShimmerVRPNComponent.h"
//#include "../ActorComponents/MyoInputComponent.h"
#include "SynOpticonActor.h"
#include "SynOpticonActorJSONExporter.h"

/**
 * 
 */
class SYNOPTICON_API SynOpticonActorLoader
{
private:
	static bool Save(FString FileName, TArray<FSynOpticonActorStruct> Data);
	static void SaveLoadBinary(FArchive& Ar, TArray<FSynOpticonActorStruct>& Data);
	
public:
	/** Loading the SynOpticon Actors Data from file: "Resources/SynOpticonActors.xml" */
	static TArray<FSynOpticonActorStruct> LoadResourceFromFile(FString fileName);
	static FString ConvertXMLToBinary(FString FileName);

	SynOpticonActorLoader();
	~SynOpticonActorLoader();

	static bool SaveFile(FString FileName);
};