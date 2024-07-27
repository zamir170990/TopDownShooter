// Fill out your copyright notice in the Description page of Project Settings.


#include "TDSHealthComponent.h"
#include "TDS_StateEffect.h"


// Sets default values for this component's properties
UTDSHealthComponent::UTDSHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTDSHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UTDSHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UTDSHealthComponent::GetCurrentHealth()
{
	return Health;
}

void UTDSHealthComponent::SetCurrentHealth(float NewHealth)
{
	Health = NewHealth;
}

void UTDSHealthComponent::ChangeHealthValue(float ChangeValue)
{
	if(!bIsImmortal)
	{
		ChangeValue = ChangeValue * CoefDamage;

		Health += ChangeValue;

		if (Health > 100.0f)
		{
			Health = 100.0f;
		}
		else
		{
			if (Health < 0.0f)
			{
				OnDead.Broadcast();
			}
		}

		OnHealthChange.Broadcast(Health, ChangeValue);
	}
	
}

void UTDSHealthComponent::ChangeHealthValueImmortal(float ChangeValue)
{
	ChangeValue = ChangeValue * CoefImmortal;

	Health += ChangeValue;

	if (Health > 100.0f)
	{
		Health = 100.0f;
	}
	else
	{
		if (Health < 0.0f)
		{
			OnDead.Broadcast();
		}
	}

	OnHealthChange.Broadcast(Health, ChangeValue);
	bIsImmortal = true;
}

void UTDSHealthComponent::ChangeHealthValueEndImmortal(float ChangeValue)
{
	bIsImmortal = false;
}

