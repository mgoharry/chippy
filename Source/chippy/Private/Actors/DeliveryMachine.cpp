// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/DeliveryMachine.h"

void ADeliveryMachine::BeginPlay()
{
	Super::BeginPlay();
}

// Handle actor beginning overlap with machine
void ADeliveryMachine::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (HasAuthority() && CurrentProductRef)
	{
		if (MainGameMode)
		{
			
			//destroys the current product and sends order details to game mode to check validity
			MainGameMode->CheckOrderValidity(CurrentProductRef->AssignedProduct, this);
			CurrentProductRef->Destroy();
			CurrentProductRef = nullptr;
			//change the machine state to be available for other products
			SetMachineState(EMachineState::EMS_Idle);
		}
	}
}
