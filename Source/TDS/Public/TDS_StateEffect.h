// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Particles/ParticleSystemComponent.h"
#include "TDS_StateEffect.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TDS_API UTDS_StateEffect : public UObject
{
	GENERATED_BODY()
	
public:
	virtual bool InitObject(AActor* Actor);
	virtual void DestroyObject();
	virtual void DestroyEffectVisual();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	TArray<TEnumAsByte<EPhysicalSurface>> PossibleInteractSurface;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	bool bIsStakable = false;

	AActor* myActor = nullptr;
	
};

UCLASS()
class TDS_API UTDS_StateEffect_ExecuteOnce : public UTDS_StateEffect
{
	GENERATED_BODY()

public:
	bool InitObject(AActor* Actor) override;
	void DestroyObject() override;

	virtual void ExecuteOnce();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Execute Once")
	float Power = 20.0f;
};

UCLASS()
class TDS_API UTDS_StateEffect_ExecuteTimer : public UTDS_StateEffect
{
	GENERATED_BODY()

public:
	bool InitObject(AActor* Actor) override;
	void DestroyObject() override;

	virtual void Execute();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting ExecuteTimer")
	float Power = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting ExecuteTimer")
	float Timer = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting ExecuteTimer")
	float RateTime = 1.0f;

	FTimerHandle TimerHandle_ExecuteTimer;
	FTimerHandle TimerHandle_EffectTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting ExecuteTimer")
	UParticleSystem* ParticleEffect = nullptr;

	UParticleSystemComponent* ParticleEmitter = nullptr;
};

UCLASS()
class TDS_API UTDS_StateEffect_ExecuteImmortal : public UTDS_StateEffect 
{
	 GENERATED_BODY()
public:

	 bool InitObject(AActor* Actor) override;
	 void DestroyObject() override;
	 void DestroyEffectVisual();

	 virtual void Execute();
	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting TimerImmortal")
	 float TimerImmortal = 8.0f;
	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting TimerImmortal")
	 float TimerEndEffect = 8.0f;
	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting TimerImmortal")
	 float TimerImmortalRate = 1.0f;
	 bool bIsImmortal;
	 
	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting TimerImmortal")
	 USoundBase* EffectSound = nullptr;

	 FTimerHandle TimerHandle_ExecuteTimerImmortal;
	 FTimerHandle TimerHandle_EffectTimerImmortal;
	 FTimerHandle TimerHandle_EffectTimerEnd;
	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting TimerImmortal")
	 UParticleSystem* ParticleEffect = nullptr;

	 UParticleSystemComponent* ParticleEmitter = nullptr;
};

