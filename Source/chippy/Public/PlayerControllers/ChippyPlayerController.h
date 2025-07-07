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

	UFUNCTION(Client, Unreliable)
	void AddOrderCard(int OrderID,FProductInfo OrderInfo);

	UFUNCTION(Client, Unreliable)
	void UpdateOrderCard(int OrderID, float inOrderPay, float inTotalBudget);
};
