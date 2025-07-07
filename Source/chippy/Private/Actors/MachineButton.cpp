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

	ProductSpawnLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("Product Spawn Location"));
	ProductSpawnLocation->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMachineButton::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent = GetComponentByClass<UInterpToMovementComponent>();

	InitialMesh = AssignedProductMesh->GetStaticMesh();

	if (GetButtonType() == EButtonType::EBT_Create) MaterialColor = InitialColor;

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

	switch (GetButtonType())
	{
	case EButtonType::EBT_Create:
		GetWorld()->SpawnActor<AProduct>(ProductClass, ProductSpawnLocation->GetComponentTransform());
		break;
	case EButtonType::EBT_Chip:
		AssignedProduct.Mesh = AssignedProductMesh->GetStaticMesh();
		break;

	case EButtonType::EBT_Color:
		AssignedProduct.Color.RGBA = MaterialColor;
		break;

	default:
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Emerald, FString::Printf(TEXT("Default!")));
	}

	//AssignedProduct.Mesh = AssignedProductMesh->GetStaticMesh();
	//AssignedProduct.Color.RGBA = MaterialColor;
	if (!InteractingCharacter) return;

	MachineButtonActivatedDelegate.ExecuteIfBound(AssignedProduct);
}

void AMachineButton::ResetPressTimer()
{
	PressHandle.Invalidate();
	MovementComponent->StopMovementImmediately();
}

void AMachineButton::Init(FProductInfo inAssignedProduct)
{
	Super::Init(inAssignedProduct);

	MaterialColor = InitialColor;

	if (ButtonDynamicMaterial)
		ButtonDynamicMaterial->SetVectorParameterValue("BaseColor", MaterialColor);

	if (AssignedMeshMaterial)
		AssignedMeshMaterial->SetVectorParameterValue("BaseColor", MaterialColor);
}

void AMachineButton::Init(FColorInfo inAssignedColor)
{
	MaterialColor = inAssignedColor.RGBA;

	if (AssignedMeshMaterial)
		AssignedMeshMaterial->SetVectorParameterValue("BaseColor", MaterialColor);

	if (ButtonDynamicMaterial)
		ButtonDynamicMaterial->SetVectorParameterValue("BaseColor", MaterialColor);
}

void AMachineButton::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMachineButton, MaterialColor)
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
				AssignedMeshMaterial->SetVectorParameterValue("BaseColor", MaterialColor);
			}
		}
	}
}

void AMachineButton::AssignProductMeshToPreview(FProductInfo inProductInfo)
{
	if (!inProductInfo.Mesh) return;

	switch (GetButtonType())
	{
	case EButtonType::EBT_Chip:
		MaterialColor = inProductInfo.Color.RGBA;
		if (AssignedMeshMaterial)
		{
			AssignedMeshMaterial->SetVectorParameterValue("BaseColor", MaterialColor);
		}
		break;

	case EButtonType::EBT_Color:
		AssignedProduct = inProductInfo;
		AssignedProductMesh->SetStaticMesh(inProductInfo.Mesh.LoadSynchronous());
		break;

	default:
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Emerald, FString::Printf(TEXT("Default!")));
	}
}

void AMachineButton::RemoveProductPreviewMesh()
{
	switch (GetButtonType())
	{
	case EButtonType::EBT_Chip:
		MaterialColor = InitialColor;
		if (AssignedMeshMaterial)
		{
			AssignedMeshMaterial->SetVectorParameterValue("BaseColor", MaterialColor);
		}
		break;

	case EButtonType::EBT_Color:
		AssignedProduct = FProductInfo();
		if (InitialMesh) AssignedProductMesh->SetStaticMesh(InitialMesh);
		break;

	default:
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Emerald, FString::Printf(TEXT("Default!")));
	}
}
