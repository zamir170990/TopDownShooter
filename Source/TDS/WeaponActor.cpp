
#include "WeaponActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/DamageType.h"
#include "CTypes.h"
#include "TDSCharacter.h"

AWeaponActor::AWeaponActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;

	SkeletalMeshWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMeshWeapon->SetGenerateOverlapEvents(false);
	SkeletalMeshWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	SkeletalMeshWeapon->SetupAttachment(RootComponent);

	StaticMeshWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh "));
	StaticMeshWeapon->SetGenerateOverlapEvents(false);
	StaticMeshWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	StaticMeshWeapon->SetupAttachment(RootComponent);

	ShootLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("ShootLocation"));
	ShootLocation -> SetupAttachment(RootComponent);
	DropMagazine = CreateDefaultSubobject<UArrowComponent>(TEXT("DropMagazine"));
	DropMagazine->SetupAttachment(RootComponent);
}

void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	WeaponInit();
}

void AWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FireTick(DeltaTime);
	ReloadTick(DeltaTime);
	DispersionTick(DeltaTime);
}

void AWeaponActor::FireTick(float DeltaTime)
{
	if (GetWeaponRound() > 0)
	{
		if (WeaponFiring)
			if (FireTimer < 0.f)
			{
				if (!WeaponReloading)
					Fire();
			}
			else
				FireTimer -= DeltaTime;
	}
	else
	{
		if (!WeaponReloading)
		{
			InitReload();
		}
	}
}

void AWeaponActor::ReloadTick(float DeltaTime)
{
	if (WeaponReloading)
	{
		if (ReloadTimer < 0.0f)
		{
			FinishReload();
		}
		else
		{
			ReloadTimer -= DeltaTime;
		}
	}
}

void AWeaponActor::DispersionTick(float DeltaTime)
{
	if (!WeaponReloading)
	{
		if (!WeaponFiring)
		{
			if (ShouldReduceDispersion)
				CurrentDispersion = CurrentDispersion - CurrentDispersionReduction;
			else
				CurrentDispersion = CurrentDispersion + CurrentDispersionReduction;
		}

		if (CurrentDispersion < CurrentDispersionMin)
		{

			CurrentDispersion = CurrentDispersionMin;

		}
		else
		{
			if (CurrentDispersion > CurrentDispersionMax)
			{
				CurrentDispersion = CurrentDispersionMax;
			}
		}
	}
	if (ShowDebug)
		UE_LOG(LogTemp, Warning, TEXT("Dispersion: MAX = %f. MIN = %f. Current = %f"), CurrentDispersionMax, CurrentDispersionMin, CurrentDispersion);
}

void AWeaponActor::WeaponInit()
{
	if (SkeletalMeshWeapon && !SkeletalMeshWeapon->SkeletalMesh)
	{
		SkeletalMeshWeapon->DestroyComponent(true);
	}

	if (StaticMeshWeapon && !StaticMeshWeapon->GetStaticMesh())
	{
		StaticMeshWeapon->DestroyComponent();
	}

	UpdateStateWeapon(EMovementState::Run_State);
}

void AWeaponActor::SetWeaponStateFire(bool bIsFire)
{
	if (CheckWeaponCanFire())
		WeaponFiring = bIsFire;
	else
		WeaponFiring = false;
	FireTimer = 0.01f;//!!!!!
}

bool AWeaponActor::CheckWeaponCanFire()
{
	return !BlockFire;
}

FProjectileInfo AWeaponActor::GetProjectile()
{
	return WeaponSetting.ProjectileSetting;
}

