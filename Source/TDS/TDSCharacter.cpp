
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

ATDSCharacter::ATDSCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ATDSCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	MovementTick(DeltaSeconds);
	UpdateSpringArmLength(DeltaSeconds);
	
	if (CurrentCursor)
	{
		APlayerController* myPC = Cast<APlayerController>(GetController());
		if (myPC)
		{
			FHitResult TraceHitResult;
			myPC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CurrentCursor->SetWorldLocation(TraceHitResult.Location);
			CurrentCursor->SetWorldRotation(CursorR);
		}
	}
	if (bIsSprint && ForwardVectorsSprint())
	{
		Sprint();
	}
	else
	{
		StopSprint();
	}
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
}

void ATDSCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitWeapon(InitWeaponName);

	if (CursorMaterial)
	{
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
	}
}

bool ATDSCharacter::ForwardVectorsSprint()
{
	FVector ForwardVector = GetCapsuleComponent()->GetForwardVector();
	FVector RightVector = GetCapsuleComponent()->GetRightVector();
	FVector VelocityVector = GetVelocity().GetSafeNormal();
	const float CosineOfAllowedAngle = FMath::Cos(FMath::DegreesToRadians(50.0f));
	float ForwardDotProduct = FVector::DotProduct(ForwardVector, VelocityVector);
	bool IsMovingForward = ForwardDotProduct > CosineOfAllowedAngle;
	float RightDotProduct = FVector::DotProduct(RightVector, VelocityVector);
	bool IsMovingTooMuchSideways = FMath::Abs(RightDotProduct) > CosineOfAllowedAngle;
	return IsMovingForward && !IsMovingTooMuchSideways;
}


void ATDSCharacter::SetupPlayerInputComponent(UInputComponent* NewInputComponent)
{
	Super::SetupPlayerInputComponent(NewInputComponent);
	NewInputComponent->BindAxis(TEXT("MoveForward"), this, &ATDSCharacter::InputAxisX);
	NewInputComponent->BindAxis(TEXT("MoveRight"), this, &ATDSCharacter::InputAxisY);
	NewInputComponent->BindAction(TEXT("FireEvent"), EInputEvent::IE_Pressed, this, &ATDSCharacter::InputAttackPressed);
	NewInputComponent->BindAction(TEXT("FireEvent"), EInputEvent::IE_Released, this, &ATDSCharacter::InputAttackReleased);
	NewInputComponent->BindAction(TEXT("ReloadEvent"), EInputEvent::IE_Released, this, &ATDSCharacter::TryReloadWeapon);
	NewInputComponent->BindAction("JumpEvent",IE_Pressed, this, &ATDSCharacter::Jump);
    NewInputComponent->BindAction("JumpEvent",IE_Released, this, &ATDSCharacter::StopJump);
    NewInputComponent->BindAction("AimEvent", IE_Pressed, this, &ATDSCharacter::InputAim);
    NewInputComponent->BindAction("AimEvent", IE_Released, this, &ATDSCharacter::InputStopAim);
    NewInputComponent->BindAction("SprintEvent", IE_Pressed, this, &ATDSCharacter::Sprint);
    NewInputComponent->BindAction("SprintEvent", IE_Released, this, &ATDSCharacter::StopSprint);
}

void ATDSCharacter::InputAim()
{
	if (CurrentWeapon && !bIsReloading)
	{
		AttachToAimSocket();
		bIsAim = true;
		if (AimAnimation)
		{
			PlayAnimMontage(AimAnimation, 1.0, NAME_None);
		}
		MovementState = EMovementState::Aim_State;
		CharacterUpdate();
		bIsCameraZoom = true;
	}
}

void ATDSCharacter::InputStopAim()
{
	if (CurrentWeapon && !bIsReloading)
	{
		bIsAim = false;
		StopAnimMontage(AimAnimation);
		AttachToIdleWeaponSocket();
		PlayAnimMontage(IdleWEaponAnimation, 1.0, NAME_None);
		MovementState = EMovementState::Walk_State;
		CharacterUpdate();
		bIsCameraZoom = false;
	}
}

