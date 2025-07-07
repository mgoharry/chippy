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
				ButtonMachine->SetButtonType(EButtonType::EBT_Color);
				ButtonMachine->Init(AvailableColors[i]);
				ButtonMachine->MachineButtonActivatedDelegate.BindUFunction(this, "ModifyProduct");
				ButtonMachine->MachineRef = this;
				MeshReceivedDelegate.AddUFunction(ButtonMachine,
				                                  "AssignProductMeshToPreview");
				MeshRemovedDelegate.AddUFunction(ButtonMachine,
				                                 "RemoveProductPreviewMesh");
			}
		}
	}
}

void AColoringMachine::ModifyProduct(FProductInfo ProductToCreate)
{
	Super::ModifyProduct(ProductToCreate);

	if (CurrentProductRef && HasAuthority() && GetMachineState() == EMachineState::EMS_Occupied)
	{
		CurrentProductRef->Init(ProductToCreate);
	}
}

void AColoringMachine::ClearProduct()
{
	Super::ClearProduct();
	
}

void AColoringMachine::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
}

void AColoringMachine::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);
}
