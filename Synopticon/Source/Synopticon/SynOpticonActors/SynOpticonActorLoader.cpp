// Fill out your copyright notice in the Description page of Project Settings.

#include "SynOpticonActorLoader.h"
//#include "Synopticon.h"


SynOpticonActorLoader::SynOpticonActorLoader()
{
}

SynOpticonActorLoader::~SynOpticonActorLoader()
{
}

TArray<FSynOpticonActorStruct> SynOpticonActorLoader::LoadResourceFromFile(FString FileName)
{
	TArray<FSynOpticonActorStruct> Result;
	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *FileName);
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

	TSharedPtr<FJsonObject> SOData = MakeShareable(new FJsonObject);

	if (FJsonSerializer::Deserialize(Reader, SOData))
	{
		TArray <TSharedPtr<FJsonValue>> SOList = SOData->GetArrayField("SynOpticonActorList");
		for (TSharedPtr<FJsonValue> SO : SOList)
		{
			TSharedPtr<FJsonObject> SOObj = SO->AsObject();
			Result.Add(USynOpticonActorJSONExporter::GetSynOpticonActorData(SOObj));
		}
	}
	else { //read data in using binary deserialization
		   ConvertXMLToBinary(FileName);

		   TArray<uint8> CompressedData;
		   if (!FFileHelper::LoadFileToArray(CompressedData, *FileName))
		   {
		   	return Result;
		   }

		   FArchiveLoadCompressedProxy Decompressor =
		   	FArchiveLoadCompressedProxy(CompressedData, ECompressionFlags::COMPRESS_ZLIB);

		   if (Decompressor.GetError())
		   {
		   	return Result;
		   }

		   FBufferArchive DecompressedBinaryArray;
		   Decompressor << DecompressedBinaryArray;

		   FMemoryReader FromBinary = FMemoryReader(DecompressedBinaryArray, true); //true, free data after done
		   FromBinary.Seek(0);
		   SaveLoadBinary(FromBinary, Result);

		   CompressedData.Empty();
		   Decompressor.FlushCache();
		   Decompressor.Close();
		   FromBinary.FlushCache();
		   FromBinary.Close();

		   // Empty & Close Buffer 
		   DecompressedBinaryArray.Empty();
		   DecompressedBinaryArray.Close();
	}

	//SAVE THIS PART FOR LATER WHEN WE WANT TO GO BACK TO SAVING IN BINARY
	

	return Result;
}

void SynOpticonActorLoader::SaveLoadBinary(FArchive& Ar, TArray<FSynOpticonActorStruct>& Data)
{
	Ar << Data;
}

bool SynOpticonActorLoader::Save(FString FileName, TArray<FSynOpticonActorStruct> Data)
{
	TSharedPtr<FJsonObject> SOData = MakeShareable(new FJsonObject);
	TArray< TSharedPtr<FJsonValue> > SOList;
	for (FSynOpticonActorStruct SO : Data)
	{
		SOList.Add(MakeShareable(new FJsonValueObject(USynOpticonActorJSONExporter::SaveSynOpticonActorData(SO))));
	}
	SOData->SetArrayField("SynOpticonActorList", SOList);

	FString OutputString;
	TSharedRef< TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(SOData.ToSharedRef(), Writer);

	return FFileHelper::SaveStringToFile(OutputString, *FileName);

	//SAVING THIS PART
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

bool SynOpticonActorLoader::SaveFile(FString FileName)
{
	TArray<FSynOpticonActorStruct> Data;
	for (AActor* Actor : ASynOpticonState::GetSynOpticonActorList())
	{
		ASynOpticonActor* SOActor = Cast<ASynOpticonActor>(Actor);
		Data.Add(SOActor->GetDataStruct());
	}
	return Save(FileName, Data);
}

FString SynOpticonActorLoader::ConvertXMLToBinary(FString FileName)
{
	TArray<FSynOpticonActorStruct> ResultList;
	FXmlFile doc;

	if (!doc.LoadFile(FileName))
	{
		return "";
	}

	FXmlNode* root = doc.GetRootNode();
	//Failed to load file
	if (root == NULL)
	{
		doc.Clear();
		return "";
	}

	//Loop throught all elements in the xml files
	for (const FXmlNode* elem = root->GetFirstChildNode(); elem != NULL; elem = elem->GetNextNode())
	{
		FString elemName = elem->GetTag();

		//a pointer for storing attribute value
		FString attr;
		if (elemName.Equals("SynOpticonActor"))
		{
			FSynOpticonActorStruct SynOpticonActorData;

			attr = elem->GetAttribute("name");
			if (!attr.IsEmpty())
			{
				SynOpticonActorData.ActorName = attr;
			}

			for (const FXmlNode* SynActorElem = elem->GetFirstChildNode(); SynActorElem != NULL; SynActorElem = SynActorElem->GetNextNode())
			{
				FString SynActorElemName = SynActorElem->GetTag();

				if (SynActorElemName.Equals("ActorComponentsGroup"))
				{

					//Go through the actor component tags
					for (const FXmlNode* ActorComponentElem = SynActorElem->GetFirstChildNode(); ActorComponentElem != NULL; ActorComponentElem = ActorComponentElem->GetNextNode())
					{
						FString ActorComponentElemName = ActorComponentElem->GetTag();
						if (ActorComponentElemName.Equals("NatNet"))
						{
							attr = ActorComponentElem->GetAttribute("attribute");
							if (!attr.IsEmpty())
							{
								SynOpticonActorData.NatNetName = attr;
							}
						}
						else if (ActorComponentElemName.Equals("EyeTrackingVRPNComponent"))
						{
							attr = ActorComponentElem->GetAttribute("GlassesID");
							if (!attr.IsEmpty())
							{
								SynOpticonActorData.EyeTrackerStruct.GlassesID = attr;
							}
						}
						else if (ActorComponentElemName.Equals("ShimmerComponentsGroup"))
						{
							//Go through the Shimmer components tags
							for (const FXmlNode* shimmerElem = ActorComponentElem->GetFirstChildNode(); shimmerElem != NULL; shimmerElem = shimmerElem->GetNextNode())
							{
								FShimmerComponentStruct ShimmerCompStruct;

								attr = shimmerElem->GetAttribute("attribute");
								if (!attr.IsEmpty())
								{
									ShimmerCompStruct.DeviceName = attr;

								}

								attr = shimmerElem->GetAttribute("HasHR");
								if (!attr.IsEmpty())
								{
									ShimmerCompStruct.HasHeartRate = attr.Equals("true") ? true : false;
								}

								attr = shimmerElem->GetAttribute("HasGSR");
								if (!attr.IsEmpty())
								{
									ShimmerCompStruct.HasGalvanicSkinResponse = attr.Equals("true") ? true : false;
								}

								SynOpticonActorData.ShimmerComponents.Add(ShimmerCompStruct);
							}
						}
						else if (ActorComponentElemName.Equals("MyoInputComponent"))
						{
							SynOpticonActorData.HasMyo = true;
						}
					}
				}
			}
			ResultList.Add(SynOpticonActorData);
			//SynOpticonActorsDataMap.Add(SynOpticonActorData.ActorName,SynOpticonActorData);
		}
	}

	FString NewFileName = FileName;
	NewFileName.RemoveFromEnd(".go");
	NewFileName = NewFileName + "_xml2bi_.go";
	Save(FileName, ResultList);
	return FileName;
}