void AWeaponActor::Fire()
{
	FireTimer = WeaponSetting.RateOfFire;
	WeaponInfo.Round = WeaponInfo.Round - 1;
	ChangeDispersionByShot();

	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), WeaponSetting.SoundFireWeapon, ShootLocation->GetComponentLocation());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponSetting.EffectFireWeapon, ShootLocation->GetComponentTransform());

	int8 NumberProjectile = GetNumberProjectileByShot();

	if (ShootLocation)
	{
		FVector SpawnLocation = ShootLocation->GetComponentLocation();
		FRotator SpawnRotation = ShootLocation->GetComponentRotation();
		FProjectileInfo ProjectileInfo = GetProjectile();
		FireShellBullets();
		FVector EndLocation;

		for (int8 i = 0; i < NumberProjectile; i++)
		{
		    EndLocation = GetFireEndLocation();

			if (ProjectileInfo.Projectile)
			{
			    FVector Dir = EndLocation - SpawnLocation;
			    Dir.Normalize();
			    FMatrix myMatrix(Dir, FVector(0, 1, 0), FVector(0, 0, 1), FVector::ZeroVector);
			    SpawnRotation = myMatrix.Rotator();

				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnParams.Owner = GetOwner();
				SpawnParams.Instigator = GetInstigator();

				AProjectileDefault* myProjectile = Cast<AProjectileDefault>(GetWorld()->SpawnActor(ProjectileInfo.Projectile, & SpawnLocation, &SpawnRotation, SpawnParams));
				if (myProjectile)
				{
					myProjectile->InitProjectile(WeaponSetting.ProjectileSetting);
				}
			}
			else
			{
				FHitResult Hit;
				TArray<AActor*> ActorsToIgnore;
				ActorsToIgnore.Add(this);

				FCollisionQueryParams Params;
				Params.AddIgnoredActors(ActorsToIgnore);
				Params.bTraceComplex = true;
				Params.bReturnPhysicalMaterial = true;

				bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, SpawnLocation, EndLocation, ECC_Visibility, Params);

				if (bHit)
				{
					EPhysicalSurface mySurfaceType = UGameplayStatics::GetSurfaceType(Hit);
					if (WeaponSetting.ProjectileSetting.HitDecals.Contains(mySurfaceType))
					{
						UMaterialInterface* myMaterial = WeaponSetting.ProjectileSetting.HitDecals[mySurfaceType];

						if (myMaterial && Hit.GetComponent())
						{
							UGameplayStatics::SpawnDecalAttached(myMaterial, FVector(20.0), Hit.GetComponent(), NAME_None, Hit.ImpactPoint, Hit.ImpactNormal.Rotation(), EAttachLocation::KeepWorldPosition, 10.0f);
						}
					}
				}
				if (WeaponSetting.ProjectileSetting.HitSound)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponSetting.ProjectileSetting.HitSound, Hit.ImpactPoint);
				}
				UGameplayStatics::ApplyDamage(Hit.GetActor(), WeaponSetting.ProjectileSetting.ProjectileDamage, GetInstigatorController(), this, NULL);
			}
		}
	}
}

void AWeaponActor::ApplyDamage(const FHitResult & HitResult)
{
	if (HitResult.GetActor() != nullptr)
	{
		const float LocalDamageAmount = 50.f;
		const TSubclassOf<UDamageType> LocalDamageType = UDamageType::StaticClass();
		const FVector ShotDirection = GetActorForwardVector();
		UGameplayStatics::ApplyPointDamage(HitResult.GetActor(), LocalDamageAmount, ShotDirection, HitResult, nullptr, this, LocalDamageType);
	}
}


void AWeaponActor::UpdateStateWeapon(EMovementState NewMovementState)
{
	BlockFire = false;

	switch (NewMovementState)
	{
	case EMovementState::Aim_State:

		CurrentDispersionMax = WeaponSetting.DispersionWeapon.Aim_StateDispersionAimMax;
		CurrentDispersionMin = WeaponSetting.DispersionWeapon.Aim_StateDispersionAimMin;
		CurrentDispersionRecoil = WeaponSetting.DispersionWeapon.Aim_StateDispersionAimRecoil;
		CurrentDispersionReduction = WeaponSetting.DispersionWeapon.Aim_StateDispersionReduction;
		break;
	case EMovementState::FireWalk_State:

		CurrentDispersionMax = WeaponSetting.DispersionWeapon.AimWalk_StateDispersionAimMax;
		CurrentDispersionMin = WeaponSetting.DispersionWeapon.AimWalk_StateDispersionAimMin;
		CurrentDispersionRecoil = WeaponSetting.DispersionWeapon.AimWalk_StateDispersionAimRecoil;
		CurrentDispersionReduction = WeaponSetting.DispersionWeapon.Aim_StateDispersionReduction;
		break;
	case EMovementState::Walk_State:

		CurrentDispersionMax = WeaponSetting.DispersionWeapon.Walk_StateDispersionAimMax;
		CurrentDispersionMin = WeaponSetting.DispersionWeapon.Walk_StateDispersionAimMin;
		CurrentDispersionRecoil = WeaponSetting.DispersionWeapon.Walk_StateDispersionAimRecoil;
		CurrentDispersionReduction = WeaponSetting.DispersionWeapon.Aim_StateDispersionReduction;
		break;
	case EMovementState::Run_State:

		CurrentDispersionMax = WeaponSetting.DispersionWeapon.Run_StateDispersionAimMax;
		CurrentDispersionMin = WeaponSetting.DispersionWeapon.Run_StateDispersionAimMin;
		CurrentDispersionRecoil = WeaponSetting.DispersionWeapon.Run_StateDispersionAimRecoil;
		CurrentDispersionReduction = WeaponSetting.DispersionWeapon.Aim_StateDispersionReduction;
		break;
	case EMovementState::SprintRun_State:
		BlockFire = true;
		SetWeaponStateFire(false);

		break;
	default:
		break;
	}
}

void AWeaponActor::ChangeDispersionByShot()
{
	CurrentDispersion = CurrentDispersion + CurrentDispersionRecoil;
}

float AWeaponActor::GetCurrentDispersion() const
{
	float Result = CurrentDispersion;
	return Result;
}

