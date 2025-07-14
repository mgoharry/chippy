#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/InterpToMovementComponent.h>
#include "Item.h"
#include "MachineButton.generated.h"

class AProduct;
/** Delegate triggered when machine button is activated, passing product information */
DECLARE_DELEGATE_OneParam(FOnMachineButtonActivated, FProductInfo);

/* Defines different types of machine buttons and their functionality */
UENUM(BlueprintType)
enum class EButtonType : uint8
{
	EBT_None UMETA(DisplayName = "None"), // Default button state
	EBT_Create UMETA(DisplayName = "Create"), // Button creates new products
	EBT_Chip UMETA(DisplayName = "Chip"), // Button handles chipping products
	EBT_Color UMETA(DisplayName = "Color") // Button handles color changes
};

//Interactive button actor used in machines for various operations like creating products,
//managing chips, and changing colors*
UCLASS()
class CHIPPY_API AMachineButton : public AItem
{
	GENERATED_BODY()

public:
	// Constructor for the machine button, sets up default components and properties
	AMachineButton();

	// Static mesh component representing the physical button in the world
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	UStaticMeshComponent* ButtonMesh;

	// Handles movement for button press animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	UInterpToMovementComponent* MovementComponent;

	/** Indicates where new products will spawn relative to the button
	in case of being a create-button */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	UArrowComponent* ProductSpawnLocation;

	virtual void BeginPlay() override;

	// Handles interaction with the button when a character activates it
	virtual void Interact(AchippyCharacter* InteractingCharacter) override;

	//Initializes the button with product-specific information and setup
	virtual void Init(FProductInfo inAssignedProduct) override;

	// Initializes the button with color-specific information and setup
	void Init(FColorInfo inAssignedColor);
	// Display overlay material if the item is interactable
	virtual void ControlOverlayMaterial(bool inState) override;

	//Delegate to fire when the button is pressed to easily notify the owning machine
	FOnMachineButtonActivated MachineButtonActivatedDelegate;

protected:
	// Creates and initializes dynamic materials for the button and preview mesh
	void CreateDynamicMaterial();

	// Update preview mesh with product information
	UFUNCTION()
	void AssignProductMeshToPreview(FProductInfo inProductInfo);

	//Removes the current preview mesh
	UFUNCTION()
	void RemoveProductPreviewMesh();

	// Resets the button to its original position after being pressed
	UFUNCTION()
	void ResetPressTimer();

	//button press animation timer handler
	FTimerHandle PressHandle;

	UPROPERTY()
	UMaterialInstanceDynamic* AssignedMeshMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* ButtonDynamicMaterial;

	//initial preview mesh
	UPROPERTY()
	USkeletalMesh* InitialMesh;
	//initial color for button and preview mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config", meta = (AllowPrivateAccess = true))
	FColor InitialColor;

	//default class of products
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config", meta = (AllowPrivateAccess = true))
	TSubclassOf<AProduct> ProductClass;

	//button type to specify its functionality
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config", meta=(AllowPrivateAccess = true))
	EButtonType ButtonType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX", meta=(AllowPrivateAccess = true))
	USoundBase* ClickSoundEffect;

public:
	// Returns the current type of the button
	UFUNCTION()
	FORCEINLINE EButtonType GetButtonType() const { return ButtonType; }

	// Sets the current button type
	UFUNCTION()
	FORCEINLINE void SetButtonType(EButtonType inButtonType) { ButtonType = inButtonType; }
};
