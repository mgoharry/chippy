// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ColoringMachine.h"

void AColoringMachine::BeginPlay()
{
	Super::BeginPlay();

	SpawnButtonsOnMachine();
}

void AColoringMachine::SpawnButtonsOnMachine()
{
	Super::SpawnButtonsOnMachine();

	if (HasAuthority() && !AvailableColors.IsEmpty())
	{
		// Spawn buttons on the server for each product in the data table

		for (int i = 0; i < AvailableColors.Num(); i++)
		{
			if (MachineButtonClass)
			{
				FVector Loc = ButtonsSpawnLocation->GetComponentLocation() + (GetActorForwardVector() * (i *
					ButtonsPadding));
				AMachineButton* ButtonMachine = GetWorld()->SpawnActor<AMachineButton>(
					MachineButtonClass, Loc, FRotator::ZeroRotator);

				ButtonMachine->Init(AvailableColors[i]);
				ButtonMachine->MachineButtonActivatedDelegate.BindUFunction(this, "ModifyProduct");
				ButtonMachine->MachineRef = this;
				MeshReceivedDelegate.AddUFunction(ButtonMachine,
				                                  "AssignProductMeshToPreview");
				MeshRemovedDelegate.AddUFunction(ButtonMachine,
				                                 "RemoveProductPreviewMesh");
			}

			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Emerald,
			                                 FString::Printf(
				                                 TEXT("%s button created!"), *AvailableColors[i].ToString()));
		}
	}
}

void AColoringMachine::ModifyProduct(FProductInfo ProductToCreate)
{
	Super::ModifyProduct(ProductToCreate);

	if (CurrentProductRef && HasAuthority() && GetMachineState() == EMachineState::EMS_Occupied)
	{
		CurrentProductRef->Init(ProductToCreate);

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Emerald,
		                                 FString::Printf(TEXT("%s Product is colored!"), *ProductToCreate.Name));
	}
}

void AColoringMachine::ClearProduct()
{
	Super::ClearProduct();
	MeshRemovedDelegate.Broadcast();
}

void AColoringMachine::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (CurrentProductRef)
	{
		MeshReceivedDelegate.Broadcast(CurrentProductRef->AssignedProduct.Mesh);
	}
}

void AColoringMachine::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);
}
