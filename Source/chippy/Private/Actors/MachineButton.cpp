// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MachineButton.h"
#include "Actors/ColoringMachine.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AMachineButton::AMachineButton()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Button Mesh"));
	ButtonMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMachineButton::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent = GetComponentByClass<UInterpToMovementComponent>();

	CreateDynamicMaterial();
}


void AMachineButton::Interact(AchippyCharacter* InteractingCharacter)
{
	IInteractable::Interact(InteractingCharacter);

	// set the owner of the button to the actor that is interacting with it
	SetOwner(InteractingCharacter);
	if (MovementComponent)
	{
		MovementComponent->Activate();
		MovementComponent->RestartMovement();
		GetWorldTimerManager().SetTimer(PressHandle, this, &ThisClass::ResetPressTimer, MovementComponent->Duration,
		                                false);
	}

	AssignedProduct.Color = MaterialColor;
	AssignedProduct.Mesh = AssignedProductMesh->GetStaticMesh();
	if (!InteractingCharacter || !AssignedProduct.Mesh) return;
	MachineButtonActivatedDelegate.ExecuteIfBound(AssignedProduct);
}

void AMachineButton::ResetPressTimer()
{
	PressHandle.Invalidate();
	MovementComponent->StopMovementImmediately();
}

void AMachineButton::Init(FProductInfo inAssignedProduct, FColor inAssignedColor)
{
	Super::Init(inAssignedProduct, inAssignedColor);

	MaterialColor = inAssignedColor;

	if (ButtonDynamicMaterial)
		ButtonDynamicMaterial->SetVectorParameterValue("BaseColor", MaterialColor);

	if (AssignedMeshMaterial)
		AssignedMeshMaterial->SetVectorParameterValue("InnerColor", MaterialColor);
}

void AMachineButton::Init(FColor inAssignedColor)
{
	MaterialColor = inAssignedColor;

	if (ButtonDynamicMaterial)
		ButtonDynamicMaterial->SetVectorParameterValue("BaseColor", MaterialColor);

	if (AssignedMeshMaterial)
		AssignedMeshMaterial->SetVectorParameterValue("InnerColor", MaterialColor);
}

void AMachineButton::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMachineButton, MaterialColor)
}

void AMachineButton::OnRep_AssignedMeshColor()
{
	if (AssignedMeshMaterial)
		AssignedMeshMaterial->SetVectorParameterValue("InnerColor", MaterialColor);
}

void AMachineButton::OnRep_ButtonColor()
{
	if (ButtonDynamicMaterial)
		ButtonDynamicMaterial->SetVectorParameterValue("BaseColor", MaterialColor);
}

void AMachineButton::CreateDynamicMaterial()
{
	if (!ButtonDynamicMaterial && ButtonMesh)
	{
		if (UMaterialInterface* BaseMat = ButtonMesh->GetMaterial(0))
		{
			ButtonDynamicMaterial = ButtonMesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, BaseMat);

			if (ButtonDynamicMaterial)
			{
				ButtonDynamicMaterial->SetVectorParameterValue("BaseColor", MaterialColor);
			}
		}
	}

	if (!AssignedMeshMaterial && AssignedProductMesh)
	{
		if (UMaterialInterface* BaseMat = AssignedProductMesh->GetMaterial(0))
		{
			AssignedMeshMaterial = AssignedProductMesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, BaseMat);

			if (AssignedMeshMaterial)
			{
				AssignedMeshMaterial->SetVectorParameterValue("InnerColor", MaterialColor);
			}
		}
	}
}

void AMachineButton::AssignProductMeshToPreview(TSoftObjectPtr<UStaticMesh> inMesh)
{
	if (inMesh)
	{
		AssignedProductMesh->SetStaticMesh(inMesh.LoadSynchronous());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Assigned product mesh is not set!"));
	}
}

void AMachineButton::RemoveProductPreviewMesh() const
{
	if (AssignedProductMesh) AssignedProductMesh->SetStaticMesh(nullptr);
}
