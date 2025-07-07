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
	virtual void SpawnButtonsOnMachine() override;

	virtual void ModifyProduct(FProductInfo ProductToCreate) override;

	virtual void ClearProduct() override;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                            int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                          int32 OtherBodyIndex) override;
};
