// Fill out your copyright notice in the Description page of Project Settings.


#include "TDS_StateEffect.h"
#include "TDSHealthComponent.h"
#include "TDS_IGameActor.h"
#include "TDSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectileDefault.h"


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
        if(ParticleEmitter)
        {
            ParticleEmitter = UGameplayStatics::SpawnEmitterAttached(ParticleEffect, myActor->GetRootComponent(), NameBoneToAttached, Loc, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false);
        }
    }
    return true;
}

void UTDS_StateEffect_ExecuteTimer::DestroyObject()
{
    if (ParticleEmitter)
    {
        if (ParticleEmitter->IsValidLowLevel())
        {
            ParticleEmitter->DestroyComponent();
        }
        ParticleEmitter = nullptr;
    }
    Super::DestroyObject();
}

void UTDS_StateEffect_ExecuteTimer::Execute()
{
    if (myActor)
    {
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
    if (EffectSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, EffectSound, myActor->GetActorLocation());
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
    if (ParticleEmitter)
    {
        if (ParticleEmitter->IsValidLowLevel())
        {
            ParticleEmitter->DestroyComponent();
        }
        ParticleEmitter = nullptr;
    }
    Super::DestroyEffectVisual();
}

void UTDS_StateEffect_ExecuteImmortal::Execute()
{
    if (myActor)
    {
        UTDSHealthComponent* myHealthComp = Cast<UTDSHealthComponent>(myActor->GetComponentByClass(UTDSHealthComponent::StaticClass()));
        if (myHealthComp)
        {
            myHealthComp->ChangeHealthValueImmortal(bIsImmortal);
        }
    }
}

///////////////////////////// STUN /////////////////////////////////////////////////

bool UTDS_StateEffect_ExecuteStun::InitObject(AActor* Actor)
{
    Super::InitObject(Actor);

    GetWorld()->GetTimerManager().SetTimer(TimerHandle_EffectTimerStun, this, &UTDS_StateEffect_ExecuteStun::DestroyObject, TimerStun, false);
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_ExecuteTimerStun, this, &UTDS_StateEffect_ExecuteStun::Execute, TimerStunRate, false);
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_EffectTimerEnd, this, &UTDS_StateEffect_ExecuteStun::DestroyEffectVisual, TimerEndEffect, false);

    if (ParticleEffectRight,ParticleEffectLeft)
    {
        ATDSCharacter* myCharacter = Cast<ATDSCharacter>(myActor);
       
        ParticleEmitterRight = UGameplayStatics::SpawnEmitterAttached(ParticleEffectRight, myCharacter->GetMesh(), FName("StunRight"), FVector(0), FRotator(90.0f, 0.0f, 0.0f), EAttachLocation::SnapToTarget, false);
        ParticleEmitterLeft = UGameplayStatics::SpawnEmitterAttached(ParticleEffectLeft, myCharacter->GetMesh(), FName("StunLeft"), FVector(0), FRotator(-90.0f, 0.0f, 0.0f), EAttachLocation::SnapToTarget, false);
    }
    if (EffectSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, EffectSound, myActor->GetActorLocation());
    }
    return true;
}

void UTDS_StateEffect_ExecuteStun::DestroyObject()
{
    ATDSCharacter* myCharacter = Cast<ATDSCharacter>(myActor);
    if (myCharacter)
    {
        myCharacter->EnableInput(GetWorld()->GetFirstPlayerController());
        myCharacter->bIsAlive = true;
        if(StunAnim)
        {
            myCharacter->StopAnimMontage(StunAnim);
        }
    }
    Super::DestroyObject();
}

void UTDS_StateEffect_ExecuteStun::DestroyEffectVisual()
{
    
    
        if (ParticleEmitterRight->IsValidLowLevel())
        {
            ParticleEmitterRight->DestroyComponent();
        }
        if (ParticleEmitterLeft->IsValidLowLevel())
        { 
            ParticleEmitterLeft->DestroyComponent();
        }
        ParticleEmitterRight = nullptr;
        ParticleEmitterLeft  = nullptr;
    
    Super::DestroyEffectVisual();
}

void UTDS_StateEffect_ExecuteStun::Execute()
{
    ATDSCharacter* myCharacter = Cast<ATDSCharacter>(myActor);
    if (myCharacter)
    {
        if(StunAnim)
        {
            myCharacter->PlayAnimMontage(StunAnim,1.0,NAME_None);
        }
        myCharacter->DisableInput(GetWorld()->GetFirstPlayerController());
        myCharacter->bIsAlive = false;
    }
}
//////////////////////////////////////// EnergyShield ////////////////////////////////////

