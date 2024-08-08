
#include "TDSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "TDSGameInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Actor.h"
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

	EnergyFieldComponent = CreateDefaultSubobject<USphereComponent>(TEXT("EnergyFieldComponent"));
	EnergyFieldComponent->InitSphereRadius(200.0f);
	EnergyFieldComponent->SetCollisionProfileName(TEXT("OverlapAll"));
	EnergyFieldComponent->SetupAttachment(RootComponent);

	EnergyFieldComponent->OnComponentBeginOverlap.AddDynamic(this, &ATDSCharacter::OnEnergyFieldOverlap);
	////////////Stamina
	MaxStamina = 100;
	CurrentStamina = MaxStamina;
	StaminaRegen = false;
	StaminaDrain = false;
}

void ATDSCharacter::OnEnergyFieldOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		UTDSHealthComponent* HealthComp = Cast<UTDSHealthComponent>(OtherActor->GetComponentByClass(UTDSHealthComponent::StaticClass()));
		if (HealthComp)
		{
			//Урон наносится в Функции StateEffect
		}
	}
}

void ATDSCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	MovementTick(DeltaSeconds);
	UpdateSpringArmLengthForAim(DeltaSeconds);
	if (SprintRunEnabled)
	{
		if (!ForwardVectorsSprint())
		{
			StopSprint();// Если персонаж больше не может бежать, остановить спринт
		}
		if (ForwardVectorsSprint())
		{
			Sprint();//Услм персонаж может бежать по данному радиусу то продолжает бег
		}
	}


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
}

void ATDSCharacter::BeginPlay()
{
	Super::BeginPlay();
	//AttachToWeaponBagSocket();
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
	NewInputComponent->BindAction(TEXT("DropCurrentWeapon"), EInputEvent::IE_Pressed, this, &ATDSCharacter::DropCurrentWeapon);

	TArray<FKey> HotKeys;
	HotKeys.Add(EKeys::One);
	HotKeys.Add(EKeys::Two);
	HotKeys.Add(EKeys::Three);
	HotKeys.Add(EKeys::Four);
	HotKeys.Add(EKeys::Five);
	HotKeys.Add(EKeys::Six);
	HotKeys.Add(EKeys::Seven);
	HotKeys.Add(EKeys::Eight);
	HotKeys.Add(EKeys::Nine);
	HotKeys.Add(EKeys::Zero);

	NewInputComponent->BindKey(HotKeys[1], IE_Pressed, this, &ATDSCharacter::TKeyPressed<1>);
	NewInputComponent->BindKey(HotKeys[2], IE_Pressed, this, &ATDSCharacter::TKeyPressed<2>);
	NewInputComponent->BindKey(HotKeys[3], IE_Pressed, this, &ATDSCharacter::TKeyPressed<3>);
	NewInputComponent->BindKey(HotKeys[4], IE_Pressed, this, &ATDSCharacter::TKeyPressed<4>);
	NewInputComponent->BindKey(HotKeys[5], IE_Pressed, this, &ATDSCharacter::TKeyPressed<5>);
	NewInputComponent->BindKey(HotKeys[6], IE_Pressed, this, &ATDSCharacter::TKeyPressed<6>);
	NewInputComponent->BindKey(HotKeys[7], IE_Pressed, this, &ATDSCharacter::TKeyPressed<7>);
	NewInputComponent->BindKey(HotKeys[8], IE_Pressed, this, &ATDSCharacter::TKeyPressed<8>);
	NewInputComponent->BindKey(HotKeys[9], IE_Pressed, this, &ATDSCharacter::TKeyPressed<9>);
	NewInputComponent->BindKey(HotKeys[0], IE_Pressed, this, &ATDSCharacter::TKeyPressed<0>);
}

void ATDSCharacter::InputWalkPressed()
{
	WalkEnabled = true;
	ChangeMovementState();
}

void ATDSCharacter::InputWalkReleased()
{
	WalkEnabled = false;
	ChangeMovementState();
}

void ATDSCharacter::InputAim()
{
	AimEnabled = true;
	ChangeMovementState();
	if (CurrentWeapon && !bIsReloading)
	{
		//AttachToAimSocket();
		if (AimAnimation)
		{
			PlayAnimMontage(AimAnimation, 1.0, NAME_None);
		}
		MovementState = EMovementState::Aim_State;
		CharacterUpdate();
	}
		bIsAim = true;
		bIsCameraZoom = true;
}

