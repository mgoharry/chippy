// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Machine.h"
#include "DeliveryMachine.generated.h"

/**
 * 
 */
UCLASS()
class CHIPPY_API ADeliveryMachine : public AMachine
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	// Handle actor beginning overlap with machine
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                            int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	USoundBase* ValidOrderSoundEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	USoundBase* InvalidOrderSoundEffect;
};
