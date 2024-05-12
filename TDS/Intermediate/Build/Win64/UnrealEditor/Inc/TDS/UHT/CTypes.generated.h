// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "CTypes.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef TDS_CTypes_generated_h
#error "CTypes.generated.h already included, missing '#pragma once' in CTypes.h"
#endif
#define TDS_CTypes_generated_h

#define FID_UnrealEngineProject_TDS_Source_TDS_CTypes_h_18_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FCharacterSpeed_Statics; \
	TDS_API static class UScriptStruct* StaticStruct();


template<> TDS_API UScriptStruct* StaticStruct<struct FCharacterSpeed>();

#define FID_UnrealEngineProject_TDS_Source_TDS_CTypes_h_31_SPARSE_DATA
#define FID_UnrealEngineProject_TDS_Source_TDS_CTypes_h_31_SPARSE_DATA_PROPERTY_ACCESSORS
#define FID_UnrealEngineProject_TDS_Source_TDS_CTypes_h_31_EDITOR_ONLY_SPARSE_DATA_PROPERTY_ACCESSORS
#define FID_UnrealEngineProject_TDS_Source_TDS_CTypes_h_31_RPC_WRAPPERS_NO_PURE_DECLS
#define FID_UnrealEngineProject_TDS_Source_TDS_CTypes_h_31_ACCESSORS
#define FID_UnrealEngineProject_TDS_Source_TDS_CTypes_h_31_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUCTypes(); \
	friend struct Z_Construct_UClass_UCTypes_Statics; \
public: \
	DECLARE_CLASS(UCTypes, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/TDS"), NO_API) \
	DECLARE_SERIALIZER(UCTypes)


#define FID_UnrealEngineProject_TDS_Source_TDS_CTypes_h_31_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UCTypes(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UCTypes(UCTypes&&); \
	NO_API UCTypes(const UCTypes&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UCTypes); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UCTypes); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UCTypes) \
	NO_API virtual ~UCTypes();


#define FID_UnrealEngineProject_TDS_Source_TDS_CTypes_h_28_PROLOG
#define FID_UnrealEngineProject_TDS_Source_TDS_CTypes_h_31_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_UnrealEngineProject_TDS_Source_TDS_CTypes_h_31_SPARSE_DATA \
	FID_UnrealEngineProject_TDS_Source_TDS_CTypes_h_31_SPARSE_DATA_PROPERTY_ACCESSORS \
	FID_UnrealEngineProject_TDS_Source_TDS_CTypes_h_31_EDITOR_ONLY_SPARSE_DATA_PROPERTY_ACCESSORS \
	FID_UnrealEngineProject_TDS_Source_TDS_CTypes_h_31_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_UnrealEngineProject_TDS_Source_TDS_CTypes_h_31_ACCESSORS \
	FID_UnrealEngineProject_TDS_Source_TDS_CTypes_h_31_INCLASS_NO_PURE_DECLS \
	FID_UnrealEngineProject_TDS_Source_TDS_CTypes_h_31_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> TDS_API UClass* StaticClass<class UCTypes>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UnrealEngineProject_TDS_Source_TDS_CTypes_h


#define FOREACH_ENUM_EMOVEMENTSTATE(op) \
	op(EMovementState::Aim_State) \
	op(EMovementState::Walk_State) \
	op(EMovementState::Run_State) 

enum class EMovementState : uint8;
template<> struct TIsUEnumClass<EMovementState> { enum { Value = true }; };
template<> TDS_API UEnum* StaticEnum<EMovementState>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
