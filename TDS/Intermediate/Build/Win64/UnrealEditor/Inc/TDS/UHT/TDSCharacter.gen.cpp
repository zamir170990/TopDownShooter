// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "TDS/TDSCharacter.h"
#include "TDS/CTypes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTDSCharacter() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_ACharacter();
	TDS_API UClass* Z_Construct_UClass_ATDSCharacter();
	TDS_API UClass* Z_Construct_UClass_ATDSCharacter_NoRegister();
	TDS_API UEnum* Z_Construct_UEnum_TDS_EMovementState();
	TDS_API UScriptStruct* Z_Construct_UScriptStruct_FCharacterSpeed();
	UPackage* Z_Construct_UPackage__Script_TDS();
// End Cross Module References
	DEFINE_FUNCTION(ATDSCharacter::execChangeMovementState)
	{
		P_GET_ENUM(EMovementState,Z_Param_NewMovementState);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->ChangeMovementState(EMovementState(Z_Param_NewMovementState));
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(ATDSCharacter::execCharacterUpdate)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->CharacterUpdate();
		P_NATIVE_END;
	}
	void ATDSCharacter::StaticRegisterNativesATDSCharacter()
	{
		UClass* Class = ATDSCharacter::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "ChangeMovementState", &ATDSCharacter::execChangeMovementState },
			{ "CharacterUpdate", &ATDSCharacter::execCharacterUpdate },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_ATDSCharacter_ChangeMovementState_Statics
	{
		struct TDSCharacter_eventChangeMovementState_Parms
		{
			EMovementState NewMovementState;
		};
		static const UECodeGen_Private::FBytePropertyParams NewProp_NewMovementState_Underlying;
		static const UECodeGen_Private::FEnumPropertyParams NewProp_NewMovementState;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_ATDSCharacter_ChangeMovementState_Statics::NewProp_NewMovementState_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_ATDSCharacter_ChangeMovementState_Statics::NewProp_NewMovementState = { "NewMovementState", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(TDSCharacter_eventChangeMovementState_Parms, NewMovementState), Z_Construct_UEnum_TDS_EMovementState, METADATA_PARAMS(0, nullptr) }; // 825789406
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ATDSCharacter_ChangeMovementState_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ATDSCharacter_ChangeMovementState_Statics::NewProp_NewMovementState_Underlying,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ATDSCharacter_ChangeMovementState_Statics::NewProp_NewMovementState,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ATDSCharacter_ChangeMovementState_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "TDSCharacter.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ATDSCharacter_ChangeMovementState_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ATDSCharacter, nullptr, "ChangeMovementState", nullptr, nullptr, Z_Construct_UFunction_ATDSCharacter_ChangeMovementState_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ATDSCharacter_ChangeMovementState_Statics::PropPointers), sizeof(Z_Construct_UFunction_ATDSCharacter_ChangeMovementState_Statics::TDSCharacter_eventChangeMovementState_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ATDSCharacter_ChangeMovementState_Statics::Function_MetaDataParams), Z_Construct_UFunction_ATDSCharacter_ChangeMovementState_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ATDSCharacter_ChangeMovementState_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_ATDSCharacter_ChangeMovementState_Statics::TDSCharacter_eventChangeMovementState_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_ATDSCharacter_ChangeMovementState()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ATDSCharacter_ChangeMovementState_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_ATDSCharacter_CharacterUpdate_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ATDSCharacter_CharacterUpdate_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "TDSCharacter.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ATDSCharacter_CharacterUpdate_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ATDSCharacter, nullptr, "CharacterUpdate", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ATDSCharacter_CharacterUpdate_Statics::Function_MetaDataParams), Z_Construct_UFunction_ATDSCharacter_CharacterUpdate_Statics::Function_MetaDataParams) };
	UFunction* Z_Construct_UFunction_ATDSCharacter_CharacterUpdate()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ATDSCharacter_CharacterUpdate_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ATDSCharacter);
	UClass* Z_Construct_UClass_ATDSCharacter_NoRegister()
	{
		return ATDSCharacter::StaticClass();
	}
	struct Z_Construct_UClass_ATDSCharacter_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const UECodeGen_Private::FBytePropertyParams NewProp_MovementState_Underlying;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MovementState_MetaData[];