void ATDSCharacter::InputStopAim()
{
	AimEnabled = false;
	ChangeMovementState();
	if (CurrentWeapon && !bIsReloading)
	{
		bIsAim = false;
		StopAnimMontage(AimAnimation);
		//AttachToIdleWeaponSocket();
		PlayAnimMontage(IdleWEaponAnimation, 1.0, NAME_None);
	    GetCharacterMovement()->MaxWalkSpeed = MovementSpeedInfo.WalkSpeedNormal;
		CharacterUpdate();
	}
		bIsCameraZoom = false;
}

void ATDSCharacter::InputAttackPressed()
{
	if (CurrentWeapon && bIsAlive && !bIsReloading && bIsAim)
	{
		AttackCharEvent(true);
		//AttachToFireSocket();
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
		//AttachToAimSocket();
		StopAnimMontage(FireAnimation);
		PlayAnimMontage(AimAnimation, 1.0, NAME_None);
		MovementState = EMovementState::Aim_State;
		CharacterUpdate();
	}
}


void ATDSCharacter::Sprint()
{
    if (CurrentStamina <= 0)
    {
		   return;
	}
	StaminaIncrease();
	TimerHandles();
	SprintRunEnabled = true;
	ChangeMovementState();
	if (!bIsReloading)
	{
		//AttachToWeaponBagSocket();
		StopAnimMontage();
		if (CanSprintForward())
		{
			bIsSprint = true;
		}
		MovementState = EMovementState::Run_State;
		CharacterUpdate();
	}
}

void ATDSCharacter::StopSprint()
{
	StaminaDrain = false;
	StaminaRegen = true;
	TimerHandles();
	SprintRunEnabled = false;
	ChangeMovementState();
	bIsSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeedInfo.WalkSpeedNormal;
}

bool ATDSCharacter::CanSprintForward()
{
	return ForwardVectorsSprint();
}

void ATDSCharacter::TrySwitchNextWeapon()
{
	if (CurrentWeapon && !CurrentWeapon->WeaponReloading && InventoryComponent->WeaponSlots.Num() > 1)
	{
		//We have more then one weapon go switch
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
			if (InventoryComponent->SwitchWeaponToIndexByNextPreviosIndex(CurrentIndexWeapon + 1, OldIndex, OldInfo, true))
			{ }
		}
	}
}

void ATDSCharacter::TrySwitchPreviosWeapon()
{
	if (CurrentWeapon && !CurrentWeapon->WeaponReloading && InventoryComponent->WeaponSlots.Num() > 1)
	{
		//We have more then one weapon go switch
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
			//InventoryComponent->SetAdditionalInfoWeapon(OldIndex, GetCurrentWeapon()->AdditionalWeaponInfo);
			if (InventoryComponent->SwitchWeaponToIndexByNextPreviosIndex(CurrentIndexWeapon - 1, OldIndex, OldInfo, false))
			{ }
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

EMovementState ATDSCharacter::GetMovementState()
{
	return MovementState;
}

TArray<UTDS_StateEffect*> ATDSCharacter::GetCurrentEffectsOnChar()
{
	return Effects;
}

int32 ATDSCharacter::GetCurrentWeaponIndex()
{
	return CurrentIndexWeapon;
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

AWeaponActor* ATDSCharacter::GetCurrentWeapon() const 
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
				FVector SpawnLocation = FVector(0);
				FRotator SpawnRotation = FRotator(0);

				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = GetInstigator();

				AWeaponActor* myWeapon = Cast<AWeaponActor>(GetWorld()->SpawnActor(myWeaponInfo.WeaponClass, &SpawnLocation, &SpawnRotation, SpawnParams));
				if (myWeapon)
				{
					FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);
					myWeapon->AttachToComponent(GetMesh(), Rule, FName("AimSocket"));
					CurrentWeapon = myWeapon;

					myWeapon->IdWeaponName = IdWeaponName;
					myWeapon->WeaponSetting = myWeaponInfo;


					myWeapon->ReloadTime = myWeaponInfo.ReloadTime;
					myWeapon->UpdateStateWeapon(MovementState);

					myWeapon->AdditionalWeaponInfo = WeaponAdditionalInfo;

					CurrentIndexWeapon = NewCurrentIndexWeapon;

					myWeapon->OnWeaponReloadStart.AddDynamic(this, &ATDSCharacter::WeaponReloadStart);
					myWeapon->OnWeaponReloadEnd.AddDynamic(this, &ATDSCharacter::WeaponReloadEnd);

					myWeapon->OnWeaponFireStart.AddDynamic(this, &ATDSCharacter::WeaponFireStart);

					// after switch try reload weapon if needed
					if (CurrentWeapon->GetWeaponRound() <= 0 && CurrentWeapon->CheckCanWeaponReload())
						CurrentWeapon->InitReload();

					if (InventoryComponent)
						InventoryComponent->OnWeaponAmmoAviable.Broadcast(myWeapon->WeaponSetting.WeaponType);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ATPSCharacter::InitWeapon - Weapon not found in table -NULL"));
		}
	}
}

