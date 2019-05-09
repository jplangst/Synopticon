// Fill out your copyright notice in the Description page of Project Settings.

#include "GazeActorJSONExporter.h"
//#include "Synopticon.h"


UGazeActorJSONExporter::UGazeActorJSONExporter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

FString UGazeActorJSONExporter::SaveGazeActorTypeAsString(EStaticRoomActorType GazeActorType)
{
	switch (GazeActorType)
	{
	case EStaticRoomActorType::SRT_CurvedScreen: return "Curved Screen";
	case EStaticRoomActorType::SRT_ComplexObject: return "Complex Object";
	case EStaticRoomActorType::SRT_ActionObject: return "Action Object";
	default: return "Screen";
	}
}

EStaticRoomActorType UGazeActorJSONExporter::GetGazeActorTypeFromString(FString GazeActorTypeString)
{
	if(GazeActorTypeString == "Curved Screen")
		return EStaticRoomActorType::SRT_CurvedScreen;
	if (GazeActorTypeString == "Complex Object")
		return EStaticRoomActorType::SRT_ComplexObject;
	if (GazeActorTypeString == "Action Object")
		return EStaticRoomActorType::SRT_ActionObject;
	return EStaticRoomActorType::SRT_Screen;
}

TSharedPtr<FJsonObject> UGazeActorJSONExporter::SaveGazeActorData(FGazeActorData GazeActorData)
{
	TSharedPtr<FJsonObject> GazeActorJson = MakeShareable(new FJsonObject);
	GazeActorJson->SetNumberField("ID", GazeActorData.ID);
	GazeActorJson->SetStringField("GazeActorType", SaveGazeActorTypeAsString(GazeActorData.GazeActorType));
	GazeActorJson->SetBoolField("IsGazeObject", GazeActorData.IsGazeObject);
	GazeActorJson->SetBoolField("IsCollisionObject", GazeActorData.IsCollisionObject);
	GazeActorJson->SetStringField("Name", GazeActorData.Name);

	TArray<TSharedPtr<FJsonValue>> Markers;
	for (FVector M : GazeActorData.Markers)
	{
		Markers.Add(MakeShareable(new FJsonValueObject(UJSONExporter::SaveFVectorAsJSON(M))));
	}
	GazeActorJson->SetArrayField("Markers", Markers);

	TArray<TSharedPtr<FJsonValue>> Vertices;
	for (FVector V : GazeActorData.Vertices)
	{
		Vertices.Add(MakeShareable(new FJsonValueObject(UJSONExporter::SaveFVectorAsJSON(V))));
	}
	GazeActorJson->SetArrayField("Vertices", Vertices);

	TArray<TSharedPtr<FJsonValue>> UVs;
	for (FVector2D uv : GazeActorData.UVs)
	{
		UVs.Add(MakeShareable(new FJsonValueObject(UJSONExporter::SaveFVector2DAsJSON(uv))));
	}
	GazeActorJson->SetArrayField("UVs", UVs);


	TArray<TSharedPtr<FJsonValue>> Indexes;
	for (int32 i : GazeActorData.Indexes)
	{
		Indexes.Add(MakeShareable(new FJsonValueNumber(i)));
	}
	GazeActorJson->SetArrayField("Indexes", Indexes);

	TArray<TSharedPtr<FJsonValue>> AOIS;
	for (FAOIData i : GazeActorData.AOIS)
	{
		AOIS.Add(MakeShareable(new FJsonValueObject(UGazeActorJSONExporter::SaveAOIData(i))));
	}
	GazeActorJson->SetArrayField("AOIS", AOIS);

	GazeActorJson->SetStringField("URL", GazeActorData.URL);
	GazeActorJson->SetBoolField("Parented", GazeActorData.Parented);
	GazeActorJson->SetStringField("ParentSceneCompName", GazeActorData.ParentSceneCompName);
	GazeActorJson->SetBoolField("Movable", GazeActorData.Movable);
	GazeActorJson->SetStringField("NatNetName", GazeActorData.NatNetName);
	GazeActorJson->SetObjectField("SizeDimensions", UJSONExporter::SaveFVectorAsJSON(GazeActorData.SizeDimensions));
	GazeActorJson->SetNumberField("FontSize", GazeActorData.FontSize);
	GazeActorJson->SetNumberField("VideoIndex", GazeActorData.VideoIndex);
	GazeActorJson->SetStringField("TextureFilepath", GazeActorData.TextureFilepath);
	GazeActorJson->SetNumberField("Grouped", GazeActorData.Grouped);
	GazeActorJson->SetNumberField("CalibrationVerticeIndex", GazeActorData.CalibrationVerticeIndex);
	GazeActorJson->SetStringField("ButtonMesh", GazeActorData.ButtonMesh);
	GazeActorJson->SetStringField("SplineMesh", GazeActorData.SplineMesh);
	GazeActorJson->SetStringField("WAMPScreenID", GazeActorData.WAMPScreenID);

	return GazeActorJson;
}