FVector AWeaponActor::ApplyDispersionToShoot(FVector DirectionShoot) const
{
	return FMath::VRandCone(DirectionShoot, GetCurrentDispersion() * PI / 180.f);
}

FVector AWeaponActor::GetFireEndLocation() const
{
	bool bShootDirection = false;
	FVector EndLocation = FVector(0.f);
	FVector tmpV = (ShootLocation->GetComponentLocation() - ShootEndLocation);

	if (tmpV.Size() > SizeVectorToChangeShootDirectionLogic)
	{
		EndLocation = ShootLocation->GetComponentLocation() + ApplyDispersionToShoot((ShootLocation->GetComponentLocation() - ShootEndLocation).GetSafeNormal()) * -20000.0f;
	}
	else
	{
		EndLocation = ShootLocation->GetComponentLocation() + ApplyDispersionToShoot(ShootLocation->GetForwardVector()) * 20000.0f;
	}
	return EndLocation;
}

int8 AWeaponActor::GetNumberProjectileByShot() const
{
	return WeaponSetting.NumberProjectileByShot;
}

int32 AWeaponActor::GetWeaponRound()
{
	return WeaponInfo.Round;
}

void AWeaponActor::InitReload()
{
	if(WeaponSetting.AnimCharReload)
	{
		OnWeaponReloadStart.Broadcast(WeaponSetting.AnimCharReload);
	}
	TimeReloadMagazineDrop();
	WeaponReloading = true;
	ReloadTimer = WeaponSetting.ReloadTime;
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), WeaponSetting.SoundReloadWeapon, GetActorLocation());
}

void AWeaponActor::FinishReload()
{
	WeaponReloading = false;
	WeaponInfo.Round = WeaponSetting.MaxRound;
	OnWeaponReloadEnd.Broadcast();
}

void AWeaponActor::TimeReloadMagazineDrop()
{
	if (WeaponSetting.MagazineDrop)
	{
		FTimerHandle UnusedHandle;
		GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &AWeaponActor::ReloadMagazineDrop, 2.0f, false);
	}
}

void AWeaponActor::ReloadMagazineDrop()
{
	if (WeaponSetting.MagazineDrop)
	{
		FTransform ShootTransform = DropMagazine->GetComponentTransform();
		AStaticMeshActor* DroppedMagazine = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(),ShootTransform.GetLocation(),ShootTransform.GetRotation().Rotator());

		if (DroppedMagazine)
		{
			UStaticMeshComponent* MeshComponent = DroppedMagazine->GetStaticMeshComponent();
			MeshComponent->SetMobility(EComponentMobility::Movable);
			MeshComponent->SetStaticMesh(WeaponSetting.MagazineDrop);
			MeshComponent->SetSimulatePhysics(true);
			FVector Impulse = ShootTransform.GetRotation().GetForwardVector() * -100.0f;
			MeshComponent->AddImpulse(Impulse);
			DroppedMagazine->SetLifeSpan(10.0f);
			MeshComponent->BodyInstance.SetCollisionProfileName(TEXT("NoCollision"));

		}
	}
}

void AWeaponActor::FireShellBullets()
{
	if (WeaponSetting.ShellBullets)
	{
		FTransform ShootTransform = DropMagazine->GetComponentTransform();
		AStaticMeshActor* ShellBullets = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), ShootTransform.GetLocation(), ShootTransform.GetRotation().Rotator());

		if (ShellBullets)
		{
			UStaticMeshComponent* MeshComponent = ShellBullets->GetStaticMeshComponent();
			MeshComponent->SetMobility(EComponentMobility::Movable);
			MeshComponent->SetStaticMesh(WeaponSetting.ShellBullets);
			MeshComponent->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			MeshComponent->SetSimulatePhysics(true);
			ShellBullets->SetLifeSpan(10.0f);
			FVector RightVector = ShootTransform.GetRotation().GetRightVector();
			float RandomSpeed = FMath::RandRange(25.0f, 50.0f);
			MeshComponent->AddImpulse(RightVector * RandomSpeed);
			FRotator RandomRotation = FRotator(
				FMath::RandRange(-180.0f, 180.0f), 
				FMath::RandRange(-180.0f, 180.0f),
				FMath::RandRange(-180.0f, 180.0f)  
			);
			MeshComponent->SetWorldRotation(RandomRotation);
		}
	}
}



//void AWeaponActor::Overlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& HitResult)
//{
//
//    ATDSCharacter* OtherCharacter = Cast<ATDSCharacter>(OtherActor);
//    if (OtherCharacter)
//    {
//        OtherCharacter->WeaponActor = this;
//
//        USkeletalMeshComponent* CharacterMesh = OtherCharacter->GetMesh();
//        if (CharacterMesh)
//        {
//            bIsWeapon = false;
//            FName SocketName = TEXT("Bag");
//
//            WeaponStaticMesh->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
//        }
//    }
//
//}


