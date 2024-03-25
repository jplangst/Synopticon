// Fill out your copyright notice in the Description page of Project Settings.

#include "GazeActorLoader.h"
//#include "Synopticon.h"


GazeActorLoader::GazeActorLoader()
{
}

GazeActorLoader::~GazeActorLoader()
{
}

FGazeObjectAndParentData GazeActorLoader::LoadResourceFromFile(FString FileName)
{
	FGazeObjectAndParentData Result;
	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *FileName);
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

	TSharedPtr<FJsonObject> GAData = MakeShareable(new FJsonObject);

	if (FJsonSerializer::Deserialize(Reader, GAData))
	{
		Result = UGazeActorJSONExporter::GetGazeObjectAndParentData(GAData->GetObjectField("GazeObjectAndParentData"));
	}
	else { //read data in using binary deserialization
		   TArray<uint8> CompressedData;
		   if (!FFileHelper::LoadFileToArray(CompressedData, *FileName))
		   {
		   	return Result;
		   }

		   FArchiveLoadCompressedProxy Decompressor =
		   	FArchiveLoadCompressedProxy(CompressedData, "DXT", ECompressionFlags::COMPRESS_ZLIB); //Added compression format, do not know if the string is valid

		   if (Decompressor.GetError())
		   {
		   	return Result;
		   }

		   FBufferArchive DecompressedBinaryArray;
		   Decompressor << DecompressedBinaryArray;

		   FMemoryReader FromBinary = FMemoryReader(DecompressedBinaryArray, true); //true, free data after done
		   FromBinary.Seek(0);
		   try
		   {
		   	SaveLoadBinary(FromBinary, Result);
		   }
		   catch (const std::exception& e)
		   {
		   	e;
		   	return Result;
		   }

		   CompressedData.Empty();
		   Decompressor.FlushCache();
		   Decompressor.Close();
		   FromBinary.FlushCache();
		   FromBinary.Close();

		   // Empty & Close Buffer 
		   DecompressedBinaryArray.Empty();
		   DecompressedBinaryArray.Close();
	}

	return Result;
}

void GazeActorLoader::SaveLoadBinary(FArchive& Ar, FGazeObjectAndParentData& Data)
{
	Ar << Data;
}

bool GazeActorLoader::Save(FString FileName, FGazeObjectAndParentData Data)
{
	if (!FileName.EndsWith(".go"))
		FileName.Append(".go");

	TSharedPtr<FJsonObject> GAData = MakeShareable(new FJsonObject);
	GAData->SetObjectField("GazeObjectAndParentData", UGazeActorJSONExporter::SaveGazeObjectAndParentData(Data));

	FString OutputString;
	TSharedRef< TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(GAData.ToSharedRef(), Writer);

	return FFileHelper::SaveStringToFile(OutputString, *FileName);

	/*FBufferArchive ToBinary;
	SaveLoadBinary(ToBinary, Data);

	TArray<uint8> CompressedData;
	FArchiveSaveCompressedProxy Compressor =
		FArchiveSaveCompressedProxy(CompressedData, ECompressionFlags::COMPRESS_ZLIB);

	Compressor << ToBinary;
	Compressor.Flush();

	bool _IsSaved = FFileHelper::SaveArrayToFile(CompressedData, *FileName);

	Compressor.FlushCache();
	CompressedData.Empty();

	ToBinary.FlushCache();
	ToBinary.Empty();

	ToBinary.Close();

	return _IsSaved;*/
}

bool GazeActorLoader::SaveFile(FString FileName)
{
	TArray<FGazeParentActorData> GazeParentsData;
	for (AActor* Parent : ASynOpticonState::GetGazeParentActorList()) {
		AGazeParentActor* GazeParent = Cast<AGazeParentActor>(Parent);
		GazeParentsData.Add(FGazeParentActorData(GazeParent->GetSceneCompName(), GazeParent->GetNatNetName(), GazeParent->GetInitialLocation()));
	}

	FSOReplayTextureStruct ReplayTextureData;
	ReplayTextureData.ReplayFilepath = FileName;
	bool HasTextureData = false;
	TArray<FGazeActorData> GazeObjectsData;
	for (AActor* GA : ASynOpticonState::GetGazeActorList())
	{
		AGazeActor* Actor = Cast<AGazeActor>(GA);
		GazeObjectsData.Add(Actor->GetDataStruct());
	}

	FGazeObjectAndParentData AllData = FGazeObjectAndParentData(GazeParentsData, GazeObjectsData, ASynOpticonState::GetGazeGroupData());

	bool Result = Save(FileName, AllData);
	

	return Result;
}

