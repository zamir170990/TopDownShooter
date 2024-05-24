// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_HP_Stamina.h"
#include "Kismet/GameplayStatics.h"
#include "TDSCharacter.h"

float UWidget_HP_Stamina::GetCharacterStamina()
{
	ATDSCharacter* OurPlayer = Cast<ATDSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	float GetStamina = OurPlayer->Stamina / 100.f;

	return GetStamina;
}

float UWidget_HP_Stamina::GetCharacterHP()
{
	ATDSCharacter* OurPlayer = Cast<ATDSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	float GetHP = OurPlayer->HP / 100.f;

	return GetHP;
}

