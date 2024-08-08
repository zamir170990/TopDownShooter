
#include "WeaponActor.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/StaticMeshActor.h"
#include "InventoryComponent.h"

int32 DebugWeaponShow = 0;
FAutoConsoleVariableRef CVarWeaponShow(
	TEXT("TPS.DebugWeapon"),
	DebugWeaponShow,
	TEXT("Draw Debug for Weapon"),
	ECVF_Cheat);

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
	/*DropMagazine = CreateDefaultSubobject<UArrowComponent>(TEXT("DropMagazine"));
	DropMagazine->SetupAttachment(RootComponent);*/
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
	ClipDropTick(DeltaTime);
	ShellDropTick(DeltaTime);
}

void AWeaponActor::FireTick(float DeltaTime)
{
	if (WeaponFiring && GetWeaponRound() > 0 && !WeaponReloading)
	{
		if (FireTimer < 0.f)
		{
			Fire();
		}
		else
			FireTimer -= DeltaTime;
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
}

void AWeaponActor::ClipDropTick(float DeltaTime)
{
	if (DropClipFlag)
	{
		if (DropClipTimer < 0.0f)
		{
			DropClipFlag = false;
			InitDropMesh(WeaponSetting.ClipDropMesh.DropMesh, WeaponSetting.ClipDropMesh.DropMeshOffset, WeaponSetting.ClipDropMesh.DropMeshImpulseDir, WeaponSetting.ClipDropMesh.DropMeshLifeTime, WeaponSetting.ClipDropMesh.ImpulseRandomDispersion, WeaponSetting.ClipDropMesh.PowerImpulse, WeaponSetting.ClipDropMesh.CustomMass);
		}
		else
			DropClipTimer -= DeltaTime;
	}
}

void AWeaponActor::ShellDropTick(float DeltaTime)
{
	if (DropShellFlag)
	{
		if (DropShellTimer < 0.0f)
		{
			DropShellFlag = false;
			InitDropMesh(WeaponSetting.ShellBullets.DropMesh, WeaponSetting.ShellBullets.DropMeshOffset, WeaponSetting.ShellBullets.DropMeshImpulseDir, WeaponSetting.ShellBullets.DropMeshLifeTime, WeaponSetting.ShellBullets.ImpulseRandomDispersion, WeaponSetting.ShellBullets.PowerImpulse, WeaponSetting.ShellBullets.CustomMass);
		}
		else
			DropShellTimer -= DeltaTime;
	}
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
	UAnimMontage* AnimToPlay = nullptr;
	if (WeaponAiming)
		AnimToPlay = WeaponSetting.AnimWeaponInfo.AnimCharFireAim;
	else
		AnimToPlay = WeaponSetting.AnimWeaponInfo.AnimCharFire;

	if (WeaponSetting.AnimWeaponInfo.AnimWeaponFire
		&& SkeletalMeshWeapon
		&& SkeletalMeshWeapon->GetAnimInstance())
	{
		SkeletalMeshWeapon->GetAnimInstance()->Montage_Play(WeaponSetting.AnimWeaponInfo.AnimWeaponFire);
	}

	if (WeaponSetting.ShellBullets.DropMesh)
	{
		if (WeaponSetting.ShellBullets.DropMeshTime < 0.0f)
		{
			InitDropMesh(WeaponSetting.ShellBullets.DropMesh, WeaponSetting.ShellBullets.DropMeshOffset, WeaponSetting.ShellBullets.DropMeshImpulseDir, WeaponSetting.ShellBullets.DropMeshLifeTime, WeaponSetting.ShellBullets.ImpulseRandomDispersion, WeaponSetting.ShellBullets.PowerImpulse, WeaponSetting.ShellBullets.CustomMass);
		}
		else
		{
			DropShellFlag = true;
			DropShellTimer = WeaponSetting.ShellBullets.DropMeshTime;
		}
	}


	FireTimer = WeaponSetting.RateOfFire;
	AdditionalWeaponInfo.Round = AdditionalWeaponInfo.Round - 1;
	ChangeDispersionByShot();

	OnWeaponFireStart.Broadcast(AnimToPlay);
	
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), WeaponSetting.SoundFireWeapon, ShootLocation->GetComponentLocation());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponSetting.EffectFireWeapon, ShootLocation->GetComponentTransform());

	int8 NumberProjectile = GetNumberProjectileByShot();

	if (ShootLocation)
	{
		FVector SpawnLocation = ShootLocation->GetComponentLocation();
		FRotator SpawnRotation = ShootLocation->GetComponentRotation();
		FProjectileInfo ProjectileInfo;
		ProjectileInfo = GetProjectile();

		FVector EndLocation;
		for (int8 i = 0; i < NumberProjectile; i++)//Shotgun
		{
			EndLocation = GetFireEndLocation();

			if (ProjectileInfo.Projectile)
			{
				//Projectile Init ballistic fire
				FVector Dir = EndLocation - SpawnLocation;

				Dir.Normalize();

				FMatrix myMatrix(Dir, FVector(0, 1, 0), FVector(0, 0, 1), FVector::ZeroVector);
				SpawnRotation = myMatrix.Rotator();



				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnParams.Owner = GetOwner();
				SpawnParams.Instigator = GetInstigator();

				AProjectileDefault* myProjectile = Cast<AProjectileDefault>(GetWorld()->SpawnActor(ProjectileInfo.Projectile, &SpawnLocation, &SpawnRotation, SpawnParams));
				if (myProjectile)
				{
					myProjectile->InitProjectile(WeaponSetting.ProjectileSetting);
				}
			}
			else
			{
				FHitResult Hit;
				TArray<AActor*> Actors;

				EDrawDebugTrace::Type DebugTrace;
				if (DebugWeaponShow)
				{
					DrawDebugLine(GetWorld(), SpawnLocation, SpawnLocation + ShootLocation->GetForwardVector() * WeaponSetting.DistacneTrace, FColor::Black, false, 5.f, (uint8)'\000', 0.5f);
					DebugTrace = EDrawDebugTrace::ForDuration;
				}
				else
					DebugTrace = EDrawDebugTrace::None;

				UKismetSystemLibrary::LineTraceSingle(GetWorld(), SpawnLocation, EndLocation * WeaponSetting.DistacneTrace,
					ETraceTypeQuery::TraceTypeQuery4, false, Actors, DebugTrace, Hit, true, FLinearColor::Red, FLinearColor::Green, 5.0f);

				if (Hit.GetActor() && Hit.PhysMaterial.IsValid())
				{
					EPhysicalSurface mySurfacetype = UGameplayStatics::GetSurfaceType(Hit);

					if (WeaponSetting.ProjectileSetting.HitDecals.Contains(mySurfacetype))
					{
						UMaterialInterface* myMaterial = WeaponSetting.ProjectileSetting.HitDecals[mySurfacetype];

						if (myMaterial && Hit.GetComponent())
						{
							UGameplayStatics::SpawnDecalAttached(myMaterial, FVector(20.0f), Hit.GetComponent(), NAME_None, Hit.ImpactPoint, Hit.ImpactNormal.Rotation(), EAttachLocation::KeepWorldPosition, 10.0f);
						}
					}
					if (WeaponSetting.ProjectileSetting.HitFXs.Contains(mySurfacetype))
					{
						UParticleSystem* myParticle = WeaponSetting.ProjectileSetting.HitFXs[mySurfacetype];
						if (myParticle)
						{
							UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), myParticle, FTransform(Hit.ImpactNormal.Rotation(), Hit.ImpactPoint, FVector(1.0f)));
						}
					}

					if (WeaponSetting.ProjectileSetting.HitSound)
					{
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponSetting.ProjectileSetting.HitSound, Hit.ImpactPoint);
					}

					UCTypes::AddEffectBySurfaceType(Hit.GetActor(), ProjectileInfo.Effect, mySurfacetype);

					UGameplayStatics::ApplyPointDamage(Hit.GetActor(), WeaponSetting.ProjectileSetting.ProjectileDamage, Hit.TraceStart, Hit, GetInstigatorController(), this, NULL);
				}

			}
		}
	}

	if (GetWeaponRound() <= 0 && !WeaponReloading)
	{
		if (CheckCanWeaponReload())
			InitReload();
	}
}

