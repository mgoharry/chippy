// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/InterpToMovementComponent.h>
#include "Item.h"
#include "Components/ArrowComponent.h"
#include "MachineButton.generated.h"

class AProduct;
DECLARE_DELEGATE_OneParam(FOnMachineButtonActivated, FProductInfo);

UENUM(BlueprintType)
enum class EButtonType : uint8
{
	EBT_None UMETA(DisplayName = "None"),
	EBT_Create UMETA(DisplayName = "Create"),
	EBT_Chip UMETA(DisplayName = "Chip"),
	EBT_Color UMETA(DisplayName = "Color")
};

UCLASS()
class CHIPPY_API AMachineButton : public AItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMachineButton();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	UStaticMeshComponent* ButtonMesh;

	UPROPERTY()
	class AColoringMachine* MachineRef;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void ResetPressTimer();

	FTimerHandle PressHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	UInterpToMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	UArrowComponent* ProductSpawnLocation;

public:
	virtual void Interact(AchippyCharacter* InteractingCharacter) override;

	virtual void Init(FProductInfo inAssignedProduct) override;

	virtual void Init(FColorInfo inAssignedColor);

	FOnMachineButtonActivated MachineButtonActivatedDelegate;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY()
	UMaterialInstanceDynamic* AssignedMeshMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* ButtonDynamicMaterial;

	void CreateDynamicMaterial();

	UFUNCTION()
	void AssignProductMeshToPreview(FProductInfo inProductInfo);

	UFUNCTION()
	void RemoveProductPreviewMesh();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	TSubclassOf<AProduct> ProductClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	EButtonType ButtonType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	FColor InitialColor;

	UPROPERTY()
	UStaticMesh* InitialMesh;

	UFUNCTION()
	FORCEINLINE EButtonType GetButtonType() const { return ButtonType; }

	UFUNCTION()
	FORCEINLINE void SetButtonType(EButtonType inButtonType) { ButtonType = inButtonType; }
};
