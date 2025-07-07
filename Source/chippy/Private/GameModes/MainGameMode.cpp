// MainGameMode handles the core game logic including order generation and budget management
// It manages the flow of the game and coordinates player interactions


#include "GameModes/MainGameMode.h"

#include "Structs/SProductInfo.h"

AMainGameMode::AMainGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}


void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	ProductsDataTable->GetAllRows("", ProductsArray);

	//Timer for random order genration
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMainGameMode::GenerateOrder, 5.0f, true);
}

// Generates a random order with random product and color, then distributes it to all player controllers
void AMainGameMode::GenerateOrder()
{
	if (ActiveOrders.Num() >= 5) return;

	//Get Random product data from bp provided data table
	int RandomProductIndex = FMath::RandRange(0, ProductsArray.Num() - 1);
	FName OrderName = FName(ProductsArray[RandomProductIndex]->Name);
	TSoftObjectPtr<UStaticMesh> OrderMesh = ProductsArray[RandomProductIndex]->Mesh;
	TSoftObjectPtr<UTexture2D> OrderTexture = ProductsArray[RandomProductIndex]->Texture;

	//Get Random Color data from bp provided array
	int RandomColorIndex = FMath::RandRange(0, AvailableColors.Num() - 1);
	FString ColorName = AvailableColors[RandomColorIndex].Name;
	FColor OrderColor = AvailableColors[RandomColorIndex].RGBA;

	FColorInfo ColorInfo(ColorName, OrderColor);
	FProductInfo OrderInfo(OrderName, OrderMesh, OrderTexture, ColorInfo);

	int OrderID = FMath::RandRange(1, 99999);
	ActiveOrders.Add(OrderID, OrderInfo);

	OnAddCardDelegate.Broadcast(OrderID, OrderInfo);
}

void AMainGameMode::CheckOrderValidity(FProductInfo inOrderInfo)
{
	float CurrentOrderPay = 0;
	for (auto ActiveOrder : ActiveOrders)
	{
		if (inOrderInfo.Name == ActiveOrder.Value.Name)
		{
			CurrentOrderPay += OrderPay / 2;


			if (inOrderInfo.Color.RGBA == ActiveOrder.Value.Color.RGBA)
				CurrentOrderPay += OrderPay / 2;

			//Order is valid, update budget
			UpdateBudget();
			//Remove order from active orders
			CurrentBudget += CurrentOrderPay;
			OnProductDeliveryDelegate.Broadcast(ActiveOrder.Key, CurrentOrderPay, CurrentBudget);
			ActiveOrders.Remove(ActiveOrder.Key);
			break;
		}
	}
}


// Updates the player's budget based on order validity
void AMainGameMode::UpdateBudget()
{
}
