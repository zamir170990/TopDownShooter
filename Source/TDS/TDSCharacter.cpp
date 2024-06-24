
#include "TDSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Materials/Material.h"
#include "iostream"
#include "TDSGameInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

ATDSCharacter::ATDSCharacter() : Super()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 800.0f;
    SpringArm->SetRelativeRotation(FRotator(-80.0f, 0.0f, 0.0f));
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
    Camera->SetupAttachment(SpringArm);
}

void ATDSCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    MovementTick(DeltaTime);
    AimSpringArmLength(DeltaTime);
    Stamina_HPInfo(DeltaTime);
}

void ATDSCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    InitWeapon(InitWeaponName);

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
    NewInputComponent->BindAction("Sprint", IE_Pressed, this, &ATDSCharacter::Sprint);
    NewInputComponent->BindAction("Sprint", IE_Released, this, &ATDSCharacter::StopSprint);
    NewInputComponent->BindAction("Reload", IE_Released, this, &ATDSCharacter::TryReloadWeapon);
}

void ATDSCharacter::MovementTick(float Default)
{
    AddMovementInput(FVector(1.0f, 0.0f, 0.0f), AxisX);
    AddMovementInput(FVector(0.0f, 1.0f, 0.0f), AxisY);

    if (MovementState == EMovementState::SprintRun_State)
    {
        FVector myRotationVector = FVector(AxisX, AxisY, 0.0f);
        FRotator myRotator = myRotationVector.ToOrientationRotator();
        SetActorRotation((FQuat(myRotator)));
    }
    else
    {
        APlayerController* myController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (myController)
        {
            FHitResult ResultHit;
            //myController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery6, false, ResultHit);// bug was here Config\DefaultEngine.Ini
            myController->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);

            float FindRotaterResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
            SetActorRotation(FQuat(FRotator(0.0f, FindRotaterResultYaw, 0.0f)));

            if (CurrentWeapon)
            {
                FVector Displacement = FVector(0);
                switch (MovementState)
                {
                case EMovementState::Aim_State:
                    Displacement = FVector(0.0f, 0.0f, 160.0f);
                    CurrentWeapon->ShouldReduceDispersion = true;
                    break;
                case EMovementState::AimWalk_State:
                    CurrentWeapon->ShouldReduceDispersion = true;
                    Displacement = FVector(0.0f, 0.0f, 160.0f);
                    break;
                case EMovementState::Walk_State:
                    Displacement = FVector(0.0f, 0.0f, 120.0f);
                    CurrentWeapon->ShouldReduceDispersion = false;
                    break;
                case EMovementState::Run_State:
                    Displacement = FVector(0.0f, 0.0f, 120.0f);
                    CurrentWeapon->ShouldReduceDispersion = false;
                    break;
                case EMovementState::SprintRun_State:
                    break;
                default:
                    break;
                }

                CurrentWeapon->ShootEndLocation = ResultHit.Location + Displacement;
                //aim cursor like 3d Widget?
            }
        }
    }
}

void ATDSCharacter::AttackCharEvent(bool bIsFiring)
{
    WeaponActor = nullptr;
    WeaponActor = GetCurrentWeapon();
    if (WeaponActor)
    {
        WeaponActor->SetWeaponStateFire(bIsFiring);
    }
    else
        UE_LOG(LogTemp, Warning, TEXT("ATDSCharacter::AttackCharEvent - CurrentWeapon -NULL"));
}

void ATDSCharacter::CharacterUpdate()
{
    //AWeaponActor* myWeapon = GetCurrentWeapon();
    //if (myWeapon)
    //{
    //    myWeapon->UpdateStateWeapon(MovementState);
    //}
    float ResSpeed = 600.0f;
    switch (MovementState)
    {
    case EMovementState::Aim_State:
        ResSpeed = MovementSpeedInfo.AimSpeed;
        break;
    case EMovementState::AimWalk_State:
        ResSpeed = MovementSpeedInfo.AimSpeedWalk;
        break;
    case EMovementState::Walk_State:
        ResSpeed = MovementSpeedInfo.WalkSpeed;
        break;
    case EMovementState::Run_State:
        ResSpeed = MovementSpeedInfo.RunSpeed;
        break;
    case EMovementState::SprintRun_State:
        ResSpeed = MovementSpeedInfo.SprintRunSpeed;
        break;
    default:
        break;
    }

    GetCharacterMovement()->MaxWalkSpeed = ResSpeed;
}

void ATDSCharacter::ChangeMovementState()
{
    if (!WalkEnabled && !SprintRunEnabled && !AimEnabled)
    {
        MovementState = EMovementState::Run_State;
    }
    else
    {
        if (SprintRunEnabled)
        {
            WalkEnabled = false;
            AimEnabled = false;
            MovementState = EMovementState::SprintRun_State;
        }
        if (!WalkEnabled && !SprintRunEnabled && !AimEnabled)
        {
            MovementState = EMovementState::AimWalk_State;
        }
        else
        {
            if (!WalkEnabled && !SprintRunEnabled && !AimEnabled)
            {
                MovementState = EMovementState::Walk_State;
            }
            else
            {
                if (!WalkEnabled && !SprintRunEnabled && !AimEnabled)
                {
                    MovementState = EMovementState::Aim_State;
                }
            }
        }
    }
    CharacterUpdate();

    AWeaponActor* myWeapon = GetCurrentWeapon();
    if (myWeapon)
    {
        myWeapon->UpdateStateWeapon(MovementState);
    }
}

