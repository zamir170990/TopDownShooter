// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_HP_Stamina.h"
# include "TDSCharacter.h"
#include "Kismet/GameplayStatics.h"

float UWidget_HP_Stamina::GetCharacterStamina()
{
	ATDSCharacter* OurPlayer = Cast<ATDSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	float GetStamina = OurPlayer->Stamina / 100.f;

	return GetStamina;
}

