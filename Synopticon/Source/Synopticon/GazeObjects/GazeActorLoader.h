#pragma once

#include "CoreMinimal.h"
#include "XmlParser.h"
#include "Serialization/BufferArchive.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/ArchiveLoadCompressedProxy.h"

#include "../SystemClasses/SynOpticonState.h"
#include "../Utils/SynOpticonStatics.h"
#include "GazeObjectMaterialComponent.h"
#include "GazeActor.h"
#include "GazeParentActor.h"
#include "GazeActorJSONExporter.h"

#include "GazeActorDataTypes.h"

using namespace std;

/** For loading rigit bodies information from the xml file */
class SYNOPTICON_API GazeActorLoader
{
private:
	static bool Save(FString FileName, FGazeObjectAndParentData Data);
	static void SaveLoadBinary(FArchive& Ar, FGazeObjectAndParentData &Data);

public:
	/** Loading the rigid bodies from binary file: "Resources/rigidbodies.go" */
	static FGazeObjectAndParentData LoadResourceFromFile(FString filepath);
	static bool ConvertXMLToBinary(FString filepath, TArray<FGazeActorData> &Out);

	GazeActorLoader();
	~GazeActorLoader();

	static bool SaveFile(FString FileName);
};
