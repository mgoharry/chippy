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


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	TSubclassOf<UOrderCardWidget> OrderCardWidgetClass;

	UPROPERTY()
	TMap<int, UOrderCardWidget*> ActiveOrdersWidgets;

	void AddOrderCard(int OrderID, FProductInfo OrderInfo);

	void UpdateAndRemoveOrder(int OrderID, float inOrderPay, float inTotalBudget);

	UFUNCTION(BlueprintNativeEvent)
	void OrderPayAnimation(float inOrderPay);
};
