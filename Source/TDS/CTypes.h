
#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "CTypes.generated.h"

UENUM(BlueprintType)
enum class EMovementState : uint8
{
    Aim_State UMETA(DisplayName="Aim State"),
    Walk_State UMETA(DisplayName = "Walk State"),
    Run_State UMETA(DisplayName = "Run State"),
};

USTRUCT(BlueprintType)
struct FCharacterSpeed
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float AimSpeed = 200.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float WalkSpeed = 300.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float RunSpeed = 600.0f;
};

USTRUCT(BlueprintType)
struct FProjectileInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProgectileSetting")
    TSubclassOf<class AProjectileDefault>Projectile = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProgectileSetting")
    float ProjectileDamage = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProgectileSetting")
    float ProjectileLifeTime = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProgectileSetting")
    float ProjectileInitSpeed = 2000.0f;

    //Hit FX Actor

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProgectileSetting")
    bool bIsLikeBomp = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProgectileSetting")
    float ProjectileMaxRadiusDamage = 200.0f;

};

USTRUCT(BlueprintType)
struct FWeaponDispersion
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion")
    float DispersionAimStart = 0.5f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion")
    float DespersionAimMax = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion")
    float DispersionAimMin = 0.1f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion")
    float DispersionAimShootCoef = 1.0f;
};

USTRUCT(BlueprintType)
struct FWeaponInfo:public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
    TSubclassOf<class AWeaponActor>WeaponClass = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    float RateOfFire = 0.5f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    float ReloadTime = 2.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    int32 MaxRound = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion")
    FWeaponDispersion DispersionWeapon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* SoundFireWeapon = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* SoundReloadWeapon = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
    UParticleSystem* EffectFireWeapon = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    FProjectileInfo ProjectileSetting;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
    float WeaponDamage = 20.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
    float DistanceTrace = 2000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitEffect")
    UDecalComponent* DecalOnHit = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
    UAnimMontage* AnimCharFire = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
    UAnimMontage* AnimCharReload = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    UStaticMesh* MagazineDrop = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    UStaticMesh* SleeveBullets = nullptr;
};

USTRUCT(BlueprintType)
struct FAddicionalWeaponInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Weapon Stats")
    int32 Round = 10;
};

UCLASS()
class TDS_API UCTypes :public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
};