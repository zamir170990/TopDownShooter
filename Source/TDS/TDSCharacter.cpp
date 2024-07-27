
#include "TDSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "iostream"
#include "TDSGameInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Actor.h"
#include "ProjectileDefault.h"
//#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TDSGameInstance.h"
#include "ProjectileDefault.h"

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

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	if (InventoryComponent)
	{
		InventoryComponent->OnSwitchWeapon.AddDynamic(this, &ATDSCharacter::InitWeapon);
	}
	HealthCharacterComponent = CreateDefaultSubobject<UTDSHealthCharacterComponent>(TEXT("HealthComponent"));
	if (HealthCharacterComponent)
	{
		HealthCharacterComponent->OnDead.AddDynamic(this, &ATDSCharacter::CharDead);
	}
}

void ATDSCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	MovementTick(DeltaSeconds);
	UpdateSpringArmLengthForAim(DeltaSeconds);
	
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
}

void ATDSCharacter::BeginPlay()
{
	Super::BeginPlay();

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

	NewInputComponent->BindAction("SwitchNextWeapo", IE_Released, this, &ATDSCharacter::TrySwitchNextWeapon);
	NewInputComponent->BindAction("SwitchPreviosWeapon", IE_Released, this, &ATDSCharacter::TrySwitchPreviosWeapon);

	NewInputComponent->BindAction(TEXT("AblityAction"), EInputEvent::IE_Pressed, this, &ATDSCharacter::TryAbilityEnabled);
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
		MovementState = EMovementState::Aim_State;
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

void ATDSCharacter::TrySwitchNextWeapon()
{
	if (InventoryComponent->WeaponSlots.Num() > 1)
	{
		int8 OldIndex = CurrentIndexWeapon;
		FAdditionalWeaponInfo OldInfo;
		if (CurrentWeapon)
		{
			OldInfo = CurrentWeapon->AdditionalWeaponInfo;
			if (CurrentWeapon->WeaponReloading)
				CurrentWeapon->CancelReload();
		}
		if (InventoryComponent)
		{
			if(InventoryComponent->SwitchWeaponToIndex(CurrentIndexWeapon + 1,OldIndex,OldInfo,true))
			{ }
		}
	}
}

void ATDSCharacter::TrySwitchPreviosWeapon()
{
	if (InventoryComponent->WeaponSlots.Num() > 1)
	{
		int8 OldIndex = CurrentIndexWeapon;
		FAdditionalWeaponInfo OldInfo;
		if (CurrentWeapon)
		{
			OldInfo = CurrentWeapon->AdditionalWeaponInfo;
			if (CurrentWeapon->WeaponReloading)
				CurrentWeapon->CancelReload();
		}
		if (InventoryComponent)
		{
			if (InventoryComponent->SwitchWeaponToIndex(CurrentIndexWeapon - 1, OldIndex, OldInfo,false))
			{
			}
		}
	}
}

void ATDSCharacter::TryAbilityEnabled()
{
	if (AbilityEffect)
	{
		UTDS_StateEffect* NewEffect = NewObject<UTDS_StateEffect>(this, AbilityEffect);
		if (NewEffect)
		{
			NewEffect->InitObject(this);
		}
	}
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
	if(bIsAlive)
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
						Displacement = FVector(0.0f, 0.0f, 40.0f);
						CurrentWeapon->ShouldReduceDispersion = true;
						break;
					case EMovementState::AimWalk_State:
						CurrentWeapon->ShouldReduceDispersion = true;
						Displacement = FVector(0.0f, 0.0f, 40.0f);
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
		UE_LOG(LogTemp, Warning, TEXT("ATDSCharacter::AttackCharEvent - CurrentWeapon -NULL"));
}

void ATDSCharacter::CharacterUpdate()
{
	float ResSpeed = 600.0f;
	switch (MovementState)
	{
	case EMovementState::Aim_State:
		ResSpeed = MovementSpeedInfo.AimSpeedNormal;
		break;
	case EMovementState::AimWalk_State:
		ResSpeed = MovementSpeedInfo.WalkSpeedNormal;
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
			MovementState = EMovementState::AimWalk_State;
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

void ATDSCharacter::InitWeapon(FName IdWeaponName,FAdditionalWeaponInfo WeaponAdditionalInfo, int32 NewCurrentIndexWeapon)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

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
				SpawnParams.Owner = this;
				SpawnParams.Instigator = GetInstigator();

				AWeaponActor* myWeapon = Cast<AWeaponActor>(GetWorld()->SpawnActor(myWeaponInfo.WeaponClass, &SpawnLocation, &SpawnRotation, SpawnParams));
				if (myWeapon)
				{
					FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);
					myWeapon->AttachToComponent(GetMesh(), Rule, FName("Bag"));
					CurrentWeapon = myWeapon;

					myWeapon->WeaponSetting = myWeaponInfo;
					//myWeapon->AdditionalWeaponInfo.Round = myWeaponInfo.MaxRound;
					//Remove !!! Debug
					myWeapon->ReloadTime = myWeaponInfo.ReloadTime;
					myWeapon->UpdateStateWeapon(MovementState);

					myWeapon->AdditionalWeaponInfo = WeaponAdditionalInfo;
					//if (InventoryComponent)
						CurrentIndexWeapon = InventoryComponent->GetWeaponIndexSlotByName(IdWeaponName);

					myWeapon->OnWeaponReloadStart.AddDynamic(this, &ATDSCharacter::WeaponReloadStart);
					myWeapon->OnWeaponReloadEnd.AddDynamic(this, &ATDSCharacter::WeaponReloadEnd);
					myWeapon->OnWeaponFireStart.AddDynamic(this, &ATDSCharacter::WeaponFireStart);

					if (CurrentWeapon->GetWeaponRound() <= 0 && CurrentWeapon->CheckCanWeaponReload())
						CurrentWeapon->InitReload();

					if (InventoryComponent)
						InventoryComponent->OnWeaponAmmoAviable.Broadcast(myWeapon->WeaponSetting.WeaponType);
				
				}
			}
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("ATDSCharacter::InitWeapon - Weapon not found in table -NULL"));
	}
}

