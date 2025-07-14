// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Product.h"

#include "ActorComponents/InteractionComponent.h"
#include "Kismet/GameplayStatics.h"

AProduct::AProduct()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AProduct::BeginPlay()
{
	Super::BeginPlay();

	//Set properties server side
	if (HasAuthority())
	{
		SphereComponent->SetSimulatePhysics(true);
		SphereComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
		SetActorEnableCollision(true);
	}

	if (AssignedProductMesh)
		AssignedProduct.Mesh = AssignedProductMesh->GetSkeletalMeshAsset();
}


// Inherits interaction functionality from Item and adds to it carry mechanic
void AProduct::Interact(AchippyCharacter* InteractingCharacter)
{
	AItem::Interact(InteractingCharacter);

	// Fires an event in the owning machine to handle product release from it
	ProductReleasedDelegate.ExecuteIfBound();

	// Initiate carrying mechanic
	InteractingCharacter->GetInteractionComponent()->CarryProduct(this);

	//Play carry-sfx on server and clients
	MC_PlaySoundEffect(CarrySoundEffect);
}

// Display overlay material if the item is interactable
void AProduct::ControlOverlayMaterial(bool inState)
{
	Super::ControlOverlayMaterial(inState);
	inState
		? AssignedProductMesh->SetOverlayMaterial(OverlayMaterialRef)
		: AssignedProductMesh->SetOverlayMaterial(nullptr);
}

void AProduct::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                     FVector NormalImpulse, const FHitResult& Hit)
{
	if (isDropped && HitSoundEffect)
	{
		//Play drop-sfx on server and clients
		MC_PlaySoundEffect(HitSoundEffect);
		isDropped = false;
	}
}

// Handles changes to products client-side
void AProduct::MC_CarryProductEffects_Implementation(bool inState)
{
	SetActorEnableCollision(inState);
	SphereComponent->SetSimulatePhysics(inState);
}
