// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "OptitrackLiveLink/Public/LiveLinkNatNetSourceFactory.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeLiveLinkNatNetSourceFactory() {}

// Begin Cross Module References
LIVELINKINTERFACE_API UClass* Z_Construct_UClass_ULiveLinkSourceFactory();
OPTITRACKLIVELINK_API UClass* Z_Construct_UClass_ULiveLinkNatNetSourceFactory();
OPTITRACKLIVELINK_API UClass* Z_Construct_UClass_ULiveLinkNatNetSourceFactory_NoRegister();
UPackage* Z_Construct_UPackage__Script_OptitrackLiveLink();
// End Cross Module References

// Begin Class ULiveLinkNatNetSourceFactory
void ULiveLinkNatNetSourceFactory::StaticRegisterNativesULiveLinkNatNetSourceFactory()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ULiveLinkNatNetSourceFactory);
UClass* Z_Construct_UClass_ULiveLinkNatNetSourceFactory_NoRegister()
{
	return ULiveLinkNatNetSourceFactory::StaticClass();
}
struct Z_Construct_UClass_ULiveLinkNatNetSourceFactory_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "LiveLinkNatNetSourceFactory.h" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetSourceFactory.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ULiveLinkNatNetSourceFactory>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_ULiveLinkNatNetSourceFactory_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_ULiveLinkSourceFactory,
	(UObject* (*)())Z_Construct_UPackage__Script_OptitrackLiveLink,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ULiveLinkNatNetSourceFactory_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ULiveLinkNatNetSourceFactory_Statics::ClassParams = {
	&ULiveLinkNatNetSourceFactory::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x000000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ULiveLinkNatNetSourceFactory_Statics::Class_MetaDataParams), Z_Construct_UClass_ULiveLinkNatNetSourceFactory_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ULiveLinkNatNetSourceFactory()
{
	if (!Z_Registration_Info_UClass_ULiveLinkNatNetSourceFactory.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ULiveLinkNatNetSourceFactory.OuterSingleton, Z_Construct_UClass_ULiveLinkNatNetSourceFactory_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ULiveLinkNatNetSourceFactory.OuterSingleton;
}
template<> OPTITRACKLIVELINK_API UClass* StaticClass<ULiveLinkNatNetSourceFactory>()
{
	return ULiveLinkNatNetSourceFactory::StaticClass();
}
ULiveLinkNatNetSourceFactory::ULiveLinkNatNetSourceFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(ULiveLinkNatNetSourceFactory);
ULiveLinkNatNetSourceFactory::~ULiveLinkNatNetSourceFactory() {}
// End Class ULiveLinkNatNetSourceFactory

// Begin Registration
struct Z_CompiledInDeferFile_FID_HostProject_Plugins_OptitrackLiveLink_Source_OptitrackLiveLink_Public_LiveLinkNatNetSourceFactory_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ULiveLinkNatNetSourceFactory, ULiveLinkNatNetSourceFactory::StaticClass, TEXT("ULiveLinkNatNetSourceFactory"), &Z_Registration_Info_UClass_ULiveLinkNatNetSourceFactory, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ULiveLinkNatNetSourceFactory), 3881121635U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HostProject_Plugins_OptitrackLiveLink_Source_OptitrackLiveLink_Public_LiveLinkNatNetSourceFactory_h_2542953457(TEXT("/Script/OptitrackLiveLink"),
	Z_CompiledInDeferFile_FID_HostProject_Plugins_OptitrackLiveLink_Source_OptitrackLiveLink_Public_LiveLinkNatNetSourceFactory_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_HostProject_Plugins_OptitrackLiveLink_Source_OptitrackLiveLink_Public_LiveLinkNatNetSourceFactory_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
