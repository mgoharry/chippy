// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Product.h"

#include "SkeletonTreeBuilder.h"
#include "ActorComponents/InteractionComponent.h"

// Sets default values
AProduct::AProduct()
{
	PrimaryActorTick.bCanEverTick = true;

	if (HasAuthority())
	{
		SphereComponent->SetSimulatePhysics(true);
		SetActorEnableCollision(true);
	}
}

void AProduct::BeginPlay()
{
	Super::BeginPlay();
	AssignedProduct.Mesh = AssignedProductMesh->GetStaticMesh();
}

void AProduct::Init(FProductInfo inAssignedProduct)
{
	Super::Init(inAssignedProduct);
}


void AProduct::Interact(AchippyCharacter* InteractingCharacter)
{
	IInteractable::Interact(InteractingCharacter);

	ProductReleasedDelegate.ExecuteIfBound();
	InteractingCharacter->GetInteractionComponent()->CarryProduct(this);
}

void AProduct::MC_CarryProductEffects_Implementation(bool inState)
{
	SetActorEnableCollision(inState);
	SphereComponent->SetSimulatePhysics(inState);
}
