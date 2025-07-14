// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Item.h"
#include "Product.generated.h"

DECLARE_DELEGATE(FProductReleased);

UCLASS()
class CHIPPY_API AProduct : public AItem
{
	GENERATED_BODY()

public:
	AProduct();
	virtual void BeginPlay() override;

	// Fires an event in the owning machine to handle product release from it
	FProductReleased ProductReleasedDelegate;

	//Inherits interaction functionality from Item and adds to it carry mechanic
	virtual void Interact(AchippyCharacter* InteractingCharacter) override;

	//Handles changes to products client-side
	UFUNCTION(NetMulticast, Unreliable)
	void MC_CarryProductEffects(bool inState);

	// Display overlay material if the item is interactable
	virtual void ControlOverlayMaterial(bool inState) override;

	//on actor hit function
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	           const FHitResult& Hit);

	bool isDropped = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USoundBase* HitSoundEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USoundBase* CarrySoundEffect;
};