void ATDSCharacter::InputAxisY(float Value)
{
}

void ATDSCharacter::InputAxisX(float Value)
{
}

AWeaponActor* ATDSCharacter::GetCurrentWeapon()
{
    return CurrentWeapon;
}

void ATDSCharacter::InitWeapon(FName IdWeaponName)
{
    UTDSGameInstance* myGI = Cast<UTDSGameInstance>(GetGameInstance());
    FWeaponInfo myWeaponInfo;
    if (myGI)
    {
        if (myGI->GetWeaponInfoByNAme(IdWeaponName,myWeaponInfo))
        {
            if (myWeaponInfo.WeaponClass)
            {
                FVector SpawnLocation = FVector(0);
                FRotator SpawnRotation = FRotator(0);

                FActorSpawnParameters SpawnParams;
                SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
                SpawnParams.Owner = GetOwner();
                SpawnParams.Instigator = GetInstigator();

                AWeaponActor* myWeapon = Cast<AWeaponActor>(GetWorld()->SpawnActor(myWeaponInfo.WeaponClass, &SpawnLocation, &SpawnRotation, SpawnParams));
                if (WeaponActor)
                {
                    FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);
                    WeaponActor->AttachToComponent(GetMesh(), Rule, FName("Bag"));
                    CurrentWeapon = WeaponActor;

                    WeaponActor->WeaponSetting = myWeaponInfo;
                    WeaponActor->WeaponInfo.Round = myWeaponInfo.MaxRound;

                    //remove !!!! Debag
                   WeaponActor->ReloadTime = myWeaponInfo.ReloadTime;

                   myWeapon->UpdateStateWeapon(MovementState);

                   myWeapon->OnWeaponReloadStart.AddDynamic(this, &ATDSCharacter::WeaponReloadStart);
                   myWeapon->OnWeaponReloadEnd.AddDynamic(this, &ATDSCharacter::WeaponReloadEnd);
                }
            }
        }

    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ATPSCharacter::InitWeapon - Weapon not found in table -NULL"));
    }
}

void ATDSCharacter::TryReloadWeapon()
{
    if (CurrentWeapon)
    {
        if (CurrentWeapon->GetWeaponRound() <= CurrentWeapon->WeaponSetting.MaxRound)
            CurrentWeapon->InitReload();
    }
}

void ATDSCharacter::WeaponReloadStart(UAnimMontage* Anim)
{
    WeaponReloadStart_BP(Anim);
}

void ATDSCharacter::WeaponReloadEnd()
{
    WeaponReloadEnd_BP();
}

void ATDSCharacter::WeaponReloadStart_BP_Implementation(UAnimMontage* Anim)
{
    // in BP
}

void ATDSCharacter::WeaponReloadEnd_BP_Implementation()
{
    // in BP
}

UDecalComponent* ATDSCharacter::GetCursorToWorld()
{
    return nullptr;
}

void ATDSCharacter::AimSpringArmLength(float DeltaTime)
{
    if (SpringArm)
    {
        if (AimCurrentSpringArmLength != AimTargetSpringArmLength)
        {
            AimCurrentSpringArmLength = FMath::FInterpTo(AimCurrentSpringArmLength, AimTargetSpringArmLength, DeltaTime, AimTransitionSpeed);
            SpringArm->TargetArmLength = AimCurrentSpringArmLength;
        }
    }
}

bool ATDSCharacter::ForwardVectorsSprint()
{
    FVector ForwardVector = GetCapsuleComponent()->GetForwardVector();
    FVector FirstVector = ForwardVector * 501.0f + GetActorLocation();
    FVector VelocityVector = GetVelocity().GetSafeNormal();
    FVector SecondVector = VelocityVector * 501.0f + GetActorLocation();
    float Tolerance = 350.0f;
    float Error = (FirstVector - SecondVector).Size();
    return Error <= Tolerance;
}

void ATDSCharacter::SetCharacterSpeed(float SpeedValue)
{
    if (SpeedValue == SpeedIdle)
    {
        GetCharacterMovement()->MaxWalkSpeed = SpeedIdle;
    }
    else if (SpeedValue == SpeedAim)
    {
        GetCharacterMovement()->MaxWalkSpeed = SpeedAim;
    }
    else if (SpeedValue == SpeedWalk)
    {
        GetCharacterMovement()->MaxWalkSpeed = SpeedWalk;
    }
    else if (SpeedValue == SpeedRun)
    {
        GetCharacterMovement()->MaxWalkSpeed = SpeedRun;
    }
}

void ATDSCharacter::MoveFB(float Value)
{
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
    AddMovementInput(Direction, Value);
    bIsMove = true;
}

