// Fill out your copyright notice in the Description page of Project Settings.


#include "TDSGameModeBase.h"
#include "TDSPlayerController.h"
#include "TDSCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATDSGameModeBase::ATDSGameModeBase() : Super()
{
	PlayerControllerClass = ATDSPlayerController::StaticClass();

	ConstructorHelpers::FClassFinder<APawn>MainCharacterATDSCharacter(TEXT("/Game/Blueprint/Character/BP_Men"));
	if (MainCharacterATDSCharacter.Class != NULL)
	{
		DefaultPawnClass = MainCharacterATDSCharacter.Class;
	}


	DefaultPawnClass = MainCharacterATDSCharacter.Class;
	HUDClass = AWidgetHUD::StaticClass();
	
}
void ATDSGameModeBase::PlayerCharacterDead()
{
}