//void AWeaponActor::ApplyDamage(const FHitResult & HitResult)
//{
//	if (HitResult.GetActor() != nullptr)
//	{
//		const float LocalDamageAmount = 50.f;
//		const TSubclassOf<UDamageType> LocalDamageType = UDamageType::StaticClass();
//		const FVector ShotDirection = GetActorForwardVector();
//		UGameplayStatics::ApplyPointDamage(HitResult.GetActor(), LocalDamageAmount, ShotDirection, HitResult, nullptr, this, LocalDamageType);
//	}
//}


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
	case EMovementState::AimWalk_State:

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
	return AdditionalWeaponInfo.Round;
}

void AWeaponActor::InitReload()
{
	WeaponReloading = true;

	ReloadTimer = WeaponSetting.ReloadTime;

	UAnimMontage* AnimToPlay = nullptr;
	if (WeaponAiming)
		AnimToPlay = WeaponSetting.AnimWeaponInfo.AnimCharReloadAim;
	else
		AnimToPlay = WeaponSetting.AnimWeaponInfo.AnimCharReload;

	OnWeaponReloadStart.Broadcast(AnimToPlay);

	UAnimMontage* AnimWeaponToPlay = nullptr;
	if (WeaponAiming)
		AnimWeaponToPlay = WeaponSetting.AnimWeaponInfo.AnimWeaponReloadAim;
	else
		AnimWeaponToPlay = WeaponSetting.AnimWeaponInfo.AnimWeaponReload;

	if (WeaponSetting.AnimWeaponInfo.AnimWeaponReload
		&& SkeletalMeshWeapon
		&& SkeletalMeshWeapon->GetAnimInstance())
	{
		SkeletalMeshWeapon->GetAnimInstance()->Montage_Play(AnimWeaponToPlay);
	}
	if (WeaponSetting.ClipDropMesh.DropMesh)
	{
		DropClipFlag = true;
		DropClipTimer = WeaponSetting.ClipDropMesh.DropMeshTime;
	}
}