bool UTDS_StateEffect_ExecuteEnergy::InitObject(AActor* Actor)
{
    Super::InitObject(Actor);

    GetWorld()->GetTimerManager().SetTimer(TimerHandle_EffectTimerEnergy, this, &UTDS_StateEffect_ExecuteEnergy::DestroyObject, TimerEnergy, false);
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_ExecuteTimerEnergy, this, &UTDS_StateEffect_ExecuteEnergy::Execute, TimerEnergyRate, true);
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_EffectTimerEnd, this, &UTDS_StateEffect_ExecuteEnergy::DestroyEffectVisual, TimerEndEffect, false);

    ATDSCharacter* MyCharacter = Cast<ATDSCharacter>(Actor);
    if (MyCharacter && MyCharacter->EnergyFieldComponent)
    {
        MyCharacter->EnergyFieldComponent->OnComponentBeginOverlap.AddDynamic(this, &UTDS_StateEffect_ExecuteEnergy::OnEnergyFieldOverlap);
        MyCharacter->EnergyFieldComponent->OnComponentEndOverlap.AddDynamic(this, &UTDS_StateEffect_ExecuteEnergy::OnEnergyFieldEndOverlap);
    }
    if (EffectSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, EffectSound, Actor->GetActorLocation());
    }
    return true;
}

void UTDS_StateEffect_ExecuteEnergy::DestroyObject()
{
    Super::DestroyObject();

    GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ExecuteTimerEnergy);
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle_EffectTimerEnergy);

    ATDSCharacter* MyCharacter = Cast<ATDSCharacter>(myActor);
    if (MyCharacter && MyCharacter->EnergyFieldComponent)
    {
        MyCharacter->EnergyFieldComponent->OnComponentBeginOverlap.RemoveDynamic(this, &UTDS_StateEffect_ExecuteEnergy::OnEnergyFieldOverlap);
        MyCharacter->EnergyFieldComponent->OnComponentEndOverlap.RemoveDynamic(this, &UTDS_StateEffect_ExecuteEnergy::OnEnergyFieldEndOverlap);
    }

    
}

void UTDS_StateEffect_ExecuteEnergy::DestroyEffectVisual()
{
    if (ParticleEmitter)
    {
        ParticleEmitter->DestroyComponent();
    }
    ParticleEmitter = nullptr;

    Super::DestroyEffectVisual();
}

void UTDS_StateEffect_ExecuteEnergy::Execute()
{
    if (myActor && ParticleEffect && ParticleEffect->IsValidLowLevel())
    {
        ATDSCharacter* MyCharacter = Cast<ATDSCharacter>(myActor);
        if (MyCharacter && MyCharacter->GetMesh())
        {
            ParticleEmitter = UGameplayStatics::SpawnEmitterAttached(ParticleEffect, MyCharacter->GetMesh(), FName("Spine2"), FVector(0), FRotator(0.0f, 0.0f, 0.0f), EAttachLocation::SnapToTarget, false);
        }
    }
    ATDSCharacter* MyCharacter = Cast<ATDSCharacter>(myActor);
    if (MyCharacter && MyCharacter->EnergyFieldComponent)
    {
        // Перебираем всех акторов в Map и наносим урон тем, кто находится в поле
        for (auto& Elem : OverlappingActorsMap)
        {
            AActor* Actor = Elem.Key;
            bool bIsOverlapping = Elem.Value;

            if (bIsOverlapping)
            {
                UTDSHealthComponent* HealthComp = Cast<UTDSHealthComponent>(Actor->GetComponentByClass(UTDSHealthComponent::StaticClass()));
                if (HealthComp)
                {
                    HealthComp->ChangeHealthValue(-DamageEnergy);
                }
            }
        }
    }
}

void UTDS_StateEffect_ExecuteEnergy::OnEnergyFieldOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ATDSCharacter* MyCharacter = Cast<ATDSCharacter>(myActor);
    if (OtherActor && OtherActor != MyCharacter)
    {
        OverlappingActorsMap.FindOrAdd(OtherActor) = true;
    }
}

void UTDS_StateEffect_ExecuteEnergy::OnEnergyFieldEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ATDSCharacter* MyCharacter = Cast<ATDSCharacter>(myActor);
    if (OtherActor && OtherActor != MyCharacter)
    {
        OverlappingActorsMap.FindOrAdd(OtherActor) = false;
    }
}