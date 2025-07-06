// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllers/ChippyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "GameModes/MainGameMode.h"

void AChippyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	MainGameMode = Cast<AMainGameMode>(GetWorld()->GetAuthGameMode());

	if (MainGameMode)
	{
		MainGameMode->PlayerControllers.Add(this);
	}
	if (MainHUDWidgetClass && IsLocalController())
	{
		MainHUDWidget = CreateWidget<UMainHUDWidget>(this, MainHUDWidgetClass);
		MainHUDWidget->AddToViewport();
	}
}

void AChippyPlayerController::AddOrderCard_Implementation(FName Name, UTexture2D* Image, FColor Color)
{
	if (MainHUDWidget)
	{
		MainHUDWidget->AddOrderCard(Name.ToString(), Image, Color);
	}
}
