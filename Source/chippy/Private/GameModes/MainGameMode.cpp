// MainGameMode handles the core game's logic, including order generation and budget management
// It manages the flow of the game and coordinates player interactions


#include "GameModes/MainGameMode.h"

#include "Actors/DeliveryMachine.h"
#include "Actors/Machine.h"
#include "Structs/SProductInfo.h"

AMainGameMode::AMainGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Load data table rows and add them to the products' array
	ProductsDataTable.LoadSynchronous()->GetAllRows("", ProductsArray);

	//Timer for starting order generation
	FTimerHandle StartTimerHandle;
	GetWorldTimerManager().SetTimer(StartTimerHandle, this, &AMainGameMode::GenerateOrder, 3,
	                                false);
}

// Generates a random order with random product and color, then distributes it to all player controllers
void AMainGameMode::GenerateOrder()
{
	//Timer for loop until orders are less than the max order count
	if (ActiveOrders.Num() >= MaxActiveOrdersCount)
	{
		GetWorldTimerManager().SetTimer(LoopingTimerHandle, this, &AMainGameMode::GenerateOrder,
		                                1,
		                                true);
		return;
	}

	LoopingTimerHandle.Invalidate();

	if (ProductsArray.IsEmpty() || AvailableColors.IsEmpty()) return;
	//Get Random product data from the provided data table in blueprints
	int RandomProductIndex = FMath::RandRange(0, ProductsArray.Num() - 1);
	FName OrderName = FName(ProductsArray[RandomProductIndex]->Name);
	TSoftObjectPtr<USkeletalMesh> OrderMesh = ProductsArray[RandomProductIndex]->Mesh;
	TSoftObjectPtr<UTexture2D> OrderTexture = ProductsArray[RandomProductIndex]->Texture;

	//Get Random Color data from the provided array in blueprints
	int RandomColorIndex = FMath::RandRange(0, AvailableColors.Num() - 1);
	FString ColorName = AvailableColors[RandomColorIndex].Name;
	FColor OrderColor = AvailableColors[RandomColorIndex].RGBA;

	//Finalize order info
	FColorInfo ColorInfo(ColorName, OrderColor);
	FProductInfo OrderInfo(OrderName, OrderMesh, OrderTexture, ColorInfo, nullptr);

	//Generate random ID to easily find order later
	int OrderID = FMath::RandRange(1, 99999);

	ActiveOrders.Add(FOrderInfo(OrderInfo, OrderID));

	//Fire event to all controllers to add the order to HUD
	OnAddCardDelegate.Broadcast(OrderID, OrderInfo);

	//Timer for next order random generation
	FTimerHandle NextOrderTimerHandle;
	GetWorldTimerManager().SetTimer(NextOrderTimerHandle, this, &AMainGameMode::GenerateOrder,
	                                FMath::RandRange(MinOrderGenRange, MaxOrderGenRange),
	                                false);
}

//Check if a similar order is in the active orders array
void AMainGameMode::CheckOrderValidity(FProductInfo inOrderInfo, class ADeliveryMachine* Machine)
{
	float CurrentOrderPay = 0;
	bool IsTempOrderValid = false;

	for (int i = 0; i < ActiveOrders.Num(); i++)
	{
		//Checks if the product is similar in shape
		if (inOrderInfo.Name == ActiveOrders[i].Product.Name)
		{
			//Checks if the product is similar in color
			if (inOrderInfo.Color.RGBA == ActiveOrders[i].Product.Color.RGBA)
			{
				//Set order index in the array to remove it later
				TempOrderIndex = i;

				//Set full pay of the order to the one specified in the game mode blueprint
				CurrentOrderPay += OrderPay;
				//Order is valid, update budget
				UpdateBudget(CurrentOrderPay, ActiveOrders[i].ID);
				// Play VFX and SFX in current machine
				if (inOrderInfo.OwnerCharacter && Machine)
				{
					inOrderInfo.OwnerCharacter->ChippyPlayVFX(Machine->MachineVFXComponent);
					inOrderInfo.OwnerCharacter->ChippyPlaySFX(Machine->ValidOrderSoundEffect,
					                                          Machine->GetActorLocation());
				}
				return;
			}
			// Save a temp element (same-shape) in case we don't have a full match in current orders
			else if (!IsTempOrderValid)
			{
				// save the index of the current same-shape element to use it
				TempOrderIndex = i;
				// an element is already saved in case we don't find a match in color
				IsTempOrderValid = true;
			}
		}
	}

	if (IsTempOrderValid)
	{
		//Set half-pay of the order since only the shape is correct
		CurrentOrderPay += OrderPay / 2;
		//Order is half-valid, update budget
		UpdateBudget(CurrentOrderPay, ActiveOrders[TempOrderIndex].ID);
		//Play VFX and SFX in current machine
		if (inOrderInfo.OwnerCharacter && Machine)
			inOrderInfo.OwnerCharacter->ChippyPlayVFX(
				Machine->MachineVFXComponent);
		inOrderInfo.OwnerCharacter->ChippyPlaySFX(Machine->ValidOrderSoundEffect,
		                                          Machine->GetActorLocation());
	}
	else
	{
		//Handle on wrong order submitted
		if (inOrderInfo.OwnerCharacter && Machine)
		{
			OnWrongOrderDelegate.Broadcast();
			inOrderInfo.OwnerCharacter->ChippyPlaySFX(Machine->InvalidOrderSoundEffect,
			                                          Machine->GetActorLocation());
		}
	}
}

// Updates the player's budget based on order validity
void AMainGameMode::UpdateBudget(float CurrentOrderPay, int CurrentOrderKey)
{
	//Update total budget
	CurrentBudget += CurrentOrderPay;

	//fire event to all controllers to update order info
	OnProductDeliveryDelegate.Broadcast(CurrentOrderKey, CurrentOrderPay, CurrentBudget);

	//Remove order from active orders
	ActiveOrders.RemoveAt(TempOrderIndex);
}
