// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "OptitrackLiveLink/Public/LiveLinkNatNetSettings.h"
#include "Runtime/LiveLinkInterface/Public/LiveLinkTypes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeLiveLinkNatNetSettings() {}

// Begin Cross Module References
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FRotator();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
LIVELINKINTERFACE_API UClass* Z_Construct_UClass_ULiveLinkSourceSettings();
LIVELINKINTERFACE_API UScriptStruct* Z_Construct_UScriptStruct_FLiveLinkSubjectKey();
OPTITRACKLIVELINK_API UClass* Z_Construct_UClass_UOptitrackLiveLinkSourceSettings();
OPTITRACKLIVELINK_API UClass* Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_NoRegister();
OPTITRACKLIVELINK_API UEnum* Z_Construct_UEnum_OptitrackLiveLink_EOptitrackLiveLinkConnectionType();
UPackage* Z_Construct_UPackage__Script_OptitrackLiveLink();
// End Cross Module References

// Begin Enum EOptitrackLiveLinkConnectionType
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EOptitrackLiveLinkConnectionType;
static UEnum* EOptitrackLiveLinkConnectionType_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EOptitrackLiveLinkConnectionType.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EOptitrackLiveLinkConnectionType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_OptitrackLiveLink_EOptitrackLiveLinkConnectionType, (UObject*)Z_Construct_UPackage__Script_OptitrackLiveLink(), TEXT("EOptitrackLiveLinkConnectionType"));
	}
	return Z_Registration_Info_UEnum_EOptitrackLiveLinkConnectionType.OuterSingleton;
}
template<> OPTITRACKLIVELINK_API UEnum* StaticEnum<EOptitrackLiveLinkConnectionType>()
{
	return EOptitrackLiveLinkConnectionType_StaticEnum();
}
struct Z_Construct_UEnum_OptitrackLiveLink_EOptitrackLiveLinkConnectionType_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetSettings.h" },
		{ "Multicast.DisplayName", "Multicast" },
		{ "Multicast.Name", "EOptitrackLiveLinkConnectionType::Multicast" },
		{ "Unicast.DisplayName", "Unicast" },
		{ "Unicast.Name", "EOptitrackLiveLinkConnectionType::Unicast" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EOptitrackLiveLinkConnectionType::Multicast", (int64)EOptitrackLiveLinkConnectionType::Multicast },
		{ "EOptitrackLiveLinkConnectionType::Unicast", (int64)EOptitrackLiveLinkConnectionType::Unicast },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_OptitrackLiveLink_EOptitrackLiveLinkConnectionType_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_OptitrackLiveLink,
	nullptr,
	"EOptitrackLiveLinkConnectionType",
	"EOptitrackLiveLinkConnectionType",
	Z_Construct_UEnum_OptitrackLiveLink_EOptitrackLiveLinkConnectionType_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_OptitrackLiveLink_EOptitrackLiveLinkConnectionType_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_OptitrackLiveLink_EOptitrackLiveLinkConnectionType_Statics::Enum_MetaDataParams), Z_Construct_UEnum_OptitrackLiveLink_EOptitrackLiveLinkConnectionType_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_OptitrackLiveLink_EOptitrackLiveLinkConnectionType()
{
	if (!Z_Registration_Info_UEnum_EOptitrackLiveLinkConnectionType.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EOptitrackLiveLinkConnectionType.InnerSingleton, Z_Construct_UEnum_OptitrackLiveLink_EOptitrackLiveLinkConnectionType_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EOptitrackLiveLinkConnectionType.InnerSingleton;
}
// End Enum EOptitrackLiveLinkConnectionType

// Begin Class UOptitrackLiveLinkSourceSettings
void UOptitrackLiveLinkSourceSettings::StaticRegisterNativesUOptitrackLiveLinkSourceSettings()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UOptitrackLiveLinkSourceSettings);
UClass* Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_NoRegister()
{
	return UOptitrackLiveLinkSourceSettings::StaticClass();
}
struct Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "Comment", "/** Live link streaming settings */" },
		{ "HideCategories", "Settings" },
		{ "IncludePath", "LiveLinkNatNetSettings.h" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetSettings.h" },
		{ "ToolTip", "Live link streaming settings" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Location_MetaData[] = {
		{ "Category", "StreamedDataOffset" },
		{ "Comment", "/** Origin of streamed data. */" },
		{ "DisplayName", "Location" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetSettings.h" },
		{ "ToolTip", "Origin of streamed data." },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Orientation_MetaData[] = {
		{ "Category", "StreamedDataOffset" },
		{ "Comment", "/** Orientation of streamed data. */" },
		{ "DisplayName", "Orientation" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetSettings.h" },
		{ "ToolTip", "Orientation of streamed data." },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Scale_MetaData[] = {
		{ "Category", "StreamedDataOffset" },
		{ "Comment", "/** Scale of streamed data. */" },
		{ "DisplayName", "Scale" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetSettings.h" },
		{ "ToolTip", "Scale of streamed data." },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CreateLabeledMarkerSubjects_MetaData[] = {
		{ "Category", "Markers" },
		{ "Comment", "/** Whether to create LiveLink subjects for labeled markers */" },
		{ "DisplayName", "Create Labeled Markers" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetSettings.h" },
		{ "ToolTip", "Whether to create LiveLink subjects for labeled markers" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CreateUnlabeledMarkerSubjects_MetaData[] = {
		{ "Category", "Markers" },
		{ "Comment", "/** Whether to create LiveLink subjects for unlabeled markers */" },
		{ "DisplayName", "Create Unlabeled Markers" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetSettings.h" },
		{ "ToolTip", "Whether to create LiveLink subjects for unlabeled markers" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LiveLinkSubject_MetaData[] = {
		{ "Category", "Live Link Roles" },
		{ "Comment", "/** Drop down of subjects */" },
		{ "DisplayName", "Live Link Subject" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetSettings.h" },
		{ "ToolTip", "Drop down of subjects" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SubjectIsCamera_MetaData[] = {
		{ "Category", "Live Link Roles" },
		{ "Comment", "/** Checkbox to assign camera role for rigid bodies */" },
		{ "DisplayName", "Stream As Camera (Rigid Bodies Only)" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetSettings.h" },
		{ "ToolTip", "Checkbox to assign camera role for rigid bodies" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TimecodeProvider_MetaData[] = {
		{ "Category", "TimecodeProvider" },
		{ "Comment", "/** Set LiveLink as timecode provider */" },
		{ "DisplayName", "Timecode Provider" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetSettings.h" },
		{ "ToolTip", "Set LiveLink as timecode provider" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SubjectName_MetaData[] = {
		{ "Category", "TimecodeProvider" },
		{ "Comment", "/** Timecode provider subject name */" },
		{ "DisplayName", "Subject Name" },
		{ "ModuleRelativePath", "Public/LiveLinkNatNetSettings.h" },
		{ "ToolTip", "Timecode provider subject name" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_Location;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Orientation;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Scale;
	static void NewProp_CreateLabeledMarkerSubjects_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_CreateLabeledMarkerSubjects;
	static void NewProp_CreateUnlabeledMarkerSubjects_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_CreateUnlabeledMarkerSubjects;
	static const UECodeGen_Private::FStructPropertyParams NewProp_LiveLinkSubject;
	static void NewProp_SubjectIsCamera_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_SubjectIsCamera;
	static void NewProp_TimecodeProvider_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_TimecodeProvider;
	static const UECodeGen_Private::FNamePropertyParams NewProp_SubjectName;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOptitrackLiveLinkSourceSettings>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::NewProp_Location = { "Location", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptitrackLiveLinkSourceSettings, Location), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Location_MetaData), NewProp_Location_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::NewProp_Orientation = { "Orientation", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptitrackLiveLinkSourceSettings, Orientation), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Orientation_MetaData), NewProp_Orientation_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::NewProp_Scale = { "Scale", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptitrackLiveLinkSourceSettings, Scale), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Scale_MetaData), NewProp_Scale_MetaData) };
void Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::NewProp_CreateLabeledMarkerSubjects_SetBit(void* Obj)
{
	((UOptitrackLiveLinkSourceSettings*)Obj)->CreateLabeledMarkerSubjects = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::NewProp_CreateLabeledMarkerSubjects = { "CreateLabeledMarkerSubjects", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UOptitrackLiveLinkSourceSettings), &Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::NewProp_CreateLabeledMarkerSubjects_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CreateLabeledMarkerSubjects_MetaData), NewProp_CreateLabeledMarkerSubjects_MetaData) };
void Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::NewProp_CreateUnlabeledMarkerSubjects_SetBit(void* Obj)
{
	((UOptitrackLiveLinkSourceSettings*)Obj)->CreateUnlabeledMarkerSubjects = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::NewProp_CreateUnlabeledMarkerSubjects = { "CreateUnlabeledMarkerSubjects", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UOptitrackLiveLinkSourceSettings), &Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::NewProp_CreateUnlabeledMarkerSubjects_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CreateUnlabeledMarkerSubjects_MetaData), NewProp_CreateUnlabeledMarkerSubjects_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::NewProp_LiveLinkSubject = { "LiveLinkSubject", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptitrackLiveLinkSourceSettings, LiveLinkSubject), Z_Construct_UScriptStruct_FLiveLinkSubjectKey, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LiveLinkSubject_MetaData), NewProp_LiveLinkSubject_MetaData) }; // 896940852
void Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::NewProp_SubjectIsCamera_SetBit(void* Obj)
{
	((UOptitrackLiveLinkSourceSettings*)Obj)->SubjectIsCamera = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::NewProp_SubjectIsCamera = { "SubjectIsCamera", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UOptitrackLiveLinkSourceSettings), &Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::NewProp_SubjectIsCamera_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SubjectIsCamera_MetaData), NewProp_SubjectIsCamera_MetaData) };
void Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::NewProp_TimecodeProvider_SetBit(void* Obj)
{
	((UOptitrackLiveLinkSourceSettings*)Obj)->TimecodeProvider = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::NewProp_TimecodeProvider = { "TimecodeProvider", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UOptitrackLiveLinkSourceSettings), &Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::NewProp_TimecodeProvider_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TimecodeProvider_MetaData), NewProp_TimecodeProvider_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::NewProp_SubjectName = { "SubjectName", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOptitrackLiveLinkSourceSettings, SubjectName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SubjectName_MetaData), NewProp_SubjectName_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::NewProp_Location,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::NewProp_Orientation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::NewProp_Scale,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::NewProp_CreateLabeledMarkerSubjects,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::NewProp_CreateUnlabeledMarkerSubjects,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::NewProp_LiveLinkSubject,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::NewProp_SubjectIsCamera,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::NewProp_TimecodeProvider,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::NewProp_SubjectName,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_ULiveLinkSourceSettings,
	(UObject* (*)())Z_Construct_UPackage__Script_OptitrackLiveLink,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::ClassParams = {
	&UOptitrackLiveLinkSourceSettings::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::PropPointers),
	0,
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::Class_MetaDataParams), Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UOptitrackLiveLinkSourceSettings()
{
	if (!Z_Registration_Info_UClass_UOptitrackLiveLinkSourceSettings.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOptitrackLiveLinkSourceSettings.OuterSingleton, Z_Construct_UClass_UOptitrackLiveLinkSourceSettings_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOptitrackLiveLinkSourceSettings.OuterSingleton;
}
template<> OPTITRACKLIVELINK_API UClass* StaticClass<UOptitrackLiveLinkSourceSettings>()
{
	return UOptitrackLiveLinkSourceSettings::StaticClass();
}
UOptitrackLiveLinkSourceSettings::UOptitrackLiveLinkSourceSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UOptitrackLiveLinkSourceSettings);
UOptitrackLiveLinkSourceSettings::~UOptitrackLiveLinkSourceSettings() {}
// End Class UOptitrackLiveLinkSourceSettings

// Begin Registration
struct Z_CompiledInDeferFile_FID_HostProject_Plugins_OptitrackLiveLink_Source_OptitrackLiveLink_Public_LiveLinkNatNetSettings_h_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ EOptitrackLiveLinkConnectionType_StaticEnum, TEXT("EOptitrackLiveLinkConnectionType"), &Z_Registration_Info_UEnum_EOptitrackLiveLinkConnectionType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 4245985703U) },
	};
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UOptitrackLiveLinkSourceSettings, UOptitrackLiveLinkSourceSettings::StaticClass, TEXT("UOptitrackLiveLinkSourceSettings"), &Z_Registration_Info_UClass_UOptitrackLiveLinkSourceSettings, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOptitrackLiveLinkSourceSettings), 372843529U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HostProject_Plugins_OptitrackLiveLink_Source_OptitrackLiveLink_Public_LiveLinkNatNetSettings_h_3102714771(TEXT("/Script/OptitrackLiveLink"),
	Z_CompiledInDeferFile_FID_HostProject_Plugins_OptitrackLiveLink_Source_OptitrackLiveLink_Public_LiveLinkNatNetSettings_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_HostProject_Plugins_OptitrackLiveLink_Source_OptitrackLiveLink_Public_LiveLinkNatNetSettings_h_Statics::ClassInfo),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_HostProject_Plugins_OptitrackLiveLink_Source_OptitrackLiveLink_Public_LiveLinkNatNetSettings_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_HostProject_Plugins_OptitrackLiveLink_Source_OptitrackLiveLink_Public_LiveLinkNatNetSettings_h_Statics::EnumInfo));
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
