// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileDefault.h"
#include "ProjectileDefault_Grenade.generated.h"

/**
 * 
 */
UCLASS()
class TDS_API AProjectileDefault_Grenade : public AProjectileDefault
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
public:

	virtual void Tick(float DelataTime) override;
	virtual void BulletCollisionSphereHit(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void ImpactProjectile()override;

	void TimerExplose(float Delatime);
	void Explose();
	
	bool TimerEnabled = false;
	float TimerToExplose = 0.0f;
	float TimeToExplose = 5.0f;


};