void AWeaponActor::FinishReload()
{
	WeaponReloading = false;

	int8 AviableAmmoFromInventory = GetAviableAmmoForReload();
	int8 AmmoNeedTakeFromInv;
	int8 NeedToReload = WeaponSetting.MaxRound - AdditionalWeaponInfo.Round;

	if (NeedToReload > AviableAmmoFromInventory)
	{
		AdditionalWeaponInfo.Round += AviableAmmoFromInventory;
		AmmoNeedTakeFromInv = AviableAmmoFromInventory;
	}
	else
	{
		AdditionalWeaponInfo.Round += NeedToReload;
		AmmoNeedTakeFromInv = NeedToReload;
	}
	OnWeaponReloadEnd.Broadcast(true, -AmmoNeedTakeFromInv);
}

void AWeaponActor::CancelReload()
{
	WeaponReloading = false;
	if (SkeletalMeshWeapon && SkeletalMeshWeapon->GetAnimInstance())
		SkeletalMeshWeapon->GetAnimInstance()->StopAllMontages(0.15f);

	OnWeaponReloadEnd.Broadcast(false, 0);
	DropClipFlag = false;
}

bool AWeaponActor::CheckCanWeaponReload()
{
	bool result = true;
	if (GetOwner())
	{
		UInventoryComponent* MyInv = Cast<UInventoryComponent>(GetOwner()->GetComponentByClass(UInventoryComponent::StaticClass()));
		if (MyInv)
		{
			int8 AviableAmmoForWeapon;
			if (!MyInv->CheckAmmoForWeapon(WeaponSetting.WeaponType, AviableAmmoForWeapon))
			{
				result = false;
				MyInv->OnWeaponNotHaveRound.Broadcast(MyInv->GetWeaponIndexSlotByName(IdWeaponName));
			}
			else
			{
				MyInv->OnWeaponHaveRound.Broadcast(MyInv->GetWeaponIndexSlotByName(IdWeaponName));
			}
		}
	}
	return result;
}

