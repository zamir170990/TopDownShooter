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

protected:
	virtual void BeginPlay() override;

public:
	ATDSCharacter();

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	//FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

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

	//Weapon	
	AWeaponActor* CurrentWeapon = nullptr;

	//for demo 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demo")
	FName InitWeaponName;

	UDecalComponent* CurrentCursor = nullptr;

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
	void InitWeapon(FName IdWeaponName);
	UFUNCTION(BlueprintCallable)
	void TryReloadWeapon();
	UFUNCTION()
	void WeaponReloadStart(UAnimMontage* Anim);
	UFUNCTION()
	void WeaponReloadEnd();
	UFUNCTION(BlueprintNativeEvent)
	void WeaponReloadStart_BP(UAnimMontage* Anim);
	UFUNCTION(BlueprintNativeEvent)
	void WeaponReloadEnd_BP();

	UFUNCTION(BlueprintCallable)
	UDecalComponent* GetCursorToWorld();

    void Jump();
    void StopJump();
    void InputAim();
    void InputStopAim();
    void Sprint();
    void StopSprint();

   bool bIsSprint = false;


    /////////////////////////////////---WEAPON---////////////////////////////////////////////

    void AttachToAimSocket();
    void AttachToWeaponBagSocket();
    void AttachToWalkWeaponSocket();
    void AttachToFireSocket();
	void AttachToIdleWeaponSocket();
	void AttachToWeaponReloadSocket();

    bool bIsAim = false;


   /////////////////////////////////---ANIMATION---////////////////////////////////////////////

    UPROPERTY(EditDefaultsOnly, Category = "FireAnimation")UAnimMontage* FireAnimation;
    UPROPERTY(EditDefaultsOnly, Category = "FireAnimation")UAnimMontage* AimAnimation;
    UPROPERTY(EditDefaultsOnly, Category = "FireAnimation")UAnimMontage* WalkWeaponAnimation;
	UPROPERTY(EditDefaultsOnly, Category = "FireAnimation")UAnimMontage* IdleWEaponAnimation;
	UPROPERTY(EditDefaultsOnly, Category = "FireAnimation")UAnimMontage* ReloadAnimation;

    /////////////////////////////////---HP---////////////////////////////////////////////

    virtual void AddHP_Implementation(float AddHP)override;

    void DecreasedHP();
    void IncreasedHP();

    UPROPERTY(EditDefaultsOnly, Category = "HP")float CurrentHP;
    UPROPERTY(EditDefaultsOnly, Category = "HP")float MinusHP = 1.0f;
    UPROPERTY(EditDefaultsOnly, Category = "HP")float PlusHP = 1.0f;
    UPROPERTY(EditDefaultsOnly, Category = "HP", meta = (ClampMin = "0", ClampMax = "100"))float HP = 100.0f;

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

    void AimSpringArmLength(float DeltaTime);
    //void MovementTick(float Default);

    float AimCurrentSpringArmLength = 800.0f;
    float AimTargetSpringArmLength = 800.0f;
    float AimTransitionSpeed = 1.0f;
 
    bool bIsCameraZomm;

    /////////////////////////////////---Movement"Speed"---////////////////////////////////////////////

   bool  CanSprintForward();

   /////////////////////////////////---Test---////////////////////////////////////////////

   bool bIsReloading = false;

};


