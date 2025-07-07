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
		MainGameMode->OnAddCardDelegate.AddUFunction(this, "AddOrderCard");
		MainGameMode->OnProductDeliveryDelegate.AddUFunction(this, "UpdateOrderCard");
	}
	if (MainHUDWidgetClass && IsLocalController())
	{
		MainHUDWidget = CreateWidget<UMainHUDWidget>(this, MainHUDWidgetClass);
		MainHUDWidget->AddToViewport();
	}
}


void AChippyPlayerController::UpdateOrderCard_Implementation(int OrderID, float inOrderPay, float inTotalBudget)
{
	if (MainHUDWidget)
	{
		MainHUDWidget->UpdateAndRemoveOrder(OrderID, inOrderPay, inTotalBudget);
	}
}


void AChippyPlayerController::AddOrderCard_Implementation(int OrderID, FProductInfo OrderInfo)
{
	if (MainHUDWidget)
	{
		MainHUDWidget->AddOrderCard(OrderID, OrderInfo);
	}
}
