// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WidgetHUD.generated.h"

/**
 * 
 */
UCLASS()
class TDS_API AWidgetHUD : public AHUD
{
	GENERATED_BODY()
	AWidgetHUD();
	virtual void BeginPlay() override;

	TSubclassOf<class UUserWidget> MainHUD;
};
