// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/GameModeBase.h"
#include "PlayerControllers/ChippyPlayerController.h"
#include "MainGameMode.generated.h"

/**
 * Main game mode class for the game
 */
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAddCard, int, FProductInfo);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnProductDelivery, int, float, float);

UCLASS()
class CHIPPY_API AMainGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	// Constructor
	AMainGameMode();

	FOnAddCard OnAddCardDelegate;
	FOnProductDelivery OnProductDeliveryDelegate;

	UPROPERTY()
	TArray<AChippyPlayerController*> PlayerControllers;

	TMap<int, FProductInfo> ActiveOrders;

	UPROPERTY(EditDefaultsOnly, Category = "Machine")
	TArray<FColorInfo> AvailableColors;

	UPROPERTY(EditDefaultsOnly, Category = "Machine")
	TObjectPtr<UDataTable> ProductsDataTable;

	void CheckOrderValidity(FProductInfo inOrderInfo);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//UPROPERTY(Replicated)
	float CurrentBudget = 1000.f;

	float OrderPay = 100.f;

	void GenerateOrder();

	void UpdateBudget();

	TArray<FProductInfo*> ProductsArray;

	TArray<FString*> ColorKeys;
};
