// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "OptitrackLiveLink/Public/LiveLinkNatNetSource.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeLiveLinkNatNetSource() {}

// Begin Cross Module References
OPTITRACKLIVELINK_API UEnum* Z_Construct_UEnum_OptitrackLiveLink_EOptitrackLiveLinkBoneNamingConvention();
UPackage* Z_Construct_UPackage__Script_OptitrackLiveLink();
// End Cross Module References

// Begin Enum EOptitrackLiveLinkBoneNamingConvention
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EOptitrackLiveLinkBoneNamingConvention;
static UEnum* EOptitrackLiveLinkBoneNamingConvention_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EOptitrackLiveLinkBoneNamingConvention.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EOptitrackLiveLinkBoneNamingConvention.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_OptitrackLiveLink_EOptitrackLiveLinkBoneNamingConvention, (UObject*)Z_Construct_UPackage__Script_OptitrackLiveLink(), TEXT("EOptitrackLiveLinkBoneNamingConvention"));
	}
	return Z_Registration_Info_UEnum_EOptitrackLiveLinkBoneNamingConvention.OuterSingleton;
}
template<> OPTITRACKLIVELINK_API UEnum* StaticEnum<EOptitrackLiveLinkBoneNamingConvention>()
{
	return EOptitrackLiveLinkBoneNamingConvention_StaticEnum();
}
struct Z_Construct_UEnum_OptitrackLiveLink_EOptitrackLiveLinkBoneNamingConvention_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "BVH.DisplayName", "BVH" },
		{ "BVH.Name", "EOptitrackLiveLinkBoneNamingConvention::BVH" },
		{ "FBX.DisplayName", "FBX" },
		{ "FBX.Name", "EOptitrackLiveLinkBoneNamingConvention::FBX" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetSource.h" },
		{ "Motive.DisplayName", "Motive" },
		{ "Motive.Name", "EOptitrackLiveLinkBoneNamingConvention::Motive" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EOptitrackLiveLinkBoneNamingConvention::Motive", (int64)EOptitrackLiveLinkBoneNamingConvention::Motive },
		{ "EOptitrackLiveLinkBoneNamingConvention::FBX", (int64)EOptitrackLiveLinkBoneNamingConvention::FBX },
		{ "EOptitrackLiveLinkBoneNamingConvention::BVH", (int64)EOptitrackLiveLinkBoneNamingConvention::BVH },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_OptitrackLiveLink_EOptitrackLiveLinkBoneNamingConvention_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_OptitrackLiveLink,
	nullptr,
	"EOptitrackLiveLinkBoneNamingConvention",
	"EOptitrackLiveLinkBoneNamingConvention",
	Z_Construct_UEnum_OptitrackLiveLink_EOptitrackLiveLinkBoneNamingConvention_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_OptitrackLiveLink_EOptitrackLiveLinkBoneNamingConvention_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_OptitrackLiveLink_EOptitrackLiveLinkBoneNamingConvention_Statics::Enum_MetaDataParams), Z_Construct_UEnum_OptitrackLiveLink_EOptitrackLiveLinkBoneNamingConvention_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_OptitrackLiveLink_EOptitrackLiveLinkBoneNamingConvention()
{
	if (!Z_Registration_Info_UEnum_EOptitrackLiveLinkBoneNamingConvention.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EOptitrackLiveLinkBoneNamingConvention.InnerSingleton, Z_Construct_UEnum_OptitrackLiveLink_EOptitrackLiveLinkBoneNamingConvention_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EOptitrackLiveLinkBoneNamingConvention.InnerSingleton;
}
// End Enum EOptitrackLiveLinkBoneNamingConvention

// Begin Registration
struct Z_CompiledInDeferFile_FID_HostProject_Plugins_OptitrackLiveLink_Source_OptitrackLiveLink_Public_LiveLinkNatNetSource_h_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ EOptitrackLiveLinkBoneNamingConvention_StaticEnum, TEXT("EOptitrackLiveLinkBoneNamingConvention"), &Z_Registration_Info_UEnum_EOptitrackLiveLinkBoneNamingConvention, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 3701849053U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HostProject_Plugins_OptitrackLiveLink_Source_OptitrackLiveLink_Public_LiveLinkNatNetSource_h_2290633133(TEXT("/Script/OptitrackLiveLink"),
	nullptr, 0,
	nullptr, 0,
	Z_CompiledInDeferFile_FID_HostProject_Plugins_OptitrackLiveLink_Source_OptitrackLiveLink_Public_LiveLinkNatNetSource_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_HostProject_Plugins_OptitrackLiveLink_Source_OptitrackLiveLink_Public_LiveLinkNatNetSource_h_Statics::EnumInfo));
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
