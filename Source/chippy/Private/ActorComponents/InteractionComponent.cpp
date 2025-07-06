// Fill out your copyright notice in the Description page of Project Settings.


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

	OwnerCharacter = Cast<AchippyCharacter>(GetOwner());
	if (OwnerCharacter)
	{
		CameraComponent = Cast<UCameraComponent>(OwnerCharacter->GetComponentByClass(UCameraComponent::StaticClass()));
	}
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//PerformLineTrace();
}

void UInteractionComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInteractionComponent, isCarryingProduct)
	DOREPLIFETIME(UInteractionComponent, CarriedProduct);
}

void UInteractionComponent::PerformLineTrace()
{
	if (!OwnerCharacter || !CameraComponent) return;

	FVector Start = CameraComponent->GetComponentLocation();
	FVector End = Start + (CameraComponent->GetForwardVector() * TraceDistance);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerCharacter);

	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.f, 0, 1.f);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_WorldStatic, QueryParams))
	{
		if (AActor* HitActor = HitResult.GetActor())
		{
			if (HitActor->Implements<UInteractable>())
			{
				InteractableInterface = HitActor;
				OwnerCharacter->HasAuthority() ? Interact() : SER_Interact(HitActor);
			}
		}
	}
	else
	{
		InteractableInterface = nullptr;
	}
}

void UInteractionComponent::Interact()
{
	if (InteractableInterface)
	{
		InteractableInterface->Interact(OwnerCharacter);
	}
}

void UInteractionComponent::CarryProduct(AProduct* Product)
{
	if (Product && !isCarryingProduct)
	{
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
	if (OwnerCharacter->HasAuthority())
	{
		SER_DropProduct();
	}
	else
	{
		SER_DropProduct();
	}
}

void UInteractionComponent::SER_DropProduct_Implementation()
{
	if (isCarryingProduct && CarriedProduct)
	{
		CarriedProduct->AttachToComponent(OwnerCharacter->GetMesh(),
		                                  FAttachmentTransformRules::SnapToTargetIncludingScale,
		                                  FName("DropProductSocket"));
		CarriedProduct->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CarriedProduct->MC_CarryProductEffects(true);
		CarriedProduct->GetSphereComponent()->AddImpulse(OwnerCharacter->GetActorForwardVector() * 5000);
		isCarryingProduct = false;
		CarriedProduct = nullptr;
	}
}

void UInteractionComponent::SER_Interact_Implementation(AActor* inInteractableActor)
{
	if (inInteractableActor)
	{
		Cast<IInteractable>(inInteractableActor)->Interact(OwnerCharacter);
	}
}

void UInteractionComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                           const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->Implements<UInteractable>())
	{
		OverlappingActor = OtherActor;
		InteractableInterface = OtherActor;
	}
}

void UInteractionComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == OverlappingActor)
	{
		OverlappingActor = nullptr;
		InteractableInterface = nullptr;
	}
}
