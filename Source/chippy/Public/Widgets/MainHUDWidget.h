// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OrderCardWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "MainHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class CHIPPY_API UMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* CardsVerticalBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentBudgetText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* BudgetEditText;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	TSubclassOf<UOrderCardWidget> OrderCardWidgetClass;

	void AddOrderCard(FString Name, UTexture2D* Image, FColor Color);

	void RemoveOrderCard();
};
