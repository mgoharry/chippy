// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OrderCardWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Structs/SProductInfo.h"
#include "MainHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class CHIPPY_API UMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere)
	UVerticalBox* CardsVerticalBox;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere)
	UTextBlock* CurrentBudgetText;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere)
	UTextBlock* BudgetEditText;

	//Order card widget to add to vertical box 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	TSubclassOf<UOrderCardWidget> OrderCardWidgetClass;

	//Array of current active orders
	UPROPERTY()
	TMap<int, UOrderCardWidget*> ActiveOrdersWidgets;

	//Creates and adds a new order card widget to the HUD
	void AddOrderCard(int OrderID, FProductInfo OrderInfo);

	//Updates the HUD when an order is completed and removes the associated order card
	void UpdateAndRemoveOrder(int OrderID, float inOrderPay, float inTotalBudget);

	//Blueprint native event for playing order payment animations
	UFUNCTION(BlueprintNativeEvent)
	void OrderPayAnimation(float inOrderPay);

	//Blueprint native event for playing order wrong animations
	UFUNCTION(BlueprintNativeEvent)
	void WrongOrderAnimation();
};