FGazeActorData UGazeActorJSONExporter::GetGazeActorData(TSharedPtr<FJsonObject> GazeActorObj)
{
	FGazeActorData GazeActorData;

	GazeActorObj->TryGetNumberField("ID", GazeActorData.ID);
	FString GazeActorType;
	GazeActorObj->TryGetStringField("GazeActorType", GazeActorType);
	GazeActorData.GazeActorType = GetGazeActorTypeFromString(GazeActorType);
	GazeActorObj->TryGetBoolField("IsGazeObject", GazeActorData.IsGazeObject);
	GazeActorObj->TryGetBoolField("IsCollisionObject", GazeActorData.IsCollisionObject);
	GazeActorObj->TryGetStringField("Name", GazeActorData.Name);
	GazeActorObj->TryGetStringField("ButtonMesh", GazeActorData.ButtonMesh);
	GazeActorObj->TryGetStringField("SplineMesh", GazeActorData.SplineMesh);

	TArray< TSharedPtr<FJsonValue> > Markers = GazeActorObj->GetArrayField("Markers");
	for (TSharedPtr<FJsonValue> Marker : Markers) {
		TSharedPtr<FJsonObject> MarkerObj = Marker->AsObject();
		GazeActorData.Markers.Add(UJSONExporter::GetFVectorFromJSON(MarkerObj));
	}

	TArray< TSharedPtr<FJsonValue> > Vertices = GazeActorObj->GetArrayField("Vertices");
	for (TSharedPtr<FJsonValue> Vertice : Vertices) {
		TSharedPtr<FJsonObject> VerticeObj = Vertice->AsObject();
		GazeActorData.Vertices.Add(UJSONExporter::GetFVectorFromJSON(VerticeObj));
	}

	TArray< TSharedPtr<FJsonValue> > UVs = GazeActorObj->GetArrayField("UVs");
	for (TSharedPtr<FJsonValue> UV : UVs) {
		TSharedPtr<FJsonObject> UVObj = UV->AsObject();
		GazeActorData.UVs.Add(UJSONExporter::GetFVector2DFromJSON(UVObj));
	}

	TArray< TSharedPtr<FJsonValue> > Indexes = GazeActorObj->GetArrayField("Indexes");
	for (TSharedPtr<FJsonValue> Index : Indexes) {
		GazeActorData.Indexes.Add(Index->AsNumber());
	}

	TArray< TSharedPtr<FJsonValue> > AOIS = GazeActorObj->GetArrayField("AOIS");
	for (TSharedPtr<FJsonValue> AOI : AOIS) {
		GazeActorData.AOIS.Add(UGazeActorJSONExporter::GetAOIData(AOI->AsObject()));
	}

	GazeActorObj->TryGetStringField("URL", GazeActorData.URL);
	GazeActorObj->TryGetBoolField("Parented", GazeActorData.Parented);
	GazeActorObj->TryGetStringField("ParentSceneCompName", GazeActorData.ParentSceneCompName);
	GazeActorObj->TryGetBoolField("Movable", GazeActorData.Movable);
	GazeActorObj->TryGetStringField("NatNetName", GazeActorData.NatNetName);
	TSharedPtr<FJsonObject> SizeDimensionsObj = GazeActorObj->GetObjectField("SizeDimensions");
	GazeActorData.SizeDimensions = UJSONExporter::GetFVectorFromJSON(SizeDimensionsObj);
	GazeActorData.FontSize = GazeActorObj->GetIntegerField("FontSize");
	GazeActorData.VideoIndex = GazeActorObj->GetIntegerField("VideoIndex");
	GazeActorObj->TryGetStringField("TextureFilepath", GazeActorData.TextureFilepath);
	GazeActorObj->TryGetNumberField("Grouped", GazeActorData.Grouped);
	GazeActorObj->TryGetNumberField("CalibrationVerticeIndex", GazeActorData.CalibrationVerticeIndex);
	GazeActorObj->TryGetStringField("WAMPScreenID", GazeActorData.WAMPScreenID);
	return GazeActorData;
}

