// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Item.h"
#include "Interfaces/Interactable.h"
#include "Product.generated.h"

DECLARE_DELEGATE(FProductReleased);

UCLASS()
class CHIPPY_API AProduct : public AItem
{
	GENERATED_BODY()

public:
	AProduct();
	virtual void BeginPlay() override;

	virtual void Init(FProductInfo inAssignedProduct, FColor inAssignedColor = FColor::White) override;

	FProductReleased ProductReleasedDelegate;

	virtual void Interact(AchippyCharacter* InteractingCharacter) override;

	UFUNCTION(NetMulticast, Unreliable)
	void MC_CarryProductEffects(bool inState);
};
