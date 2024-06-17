// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetHUD.h"
#include "Blueprint/UserWidget.h"

AWidgetHUD::AWidgetHUD() : Super()
{
	ConstructorHelpers::FClassFinder <UUserWidget> HP_Stamina(TEXT("/Game/UI/HP_Stamina"));
	MainHUD = HP_Stamina.Class;
}

void AWidgetHUD::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* CharacterWidget = CreateWidget(GetWorld(), MainHUD);
	CharacterWidget->AddToViewport();
}