TSharedPtr<FJsonObject> UGazeActorJSONExporter::SaveGazeObjectAndParentData(FGazeObjectAndParentData GazeObjectAndParentData)
{
	TSharedPtr<FJsonObject> GazeObjectAndParentDataJson = MakeShareable(new FJsonObject);

	TArray< TSharedPtr<FJsonValue> > GazeParentActorsData;
	for (FGazeParentActorData GazeParentActorData : GazeObjectAndParentData.GazeParentActorsData) 
	{
		GazeParentActorsData.Add(MakeShareable(new FJsonValueObject(SaveGazeParentActorData(GazeParentActorData))));
	}
	GazeObjectAndParentDataJson->SetArrayField("GazeParentActorsData", GazeParentActorsData);

	TArray< TSharedPtr<FJsonValue> > GazeActorsData;
	for (FGazeActorData GazeActorData : GazeObjectAndParentData.GazeActorsData)
	{
		GazeActorsData.Add(MakeShareable(new FJsonValueObject(SaveGazeActorData(GazeActorData))));
	}
	GazeObjectAndParentDataJson->SetArrayField("GazeActorsData", GazeActorsData);

	TArray< TSharedPtr<FJsonValue> > GazeGroupData;
	for (FGazeGroupData GazeGroup : GazeObjectAndParentData.GazeGroupData)
	{
		GazeGroupData.Add(MakeShareable(new FJsonValueObject(SaveGazeGroupData(GazeGroup))));
	}
	GazeObjectAndParentDataJson->SetArrayField("GazeGroupData", GazeGroupData);

	return GazeObjectAndParentDataJson;
}

FGazeObjectAndParentData UGazeActorJSONExporter::GetGazeObjectAndParentData(TSharedPtr<FJsonObject> GazeObjectAndParentDataObj)
{
	FGazeObjectAndParentData GazeObjectAndParentData;

	TArray<TSharedPtr<FJsonValue>> GazeParentActorsData = GazeObjectAndParentDataObj->GetArrayField("GazeParentActorsData");
	for (TSharedPtr<FJsonValue> GazeParentActor : GazeParentActorsData)
	{
		TSharedPtr<FJsonObject> GazeParentObj = GazeParentActor->AsObject();
		GazeObjectAndParentData.GazeParentActorsData.Add(GetGazeParentActorData(GazeParentObj));
	}

	TArray<TSharedPtr<FJsonValue>> GazeActorsData = GazeObjectAndParentDataObj->GetArrayField("GazeActorsData");
	for (TSharedPtr<FJsonValue> GazeActorData : GazeActorsData)
	{
		TSharedPtr<FJsonObject> GazeActorObj = GazeActorData->AsObject();
		GazeObjectAndParentData.GazeActorsData.Add(GetGazeActorData(GazeActorObj));
	}

	TArray<TSharedPtr<FJsonValue>> GazeGroupData = GazeObjectAndParentDataObj->GetArrayField("GazeGroupData");
	for (TSharedPtr<FJsonValue> GazeGroup : GazeGroupData)
	{
		TSharedPtr<FJsonObject> GazeGroupObj = GazeGroup->AsObject();
		GazeObjectAndParentData.GazeGroupData.Add(GetGazeGroupData(GazeGroupObj));
	}

	return GazeObjectAndParentData;
}

