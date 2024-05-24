// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTypes.h"
#include "GameFramework/Character.h"

#include "Math/UnrealMathUtility.h"
#include "TDSCharacter.generated.h"

UCLASS()
class TDS_API ATDSCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ATDSCharacter();
private:
  


protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* NewInputComponent) override;
   /////////////////////////////////---INPUT---////////////////////////////////////////////
    void MoveFB(float Value);
    void MoveRL(float Value);
    void Jump();
    void StopJump();
    void Aim();
    void StopAim();
    void Fire();
    void StopFire();






    bool bFirePressed = false;
    bool bAimPressed = false;

private:
public:

    UPROPERTY(EditDefaultsOnly, Category = "FireAnimation")
    UAnimMontage* FireAnimation;
    UPROPERTY(EditDefaultsOnly, Category = "FireAnimation")
    UAnimMontage* AimAnimation;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    EMovementState MovementState = EMovementState::Run_State;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    FCharacterSpeed MovementInfo;


 
    void MovementTick(float Default);
    UFUNCTION(BlueprintCallable)
    void CharacterUpdate();
    UFUNCTION(BlueprintCallable)
    void ChangeMovementState(EMovementState NewMovementState);
};


