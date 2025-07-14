// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/Interactable.h"

// Add default functionality here for any IInteractable functions that are not pure virtual.
// Handles general interaction
void IInteractable::Interact()
{
}

// Overload incase we need access to the interacting character
void IInteractable::Interact(AchippyCharacter* InteractingActor)
{
}

void IInteractable::ControlOverlayMaterial(bool inState)
{
}
