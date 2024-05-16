// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "OptitrackLiveLink/Public/OptitrackLiveLinkComponent.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeOptitrackLiveLinkComponent() {}

// Begin Cross Module References
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FColor();
ENGINE_API UClass* Z_Construct_UClass_USceneComponent();
OPTITRACKLIVELINK_API UClass* Z_Construct_UClass_UOptitrackLiveLinkComponent();
OPTITRACKLIVELINK_API UClass* Z_Construct_UClass_UOptitrackLiveLinkComponent_NoRegister();
UPackage* Z_Construct_UPackage__Script_OptitrackLiveLink();
// End Cross Module References

// Begin Class UOptitrackLiveLinkComponent
void UOptitrackLiveLinkComponent::StaticRegisterNativesUOptitrackLiveLinkComponent()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UOptitrackLiveLinkComponent);
UClass* Z_Construct_UClass_UOptitrackLiveLinkComponent_NoRegister()
{
	return UOptitrackLiveLinkComponent::StaticClass();
}
struct Z_Construct_UClass_UOptitrackLiveLinkComponent_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Rendering Tags Activation Cooking Physics LOD AssetUserData Collision Navigation Trigger PhysicsVolume" },
		{ "IncludePath", "OptitrackLiveLinkComponent.h" },
		{ "ModuleRelativePath", "Public/OptitrackLiveLinkComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mLineThicknessScale_MetaData[] = {
		{ "Category", "Asset" },
		{ "Comment", "/** Line thickness */" },
		{ "DisplayName", "Line Thickness" },
		{ "ModuleRelativePath", "Public/OptitrackLiveLinkComponent.h" },
		{ "ToolTip", "Line thickness" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mDisplayAsset_MetaData[] = {
		{ "Category", "Asset" },
		{ "Comment", "/** Diplay Asset */" },
		{ "DisplayName", "Display Asset" },
		{ "ModuleRelativePath", "Public/OptitrackLiveLinkComponent.h" },
		{ "ToolTip", "Diplay Asset" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mAssetColor_MetaData[] = {
		{ "Category", "Asset" },
		{ "Comment", "/** Asset color */" },
		{ "DisplayName", "Asset Color" },
		{ "ModuleRelativePath", "Public/OptitrackLiveLinkComponent.h" },
		{ "ToolTip", "Asset color" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mMarkerSize_MetaData[] = {
		{ "Category", "Markers" },
		{ "Comment", "/** Marker Size */" },
		{ "DisplayName", "Marker Size" },
		{ "ModuleRelativePath", "Public/OptitrackLiveLinkComponent.h" },
		{ "ToolTip", "Marker Size" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mDisplayMarkers_MetaData[] = {
		{ "Category", "Markers" },
		{ "Comment", "/** Display Markers */" },
		{ "DisplayName", "Display Markers" },
		{ "ModuleRelativePath", "Public/OptitrackLiveLinkComponent.h" },
		{ "ToolTip", "Display Markers" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mMarkerColor_MetaData[] = {
		{ "Category", "Markers" },
		{ "Comment", "/** Marker color */" },
		{ "DisplayName", "Marker Color" },
		{ "ModuleRelativePath", "Public/OptitrackLiveLinkComponent.h" },
		{ "ToolTip", "Marker color" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_mLineThicknessScale;
	static void NewProp_mDisplayAsset_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_mDisplayAsset;
	static const UECodeGen_Private::FStructPropertyParams NewProp_mAssetColor;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_mMarkerSize;
	static void NewProp_mDisplayMarkers_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_mDisplayMarkers;
	static const UECodeGen_Private::FStructPropertyParams NewProp_mMarkerColor;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOptitrackLiveLinkComponent>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UOptitrackLiveLinkComponent_Statics::NewProp_mLineThicknessScale = { "mLineThicknessScale", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptitrackLiveLinkComponent, mLineThicknessScale), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mLineThicknessScale_MetaData), NewProp_mLineThicknessScale_MetaData) };
void Z_Construct_UClass_UOptitrackLiveLinkComponent_Statics::NewProp_mDisplayAsset_SetBit(void* Obj)
{
	((UOptitrackLiveLinkComponent*)Obj)->mDisplayAsset = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UOptitrackLiveLinkComponent_Statics::NewProp_mDisplayAsset = { "mDisplayAsset", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UOptitrackLiveLinkComponent), &Z_Construct_UClass_UOptitrackLiveLinkComponent_Statics::NewProp_mDisplayAsset_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mDisplayAsset_MetaData), NewProp_mDisplayAsset_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UOptitrackLiveLinkComponent_Statics::NewProp_mAssetColor = { "mAssetColor", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptitrackLiveLinkComponent, mAssetColor), Z_Construct_UScriptStruct_FColor, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mAssetColor_MetaData), NewProp_mAssetColor_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UOptitrackLiveLinkComponent_Statics::NewProp_mMarkerSize = { "mMarkerSize", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptitrackLiveLinkComponent, mMarkerSize), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mMarkerSize_MetaData), NewProp_mMarkerSize_MetaData) };
void Z_Construct_UClass_UOptitrackLiveLinkComponent_Statics::NewProp_mDisplayMarkers_SetBit(void* Obj)
{
	((UOptitrackLiveLinkComponent*)Obj)->mDisplayMarkers = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UOptitrackLiveLinkComponent_Statics::NewProp_mDisplayMarkers = { "mDisplayMarkers", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UOptitrackLiveLinkComponent), &Z_Construct_UClass_UOptitrackLiveLinkComponent_Statics::NewProp_mDisplayMarkers_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mDisplayMarkers_MetaData), NewProp_mDisplayMarkers_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UOptitrackLiveLinkComponent_Statics::NewProp_mMarkerColor = { "mMarkerColor", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptitrackLiveLinkComponent, mMarkerColor), Z_Construct_UScriptStruct_FColor, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mMarkerColor_MetaData), NewProp_mMarkerColor_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UOptitrackLiveLinkComponent_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptitrackLiveLinkComponent_Statics::NewProp_mLineThicknessScale,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptitrackLiveLinkComponent_Statics::NewProp_mDisplayAsset,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptitrackLiveLinkComponent_Statics::NewProp_mAssetColor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptitrackLiveLinkComponent_Statics::NewProp_mMarkerSize,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptitrackLiveLinkComponent_Statics::NewProp_mDisplayMarkers,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptitrackLiveLinkComponent_Statics::NewProp_mMarkerColor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOptitrackLiveLinkComponent_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UOptitrackLiveLinkComponent_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_USceneComponent,
	(UObject* (*)())Z_Construct_UPackage__Script_OptitrackLiveLink,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOptitrackLiveLinkComponent_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOptitrackLiveLinkComponent_Statics::ClassParams = {
	&UOptitrackLiveLinkComponent::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UOptitrackLiveLinkComponent_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UOptitrackLiveLinkComponent_Statics::PropPointers),
	0,
	0x00B000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOptitrackLiveLinkComponent_Statics::Class_MetaDataParams), Z_Construct_UClass_UOptitrackLiveLinkComponent_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UOptitrackLiveLinkComponent()
{
	if (!Z_Registration_Info_UClass_UOptitrackLiveLinkComponent.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOptitrackLiveLinkComponent.OuterSingleton, Z_Construct_UClass_UOptitrackLiveLinkComponent_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOptitrackLiveLinkComponent.OuterSingleton;
}
template<> OPTITRACKLIVELINK_API UClass* StaticClass<UOptitrackLiveLinkComponent>()
{
	return UOptitrackLiveLinkComponent::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UOptitrackLiveLinkComponent);
UOptitrackLiveLinkComponent::~UOptitrackLiveLinkComponent() {}
// End Class UOptitrackLiveLinkComponent

// Begin Registration
struct Z_CompiledInDeferFile_FID_HostProject_Plugins_OptitrackLiveLink_Source_OptitrackLiveLink_Public_OptitrackLiveLinkComponent_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UOptitrackLiveLinkComponent, UOptitrackLiveLinkComponent::StaticClass, TEXT("UOptitrackLiveLinkComponent"), &Z_Registration_Info_UClass_UOptitrackLiveLinkComponent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOptitrackLiveLinkComponent), 2378228474U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HostProject_Plugins_OptitrackLiveLink_Source_OptitrackLiveLink_Public_OptitrackLiveLinkComponent_h_3641358683(TEXT("/Script/OptitrackLiveLink"),
	Z_CompiledInDeferFile_FID_HostProject_Plugins_OptitrackLiveLink_Source_OptitrackLiveLink_Public_OptitrackLiveLinkComponent_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_HostProject_Plugins_OptitrackLiveLink_Source_OptitrackLiveLink_Public_OptitrackLiveLinkComponent_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