TSharedPtr<FJsonObject> UGazeActorJSONExporter::SaveGazeParentActorData(FGazeParentActorData GazeParentActorData)
{
	TSharedPtr<FJsonObject> GazeParentActorDataJson = MakeShareable(new FJsonObject);
	GazeParentActorDataJson->SetStringField("SceneCompName", GazeParentActorData.SceneCompName);
	GazeParentActorDataJson->SetStringField("NatNetName", GazeParentActorData.NatNetName);
	GazeParentActorDataJson->SetObjectField("InitialLocation", UJSONExporter::SaveFVectorAsJSON(GazeParentActorData.InitialLocation));
	GazeParentActorDataJson->SetObjectField("CalibrationQuat", UJSONExporter::SaveFQuatAsJSON(GazeParentActorData.CalibrationQuat));

	return GazeParentActorDataJson;
}

FGazeParentActorData UGazeActorJSONExporter::GetGazeParentActorData(TSharedPtr<FJsonObject> GazeParentActorDataObj)
{	
	FGazeParentActorData GazeParentActorData;
	GazeParentActorDataObj->TryGetStringField("SceneCompName", GazeParentActorData.SceneCompName);
	GazeParentActorDataObj->TryGetStringField("NatNetName", GazeParentActorData.NatNetName);
	GazeParentActorData.InitialLocation = UJSONExporter::GetFVectorFromJSON(GazeParentActorDataObj->GetObjectField("InitialLocation"));
	GazeParentActorData.CalibrationQuat = UJSONExporter::GetFQuatFromJSON(GazeParentActorDataObj->GetObjectField("CalibrationQuat"));

	return GazeParentActorData;
}

TSharedPtr<FJsonObject> UGazeActorJSONExporter::SaveGazeGroupData(FGazeGroupData GazeGroupData)
{
	TSharedPtr<FJsonObject> GazeGroupDataJson = MakeShareable(new FJsonObject);

	GazeGroupDataJson->SetStringField("GroupName", GazeGroupData.GroupName);
	GazeGroupDataJson->SetNumberField("GroupID", GazeGroupData.GroupID);

	return GazeGroupDataJson;
}

FGazeGroupData UGazeActorJSONExporter::GetGazeGroupData(TSharedPtr<FJsonObject> GazeGroupDataObj)
{
	FGazeGroupData GazeGroupData;

	GazeGroupDataObj->TryGetStringField("GroupName", GazeGroupData.GroupName);
	GazeGroupDataObj->TryGetNumberField("GroupID", GazeGroupData.GroupID);

	return GazeGroupData;
}

TSharedPtr<FJsonObject> UGazeActorJSONExporter::SaveAOIData(FAOIData AOIData) {
	TSharedPtr<FJsonObject> AOIDataJson = MakeShareable(new FJsonObject);

	AOIDataJson->SetStringField("AOIName", AOIData.Name);
	for (FVector2D Coordinate : AOIData.Coordinates) {

	}

	TArray<TSharedPtr<FJsonValue>> Coordinates;
	for (FVector2D coor : AOIData.Coordinates)
	{
		Coordinates.Add(MakeShareable(new FJsonValueObject(UJSONExporter::SaveFVector2DAsJSON(coor))));
	}
	AOIDataJson->SetArrayField("Coordinates", Coordinates);

	AOIDataJson->SetNumberField("AOIID", AOIData.ID);

	return AOIDataJson;
}

FAOIData UGazeActorJSONExporter::GetAOIData(TSharedPtr<FJsonObject> AOIDataObj) {
	FAOIData AOIData;

	AOIDataObj->TryGetStringField("AOIName", AOIData.Name);
	AOIDataObj->TryGetNumberField("AOIID", AOIData.ID);
	TArray< TSharedPtr<FJsonValue> > Coordinates = AOIDataObj->GetArrayField("Coordinates");
	for (TSharedPtr<FJsonValue> coor : Coordinates) {
		TSharedPtr<FJsonObject> coorObj = coor->AsObject();
		AOIData.Coordinates.Add(UJSONExporter::GetFVector2DFromJSON(coorObj));
	}
	return AOIData;
}