// Fill out your copyright notice in the Description page of Project Settings.


#include "TDSGameModeBase.h"

ATDSGameModeBase::ATDSGameModeBase() : Super()
{

	ConstructorHelpers::FClassFinder<APawn>MainCharacterATDSCharacter(TEXT("/Game/Blueprint/Character/BP_Men"));

	DefaultPawnClass = MainCharacterATDSCharacter.Class;
	HUDClass = AWidgetHUD::StaticClass();
	
} 