int8 AWeaponActor::GetAviableAmmoForReload()
{
	int8 AviableAmmoForWeapon = WeaponSetting.MaxRound;
	if (GetOwner())
	{
		UInventoryComponent* MyInv = Cast<UInventoryComponent>(GetOwner()->GetComponentByClass(UInventoryComponent::StaticClass()));
		if (MyInv)
		{
			if (MyInv->CheckAmmoForWeapon(WeaponSetting.WeaponType, AviableAmmoForWeapon))
			{
				AviableAmmoForWeapon = AviableAmmoForWeapon;
			}
		}
	}
	return AviableAmmoForWeapon;
}

void AWeaponActor::InitDropMesh(UStaticMesh* DropMesh, FTransform Offset, FVector DropImpulseDirection, float LifeTimeMesh, float ImpulseRandomDispersion, float PowerImpulse, float CustomMass)
{

	if (DropMesh)
	{
		FTransform Transform;

		FVector LocalDir = this->GetActorForwardVector() * Offset.GetLocation().X + this->GetActorRightVector() * Offset.GetLocation().Y + this->GetActorUpVector() * Offset.GetLocation().Z;

		Transform.SetLocation(GetActorLocation() + LocalDir);
		Transform.SetScale3D(Offset.GetScale3D());

		Transform.SetRotation((GetActorRotation() + Offset.Rotator()).Quaternion());
		AStaticMeshActor* NewActor = nullptr;


		FActorSpawnParameters Param;
		Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		Param.Owner = this;
		NewActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Transform, Param);

		if (NewActor && NewActor->GetStaticMeshComponent())
		{
			NewActor->GetStaticMeshComponent()->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));
			NewActor->GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

			//set parameter for new actor
			NewActor->SetActorTickEnabled(false);
			NewActor->InitialLifeSpan = LifeTimeMesh;

			NewActor->GetStaticMeshComponent()->Mobility = EComponentMobility::Movable;
			NewActor->GetStaticMeshComponent()->SetSimulatePhysics(true);
			NewActor->GetStaticMeshComponent()->SetStaticMesh(DropMesh);

			NewActor->GetStaticMeshComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
			NewActor->GetStaticMeshComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
			NewActor->GetStaticMeshComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);
			NewActor->GetStaticMeshComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Block);
			NewActor->GetStaticMeshComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECollisionResponse::ECR_Block);
			NewActor->GetStaticMeshComponent()->SetCollisionResponseToChannel(ECC_PhysicsBody, ECollisionResponse::ECR_Block);

			if (CustomMass > 0.0f)
			{
				NewActor->GetStaticMeshComponent()->SetMassOverrideInKg(NAME_None, CustomMass, true);
			}

			if (!DropImpulseDirection.IsNearlyZero())
			{
				FVector FinalDir;
				LocalDir = LocalDir + (DropImpulseDirection * 1000.0f);

				if (!FMath::IsNearlyZero(ImpulseRandomDispersion))
					FinalDir += UKismetMathLibrary::RandomUnitVectorInConeInDegrees(LocalDir, ImpulseRandomDispersion);
				FinalDir.GetSafeNormal(0.0001f);

				NewActor->GetStaticMeshComponent()->AddImpulse(FinalDir * PowerImpulse);
			}
		}


	}
}



