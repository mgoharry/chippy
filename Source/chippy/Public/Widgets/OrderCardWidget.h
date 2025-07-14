// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "OrderCardWidget.generated.h"

/**
 * 
 */
UCLASS()
class CHIPPY_API UOrderCardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UImage* ProductImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ProductInfo;

	//bp native event to remove card and play animation
	UFUNCTION(BlueprintNativeEvent)
	void BP_RemoveCard();
};