void ATDSCharacter::MoveRL(float Value)
{
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
    AddMovementInput(Direction, Value);
    bIsMove = true;
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
    if(WeaponActor)
    {
        AttachToAimSocket();
        AimTargetSpringArmLength = 1050.0f;
        SetCharacterSpeed(SpeedAim);
        bIsAim = true;
        bIsSprint = false;
        if (AimAnimation)
        {
            PlayAnimMontage(AimAnimation, 1.0, NAME_None);
        }
    }
}

void ATDSCharacter::StopAim()
{   
    if(WeaponActor)
    {
        bIsAim = false;
        AimTargetSpringArmLength = 800.0f;
        StopAnimMontage(AimAnimation);
        PlayAnimMontage(WalkWeaponAnimation, 1.0, NAME_None);
        AttachToWalkWeaponSocket();
        SetCharacterSpeed(SpeedWalk);
    }
}

void ATDSCharacter::Fire()
{
    if(WeaponActor)
    {
        if(bIsAim)
        {
            AttackCharEvent(true);
        
            SetCharacterSpeed(SpeedFire);
            AttachToFireSocket();
            bFire = true;
            if (FireAnimation)
            {
                SetCharacterSpeed(SpeedAim);
                PlayAnimMontage(FireAnimation, 1.0, NAME_None);
            }
        }
    }
}

void ATDSCharacter::StopFire()
{
    if(WeaponActor)
    {
        AttackCharEvent(false);
        AttachToWalkWeaponSocket();
        StopAnimMontage(FireAnimation);
        if (bIsAim)
        {
            bFire = false;
            if (AimAnimation)
            {
                AttachToAimSocket();
                PlayAnimMontage(AimAnimation, 1.0, NAME_None);
            }
        }
    }
}

void ATDSCharacter::Sprint()
{
    if (!bIsAim && Stamina >= MinusStamina)
    { 
        StopAnimMontage(WalkWeaponAnimation);
        SetCharacterSpeed(SpeedRun);
        DecreasedStamina();
        bIsSprint = true;
        AttachToWeaponBagSocket();
    }
}

void ATDSCharacter::StopSprint()
{
    bIsSprint = false;
    SetCharacterSpeed(SpeedWalk);
    IncreasedStamina();
}

void ATDSCharacter::DecreasedStamina()
{
    CurrentStamina = Stamina - MinusStamina;
    Stamina = CurrentStamina;
    Stamina = FMath::Clamp(Stamina, 0.f, 100.f);
}

void ATDSCharacter::IncreasedStamina()
{
    if (bIsSprint == false)
    {
        CurrentStamina = Stamina + PlusStamina;
        Stamina = CurrentStamina;
    }
}

void ATDSCharacter::Stamina_HPInfo(float DeltaTime)
{
    Stamina = FMath::Clamp(Stamina, 0.f, 100.f);
    if (bIsSprint && Stamina > 0.f)
    {
        DecreasedStamina();
    }
    else if (!bIsSprint && Stamina < 100.f)
    {
        IncreasedStamina();
    }
    if (FMath::IsNearlyZero(Stamina))
    {
        StopSprint();
    }
    HP = FMath::Clamp(HP, 0.f, 100.f);
    if (bIsSprint)
    {
        if (ForwardVectorsSprint())
        {
            Sprint();
        }
        else
        {
            StopSprint();
            bIsSprint = false;
        }
    }
}

void ATDSCharacter::AddStamina_Implementation(float AddStamina)
{
    if (Stamina < 100)
    {
        Stamina = Stamina + AddStamina;
    }
}

void ATDSCharacter::DecreasedHP()
{
    CurrentHP = HP - MinusHP;
    HP = CurrentHP;
}

void ATDSCharacter::IncreasedHP()
{
    if (bIsSprint == false)
    {
        CurrentHP = HP + PlusHP;
        HP = CurrentStamina;
    }
}

void ATDSCharacter::AddHP_Implementation(float AddHP)
{
    if (HP < 100)
    {
        HP = HP + AddHP;
    }
}
void ATDSCharacter::AttachToAimSocket()
{
    if (WeaponActor)
    {
        WeaponActor->DetachFromCharacter();
        USkeletalMeshComponent* CharacterMesh = GetMesh();
        if (CharacterMesh)
        {
            WeaponActor->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "AimSocket");
        }
    }
}

void ATDSCharacter::AttachToWeaponBagSocket()
{
    if (WeaponActor)
    {
        WeaponActor->DetachFromCharacter();
        USkeletalMeshComponent* CharacterMesh = GetMesh();
        if (CharacterMesh)
        {
            WeaponActor->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "Bag");
        }
    }
}

void ATDSCharacter::AttachToWalkWeaponSocket()
{
    if (WeaponActor)
    {
        WeaponActor->DetachFromCharacter();
        USkeletalMeshComponent* CharacterMesh = GetMesh();
        if (CharacterMesh)
        {
            WeaponActor->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "WalkWeaponSocket");
        }
    }
}

void ATDSCharacter::AttachToFireSocket()
{
    if (WeaponActor)
    {
        WeaponActor->DetachFromCharacter();
        USkeletalMeshComponent* CharacterMesh = GetMesh();
        if (CharacterMesh)
        {
            WeaponActor->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "FireSocket");
        }
    }
}