void ATDSCharacter::TryReloadWeapon()
{
	if (CurrentWeapon && !CurrentWeapon->WeaponReloading)
	{
		if (CurrentWeapon->GetWeaponRound() <= CurrentWeapon->WeaponSetting.MaxRound)
			CurrentWeapon->InitReload();
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

void ATDSCharacter::WeaponReloadEnd(bool bIsSuccess, int32 AmmoTake)
{
	if (InventoryComponent && CurrentWeapon)
	{
		InventoryComponent->AmmoSlotChangeValue(CurrentWeapon->WeaponSetting.WeaponType,AmmoTake);
		InventoryComponent->SetAdditionalInfoWeapon(CurrentIndexWeapon, CurrentWeapon->AdditionalWeaponInfo);
	}
	WeaponReloadEnd_BP(bIsSuccess);
	bIsReloading = false;
	InputAttackReleased();
}

void ATDSCharacter::WeaponReloadStart_BP_Implementation(UAnimMontage* Anim)
{
	// in BP
}

void ATDSCharacter::WeaponReloadEnd_BP_Implementation(bool bIsSuccess)
{
	// in BP
}

void ATDSCharacter::WeaponFireStart(UAnimMontage* Anim)
{
	if (InventoryComponent && CurrentWeapon)
		InventoryComponent->SetAdditionalInfoWeapon(CurrentIndexWeapon, CurrentWeapon->AdditionalWeaponInfo);
	WeaponFireStart_BP(Anim);
}

void ATDSCharacter::WeaponFireStart_BP_Implementation(UAnimMontage* Anim)
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

void ATDSCharacter::RemoveCurrentWeapon()
{
}

FProjectileInfo ATDSCharacter::GetWProjectile()
{
	return WeaponSetting.ProjectileSetting;
}

EPhysicalSurface ATDSCharacter::GetSurfuceType()
{
	EPhysicalSurface Result = EPhysicalSurface::SurfaceType_Default;
	if (HealthCharacterComponent)
	{
		if (HealthCharacterComponent->GetCurrentShield() <= 0)
		{
			if (GetMesh())
			{
				UMaterialInterface* myMaterial = GetMesh()->GetMaterial(0);
				if (myMaterial)
				{
					Result = myMaterial->GetPhysicalMaterial()->SurfaceType;
				}
			}
		}
	}
	return Result;
}

TArray<UTDS_StateEffect*> ATDSCharacter::GetAllCurrentEffects()
{
	return Effects;
}

void ATDSCharacter::RemoveEffect(UTDS_StateEffect* RemoveEffect)
{
	Effects.Remove(RemoveEffect);
}

void ATDSCharacter::AddEffect(UTDS_StateEffect* newEffect)
{
	Effects.Add(newEffect);
}

void ATDSCharacter::CharDead()
{
	float TimeAnim = 0.0f;
	int32 rnd = FMath::RandHelper(DeadsAnim.Num());
	if (DeadsAnim.IsValidIndex(rnd) && DeadsAnim[rnd] && GetMesh() && GetMesh()->GetAnimInstance())
	{
		TimeAnim = DeadsAnim[rnd]->GetPlayLength();
		GetMesh()->GetAnimInstance()->Montage_Play(DeadsAnim[rnd]);
	}

	bIsAlive = false;

	UnPossessed();
	//GetWorldTimerManager().SetTimer(TimerHandle_RagDollTimer, this, &ATDSCharacter::EnableRagdoll, TimeAnim, false);
	GetCursorToWorld()->SetVisibility(false);
}

void ATDSCharacter::EnableRagdoll()
{
	if (GetMesh())
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		GetMesh()->SetSimulatePhysics(true);
	}
}

float ATDSCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (bIsAlive)
	{
		HealthCharacterComponent->ChangeHealthValue(-DamageAmount);
	}
	//if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		AProjectileDefault* myProjectile = Cast<AProjectileDefault>(DamageCauser);
		if (myProjectile)
		{
			UCTypes::AddEffectBySurfaceType(this, myProjectile->ProjectileSetting.Effect, GetSurfuceType());
		}
	}

	return ActualDamage;
}