void ATDSCharacter::TryReloadWeapon()
{
	if (bIsAlive && CurrentWeapon && !CurrentWeapon->WeaponReloading)
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
 			//AttachToWeaponReloadSocket();
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

bool ATDSCharacter::TrySwitchWeaponToIndexByKeyInput(int32 ToIndex)
{
	bool bIsSuccess = false;
	if (CurrentWeapon && !CurrentWeapon->WeaponReloading && InventoryComponent->WeaponSlots.IsValidIndex(ToIndex))
	{
		if (CurrentIndexWeapon != ToIndex && InventoryComponent)
		{
			int32 OldIndex = CurrentIndexWeapon;
			FAdditionalWeaponInfo OldInfo;

			if (CurrentWeapon)
			{
				OldInfo = CurrentWeapon->AdditionalWeaponInfo;
				if (CurrentWeapon->WeaponReloading)
					CurrentWeapon->CancelReload();
			}
			bIsSuccess = InventoryComponent->SwitchWeaponByIndex(ToIndex, OldIndex, OldInfo);
		}
	}
	return bIsSuccess;
}

void ATDSCharacter::DropCurrentWeapon()
{
	if (InventoryComponent)
	{
		FDropItem ItemInfo;
		InventoryComponent->DropWeapobByIndex(CurrentIndexWeapon, ItemInfo);
	}
	//AttachToIdleWeaponSocket();
}

void ATDSCharacter::CharDead_BP_Implementation()
{
	// in BP
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

//void ATDSCharacter::AttachToAimSocket()
//{
//	bIsAim = true;
//	if (CurrentWeapon)
//		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
//		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("AimSocket"));
//}
//
//void ATDSCharacter::AttachToWalkWeaponSocket()
//{
//	if (CurrentWeapon)
//		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
//		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("WalkWeaponSocket"));
//}
//
//void ATDSCharacter::AttachToFireSocket()
//{
//	if (CurrentWeapon)
//		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
//		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("FireSocket"));
//}
//
//void ATDSCharacter::AttachToIdleWeaponSocket()
//{
//	if (CurrentWeapon)
//		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
//		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("IdleWalkSocket"));
//}
//
//void ATDSCharacter::AttachToWeaponBagSocket()
//{
//	if (CurrentWeapon)
//		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
//		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("Bag"));
//}
//
//void ATDSCharacter::AttachToWeaponReloadSocket()
//{
//	if (CurrentWeapon)
//		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
//	CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("ReloadSocket"));
//}

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

	if (GetController())
	{
		GetController()->UnPossess();
	}
	UnPossessed();
	//GetWorldTimerManager().SetTimer(TimerHandle_RagDollTimer, this, &ATDSCharacter::EnableRagdoll, TimeAnim, false);
	GetCursorToWorld()->SetVisibility(false);
	AttackCharEvent(false);
	CharDead_BP();
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
/////////////////Stamina

void ATDSCharacter::TimerHandles()
{
	GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimerHandle, this, &ATDSCharacter::StaminaReduce, 0.01f, StaminaRegen);
	GetWorld()->GetTimerManager().SetTimer(StaminaDraiTimerHandle, this, &ATDSCharacter::StaminaIncrease, 0.01f, StaminaDrain);
}

void ATDSCharacter::StaminaReduce()
{
	if (CurrentStamina < MaxStamina)
	{
		CurrentStamina += 0.05;
	}
	else
		StaminaRegen = false;
}

void ATDSCharacter::StaminaIncrease()
{
	if (CurrentStamina > 0)
	{
		CurrentStamina -= 0.1;
	}
	else
	{
		StaminaDrain = false;
		StopSprint();
	}
}