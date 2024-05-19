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
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values
ATDSCharacter::ATDSCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATDSCharacter::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ATDSCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    MovementTick(DeltaTime);
}

void ATDSCharacter::SetupPlayerInputComponent(UInputComponent* NewInputComponent)
{
    Super::SetupPlayerInputComponent(InputComponent);
 
}



void ATDSCharacter::MovementTick(float Default)
{
    APlayerController* myController = UGameplayStatics::GetPlayerController(GetWorld(), 0); 
    if (myController)
    {
        FHitResult ResultHit;
       // myController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery6, false, ResultHit);
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

