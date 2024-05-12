// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "TDS/TDSPlayerController.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTDSPlayerController() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_APlayerController();
	TDS_API UClass* Z_Construct_UClass_ATDSPlayerController();
	TDS_API UClass* Z_Construct_UClass_ATDSPlayerController_NoRegister();
	UPackage* Z_Construct_UPackage__Script_TDS();
// End Cross Module References
	void ATDSPlayerController::StaticRegisterNativesATDSPlayerController()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ATDSPlayerController);
	UClass* Z_Construct_UClass_ATDSPlayerController_NoRegister()
	{
		return ATDSPlayerController::StaticClass();
	}
	struct Z_Construct_UClass_ATDSPlayerController_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ATDSPlayerController_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_APlayerController,
		(UObject* (*)())Z_Construct_UPackage__Script_TDS,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ATDSPlayerController_Statics::DependentSingletons) < 16);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ATDSPlayerController_Statics::Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "HideCategories", "Collision Rendering Transformation" },
		{ "IncludePath", "TDSPlayerController.h" },
		{ "ModuleRelativePath", "TDSPlayerController.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ATDSPlayerController_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ATDSPlayerController>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_ATDSPlayerController_Statics::ClassParams = {
		&ATDSPlayerController::StaticClass,
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
		0x009002A4u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ATDSPlayerController_Statics::Class_MetaDataParams), Z_Construct_UClass_ATDSPlayerController_Statics::Class_MetaDataParams)
	};
	UClass* Z_Construct_UClass_ATDSPlayerController()
	{
		if (!Z_Registration_Info_UClass_ATDSPlayerController.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ATDSPlayerController.OuterSingleton, Z_Construct_UClass_ATDSPlayerController_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_ATDSPlayerController.OuterSingleton;
	}
	template<> TDS_API UClass* StaticClass<ATDSPlayerController>()
	{
		return ATDSPlayerController::StaticClass();
	}
	ATDSPlayerController::ATDSPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(ATDSPlayerController);
	ATDSPlayerController::~ATDSPlayerController() {}
	struct Z_CompiledInDeferFile_FID_UnrealEngineProject_TDS_Source_TDS_TDSPlayerController_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealEngineProject_TDS_Source_TDS_TDSPlayerController_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_ATDSPlayerController, ATDSPlayerController::StaticClass, TEXT("ATDSPlayerController"), &Z_Registration_Info_UClass_ATDSPlayerController, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ATDSPlayerController), 3459548901U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealEngineProject_TDS_Source_TDS_TDSPlayerController_h_4150799444(TEXT("/Script/TDS"),
		Z_CompiledInDeferFile_FID_UnrealEngineProject_TDS_Source_TDS_TDSPlayerController_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealEngineProject_TDS_Source_TDS_TDSPlayerController_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
