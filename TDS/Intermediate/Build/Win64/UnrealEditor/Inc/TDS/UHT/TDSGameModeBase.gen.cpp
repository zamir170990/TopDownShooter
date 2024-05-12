// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "TDS/TDSGameModeBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTDSGameModeBase() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	TDS_API UClass* Z_Construct_UClass_ATDSGameModeBase();
	TDS_API UClass* Z_Construct_UClass_ATDSGameModeBase_NoRegister();
	UPackage* Z_Construct_UPackage__Script_TDS();
// End Cross Module References
	void ATDSGameModeBase::StaticRegisterNativesATDSGameModeBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ATDSGameModeBase);
	UClass* Z_Construct_UClass_ATDSGameModeBase_NoRegister()
	{
		return ATDSGameModeBase::StaticClass();
	}
	struct Z_Construct_UClass_ATDSGameModeBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ATDSGameModeBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_TDS,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ATDSGameModeBase_Statics::DependentSingletons) < 16);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ATDSGameModeBase_Statics::Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "TDSGameModeBase.h" },
		{ "ModuleRelativePath", "TDSGameModeBase.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ATDSGameModeBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ATDSGameModeBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_ATDSGameModeBase_Statics::ClassParams = {
		&ATDSGameModeBase::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009002ACu,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ATDSGameModeBase_Statics::Class_MetaDataParams), Z_Construct_UClass_ATDSGameModeBase_Statics::Class_MetaDataParams)
	};
	UClass* Z_Construct_UClass_ATDSGameModeBase()
	{
		if (!Z_Registration_Info_UClass_ATDSGameModeBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ATDSGameModeBase.OuterSingleton, Z_Construct_UClass_ATDSGameModeBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_ATDSGameModeBase.OuterSingleton;
	}
	template<> TDS_API UClass* StaticClass<ATDSGameModeBase>()
	{
		return ATDSGameModeBase::StaticClass();
	}
	ATDSGameModeBase::ATDSGameModeBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(ATDSGameModeBase);
	ATDSGameModeBase::~ATDSGameModeBase() {}
	struct Z_CompiledInDeferFile_FID_UnrealEngineProject_TDS_Source_TDS_TDSGameModeBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealEngineProject_TDS_Source_TDS_TDSGameModeBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_ATDSGameModeBase, ATDSGameModeBase::StaticClass, TEXT("ATDSGameModeBase"), &Z_Registration_Info_UClass_ATDSGameModeBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ATDSGameModeBase), 3485829217U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealEngineProject_TDS_Source_TDS_TDSGameModeBase_h_4060015139(TEXT("/Script/TDS"),
		Z_CompiledInDeferFile_FID_UnrealEngineProject_TDS_Source_TDS_TDSGameModeBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealEngineProject_TDS_Source_TDS_TDSGameModeBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
