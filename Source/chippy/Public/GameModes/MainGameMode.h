// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PlayerControllers/ChippyPlayerController.h"
#include "MainGameMode.generated.h"

/**
 * Main game mode class for the game
 */
DECLARE_MULTICAST_DELEGATE(FOnWrongOrder);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAddCard, int, FProductInfo);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnProductDelivery, int, float, float);

// Struct to hold product data or order data including name, mesh, texture,
// color and current carrier

USTRUCT(BlueprintType)
struct FOrderInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FProductInfo Product;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int ID;
};

UCLASS()
class CHIPPY_API AMainGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	// Constructor
	AMainGameMode();

	//Check if a similar order is in the active orders array
	void CheckOrderValidity(FProductInfo inOrderInfo, class ADeliveryMachine* Machine);

	//Array to keep track of current player controllers
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Players")
	TArray<AChippyPlayerController*> PlayerControllers;

	//Array of available colors to paint and make orders of and send to assiociated machines
	UPROPERTY(EditDefaultsOnly, Category = "Machines", meta=(AllowPrivateAccess = true))
	TArray<FColorInfo> AvailableColors;

	//Array of available orders and product to make and send to assiociated machines
	UPROPERTY(EditDefaultsOnly, Category = "Machines", meta=(AllowPrivateAccess = true))
	TSoftObjectPtr<UDataTable> ProductsDataTable;

	//Delegate to tell all controllers to add an order card
	FOnAddCard OnAddCardDelegate;
	//Delegate to tell all controllers there was a wrong order submitted
	FOnWrongOrder OnWrongOrderDelegate;
	//Delegate to tell all controllers to update and remove an order card
	FOnProductDelivery OnProductDeliveryDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Generates a random order with random product and color, then distributes it to all player controllers
	UFUNCTION()
	void GenerateOrder();

	FTimerHandle LoopingTimerHandle;

	// Updates the player's budget based on order validity
	void UpdateBudget(float CurrentOrderPay, int CurrentOrderKey);

	//Array to fill with available products
	TArray<FProductInfo*> ProductsArray;

	//Array to keep track of current active orders keys
	TArray<FOrderInfo> ActiveOrders;

	// global variable to keep track of current temp order index
	int TempOrderIndex = 0;

	// Game mode settings //
	UPROPERTY(EditDefaultsOnly, Category = "Orders", meta=(AllowPrivateAccess = true))
	int MaxActiveOrdersCount = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Orders", meta=(AllowPrivateAccess = true))
	float CurrentBudget = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Orders", meta=(AllowPrivateAccess = true))
	float OrderPay = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Orders", meta=(AllowPrivateAccess = true))
	int MinOrderGenRange = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Orders", meta=(AllowPrivateAccess = true))
	int MaxOrderGenRange = 7;
};