void ATDSCharacter::InputAttackPressed()
{
	if (CurrentWeapon && !bIsReloading && bIsAim)
	{
		AttackCharEvent(true);
		AttachToFireSocket();
		if (FireAnimation)
		{
			PlayAnimMontage(FireAnimation, 1.0, NAME_None);
		}
		CharacterUpdate();
		MovementState = EMovementState::FireWalk_State;
	}
}

void ATDSCharacter::InputAttackReleased()
{
	if (CurrentWeapon && !bIsReloading && bIsAim)
	{
		AttackCharEvent(false);
		AttachToAimSocket();
		StopAnimMontage(FireAnimation);
		PlayAnimMontage(AimAnimation, 1.0, NAME_None);
		MovementState = EMovementState::Aim_State;
		CharacterUpdate();
	}
}


void ATDSCharacter::Sprint()
{
	if(!bIsReloading)
	{
		ForwardVectorsSprint();
		AttachToWeaponBagSocket();
		StopAnimMontage();
		if (CanSprintForward())
		{
			bIsSprint = true;
		}
		MovementState = EMovementState::Run_State;
		CharacterUpdate();
	}
}

bool ATDSCharacter::CanSprintForward()
{
	return ForwardVectorsSprint();
}


void ATDSCharacter::StopSprint()
{
	bIsSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeedInfo.WalkSpeedNormal;
}

void ATDSCharacter::Jump()
{
    bPressedJump = true;
}

void ATDSCharacter::StopJump()
{
    bPressedJump = false;
}

void ATDSCharacter::InputAxisY(float Value)
{
	AxisY = Value;
}

void ATDSCharacter::InputAxisX(float Value)
{
	AxisX = Value;
}

void ATDSCharacter::MovementTick(float DeltaTime)
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
			//myController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery6, false, ResultHit);
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
				case EMovementState::FireWalk_State:
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
			}
		}
	}
}

void ATDSCharacter::AttackCharEvent(bool bIsFiring)
{
	AWeaponActor* myWeapon = nullptr;
	myWeapon = GetCurrentWeapon();
	if (myWeapon)
	{
		myWeapon->SetWeaponStateFire(bIsFiring);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("ATPSCharacter::AttackCharEvent - CurrentWeapon -NULL"));
}

