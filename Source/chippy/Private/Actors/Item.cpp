#include <Actors/Item.h>

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	RootComponent = SphereComponent;

	AssignedProductMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Assigned Product Mesh"));
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
		ProductDynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		AssignedProductMesh->SetMaterial(0, ProductDynamicMaterial);
	}
}

/**
 * Initializes the item with product information
 * inAssignedProduct - Product information containing mesh, color and other properties
 */
void AItem::Init(FProductInfo inAssignedProduct)
{
	if (inAssignedProduct.Mesh)
	{
		AssignedProduct = inAssignedProduct;
		MaterialColor = inAssignedProduct.Color.RGBA;
		ProductAnimationAsset = AssignedProduct.Animation;
		ReplicatedSkeletalMesh = inAssignedProduct.Mesh.LoadSynchronous();

		AssignedProductMesh->SetSkeletalMeshAsset(AssignedProduct.Mesh.LoadSynchronous());
		AssignedProductMesh->PlayAnimation(AssignedProduct.Animation, true);

		if (ProductDynamicMaterial)
			ProductDynamicMaterial->SetVectorParameterValue("BaseColor", MaterialColor);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Assigned product mesh is not set!"));
	}
}

/**
 * Handles character interaction with the item
 * InteractingCharacter is the character that is interacting with this item
 */
void AItem::Interact(AchippyCharacter* InteractingCharacter)
{
	IInteractable::Interact(InteractingCharacter);

	if (!InteractingCharacter) return;
	AssignedProduct.OwnerCharacter = InteractingCharacter;
	InteractingCharacter->ChippyPlayAnimations(InteractAnimation);
}

// Display overlay material if the item is interactable
void AItem::ControlOverlayMaterial(bool inState)
{
	IInteractable::ControlOverlayMaterial(inState);
}

/**
 * Replication callback for color changes
 * Updates the material color when replicated from server to all clients
 */
void AItem::OnRep_Color()
{
	if (ProductDynamicMaterial)
	{
		ProductDynamicMaterial->SetVectorParameterValue("BaseColor", MaterialColor);
	}
}

void AItem::OnRep_Mesh()
{
	if (AssignedProductMesh)
	{
		AssignedProductMesh->SetSkeletalMeshAsset(ReplicatedSkeletalMesh);
		if (ProductAnimationAsset) AssignedProductMesh->PlayAnimation(ProductAnimationAsset, true);
	}
}

void AItem::MC_PlaySoundEffect_Implementation(USoundBase* SoundEffect)
{
	if (SoundEffect) UGameplayStatics::PlaySoundAtLocation(this, SoundEffect, GetActorLocation());
}


void AItem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItem, MaterialColor);
	DOREPLIFETIME(AItem, ReplicatedSkeletalMesh);
	DOREPLIFETIME(AItem, ProductAnimationAsset);
}