void ATDSCharacter::UpdateSpringArmLengthForAim(float DeltaTime)
{
	if(bIsAim && CurrentWeapon)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			FHitResult Hit;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, Hit);
			FVector CursorLocation = Hit.ImpactPoint;
			FVector CharacterLocation = GetActorLocation();
			float DistanceToCursor = FVector::Dist(CursorLocation, CharacterLocation);
			const float DistanceThreshold = 700.0f; 
			if (DistanceToCursor > DistanceThreshold)
			{
				TargetSpringArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, MaxSpringArmLength, DeltaTime, InterpSpeed);//+длина
			}
			else
			{
				TargetSpringArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, MinSpringArmLength, DeltaTime, InterpSpeed);//-длинна
			}
		}
		CameraBoom->TargetArmLength = TargetSpringArmLength;
	}
	if (!bIsAim)
	{
		CameraBoom->TargetArmLength = TargetSpringArmLength;
		TargetSpringArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, MinSpringArmLength, DeltaTime, InterpSpeed);//-длинна
	}
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

}

//void ATDSCharacter::StunEffect()
//{
//	if (StunAnim)
//	{
//		PlayAnimMontage(StunAnim, 1.0, NAME_None);
//	}
//}
//
//void ATDSCharacter::StunEffectEnd()
//{
//	StopAnimMontage(StunAnim);
//}

void ATDSCharacter::AddStamina_Implementation(float AddStamina)
{
	if (Stamina < 100)
	{
		Stamina = Stamina + AddStamina;
	}
}

