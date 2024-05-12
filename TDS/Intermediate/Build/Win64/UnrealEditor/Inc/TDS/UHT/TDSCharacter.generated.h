// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "TDSCharacter.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
enum class EMovementState : uint8;
#ifdef TDS_TDSCharacter_generated_h
#error "TDSCharacter.generated.h already included, missing '#pragma once' in TDSCharacter.h"
#endif
#define TDS_TDSCharacter_generated_h

#define FID_UnrealEngineProject_TDS_Source_TDS_TDSCharacter_h_13_SPARSE_DATA
#define FID_UnrealEngineProject_TDS_Source_TDS_TDSCharacter_h_13_SPARSE_DATA_PROPERTY_ACCESSORS
#define FID_UnrealEngineProject_TDS_Source_TDS_TDSCharacter_h_13_EDITOR_ONLY_SPARSE_DATA_PROPERTY_ACCESSORS
#define FID_UnrealEngineProject_TDS_Source_TDS_TDSCharacter_h_13_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execChangeMovementState); \
	DECLARE_FUNCTION(execCharacterUpdate);


#define FID_UnrealEngineProject_TDS_Source_TDS_TDSCharacter_h_13_ACCESSORS
#define FID_UnrealEngineProject_TDS_Source_TDS_TDSCharacter_h_13_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesATDSCharacter(); \
	friend struct Z_Construct_UClass_ATDSCharacter_Statics; \
public: \
	DECLARE_CLASS(ATDSCharacter, ACharacter, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/TDS"), NO_API) \
	DECLARE_SERIALIZER(ATDSCharacter)


#define FID_UnrealEngineProject_TDS_Source_TDS_TDSCharacter_h_13_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ATDSCharacter(ATDSCharacter&&); \
	NO_API ATDSCharacter(const ATDSCharacter&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ATDSCharacter); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ATDSCharacter); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(ATDSCharacter) \
	NO_API virtual ~ATDSCharacter();


#define FID_UnrealEngineProject_TDS_Source_TDS_TDSCharacter_h_10_PROLOG
#define FID_UnrealEngineProject_TDS_Source_TDS_TDSCharacter_h_13_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_UnrealEngineProject_TDS_Source_TDS_TDSCharacter_h_13_SPARSE_DATA \
	FID_UnrealEngineProject_TDS_Source_TDS_TDSCharacter_h_13_SPARSE_DATA_PROPERTY_ACCESSORS \
	FID_UnrealEngineProject_TDS_Source_TDS_TDSCharacter_h_13_EDITOR_ONLY_SPARSE_DATA_PROPERTY_ACCESSORS \
	FID_UnrealEngineProject_TDS_Source_TDS_TDSCharacter_h_13_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_UnrealEngineProject_TDS_Source_TDS_TDSCharacter_h_13_ACCESSORS \
	FID_UnrealEngineProject_TDS_Source_TDS_TDSCharacter_h_13_INCLASS_NO_PURE_DECLS \
	FID_UnrealEngineProject_TDS_Source_TDS_TDSCharacter_h_13_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> TDS_API UClass* StaticClass<class ATDSCharacter>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UnrealEngineProject_TDS_Source_TDS_TDSCharacter_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
