// Fill out your copyright notice in the Description page of Project Settings.


#include <Actors/Item.h>

#include "Net/UnrealNetwork.h"

// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	RootComponent = SphereComponent;

	AssignedProductMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Assigned Product Mesh"));
	AssignedProductMesh->SetupAttachment(RootComponent);

	bReplicates = true;
}


// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();


	UMaterialInterface* BaseMaterial = AssignedProductMesh->GetMaterial(0);
	if (BaseMaterial)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		AssignedProductMesh->SetMaterial(0, DynamicMaterial);
	}
}

void AItem::Init(FProductInfo inAssignedProduct)
{
	if (inAssignedProduct.Mesh)
	{
		AssignedProduct = inAssignedProduct;
		MaterialColor = inAssignedProduct.Color.RGBA;

		AssignedProductMesh->SetStaticMesh(AssignedProduct.Mesh.LoadSynchronous());

		if (DynamicMaterial)
			DynamicMaterial->SetVectorParameterValue("BaseColor", MaterialColor);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Assigned product mesh is not set!"));
	}
}

void AItem::Interact(AchippyCharacter* InteractingCharacter)
{
	IInteractable::Interact(InteractingCharacter);
}

void AItem::OnRep_Color()
{
	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue("BaseColor", MaterialColor);
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItem, MaterialColor);
}
