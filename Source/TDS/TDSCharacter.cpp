// Fill out your copyright notice in the Description page of Project Settings.


#include "TDSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Bullet.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

ATDSCharacter::ATDSCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ATDSCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void ATDSCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    MovementTick(DeltaTime);
}

void ATDSCharacter::SetupPlayerInputComponent(UInputComponent* NewInputComponent)
{
    Super::SetupPlayerInputComponent(NewInputComponent);

    NewInputComponent->BindAxis("MoveFB", this, &ATDSCharacter::MoveFB);
    NewInputComponent->BindAxis("MoveRL", this, &ATDSCharacter::MoveRL);

    NewInputComponent->BindAction("Jump",IE_Pressed, this, &ATDSCharacter::Jump);
    NewInputComponent->BindAction("Jump",IE_Released, this, &ATDSCharacter::StopJump);

    NewInputComponent->BindAction("Aim", IE_Pressed, this, &ATDSCharacter::Aim);
    NewInputComponent->BindAction("Aim", IE_Released, this, &ATDSCharacter::StopAim);

    NewInputComponent->BindAction("Fire", IE_Pressed, this, &ATDSCharacter::Fire);
    NewInputComponent->BindAction("Fire", IE_Released, this, &ATDSCharacter::StopFire);
 
}

void ATDSCharacter::MoveFB(float Value)
{
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
    AddMovementInput(Direction, Value);
}

void ATDSCharacter::MoveRL(float Value)
{
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
    AddMovementInput(Direction, Value);
}

void ATDSCharacter::Jump()
{
    bPressedJump = true;
}

void ATDSCharacter::StopJump()
{
    bPressedJump = false;
}

void ATDSCharacter::Aim()
{
    bAimPressed = true;
    if (AimAnimation)
    {
        PlayAnimMontage(AimAnimation, 1.0, NAME_None);
    }
}

void ATDSCharacter::StopAim()
{
}

void ATDSCharacter::Fire()
{
    bFirePressed = true;
    if (!bAimPressed)
    {
        return;
    }

    if (FireAnimation)
    {
        PlayAnimMontage(FireAnimation, 1.0, NAME_None);
    }
}



void ATDSCharacter::StopFire()
{
    if(AimAnimation)
    {
            PlayAnimMontage(AimAnimation, 1.0, NAME_None);
    }

    if (FireAnimation)
    {
        StopAnimMontage(FireAnimation);
    }
}

void ATDSCharacter::MovementTick(float Default)
{
    APlayerController* myController = UGameplayStatics::GetPlayerController(GetWorld(), 0); 
    if (myController)
    {
        FHitResult ResultHit;
        myController->GetHitResultUnderCursor(ECC_GameTraceChannel11,true, ResultHit);
        float FindRotaterResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
        SetActorRotation(FQuat(FRotator(0.0f,FindRotaterResultYaw, 0.0f)));
    }
}

void ATDSCharacter::CharacterUpdate()
{
    float ResSpeed = 600.0f;
    switch (MovementState)
    {
    case EMovementState::Aim_State:
        ResSpeed = MovementInfo.AimSpeed;
        break;
    case EMovementState::Walk_State:
        ResSpeed = MovementInfo.WalkSpeed;
        break;
    case EMovementState::Run_State:
        ResSpeed = MovementInfo.RunSpeed;
        break;
    default:
        break;
    }
    GetCharacterMovement()->MaxWalkSpeed = ResSpeed;
}

void ATDSCharacter::ChangeMovementState(EMovementState NewMovementState)
{
    MovementState = NewMovementState;
    CharacterUpdate();
}

