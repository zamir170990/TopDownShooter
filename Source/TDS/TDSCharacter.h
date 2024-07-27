// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTypes.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "WeaponActor.h"
#include "Math/UnrealMathUtility.h"
#include "InventoryComponent.h"
#include "InteractInterface.h"
#include "TDS_IGameActor.h"
#include "TDSHealthCharacterComponent.h"
#include "TDS_StateEffect.h"
#include "TDSCharacter.generated.h"

class AWeaponActor;

UCLASS(Blueprintable)
class TDS_API ATDSCharacter : public ACharacter, public IInteractInterface, public ITDS_IGameActor
{
    GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	ATDSCharacter();

	FTimerHandle TimerHandle_RagDollTimer;

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	//FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* InventoryComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UTDSHealthCharacterComponent* HealthCharacterComponent;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;


	/** A decal that projects to the cursor location. */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class UDecalComponent* CursorToWorld;

public:
	//Cursor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);

	//Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	EMovementState MovementState = EMovementState::Run_State;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FCharacterSpeed MovementSpeedInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool SprintRunEnabled = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool WalkEnabled = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool AimEnabled = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	TSubclassOf<UTDS_StateEffect> AbilityEffect;
	//Weapon	
	AWeaponActor* CurrentWeapon = nullptr;
	UDecalComponent* CurrentCursor = nullptr;

	//Effect
	TArray<UTDS_StateEffect*> Effects;

	//Inputs
	UFUNCTION()
	void InputAxisY(float Value);
	UFUNCTION()
	void InputAxisX(float Value);
	UFUNCTION()
	void InputAttackPressed();
	UFUNCTION()
	void InputAttackReleased();

	float AxisX = 0.0f;
	float AxisY = 0.0f;

	// Tick Func
	UFUNCTION()
	void MovementTick(float DeltaTime);

	//Func
	UFUNCTION(BlueprintCallable)
	void AttackCharEvent(bool bIsFiring);
	UFUNCTION(BlueprintCallable)
	void CharacterUpdate();
	UFUNCTION(BlueprintCallable)
	void ChangeMovementState();

	UFUNCTION(BlueprintCallable)
	AWeaponActor* GetCurrentWeapon();
	UFUNCTION(BlueprintCallable)
	void InitWeapon(FName IdWeaponName, FAdditionalWeaponInfo WeaponAdditionalInfo, int32 NewCurrentIndexWeapon);
	UFUNCTION(BlueprintCallable)
	void RemoveCurrentWeapon();
	UFUNCTION(BlueprintCallable)
	void TryReloadWeapon();
	UFUNCTION()
	void WeaponReloadStart(UAnimMontage* Anim);
	UFUNCTION()
	void WeaponReloadEnd(bool bIsSuccess, int32 AmmoTake);
	UFUNCTION(BlueprintNativeEvent)
	void WeaponReloadStart_BP(UAnimMontage* Anim);
	UFUNCTION(BlueprintNativeEvent)
	void WeaponReloadEnd_BP(bool bIsSuccess);

	UFUNCTION()
	void WeaponFireStart(UAnimMontage* Anim);
	UFUNCTION(BlueprintNativeEvent)
	void WeaponFireStart_BP(UAnimMontage* Anim);

	UFUNCTION(BlueprintCallable)
	UDecalComponent* GetCursorToWorld();

    void Jump();
    void StopJump();
    void InputAim();
    void InputStopAim();
    void Sprint();
    void StopSprint();
	void TrySwitchNextWeapon();
	void TrySwitchPreviosWeapon();
	void TryAbilityEnabled();

   bool bIsSprint = false;


    /////////////////////////////////---WEAPON---////////////////////////////////////////////

    void AttachToAimSocket();
    void AttachToWeaponBagSocket();
    void AttachToWalkWeaponSocket();
    void AttachToFireSocket();
	void AttachToIdleWeaponSocket();
	void AttachToWeaponReloadSocket();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 CurrentIndexWeapon = 0;


    bool bIsAim = false;


   /////////////////////////////////---ANIMATION---////////////////////////////////////////////

    UPROPERTY(EditDefaultsOnly, Category = "FireAnimation")UAnimMontage* FireAnimation;
    UPROPERTY(EditDefaultsOnly, Category = "FireAnimation")UAnimMontage* AimAnimation;
    UPROPERTY(EditDefaultsOnly, Category = "FireAnimation")UAnimMontage* WalkWeaponAnimation;
	UPROPERTY(EditDefaultsOnly, Category = "FireAnimation")UAnimMontage* IdleWEaponAnimation;
	UPROPERTY(EditDefaultsOnly, Category = "FireAnimation")UAnimMontage* ReloadAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dead")TArray<UAnimMontage*> DeadsAnim;

	/*UPROPERTY(EditDefaultsOnly, Category = "FireAnimation")UAnimMontage* StunAnim;
	UFUNCTION(BlueprintNativeEvent)
	void StunEffect();
	UFUNCTION(BlueprintNativeEvent)
	void StunEffectEnd();*/

    /////////////////////////////////---HP---////////////////////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsAlive = true;
  

    /////////////////////////////////---STAMINA---////////////////////////////////////////////

    virtual void AddStamina_Implementation(float AddStamina)override;
    void Stamina_HPInfo(float DeltaTime);

    void DecreasedStamina();
    void IncreasedStamina();

    UPROPERTY(EditDefaultsOnly, Category = "Stamina")float CurrentStamina;
    UPROPERTY(EditDefaultsOnly, Category = "Stamina")float MinusStamina = 1.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Stamina")float PlusStamina = 1.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Stamina", meta = (ClampMin = "0", ClampMax = "100"))float Stamina = 100.0f;

   /////////////////////////////////---CAMERA_COMPONENT---////////////////////////////////////////////

    UPROPERTY(VisibleAnywhere) USpringArmComponent* SpringArm;
    UPROPERTY(VisibleAnywhere) UCameraComponent* Camera;

   UFUNCTION(BlueprintCallable) bool ForwardVectorsSprint();
 
    bool bIsCameraZoom = false;



    /////////////////////////////////---Movement"Speed"---////////////////////////////////////////////

   bool  CanSprintForward();

   /////////////////////////////////---Test---////////////////////////////////////////////

   bool bIsReloading = false;


   float TargetSpringArmLength = 800.0f;
   float MaxSpringArmLength = 1300.0f;
   float MinSpringArmLength = 800.0f;
   float InterpSpeed = 2.0f;
   void  UpdateSpringArmLengthForAim(float DeltaTime);

	FProjectileInfo GetWProjectile();
	UPROPERTY()FWeaponInfo WeaponSetting;


	//Interface
	EPhysicalSurface GetSurfuceType() override;
	TArray<UTDS_StateEffect*> GetAllCurrentEffects() override;
	void RemoveEffect(UTDS_StateEffect* RemoveEffect)override;
	void AddEffect(UTDS_StateEffect* newEffect)override;
	//End Interface

	UFUNCTION()
	void CharDead();
	void EnableRagdoll();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	//stun

};


