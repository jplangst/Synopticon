// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Json.h"

#include "JSONExporter.generated.h"
/**
 * 
 */
UCLASS()
class SYNOPTICON_API UJSONExporter : public UObject
{
	GENERATED_UCLASS_BODY()
public:

	static TSharedPtr<FJsonObject> SaveFVectorAsJSON(FVector Vector);
	static FVector GetFVectorFromJSON(TSharedPtr<FJsonObject> VectorJson);

	static FVector2D GetFVector2DFromJSON(TSharedPtr<FJsonObject> VectorJson);
	static TSharedPtr<FJsonObject> SaveFVector2DAsJSON(FVector2D Vector);

	static FQuat GetFQuatFromJSON(TSharedPtr<FJsonObject> QuatJson);
	static TSharedPtr<FJsonObject> SaveFQuatAsJSON(FQuat Quat);

	template <typename K, typename V> 
	static TSharedPtr<FJsonObject> SaveTMapAsJSON(TMap<K, V> Map);
};
