// Fill out your copyright notice in the Description page of Project Settings.

#include "JSONExporter.h"
//#include "Synopticon.h"


UJSONExporter::UJSONExporter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

TSharedPtr<FJsonObject> UJSONExporter::SaveFVectorAsJSON(FVector Vector)
{
	TSharedPtr<FJsonObject> VectorJSON = MakeShareable(new FJsonObject);
	VectorJSON->SetNumberField("X", Vector.X);
	VectorJSON->SetNumberField("Y", Vector.Y);
	VectorJSON->SetNumberField("Z", Vector.Z);

	return VectorJSON;
}

TSharedPtr<FJsonObject> UJSONExporter::SaveFVector2DAsJSON(FVector2D Vector)
{
	TSharedPtr<FJsonObject> Vector2DJSON = MakeShareable(new FJsonObject);
	Vector2DJSON->SetNumberField("X", Vector.X);
	Vector2DJSON->SetNumberField("Y", Vector.Y);

	return Vector2DJSON;
}

template <typename K, typename V>
TSharedPtr<FJsonObject> UJSONExporter::SaveTMapAsJSON(TMap<K, V> Map)
{
	TSharedPtr<FJsonObject> MapJSON = MakeShareable(new FJsonObject);

	//TArray<TSharedPtr<FJsonValue>> MapContent;
	//for (auto& Pair : Map)
	//{
	//	TSharedPtr<FJsonObject> PairObject = MakeShareable(new FJsonObject);
	//	PairObject->SetField("Key", new FJsonValueObject(Pair.Key));
	//	PairObject->SetField("Value", Pair.Value);
	//	MapContent.Add(MakeShareable(new FJsonValueObject(PairObject)));
	//}

	//MapJSON->SetArrayField("TMap", MapContent);

	return MapJSON;
}

FVector UJSONExporter::GetFVectorFromJSON(TSharedPtr<FJsonObject> VectorJson)
{
	FVector Vector;
	Vector.X = VectorJson->GetNumberField("X");
	Vector.Y = VectorJson->GetNumberField("Y");
	Vector.Z = VectorJson->GetNumberField("Z");
	return Vector;
}

FVector2D UJSONExporter::GetFVector2DFromJSON(TSharedPtr<FJsonObject> VectorJson)
{
	FVector2D Vector;
	Vector.X = VectorJson->GetNumberField("X");
	Vector.Y = VectorJson->GetNumberField("Y");
	return Vector;
}

FQuat UJSONExporter::GetFQuatFromJSON(TSharedPtr<FJsonObject> QuatJson) {
	double X = -1;
	double Y = -1;
	double Z = -1;
	double W = -1;

	if (QuatJson->TryGetNumberField("X", X) && QuatJson->TryGetNumberField("Y", Y) 
		&& QuatJson->TryGetNumberField("Z", Z) && QuatJson->TryGetNumberField("W", W)) {
		return FQuat(X, Y, Z, W);
	}
	else {
		return FQuat::Identity;
	}	
}

TSharedPtr<FJsonObject> UJSONExporter::SaveFQuatAsJSON(FQuat Quat) {
	TSharedPtr<FJsonObject> QuatJSON = MakeShareable(new FJsonObject);
	QuatJSON->SetNumberField("X", Quat.X);
	QuatJSON->SetNumberField("Y", Quat.Y);
	QuatJSON->SetNumberField("Z", Quat.Z);
	QuatJSON->SetNumberField("W", Quat.W);

	return QuatJSON;
}