#endif
		static const UECodeGen_Private::FEnumPropertyParams NewProp_MovementState;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MovementInfo_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_MovementInfo;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ATDSCharacter_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_ACharacter,
		(UObject* (*)())Z_Construct_UPackage__Script_TDS,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ATDSCharacter_Statics::DependentSingletons) < 16);
	const FClassFunctionLinkInfo Z_Construct_UClass_ATDSCharacter_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_ATDSCharacter_ChangeMovementState, "ChangeMovementState" }, // 2491098600
		{ &Z_Construct_UFunction_ATDSCharacter_CharacterUpdate, "CharacterUpdate" }, // 2566595300
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ATDSCharacter_Statics::FuncInfo) < 2048);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ATDSCharacter_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Navigation" },
		{ "IncludePath", "TDSCharacter.h" },
		{ "ModuleRelativePath", "TDSCharacter.h" },
	};
#endif
	const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_ATDSCharacter_Statics::NewProp_MovementState_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ATDSCharacter_Statics::NewProp_MovementState_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "TDSCharacter.h" },
	};
#endif
	const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_ATDSCharacter_Statics::NewProp_MovementState = { "MovementState", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ATDSCharacter, MovementState), Z_Construct_UEnum_TDS_EMovementState, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ATDSCharacter_Statics::NewProp_MovementState_MetaData), Z_Construct_UClass_ATDSCharacter_Statics::NewProp_MovementState_MetaData) }; // 825789406
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ATDSCharacter_Statics::NewProp_MovementInfo_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "TDSCharacter.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_ATDSCharacter_Statics::NewProp_MovementInfo = { "MovementInfo", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ATDSCharacter, MovementInfo), Z_Construct_UScriptStruct_FCharacterSpeed, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ATDSCharacter_Statics::NewProp_MovementInfo_MetaData), Z_Construct_UClass_ATDSCharacter_Statics::NewProp_MovementInfo_MetaData) }; // 3627308487
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ATDSCharacter_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ATDSCharacter_Statics::NewProp_MovementState_Underlying,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ATDSCharacter_Statics::NewProp_MovementState,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ATDSCharacter_Statics::NewProp_MovementInfo,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_ATDSCharacter_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ATDSCharacter>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_ATDSCharacter_Statics::ClassParams = {
		&ATDSCharacter::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		Z_Construct_UClass_ATDSCharacter_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		UE_ARRAY_COUNT(Z_Construct_UClass_ATDSCharacter_Statics::PropPointers),
		0,
		0x009000A4u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ATDSCharacter_Statics::Class_MetaDataParams), Z_Construct_UClass_ATDSCharacter_Statics::Class_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ATDSCharacter_Statics::PropPointers) < 2048);
	UClass* Z_Construct_UClass_ATDSCharacter()
	{
		if (!Z_Registration_Info_UClass_ATDSCharacter.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ATDSCharacter.OuterSingleton, Z_Construct_UClass_ATDSCharacter_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_ATDSCharacter.OuterSingleton;
	}
	template<> TDS_API UClass* StaticClass<ATDSCharacter>()
	{
		return ATDSCharacter::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(ATDSCharacter);
	ATDSCharacter::~ATDSCharacter() {}
	struct Z_CompiledInDeferFile_FID_UnrealEngineProject_TDS_Source_TDS_TDSCharacter_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealEngineProject_TDS_Source_TDS_TDSCharacter_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_ATDSCharacter, ATDSCharacter::StaticClass, TEXT("ATDSCharacter"), &Z_Registration_Info_UClass_ATDSCharacter, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ATDSCharacter), 2832515801U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealEngineProject_TDS_Source_TDS_TDSCharacter_h_1633944963(TEXT("/Script/TDS"),
		Z_CompiledInDeferFile_FID_UnrealEngineProject_TDS_Source_TDS_TDSCharacter_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealEngineProject_TDS_Source_TDS_TDSCharacter_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
