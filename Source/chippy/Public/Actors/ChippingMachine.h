// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Machine.h"
#include "ChippingMachine.generated.h"

/**
 * 
 */
UCLASS()
class CHIPPY_API AChippingMachine : public AMachine
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

private:
	// Spawns interactive buttons on the machine for product modification
	virtual void SpawnButtonsOnMachine() override;

	// Applies modifications to the current product based on provided info
	virtual void ModifyProduct(FProductInfo ProductToCreate) override;

	// start a timer to begin working on the assigned product
	virtual void StartWorkingOnProduct(FProductInfo ProductToCreate) override;

	// Removes the current product from the machine
	virtual void ClearProduct() override;

	// Handle actor beginning overlap with machine
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                            int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
