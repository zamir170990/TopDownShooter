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
	AActor* myActor = nullptr;

	virtual bool InitObject(AActor* Actor);
	virtual void DestroyObject();
	virtual void DestroyEffectVisual();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	TArray<TEnumAsByte<EPhysicalSurface>> PossibleInteractSurface;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	bool bIsStakable = false;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting ExecuteTimer")
	UParticleSystem* ParticleEffect = nullptr;

	UParticleSystemComponent* ParticleEmitter = nullptr;

	FTimerHandle TimerHandle_ExecuteTimer;
	FTimerHandle TimerHandle_EffectTimer;
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

	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting TimerStun")
	 USoundBase* EffectSound = nullptr;
	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting TimerImmortal")
	 UParticleSystem* ParticleEffect = nullptr;

	 UParticleSystemComponent* ParticleEmitter = nullptr;

	 FTimerHandle TimerHandle_ExecuteTimerImmortal;
	 FTimerHandle TimerHandle_EffectTimerImmortal;
	 FTimerHandle TimerHandle_EffectTimerEnd;
};
//////////////////////////////////////// STUN////////////////////////////////////
UCLASS()
class TDS_API UTDS_StateEffect_ExecuteStun : public UTDS_StateEffect
{
	GENERATED_BODY()
public:

	bool InitObject(AActor* Actor) override;
	void DestroyObject() override;
	void DestroyEffectVisual();

	virtual void Execute();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting TimerStun")
	float TimerStun = 8.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting TimerStun")
	float TimerEndEffect = 8.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting TimerStun")
	float TimerStunRate = 1.0f;

	bool bIsImmortal;

	UPROPERTY(EditDefaultsOnly, Category = "FireAnimation")UAnimMontage* StunAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting TimerStun")
	USoundBase* EffectSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting TimerStun")
	UParticleSystem* ParticleEffectLeft = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting TimerStun")
	UParticleSystem* ParticleEffectRight = nullptr;

	UParticleSystemComponent* ParticleEmitterLeft = nullptr;
	UParticleSystemComponent* ParticleEmitterRight = nullptr;

	FTimerHandle TimerHandle_ExecuteTimerStun;
	FTimerHandle TimerHandle_EffectTimerStun;
	FTimerHandle TimerHandle_EffectTimerEnd;
};
//////////////////////////////////////// Energy ////////////////////////////////////
UCLASS()
class TDS_API UTDS_StateEffect_ExecuteEnergy : public UTDS_StateEffect
{
	GENERATED_BODY()
public:

	bool InitObject(AActor* Actor) override;
	void DestroyObject() override;
	void DestroyEffectVisual();

	virtual void Execute();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting TimerEnergy")
	float TimerEnergy = 8.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting TimerEnergy")
	float TimerEndEffect = 8.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting TimerEnergy")
	float TimerEnergyRate = 1.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageEnergy = 10.0f;

	bool bIsEnergy;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Array Energy")
	TMap<AActor*, bool> OverlappingActorsMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Energy")
	USoundBase* EffectSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Energy")
	UParticleSystem* ParticleEffect = nullptr;

	UParticleSystemComponent* ParticleEmitter = nullptr;

	FTimerHandle TimerHandle_ExecuteTimerEnergy;
	FTimerHandle TimerHandle_EffectTimerEnergy;
	FTimerHandle TimerHandle_EffectTimerEnd;


	UFUNCTION()
	void OnEnergyFieldEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnEnergyFieldOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};