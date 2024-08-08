// Fill out your copyright notice in the Description page of Project Settings.


#include "TDSHealthCharacterComponent.h"



void UTDSHealthCharacterComponent::ChangeHealthValue(float ChangeValue)
{
	float CurrentDamage = ChangeValue * CoefDamage;

	if (Shield > 0.0f && ChangeValue < 0.0f)
	{
		ChangeShieldValue(ChangeValue);

		if (Shield < 0.0f)
		{

		}
	}
	else
	{
		Super::ChangeHealthValue(ChangeValue);
	}
}

float UTDSHealthCharacterComponent::GetCurrentShield()
{
	return Shield;
}

void UTDSHealthCharacterComponent::ChangeShieldValue(float ChangeValue)
{
	Shield += ChangeValue;
	OnShieldChange.Broadcast(Shield, ChangeValue);
	if (Shield > 100.0f)
	{
		Shield = 100.0f;
	}
	else
	{
		if (Shield < 0.0f)
			Shield = 0.0f;
	}

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_CollDownShieldTimer, this, &UTDSHealthCharacterComponent::CoolDownShieldEnd, CoolDownShieldRecoverTime, false);

		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ShieldRecoveryRateTimer);
	}
}

void UTDSHealthCharacterComponent::CoolDownShieldEnd()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_ShieldRecoveryRateTimer, this, &UTDSHealthCharacterComponent::RecoveryShield, ShieldRecoverRate, true);
	}
}

void UTDSHealthCharacterComponent::RecoveryShield()
{
	float tmp = Shield;
	tmp = tmp + ShieldRecoverValue;
	if (tmp > 100.0f)
	{
		Shield = 100.0f;
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ShieldRecoveryRateTimer);
		}
	}
	else
	{
		Shield = tmp;
	}

	OnShieldChange.Broadcast(Shield, ShieldRecoverValue);
}

float UTDSHealthCharacterComponent::GetShieldValue()
{
	return Shield;
}


