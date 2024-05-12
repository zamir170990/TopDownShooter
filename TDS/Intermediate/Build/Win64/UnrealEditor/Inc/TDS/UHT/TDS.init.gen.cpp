// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTDS_init() {}
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_TDS;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_TDS()
	{
		if (!Z_Registration_Info_UPackage__Script_TDS.OuterSingleton)
		{
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/TDS",
				nullptr,
				0,
				PKG_CompiledIn | 0x00000000,
				0xC4CFF2DE,
				0x36E2235D,
				METADATA_PARAMS(0, nullptr)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_TDS.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_TDS.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_TDS(Z_Construct_UPackage__Script_TDS, TEXT("/Script/TDS"), Z_Registration_Info_UPackage__Script_TDS, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0xC4CFF2DE, 0x36E2235D));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
