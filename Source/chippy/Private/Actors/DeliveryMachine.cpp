// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/DeliveryMachine.h"

void ADeliveryMachine::BeginPlay()
{
	Super::BeginPlay();
}

void ADeliveryMachine::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (CurrentProductRef && HasAuthority())
	{
		if (MainGameMode)
		{
			MainGameMode->CheckOrderValidity(CurrentProductRef->AssignedProduct);
			CurrentProductRef->Destroy();
			CurrentProductRef = nullptr;
			SetMachineState(EMachineState::EMS_Idle);
		}
	}
}
