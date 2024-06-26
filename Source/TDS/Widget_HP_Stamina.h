// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_HP_Stamina.generated.h"

class ATDSCharacter;

UCLASS()
class TDS_API UWidget_HP_Stamina : public UUserWidget
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetCharacterStamina();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetCharacterHP();

};
