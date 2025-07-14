#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Structs/SProductInfo.h"
#include "Interfaces/Interactable.h"
#include "Item.generated.h"


// Represents an interactive item in the game world that can be picked up and used
// Handles mesh representation, material changes, and interaction logic

UCLASS()
class CHIPPY_API AItem : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Initialize the item with product information
	virtual void Init(FProductInfo inAssignedProduct);

	//Handle interaction with a character (IInteractable interface implementation)
	virtual void Interact(AchippyCharacter* InteractingCharacter) override;
	// Display overlay material if the item is interactable
	virtual void ControlOverlayMaterial(bool inState) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// Collision and mesh components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* AssignedProductMesh;

protected:
	// Material and mesh handling properties
	UPROPERTY()
	UMaterialInstanceDynamic* ProductDynamicMaterial;

	UPROPERTY(ReplicatedUsing = "OnRep_Color")
	FColor MaterialColor;

	UPROPERTY(ReplicatedUsing = "OnRep_Mesh")
	USkeletalMesh* ReplicatedSkeletalMesh;
	UPROPERTY(Replicated)
	UAnimationAsset* ProductAnimationAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UMaterialInterface* OverlayMaterialRef;

	//Replicate material changes to clients
	UFUNCTION()
	virtual void OnRep_Color();

	//Replicate mesh changes to clients
	UFUNCTION()
	virtual void OnRep_Mesh();

public:
	// Product data
	UPROPERTY()
	FProductInfo AssignedProduct;

	// Character Animation to play when they interact
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* InteractAnimation;

	//play sounds to clients
	UFUNCTION(NetMulticast, Unreliable)
	void MC_PlaySoundEffect(USoundBase* SoundEffect);
	// Returns SphereComponent
	FORCEINLINE USphereComponent* GetSphereComponent() const { return SphereComponent; }
};
