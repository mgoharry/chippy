// Component that handles interaction with interactable objects and product carrying mechanics

#pragma once

#include "CoreMinimal.h"
#include "Actors/Product.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "Interfaces/Interactable.h"
#include "InteractionComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CHIPPY_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// Initiates interaction with currently interactable actor
	UFUNCTION()
	void Interact();

	// Performs line trace to detect interactable objects
	UFUNCTION()
	void PerformLineTrace();

	// Product carrying handler
	void CarryProduct(AProduct* Product);

	// object dropping handler
	void DropProduct();

protected:

	//clear reference of previously interactable object
	void ClearInteractableRef();
	// Server-side interaction handler
	UFUNCTION(Server, unreliable)
	void SER_Interact(AActor* inInteractableActor);

	// Server implementation for dropping carried product with physics impulse
	UFUNCTION(Server, unreliable)
	void SER_DropProduct();

	//Checks if player is already carrying an object
	UPROPERTY(Replicated)
	bool isCarryingProduct = false;

	//Reference to currently carried product
	UPROPERTY(Replicated)
	AProduct* CarriedProduct;

	// owning actor's references
	UPROPERTY()
	AchippyCharacter* OwnerCharacter;

	UPROPERTY()
	UCameraComponent* CameraComponent;

	// Currently line-traced object properties
	UPROPERTY()
	AActor* OverlappingActor;

	UPROPERTY()
	TScriptInterface<IInteractable> InteractableInterface;

	// adjustable data from blueprints
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float TraceDistance = 1500.0f;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	float TraceLoopTime = 0.2f;
};
