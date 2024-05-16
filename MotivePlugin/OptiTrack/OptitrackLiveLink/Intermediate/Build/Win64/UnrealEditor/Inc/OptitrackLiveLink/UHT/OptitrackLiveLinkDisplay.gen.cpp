// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "OptitrackLiveLink/Public/OptitrackLiveLinkDisplay.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeOptitrackLiveLinkDisplay() {}

// Begin Cross Module References
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FColor();
ENGINE_API UClass* Z_Construct_UClass_AActor();
ENGINE_API UClass* Z_Construct_UClass_USceneComponent_NoRegister();
OPTITRACKLIVELINK_API UClass* Z_Construct_UClass_AOptitrackLiveLinkDisplay();
OPTITRACKLIVELINK_API UClass* Z_Construct_UClass_AOptitrackLiveLinkDisplay_NoRegister();
UPackage* Z_Construct_UPackage__Script_OptitrackLiveLink();
// End Cross Module References

// Begin Class AOptitrackLiveLinkDisplay
void AOptitrackLiveLinkDisplay::StaticRegisterNativesAOptitrackLiveLinkDisplay()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AOptitrackLiveLinkDisplay);
UClass* Z_Construct_UClass_AOptitrackLiveLinkDisplay_NoRegister()
{
	return AOptitrackLiveLinkDisplay::StaticClass();
}
struct Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "Comment", "// All non-relevant categories are hidden in the editor to prevent the user from thinking they would work\n" },
		{ "HideCategories", "Rendering Replication Collision HLOD Physics Networking WorldPartition Input Actor Cooking DataLayers LevelInstance" },
		{ "IncludePath", "OptitrackLiveLinkDisplay.h" },
		{ "ModuleRelativePath", "Public/OptitrackLiveLinkDisplay.h" },
		{ "ToolTip", "All non-relevant categories are hidden in the editor to prevent the user from thinking they would work" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Root_MetaData[] = {
		{ "Comment", "/** Root Component to allow easy transform */" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/OptitrackLiveLinkDisplay.h" },
		{ "ToolTip", "Root Component to allow easy transform" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mLineThicknessScale_MetaData[] = {
		{ "Category", "Assets" },
		{ "Comment", "/** Line thickness */" },
		{ "DisplayName", "Line Thickness" },
		{ "ModuleRelativePath", "Public/OptitrackLiveLinkDisplay.h" },
		{ "ToolTip", "Line thickness" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mDrawAssets_MetaData[] = {
		{ "Category", "Assets" },
		{ "Comment", "/** Display all assets */" },
		{ "DisplayName", "Display Assets" },
		{ "ModuleRelativePath", "Public/OptitrackLiveLinkDisplay.h" },
		{ "ToolTip", "Display all assets" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mAssetColor_MetaData[] = {
		{ "Category", "Assets" },
		{ "Comment", "/** All Assets Color */" },
		{ "DisplayName", "Asset Color" },
		{ "ModuleRelativePath", "Public/OptitrackLiveLinkDisplay.h" },
		{ "ToolTip", "All Assets Color" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mSkeletonLineThickness_MetaData[] = {
		{ "Category", "Skeletons" },
		{ "Comment", "/** Skeleton Line Thickness */" },
		{ "DisplayName", "Skeleton Line Thickness" },
		{ "ModuleRelativePath", "Public/OptitrackLiveLinkDisplay.h" },
		{ "ToolTip", "Skeleton Line Thickness" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mDrawSkeletons_MetaData[] = {
		{ "Category", "Skeletons" },
		{ "Comment", "/** Display streamed skeleton data visuals */" },
		{ "DisplayName", "Display Skeleton Data" },
		{ "ModuleRelativePath", "Public/OptitrackLiveLinkDisplay.h" },
		{ "ToolTip", "Display streamed skeleton data visuals" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mSkeletonColor_MetaData[] = {
		{ "Category", "Skeletons" },
		{ "Comment", "/** Skeleton color */" },
		{ "DisplayName", "Skeleton Color" },
		{ "ModuleRelativePath", "Public/OptitrackLiveLinkDisplay.h" },
		{ "ToolTip", "Skeleton color" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mRigidBodyLineThickness_MetaData[] = {
		{ "Category", "Rigid Bodies" },
		{ "Comment", "/** Rigid body Line Thickness */" },
		{ "DisplayName", "Rigid Body Line Thickness" },
		{ "ModuleRelativePath", "Public/OptitrackLiveLinkDisplay.h" },
		{ "ToolTip", "Rigid body Line Thickness" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mDrawRigidBodies_MetaData[] = {
		{ "Category", "Rigid Bodies" },
		{ "Comment", "/** Display streamed rigid body data visuals */" },
		{ "DisplayName", "Display Rigid Body Data" },
		{ "ModuleRelativePath", "Public/OptitrackLiveLinkDisplay.h" },
		{ "ToolTip", "Display streamed rigid body data visuals" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mRigidBodyColor_MetaData[] = {
		{ "Category", "Rigid Bodies" },
		{ "Comment", "/** Rigid body color */" },
		{ "DisplayName", "Rigid Body Color" },
		{ "ModuleRelativePath", "Public/OptitrackLiveLinkDisplay.h" },
		{ "ToolTip", "Rigid body color" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mTMarkersetLineThickness_MetaData[] = {
		{ "Category", "Trained Markersets" },
		{ "Comment", "/** Trained Markerset Line Thickness */" },
		{ "DisplayName", "Trained Markerset Line Thickness" },
		{ "ModuleRelativePath", "Public/OptitrackLiveLinkDisplay.h" },
		{ "ToolTip", "Trained Markerset Line Thickness" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mDrawTMarkersets_MetaData[] = {
		{ "Category", "Trained Markersets" },
		{ "Comment", "/** Display streamed trained markerset visuals */" },
		{ "DisplayName", "Display Trained Markersets" },
		{ "ModuleRelativePath", "Public/OptitrackLiveLinkDisplay.h" },
		{ "ToolTip", "Display streamed trained markerset visuals" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mTMarkersetColor_MetaData[] = {
		{ "Category", "Trained Markersets" },
		{ "Comment", "/** Trained Markerset color */" },
		{ "DisplayName", "Trained Markerset Color" },
		{ "ModuleRelativePath", "Public/OptitrackLiveLinkDisplay.h" },
		{ "ToolTip", "Trained Markerset color" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mLabeledMarkerSize_MetaData[] = {
		{ "Category", "Labeled Markers" },
		{ "Comment", "/** Marker Size */" },
		{ "DisplayName", "Labeled Marker Size" },
		{ "ModuleRelativePath", "Public/OptitrackLiveLinkDisplay.h" },
		{ "ToolTip", "Marker Size" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mDrawLabeledMarkers_MetaData[] = {
		{ "Category", "Labeled Markers" },
		{ "Comment", "/** Display Markers */" },
		{ "DisplayName", "Display Labeled Marker Data" },
		{ "ModuleRelativePath", "Public/OptitrackLiveLinkDisplay.h" },
		{ "ToolTip", "Display Markers" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mLabeledMarkerColor_MetaData[] = {
		{ "Category", "Labeled Markers" },
		{ "Comment", "/** Marker color */" },
		{ "DisplayName", "Labeled Marker Color" },
		{ "ModuleRelativePath", "Public/OptitrackLiveLinkDisplay.h" },
		{ "ToolTip", "Marker color" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mUnlabeledMarkerSize_MetaData[] = {
		{ "Category", "Unlabeled Markers" },
		{ "Comment", "/** Marker Size */" },
		{ "DisplayName", "Unlabeled Marker Size" },
		{ "ModuleRelativePath", "Public/OptitrackLiveLinkDisplay.h" },
		{ "ToolTip", "Marker Size" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mDrawUnlabeledMarkers_MetaData[] = {
		{ "Category", "Unlabeled Markers" },
		{ "Comment", "/** Display Markers */" },
		{ "DisplayName", "Display Unlabeled Marker Data" },
		{ "ModuleRelativePath", "Public/OptitrackLiveLinkDisplay.h" },
		{ "ToolTip", "Display Markers" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mUnlabeledMarkerColor_MetaData[] = {
		{ "Category", "Unlabeled Markers" },
		{ "Comment", "/** Marker color */" },
		{ "DisplayName", "Unlabeled Marker Color" },
		{ "ModuleRelativePath", "Public/OptitrackLiveLinkDisplay.h" },
		{ "ToolTip", "Marker color" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Root;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_mLineThicknessScale;
	static void NewProp_mDrawAssets_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_mDrawAssets;
	static const UECodeGen_Private::FStructPropertyParams NewProp_mAssetColor;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_mSkeletonLineThickness;
	static void NewProp_mDrawSkeletons_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_mDrawSkeletons;
	static const UECodeGen_Private::FStructPropertyParams NewProp_mSkeletonColor;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_mRigidBodyLineThickness;
	static void NewProp_mDrawRigidBodies_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_mDrawRigidBodies;
	static const UECodeGen_Private::FStructPropertyParams NewProp_mRigidBodyColor;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_mTMarkersetLineThickness;
	static void NewProp_mDrawTMarkersets_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_mDrawTMarkersets;
	static const UECodeGen_Private::FStructPropertyParams NewProp_mTMarkersetColor;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_mLabeledMarkerSize;
	static void NewProp_mDrawLabeledMarkers_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_mDrawLabeledMarkers;
	static const UECodeGen_Private::FStructPropertyParams NewProp_mLabeledMarkerColor;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_mUnlabeledMarkerSize;
	static void NewProp_mDrawUnlabeledMarkers_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_mDrawUnlabeledMarkers;
	static const UECodeGen_Private::FStructPropertyParams NewProp_mUnlabeledMarkerColor;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AOptitrackLiveLinkDisplay>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_Root = { "Root", nullptr, (EPropertyFlags)0x0114000000080008, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AOptitrackLiveLinkDisplay, Root), Z_Construct_UClass_USceneComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Root_MetaData), NewProp_Root_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mLineThicknessScale = { "mLineThicknessScale", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AOptitrackLiveLinkDisplay, mLineThicknessScale), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mLineThicknessScale_MetaData), NewProp_mLineThicknessScale_MetaData) };
void Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mDrawAssets_SetBit(void* Obj)
{
	((AOptitrackLiveLinkDisplay*)Obj)->mDrawAssets = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mDrawAssets = { "mDrawAssets", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(AOptitrackLiveLinkDisplay), &Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mDrawAssets_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mDrawAssets_MetaData), NewProp_mDrawAssets_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mAssetColor = { "mAssetColor", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AOptitrackLiveLinkDisplay, mAssetColor), Z_Construct_UScriptStruct_FColor, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mAssetColor_MetaData), NewProp_mAssetColor_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mSkeletonLineThickness = { "mSkeletonLineThickness", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AOptitrackLiveLinkDisplay, mSkeletonLineThickness), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mSkeletonLineThickness_MetaData), NewProp_mSkeletonLineThickness_MetaData) };
void Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mDrawSkeletons_SetBit(void* Obj)
{
	((AOptitrackLiveLinkDisplay*)Obj)->mDrawSkeletons = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mDrawSkeletons = { "mDrawSkeletons", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(AOptitrackLiveLinkDisplay), &Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mDrawSkeletons_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mDrawSkeletons_MetaData), NewProp_mDrawSkeletons_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mSkeletonColor = { "mSkeletonColor", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AOptitrackLiveLinkDisplay, mSkeletonColor), Z_Construct_UScriptStruct_FColor, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mSkeletonColor_MetaData), NewProp_mSkeletonColor_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mRigidBodyLineThickness = { "mRigidBodyLineThickness", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AOptitrackLiveLinkDisplay, mRigidBodyLineThickness), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mRigidBodyLineThickness_MetaData), NewProp_mRigidBodyLineThickness_MetaData) };
void Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mDrawRigidBodies_SetBit(void* Obj)
{
	((AOptitrackLiveLinkDisplay*)Obj)->mDrawRigidBodies = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mDrawRigidBodies = { "mDrawRigidBodies", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(AOptitrackLiveLinkDisplay), &Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mDrawRigidBodies_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mDrawRigidBodies_MetaData), NewProp_mDrawRigidBodies_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mRigidBodyColor = { "mRigidBodyColor", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AOptitrackLiveLinkDisplay, mRigidBodyColor), Z_Construct_UScriptStruct_FColor, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mRigidBodyColor_MetaData), NewProp_mRigidBodyColor_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mTMarkersetLineThickness = { "mTMarkersetLineThickness", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AOptitrackLiveLinkDisplay, mTMarkersetLineThickness), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mTMarkersetLineThickness_MetaData), NewProp_mTMarkersetLineThickness_MetaData) };
void Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mDrawTMarkersets_SetBit(void* Obj)
{
	((AOptitrackLiveLinkDisplay*)Obj)->mDrawTMarkersets = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mDrawTMarkersets = { "mDrawTMarkersets", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(AOptitrackLiveLinkDisplay), &Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mDrawTMarkersets_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mDrawTMarkersets_MetaData), NewProp_mDrawTMarkersets_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mTMarkersetColor = { "mTMarkersetColor", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AOptitrackLiveLinkDisplay, mTMarkersetColor), Z_Construct_UScriptStruct_FColor, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mTMarkersetColor_MetaData), NewProp_mTMarkersetColor_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mLabeledMarkerSize = { "mLabeledMarkerSize", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AOptitrackLiveLinkDisplay, mLabeledMarkerSize), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mLabeledMarkerSize_MetaData), NewProp_mLabeledMarkerSize_MetaData) };
void Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mDrawLabeledMarkers_SetBit(void* Obj)
{
	((AOptitrackLiveLinkDisplay*)Obj)->mDrawLabeledMarkers = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mDrawLabeledMarkers = { "mDrawLabeledMarkers", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(AOptitrackLiveLinkDisplay), &Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mDrawLabeledMarkers_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mDrawLabeledMarkers_MetaData), NewProp_mDrawLabeledMarkers_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mLabeledMarkerColor = { "mLabeledMarkerColor", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AOptitrackLiveLinkDisplay, mLabeledMarkerColor), Z_Construct_UScriptStruct_FColor, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mLabeledMarkerColor_MetaData), NewProp_mLabeledMarkerColor_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mUnlabeledMarkerSize = { "mUnlabeledMarkerSize", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AOptitrackLiveLinkDisplay, mUnlabeledMarkerSize), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mUnlabeledMarkerSize_MetaData), NewProp_mUnlabeledMarkerSize_MetaData) };
void Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mDrawUnlabeledMarkers_SetBit(void* Obj)
{
	((AOptitrackLiveLinkDisplay*)Obj)->mDrawUnlabeledMarkers = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mDrawUnlabeledMarkers = { "mDrawUnlabeledMarkers", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(AOptitrackLiveLinkDisplay), &Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mDrawUnlabeledMarkers_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mDrawUnlabeledMarkers_MetaData), NewProp_mDrawUnlabeledMarkers_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mUnlabeledMarkerColor = { "mUnlabeledMarkerColor", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AOptitrackLiveLinkDisplay, mUnlabeledMarkerColor), Z_Construct_UScriptStruct_FColor, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mUnlabeledMarkerColor_MetaData), NewProp_mUnlabeledMarkerColor_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_Root,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mLineThicknessScale,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mDrawAssets,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mAssetColor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mSkeletonLineThickness,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mDrawSkeletons,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mSkeletonColor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mRigidBodyLineThickness,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mDrawRigidBodies,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mRigidBodyColor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mTMarkersetLineThickness,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mDrawTMarkersets,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mTMarkersetColor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mLabeledMarkerSize,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mDrawLabeledMarkers,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mLabeledMarkerColor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mUnlabeledMarkerSize,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mDrawUnlabeledMarkers,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::NewProp_mUnlabeledMarkerColor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AActor,
	(UObject* (*)())Z_Construct_UPackage__Script_OptitrackLiveLink,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::ClassParams = {
	&AOptitrackLiveLinkDisplay::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::PropPointers),
	0,
	0x009000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::Class_MetaDataParams), Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_AOptitrackLiveLinkDisplay()
{
	if (!Z_Registration_Info_UClass_AOptitrackLiveLinkDisplay.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AOptitrackLiveLinkDisplay.OuterSingleton, Z_Construct_UClass_AOptitrackLiveLinkDisplay_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AOptitrackLiveLinkDisplay.OuterSingleton;
}
template<> OPTITRACKLIVELINK_API UClass* StaticClass<AOptitrackLiveLinkDisplay>()
{
	return AOptitrackLiveLinkDisplay::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(AOptitrackLiveLinkDisplay);
// End Class AOptitrackLiveLinkDisplay

// Begin Registration
struct Z_CompiledInDeferFile_FID_HostProject_Plugins_OptitrackLiveLink_Source_OptitrackLiveLink_Public_OptitrackLiveLinkDisplay_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AOptitrackLiveLinkDisplay, AOptitrackLiveLinkDisplay::StaticClass, TEXT("AOptitrackLiveLinkDisplay"), &Z_Registration_Info_UClass_AOptitrackLiveLinkDisplay, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AOptitrackLiveLinkDisplay), 4021531032U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HostProject_Plugins_OptitrackLiveLink_Source_OptitrackLiveLink_Public_OptitrackLiveLinkDisplay_h_3873720644(TEXT("/Script/OptitrackLiveLink"),
	Z_CompiledInDeferFile_FID_HostProject_Plugins_OptitrackLiveLink_Source_OptitrackLiveLink_Public_OptitrackLiveLinkDisplay_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_HostProject_Plugins_OptitrackLiveLink_Source_OptitrackLiveLink_Public_OptitrackLiveLinkDisplay_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