void ATDSCharacter::CharacterUpdate()
{
	float ResSpeed = 600.0f;
	switch (MovementState)
	{
	case EMovementState::Aim_State:
		ResSpeed = MovementSpeedInfo.AimSpeedNormal;
		break;
	case EMovementState::FireWalk_State:
		ResSpeed = MovementSpeedInfo.FireSpeed;
		break;
	case EMovementState::Walk_State:
		ResSpeed = MovementSpeedInfo.WalkSpeedNormal;
		break;
	case EMovementState::Run_State:
		ResSpeed = MovementSpeedInfo.RunSpeedNormal;
		break;
	case EMovementState::SprintRun_State:
		ResSpeed = MovementSpeedInfo.SprintRunSpeedRun;
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
		if (WalkEnabled && !SprintRunEnabled && AimEnabled)
		{
			MovementState = EMovementState::FireWalk_State;
		}
		else
		{
			if (WalkEnabled && !SprintRunEnabled && !AimEnabled)
			{
				MovementState = EMovementState::Walk_State;
			}
			else
			{
				if (!WalkEnabled && !SprintRunEnabled && AimEnabled)
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
		if (myGI->GetWeaponInfoByName(IdWeaponName, myWeaponInfo))
		{
			if (myWeaponInfo.WeaponClass)
			{
				FVector SpawnLocation = GetMesh()->GetSocketLocation(FName("Bag"));
	            FRotator SpawnRotation = GetMesh()->GetSocketRotation(FName("Bag"));

				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnParams.Owner = GetOwner();
				SpawnParams.Instigator = GetInstigator();

				AWeaponActor* myWeapon = Cast<AWeaponActor>(GetWorld()->SpawnActor(myWeaponInfo.WeaponClass, &SpawnLocation, &SpawnRotation, SpawnParams));
				if (myWeapon)
				{
					FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);
					myWeapon->AttachToComponent(GetMesh(), Rule, FName("Bag"));
					CurrentWeapon = myWeapon;

					myWeapon->WeaponSetting = myWeaponInfo;
					myWeapon->WeaponInfo.Round = myWeaponInfo.MaxRound;
					//Remove !!! Debug
					myWeapon->ReloadTime = myWeaponInfo.ReloadTime;
					myWeapon->UpdateStateWeapon(MovementState);

					myWeapon->OnWeaponReloadStart.AddDynamic(this, &ATDSCharacter::WeaponReloadStart);
					myWeapon->OnWeaponReloadEnd.AddDynamic(this, &ATDSCharacter::WeaponReloadEnd);
				}
			}
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("ATPSCharacter::InitWeapon - Weapon not found in table -NULL"));
	}
}

void ATDSCharacter::TryReloadWeapon()
{
	if (CurrentWeapon)
	{
		if (CurrentWeapon->GetWeaponRound() <= CurrentWeapon->WeaponSetting.MaxRound)
		{
			CurrentWeapon->InitReload();
		}
	}
}

void ATDSCharacter::WeaponReloadStart(UAnimMontage* Anim)
{
	bIsReloading = true;
	WeaponReloadStart_BP(Anim);
		if (ReloadAnimation)
		{
			PlayAnimMontage(ReloadAnimation, 1.0, NAME_None);
			AttachToWeaponReloadSocket();
		}
}

void ATDSCharacter::WeaponReloadEnd()
{
	bIsReloading = false;
	WeaponReloadEnd_BP();
	InputAttackReleased();
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
	return CurrentCursor;
}

void ATDSCharacter::AttachToAimSocket()
{
	bIsAim = true;
	if (CurrentWeapon)
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("AimSocket"));
}

void ATDSCharacter::AttachToWalkWeaponSocket()
{
	if (CurrentWeapon)
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("WalkWeaponSocket"));
}

void ATDSCharacter::AttachToFireSocket()
{
	if (CurrentWeapon)
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("FireSocket"));
}

void ATDSCharacter::AttachToIdleWeaponSocket()
{
	if (CurrentWeapon)
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("IdleWalkSocket"));
}

void ATDSCharacter::AttachToWeaponBagSocket()
{
	if (CurrentWeapon)
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("Bag"));
}

void ATDSCharacter::AttachToWeaponReloadSocket()
{
	if (CurrentWeapon)
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("ReloadSocket"));
}

void ATDSCharacter::UpdateSpringArmLength(float DeltaSecond)
{
	float TargetLength = bIsCameraZoom ? WeaponSetting.ProjectileSetting.SpringArmAim : WeaponSetting.ProjectileSetting.DefaultArmLength;
	float CurrentLength = CameraBoom->TargetArmLength;
	float InterpSpeed = 1.0f;

	float NewLength = FMath::FInterpTo(CurrentLength, TargetLength, DeltaSecond, InterpSpeed);
	CameraBoom->TargetArmLength = NewLength;
	UE_LOG(LogTemp, Warning, TEXT("SpringArm Aim Length: %f"), WeaponSetting.ProjectileSetting.SpringArmAim);
	UE_LOG(LogTemp, Warning, TEXT("Default Arm Length: %f"), WeaponSetting.ProjectileSetting.DefaultArmLength);

}

FProjectileInfo ATDSCharacter::GetWProjectile()
{
	return WeaponSetting.ProjectileSetting;
}

