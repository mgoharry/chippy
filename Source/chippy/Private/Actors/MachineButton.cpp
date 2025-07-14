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

	//initialize interp to movement component to quickly handle press animations
	MovementComponent = GetComponentByClass<UInterpToMovementComponent>();

	//assign preview mesh as the default initial mesh set from blueprints
	InitialMesh = AssignedProductMesh->GetSkeletalMeshAsset();

	//set default color in case it's just a create button
	if (GetButtonType() == EButtonType::EBT_Create) MaterialColor = InitialColor;

	// creates and initializes dynamic materials for both button and assigned product meshes
	CreateDynamicMaterial();
}


// Handles interaction with the button, processing different button types (Create/Chip/Color)
// @param InteractingCharacter - The character that triggered the interaction
void AMachineButton::Interact(AchippyCharacter* InteractingCharacter)
{
	AItem::Interact(InteractingCharacter);
	// set the owner of the button to the actor that is interacting with it
	SetOwner(InteractingCharacter);
	if (MovementComponent)
	{
		MovementComponent->Activate();
		MovementComponent->RestartMovement();
		GetWorldTimerManager().SetTimer(PressHandle, this, &ThisClass::ResetPressTimer, MovementComponent->Duration,
		                                false);
	}

	MC_PlaySoundEffect(ClickSoundEffect);

	//Handles interaction based on the button's type
	switch (GetButtonType())
	{
	case EButtonType::EBT_Create:
		GetWorld()->SpawnActor<AProduct>(ProductClass, ProductSpawnLocation->GetComponentTransform());
		break;
	case EButtonType::EBT_Chip:
		AssignedProduct.Mesh = AssignedProductMesh->GetSkeletalMeshAsset();
		break;

	case EButtonType::EBT_Color:
		AssignedProduct.Color.RGBA = MaterialColor;
		break;

	default:
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Emerald, FString::Printf(TEXT("Default!")));
	}

	//fire a delegate, so the machine can easily know this button was pressed
	//also sends product info with it to be correctly modified
	MachineButtonActivatedDelegate.ExecuteIfBound(AssignedProduct);
}

// Handles the reset of button press animation
void AMachineButton::ResetPressTimer()
{
	PressHandle.Invalidate();
	MovementComponent->StopMovementImmediately();
}

// Initializes the button with product information, setting up the mesh
void AMachineButton::Init(FProductInfo inAssignedProduct)
{
	Super::Init(inAssignedProduct);

	AssignedProductMesh->SetSkeletalMeshAsset(AssignedProduct.Mesh.LoadSynchronous());

	ReplicatedSkeletalMesh = inAssignedProduct.Mesh.LoadSynchronous();

	MaterialColor = InitialColor;

	//Sets button's color 
	if (ButtonDynamicMaterial)
		ButtonDynamicMaterial->SetVectorParameterValue("BaseColor", MaterialColor);

	//Sets preview mesh's color
	if (AssignedMeshMaterial)
		AssignedMeshMaterial->SetVectorParameterValue("BaseColor", MaterialColor);
}

// Initializes the button with color information
void AMachineButton::Init(FColorInfo inAssignedColor)
{
	MaterialColor = inAssignedColor.RGBA;

	//Sets preview mesh's color
	if (AssignedMeshMaterial)
		AssignedMeshMaterial->SetVectorParameterValue("BaseColor", MaterialColor);

	//Sets button's color 
	if (ButtonDynamicMaterial)
		ButtonDynamicMaterial->SetVectorParameterValue("BaseColor", MaterialColor);
}

// Display overlay material if the item is interactable
void AMachineButton::ControlOverlayMaterial(bool inState)
{
	Super::ControlOverlayMaterial(inState);
	inState
		? ButtonMesh->SetOverlayMaterial(OverlayMaterialRef)
		: ButtonMesh->SetOverlayMaterial(nullptr);
}


// Creates and initializes dynamic materials for both button and assigned product meshes
// Sets up material instances for color modification
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

// Updates the preview mesh based on the button type and product information
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
		AssignedProductMesh->SetSkeletalMesh(inProductInfo.Mesh.LoadSynchronous());
		ProductAnimationAsset = AssignedProduct.Animation;
		ReplicatedSkeletalMesh = inProductInfo.Mesh.LoadSynchronous();
		AssignedProductMesh->PlayAnimation(AssignedProduct.Animation, true);
		break;

	default:
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Emerald, FString::Printf(TEXT("Default!")));
	}
}

// Resets the preview mesh to initial state based on button type
// Restores default colors and meshes
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
		if (InitialMesh)
		{
			AssignedProductMesh->SetSkeletalMesh(InitialMesh);
			ProductAnimationAsset = nullptr;
			ReplicatedSkeletalMesh = InitialMesh;
		}
		break;

	default:
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Emerald, FString::Printf(TEXT("Default!")));
	}
}
