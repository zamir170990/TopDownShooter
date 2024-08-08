// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TDSHealthComponent.h"
#include "TDSHealthCharacterComponent.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShieldChange, float, Shield, float, Damage);

UCLASS()
class TDS_API UTDSHealthCharacterComponent : public UTDSHealthComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Health")
	FOnShieldChange OnShieldChange;

	FTimerHandle TimerHandle_CollDownShieldTimer;
	FTimerHandle TimerHandle_ShieldRecoveryRateTimer;
protected:

	float Shield = 100.0f;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
	float CoolDownShieldRecoverTime = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
	float ShieldRecoverValue = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
	float ShieldRecoverRate = 0.1f;

	void ChangeHealthValue(float ChangeValue) override;

	float GetCurrentShield();

	void ChangeShieldValue(float ChangeValue);

	void CoolDownShieldEnd();

	void RecoveryShield();

	UFUNCTION(BlueprintCallable)
	float GetShieldValue();
};