bool GazeActorLoader::ConvertXMLToBinary(FString filepath, TArray<FGazeActorData> &RigidBodies)
{
	FXmlFile doc;

	if (!doc.LoadFile(filepath))
	{		
		return false;
	}
	FXmlNode* root = doc.GetRootNode();

	//Failed to load file
	if (root == NULL)
	{
		doc.Clear();
		return false;
	}

	//Loop throught all elements in the xml files
	for (const FXmlNode* elem = root->GetFirstChildNode(); elem != NULL; elem = elem->GetNextNode())
	{
		FString elemName = elem->GetTag();

		//a pointer for storing attribute value
		FString attr;
		if (elemName.Equals("RigidBody"))
		{
			FGazeActorData rigidBodyData;
			for (const FXmlNode* rElem = elem->GetFirstChildNode(); rElem != NULL; rElem = rElem->GetNextNode())
			{
				FString rElemName = rElem->GetTag();
				if (rElemName.Equals("Name"))
				{
					attr = rElem->GetContent();
					if (!attr.IsEmpty())
						rigidBodyData.Name = attr;
				}
				else if (rElemName.Equals("NatNetName"))
				{
					attr = rElem->GetContent();
					if (!attr.IsEmpty())
						rigidBodyData.NatNetName = attr;

					rigidBodyData.Movable = true;
				}
				else if (rElemName == "Markers")
				{
					for (const FXmlNode* markerElem = rElem->GetFirstChildNode(); markerElem != NULL; markerElem = markerElem->GetNextNode())
					{
						FVector markerPosition;
						attr = markerElem->GetAttribute("x");
						if (!attr.IsEmpty())
							markerPosition.X = USynOpticonStatics::ConvertFStringToFloat(attr);
						attr = markerElem->GetAttribute("y");
						if (!attr.IsEmpty())
							markerPosition.Y = USynOpticonStatics::ConvertFStringToFloat(attr);
						attr = markerElem->GetAttribute("z");
						if (!attr.IsEmpty())
							markerPosition.Z = USynOpticonStatics::ConvertFStringToFloat(attr);
						/*markerPosition = USynOpticonStatics::ConvertFromMotiveToUnrealEngineCoordinateSystem(markerPosition,
						SettingsManager::GetInstance()->GetTrackerOffset(),
						SettingsManager::GetInstance()->GetPosTrackingUnitToOgreUnitFactor());*/
						rigidBodyData.Markers.Add(markerPosition);
					}
				}
				else if (rElemName == "Dimensions")
				{
					attr = rElem->GetAttribute("Width");
					if (!attr.IsEmpty())
						rigidBodyData.SizeDimensions.X = USynOpticonStatics::ConvertFStringToFloat(attr);
					attr = rElem->GetAttribute("Length");
					if (!attr.IsEmpty())
						rigidBodyData.SizeDimensions.Y = USynOpticonStatics::ConvertFStringToFloat(attr);
					attr = rElem->GetAttribute("Height");
					if (!attr.IsEmpty())
						rigidBodyData.SizeDimensions.Z = USynOpticonStatics::ConvertFStringToFloat(attr);
				}
				else if (rElemName == "URL")
				{
					attr = rElem->GetContent();
					if (!attr.IsEmpty())
						rigidBodyData.URL = attr;
					else
						rigidBodyData.URL = "";
				}
			}
			RigidBodies.Add(rigidBodyData);
		}
	}

	FString FileName = filepath;
	FileName.RemoveFromEnd(".go");
	FileName = FileName + "_xml2bi_.go";
	//Save(filepath, RigidBodies);
	return false; //Was true
}