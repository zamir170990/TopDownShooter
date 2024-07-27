// Fill out your copyright notice in the Description page of Project Settings.


#include "TDS_StateEffect.h"
#include "TDSHealthComponent.h"
#include "TDS_IGameActor.h"
//#include "TDSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"



bool UTDS_StateEffect::InitObject(AActor* Actor)
{
	myActor = Actor;

	ITDS_IGameActor* myInterface = Cast<ITDS_IGameActor>(myActor);
	if (myInterface)
	{
		myInterface->AddEffect(this);
	}

	return true;
}

void UTDS_StateEffect::DestroyObject()
{
	ITDS_IGameActor* myInterface = Cast<ITDS_IGameActor>(myActor);
	if (myInterface)
	{
		myInterface->RemoveEffect(this);
	}

	myActor = nullptr;
	if (this && this->IsValidLowLevel())
	{
		this->ConditionalBeginDestroy();
	}
}

void UTDS_StateEffect::DestroyEffectVisual()
{
	ITDS_IGameActor* myInterface = Cast<ITDS_IGameActor>(myActor);
	if (myInterface)
	{
		myInterface->RemoveEffect(this);
	}
}

bool UTDS_StateEffect_ExecuteOnce::InitObject(AActor* Actor)
{
	Super::InitObject(Actor);
	ExecuteOnce();
	return true;
}

void UTDS_StateEffect_ExecuteOnce::DestroyObject()
{
	Super::DestroyObject();
}

void UTDS_StateEffect_ExecuteOnce::ExecuteOnce()
{
	if (myActor)
	{
		UTDSHealthComponent* myHealthComp = Cast<UTDSHealthComponent>(myActor->GetComponentByClass(UTDSHealthComponent::StaticClass()));
		if (myHealthComp)
		{
			myHealthComp->ChangeHealthValue(Power);
		}
	}

	DestroyObject();
}

bool UTDS_StateEffect_ExecuteTimer::InitObject(AActor* Actor)
{
	Super::InitObject(Actor);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_EffectTimer, this, &UTDS_StateEffect_ExecuteTimer::DestroyObject, Timer, false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_ExecuteTimer, this, &UTDS_StateEffect_ExecuteTimer::Execute, RateTime, true);

	if (ParticleEffect)
	{
		FName NameBoneToAttached;
		FVector Loc = FVector(0);

		ParticleEmitter = UGameplayStatics::SpawnEmitterAttached(ParticleEffect, myActor->GetRootComponent(), NameBoneToAttached, Loc, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false);
	}

	return true;
}

void UTDS_StateEffect_ExecuteTimer::DestroyObject()
{
	ParticleEmitter->DestroyComponent();
	ParticleEmitter = nullptr;
	Super::DestroyObject();
}

void UTDS_StateEffect_ExecuteTimer::Execute()
{
	if (myActor)
	{
		//UGameplayStatics::ApplyDamage(myActor,Power,nullptr,nullptr,nullptr);	
		UTDSHealthComponent* myHealthComp = Cast<UTDSHealthComponent>(myActor->GetComponentByClass(UTDSHealthComponent::StaticClass()));
		if (myHealthComp)
		{
			myHealthComp->ChangeHealthValue(Power);
		}
	}
}

bool UTDS_StateEffect_ExecuteImmortal::InitObject(AActor* Actor)
{
	Super::InitObject(Actor);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_EffectTimerImmortal, this, &UTDS_StateEffect_ExecuteImmortal::DestroyObject, TimerImmortal, false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_ExecuteTimerImmortal, this, &UTDS_StateEffect_ExecuteImmortal::Execute, TimerImmortalRate, true);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_EffectTimerEnd, this, &UTDS_StateEffect_ExecuteImmortal::DestroyEffectVisual, TimerEndEffect, false);

	if (ParticleEffect)
	{
		FName NameBoneToAttached;
		FVector Loc;
		Loc = FVector(0.0f, 0.0f, -100.0f);

		ParticleEmitter = UGameplayStatics::SpawnEmitterAttached(ParticleEffect, myActor->GetRootComponent(), NameBoneToAttached, Loc, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false);
	}
	return true;
}

void UTDS_StateEffect_ExecuteImmortal::DestroyObject()
{
	if (myActor)
	{
		UTDSHealthComponent* myHealthComp = Cast<UTDSHealthComponent>(myActor->GetComponentByClass(UTDSHealthComponent::StaticClass()));
		if (myHealthComp)
		{
			myHealthComp->ChangeHealthValueEndImmortal(bIsImmortal);
		}
	}
	Super::DestroyObject();
}

void UTDS_StateEffect_ExecuteImmortal::DestroyEffectVisual()
{
	ParticleEmitter->DestroyComponent();
	ParticleEmitter = nullptr;
	Super::DestroyEffectVisual();
}

void UTDS_StateEffect_ExecuteImmortal::Execute()
{
	if (EffectSound)
	{
    	UGameplayStatics::PlaySoundAtLocation(GetWorld(), EffectSound,FVector(0));
	}
	if (myActor)
	{
	    UTDSHealthComponent* myHealthComp = Cast<UTDSHealthComponent>(myActor->GetComponentByClass(UTDSHealthComponent::StaticClass()));
		if (myHealthComp)
		{
			myHealthComp->ChangeHealthValueImmortal(bIsImmortal);
		}
	}
}

//STUN

bool UTDS_StateEffect_ExecuteStun::InitObject(AActor* Actor)
{
	Super::InitObject(Actor);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_EffectTimerStun, this, &UTDS_StateEffect_ExecuteStun::DestroyObject, TimerStun, false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_ExecuteTimerStun, this, &UTDS_StateEffect_ExecuteStun::Execute, TimerStunRate, true);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_EffectTimerEnd, this, &UTDS_StateEffect_ExecuteStun::DestroyEffectVisual, TimerEndEffect, false);

	if (ParticleEffect)
	{
		FName NameBoneToAttached;
		FVector Loc;
		Loc = FVector(0.0f, 0.0f, -100.0f);

		ParticleEmitter = UGameplayStatics::SpawnEmitterAttached(ParticleEffect, myActor->GetRootComponent(), NameBoneToAttached, Loc, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false);
	}
	return true;
}

void UTDS_StateEffect_ExecuteStun::DestroyObject()
{
	//ATDSCharacter* myCharacter;
	//if (myCharacter)
	//{
	//	myCharacter->StunEffectEnd();
	//}
	Super::DestroyObject();
}

void UTDS_StateEffect_ExecuteStun::DestroyEffectVisual()
{
	ParticleEmitter->DestroyComponent();
	ParticleEmitter = nullptr;
	Super::DestroyEffectVisual();
}

void UTDS_StateEffect_ExecuteStun::Execute()
{
	if (EffectSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EffectSound, FVector(0));
	}
	/*ATDSCharacter* myCharacter;
	if (myCharacter)
	{
		myCharacter->StunEffect();
	}*/
}