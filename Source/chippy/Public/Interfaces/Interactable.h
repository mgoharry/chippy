// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "chippy/chippyCharacter.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

class IInteractable
{
	GENERATED_BODY()

public:
	// Handles general interaction
	virtual void Interact();

	// Overload in case we need access to the interacting character
	virtual void Interact(AchippyCharacter* InteractingCharacter);

	virtual void ControlOverlayMaterial(bool inState);
};
