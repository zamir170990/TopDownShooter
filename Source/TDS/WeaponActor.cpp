
#include "WeaponActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Engine/SkeletalMeshSocket.h"
#include "TDSCharacter.h"

void AWeaponActor::Overlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& HitResult)
{

    ATDSCharacter* OtherCharacter = Cast<ATDSCharacter>(OtherActor);
    if (OtherCharacter)
    {
        OtherCharacter->WeaponActor = this;

        USkeletalMeshComponent* CharacterMesh = OtherCharacter->GetMesh();
        if (CharacterMesh)
        {
            bIsWeapon = false;
            FName SocketName = TEXT("Bag");

            WeaponStaticMesh->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
        }
    }

}
void AWeaponActor::AttachToCharacterSocket(ACharacter* Character, FName Weapon)
{

    if (Character && WeaponStaticMesh)
    {
        USkeletalMeshComponent* CharacterMesh = Character->GetMesh();

        if (CharacterMesh)
        {
            WeaponStaticMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
            WeaponStaticMesh->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, Weapon);
        }
    }
}

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
	ShootLocation->SetupAttachment(RootComponent);
}

bool AWeaponActor::IsAttachedToCharacter() const
{
	return GetAttachParentActor() != nullptr;
}

void AWeaponActor::DetachFromCharacter()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
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
}

void AWeaponActor::FireTick(float DeltaTime)
{
	if(GetWeaponRound() > 0)
	{
		if (WeaponFiring)
		{
			if (FireTimer < 0.f)
			{
				if (GetWeaponRound() > 0)
				{
					if (WeaponReloading)
					{
						Fire();
					}
				}
				else
				{
					if (WeaponReloading)
					{
		                FireTimer -= DeltaTime;
					}
				}
			}
		}
	}
	else
	{
	   InitReload();
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
}

void AWeaponActor::SetWeaponStateFire(bool bIsFire)
{
	if (CheckWeaponCanFire())
		WeaponFiring = bIsFire;
	else
		WeaponFiring = false;
}

bool AWeaponActor::CheckWeaponCanFire()
{
	return true;
}

FProjectileInfo AWeaponActor::GetProjectile()
{
	return WeaponSetting.ProjectileSetting;
}

void AWeaponActor::Fire()
{
	FireTimer = WeaponSetting.RateOfFire;

	WeaponInfo.Round = WeaponInfo.Round - 1;

	if (ShootLocation)
	{
		FVector SpawnLocation = ShootLocation->GetComponentLocation();
		FRotator SpawnRotation = ShootLocation->GetComponentRotation();
		FProjectileInfo ProjectileInfo;
		ProjectileInfo = GetProjectile();

		if (ProjectileInfo.Projectile)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.Owner = GetOwner();
			SpawnParams.Instigator = GetInstigator();

			AProjectileDefault* myProjectile = Cast<AProjectileDefault>(GetWorld()->SpawnActor(ProjectileInfo.Projectile, &SpawnLocation, &SpawnRotation, SpawnParams));
			if (myProjectile)
			{
				myProjectile->InitialLifeSpan = 20.0f;
			}
		}
		else
		{

		}
	}
}

void AWeaponActor::UpdateStateWeapon(EMovementState NewMovementState)
{
	ChangeDispersion();
}

void AWeaponActor::ChangeDispersion()
{
}

int32 AWeaponActor::GetWeaponRound()
{
	return WeaponInfo.Round;
}

void AWeaponActor::InitReload()
{
	WeaponReloading = true;

	ReloadTimer = WeaponSetting.ReloadTime;

	//ToDo Anim
}

void AWeaponActor::FinishReload()
{
	WeaponReloading = false;
	WeaponInfo.Round = WeaponSetting.MaxRound;
}




