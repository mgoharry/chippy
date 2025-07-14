// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ChippingMachine.h"

void AChippingMachine::BeginPlay()
{
	Super::BeginPlay();

	// Spawns interactive buttons on the machine for product modification
	SpawnButtonsOnMachine();
}

// Spawn buttons on the server for each product in the data table
void AChippingMachine::SpawnButtonsOnMachine()
{
	Super::SpawnButtonsOnMachine();

	if (HasAuthority() && ProductsDataTable)
	{
		TArray<FProductInfo*> Products;
		ProductsDataTable->GetAllRows("", Products);

		for (int i = 0; i < Products.Num(); i++)
		{
			if (MachineButtonClass)
			{
				//Calculate location to spawn the button in
				FVector Loc = ButtonsSpawnLocation->GetComponentLocation() + (GetActorForwardVector() * (i *
					ButtonsPadding));

				//Spawns button on the calculated location
				AMachineButton* ButtonMachine = GetWorld()->SpawnActor<AMachineButton>(
					MachineButtonClass, Loc, FRotator::ZeroRotator);

				//Sets the button's type for later customization
				ButtonMachine->SetButtonType(EButtonType::EBT_Chip);

				//initializes button with the provided data
				ButtonMachine->Init(*Products[i]);

				//Binds the button's activation delegate to the ModifyProduct function, which will be called when the button is pressed
				ButtonMachine->MachineButtonActivatedDelegate.BindUFunction(this, "StartWorkingOnProduct");


				//Binds delegates for when the product is assigned to / removed from the machine for visual representation on buttons
				MeshReceivedDelegate.AddUFunction(ButtonMachine,
				                                  "AssignProductMeshToPreview");
				MeshRemovedDelegate.AddUFunction(ButtonMachine,
				                                 "RemoveProductPreviewMesh");
			}
		}
	}
}

// start a timer to begin working on the assigned product
void AChippingMachine::StartWorkingOnProduct(FProductInfo ProductToCreate)
{
	Super::StartWorkingOnProduct(ProductToCreate);
	//check if we're on server and the machine is occupied with a product
	if (CurrentProductRef && HasAuthority() && GetMachineState() == EMachineState::EMS_Occupied)
	{
		//Timer to simulate the machine working on a product
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, "ModifyProduct", ProductToCreate);
		GetWorldTimerManager().SetTimer(StartProcessTimerHandle, TimerDelegate, ProductProcessTime, false);

		// Updates the machine's current state and triggers related events
		SetMachineState(EMachineState::EMS_Working);
	}
}

// Applies modifications to the current product based on provided info
void AChippingMachine::ModifyProduct(FProductInfo ProductToCreate)
{
	Super::ModifyProduct(ProductToCreate);

	if (!CurrentProductRef) return;
	//Keeps the product color without changing it
	ProductToCreate.Color = CurrentProductRef->AssignedProduct.Color;

	//initializes the product with the provided data from the clicked button
	CurrentProductRef->Init(ProductToCreate);

	//Play finish-sfx on server and clients
	CurrentProductRef->MC_PlaySoundEffect(FinishedSoundEffect);

	// Updates the machine's current state and triggers related events
	SetMachineState(EMachineState::EMS_Done);
}


// Removes the current product from the machine
void AChippingMachine::ClearProduct()
{
	Super::ClearProduct();
}

// Handle actor beginning overlap with machine
void AChippingMachine::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}
