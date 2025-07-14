// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Structs/SProductInfo.h"
#include "Widgets/MainHUDWidget.h"
#include "ChippyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CHIPPY_API AChippyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Config")
	TSubclassOf<UMainHUDWidget> MainHUDWidgetClass;

	UPROPERTY()
	UMainHUDWidget* MainHUDWidget;

	UPROPERTY()
	class AMainGameMode* MainGameMode;

	//Accesses HUD to create and add a new order card widget
	UFUNCTION(Client, Unreliable)
	void AddOrderCard(int OrderID, FProductInfo OrderInfo);

	//Accesses the HUD when an order is completed and removes the associated order card
	UFUNCTION(Client, Unreliable)
	void UpdateOrderCard(int OrderID, float inOrderPay, float inTotalBudget);

	//Accesses HUD to view a wrong order animation
	UFUNCTION(Client, Unreliable)
	void WrongOrder();
};
