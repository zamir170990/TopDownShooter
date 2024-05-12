// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "TDS/CTypes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCTypes() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_UBlueprintFunctionLibrary();
	TDS_API UClass* Z_Construct_UClass_UCTypes();
	TDS_API UClass* Z_Construct_UClass_UCTypes_NoRegister();
	TDS_API UEnum* Z_Construct_UEnum_TDS_EMovementState();
	TDS_API UScriptStruct* Z_Construct_UScriptStruct_FCharacterSpeed();
	UPackage* Z_Construct_UPackage__Script_TDS();
// End Cross Module References
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_EMovementState;
	static UEnum* EMovementState_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_EMovementState.OuterSingleton)
		{
			Z_Registration_Info_UEnum_EMovementState.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_TDS_EMovementState, (UObject*)Z_Construct_UPackage__Script_TDS(), TEXT("EMovementState"));
		}
		return Z_Registration_Info_UEnum_EMovementState.OuterSingleton;
	}
	template<> TDS_API UEnum* StaticEnum<EMovementState>()
	{
		return EMovementState_StaticEnum();
	}
	struct Z_Construct_UEnum_TDS_EMovementState_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_TDS_EMovementState_Statics::Enumerators[] = {
		{ "EMovementState::Aim_State", (int64)EMovementState::Aim_State },
		{ "EMovementState::Walk_State", (int64)EMovementState::Walk_State },
		{ "EMovementState::Run_State", (int64)EMovementState::Run_State },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_TDS_EMovementState_Statics::Enum_MetaDataParams[] = {
		{ "Aim_State.DisplayName", "Aim State" },
		{ "Aim_State.Name", "EMovementState::Aim_State" },
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "CTypes.h" },
		{ "Run_State.DisplayName", "Run State" },
		{ "Run_State.Name", "EMovementState::Run_State" },
		{ "Walk_State.DisplayName", "Walk State" },
		{ "Walk_State.Name", "EMovementState::Walk_State" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_TDS_EMovementState_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_TDS,
		nullptr,
		"EMovementState",
		"EMovementState",
		Z_Construct_UEnum_TDS_EMovementState_Statics::Enumerators,
		RF_Public|RF_Transient|RF_MarkAsNative,
		UE_ARRAY_COUNT(Z_Construct_UEnum_TDS_EMovementState_Statics::Enumerators),
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_TDS_EMovementState_Statics::Enum_MetaDataParams), Z_Construct_UEnum_TDS_EMovementState_Statics::Enum_MetaDataParams)
	};
	UEnum* Z_Construct_UEnum_TDS_EMovementState()
	{
		if (!Z_Registration_Info_UEnum_EMovementState.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EMovementState.InnerSingleton, Z_Construct_UEnum_TDS_EMovementState_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_EMovementState.InnerSingleton;
	}
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_CharacterSpeed;
class UScriptStruct* FCharacterSpeed::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_CharacterSpeed.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_CharacterSpeed.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FCharacterSpeed, (UObject*)Z_Construct_UPackage__Script_TDS(), TEXT("CharacterSpeed"));
	}
	return Z_Registration_Info_UScriptStruct_CharacterSpeed.OuterSingleton;
}
template<> TDS_API UScriptStruct* StaticStruct<FCharacterSpeed>()
{
	return FCharacterSpeed::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FCharacterSpeed_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_AimSpeed_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_AimSpeed;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_WalkSpeed_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_WalkSpeed;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RunSpeed_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_RunSpeed;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FCharacterSpeed_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "CTypes.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FCharacterSpeed_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FCharacterSpeed>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FCharacterSpeed_Statics::NewProp_AimSpeed_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "CTypes.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FCharacterSpeed_Statics::NewProp_AimSpeed = { "AimSpeed", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FCharacterSpeed, AimSpeed), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FCharacterSpeed_Statics::NewProp_AimSpeed_MetaData), Z_Construct_UScriptStruct_FCharacterSpeed_Statics::NewProp_AimSpeed_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FCharacterSpeed_Statics::NewProp_WalkSpeed_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "CTypes.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FCharacterSpeed_Statics::NewProp_WalkSpeed = { "WalkSpeed", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FCharacterSpeed, WalkSpeed), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FCharacterSpeed_Statics::NewProp_WalkSpeed_MetaData), Z_Construct_UScriptStruct_FCharacterSpeed_Statics::NewProp_WalkSpeed_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FCharacterSpeed_Statics::NewProp_RunSpeed_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "CTypes.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FCharacterSpeed_Statics::NewProp_RunSpeed = { "RunSpeed", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FCharacterSpeed, RunSpeed), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FCharacterSpeed_Statics::NewProp_RunSpeed_MetaData), Z_Construct_UScriptStruct_FCharacterSpeed_Statics::NewProp_RunSpeed_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FCharacterSpeed_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCharacterSpeed_Statics::NewProp_AimSpeed,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCharacterSpeed_Statics::NewProp_WalkSpeed,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCharacterSpeed_Statics::NewProp_RunSpeed,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FCharacterSpeed_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_TDS,
		nullptr,
		&NewStructOps,
		"CharacterSpeed",
		Z_Construct_UScriptStruct_FCharacterSpeed_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FCharacterSpeed_Statics::PropPointers),
		sizeof(FCharacterSpeed),
		alignof(FCharacterSpeed),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FCharacterSpeed_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FCharacterSpeed_Statics::Struct_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FCharacterSpeed_Statics::PropPointers) < 2048);
	UScriptStruct* Z_Construct_UScriptStruct_FCharacterSpeed()
	{
		if (!Z_Registration_Info_UScriptStruct_CharacterSpeed.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_CharacterSpeed.InnerSingleton, Z_Construct_UScriptStruct_FCharacterSpeed_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_CharacterSpeed.InnerSingleton;
	}
	void UCTypes::StaticRegisterNativesUCTypes()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UCTypes);
	UClass* Z_Construct_UClass_UCTypes_NoRegister()
	{
		return UCTypes::StaticClass();
	}
	struct Z_Construct_UClass_UCTypes_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UCTypes_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UBlueprintFunctionLibrary,
		(UObject* (*)())Z_Construct_UPackage__Script_TDS,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UCTypes_Statics::DependentSingletons) < 16);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCTypes_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "CTypes.h" },
		{ "ModuleRelativePath", "CTypes.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UCTypes_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UCTypes>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UCTypes_Statics::ClassParams = {
		&UCTypes::StaticClass,
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
		0x001000A0u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UCTypes_Statics::Class_MetaDataParams), Z_Construct_UClass_UCTypes_Statics::Class_MetaDataParams)
	};
	UClass* Z_Construct_UClass_UCTypes()
	{
		if (!Z_Registration_Info_UClass_UCTypes.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UCTypes.OuterSingleton, Z_Construct_UClass_UCTypes_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UCTypes.OuterSingleton;
	}
	template<> TDS_API UClass* StaticClass<UCTypes>()
	{
		return UCTypes::StaticClass();
	}
	UCTypes::UCTypes(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UCTypes);
	UCTypes::~UCTypes() {}
	struct Z_CompiledInDeferFile_FID_UnrealEngineProject_TDS_Source_TDS_CTypes_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
		static const FStructRegisterCompiledInInfo ScriptStructInfo[];
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealEngineProject_TDS_Source_TDS_CTypes_h_Statics::EnumInfo[] = {
		{ EMovementState_StaticEnum, TEXT("EMovementState"), &Z_Registration_Info_UEnum_EMovementState, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 825789406U) },
	};
	const FStructRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealEngineProject_TDS_Source_TDS_CTypes_h_Statics::ScriptStructInfo[] = {
		{ FCharacterSpeed::StaticStruct, Z_Construct_UScriptStruct_FCharacterSpeed_Statics::NewStructOps, TEXT("CharacterSpeed"), &Z_Registration_Info_UScriptStruct_CharacterSpeed, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FCharacterSpeed), 3627308487U) },
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealEngineProject_TDS_Source_TDS_CTypes_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UCTypes, UCTypes::StaticClass, TEXT("UCTypes"), &Z_Registration_Info_UClass_UCTypes, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UCTypes), 3659091465U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealEngineProject_TDS_Source_TDS_CTypes_h_2452939613(TEXT("/Script/TDS"),
		Z_CompiledInDeferFile_FID_UnrealEngineProject_TDS_Source_TDS_CTypes_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealEngineProject_TDS_Source_TDS_CTypes_h_Statics::ClassInfo),
		Z_CompiledInDeferFile_FID_UnrealEngineProject_TDS_Source_TDS_CTypes_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealEngineProject_TDS_Source_TDS_CTypes_h_Statics::ScriptStructInfo),
		Z_CompiledInDeferFile_FID_UnrealEngineProject_TDS_Source_TDS_CTypes_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealEngineProject_TDS_Source_TDS_CTypes_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
