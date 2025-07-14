// InteractionComponent.cpp
// Component handling player interaction with objects and product carrying mechanics
// Performs line tracing to detect interactable objects and manages their interaction states


#include "ActorComponents/InteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	//Assign references from the owning character
	OwnerCharacter = Cast<AchippyCharacter>(GetOwner());

	if (OwnerCharacter && OwnerCharacter->IsLocallyControlled())
	{
		CameraComponent = Cast<UCameraComponent>(OwnerCharacter->GetComponentByClass(UCameraComponent::StaticClass()));

		//Start timer for a looping line trace
		FTimerHandle TraceLoopTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TraceLoopTimerHandle, this, &UInteractionComponent::PerformLineTrace,
		                                       TraceLoopTime, true);
	}
}

void UInteractionComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInteractionComponent, isCarryingProduct)
	DOREPLIFETIME(UInteractionComponent, CarriedProduct);
}

// Performs line trace from the camera to detect interactable objects and manages their state
void UInteractionComponent::PerformLineTrace()
{
	// Early return if required components are not available
	if (!OwnerCharacter || !CameraComponent) return;

	// Calculate trace start and end points from the camera componenet
	FVector Start = CameraComponent->GetComponentLocation();
	FVector End = Start + (CameraComponent->GetForwardVector() * TraceDistance);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerCharacter);

	// Perform actual line trace
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_WorldStatic, QueryParams))
	{
		if (AActor* HitActor = HitResult.GetActor())
		{
			//If the hit actor is interactable, store reference and enable highlight
			if (HitActor->Implements<UInteractable>())
			{
				ClearInteractableRef();
				InteractableInterface = HitActor;
				OverlappingActor = HitActor;
				InteractableInterface->ControlOverlayMaterial(true);
			}
			// If not interactable, clear previous references and disable highlight
			else
			{
				ClearInteractableRef();
			}
		}
	}
}

void UInteractionComponent::Interact()
{
	SER_Interact(OverlappingActor);
}

// Attaches product to character's product socket and updates carrying state
void UInteractionComponent::CarryProduct(AProduct* Product)
{
	// Only allow picking up if not already carrying something
	if (Product && !isCarryingProduct)
	{
		// Disable product effects and attach to character mesh
		Product->MC_CarryProductEffects(false);
		Product->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
		                           FName("ProductSocket"));

		CarriedProduct = Product;
		isCarryingProduct = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Product is not set!"));
	}
}

void UInteractionComponent::DropProduct()
{
	SER_DropProduct();
}

//clear reference of previously interactable object
void UInteractionComponent::ClearInteractableRef()
{
	if (InteractableInterface)
	{
		InteractableInterface->ControlOverlayMaterial(false);
		InteractableInterface = nullptr;
		OverlappingActor = nullptr;
	}
}

// Server implementation for dropping carried product with physics impulse
void UInteractionComponent::SER_DropProduct_Implementation()
{
	if (isCarryingProduct && CarriedProduct)
	{
		// Move to drop socket before detaching
		CarriedProduct->AttachToComponent(OwnerCharacter->GetMesh(),
		                                  FAttachmentTransformRules::SnapToTargetIncludingScale,
		                                  FName("DropProductSocket"));
		// Detach and enable physics
		CarriedProduct->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CarriedProduct->MC_CarryProductEffects(true);
		// Add forward throwing impulse
		CarriedProduct->GetSphereComponent()->AddImpulse(OwnerCharacter->GetActorForwardVector() * 5000);

		// Reset carrying state
		CarriedProduct->isDropped = true;
		isCarryingProduct = false;
		CarriedProduct = nullptr;
	}
}

// Server-side interaction handler
void UInteractionComponent::SER_Interact_Implementation(AActor* inInteractableActor)
{
	//calls the interaction handler on the object if it's available
	if (inInteractableActor)
	{
		Cast<IInteractable>(inInteractableActor)->Interact(OwnerCharacter);
	}
}
