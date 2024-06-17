// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTypes.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "WeaponActor.h"
#include "Math/UnrealMathUtility.h"
#include "InteractInterface.h"
#include "TDSCharacter.generated.h"

class AWeaponActor;

UCLASS(Blueprintable)
class TDS_API ATDSCharacter : public ACharacter, public IInteractInterface
{
    GENERATED_BODY()

public:
   
    ATDSCharacter();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

   /////////////////////////////////---INPUT---////////////////////////////////////////////

    virtual void SetupPlayerInputComponent(class UInputComponent* NewInputComponent) override;
    void MoveFB(float Value);
    void MoveRL(float Value);
    void Jump();
    void StopJump();
    void Aim();
    void StopAim();
    void Fire();
    void StopFire();
    void Sprint();
    void StopSprint();

    bool bIsSprint;
    bool bIsMove = false;

    /////////////////////////////////---WEAPON---////////////////////////////////////////////

    AWeaponActor* WeaponActor = 0;

    void AttachToAimSocket();
    void AttachToWeaponBagSocket();
    void AttachToWalkWeaponSocket();
    void AttachToFireSocket();

    bool bFire = false;
    bool bIsAim = false;

    AWeaponActor* CurrentWeapon = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demo")
    FName InitWeaponName;
    UDecalComponent* CurrentCursor = nullptr;

    UFUNCTION(BlueprintCallable)
    void AttackCharEvent(bool bIsFiring);
    UFUNCTION(BlueprintCallable)
    void CharacterUpdate();
    UFUNCTION(BlueprintCallable)
    AWeaponActor* GetCurrentWeapon();
    UFUNCTION(BlueprintCallable)
    void InitWeapon(FName IdWeapon);

    UFUNCTION(BlueprintCallable)
    UDecalComponent* GetCursorToWorld();
    UFUNCTION(BlueprintCallable)
    void TryReloadWeapon();

    /////////////////////////////////---ANIMATION---////////////////////////////////////////////

    UPROPERTY(EditDefaultsOnly, Category = "FireAnimation")
    UAnimMontage* FireAnimation;
    UPROPERTY(EditDefaultsOnly, Category = "FireAnimation")
    UAnimMontage* AimAnimation;
    UPROPERTY(EditDefaultsOnly, Category = "FireAnimation")
    UAnimMontage* WalkWeaponAnimation;

    /////////////////////////////////---HP---////////////////////////////////////////////

    virtual void AddHP_Implementation(float AddHP)override;

    void DecreasedHP();
    void IncreasedHP();

    UPROPERTY(EditDefaultsOnly, Category = "HP")
    float CurrentHP;
    UPROPERTY(EditDefaultsOnly, Category = "HP")
    float MinusHP = 1.0f;
    UPROPERTY(EditDefaultsOnly, Category = "HP")
    float PlusHP = 1.0f;
    UPROPERTY(EditDefaultsOnly, Category = "HP", meta = (ClampMin = "0", ClampMax = "100"))
    float HP = 100.0f;

    /////////////////////////////////---STAMINA---////////////////////////////////////////////

    virtual void AddStamina_Implementation(float AddStamina)override;
    void Stamina_HPInfo(float DeltaTime);

    void DecreasedStamina();
    void IncreasedStamina();

    UPROPERTY(EditDefaultsOnly, Category = "Stamina")
    float CurrentStamina;
    UPROPERTY(EditDefaultsOnly, Category = "Stamina")
    float MinusStamina = 1.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Stamina")
    float PlusStamina = 1.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Stamina", meta = (ClampMin = "0", ClampMax = "100"))
    float Stamina = 100.0f;

    /////////////////////////////////---CAMERA_COMPONENT---////////////////////////////////////////////

    UPROPERTY(VisibleAnywhere) USpringArmComponent* SpringArm;
    UPROPERTY(VisibleAnywhere) UCameraComponent* Camera;

    UFUNCTION(BlueprintCallable) bool ForwardVectorsSprint();

    void AimSpringArmLength(float DeltaTime);
    void MovementTick(float Default);

    float AimCurrentSpringArmLength = 800.0f;
    float AimTargetSpringArmLength = 800.0f;
    float AimTransitionSpeed = 1.0f;
 
    bool bIsCameraZomm;

    /////////////////////////////////---MovementSpeed---////////////////////////////////////////////

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Speed")
    float SpeedIdle = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Speed")
    float SpeedAim = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Speed")
    float SpeedWalk = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Speed")
    float SpeedRun = 600.0f;

    void SetCharacterSpeed(float SpeedValue);

    /////////////////////////////////---Test---////////////////////////////////////////////

};


