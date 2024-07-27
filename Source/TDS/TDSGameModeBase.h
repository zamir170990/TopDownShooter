// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WidgetHUD.h"
#include "TDSGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TDS_API ATDSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	ATDSGameModeBase();

	void PlayerCharacterDead();
};
