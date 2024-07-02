// Fill out your copyright notice in the Description page of Project Settings.
#include "ProjectileDefault_Grenade.h"
 #include "Kismet/GameplayStatics.h"


void AProjectileDefault_Grenade::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectileDefault_Grenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimerExplose(DeltaTime);
}

void AProjectileDefault_Grenade::TimerExplose(float DeltaTime)
{
	if (TimerEnabled)
	{
		if (TimerToExplose > TimeToExplose)
		{
			Explose();
		}
		else
		{
			TimerToExplose += DeltaTime;
		}
	}
}

void AProjectileDefault_Grenade::BulletCollisionSphereHit(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::BulletCollisionSphereHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}

void AProjectileDefault_Grenade::ImpactProjectile()
{
	//Init Grenade
	TimerEnabled = true;
}

void AProjectileDefault_Grenade::Explose()
{
	TimerEnabled = false;
	if (ProjectileSetting.ExploseFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileSetting.ExploseFX, GetActorLocation(), GetActorRotation(), FVector(1.0f));
	}
	if (ProjectileSetting.ExploseSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileSetting.ExploseSound, GetActorLocation());
	}
	DebugExplosionRadius();
	TArray<AActor*> IgnoredActor;
	UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(),
		ProjectileSetting.ExploseMaxDamage,
		ProjectileSetting.ExploseMaxDamage * ProjectileSetting.DamageFalloffCoefficient,
		GetActorLocation(),
		ProjectileSetting.MaxDamageDistance,
		ProjectileSetting.DamageFalloffDistance,
		5,
		NULL, IgnoredActor, nullptr, nullptr);

	this->Destroy();
}
void AProjectileDefault_Grenade::DebugExplosionRadius()
{
		float HalfDamageRadius = ProjectileSetting.MaxDamageDistance + ((ProjectileSetting.DamageFalloffDistance - ProjectileSetting.MaxDamageDistance) / 2);
		DrawDebugSphere(GetWorld(), GetActorLocation(), ProjectileSetting.MaxDamageDistance, 12, FColor::Red, false, 10.0f);
		DrawDebugSphere(GetWorld(), GetActorLocation(), HalfDamageRadius, 12, FColor::Yellow, false, 10.0f);
		DrawDebugSphere(GetWorld(), GetActorLocation(), ProjectileSetting.DamageFalloffDistance, 12, FColor::Green, false, 10.0f);
}
