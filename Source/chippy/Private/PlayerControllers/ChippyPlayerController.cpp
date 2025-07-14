// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllers/ChippyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "GameModes/MainGameMode.h"

void AChippyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//Get and assign game mode for later use
	MainGameMode = Cast<AMainGameMode>(GetWorld()->GetAuthGameMode());

	if (MainGameMode)
	{
		//Adds this controller to the list of current player controllers in game and remove from blueprints if disconnects
		MainGameMode->PlayerControllers.Add(this);

		//Fired from the game mode when it's time to add an order card
		MainGameMode->OnAddCardDelegate.AddUFunction(this, "AddOrderCard");

		//Fired from the game mode when it's time to update and remove an order card
		MainGameMode->OnProductDeliveryDelegate.AddUFunction(this, "UpdateOrderCard");

		//Fired from the game mode when a wrong order was submitted
		MainGameMode->OnWrongOrderDelegate.AddUFunction(this, "WrongOrder");
	}
	if (MainHUDWidgetClass && IsLocalController())
	{
		//Adds HUD widget to screen
		MainHUDWidget = CreateWidget<UMainHUDWidget>(this, MainHUDWidgetClass);
		MainHUDWidget->AddToViewport();
	}
}

/**
 * Accesses the HUD when a wrong order is submitted
 */
void AChippyPlayerController::WrongOrder_Implementation()
{
	if (MainHUDWidget)
	{
		MainHUDWidget->WrongOrderAnimation();
	}
}

/**
 * Accesses the HUD when an order is completed and removes the associated order card
 * @param OrderID - ID of the order to remove
 * @param inOrderPay - Payment amount for the completed order
 * @param inTotalBudget - Updated total budget after payment
 */
void AChippyPlayerController::UpdateOrderCard_Implementation(int OrderID, float inOrderPay, float inTotalBudget)
{
	if (MainHUDWidget)
	{
		MainHUDWidget->UpdateAndRemoveOrder(OrderID, inOrderPay, inTotalBudget);
	}
}


/**
 * Accesses HUD to create and add a new order card widget
 * @param OrderID - Unique ID for the order
 * @param OrderInfo - Product information to display on the card
 */
void AChippyPlayerController::AddOrderCard_Implementation(int OrderID, FProductInfo OrderInfo)
{
	if (MainHUDWidget)
	{
		MainHUDWidget->AddOrderCard(OrderID, OrderInfo);
	}
}
