// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "OptitrackLiveLink/Public/LiveLinkNatNetRetarget.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeLiveLinkNatNetRetarget() {}

// Begin Cross Module References
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FRotator();
LIVELINKANIMATIONCORE_API UClass* Z_Construct_UClass_ULiveLinkRetargetAsset();
OPTITRACKLIVELINK_API UClass* Z_Construct_UClass_UOptiTrackLiveLinkRetarget();
OPTITRACKLIVELINK_API UClass* Z_Construct_UClass_UOptiTrackLiveLinkRetarget_NoRegister();
UPackage* Z_Construct_UPackage__Script_OptitrackLiveLink();
// End Cross Module References

// Begin Class UOptiTrackLiveLinkRetarget
void UOptiTrackLiveLinkRetarget::StaticRegisterNativesUOptiTrackLiveLinkRetarget()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UOptiTrackLiveLinkRetarget);
UClass* Z_Construct_UClass_UOptiTrackLiveLinkRetarget_NoRegister()
{
	return UOptiTrackLiveLinkRetarget::StaticClass();
}
struct Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "LiveLinkNatNetRetarget.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RootBoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "Comment", "//~ End ULiveLinkRetargetAsset interface\n" },
		{ "DisplayName", "Root Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_HipBoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Hip Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AbBoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Ab Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ChestBoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Chest Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_NeckBoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Neck Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_HeadBoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Head Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LShoulderBoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Left Shoulder Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LUArmBoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Left Upper Arm Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LFArmBoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Left Forearm Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LHandBoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Left Hand Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RShoulderBoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Right Shoulder Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RUArmBoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Right Upper Arm Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RFArmBoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Right Forearm Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RHandBoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Right Hand Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LThighBoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Left Thigh Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LShinBoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Left Shin Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LFootBoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Left Foot Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LToeBoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Left Toe Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RThighBoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Right Thigh Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RShinBoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Right Shin Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RFootBoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Right Foot Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RToeBoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Right Toe Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LThumb1BoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Left Thumb1 Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LThumb2BoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Left Thumb2 Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LThumb3BoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Left Thumb3 Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LIndex1BoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Left Index1 Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LIndex2BoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Left Index2 Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LIndex3BoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Left Index3 Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LMiddle1BoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Left Middle1 Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LMiddle2BoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Left Middle2 Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LMiddle3BoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Left Middle3 Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LRing1BoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Left Ring1 Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LRing2BoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Left Ring2 Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LRing3BoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Left Ring3 Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LPinky1BoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Left Pinky1 Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LPinky2BoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Left Pinky2 Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LPinky3BoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Left Pinky3 Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RThumb1BoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Right Thumb1 Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RThumb2BoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Right Thumb2 Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RThumb3BoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Right Thumb3 Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RIndex1BoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Right Index1 Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RIndex2BoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Right Index2 Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RIndex3BoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Right Index3 Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RMiddle1BoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Right Middle1 Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RMiddle2BoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Right Middle2 Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RMiddle3BoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Right Middle3 Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RRing1BoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Right Ring1 Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RRing2BoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Right Ring2 Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RRing3BoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Right Ring3 Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RPinky1BoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Right Pinky1 Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RPinky2BoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Right Pinky2 Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RPinky3BoneName_MetaData[] = {
		{ "Category", "Retargeting" },
		{ "DisplayName", "Right Pinky3 Bone Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mScaleBones_MetaData[] = {
		{ "Category", "Settings" },
		{ "DisplayName", "Scale Bones" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mLThumb1Rotation_MetaData[] = {
		{ "Category", "Settings" },
		{ "DisplayName", "Left Thumb1 Rotation Offset" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mLThumb2Rotation_MetaData[] = {
		{ "Category", "Settings" },
		{ "DisplayName", "Left Thumb2 Rotation Offset" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mLThumb3Rotation_MetaData[] = {
		{ "Category", "Settings" },
		{ "DisplayName", "Left Thumb3 Rotation Offset" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mRThumb1Rotation_MetaData[] = {
		{ "Category", "Settings" },
		{ "DisplayName", "Right Thumb1 Rotation Offset" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mRThumb2Rotation_MetaData[] = {
		{ "Category", "Settings" },
		{ "DisplayName", "Right Thumb2 Rotation Offset" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mRThumb3Rotation_MetaData[] = {
		{ "Category", "Settings" },
		{ "DisplayName", "Right Thumb3 Rotation Offset" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetRetarget.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStrPropertyParams NewProp_RootBoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_HipBoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_AbBoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_ChestBoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_NeckBoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_HeadBoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LShoulderBoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LUArmBoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LFArmBoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LHandBoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_RShoulderBoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_RUArmBoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_RFArmBoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_RHandBoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LThighBoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LShinBoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LFootBoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LToeBoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_RThighBoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_RShinBoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_RFootBoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_RToeBoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LThumb1BoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LThumb2BoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LThumb3BoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LIndex1BoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LIndex2BoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LIndex3BoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LMiddle1BoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LMiddle2BoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LMiddle3BoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LRing1BoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LRing2BoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LRing3BoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LPinky1BoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LPinky2BoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LPinky3BoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_RThumb1BoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_RThumb2BoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_RThumb3BoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_RIndex1BoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_RIndex2BoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_RIndex3BoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_RMiddle1BoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_RMiddle2BoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_RMiddle3BoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_RRing1BoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_RRing2BoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_RRing3BoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_RPinky1BoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_RPinky2BoneName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_RPinky3BoneName;
	static void NewProp_mScaleBones_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_mScaleBones;
	static const UECodeGen_Private::FStructPropertyParams NewProp_mLThumb1Rotation;
	static const UECodeGen_Private::FStructPropertyParams NewProp_mLThumb2Rotation;
	static const UECodeGen_Private::FStructPropertyParams NewProp_mLThumb3Rotation;
	static const UECodeGen_Private::FStructPropertyParams NewProp_mRThumb1Rotation;
	static const UECodeGen_Private::FStructPropertyParams NewProp_mRThumb2Rotation;
	static const UECodeGen_Private::FStructPropertyParams NewProp_mRThumb3Rotation;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOptiTrackLiveLinkRetarget>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RootBoneName = { "RootBoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, RootBoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RootBoneName_MetaData), NewProp_RootBoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_HipBoneName = { "HipBoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, HipBoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_HipBoneName_MetaData), NewProp_HipBoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_AbBoneName = { "AbBoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, AbBoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AbBoneName_MetaData), NewProp_AbBoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_ChestBoneName = { "ChestBoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, ChestBoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ChestBoneName_MetaData), NewProp_ChestBoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_NeckBoneName = { "NeckBoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, NeckBoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_NeckBoneName_MetaData), NewProp_NeckBoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_HeadBoneName = { "HeadBoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, HeadBoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_HeadBoneName_MetaData), NewProp_HeadBoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LShoulderBoneName = { "LShoulderBoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, LShoulderBoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LShoulderBoneName_MetaData), NewProp_LShoulderBoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LUArmBoneName = { "LUArmBoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, LUArmBoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LUArmBoneName_MetaData), NewProp_LUArmBoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LFArmBoneName = { "LFArmBoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, LFArmBoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LFArmBoneName_MetaData), NewProp_LFArmBoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LHandBoneName = { "LHandBoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, LHandBoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LHandBoneName_MetaData), NewProp_LHandBoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RShoulderBoneName = { "RShoulderBoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, RShoulderBoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RShoulderBoneName_MetaData), NewProp_RShoulderBoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RUArmBoneName = { "RUArmBoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, RUArmBoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RUArmBoneName_MetaData), NewProp_RUArmBoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RFArmBoneName = { "RFArmBoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, RFArmBoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RFArmBoneName_MetaData), NewProp_RFArmBoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RHandBoneName = { "RHandBoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, RHandBoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RHandBoneName_MetaData), NewProp_RHandBoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LThighBoneName = { "LThighBoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, LThighBoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LThighBoneName_MetaData), NewProp_LThighBoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LShinBoneName = { "LShinBoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, LShinBoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LShinBoneName_MetaData), NewProp_LShinBoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LFootBoneName = { "LFootBoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, LFootBoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LFootBoneName_MetaData), NewProp_LFootBoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LToeBoneName = { "LToeBoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, LToeBoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LToeBoneName_MetaData), NewProp_LToeBoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RThighBoneName = { "RThighBoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, RThighBoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RThighBoneName_MetaData), NewProp_RThighBoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RShinBoneName = { "RShinBoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, RShinBoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RShinBoneName_MetaData), NewProp_RShinBoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RFootBoneName = { "RFootBoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, RFootBoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RFootBoneName_MetaData), NewProp_RFootBoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RToeBoneName = { "RToeBoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, RToeBoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RToeBoneName_MetaData), NewProp_RToeBoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LThumb1BoneName = { "LThumb1BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, LThumb1BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LThumb1BoneName_MetaData), NewProp_LThumb1BoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LThumb2BoneName = { "LThumb2BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, LThumb2BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LThumb2BoneName_MetaData), NewProp_LThumb2BoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LThumb3BoneName = { "LThumb3BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, LThumb3BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LThumb3BoneName_MetaData), NewProp_LThumb3BoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LIndex1BoneName = { "LIndex1BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, LIndex1BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LIndex1BoneName_MetaData), NewProp_LIndex1BoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LIndex2BoneName = { "LIndex2BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, LIndex2BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LIndex2BoneName_MetaData), NewProp_LIndex2BoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LIndex3BoneName = { "LIndex3BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, LIndex3BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LIndex3BoneName_MetaData), NewProp_LIndex3BoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LMiddle1BoneName = { "LMiddle1BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, LMiddle1BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LMiddle1BoneName_MetaData), NewProp_LMiddle1BoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LMiddle2BoneName = { "LMiddle2BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, LMiddle2BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LMiddle2BoneName_MetaData), NewProp_LMiddle2BoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LMiddle3BoneName = { "LMiddle3BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, LMiddle3BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LMiddle3BoneName_MetaData), NewProp_LMiddle3BoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LRing1BoneName = { "LRing1BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, LRing1BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LRing1BoneName_MetaData), NewProp_LRing1BoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LRing2BoneName = { "LRing2BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, LRing2BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LRing2BoneName_MetaData), NewProp_LRing2BoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LRing3BoneName = { "LRing3BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, LRing3BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LRing3BoneName_MetaData), NewProp_LRing3BoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LPinky1BoneName = { "LPinky1BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, LPinky1BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LPinky1BoneName_MetaData), NewProp_LPinky1BoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LPinky2BoneName = { "LPinky2BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, LPinky2BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LPinky2BoneName_MetaData), NewProp_LPinky2BoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LPinky3BoneName = { "LPinky3BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, LPinky3BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LPinky3BoneName_MetaData), NewProp_LPinky3BoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RThumb1BoneName = { "RThumb1BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, RThumb1BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RThumb1BoneName_MetaData), NewProp_RThumb1BoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RThumb2BoneName = { "RThumb2BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, RThumb2BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RThumb2BoneName_MetaData), NewProp_RThumb2BoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RThumb3BoneName = { "RThumb3BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, RThumb3BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RThumb3BoneName_MetaData), NewProp_RThumb3BoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RIndex1BoneName = { "RIndex1BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, RIndex1BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RIndex1BoneName_MetaData), NewProp_RIndex1BoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RIndex2BoneName = { "RIndex2BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, RIndex2BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RIndex2BoneName_MetaData), NewProp_RIndex2BoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RIndex3BoneName = { "RIndex3BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, RIndex3BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RIndex3BoneName_MetaData), NewProp_RIndex3BoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RMiddle1BoneName = { "RMiddle1BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, RMiddle1BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RMiddle1BoneName_MetaData), NewProp_RMiddle1BoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RMiddle2BoneName = { "RMiddle2BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, RMiddle2BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RMiddle2BoneName_MetaData), NewProp_RMiddle2BoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RMiddle3BoneName = { "RMiddle3BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, RMiddle3BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RMiddle3BoneName_MetaData), NewProp_RMiddle3BoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RRing1BoneName = { "RRing1BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, RRing1BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RRing1BoneName_MetaData), NewProp_RRing1BoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RRing2BoneName = { "RRing2BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, RRing2BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RRing2BoneName_MetaData), NewProp_RRing2BoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RRing3BoneName = { "RRing3BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, RRing3BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RRing3BoneName_MetaData), NewProp_RRing3BoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RPinky1BoneName = { "RPinky1BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, RPinky1BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RPinky1BoneName_MetaData), NewProp_RPinky1BoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RPinky2BoneName = { "RPinky2BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, RPinky2BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RPinky2BoneName_MetaData), NewProp_RPinky2BoneName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RPinky3BoneName = { "RPinky3BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, RPinky3BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RPinky3BoneName_MetaData), NewProp_RPinky3BoneName_MetaData) };
void Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_mScaleBones_SetBit(void* Obj)
{
	((UOptiTrackLiveLinkRetarget*)Obj)->mScaleBones = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_mScaleBones = { "mScaleBones", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UOptiTrackLiveLinkRetarget), &Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_mScaleBones_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mScaleBones_MetaData), NewProp_mScaleBones_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_mLThumb1Rotation = { "mLThumb1Rotation", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, mLThumb1Rotation), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mLThumb1Rotation_MetaData), NewProp_mLThumb1Rotation_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_mLThumb2Rotation = { "mLThumb2Rotation", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, mLThumb2Rotation), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mLThumb2Rotation_MetaData), NewProp_mLThumb2Rotation_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_mLThumb3Rotation = { "mLThumb3Rotation", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, mLThumb3Rotation), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mLThumb3Rotation_MetaData), NewProp_mLThumb3Rotation_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_mRThumb1Rotation = { "mRThumb1Rotation", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, mRThumb1Rotation), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mRThumb1Rotation_MetaData), NewProp_mRThumb1Rotation_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_mRThumb2Rotation = { "mRThumb2Rotation", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, mRThumb2Rotation), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mRThumb2Rotation_MetaData), NewProp_mRThumb2Rotation_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_mRThumb3Rotation = { "mRThumb3Rotation", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptiTrackLiveLinkRetarget, mRThumb3Rotation), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mRThumb3Rotation_MetaData), NewProp_mRThumb3Rotation_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RootBoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_HipBoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_AbBoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_ChestBoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_NeckBoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_HeadBoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LShoulderBoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LUArmBoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LFArmBoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LHandBoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RShoulderBoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RUArmBoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RFArmBoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RHandBoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LThighBoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LShinBoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LFootBoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LToeBoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RThighBoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RShinBoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RFootBoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RToeBoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LThumb1BoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LThumb2BoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LThumb3BoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LIndex1BoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LIndex2BoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LIndex3BoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LMiddle1BoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LMiddle2BoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LMiddle3BoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LRing1BoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LRing2BoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LRing3BoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LPinky1BoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LPinky2BoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_LPinky3BoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RThumb1BoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RThumb2BoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RThumb3BoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RIndex1BoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RIndex2BoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RIndex3BoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RMiddle1BoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RMiddle2BoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RMiddle3BoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RRing1BoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RRing2BoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RRing3BoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RPinky1BoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RPinky2BoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_RPinky3BoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_mScaleBones,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_mLThumb1Rotation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_mLThumb2Rotation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_mLThumb3Rotation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_mRThumb1Rotation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_mRThumb2Rotation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::NewProp_mRThumb3Rotation,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_ULiveLinkRetargetAsset,
	(UObject* (*)())Z_Construct_UPackage__Script_OptitrackLiveLink,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::ClassParams = {
	&UOptiTrackLiveLinkRetarget::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::PropPointers),
	0,
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::Class_MetaDataParams), Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UOptiTrackLiveLinkRetarget()
{
	if (!Z_Registration_Info_UClass_UOptiTrackLiveLinkRetarget.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOptiTrackLiveLinkRetarget.OuterSingleton, Z_Construct_UClass_UOptiTrackLiveLinkRetarget_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOptiTrackLiveLinkRetarget.OuterSingleton;
}
template<> OPTITRACKLIVELINK_API UClass* StaticClass<UOptiTrackLiveLinkRetarget>()
{
	return UOptiTrackLiveLinkRetarget::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UOptiTrackLiveLinkRetarget);
UOptiTrackLiveLinkRetarget::~UOptiTrackLiveLinkRetarget() {}
// End Class UOptiTrackLiveLinkRetarget

// Begin Registration
struct Z_CompiledInDeferFile_FID_HostProject_Plugins_OptitrackLiveLink_Source_OptitrackLiveLink_Public_LiveLinkNatNetRetarget_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UOptiTrackLiveLinkRetarget, UOptiTrackLiveLinkRetarget::StaticClass, TEXT("UOptiTrackLiveLinkRetarget"), &Z_Registration_Info_UClass_UOptiTrackLiveLinkRetarget, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOptiTrackLiveLinkRetarget), 1784606530U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HostProject_Plugins_OptitrackLiveLink_Source_OptitrackLiveLink_Public_LiveLinkNatNetRetarget_h_852632535(TEXT("/Script/OptitrackLiveLink"),
	Z_CompiledInDeferFile_FID_HostProject_Plugins_OptitrackLiveLink_Source_OptitrackLiveLink_Public_LiveLinkNatNetRetarget_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_HostProject_Plugins_OptitrackLiveLink_Source_OptitrackLiveLink_Public_LiveLinkNatNetRetarget_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
