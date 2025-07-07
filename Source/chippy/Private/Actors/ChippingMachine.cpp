// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ChippingMachine.h"
#include "Net/UnrealNetwork.h"

void AChippingMachine::BeginPlay()
{
	Super::BeginPlay();

	SpawnButtonsOnMachine();
}

void AChippingMachine::SpawnButtonsOnMachine()
{
	Super::SpawnButtonsOnMachine();

	if (HasAuthority() && ProductsDataTable)
	{
		// Spawn buttons on the server for each product in the data table
		TArray<FProductInfo*> Products;
		ProductsDataTable->GetAllRows("", Products);

		for (int i = 0; i < Products.Num(); i++)
		{
			if (MachineButtonClass)
			{
				FVector Loc = ButtonsSpawnLocation->GetComponentLocation() + (GetActorForwardVector() * (i *
					ButtonsPadding));
				AMachineButton* ButtonMachine = GetWorld()->SpawnActor<AMachineButton>(
					MachineButtonClass, Loc, FRotator::ZeroRotator);
				ButtonMachine->SetButtonType(EButtonType::EBT_Chip);
				ButtonMachine->Init(*Products[i]);
				ButtonMachine->MachineButtonActivatedDelegate.BindUFunction(this, "ModifyProduct");

				MeshReceivedDelegate.AddUFunction(ButtonMachine,
												  "AssignProductMeshToPreview");
				MeshRemovedDelegate.AddUFunction(ButtonMachine,
												 "RemoveProductPreviewMesh");
			}

			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Emerald,
			                                 FString::Printf(TEXT("%s button created!"), *Products[i]->Name));
		}
	}
}

void AChippingMachine::ModifyProduct(FProductInfo ProductToCreate)
{
	Super::ModifyProduct(ProductToCreate);

	if (CurrentProductRef && HasAuthority() && GetMachineState() == EMachineState::EMS_Occupied)
	{
		ProductToCreate.Color = CurrentProductRef->AssignedProduct.Color;
		CurrentProductRef->Init(ProductToCreate);
	}
}

void AChippingMachine::ClearProduct()
{
	Super::ClearProduct();
}

void AChippingMachine::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AChippingMachine::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);
}
